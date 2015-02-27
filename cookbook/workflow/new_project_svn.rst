.. index::
   single: Flusso di lavoro; Subversion

.. _how-to-create-and-store-a-symfony2-project-in-subversion:

Creare e memorizzare un progetto Symfony in Subversion
======================================================

.. tip::

    Questa voce è specifica per Subversion e si basa sui principi
    di :doc:`/cookbook/workflow/new_project_git`.

Una volta letto :doc:`/book/page_creation` e aver preso familiarità con l'uso
di Symfony, si è senza dubbio pronti per iniziare un progetto. Il metodo
preferito per gestire progetti Symfony è l'uso di `git`_, ma qualcuno preferisce
usare `Subversion`_, che va bene lo stesso! In questa ricetta, si vedrà come
gestire un progetto usando `svn`_, in modo simile a quanto si farebbe
con `git`_.

.. tip::

    Questo è **un** metodo per memorizzare un progetto Symfony in un
    repository Subversion. Ci sono molti modi di farlo e questo è semplicemente
    uno che funziona.

Il repository Subversion
------------------------

Per questa ricetta, si suppone che lo schema del repository segua la struttura
standard, molto diffusa:

.. code-block:: text

    progetto/
        branches/
        tags/
        trunk/

.. tip::

    La maggior parte degli host con subversion dovrebbero seguire questa pratica.
    Questo è lo schema raccomandato in `Controllo di versione con Subversion`_ e quello
    usato da quasi tutti gli host gratuiti (vedere :ref:`svn-hosting`).

Preparazione del progetto
-------------------------

Per iniziare, occorre scaricare Symfony e preparare Subversion:

1. Scaricare `Symfony Standard Edition`_, con o senza venditori.

2. Scompattare la distribuzione. Questo creerà una cartella chiamata Symfony, con
   la struttura del nuovo progetto, i file di configurazione, ecc. Rinominarla con
   il nome che si desidera.

3. Eseguire il checkout del repository Subversion che ospiterà questo progetto. Supponiamo
   che sia ospitato su `Google code`_ e che si chiami ``progetto``:

   .. code-block:: bash

        $ svn checkout http://progetto.googlecode.com/svn/trunk progetto

4. Copiare i file del progetto Symfony nella cartella di subversion:

   .. code-block:: bash

        $ mv Symfony/* progetto/

5. Impostare le regole di ignore. Non tutto *andrebbe* memorizzato nel
   repository subversion. Alcuni file (come la cache) sono generati e altri
   (come la configurazione della base dati) devono essere personalizzati su
   ciascuna macchina. Ciò implica l'uso della proprietà ``svn:ignore``, che consente
   di ignorare specifici file.

   .. code-block:: bash

        $ cd progetto/
        $ svn add --depth=empty app app/cache app/logs app/config web

        $ svn propset svn:ignore "vendor" .
        $ svn propset svn:ignore "bootstrap*" app/
        $ svn propset svn:ignore "parameters.yml" app/config/
        $ svn propset svn:ignore "*" app/cache/
        $ svn propset svn:ignore "*" app/logs/

        $ svn propset svn:ignore "bundles" web

        $ svn ci -m "commit della lista di ignore di Symfony (vendor, app/bootstrap*, app/config/parameters.yml, app/cache/*, app/logs/*, web/bundles)"

6. Tutti gli altri file possono essere aggiunti al progetto:

   .. code-block:: bash

        $ svn add --force .
        $ svn ci -m "aggiunta Symfony Standard 2.X.Y"

7. Copiare ``app/config/parameters.yml`` su ``app/config/parameters.yml.dist``.
   Il file ``parameters.yml`` è ignorato da svn (vedere sopra) in modo che le
   impostazioni delle singole macchine, come le password della base dati, non siano
   inserite. Creando il file ``parameters.yml.dist``, i nuovi sviluppatori possono
   prendere subito il progetto, copiare questo file in ``parameters.yml``, personalizzarlo
   e iniziare a sviluppare.

8. Infine, scaricare tutte le librerie dei venditori, eseguendo
   composer. Per maggiori dettagli, vedere :ref:`installation-updating-vendors`.

.. tip::

    Se ci si basa su versioni "dev", ci si può basare su git per installare
    tali librerie, poiché non dispongono di archivi da scaricare.

A questo punto, si ha un progetto Symfony pienamente funzionante, memorizzato nel
proprio repository Subversion. Si può iniziare lo sviluppo, con i commit verso
il repository.

Si può continuare a seguire il capitolo :doc:`/book/page_creation` per imparare
di più su come configurare e sviluppare un'applicazione.

.. tip::

    La Standard Edition di Symfony ha alcune funzionalità di esempio. Per rimuovere
    il codice di esempio, seguire le istruzioni nella ricetta
    ":doc:`/cookbook/bundles/remove`".

.. include:: _vendor_deps.rst.inc

.. _svn-hosting:

Soluzioni di hosting subversion
-------------------------------

La differenza maggiore tra `git`_ e `svn`_ è che Subversion *necessita* di un
repository centrale per funzionare. Ci sono diverse soluzioni:

- Hosting autonomo: creare il proprio repository e accedervi tramite filesystem o
  tramite rete. Per maggiori informazioni, leggere
  `Controllo di versione con Subversion`_.

- Hosting di terze parti: ci sono molte buone soluzioni di hosting gratuito a disposizione,
  come `GitHub`_, `Google code`_, `SourceForge`_ o `Gna`_. Alcune di queste offrono
  anche hosting git.

.. _`git`: http://git-scm.com/
.. _`svn`: http://subversion.apache.org/
.. _`Subversion`: http://subversion.apache.org/
.. _`Symfony Standard Edition`: http://symfony.com/download
.. _`Controllo di versione con Subversion`: http://svnbook.red-bean.com/
.. _`GitHub`: https://github.com/
.. _`Google code`: http://code.google.com/hosting/
.. _`SourceForge`: http://sourceforge.net/
.. _`Gna`: http://gna.org/
