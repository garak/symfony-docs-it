.. index::
   single: Configurazione; Semantica
   single: Bundle; Configurazione dell'estensione

Caricare la configurazione di un servizio in un bundle
======================================================

In Symfony, ci si troverà a usare molti servizi. Questi servizi possono
essere registrati nella cartella ``app/config`` di un'applicazione. Ma, quando si
vuole disaccoppiare un bundle per usarlo in altri progetto, si vuole includere la
configurazione di un servizio nel bundle stesso. Questa ricetta spiega come
poterlo fare.

Creare una classe Extension
---------------------------

Per poter caricare la configurazione di un servizio, si deve creare una classe Extension
per il bundle. Questa classe segue alcune convenzioni, per poter essere individuata
automaticamente. Si vedrà più avanti come poterla cambiare a seconda delle esigenze.
Per impostazione predefinita, la classe Extension deve seguire queste
convenzioni:

* Deve trovarsi nello spazio dei nomi ``DependencyInjection`` del bundle;

* Il nome deve essere uguale a quello del bundle, con il suffisso ``Bundle`` sostituito da
  ``Extension`` (p.e. la classe Extension di ``AcmeHelloBundle`` si deve
  chiamare ``AcmeHelloExtension``).

La classe Extension deve implementare
:class:`Symfony\\Component\\DependencyInjection\\Extension\\ExtensionInterface`,
ma solitamente estende semplicemente la classe
:class:`Symfony\\Component\\DependencyInjection\\Extension\\Extension`::

    // src/Acme/HelloBundle/DependencyInjection/AcmeHelloExtension.php
    namespace Acme\HelloBundle\DependencyInjection;

    use Symfony\Component\HttpKernel\DependencyInjection\Extension;
    use Symfony\Component\DependencyInjection\ContainerBuilder;

    class AcmeHelloExtension extends Extension
    {
        public function load(array $configs, ContainerBuilder $container)
        {
            // qui verranno caricati i file
        }
    }

Registrare a mano una classe Extension
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Se non si seguono tali convenzioni, occorrerà registrare a mano
un'estensione. Per registrare a mano una classe Extension, sovrascrivere il metodo
:method:`Bundle::getContainerExtension() <Symfony\\Component\\HttpKernel\\Bundle\\Bundle::getContainerExtension>`
per restituire l'istanza desiderata::

    // ...
    use Acme\HelloBundle\DependencyInjection\UnconventionalExtensionClass;

    class AcmeHelloBundle extends Bundle
    {
        public function getContainerExtension()
        {
            return new UnconventionalExtensionClass();
        }
    }

Poiché la classe Extension non segue la convenzione sul nome, si deve
anche sovrascrivere il metodo
:method:`Extension::getAlias() <Symfony\\Component\\DependencyInjection\\Extension\\Extension::getAlias>`
e restituire l'alias corretto. L'alias è il nome usato per fare riferimento al
bundle nel contenitore (p.e. nel file ``app/config/config.yml``). Per impostazione
predefinita, lo si fa rimuovendo il prefisso ``Extension`` e convertendo il
nome della classe con i trattini bassi (p.e. l'alias di ``AcmeHelloExtension`` è
``acme_hello``).

Usare il metodo ``load()``
--------------------------

Nel metodo ``load()`` saranno caricati tutti i servizi e i parametri legati all'estensione.
Questo metodo non prende l'istanza effettiva del contenitore, ma una
copia. Questo contenitore ha solo i parametri del contenitore effettivo. Dopo
aver caricato servizi e parametri, la copia sarà fuso nel contenitore effettivo,
per assicurare che tutti i servizi e i parametri siano anche aggiunti al contenitore
effettivo.

Nel metodo ``load()`` si può usare codice PHP per registrare definizioni di servizi,
ma è opiù comune inserire tali definizioni in un file di configurazione
(con formato YAML, XML o PHP). Fortunatamente, si possono usare i caricatori di file
dell'estensione.

Per esempio, si supponga di avere un file ``services.xml`` nella cartella
``Resources/config`` di un bundle, il metodo ``load`` assomiglia a questo::

    use Symfony\Component\DependencyInjection\Loader\XmlFileLoader;
    use Symfony\Component\Config\FileLocator;

    // ...
    public function load(array $configs, ContainerBuilder $container)
    {
        $loader = new XmlFileLoader(
            $container,
            new FileLocator(__DIR__.'/../Resources/config')
        );
        $loader->load('services.xml');
    }

Altri caricatori disponibili sono ``YamlFileLoader``, ``PhpFileLoader`` e
``IniFileLoader``.

.. note::

    ``IniFileLoader`` si può usare solo per caricare parametri, che possono
    essere caricati solo come stringhe.

Usare la configurazione per cambiare i servizi
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

La classe Extension gestisce anche la configurazione per quello
specifico bundle (p.e. la configurazione in ``app/config/config.yml``). Per
approfondire, vedere la ricetta ":doc:`/cookbook/bundles/configuration`".
