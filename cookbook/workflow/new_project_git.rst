Come creare e memorizzare un progetto Symfony2 in git
=====================================================

.. tip::

    Sebbene questa guida riguardi nello specifico git, gli stessi principi
    valgono in generale se si memorizza un progetto in Subversion.

Una volta letto :doc:`/book/page_creation` e preso familiarità con l'uso
di Symfony, si vorrà certamente iniziare un proprio progetto. In questa ricetta
si imparerà il modo migliore per iniziare un nuovo progetto Symfony2, memorizzato
usando il sistema di controllo dei sorgenti `git`_.

Preparazione del progetto
-------------------------

Per iniziare, occorre scaricare Symfony e inizializzare il repository
locale:

1. Scaricare `Symfony2 Standard Edition`_ senza venditori.

2. Scompattare la distribuzione. Questo creerà una cartella chiamata "Symfony" con la
   struttura del nuovo progetto, i file di configurazione, ecc. Si può rinominare la cartella a piacere.
   
3. Creare un nuovo file chiamato ``.gitignore`` nella radice del nuovo progetto
   (ovvero vicino al file ``deps``) e copiarvi le righe seguenti. I file corrispondenti
   a questi schemi saranno ignorati da git:

    .. code-block:: text

        /web/bundles/
        /app/bootstrap*
        /app/cache/*
        /app/logs/*
        /vendor/  
        /app/config/parameters.ini

4. Copiare ``app/config/parameters.ini`` in ``app/config/parameters.ini.dist``.
   Il file ``parameters.ini`` è ignorato da git (vedi sopra), quindi le impostazioni
   specifiche della macchina, come le password del database, non saranno inviate. Creando
   il file ``parameters.ini.dist``, i nuovi sviluppatori potranno clonare rapidamente il
   progetto, copiando questo file in ``parameters.ini`` e personalizzandolo.

5. Inizializzare il proprio repository git:

    .. code-block:: bash
    
        $ git init

6. Aggiungere tutti i file in git:

    .. code-block:: bash
    
        $ git add .

7. Creare un commit iniziale con il nuovo progetto:

    .. code-block:: bash
    
        $ git commit -m "Commit iniziale"

8. Infine, scaricare tutte le librerie dei venditori:

    .. code-block:: bash
    
        $ php bin/vendors install

A questo punto, si ha un progetto Symfony2 pienamente funzionante e correttamente
copiato su git. Si può iniziare subito a sviluppare, inviando i commit delle
modifiche al proprio repository git.

Si può continuare a seguire il capitolo :doc:`/book/page_creation` per imparare
di più su come configurare e sviluppare la propria applicazione.

.. tip::

    Symfony2 Standard Edition è distribuito con alcuni esempi di funzionamento. Per
    rimuovere il codice di esempio, seguire le istruzioni nel `file Readme di Standard Edition`_.

Gestire le librerie dei venditori
---------------------------------

Ogni progetto Symfony usa un nutrito gruppo di librerie di "venditori" di terze parti.

Per impostazione predefinita, tali librerie sono scaricate tramite lo script ``php bin/vendors install``.
Tale script legge il file ``deps`` e scarica le librerie specificate nella cartella
``vendor/``. Legge anche il file ``deps.lock``, facendo puntare ogni libreria al
relativo commit di git.

In questa preparazione, le librerie devi venditori sono parte del proprio repository git,
nemmeno come sotto-moduli. Ci si basa invece sui file ``deps`` e ``deps.lock`` e sullo
script ``bin/vendors`` per gestire tutto. Questi file sono parte del proprio repository,
quindi le versioni necessarie di ogni libreria di terze parti sono versionate in git
e si può usare lo script per mantenere il proprio progetto
aggiornato.

Ogni volta che uno sviluppatore clona un progetto, deve eseguire lo script ``php bin/vendors install``
per assicurarsi che tutte le necessarie librerie dei venditori siano scaricate.

.. sidebar:: Aggiornare Symfony

    Poiché Symfony non è altro che un gruppo di librerie di terze parti, controllate
    interamente tramite i file ``deps`` e ``deps.lock``,
    aggiornare Symfony significa semplicemente aggiornare ciascuno di questi file,
    per far corrispondere il loro stato all'ultima versione di Symfony Standard Edition.

    Ovviamente, se si aggiungono nuove voci a ``deps`` o a ``deps.lock``, ci si deve
    assicurare di sostituire solo le parti originale (ovvero assicurarsi di non
    cancellare nessuna delle voci personalizzate).

.. caution::

    C'è anche un comando ``php bin/vendors upgrade``, ma non ha niente a che fare con
    l'aggiornamento del proprio progetto e probabilmente non si avrà mai bisogno di
    usarlo.

Venditori e sotto-moduli
~~~~~~~~~~~~~~~~~~~~~~~~

Invece di usare il sistema basato su ``deps`` e ``bin/vendors`` per gestire le librerie
dei venditori, si potrebbe invece voler usare i `sotto-moduli di git`_.
Non c'è nulla di sbagliato in questo approccio, ma il sistema ``deps`` è la via
ufficiale per risolvere questo problema e i sotto-moduli di git possono a volte
creare delle difficoltà.

Memorizzare il progetto su un server remoto
-------------------------------------------

Si è ora in possesso di un progetto Symfony2 pienamente funzionante e copiato in git.
Tuttavia, spesso si vuole memorizzare il proprio progetto un server remoto, sia per
motivi di backup, sia per fare in modo che altri sviluppatori possano collaborare
al progetto.

Il modo più facile per memorizzare il proprio progetto su un server remoto è l'utilizzo
di `GitHub`_. I repository pubblici sono gratuiti, mentre per quelli privati è necessario
pagare mensilmente.

In alternativa, si può ospitare un proprio repository git su un qualsiasi server, creando
un `barebones repository`_ e usando quello. Una libreria che può aiutare in tal senso
è `Gitosis`_.

.. _`git`: http://git-scm.com/
.. _`Symfony2 Standard Edition`: http://symfony.com/download
.. _`Readme di Standard Edition`: https://github.com/symfony/symfony-standard/blob/master/README.md
.. _`sotto-moduli di git`: http://book.git-scm.com/5_submodules.html
.. _`GitHub`: https://github.com/
.. _`barebones repository`: http://progit.org/book/ch4-4.html
.. _`Gitosis`: https://github.com/res0nat0r/gitosis
