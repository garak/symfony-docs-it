.. index::
   single: Configurazione; Semantica
   single: Bundle; Configurazione dell'estensione

Come esporre una configurazione semantica per un bundle
=======================================================

Se si apre il file di configurazione della propria applicazione (di solito ``app/config/config.yml``),
si vedranno un certo numero di "spazi di nomi" di configurazioni, come ``framework``,
``twig`` e ``doctrine``. Ciasucno di questi configura uno specifico bundle, consentendo di
configurare cose ad alto livello e quindi lasciando al bundle tutte le modifiche complesse
e di basso livello.

Per esempio, il codice seguente dice a ``FrameworkBundle`` di abilitare l'integrazione
con i form, che implica la definizione di alcuni servizi, così come anche
l'integrazione di altri componenti correlati:

.. configuration-block::

    .. code-block:: yaml
    
        framework:
            # ...
            form:            true

    .. code-block:: xml
    
        <framework:config>
            <framework:form />
        </framework:config>

    .. code-block:: php
    
        $container->loadFromExtension('framework', array(
            // ...
            'form'            => true,
            // ...
        ));

Quando si crea un bundle, si hanno due scelte sulla gestione della configurazione:

1. **Normale configurazione di servizi** (*facile*):
  
    Si possono specificare i propri servizi in un file di configurazione (p.e. ``services.yml``)
    posto nel proprio bundle e quindi importarlo dalla configurazione principale della
    propria applicazione. Questo è molto facile, rapido ed efficace. Se si usano i
    :ref:`parametri<book-service-container-parameters>`, si avrà ancora la
    flessibilità di personalizzare il bundle dalla configurazione della propria
    applicazione. Vedere ":ref:`service-container-imports-directive`" per ulteriori
    dettagli.

2. **Esporre una configurazione semantica** (*avanzato*):

    Questo è il modo usato per la configurazione dei bundle del nucleo (come
    descritto sopra). L'idea di base è che, invece di far sovrascrivere all'utente
    i singoli parametri, lasciare che ne configuri alcune opzioni create
    specificatamente. Lo sviluppatore del bundle deve quindi analizzare tale
    configurazione e caricare i servizi all'interno di una classe "Extension". Con
    questo metodo, non si avrà bisogno di importare alcuna risorsa di configurazione
    dall'appplicazione principale: la classe Extension può gestire tutto.

La seconda opzione, di cui parleremo, è molto più flessibili, ma richiede anche
più tempo di preparazione. Se si ci sta chiedendo quale metodo scegliere,
probabilmente è una buona idea partire col primo metodo, poi cambiare al secondo,
se se ne ha necessità.

Il secondo metodo ha diversi vantaggi:

* Molto più potente che definire semplici parametri: un valore specifico di un'opzione
  può scatenare la creazioni di molte definizioni di servizi;

* Possibilità di avere una gerarchia di configurazioni

* Fusione intelligente quando diversi file di configurazione (p.e. ``config_dev.yml``
  e ``config.yml``) sovrascrivono le proprie configurazioni a vicenda;

* Validazione della configurazione (se si usa una
  :ref:`classe di configurazione<cookbook-bundles-extension-config-class>`);

* auto-completamento nell'IDE quando si crea un XSD e lo sviluppatore usa XML.

.. sidebar:: Sovrascrivere parametri dei bundle

    Se un bundle fornisce una classe Extension, in generale *non* si dovrebbe
    sovrascrivere alcun parametro del contenitore di servizi per quel bundle.
    L'idea è che se è presente una classe Extension, ogni impostazione configurabile
    sia presente nella configurazione messa a disposizione da tale classe.
    In altre parole, la classe Extension definisce tutte le impostazioni supportate
    pubblicamente, per i quali sarà mantenuta
    una retro-compatibilità. 

.. index::
   single: Bundle; Extension
   single: Dependency Injection, Extension

Creare una classe Extension
---------------------------

Se si sceglie di esporre una configurazione semantica per il proprio bundle, si avrà
prima bisogno di creare una nuova classe "Extension", per gestire il processo.
Tale classe va posta nella cartella ``DependencyInjection`` del proprio bundle
e il suo nome va costruito sostituendo il postfisso ``Bundle`` del nome della classe
del bundle con ``Extension``. Per esempio, la classe Extension di
``AcmeHelloBundle`` si chiamerebbe ``AcmeHelloExtension``::

    // Acme/HelloBundle/DependencyInjection/AcmeHelloExtension.php
    use Symfony\Component\HttpKernel\DependencyInjection\Extension;
    use Symfony\Component\DependencyInjection\ContainerBuilder;

    class AcmeHelloExtension extends Extension
    {
        public function load(array $configs, ContainerBuilder $container)
        {
            // qui sta tutta la logica
        }

        public function getXsdValidationBasePath()
        {
            return __DIR__.'/../Resources/config/';
        }

        public function getNamespace()
        {
            return 'http://www.example.com/symfony/schema/';
        }
    }

