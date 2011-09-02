Un quadro generale
==================

Volete provare Symfony2 avendo a disposizione solo dieci minuti? Questa prima
parte di questa guida è stata scritta appositamente: spiega come
partire subito con Symfony2, mostrando la struttura di un semplice progetto
già pronto.

Chi ha già usato un framework per il web si troverà come a casa con Symfony2.

.. tip::

    Si vuole imparare perché e quando si ha bisogno di un framework? Si legga il
    documento "`Symfony in 5 minuti`_".

Scaricare Symfony2
------------------

Prima di tutto, verificare di avere almeno PHP 5.3.2 (o successivo) installato e configurato correttamente per funzionare con un server web,
come Apache.

Pronti? Iniziamo scaricando "`Symfony2 Standard Edition_`", una :term:`distribuzione`
di Symfony preconfigurata per gli usi più comuni e che contiene anche del codice
che dimostra come usare Symfony2 (con l'archivio che include i *venditori*, si
parte ancora più velocemente).

Scaricare l'archivio e scompattarlo nella cartella radice del web. Si dovrebbe
ora avere una cartella ``Symfony/``, come la seguente:

.. code-block:: text

    www/ <- cartella radice del web
        Symfony/ <- archivio scompattato
            app/
                cache/
                config/
                logs/
                Resources/
            bin/
            src/
                Acme/
                    DemoBundle/
                        Controller/
                        Resources/
                        ...
                vendor/
                    symfony/
                doctrine/
                ...
            web/
                app.php
                ...

.. note::

    Se è stata scaricata la Standard Edition *senza venditori*, basta eseguire
    il comando seguente per scaricare tutte le librerie dei venditori:

    .. code-block:: bash

        php bin/vendors install

Verifica della configurazione
-----------------------------

Per evitare mal di testa successivamente, Symfony2 dispone di uno strumento per testare
la configurazione, per verificare configurazioni errate di PHP o del server web. Usare
il seguente URL per avviare la diagnosi sulla propria macchina:

.. code-block:: text

    http://localhost/Symfony/web/config.php

Se ci sono dei problemi, correggerli. Si potrebbe anche voler modificare la propria
configurazione, seguendo le raccomandazioni fornite. Quando è tutto a posto,
cliccare su "*Bypass configuration and go to the Welcome page*" per richiedere
la prima "vera" pagina di Symfony2:

.. code-block:: text

    http://localhost/Symfony/web/app_dev.php/

Symfony2 dovrebbe congratularsi per il duro lavoro svolto finora!

.. image:: /images/quick_tour/welcome.jpg
   :align: center

Capire i fondamenti
-------------------

Uno degli obiettivi principali di un framework è quello di assicurare la `Separazione degli ambiti`_.
Ciò mantiene il proprio codice organizzato e consente alla propria applicazione di
evolvere facilmente nel tempo, evitando il miscuglio di chiamate al database, tag HTML
e logica di business nello stesso script. Per raggiungere questo obiettivo con Symfony,
occorre prima imparare alcuni termini e concetti fondamentali.

.. tip::

    Chi volesse la prova che usare un framework sia meglio che mescolare tutto nello
    stesso script, legga il capitolo ":doc:`/book/from_flat_php_to_symfony2`" del
    libro.

La distribuzione offre alcuni esempi di codice, che possono essere usati per capire meglio
i concetti fondamentali di Symfony. Si vada al seguente URL per essere salutati da Symfony2
(sostituire *Fabien* col proprio nome):

.. code-block:: text

    http://localhost/Symfony/web/app_dev.php/demo/hello/Fabien

.. image:: /images/quick_tour/hello_fabien.png
   :align: center

Cosa sta accadendo? Dissezionamo l'URL:

* ``app_dev.php``: È un :term:`front controller`. È l'unico punto di ingresso
   dell'applicazione e risponde a ogni richiesta dell'utente;

* ``/demo/hello/Fabien``: È il *percorso virtuale* alla risorsa a cui l'utente
  vuole accedere .

È responsabilità dello sviluppatore scrivere il codice che mappa la *richiesta*
dell'utente (``/demo/hello/Fabien``) alla *risorsa* a essa associata
(la pagina HTML ``Hello Fabien!``).

Rotte
~~~~~

