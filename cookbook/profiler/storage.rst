.. index::
    single: Profilazione; Configurazione della memorizzazione

Cambiare la memorizzazione del profilatore
==========================================

Per impostazione predefinita, il profilatore memorizza i dati raccolti in file nella cartella della cache.
Si può modificare la memorizzazione utilizzata, tramite le opzioni ``dsn``, ``username``,
``password`` e ``lifetime``. Per esempio, la configurazione seguente
usa MySQL come memorizzazione per il profilatore, con una scadenza di un'ora:

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        framework:
            profiler:
                dsn:      "mysql:host=localhost;dbname=%database_name%"
                username: "%database_user%"
                password: "%database_password%"
                lifetime: 3600

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
                <framework:profiler
                    dsn="mysql:host=localhost;dbname=%database_name%"
                    username="%database_user%"
                    password="%database_password%"
                    lifetime="3600"
                />
            </framework:config>
        </container>

    .. code-block:: php

        // app/config/config.php

        // ...
        $container->loadFromExtension('framework', array(
            'profiler' => array(
                'dsn'      => 'mysql:host=localhost;dbname=%database_name%',
                'username' => '%database_user',
                'password' => '%database_password%',
                'lifetime' => 3600,
            ),
        ));

Il :doc:`componente HttpKernel </components/http_kernel/introduction>` supporta
attualmente le seguenti implementazioni di memorizzazione per il profilatore:

* :class:`Symfony\\Component\\HttpKernel\\Profiler\\FileProfilerStorage`
* :class:`Symfony\\Component\\HttpKernel\\Profiler\\MemcachedProfilerStorage`
* :class:`Symfony\\Component\\HttpKernel\\Profiler\\MemcacheProfilerStorage`
* :class:`Symfony\\Component\\HttpKernel\\Profiler\\MongoDbProfilerStorage`
* :class:`Symfony\\Component\\HttpKernel\\Profiler\\MysqlProfilerStorage`
* :class:`Symfony\\Component\\HttpKernel\\Profiler\\RedisProfilerStorage`
* :class:`Symfony\\Component\\HttpKernel\\Profiler\\SqliteProfilerStorage`
