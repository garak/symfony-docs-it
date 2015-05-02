.. index::
    single: Modificare la struttura predefinita della cartelle 

Modificare la struttura predefinita della cartelle
==================================================

Symfony viene distribuito con una struttura di cartelle predefinita. Si può
facilmente modificare tale struttura e crearne una propria. La struttura
predefinita è:

.. code-block:: text

    progetto/
    ├─ app/
    │  ├─ cache/
    │  ├─ config/
    │  ├─ logs/
    │  └─ ...
    ├─ src/
    │  └─ ...
    ├─ vendor/
    │  └─ ...
    └─ web/
       ├─ app.php
       └─ ...

.. _override-cache-dir:

Spostare la cartella ``cache``
------------------------------

Si può spostare la cartella della cache, sovrascrivendo il metodo ``getCacheDir``
nella classe ``AppKernel`` dell'applicazione::

    // app/AppKernel.php

    // ...
    class AppKernel extends Kernel
    {
        // ...

        public function getCacheDir()
        {
            return $this->rootDir.'/'.$this->environment.'/cache';
        }
    }

``$this->rootDir`` è il percorso assoluto della cartella ``app`` e ``$this->environment``
è l'ambiente corrente (p.e. ``dev``). In questo caso, abbiamo spostato la posizione
della cartella della cache in ``app/{ambiente}/cache``.

.. warning::

    Si dovrebbe mantenere una cartella ``cache`` diversa per ogni ambiente,
    altrimenti potrebbe succedere qualcosa di inaspettato. Ogni ambiente genera
    la cache dei propri file di configurazione e quindi ognuno necessita della propria
    cartella, per memorizzare tali file.

.. _override-logs-dir:

Spostare la cartella ``logs``
-----------------------------

Si può spostare la cartella ``logs`` allo stesso modo de cartella ``cache``,
con la sola differenza che occorre sovrascrivere il metodo
``getLogDir``::

    // app/AppKernel.php

    // ...
    class AppKernel extends Kernel
    {
        // ...

        public function getLogDir()
        {
            return $this->rootDir.'/'.$this->environment.'/logs';
        }
    }

Abbiamo cambiato la posizione della cartella in ``app/{ambiente}/logs``.

.. _override-web-dir:

Spostare la cartella ``web``
----------------------------

Se occorre rinominare o spostare la cartella ``web``, è sufficiente garantire
che il percorso della cartella ``app`` sia corretto nei front controller
``app.php`` e ``app_dev.php``. Se si rinomina solamente la cartella,
non serve far nulla. Se invece la si sposta in un'altra posizione, potrebbe essere
necesario modificare i percorsi nei file::

    require_once __DIR__.'/../Symfony/app/bootstrap.php.cache';
    require_once __DIR__.'/../Symfony/app/AppKernel.php';

Da Symfony 2.1 (in cui è stato introdotto Composer), occorre anche modificare
l'opzione ``extra.symfony-web-dir`` nel file ``composer.json``:

.. code-block:: javascript

    {
        ...
        "extra": {
            ...
            "symfony-web-dir": "nuova_cartella_web"
        }
    }

.. tip::

    Alcuni host condivisi hanno una cartella radice del web chiamata ``public_html``.
    Rinominare la cartella da ``web`` a ``public_html`` è un modo per far funzionare
    un progetto Symfony su un host condiviso. Un altro modo consiste nel fare deploy
    dell'applicazione in una cartella fuori dalla radice del web, cancellare la
    cartella ``public_html`` e sostituirla con un collegamento simbolico alla cartella
    ``web`` del progetto.

.. note::

    Se si usa AsseticBundle, occorre configurarlo in modo che possa usare la
    cartella ``web`` corretta:

    .. configuration-block::

        .. code-block:: yaml

            # app/config/config.yml

            # ...
            assetic:
                # ...
                read_from: "%kernel.root_dir%/../../public_html"

        .. code-block:: xml

            <!-- app/config/config.xml -->

            <!-- ... -->
            <assetic:config read-from="%kernel.root_dir%/../../public_html" />

        .. code-block:: php

            // app/config/config.php

            // ...
            $container->loadFromExtension('assetic', array(
                // ...
                'read_from' => '%kernel.root_dir%/../../public_html',
            ));

    Ora basta eseguire nuovamente il dump delle risorse e l'applicazione dovrebbe
    funzionare:

    .. code-block:: bash

        $ php app/console cache:clear --env=prod
        $ php app/console assetic:dump --env=prod --no-debug

Spostare la cartella ``vendor``
-------------------------------

Per spostare la cartella ``vendor``, si devono modificare i file
 ``app/autoload.php`` e ``composer.json``.

La modifica in ``composer.json`` sarà simile a questa:

.. code-block:: json

    {
        ...
        "config": {
            "bin-dir": "bin",
            "vendor-dir": "/una/cartella/vendor"
        },
        ...
    }

In ``app/autoload.php``, si deve modificare il percorso che porta al file
``vendor/autoload.php``::

    // app/autoload.php
    // ...
    $loader = require '/una/cartella/vendor/autoload.php';

.. tip::

    Queste modifiche sono interessanti se si lavora in un ambiente virtuale
    e non si può usare NFS. Per esempio, se si esegue un'applicazione Symfony usando
    Vagrant/VirtualBox in un sistema operativo ospite.
