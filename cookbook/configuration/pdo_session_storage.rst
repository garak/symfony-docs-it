.. index::
   single: Session; Database Storage

Usare PdoSessionStorage per salvare le sessioni nella base dati
===============================================================

Normalmente, nella gestione delle sessioni, Symfony2 salva le relative informazioni
all'interno di file. Solitamente, i siti web di dimensioni medio grandi utilizzano
la base dati, invece dei file, per salvare i dati di sessione. Questo perchè le basi dati
sono più semplici da utilizzare e sono più scalabili in ambienti multi-webserver.

Symfony2 ha, al suo interno, una soluzione per l'archiviazione delle sessioni su base dati, chiamata
:class:`Symfony\\Component\\HttpFoundation\\SessionStorage\\PdoSessionStorage`.
Per utilizzarla è sufficiente cambiare alcuni parametri di ``config.yml`` (o del
proprio formato di configurazione):

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        framework:
            session:
                # ...
                storage_id:     session.storage.pdo

        parameters:
            pdo.db_options:
                db_table:    sessione
                db_id_col:   sessione_id
                db_data_col: sessione_value
                db_time_col: sessione_time

        services:
            pdo:
                class: PDO
                arguments:
                    dsn:      "mysql:dbname=db_sessione"
                    user:     utente_db
                    password: password_db

            session.storage.pdo:
                class:     Symfony\Component\HttpFoundation\SessionStorage\PdoSessionStorage
                arguments: [@pdo, %session.storage.options%, %pdo.db_options%]

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <framework:config>
            <framework:session storage-id="session.storage.pdo" lifetime="3600" auto-start="true"/>
        </framework:config>

        <parameters>
            <parameter key="pdo.db_options" type="collection">
                <parameter key="db_table">sessione</parameter>
                <parameter key="db_id_col">sessione_id</parameter>
                <parameter key="db_data_col">sessione_value</parameter>
                <parameter key="db_time_col">sessione_time</parameter>
            </parameter>
        </parameters>

        <services>
            <service id="pdo" class="PDO">
                <argument>mysql:dbname=db_sessione</argument>
                <argument>utente_db</argument>
                <argument>password_db</argument>
            </service>

            <service id="session.storage.pdo" class="Symfony\Component\HttpFoundation\SessionStorage\PdoSessionStorage">
                <argument type="service" id="pdo" />
                <argument>%session.storage.options%</argument>
                <argument>%pdo.db_options%</argument>
            </service>
        </services>

    .. code-block:: php

        // app/config/config.yml
        use Symfony\Component\DependencyInjection\Definition;
        use Symfony\Component\DependencyInjection\Reference;

        $container->loadFromExtension('framework', array(
            // ...
            'session' => array(
                // ...
                'storage_id' => 'session.storage.pdo',
            ),
        ));

        $container->setParameter('pdo.db_options', array(
            'db_table'      => 'sessione',
            'db_id_col'     => 'sessione_id',
            'db_data_col'   => 'sessione_value',
            'db_time_col'   => 'sessione_time',
        ));

        $pdoDefinition = new Definition('PDO', array(
            'mysql:dbname=db_sessione',
            'utente_db',
            'password_db',
        ));
        $container->setDefinition('pdo', $pdoDefinition);

        $storageDefinition = new Definition('Symfony\Component\HttpFoundation\SessionStorage\PdoSessionStorage', array(
            new Reference('pdo'),
            '%session.storage.options%',
            '%pdo.db_options%',
        ));
        $container->setDefinition('session.storage.pdo', $storageDefinition);

* ``db_table``: Nome della tabella, nella base dati, per le sessioni
* ``db_id_col``: Nome della colonna id della tabella delle sessioni (VARCHAR(255) o maggiore)
* ``db_data_col``: Nome della colonna dove salvare il valore della sessione (TEXT o CLOB)
* ``db_time_col``: Nome della colonna per la registrazione del tempo della sessione (INTEGER)

Condividere le informazioni di connessione della base dati
----------------------------------------------------------

Grazie a questa configurazione, i parametri della connessione alla base dati sono definiti
solo per l'archiviazione dei dati di sessione. La qual cosa è perfetta se si usa
una base dati differente per i dati di sessione.

Ma se si preferisce salvare i dati di sessione nella stessa base dati in cui
risiedono i rimanenti dati del progetto, è possibile utilizzare i parametri di 
connessione di parameter.ini, richiamandone la configurazione della base dati:

.. configuration-block::

    .. code-block:: yaml

        pdo:
            class: PDO
            arguments:
                - "mysql:dbname=%database_name%"
                - %database_user%
                - %database_password%

    .. code-block:: xml

        <service id="pdo" class="PDO">
            <argument>mysql:dbname=%database_name%</argument>
            <argument>%database_user%</argument>
            <argument>%database_password%</argument>
        </service>

    .. code-block:: xml

        $pdoDefinition = new Definition('PDO', array(
            'mysql:dbname=%database_name%',
            '%database_user%',
            '%database_password%',
        ));

Esempi di dichiarazioni SQL
---------------------------

MySQL
~~~~~

La dichiarazione SQL per creare la necessaria tabella nella base dati potrebbe essere
simile alla seguente (MySQL):

.. code-block:: sql

    CREATE TABLE `sessione` (
        `sessione_id` varchar(255) NOT NULL,
        `sessione_value` text NOT NULL,
        `sessione_time` int(11) NOT NULL,
        PRIMARY KEY (`session_id`)
    ) ENGINE=InnoDB DEFAULT CHARSET=utf8;

PostgreSQL
~~~~~~~~~~

Per PostgreSQL, la dichiarazione sarà simile alla seguente:

.. code-block:: sql

    CREATE TABLE sessione (
        sessione_id character varying(255) NOT NULL,
        sessione_value text NOT NULL,
        sessione_time integer NOT NULL,
        CONSTRAINT session_pkey PRIMARY KEY (session_id),
    );
