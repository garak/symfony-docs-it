.. index::
   single: Interno

Interno
=======

Se si vuole capire come funziona Symfony2 ed estenderlo, in questa sezione si potranno
trovare spiegazioni approfondite dell'interno di
Symfony2.

.. note::

    La lettura di questa sezione è necessaria solo per capire come funziona Symfony2 dietro
    le quinte oppure se si vuole estendere Symfony2.

Panoramica
----------

Il codice di Symfony2 è composto da diversi livelli indipendenti. Ogni livello
è costruito sulla base del precedente.

.. tip::

    L'auto-caricamento non viene gestito direttamente dal framework, ma
    dall'autoloader di Composer (``vendor/autoload.php``), incluso nel
    file ``app/autoload.php``.

Il componente HttpFoundation
~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Il livello più profondo è il componente :namespace:`Symfony\\Component\\HttpFoundation`.
HttpFoundation fornisce gli oggetti principali necessari per trattare con HTTP.
È un'astrazione orientata gli oggetti di alcune funzioni e variabili native di
PHP:

* La classe :class:`Symfony\\Component\\HttpFoundation\\Request` astrae le
  variabili globali principali di PHP, come ``$_GET``, ``$_POST``, ``$_COOKIE``,
  ``$_FILES`` e ``$_SERVER``;

* La classe :class:`Symfony\\Component\\HttpFoundation\\Response` astrae alcune
  funzioni PHP, come ``header()``, ``setcookie()`` ed ``echo``;

* La classe :class:`Symfony\\Component\\HttpFoundation\\Session` e l'interfaccia
  :class:`Symfony\\Component\\HttpFoundation\\SessionStorage\\SessionStorageInterface`
  astraggono le funzioni di gestione della sessione ``session_*()``.

.. note::

    Si può approfondire nel :doc:`componente HttpFoundation </components/http_foundation/introduction>`.

Il componente ``HttpKernel``
~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Sopra HttpFoundation c'è il componente :namespace:`Symfony\\Component\\HttpKernel`.
HttpKernel gestisce la parte dinamica di HTTP e incapsula in modo leggero
le classi ``Request`` e ``Response``, per standardizzare il modo in cui sono gestite
le richieste. Fornisce anche dei punti di estensione e degli strumenti che lo
rendono il punto di partenza ideale per creare un framework web senza troppe sovrastrutture.

Opzionalmente, aggiunge anche configurabilità ed estensibilità, grazie al
componente Dependency Injection e a un potente sistema di plugin (bundle).

.. seealso::

    Approfondimento sul :doc:`componente HttpKernel </components/http_kernel/introduction>`,
    su :doc:`dependency injection </book/service_container>` e
    sui :doc:`bundle </cookbook/bundles/best_practices>`.

Il bundle ``FrameworkBundle``
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Il bundle :namespace:`Symfony\\Bundle\\FrameworkBundle` è il bundle che lega insieme i
componenti e le librerie principali, per fare un framework MVC leggero e
veloce. Dispone in una configurazione predefinita adeguata e di convenzioni che facilitano
la curva di apprendimento.

.. index::
   single: Interno; Kernel

Kernel
------

La classe :class:`Symfony\\Component\\HttpKernel\\HttpKernel` è la classe centrale
di Symfony2 ed è responsabile della gestione delle richieste del client. Il suo scopo
principale è "convertire" un oggetto :class:`Symfony\\Component\\HttpFoundation\\Request`
in un oggetto :class:`Symfony\\Component\\HttpFoundation\\Response`.

Ogni kernel di Symfony2 implementa
:class:`Symfony\\Component\\HttpKernel\\HttpKernelInterface`::

    function handle(Request $request, $type = self::MASTER_REQUEST, $catch = true)

.. index::
   single: Interno; Risoluzione dei controllori

Controllori
~~~~~~~~~~~

Per convertire una ``Request`` in una ``Response``, il kernel si appoggia a un
"controllore". Un controllore può essere qualsiasi funzione o metodo PHP valido.

