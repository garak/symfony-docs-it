I tag della dependency injection
================================

I tag della dependency injection sono piccole stringhe, che si possono applicare a un servizio
per "marcarlo" per essere usato in un modo speciale. Per esempio, se si ha un servizio
che si vuole registrare come ascoltatore di uno degli eventi del nucleo di Symfony,
lo si può marcare con il tag ``kernel.event_listener``.

Si possono approfondire i tag leggendo la sezione ":ref:`book-service-container-tags`"
del capitolo sul contenitore di servizi.

Di seguito si trovano informazioni su tutti i tag disponibili in Symfony. Potrebbero
esserci altri tag in alcuni bundle utilizzati, che non sono elencati qui.

+-----------------------------------+---------------------------------------------------------------------------+
| Nome tag                          | Utilizzo                                                                  |
+===================================+===========================================================================+
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
| `security.remember_me_aware`_     | Consentire il "ricordami" nell'autenticazione                             |
+-----------------------------------+---------------------------------------------------------------------------+
| `serializer.encoder`_             | Registrare un nuovo codificatore nel servizio ``serializer``              |
+-----------------------------------+---------------------------------------------------------------------------+
| `serializer.normalizer`_          | Registrare un nuovo normalizzatore nel servizio ``serializer``            |
+-----------------------------------+---------------------------------------------------------------------------+
| `swiftmailer.default.plugin`_     | Registrare un plugin di SwiftMailer                                       |
+-----------------------------------+---------------------------------------------------------------------------+
| `templating.helper`_              | Rendere il servizio disponibile nei template PHP                          |
+-----------------------------------+---------------------------------------------------------------------------+
| `translation.loader`_             | Registrare un servizio che carica traduzioni                              |
+-----------------------------------+---------------------------------------------------------------------------+
| `translation.extractor`_          | Registrare un servizio personalizzato che estrae messaggi di traduzione   |
+-----------------------------------+---------------------------------------------------------------------------+
| `translation.dumper`_             | Registrare un servizio personalizzato che esporta messaggi di traduzione  |
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

Un Factory worker è una classe che implementa ``Assetic\Factory\Worker\WorkerInterface``.
Il suo metodo ``process($asset)`` viene richiamato per ogni risorsa, dopo la creazione.
Si può modificare una risorsa o anche restituirne una nuova.

Per poter aggiungere un nuovo worker, creare una classe::

    use Assetic\Asset\AssetInterface;
    use Assetic\Factory\Worker\WorkerInterface;

    class MyWorker implements WorkerInterface
    {
        public function process(AssetInterface $asset)
        {
            // ... modificare $asset o restituirne uno nuovo
        }

    }

Quindi registrarla come servizio:

.. configuration-block::

    .. code-block:: yaml

        services:
            acme.my_worker:
                class: MyWorker
                tags:
                    - { name: assetic.factory_worker }

    .. code-block:: xml

        <?xml version="1.0" encoding="UTF-8" ?>
        <container xmlns="http://symfony.com/schema/dic/services"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://symfony.com/schema/dic/services http://symfony.com/schema/dic/services/services-1.0.xsd">

            <services>
                <service id="acme.my_worker" class="MyWorker>
                    <tag name="assetic.factory_worker" />
                </service>
            </services>
        </container>

    .. code-block:: php

        $container
            ->register('acme.my_worker', 'MyWorker')
            ->addTag('assetic.factory_worker')
        ;

assetic.filter
--------------

**Scopo**: Registrare un filtro

AsseticBundle usa questo tag per registrare filtri comuni. Lo si può usare
per registrare i propri filtri.

Occorre prima di tutto creare un filtro::

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

Definire quindi un servizio:

.. configuration-block::

    .. code-block:: yaml

        services:
            acme.my_filter:
                class: MyFilter
                tags:
                    - { name: assetic.filter, alias: my_filter }

    .. code-block:: xml

        <?xml version="1.0" encoding="UTF-8" ?>
        <container xmlns="http://symfony.com/schema/dic/services"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://symfony.com/schema/dic/services http://symfony.com/schema/dic/services/services-1.0.xsd">

            <services>
                <service id="acme.my_filter" class="MyFilter">
                    <tag name="assetic.filter" alias="my_filter" />
                </service>
            </services>
        </container>

    .. code-block:: php

        $container
            ->register('acme.my_filter', 'MyFilter')
            ->addTag('assetic.filter', array('alias' => 'my_filter'))
        ;

