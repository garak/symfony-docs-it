I tag della dependency injection
================================

I tag della dependency injection sono piccole stringhe, che si possono applicare a un servizio
per "marcarlo" per essere usato in un modo speciale. Per esempio, se si ha un servizio
che si vuole registrare come ascoltatore di uno degli eventi del nucleo di Symfony,
lo si può marcare con il tag ``kernel.event_listener``.

Si possono approfondire i tag leggendo la sezione ":ref:`book-service-container-tags`"
del capitolo sul contenitore di servizi.

Di seguito si trovano informazioni su tutti i tag disponibili in Symfony2. Potrebbero
esserci altri tag in alcuni bundle utilizzati, che non sono elencati qui.

+-----------------------------------+---------------------------------------------------------------------------+
| Nome tag                          | Utilizzo                                                                  |
+-----------------------------------+---------------------------------------------------------------------------+
| `assetic.asset`_                  | Registrare una risorsa nel gestore di risorse corrente                    |
+-----------------------------------+---------------------------------------------------------------------------+
| `assetic.factory_worker`_         | Aggiungere un factory worker                                              |
+-----------------------------------+---------------------------------------------------------------------------+
| `assetic.filter`_                 | Registrare un filtro                                                      |
+-----------------------------------+---------------------------------------------------------------------------+
| `assetic.formula_loader`_         | Aggiungere un formula loader al gestore di risorse corrente               |
+-----------------------------------+---------------------------------------------------------------------------+
| `assetic.formula_resource`_       | Aggiungere una risorsa al gestore di risorse corrente                     |
+-----------------------------------+---------------------------------------------------------------------------+
| `assetic.templating.php`_         | Rimuovere questo servizio se i template PHP sono disabilitati             |
+-----------------------------------+---------------------------------------------------------------------------+
| `assetic.templating.twig`_        | Rimuovere questo servizio se i template Twig sono disabilitati            |
+-----------------------------------+---------------------------------------------------------------------------+
| `data_collector`_                 | Creare una classe che raccolga dati personalizzati per il profilatore     |
+-----------------------------------+---------------------------------------------------------------------------+
| `doctrine.event_listener`_        | Aggiungere un ascoltatore di eventi Doctrine                              |
+-----------------------------------+---------------------------------------------------------------------------+
| `doctrine.event_subscriber`_      | Aggiungere un sottoscrittore di eventi Doctrine                           |
+-----------------------------------+---------------------------------------------------------------------------+
| `form.type`_                      | Creare un tipo di campo personalizzato per i form                         |
+-----------------------------------+---------------------------------------------------------------------------+
| `form.type_extension`_            | Creare un "form extension" personalizzato                                 |
+-----------------------------------+---------------------------------------------------------------------------+
| `form.type_guesser`_              | Aggiungere logica per "form type guessing"                                |
+-----------------------------------+---------------------------------------------------------------------------+
| `kernel.cache_clearer`_           | Registrare un servizio da richiamare durante la pulizia della cache       |
+-----------------------------------+---------------------------------------------------------------------------+
| `kernel.cache_warmer`_            | Registrare un servizio da richiamare durante il cache warming             |
+-----------------------------------+---------------------------------------------------------------------------+
| `kernel.event_listener`_          | Ascoltare diversi eventi/agganci in Symfony                               |
+-----------------------------------+---------------------------------------------------------------------------+
| `kernel.event_subscriber`_        | Sottoscrivere un insieme di vari eventi/agganci in Symfony                |
+-----------------------------------+---------------------------------------------------------------------------+
| `kernel.fragment_renderer`_       | Aggiunta di nuove strategie di resa del contenuto HTTP                    |
+-----------------------------------+---------------------------------------------------------------------------+
| `monolog.logger`_                 | Log con un canale di log personalizzato                                   |
+-----------------------------------+---------------------------------------------------------------------------+
| `monolog.processor`_              | Aggiunta di un processore personalizzato per i log                        |
+-----------------------------------+---------------------------------------------------------------------------+
| `routing.loader`_                 | Registrare un servizio personalizzato che carica rotte                    |
+-----------------------------------+---------------------------------------------------------------------------+
| `security.voter`_                 | Aggiunta di un votante alla logica di autorizzazione di Symfony           |
+-----------------------------------+---------------------------------------------------------------------------+
| `security.remember_me_aware`_     | Consentire il "ricorami" nell'autenticazione                              |
+-----------------------------------+---------------------------------------------------------------------------+
| `swiftmailer.plugin`_             | Registre un plugin di SwiftMailer                                         |
+-----------------------------------+---------------------------------------------------------------------------+
| `templating.helper`_              | Rendere il servizio disponibile nei template PHP                          |
+-----------------------------------+---------------------------------------------------------------------------+
| `translation.loader`_             | Registrare un servizio che carica traduzioni                              |
+-----------------------------------+---------------------------------------------------------------------------+
| `twig.extension`_                 | Registrare un'estensione di Twig                                          |
+-----------------------------------+---------------------------------------------------------------------------+
| `twig.loader`_                    | Registrare un servizio personalizzato che carica template Twig            |
+-----------------------------------+---------------------------------------------------------------------------+
| `validator.constraint_validator`_ | Creare un vincolo di validazione personalizzato                           |
+-----------------------------------+---------------------------------------------------------------------------+
| `validator.initializer`_          | Registrare un servizio che inizializzi gli oggetti prima della validazione|
+-----------------------------------+---------------------------------------------------------------------------+