Il kernel delega la scelta di quale controllore debba essere eseguito a un'implementazione
di
:class:`Symfony\\Component\\HttpKernel\\Controller\\ControllerResolverInterface`::

    public function getController(Request $request);

    public function getArguments(Request $request, $controller);

Il metodo
:method:`Symfony\\Component\\HttpKernel\\Controller\\ControllerResolverInterface::getController`
restituisce il controllore (una funzione PHP) associato alla ``Request`` data.
L'implementazionoe predefinita
(:class:`Symfony\\Component\\HttpKernel\\Controller\\ControllerResolver`)
cerca un attributo ``_controller`` della richiesta, che rappresenta il nome del
controllore (una stringa "classe::metodo", come ``Bundle\BlogBundle\PostController:indexAction``).

.. tip::

    L'implementazione predefinita usa
    :class:`Symfony\\Bundle\\FrameworkBundle\\EventListener\\RouterListener`
    per definire l'attributo ``_controller`` della richista (vedere :ref:`kernel-core-request`).

Il metodo
:method:`Symfony\\Component\\HttpKernel\\Controller\\ControllerResolverInterface::getArguments`
restituisce un array di parametri da passare al controllore. L'implementazione
predefinita risolve automaticamente i parametri, basandosi sugli attributi di
``Request``.

.. sidebar:: Parametri del controllore dai parametri della richiesta

    Per ciascun parametro, Symfony2 prova a prendere il valore dell'attributo della
    richiesta che abbia lo stesso nome. Se non definito, viene usato il valore del
    parametro predefinito, se specificato::

        // Symfony2 cerca un attributo 'id' (obbligatorio)
        // e uno 'admin' (facoltativo)
        public function showAction($id, $admin = true)
        {
            // ...
        }

.. index::
  single: Interno; Gestione della richiesta

Gestione delle richieste
~~~~~~~~~~~~~~~~~~~~~~~~

Il metodo ``handle()`` prende una ``Request`` e restituisce *sempre* una ``Response``.
Per convertire ``Request``, ``handle()`` si appoggia su ``Resolver`` e su una catena
ordinata di notifiche di eventi (vedere la prossima sezione per maggiori informazioni
sugli oggetti
``Event``):

#. Prima di tutto, viene notificato l'evento ``kernel.request``, se uno degli
   ascoltatori restituisce una ``Response``, salta direttamente al passo 8;

#. Viene chiamato ``Resolver``, per decidere quale controllore eseguire;

#. Gli ascoltatori dell'evento ``kernel.controller`` possono ora manipolare il
   controllore, nel modo che preferiscono (cambiarlo, avvolgerlo, ecc.);

#. Il kernel verifica che il controllore sia effettivamente un metodo valido;

#. Viene chiamato ``Resolver``, per decidere i parametri da passare al controllore;

#. Il kernel richiama il controllore;

#. Se il controllore non restituisce una ``Response``, gli ascoltatori dell'evento
   ``kernel.view`` possono convertire il valore restituito dal controllore in una ``Response``;

#. Gli ascoltatori dell'evento ``kernel.response`` possono manipolare la ``Response``
   (sia il contenuto che gli header);

#. Viene restituita la risposta.

Se viene lanciata un'eccezione durante il processo, viene notificato l'evento
``kernel.exception`` e gli ascoltatori possono convertire l'eccezione in una risposta.
Se funziona, viene notificato l'evento ``kernel.response``, altrimenti l'eccezione
viene lanciata nuovamente.

Se non si vuole che le eccezioni siano catturate (per esempio per richieste incluse),
disabilitare l'evento ``kernel.exception``, passando ``false`` come terzo parametro
del metodo ``handle()``.

.. index::
  single: Interno; Richieste interne

Richieste interne
~~~~~~~~~~~~~~~~~

In qualsiasi momento, durante la gestione della richiesta (quella "principale"), si può
gestire una sotto-richiesta. Si può passare il tipo di richiesta al metodo ``handle()``,
come secondo parametro:

* ``HttpKernelInterface::MASTER_REQUEST``;
* ``HttpKernelInterface::SUB_REQUEST``.

Il tipo è passato a tutti gli eventi e gli ascoltatori possono agire di conseguenza
(alcuni processi possono avvenire solo sulla richiesta principale).

