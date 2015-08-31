.. index::
    single: Aggiornamento; Versione minore

Aggiornare una versione minore (p.e. da 2.5.3 a 2.6.1)
======================================================

Se si sta aggiornando una versione minore (in cui cambia il numero di mezzo), si
dovrebbero incontrare anche modifiche alla retrocompatibilità non significanti.
Per dettagli, vedere :doc:`/contributing/code/bc`.

Tuttavia, sono possibili alcune modifiche non retrocompatibili, che ora vedremo
come affrontare.

Ci sono due passi nell'aggiornamento:

#. :ref:`Aggiornare Symfony tramite Composer <upgrade-minor-symfony-composer>`;
#. :ref:`Aggiornare il proprio codice <upgrade-minor-symfony-code>`.

.. _`upgrade-minor-symfony-composer`:

1) Aggiornare le librerie Symfony tramite Composer
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Prima, occorre aggiornare Symfony, modificando il file ``composer.json`` per
usare la nuova versione:

.. code-block:: json

    {
        "...": "...",

        "require": {
            "symfony/symfony": "2.6.*",
        },
        "...": "...",
    }

Quindi, usare Composer per scaricare le nuove versioni delle librerie:

.. code-block:: bash

    $ composer update symfony/symfony

.. include:: /cookbook/upgrade/_update_all_packages.rst.inc

.. _`upgrade-minor-symfony-code`:

2) Aggiornare il proprio codice per funzionare con la nuova versione
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

In teoria, dovrebbe essere tutto a posto. Tuttavia, *potrebbe* essere necessario eseguire alcune
modifiche nel proprio codice, per far funzionare tutto. Inoltre, alcune caratteristiche usate
potrebbero funzionare, ma essere ora deprecate. Questo non è in realtà un problema,
ma essere consapevoli di tali deprecati può essere un primo passo per sistemarli per tempo.

Ogni versione di Symfony dispone di un file UPGRADE (come `UPGRADE-2.7.md`_),
che descrive tali modifiche. Se si seguono le istruzioni presenti nel documento
e si aggiorna il codice di conseguenza, dovrebbe essere sicuro
aggiornare in futuro.

Questi documenti si possono trovare anche nel `repository di Symfony`_.

.. _`repository di Symfony`: https://github.com/symfony/symfony
.. _`UPGRADE-2.7.md`: https://github.com/symfony/symfony/blob/2.7/UPGRADE-2.7.md