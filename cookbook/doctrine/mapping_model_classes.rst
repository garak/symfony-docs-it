.. index::
   single: Doctrine; Classi di mappatura dei modelli

Fornire classi di modello per varie implementazioni Doctrine
============================================================

Quando si costruisce un bundle che può venire usato non solo con Doctrine, ma
anche con CouchDB ODM, MongoDB ODM, PHPCR ODM, bisognerebbe scrivere comunque
una sola classe per il modello. I bundle  di Doctrine forniscono uno step di compilazione per
registrare i mapping delle proprie classi di modello.

.. note::

    Per bundle di cui non riutilizzabili, l'opzione più semplice è quella di mettere le classi di modello
    nella collocazione di default: ``Entity`` per Doctrine ORM o ``Document``
    per uno degli altri ODM. Per bundle riutilizzabili, invece di duplicare le classi di modello
    al solo fine di ottenere la mappatura automatica, si usi lo step di compilazione

.. versionadded:: 2.3
Lo step di compilazione del mapping di base è stato aggiunto in Symfony 2.3. I bundle di Doctrine
    lo supportano da DoctrineBundle >= 1.2.1, MongoDBBundle >= 3.0.0,
    PHPCRBundle >= 1.0.0-alpha2 e il CouchDBBundle (senza versione) supporta lo step di
    compilazione a partire da quando è stato fatto il merge della
    `CouchDB Mapping Compiler Pass pull request`_

    Se si vuole che il proprio bundle supporti versioni più vecchie di Symfony e
    Doctrine, si può fornire una copia dello step di compilazione all'interno dell bundle.
    Si veda per esempio `FOSUserBundle mapping configuration`_
    ``addRegisterMappingsPass``.


Nella classe del proprio bundle si scriva il seguente codice per registrare lo step di compilazione.
Questo esempio è stato scritto per il FOSUserBundle, quindi una parte di esso dovrà essere
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

Si noti il controllo :phpfunction:`class_exists`. Questo è cruciale, in quanto non di vuole  che il proprio
bundle abbia una dipendenza diretta verso tutti i bundle Doctrine, ma si vuole lasciare che sia l'utente
a decidere quale usare.

Lo step di compilazione fornisce metodi factory per tutti i driver forniti da Doctrine:
Annotazioni, XML, Yaml, PHP and StaticPHP. Gli argomenti sono:

* una mappa/hash dei path assoluti al namespace;
* un array di parametri di container che il proprio bundle usa per specificare il nome del
  manager Doctrine che si usa. Nell'esempio precedente, il FOSUserBundle
  salva il nome del manager che viene usato dentro il parametro ``fos_user.model_manager_name``.
  Lo step di compilazione accoda il parametro che Doctrine sta usando
  per specificare il nome del manager di default. Il primo parametro che viene trovato viene usato
  e  i mapping vengono  registrati con quel manager;
* un nome di parametro di container opzionale che viene usato dallo step di compilazione
  perdeterminare se questo tipo di Doctrine viene usato in assoluto (questo è rilevante se
  l'utente ha più di un tipo di bundle di Doctrine installato e il proprio
  bundle viene utilizzato con solo un tipo di Doctrine.

.. note::

    Il metodo factory fa uso del ``SymfonyFileLocator`` di Doctrine, quindi
    troverà file XML e YML solo se non contengono il
    namespace completo come nome del file. Questa è una scelta di design: il ``SymfonyFileLocator``
    semplifica le cose assumendo che i file usino semplicemente una versione "corta"
    della classe come nome del file (ad es. ``BlogPost.orm.xml``)

    Se serve anche di mappare una classe di base, si può registrare lo step di compilazione
    con il ``DefaultFileLocator`` come si vede qui. Questo codice è stato semplicemente preso dal
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

    Ora si piazzi il file di mapping dentro a ``/Resources/config/doctrine-base`` con il
    FQCN, separato da ``.`` al posto di ``\``, ad esempio
    ``Other.Namespace.Model.Name.orm.xml``. Non si possono mischiare i due  in quanto altrimenti
    il SymfonyFileLocator si confonde.

    Adattate in ragione delle altre implementazioni di Doctrine.

.. _`CouchDB Mapping Compiler Pass pull request`: https://github.com/doctrine/DoctrineCouchDBBundle/pull/27
.. _`FOSUserBundle mapping configuration`: https://github.com/FriendsOfSymfony/FOSUserBundle/blob/master/FOSUserBundle.php
