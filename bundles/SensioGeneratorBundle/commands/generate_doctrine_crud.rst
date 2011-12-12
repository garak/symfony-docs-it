Generare un controllore CRUD in base a un entità Doctrine
=========================================================

Utilizzo
--------

Il comando ``generate:doctrine:crud`` genera un controllore di base per una data entità,
posta in un dato bundle. Questo controllore consente di eseguire le cinque operazioni di
base su un modello.

* Elenco di tutte le righe,
* Visualizzazioone di una riga data, identificata dalla sua chiave primaria,
* Creazione di una nuova riga,
* Modifica di una riga esistente,
* Cancellazione di una riga esistente.

Per impostazione predefinita, il comando gira in modalità interattiva e pone domande per
determinare il nome dell'entità, il prefisso delle rotte e se generare o meno le azioni
di scrittura:

.. code-block:: bash

    php app/console generate:doctrine:crud

Per disattivare la modalità interattiva, usare l'opzione `--no-interaction`, senza
dimenticare di passare tutte le opzioni necessarie:

.. code-block:: bash

    php app/console generate:doctrine:crud --entity=AcmeBlogBundle:Post --format=annotation --with-write --no-interaction

Opzioni disponibili
-------------------

* ``--entity``: Il nome dell'entità, fornito nella notazione breve, contenente il nome
  del bundle che contiene l'entità e il nome dell'entità stessa. Per esempio:
  ``AcmeBlogBundle:Post``:

    .. code-block:: bash

        php app/console generate:doctrine:crud --entity=AcmeBlogBundle:Post

* ``--route-prefix``: Il prefisso da usare per ogni rotta che identifica
  un'azione:

    .. code-block:: bash

        php app/console generate:doctrine:crud --route-prefix=acme_post

* ``--with-write``: (**no**) [valori: yes|no] Se generare o meno le azioni
  `new`, `create`, `edit`, `update` e `delete`:

    .. code-block:: bash

        php app/console generate:doctrine:crud --with-write

* ``--format``: (**annotation**) [valori: yml, xml, php o annotation]
  Determina il formato da usare per i file di configurazione, come quelli delle
  rotte. Il valore predefinito è ``annotation``. La scelta del formato
  ``annotation`` si aspetta che ``SensioFrameworkExtraBundle`` sia già
  installato:

    .. code-block:: bash

        php app/console generate:doctrine:crud --format=annotation

