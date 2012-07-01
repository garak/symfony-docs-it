.. index::
   single: Flusso di lavoro; Git

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
   (ovvero vicino al file ``composer.json``) e copiarvi le righe seguenti. I file corrispondenti
   a questi schemi saranno ignorati da git:

    .. code-block:: text

        /web/bundles/
        /app/bootstrap*
        /app/cache/*
        /app/logs/*
        /vendor/  
        /app/config/parameters.yml

.. tip::

   Si potrebbe anche voler creare un file .gitignore, da usare su tutto il sistema,
   nel qual caso, si possono trovare ulteriori informazioni qui: `Github .gitignore`_
   In questo modo, si possono escludere i file e le cartelle usati più spesso dal proprio IDE per ogni progetto.

4. Copiare ``app/config/parameters.yml`` in ``app/config/parameters.yml.dist``.
   Il file ``parameters.yml`` è ignorato da git (vedi sopra), quindi le impostazioni
   specifiche della macchina, come le password del database, non saranno inviate. Creando
   il file ``parameters.yml.dist``, i nuovi sviluppatori potranno clonare rapidamente il
   progetto, copiando questo file in ``parameters.yml`` e personalizzandolo.

5. Inizializzare il proprio repository git:

   .. code-block:: bash
    
        $ git init

6. Aggiungere tutti i file in git:

   .. code-block:: bash
    
        $ git add .

7. Creare un commit iniziale con il nuovo progetto:

   .. code-block:: bash
    
        $ git commit -m "Commit iniziale"

8. Infine, scaricare tutte le librerie dei venditori, eseguendo
   composer. Per dettagli, vedere :ref:`installation-updating-vendors`.

A questo punto, si ha un progetto Symfony2 pienamente funzionante e correttamente
copiato su git. Si può iniziare subito a sviluppare, inviando i commit delle
modifiche al proprio repository git.

Si può continuare a seguire il capitolo :doc:`/book/page_creation` per imparare
di più su come configurare e sviluppare la propria applicazione.

.. tip::

    Symfony2 Standard Edition è distribuito con alcuni esempi di funzionamento. Per
    rimuovere il codice di esempio, seguire le istruzioni nel file `Readme di Standard Edition`_.

.. _cookbook-managing-vendor-libraries:

.. include:: _vendor_deps.rst.inc

Venditori e sotto-moduli
~~~~~~~~~~~~~~~~~~~~~~~~

Invece di usare il sistema basato su ``composer.json`` per gestire le librerie
dei venditori, si potrebbe invece voler usare i `sotto-moduli di git`_.
Non c'è nulla di sbagliato in questo approccio, ma il sistema ``composer.json`` è la via
ufficiale per risolvere questo problema e probabilmente è più facile da gestire.
Diversamente dai sotto-moduli di git, ``Composer`` è abbaatanza intelligente da calcolare
le dipendenze tra le librerie.

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
un `repository privato`_ e usando quello. Una libreria che può aiutare in tal senso
è `Gitolite`_.

.. _`git`: http://git-scm.com/
.. _`Symfony2 Standard Edition`: http://symfony.com/download
.. _`Readme di Standard Edition`: https://github.com/symfony/symfony-standard/blob/master/README.md
.. _`sotto-moduli di git`: http://book.git-scm.com/5_submodules.html
.. _`GitHub`: https://github.com/
.. _`repository privato`: http://progit.org/book/ch4-4.html
.. _`Gitolite`: https://github.com/sitaramc/gitolite
.. _`Github .gitignore`: http://help.github.com/ignore-files/
