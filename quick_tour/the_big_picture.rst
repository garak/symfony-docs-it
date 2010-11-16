Un quadro generale
==================

Volete provare Symfony2 avendo a disposizione solo dieci minuti? Questa prima
parte di questa guida è stata scritta appositamente. Essa spiega come
partire subito con Symfony2, mostrando la struttura di un semplice progetto
già pronto.

Avendo già usato un framework per il web, ci si troverà come a casa con Symfony2.

.. index::
   pair: Sandbox; Scaricamento

Scaricare e installare Symfony2
-------------------------------

Prima di tutto, verificare di avere almeno PHP 5.3.2 installato e configurato
correttamente per funzionare con un server web, come Apache.

Pronti? Iniziamo scaricando Symfony2. Per iniziare ancora più rapidamente,
useremo la sandbox di Symfony2. È un progetto in cui tutte le librerie e
alcuni semplici controllori sono giù inclusi e la configurazione di base
è già stata fatta. Il grosso vantaggio della sandbox, rispetto ad altri tipi
di installazione, è che si può iniziare a sperimentare Symfony2 immediatamente.

Scaricare la `sandbox`_ e scompattarla nella cartella radice del web. Si dovrebbe
ora avere una cartella ``sandbox/``::

    www/ <- cartella radice del web
        sandbox/ <- archivio scompattato
            app/
                cache/
                config/
                logs/
            src/
                Application/
                    HelloBundle/
                        Controller/
                        Resources/
                vendor/
                    symfony/
            web/

.. index::
   single: Installazione; Verifica

Verifica della configurazione
-----------------------------

Per evitare mal di testa successivamente, verificare che la configurazione sia in
grado di eseguire un progetto Symfony2 senza problemi, richiamando il seguente
URL:

    http://localhost/sandbox/web/check.php

Leggere attentamente il risultato dello script e risolvere eventuali problemi
riscontrati.

Ora, richiedere la prima "vera" pagina di Symfony2:

    http://localhost/sandbox/web/app_dev.php/

Symfony2 dovrebbe congratularsi per il duro lavoro svolto finora!

Creare la prima applicazione
----------------------------

La sandbox è distribuita con una semplice ":term:`applicazione`" Hello World,
che sarà l'applicazione che useremo per imparare di più su Symfony2. Andare
al seguente URL per essere salutati da Symfony2 (sostituire "Fabien" col
proprio nome):

    http://localhost/sandbox/web/app_dev.php/hello/Fabien

Cosa sta succedendo? Analizziamo l'URL:

.. index:: Front Controller

* ``app_dev.php``: Questo è un "front controller". È il punto unico di ingresso
  dell'applicazione e risponde a tutte le richieste dell'utente;

* ``/hello/Fabien``: Questo è un percorso "virtual" alla risorsa a cui l'utente
  vuole accedere.

La responsabilità dello sviluppatore è quella di scrivere il codice che mappa
la richiesta dell'utente (``/hello/Fabien``) alla risorsa associata (``Hello
Fabien!``).

.. index::
   single: Configurazione

Configurazione
~~~~~~~~~~~~~~

Come fa Symfony2 a dirigere la richiesta al codice? Semplicmente leggendo
alcuni file di configurazione.

Tutti i file di configurazione di Symfony2 possono essere scritti in PHP, XML
o `YAML`_ (YAML è un semplice formato che rende la descrizione di una
configurazione molto semplice).

.. tip::

   L'impostazione predefinita della sandbox è usare YAML, ma si può passare
   facilmente a XML o PHP modificando il file ``app/AppKernel.php``. Lo si
   può fare ora, guardando le istruzioni in fondo a questo file (la guida
   mostra la configurazione per tutti i formati supportati).

.. index::
   single: Rotte
   pair: Configurazione; Rotte

Rotte
~~~~~

Dunque, Symfony2 dirige la richiesta in base al file di configurazione delle
rotte:

