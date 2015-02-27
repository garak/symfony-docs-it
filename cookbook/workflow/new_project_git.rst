.. index::
   single: Flusso di lavoro; Git

.. _how-to-create-and-store-a-symfony2-project-in-git:

Creare e memorizzare un progetto Symfony in git
===============================================

.. tip::

    Sebbene questa guida riguardi nello specifico git, gli stessi principi
    valgono in generale se si memorizza un progetto in Subversion.

Una volta letto :doc:`/book/page_creation` e preso familiarità con l'uso
di Symfony, si vorrà certamente iniziare un proprio progetto. In questa ricetta
si imparerà il modo migliore per iniziare un nuovo progetto Symfony, memorizzato
usando il sistema di controllo dei sorgenti `git`_.

Preparazione del progetto
-------------------------

Per iniziare, occorre scaricare Symfony e inizializzare il repository
locale:

#. Scaricare `Symfony Standard Edition`_, usando Composer:

   .. code-block:: bash

        $ php composer.phar create-project symfony/framework-standard-edition path/ "~2.5"

.. tip::

    Aggiungere l'opzione  ``-vvv`` per vedere cosa sta facendo Composer: Questo è particolarmente
    utile su una connessione lenta, in cui si potrebbe avere l'impressione che non stia accadendo
    nulla.

   Composer scaricherà la Standard Edition, insieme a tutti le necessarie
   librerie dei venditori. Per maggiori informazioni su come scaricare Symfony con
   Composer, vedere `installare Symfony usando Composer`_.

#. Inizializzare il repository git:

   .. code-block:: bash

        $ git init

#. Aggiungere tutti i file in git:

   .. code-block:: bash

        $ git add .

   .. tip::

      Non tutti i file che sono stati scaricati da Composer nel primo passo
      verranno aggiunti a Git. Alcuni file e cartelle non vanno in Git, come le dipendenze del progetto
      (gestite da Composer), ``parameters.yml`` (che contiene informazioni sensibili,
      come le credenziali per la base dati), i file di log e di cache e le risorse esportate (che sono
      create automaticamente dal progetto). Per evitare di aggiungere accidentalmente tali file
      e cartelle, la Standard Edition dispone di un
      file chiamato ``.gitignore``, che contiene una lista di file e cartelle che Git deve
      ignorare.

   .. tip::

      Si può anche creare un file ``.gitignore`` da usare per tutto il sistema.
      Questo consente di escludere file e cartelle, in ogni progetto, che vengono creati
      da un IDE o dal sistema operativo. Per maggiori dettagli, vedere `GitHub .gitignore`_.

#. Creare un commit iniziale con il nuovo progetto:

   .. code-block:: bash

        $ git commit -m "Commit iniziale"

A questo punto, si ha un progetto Symfony pienamente funzionante e correttamente
copiato su git. Si può iniziare subito a sviluppare, inviando i commit delle
modifiche al proprio repository git.

Si può continuare a seguire il capitolo :doc:`/book/page_creation` per imparare
di più su come configurare e sviluppare un'applicazione.

.. tip::

    Symfony Standard Edition è distribuito con alcuni esempi di funzionamento. Per
    rimuovere il codice di esempio, seguire le istruzioni nella ricetta
    ":doc:`/cookbook/bundles/remove`".

.. _cookbook-managing-vendor-libraries:

.. include:: _vendor_deps.rst.inc

Venditori e sottomoduli
~~~~~~~~~~~~~~~~~~~~~~~

Invece di usare il sistema basato su ``composer.json`` per gestire le librerie
dei venditori, si potrebbe invece voler usare i `sottomoduli di git`_.
Non c'è nulla di sbagliato in questo approccio, ma il sistema ``composer.json`` è la via
ufficiale per risolvere questo problema e probabilmente è più facile da gestire.
Diversamente dai sottomoduli di git, ``Composer`` è abbastanza intelligente da calcolare
le dipendenze tra le librerie.

Memorizzare il progetto su un server remoto
-------------------------------------------

Si è ora in possesso di un progetto Symfony pienamente funzionante e memorizzato in git.
Tuttavia, spesso si vuole memorizzare un progetto un server remoto, sia per
questioni di backup, sia per fare in modo che altri sviluppatori possano collaborare
al progetto stesso.

Il modo più facile per memorizzare un progetto su un server remoto è l'utilizzo
di servizi come `GitHub`_ o `Bitbucket`. Ovviamente ci sono molti altri servizi
in giro, si può iniziare una ricerca su
`confronto tra servizi di hosting`_.

In alternativa, si può ospitare un proprio repository git su un qualsiasi server, creando
un `repository privato`_ e usando quello. Una libreria che può aiutare in tal senso
è `Gitolite`_.

.. _`git`: http://git-scm.com/
.. _`Symfony Standard Edition`: http://symfony.com/download
.. _`installare Symfony usando Composer`: http://symfony.com/doc/current/book/installation.html#option-1-composer
.. _`sottomoduli di git`: http://git-scm.com/book/en/Git-Tools-Submodules
.. _`GitHub`: https://github.com/
.. _`repository privato`: http://git-scm.com/book/en/Git-Basics-Getting-a-Git-Repository
.. _`Gitolite`: https://github.com/sitaramc/gitolite
.. _`GitHub .gitignore`: https://help.github.com/articles/ignoring-files
.. _`Bitbucket`: https://bitbucket.org/
.. _`confronto tra servizi di hosting`: http://en.wikipedia.org/wiki/Comparison_of_open-source_software_hosting_facilities
