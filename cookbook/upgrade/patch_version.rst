.. index::
    single: Aggiornamento; Versione patch

Aggiornare una versione patch (p.e. da 2.6.0 a 2.6.1)
=====================================================

Quando viene rilasciata una versione patch (cambia solo l'ultimo numero), si tratta
di un rilascio che contiene solo risoluzioni di bug. Questo vuol dire che
aggiornare è *molto* facile:

.. code-block:: bash

    $ composer update symfony/symfony

Fatto! Non si dovrebbero incontrare problemi di retrocompatibilità, né aver
bisogno di modificare alcunché nel proprio codice. Questo perché, all'inizio
del progetto, il file ``composer.json`` incluso in Symfony ha usato un vincolo
come ``2.6.*``,  in cui solamente l'*ultimo* numero della versione cambierà a ogni
aggiornamento.

.. tip::

    Si raccomanda di aggiornare alle versioni patch il più presto possibile, perché
    potrebbero risolvere importanti bug di sicurezza.

.. include:: /cookbook/upgrade/_update_all_packages.rst.inc
