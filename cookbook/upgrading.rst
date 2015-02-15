Aggiornare un progetto Symfony
==============================

Quindi è uscito un nuovo rilascio di Symfony e si vuole aggiornare, bene! Fortunatamente,
poiché Symfony mantiene molto bene la retrocompatibilità, il processo *dovrebbe*
essere facile.

Ci sono due tipi di aggiornamenti, che differiscono di poco:

* :ref:`upgrading-patch-version`
* :ref:`upgrading-minor-version`

.. _upgrading-patch-version:

Aggiornare una versione patch (p.e. da 2.6.0 a 2.6.1)
-----------------------------------------------------

Se si sta aggiornando una versione patch (cambia solo l'ultimo numero),
allora è *molto* facile:

.. code-block:: bash

    $ composer update symfony/symfony

Fatto! Non si dovrebbero incontrare problemi di retrocompatibilità, né aver
bisogno di modificare alcunché nel proprio codice. Questo perché, all'inizio
del progetto, il file ``composer.json`` incluso in Symfony ha usato un vincolo
come ``2.6.*``,  in cui solamente l'*ultimo* numero della versione cambierà a ogni
aggiornamento.

Si potrebbero voler aggiornare anche le altre librerie. Per chi ha fatto un
buon lavoro con i `vincoli di versione`_ in ``composer.json``, si può fare
in sicurezza, eseguendo:

.. code-block:: bash

    $ composer update

Attenzione, però: se si hanno `vincoli di versione`_ incorretti in ``composer.json``,
(come ``dev-master``), alcune librerie esterne potrebbero aggiornarsi
a versioni che contengono modifiche non retrocompatibili.

.. _upgrading-minor-version:

Aggiornare una versione minore (p.e. da 2.5.3 a 2.6.1)
------------------------------------------------------

Se si sta aggiornando una versione minore (in cui cambia il numero di mezzo), si
dovebbero incontrare anche modifiche alla retrocompatibilità non significanti.
Per deettagli, vedere :doc:`/contributing/code/bc`.

Tuttavia, sono possibili alcune modifiche non retrocompatibili, che ora vedremo
come affrontare.

Ci sono due passi nell'aggiornamento:

:ref:`upgrade-minor-symfony-composer`;
:ref:`upgrade-minor-symfony-code`

.. _`upgrade-minor-symfony-composer`:

1) Aggiornare le librerie Symfony tramite Composer
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Prima, occorre aggiornare Symfony, modificando il file ``composer.json`` per
usare la nuova versione:

.. code-block:: json

    {
        "...": "...",

        "require": {
            "php": ">=5.3.3",
            "symfony/symfony": "2.6.*",
            "...": "... non cambiare altro..."
        },
        "...": "...",
    }

Quindi, usare Composer per scaricare le nuove versioni delle librerie:

.. code-block:: bash

    $ composer update symfony/symfony

Si potrebbero voler aggiornare anche le altre librerie. Per chi ha fatto un
buon lavoro con i `vincoli di versione`_ in ``composer.json``, si può fare
in sicurezza, eseguendo:

.. code-block:: bash

    $ composer update

Attenzione, però: se si hanno `vincoli di versione`_ incorretti in ``composer.json``,
(come ``dev-master``), alcune librerie esterne potrebbero aggiornarsi
a versioni che contengono modifiche non retrocompatibili.

.. _`upgrade-minor-symfony-code`:

2) Aggiornare il proprio codice per funzionare con la nuova versione
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

In teoria, dovrebbe essere tutto a posto. Tuttavia, *potrebbe* essere necessario eseguire alcune
modifiche nel proprio codice, per far funzionare tutto. Inoltre, alcune caratteristiche usate
potrebbero funzionare, ma essere ora deprecate. Questo non è in realtà un problema,
ma essere consapevoli di tali deprecati può essere un primo passo per sistemarli
per tempo.

Ogni versione di Symfony dispone di un file UPGRADE, che descrive tali
modifiche. Di seguito ci sono i collegamenti ai file di ciascuna versione, che vanno
letti per vedere se siano necessarie modifiche al proprio codice.

.. tip::

    Se non si trova qui la versione a cui si sta aggiornando, basta cercare il file
    UPGRADE-X.X.md nella versione corrispondente del `repository di Symfony`_.

Aggiornare a Symfony 2.6
........................

Innanzitutto, ovviamente, aggiornare il file ``composer.json`` con la versione ``2.6``
di Symfony, come descritto in precedenza in :ref:`upgrade-minor-symfony-composer`.

Quindi, verificare il documento `UPGRADE-2.6`_, cercando i dettagli sulle modifiche al codice
che potrebbero essere necessarie nel proprio progetto.

Aggiornare a Symfony 2.5
........................

Innanzitutto, ovviamente, aggiornare il file ``composer.json`` con la versione ``2.5``
di Symfony, come descritto in precedenza in :ref:`upgrade-minor-symfony-composer`.

Quindi, verificare il documento `UPGRADE-2.5`_, cercando i dettagli sulle modifiche al codice
che potrebbero essere necessarie nel proprio progetto.

.. _`UPGRADE-2.5`: https://github.com/symfony/symfony/blob/2.5/UPGRADE-2.5.md
.. _`UPGRADE-2.6`: https://github.com/symfony/symfony/blob/2.6/UPGRADE-2.6.md
.. _`repository di Symfony`: https://github.com/symfony/symfony
.. _`Composer Package Versions`: https://getcomposer.org/doc/01-basic-usage.md#package-versions
.. _`vincoli di versione`: https://getcomposer.org/doc/01-basic-usage.md#package-versions