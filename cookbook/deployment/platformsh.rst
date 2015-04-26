.. index::
   single: Deploy; Deploy su Platform.sh

Deploy su Platform.sh
=====================

Questa ricetta descrive passo-passo il deploy di un'applicazione web Symfony su
`Platform.sh`_. Si può saperne di più sull'uso di Symfony con Platform.sh leggendo
la `documentazione di Platform.sh`_.

Deploy di un sito esistente
---------------------------

In questa guida si ipotizzerà di avere del codice già versionato con Git.

Creare un progetto su Platform.sh
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Ci seve iscrivere a un `progetto Platform.sh`_. Scegliere il piano sviluppo
e seguire il processo di checkout. Una volta che il progetto è pronto, dargli un nome
e scegliere **Import an existing site**.

Preparare l'applicazione
~~~~~~~~~~~~~~~~~~~~~~~~

Per il deploy di u'applicazione Symfony su Platform.sh, basta aggiungere un file
``.platform.app.yaml`` nella radice del repository Git. Tale file dira à
Platform.sh come eseguire il deploy dell'applicazione (si possono approfondire i
`file di configurazione Platform.sh`_).

.. code-block:: yaml

    # .platform.app.yaml

    # Questo file descrive un'applicazione. Si possono avere più applicazioni
    # nello stesso progetto.

    # Il nome dell'app. Deve essere univoco in un progetto.
    name: mioprogetto

    # Il toolstack usato per costruire l'applicazione.
    toolstack: "php:symfony"

    # Le relazioni tra applicazione e servizi o altre applicazioni.
    # La parte a sinistra è il nome della relazione, esposto
    # all'applicazione nella variabile PLATFORM_RELATIONSHIPS. La parte
    # destra è nella forma `<servizio>:<endpoint>`.
    relationships:
        database: "mysql:mysql"

    # La configurazione dell'app, quando esposta al web.
    web:
        # La cartella pubblica dell'app, relativa alla sua radice.
        document_root: "/web"
        # Il front controller a cui inviare richieste non statiche.
        passthru: "/app.php"

    # La dimensione del disco persistente dell'applicazione (in MB).
    disk: 2048

    # I percorsi da montare al deploy del pacchetto.
    mounts:
        "/app/cache": "shared:files/cache"
        "/app/logs": "shared:files/logs"

    # Gli agganci da eseguire al deploy del pacchetto.
    hooks:
        build: |
          rm web/app_dev.php
          app/console --env=prod assetic:dump --no-debug
        deploy: |
          app/console --env=prod cache:clear

Come best practice, si dovrebbe aggiungere una cartella ``.platform`` nella radice del
repository Git, con i seguenti file:

.. code-block:: yaml

    # .platform/routes.yaml
    "http://{default}/":
        type: upstream
        upstream: "php:php"

.. code-block:: yaml

    # .platform/services.yaml
    mysql:
        type: mysql
        disk: 2048

Si può trovare un esempio di queste configurazioni su `GitHub`_. L'elenco dei
`servizi disponibili`_ si trova nella documentazione di Platform.sh.

Configurare l'accesso alla base dati
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Platform.sh sovrascrive la configurazione specifica della base dati, importando il
seguente file::

    // app/config/parameters_platform.php
    <?php
    $relationships = getenv("PLATFORM_RELATIONSHIPS");
        if (!$relationships) {
            return;
    }

    $relationships = json_decode(base64_decode($relationships), true);

    foreach ($relationships['database'] as $endpoint) {
        if (empty($endpoint['query']['is_master'])) {
          continue;
        }

        $container->setParameter('database_driver', 'pdo_' . $endpoint['scheme']);
        $container->setParameter('database_host', $endpoint['host']);
        $container->setParameter('database_port', $endpoint['port']);
        $container->setParameter('database_name', $endpoint['path']);
        $container->setParameter('database_user', $endpoint['username']);
        $container->setParameter('database_password', $endpoint['password']);
        $container->setParameter('database_path', '');
    }

    # Memorizza la sessione in /tmp.
    ini_set('session.save_path', '/tmp/sessions');

Assicurarsi che questo file sia negli *imports*:

.. code-block:: yaml

    # app/config/config.yml
    imports:
        - { resource: parameters_platform.php }

Deploy dell'applicazione
~~~~~~~~~~~~~~~~~~~~~~~~

Ora si deve aggiungere un remote verso Platform.sh nel repository Git (copiare il
comando visibile nell'interfaccia web di Platform.sh):

.. code-block:: bash

    $ git remote add platform [ID-PROGETTO]@git.[CLUSTER].platform.sh:[ID-PROGETTO].git

``ID-PROGETTO``
    Identificatore univoco del progetto. Qualcosa come ``kjh43kbobssae``
``CLUSTER``
    Posizione del server verso cui avviene il deploy del progetto. Può essere ``eu`` oppure ``us``

Eseguire un commit dei file specifici di Platform.sh, creati nella sezione precedente:

.. code-block:: bash

    $ git add .platform.app.yaml .platform/*
    $ git add app/config/config.yml app/config/parameters_platform.php
    $ git commit -m "Aggiunge i file di configurazione di Platform.sh"

Eseguire un push al nuovo remote:

.. code-block:: bash

    $ git push platform master

Ecco fatto! Il deploy dell'applicazione su Platform.sh è concluso e presto si sarà in grado
di accedervi tramite browser.

Si dovra eseguire d'ora in poi il push di ogni modifica del codice su Git, per eseguire
un nuovo deploy su Platform.sh.

Si possono trovare più informazioni su `migrazione di basi dati e file <migrate-existing-site>`_ nella
documentazione di Platform.sh.

Deploy di un nuovo sito
-----------------------

Si può creare un nuovo `progetto Platform.sh`_. Scegliere il piano di sviluppo e
seguire il processo di checkout.

Una volta pronto il progetto, dargli un nome e scegliere **Create a new site**.
Scegliere lo stack *Symfony* e un nuovo punto di partenza, come *Standard*.

Fatto! L'applicazione Symfony sarà inizializzata e deployata. Sarà presto
visibile tramite browser.

.. _`Platform.sh`: https://platform.sh
.. _`documentazione di Platform.sh`: https://docs.platform.sh/toolstacks/symfony/symfony-getting-started
.. _`progetto Platform.sh`: https://marketplace.commerceguys.com/platform/buy-now
.. _`file di configurazione Platform.sh`: https://docs.platform.sh/reference/configuration-files
.. _`GitHub`: https://github.com/platformsh/platformsh-examples
.. _`servizi disponibili`: https://docs.platform.sh/reference/configuration-files/#configure-services
.. _`migrate-existing-site`: https://docs.platform.sh/toolstacks/symfony/migrate-existing-site/