Symfony2 dirige la richiesta al codice che la gestisce, cercando la corrispondenza
tra l'URL richiesto e alcuni schemi configurati. Per impostazione predefinita, questi
schemi (chiamate "rotte") sono definite nel file di configurazione ``app/config/routing.yml``.
Se si è nell':ref:`ambiente<quick-tour-big-picture-environments>` ``dev``,
indicato dal front controller app_**dev**.php, viene caricato il file di configurazione
``app/config/routing_dev.yml``. Nella Standard Edition, le rotte delle pagine di demo
sono in quel file:

 .. code-block:: yaml

    # app/config/routing_dev.yml
    _welcome:
            pattern:  /
        defaults: { _controller: AcmeDemoBundle:Welcome:index }

    _demo:
        resource: "@AcmeDemoBundle/Controller/DemoController.php"
        type:     annotation
        prefix:   /demo

    # ...

Le prime righe (dopo il commento) definiscono quale codice
richiamare quanto l'utente richiede la risorsa "``/``" (come la pagina di benvenuto
vista prima). Quando richiesto, il controllore ``AcmeDemoBundle:Welcome:index`` sarà
eseguito. Nella prossima sezione, si imparerà esattamente quello che significa.

.. tip::

    La Standard Edition usa `YAML`_  per i suoi file di configurazione,
    ma Symfony2 supporta nativamente anche XML, PHP e le annotazioni.
    I diversi formati sono compatibili e possono essere usati alternativamente
    in un'applicazione. Inoltre, le prestazioni dell'applicazione non dipendono
    dal formato scelto, perché tutto viene messo in cache alla prima richiesta.


Controllori
~~~~~~~~~~~

Il controllore è una funzione o un metodo PHP che gestisce le *richieste* in entrata
e restituisce delle *risposte* (spesso codice HTML). Invece di usare variabili e
funzioni globali di PHP (come ``$_GET`` or ``header()``) per gestire questi messaggi
HTTP, Symfony usa degli oggetti: :class:`Symfony\\Component\\HttpFoundation\\Request`
e :class:`Symfony\\Component\\HttpFoundation\\Response`.  Il controllore più semplice
possibile potrebbe creare la risposta a mano, basandosi sulla richiesta::

    use Symfony\Component\HttpFoundation\Response;

    $name = $request->query->get('name');

    return new Response('Hello '.$name, 200, array('Content-Type' => 'text/plain'));

.. note::

    Symfony2 abbraccia le specifiche HTTP, che sono delle regole che governano
    tutte le comunicazioni sul web. Si legga il capitolo ":doc:`/book/http_fundamentals`"
    del libro per sapere di più sull'argomento e sulle sue potenzialità.


Symfony2 sceglie il controllore basandosi sul valore ``_controller`` della configurazione
delle rotte: ``AcmeDemoBundle:Welcome:index``. Questa stringa è il *nome logico* del
controllore e fa riferimento al metodo ``indexAction`` della classe
``Acme\DemoBundle\Controller\WelcomeController``::

    // src/Acme/DemoBundle/Controller/WelcomeController.php
    namespace Acme\DemoBundle\Controller;

    use Symfony\Bundle\FrameworkBundle\Controller\Controller;

    class WelcomeController extends Controller
    {
        public function indexAction()
        {
            return $this->render('AcmeDemoBundle:Welcome:index.html.twig');
        }
    }

.. tip::

    Si sarebbero potuti usare i nomi completi di classe e metodi,
    ``Acme\DemoBundle\Controller\WelcomeController::indexAction``, per il valore
    di ``_controller``. Ma se si seguono alcune semplici convenzioni, il nome logico
    è più breve e consente maggiore flessibilità.

La classe ``WelcomeController`` estende la classe predefinita ``Controller``,
che fornisce alcuni utili metodi scorciatoia, come il metodo
:method:`Symfony\\Bundle\\FrameworkBundle\\Controller\\Controller::render`, che
carica e rende un template (``AcmeDemoBundle:Welcome:index.html.twig``).
Il vaore restituito è un oggetto risposta, popolato con il contenuto resto. Quindi,
se ci sono nuove necessità, l'oggetto risposta può essere manipolato prima di essere
inviato al browser::

    public function indexAction()
    {
        $response = $this->render('AcmeDemoBundle:Welcome:index.txt.twig');
        $response->headers->set('Content-Type', 'text/plain');

        return $response;
    }

