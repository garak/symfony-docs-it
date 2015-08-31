.. index::
    single: Aggiornamento; Versione maggiore

Aggiornare una versione maggiore (p.e. da 2.7.0 a 3.0.0)
========================================================

Dopo qualche anno, Symfony rilascia una nuova versione maggiore (cambia il
primo numero). Tali rilasci sono meno facili da aggiornare, perché non garantiscono
retrocompatibilità. Tuttavia, Symfony cerca di rendere questo processo di aggiornamento
il più liscio possibile.

Questo vuol dire che si può aggiornare gran parte del codice prima che la versione maggiore
sia effettivamente rilasciata. Questa è definita *compatibilità in avanti*.

Ci sono alcuni passi da compiere per aggiornare a una versione maggiore:

#. :ref:`Eliminare i deprecati nel codice <upgrade-major-symfony-deprecations>`;
#. :ref:`Aggiornare alla nuova versione tramite Composer <upgrade-major-symfony-composer>`.
#. :ref:`Aggiornare il codice per funzionare con la nuova versione <upgrade-major-symfony-after>`

.. _upgrade-major-symfony-deprecations:

1) Eliminare i deprecati nel codice
-----------------------------------

Durante il ciclo di vita di una versione maggiore, vengono aggiunte nuove caratteristiche e possono
cambiare firme di metodi e usi di API pubbliche. Tuttavia,
:doc:`le versioni minori </cookbook/upgrade/minor_version>` non dovrebbero contenere modifiche
non retrocompatibili. Per poterlo fare, il "vecchio" codice (funzioni,
classi, ecc.) funziona ancora, ma è segnalato come *deprecato*, cioè
sarà rimosso o modificato nel futuro e quindi non va più usato.

Al rilascio della versione maggiore (p.e. 3.0.0), tutte le caratteristiche deprecate
sono rimosse. Quindi, se il codice è stato aggiornato per non usare più
tali funzionalità deprecate nell'ultima versione prima di quella maggiore (p.e.
2.8.*), si dovrebbe poter aggiornare senza problemi.

Per offrire un aiuto, le ultime versioni minori segnaleranno il codice deprecato.
Per esempio,, 2.7 e 2.8 mostrano informazioni sul codice deprecato. Quando si apre
un'applicazione in :doc:`ambiente dev </cookbook/configuration/environments>`,
tali notifiche sono mostrate nella barra di debug del web:

.. image:: /images/cookbook/deprecations-in-profiler.png

Deprecati in PHPUnit
~~~~~~~~~~~~~~~~~~~~

PHPUnit gestirà gli avvisi sul codice depracato come se fossero errori. Questo vuol dire
che tutti i test falliscono.

Per evitare questo comportamento, si può installare il bridge per PHPUnit:

.. code-block:: bash

    $ composer require symfony/phpunit-bridge

Ora i test passeranno e si potrà avere un utile sommario delle informazioni sul
codice deprecato, alla fine dei test:

.. code-block:: text

    $ phpunit
    ...

    OK (10 tests, 20 assertions)

    Remaining deprecation notices (6)

    The "request" service is deprecated and will be removed in 3.0. Add a typehint for
    Symfony\Component\HttpFoundation\Request to your controller parameters to retrieve the
    request instead: 6x
        3x in PageAdminTest::testPageShow from Symfony\Cmf\SimpleCmsBundle\Tests\WebTest\Admin
        2x in PageAdminTest::testPageList from Symfony\Cmf\SimpleCmsBundle\Tests\WebTest\Admin
        1x in PageAdminTest::testPageEdit from Symfony\Cmf\SimpleCmsBundle\Tests\WebTest\Admin

.. _upgrade-major-symfony-composer:

2) Aggiornare alla nuova versione tramite Composer
--------------------------------------------------

Se il codice è scevro di deprecati, si può aggiornare Symfony tramite
Composer, modificando il file ``composer.json``:

.. code-block:: json

    {
        "...": "...",

        "require": {
            "symfony/symfony": "3.0.*",
        },
        "...": "...",
    }

Usare quindi Composer per scaricare le nuove versioni delle librerie:

.. code-block:: bash

    $ composer update symfony/symfony

.. include:: /cookbook/upgrade/_update_all_packages.rst.inc

.. _upgrade-major-symfony-after:

3) Aggiornare il codice per funzionare con la nuova versione
------------------------------------------------------------

A questo punto, ci sono buone possibilità di aver concluso! Tuttavia, la versione maggiore
successiva *potrebbe* contenere rotture di retrocompatibilità, perché non è sempre possibile garantirla.
Assicurarsi di leggere il documento ``UPGRADE-X.0.md`` (dove X è la nuova versione maggiore)
incluso nel repository di Symfony, per conoscere ogni rottura di retrocompatibilità di cui occorre
essere a conoscenza.
