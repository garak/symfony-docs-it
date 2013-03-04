.. index::
    single: Console; Uso

Uso di Console
==============

Oltre alle opzioni specificate per i comandi, ci sono anche alcune opzioni
predefinite, oltre che alcuni comandi predefiniti per il componente Console.

.. note::

    Questi esempi ipotizzano che sia stato aggiunto un file ``app/console``, da eseguire
    dalla linea di comando::

        #!/usr/bin/env php
        # app/console
        <?php

        use Symfony\Component\Console\Application;

        $application = new Application();
        // ...
        $application->run();

Comandi predefiniti
~~~~~~~~~~~~~~~~~~~

C'è un comando ``list``, che mostra tutti i comandi registrati e le
opzioni disponibili:

.. code-block:: bash

    $ php app/console list

Si può ottenere lo stesso risultato, non eseguendo alcun comando

.. code-block:: bash

    $ php app/console

Il comando help elenca le informazioni di aiuto per il comando specificato. Per
esempio, per ottenere aiuto sul comando ``list``:

.. code-block:: bash

    $ php app/console help list

Eseguendo ``help`` senza specificare alcun comando mostrerà le opzioni globali:

.. code-block:: bash

    $ php app/console help

Opzioni globali
~~~~~~~~~~~~~~~

Si possono ottenere informazioni per ogni comando, con l'opzione ``--help``. Per ottenere
aiuto per il comando ``list``:

.. code-block:: bash

    $ php app/console list --help
    $ php app/console list -h

Si può avere in elenco meno verboso con:

.. code-block:: bash

    $ php app/console list --quiet
    $ php app/console list -q

Si possono ottenere messaggi più verbosi (se supportato dal comando)
con:

.. code-block:: bash

    $ php app/console list -verbose
    $ php app/console list -v

Se si impostano, in modo facoltativo, nome e versione dell'applicazione::

    $application = new Application('Applicazione Acme Console', '1.2');

si può usare:

.. code-block:: bash

    $ php app/console list -version
    $ php app/console list -V

per ottnere queste informazioni:

.. code-block:: text

    Applicazioni Acme Console version 1.2

Se non si forniscono entrambi i parametri, si otterrà solamente:

.. code-block:: text

    console tool

Si può forzare la colorazione ANSI con:

.. code-block:: bash

    $ php app/console list --ansi

o disattivarla con:

.. code-block:: bash

    $ php app/console list --no-ansi

Si possono aggirare le domande interattive del comando in esecuzione con:

.. code-block:: bash

    $ php app/console list --no-interaction
    $ php app/console list -n

Sintassi breve
~~~~~~~~~~~~~~

Non occorre scrivere i nomi interi dei comandi. Basta scrivere la più breve parte
non ambigua di un comando, per eseguirlo. Quindi, se non ci sono comandi con un nome
simile, si può richiamare ``help`` in questo modo:

.. code-block:: bash

    $ php app/console h

Se si hanno comandi che usano ``:`` per gli spazi dei nomi, occorre scrivere un pezzo
di testo non ambiguo per ogni parte. Se è stato creato il comando
``demo:saluta``, come mostrato in :doc:`/components/console/introduction`, lo si può
eseguire con:

.. code-block:: bash

    $ php app/console d:g Fabien

Se si sceglie un comando troppo breve e quindi ambiguo (cioè più di un comando
corrisponde), non verrà eseguito alcun comando,
ma verranno mostrati dei sugerrimenti sui possibili comandi da eseguire.