.. configuration-block::

    .. code-block:: yaml

        # app/config/routing.yml
        homepage:
            pattern:  /
            defaults: { _controller: FrameworkBundle:Default:index }

        hello:
            resource: HelloBundle/Resources/config/routing.yml

    .. code-block:: xml

        <!-- app/config/routing.xml -->
        <?xml version="1.0" encoding="UTF-8" ?>

        <routes xmlns="http://www.symfony-project.org/schema/routing"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://www.symfony-project.org/schema/routing http://www.symfony-project.org/schema/routing/routing-1.0.xsd">

            <route id="homepage" pattern="/">
                <default key="_controller">FrameworkBundle:Default:index</default>
            </route>

            <import resource="HelloBundle/Resources/config/routing.xml" />
        </routes>

    .. code-block:: php

        // app/config/routing.php
        use Symfony\Component\Routing\RouteCollection;
        use Symfony\Component\Routing\Route;

        $collection = new RouteCollection();
        $collection->addRoute('homepage', new Route('/', array(
            '_controller' => 'FrameworkBundle:Default:index',
        )));
        $collection->addCollection($loader->import("HelloBundle/Resources/config/routing.php"));

        return $collection;

Le prime righe del file di configurazione delle rotte definiscono quale codice
richiamare quanto l'utente richiede la risorsa "``/``". L'ultima parte è quella
più interessante, in cui si importano altri file di configurazione delle rotte,
come segue:

.. configuration-block::

    .. code-block:: yaml

        # src/Application/HelloBundle/Resources/config/routing.yml
        hello:
            pattern:  /hello/:name
            defaults: { _controller: HelloBundle:Hello:index }

    .. code-block:: xml

        <!-- src/Application/HelloBundle/Resources/config/routing.xml -->
        <?xml version="1.0" encoding="UTF-8" ?>

        <routes xmlns="http://www.symfony-project.org/schema/routing"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://www.symfony-project.org/schema/routing http://www.symfony-project.org/schema/routing/routing-1.0.xsd">

            <route id="hello" pattern="/hello/:name">
                <default key="_controller">HelloBundle:Hello:index</default>
            </route>
        </routes>

    .. code-block:: php

        // src/Application/HelloBundle/Resources/config/routing.php
        use Symfony\Component\Routing\RouteCollection;
        use Symfony\Component\Routing\Route;

        $collection = new RouteCollection();
        $collection->addRoute('hello', new Route('/hello/:name', array(
            '_controller' => 'HelloBundle:Hello:index',
        )));

        return $collection;

Ecco fatto! Come si può vedere, lo schema della risorsa "``/hello/:name``"
(una stringa che inizia con due-punti, come ``:name``, è un segnaposto) è mappata
su un controllore, riferito dal valore ``_controller``.

.. index::
   single: Controllore
   single: MVC; Controllore

Controllori
~~~~~~~~~~~

Il controllore è responsabile di restituire una rappresentazione della
risorsa (il più delle volte in HTML) ed è definito come una classe PHP:

.. code-block:: php
   :linenos:

    // src/Application/HelloBundle/Controller/HelloController.php

    namespace Application\HelloBundle\Controller;

    use Symfony\Bundle\FrameworkBundle\Controller\Controller;

    class HelloController extends Controller
    {
        public function indexAction($name)
        {
            return $this->render('HelloBundle:Hello:index.php', array('name' => $name));

            // render a Twig template instead
            // return $this->render('HelloBundle:Hello:index.twig', array('name' => $name));
        }
    }

Il codice è alquanto chiaro, ma spieghiamolo riga per riga:

* *linea 3*: Symfony2 sfrutta le nuove caratteristiche di PHP 5.3, quindi tutti i
  controllori hanno un appropriato namespace (il namespace è la prima parte del
  valore della rotta di ``_controller``: ``HelloBundle``).

* *linea 7*: Il nome del controllore è una concatenazione della seconda parte del
  valore della rotta di ``_controller`` (``Hello``) e ``Controller``. Estende la
  classe ``Controller``, che fornisce utili scorciatoie (come vedremo più avanti in
  questa guida).

