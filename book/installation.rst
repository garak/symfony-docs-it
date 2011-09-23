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
    un sistema di versionamento, si veda `Uso di un controllo dei sorgenti`_.

Scaricare una distribuzione Symfony2
------------------------------------

.. tip::

    Verificare innanzitutto di avere un server web (come Apache) installato
    e funzionante con PHP 5.3.2 o successivi. Per ulteriori informazioni sui 
    requisiti di Symfony2, si veda il :doc:`riferimento sui requisiti</reference/requirements>`.

Symfony2 ha dei pacchetti con delle "distribuzioni", che sono applicazioni funzionanti che
includono le librerie del nucleo di Symfony2, una selezione di bundle utili e alcune
configurazioni predefinite. Scaricando una distribuzione di Symfony2, si ottiene uno
scheletro di un'applicazione funzionante, che può essere subito usata per sviluppare
la propria applicazione.

Si può iniziare visitando la pagina di scaricamento di Symfony2, `http://symfony.com/download`_.
Su questa pagina, si vedrà la *Symfony Standard Edition*, che è la distribuzione
principale di Symfony2. Si possono fare due scelte:

* Scaricare l'archivio, ``.tgz`` o ``.zip`` sono equivalenti, si può
  scegliere quello che si preferisce;

* Scaricare la distribuzione con o senza venditori. Se si ha `Git`_ installato
  sulla propria macchina, si dovrebbe scaricare Symfony2 senza venditori, perché
  aggiunge più flessibilità nell'inclusione delle librerie di terze parti.

Si scarichi uno degli archivi e lo si scompatti da qualche parte sotto la cartella
radice del web del proprio server. Da una linea di comando UNIX, si può farlo con
uno dei seguenti comandi (sostituire ``###`` con il vero nome del file):

.. code-block:: bash

    # per il file .tgz
    tar zxvf Symfony_Standard_Vendors_2.0.###.tgz

    # per il file .zip
    unzip Symfony_Standard_Vendors_2.0.###.zip

Finito il procedimento, si dovrebbe avere una cartella ``Symfony/``, che
assomiglia a questa:

.. code-block:: text

    www/ <- la propria cartella radice del web
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

Aggiornare i venditori
~~~~~~~~~~~~~~~~~~~~~~

Alla fine, se la scelta è caduta sull'archivio senza venditori, installare i
venditori eseguendo dalla linea di comando la seguente istruzione:

.. code-block:: bash

    php bin/vendors install

Questo comando scarica tutte le librerie dei venditori necessarie, incluso
Symfony stesso, nella cartella ``vendor/``. Per ulteriori informazioni sulla
gestione delle librerie di venditori di terze parti in Symfony2, si veda
":ref:`cookbook-managing-vendor-libraries`".

Configurazione
~~~~~~~~~~~~~~

A questo punto, tutte le librerie di terze parti che ci occorrono sono nella
cartella ``vendor/``. Abbiamo anche una configurazione predefinita dell'applicazione
in ``app/`` e un po' di codice di esempio in ``src/``.

Symfony2 dispone di uno strumento visuale per la verifica della configurazione del server,
per assicurarsi che il server web e PHP siano configurati per usare Symfony2. Usare il
seguente URL per la verifica della configurazione:

.. code-block:: text

    http://localhost/Symfony/web/config.php

Se ci sono problemi, correggerli prima di proseguire.

.. sidebar:: Impostare i permessi

    Un problema comune è che le cartelle ``app/cache`` e ``app/logs`` devono essere
    scrivibili sia dal server web che dall'utente della linea di comando. Su sistemi
    UNIX, se l'utente del server web è diverso da quello della linea di comando,
    si possono eseguire i seguenti comandi una sola volta sul proprio progetto, per
    assicurarsi che i permessi siano impostati correttamente. Cambiare ``www-data``
    con l'utente del server web e ``tuonome`` con l'utente della linea di comando:

    **1. Usare ACL su un sistema che supporta chmod +a**

    Molti sistemi consento di usare il comando ``chmod +a``. Provare prima questo e, in
    caso di errore, provare il metodo successivo:

    .. code-block:: bash

        rm -rf app/cache/*
        rm -rf app/logs/*

        sudo chmod +a "www-data allow delete,write,append,file_inherit,directory_inherit" app/cache app/logs
        sudo chmod +a "tuonome allow delete,write,append,file_inherit,directory_inherit" app/cache app/logs

    **2. Usare ACL su un sistema che non supporta chmod +a**

    Alcuni sistemi non supportano ``chmod +a``, ma supportano un altro programma
    chiamato ``setfacl``. Si potrebbe aver bisogno di `abilitare il supporto ACL`_ sulla
    propria partizione e installare setfacl prima di usarlo (come nel caso di Ubuntu),
    in questo modo:

    .. code-block:: bash

        sudo setfacl -R -m u:www-data:rwx -m u:tuonome:rwx app/cache app/logs
        sudo setfacl -dR -m u:www-data:rwx -m u:tuonome:rwx app/cache app/logs

    **3. Senza usare ACL**

    Se non è possibile modificare l'ACL delle cartelle, occorrerà modificare
    l'umask in modo che le cartelle cache e log siano scrivibili dal gruppo
    o da tutti (a seconda che gli utenti di server web e linea di comando siano
    o meno nello stesso gruppo). Per poterlo fare, inserire la riga seguente
    all'inizio dei file ``app/console``, ``web/app.php`` e
    ``web/app_dev.php``:

    .. code-block:: php

        umask(0002); // Imposta i permessi a 0775

        // oppure

        umask(0000); // Imposta i permessi a 0777

    Si noti che l'uso di ACL è raccomandato quando si ha accesso al server,
    perché la modifica di umask non è thread-safe.

Quando tutto è a posto, cliccare su "Go to the Welcome page" per accedere alla
prima "vera" pagina di Symfony2:

.. code-block:: text

    http://localhost/Symfony/web/app_dev.php/

Symfony2 dovrebbe dare il suo benvenuto e congratularsi per il lavoro svolto finora!

.. image:: /images/quick_tour/welcome.jpg

Iniziare lo sviluppo
--------------------

Ora che si dispone di un'applicazione Symfony2 pienamente funzionante, si può iniziare
lo sviluppo. La distribuzione potrebbe contenere del codice di esempio, verificare il file
``README.rst`` incluso nella distribuzione (aprendolo come file di testo) per sapere
quale codice di esempio è incluso nella distribuzione scelta e come poterlo rimuovere
in un secondo momento.

Per chi è nuovo in Symfony, in ":doc:`page_creation`" si può imparare come creare
pagine, cambiare configurazioni e tutte le altre cose di cui si avrà bisogno nella
nuova applicazione.

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

    vendor/

Ora la cartella dei venditori non sarà inviata in commi al controllo di sorgenti.
Questo è bene (anzi, benissimo!) perché quando qualcun altro clonerà o farà checkout
del progetto, potrà semplicemente eseguire lo script ``php bin/vendors install`` per
scaricare tutte le librerie dei venditori necessarie.

.. _`abilitare il supporto ACL`: https://help.ubuntu.com/community/FilePermissions#ACLs
.. _`http://symfony.com/download`: http://symfony.com/download
.. _`Git`: http://git-scm.com/
.. _`GitHub Bootcamp`: http://help.github.com/set-up-git-redirect
