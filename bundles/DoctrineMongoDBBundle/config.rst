Configurazione di DoctrineMongoDBBundle
=======================================

Configurazione di esempio
-------------------------

.. code-block:: yaml

    # app/config/config.yml
    doctrine_mongodb:
        connections:
            default:
                server: mongodb://localhost:27017
                options: {}
        default_database: hello_%kernel.environment%
        document_managers:
            default:
                mappings:
                    AcmeDemoBundle: ~
                filters:
                    filter-name:
                        class: Class\Example\Filter\ODM\ExampleFilter
                        enabled: true
                metadata_cache_driver: array # array, apc, xcache, memcache

.. tip::

    Se ogni ambiente necessita di un diverso URI di connessione a MongoDB, si possono
    definirli in un parametro separato e farvi riferimento nella configurazione:

    .. code-block:: yaml

        # app/config/parameters.yml
        mongodb_server: mongodb://localhost:27017

    .. code-block:: yaml

        # app/config/config.yml
        doctrine_mongodb:
            connections:
                default:
                    server: %mongodb_server%

Se si vuole usare memcache per la cache dei meta-dati, occorre configurare
l'istanza ``Memcache``. Per esempio, si può fare come segue:

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        doctrine_mongodb:
            default_database: hello_%kernel.environment%
            connections:
                default:
                    server: mongodb://localhost:27017
                    options: {}
            document_managers:
                default:
                    mappings:
                        AcmeDemoBundle: ~
                    metadata_cache_driver:
                        type: memcache
                        class: Doctrine\Common\Cache\MemcacheCache
                        host: localhost
                        port: 11211
                        instance_class: Memcache

    .. code-block:: xml

        <?xml version="1.0" ?>

        <container xmlns="http://symfony.com/schema/dic/services"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xmlns:doctrine_mongodb="http://symfony.com/schema/dic/doctrine/odm/mongodb"
            xsi:schemaLocation="http://symfony.com/schema/dic/services http://symfony.com/schema/dic/services/services-1.0.xsd
                                http://symfony.com/schema/dic/doctrine/odm/mongodb http://symfony.com/schema/dic/doctrine/odm/mongodb/mongodb-1.0.xsd">

            <doctrine_mongodb:config default-database="hello_%kernel.environment%">
                <doctrine_mongodb:document-manager id="default">
                    <doctrine_mongodb:mapping name="AcmeDemoBundle" />
                    <doctrine_mongodb:metadata-cache-driver type="memcache">
                        <doctrine_mongodb:class>Doctrine\Common\Cache\MemcacheCache</doctrine_mongodb:class>
                        <doctrine_mongodb:host>localhost</doctrine_mongodb:host>
                        <doctrine_mongodb:port>11211</doctrine_mongodb:port>
                        <doctrine_mongodb:instance-class>Memcache</doctrine_mongodb:instance-class>
                    </doctrine_mongodb:metadata-cache-driver>
                </doctrine_mongodb:document-manager>
                <doctrine_mongodb:connection id="default" server="mongodb://localhost:27017">
                    <doctrine_mongodb:options>
                    </doctrine_mongodb:options>
                </doctrine_mongodb:connection>
            </doctrine_mongodb:config>
        </container>


Configurazione della mappatura
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

La definizione esplicita di tutti i documenti mappati è l'unica configurazione
necessaria per l'ODM e ci sono numerose opzioni di configurazione che si possono
conrtollare. La mappatura ha le seguenti opzioni di configurazione:

- ``type`` uno tra ``annotations``, ``xml``, ``yml``, ``php`` o ``staticphp``.
  Specifica il tipo di meta-dati usati dalla mappatura.

- ``dir`` percorso per la mappatura o per i file entità (a seconda del driver). Se
  questo percorso è relativo, si presume sia relativo alla radice del bundle.
  Questo funziona se il nome della propria mappatura è il nome di un bundle. Se si vuole
  usare questa opzione per specificare un percorso assoluto, occorre aggiungere un prefisso
  con i parametri del kernel esistenti nel DIC (per esempio, %kernel.root_dir%).

