Un quadro generale
==================

Volete provare Symfony2 avendo a disposizione solo dieci minuti? Questa prima
parte di questa guida è stata scritta appositamente: spiega come
partire subito con Symfony2, mostrando la struttura di un semplice progetto già pronto.

Chi ha già usato un framework per il web si troverà come a casa con Symfony2. Altrimenti,
benvenuti in un nuovo mondo per sviluppare applicazioni web!

.. tip::

    Si vuole imparare perché e quando si ha bisogno di un framework? Si legga il
    documento "`Symfony in 5 minuti`_".

Scaricare Symfony2
------------------

Prima di tutto, verificare di aver installato e configurato correttamente un server web (come
Apache) con PHP 5.3.3 o successivi.

.. tip::

    Se si ha PHP 5.4, si può usare il server web incluso. Basta
    seguire le apposite
    :ref:`istruzioni<quick-tour-big-picture-built-in-server>`.

Pronti? Iniziamo scaricando "`Symfony2 Standard Edition`_", una :term:`distribuzione`
di Symfony preconfigurata per gli usi più comuni e che contiene anche del codice
che dimostra come usare Symfony2 (con l'archivio che include i *venditori*, si
parte ancora più velocemente).

Scaricare l'archivio e scompattarlo nella cartella radice del web (se si
usa il server web di PHP, lo si può scompattare ovunque). Si dovrebbe
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

    Se si ha familiarità con Composer, si può scaricare Composer e quindi eseguire
    il comando seguente, invece di scaricare l'archivio (sostituire
    ``2.2.0`` la versione più recente di Symfony, per esempio ``2.2.1``:

    .. code-block:: bash

        $ php composer.phar create-project symfony/framework-standard-edition Symfony 2.2.0

.. _`quick-tour-big-picture-built-in-server`:
   
Se si ha PHP 5.4, si può usare il server web incluso:

.. code-block:: bash

    # verifica la configurazione di PHP CLI
    $ php app/check.php

    # esegue il server web incluso
    $ php app/console server:run

Quindi l'URL dell'applicazione sarà "http://localhost:8000/app_dev.php"

Il server web incluso andrebbe usato solo durante lo sviluppo, ma può
aiutare a iniziare unprogetto in modo rapido e facile.

Se si usa un server web classico, come Apache, l'URL dipende dalla propria
configurazione. Se Symfony è stato scompattato sotto la cartella radice, in
una cartella ``Symfony``, l'URL dell'applicazione sarà:
"http://localhost/Symfony/web/app_dev.php".

.. note::

    Maggiori informazioni nella sezione :doc:`/cookbook/configuration/web_server_configuration`.


Verifica della configurazione
-----------------------------

Per evitare mal di testa successivamente, Symfony2 dispone di uno strumento per testare
la configurazione, per verificare configurazioni errate di PHP o del server web. Usare
il seguente URL per avviare la diagnosi sulla propria macchina:

.. code-block:: text

    http://localhost/config.php

.. note::

    Tutti gli URL degli esempi ipotizzano una configurazione del server web che punti
    direttamente alla cartella ``web/`` del nuovo progetto, che è diverso
    e più avanzato rispetto al processo mostrato sopra. Quindi, l'URL sulla propria
    macchina varierà, per esempio ``http://localhost:8000/config.php``
    o ``http://localhost/Symfony/web/config.php``. Si veda
    :ref:`above section<quick-tour-big-picture-built-in-server>` per dettagli
    su come dovrebbe essere l'URL e usarlo in tutti gli esempi.

Se ci sono dei problemi, correggerli. Si potrebbe anche voler modificare la propria
configurazione, seguendo le raccomandazioni fornite. Quando è tutto a posto,
cliccare su "*Bypass configuration and go to the Welcome page*" per richiedere
la prima "vera" pagina di Symfony2:

.. code-block:: text

    http://localhost/app_dev.php/

Symfony2 dovrebbe congratularsi per il duro lavoro svolto finora!

.. image:: /images/quick_tour/welcome.png
   :align: center

Capire i fondamenti
-------------------

Uno degli obiettivi principali di un framework è quello di assicurare la `Separazione degli ambiti`_.
Ciò mantiene il proprio codice organizzato e consente alla propria applicazione di
evolvere facilmente nel tempo, evitando il miscuglio di chiamate alla base dati, tag HTML
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

    http://localhost/app_dev.php/demo/hello/Fabien