assetic.asset
-------------

**Scopo**: Registrare una risorsa nel gestore di risorse corrente

assetic.factory_worker
----------------------

**Scopo**: Aggiungere un factory worker

A Factory worker is a class implementing ``Assetic\Factory\Worker\WorkerInterface``.
Its ``process($asset)`` method is called for each asset after asset creation.
You can modify an asset or even return a new one.

In order to add a new worker, first create a class::

    use Assetic\Asset\AssetInterface;
    use Assetic\Factory\Worker\WorkerInterface;

    class MyWorker implements WorkerInterface
    {
        public function process(AssetInterface $asset)
        {
            // ... change $asset or return a new one
        }

    }

And then add register it as a tagged service:

.. configuration-block::

    .. code-block:: yaml

        services:
            acme.my_worker:
                class: MyWorker
                tags:
                    - { name: assetic.factory_worker }

    .. code-block:: xml

        <service id="acme.my_worker" class="MyWorker>
            <tag name="assetic.factory_worker" />
        </service>

    .. code-block:: php

        $container
            ->register('acme.my_worker', 'MyWorker')
            ->addTag('assetic.factory_worker')
        ;

assetic.filter
--------------

**Scopo**: Registrare un filtro

AsseticBundle uses this tag to register common filters. You can also use
this tag to register your own filters.

First, you need to create a filter::

    use Assetic\Asset\AssetInterface;
    use Assetic\Filter\FilterInterface;

    class MyFilter implements FilterInterface
    {
        public function filterLoad(AssetInterface $asset)
        {
            $asset->setContent('alert("yo");' . $asset->getContent());
        }

        public function filterDump(AssetInterface $asset)
        {
            // ...
        }
    }

Second, define a service:

.. configuration-block::

    .. code-block:: yaml

        services:
            acme.my_filter:
                class: MyFilter
                tags:
                    - { name: assetic.filter, alias: my_filter }

    .. code-block:: xml

        <service id="acme.my_filter" class="MyFilter">
            <tag name="assetic.filter" alias="my_filter" />
        </service>

    .. code-block:: php

        $container
            ->register('acme.my_filter', 'MyFilter')
            ->addTag('assetic.filter', array('alias' => 'my_filter'))
        ;

Finally, apply the filter:

.. code-block:: jinja

    {% javascripts
        '@AcmeBaseBundle/Resources/public/js/global.js'
        filter='my_filter'
    %}
        <script src="{{ asset_url }}"></script>
    {% endjavascripts %}

You can also apply your filter via the ``assetic.filters.my_filter.apply_to``
config option as it's described here: :doc:`/cookbook/assetic/apply_to_option`.
In order to do that, you must define your filter service in a separate xml
config file and point to this file's path via the ``assetic.filters.my_filter.resource``
configuration key.

assetic.formula_loader
----------------------

**Scopo**: Aggiungere un formula loader al gestore di risorse corrente

A Formula loader is a class implementing
``Assetic\\Factory\Loader\\FormulaLoaderInterface`` interface. This class
is responsible for loading assets from a particular kind of resources (for
instance, twig template). Assetic ships loaders for php and twig templates.

An ``alias`` attribute defines the name of the loader.

assetic.formula_resource
------------------------

**Scopo**: Aggiungere una risorsa al gestore di risorse corrente

A resource is something formulae can be loaded from. For instance, twig
templates are resources.

assetic.templating.php
----------------------

**Scopo**: Rimuovere questo servizio se i template PHP sono disabilitati

The tagged service will be removed from the container if the
``framework.templating.engines`` config section does not contain php.

assetic.templating.twig
-----------------------

**Scopo**: Rimuovere questo servizio se i template Twig sono disabilitati

The tagged service will be removed from the container if
``framework.templating.engines`` config section does not contain twig.

data_collector
--------------

**Scopo**: creare una classe che raccolga dati personalizzati per il profilatore

Per dettagli su come creare i propri insiemi di dati, leggere la ricetta
:doc:`/cookbook/profiler/data_collector`.

doctrine.event_listener
-----------------------

**Scopo**: Aggiungere un ascoltatore di eventi Doctrine

For details on creating Doctrine event listeners, read the cookbook article:
:doc:`/cookbook/doctrine/event_listeners_subscribers`.

doctrine.event_subscriber
-------------------------

**Scopo**: Aggiungere un sottoscrittore di eventi Doctrine

