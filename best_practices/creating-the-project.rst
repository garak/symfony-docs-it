Creare il progetto
==================

Installazione di Symfony
------------------------

In passato, i progetti Symfony erano creati con `Composer`_, il gestore di dipendenze
per applicazioni PHP. Tuttavia, la raccomandazione attuale è di usare **l'installatore di Symfony**,
che deve essere installato prima della creazione del primo progetto.

.. best-practice::

    Usare l'installatore di Symfony per creare nuovi progetti basati su Symfony.

Leggere il :doc:`capitolo dell'installazione </book/installation>` del libro di Symfony per
sapere come installare e usare l'installatore di Symfony.

.. _linux-and-mac-os-x-systems:
.. _windows-systems:

Creare un'applicazione Blog
---------------------------

Ora che è tutto pronto, si può creare un nuovo progetto, basato su
Symfony. Nella console dei comandi, andare nella cartelle in cui si hanno i permessi
per creare file ed eseguire i comandi seguenti:

.. code-block:: bash

    # Linux, Mac OS X
    $ cd progetti/
    $ symfony new blog

    # Windows
    c:\> cd progetti/
    c:\progetti\> php symfony.phar new blog

Questo comando crea una nuova cartella chiamata ``blog``, con dentro un nuovo
progetto, basato sulla versione stabile più recente di Symfony al momento disponibile. Inoltre,
l'installatore  verifica se il sistema soddisfa i requisiti tecnici per eseguire le applicazioni
Symfony. In caso contrario, si vedrà una lista di modifiche necessarie per soddisfare tali
requisiti.

.. tip::

    Per ragioni di sicurezza, tutte le versioni di Symfony sono firmate digitalmente. Se si
    vuole verificare l'integrità di una versione di Symfony, dare un'occhiata al
    `repository dei checksum pubblici`_ e
    seguire `questi passi`_ .

Strutturare l'applicazione
--------------------------

Dopo aver creato l'applicazione, spostandosi nella cartella ``blog/``, si vedrà il seguente
insieme di file e cartelle, generato automaticamente:

.. code-block:: text

    blog/
    ├─ app/
    │  ├─ console
    │  ├─ cache/
    │  ├─ config/
    │  ├─ logs/
    │  └─ Resources/
    ├─ src/
    │  └─ AppBundle/
    ├─ vendor/
    └─ web/

Questa gerarchia di file e cartelle è la struttura proposta da Symfony per
organizzare un'applicazione. Lo scopo di ciascuna cartella è il
seguente:

* ``app/cache/`` contiene tutti i file di cache generati dall'applicazione;
* ``app/config/`` contiene la configurazione definita per ciascun ambiente;
* ``app/logs/`` contiene tutti i file di log generati dall'applicazione;
* ``app/Resources/`` contiene tutti i file template e di traduzione
    dell'applicazione;
* ``src/AppBundle/`` contiene codice specifico per Symfony (controllori e rotte),
   codice di dominio (p.e. le classi Doctrine) e tutta la logica di business;
* ``vendor/`` in questa cartella Composer installa tutte le dipendenze dell'applicazione;
    non si dovrebbe mai modificare il suo contenuto;
* ``web/`` contiene il front controller e tutti le risorse per il web, come i fogli di stile, i
   file JavaScript e le immagini.

I bundle dell'applicazione
--------------------------

Quando è stato rilasciato Symfony 2.0, la maggior parte degli sviluppatori ha adottato, in modo naturale,
lo stesso approccio usato in symfony 1.x, suddividendo l'applicazione in moduli logici. Proprio per questo,
molte applicazioni Symfony separano i bundle dal punto di vista logico: UserBundle,
ProductBundle, InvoiceBundle, eccetera.

Tuttavia, i bundle sono stati concepiti come moduli software da riutilizzare in maniera
autonoma. Se UserBundle non può essere riusato "così com'è" in un'altra applicazione Symfony,
allora non è più un bundle. Inoltre, InvoiceBundle dipende da
ProductBundle, quindi non esiste alcun vantaggio ad avere due bundle separati.

.. best-practice::

    Creare solamente un bundle, chiamato AppBundle, per la logica dell'applicazione

Implementando solamente il bundle AppBundle in un progetto, si renderà il codice più conciso
e facile da capire. A partire da Symfony 2.6, la documentazione ufficiale di
Symfony mostra gli esempi con il bundle AppBundle.

.. note::

    Non è necessario aggiungere il prefisso dell'azienda (*vendor*) ad AppBundle (p.e.
    AcmeAppBundle), dato che questo bundle, specifico dell'applicazione, non verrà mai
    condiviso con terzi.

Detto questo, la struttura di cartelle raccomandata di un'applicazione Symfony
è la seguente:

.. code-block:: text

    blog/
    ├─ app/
    │  ├─ console
    │  ├─ cache/
    │  ├─ config/
    │  ├─ logs/
    │  └─ Resources/
    ├─ src/
    │  └─ AppBundle/
    ├─ vendor/
    └─ web/
       ├─ app.php
       └─ app_dev.php

.. tip::

    Se l'installazione di Symfony non dispone di un AppBundle già generato,
    lo si può generare a mano, con questo comando:

    .. code-block:: bash

        $ php app/console generate:bundle --namespace=AppBundle --dir=src --format=annotation --no-interaction

Estendere la struttura delle cartelle
-------------------------------------

Se un progetto o un'infrastruttura richiedono alcune modifiche alla struttura predefinita
delle cartelle, è possibile
:doc:`ridefinire la posizione delle cartelle principali </cookbook/configuration/override_dir_structure>`:
``cache/``, ``logs/`` e ``web/``.

Symfony3, inoltre, userà una struttura di cartelle leggermente diversa, quando
sarà rilasciato:

.. code-block:: text

    blog-symfony3/
    ├─ app/
    │  ├─ config/
    │  └─ Resources/
    ├─ bin/
    │  └─ console
    ├─ src/
    ├─ var/
    │  ├─ cache/
    │  └─ logs/
    ├─ vendor/
    └─ web/

Le modifiche sono piuttosto superficiali ma, per ora, si consiglia di utilizzare
la struttura di cartelle di Symfony2.

.. _`Composer`: https://getcomposer.org/
.. _`Get Started`: https://getcomposer.org/doc/00-intro.md
.. _`Composer download page`: https://getcomposer.org/download/
.. _`repository dei checksum pubblici`: https://github.com/sensiolabs/checksums
.. _`questi passi`: http://fabien.potencier.org/article/73/signing-project-releases
