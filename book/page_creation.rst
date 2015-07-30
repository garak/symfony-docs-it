.. index::
   single: Creazione di pagine

.. _creating-pages-in-symfony2:

Creare pagine in Symfony
========================

La creazione di una nuova pagina in Symfony è un semplice processo in due passi:

* *Creare una rotta*: Una rotta definisce l'URL (p.e. ``/about``) verso la pagina
  e specifica un controllore (che è una funzione PHP) che Symfony dovrebbe
  eseguire quando l'URL della richiesta in arrivo corrisponde allo schema della rotta;

* *Creare un controllore*: Un controllore è una funzione PHP che prende la richiesta in
  entrata e la trasforma in un oggetto ``Response`` di Symfony, che viene poi
  restituito all'utente.

Questo semplice approccio è molto bello, perché corrisponde al modo in cui funziona il web.
Ogni interazione sul web inizia con una richiesta HTTP. Il lavoro di
un'applicazione è semplicemente quello di interpretare la richiesta e restituire l'appropriata
risposta HTTP.

Symfony segue questa filosofia e fornisce strumenti e convenzioni per mantenere
un'applicazione organizzata, man mano che cresce in utenti e in complessità.

.. index::
   single: Creazione di pagine; Ambienti e front controller

.. _page-creation-environments:

Ambienti e front controller
~~~~~~~~~~~~~~~~~~~~~~~~~~~

Ogni applicazione Symfony gira in un :term:`ambiente`. Un ambiente
è un insieme specifico di configurazioni e bundle caricati, rappresentato da una stringa.
La stessa applicazione può girare con diverse configurazioni, se eseguita
in diversi ambienti. Symfony dispone di tre ambienti
predefiniti: ``dev``, ``test`` e ``prod``. È comunque possibile crearne di altri.

Gli ambienti sono utili, perché consentono a una singola applicazione di avere un ambiente (dev)
pensato per il debug e un altro (prod) ottimizziato per la velocità. Si possono
anche caricare bundle specifici, in base all'ambiente. Per esempio,
Symfony dispone di un WebProfilerBundle (descritto più avanti), abilitato
solamente in ``dev`` e in ``test``.

Symfony dispone di due front controller pubblici: ``app_dev.php`` 
fornisce l'ambiente ``dev``, mentre ``app.php`` fornisce l'ambiente ``prod``.
Ogni accesso via web a Symfony normalmente passa per uno di questi due front controller.
(L'ambiente ``test`` normalmente si usa solo quando si eseguono i test e quindi
non dispone di un front controller dedicato. La linea di comando fornisce ugualmente
un front controller utilizzabile con qualsiasi ambiente.)

Quando il front controller inizializza il kernel, fornisce due parametri:
l'ambiente e la modalità di debug con cui il kernel deve girare.
Per far rispondere velocemente l'applicazione, Symfony mantiene una cache sotto la
cartella ``app/cache/``. Quando il debug è abilitato (come in ``app_dev.php``),
la cache viene rinfrescata automaticamente a ogni modifica del
codice o della configurazione. In debug, Symfony va più
lentamente, ma le modifiche sono rispettate senza dover pulire a mano la
cache.

.. index::
   single: Creazione di pagine; Esempio

La pagina "numero casuale"
--------------------------

In questo capitolo, svilupperemo un'applicazione per generare numeri casuali.
Quando avremo finito, l'utente sarà in grado di ottenere un numero casuale tra ``1``
e il limite superiore, impostato da URL:

.. code-block:: text

    http://localhost/app_dev.php/random/100

In realtà, si potrà sostituire ``100`` con qualsiasi altro numero, che funga da limite
superiore per il numero da generare. Per creare la pagina, seguiamo il semplice processo
in due passi.

.. note::

    La guida presume che Symfony sia stato già scaricato e il server web
    configurato. L'URL precedente presume che ``localhost`` punti alla cartella
    ``web`` del nuovo progetto Symfony. Per informazioni dettagliate su
    questo processo, vedere la documentazione del server web usato.
    Ecco le pagine di documentazione per alcuni server web:

    * Per il server Apache, fare riferimento alla `documentazione su DirectoryIndex di Apache`_.
    * Per Nginx, fare riferimento alla `documentazione su HttpCoreModule di Nginx`_.

Prima di iniziare: creare il bundle
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Prima di iniziare, occorrerà creare un *bundle*. In Symfony, un :term:`bundle`
è come un plugin, tranne per il fatto che tutto il codice nella propria applicazione
starà dentro a un bundle.

Un bundle non è nulla di più di una cartella che ospita ogni cosa correlata a una
specifica caratteristica, incluse classi PHP, configurazioni e anche fogli di stile
e file JavaScript (si veda :ref:`page-creation-bundles`).

A seconda della modalità di installazione di Symfony, si potrebbe già a vere un bundle, chiamato
AcmeDemoBundle. Controllare nella cartella ``src/`` del progetto se c'è
una cartella ``DemoBundle/`` sotto la cartella ``Acme/``. Se tali
cartelle esistono, saltare il resto di questa sezione e andare direttamente alla
creazione della rotta.

Per creare un bundle chiamato ``AcmeHelloBundle`` (un bundle creato appositamente in
questo capitolo), eseguire il seguente comando e seguire le istruzioni su schermo
(usando tutte le opzioni predefinite):