.. note::

    I metodi ``getXsdValidationBasePath`` e ``getNamespace`` servono solo
    se il bundle fornisce degli XSD facoltativi per la configurazione.

La presenza della classe precedente vuol dire che si può definire uno spazio dei nomi
``acme_hello`` in un qualsiasi file di configurazione. Lo spazio dei nomi ``acme_hello``
viene dal nome della classe Extension, a cui è stata rimossa la parola ``Extension``
e posto in minuscolo e con trattini bassi il resto del nome. In altre parole,,
``AcmeHelloExtension`` diventa ``acme_hello``.

Si può iniziare specificando la configurazione sotto questo spazio dei nomi:

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        acme_hello: ~

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <?xml version="1.0" ?>

        <container xmlns="http://symfony.com/schema/dic/services"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xmlns:acme_hello="http://www.example.com/symfony/schema/"
            xsi:schemaLocation="http://www.example.com/symfony/schema/ http://www.example.com/symfony/schema/hello-1.0.xsd">

           <acme_hello:config />
           ...

        </container>

    .. code-block:: php

        // app/config/config.php
        $container->loadFromExtension('acme_hello', array());

.. tip::

    Seguendo le convenzioni di nomenclatura viste sopra, il metodo ``load()``
    della propria estensione sarà sempre richiamato, a patto che il proprio bundle
    sia registrato nel Kernel. In altre parole, anche se l'utente non fornisce
    alcuna configurazione (cioè se la voce ``acme_hello`` non appare mai),
    il metodo ``load()`` sarà richiamato, passandogli un array ``$configs``
    vuoto. Si possono comunque fornire valori predefiniti adeguati per il proprio
    bundle, se lo si desidera.

Analisi dell'array ``$configs``
-------------------------------

Ogni volta che un utente include lo spazio dei nomi ``acme_hello`` in un file di
configurazione, la configurazione sotto di esso viene aggiunta a un array di configurazioni
e passata al metodo ``load()`` dell'estensione (Symfony2 converte automaticamente
XML e YAML in array).

Si prenda la seguente configurazione:

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        acme_hello:
            foo: fooValue
            bar: barValue

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <?xml version="1.0" ?>

        <container xmlns="http://symfony.com/schema/dic/services"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xmlns:acme_hello="http://www.example.com/symfony/schema/"
            xsi:schemaLocation="http://www.example.com/symfony/schema/ http://www.example.com/symfony/schema/hello-1.0.xsd">

            <acme_hello:config foo="fooValue">
                <acme_hello:bar>barValue</acme_hello:bar>
            </acme_hello:config>

        </container>

    .. code-block:: php

        // app/config/config.php
        $container->loadFromExtension('acme_hello', array(
            'foo' => 'fooValue',
            'bar' => 'barValue',
        ));

L'array passato al metodo ``load()`` sarà simile a questo::

    array(
        array(
            'foo' => 'fooValue',
            'bar' => 'barValue',
        )
    )

Si noti che si tratta di un *array di array*, non di un semplice array di valori di
configurazione. È stato fatto intenzionalmente. Per esempio, se ``acme_hello``
appare in un altro file di configurazione, come ``config_dev.yml``, con valori diversi
sotto di esso, l'array in uscita sarà simile a questo::

    array(
        array(
            'foo' => 'fooValue',
            'bar' => 'barValue',
        ),
        array(
            'foo' => 'fooDevValue',
            'baz' => 'newConfigEntry',
        ),
    )

L'ordine dei due array dipende da quale è stato definito prima. È compito di chi
sviluppa il bundle, quindi, decidere in che modo tali configurazioni vadano fuse
insieme. Si potrebbe, per esempio, voler fare in modo che i valori successivi
sovrascrivano quelli precedenti, oppure fonderli in qualche modo.

Successivamente, nella sezione :ref:`classe Configuration<cookbook-bundles-extension-config-class>`,
si imparerà un modo robusto per gestirli. Per ora, ci si può accontentare di
fonderli a mano::

    public function load(array $configs, ContainerBuilder $container)
    {
        $config = array();
        foreach ($configs as $subConfig) {
            $config = array_merge($config, $subConfig);
        }

        // usare ora l'array $config
    }

.. caution::

    Assicurarsi che la tecnica di fusione vista sopra abbia senso per il proprio bundle.
    Questo è solo un esempio e andrebbe usato con la dovuta cautela.

Usare il metodo ``load()``
--------------------------

