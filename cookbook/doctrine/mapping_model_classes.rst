.. index::
   single: Doctrine; Classi di mappatura dei modelli

Fornire classi di modello per varie implementazioni Doctrine
============================================================

Quando si costruisce un bundle che può venire usato non solo con Doctrine, ma
anche con CouchDB ODM, MongoDB ODM, PHPCR ODM, bisognerebbe scrivere comunque
una sola classe per il modello. I bundle  di Doctrine forniscono un passo di compilatore per
registrare la mappature di una classe del modello.

.. note::

    Per bundle non riutilizzabili, l'opzione più semplice è quella di mettere le classi del modello
    nella posizione predefinita: ``Entity`` per l'ORM di Doctrine o ``Document``
    per uno degli altri ODM. Per bundle riutilizzabili, invece di duplicare le classi del modello
    al solo fine di ottenere la mappatura automatica, si usi il passo di compilatore

.. versionadded:: 2.3
    Il passo di compilatore della mappatura  di base è stato aggiunto in Symfony 2.3. I bundle di Doctrine
    lo supportano da DoctrineBundle >= 1.2.1, MongoDBBundle >= 3.0.0,
    PHPCRBundle >= 1.0.0-alpha2 e CouchDBBundle (senza versione) supporta il passo di
    compilazione a partire da quando è stato fatto il merge della
    `pull request CouchDB Mapping Compiler Pass`_.

    Se si vuole che un bundle supporti versioni più vecchie di Symfony e
    Doctrine, si può fornire una copia del passo di compilatore all'interno dell bundle.
    Si veda per esempio nella `configurazione della mappatura di FOSUserBundle`_
    ``addRegisterMappingsPass``.


Nella classe del bundle, scrivere il seguente codice per registrare il passo di compilatore.
Questo esempio è stato scritto per FOSUserBundle, quindi una parte di esso dovrà essere
adattata al proprio caso::

    use Doctrine\Bundle\DoctrineBundle\DependencyInjection\Compiler\DoctrineOrmMappingsPass;
    use Doctrine\Bundle\MongoDBBundle\DependencyInjection\Compiler\DoctrineMongoDBMappingsPass;
    use Doctrine\Bundle\CouchDBBundle\DependencyInjection\Compiler\DoctrineCouchDBMappingsPass;
    use Doctrine\Bundle\PHPCRBundle\DependencyInjection\Compiler\DoctrinePhpcrMappingsPass;

    class FOSUserBundle extends Bundle
    {
        public function build(ContainerBuilder $container)
        {
            parent::build($container);
            // ...

            $modelDir = realpath(__DIR__.'/Resources/config/doctrine/model');
            $mappings = array(
                $modelDir => 'FOS\UserBundle\Model',
            );

            $ormCompilerClass = 'Doctrine\Bundle\DoctrineBundle\DependencyInjection\Compiler\DoctrineOrmMappingsPass';
            if (class_exists($ormCompilerClass)) {
                $container->addCompilerPass(
                    DoctrineOrmMappingsPass::createXmlMappingDriver(
                        $mappings,
                        array('fos_user.model_manager_name'),
                        'fos_user.backend_type_orm'
                ));
            }

            $mongoCompilerClass = 'Doctrine\Bundle\MongoDBBundle\DependencyInjection\Compiler\DoctrineMongoDBMappingsPass';
            if (class_exists($mongoCompilerClass)) {
                $container->addCompilerPass(
                    DoctrineMongoDBMappingsPass::createXmlMappingDriver(
                        $mappings,
                        array('fos_user.model_manager_name'),
                        'fos_user.backend_type_mongodb'
                ));
            }

            $couchCompilerClass = 'Doctrine\Bundle\CouchDBBundle\DependencyInjection\Compiler\DoctrineCouchDBMappingsPass';
            if (class_exists($couchCompilerClass)) {
                $container->addCompilerPass(
                    DoctrineCouchDBMappingsPass::createXmlMappingDriver(
                        $mappings,
                        array('fos_user.model_manager_name'),
                        'fos_user.backend_type_couchdb'
                ));
            }

            $phpcrCompilerClass = 'Doctrine\Bundle\PHPCRBundle\DependencyInjection\Compiler\DoctrinePhpcrMappingsPass';
            if (class_exists($phpcrCompilerClass)) {
                $container->addCompilerPass(
                    DoctrinePhpcrMappingsPass::createXmlMappingDriver(
                        $mappings,
                        array('fos_user.model_manager_name'),
                        'fos_user.backend_type_phpcr'
                ));
            }
        }
    }

