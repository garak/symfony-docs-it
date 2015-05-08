.. index::
    single: Doctrine; Riferimento configurazione ORM
    single: Riferimento configurazione; ORM Doctrine

Configurazione di DoctrineBundle ("doctrine")
=============================================

Configurazione predefinita completa
-----------------------------------

.. configuration-block::

    .. code-block:: yaml

        doctrine:
            dbal:
                default_connection:   default
                types:
                    # Un insieme di tipi personalizzati
                    # Esempio
                    un_tipo_personalizzato:
                        class:                Acme\HelloBundle\MioTipoPersonalizzato
                        commented:            true
                # Se abilitato, tutte le tabelle non prefissate con "sf2_" saranno ignorate.
                # Questo serve per tabelle personalizzate che non devono essere alterate automaticamente.
                #schema_filter:        ^sf2_

                connections:
                    # Un insieme di nomi di connessioni (p.e. default, conn2, ecc.)
                    default:
                        dbname:               ~
                        host:                 localhost
                        port:                 ~
                        user:                 root
                        password:             ~
                        charset:              ~
                        path:                 ~
                        memory:               ~

                        # Il socket unix da usare per MySQL
                        unix_socket:          ~

                        # True per usare una connessione persistente per il driver ibm_db2
                        persistent:           ~

                        # Protocollo da usare per il driver ibm_db2 (se omesso, TCPIP)
                        protocol:             ~

                        # True per usare dbname come nome di servizio invece di SID per Oracle
                        service:              ~

                        # Modalità di sessione da usare per il driver oci8
                        sessionMode:          ~

                        # True per usare un pool di server con il driver oci8
                        pooled:               ~

                        # Configurazione di MultipleActiveResultSets per il driver pdo_sqlsrv
                        MultipleActiveResultSets:  ~
                        driver:               pdo_mysql
                        platform_service:     ~

                        # la versione del server della base dati
                        server_version:       ~

                        # se true, un canale "doctrine" di monolog conterrà le query
                        logging:              %kernel.debug%
                        profiling:            %kernel.debug%
                        driver_class:         ~
                        wrapper_class:        ~
                        options:
                            # array di opzioni
                            key:                  []
                        mapping_types:
                            # array di tipi di mappature
                            name:                 []
                        slaves:

                            # insieme di nomi di connessioni slave (p.e. slave1, slave2)
                            slave1:
                                dbname:               ~
                                host:                 localhost
                                port:                 ~
                                user:                 root
                                password:             ~
                                charset:              ~
                                path:                 ~
                                memory:               ~

                                # Il socket unix da usare per MySQL
                                unix_socket:          ~

                                # True per usare una connessione persistente per il driver ibm_db2
                                persistent:           ~

                                # Protocollo da usare per il driver ibm_db2 (se omesso, TCPIP)
                                protocol:             ~

                                # True per usare dbname come nome di servizio invece di SID per Oracle
                                service:              ~

                                # Modalità di sessione da usare per il driver oci8
                                sessionMode:          ~

                                # True per usare un pool di server con il driver oci8
                                pooled:               ~

                                # la versione del server della base dati
                                server_version:       ~

                                # Configurazione di MultipleActiveResultSets per il driver pdo_sqlsrv
                                MultipleActiveResultSets:  ~

            orm:
                default_entity_manager:  ~
                auto_generate_proxy_classes:  false
                proxy_dir:            "%kernel.cache_dir%/doctrine/orm/Proxies"
                proxy_namespace:      Proxies
                # cercare la classe "ResolveTargetEntityListener" per una ricetta a riguardo
                resolve_target_entities: []
                entity_managers:
                    # Un insieme di nomi di gestori di entità (p.e. un_em, un_altro_em)
                    un_em:
                        query_cache_driver:
                            type:                 array # Obbligatorio
                            host:                 ~
                            port:                 ~
                            instance_class:       ~
                            class:                ~
                        metadata_cache_driver:
                            type:                 array # Obbligatorio
                            host:                 ~
                            port:                 ~
                            instance_class:       ~
                            class:                ~
                        result_cache_driver:
                            type:                 array # Obbligatorio
                            host:                 ~
                            port:                 ~
                            instance_class:       ~
                            class:                ~
                        connection:           ~
                        class_metadata_factory_name:  Doctrine\ORM\Mapping\ClassMetadataFactory
                        default_repository_class:  Doctrine\ORM\EntityRepository
                        auto_mapping:         false
                        hydrators:

                            # Un array di nomi di idratatori
                            hydrator_name:                 []
                        mappings:
                            # Un array di mappature, che può essere un nome di bundle o qualcosa d'altro
                            mapping_name:
                                mapping:              true
                                type:                 ~
                                dir:                  ~
                                alias:                ~
                                prefix:               ~
                                is_bundle:            ~
                        dql:
                            # un insieme di funzioni stringa
                            string_functions:
                                # esempio
                                # test_string: Acme\HelloBundle\DQL\StringFunction

                            # un insieme di funzioni numeriche
                            numeric_functions:
                                # esempio
                                # test_numeric: Acme\HelloBundle\DQL\NumericFunction

                            # un insieme di funzioni datetime
                            datetime_functions:
                                # esempio
                                # test_datetime: Acme\HelloBundle\DQL\DatetimeFunction

                        # Registra filtri SQL nel gestore di entità
                        filters:
                            # Un array di filtri
                            some_filter:
                                class:                ~ # Obbligatorio
                                enabled:              false

    .. code-block:: xml

        <?xml version="1.0" encoding="UTF-8" ?>
        <container xmlns="http://symfony.com/schema/dic/services"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xmlns:doctrine="http://symfony.com/schema/dic/doctrine"
            xsi:schemaLocation="http://symfony.com/schema/dic/services
                http://symfony.com/schema/dic/services/services-1.0.xsd
                http://symfony.com/schema/dic/doctrine
                http://symfony.com/schema/dic/doctrine/doctrine-1.0.xsd">

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
                        server-version="5.6"
                    >
                        <doctrine:option key="foo">bar</doctrine:option>
                        <doctrine:mapping-type name="enum">string</doctrine:mapping-type>
                    </doctrine:connection>
                    <doctrine:connection name="conn1" />
                    <doctrine:type name="custom">Acme\HelloBundle\MyCustomType</doctrine:type>
                </doctrine:dbal>

                <doctrine:orm
                    default-entity-manager="default"
                    auto-generate-proxy-classes="false"
                    proxy-namespace="Proxies"
                    proxy-dir="%kernel.cache_dir%/doctrine/orm/Proxies"
                >
                    <doctrine:entity-manager
                        name="default"
                        query-cache-driver="array"
                        result-cache-driver="array"
                        connection="conn1"
                        class-metadata-factory-name="Doctrine\ORM\Mapping\ClassMetadataFactory"
                    >
                        <doctrine:metadata-cache-driver
                            type="memcache"
                            host="localhost"
                            port="11211"
                            instance-class="Memcache"
                            class="Doctrine\Common\Cache\MemcacheCache"
                        />

                        <doctrine:mapping name="AcmeHelloBundle" />

                        <doctrine:dql>
                            <doctrine:string-function name="test_string">
                                Acme\HelloBundle\DQL\StringFunction
                            </doctrine:string-function>

                            <doctrine:numeric-function name="test_numeric">
                                Acme\HelloBundle\DQL\NumericFunction
                            </doctrine:numeric-function>

                            <doctrine:datetime-function name="test_datetime">
                                Acme\HelloBundle\DQL\DatetimeFunction
                            </doctrine:datetime-function>
                        </doctrine:dql>
                    </doctrine:entity-manager>

                    <doctrine:entity-manager name="em2" connection="conn2" metadata-cache-driver="apc">
                        <doctrine:mapping
                            name="DoctrineExtensions"
                            type="xml"
                            dir="%kernel.root_dir%/../vendor/gedmo/doctrine-extensions/lib/DoctrineExtensions/Entity"
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
            # la distribuzione standard sovrascrive a true in debug, false altrimenti
            auto_generate_proxy_classes: false
            proxy_namespace: Proxies
            proxy_dir: "%kernel.cache_dir%/doctrine/orm/Proxies"
            default_entity_manager: default
            metadata_cache_driver: array
            query_cache_driver: array
            result_cache_driver: array

