.. index::
   single: Installazione

Installare e configurare Symfony
================================

Lo scopo di questo capitolo è quello di ottenere un'applicazione funzionante basata
su Symfony. Fortunatamente, Symfony offre delle "distribuzioni", che sono
progetti Symfony di partenza funzionanti, che possono essere scaricati per iniziare
immediatamente a sviluppare.

.. tip::

    Se si stanno cercando le istruzioni per creare un nuovo progetto e memorizzarlo con
    un sistema di versionamento, si veda `Usare un controllo di sorgenti`_.

Scaricare una distribuzione Symfony2
------------------------------------

.. tip::

    Verificare innanzitutto di avere un server web (come Apache) installato
    e funzionante con PHP 5.3.3 o successivi. Per ulteriori informazioni sui requisiti di Symfony2,
    si veda il :doc:`riferimento sui requisiti</reference/requirements>`.

Symfony2 ha dei pacchetti con delle "distribuzioni", che sono applicazioni funzionanti che
includono le librerie del nucleo di Symfony2, una selezione di bundle utili e alcune
configurazioni predefinite. Scaricando una distribuzione di Symfony2, si ottiene uno
scheletro di un'applicazione funzionante, che può essere subito usata per sviluppare
la propria applicazione.

Si può iniziare visitando la pagina di scaricamento di Symfony2, `http://symfony.com/download`_.
Su questa pagina, si vedrà la *Symfony Standard Edition*, che è la distribuzione
principale di Symfony2. Si possono fare due scelte:

Opzione 1) Composer
~~~~~~~~~~~~~~~~~~~

`Composer`_ è una libreria di gestione delle dipendenze per PHP, utilizzabile per
scaricare Symfony2 Standard Edition.

Iniziare con lo `scaricare Composer`_ sul proprio computer. Se si ha
curl installato, è facile:

.. code-block:: bash

    $ curl -s https://getcomposer.org/installer | php

.. note::

    Se il computer non è pronto per usare Composer, si otterranno alcune raccomandazioni
    all'esecuzione del comando. Seguire tali raccomandazioni per far funzionare Composer
    correttamente.

Composer è un file PHAR eseguibile, che si può usare per scaricare la distribuzione
Standard:

.. code-block:: bash

    $ php composer.phar create-project symfony/framework-standard-edition /percorso/web/Symfony 2.3.0

.. tip::

    Per una versione esatta, sostituire "2.3.0" con l'ultima versione di Symfony.
    Per dettagli, si veda la `pagina di installazione di Symfony`_

.. tip::

    Per scaricare i file dei venditori più velocemente, aggiungere l'opzione ``--prefer-dist``
    alla fine di ogni comando di Composer.

Questo comando può richiedere diversti minuti, mentre Composer scarica la distribuzione Standard
e tutte le librerie dei venditori necessarie. Quando avrà finito,
si dovrebbe avere una cartella simile a questa:

.. code-block:: text

    percorso/web/ <- la cartella radice del web
        Symfony/ <- l'archivio scompattato
            app/
                cache/
                config/
                logs/
            src/
                ...
            vendor/
                ...
            web/
                app.php
                ...

Opzione 2) Scaricare un archivio
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Si può anche scaricare un archivio della Standard Edition. Qui, si possono fare
due scelte:

* Scaricare un archivio ``.tgz`` o ``.zip`` (sono equivalenti, scegliere quello che
  si preferisce);

* Scaricare la distribuzione con o senza venditori. Se si pensa di usare
  molte librerie o bundle di terze parti e gestirli tramite Composer, probabilmente
  sarà meglio scaricare quella senza venditori.

