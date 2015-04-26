.. index::
   single: Sessione; Memorizzazione su base dati

Usare PdoSessionStorage per salvare le sessioni nella base dati
===============================================================

.. caution::

    Ci sono stati alcuni cambiamenti non retrocomptabili in Symfony 2.6: lo schema della
    base dati è cambiato leggermente. Vedere :ref:`Symfony 2.6 Changes <pdo-session-handle-26-changes>`
    per dettagli.

Normalmente, nella gestione delle sessioni, Symfony2 salva le relative informazioni
all'interno di file. Solitamente, i siti web di dimensioni medio grandi utilizzano
la basi dati, invece dei file, per salvare i dati di sessione. Questo perché le basi dati
sono più semplici da utilizzare e sono più scalabili in ambienti multi-webserver.

Symfony ha, al suo interno, una soluzione per l'archiviazione delle sessioni su base dati, chiamata
:class:`Symfony\\Component\\HttpFoundation\\Session\\Storage\\PdoSessionStorage`.
Per utilizzarla è sufficiente cambiare alcuni parametri nel file di configurazione principale:

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        framework:
            session:
                # ...
                handler_id: session.handler.pdo

        services:
            session.handler.pdo:
                class:     Symfony\Component\HttpFoundation\Session\Storage\Handler\PdoSessionHandler
                public:    false
                arguments:
                    - "mysql:dbname=miabasedati"
                    - { db_username: mioutente, db_password: miapassword }

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <framework:config>
            <framework:session handler-id="session.handler.pdo" cookie-lifetime="3600" auto-start="true"/>
        </framework:config>

        <services>
            <service id="session.handler.pdo" class="Symfony\Component\HttpFoundation\Session\Storage\Handler\PdoSessionHandler" public="false">
                <argument>mysql:dbname=miabasedati</agruement>
                <argument type="collection">
                    <argument key="db_username">mioutente</argument>
                    <argument key="db_password">miapassword</argument>
                </argument>
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

        $storageDefinition = new Definition('Symfony\Component\HttpFoundation\Session\Storage\Handler\PdoSessionHandler', array(
            'mysql:dbname=miabasedati',
            array('db_username' => 'mioutente', 'db_password' => 'miapassword')
        ));
        $container->setDefinition('session.handler.pdo', $storageDefinition);

Configurare i nomi di tabelle e colonne
---------------------------------------

Ci deve essere una tabella ``sessioni``, con varie colonne.
Il nome della tabella e i nomi delle colonne possono essere configurati, passando
un secondo array di parametri a ``PdoSessionHandler``:

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        services:
            # ...
            session.handler.pdo:
                class:     Symfony\Component\HttpFoundation\Session\Storage\Handler\PdoSessionHandler
                public:    false
                arguments:
                    - "mysql:dbname=miabasedati"
                    - { db_table: sessions, db_username: mioutente, db_password: miapassword }

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <services>
            <service id="session.handler.pdo" class="Symfony\Component\HttpFoundation\Session\Storage\Handler\PdoSessionHandler" public="false">
                <argument>mysql:dbname=miabasedati</agruement>
                <argument type="collection">
                    <argument key="db_table">sessions</argument>
                    <argument key="db_username">mioutente</argument>
                    <argument key="db_password">miapassword</argument>
                </argument>
            </service>
        </services>

    .. code-block:: php

        // app/config/config.php

        use Symfony\Component\DependencyInjection\Definition;
        // ...

        $storageDefinition = new Definition('Symfony\Component\HttpFoundation\Session\Storage\Handler\PdoSessionHandler', array(
            'mysql:dbname=miabasedati',
            array('db_table' => 'sessions', 'db_username' => 'mioutente', 'db_password' => 'miapassword')
        ));
        $container->setDefinition('session.handler.pdo', $storageDefinition);

.. versionadded:: 2.6
    L'opzione ``db_lifetime_col`` è stata introdotta in Symfony 2.6. In precedenza,
    tale colonna non esisteva.

Questi sono i parametri da configurare:

``db_table`` (predefinito ``sessions``):
    Nome della tabella, nella base dati, per le sessioni.

``db_id_col`` (predefinito ``sess_id``):
    Nome della colonna id della tabella delle sessioni (VARCHAR(128) o maggiore).