For details on creating Doctrine event subscribers, read the cookbook article:
:doc:`/cookbook/doctrine/event_listeners_subscribers`.

.. _dic-tags-form-type:

form.type
---------

**Scopo**: Creare un tipo di campo personalizzato per i form

Per dettagli su come creare il proprio tipo di campo, leggere la ricetta
:doc:`/cookbook/form/create_custom_field_type`.

form.type_extension
-------------------

**Scopo**: Creare un "form extension" personalizzato

Le estensioni dei form sono un modo per portare un "aggancio" nella creazione di qualsiasi
campo del proprio form. Per esempio, l'aggiunta di un tokek per il CSRF si fa tramite
un'estensione del form (:class:`Symfony\\Component\\Form\\Extension\\Csrf\\Type\\FormTypeCsrfExtension`).

Un'estensione di form può modificare qualsiasi parte di qualsiasi campo di un form. Per
creare un'estensione, creare prima di tutto una classe che implementi l'interfaccia
:class:`Symfony\\Component\\Form\\FormTypeExtensionInterface`.
Per semplicità, spesso si estenderà la classe
:class:`Symfony\\Component\\Form\\AbstractTypeExtension` invece che direttamente
l'interfaccia::

    // src/Acme/MainBundle/Form/Type/MyFormTypeExtension.php
    namespace Acme\MainBundle\Form\Type;

    use Symfony\Component\Form\AbstractTypeExtension;

    class MyFormTypeExtension extends AbstractTypeExtension
    {
        // ... inserire i metodi che si vogliono sovrascrivere
        // come buildForm(), buildView(), finishView(), setDefaultOptions()
    }

Per far conoscere a Symfony la propria estensione e usarla, usare il
tag `form.type_extension`:

.. configuration-block::

    .. code-block:: yaml

        services:
            main.form.type.my_form_type_extension:
                class: Acme\MainBundle\Form\Type\MyFormTypeExtension
                tags:
                    - { name: form.type_extension, alias: field }

    .. code-block:: xml

        <service id="main.form.type.my_form_type_extension" class="Acme\MainBundle\Form\Type\MyFormTypeExtension">
            <tag name="form.type_extension" alias="field" />
        </service>

    .. code-block:: php

        $container
            ->register('main.form.type.my_form_type_extension', 'Acme\MainBundle\Form\Type\MyFormTypeExtension')
            ->addTag('form.type_extension', array('alias' => 'field'))
        ;

La chiave ``alias`` del tag è il tipo di campo a cui questa estensione va applicata.
Per esempio, per applicare l'estensione a qualsiasi campo, usare il valore
"field".

form.type_guesser
-----------------

**Scopo**: Aggiungere la propria logica per "indovinare" il tipo di form

Questo tag consente di aggiungere la propria logica al processo per
:ref:`indovinare<book-forms-field-guessing>` il form. Per impostazione predefinita, il form
viene indovinato dagli "indovini", in base ai meta-dati di validazione e ai meta-dati di Doctrine (se si usa Doctrine).

Per aggiungere i propri indovini, creare una classe che implementi l'interfaccia
:class:`Symfony\\Component\\Form\\FormTypeGuesserInterface`. Quindi, assegnare al
servizio il tag ``form.type_guesser`` (che non ha opzioni).

Per avere un'idea della classe, dare un'occhiata alla classe ``ValidatorTypeGuesser``
nel componente ``Form``.

kernel.cache_clearer
--------------------

**Scopo**: Registrare un servizio da richiamare durante la pulizia della cache

Cache clearing occurs whenever you call ``cache:clear`` command. If your
bundle caches files, you should add custom cache clearer for clearing those
files during the cache clearing process.

In order to register your custom cache clearer, first you must create a
service class::

    // src/Acme/MainBundle/Cache/MyClearer.php
    namespace Acme\MainBundle\Cache;

    use Symfony\Component\HttpKernel\CacheClearer\CacheClearerInterface;

    class MyClearer implements CacheClearerInterface
    {
        public function clear($cacheDir)
        {
            // clear your cache
        }

    }

Then register this class and tag it with ``kernel.cache:clearer``:

.. configuration-block::

    .. code-block:: yaml

        services:
            my_cache_clearer:
                class: Acme\MainBundle\Cache\MyClearer
                tags:
                    - { name: kernel.cache_clearer }

    .. code-block:: xml

        <service id="my_cache_clearer" class="Acme\MainBundle\Cache\MyClearer">
            <tag name="kernel.cache_clearer" />
        </service>

    .. code-block:: php

        $container
            ->register('my_cache_clearer', 'Acme\MainBundle\Cache\MyClearer')
            ->addTag('kernel.cache_clearer')
        ;

kernel.cache_warmer
-------------------

**Scopo**: Registrare un servizio da richiamare durante il processo di preparazione della cache

