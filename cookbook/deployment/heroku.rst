.. index::
   single: Deploy; Deploy su Heroku Cloud

Deploy su Heroku Cloud
======================

Questa ricetta descrive passo-passo come eseguire il deploy di un'applicazione Symfony
sulla piattaforma cloud Heroku. Il contenuto si basa sull'`articolo originale`_
pubblicato Heroku.

Inizio
------

Per creare un nuovo sito Heroku, `iscriversi a Heroku`_ o entrare
con le proprie credenziali. Quindi scaricare e installare `Heroku Toolbelt`_ sul
proprio computer.

Si puà anche dare un'occhiata alla guida `getting Started with PHP on Heroku`_, per
acquisire familiarità con le specifiche di come funzionano le applicazioni PHP su Heroku.

Preparare l'applicazione
~~~~~~~~~~~~~~~~~~~~~~~~

Il deploy di un'applicazione Symfony su Heroku non richiede alcuna modifica nel
codice, ma richiede alcuni piccoli aggiustamenti alla configurazione.

La posizione standard dei log di Symfony è la cartella ``app/log/``.
Questo non è idale, perché Heroku usa un `filesystem effimero`_. Su
Heroku, il modo migliore per salvare i log è `Logplex`_. Il modo migliore per
inviare dati di log a Logplex è la scrittura su ``STDERR`` o ``STDOUT``. Fortunamente, 
Symfony usa l'eccellente libreria Monolog per gestire i log. Quindi, il cambio di
destinazione per i log implica una semplice modifica nella configurazione.

Aprire il file ``app/config/config_prod.yml``, individuare la sezione
``monolog/handlers/nested``  (oppure crearla, nel caso non esista ancora) e 
cambiare il valore di ``path`` da
``"%kernel.logs_dir%/%kernel.environment%.log"`` a ``"php://stderr"``:

.. code-block:: yaml

    # app/config/config_prod.yml
    monolog:
        # ...
        handlers:
            # ...
            nested:
                # ...
                path: "php://stderr"

Una volta eseguito il deploy dell'applicazione, eseguire ``heroku logs --tail`` per impedire
al flusso di log da Heroku di aprirsi nel terminale.

Creare una nuova applicazione su Heroku
---------------------------------------

Per creare una nuova applicazione Heroku, usare il comando ``create``
da CLI:

.. code-block:: bash

    $ heroku create

    Creating mighty-hamlet-1981 in organization heroku... done, stack is cedar
    http://mighty-hamlet-1981.herokuapp.com/ | git@heroku.com:mighty-hamlet-1981.git
    Git remote heroku added

Si è ora pronti per il deploy dell'applicazione, come spiegato nella prossima sezione.

Deploy dell'applicazione su Heroku
----------------------------------

Per il deploy dell'applicazione su Heroku, si deve prima creare un profilo ``Procfile``, 
che dice a Heroku quale comando usare per lanciare il server web con le
impostazioni corrette. Dopo averlo fatto, assicurarsi che l'applicazione Symfony
giri in ambiente ``prod`` e si sarà pronti per eseguire un ``git push``.
ecco fatto!

Creare un Procfile
~~~~~~~~~~~~~~~~~~

Heroku lancerà un server web Apache insieme a PHP, per servire le
applicazioni. Tuttavia, alle applicazioni Symfony si applicano due circostanze speciali:

#. La document root è nella cartella ``web/`` e non nella cartella radice
   dell'applicazione;
#. La cartella ``bin-dir`` di Composer, in cui sono posti i binari dei venditori (compresi gli
   script di avvio di Heroku), è ``bin/`` e non la predefinita ``vendor/bin``.

.. note::

    I binari dei venditori sono solitamente installati da Composer in ``vendor/bin``, ma
    a volte (p.e. con un progeetto basato su Symfony Standard Edition), la
    posizione sarà diversa. In caso di dubbi, si può sempre eseguire
    ``composer config bin-dir`` per trovare la posizione corretta.

Creare  un nuovo file chiamato ``Procfile`` (senza estensione) nella cartella
radice dell'applicazione e inserirvi il seguente contenuto:

.. code-block:: text

    web: bin/heroku-php-apache2 web/

Se si preferisce lavorare sulla console, eseguire i seguenti comandi
per creare il file ``Procfile`` e aggiungerlo al repository:

.. code-block:: bash

    $ echo "web: bin/heroku-php-apache2 web/" > Procfile
    $ git add .
    $ git commit -m "Procfile for Apache and PHP"
    [master 35075db] Procfile for Apache and PHP
     1 file changed, 1 insertion(+)

