Personalizzare il processo di boostrap prima dei test
=====================================================

A volte, quando si eseguono i test, occorre fare qualcosa in più nel bootstrap,
prima dell'esecuzione stessa. Per esempio, se si esegue un test funzionale ed è
stata introdotta una nuova risorsa di traduzione, occorrerà pulire la cache
prima di eseguire tale test. Questa ricetta copre il suddetto argomento.

Iniziamo aggiungendo il seguente file::

    // app/tests.bootstrap.php
    if (isset($_ENV['BOOTSTRAP_CLEAR_CACHE_ENV'])) {
        passthru(sprintf(
            'php "%s/console" cache:clear --env=%s --no-warmup',
            __DIR__,
            $_ENV['BOOTSTRAP_CLEAR_CACHE_ENV']
        ));
    }

    require __DIR__.'/bootstrap.php.cache';

Sostituire il file ``bootstrap.php.cache`` in ``app/phpunit.xml.dist``
con ``tests.bootstrap.php``:

.. code-block:: xml

    <!-- app/phpunit.xml.dist -->
    bootstrap = "tests.bootstrap.php"

Ora possiamo definire, nel nostro file ``phpunit.xml.dist``, quale ambiente della cache
vogliamo pulire:

.. code-block:: xml

    <!-- app/phpunit.xml.dist -->
    <php>
        <env name="BOOTSTRAP_CLEAR_CACHE_ENV" value="test"/>
    </php>

Ora abbiamo una variabile di ambiente (come ``$_ENV``) a disposizione
nel fil di boostrap personalizzato (``tests.bootstrap.php``).