* *linea 9*: Ogni controllore è composto da diverse azioni. Da configurazione,
  la pagina "hello" è gestita dall'azione ``index`` (la terza parte del valore
  di rotta di ``_controller``). Questo metodo riceve i valori dei segnaposto come
  parametri (nel nostro caso, ``$name``).

* *linea 11*: Il metodo ``render()`` carica e rende un template
  (``HelloBundle:Hello:index``) con le variabili passate come secondo parametro.

Ma che cos'è un :term:`bundle`? Tutto il codice scritto dallo sviluppatore in un
progetto Symfony2 è organizzato in dei bundle. Dal punto di vista di Symfony2,
un bundle è un insieme strutturato di file (script PHP, fogli di stile, JavaScript,
immagini, ecc.) che implementano una singola caratteristica (un blog, un forum, ecc.)
e che possono facilmente essere condivisi con altri sviluppatori. Nel nostro
esempio, abbiamo un unico bundle, ``HelloBundle``.

Template
~~~~~~~~

Quindi, il controllore rende il template ``HelloBundle:Hello:index.php``. Ma cosa
c'è nel nome di un template? ``HelloBundle`` è il nome del bundle, ``Hello`` è
il controllore e ``index.php`` il nome del file del template. Il template stesso
è fatto di HTML e di semplici espressioni PHP:

.. code-block:: html+php

    <!-- src/Application/HelloBundle/Resources/views/Hello/index.php -->
    <?php $view->extend('HelloBundle::layout.php') ?>

    Hello <?php echo $name ?>!

Congratulazioni! Avete appena guardato il vostro primo pezzo di codice di
Symfony2. Non è stato difficile, vero? Symfony2 rende molto facile implementare
siti web migliori e più veloci.

.. index::
   single: Ambiente
   single: Configurazione; Ambiente

Lavorare con gli ambienti
-------------------------

Ora che si possiede una migliore comprensione di come funziona Symfony2, è
ora di dare un'occhiata più da vicino al fondo della pagina: si noterà
una piccola barra con i logo di Symfony2 e di PHP. Questa barra è chiamata
"Web Debug Toolbar" ed è il miglior amico dello sviluppatore. Ovviamente
questo strumento non deve essere mostrato quando si rilascia l'applicazione
su un server di produzione. Per questo motivo si troverà un altro front
controller (``app.php``) nella cartella ``web/``, ottimizzato per l'ambiente
di produzione:

    http://localhost/sandbox/web/app.php/hello/Fabien

Se si usa Apache con ``mod_rewrite`` abilitato, si può anche omettere la
parte ``app.php`` dell'URL:

    http://localhost/sandbox/web/hello/Fabien

Infine, ma non meno importante, sui server di produzione si dovrebbe far
puntare la cartella radice del web alla cartella ``web/``, per rendere
l'installazione sicura e avere URL più allettanti:

    http://localhost/hello/Fabien

Per rendere l'ambiente di produzione più veloce possibile, Symfony2
mantiene una cache sotto la cartella ``app/cache/``. Quando si fanno
delle modifiche al codice o alla configurazione, occorre rimuovere
a mano i file in cache. Per questo si dovrebbe sempre usare il front
controller di sviluppo (``app_dev.php``) mentre si lavora al progetto.

Considerazioni finali
---------------------

I dieci minuti sono finiti. Ora si dovrebbe essere in grado di creare
le proprie semplici rotte, i controllori e i template. Come esercizio,
si può provare a costruire qualcosa di più utile dell'applicazione
"Hello"! Ma se siete impazienti di imparare di più su Symfony2, si può
proseguire leggendo anche subito la prossima parte di questa guida,
in cui approfondiremo il sistema dei template.

.. _sandbox: http://symfony-reloaded.org/code#sandbox
.. _YAML:    http://www.yaml.org/
