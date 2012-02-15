.. index::
   single: Doctrine; Riferimento configurazione ORM
   single: Riferimento configurazione; ORM Doctrine

Riferimento configurazione
==========================

.. configuration-block::

    .. code-block:: yaml

        doctrine:
            dbal:
                default_connection:   default
                connections:
                    default:
                        dbname:               database
                        host:                 localhost
                        port:                 1234
                        user:                 user
                        password:             secret
                        driver:               pdo_mysql
                        driver_class:         MyNamespace\MyDriverImpl
                        options:
                            foo: bar
                        path:                 %kernel.data_dir%/data.sqlite
                        memory:               true
                        unix_socket:          /tmp/mysql.sock
                        wrapper_class:        MyDoctrineDbalConnectionWrapper
                        charset:              UTF8
                        logging:              %kernel.debug%
                        platform_service:     MyOwnDatabasePlatformService
                        mapping_types:
                            enum: string
                    conn1:
                        # ...
                types:
                    custom: Acme\HelloBundle\MyCustomType
            orm:
                auto_generate_proxy_classes:    false
                proxy_namespace:                Proxies
                proxy_dir:                      %kernel.cache_dir%/doctrine/orm/Proxies
                default_entity_manager:         default # The first defined is used if not set
                entity_managers:
                    default:
                        # The name of a DBAL connection (the one marked as default is used if not set)
                        connection:                     conn1
                        mappings: # Required
                            AcmeHelloBundle: ~
                        class_metadata_factory_name:    Doctrine\ORM\Mapping\ClassMetadataFactory
                        # All cache drivers have to be array, apc, xcache or memcache
                        metadata_cache_driver:          array
                        query_cache_driver:             array
                        result_cache_driver:
                            type:           memcache
                            host:           localhost
                            port:           11211
                            instance_class: Memcache
                            class:          Doctrine\Common\Cache\MemcacheCache
                        dql:
                            string_functions:
                                test_string: Acme\HelloBundle\DQL\StringFunction
                            numeric_functions:
                                test_numeric: Acme\HelloBundle\DQL\NumericFunction
                            datetime_functions:
                                test_datetime: Acme\HelloBundle\DQL\DatetimeFunction
                        hydrators:
                            custom: Acme\HelloBundle\Hydrators\CustomHydrator
                    em2:
                        # ...

    .. code-block:: xml

        <container xmlns="http://symfony.com/schema/dic/services"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xmlns:doctrine="http://symfony.com/schema/dic/doctrine"
            xsi:schemaLocation="http://symfony.com/schema/dic/services http://symfony.com/schema/dic/services/services-1.0.xsd
                                http://symfony.com/schema/dic/doctrine http://symfony.com/schema/dic/doctrine/doctrine-1.0.xsd">

            <doctrine:config>
                <doctrine:dbal default-connection="default">
                    <doctrine:connection
                        name="default"
                        dbname="database"
                        host="localhost"
                        port="1234"
                        user="user"
                        password="secret"
                        driver="pdo_mysql"
                        driver-class="MyNamespace\MyDriverImpl"
                        path="%kernel.data_dir%/data.sqlite"
                        memory="true"
                        unix-socket="/tmp/mysql.sock"
                        wrapper-class="MyDoctrineDbalConnectionWrapper"
                        charset="UTF8"
                        logging="%kernel.debug%"
                        platform-service="MyOwnDatabasePlatformService"
                    >
                        <doctrine:option key="foo">bar</doctrine:option>
                        <doctrine:mapping-type name="enum">string</doctrine:mapping-type>
                    </doctrine:connection>
                    <doctrine:connection name="conn1" />
                    <doctrine:type name="custom">Acme\HelloBundle\MyCustomType</doctrine:type>
                </doctrine:dbal>

                <doctrine:orm default-entity-manager="default" auto-generate-proxy-classes="false" proxy-namespace="Proxies" proxy-dir="%kernel.cache_dir%/doctrine/orm/Proxies">
                    <doctrine:entity-manager name="default" query-cache-driver="array" result-cache-driver="array" connection="conn1" class-metadata-factory-name="Doctrine\ORM\Mapping\ClassMetadataFactory">
                        <doctrine:metadata-cache-driver type="memcache" host="localhost" port="11211" instance-class="Memcache" class="Doctrine\Common\Cache\MemcacheCache" />
                        <doctrine:mapping name="AcmeHelloBundle" />
                        <doctrine:dql>
                            <doctrine:string-function name="test_string>Acme\HelloBundle\DQL\StringFunction</doctrine:string-function>
                            <doctrine:numeric-function name="test_numeric>Acme\HelloBundle\DQL\NumericFunction</doctrine:numeric-function>
                            <doctrine:datetime-function name="test_datetime>Acme\HelloBundle\DQL\DatetimeFunction</doctrine:datetime-function>
                        </doctrine:dql>
                    </doctrine:entity-manager>
                    <doctrine:entity-manager name="em2" connection="conn2" metadata-cache-driver="apc">
                        <doctrine:mapping
                            name="DoctrineExtensions"
                            type="xml"
                            dir="%kernel.root_dir%/../src/vendor/DoctrineExtensions/lib/DoctrineExtensions/Entity"
                            prefix="DoctrineExtensions\Entity"
                            alias="DExt"
                        />
                    </doctrine:entity-manager>
                </doctrine:orm>
            </doctrine:config>
        </container>