- ``prefix`` un prefisso comune dello spazio dei nomi, condiviso da tuti i documenti della mappatura.
  Questo prefisso non deve essere in conflitto con i prefissi di altre mappature definite,
  altrimenti Doctrine potrebbe non trovare alcuni documenti. Il valore predefinito di questa opzione
  è lo spazio dei nomi del bundle + ``Document``, per esempio per un bundle
  chiamato ``AcmeHelloBundle``, il prefisso sarebbe
  ``Acme\HelloBundle\Document``.

- ``alias`` Doctrine offre un sistema di alias degli spazi dei nomi dei documenti, con nomi più semplici
  e più brevi da usare nelle query per accedere al repository.

- ``is_bundle`` questa opzione ha un valore derivato da ``dir`` ed è impostata in modo predefinito a ``true``
  se ``dir`` è relativa, verificata con ``file_exists()`` che restituisca ``false``. È invece ``false``
  se il controllo di esistenza restituisce ``true``. In questo caso, è stato specificato un percorso
  assoluto e i file dei meta-dati sono molto probabilmente in una cartella fuori
  da un bundle.

Per evitare di dover configurare un sacco di informazioni per la mappatura, si
dovrebbero seguire le seguenti convenzioni:

1. Inserire tutti i documenti in una cartella ``Document/`` dentro al proprio bundle. Per
   esempio ``Acme/HelloBundle/Document/``.

2. Se si usa una mappatura xml, yml o php, mettere tutti i file di configurazione
   nella cartella ``Resources/config/doctrine/``, con suffisso
   rispettivamente mongodb.xml, mongodb.yml o mongodb.php.

3. Se esiste una cartella ``Document/``, ma nessuna cartella
   ``Resources/config/doctrine/``, si presume l'uso di annotazioni.

La configurazione seguente mostra tanto esempi di mappatura:

.. code-block:: yaml

    doctrine_mongodb:
        document_managers:
            default:
                mappings:
                    MyBundle1: ~
                    MyBundle2: yml
                    MyBundle3: { type: annotation, dir: Documents/ }
                    MyBundle4: { type: xml, dir: Resources/config/doctrine/mapping }
                    MyBundle5:
                        type: yml
                        dir: my-bundle-mappings-dir
                        alias: BundleAlias
                    doctrine_extensions:
                        type: xml
                        dir: %kernel.root_dir%/../src/vendor/DoctrineExtensions/lib/DoctrineExtensions/Documents
                        prefix: DoctrineExtensions\Documents\
                        alias: DExt

Filtri
~~~~~~

Si possono aggiungere facilmente filtri a un gestore di documenti, usando
la sintassi seguente:

.. code-block:: yaml

    doctrine_mongodb:
        document_managers:
            default:
                filters:
                    filter-one:
                        class: Class\ExampleOne\Filter\ODM\ExampleFilter
                        enabled: true
                    filter-two:
                        class: Class\ExampleTwo\Filter\ODM\ExampleFilter
                        enabled: false

I filtri sono usati per aggiungere condizini al queryBuilder, indipendentemente da dove la query sia generata.

Connessioni multiple 
~~~~~~~~~~~~~~~~~~~~

Se servono connessioni e gestori di documenti multipli, si può usare
la sintassi seguente:

.. configuration-block::

    .. code-block:: yaml

        doctrine_mongodb:
            default_database: hello_%kernel.environment%
            default_connection: conn2
            default_document_manager: dm2
            metadata_cache_driver: apc
            connections:
                conn1:
                    server: mongodb://localhost:27017
                conn2:
                    server: mongodb://localhost:27017
            document_managers:
                dm1:
                    connection: conn1
                    metadata_cache_driver: xcache
                    mappings:
                        AcmeDemoBundle: ~
                dm2:
                    connection: conn2
                    mappings:
                        AcmeHelloBundle: ~

    .. code-block:: xml

        <?xml version="1.0" ?>

        <container xmlns="http://symfony.com/schema/dic/services"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xmlns:doctrine_mongodb="http://symfony.com/schema/dic/doctrine/odm/mongodb"
            xsi:schemaLocation="http://symfony.com/schema/dic/services http://symfony.com/schema/dic/services/services-1.0.xsd
                                http://symfony.com/schema/dic/doctrine/odm/mongodb http://symfony.com/schema/dic/doctrine/odm/mongodb/mongodb-1.0.xsd">

            <doctrine_mongodb:config
                    default-database="hello_%kernel.environment%"
                    default-document-manager="dm2"
                    default-connection="dm2"
                    proxy-namespace="MongoDBODMProxies"
                    auto-generate-proxy-classes="true">
                <doctrine_mongodb:connection id="conn1" server="mongodb://localhost:27017">
                    <doctrine_mongodb:options>
                    </doctrine_mongodb:options>
                </doctrine_mongodb:connection>
                <doctrine_mongodb:connection id="conn2" server="mongodb://localhost:27017">
                    <doctrine_mongodb:options>
                    </doctrine_mongodb:options>
                </doctrine_mongodb:connection>
                <doctrine_mongodb:document-manager id="dm1" metadata-cache-driver="xcache" connection="conn1">
                    <doctrine_mongodb:mapping name="AcmeDemoBundle" />
                </doctrine_mongodb:document-manager>
                <doctrine_mongodb:document-manager id="dm2" connection="conn2">
                    <doctrine_mongodb:mapping name="AcmeHelloBundle" />
                </doctrine_mongodb:document-manager>
            </doctrine_mongodb:config>
        </container>

Si possono quindi recuperare i servizi configurati::

    $conn1 = $container->get('doctrine_mongodb.odm.conn1_connection');
    $conn2 = $container->get('doctrine_mongodb.odm.conn2_connection');

E si può anche recuperare il gestore di documenti configurato che usa i servizi
di connessione visti sopra::

    $dm1 = $container->get('doctrine_mongodb.odm.dm1_document_manager');
    $dm2 = $container->get('doctrine_mongodb.odm.dm2_document_manager');

Connettersi a un pool di server mongodb con una connessione
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Ci si può connettere a molti server mongodb in un'unica connessione, se si usa
un insieme di replice, elencando tutti i server nella stringa di connessione,
separati da virgole.

.. configuration-block::

    .. code-block:: yaml

        doctrine_mongodb:
            # ...
            connections:
                default:
                    server: 'mongodb://mongodb-01:27017,mongodb-02:27017,mongodb-03:27017'

Dove mongodb-01, mongodb-02 e mongodb-03 sono i nomi di host delle macchine. Se si preferisce,
si possono usare gli indirizzi IP.

Riprovare connessioni e query
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

MongoDB di Doctrine supporta automaticamente nuovi tentativi di connessioni e query dopo
un'eccezione, che è utile quando si ha a che fare con situazioni come il
fallimento di una replica. Questo allevia molto il bisogno di controllare le eccezioni
nel driver PHP per MongoDB nell'applicazione e di riprovare a mano le operazioni.

Si può specificare il numero di volte in cui riprovare le connessioni e le query, tramite le
opzioni `retry_connect` e `retry_query` nella configurazione del gestore di documenti.
I valori predefiniti di queste opzioni sono zero, che vuol dire che nessuna operazione sarà riprovata.

Configurazione predefinita completa
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

.. configuration-block::

    .. code-block:: yaml

        doctrine_mongodb:
            document_managers:

                # Prototype
                id:
                    connection:           ~
                    database:             ~
                    logging:              true
                    auto_mapping:         false
                    retry_connect:        0
                    retry_query:          0
                    metadata_cache_driver:
                        type:                 ~
                        class:                ~
                        host:                 ~
                        port:                 ~
                        instance_class:       ~
                    mappings:

                        # Prototype
                        name:
                            mapping:              true
                            type:                 ~
                            dir:                  ~
                            prefix:               ~
                            alias:                ~
                            is_bundle:            ~
            connections:

                # Prototype
                id:
                    server:               ~
                    options:
                        connect:              ~
                        persist:              ~
                        timeout:              ~
                        replicaSet:           ~
                        username:             ~
                        password:             ~
            proxy_namespace:      MongoDBODMProxies
            proxy_dir:            %kernel.cache_dir%/doctrine/odm/mongodb/Proxies
            auto_generate_proxy_classes:  false
            hydrator_namespace:   Hydrators
            hydrator_dir:         %kernel.cache_dir%/doctrine/odm/mongodb/Hydrators
            auto_generate_hydrator_classes:  false
            default_document_manager:  ~
            default_connection:   ~
            default_database:     default
