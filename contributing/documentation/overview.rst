Contribuire alla documentazione
===============================

La documentazione è importante tanto quanto il codice. E segue esattamente gli stessi principi:
DRY, test, facilità di manutenzione, estensibilità, ottimizzazione e refactoring
solo per nominarne alcuni. E certamente, la documentazione ha bug, errori di battitura,
difficoltà di lettura dei tutorial, e molto altro.

Contribuire
------------

Prima di contribuire, è necessario familiarizzare con il
:doc:`formato del linguaggio di demarcazione<format>` usato per la documentazione.

La documentazione di Symfony 2 è ospitata da GitHub:

.. code-block:: bash

    https://github.com/symfony/symfony-docs

Se si vuole inviare una patch, questo è il repository ufficiale della documentazione di
cui fare il `fork`_:

.. code-block:: bash

    $ git clone git://github.com/symfony/symfony-docs.git

Quindi, creare un branch dedicato per i propri cambiamenti (per una migliore organizzazione):

.. code-block:: bash

    $ git checkout -b modifica_pippo_pluto

Si possono ora fare le proprie modifiche direttamente su questo branch e committarle.
Quando si è finito, fare push di questo branch sul *proprio* fork in GitHub e inviare
una pull request. La pull request sarà tra il proprio branch ``modifica_pippo_pluto`` e
il branch ``master`` di ``symfony-docs``.

.. image:: /images/docs-pull-request.png
   :align: center

GitHub spiega in dettagli l'argomento `pull request`_.

.. note::

    La documentazione di Symfony2 è distribuita sotto :doc:`licenza <license>`
    Creative Commons Attribuzione - Condividi allo stesso modo 3.0 Unported.

Riportare una problematica
--------------------------

Il modo più semplice di contribuire è riportando una problematica: un errore di battitura,
un errore grammaticale, un bug nel codice di esempio, e così via.

Passi:

* Segnalare un bug attraverso il tracker dei bug;

* *(facoltativo)* Inviare una patch.

Traduzione
-----------

Leggere il capitolo sulle :doc:`traduzioni <translations>`.

.. _`fork`: http://help.github.com/fork-a-repo/
.. _`pull request`: http://help.github.com/pull-requests/