Infine applicare il filtro:

.. code-block:: jinja

    {% javascripts
        '@AcmeBaseBundle/Resources/public/js/global.js'
        filter='my_filter'
    %}
        <script src="{{ asset_url }}"></script>
    {% endjavascripts %}

Si può anche applicare un filtro tramite l'opzione di configurazione ``assetic.filters.my_filter.apply_to``,
come spiegato in :doc:`/cookbook/assetic/apply_to_option`.
Per poterlo fare, si deve definire il servizio per il filtro in un file xml a parte
e puntare al percorso di tale file, tramite la chiave di configurazione
``assetic.filters.my_filter.resource``.

assetic.formula_loader
----------------------

**Scopo**: Aggiungere un formula loader al gestore di risorse corrente

Un formula loader è una classe che implementa l'interfaccia
``Assetic\\Factory\Loader\\FormulaLoaderInterface``. Tale classe
è responsabile del caricamento di risorse di un certo tipo (per
esempio, template Twig). Assetic dispone di loader per template PHP e Twig.

Un attributo ``alias`` definisce il nome del loader.

assetic.formula_resource
------------------------

**Scopo**: Aggiungere una risorsa al gestore di risorse corrente

Una risorsa è qualcosa che possa essere caricato da un formula loader. Per esempio,
i template Twig sono risorse.

assetic.templating.php
----------------------

**Scopo**: Rimuovere questo servizio se i template PHP sono disabilitati

Il servizio sarà rimosso dal contenitore, se la sezione 
``framework.templating.engines`` non contiene php.

assetic.templating.twig
-----------------------

**Scopo**: Rimuovere questo servizio se i template Twig sono disabilitati

Il servizio sarà rimosso dal contenitore, se la sezione 
``framework.templating.engines`` non contiene twig.

data_collector
--------------

**Scopo**: creare una classe che raccolga dati personalizzati per il profilatore

Per dettagli su come creare i propri insiemi di dati, leggere la ricetta
:doc:`/cookbook/profiler/data_collector`.

doctrine.event_listener
-----------------------

**Scopo**: Aggiungere un ascoltatore di eventi Doctrine

Per dettagli su come creare ascoltatori di eventi, leggere la ricetta
:doc:`/cookbook/doctrine/event_listeners_subscribers`.

doctrine.event_subscriber
-------------------------

**Scopo**: Aggiungere un sottoscrittore di eventi Doctrine

Per dettagli su come creare sottoscrittori di eventi, leggere la ricetta
:doc:`/cookbook/doctrine/event_listeners_subscribers`.

.. _dic-tags-form-type:

form.type
---------

**Scopo**: Creare un tipo di campo personalizzato per i form

Per dettagli su come creare un tipo di campo, leggere la ricetta
:doc:`/cookbook/form/create_custom_field_type`.

form.type_extension
-------------------

**Scopo**: Creare un "form extension" personalizzato

Le estensioni dei form sono un modo per portare un "aggancio" nella creazione di qualsiasi
campo di un form. Per esempio, l'aggiunta di un token per il CSRF si fa tramite
un'estensione del form
(:class:`Symfony\\Component\\Form\\Extension\\Csrf\\Type\\FormTypeCsrfExtension`).

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

        <?xml version="1.0" encoding="UTF-8" ?>
        <container xmlns="http://symfony.com/schema/dic/services"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://symfony.com/schema/dic/services http://symfony.com/schema/dic/services/services-1.0.xsd">

            <services>
                <service
                    id="main.form.type.my_form_type_extension"
                    class="Acme\MainBundle\Form\Type\MyFormTypeExtension">

                    <tag name="form.type_extension" alias="field" />
                </service>
            </services>
        </container>

    .. code-block:: php

        $container
            ->register(
                'main.form.type.my_form_type_extension',
                'Acme\MainBundle\Form\Type\MyFormTypeExtension'
            )
            ->addTag('form.type_extension', array('alias' => 'field'))
        ;

La chiave ``alias`` del tag è il tipo di campo a cui questa estensione va applicata.
Per esempio, per applicare l'estensione a qualsiasi campo, usare il valore
"field".

.. _reference-dic-type_guesser:

form.type_guesser
-----------------

**Scopo**: Aggiungere la propria logica per "indovinare" il tipo di form