.. code-block:: bash

    $ php app/console generate:bundle --namespace=Acme/DemoBundle --format=yml

Dietro le quinte, viene creata una cartella per il bundle in ``src/Acme/DemoBundle``.
Inoltre viene aggiunta automaticamente una riga al file ``app/AppKernel.php``, in modo
che il bundle sia registrato nel kernel::

    // app/AppKernel.php
    public function registerBundles()
    {
        $bundles = array(
            // ...
            new Acme\DemoBundle\AcmeDemoBundle(),
        );
        // ...

        return $bundles;
    }

Ora che si è impostato il bundle, si può iniziare a costruire la propria applicazione,
dentro il bundle stesso.

Passo 1: creare la rotta
~~~~~~~~~~~~~~~~~~~~~~~~

Per impostazione predefinita, il file di configurazione delle rotte in un'applicazione
Symfony si trova in ``app/config/routing.yml``. Come ogni configurazione in Symfony,
si può anche scegliere di usare XML o PHP per configurare le rotte.

Se si guarda il file principale delle rotte, si vedrà che Symfony ha già aggiunto
una voce, quando è stato generato AcmeDemoBundle:

.. configuration-block::

    .. code-block:: yaml

        # app/config/routing.yml
        acme_website:
            resource: "@AcmeDemoBundle/Resources/config/routing.yml"
            prefix:   /

    .. code-block:: xml

        <!-- app/config/routing.xml -->
        <?xml version="1.0" encoding="UTF-8" ?>
        <routes xmlns="http://symfony.com/schema/routing"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://symfony.com/schema/routing
                http://symfony.com/schema/routing/routing-1.0.xsd">

            <import
                resource="@AcmeDemoBundle/Resources/config/routing.xml"
                prefix="/" />
        </routes>

    .. code-block:: php

        // app/config/routing.php
        use Symfony\Component\Routing\RouteCollection;

        $acmeDemo = $loader->import('@AcmeDemoBundle/Resources/config/routing.php');
        $acmeDemo->addPrefix('/');

        $collection = new RouteCollection();
        $collection->addCollection($acmeDemo);

        return $collection;

Questa voce è molto basica: dice a Symfony di caricare la configurazione delle rotte
dal file ``Resources/config/routing.yml`` (``routing.xml`` o ``routing.php``
rispettivamente negli esempi di codice XML e PHP), che si trova dentro
AcmeDemoBundle. Questo vuol dire che si mette la configurazione delle rotte direttamente in
``app/config/routing.yml`` o si organizzano le proprie rotte attraverso la propria applicazione
e le si importano da qui.

.. note::

    Non si è limitati a caricare configurazioni di rotte che condividono lo stesso
    formato. Per esempio, si potrebbe anche caricare un file YAML in una configurazione XML
    e viceversa.

Ora che il file ``routing.yml`` del bundle è stato importato, aggiungere la nuova rotta,
che definisce l'URL della pagina che stiamo per creare:

.. configuration-block::

    .. code-block:: yaml

        # src/Acme/DemoBundle/Resources/config/routing.yml
        random:
            path:     /random/{limit}
            defaults: { _controller: AcmeDemoBundle:Random:index }

    .. code-block:: xml

        <!-- src/Acme/DemoBundle/Resources/config/routing.xml -->
        <?xml version="1.0" encoding="UTF-8" ?>
        <routes xmlns="http://symfony.com/schema/routing"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://symfony.com/schema/routing
                http://symfony.com/schema/routing/routing-1.0.xsd">

            <route id="random" path="/random/{limit}">
                <default key="_controller">AcmeDemoBundle:Random:index</default>
            </route>
        </routes>

    .. code-block:: php

        // src/Acme/DemoBundle/Resources/config/routing.php
        use Symfony\Component\Routing\RouteCollection;
        use Symfony\Component\Routing\Route;

        $collection = new RouteCollection();
        $collection->add('random', new Route('/random/{limit}', array(
            '_controller' => 'AcmeDemoBundle:Random:index',
        )));

        return $collection;

Il routing consiste di due pezzi di base: il percorso (``path``), che è l'URL
a cui la rotta corrisponderà, e un array ``defaults``, che specifica il controllore
che sarà eseguito. La sintassi dei segnaposto nello schema (``{limit}``) è un jolly.
Vuol dire che  ``/random/10``, ``/random/327`` o ogni altro URL simile
corrisponderanno a questa rotta. Il parametro del segnaposto ``{limit}`` sarà anche
passato al controllore, in modo da poter usare il suo valore per salutare personalmente
l'utente.

.. note::

  Il sistema delle rotte ha molte altre importanti caratteristiche per creare strutture
  di URL flessibili e potenti nella propria applicazioni. Per maggiori dettagli, si veda
  il capitolo dedicato alle :doc:`Rotte </book/routing>`.

Passo 2: creare il controllore
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Quando un URL come ``/hello/Ryan`` viene gestita dall'applicazione, la rotta ``hello``
viene corrisposta e il controllore ``AcmeDemoBundle:Hello:index`` eseguito dal
framework. Il secondo passo del processo di creazione della pagina è quello di creare
tale controllore.