.. index::
   pair: Kernel; Evento

Eventi
~~~~~~

Ogni evento lanciato dal kernel è una sotto-classe di
:class:`Symfony\\Component\\HttpKernel\\Event\\KernelEvent`. Questo vuol dire che
ogni evento ha accesso alle stesse informazioni di base:

* :method:`Symfony\\Component\\HttpKernel\\Event\\KernelEvent::getRequestType` - restituisce
  il *tipo* della richiesta (``HttpKernelInterface::MASTER_REQUEST``
  o ``HttpKernelInterface::SUB_REQUEST``);

* :method:`Symfony\\Component\\HttpKernel\\Event\\KernelEvent::getKernel` - restituisce
  il kernel che gestisce la richiesta;

* :method:`Symfony\\Component\\HttpKernel\\Event\\KernelEvent::getRequest` - restituisce
  la ``Request`` attualmente in gestione.

``getRequestType()``
....................

Il metodo ``getRequestType()`` consente di sapere il tipo di richiesta. Per esempio,
se un ascoltatore deve essere attivo solo per richieste principali,
aggiungere il seguente codice all'inizio del proprio metodo ascoltatore::

    use Symfony\Component\HttpKernel\HttpKernelInterface;

    if (HttpKernelInterface::MASTER_REQUEST !== $event->getRequestType()) {
        // restituire immediatamente
        return;
    }

.. tip::

    Se non si ha familiarità con il distributore di eventi di Symfony2, leggere prima
    la
    :doc:`documentazione del componente Event Dispatcher</components/event_dispatcher/introduction>`.

.. index::
   single: Evento; kernel.request

.. _kernel-core-request:

Evento ``kernel.request``
.........................

*Classe evento*: :class:`Symfony\\Component\\HttpKernel\\Event\\GetResponseEvent`

Lo scopo di questo evento e di restituire subito un oggetto ``Response`` oppure
impostare delle variabili in modo che il controllore sia richiamato dopo l'evento.
Qualsiasi ascoltatore può restituire un oggetto ``Response``, tramite il metodo
``setResponse()`` sull'evento. In questo caso, tutti gli altri ascoltatori non saranno richiamati.

Questo evento è usato da ``FrameworkBundle`` per popolare l'attributo ``_controller`` della
``Request``, tramite
:class:`Symfony\\Bundle\\FrameworkBundle\\EventListener\\RouterListener`. RequestListener
usa un oggetto :class:`Symfony\\Component\\Routing\\RouterInterface` per corrispondere alla
``Request`` e determinare il nome del controllore (memorizzato nell'attributo
``_controller`` di ``Request``).

.. seealso::

    Approfondire l':ref:`evento kernel.request <component-http-kernel-kernel-request>`.

.. index::
   single: Evento; kernel.controller

Evento ``kernel.controller``
............................

*Classe evento*: :class:`Symfony\\Component\\HttpKernel\\Event\\FilterControllerEvent`

Questo evento non è usato da ``FrameworkBundle``, ma può essere un punto di ingresso usato
per modificare il controllore da eseguire::

    use Symfony\Component\HttpKernel\Event\FilterControllerEvent;

    public function onKernelController(FilterControllerEvent $event)
    {
        $controller = $event->getController();
        // ...

        // il controllore può essere cambiato da qualsiasi funzione PHP
        $event->setController($controller);
    }

.. seealso::

    Approfondire l':ref:`evento kernel.controller <component-http-kernel-kernel-controller>`.

.. index::
   single: Evento; kernel.view

Evento ``kernel.view``
......................

*Classe evento*: :class:`Symfony\\Component\\HttpKernel\\Event\\GetResponseForControllerResultEvent`

Questo evento non è usato da ``FrameworkBundle``, ma può essere usato per implementare un
sotto-sistema di viste. Questo evento è chiamato *solo* se il controllore *non*
restituisce un oggetto ``Response``. Lo scopo dell'evento è di consentire a qualcun altro
di restituire un valore da convertire in una ``Response``.