Questo tag consente di aggiungere la propria logica al processo per :ref:`indovinare <book-forms-field-guessing>` il form.
Per impostazione predefinita, il form viene indovinato dagli "indovini", in base ai metadati
di validazione e ai metadati di Doctrine (se si usa Doctrine) o di Propel
(se si usa Propel).

.. seealso::

    Per sapere come aggiungere i propri indovini, vedere
    :doc:`/components/form/type_guesser`.

kernel.cache_clearer
--------------------

**Scopo**: Registrare un servizio da richiamare durante la pulizia della
cache

La pulizia della cache avviene a ogni chiamata del comando ``cache:clear``. Se un
bundle mette dei file in cache, si dovrebbe aggiungere un pulitore personalizzato, per
pulirli durante il processo di pulizia della cache.

Per registrare un pulitore personalizzato, occorre innanzitutto creare
un servizio::

    // src/Acme/MainBundle/Cache/MyClearer.php
    namespace Acme\MainBundle\Cache;

    use Symfony\Component\HttpKernel\CacheClearer\CacheClearerInterface;

    class MyClearer implements CacheClearerInterface
    {
        public function clear($cacheDir)
        {
            // pulire i propri file dalla cache
        }

    }

Quindi registrare la classe e assegnarle il tag ``kernel.cache_clearer``:

.. configuration-block::

    .. code-block:: yaml

        services:
            mio_pulitore:
                class: Acme\MainBundle\Cache\MioPulitore
                tags:
                    - { name: kernel.cache_clearer }

    .. code-block:: xml

        <?xml version="1.0" encoding="UTF-8" ?>
        <container xmlns="http://symfony.com/schema/dic/services"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://symfony.com/schema/dic/services http://symfony.com/schema/dic/services/services-1.0.xsd">

            <services>
                <service id="mio_pulitore" class="Acme\MainBundle\Cache\MioPulitore">
                    <tag name="kernel.cache_clearer" />
                </service>
            </services>
        </container>

    .. code-block:: php

        $container
            ->register('mio_pulitore', 'Acme\MainBundle\Cache\MioPulitore')
            ->addTag('kernel.cache_clearer')
        ;

kernel.cache_warmer
-------------------

**Scopo**: Registrare un servizio da richiamare durante il processo di preparazione della
cache

Ogni volta che si richiama il task ``cache:warmup`` o ``cache:clear``, la cache viene
preparata (a meno che non si passi ``--no-warmup`` a ``cache:clear``). Questo accade anche
durante la gestione della richiesta, in mancanza di un precedente comando. Lo scopo è di
inizializzare ogni cache necessaria all'applicazione e prevenire un "cache hit",
cioè una generazione dinamica della cache, da parte del primo
utente.

Per registrare un preparatore di cache, creare innanzitutto un servizio che implementi
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