.. image:: /images/quick_tour/hello_fabien.png
   :align: center

Cosa sta accadendo? Dissezioniamo l'URL:

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
        path:  /
        defaults: { _controller: AcmeDemoBundle:Welcome:index }

    _demo:
        resource: "@AcmeDemoBundle/Controller/DemoController.php"
        type:     annotation
        prefix:   /demo

    # ...

Le prime tre righe (dopo il commento) definiscono quale codice
richiamare quanto l'utente richiede la risorsa "``/``" (come la pagina di benvenuto
vista prima). Quando richiesto, il controllore ``AcmeDemoBundle:Welcome:index`` sarà
eseguito. Nella prossima sezione, si imparerà esattamente quello che significa.

.. tip::

    La Standard Edition usa `YAML`_  per i suoi file di configurazione,
    ma Symfony2 supporta nativamente anche XML, PHP e le annotazioni.
    I diversi formati sono compatibili e possono essere usati alternativamente
    in un'applicazione. Inoltre, le prestazioni dell'applicazione non dipendono
    dal formato scelto, perché tutto viene messo in cache alla prima
    richiesta.

Controllori
~~~~~~~~~~~

Il controllore è una funzione o un metodo PHP che gestisce le *richieste* in entrata
e restituisce delle *risposte* (spesso codice HTML). Invece di usare variabili e
funzioni globali di PHP (come ``$_GET`` o ``header()``) per gestire questi messaggi
HTTP, Symfony usa degli oggetti: :class:`Symfony\\Component\\HttpFoundation\\Request`
e :class:`Symfony\\Component\\HttpFoundation\\Response`.  Il controllore più semplice
possibile potrebbe creare la risposta a mano, basandosi sulla richiesta::

    use Symfony\Component\HttpFoundation\Response;

    $name = $request->query->get('name');

    return new Response('Hello '.$name, 200, array('Content-Type' => 'text/plain'));

.. note::

    Symfony2 abbraccia le specifiche HTTP, che sono delle regole che governano
    tutte le comunicazioni sul web. Si legga il capitolo ":doc:`/book/http_fundamentals`"
    del libro per sapere di più sull'argomento e sulle sue
    potenzialità.

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
:ref:`render()<controller-rendering-templates>`, che carica e rende
un template (``AcmeDemoBundle:Welcome:index.html.twig``). Il valore restituito
è un oggetto risposta, popolato con il contenuto resto. Quindi, se ci sono nuove
necessità, l'oggetto risposta può essere manipolato prima di essere inviato al browser::

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
    Il capitolo ":doc:`Il controllore</book/controller>`" del libro dice tutto
    sui controllori di Symfony2.

Il nome del template, ``AcmeDemoBundle:Welcome:index.html.twig``, è il
*nome logico* del template e fa riferimento al file ``Resources/views/Welcome/index.html.twig``
dentro ``AcmeDemoBundle`` (localizzato in ``src/Acme/DemoBundle``). La sezione successiva
sui bundle ne spiega l'utilità.

Diamo ora un altro sguardo al file di configurazione delle rotte e cerchiamo la voce
``_demo``:

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
In questo file, le rotte sono definite come annotazioni sui metodi delle azioni::

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

Il controllore rende il template ``src/Acme/DemoBundle/Resources/views/Demo/hello.html.twig``
(oppure ``AcmeDemoBundle:Demo:hello.html.twig``, se si usa il nome logico):

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
bundle. Nel linguaggio di Symfony2, un bundle è un insieme strutturato di file (file
PHP, fogli di stile, JavaScript, immagini, ...) che implementano una singola
caratteristica (un blog, un forum, ...) e che può essere condivisa facilmente con
altri sviluppatori. Finora abbiamo manipolato un solo bundle, ``AcmeDemoBundle``.
Impareremo di più sui bundle nell'ultimo capitolo di questa guida.

.. _quick-tour-big-picture-environments:

Lavorare con gli ambienti
-------------------------

Ora che si possiede una migliore comprensione di come funziona Symfony2, è
ora di dare un'occhiata più da vicino al fondo della pagina: si noterà
una piccola barra con il logo di Symfony2. Questa barra è chiamata
"barra di debug del web" ed è il miglior amico dello sviluppatore.