Ogni volta che si richiama il task ``cache:warmup`` o ``cache:clear``, la cache viene
preparata (a meno che non si passi ``--no-warmup`` a ``cache:clear``). Lo scopo è di
inizializzare ogni cache necessaria all'applicazione e prevenire un "cache hit",
cioè una generazione dinamica della cache, da parte del primo
utente.

Per registrare il proprio preparatore di cache, creare innanzitutto un servizio che implementi
l'interfaccia :class:`Symfony\\Component\\HttpKernel\\CacheWarmer\\CacheWarmerInterface`::

    // src/Acme/MainBundle/Cache/MyCustomWarmer.php
    namespace Acme\MainBundle\Cache;

    use Symfony\Component\HttpKernel\CacheWarmer\CacheWarmerInterface;

    class MyCustomWarmer implements CacheWarmerInterface
    {
        public function warmUp($cacheDir)
        {
            // fare qualcosa per preparare la cache
        }

        public function isOptional()
        {
            return true;
        }
    }

Il metodo ``isOptional`` deve resituire ``true`` se è possibile usare l'applicazione senza
richiamare il preparatore di cache. In Symfony 2.0, i preparatori facoltativi
vengono eseguiti ugualmente, quindi questa funzione non ha effetto.

Per registrare il proprio preparatore di cache, usare il tag ``kernel.cache_warmer``:

.. configuration-block::

    .. code-block:: yaml

        services:
            main.warmer.my_custom_warmer:
                class: Acme\MainBundle\Cache\MyCustomWarmer
                tags:
                    - { name: kernel.cache_warmer, priority: 0 }

    .. code-block:: xml

        <service id="main.warmer.my_custom_warmer" class="Acme\MainBundle\Cache\MyCustomWarmer">
            <tag name="kernel.cache_warmer" priority="0" />
        </service>

    .. code-block:: php

        $container
            ->register('main.warmer.my_custom_warmer', 'Acme\MainBundle\Cache\MyCustomWarmer')
            ->addTag('kernel.cache_warmer', array('priority' => 0))
        ;

Il valore ``priority`` è facoltativo ed è predefinito a 0. Questo valore può essere tra
-255 e 255 e i prepratori saranno eseguiti con un ordine basato sulla loro
priorità.

.. _dic-tags-kernel-event-listener:

kernel.event_listener
---------------------

**Scopo**: Ascoltare vari eventi/agganci in Symfony

Questo tag consente di agganciare le proprie classi al processo di Symfony, in vari
punti.

Per un esempio completo di questo ascoltatore, leggere la ricetta
:doc:`/cookbook/service_container/event_listener`.

Per altri esempi pratici di un ascoltatore del nucleo, vedere la ricetta
:doc:`/cookbook/request/mime_type`.

Riferimenti sugli ascoltatori del nucleo
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Quando si aggiungono i propri ascoltatori, potrebbe essere utile conoscere gli altri
ascoltatori del nucleo di Symfony e le loro priorità.

.. note::

    Tutti gli ascoltatori qui elencati potrebbero non ascoltare, a seconda di ambiente,
    impostazioni e bundle. Inoltre, bundle di terze parti potrebbero aggiungere altri
    ascoltatori, non elencati qui.

kernel.request
..............

+-------------------------------------------------------------------------------------------+-----------+
| Nome della classe dell'ascoltatore                                                        | Priorità  |
+-------------------------------------------------------------------------------------------+-----------+
| :class:`Symfony\\Component\\HttpKernel\\EventListener\\ProfilerListener`                  | 1024      |
+-------------------------------------------------------------------------------------------+-----------+
| :class:`Symfony\\Bundle\\FrameworkBundle\\EventListener\\TestSessionListener`             | 192       |
+-------------------------------------------------------------------------------------------+-----------+
| :class:`Symfony\\Bundle\\FrameworkBundle\\EventListener\\SessionListener`                 | 128       |
+-------------------------------------------------------------------------------------------+-----------+
| :class:`Symfony\\Component\\HttpKernel\\EventListener\\RouterListener`                    | 32        |
+-------------------------------------------------------------------------------------------+-----------+
| :class:`Symfony\\Component\\HttpKernel\\EventListener\\LocaleListener`                    | 16        |
+-------------------------------------------------------------------------------------------+-----------+
| :class:`Symfony\\Component\\Security\\Http\\Firewall`                                     | 8         |
+-------------------------------------------------------------------------------------------+-----------+

kernel.controller
.................

+-------------------------------------------------------------------------------------------+----------+
| Nome della classe dell'ascoltatore                                                        | Priorità |
+-------------------------------------------------------------------------------------------+----------+
| :class:`Symfony\\Bundle\\FrameworkBundle\\DataCollector\\RequestDataCollector`            | 0        |
+-------------------------------------------------------------------------------------------+----------+

kernel.response
...............