Panoramica della configurazione
-------------------------------

Il seguente esempio di configurazione mostra tutte le configurazioni predefinite, che
l'ORM risolve:

.. code-block:: yaml

    doctrine:
        orm:
            auto_mapping: true
            # the standard distribution overrides this to be true in debug, false otherwise
            auto_generate_proxy_classes: false
            proxy_namespace: Proxies
            proxy_dir: %kernel.cache_dir%/doctrine/orm/Proxies
            default_entity_manager: default
            metadata_cache_driver: array
            query_cache_driver: array
            result_cache_driver: array

Ci sono molte altre opzioni di configurazione che si possono usare per sovrascrivere
determinate classi, ma sono solo per casi molto avanzati.

Driver per la cache
~~~~~~~~~~~~~~~~~~~

Per i driver della cache, si può specificare "array", "apc", "memcache"
o "xcache".

L'esempio seguente mostra una panoramica delle configurazioni di cache:

.. code-block:: yaml

    doctrine:
        orm:
            auto_mapping: true
            metadata_cache_driver: apc
            query_cache_driver: xcache
            result_cache_driver:
                type: memcache
                host: localhost
                port: 11211
                instance_class: Memcache

Configurazioni della mappatura
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

La definizione esplicita di tutte le entità mappate è l'unica configurazione
necessaria per l'ORm e ci sono diverse opzioni di configurazione controllabili.
La mappatura dispone delle seguenti opzioni di configurazione:

* ``type`` Uno tra ``annotation``, ``xml``, ``yml``, ``php`` o ``staticphp``.
  Specifica quale di tipo di metadati usa la mappatura.

* ``dir`` Percorso per la mappatura o per i file entità (a seconda del driver). Se
  questo percorso è relativo, si assume sia relativo alla radice dei bundle. Funziona
  solo se il nome della propria mappatura è il nome di un bundle. Se si vuole usare
  questa opzione per specificare percorsi assoluti, si dovrebbe aggiungere al percorso
  un prefisso con i parametri del kernel nel DIC (per esempio %kernel.root_dir%).

* ``prefix`` Un prefisso comune di spazio dei nomi che tutte le entità di questa
  mappatura condividono. Questo prefisso non deve essere in conflitto con i prefissi
  di altre mappature definite, altrimenti alcune entità non saranno trovate da Doctrine.
  Questa opzione ha come valore predefinito lo spazio dei nomi del bundle + ``Entity``,
  per esempio per un bundle chiamato ``AcmeHelloBundle`` il prefisso sarebbe
  ``Acme\HelloBundle\Entity``.

