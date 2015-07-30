.. index::
   single: Componenti; Installazione
   single: Componenti; Uso

.. _how-to-install-and-use-the-symfony2-components:

Installare e usare i componenti di Symfony
==========================================

Se si inizia un nuovo progetto (o se si ha già un progetto) che userà
uno o più componenti, il modo più semplice per integrare tutto è con `Composer`_.
Composer è abbastanza intelligente da scaricare i componenti necessari e occuparsi
del caricamento automatico, in modo che si può iniziare a usare immediatamente le librerie.

Questo articolo approfondirà l'uso di :doc:`/components/finder`, tuttavia è
applicabile all'uso di qualsiasi componente.

Uso del componente Finder
-------------------------

**1.** Se si sta creando un nuovo progetto, creare una cartella vuota.

**2.** Creare un file chiamato ``composer.json`` e incollarvi dentro il codice seguente:

.. code-block:: bash

    $ composer require symfony/finder

Il nome ``symfony/finder`` è scritto in cima alla documentazione del
componente desiderato.

.. tip::

    `Installare composer`_, se non fosse già presente sul sistem.
    A seconda di come lo si installa, si potrebbe avere un file ``composer.phar``
    nella cartella. In questo caso, nessun problema! Basta eseguire
    ``php composer.phar require symfony/finder``.

**3.** Scrivere il proprio codice!

Una volta che Composer ha scaricato i componenti, basterà includere il
file ``vendor/autoload.php`` generato da Composer stesso. Tale file si
occupa di autocaricare tutte le librerie, in modo che si possano usare
immediatamente::

    // File esempio: src/script.php

    // cambiare il percorso in quello della cartella "vendor/"
    // relativamente a questo file
    require_once __DIR__.'/../vendor/autoload.php';

    use Symfony\Component\Finder\Finder;

    $finder = new Finder();
    $finder->in('../data/');

    // ...

Usare tutti i componenti
------------------------

Se si vogliono usare tutti i componenti di Symfony, invece di aggiungerli
uno per uno, si può includere il pacchetto ``symfony/symfony``:

.. code-block:: bash

    $ composer require symfony/symfony

Questo includerà anche librerie di bundle e di bridge, che potrebbero non essere
effettivamente necessarie.

E ora?
------

Ora che i componenti sono installati e autocaricati, leggere la documentazione
specifica dei componenti per saperne di più sul loro uso.

Buon divertimento!

.. _Composer: http://getcomposer.org
.. _Installare composer: http://getcomposer.org/download/
