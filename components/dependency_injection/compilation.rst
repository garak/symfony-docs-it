.. index::
   single: DependencyInjection; Compilazione

Compilazione del contenitore
============================

Ci sono diverse ragioni per compilare il contenitore di servizi. Tra queste, poter
verificare potenziali problemi, come i riferimenti circolari, e rendere il contenitore più
efficiente, risolvendo i parametri e rimuovendo i servizi
inutilizzati. Inoltre, alcune caratteristiche, come l'uso
di :doc:`servizi genitori </components/dependency_injection/parentservices>`,
necessitano di un contenitore compilato.

La compilazione avviene eseguendo::

    $container->compile();

Il metodo ``compile`` usa dei **passi di compilatore** per la compilazione. Il componente
DependencyInjection dispone di diversi passi, registrati automaticamente per la
compilazione. Per esempio, :class:`Symfony\\Component\\DependencyInjection\\Compiler\\CheckDefinitionValidityPass`
verifica diversi problemi potenziali con le definizioni impostate nel
contenitore. Dopo questo e molti altri passi, che verificano la validità del
contenitore, ulteriori passi sono usati per ottimizzare la configurazione, prima che sia
messa in cache. Per esempio, vengono rimossi i servizi privati e i servizi astratti, e
vengono risolti gli alias.

.. _components-dependency-injection-extension:

Gestire la configurazione con le estensioni
-------------------------------------------

Oltre a caricare la configurazione direttamente nel contenitore, come mostrato in
:doc:`/components/dependency_injection/introduction`, la si può gestire registrando
estensioni con il contenitore. Il primo passo nel processo di compilazione consiste
nel caricare la configurazione da un classe estensione, registrata con il
contenitore. Diversamente dal caricamento diretto della configurazione, sono processate
solo quando il contenitore viene compilato. Se l'applicazione è modulare, le estensioni
consentono a ciascun modulo di registrare e gestire la propria configurazione dei servizi.

Le estensioni devono implementare :class:`Symfony\\Component\\DependencyInjection\\Extension\\ExtensionInterface`
e possono essere registrare con il contenitore in questo modo::

    $container->registerExtension($extension);

Il lavoro principale dell'estensione viene eseguito nel metodo ``load``. In questo
metodo si possono caricare configurazioni da uno o più file, oltre che
manipolare le definizioni del contenitore, usando i metodi mostrati in :doc:`/components/dependency_injection/definitions`.

Al metodo ``load`` viene passato un nuovo contenitore da configurare, il quale viene poi
fuso nel contenitore con cui è registrato. Questo consente di avere diverse
estensioni, che gestiscono le definizioni in modo indipendente.
Quando vengono aggiunte, le estensioni non aggiungono configurazioni al contenitore, ma
sono processato quando viene richiamato il metodo ``compile`` del contenitore.

Un'estensione molto semplice potrebbe solo caricare file di configurazione nel contenitore::

    use Symfony\Component\DependencyInjection\ContainerBuilder;
    use Symfony\Component\DependencyInjection\Loader\XmlFileLoader;
    use Symfony\Component\DependencyInjection\Extension\ExtensionInterface;
    use Symfony\Component\Config\FileLocator;

    class AcmeDemoExtension implements ExtensionInterface
    {
        public function load(array $configs, ContainerBuilder $container)
        {
            $loader = new XmlFileLoader(
                $container,
                new FileLocator(__DIR__.'/../Resources/config')
            );
            $loader->load('services.xml');
        }

        // ...
    }

Questo non fornisce molti vantaggi, rispetto a caricare il file direttamente nel
contenitore generale in via di costruzione. Consente solo ai file di essere suddivisi tra
i moduli/bundle. La possibilità di influenzare la configurazione di un modulo dai file
di configurazione esterni al modulo/bundle è necessaria per rendere configurabile
un'applicazione complessa. Lo si può fare specificando che delle sezioni dei file di
configurazione caricati direttamente nel contenitore appartengono a una particolare
estensione. Tali sezioni non saranno processate direttamente dal contenitore, ma
dall'estensione relativa.

L'estensione deve specificare un metodo ``getAlias``, per implementare l'interfaccia::

    // ...

    class AcmeDemoExtension implements ExtensionInterface
    {
        // ...

        public function getAlias()
        {
            return 'acme_demo';
        }
    }

Per i file di configurazione YAML, specificare l'alias per l'estensione come chiave
vorrà dire che tali valori sono passati al metodo ``load`` dell'estensione:

.. code-block:: yaml

    # ...
    acme_demo:
        pippo: valoreDiPippo
        pluto: valoreDiPluto