+-------------------------------------------------------------------------------------------+----------+
| Nome della classe dell'ascoltatore                                                        | Priorità |
+-------------------------------------------------------------------------------------------+----------+
| :class:`Symfony\\Component\\HttpKernel\\EventListener\\EsiListener`                       | 0        |
+-------------------------------------------------------------------------------------------+----------+
| :class:`Symfony\\Component\\HttpKernel\\EventListener\\ResponseListener`                  | 0        |
+-------------------------------------------------------------------------------------------+----------+
| :class:`Symfony\\Bundle\\SecurityBundle\\EventListener\\ResponseListener`                 | 0        |
+-------------------------------------------------------------------------------------------+----------+
| :class:`Symfony\\Component\\HttpKernel\\EventListener\\ProfilerListener`                  | -100     |
+-------------------------------------------------------------------------------------------+----------+
| :class:`Symfony\\Bundle\\FrameworkBundle\\EventListener\\TestSessionListener`             | -128     |
+-------------------------------------------------------------------------------------------+----------+
| :class:`Symfony\\Bundle\\WebProfilerBundle\\EventListener\\WebDebugToolbarListener`       | -128     |
+-------------------------------------------------------------------------------------------+----------+
| :class:`Symfony\\Component\\HttpKernel\\EventListener\\StreamedResponseListener`          | -1024    |
+-------------------------------------------------------------------------------------------+----------+

kernel.exception
................

+-------------------------------------------------------------------------------------------+----------+
| Nome della classe dell'ascoltatore                                                        | Priorità |
+-------------------------------------------------------------------------------------------+----------+
| :class:`Symfony\\Component\\HttpKernel\\EventListener\\ProfilerListener`                  | 0        |
+-------------------------------------------------------------------------------------------+----------+
| :class:`Symfony\\Component\\HttpKernel\\EventListener\\ExceptionListener`                 | -128     |
+-------------------------------------------------------------------------------------------+----------+

kernel.terminate
................

+-------------------------------------------------------------------------------------------+----------+
| Nome della classe dell'ascoltatore                                                        | Priorità |
+-------------------------------------------------------------------------------------------+----------+
| :class:`Symfony\\Bundle\\SwiftmailerBundle\\EventListener\\EmailSenderListener`           | 0        |
+-------------------------------------------------------------------------------------------+----------+

.. _dic-tags-kernel-event-subscriber:

kernel.event_subscriber
-----------------------

**Scopo**: Sottoscrivere un insieme di vari eventi/agganci in Symfony

.. versionadded:: 2.1
   La possibilità di aggiungere sottoscrittori di eventi del kernle è nuova nella 2.1.

Per abilitare un sottoscrittore personalizzato, aggiungerlo come normale servizio in una delle
configurazioni e assegnarli il tag ``kernel.event_subscriber``:

.. configuration-block::

    .. code-block:: yaml

        services:
            kernel.subscriber.your_subscriber_name:
                class: Nome\Pienamente\QUalificato\Classe\Subscriber
                tags:
                    - { name: kernel.event_subscriber }

    .. code-block:: xml

        <service id="kernel.subscriber.your_subscriber_name" class="Nome\Pienamente\QUalificato\Classe\Subscriber">
            <tag name="kernel.event_subscriber" />
        </service>

    .. code-block:: php

        $container
            ->register('kernel.subscriber.your_subscriber_name', 'Nome\Pienamente\QUalificato\Classe\Subscriber')
            ->addTag('kernel.event_subscriber')
        ;

.. note::

    Il servizio deve implementare l'inferfaccia
    :class:`Symfony\Component\EventDispatcher\EventSubscriberInterface`.

.. note::

    Se il servizio è creato da un factory, si **DEVE** impostare correttamente il parametro ``class``
    del tag, per poterlo far funzionare correttamente.

kernel.fragment_renderer
------------------------

**Scopo**: Aggiunta di una nuova strategia di resa del contenuto HTTP.

Per aggiungere una nuova strategia di resa, in aggiunta a quelle predefinite come
``EsiFragmentRenderer``, creare una classe che implementi
:class:`Symfony\\Component\\HttpKernel\\Fragment\\FragmentRendererInterface`,
registrarla come servizio, assegnando il tag ``kernel.fragment_renderer``.

.. _dic_tags-monolog:

monolog.logger
--------------

**Scopo**: Usare un canale di log personalizzato con Monolog

Monolog consente di condividere i suoi gestori tra vari canali di log.
Il servizio logger usa il canale ``app``, ma si può cambiare il canale
quando si inietta il logger in un servizio.

