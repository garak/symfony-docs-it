.. index::
   single: Bundles; Best Practices

Bundle Best Practice
====================

Un bundle è una directory con una struttura ben definita che può contenere
qualsiasi cosa da classi a controller e risorse web. Anche se i bundle sono
molto flessibili, è consigliabile seguire alcune best practice se si ha
intenzione di distribuirli.

.. index::
   pair: Bundles; Naming Conventions

Nome del Bundle
---------------

Un bundle è anche un namespace PHP composto da diversi segmenti:

* Il **namespace principale**: ``Bundle`` per bundle riutilizzabili o 
  ``Application`` per bundle specifici di un'applicazione;
* Il **vendor namespace** (opzionale per gli ``Application`` bundle): qualcosa
  di univoco per voi o per la vostra azienda (esempio ``Sensio``);
* *(opzionale)* I **namespace di categoria** per organizzare meglio un grande
  insieme di bundle;
* Il **nome del bundle**.

.. caution::
   Il namespace del vendor e quello della categoria sono disponibili solo
   dalla PR3 di Symfony2.

Il nome del bundle deve seguire queste semplici regole:

* Usare solo caratteri alfanumerici e unserscore;
* Usare un nome CamelCase;
* Usare un nome breve e descrittivo (non più di due parole);
* Usare come prefisso al nome la concatenazione dei namespace del vendor
  e della categoria:
* Usare ``Bundle`` come suffisso al nome.

Alcuni buoni nomi per bundle:

=================================== ==========================
Namespace                           Nome del Bundle
=================================== ==========================
``Bundle\Sensio\BlogBundle``        ``SensioBlogBundle``
``Bundle\Sensio\Social\BlogBundle`` ``SensioSocialBlogBundle``
``Application\BlogBundle``          ``BlogBundle``
=================================== ==========================

Struttura delle Directory
-------------------------

La struttura base delle directory di un bundle ``HelloBundle`` deve essere
la seguente::

    XXX/...
        HelloBundle/
            HelloBundle.php
            Controller/
            Resources/
                meta/
                    LICENSE
                config/
                doc/
                    index.rst
                translations/
                views/
                web/
            Tests/

La o le ``XXX`` directory riflettono la struttura del namespace del bundle.

I seguenti file sono obbligatori:

* ``HelloBundle.php``;
* ``Resources/meta/LICENSE``: L'intera licenza del codice;
* ``Resources/doc/index.rst``: Il file radice per la documentazione del Bundle.

.. note::
   Queste convenzioni assicurano agli strumenti automatici di poter lavorare
   su una struttura standard.

La profondità delle sotto-directory dovrebbe essere mantenuta al minimo possibile
per le classi ed i file più utilizzati (al massimo 2 livelli). Ulteriori livelli
possono essere definiti per file non strategici e meno utilizzati.

La directory del bundle è accessibile in sola lettura. Nel caso in cui si avesse
la necessità di scrivere dei file temporanei essi andrebbero posizionati nelle
directory ``cache/`` o ``log/`` dell'applicazione host. Gli strumenti possono
generare file nella struttura delle directory del bundle ma solo se i file
generati faranno parte del repository.

I seguenti file e classi hanno specifici posizionamenti:

========================= ===========================
Tipo                      Directory
========================= ===========================
Controller                ``Controller/``
File di traduzione        ``Resources/translations/``
Template                  ``Resources/views/``
Unit e Functional Test    ``Tests/``
Risorse Web               ``Resources/web/``
Configurazione            ``Resources/config/``
Comandi                   ``Command/``
========================= ===========================

Classi
------

La struttura delle directory del bundle è utilizzata come la gerarchia del
namespace. Per esempio un controller ``HelloController`` è posizionato in
``Bundle/HelloBundle/Controller/HelloController.php`` ed il nome completo
della classe è ``Bundle\HelloBundle\Controller\HelloController``.

Tutte le classi ed i file devono seguire i coding :doc:`standards
</contributing/code/standards>` di Symfony2.

Alcune classi dovrebbero essere di facciata e dovrebbero essere più brevi
possibile come Commands, Helpers, Listeners, and Controllers.

Classi che si connettono all'Event Dispatcher dovrebbero avere il suffisso
``Listener``.

Classi delle eccezioni dovrebbero essere memorizzate in un sotto-namespace
``Exception``.

Vendor
------

Un bundle non può contenere librerie PHP di terze parti. Deve invece essere
basato sul sistema standard di autoloading di Symfony2.

Un bundle non dovrebbe contenere librerie di terze parti scritte in JavaScript,
CSS o altri linguaggi.

Test
----

Un bundle dovrebbe essere distribuito con una test suite scritta con PHPUnit
e posizionata nella directory ``Tests/``. I Test dovrebbero seguire i
seguenti principi:

* La suite di test deve essere eseguibile chiamando semplicemente il comando
  ``phpunit`` da un'applicazione d'esempio;
* I test funzionali dovrebbero essere utilizzati solamente per testare l'output
  di risposta e fornire eventuali informazioni di profilig se presenti;
* Il code coverage dovrebbe coprire almeno il 95% del codice.

.. note::
   Una suite di test non deve contenere uno script ``AllTests.php`` ma deve
   poter contare sull'esistenza del file ``phpunit.xml.dist``.

Documentazione
--------------

Tutte le classi e le funzioni devono essere complete di PHPDoc.

Documentazione più approfondita dovrebbe essere fornita nel formato 
:doc:`reStructuredText</contributing/documentation/format>`, nella directory
``Resources/doc/``; il file ``Resources/doc/index.rst`` è l'unico obbligatorio.

Controller
----------

I controller di un bundle non devono estendere
:class:`Symfony\\Bundle\\FrameworkBundle\\Controller\\Controller`. Essi possono
invece implementare
:class:`Symfony\\Foundation\\DependencyInjection\\ContainerAwareInterface` oppure
estender :class:`Symfony\\Foundation\\DependencyInjection\\ContainerAware`.

.. note::
    Osservando i metodi :class:`Symfony\\Bundle\\FrameworkBundle\\Controller\\Controller`
    si può vedere come essi siano semplicemente delle scorciatoie per rendere più
    semplice l'apprendimento.

Template
--------

Se un bundle fornisce template essi dovrebbero essere definiti semplicemente
in PHP. Un bundle non fornisce un layout principale ma estende un template
di default ``base`` (che deve contenere due slot: ``content`` e ``head``).

.. note::
   L'unico altro template engine supportato è Twig ma solo per casi specifici.

File di Traduzione
------------------

Se un bundle fornisce messagi tradotti essi devono essere definiti nel formato
XLIFF; il dominio dovrebbe essere specificato dopo il nome del bundle
(``bundle.hello``).

Un bundle non deve sovrascrivere un messaggio esistente di un altro bundle.

Configurazione
--------------

La configurazione deve essere fatta con il :doc:`mechanism
</guides/bundles/configuration>` integrato in Symfony2. Un bundle dovrebbe
offrire tutte le sue configurazioni di default in XML.
