.. index::
   single: Installazione

Installare e configurare Symfony
================================

Lo scopo di questo capitolo è mettere in grado di avere un'applicazione funzionante
basata su Symfony. Per semplificare il processo di creazione di nuove
applicazioni, Symfony fornisce un installatore.

Installare l'installatore di Symfony
------------------------------------

L'utilizzo dell'**installatore di Symfony** è l'unico modo raccomandato di creare nuove
applicazioni Symfony. Questo installatore è un'applicazione PHP, che va installata
solo una volta e che può quindi creare tutte le applicazioni Symfony.

.. note::

    L'installatore richiede PHP 5.4 o successivi. Se si usa ancora la vecchia versione
    PHP 5.3, non si può usare l'installatore di Symfony. Leggere
    :ref:`book-creating-applications-without-the-installer` per sapere
    come procedere.

A seconda del sistema operativo, l'installatore va installato in modi
diversi.

Sistemi Linux e Mac OS X
~~~~~~~~~~~~~~~~~~~~~~~~

Aprire un terminale ed eseguire i seguenti tre comandi:

.. code-block:: bash

    $ sudo curl -LsS http://symfony.com/installer -o /usr/local/bin/symfony
    $ sudo chmod a+x /usr/local/bin/symfony

Questo creerà nel sistema un comando globale ``symfony``.

Sistemi Windows
~~~~~~~~~~~~~~~

Aprire la console dei comandi ed eseguire il seguente comando:

.. code-block:: bash

    c:\> php -r "readfile('http://symfony.com/installer');" > symfony

Quindi, spostare il file ``symfony.phar`` nella cartella dei progetti ed
eseguirlo, come segue:

.. code-block:: bash

    c:\> move symfony c:\progetti
    c:\progetti\> php symfony

Creare l'applicazione Symfony
-----------------------------

Una volta che l'installatore Symfony è pronto, creare la prima applicazione Symfony con
il comando ``new``:

.. code-block:: bash

    # Linux, Mac OS X
    $ symfony new progetto

    # Windows
    c:\> cd projects/
    c:\projects\> php symfony.phar new progetto

Questo comando crea una nuova cartella, chiamata ``progetto``, che contiene un
nuovo progetto, basato sulla versione di Symfony più recente. Inoltre,
l'installatore verifica se il sistema soddisfa i requisiti tecnici per
eseguire applicazioni Symfony. In caso negativo, si vedrà una lista di modifiche
necessarie a soddisfare tali requisiti.

.. tip::

    Per ragioni di sicurezza, tutte le versioni di Symfony sono firmate digitalmente prima
    di essere distribuite. Se si vuole verificare l'integrità di una versione di Symfony,
    seguire i passi `spiegati in questo post`_.

Basare un progetto su una specifica versione di Symfony
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Se un progetto deve essere basato su una specifica versione di Symfony, passare il numero
di versione come secondo parametro del comando ``new``:

.. code-block:: bash

    # usa la versione più recente di un ramo di Symfony
    $ symfony new progetto 2.3
    $ symfony new progetto 2.5
    $ symfony new progetto 2.6

    # usa una specifica versione di Symfony
    $ symfony new progetto 2.3.26
    $ symfony new progetto 2.6.5

    # usa la versione LTS (Long Term Support) più recente
    $ symfony new progetto lts

Se si vuole basare un progetto sull'ultima :ref:`versione LTS di Symfony <releases-lts>`,
passare ``lts`` come secondo parametro del comando ``new``:

.. code-block:: bash

    # Linux, Mac OS X
    $ symfony new progetto lts

    # Windows
    c:\projects\> php symfony new progetto lts

Leggere il :doc:`processo di rilascio di Symfony </contributing/community/releases>`
per comprendere meglio il motivo per cui esistono varie versioni di Symfony e quale
usare per i propri progetti.

.. _book-creating-applications-without-the-installer:

Creare applicazioni Symfony senza l'installatore
------------------------------------------------

