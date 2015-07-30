Git
===

Questo documento spiega alcune convezioni e specifiche del modo in cui viene gestito
il codice di Symfony con Git.

Richieste di pull
-----------------

A ogni merge di una richiesta di pull, tutte le informazioni contenute nella richiesta
di pull (commenti inclusi) sono salvate nel repository.

Si possono trovare facilmente i merge delle richieste di pull, perché i messaggi di commit
seguono sempre questo schema:

.. code-block:: text

    merged branch NOME_UTENTE/NOME_RAMO (PR #1111)

Il riferimento alla richiesta di pull consente di dare un'occhiata alla richiesta di pull originale su
Github: https://github.com/symfony/symfony/pull/1111. Ma tutte le informazioni
ottenibili da Github sono disponibili anche nel repository stesso.

Il messaggio del commit di merge contiene il messaggio originale dell'autore delle
modifiche. Spesso, questo può aiutare a capire l'argomento delle modifiche e le
ragioni che ne sono all'origine.

Inoltre, la discussione completa che potrebbe esserci stata viene ugualmente
memorizzata come nota Git (prima del 22 marzo 2013, la discussione era parte
del messaggio principale di merge). Per accedere a tali note, aggiungere la riga
seguente al proprio file ``.git/config``:

.. code-block:: ini

    fetch = +refs/notes/*:refs/notes/*

Dopo un fetch, per ottenere la discussione su un commit basta
aggiungere ``--show-notes=github-comments`` al comando ``git show``:

.. code-block:: bash

    $ git show HEAD --show-notes=github-comments