Impostare l'ambiente t``prod``
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Durante un deploy, Heroku esegue ``composer install --no-dev`` per installare tutte le
dipendenze richieste dall'applicazione. Tuttavia, tipici `comandi post-installazione`_
in ``composer.json``, p.e. per installare risorse o pulire la cache, sarebbero
eseguiti nell'ambiente ``dev`` di Symfony.

Questo comportamento non è quello desiderato, essendo l'applicazione in produzione (anche se
la si usa solo come esperimento o come stage), quindi ogni passo di build
dovrebbe usare lo stesso ambiente, ``prod``.

Per fortuna, la soluzione al problema è molto semplice: Symfony cercherà una
variabile d'ambiente di nome ``SYMFONY_ENV`` e la userà, a meno che l'ambiente
non sia esplicitamente impostato. Heroku espone tutte le  `variabili di configurazione`_ come
variabili d'ambiente, quindi basta un singolo comando per preparare il deploy:

.. code-block:: bash

    $ heroku config:set SYMFONY_ENV=prod

Push su Heroku
~~~~~~~~~~~~~~

Il passo successivo è quello eseguire il deploy dell'applicazione su Heroku. La prima
volta che lo si fa, si potrebbe vedere un messaggio simile al seguente:

.. code-block:: bash

    The authenticity of host 'heroku.com (50.19.85.132)' can't be established.
    RSA key fingerprint is 8b:48:5e:67:0e:c9:16:47:32:f2:87:0c:1f:c8:60:ad.
    Are you sure you want to continue connecting (yes/no)?

In tal caso, occorre confermare, scrivendo per esteso ``yes`` e dando invio.
Sarebbe meglio verificare che effettivamente la chiave dell'impronta digitale RSA sia corretta.

Il deploy può quindi avvenire con questo comando:

.. code-block:: bash

    $ git push heroku master

    Initializing repository, done.
    Counting objects: 130, done.
    Delta compression using up to 4 threads.
    Compressing objects: 100% (107/107), done.
    Writing objects: 100% (130/130), 70.88 KiB | 0 bytes/s, done.
    Total 130 (delta 17), reused 0 (delta 0)

    -----> PHP app detected

    -----> Setting up runtime environment...
           - PHP 5.5.12
           - Apache 2.4.9
           - Nginx 1.4.6

    -----> Installing PHP extensions:
           - opcache (automatic; bundled, using 'ext-opcache.ini')

    -----> Installing dependencies...
           Composer version 64ac32fca9e64eb38e50abfadc6eb6f2d0470039 2014-05-24 20:57:50
           Loading composer repositories with package information
           Installing dependencies from lock file
             - ...

           Generating optimized autoload files
           Creating the "app/config/parameters.yml" file
           Clearing the cache for the dev environment with debug true
           Installing assets using the hard copy option
           Installing assets for Symfony\Bundle\FrameworkBundle into web/bundles/framework
           Installing assets for Acme\DemoBundle into web/bundles/acmedemo
           Installing assets for Sensio\Bundle\DistributionBundle into web/bundles/sensiodistribution

    -----> Building runtime environment...

    -----> Discovering process types
           Procfile declares types -> web

    -----> Compressing... done, 61.5MB

    -----> Launching... done, v3
           http://mighty-hamlet-1981.herokuapp.com/ deployed to Heroku

    To git@heroku.com:mighty-hamlet-1981.git
     * [new branch]      master -> master

Ecco fatto! Se ora si apre il browser, o puntando manualmente
all'URL fornita da ``heroku create`` o usando Heroku Toolbelt,
l'applicazione risponderà:

.. code-block:: bash

    $ heroku open
    Opening mighty-hamlet-1981... done

Si dovrebbe vedere l'applicazione Symfony nel browser.

.. _`articolo originale`: https://devcenter.heroku.com/articles/getting-started-with-symfony2
.. _`iscriversi a Heroku`: https://signup.heroku.com/signup/dc
.. _`Heroku Toolbelt`: https://devcenter.heroku.com/articles/getting-started-with-php#local-workstation-setup
.. _`getting Started with PHP on Heroku`: https://devcenter.heroku.com/articles/getting-started-with-php
.. _`filesystem effimero`: https://devcenter.heroku.com/articles/dynos#ephemeral-filesystem
.. _`Logplex`: https://devcenter.heroku.com/articles/logplex
.. _`verified that the RSA key fingerprint is correct`: https://devcenter.heroku.com/articles/git-repository-ssh-fingerprints
.. _`comandi post-installazione`: https://getcomposer.org/doc/articles/scripts.md
.. _`variabili di configurazione`: https://devcenter.heroku.com/articles/config-vars