Se questo file viene caricato nella configurazione, i valori in esso sono processati
solo quando il contenitore viene compilato nel punto in cui viene caricata l'estensione::

    use Symfony\Component\DependencyInjection\ContainerBuilder;
    use Symfony\Component\Config\FileLocator;
    use Symfony\Component\DependencyInjection\Loader\YamlFileLoader;

    $container = new ContainerBuilder();
    $container->registerExtension(new AcmeDemoExtension);

    $loader = new YamlFileLoader($container, new FileLocator(__DIR__));
    $loader->load('config.yml');

    // ...
    $container->compile();

.. note::

    Quando si carica un file di configurazione che usa un alias di estensione come chiave,
    l'estensione deve essere già stata registrata nel costruttore di contenitore
    o verrà sollevata un'eccezione.

I valori di tali sezioni dei file di configurazione sono passati al primo parametro
del metodo ``load`` dell'estensione::

    public function load(array $configs, ContainerBuilder $container)
    {
        $foo = $configs[0]['pippo']; //valoreDiPippo
        $bar = $configs[0]['pluto']; //valoreDiPluto
    }

Il parametro ``$configs`` è un array contenente ogni diverso file di configurazione
caricato nel contenitore. Nell'esempio precedente viene caricato solo un unico file di
configurazione, ma sarà comunque dentro un array. L'array sarà simile a
questo::

    array(
        array(
            'pippo' => 'valoreDiPippo',
            'pluto' => 'valoreDiPluto',
        ),
    )

Sebbene sia possibile gestire manualmente la fusione dei vari file, è molto meglio
usare il :doc:`componente Config</components/config/introduction>` per fondere e
validare i valori di configurazione. Usando il processo di configurazione si può
accedere ai valori di configurazione in questo modo::

    use Symfony\Component\Config\Definition\Processor;
    // ...

    public function load(array $configs, ContainerBuilder $container)
    {
        $configuration = new Configuration();
        $processor = new Processor();
        $config = $processor->processConfiguration($configuration, $configs);

        $foo = $config['pippo']; //valoreDiPippo
        $bar = $config['pluto']; //valoreDiPluto

        // ...
    }

Ci sono altri due metodi da implementare. Uno per restituire lo spazio dei nomi XML,
in modo che le parti rilevanti di un file di configurazione XML siano passate
all'estensione. L'altro per specificare il percorso di base ai file XSD per validare
la configurazione XML::

    public function getXsdValidationBasePath()
    {
        return __DIR__.'/../Resources/config/';
    }

    public function getNamespace()
    {
        return 'http://www.example.com/symfony/schema/';
    }

.. note::

    La validazione XSD è facoltativa, restituendo ``false`` dal metodo ``getXsdValidationBasePath``
    sarà disabilitata.

La versione XML della configurazione sarà dunque simile a questa:

.. code-block:: xml

    <?xml version="1.0" ?>
    <container xmlns="http://symfony.com/schema/dic/services"
        xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
        xmlns:acme_demo="http://www.example.com/symfony/schema/"
        xsi:schemaLocation="http://www.example.com/symfony/schema/ http://www.example.com/symfony/schema/hello-1.0.xsd">

        <acme_demo:config>
            <acme_demo:pippo>valoreDiPippo</acme_hello:foo>
            <acme_demo:pluto>valoreDiPluto</acme_demo:bar>
        </acme_demo:config>
    </container>

.. note::

    Nel framework Symfony c'è una classe base ``Extension``, che
    implementa questi metodi e un metodo scorciatoia per processare la
    configurazione. Vedere :doc:`/cookbook/bundles/extension` per maggiori dettagli.

Il valore di configurazione processato ora può essere aggiunto come parametro del contenitore,
come se fosse elencato nella sezione ``parameters`` del config, ma con il beneficio
aggiuntivo di fondere file diversi e della validazione della configurazione::

    public function load(array $configs, ContainerBuilder $container)
    {
        $configuration = new Configuration();
        $processor = new Processor();
        $config = $processor->processConfiguration($configuration, $configs);

        $container->setParameter('acme_demo.PIPPO', $config['pippo'])

        // ...
    }

Si possono stabilire requisiti di configurazione più complessi nelle classi
estensione. Per esempio, si può scegliere di caricare un file di configurazione principale,
ma anche di carne uno secondario solo se un certo parametro è impostato::

    public function load(array $configs, ContainerBuilder $container)
    {
        $configuration = new Configuration();
        $processor = new Processor();
        $config = $processor->processConfiguration($configuration, $configs);

        $loader = new XmlFileLoader(
            $container,
            new FileLocator(__DIR__.'/../Resources/config')
        );
        $loader->load('services.xml');

        if ($config['advanced']) {
            $loader->load('advanced.xml');
        }
    }