Scaricare uno degli archivi e scompattarlo da qualche parte sotto la cartella
radice del web del server. Da linea di comando UNIX, lo si può fare con
uno dei seguenti comandi (sostituire ``###`` con il vero nome del file):

.. code-block:: bash

    # per il file .tgz
    $ tar zxvf Symfony_Standard_Vendors_2.3.###.tgz

    # per il file .zip
    $ unzip Symfony_Standard_Vendors_2.3.###.zip

Se si è optato per la versione senza venditori, occorerà leggere la 
prossima sezione.

.. note::

    Si può facilmente modificare la struttura predefinita di cartelle. Si veda
    :doc:`/cookbook/configuration/override_dir_structure` per maggiori
    informazioni.

Tutti i file pubblici e il front controller, che gestisce le richieste in arrivo in
un'applicazione Symfony2, si trovano nella cartella ``Symfony/web/``. Quindi, ipotizzando
di aver decompresso l'archivio nella cartella radice del server web o di un host virtuale,
gli URL dell'applicazione inizieranno con ``http://localhost/Symfony/web/``.

.. note::

    Gli esempi che seguono ipotizzano che le impostazioni sulla cartella radice non siano state modificate,
    quindi tutti gli URL inizieranno con ``http://localhost/Symfony/web/``

.. _installation-updating-vendors:

Aggiornare i venditori
~~~~~~~~~~~~~~~~~~~~~~

A questo punto, si dispone di un progetto Symfony funzionale, nel quale
si può iniziare a sviluppare la propria applicazione. Un progetto Symfony dipende
da diverse librerie esterne. Queste vanno scaricate nella cartella `vendor/`
del progetto, tramite una libreria chiamata `Composer`_.

A seconda di come Symfony è stato scaricato, si potrebbe aver bisogno o meno di
aggiornare i venditori. Aggiornare i venditori è sempre sicuro e garantisce
di disporre di tutte le librerie necessarie.

Passo 1: Ottenere `Composer`_ (il nuovo bellissimo sistema di pacchetti PHP)

.. code-block:: bash

    $ curl -s http://getcomposer.org/installer | php

Assicurarsi di scaricare ``composer.phar`` nella stessa cartella in cui si trova
il file ``composer.json`` (per impostazione predefinita, la radice del progetto
Symfony).

Passo 2: Installare i venditori

.. code-block:: bash

    $ php composer.phar install

Questo comando scarica tutte le librerie dei venditori necessarie, incluso
Symfony stesso, nella cartella ``vendor/``.

.. note::

    Se non si ha ``curl`` installato, si può anche scaricare il file ``installer``
    a mano, da http://getcomposer.org/installer. Mettere il file nel progetto ed
    eseguire:

    .. code-block:: bash

        $ php installer
        $ php composer.phar install

.. tip::

    Quando si esegue ``php composer.phar install`` o ``php composer.phar update``,
    composer eseguirà dei comandi post installazione/aggiornamento per pulire la cache
    e installare le risorse. Per impostazione predefinita, le risorse saranno copiate
    nella cartella ``web``.

    Invece di copiare le risorse, si possono creare dei collegamenti simbolici, se
    supportato dal sistema operativo. Per creare collegamenti simbolici, aggiungere
    una voce nel nodo ``extra`` del file composer.json, con chiave
    ``symfony-assets-install`` e valore ``symlink``:

    .. code-block:: json

        "extra": {
            "symfony-app-dir": "app",
            "symfony-web-dir": "web",
            "symfony-assets-install": "symlink"
        }

    Passando ``relative`` invece di ``symlink`` a symfony-assets-install, il comando genererà
    collegamenti simbolici relativi.
        
Configurazione
~~~~~~~~~~~~~~

A questo punto, tutte le librerie di terze parti necessarie sono nella
cartella ``vendor/``. Si dispone anche una configurazione predefinita dell'applicazione
in ``app/`` e un po' di codice di esempio in ``src/``.

Symfony2 dispone di uno strumento visuale per la verifica della configurazione del server,
per assicurarsi che il server web e PHP siano configurati per usare Symfony2. Usare il
seguente URL per la verifica della configurazione:

.. code-block:: text

    http://localhost/config.php

Se ci sono problemi, correggerli prima di proseguire.

.. sidebar:: Impostare i permessi

    Un problema comune è che le cartelle ``app/cache`` e ``app/logs`` devono essere
    scrivibili sia dal server web che dall'utente della linea di comando. Su sistemi
    UNIX, se l'utente del server web è diverso da quello della linea di comando,
    si possono eseguire i seguenti comandi una sola volta sul proprio progetto, per
    assicurarsi che i permessi siano impostati correttamente.

    **1. Usare ACL su un sistema che supporta chmod +a**

    Molti sistemi consento di usare il comando ``chmod +a``. Provare prima questo e, in
    caso di errore, provare il metodo successivo. Viene usato un comando per cercare di
    determinare l'utente con cui gira il server web e impostarlo come ``APACHEUSER``:

    .. code-block:: bash

        $ rm -rf app/cache/*
        $ rm -rf app/logs/*

        $ APACHEUSER=`ps aux | grep -E '[a]pache|[h]ttpd' | grep -v root | head -1 | cut -d\  -f1`
        $ sudo chmod +a "$APACHEUSER allow delete,write,append,file_inherit,directory_inherit" app/cache app/logs
        $ sudo chmod +a "`whoami` allow delete,write,append,file_inherit,directory_inherit" app/cache app/logs


    **2. Usare ACL su un sistema che non supporta chmod +a**

    Alcuni sistemi non supportano ``chmod +a``, ma supportano un altro programma
    chiamato ``setfacl``. Si potrebbe aver bisogno di `abilitare il supporto ACL`_ sulla
    propria partizione e installare setfacl prima di usarlo (come nel caso di Ubuntu). Viene
    usato un comando per cercare di determinare l'utente con cui gira il server web e impostarlo come
    ``APACHEUSER``:

    .. code-block:: bash

		$ APACHEUSER=`ps aux | grep -E '[a]pache|[h]ttpd' | grep -v root | head -1 | cut -d\  -f1`
		$ sudo setfacl -R -m u:$APACHEUSER:rwX -m u:`whoami`:rwX app/cache app/logs
		$ sudo setfacl -dR -m u:$APACHEUSER:rwX -m u:`whoami`:rwX app/cache app/logs
		
    **3. Senza usare ACL**

    Se non è possibile modificare l'ACL delle cartelle, occorrerà modificare
    l'umask in modo che le cartelle cache e log siano scrivibili dal gruppo
    o da tutti (a seconda che gli utenti di server web e linea di comando siano
    o meno nello stesso gruppo). Per poterlo fare, inserire la riga seguente
    all'inizio dei file ``app/console``, ``web/app.php`` e
    ``web/app_dev.php``::

        umask(0002); // Imposta i permessi a 0775

        // oppure

        umask(0000); // Imposta i permessi a 0777

    Si noti che l'uso di ACL è raccomandato quando si ha accesso al server,
    perché la modifica di umask non è thread-safe.

Quando tutto è a posto, cliccare su "Go to the Welcome page" per accedere alla
prima "vera" pagina di Symfony2:

.. code-block:: text

    http://localhost/app_dev.php/

Symfony2 dovrebbe dare il suo benvenuto e congratularsi per il lavoro svolto finora!

.. image:: /images/quick_tour/welcome.png

.. tip::
    
    Per ottenere URL brevi, si dovrebbe far puntare la cartella radice del
    server web o un host virtuale alla cartella ``Symfony/web/``. Sebbene
    non sia obbligatorio per lo sviluppo, è raccomandato nel momento in cui
    l'applicazione va in produzione, perché tutti i file di sistema e di configurazione
    diventeranno inaccessibili ai client. Perinformazioni sulla configurazione di
    uno specifico server web, leggere
    :doc:`/cookbook/configuration/web_server_configuration`
    o consultare la documentazione ufficiale del server:
    `Apache`_ | `Nginx`_ .

Iniziare lo sviluppo
--------------------

Ora che si dispone di un'applicazione Symfony2 pienamente funzionante, si può iniziare
lo sviluppo. La distribuzione potrebbe contenere del codice di esempio, verificare il file
``README.md`` incluso nella distribuzione (aprendolo come file di testo) per sapere
quale codice di esempio è incluso nella distribuzione scelta.

Per chi è nuovo in Symfony, in ":doc:`page_creation`" si può imparare come creare
pagine, cambiare configurazioni e tutte le altre cose di cui si avrà bisogno nella
nuova applicazione.

Dare un'occhiata anche al :doc:`ricettario</cookbook/index>`, che contiene
una varietà di articoli su come risolvere problemi specifici con Symfony.

.. note::

    Se si vuole rimuovere il codice di esempio dalla distribuzione, dare un'occhiata
    a questa ricetta: ":doc:`/cookbook/bundles/remove`"

Usare un controllo di sorgenti
------------------------------

Se si usa un sistema di controllo di versioni, come ``Git`` o ``Subversion``, lo si
può impostare e iniziare a fare commit nel proprio progetto, come si fa normalmente.
Symfony Standard edition *è* il punto di partenza per il nuovo
progetto.

Per istruzioni specifiche su come impostare al meglio il proprio progetto per essere
memorizzato in git, si veda :doc:`/cookbook/workflow/new_project_git`.

Ignorare la cartella ``vendor/``
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Chi ha scelto di scaricare l'archivio *senza venditori* può tranquillamente
ignorare l'intera cartella ``vendor/`` e non inviarla in commit al controllo di sorgenti.
Con ``Git``, lo si può fare aggiungendo al file ``.gitignore`` la
seguente riga:

.. code-block:: text

    /vendor/

Ora la cartella dei venditori non sarà inviata in commit al controllo di sorgenti.
Questo è bene (anzi, benissimo!), perché quando qualcun altro clonerà o farà checkout
del progetto, potrà semplicemente eseguire lo script ``php composer.phar install`` per
scaricare tutte le librerie dei venditori necessarie.

.. _`abilitare il supporto ACL`: https://help.ubuntu.com/community/FilePermissionsACLs
.. _`http://symfony.com/download`: http://symfony.com/download
.. _`Git`: http://git-scm.com/
.. _`GitHub Bootcamp`: http://help.github.com/set-up-git-redirect
.. _`Composer`: http://getcomposer.org/
.. _`scaricare Composer`: http://getcomposer.org/download/
.. _`Apache`: http://httpd.apache.org/docs/current/mod/core.html#documentroot
.. _`Nginx`: http://wiki.nginx.org/Symfony
.. _`pagina di installazione di Symfony`:    http://symfony.com/download