Indipendentemente da come lo si raggiunge, lo scopo finale del proprio controllore
è sempre quello di restituire l'oggetto ``Response`` da inviare all'utente. Questo
oggetto ``Response`` può essere popolato con codice HTML, rappresentare un rinvio del
client o anche restituire il contenuto di un'immagine JPG, con un header ``Content-Type`` del valore ``image/jpg``.

.. tip::

    Estendere la classe base ``Controller`` è facoltativo. Di fatto, un controllore
    può essere una semplice funzione PHP, o anche una funzione anonima PHP.
    Il capitolo ":doc:`The Controller</book/controller>`" del libro dice tutto
    sui controllori di Symfony2.

Il nome del template, ``AcmeDemoBundle:Welcome:index.html.twig``, è il *nome logico*
del template e fa riferimento al file
``Resources/views/Welcome/index.html.twig``
dentro ``AcmeDemoBundle`` (localizzato in ``src/Acme/DemoBundle``). La sezione successiva
sui bundle ne spiega l'utilità.

Diamo ora un altro sguardo al file di configurazione delle rotte e cerchiamo la voce ``_demo``:

.. code-block:: yaml

    # app/config/routing_dev.yml
    _demo:
        resource: "@AcmeDemoBundle/Controller/DemoController.php"
        type:     annotation
        prefix:   /demo

Symfony2 può leggere e importare informazioni sulle rotte da diversi file, scritti
in YAML, XML, PHP o anche inseriti in annotazioni PHP. Qui, il *nome logico*
del file è ``@AcmeDemoBundle/Controller/DemoController.php`` e si riferisce al file
``src/Acme/DemoBundle/Controller/DemoController.php``.
In questo file, le rotte sono
definite come annotazioni sui metodi delle azioni::

    // src/Acme/DemoBundle/Controller/DemoController.php
    use Sensio\Bundle\FrameworkExtraBundle\Configuration\Route;
    use Sensio\Bundle\FrameworkExtraBundle\Configuration\Template;

    class DemoController extends Controller
    {
        /**
         * @Route("/hello/{name}", name="_demo_hello")
         * @Template()
         */
        public function helloAction($name)
        {
            return array('name' => $name);
        }

        // ...
    }

L'annotazione ``@Route()`` definisce una nuova rotta con uno schema
``/hello/{name}``, che esegue il metodo ``helloAction`` quando trovato.
Una stringa racchiusa tra parentesi graffe, come ``{name}``, è chiamata segnaposto.
Come si può vedere, il suo valore può essere recuperato tramite il parametro ``$name`` del metodo.

.. note::

    Anche se le annotazioni sono sono supportate nativamente da PHP, possono
    essere usate in Symfony2 come mezzo conveniente per configurare i comportamenti
    del framework e mantenere la configurazione accanto al codice.

Dando un'occhiata più attenta al codice del controllore, si può vedere che invece di
rendere un template e restituire un oggetto ``Response`` come prima, esso restituisce
solo un array di parametri. L'annotazione ``@Template()`` dice a Symfony di rendere
il template al posto nostro, passando ogni variabili dell'array al template. Il nome
del template resto segue il nome del controllore. Quindi, nel nostro esempio, viene
reso il template ``AcmeDemoBundle:Demo:hello.html.twig`` (localizzato in
``src/Acme/DemoBundle/Resources/views/Demo/hello.html.twig``).