.. note::

    La registrazione di un'estensione nel contenitore non è sufficiente
    per includerla tra le estensioni processate durante la compilazione del contenitore.
    Caricare la configurazione che usa l'alias dell'estensione come chiave, come mostrato in
    precedenza, assicurerà il suo caricamento. Si può anche dire al costruttore di contenitore 
    di caricarla, usando il metodo
    :method:`Symfony\\Component\\DependencyInjection\\ContainerBuilder::loadFromExtension`::


        use Symfony\Component\DependencyInjection\ContainerBuilder;

        $container = new ContainerBuilder();
        $extension = new AcmeDemoExtension();
        $container->registerExtension($extension);
        $container->loadFromExtension($extension->getAlias());
        $container->compile();

.. note::

    Se si deve manipolare la configurazione caricata da un'estensione,
    non lo si può fare da un'altra estensione, perché usa un contenitore nuovo.
    Invece, si deve usare un passo di compilatore, che funziona con il contenitore
    dopo che le estensioni sono state processate.

.. _components-dependency-injection-compiler-passes:

Prependere la configurazione passata all'estensione
---------------------------------------------------

.. versionadded:: 2.2
    La possibilità di prependere la configurazione di un bundle è nuova in
    Symfony 2.2.

Una Extension può prependere la configurazione di un altro bundle, prima della chiamata al metodo ``load()``,
implementando :class:`Symfony\\Component\\DependencyInjection\\Extension\\PrependExtensionInterface`::

    use Symfony\Component\DependencyInjection\Extension\PrependExtensionInterface;
    // ...

    class AcmeDemoExtension implements ExtensionInterface, PrependExtensionInterface
    {
        // ...

        public function prepend()
        {
            // ...

            $container->prependExtensionConfig($name, $config);

            // ...
        }
    }

Per maggiori dettagli, si veda :doc:`/cookbook/bundles/prepend_extension`, che è specifica
del framework Symfony, ma contiene più informazioni su questa caratteristica.

Creare un passo di compilatore
------------------------------

Si possono anche creare e registrare i propri passi di compilatore con il contenitore.
Per creare un passo di compilatore, si deve implementare l'interfaccia
:class:`Symfony\\Component\\DependencyInjection\\Compiler\\CompilerPassInterface`.
Il compilatore offre la possibilità di manipolare le definizioni del servizio che sono state
compilate. Questo può essere molto potente, ma non necessario nell'uso
quotidiano.

Il passo di compilatore deve avere il metodo ``process``, che viene passato al contenitore
che si sta compilando::

    use Symfony\Component\DependencyInjection\Compiler\CompilerPassInterface;
    use Symfony\Component\DependencyInjection\ContainerBuilder;

    class CustomCompilerPass implements CompilerPassInterface
    {
        public function process(ContainerBuilder $container)
        {
           // ...
        }
    }

Si possono manipolare parametri e definizioni del contenitore, usando i metodi descritti
in :doc:`/components/dependency_injection/definitions`. Un cosa che si fa solitamente in
un passo di compilatore è la ricerca di tutti i servizi con determinato tag, in modo
da poterli processare in qualche modo o collegarli dinamicamente in qualche
altro servizio.

Registrare un passo di compilatore
----------------------------------

Occorre registrare il proprio passo di compilatore con il contenitore. Il suo metodo ``process``
sarà richiamato quando il contenitore viene compilato::

    use Symfony\Component\DependencyInjection\ContainerBuilder;

    $container = new ContainerBuilder();
    $container->addCompilerPass(new CustomCompilerPass);

.. note::

    I passi di compilatore sono registrati in modo diverso, se si usa il
    framework completo, si veda :doc:`/cookbook/service_container/compiler_passes`
    per maggiori dettagli.

Controllare l'ordine dei passi
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

I passi di compilatore predefiniti sono raggruppati in passi di ottimizzazione e passi di
rimozione. I passi di ottimizzazione girano prima e includono compiti come la risoluzione
di riferimenti dentro le definizioni. I passi di rimozione eseguono compiti come la
rimozione di alias privati e di servizi inutilizzati. Si può scegliere in quale ordine
sia eseguito ogni passo aggiuntivo. Per impostazione predefinita, sono eseguiti prima dei passi di ottimizzazione.

Si possono usare le seguenti costanti come secondo parametro quando si registra un
passo con il contenitore, per controllare in quale posizione vada il passo:

* ``PassConfig::TYPE_BEFORE_OPTIMIZATION``
* ``PassConfig::TYPE_OPTIMIZE``
* ``PassConfig::TYPE_BEFORE_REMOVING``
* ``PassConfig::TYPE_REMOVE``
* ``PassConfig::TYPE_AFTER_REMOVING``

Per esempio, per eseguire il proprio passo dopo i passi di rimozione predefiniti::

    use Symfony\Component\DependencyInjection\ContainerBuilder;
    use Symfony\Component\DependencyInjection\Compiler\PassConfig;

    $container = new ContainerBuilder();
    $container->addCompilerPass(
        new CustomCompilerPass,
        PassConfig::TYPE_AFTER_REMOVING
    );

