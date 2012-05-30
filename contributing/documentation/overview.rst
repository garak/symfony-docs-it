Contribuire alla documentazione
===============================

La documentazione è importante tanto quanto il codice. E segue esattamente gli stessi principi:
DRY, test, facilità di manutenzione, estensibilità, ottimizzazione e refactoring,
solo per nominarne alcuni. E certamente la documentazione ha bug, errori di battitura, difficoltà di lettura dei tutorial
e molto altro.

Contribuire
-----------

Prima di contribuire, è necessario famigliarizzare con il :doc:`linguaggio di markup<format>` 
usato per la documentazione.

La documentazione di Symfony 2 è ospitata da GitHub:

.. code-block:: text

    https://github.com/symfony/symfony-docs

Se si vuole inviare una patch, fare un `fork`_ del repository ufficiale su GitHub
e fare un clone:

.. code-block:: bash

    git://github.com/symfony/symfony-docs.git

A meno di non documentare una caratteristica aggiunta in Symfony 2.1, le modifiche
vanno basate sul ramo 2.0, non sul ramo master. Per poterlo fare,
eseguire un checkout del ramo 2.0 prima del prossimo passo:

.. code-block:: bash

    $ git checkout 2.0

Quindi, creare un ramo dedicato per le proprie modifiche (per questioni organizzative):

.. code-block:: bash

    $ git checkout -b miglioramenti_di_pippo_e_pluto

Si possono ora eseguire le proprie modifiche in tale ramo. Quando si ha finito,
fare il push di quest ramo nel *proprio* fork su GitHub e richiedere un pull.
La richiesta di pull sarà tra il proprio ramo ``miglioramenti_di_pippo_e_pluto`` e
il ramo ``master`` di ``symfony-docs``.

.. image:: /images/docs-pull-request.png
   :align: center

Se le proprie modifiche sono basate sul ramo 2.0, occorre seguire il collegamento di modifica del commit
e cambiare il ramo base in @2.0:

.. image:: /images/docs-pull-request-change-base.png
   :align: center

GitHub spiega l'argomento in modo dettagliato, su `richieste di pull`_.

.. note::

    La documentazione di Symfony2 è rilasciata sotto :doc:`licenza <license>`
    Creative Commons Attribuzione - Condividi allo stesso modo 3.0 Unported.

.. tip::

    Le modifiche appaiono sul sito symfony.com website non oltre 15 minuti
    dopo il merge della richiesta di pull nella documentazione. Si può verificare
    se le proprie modifiche non abbiano introdotto problemi di markup, guardando la
    pagina `Errori di build della documentazione`_ (aggiornata ogni notte alle 3,
    quando il server ricostruisce la documentazione).

Segnalare una problematica
--------------------------

Il modo più semplice di contribuire è segnalando una problematica: un errore di battitura,
un errore grammaticale, un bug nel codice di esempio, e così via

Passi:

* Segnalare un bug attraverso il bug Tracker;

* *(opzionale)* Inviare una patch.

Traduzione
----------

Leggere la :doc:`documentazione <translations>` apposita.

.. _`fork`: http://help.github.com/fork-a-repo/
.. _`richieste di pull`: http://help.github.com/pull-requests/
.. _`Errori di build della documentazione`: http://symfony.com/doc/build_errors
