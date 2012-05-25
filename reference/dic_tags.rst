I tag della dependency injection
================================

I tag della dependency injection sono piccole stringhe, che si possono applicare a un servizio
per "marcarlo" per essere usato in un modo speciale. Per esempio, se si ha un servizio
che si vuole registrare come ascoltatore di uno degli eventi del nucleo di Symfony,
lo si può marcare con il tag ``kernel.event_listener``.

Di seguito si trovano informazioni su tutti i tag disponibili in Symfony2:

+-----------------------------------+---------------------------------------------------------------------------+
| Nome tag                          | Utilizzo                                                                  |
+-----------------------------------+---------------------------------------------------------------------------+
| `data_collector`_                 | Creare una classe che raccolga dati personalizzati per il profilatore     |
+-----------------------------------+---------------------------------------------------------------------------+
| `form.type`_                      | Creare un tipo di campo personalizzato per i form                         |
+-----------------------------------+---------------------------------------------------------------------------+
| `form.type_extension`_            | Creare un "form extension" personalizzato                                 |
+-----------------------------------+---------------------------------------------------------------------------+
| `form.type_guesser`_              | Aggiungere logica per "form type guessing"                                |
+-----------------------------------+---------------------------------------------------------------------------+
| `kernel.cache_warmer`_            | Registrareu un servizio da richiamare durante il cache warming            |
+-----------------------------------+---------------------------------------------------------------------------+
| `kernel.event_listener`_          | Ascoltare diversi eventi/hook in Symfony                               |
+-----------------------------------+---------------------------------------------------------------------------+
| `monolog.logger`_                 | Log con un canale di log personalizzato                                     |
+-----------------------------------+---------------------------------------------------------------------------+
| `monolog.processor`_              | Aggiunta di un processore personalizzato per i log                                        |
+-----------------------------------+---------------------------------------------------------------------------+
| `routing.loader`_                 | Registrare un servizio personalizzato che carica rotte                               |
+-----------------------------------+---------------------------------------------------------------------------+
| `security.voter`_                 | Aggiunta di un votante alla logica di autorizzazione di Symfony                       |
+-----------------------------------+---------------------------------------------------------------------------+
| `security.remember_me_aware`_     | Consentire il "ricorami" nell'autenticazione                                       |
+-----------------------------------+---------------------------------------------------------------------------+
| `security.listener.factory`_      | Necessario quando si cre un sistema di autenticazione personalizzato                    |
+-----------------------------------+---------------------------------------------------------------------------+
| `swiftmailer.plugin`_             | Registre un plugin di SwiftMailer                                      |
+-----------------------------------+---------------------------------------------------------------------------+
| `templating.helper`_              | Rendere il servizio disponibile nei template PHP                              |
+-----------------------------------+---------------------------------------------------------------------------+
| `translation.loader`_             | Registrare un servizio che carica traduzioni                       |
+-----------------------------------+---------------------------------------------------------------------------+
| `twig.extension`_                 | Registrare un'estensione di Twig                                          |
+-----------------------------------+---------------------------------------------------------------------------+
| `validator.constraint_validator`_ | Creare un vincolo di validazione                              |
+-----------------------------------+---------------------------------------------------------------------------+
| `validator.initializer`_          | Registrare un servizio che inizializza oggetti prima della validazione             |
+-----------------------------------+---------------------------------------------------------------------------+

data_collector
--------------

**Scopo**: creare a class that collects custom data for the profiler

For details on creating your own custom data collection, read the cookbook
article: :doc:`/cookbook/profiler/data_collector`.

Per abilitare un helper di template personalizzato, aggiungerlo come normale servizio in
una delle proprie configurazioni, taggarlo con ``templating.helper`` e definire un
attributo ``alias`` (l'helper sarà accessibile nei template tramite questo
alias):

.. configuration-block::

    .. code-block:: yaml

        services:
            templating.helper.your_helper_name:
                class: Fully\Qualified\Helper\Class\Name
                tags:
                    - { name: templating.helper, alias: alias_name }

    .. code-block:: xml

        <service id="templating.helper.your_helper_name" class="Fully\Qualified\Helper\Class\Name">
            <tag name="templating.helper" alias="alias_name" />
        </service>

    .. code-block:: php

        $container
            ->register('templating.helper.your_helper_name', 'Fully\Qualified\Helper\Class\Name')
            ->addTag('templating.helper', array('alias' => 'alias_name'))
        ;

.. _reference-dic-tags-twig-extension:

Abilitare estensioni personalizzate in Twig
-------------------------------------------

Per abilitare un'estensione di Twig, aggiungerla come normale servizio in una delle
proprie configurazioni e taggarla con ``twig.extension``:

.. configuration-block::

    .. code-block:: yaml

        services:
            twig.extension.your_extension_name:
                class: Fully\Qualified\Extension\Class\Name
                tags:
                    - { name: twig.extension }

    .. code-block:: xml

        <service id="twig.extension.your_extension_name" class="Fully\Qualified\Extension\Class\Name">
            <tag name="twig.extension" />
        </service>

    .. code-block:: php

        $container
            ->register('twig.extension.your_extension_name', 'Fully\Qualified\Extension\Class\Name')
            ->addTag('twig.extension')
        ;

Per informazioni su come creare la classe estensione di Twig, vedere la
`documentazione di Twig`_ sull'argomento.