.. _components-dependency-injection-dumping:

Esportare la configurazione per le prestazioni
----------------------------------------------

L'uso di file di configurazione per gestire il contenitore di servizi può essere molto più
facile da capire rispetto all'uso di PHP, appena ci sono molti servizi. Questa facilità
ha un prezzo, quando si considerano le prestazioni, perché i file di configurazione
necessitano di essere analizzati, in modo da costruire la configurazione in PHP. Si
possono prendere due piccioni con una fava, usando i file di configurazione e poi
esportando e mettendo in cache la configurazione risultante. ``PhpDumper`` rende
facile l'esportazione del contenitore compilato::

    use Symfony\Component\DependencyInjection\ContainerBuilder;
    use Symfony\Component\DependencyInjection\Dumper\PhpDumper;

    $file = __DIR__ .'/cache/container.php';

    if (file_exists($file)) {
        require_once $file;
        $container = new ProjectServiceContainer();
    } else {
        $container = new ContainerBuilder();
        // ...
        $container->compile();

        $dumper = new PhpDumper($container);
        file_put_contents($file, $dumper->dump());
    }

``ProjectServiceContainer`` è il nome predefinito dato alla classe del contenitore
esportata: lo si può cambiare tramite l'opzione ``class``, al momento
dell'esportazione::

    // ...
    $file = __DIR__ .'/cache/container.php';

    if (file_exists($file)) {
        require_once $file;
        $container = new MyCachedContainer();
    } else {
        $container = new ContainerBuilder();
        // ...
        $container->compile();

        $dumper = new PhpDumper($container);
        file_put_contents(
            $file,
            $dumper->dump(array('class' => 'MyCachedContainer'))
        );
    }

Si otterrà la velocità del contenitore compilato in PHP con la facilità di usare file di
configurazione. Inoltre, esportare il contenitore in questo modo ottimizza ulteriormente
i servizi creati dal contenitore.

Nell'esempio precedente, occorrerà pulire il contenitore in cache ogni volta
che si fa una modifica. L'aggiunta di una variabile che determini se si è in
modalità di debug consente di mantenere la velocità del contenitore in cache in
produzione, mantenendo una configurazione aggiornata durante lo sviluppo dell'applicazione::

    // ...

    // impostare $isDebug in base a una logica del progetto
    $isDebug = ...;

    $file = __DIR__ .'/cache/container.php';

    if (!$isDebug && file_exists($file)) {
        require_once $file;
        $container = new MyCachedContainer();
    } else {
        $container = new ContainerBuilder();
        // ...
        $container->compile();

        if (!$isDebug) {
            $dumper = new PhpDumper($container);
            file_put_contents(
                $file,
                $dumper->dump(array('class' => 'MyCachedContainer'))
            );
        }
    }

Si può fare un ulteriore miglioramento solo ricompilando il contenitore in modalità
debug quando le modifiche sono state fatte alla sua configurazione, piuttosto che a ogni
richiesta. Lo si può fare mettendo in cache i file risorse usati per configurare
il contenitore, come descritto nella documentazione del componente config,
":doc:`/components/config/caching`".

Non occorre calcolare quali file mettere in cache, perché il costruttore del contenitore
tiene traccia di tutte le risorse usate per configurarlo, non solo dei file di configurazione,
ma anche le classi estensione e i passi di compilatore. Ciò significa che qualsiasi
modifica a uno di tali file invaliderà la cache e farà scattare la ricostruzione
del contenitore. Basta chiedere al contenitore queste risorse e usarle
come meta dati per la cache::

    // ...

    // impostare $isDebug in base a qualcosa nel progetto
    $isDebug = ...;

    $file = __DIR__ .'/cache/container.php';
    $containerConfigCache = new ConfigCache($file, $isDebug);

    if (!$containerConfigCache->isFresh()) {
        $containerBuilder = new ContainerBuilder();
        // ...
        $containerBuilder->compile();

        $dumper = new PhpDumper($containerBuilder);
        $containerConfigCache->write(
            $dumper->dump(array('class' => 'MyCachedContainer')),
            $containerBuilder->getResources()
        );
    }

    require_once $file;
    $container = new MyCachedContainer();

Ora il contenitore in cache esportato viene usato indipendentemente dalla modalità di debug.
La differenza è che ``ConfigCache`` è impostato a debug con il secondo parametro del suo
costruttore. Quando la cache non è in debug, sarà sempre usato il contenitore in cache, se
esiste. In debug, viene scritto un file aggiuntivo di meta dati, con i timestamp di
tutti i file risorsa. Vengono poi verificate eventuali modifiche dei file, nel caso in cui
la cache debba essere considerata vecchia.

.. note::

    Nel framework completo, compilazione e messa in cache del contenitore sono
    eseguite automaticamente.