Ci sono molte altre opzioni di configurazione che si possono usare per sovrascrivere
determinate classi, ma sono solo per casi molto avanzati.

Driver per la cache
~~~~~~~~~~~~~~~~~~~

Per i driver della cache, si può specificare "array", "apc", "memcache", "memcached",
"xcache" o "service".

L'esempio seguente mostra una panoramica delle configurazioni di cache:

.. code-block:: yaml

    doctrine:
        orm:
            auto_mapping: true
            metadata_cache_driver: apc
            query_cache_driver:
                type: service
                id: my_doctrine_common_cache_service
            result_cache_driver:
                type: memcache
                host: localhost
                port: 11211
                instance_class: Memcache

Configurazioni della mappatura
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

La definizione esplicita di tutte le entità mappate è l'unica configurazione
necessaria per l'ORM e ci sono diverse opzioni di configurazione controllabili.
La mappatura dispone delle seguenti opzioni di configurazione:

type
....

Uno tra ``annotation``, ``xml``, ``yml``, ``php`` o ``staticphp``.
Specifica quale di tipo di meta-dati usa la mappatura.

dir
...

Percorso per la mappatura o per i file entità (a seconda del driver). Se
questo percorso è relativo, si assume sia relativo alla radice dei bundle. Funziona
solo se il nome della propria mappatura è il nome di un bundle. Se si vuole usare
questa opzione per specificare percorsi assoluti, si dovrebbe aggiungere al percorso
un prefisso con i parametri del kernel nel DIC (per esempio %kernel.root_dir%).

