.. index::
   single: Sessioni, cartella delle sessioni

Configurare la cartella in cui salvare le sessioni
==================================================

Per impostazione predefinita, Symfony Standard Edition usa i valori globali di ``php.ini``
per ``session.save_handler`` e ``session.save_path`` per determinare dove
salvare le sessioni. Questo a causa della seguente configurazione:

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        framework:
            session:
                # handler_id impostato a null userà il gestore di sessioni predefinito da php.ini
                handler_id: ~

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <?xml version="1.0" encoding="UTF-8" ?>
        <container xmlns="http://symfony.com/schema/dic/services"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xmlns:framework="http://symfony.com/schema/dic/symfony"
            xsi:schemaLocation="http://symfony.com/schema/dic/services
                http://symfony.com/schema/dic/services/services-1.0.xsd
                http://symfony.com/schema/dic/symfony
                http://symfony.com/schema/dic/symfony/symfony-1.0.xsd"
        >
            <framework:config>
                <!-- handler_id impostato a null userà il gestore di sessioni predefinito da php.ini -->
                <framework:session handler-id="null" />
            </framework:config>
        </container>

    .. code-block:: php

        // app/config/config.php
        $container->loadFromExtension('framework', array(
            'session' => array(
                // handler_id impostato a null userà il gestore di sessioni predefinito da php.ini
                'handler_id' => null,
            ),
        ));

Con questa configurazione, cambiare *dove* i meta-dati di sessione siano memorizzati
dipende completamente dalla configurazione di php.ini.

Tuttavia, se si ha la seguente configurazione, Symfony memorizzerà i dati di sessione
in file nella cartella della cache, ``%kernel.cache_dir%/sessions``. Questo vuol
dire che le sessioni andranno perse ogni volta che si pulisce la cache:

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        framework:
            session: ~

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <?xml version="1.0" encoding="UTF-8" ?>
        <container xmlns="http://symfony.com/schema/dic/services"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xmlns:framework="http://symfony.com/schema/dic/symfony"
            xsi:schemaLocation="http://symfony.com/schema/dic/services
                http://symfony.com/schema/dic/services/services-1.0.xsd
                http://symfony.com/schema/dic/symfony
                http://symfony.com/schema/dic/symfony/symfony-1.0.xsd"
        >
            <framework:config>
                <framework:session />
            </framework:config>
        </container>

    .. code-block:: php

        // app/config/config.php
        $container->loadFromExtension('framework', array(
            'session' => array(),
        ));

Usando una cartella diversa per salvare i dati delle sessioni è uno dei metodi per
assicurarsi che le sessioni attive non vadano perdute durante la pulizia della cache di Symfony.

.. tip::

    Un metodo eccellente (ma più complesso) consiste nell'uso di un gestore di sessione
    diverso, disponibile in Symfony. Si veda
    :doc:`/components/http_foundation/session_configuration` per un
    approfondimento sui gestori di sessione. C'è anche una ricetta sulla
    memorizzazione delle sessioni nella :doc:`base dati</cookbook/configuration/pdo_session_storage>`.

Per modificare la cartella in cui Symfony salva i dati di sessione, è sufficiente
cambiare la configurazione del framework. In questo esempio, la cartella delle sessioni
sarà cambiata in ``app/sessions``:

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        framework:
            session:
                handler_id: session.handler.native_file
                save_path: "%kernel.root_dir%/sessions"

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <?xml version="1.0" encoding="UTF-8" ?>
        <container xmlns="http://symfony.com/schema/dic/services"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xmlns:framework="http://symfony.com/schema/dic/symfony"
            xsi:schemaLocation="http://symfony.com/schema/dic/services
                http://symfony.com/schema/dic/services/services-1.0.xsd
                http://symfony.com/schema/dic/symfony
                http://symfony.com/schema/dic/symfony/symfony-1.0.xsd"
        >
            <framework:config>
                <framework:session handler-id="session.handler.native_file"
                    save-path="%kernel.root_dir%/sessions"
                />
            </framework:config>
        </container>

    .. code-block:: php

        // app/config/config.php
        $container->loadFromExtension('framework', array(
            'session' => array(
                'handler_id' => 'session.handler.native_file',
                'save_path'  => '%kernel.root_dir%/sessions',
            ),
        ));

