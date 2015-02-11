.. _running-symfony2-tests:

Eseguire i test di Symfony
==========================

Prima di inviare una :doc:`patch <patches>`, occorre eseguire
tutti i test di Symfony, per assicurarsi di non aver rotto nulla.

PHPUnit
-------

Per eseguire i test di Symfony, `installare`_ prima PHPUnit 3.7 o successivi.

Dipendenze (opzionali)
----------------------

Per eseguire tutti i test, inclusi quelli che hanno dipendenze esterne,
Symfony deve poterle scaricare. Per impostazione predefinita, sono
auto-caricati dalla cartella ``vendor/`` (vedere
``autoload.php.dist``).

I test necessitano delle seguenti librerie di terze parti:

* Doctrine
* Swift Mailer
* Twig
* Monolog

Per installarle tutte, usare `Composer`_:

Passo 1: :doc:`installare Composer a livello globale </cookbook/composer>`

Passo 2: installare i venditori

.. code-block:: bash

    $ composer install

.. note::

    Si noti che lo script ha bisogno di tempo per terminare.

Dopo l'installazione, si possono aggiornare i venditori alle loro ultime versioni, con
il comando seguente:

.. code-block:: bash

    $ composer --dev update

Esecuzione
----------

Prima di tutto, aggiornare i venditori (vedere sopra).

Quindi, eseguire i test dalla cartella radice di Symfony, con il comando
seguente:

.. code-block:: bash

    $ phpunit

L'output dovrebbe mostrare `OK`. Altrimenti, occorre appurare quello che si è verificato e
se i test sono rotti per colpa di una propria modifica.

.. tip::

    Se si vuole testare un singolo componente, scriverne il percorso dopo il comando `phpunit`,
    p.e.:

    .. code-block:: bash

        $ phpunit src/Symfony/Component/Finder/

.. tip::

    Eseguire i test prima di applicare le proprie modifiche, per assicurarsi che girino
    correttamente con la propria configurazione.

Copertura del codice
--------------------

Se si aggiunge una nuova caratteristica, occorre anche verificare la copertura del codice,
usando l'opzione ``coverage-html``:

.. code-block:: bash

    $ phpunit --coverage-html=cov/

Verificare la copertura del codice, aprendo la pagina generata ``cov/index.html`` in
un browser.

.. tip::

    La copertura del codice funziona solo con XDebug abilitato e tutte le 
    dipendenze installate.

.. _installare: http://www.phpunit.de/manual/current/en/installation.html
.. _`Composer`: http://getcomposer.org/