Se si usa ancora PHP 5.3 o se non si può eseguire l'installatore per altre ragioni,
si possono creare applicazioni Symfony usando un metodo alternativo di installazione,
basato su `Composer`_.

Composer è un gestore di dipendenze, usato da applicazioni PHP moderne, e può essere usato
per creare nuove applicazioni basate sul framework Symfony. Se non lo si ha già
installato globalmente, seguire la prossima sezione.

Installare Composer globalmente
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Iniziare con :doc:`installare Composer globalmente </cookbook/composer>`.

Creare un'applicazione Symfony con Composer
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Una volta installato Composer, eseguire il comando ``create-project``
per creare una nuova applicazione Symfony, basata sull'ultima versione stabile:

.. code-block:: bash

    $ composer create-project symfony/framework-standard-edition progetto

Se si deve basare l'applicazione su una specifica versione di Symfony, fornire la
versione come secondo parametro del comando ``create-project``:

.. code-block:: bash

    $ composer create-project symfony/framework-standard-edition progetto '2.3.*'

.. tip::

    Con una connessione Internet lenta, si potrebbe pensare come Composer non stia
    facendo nulla. Nel caso, aggiungere l'opzione ``-vvv`` al comando precedente
    per mostrare un output dettagliato di tutto ciò che Composer sta facendo.

Eseguire l'applicazione Symfony
-------------------------------

Symfony sfrutta il server web interno fornito da PHP per eseguire applicazioni
mentre le si sviluppa. Quindi, per eseguire un'applicazione Symfony basta andare
nella cartella del progetto ed eseguire il seguente comando:

.. code-block:: bash

    $ cd progetto/
    $ php app/console server:run

Quindi, aprire un browser ed accedere all'URL ``http://localhost:8000`` per vedere
la pagina di benvenuto di Symfony:

.. image:: /images/quick_tour/welcome.png
   :align: center
   :alt:   Pagina di benvenuto di Symfony

Al posto di questa pagina di benvenuto, si potrebbe vedere una pagina bianca o di errore.
Questo dipende da un problema di configurazione dei permessi delle cartelle. Ci sono varie
possibili soluzioni, a seconda del sistema operativo. Sono tutte spiegate
nella sezione :ref:`Impostazione dei permessi <book-installation-permissions>`.


.. note::

    Il server interno di PHP è disponibile in PHP 5.4 o successivi. Se si usa ancora
    la vecchia versione 5.3, occorrerà configurare un *host virtuale* nel
    proprio server web.

Il comando ``server:run`` è disponibile solo durante lo sviluppo di un'applicazione. Per
eseguire applicazioni Symfony su server di produzione, si dovrà configurare un
server web `Apache`_ o `Nginx`_, come spiegato in
:doc:`/cookbook/configuration/web_server_configuration`.

Dopo aver finito di lavorare su un'applicazione Symfony, si può fermare il
server con il comando ``server:stop``:

.. code-block:: bash

    $ php app/console server:stop

Verifica della configurazione di un'applicazione Symfony
--------------------------------------------------------

Le applicazioni Symfony dispongono di un test per la configurazione del server, che mostra
se l'ambiente è pronto per usare Symfony. Accedere al seguente URL per verificare la propria
configurazione:

.. code-block:: text

    http://localhost:8000/config.php

Se ci sono problemi, correggerli prima di procedere.

.. _book-installation-permissions:

.. sidebar:: Impostare i permessi

    Un problema comune durante l'installazione è che le cartelle ``app/cache`` e
    ``app/logs`` devono essere scrivibili sia dal server web che dall'utente
    della linea di comando. Su sistemi UNIX, se l'utente del server web è diverso
    da quello della linea di comando, si possono provare le soluzioni seguenti.

    **1. Usare lo stesso utente per CLI e server web**

    In ambienti di sviluppo, è pratica comune usare lo stesso utente
    per CLI e server web, evitando così problemi di permessi
    per nuovi progetti. Lo si può fare modificando la configurazione del server web
    (cioè solitamente httpd.conf o apache2.conf per Apache) e impostandone
    l'utente in modo che sia lo stesso di CLI (p.e. per Apache, aggiornare i valori User
    e Group).

    **2. Usare ACL su un sistema che supporta chmod +a**

    Molti sistemi consento di usare il comando ``chmod +a``. Provare prima questo e, in
    caso di errore, provare il metodo successivo. Viene usato un comando per cercare di
    determinare l'utente con cui gira il server web e impostarlo come ``HTTPDUSER``:

    .. code-block:: bash

        $ rm -rf app/cache/*
        $ rm -rf app/logs/*

        $ HTTPDUSER=`ps aux | grep -E '[a]pache|[h]ttpd|[_]www|[w]ww-data|[n]ginx' | grep -v root | head -1 | cut -d\  -f1`
        $ sudo chmod +a "$HTTPDUSER allow delete,write,append,file_inherit,directory_inherit" app/cache app/logs
        $ sudo chmod +a "`whoami` allow delete,write,append,file_inherit,directory_inherit" app/cache app/logs


    **3. Usare ACL su un sistema che non supporta chmod +a**

    Alcuni sistemi non supportano ``chmod +a``, ma supportano un altro programma
    chiamato ``setfacl``. Si potrebbe aver bisogno di `abilitare il supporto ACL`_ sulla
    propria partizione e installare setfacl prima di usarlo (come nel caso di Ubuntu). Viene
    usato un comando per cercare di determinare l'utente con cui gira il server web e impostarlo come
    ``HTTPDUSER``:

    .. code-block:: bash

        $ HTTPDUSER=`ps aux | grep -E '[a]pache|[h]ttpd|[_]www|[w]ww-data|[n]ginx' | grep -v root | head -1 | cut -d\  -f1`
        $ sudo setfacl -R -m u:"$HTTPDUSER":rwX -m u:`whoami`:rwX app/cache app/logs
        $ sudo setfacl -dR -m u:"$HTTPDUSER":rwX -m u:`whoami`:rwX app/cache app/logs

    Se non funziona, provare aggiungendo l'opzione ``-n``.

    **4. Senza usare ACL**

    Se non è possibile modificare l'ACL delle cartelle, occorrerà modificare
    l'umask in modo che le cartelle cache e log siano scrivibili dal gruppo
    o da tutti (a seconda che gli utenti di server web e linea di comando siano
    o meno nello stesso gruppo). Per poterlo fare, inserire la riga seguente
    all'inizio dei file ``app/console``, ``web/app.php`` e ``web/app_dev.php``::

        umask(0002); // Imposta i permessi a 0775

        // oppure

        umask(0000); // Imposta i permessi a 0777

    Si noti che l'uso di ACL è raccomandato quando si ha accesso al server,
    perché la modifica di umask non è thread-safe.

.. _installation-updating-vendors:

Aggiornare applicazioni Symfony
-------------------------------

A questo punti, si dispone di un'applicazione Symfony pienamente funzionale,
in cui si può sviluppare il proprio progetto. Un'applicazione Symfony dipende da
varie librerie esterne. Queste sono scaricate nella cartella ``vendor/`` e
sono gestite esclusivamente da Composer.

L'aggiornamento frequente di queste librerie di terze parti è una buona pratica, per prevenire bug
e vulnerabilità di sicurezza. Eseguire il comando ``update`` di Composer per aggiornarle
tutte insieme:

.. code-block:: bash

    $ cd progetto/
    $ composer update

A seconda della complessità del progetto, questo processo di aggiornamento può impiegare anche
vari minuti per essere completato.

.. _installing-a-symfony2-distribution:

Installare una distribuzione di Symfony
---------------------------------------

Il progetto Symfony impacchetta "distribuzioni", che sono applicazioni pienamente funzionali,
che includono le librerie del nucleo di Symfony, una selezione di bundle utili, una struttura
di cartelle appropriata e alcune configurazioni predefinite. In effetti, quando è stata creata
un'applicazione Symfony, nelle sezioni precedenti, in realtà è stata scaricata la
distribuzione predefinita fornita da Symfony, chiamata *Symfony Standard Edition*.

*Symfony Standard Edition* è la distribuzione più popolare ed è anche la
scelta migliore per sviluppatore che iniziano con Symfony. Tuttavia, la comunità di Symfony
ha pubblicato altre distribuzioni, che si potrebbe voler usare in
un'applicazione:

* `Symfony CMF Standard Edition`_ è una distribuzione pensata per partire con
  il progetto `Symfony CMF`_, che rende più facile per gli
  sviluppatori l'aggiunta di funzionalità CMS ad applicazioni basate sul
  framework Symfony.
* `Symfony REST Edition`_ mostra come costruire un'applicazione che fornisca un'API
  REST, usando `FOSRestBundle`_ e vari altri bundle correlati.

Uso di un controllo dei sorgenti
--------------------------------

Se si usa un sistema di controllo di versione, come `Git`_, si può tranquillamente eseguire il commit
do tutto il codice del progetto. Questo perché le applicazioni Symfony contengono già un file
``.gitignore``, preparato appositamente per Symfony.

Per istruzioni specifiche su come impostare al meglio un progetto per essere memorizzato
in Git, vedere :doc:`/cookbook/workflow/new_project_git`.

Usare un'applicazione Symfony versionata
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Quando si usa Composer èer gestire le dipendenze di un'applicazione, si raccomanda di
ignorare l'intera cartella ``vendor/``, prima di eseguire commit di codice nel
repository. Questo vuole dire che, quando si esegue il checkout di un'applicazione Symfony da un
repository Git, non ci sarà la cartella ``vendor/`` e l'applicazione non funzionerà
immediatamente.

Per farlo funzionare, eseguire il checkout dell'applicazione Symfony ed eseguire il comando
``install`` di Composer, per scaricare e installare tutte le dipendenze richieste
dall'applicazione:

.. code-block:: bash

    $ cd progetto/
    $ composer install

Come fa Composer a sapere quali dipendenze installare? Perché quando si esegue il
commit di un'applicazione Symfony su un repository, si includono i file ``composer.json`` e
``composer.lock`` nel commit. Questi file dicono a Composer quali
dipendenze (e in quali specifiche versioni) installare nell'applicazione.

Iniziare lo sviluppo
--------------------

Ora che si dispone di un'applicazione Symfony pienamente funzionale, si può iniziare
lo sviluppo! La distribuzione potrebbe contenere del codice di esempio, verificare sul file
``README.md`` incluso (aprirlo come file di testo) per
conoscere l'eventuale codice di esempio incluso nella distribuzione.

Chi è nuovo su Symfony può fare riferimento a ":doc:`page_creation`", dove si imparerà
come creare pagine, cambiare configurazione e ogni altra cosa necessaria per
la nuova applicazione.

Assicurarsi di dare un'occhiata anche al :doc:`ricettario </cookbook/index>`, che contiene
una grande varietà di ricette, pensate per risolvere problemi specifici con Symfony.

.. note::

    Se si vuole rimuovere il codice di esempio dalla distribuzione, dare un'occhiata
    a questa ricetta: ":doc:`/cookbook/bundles/remove`"

.. _`spiegati in questo post`: http://fabien.potencier.org/article/73/signing-project-releases
.. _`Composer`: http://getcomposer.org/
.. _`Composer download page`: https://getcomposer.org/download/
.. _`Apache`: http://httpd.apache.org/docs/current/mod/core.html#documentroot
.. _`Nginx`: http://wiki.nginx.org/Symfony
.. _`abilitare il supporto ACL`: https://help.ubuntu.com/community/FilePermissionsACLs
.. _`Symfony CMF Standard Edition`: https://github.com/symfony-cmf/symfony-cmf-standard
.. _`Symfony CMF`: http://cmf.symfony.com/
.. _`Symfony REST Edition`: https://github.com/gimler/symfony-rest-edition
.. _`FOSRestBundle`: https://github.com/FriendsOfSymfony/FOSRestBundle
.. _`Git`: http://git-scm.com/