.. configuration-block::

    .. code-block:: yaml

        services:
            mio_servizio:
                class: Nome\Pienamente\QUalificato\Classe\Loader
                arguments: ["@logger"]
                tags:
                    - { name: monolog.logger, channel: acme }

    .. code-block:: xml

        <service id="mio_servizio" class="Nome\Pienamente\QUalificato\Classe\Loader">
            <argument type="service" id="logger" />
            <tag name="monolog.logger" channel="acme" />
        </service>

    .. code-block:: php

        $definition = new Definition('Nome\Pienamente\QUalificato\Classe\Loader', array(new Reference('logger'));
        $definition->addTag('monolog.logger', array('channel' => 'acme'));
        $container->register('mio_servizio', $definition);;

.. note::

    Questo funziona solo quando il servizio logger è un parametro del costruttore,
    non quando viene iniettato tramite setter.

.. _dic_tags-monolog-processor:

monolog.processor
-----------------

**Scopo**: Aggiungere un processore personalizzato per i log

Monolog consente di aggiungere processori nel logger o nei gestori, per aggiungere dati
extra nelle registrazioni. Un processore riceve la registrazione come parametro e
deve restituirlo dopo aver aggiunto degli extra nell'attributo ``extra`` della
registrazione.

Vediamo come usare ``IntrospectionProcessor`` per aggiungere nome del file,
riga, classe e metodo in cui il logger è stato fatto partire.

Si può aggiungere un processore globalmente:

.. configuration-block::

    .. code-block:: yaml

        services:
            mio_servizio:
                class: Monolog\Processor\IntrospectionProcessor
                tags:
                    - { name: monolog.processor }

    .. code-block:: xml

        <service id="mio_servizio" class="Monolog\Processor\IntrospectionProcessor">
            <tag name="monolog.processor" />
        </service>

    .. code-block:: php

        $definition = new Definition('Monolog\Processor\IntrospectionProcessor');
        $definition->addTag('monolog.processor');
        $container->register('mio_servizio', $definition);

.. tip::

    Se il proprio servizio non è richiamabile (usando ``__invoke``) si può aggiungere
    l'attributo ``method`` nel tag, per usare un metodo specifico.

Si può anche aggiungere un processore per un gestore specifico, usando l'attributo
``handler``:

.. configuration-block::

    .. code-block:: yaml

        services:
            mio_servizio:
                class: Monolog\Processor\IntrospectionProcessor
                tags:
                    - { name: monolog.processor, handler: firephp }

    .. code-block:: xml

        <service id="mio_servizio" class="Monolog\Processor\IntrospectionProcessor">
            <tag name="monolog.processor" handler="firephp" />
        </service>

    .. code-block:: php

        $definition = new Definition('Monolog\Processor\IntrospectionProcessor');
        $definition->addTag('monolog.processor', array('handler' => 'firephp');
        $container->register('mio_servizio', $definition);

Si può anche aggiungere un processore per uno specifico canale di log, usando
l'attributo ``channel``. Il seguente registrerà il processore solo per il canale di log
``security``, usato dal componente Security:

.. configuration-block::

    .. code-block:: yaml

        services:
            mio_servizio:
                class: Monolog\Processor\IntrospectionProcessor
                tags:
                    - { name: monolog.processor, channel: security }

    .. code-block:: xml

        <service id="mio_servizio" class="Monolog\Processor\IntrospectionProcessor">
            <tag name="monolog.processor" channel="security" />
        </service>

    .. code-block:: php

        $definition = new Definition('Monolog\Processor\IntrospectionProcessor');
        $definition->addTag('monolog.processor', array('channel' => 'security');
        $container->register('mio_servizio', $definition);

.. note::

    Non si può usare sia l'attributo ``handler`` che ``channel`` per lo stesso tag,
    perché i gestori sono condivisi tra tutti i canali.

routing.loader
--------------

**Scopo**: Registrare un servizio che carichi delle rotte

Per abilitare un caricatore di rotte personalizzato, aggiungerlo come servizio in
una configurazione e assegnargli il tag ``routing.loader``:

.. configuration-block::

    .. code-block:: yaml

        services:
            routing.loader.your_loader_name:
                class: Nome\Pienamente\QUalificato\Classe\Loader
                tags:
                    - { name: routing.loader }

    .. code-block:: xml

        <service id="routing.loader.your_loader_name" class="Nome\Pienamente\QUalificato\Classe\Loader">
            <tag name="routing.loader" />
        </service>

    .. code-block:: php

        $container
            ->register('routing.loader.your_loader_name', 'Nome\Pienamente\QUalificato\Classe\Loader')
            ->addTag('routing.loader')
        ;

Per maggiori informazioni, vedere :doc:`/cookbook/routing/custom_route_loader`.

security.remember_me_aware
--------------------------

**Scopo**: Consetire il "ricordami" nell'autenticazione

Questo tag è usato internamente per consentire il "ricordami" nell'autenticazione.
Se si ha un metodo di autenticazione personalizzato, in cui l'utente può essere
ricordato, occorre usare questo tag.

Se il proprio factory di autenticazione personalizzato estende
:class:`Symfony\\Bundle\\SecurityBundle\\DependencyInjection\\Security\\Factory\\AbstractFactory`
e il proprio ascoltatore di autenticazione personalizzato estende
:class:`Symfony\\Component\\Security\\Http\\Firewall\\AbstractAuthenticationListener`,
allora il proprio ascoltatore avrà automaticamente questo tag applicato e
funzionerà tutto in modo automatico.

security.voter
--------------

**Scopo**: Aggiungere un votante personalizzato alla logica di autorizzazione di Symfony

Quando si riciama ``isGranted`` nel contesto di sicurezza di Symfony, viene usato dietro
le quinte un sistema di "votanti", per determinare se l'utente possa accedere. Il tag
``security.voter`` consente di aggiungere il proprio votante personalizzato a tale sistema.

Per maggiori informazioni, leggere la ricetta :doc:`/cookbook/security/voters`.

swiftmailer.plugin
------------------

**Scopo**: Registrare un plugin di SwiftMailer

Se si usa (o si vuole creare) un plugin di SwiftMailer, lo si può registrare con
SwiftMailer creando un servizio per il plugin e assegnadogli il tag
``swiftmailer.plugin`` (che non ha opzioni).

Un plugin di SwiftMailer deve implementare l'interfaccia ``Swift_Events_EventListener``.
Per maggiori informazioni sui plugin, vedere la `documentazione dei plugin di SwiftMailer`_.

Molti plugin di SwiftMailer sono nel nucleo di Symfony e possono essere attivati tramite
varie configurazioni. Per dettagli, vedere :doc:`/reference/configuration/swiftmailer`.

templating.helper
-----------------

**Scopo**: Rendere i proprio servizi disponibili nei template PHP

Per abilitare un helper personalizzato per i template, aggiungerlo come normale servizio
in una configurazione, assegnarli il tag ``templating.helper`` e definire un attributo
``alias`` (l'helper sarà accessibile tramite tale alias nei
template):

.. configuration-block::

    .. code-block:: yaml

        services:
            templating.helper.il mio_helper:
                class: Nome\Pienamente\QUalificato\Classe\Helper
                tags:
                    - { name: templating.helper, alias: nome_alias }

    .. code-block:: xml

        <service id="templating.helper.il mio_helper" class="Nome\Pienamente\QUalificato\Classe\Helper">
            <tag name="templating.helper" alias="nome_alias" />
        </service>

    .. code-block:: php

        $container
            ->register('templating.helper.il mio_helper', 'Nome\Pienamente\QUalificato\Classe\Helper')
            ->addTag('templating.helper', array('alias' => 'nome_alias'))
        ;

translation.loader
------------------

**Scopo**: Registrare un servizio personalizzato che carichi delle traduzioni

Per impostazione predefinita, le traduzioni sono caricate dal filesystem in vari
formati (YAML, XLIFF, PHP, ecc). Se occorre caricare traduzioni da altre sorgenti,
creare una classe che implementi l'interfaccia
:class:`Symfony\\Component\\Translation\\Loader\\LoaderInterface`::

    // src/Acme/MainBundle/Translation/MyCustomLoader.php
    namespace Acme\MainBundle\Translation;

    use Symfony\Component\Translation\Loader\LoaderInterface;
    use Symfony\Component\Translation\MessageCatalogue;

    class MyCustomLoader implements LoaderInterface
    {
        public function load($resource, $locale, $domain = 'messages')
        {
            $catalogue = new MessageCatalogue($locale);

            // caricare in qualche modo le traduzioni dalla "risorsa"
            // quindi impostarle nel catalogo
            $catalogue->set('hello.world', 'Hello World!', $domain);

            return $catalogue;
        }
    }

Il proprio metodo ``load`` ha la responsabilità di restituire un
:Class:`Symfony\\Component\\Translation\\MessageCatalogue`.

Registrare il caricatore come servizio e assegnargli il tag ``translation.loader``:

.. configuration-block::

    .. code-block:: yaml

        services:
            main.translation.my_custom_loader:
                class: Acme\MainBundle\Translation\MyCustomLoader
                tags:
                    - { name: translation.loader, alias: bin }

    .. code-block:: xml

        <service id="main.translation.my_custom_loader" class="Acme\MainBundle\Translation\MyCustomLoader">
            <tag name="translation.loader" alias="bin" />
        </service>

    .. code-block:: php

        $container
            ->register('main.translation.my_custom_loader', 'Acme\MainBundle\Translation\MyCustomLoader')
            ->addTag('translation.loader', array('alias' => 'bin'))
        ;

L'opzione ``alias`` è obbligatoria e molto importante: definisce il "suffisso" del file
che sarà usato per i file risorsa che usano questo caricatore. Per esempio, si
supponga di avere un formato personalizzato ``bin``, da caricare.
Se si ha un file ``bin`` che contiene traduzioni in francese per il dominio ``messages``,
si potrebbe avere un file ``app/Resources/translations/messages.fr.bin``.

Quando Symfony prova a caricare il file ``bin``, passa il percorso del caricatore personalizzato
nel parametro ``$resource``. Si può quindi implementare la logica desiderata su tale file,
in modo da caricare le proprie traduzioni.

Se si caricano traduzioni da una base dati, occorrerà comunque un file risorsa,
ma potrebbe essere vuoto o contenere poche informazioni sul caricamento di tali
risorse dalla base dati. Il file è la chiave per far scattare il metodo
``load`` del proprio caricatore personalizzato.

.. _reference-dic-tags-twig-extension:

twig.extension
--------------

**Scopo**: Registrare un'estensione personalizzata di Twig

Per abilitare un'estensione di Twig, aggiungere un normale servizio in una
configurazione e assegnargli il tag ``twig.extension``:

.. configuration-block::

    .. code-block:: yaml

        services:
            twig.extension.your_extension_name:
                class: Nome\Pienamente\QUalificato\Classe\Extension
                tags:
                    - { name: twig.extension }

    .. code-block:: xml

        <service id="twig.extension.your_extension_name" class="Nome\Pienamente\QUalificato\Classe\Extension">
            <tag name="twig.extension" />
        </service>

    .. code-block:: php

        $container
            ->register('twig.extension.your_extension_name', 'Nome\Pienamente\QUalificato\Classe\Extension')
            ->addTag('twig.extension')
        ;

Per sapere come creare la classe estensione di Twig, vedere la
`documentazione di Twig`_ sull'argomento oppure leggere la ricetta
:doc:`/cookbook/templating/twig_extension`

Prima di scrivere la propria estensione, dare un'occhiata al
`repository ufficiale delle estensioni di Twig`_, che contiene molte estensioni utili.
Per esempio, ``Intl`` e il suo filtro ``localizeddate``, che formatta
una data in base al locale dell'utente. Anche aueste estensioni ufficiali di Twig
devono essere aggiunte come normali servizi:

.. configuration-block::

    .. code-block:: yaml

        services:
            twig.extension.intl:
                class: Twig_Extensions_Extension_Intl
                tags:
                    - { name: twig.extension }

    .. code-block:: xml

        <service id="twig.extension.intl" class="Twig_Extensions_Extension_Intl">
            <tag name="twig.extension" />
        </service>

    .. code-block:: php

        $container
            ->register('twig.extension.intl', 'Twig_Extensions_Extension_Intl')
            ->addTag('twig.extension')
        ;

twig.loader
-----------

**Scopo**: Registrare un servizio personalizzato che carica template Twig

Per impostazione predefinita, Symfony usa solo la classe `Twig Loader`_.
:class:`Symfony\\Bundle\\TwigBundle\\Loader\\FilesystemLoader`. Se si ha l'esigenza
di caricare template Twig da un'altra risorsa, si può creare un servizio per il
nuovo caricatore e assegnarli il tag ``twig.loader``:

.. configuration-block::

    .. code-block:: yaml

        services:
            acme.demo_bundle.loader.caricatore_twig:
                class: Acme\DemoBundle\Loader\CaricatoreTwig
                tags:
                    - { name: twig.loader }

    .. code-block:: xml

        <service id="acme.demo_bundle.loader.caricatore_twig" class="Acme\DemoBundle\Loader\CaricatoreTwig">
            <tag name="twig.loader" />
        </service>

    .. code-block:: php

        $container
            ->register('acme.demo_bundle.loader.caricatore_twig', 'Acme\DemoBundle\Loader\CaricatoreTwig')
            ->addTag('twig.loader')
        ;

validator.constraint_validator
------------------------------

**Scopo**: Creare il proprio vincolo di validazione personalizzato

Questo tag consente di creare e registrare i propri vincoli di validazione.
Per maggiori informazioni, leggere la ricetta :doc:`/cookbook/validation/custom_constraint`.

validator.initializer
---------------------

**Scopo**: Registrare un servizio che inizializzi gli oggetti prima della validazione

Questo tag fornisce un pezzo di funzionalità non comune, che consente di eseguire
alcune azioni su un oggetto prima che venga validato. Per esempio,
è usato da Doctrine per cercare tutti i dati di un oggetto caricati in modo pigro,
prima che venga validato. Senza questo, alcuni dati su un entità Doctrine apparirebbero
come mancanti durante la validazione, anche se non lo fossero
realmente.

Se si deve usare questo tag, fare una nuova classe che implementi l'interfaccia
:class:`Symfony\\Component\\Validator\\ObjectInitializerInterface`.
Quindi, assegnare il tag ``validator.initializer`` (che non ha opzioni).

Per un esempio, vedere la classe ``EntityInitializer`` dentro Doctrine Bridge.

.. _`documentazione di Twig`: http://twig.sensiolabs.org/doc/advanced.html#creating-an-extension
.. _`repository ufficiale delle estensioni di Twig`: https://github.com/fabpot/Twig-extensions
.. _`KernelEvents`: https://github.com/symfony/symfony/blob/2.2/src/Symfony/Component/HttpKernel/KernelEvents.php
.. _`documentazione dei plugin di SwiftMailer`: http://swiftmailer.org/docs/plugins.html
.. _`Twig Loader`: http://twig.sensiolabs.org/doc/api.html#loaders