``db_data_col`` (predefinito ``sess_data``):
    Nome della colonna dove salvare il valore della sessione (BLOB)

``db_time_col`` (predefinito ``sess_time``):
    Nome della colonna per la registrazione del tempo della sessione (INTEGER)

``db_lifetime_col`` (predefinito ``sess_lifetime``):
    Nome della colonna per il tempo di vita della sessione (INTEGER).


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
            session.handler.pdo:
                class:     Symfony\Component\HttpFoundation\Session\Storage\Handler\PdoSessionHandler
                public:    false
                arguments:
                    - "mysql:host=%database_host%;port=%database_port%;dbname=%database_name%"
                    - { db_username: %database_user%, db_password: %database_password% }

    .. code-block:: xml

        <service id="session.handler.pdo" class="Symfony\Component\HttpFoundation\Session\Storage\Handler\PdoSessionHandler" public="false">
            <argument>mysql:host=%database_host%;port=%database_port%;dbname=%database_name%</agruement>
            <argument type="collection">
                <argument key="db_username">%database_user%</argument>
                <argument key="db_password">%database_password%</argument>
            </argument>
        </service>

    .. code-block:: php

        $storageDefinition = new Definition('Symfony\Component\HttpFoundation\Session\Storage\Handler\PdoSessionHandler', array(
            'mysql:host=%database_host%;port=%database_port%;dbname=%database_name%',
            array('db_username' => '%database_user%', 'db_password' => '%database_password%')
        ));

Esempi di dichiarazioni SQL
---------------------------

.. _pdo-session-handle-26-changes:

.. sidebar:: Modifiche allo schema necessarie se si aggiorna a Symfony 2.6

    Se si usava ``PdoSessionHandler`` prima di Symfony 2.6 e si vuole aggiornare, occorrono
    alcune modifiche alla tabella delle sessioni:

    * Si deve aggiungere una colonna per la vita della sessione (predefinito: ``sess_lifetime``), di
      tipo INTEGER;
    * Il tipo della colonna dei dati (predefinito: ``sess_data``) va cambiato in
      BLOB.

    Vedere le istruzioni SQL più avanti, per maggiori dettagli.

    Per mantenere la vecchia funzionalità, cambiare il nome della classe
    in ``LegacyPdoSessionHandler``, al posto di ``PdoSessionHandler`` (la
    classe legacy è stata aggiunta in Symfony 2.6.2).

MySQL
~~~~~

L'istruzione SQL per creare la necessaria tabella nella base dati potrebbe essere
simile alla seguente (MySQL):

.. code-block:: sql

    CREATE TABLE `sessione` (
        `id_sessione` VARBINARY(128) NOT NULL PRIMARY KEY,
        `valore_sessione` BLOB NOT NULL,
        `tempo_sessione` INTEGER UNSIGNED NOT NULL,
        `vita_sessione` MEDIUMINT NOT NULL
    ) COLLATE utf8_bin, ENGINE = InnoDB;

.. note::

    Una colonna di tipo ``BLOB`` può memorizzare fino a 64 kb. Se i dati memorizzati nella
    sessione sono maggiori, potrebbe essere lanciata un'eccezione oppure la sessione potrebbe
    essere azzerata in modo silenzioso. Si consideri l'uso di ``MEDIUMBLOB``, nel caso in cui
    occorra più spazio.

PostgreSQL
~~~~~~~~~~

Per PostgreSQL, la dichiarazione sarà simile alla seguente:

.. code-block:: sql

    CREATE TABLE sessione (
        id_sessione VARCHAR(128) NOT NULL PRIMARY KEY,
        valore_sessione BYTEA NOT NULL,
        tempo_sessione INTEGER NOT NULL,
        vita_sessione INTEGER NOT NULL
    );

Microsoft SQL Server
~~~~~~~~~~~~~~~~~~~~

Per MSSQL, l'istruzione potrebbe essere come la seguente:

.. code-block:: sql

    CREATE TABLE [dbo].[sessione](
        [id_sessione] [nvarchar](255) NOT NULL,
        [valore_sessione] [ntext] NOT NULL,
        [tempo_sessione] [int] NOT NULL,
        [vita_sessione] [int] NOT NULL,
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