* ``alias`` Doctrine offre un modo per avere alias di spazi dei nomi con nomi più
  corti e semplici, da usare nelle query DQL o per l'accesso al Repository. Quando
  si usa un bundle, l'alias predefinito è il nome del bundle.

* ``is_bundle`` Questa opzione è un valore derivato da ``dir`` ed ha ``true`` come
  valore predefinito, se la cartella è fornita da una verifica con ``file_exists()``
  che restituisca ``false``. È ``false`` se la verifica restituisce ``true``. In
  questo caso, un percorso assoluto  è stato specificato e i file dei metadati sono
  probabilmente in una cartella fuori da un bundle.

.. index::
    single: Configurazione; Doctrine DBAL
    single: Doctrine; Configurazione DBAL

.. _`reference-dbal-configuration`:

Configurazione Doctrine DBAL
----------------------------

.. note::

    DoctrineBundle supporta tutti i parametri che i driver predefiniti di Doctrine
    acceettano, convertiti alla nomenclatura XML o YML di Symfony.
    Vedere la `documentazione DBAL`_ di Doctrine per maggiori informazioni.

Oltre alle opzioni di Doctrine, ci sono alcune opzioni relative a Symfony, che
si possono configurare. Il blocco seguente mostra tutte le voci di configurazione:

.. configuration-block::

    .. code-block:: yaml

        doctrine:
            dbal:
                dbname:               database
                host:                 localhost
                port:                 1234
                user:                 user
                password:             secret
                driver:               pdo_mysql
                driver_class:         MyNamespace\MyDriverImpl
                options:
                    foo: bar
                path:                 %kernel.data_dir%/data.sqlite
                memory:               true
                unix_socket:          /tmp/mysql.sock
                wrapper_class:        MyDoctrineDbalConnectionWrapper
                charset:              UTF8
                logging:              %kernel.debug%
                platform_service:     MyOwnDatabasePlatformService
                mapping_types:
                    enum: string
                types:
                    custom: Acme\HelloBundle\MyCustomType

    .. code-block:: xml

        <!-- xmlns:doctrine="http://symfony.com/schema/dic/doctrine" -->
        <!-- xsi:schemaLocation="http://symfony.com/schema/dic/doctrine http://symfony.com/schema/dic/doctrine/doctrine-1.0.xsd"> -->

        <doctrine:config>
            <doctrine:dbal
                name="default"
                dbname="database"
                host="localhost"
                port="1234"
                user="user"
                password="secret"
                driver="pdo_mysql"
                driver-class="MyNamespace\MyDriverImpl"
                path="%kernel.data_dir%/data.sqlite"
                memory="true"
                unix-socket="/tmp/mysql.sock"
                wrapper-class="MyDoctrineDbalConnectionWrapper"
                charset="UTF8"
                logging="%kernel.debug%"
                platform-service="MyOwnDatabasePlatformService"
            >
                <doctrine:option key="foo">bar</doctrine:option>
                <doctrine:mapping-type name="enum">string</doctrine:mapping-type>
                <doctrine:type name="custom">Acme\HelloBundle\MyCustomType</doctrine:type>
            </doctrine:dbal>
        </doctrine:config>

Se si vogliono configurare connessioni multiple in YAML, si possono mettere sotto la
voce ``connections`` e dar loro un nome univoco:

.. code-block:: yaml

    doctrine:
        dbal:
            default_connection:       default
            connections:
                default:
                    dbname:           Symfony2
                    user:             root
                    password:         null
                    host:             localhost
                customer:
                    dbname:           customer
                    user:             root
                    password:         null
                    host:             localhost

Il servizio ``database_connection`` fa sempre riferimento alla configurazione
predefinita, che è la prima definita o l'unica configurata tramite il
parametro ``default_connection``.

Ogni connessione è anche accessibile tramite il servizio ``doctrine.dbal.[name]_connection``,
in cui ``[name]`` è il nome della connessione.

.. _documentazione DBAL: http://www.doctrine-project.org/docs/dbal/2.0/en