Prima di scrivere la propria estensione, dare un'occhiata al
`repository ufficiale di estensioni Twig`_ , che include tante estensioni
utili. Per esempio, ``Intl`` e il suo filtro ``localizeddate``, che
formatta una data a seconda del locale dell'utente. Anche queste estensioni ufficiali
devono essere aggiunte come servizi:

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

.. _dic-tags-kernel-event-listener:

Abilitare ascoltatori personalizzati
------------------------------------

Per abilitare un ascoltatore personalizzato, aggiungerlo come normale servizio in una
della proprie configurazioni e taggarlo con ``kernel.event_listener``. Bisogna fornire
il nome dell'evento che il proprio ascolta, come anche il metodo che sarà
richiamato:

.. configuration-block::

    .. code-block:: yaml

        services:
            kernel.listener.your_listener_name:
                class: Fully\Qualified\Listener\Class\Name
                tags:
                    - { name: kernel.event_listener, event: xxx, method: onXxx }

    .. code-block:: xml

        <service id="kernel.listener.your_listener_name" class="Fully\Qualified\Listener\Class\Name">
            <tag name="kernel.event_listener" event="xxx" method="onXxx" />
        </service>

    .. code-block:: php

        $container
            ->register('kernel.listener.your_listener_name', 'Fully\Qualified\Listener\Class\Name')
            ->addTag('kernel.event_listener', array('event' => 'xxx', 'method' => 'onXxx'))
        ;

.. note::

    Si può anche specificare una priorità, come attributo del tag ``kernel.event_listener``
    (probabilmente il metodo o gli attributi dell'evento), con un intero positivo oppure
    negativo. Questo consente di assicurarsi che i propri ascoltatori siano sempre
    richiamati prima o dopo altri ascoltatori che ascoltano lo stesso evento.

Abilitare motori di template personalizzati
-------------------------------------------

Per abilitare un motore di template personalizzato, aggiungerlo come normale servizio in
una delle proprie configurazioni, con il tag ``templating.engine``:

.. configuration-block::

    .. code-block:: yaml

        services:
            templating.engine.your_engine_name:
                class: Fully\Qualified\Engine\Class\Name
                tags:
                    - { name: templating.engine }

    .. code-block:: xml

        <service id="templating.engine.your_engine_name" class="Fully\Qualified\Engine\Class\Name">
            <tag name="templating.engine" />
        </service>

    .. code-block:: php

        $container
            ->register('templating.engine.your_engine_name', 'Fully\Qualified\Engine\Class\Name')
            ->addTag('templating.engine')
        ;

Abilitare caricatori di rotte personalizzati
--------------------------------------------

Per abilitare un caricatore personalizzato di rotte, aggiungerlo come normale servizio
in una delle proprie configurazioni, con il tag ``routing.loader``:

.. configuration-block::

    .. code-block:: yaml

        services:
            routing.loader.your_loader_name:
                class: Fully\Qualified\Loader\Class\Name
                tags:
                    - { name: routing.loader }

    .. code-block:: xml

        <service id="routing.loader.your_loader_name" class="Fully\Qualified\Loader\Class\Name">
            <tag name="routing.loader" />
        </service>

    .. code-block:: php

        $container
            ->register('routing.loader.your_loader_name', 'Fully\Qualified\Loader\Class\Name')
            ->addTag('routing.loader')
        ;

.. _dic_tags-monolog:

Usare un canale di log personalizzato con Monolog
-------------------------------------------------

Monolog consente di condividere i suoi gestori tra diversi canali di log.
Il servizio di log usa il canale ``app``, ma si può cambiare canale quando si
inietta il logger in un servizio.

.. configuration-block::

    .. code-block:: yaml

        services:
            mio_servizio:
                class: Fully\Qualified\Loader\Class\Name
                arguments: [@logger]
                tags:
                    - { name: monolog.logger, channel: acme }

    .. code-block:: xml

        <service id="mio_servizio" class="Fully\Qualified\Loader\Class\Name">
            <argument type="service" id="logger" />
            <tag name="monolog.logger" channel="acme" />
        </service>

    .. code-block:: php

        $definition = new Definition('Fully\Qualified\Loader\Class\Name', array(new Reference('logger'));
        $definition->addTag('monolog.logger', array('channel' => 'acme'));
        $container->register('mio_servizio', $definition);;

.. note::

    Funziona solo quando il servizio di log è il parametro di un costruttore, non
    quando è iniettato tramite setter.

.. _dic_tags-monolog-processor:

Aggiungere un processore per Monolog
------------------------------------

Monolog consente di aggiungere processori nel logger o nei gestori, per aggiungere
dati extra nelle registrazioni. Un processore riceve la registrazione come parametro e
deve restituirlo dopo aver aggiunto dei dati extra, nell'attributo ``extra`` della
registrazione.

Vediamo come poter usare ``IntrospectionProcessor`` per aggiungere file, riga, classe
e metodo quando il logger viene attivato.

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

    Se il proprio servizio non è un metodo (che usa ``__invoke``), si può aggiungere
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
l'attributo ``channel``. Registrerà il processore solo per il canale di log
``security``, usato nel componente Security:

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

    Non si possono usare entrambi gli attributi ``handler`` e ``channel`` per lo stesso
    tag, perché i gestori sono condivisi tra tutti i canali.

..  _`documentazione di Twig`: http://twig.sensiolabs.org/doc/extensions.html
..  _`repository ufficiale di estensioni Twig`: http://github.com/fabpot/Twig-extensions