.. tip::

    Le annotazioni ``@Route()`` e ``@Template()`` sono più potenti dei semplici
    esempi mostrati in questa guida. Si può approfondire l'argomento "`annotazioni
    nei controllori`_" nella documentazione ufficiale.

Template
~~~~~~~~

Il controllore rende il template ``src/Acme/DemoBundle/Resources/views/Demo/hello.html.twig`` (oppure ``AcmeDemoBundle:Demo:hello.html.twig`` se si usa il nome logico):

.. code-block:: jinja

    {# src/Acme/DemoBundle/Resources/views/Demo/hello.html.twig #}
    {% extends "AcmeDemoBundle::layout.html.twig" %}

    {% block title "Hello " ~ name %}

    {% block content %}
        <h1>Hello {{ name }}!</h1>
    {% endblock %}

Per impostazione predefinita, Symfony2 usa `Twig`_ come sistema di template, ma si
possono anche usare i tradizionali template PHP, se si preferisce. Il prossimo
capitolo introdurrà il modo in cui funzionano i template in in Symfony2.

Bundle
~~~~~~

Forse vi siete chiesti perché il termine :term:`bundle` viene usato così tante volte
finora. Tutto il codice che si scrive per la propria applicazione è organizzato in
bundle.
Nel lingaggio di Symfony2, un bundle è un insieme strutturato di file (file
PHP, fogli di stile, JavaScript, immagini, ...) che implementano una singola
caratteristica (un blog, un forum, ...) e che può essere condivisa facilmente con
altri sviluppatori.
Finora abbiamo manipolato un solo bundle, ``AcmeDemoBundle``.
Impareremo di più sui bundle nell'ultimo capitolo di questa guida.

.. _quick-tour-big-picture-environments:

Lavorare con gli ambienti
-------------------------

Ora che si possiede una migliore comprensione di come funziona Symfony2, è
ora di dare un'occhiata più da vicino al fondo della pagina: si noterà
una piccola barra con i logo di Symfony2 e di PHP. Questa barra è chiamata
"Web Debug Toolbar" ed è il miglior amico dello sviluppatore.

.. image:: /images/quick_tour/web_debug_toolbar.png
   :align: center

Ma quello che si vede all'inizio è solo la punta dell'iceberg: cliccando
sullo strano numero esadecimale rivelerà un altro strumento di debug veramente
utile di Symfony2: il profilatore.

.. image:: /images/quick_tour/profiler.png
   :align: center

Ovviamente questo strumento non deve essere mostrato quando si rilascia l'applicazione
su un server di produzione. Per questo motivo si troverà un altro front controller (``app.php``) nella cartella ``web/``,
ottimizzato per l'ambiente di produzione:

.. code-block:: text

    http://localhost/Symfony/web/app.php/demo/hello/Fabien

Se si usa Apache con ``mod_rewrite`` abilitato, si può anche omettere la
parte ``app.php`` dell'URL:

.. code-block:: text

    http://localhost/Symfony/web/demo/hello/Fabien

Infine, ma non meno importante, sui server di produzione si dovrebbe far
puntare la cartella radice del web alla cartella ``web/``,per rendere
l'installazione sicura e avere URL più allettanti:

.. code-block:: text

    http://localhost/demo/hello/Fabien

Per rendere l'ambiente di produzione più veloce possibile, Symfony2
mantiene una cache sotto la cartella ``app/cache/``. Quando si fanno
delle modifiche al codice o alla configurazione, occorre rimuovere
a mano i file in cache. Per questo si dovrebbe sempre usare il front
controller di sviluppo (``app_dev.php``) mentre si lavora al progetto.

Diversi :term:`ambienti<ambiente>` di una stessa applicazione differiscono
solo nella loro configurazione.
In effetti, una configurazione può ereditare
da un'altra:

.. code-block:: yaml

    # app/config/config_dev.yml
    imports:
        - { resource: config.yml }

    web_profiler:
        toolbar: true
        intercept_redirects: false

L'ambiente ``dev`` (che carica il file di configurazione ``config_dev.yml``)
importa il file globale ``config.yml`` e lo modifica, in questo caso,
abilitando la Web Debug Toolbar.

Considerazioni finali
---------------------

Congratulazioni! Avete avuto il vostro primo assaggio di codice di Symfony2.
Non era così difficile, vero? C'è ancora molto da esplorare, ma dovreste
già vedere come Symfony2 rende veramente facile implementare siti web in modo
migliore e più veloce. Se siete ansiosi di saperne di più, andate alla prossima
sezione: ":doc:`La vista<the_view>`".

.. _Symfony2 Standard Edition:      http://symfony.com/download
.. _Symfony in 5 minuti:            http://symfony.com/symfony-in-five-minutes
.. _Separazione degli ambiti:       http://en.wikipedia.org/wiki/Separation_of_concerns
.. _YAML:                           http://www.yaml.org/
.. _annotazioni nei controllori:    http://bundles.symfony-reloaded.org/frameworkextrabundle/
.. _Twig:                           http://twig.sensiolabs.org/