Con ``load()``, la variabile ``$container`` si riferisce a un contenitore che conosce solo
la configurazione del proprio spazio dei nomi (cioè non contiene informazioni su servizi
caricati da altri bundle). Lo scopo del metodo ``load()`` è quello di manipolare
il contenitore, aggiungere e configurare ogni metodo o servizio necessario per il
proprio bundle.

Caricare risorse di configurazioni esterne
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Una cosa che si fa di solito è caricare un file di configurazione esterno, che potrebbe
contenere i servizi necessari al proprio bundle. Per esempio, si supponga di avere
un file ``services.xml``, che contiene molte delle configurazioni di servizio del proprio
bundle::

    use Symfony\Component\DependencyInjection\Loader\XmlFileLoader;
    use Symfony\Component\Config\FileLocator;

    public function load(array $configs, ContainerBuilder $container)
    {
        // prepara la propria variabile $config

        $loader = new XmlFileLoader($container, new FileLocator(__DIR__.'/../Resources/config'));
        $loader->load('services.xml');
    }

Lo si potrebbe anche con una condizione, basata su uno dei valori di configurazione.
Per esempio, si supponga di voler caricare un insieme di servizi, ma solo se un'opzione
``enabled`` è impostata a ``true``::

    public function load(array $configs, ContainerBuilder $container)
    {
        // prepara la propria variabile $config

        $loader = new XmlFileLoader($container, new FileLocator(__DIR__.'/../Resources/config'));
        
        if (isset($config['enabled']) && $config['enabled']) {
            $loader->load('services.xml');
        }
    }

Configurare servizi e impostare parametri
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Una volta caricati alcune configurazioni di servizi, si potrebbe aver bisogno di modificare
la configurazione in base ad alcuni valori inseriti. Per esempio, si supponga di avere
un servizio il cui primo parametro è una stringa "type", che sarà usata
internamente. Si vorrebbe che fosse facilmente configurata dall'utente del bundle, quindi
nella proprio file di configurazione del servizio (``services.xml``), si definisce questo
servizio e si usa un parametro vuoto, come ``acme_hello.my_service_type``, come primo
parametro:

.. code-block:: xml

    <!-- src/Acme/HelloBundle/Resources/config/services.xml -->
    <container xmlns="http://symfony.com/schema/dic/services"
        xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
        xsi:schemaLocation="http://symfony.com/schema/dic/services http://symfony.com/schema/dic/services/services-1.0.xsd">

        <parameters>
            <parameter key="acme_hello.my_service_type" />
        </parameters>

        <services>
            <service id="acme_hello.my_service" class="Acme\HelloBundle\MyService">
                <argument>%acme_hello.my_service_type%</argument>
            </service>
        </services>
    </container>

Ma perché definire un parametro vuoto e poi passarlo al proprio servizio?
La risposa è che si imposterà questo parametro nella propria classe Extension, in base
ai valori di configurazione in entrata. Si supponga, per esempio, di voler consentire
all'utente di definire questa opzione *type* sotto una chiave di nome ``my_type``.
Aggiungere al metodo ``load()`` il codice seguente::

    public function load(array $configs, ContainerBuilder $container)
    {
        // prepara la propria variabile $config

        $loader = new XmlFileLoader($container, new FileLocator(__DIR__.'/../Resources/config'));
        $loader->load('services.xml');

        if (!isset($config['my_type'])) {
            throw new \InvalidArgumentException('The "my_type" option must be set');
        }

        $container->setParameter('acme_hello.my_service_type', $config['my_type']);
    }

L'utente ora è in grado di configurare effettivamente il servizio, specificando il
valore di configurazione ``my_type``:

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        acme_hello:
            my_type: foo
            # ...

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <?xml version="1.0" ?>

        <container xmlns="http://symfony.com/schema/dic/services"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xmlns:acme_hello="http://www.example.com/symfony/schema/"
            xsi:schemaLocation="http://www.example.com/symfony/schema/ http://www.example.com/symfony/schema/hello-1.0.xsd">

            <acme_hello:config my_type="foo">
                <!-- ... -->
            </acme_hello:config>

        </container>

    .. code-block:: php

        // app/config/config.php
        $container->loadFromExtension('acme_hello', array(
            'my_type' => 'foo',
            // ...
        ));

Parametri globali
~~~~~~~~~~~~~~~~~

Quando si configura il contenitore, si hanno a disposizione i seguenti parametri
globali:

* ``kernel.name``
* ``kernel.environment``
* ``kernel.debug``
* ``kernel.root_dir``
* ``kernel.cache_dir``
* ``kernel.logs_dir``
* ``kernel.bundle_dirs``
* ``kernel.bundles``
* ``kernel.charset``