Si noti il controllo :phpfunction:`class_exists`. Questo è cruciale, in quanto non si vuole che un
bundle abbia una dipendenza diretta verso tutti i bundle di Doctrine, ma si vuole lasciare che sia l'utente
a decidere quale usare.

Il passo di compilatore fornisce metodi factory per tutti i driver forniti da Doctrine:
Annotazioni, XML, Yaml, PHP e StaticPHP. I parametri sono:

* una mappa/hash dei percorsi assoluti dello spazio dei nomi;
* un array di parametri del contenitore che il bundle usa per specificare il nome del
  gestore Doctrine che si usa. Nell'esempio precedente, FOSUserBundle
  salva il nome del gestore che viene usato nel parametro ``fos_user.model_manager_name``.
  Il passo di compilatore accoda il parametro che Doctrine sta usando
  per specificare il nome del gestore predefinito. Il primo parametro che viene trovato viene usato
  e le mappature vengono  registrate con quel gestore;
* un nome di parametro del contenitore opzionale, che viene usato dal passo di compilatore
  predeterminare se questo tipo di Doctrine viene usato in assoluto (questo è rilevante se
  l'utente ha più di un tipo di bundle di Doctrine installato e il proprio
  bundle viene utilizzato con solo un tipo di Doctrine.

.. note::

    Il metodo factory fa uso del ``SymfonyFileLocator`` di Doctrine, quindi
    troverà file XML e YML solo se non contengono lo spazio
    dei nomi completo come nome del file. Questa è una scelta progettuale: il ``SymfonyFileLocator``
    semplifica le cose assumendo che i file usino semplicemente una versione "corta"
    della classe come nome del file (p.e. ``BlogPost.orm.xml``)

    Se occorre anche mappare una classe di base, si può registrare il passo di compilatore
    con il ``DefaultFileLocator``, come si vede qui. Questo codice è stato semplicemente preso dal
    ``DoctrineOrmMappingsPass`` e adattato per usare il ``DefaultFileLocator``
    al posto del ``SymfonyFileLocator``::

        private function buildMappingCompilerPass()
        {
            $arguments = array(array(realpath(__DIR__ . '/Resources/config/doctrine-base')), '.orm.xml');
            $locator = new Definition('Doctrine\Common\Persistence\Mapping\Driver\DefaultFileLocator', $arguments);
            $driver = new Definition('Doctrine\ORM\Mapping\Driver\XmlDriver', array($locator));

            return new DoctrineOrmMappingsPass(
                $driver,
                array('Full\Namespace'),
                array('your_bundle.manager_name'),
                'your_bundle.orm_enabled'
            );
        }

    Ora si metta il file di mappatura dentro a ``/Resources/config/doctrine-base`` con il nome
    completo della classe, separato da ``.`` al posto di ``\``, per esempio
    ``Altro.SpazioDeiNomi.Nome.Modello.orm.xml``. Non si possono mischiare i due, perché altrimenti
    il SymfonyFileLocator si confonde.

    Adattare in ragione delle altre implementazioni di Doctrine.

.. _`pull request CouchDB Mapping Compiler Pass`: https://github.com/doctrine/DoctrineCouchDBBundle/pull/27
.. _`configurazione della mappatura di FOSUserBundle`: https://github.com/FriendsOfSymfony/FOSUserBundle/blob/master/FOSUserBundle.php
