.. index::
    single: Sessione; Memorizzazione su base dati

Usare PdoSessionStorage per salvare le sessioni nella base dati
===============================================================

Normalmente, nella gestione delle sessioni, Symfony salva le relative informazioni
all'interno di file. Solitamente, i siti web di dimensioni medio grandi utilizzano
la basi dati, invece dei file, per salvare i dati di sessione. Questo perché le basi dati
sono più semplici da utilizzare e sono più scalabili in ambienti multi-webserver.

Symfony ha, al suo interno, una soluzione per l'archiviazione delle sessioni su base dati, chiamata
:class:`Symfony\\Component\\HttpFoundation\\Session\\Storage\\PdoSessionHandler`.
Per utilizzarla basta cambiare alcuni parametri nel file principale di configurazione:

.. versionadded:: 2.1
    In Symfony 2.1 sono stati leggermente modificati classe e spazio dei nomi. Ora si può
    trovare la classe `PdoSessionStorage` nello spazio dei nomi `Session\\Storage`:
    ``Symfony\Component\HttpFoundation\Session\Storage\PdoSessionStorage``. Si noti inoltre
    che in Symfony 2.1 va configurato ``handler_id`` e non ``storage_id``, come in Symfony 2.0.
    Più avanti, si noterà che ``%session.storage.options%`` non è più usato.

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        framework:
            session:
                # ...
                handler_id: session.handler.pdo

        parameters:
            pdo.db_options:
                db_table:    sessione
                db_id_col:   id_sessione
                db_data_col: valore_sessione
                db_time_col: tempo_sessione

        services:
            pdo:
                class: PDO
                arguments:
                    dsn:      "mysql:dbname=basedati"
                    user:     utente
                    password: password
                calls:
                    - [setAttribute, [3, 2]] # \PDO::ATTR_ERRMODE, \PDO::ERRMODE_EXCEPTION

            session.handler.pdo:
                class:     Symfony\Component\HttpFoundation\Session\Storage\Handler\PdoSessionHandler
                arguments: ["@pdo", "%pdo.db_options%"]

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <framework:config>
            <framework:session handler-id="session.handler.pdo" cookie-lifetime="3600" auto-start="true"/>
        </framework:config>

        <parameters>
            <parameter key="pdo.db_options" type="collection">
                <parameter key="db_table">sessione</parameter>
                <parameter key="db_id_col">id_sessione</parameter>
                <parameter key="db_data_col">valore_sessione</parameter>
                <parameter key="db_time_col">tempo_sessione</parameter>
            </parameter>
        </parameters>

        <services>
            <service id="pdo" class="PDO">
                <argument>mysql:dbname=basedati</argument>
                <argument>utente</argument>
                <argument>password</argument>
                <call method="setAttribute">
                    <argument type="constant">PDO::ATTR_ERRMODE</argument>
                    <argument type="constant">PDO::ERRMODE_EXCEPTION</argument>
                </call>
            </service>

            <service id="session.handler.pdo" class="Symfony\Component\HttpFoundation\Session\Storage\Handler\PdoSessionHandler">
                <argument type="service" id="pdo" />
                <argument>%pdo.db_options%</argument>
            </service>
        </services>

    .. code-block:: php

        // app/config/config.php
        use Symfony\Component\DependencyInjection\Definition;
        use Symfony\Component\DependencyInjection\Reference;

        $container->loadFromExtension('framework', array(
            ...,
            'session' => array(
                // ...,
                'handler_id' => 'session.handler.pdo',
            ),
        ));

        $container->setParameter('pdo.db_options', array(
            'db_table'      => 'sessione',
            'db_id_col'     => 'id_sessione',
            'db_data_col'   => 'valore_sessione',
            'db_time_col'   => 'tempo_sessione',
        ));

        $pdoDefinition = new Definition('PDO', array(
            'mysql:dbname=basedati',
            'utente',
            'password',
        ));
        $pdoDefinition->addMethodCall('setAttribute', array(\PDO::ATTR_ERRMODE, \PDO::ERRMODE_EXCEPTION));
        $container->setDefinition('pdo', $pdoDefinition);

        $storageDefinition = new Definition('Symfony\Component\HttpFoundation\Session\Storage\Handler\PdoSessionHandler', array(
            new Reference('pdo'),
            '%pdo.db_options%',
        ));
        $container->setDefinition('session.handler.pdo', $storageDefinition);

Questi sono i parametri da configurare:

``db_table``
    Nome della tabella, nella base dati, per le sessioni.

``db_id_col``
    Nome della colonna id della tabella delle sessioni (``VARCHAR(255)`` o maggiore).

``db_data_col``
    Nome della colonna dove salvare il valore della sessione (``TEXT`` o ``CLOB``)

``db_time_col``
    Nome della colonna per la registrazione del tempo della sessione (``INTEGER``)

Condividere le informazioni di connessione della base dati
----------------------------------------------------------

Grazie a questa configurazione, i parametri della connessione alla base dati sono definiti
solo per l'archiviazione dei dati di sessione. La qual cosa è perfetta se si usa
una base dati differente per i dati di sessione.

Ma se si preferisce salvare i dati di sessione nella stessa base dati in cui
risiedono i rimanenti dati del progetto, è possibile utilizzare i parametri di connessione di
``parameter.yml``, richiamandone la configurazione della base dati:

.. configuration-block::

    .. code-block:: yaml

        services:
            pdo:
                class: PDO
                arguments:
                    - "mysql:host=%database_host%;port=%database_port%;dbname=%database_name%"
                    - "%database_user%"
                    - "%database_password%"

    .. code-block:: xml

        <service id="pdo" class="PDO">
            <argument>mysql:host=%database_host%;port=%database_port%;dbname=%database_name%</argument>
            <argument>%database_user%</argument>
            <argument>%database_password%</argument>
        </service>

    .. code-block:: php

        $pdoDefinition = new Definition('PDO', array(
            'mysql:host=%database_host%;port=%database_port%;dbname=%database_name%',
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
        `id_sessione` varchar(255) NOT NULL,
        `valore_sessione` text NOT NULL,
        `tempo_sessione` int(11) NOT NULL,
        PRIMARY KEY (`id_sessione`)
    ) ENGINE=InnoDB DEFAULT CHARSET=utf8;

PostgreSQL
~~~~~~~~~~

Per PostgreSQL, la dichiarazione sarà simile alla seguente:

.. code-block:: sql

    CREATE TABLE sessione (
        id_sessione character varying(255) NOT NULL,
        valore_sessione text NOT NULL,
        tempo_sessione integer NOT NULL,
        CONSTRAINT session_pkey PRIMARY KEY (id_sessione),
    );

Microsoft SQL Server
~~~~~~~~~~~~~~~~~~~~

Per MSSQL, l'istruzione potrebbe essere come la seguente:

.. code-block:: sql

    CREATE TABLE [dbo].[sessione](
        [id_sessione] [nvarchar](255) NOT NULL,
        [valore_sessione] [ntext] NOT NULL,
        [tempo_sessione] [int] NOT NULL,
        PRIMARY KEY CLUSTERED(
            [id_sessione] ASC
        ) WITH (
            PAD_INDEX  = OFF,
            STATISTICS_NORECOMPUTE  = OFF,
            IGNORE_DUP_KEY = OFF,
            ALLOW_ROW_LOCKS  = ON,
            ALLOW_PAGE_LOCKS  = ON
        ) ON [PRIMARY]
    ) ON [PRIMARY] TEXTIMAGE_ON [PRIMARY]