prefix
......

Un prefisso comune di spazio dei nomi che tutte le entità di questa
mappatura condividono. Questo prefisso non deve essere in conflitto con i prefissi
di altre mappature definite, altrimenti alcune entità non saranno trovate da Doctrine.
Questa opzione ha come valore predefinito lo spazio dei nomi del bundle + ``Entity``, per esempio
per un bundle chiamato ``AcmeHelloBundle`` il prefisso sarebbe ``Acme\HelloBundle\Entity``.

alias
.....

Doctrine offre un modo per avere alias di spazi dei nomi con nomi più
corti e semplici, da usare nelle query DQL o per l'accesso al Repository. Quando
si usa un bundle, l'alias predefinito è il nome del bundle.

is_bundle
.........

Questa opzione è un valore derivato da ``dir`` e ha ``true`` come
valore predefinito, se la cartella è fornita da una verifica con ``file_exists()``
che restituisca ``false``. È ``false`` se la verifica restituisce ``true``. In
questo caso, un percorso assoluto  è stato specificato e i file dei meta-dati sono
probabilmente in una cartella fuori da un bundle.

.. index::
    single: Configurazione; Doctrine DBAL
    single: Doctrine; Configurazione DBAL

.. _`reference-dbal-configuration`:

Configurazione Doctrine DBAL
----------------------------