.. caution::

    Tutti i nomi di parametri e di servizi che iniziano con ``_`` sono riservati al
    framework e non se ne dovrebbero definire altri nei bundle.

.. _cookbook-bundles-extension-config-class:

Validazione e fusione con una classe Configuration
--------------------------------------------------

Finora, la fusione degli array di configurazione è stata fatta a mano, verificando la
presenza di valori di configurazione con la funzione ``isset()`` di PHP.
Un sistema opzionale *Configuration* è disponibile, per aiutare nella fusione, nella
validazione, con i valori predefiniti e per la normalizzazione dei formati.

.. note::

    La normalizzazione dei formati riguarda alcuni formati, soprattutto XML, che
    offrono array di configurazione leggermente diversi, per cui tali array hanno
    bisgno di essere normalizzati, per corrispondere a tutti gli altri.

Per sfruttare questo sistema, si creerà una classe ``Configuration`` e si costruirà
un albero, che definisce la propria configurazione in tale classe::

    // src/Acme/HelloBundle/DependencyExtension/Configuration.php
    namespace Acme\HelloBundle\DependencyInjection;

    use Symfony\Component\Config\Definition\Builder\TreeBuilder;
    use Symfony\Component\Config\Definition\ConfigurationInterface;

    class Configuration implements ConfigurationInterface
    {
        public function getConfigTreeBuilder()
        {
            $treeBuilder = new TreeBuilder();
            $rootNode = $treeBuilder->root('acme_hello');

            $rootNode
                ->children()
                    ->scalarNode('my_type')->defaultValue('bar')->end()
                ->end()
            ;

            return $treeBuilder;
        }

Questo è un esempio *molto* semplice, ma si può ora usare questa classe nel proprio
metodo ``load()``, per fondere la propria configurazione e forzare la validazione. Se
viene passata un'opzione che non sia ``my_type``, l'utente sarà avvisato con un'eccezione
del passaggio di un'opzione non supportata::

    use Symfony\Component\Config\Definition\Processor;
    // ...

    public function load(array $configs, ContainerBuilder $container)
    {
        $processor = new Processor();
        $configuration = new Configuration();
        $config = $processor->processConfiguration($configuration, $configs);
    
        // ...
    }

Il metodo ``processConfiguration()`` usa l'albero di configurazione definito nella classe
``Configuration`` per validare, normalizzare e fondere tutti gli array di configurazione
insieme.

La classe ``Configuration`` può essere molto più complicata di quanto mostrato qui, poiché
supporta nodi array, nodi "prototipo", validazione avanzata, normalizzazione specifica di
XML e fusione avanzata. Il modo migliore per vederla in azione è guardare alcune classi
Configuration del nucleo, come quella `FrameworkBundle`_ o di
`TwigBundle`_.

.. index::
   pair: Convenzione; Configuration

Convenzioni per l'estensione
----------------------------

Quando si crea un'estensione, seguire queste semplici convenzioni:

* L'estensione deve trovarsi nel sotto-spazio dei nomi ``DependencyInjection``;

* l'estensione deve avere lo stesso nome del bundle, ma con
  ``Extension`` (``AcmeHelloExtension`` per ``AcmeHelloBundle``);

* L'estensione deve fornire uno schema XSD.

Se si seguono queste semplici convenzioni, la propria estensione sarà registrata
automaticamente da Symfony2. In caso contrario, sovrascrivere il metodo
:method:`Symfony\\Component\\HttpKernel\\Bundle\\Bundle::build` nel proprio
bundle::

    use Acme\HelloBundle\DependencyInjection\UnconventionalExtensionClass;

    class AcmeHelloBundle extends Bundle
    {
        public function build(ContainerBuilder $container)
        {
            parent::build($container);

            // registrare a mano estensioni che non seguono le convenzioni
            $container->registerExtension(new UnconventionalExtensionClass());
        }
    }

In questo caso, la classe Extension deve implementare anche un metodo ``getAlias()`` e
restituire un alias univoco, con nome che dipende dal bundle (p.e. ``acme_hello``). 
Questo perché il nome della classe non segue le convenzioni e non finisce per
``Extension``.

Inoltre, il metodo ``load()`` dell'estensione sarà richiamato *solo* se l'utente
specifica l'alias ``acme_hello`` in almeno un file di configurazione. Ancora,
questo perché la classe Extension non segue le convenzioni viste sopra, quindi
non succede nulla in modo automatico.

.. _`FrameworkBundle`: https://github.com/symfony/symfony/blob/master/src/Symfony/Bundle/FrameworkBundle/DependencyInjection/Configuration.php
.. _`TwigBundle`: https://github.com/symfony/symfony/blob/master/src/Symfony/Bundle/TwigBundle/DependencyInjection/Configuration.php