.. image:: /images/quick_tour/web_debug_toolbar.png
   :align: center

Ma quello che si vede all'inizio è solo la punta dell'iceberg: cliccando
sullo strano numero esadecimale, rivelerà un altro strumento di debug veramente
utile di Symfony2: il profilatore.

.. image:: /images/quick_tour/profiler.png
   :align: center

.. note::

    Si possono ottenere rapidamente maggiori informazioni posizionando il cursore
    sopra gli elementi della barra di debug del web o cliccandovi sopra, per andare
    alle rispettive pagine del profilatore.

Se caricato e abilitato (lo è in :ref:`environment<quick-tour-big-picture-environments-intro>` ``dev``),
il profilatore fornisce un'interfaccia web a un *immenso* ammontare di informazioni registrate
a ogni richiesta, inclusi log, tempi di richiesta, parametri GET o POST,
dettagli di sicurezza, query alla base dati e così via.

Ovviamente, questo strumento non deve essere mostrato quando si rilascia l'applicazione
su un server di produzione. Per questo motivo, si troverà un altro front controller (``app.php``)
nella cartella ``web/``, ottimizzato per l'ambiente di produzione:

.. _quick-tour-big-picture-environments-intro:

Ma che cos'è un ambiente? Un :term:`Ambiente` è una stringa (come
``dev`` o ``prod``) che rappresenta un gruppo di configurazioni usate
per far girare l'applicazione.

Di solito, la configurazione comune è posta in ``config.yml`` e sovrascritta
ove necessario, per ciascun ambiente. Per esempio:

.. code-block:: yaml

    # app/config/config_dev.yml
    imports:
        - { resource: config.yml }

    web_profiler:
        toolbar: true
        intercept_redirects: false

In questo esempio, l'ambiente ``dev`` carica il file di configurazione ``config_dev.yml``,
che importa il file globale ``config.yml`` e quindi lo modifica,
abilitando la barra di debug del web.

Richiamando il file ``app_dev.php`` nel browser, si esegue
l'applicazione Symfony in ambiente ``dev``. Per vedere l'applicazione
in ambiente ``prod``, richiamare invece ``app.php``. Le rotte di demo
nell'applicazione sono disponibili solo in ambiente ``dev``, ma se
lo fossero anche in ambiente ``prod``, si potrebbe vederle
in ambiente ``prod`` andando su:

.. code-block:: text

    http://localhost/app.php/demo/hello/Fabien

Se, invece di usare il server web di PHP, si usa Apache con ``mod_rewrite``
abilitato, sfruttando il file ``.htaccess`` fornito da Symfony2
in ``web/``, si può anche omettere la parte ``app.php`` dell'URL. Il file
``.htaccess`` punta tutte le richieste al front controller ``app.php``:

.. code-block:: text

    http://localhost/demo/hello/Fabien

.. note::

    Si noti che i tre URL qui forniti sono solo **esempi** di come un URL potrebbe
    apparire in produzione usando un front controller. Se li si
    prova effettivamente in un'installazione base della *Standard Edition di Symfony*,
    si otterrà un errore 404, perché *AcmeDemoBundle* è abilitato solo in
    ambiente ``dev`` e le sue rotte importate in ``app/config/routing_dev.yml``.

Per maggiori dettagli sugli ambienti, vedere ":ref:`Ambienti e front controller<page-creation-environments>`".

Considerazioni finali
---------------------

Congratulazioni! Avete avuto il vostro primo assaggio di codice di Symfony2.
Non era così difficile, vero? C'è ancora molto da esplorare, ma dovreste
già vedere come Symfony2 rende veramente facile implementare siti web in modo
migliore e più veloce. Se siete ansiosi di saperne di più, andate alla prossima
sezione: ":doc:`la vista<the_view>`".

.. _Symfony2 Standard Edition:      http://symfony.com/download
.. _Symfony in 5 minuti:            http://symfony.com/symfony-in-five-minutes
.. _Separazione degli ambiti:       http://en.wikipedia.org/wiki/Separation_of_concerns
.. _YAML:                           http://www.yaml.org/
.. _annotazioni nei controllori:    http://symfony.com/it/doc/current/bundles/SensioFrameworkExtraBundle/index.html#annotazioni-per-i-controllori
.. _Twig:                           http://twig.sensiolabs.org/
.. _`pagina di installazione di Symfony`: http://symfony.com/download