Il valore restituito dal controllore è accessibile tramite il metodo
``getControllerResult``::

    use Symfony\Component\HttpKernel\Event\GetResponseForControllerResultEvent;
    use Symfony\Component\HttpFoundation\Response;

    public function onKernelView(GetResponseForControllerResultEvent $event)
    {
        $val = $event->getControllerResult();
        $response = new Response();

        // ... personalizzare in qualche modo la risposta dal valore restituito

        $event->setResponse($response);
    }

.. seealso::

    Approfondire l':ref:`evento kernel.view <component-http-kernel-kernel-view>`.

.. index::
   single: Evento; kernel.response

Evento ``kernel.response``
..........................

*Classe evento*: :class:`Symfony\\Component\\HttpKernel\\Event\\FilterResponseEvent`

Lo scopo di questo evento è di consentire ad altri sistemi di modificare o sostituire
l'oggetto ``Response`` dopo la sua creazione::

    public function onKernelResponse(FilterResponseEvent $event)
    {
        $response = $event->getResponse();

        // ... modificare l'oggetto Response
    }

``FrameworkBundle`` registra diversi ascoltatori:

* :class:`Symfony\\Component\\HttpKernel\\EventListener\\ProfilerListener`:
  raccoglie dati per la richiesta corrente;

* :class:`Symfony\\Bundle\\WebProfilerBundle\\EventListener\\WebDebugToolbarListener`:
  inserisce la barra di web debug;

* :class:`Symfony\\Component\\HttpKernel\\EventListener\\ResponseListener`: aggiusta
  il ``Content-Type`` della risposta, in base al formato della richiesta;

* :class:`Symfony\\Component\\HttpKernel\\EventListener\\EsiListener`: aggiunge un
  header HTTP ``Surrogate-Control`` quando si deve cercare dei tag ESI nella
  risposta.

.. seealso::

    Approfondire l':ref:`evento kernel.response <component-http-kernel-kernel-response>`.

.. index::
   single: Evento; kernel.terminate

Evento ``kernel.terminate``
...........................

Lo scopo di questo evento è quello di eseguire compiti più "pesanti", dopo che la risposta
sia stata inviata al client.

.. seealso::

    Approfondire l':ref:`evento kernel.terminate <component-http-kernel-kernel-terminate>`.

.. index::
   single: Evento; kernel.exception

.. _kernel-kernel.exception:

Evento ``kernel.exception``
...........................

*Classe evento*: :class:`Symfony\\Component\\HttpKernel\\Event\\GetResponseForExceptionEvent`

``FrameworkBundle`` registra un
:class:`Symfony\\Component\\HttpKernel\\EventListener\\ExceptionListener`, che
gira la ``Request`` a un controllore dato (il valore del parametro
``exception_listener.controller``, che deve essere nel formato
``classe::metodo``).

Un ascoltatore di questo evento può creare e impostare un oggetto ``Response``, creare
e impostare un nuovo oggetto ``Exception``, oppure non fare nulla::

    use Symfony\Component\HttpKernel\Event\GetResponseForExceptionEvent;
    use Symfony\Component\HttpFoundation\Response;

    public function onKernelException(GetResponseForExceptionEvent $event)
    {
        $exception = $event->getException();
        $response = new Response();
        // prepara l'oggetto Response in base all'eccezione catturata
        $event->setResponse($response);

        // in alternativa si può impostare una nuova eccezione
        // $exception = new \Exception('Una qualche ecccezione speciale');
        // $event->setException($exception);
    }

.. note::

    Poiché Symfony assicura che il codice di stato della risposta sia impostato nel
    modo più appropriato a seconda dell'eccezione, impostare lo stato nella risposta non
    funziona. Se si vuole sovrascrivere il codice di stato (che non andrebbe fatto senza
    buone ragioni), impostare l'header ``X-Status-Code``::

        return new Response(
            'Error',
            404 // ignorato,
            array('X-Status-Code' => 200)
        );

.. index::
   single: Distributore di eventi

Il distributore di eventi
-------------------------

Event Dispatcher (distributore di eventi) è un componente, responsabile di gran parte
della logica sottostante e del flusso dietro a una richiesta di Symfony. Per maggiori informazioni,
vedere la :doc:`documentazione del componente Event Dispatcher</components/event_dispatcher/introduction>`.