Il controllore ha il nome *logico* ``AcmeDemoBundle:Random:index`` ed è mappato
sul metodo ``indexAction`` di una classe PHP chiamata
``Acme\DemoBundle\Controller\RandomController``. Iniziamo creando questo file dentro
il nostro AcmeDemoBundle::

    // src/Acme/DemoBundle/Controller/RandomController.php
    namespace Acme\DemoBundle\Controller;

    class RandomController
    {
    }

In realtà, il controllore non è nulla di più di un metodo PHP, che va creato e che
Symfony eseguirà. È qui che il codice usa l'informazione dalla richiesta per
costruire e preparare la risorsa che è stata richiesta. Tranne per alcuni casi avanzati,
il prodotto finale di un controllore è sempre lo stesso: un oggetto ``Response`` di
Symfony.

Creare il metodo ``indexAction``, che Symfony eseguirà quando la rotta ``hello`` sarà
corrisposta::

    // src/Acme/DemoBundle/Controller/RandomController.php
    namespace Acme\DemoBundle\Controller;

    use Symfony\Component\HttpFoundation\Response;

    class RandomController
    {
        public function indexAction($limit)
        {
            return new Response(
                '<html><body>Numero: '.rand(1, $limit).'</body></html>'
            );
        }
    }

Il controllore è semplice: esso crea un nuovo oggetto ``Response``, il cui primo
parametro è il contenuto che sarà usato dalla risposta (in questo esempio, una
piccola pagina HTML).

Congratulazioni! Dopo aver creato solo una rotta e un controllore, abbiamo già una
pagina pienamente funzionante! Se si è impostato tutto correttamente, la propria
applicazione dovrebbe salutare:

.. code-block:: text

    http://localhost/app_dev.php/random/10

.. _book-page-creation-prod-cache-clear:

.. tip::

    Si può anche vedere l'applicazione nell':ref:`ambiente <environments-summary>`
    "prod", visitando:

    .. code-block:: text

        http://localhost/app.php/random/10

    Se si ottiene un errore, è probabilmente perché occorre pulire la cache,
    eseguendo:

    .. code-block:: bash

        $ php app/console cache:clear --env=prod --no-debug

Un terzo passo, facoltativo ma comune, del processo è quello di creare un template.

.. note::

   I controllori sono il punto principale di ingresso del codice e un ingrediente
   chiave della creazione di pagine. Si possono trovare molte più informazioni nel
   :doc:`capitolo sul controllore </book/controller>`.

Passo 3 (facoltativo): creare il template
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

I template consentono di spostare tutta la presentazione (p.e. il codice HTML) in un file
separato e riusare diverse porzioni del layout della pagina. Invece di scrivere il codice
HTML dentro al controllore, meglio rendere un template:

.. code-block:: php
    :linenos:

    // src/Acme/DemoBundle/Controller/RandomController.php
    namespace Acme\DemoBundle\Controller;

    use Symfony\Bundle\FrameworkBundle\Controller\Controller;

    class RandomController extends Controller
    {
        public function indexAction($limit)
        {
            $number = rand(1, $limit);

            return $this->render(
                'AcmeDemoBundle:Random:index.html.twig',
                array('number' => $number)
            );

            // rende invece un template PHP
            // return $this->render(
            //     'AcmeDemoBundle:Random:index.html.php',
            //     array('number' => $number)
            // );
        }
    }

.. note::

   Per poter usare il  metodo :method:`Symfony\\Bundle\\FrameworkBundle\\Controller\\Controller::render`,
   il controllore deve estendere la classe
   :class:`Symfony\\Bundle\\FrameworkBundle\\Controller\\Controller`,
   che aggiunge scorciatoie per compiti comuni nei controllori. Ciò viene fatto
   nell'esempio precedente aggiungendo l'istruzione ``use`` alla riga 4 ed
   estendendo ``Controller`` alla riga 6.

Il metodo ``render()`` crea un oggetto ``Response`` riempito con il contenuto del
template dato. Come ogni altro controllore, alla fine l'oggetto ``Response``
viene restituito. 

Si noti che ci sono due diversi esempi su come rendere il template. Per impostazione
predefinita, Symfony supporta due diversi linguaggi di template: i classici
template PHP e i template, concisi ma potenti, `Twig`_. Non ci si allarmi,
si è liberi di scegliere tra i due, o anche tutti e due nello stesso progetto.

Il controllore rende il template ``AcmeDemoBundle:Hello:index.html.twig``,
che usa la seguente convenzioni dei nomi:

    **NomeBundle**:**NomeControllore**:**NomeTemplate**

Questo è il nome *logico* del template, che è mappato su una locazione fisica,
usando la seguente convenzione:

    **/percorso/di/NomeBundle**/Resources/views/**NomeControllore**/**NomeTemplate**

In questo caso, ``AcmeHelloBundle`` è il nome del bundle, ``Hello`` è il
controllore e ``index.html.twig`` il template:

.. configuration-block::

    .. code-block:: jinja
       :linenos:

        {# src/Acme/DemoBundle/Resources/views/Random/index.html.twig #}
        {% extends '::base.html.twig' %}

        {% block body %}
            Numero: {{ number }}!
        {% endblock %}

    .. code-block:: html+php

        <!-- src/Acme/DemoBundle/Resources/views/Random/index.html.php -->
        <?php $view->extend('::base.html.php') ?>

        Numero: <?php echo $view->escape($number) ?>

Analizziamo il template Twig riga per riga:

* *riga 2*: Il token ``extends`` definisce un template padre. Il template definisce
  esplicitamente un file di layout, dentro il quale sarà inserito.

* *riga 4*: Il token ``block`` dice che ogni cosa al suo interno va posta dentro
  un blocco chiamato ``body``. Come vedremo, è responsabilità del template padre
  (``base.html.twig``) rendere alla fine il blocco chiamato
  ``body``.

Il template padre, ``::base.html.twig``, manca delle porzioni **NomeBundle**
e **NomeControllore** del suo nome (per questo ha il doppio duepunti (``::``)
all'inizio). Questo vuol dire che il template risiede fuori dai bundle, nella
cartella ``app``:

.. configuration-block::

    .. code-block:: html+jinja

        {# app/Resources/views/base.html.twig #}
        <!DOCTYPE html>
        <html>
            <head>
                <meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
                <title>{% block title %}Benvenuto!{% endblock %}</title>
                {% block stylesheets %}{% endblock %}
                <link rel="shortcut icon" href="{{ asset('favicon.ico') }}" />
            </head>
            <body>
                {% block body %}{% endblock %}
                {% block javascripts %}{% endblock %}
            </body>
        </html>

    .. code-block:: html+php

        <!-- app/Resources/views/base.html.php -->
        <!DOCTYPE html>
        <html>
            <head>
                <meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
                <title><?php $view['slots']->output('title', 'Benvenuto!') ?></title>
                <?php $view['slots']->output('stylesheets') ?>
                <link rel="shortcut icon"
                    href="<?php echo $view['assets']->getUrl('favicon.ico') ?>" />
            </head>
            <body>
                <?php $view['slots']->output('_content') ?>
                <?php $view['slots']->output('javascripts') ?>
            </body>
        </html>

Il template di base definisce il layout HTML e rende il blocco ``body``, che era
stato definito nel template ``index.html.twig``. Rende anche un blocco ``title``,
che si può scegliere di definire nel template nel template ``index.html.twig``.
Poiché non è stato definito il blocco ``title`` nel template figlio, il suo valore
predefinito è "Benvenuto!".

I template sono un modo potente per rendere e organizzare il contenuto della propria
pagina. Un template può rendere qualsiasi cosa, dal codice HTML al CSS, o ogni
altra cosa che il controllore abbia bisogno di restituire.

Nel ciclo di vita della gestione di una richiesta, il motore dei template è solo
uno strumento opzionale. Si ricordi che lo scopo di ogni controllore è quello di
restituire un oggetto ``Response``. I template sono uno strumento potente, ma
facoltativo, per creare il contenuto per un oggetto ``Response``.

.. index::
   single: Struttura delle cartelle

Struttura delle cartelle
------------------------

Dopo solo poche sezioni, si inizia già a capire la filosofia che sta dietro alla
creazione e alla resa delle pagine in Symfony. Abbiamo anche già iniziato a vedere
come i progetti Symfony siano strutturati e organizzati. Alla fine di questa sezione,
sapremo dove cercare e inserire i vari tipi di file, e perché.

Sebbene interamente flessibili, per impostazione predefinita, ogni :term:`applicazione`
Symfony ha la stessa struttura di cartelle raccomandata:

``app/``
    Questa cartella contiene la configurazione dell'applicazione;

``src/``
    Tutto il codice PHP del progetto sta all'interno di questa cartella;

``vendor/``
    Ogni libreria dei venditori è inserita qui, per convenzione;

``web/``
    Questa è la cartella radice del web e contiene ogni file accessibile pubblicamente;

.. seealso::

    Si può facilmente ridefinire la struttura predefinita delle cartelle. Vedere
    :doc:`/cookbook/configuration/override_dir_structure` per maggiori
    informazioni.

.. _the-web-directory:

La cartella web
~~~~~~~~~~~~~~~

La cartella radice del web è la casa di tutti i file pubblici e statici, inclusi
immagini, fogli di stile, file JavaScript. È anche il posto in cui stanno tutti
i :term:`front controller`::

    // web/app.php
    require_once __DIR__.'/../app/bootstrap.php.cache';
    require_once __DIR__.'/../app/AppKernel.php';

    use Symfony\Component\HttpFoundation\Request;

    $kernel = new AppKernel('prod', false);
    $kernel->loadClassCache();
    $kernel->handle(Request::createFromGlobals())->send();

Il file del front controller (``app.php`` in questo esempio) è il file PHP che viene
eseguito quando si usa un'applicazione Symfony e il suo compito è quello di usare una
classe kernel, ``AppKernel``, per inizializzare l'applicazione.

.. tip::

    Aver un front controller vuol dire avere URL diverse e più flessibili rispetto
    a una tipica applicazione in puro PHP. Quando si usa un front controller,
    gli URL sono formattati nel modo seguente:

    .. code-block:: text

        http://localhost/app.php/random/10

    Il front controller, ``app.php``, viene eseguito e l'URL "interno" 
    ``/random/10`` è dirottato internamente, usando la configurazione delle rotte.
    Usando ``mod_rewrite`` di Apache, si può forzare l'esecuzione del file ``app.php``
    senza bisogno di specificarlo nell'URL:

    .. code-block:: text

        http://localhost/random/10

Sebbene i front controller siano essenziali nella gestione di ogni richiesta, raramente
si avrà bisogno di modificarli o anche di pensarci. Saranno brevemente menzionati ancora
nella sezione `Ambienti`_.

La cartella dell'applicazione (``app``)
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Come visto nel front controller, la classe ``AppKernel`` è il punto di ingresso principale
dell'applicazione ed è responsabile di tutta la configurazione. Per questo è memorizzata
nella cartella ``app/``.

Questa classe deve implementare due metodi, che definiscono tutto ciò di cui Symfony
ha bisogno di sapere sulla propria applicazione. Non ci si deve preoccupare di questi
metodi all'inizio, Symfony li riempie al posto nostro con delle impostazioni
predefinite.

``registerBundles()``
    Restituisce un array di tutti bundle necessari per eseguire
    l'applicazione (vedere :ref:`page-creation-bundles`);

``registerContainerConfiguration()``
    Carica il file della configurazione principale
    dell'applicazione (vedere la sezione `Configurazione dell'applicazione`_).

Nello sviluppo quotidiano, per lo più si userà la cartella ``app/`` per modificare i
file di configurazione e delle rotte nella cartella ``app/config/`` (vedere
`Configurazione dell'applicazione`_). Essa contiene anche la cartella della cache
dell'applicazione (``app/cache``), la cartella dei log (``app/logs``) e la cartella
dei file risorsa a livello di applicazione, come i template (``app/Resources``).
Ognuna di queste cartella sarà approfondita nei capitoli successivi.

.. _autoloading-introduction-sidebar:

.. sidebar:: Autoload

    Quando Symfony si carica, un file speciale chiamato ``app/autoload.php`` viene incluso.
    Questo file è responsabile di configurare l'autoloader, che auto-caricherà i file
    dell'applicazione dalla cartella ``src/`` e le librerie di terze parti dalla
    cartella ``vendor/`` menzionate nel file ``composer.json``.

    Grazie all'autoloader, non si avrà mai bisogno di usare le istruzioni ``include``
    o ``require``. Al posto loro, Composer usa lo spazio dei nomi di una classe per
    determinare la sua posizione e includere automaticamente il file al posto
    nostro, nel momento in cui la classe è necessaria.

    L'autoloader è già configurato per cercare nella cartella ``src/``
    tutte le proprie classi PHP. Per poterlo far funzionare, il nome della classe
    e quello del file devono seguire lo stesso schema:

    .. code-block:: text

        Nome della classe:
            Acme\DemoBundle\Controller\RandomController
        Percorso:
            src/Acme/DemoBundle/Controller/RandomController.php

La cartella dei sorgenti (``src``)
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Detto semplicemente, la cartella ``src/`` contiene tutto il codice (codice PHP,
template, file di configurazione, fogli di stile, ecc.) che guida la *propria*
applicazione. Quando si sviluppa, gran parte del lavoro sarà svolto
dentro uno o più bundle creati in questa cartella.

Ma cos'è esattamente un :term:`bundle`?

.. _page-creation-bundles:

Il sistema dei bundle
---------------------

Un bundle è simile a un plugin in altri software, ma anche meglio. La differenza
fondamentale è che *tutto* è un bundle in Symfony, incluse le funzionalità
fondamentali del framework o il codice scritto per la propria applicazione.
I bundle sono cittadini di prima classe in Symfony. Questo fornisce la flessibilità
di usare caratteristiche già pronte impacchettate in `bundle di terze parti` o di
distribuire i propri bundle. Rende facile scegliere quali caratteristiche abilitare
nella propria applicazione per ottimizzarla nel modo preferito.

.. note::

   Pur trovando qui i fondamentali, un'intera ricetta è dedicata all'organizzazione e
   alle pratiche migliori in :doc:`bundle</cookbook/bundles/best_practices>`.

Un bundle è semplicemente un insieme strutturato di file dentro una cartella, che implementa
una singola caratteristica. Si potrebbe creare un ``BlogBundle``, un ``ForumBundle`` o un
bundle per la gestione degli utenti (molti di questi già esistono come bundle open source).
Ogni cartella contiene tutto ciò che è relativo a quella caratteristica, inclusi file
PHP, template, fogli di stile, JavaScript, test e tutto il resto.
Ogni aspetto di una caratteristica esiste in un bundle e ogni caratteristica risiede
in un bundle.

Un'applicazione è composta di bundle, come definito nel metodo ``registerBundles()``
della classe ``AppKernel``::

    // app/AppKernel.php
    public function registerBundles()
    {
        $bundles = array(
            new Symfony\Bundle\FrameworkBundle\FrameworkBundle(),
            new Symfony\Bundle\SecurityBundle\SecurityBundle(),
            new Symfony\Bundle\TwigBundle\TwigBundle(),
            new Symfony\Bundle\MonologBundle\MonologBundle(),
            new Symfony\Bundle\SwiftmailerBundle\SwiftmailerBundle(),
            new Symfony\Bundle\DoctrineBundle\DoctrineBundle(),
            new Symfony\Bundle\AsseticBundle\AsseticBundle(),
            new Sensio\Bundle\FrameworkExtraBundle\SensioFrameworkExtraBundle(),
        );

        if (in_array($this->getEnvironment(), array('dev', 'test'))) {
            $bundles[] = new Acme\DemoBundle\AcmeDemoBundle();
            $bundles[] = new Symfony\Bundle\WebProfilerBundle\WebProfilerBundle();
            $bundles[] = new Sensio\Bundle\DistributionBundle\SensioDistributionBundle();
            $bundles[] = new Sensio\Bundle\GeneratorBundle\SensioGeneratorBundle();
        }

        return $bundles;
    }

Col metodo ``registerBundles()``, si ha il controllo totale su quali bundle siano usati
dalla propria applicazione (inclusi i bundle del nucleo di Symfony).

.. tip::

   Un bundle può stare *ovunque*, purché possa essere auto-caricato (tramite
   l'autoloader configurato in ``app/autoload.php``).

Creare un bundle
~~~~~~~~~~~~~~~~

Symfony Standard Edition contiene un task utile per creare un bundle pienamente
funzionante. Ma anche creare un bundle a mano è molto facile.

Per dimostrare quanto è semplice il sistema dei bundle, creiamo un nuovo bundle,
chiamato ``AcmeTestBundle``, e abilitiamolo.

.. tip::

    La parte ``Acme`` è solo un nome fittizio, che andrebbe sostituito da un nome di
    "venditore" che rappresenti la propria organizzazione (p.e. ``ABCTestBundle``
    per un'azienda chiamata ``ABC``).

Iniziamo creando una cartella ``src/Acme/TestBundle/`` e aggiungendo un nuovo file
chiamato ``AcmeTestBundle.php``::

    // src/Acme/TestBundle/AcmeTestBundle.php
    namespace Acme\TestBundle;

    use Symfony\Component\HttpKernel\Bundle\Bundle;

    class AcmeTestBundle extends Bundle
    {
    }

.. tip::

   Il nome AcmeTestBundle segue le
   :ref:`convenzioni sui nomi dei bundle<bundles-naming-conventions>`. Si
   potrebbe anche scegliere di accorciare il nome del bundle semplicemente a TestBundle,
   chiamando la classe ``TestBundle`` (e chiamando il file ``TestBundle.php``).

Questa classe vuota è l'unico pezzo necessario a creare un nuovo bundle. Sebbene solitamente
vuota, questa classe è potente e può essere usata per personalizzare il comportamento
del bundle.

Ora che il bundle è stato creato, va abilitato tramite la classe ``AppKernel``::

    // app/AppKernel.php
    public function registerBundles()
    {
        $bundles = array(
            // ...
            // registra il bundle
            new Acme\TestBundle\AcmeTestBundle(),
        );
        // ...

        return $bundles;
    }

Sebbene non faccia ancora nulla, AcmeTestBundle è ora pronto per essere usato.

Symfony fornisce anche un'interfaccia a linea di comando per generare
uno scheletro di base per un bundle:

.. code-block:: bash

    $ php app/console generate:bundle --namespace=Acme/TestBundle

Lo scheletro del bundle è generato con controllore, template e rotte, tutti
personalizzabili. Approfondiremo più avanti la linea di comando di
Symfony.

.. tip::

   Ogni volta che si crea un nuovo bundle o che si usa un bundle di terze parti,
   assicurarsi sempre che il bundle sia abilitato in ``registerBundles()``. Se si usa
   il comando ``generate:bundle``, l'abilitazione è automatica.

Struttura delle cartelle dei bundle
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

La struttura delle cartelle di un bundle è semplice e flessibile. Per impostazione
predefinita, il sistema dei bundle segue un insieme di convenzioni, che aiutano a
mantenere il codice coerente tra tutti i bundle di Symfony. Si dia un'occhiata a
``AcmeHelloBundle``, perché contiene alcuni degli elementi più comuni di un bundle:

``Controller/``
    contiene i controllori del bundle (p.e. ``HelloController.php``);

``DependencyInjection/``
    contiene alcune estensioni di classi,
    che possono importare configurazioni di servizi, registrare passi di compilatore o altro
    (tale cartella non è indispensabile);

``Resources/config/``
    contiene la configurazione, compresa la configurazione delle rotte (p.e. ``routing.yml``);

``Resources/views/``
    contiene i template, organizzati per nome di controllore (p.e. ``Hello/index.html.twig``);

``Resources/public/``
    contiene le risorse per il web (immagini, fogli di stile, ecc.)
    ed è copiata o collegata simbolicamente alla cartella ``web/`` del progetto, tramite
    il comando ``assets:install``;

``Tests/``
 contiene tutti i test del bundle.

Un bundle può essere grande o piccolo, come la caratteristica che implementa. Contiene
solo i file che occorrono e niente altro.

Andando avanti nel libro, si imparerà come persistere gli oggetti in una base dati,
creare e validare form, creare traduzioni per la propria applicazione, scrivere
test e molto altro. Ognuno di questi ha il suo posto e il suo ruolo dentro il
bundle.

Configurazione dell'applicazione
--------------------------------

Un'applicazione è composta da un insieme di bundle, che rappresentano tutte le
caratteristiche e le capacità dell'applicazione stessa. Ogni bundle può essere
personalizzato tramite file di configurazione, scritti in YAML, XML o PHP. Per
impostazione predefinita, il file di configurazione principale risiede nella cartella
``app/config/`` è si chiama ``config.yml``, ``config.xml`` o ``config.php``, a seconda
del formato scelto:

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        imports:
            - { resource: parameters.yml }
            - { resource: security.yml }

        framework:
            secret:          "%secret%"
            router:          { resource: "%kernel.root_dir%/config/routing.yml" }
            # ...

        # Configurazione di Twig
        twig:
            debug:            "%kernel.debug%"
            strict_variables: "%kernel.debug%"

        # ...

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <?xml version="1.0" encoding="UTF-8" ?>
        <container xmlns="http://symfony.com/schema/dic/services"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xmlns:framework="http://symfony.com/schema/dic/symfony"
            xmlns:twig="http://symfony.com/schema/dic/twig"
            xsi:schemaLocation="http://symfony.com/schema/dic/services
                http://symfony.com/schema/dic/services/services-1.0.xsd
                http://symfony.com/schema/dic/symfony
                http://symfony.com/schema/dic/symfony/symfony-1.0.xsd
                http://symfony.com/schema/dic/twig
                http://symfony.com/schema/dic/twig/twig-1.0.xsd">

            <imports>
                <import resource="parameters.yml" />
                <import resource="security.yml" />
            </imports>

            <framework:config secret="%secret%">
                <framework:router resource="%kernel.root_dir%/config/routing.xml" />
                <!-- ... -->
            </framework:config>

            <!-- Configurazione di Twig -->
            <twig:config debug="%kernel.debug%" strict-variables="%kernel.debug%" />

            <!-- ... -->
        </container>

    .. code-block:: php

        // app/config/config.php
        $this->import('parameters.yml');
        $this->import('security.yml');

        $container->loadFromExtension('framework', array(
            'secret' => '%secret%',
            'router' => array(
                'resource' => '%kernel.root_dir%/config/routing.php',
            ),
            // ...
        ));

        // Configurazione di Twig
        $container->loadFromExtension('twig', array(
            'debug'            => '%kernel.debug%',
            'strict_variables' => '%kernel.debug%',
        ));

        // ...

.. note::

   Vedremo esattamente come caricare ogni formato di file nella prossima sezione,
   `Ambienti`_.

Ogni voce di primo livello, come ``framework`` o ``twig``, definisce la configurazione
per un particolare bundle. Per esempio, la voce ``framework`` definisce la configurazione
per il bundle del nucleo di Symfony FrameworkBundle e include configurazioni per
rotte, template e altri sistemi fondamentali.

Per ora, non ci preoccupiamo delle opzioni di configurazione specifiche di ogni
sezione. Il file di configurazione ha delle opzioni predefinite impostate.
Leggendo ed esplorando ogni parte di Symfony, le opzioni di configurazione
specifiche saranno man mano approfondite.

.. sidebar:: Formati di configurazione

    Nei vari capitoli, tutti gli esempi di configurazione saranno mostrati in tutti e
    tre i formati (YAML, XML e PHP). Ciascuno ha i suoi vantaggi e svantaggi. La scelta
    è lasciata allo sviluppatore:

    * *YAML*: Semplice, pulito e leggibile (se ne può sapere di più in
      ":doc:`/components/yaml/yaml_format`");

    * *XML*: Più potente di YAML e supportato nell'autocompletamento dagli IDE;

    * *PHP*: Molto potente, ma meno leggibile dei formati di configurazione standard.

Esportazione della configurazione predefinita
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Si può esportare la configurazione predefinita per un bundle in yaml sulla console, usando
il comando ``config:dump-reference``. Ecco un esempio di esportazione della configurazione
predefinita di FrameworkBundle:

.. code-block:: bash

    $ app/console config:dump-reference FrameworkBundle

Si può anche usare l'alias dell'estensione (voce di configurazione):

.. code-block:: bash

    $ app/console config:dump-reference framework

.. note::

    Vedere la ricetta :doc:`/cookbook/bundles/extension` per
    informazioni sull'aggiunta di configurazioni per un bundle.

.. index::
   single: Ambienti; Introduzione

.. _environments-summary:

Ambienti
--------

Un'applicazione può girare in vari ambienti. I diversi ambienti condividono lo stesso
codice PHP (tranne per il front controller), ma usano differenti configurazioni.
Per esempio, un ambiente ``dev`` salverà nei log gli avvertimenti e gli errori,
mentre un ambiente ``prod`` solamente gli errori. Alcuni file sono ricostruiti a
ogni richiesta nell'ambiente ``dev`` (per facilitare gli sviluppatori), ma salvati
in cache nell'ambiente ``prod``. Tutti gli ambienti stanno insieme nella stessa
macchina e sono eseguiti nella stessa applicazione.

Un progetto Symfony generalmente inizia con tre ambienti (``dev``, ``test``
e ``prod``), ma creare nuovi ambienti è facile. Si può vedere la propria applicazione
in ambienti diversi, semplicemente cambiando il front controller nel
browser. Per vedere l'applicazione in ambiente ``dev``, accedere all'applicazione
tramite il front controller di sviluppo:

.. code-block:: text

    http://localhost/app_dev.php/random/10

Se si preferisce vedere come l'applicazione si comporta in ambiente di produzione,
richiamare invece il front controller ``prod``:

.. code-block:: text

    http://localhost/app.php/random/10

Essendo l'ambiente ``prod`` ottimizzato per la velocità, la configurazione, le
rotte e i template Twig sono compilato in classi in puro PHP e messi in cache.
Per vedere delle modifiche in ambiente ``prod``, occorrerà pulire tali file
in cache e consentire che siano ricostruiti:

.. code-block:: bash

    $ php app/console cache:clear --env=prod --no-debug

.. note::

   Se si apre il file ``web/app.php``, si troverà che è configurato esplicitamente
   per usare l'ambiente ``prod``::

       $kernel = new AppKernel('prod', false);

   Si può creare un nuovo front controller per un nuovo ambiente, copiando questo
   file e cambiando ``prod`` con un altro valore.

.. note::

    L'ambiente ``test`` è usato quando si eseguono i test automatici e non può
    essere acceduto direttamente tramite il browser. Vedere il 
    :doc:`capitolo sui test</book/testing>` per maggiori dettagli.

.. index::
   single: Ambienti; Configurazione

Configurazione degli ambienti
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

La classe ``AppKernel`` è responsabile del caricare effettivamente i file
di configurazione scelti::

    // app/AppKernel.php
    public function registerContainerConfiguration(LoaderInterface $loader)
    {
        $loader->load(
            __DIR__.'/config/config_'.$this->getEnvironment().'.yml'
        );
    }

Sappiamo già che l'estensione ``.yml`` può essere cambiata in ``.xml`` o
``.php``, se si preferisce usare XML o PHP per scrivere la propria configurazione.
Si noti anche che ogni ambiente carica i propri file di configurazione. Consideriamo
il file di configurazione per l'ambiente ``dev``.

.. configuration-block::

    .. code-block:: yaml

        # app/config/config_dev.yml
        imports:
            - { resource: config.yml }

        framework:
            router:   { resource: "%kernel.root_dir%/config/routing_dev.yml" }
            profiler: { only_exceptions: false }

        # ...

    .. code-block:: xml

        <!-- app/config/config_dev.xml -->
        <?xml version="1.0" encoding="UTF-8" ?>
        <container xmlns="http://symfony.com/schema/dic/services"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xmlns:framework="http://symfony.com/schema/dic/symfony"
            xsi:schemaLocation="http://symfony.com/schema/dic/services
                http://symfony.com/schema/dic/services/services-1.0.xsd
                http://symfony.com/schema/dic/symfony
                http://symfony.com/schema/dic/symfony/symfony-1.0.xsd">

            <imports>
                <import resource="config.xml" />
            </imports>

            <framework:config>
                <framework:router resource="%kernel.root_dir%/config/routing_dev.xml" />
                <framework:profiler only-exceptions="false" />
            </framework:config>

            <!-- ... -->
        </container>

    .. code-block:: php

        // app/config/config_dev.php
        $loader->import('config.php');

        $container->loadFromExtension('framework', array(
            'router' => array(
                'resource' => '%kernel.root_dir%/config/routing_dev.php',
            ),
            'profiler' => array('only-exceptions' => false),
        ));

        // ...

La voce ``imports`` è simile all'istruzione ``include`` di PHP e garantisce
che il file di configurazione principale (``config.yml``) sia caricato per primo.
Il resto del file gestisce la configurazione per aumentare il livello di log, oltre
ad altre impostazioni utili all'ambiente di sviluppo.

Sia l'ambiente ``prod`` che quello ``test`` seguono lo stesso modello: ogni ambiente
importa il file di configurazione di base e quindi modifica i suoi file di configurazione
per soddisfare le esigenze dello specifico ambiente. Questa è solo una convenzione, ma
consente di riusare la maggior parte della propria configurazione e personalizzare solo
le parti diverse tra gli ambienti.

Riepilogo
---------

Congratulazioni! Ora abbiamo visto ogni aspetto fondamentale di Symfony e scoperto
quanto possa essere facile e flessibile. Pur essendoci ancora *moltissime*
caratteristiche da scoprire, assicuriamoci di tenere a mente alcuni aspetti
fondamentali:

* creare una pagine è un processo in tre passi, che coinvolge una **rotta**, un **controllore**
  e (opzionalmente) un **template**.

* ogni progetto contiene solo alcune cartelle principali: ``web/`` (risorse web e
  front controller), ``app/`` (configurazione), ``src/`` (i propri bundle)
  e ``vendor/`` (codice di terze parti) (c'è anche la cartella ``bin/``, usata per aiutare
  nell'aggiornamento delle librerie dei venditori);

* ogni caratteristica in Symfony (incluso in nucleo del framework stesso) è organizzata in
  *bundle*, insiemi strutturati di file relativi a tale caratteristica;

* la **configurazione** per ciascun bundle risiede nella cartella ``app/config`` e
  può essere specificata in YAML, XML o PHP;

* la **configurazione dell'applicazione** globale si trova nella cartella
  ``app/config``;

* ogni **ambiente** è accessibile tramite un diverso front controller (p.e.
  ``app.php`` e ``app_dev.php``) e carica un diverso file di configurazione.

Da qui in poi, ogni capitolo introdurrà strumenti sempre più potenti e concetti
sempre più avanzati. Più si imparerà su Symfony, più si apprezzerà la flessibilità
della sua architettura e la potenza che dà nello sviluppo rapido di
applicazioni.

.. _`Twig`: http://twig.sensiolabs.org
.. _`bundle di terze parti`: http://knpbundles.com
.. _`Symfony Standard Edition`: http://symfony.com/download
.. _`documentazione su DirectoryIndex di Apache`: http://httpd.apache.org/docs/current/mod/mod_dir.html
.. _`documentazione su HttpCoreModule di Nginx`: http://wiki.nginx.org/HttpCoreModule#location