Il metodo ``isOptional`` deve restituire ``true`` se è possibile usare l'applicazione senza
richiamare il preparatore di cache. In Symfony, i preparatori facoltativi
vengono eseguiti ugualmente (lo si può cambiare, usando l'opzione
``--no-optional-warmers`` durante l'esecuzione del comando).

Per registrare un preparatore di cache, usare il tag ``kernel.cache_warmer``:


.. configuration-block::

    .. code-block:: yaml

        services:
            main.warmer.mio_preparatore:
                class: Acme\MainBundle\Cache\MioPreparatore
                tags:
                    - { name: kernel.cache_warmer, priority: 0 }

    .. code-block:: xml

        <?xml version="1.0" encoding="UTF-8" ?>
        <container xmlns="http://symfony.com/schema/dic/services"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://symfony.com/schema/dic/services http://symfony.com/schema/dic/services/services-1.0.xsd">

            <services>
                <service id="main.warmer.mio_preparatore" 
                    class="Acme\MainBundle\Cache\MioPreparatore"
                >
                    <tag name="kernel.cache_warmer" priority="0" />
                </service>
            </services>
        </container>

    .. code-block:: php

        $container
            ->register('main.warmer.mio_preparatore', 'Acme\MainBundle\Cache\MioPreparatore')
            ->addTag('kernel.cache_warmer', array('priority' => 0))
        ;

.. note::

    Il valore ``priority`` è facoltativo ed è predefinito a 0. I preparatori saranno
    eseguiti con un ordine basato sulla loro priorità.

Preparatori di cache del nucleo
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

+-------------------------------------------------------------------------------------------+-----------+
| Nome della classe del preparatore                                                         | Priorità  |
+===========================================================================================+===========+
| :class:`Symfony\\Bundle\\FrameworkBundle\\CacheWarmer\\TemplatePathsCacheWarmer`          | 20        |
+-------------------------------------------------------------------------------------------+-----------+
| :class:`Symfony\\Bundle\\FrameworkBundle\\CacheWarmer\\RouterCacheWarmer`                 | 0         |
+-------------------------------------------------------------------------------------------+-----------+
| :class:`Symfony\\Bundle\\TwigBundle\\CacheWarmer\\TemplateCacheCacheWarmer`               | 0         |
+-------------------------------------------------------------------------------------------+-----------+

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

Per un elenco di ascoltatori associati con ciascun evento del kernel, si veda il
:doc:`riferimento agli eventi di Symfony </reference/events>`.

.. _dic-tags-kernel-event-subscriber:

kernel.event_subscriber
-----------------------

**Scopo**: Sottoscrivere un insieme di vari eventi/agganci in Symfony

Per abilitare un sottoscrittore personalizzato, aggiungerlo come normale servizio in una delle
configurazioni e assegnarli il tag ``kernel.event_subscriber``:

.. configuration-block::

    .. code-block:: yaml

        services:
            kernel.subscriber.nome_sottoscrittore:
                class: Nome\Pienamente\Qualificato\Classe\Subscriber
                tags:
                    - { name: kernel.event_subscriber }

    .. code-block:: xml

        <?xml version="1.0" encoding="UTF-8" ?>
        <container xmlns="http://symfony.com/schema/dic/services"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://symfony.com/schema/dic/services http://symfony.com/schema/dic/services/services-1.0.xsd">

            <services>
                <service
                    id="kernel.subscriber.nome_sottoscrittore"
                    class="Nome\Pienamente\Qualificato\Classe\Subscriber">

                    <tag name="kernel.event_subscriber" />
                </service>
            </services>
        </container>

    .. code-block:: php

        $container
            ->register(
                'kernel.subscriber.nome_sottoscrittore', 
                'Nome\Pienamente\Qualificato\Classe\Subscriber'
            )
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
                class: Nome\Pienamente\Qualificato\Classe\Loader
                arguments: ["@logger"]
                tags:
                    - { name: monolog.logger, channel: acme }

    .. code-block:: xml

        <?xml version="1.0" encoding="UTF-8" ?>
        <container xmlns="http://symfony.com/schema/dic/services"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://symfony.com/schema/dic/services http://symfony.com/schema/dic/services/services-1.0.xsd">

            <services>
                <service id="mio_servizio" class="Nome\Pienamente\Qualificato\Classe\Loader">
                    <argument type="service" id="logger" />
                    <tag name="monolog.logger" channel="acme" />
                </service>
            </services>
        </container>

    .. code-block:: php

        $definition = new Definition('Nome\Pienamente\Qualificato\Classe\Loader', array(
            new Reference('logger')
        );
        $definition->addTag('monolog.logger', array('channel' => 'acme'));
        $container->register('mio_servizio', $definition);;

.. tip::

    Se si usa MonologBundle 2.4 o successivi, si possono configurare canali personalizzati
    nella configurazione e recuperare il corrispondente servizio logger direttamente dal
    contenitore di servizi (vedere :ref:`cookbook-monolog-channels-config`).

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

        <?xml version="1.0" encoding="UTF-8" ?>
        <container xmlns="http://symfony.com/schema/dic/services"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://symfony.com/schema/dic/services http://symfony.com/schema/dic/services/services-1.0.xsd">

            <services>
                <service id="mio_servizio" class="Monolog\Processor\IntrospectionProcessor">
                    <tag name="monolog.processor" />
                </service>
            </services>
        </container>

    .. code-block:: php

        $container
            ->register('mio_servizio', 'Monolog\Processor\IntrospectionProcessor')
            ->addTag('monolog.processor')
        ;

.. tip::

    Se il servizio non è richiamabile (usando ``__invoke``), si può aggiungere
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

        <?xml version="1.0" encoding="UTF-8" ?>
        <container xmlns="http://symfony.com/schema/dic/services"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://symfony.com/schema/dic/services http://symfony.com/schema/dic/services/services-1.0.xsd">

            <services>
                <service id="mio_servizio" class="Monolog\Processor\IntrospectionProcessor">
                    <tag name="monolog.processor" handler="firephp" />
                </service>
            </services>
        </container>

    .. code-block:: php

        $container
            ->register('mio_servizio', 'Monolog\Processor\IntrospectionProcessor')
            ->addTag('monolog.processor', array('handler' => 'firephp'))
        ;

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

        <?xml version="1.0" encoding="UTF-8" ?>
        <container xmlns="http://symfony.com/schema/dic/services"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://symfony.com/schema/dic/services http://symfony.com/schema/dic/services/services-1.0.xsd">

            <services>
                <service id="mio_servizio" class="Monolog\Processor\IntrospectionProcessor">
                    <tag name="monolog.processor" channel="security" />
                </service>
            </services>
        </container>

    .. code-block:: php

        $container
            ->register('mio_servizio', 'Monolog\Processor\IntrospectionProcessor')
            ->addTag('monolog.processor', array('channel' => 'security'))
        ;

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
            routing.loader.nome_caricatore:
                class: Nome\Pienamente\Qualificato\Classe\Caricatore
                tags:
                    - { name: routing.loader }

    .. code-block:: xml

        <?xml version="1.0" encoding="UTF-8" ?>
        <container xmlns="http://symfony.com/schema/dic/services"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://symfony.com/schema/dic/services http://symfony.com/schema/dic/services/services-1.0.xsd">

            <services>
                <service
                    id="routing.loader.nome_caricatore"
                    class="Nome\Pienamente\Qualificato\Classe\Caricatore">

                    <tag name="routing.loader" />
                </service>
            </services>
        </container>

    .. code-block:: php

        $container
            ->register('routing.loader.nome_caricatore', 'Nome\Pienamente\Qualificato\Classe\Caricatore')
            ->addTag('routing.loader')
        ;

Per maggiori informazioni, vedere :doc:`/cookbook/routing/custom_route_loader`.

security.remember_me_aware
--------------------------

**Scopo**: Consentire il "ricordami" nell'autenticazione

Questo tag è usato internamente per consentire il "ricordami" nell'autenticazione.
Se si ha un metodo di autenticazione personalizzato, in cui l'utente può essere
ricordato, occorre usare questo tag.

Se il factory di autenticazione personalizzato estende
:class:`Symfony\\Bundle\\SecurityBundle\\DependencyInjection\\Security\\Factory\\AbstractFactory`
e l'ascoltatore di autenticazione personalizzato estende
:class:`Symfony\\Component\\Security\\Http\\Firewall\\AbstractAuthenticationListener`,
allora l'ascoltatore avrà automaticamente questo tag applicato e
funzionerà tutto in modo automatico.

security.voter
--------------

**Scopo**: Aggiungere un votante personalizzato alla logica di autorizzazione di Symfony

Quando si richiama ``isGranted`` nel contesto di sicurezza di Symfony, viene usato dietro
le quinte un sistema di "votanti", per determinare se l'utente possa accedere. Il tag
``security.voter`` consente di aggiungere un votante personalizzato a tale sistema.

Per maggiori informazioni, leggere la ricetta :doc:`/cookbook/security/voters`.

.. _reference-dic-tags-serializer-encoder:

serializer.encoder
------------------

**Scopo**: Registrare un nuovo codificatore nel servizio ``serializer``

La classe con il tag deve implementare :class:`Symfony\\Component\\Serializer\\Encoder\\EncoderInterface`
e :class:`Symfony\\Component\\Serializer\\Encoder\\DecoderInterface`.

Per maggiori dettagli, vedere :doc:`/cookbook/serializer`.

.. _reference-dic-tags-serializer-normalizer:

serializer.normalizer
---------------------

**Scopo**: Registrare un nuovo normalizzatore nel servizio ``serializer``

La classe con il tag deve implementare :class:`Symfony\\Component\\Serializer\\Normalizer\\NormalizerInterface`
e :class:`Symfony\\Component\\Serializer\\Normalizer\\DenormalizerInterface`.

Per maggiori dettagli, vedere :doc:`/cookbook/serializer`.

swiftmailer.default.plugin
--------------------------

**Scopo**: Registrare un plugin di SwiftMailer

Se si usa (o si vuole creare) un plugin di SwiftMailer, lo si può registrare con
SwiftMailer creando un servizio per il plugin e assegnandogli il tag
``swiftmailer.default.plugin`` (che non ha opzioni).

.. note::

    In questo tag, ``default`` è il nome del mailer. Se si hanno più
    mailer configurati o se per qualche motivo è stato cambiato il nome del mailer
    predefinito, anche in questo tag il nome va cambiato di
    conseguenza.

Un plugin di SwiftMailer deve implementare l'interfaccia ``Swift_Events_EventListener``.
Per maggiori informazioni sui plugin, vedere la `documentazione dei plugin di SwiftMailer`_.

Molti plugin di SwiftMailer sono nel nucleo di Symfony e possono essere attivati tramite
varie configurazioni. Per dettagli, vedere :doc:`/reference/configuration/swiftmailer`.

templating.helper
-----------------

**Scopo**: Rendere dei servizi disponibili nei template PHP

Per abilitare un aiutante personalizzato per i template, aggiungerlo come normale servizio
in una configurazione, assegnarli il tag ``templating.helper`` e definire un attributo
``alias`` (l'aiutante sarà accessibile tramite tale alias nei
template):

.. configuration-block::

    .. code-block:: yaml

        services:
            templating.helper.mio_aiutante:
                class: Nome\Pienamente\Qualificato\Classe\Aiutante
                tags:
                    - { name: templating.helper, alias: nome_alias }

    .. code-block:: xml

        <?xml version="1.0" encoding="UTF-8" ?>
        <container xmlns="http://symfony.com/schema/dic/services"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://symfony.com/schema/dic/services http://symfony.com/schema/dic/services/services-1.0.xsd">

            <services>
                <service
                    id="templating.helper.mio_aiutante"
                    class="Nome\Pienamente\Qualificato\Classe\Aiutante">

                    <tag name="templating.helper" alias="nome_alias" />
                </service>
            </services>
        </container>

    .. code-block:: php

        $container
            ->register('templating.helper.mio_aiutante', 'Nome\Pienamente\Qualificato\Classe\Aiutante')
            ->addTag('templating.helper', array('alias' => 'nome_alias'))
        ;

.. _dic-tags-translation-loader:

translation.loader
------------------

**Scopo**: Registrare un servizio personalizzato che carichi delle traduzioni

Per impostazione predefinita, le traduzioni sono caricate dal filesystem in vari
formati (YAML, XLIFF, PHP, ecc).

.. seealso::

    Si veda come :ref:`caricare formati personalizzati <components-translation-custom-loader>`,
    nella sezione nei componenti.

Registrare il caricatore come servizio e assegnargli il tag ``translation.loader``:

.. configuration-block::

    .. code-block:: yaml

        services:
            main.translation.mio_caricatore:
                class: Acme\MainBundle\Translation\MioCaricatore
                tags:
                    - { name: translation.loader, alias: bin }

    .. code-block:: xml

        <?xml version="1.0" encoding="UTF-8" ?>
        <container xmlns="http://symfony.com/schema/dic/services"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://symfony.com/schema/dic/services http://symfony.com/schema/dic/services/services-1.0.xsd">

            <services>
                <service
                    id="main.translation.mio_caricatore"
                    class="Acme\MainBundle\Translation\MioCaricatore">

                    <tag name="translation.loader" alias="bin" />
                </service>
            </services>
        </container>

    .. code-block:: php

        $container
            ->register(
                'main.translation.mio_caricatore',
                'Acme\MainBundle\Translation\MioCaricatore'
            )
            ->addTag('translation.loader', array('alias' => 'bin'))
        ;

L'opzione ``alias`` è obbligatoria e molto importante: definisce il "suffisso" del file
che sarà usato per i file risorsa che usano questo caricatore. Per esempio, si
supponga di avere un formato personalizzato ``bin``, da caricare.
Se si ha un file ``bin`` che contiene traduzioni in francese per il dominio
``messages``, si potrebbe avere un file
``app/Resources/translations/messages.fr.bin``.

Quando Symfony prova a caricare il file ``bin``, passa il percorso del caricatore personalizzato
nel parametro ``$resource``. Si può quindi implementare la logica desiderata su tale file,
in modo da caricare le proprie traduzioni.

Se si caricano traduzioni da una base dati, occorrerà comunque un file risorsa,
ma potrebbe essere vuoto o contenere poche informazioni sul caricamento di tali
risorse dalla base dati. Il file è la chiave per far scattare il metodo
``load`` del caricatore personalizzato.

translation.extractor
---------------------

**Scopo**: Registrare un servizio personalizzato che estragga messaggi da un
file

.. versionadded:: 2.1
   La possibilità di aggiungere estrattori di messaggi è nuova in Symfony 2.1.

Quando si esegue il comando ``translation:update``, esso usa degli estrattori per
estrarre messaggi di traduzione da un file. Per impostazione predefinita, Symfony
ha un :class:`Symfony\\Bridge\\Twig\\Translation\\TwigExtractor` e un
:class:`Symfony\\Bundle\\FrameworkBundle\\Translation\\PhpExtractor`, che
aiutano a trovare ed estrarre chiavi di traduzione da template Twig e file PHP.

Si può creare un estrattore, creando una classe che implementi
:class:`Symfony\\Component\\Translation\\Extractor\\ExtractorInterface` e
assegnando al servizio il tag ``translation.extractor``. Il tag ha un'opzione
obbligatoria: ``alias``, che definisce il nome dell'estrattore::

    // src/Acme/DemoBundle/Translation/PippoExtractor.php
    namespace Acme\DemoBundle\Translation;

    use Symfony\Component\Translation\Extractor\ExtractorInterface;
    use Symfony\Component\Translation\MessageCatalogue;

    class PippoExtractor implements ExtractorInterface
    {
        protected $prefix;

        /**
         * Estrae messaggi di traduzione da una cartella di template al catalogo.
         */
        public function extract($directory, MessageCatalogue $catalogue)
        {
            // ...
        }

        /**
         * Imposta il prefisso da usare per i nuovi messaggi trovati.
         */
        public function setPrefix($prefix)
        {
            $this->prefix = $prefix;
        }
    }

.. configuration-block::

    .. code-block:: yaml

        services:
            acme_demo.translation.extractor.foo:
                class: Acme\DemoBundle\Translation\PippoExtractor
                tags:
                    - { name: translation.extractor, alias: foo }

    .. code-block:: xml

        <?xml version="1.0" encoding="UTF-8" ?>
        <container xmlns="http://symfony.com/schema/dic/services"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://symfony.com/schema/dic/services http://symfony.com/schema/dic/services/services-1.0.xsd">

            <services>
                <service
                    id="acme_demo.translation.extractor.foo"
                    class="Acme\DemoBundle\Translation\PippoExtractor">

                    <tag name="translation.extractor" alias="foo" />
                </service>
            </services>
        </container>

    .. code-block:: php

        $container->register(
            'acme_demo.translation.extractor.pippo',
            'Acme\DemoBundle\Translation\PippoExtractor'
        )
            ->addTag('translation.extractor', array('alias' => 'pippo'));

translation.dumper
------------------

**Scopo**: Registrare un servizio personalizzato che esporti messaggi in un file

.. versionadded:: 2.1
   La possibilità di aggiungere esportatori di messaggi è nuova in Symfony 2.1.

Dopo che un `Extractor <translation.extractor>`_ ha estratto tutti i messaggi dai
template, vengono eseguiti gli esportatori, per esportare i messaggi in un file di
traduzione in uno specifico formato.

Symfony dispone di diversi esportatori:

* :class:`Symfony\\Component\\Translation\\Dumper\\CsvFileDumper`
* :class:`Symfony\\Component\\Translation\\Dumper\\IcuResFileDumper`
* :class:`Symfony\\Component\\Translation\\Dumper\\IniFileDumper`
* :class:`Symfony\\Component\\Translation\\Dumper\\MoFileDumper`
* :class:`Symfony\\Component\\Translation\\Dumper\\PoFileDumper`
* :class:`Symfony\\Component\\Translation\\Dumper\\QtFileDumper`
* :class:`Symfony\\Component\\Translation\\Dumper\\XliffFileDumper`
* :class:`Symfony\\Component\\Translation\\Dumper\\YamlFileDumper`

Si può creare un estrattore, estendendo
:class:`Symfony\\Component\\Translation\\Dumper\\FileDumper` o implementando
:class:`Symfony\\Component\\Translation\\Dumper\\DumperInterface` e assegnado al
servizio il tag ``translation.dumper``. Il tag ha un'unica opzione: ``alias``
È il nome usato per determinare quale esportatore va usato.

.. configuration-block::

    .. code-block:: yaml

        services:
            acme_demo.translation.dumper.json:
                class: Acme\DemoBundle\Translation\JsonFileDumper
                tags:
                    - { name: translation.dumper, alias: json }

    .. code-block:: xml

        <?xml version="1.0" encoding="UTF-8" ?>
        <container xmlns="http://symfony.com/schema/dic/services"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://symfony.com/schema/dic/services http://symfony.com/schema/dic/services/services-1.0.xsd">

            <services>
                <service
                    id="acme_demo.translation.dumper.json"
                    class="Acme\DemoBundle\Translation\JsonFileDumper">

                    <tag name="translation.dumper" alias="json" />
                </service>
            </services>
        </container>

    .. code-block:: php

        $container->register(
            'acme_demo.translation.dumper.json',
            'Acme\DemoBundle\Translation\JsonFileDumper'
        )
            ->addTag('translation.dumper', array('alias' => 'json'));

.. seealso::

    Si veda come :ref:`esportare in formati personalizzati <components-translation-custom-dumper>`
    nella sezione dei componenti.

.. _reference-dic-tags-twig-extension:

twig.extension
--------------

**Scopo**: Registrare un'estensione personalizzata di Twig

Per abilitare un'estensione di Twig, aggiungere un normale servizio in una
configurazione e assegnargli il tag ``twig.extension``:

.. configuration-block::

    .. code-block:: yaml

        services:
            twig.extension.nome_estensione:
                class: Nome\Pienamente\Qualificato\Classe\Extension
                tags:
                    - { name: twig.extension }

    .. code-block:: xml

        <?xml version="1.0" encoding="UTF-8" ?>
        <container xmlns="http://symfony.com/schema/dic/services"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://symfony.com/schema/dic/services http://symfony.com/schema/dic/services/services-1.0.xsd">

            <services>
                <service
                    id="twig.extension.nome_estensione"
                    class="Nome\Pienamente\Qualificato\Classe\Extension">

                    <tag name="twig.extension" />
                </service>
            </services>
        </container>

    .. code-block:: php

        $container
            ->register(
                'twig.extension.nome_estensione',
                'Nome\Pienamente\Qualificato\Classe\Extension'
            )
            ->addTag('twig.extension')
        ;

Per sapere come creare la classe estensione di Twig, vedere la
`documentazione di Twig`_ sull'argomento oppure leggere la ricetta
:doc:`/cookbook/templating/twig_extension`

Prima di scrivere la propria estensione, dare un'occhiata al
`repository ufficiale delle estensioni di Twig`_, che contiene molte estensioni utili.
Per esempio, ``Intl`` e il suo filtro ``localizeddate``, che formatta
una data in base al locale dell'utente. Anche queste estensioni ufficiali di Twig
devono essere aggiunte come normali servizi:

.. configuration-block::

    .. code-block:: yaml

        services:
            twig.extension.intl:
                class: Twig_Extensions_Extension_Intl
                tags:
                    - { name: twig.extension }

    .. code-block:: xml

        <?xml version="1.0" encoding="UTF-8" ?>
        <container xmlns="http://symfony.com/schema/dic/services"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://symfony.com/schema/dic/services http://symfony.com/schema/dic/services/services-1.0.xsd">

            <services>
                <service id="twig.extension.intl" class="Twig_Extensions_Extension_Intl">
                    <tag name="twig.extension" />
                </service>
            </services>
        </container>

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

        <?xml version="1.0" encoding="UTF-8" ?>
        <container xmlns="http://symfony.com/schema/dic/services"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://symfony.com/schema/dic/services http://symfony.com/schema/dic/services/services-1.0.xsd">

            <services>
                <service
                    id="acme.demo_bundle.loader.caricatore_twig"
                    class="Acme\DemoBundle\Loader\CaricatoreTwig">

                    <tag name="twig.loader" />
                </service>
            </services>
        </container>

    .. code-block:: php

        $container
            ->register(
                'acme.demo_bundle.loader.caricatore_twig',
                'Acme\DemoBundle\Loader\CaricatoreTwig'
            )
            ->addTag('twig.loader')
        ;

validator.constraint_validator
------------------------------

**Scopo**: Creare un vincolo di validazione personalizzato

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

Per un esempio, vedere la classe ``EntityInitializer`` dentro il bridge di
Doctrine.

.. _`documentazione di Twig`: http://twig.sensiolabs.org/doc/advanced.html#creating-an-extension
.. _`repository ufficiale delle estensioni di Twig`: https://github.com/fabpot/Twig-extensions
.. _`documentazione dei plugin di SwiftMailer`: http://swiftmailer.org/docs/plugins.html
.. _`Twig Loader`: http://twig.sensiolabs.org/doc/api.html#loaders