.. seealso::

    Approfondire l':ref:`evento kernel.exception <component-http-kernel-kernel-exception>`.

.. index::
   single: Profilatore

.. _internals-profiler:

Profilatore
-----------

Se abilitato, il profilatore di Symfony2 raccoglie informazioni utili su ogni richiesta
fatta alla propria applicazione e le memorizza per analisi successive. L'uso del
profilatore in ambienti di sviluppo aiuta il debug del proprio codice e a migliorare le
prestazioni. Lo si può usare anche in ambienti di produzione, per approfondire i
problemi che si presentano.

Raramente si avrà a che fare direttamente con il profilatore, visto che Symfony2 fornisce
strumenti di visualizzazione, come la barra di web debug e il profilatore web. Se si usa
Symfony2 Standard Edition, il profilatore, la barra di web debug e il profilatore
web sono già configurati con impostazioni appropriate.

.. note::

    Il profilatore raccoglie informazioni per tutte le richieste (richieste semplici,
    rinvii, eccezioni, richieste Ajax, richieste ESI) e per tutti i metodi e formati
    HTTP. Questo vuol dire che per un singolo URL si possono avere diversi dati di
    profilo associati (uno per ogni coppia richiesta/risposta
    esterna).

.. index::
   single: Profilatore; Visualizzazione

Visualizzare i dati di profilo
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Usare la barra di web debug
...........................

In ambiente di sviluppo, la barra di web debug è disponibile in fondo a
ogni pagina. Essa mostra un buon riassunto dei dati di profile, che danno
accesso immediato a moltissime informazioni utili, quando qualcosa non
funziona come ci si aspetta.

Se il riassunto fornito dalla barra di web debug non basta, cliccare sul
collegamento del token (una stringa di 13 caratteri casuali) per accedere al profilatore web.

.. note::

    Se il token non è cliccabile, vuol dire che le rotte del profilatore non sono state
    registrate (vedere sotto per le informazioni sulla configurazione).

Analizzare i dati di profilo con il profilatore web
...................................................

Il profilatore web è uno strumento di visualizzazione per i dati di profilo, che può
essere usato in sviluppo per il debug del codice e l'aumento delle prestazioni. Ma lo
si può anche usare per approfondire problemi occorsi in produzione. Espone tutte le
informazioni raccolte dal profilatore in un'interfaccia web.

.. index::
   single: Profilatore; Usare il servizio del profilatore

Accedere alle informazioni di profilo
.....................................

Non occorre usare il visualizzatore predefinito per accedere alle informazioni di
profilo. Ma come si possono recuperare informazioni di profilo per una specifica
richiesta, dopo che è accaduta? Quando il profilatore memorizza i dati su una richiesta, vi
associa anche un token. Questo token è disponibile nell'header HTTP ``X-Debug-Token``
della risposta::

    $profile = $container->get('profiler')->loadProfileFromResponse($response);

    $profile = $container->get('profiler')->loadProfile($token);

.. tip::

    Quando il profilatore è abiliato, ma non lo è la barra di web debug, oppure quando si
    vuole il token di una richiesta Ajax, usare uno strumento come Firebug per ottenere
    il valore dell'header HTTP ``X-Debug-Token``.

Usare il metodo :method:`Symfony\\Component\\HttpKernel\\Profiler\\Profiler::find`
per accedere ai token, in base a determinati criteri::

    // gli ultimi 10 token
    $tokens = $container->get('profiler')->find('', '', 10);

    // gli ultimi 10 token per URL che contengono /admin/
    $tokens = $container->get('profiler')->find('', '/admin/', 10);

    // gli ultimi 10 token per richieste locali
    $tokens = $container->get('profiler')->find('127.0.0.1', '', 10);

Se si vogliono manipolare i dati di profilo su macchine diverse da quella che
ha generato le informazioni, usare i metodi
:method:`Symfony\\Component\\HttpKernel\\Profiler\\Profiler::export` e
:method:`Symfony\\Component\\HttpKernel\\Profiler\\Profiler::import`::

    // sulla macchina di produzione
    $profile = $container->get('profiler')->loadProfile($token);
    $data = $profiler->export($profile);

    // sulla macchina di sviluppo
    $profiler->import($data);

