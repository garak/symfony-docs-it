Generare lo scheletro di un nuovo bundle
========================================

Utilizzo
--------

Il comando ``generate:bundle`` genera la struttura di un nuovo bundle, attivandolo
automaticamente nell'applicazione.

Per impostazione predefinita, il comando è eseguito in modalità interattiva e pone domande
volte a determinare nome, posizione, formato di configurazione e struttura predefinita del
bundle:

.. code-block:: bash

    $ php app/console generate:bundle

Per disattivare la modalità interattiva, usare l'opzione `--no-interaction`, senza
dimenticare di passare tutte le opzioni necessarie:

.. code-block:: bash

    $ php app/console generate:bundle --namespace=Acme/Bundle/BlogBundle --no-interaction

Opzioni disponibili
-------------------

* ``--namespace``: Lo spazio dei nomi del bundle da creare. Lo spazio dei nomi dovrebbe
  iniziare con un nome di "venditore", come il nome della propria azienda, del
  progetto o del cliente, seguito da uno o più sotto-spazi dei nomi, facoltativi, e
  terminare col nome del bundle stesso (che deve avere "Bundle" come
  suffisso):

  .. code-block:: bash

        $ php app/console generate:bundle --namespace=Acme/Bundle/BlogBundle

* ``--bundle-name``: Il nomde del bundle, opzionale. Deve essere una stringa con suffisso
  ``Bundle``:

    .. code-block:: bash

        $ php app/console generate:bundle --bundle-name=AcmeBlogBundle

* ``--dir``: La cartella in cui mettere il bundle. Per convenzione, il comando individua
  e usa la cartella ``src/`` dell'applicazione:

    .. code-block:: bash

        $ php app/console generate:bundle --dir=/var/www/myproject/src

* ``--format``: (**annotation**) [valori: yml, xml, php o annotation]
  Determina il formato da usare per i file di configurazione, come quello delle
  rotte. Il valore predefinito è ``annotation``. La scelta del formato
  ``annotation`` si aspetta che ``SensioFrameworkExtraBundle`` sia già
  installato:

    .. code-block:: bash

        $ php app/console generate:bundle --format=annotation

* ``--structure``: (**no**) [valori: yes|no] Se generare o meno una struttura di cartelle
  completa, incluse le cartelle pubbliche vuote per la documentazione, per le risorse
  del web e per i dizionari delle traduzioni:

    .. code-block:: bash

        $ php app/console generate:bundle --structure=yes