DoctrineBundle supporta tutti i parametri che i driver predefiniti di Doctrine
accettano, convertiti alla nomenclatura XML o YML di Symfony.
Vedere la `documentazione DBAL`_ di Doctrine per maggiori informazioni.
Il blocco seguente mostra tutte le voci di configurazione:

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
                # opzione driverClass di DBAL
                driver_class:         MyNamespace\MyDriverImpl
                # opzione driverOptions di DBAL
                options:
                    foo: bar
                path:                 "%kernel.data_dir%/data.sqlite"
                memory:               true
                unix_socket:          /tmp/mysql.sock
                # opzione wrapperClass di DBAL
                wrapper_class:        MyDoctrineDbalConnectionWrapper
                charset:              UTF8
                logging:              "%kernel.debug%"
                platform_service:     MyOwnDatabasePlatformService
                server_version:       5.6
                mapping_types:
                    enum: string
                types:
                    custom: Acme\HelloBundle\MyCustomType
                # opzione keepSlave di DBAL
                keep_slave:           false

    .. code-block:: xml

        <?xml version="1.0" encoding="UTF-8" ?>
        <container xmlns="http://symfony.com/schema/dic/services"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xmlns:doctrine="http://symfony.com/schema/dic/doctrine"
            xsi:schemaLocation="http://symfony.com/schema/dic/services
                http://symfony.com/schema/dic/services/services-1.0.xsd
                http://symfony.com/schema/dic/doctrine
                http://symfony.com/schema/dic/doctrine/doctrine-1.0.xsd"
        >

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
                    server-version="5.6">

                    <doctrine:option key="foo">bar</doctrine:option>
                    <doctrine:mapping-type name="enum">string</doctrine:mapping-type>
                    <doctrine:type name="custom">Acme\HelloBundle\MyCustomType</doctrine:type>
                </doctrine:dbal>
            </doctrine:config>
        </container>

.. note::

    L'opzione ``server_version`` è stata aggiunta in Doctrine DBAL 2.5, che è usato
    da DoctrineBundle 1.3. Il valore di tale opzione deve corrispondere a quella del
    server della base dati (usare il comando ``postgres -V`` o ``psql -V`` per conoscere la propria
    versione di PostgreSQL e ``mysql -V`` per quella di MySQL).

    Se non si definisce questa opzione e la base dati non è ancora stata create,
    si potrebbero ottenere errori ``PDOException``, perché Doctrine proverà a indovinare la
    versione della base dati, senza trovarne una disponibile.

Se si vogliono configurare connessioni multiple in YAML, si possono mettere sotto la
voce ``connections`` e dar loro un nome univoco:

.. code-block:: yaml

    doctrine:
        dbal:
            default_connection:       default
            connections:
                default:
                    dbname:           Symfony
                    user:             root
                    password:         null
                    host:             localhost
                    server_version:   5.6
                customer:
                    dbname:           customer
                    user:             root
                    password:         null
                    host:             localhost
                    server_version:   5.7

Il servizio ``database_connection`` fa sempre riferimento alla configurazione
predefinita, che è la prima definita o l'unica configurata tramite il
parametro ``default_connection``.

Ogni connessione è anche accessibile tramite il servizio ``doctrine.dbal.[nome]_connection``,
in cui ``[nome]`` è il nome della connessione.

.. _documentazione DBAL: http://docs.doctrine-project.org/projects/doctrine-dbal/en/latest/reference/configuration.html

Sintassi abbreviata della configurazione
----------------------------------------

Se si una singolo gestore di entità, tutte le opzioni disponibili
possono essere inserite direttamente sotto il livello ``doctrine.orm``. 

.. code-block:: yaml

    doctrine:
        orm:
            # ...
            query_cache_driver:
               # ...
            metadata_cache_driver:
                # ...
            result_cache_driver:
                # ...
            connection: ~
            class_metadata_factory_name:  Doctrine\ORM\Mapping\ClassMetadataFactory
            default_repository_class:  Doctrine\ORM\EntityRepository
            auto_mapping: false
            hydrators:
                # ...
            mappings:
                # ...
            dql:
                # ...
            filters:
                # ...

Questa versione abbreviata è usata comunemente in altre sezioni della documentazione.
Tenere a mente che non si possono usare entrambe le sintassi contemporaneamente.

.. _`DQL User Defined Functions`: http://docs.doctrine-project.org/projects/doctrine-orm/en/latest/cookbook/dql-user-defined-functions.html
