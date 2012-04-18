Eseguire i test di Symfony2
===========================

Prima di inviare una :doc:`patch <patches>`, occorre eseguire
tutti i test di Symfony2, per assicurarsi di non aver rotto nulla.

PHPUnit
-------

Per eseguire i test di Symfony2, `installare`_ prima PHPUnit 3.5.11 o successivi.

.. code-block:: bash

    $ pear channel-discover pear.phpunit.de
    $ pear channel-discover components.ez.no
    $ pear channel-discover pear.symfony-project.com
    $ pear install phpunit/PHPUnit

Dipendenze (opzionali)
----------------------

Per eseguire tutti i test, inclusi quelli che hanno dipendenze esterne,
Symfony2 deve poterle scaricare. Per impostazione predefinita, sono
auto-caricati dalla cartella `vendor/` (vedere
`autoload.php.dist`).

I test necessitano delle seguenti librerie di terze parti:

* Doctrine
* Swiftmailer
* Twig
* Monolog

Per installarle tutte, eseguire lo script `vendors`:

.. code-block:: bash

    $ php vendors.php install

.. note::

    Si noti che lo script ha bisogno di tempo per terminare.

Dopo l'installazione, si possono aggiornare i venditori alle loro ultime versioni, con
il comando seguente:

.. code-block:: bash

    $ php vendors.php update

Esecuzione
----------

Prima di tutto, aggiornare i venditori (vedere sopra).

Quindi, eseguire i test dalla cartella radice di Symfony2, con il comando
seguente:

.. code-block:: bash

    $ phpunit

L'output dovrebbe mostrare `OK`. Altrimenti, occorre appurare quello che si è verificato e
se i test sono rotti per colpa di una propria modifica.

.. tip::

    Eseguire i test prima di applicare le proprie modifiche, per assicurarsi che girino
    correttamente con la propria configurazione.

Copertura del codice
--------------------

Se si aggiunge una nuova caratteristica, occorre anche verificare la copertura del codice,
usando l'opzione `coverage-html`:

.. code-block:: bash

    $ phpunit --coverage-html=cov/

Verificare la copertura del codice, aprendo la pagina generata `cov/index.html` in
un browser.

.. tip::

    La copertura del codice funziona solo con XDebug abilitato e tutte le 
    dipendenze installate.

.. _installare: http://www.phpunit.de/manual/current/en/installation.html