.. index::
   single: Profilatore; Visualizzare

Configurazione
..............

La configurazione predefinita di Symfony2 ha delle impostazioni adeguate per il
profilatore, la barra di web debug e il profilatore web. Ecco per esempio
la configurazione per l'ambiente di sviluppo:

.. configuration-block::

    .. code-block:: yaml

        # carica il profilatore
        framework:
            profiler: { only_exceptions: false }

        # abilita il profilatore web 
        web_profiler:
            toolbar: true
            intercept_redirects: true

    .. code-block:: xml

        <?xml version="1.0" encoding="UTF-8" ?>
        <container xmlns="http://symfony.com/schema/dic/services"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xmlns:webprofiler="http://symfony.com/schema/dic/webprofiler"
            xmlns:framework="http://symfony.com/schema/dic/symfony"
            xsi:schemaLocation="http://symfony.com/schema/dic/services http://symfony.com/schema/dic/services/services-1.0.xsd
                                http://symfony.com/schema/dic/webprofiler http://symfony.com/schema/dic/webprofiler/webprofiler-1.0.xsd
                                http://symfony.com/schema/dic/symfony http://symfony.com/schema/dic/symfony/symfony-1.0.xsd">

            <!-- carica il profilatore -->
            <framework:config>
                <framework:profiler only-exceptions="false" />
            </framework:config>

            <!-- abilita il profilatore web -->
            <webprofiler:config
                toolbar="true"
                intercept-redirects="true"
                verbose="true"
            />
        </container>

    .. code-block:: php

        // carica il profilatore
        $container->loadFromExtension('framework', array(
            'profiler' => array('only-exceptions' => false),
        ));

        // abilita il profilatore web
        $container->loadFromExtension('web_profiler', array(
            'toolbar'             => true,
            'intercept-redirects' => true,
            'verbose'             => true,
        ));

Quando ``only-exceptions`` è impostato a ``true``, il profilatore raccoglie dati solo
quando l'applicazione solleva un'eccezione.

Quando ``intercept-redirects`` è impostata ``true``, il profilatore web intercetta i
rinvii e dà l'opportunità di guardare i dati raccolti, prima di seguire il
rinvio.

Se si abilita il profilatore web, occorre anche montare le rotte del profilatore:

.. configuration-block::

    .. code-block:: yaml

        _profiler:
            resource: @WebProfilerBundle/Resources/config/routing/profiler.xml
            prefix:   /_profiler

    .. code-block:: xml

        <?xml version="1.0" encoding="UTF-8" ?>
        <routes xmlns="http://symfony.com/schema/routing"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://symfony.com/schema/routing
                http://symfony.com/schema/routing/routing-1.0.xsd">

            <import
                resource="@WebProfilerBundle/Resources/config/routing/profiler.xml"
                prefix="/_profiler"
            />
        </routes>

    .. code-block:: php

        $collection->addCollection(
            $loader->import(
                "@WebProfilerBundle/Resources/config/routing/profiler.xml"
            ),
            '/_profiler'
        );

Poiché il profilatore aggiunge un po' di sovraccarico, probabilmente lo si abiliterà solo
in alcune circostanze in ambiente di produzione. L'impostazione ``only-exceptions``
limita il profilo alle pagine 500, ma che succede se si vogliono più informazioni quando
il client ha uno specifico indirizzo IP, oppure per una parte limitata del sito? Si
può usare un Profiler Matcher, su cui si può approfondire
in ":doc:`/cookbook/profiler/matchers`".

Imparare di più dal ricettario
------------------------------

* :doc:`/cookbook/testing/profiling`
* :doc:`/cookbook/profiler/data_collector`
* :doc:`/cookbook/event_dispatcher/class_extension`
* :doc:`/cookbook/event_dispatcher/method_behavior`

.. _`componente Dependency Injection di Symfony2`: https://github.com/symfony/DependencyInjection
