.. index::
   single: Componenti; Installazione
   single: Componenti; Uso

Installare e usare i componenti di Symfony2
===========================================

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

.. code-block:: json

    {
        "require": {
            "symfony/finder": "2.2.*"
        }
    }

Se si ha già un file ``composer.json``, aggiungere semplicemente la riga. Potrebbe
essere necessario modificare la versione (p.e. ``2.1.1`` o ``2.2.*``).

Si possono cercare nomi e versioni dei componenti su `packagist.org`_.

**3.** `Installare composer`_, se non è già presente sul proprio sistema: 

**4.** Scaricare le librerie dei venditori e generare il file ``vendor/autoload.php``:

.. code-block:: bash

    $ php composer.phar install

**5.** Scrivere il proprio codice:

Una volta che Composer ha scaricato i componenti, basterà includere il
file ``vendor/autoload.php`` generato da Composer stesso. Tale file si
occupa di autocaricare tutte le librerie, in modo che si possano usare
immediatamente::

        // File: src/script.php

        // cambiare il percorso in quello della cartella "vendor/", relativamente a questo file
        require_once '../vendor/autoload.php';

        use Symfony\Component\Finder\Finder;

        $finder = new Finder();
        $finder->in('../data/');

        // ...

.. tip::

    Se si vogliono usare tutti i componenti di Symfony2, invece di aggiungerli
    uno alla volta:

        .. code-block:: json

            {
                "require": {
                    "symfony/finder": "2.2.*",
                    "symfony/dom-crawler": "2.2.*",
                    "symfony/css-selector": "2.2.*"
                }
            }

    si può usare:

        .. code-block:: json

            {
                "require": {
                    "symfony/symfony": "2.2.*"
                }
            }

    Questo includerà le librerie dei bundle e dei bridge, che potrebbero non
    essere necessarie.

E ora?
------

Ora che i componenti sono installati e autocaricati, leggere la documentazione
specifica dei componenti per saperne di più sul loro uso.

Buon divertimento!

.. _Composer: http://getcomposer.org
.. _Installare composer: http://getcomposer.org/download/
.. _packagist.org: https://packagist.org/
