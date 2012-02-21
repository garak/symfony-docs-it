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
    indipendentemente, con l'aiuto della classe
    :class:`Symfony\\Component\\ClassLoader\\UniversalClassLoader` e
    del file ``src/autoload.php``. Leggere il :doc:`capitolo dedicato
    </cookbook/tools/autoloader>` per maggiori informazioni.

Il componente ``HttpFoundation``
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

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

Il componente ``HttpKernel``
~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Sopra HttpFoundation c'è il componente :namespace:`Symfony\\Component\\HttpKernel`.
HttpKernel gestisce la parte dinamica di HTTP e incapsula in modo leggero
le classi ``Request`` e ``Response``, per standardizzare il modo in cui sono gestite
le richieste. Fornisce anche dei punti di estensione e degli strumenti che lo
rendono il punto di partenza ideale per creare un framework web senza troppe
sovrastrutture.

Opzionalmente, aggiunge anche configurabilità ed estensibilità, grazie al
componente Dependency Injection e a un potente sistema di plugin (bundle).

.. seealso::

    Approfondimento sul componente :doc:`HttpKernel <kernel>`. Approfondimento
    sul componente :doc:`Dependency Injection </book/service_container>`
    e sui :doc:`Bundle </cookbook/bundles/best_practices>`.


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
di :class:`Symfony\\Component\\HttpKernel\\Controller\\ControllerResolverInterface`::

    public function getController(Request $request);

    public function getArguments(Request $request, $controller);

Il metodo
:method:`Symfony\\Component\\HttpKernel\\Controller\\ControllerResolverInterface::getController`
restituisce il controllore (una funzione PHP) associato alla ``Request`` data.
L'implementazionoe predefinita
(:class:`Symfony\\Component\\HttpKernel\\Controller\\ControllerResolver`)
cerca un attributo ``_controller`` della richiesta, che rappresenta il nome del
controllore (una stringa "classe::metodo", come
``Bundle\BlogBundle\PostController:indexAction``).

.. tip::

    L'implementazione predefinita usa
    :class:`Symfony\\Bundle\\FrameworkBundle\\EventListener\\RouterListener`
    per definire l'attributo ``_controller`` della richista (vedere :ref:`kernel-core-request`).

Il metodo
:method:`Symfony\\Component\\HttpKernel\\Controller\\ControllerResolverInterface::getArguments`
restituisce un array di parametri da passare al controllore. L'implementazione
predefinita risolve automaticamente i parametri, basandosi sugli attributi di ``Request``.

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
sugli oggetti ``Event``):

1. Prima di tutto, viene notificato l'evento ``kernel.request``, se uno degli
   ascoltatori restituisce una ``Response``, salta direttamente al passo 8;

2. Viene chiamato ``Resolver``, per decidere quale controllore eseguire;

3. Gli ascoltatori dell'evento ``kernel.controller`` possono ora manipolare il
   controllore, nel modo che preferiscono (cambiarlo, avvolgerlo, ecc.);

4. Il kernel verifica che il controllore sia effettivamente un metodo valido;

5. Viene chiamato ``Resolver``, per decidere i parametri da passare al controllore;

6. Il kernel richiama il controllore;

7. Se il controllore non restituisce una ``Response``, gli ascoltatori dell'evento
   ``kernel.view`` possono convertire il valore restituito dal controllore in una ``Response``;

8. Gli ascoltatori dell'evento ``kernel.response`` possono manipolare la ``Response``
   (sia il contenuto che gli header);

9. Viene restituita la risposta.

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

* ``getRequestType()`` - restituisce il *tipo* della richiesta
  (``HttpKernelInterface::MASTER_REQUEST`` o ``HttpKernelInterface::SUB_REQUEST``);

* ``getKernel()`` - restituisce il kernel che gestisce la richiesta;

* ``getRequest()`` - restituisce la ``Request`` attualmente in gestione.

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
    la sezione :ref:`event_dispatcher`.

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

.. index::
   single: Evento; kernel.controller

Evento ``kernel.controller``
............................

*Classe evento*: :class:`Symfony\\Component\\HttpKernel\\Event\\FilterControllerEvent`

Questo evento non è usato da ``FrameworkBundle``, ma può essere un punto di ingresso usato
per modificare il controllore da eseguire:

.. code-block:: php

    use Symfony\Component\HttpKernel\Event\FilterControllerEvent;

    public function onKernelController(FilterControllerEvent $event)
    {
        $controller = $event->getController();
        // ...

        // il controllore può essere cambiato da qualsiasi funzione PHP
        $event->setController($controller);
    }

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
        $val = $event->getReturnValue();
        $response = new Response();
        // personalizzare in qualche modo la risposta dal valore restituito

        $event->setResponse($response);
    }

.. index::
   single: Evento; kernel.response

Evento ``kernel.response``
..........................

*Classe evento*: :class:`Symfony\\Component\\HttpKernel\\Event\\FilterResponseEvent`

Lo scopo di questo evento è di consentire ad altri sistemi di modificare o sostituire
l'oggetto ``Response`` dopo la sua creazione:

.. code-block:: php

    public function onKernelResponse(FilterResponseEvent $event)
    {
        $response = $event->getResponse();
        // .. modificare l'oggetto Response
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
e impostare un nuovo oggetto ``Exception``, oppure non fare nulla:

.. code-block:: php

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

.. index::
   single: Distributore di eventi

.. _`book-internals-event-dispatcher`:

Il distributore di eventi
-------------------------

Il codice orientato agli oggetti è riuscito ad assicurare l'estensibilità del codice.
Creando classi con responsabilità ben definite, il codice diventa più flessibile e
lo sviluppatore può estendere le classi con delle sotto-classi, per modificare il loro
comportamento. Ma se si vogliono condividere le modifiche con altri sviluppatori che
hanno fatto a loro volta delle sotto-classi, l'ereditarietà inizia a diventare un problema.

Consideriamo un esempio dal mondo reale, in cui si vuole fornire un sistema di plugin per
il proprio progetto. Un plugin dovrebbe essere in grado di aggiungere metodi o di fare
qualcosa prima o dopo che un altro metodo venga eseguito, senza interferire con altri
plugin. Questo non è un problema facile da risolvere con l'ereditarietà singola, mentre
l'ereditarietà multipla (ove possibile in PHP) ha i suoi difetti.

Il distributore di eventi di Symfony2 implementa il pattern `Observer`_ in un modo semplice
ed efficace, per rendere possibili tutte queste cose e per rendere i propri progetti
veramente estensibili.

Prendiamo un semplice esempio dal `componente HttpKernel di Symfony2`_. Una volta che
un oggetto ``Response`` è stato creato, potrebbe essere utile consentire ad altri elementi
del sistema di modificarlo (p.e. aggiungere degli header per la cache) prima che sia
effettivamente usato. Per poterlo fare, il kernel di Symfony2 lancia un evento,
``kernel.response``. Ecco come funziona:

* Un *ascoltatore* (un oggetto PHP) dice all'oggetto *distributore* centrale che vuole
  ascoltare l'evento ``kernel.response``;

* A un certo punto, il kernel di Symfony2 dice all'oggetto *distributore* di distribuire
  l'evento ``kernel.response``, passando con esso un oggetto ``Event``, che ha accesso
  all'oggetto ``Response``;

* Il distributore notifica a (cioè chiamat un metodo su) tutti gli ascoltatori dell'evento
  ``kernel.response``, consentendo a ciascuno di essi di effettuare modifiche sull'oggetto
  ``Response``.

.. index::
   single: Distributore di eventi; Eventi

.. _event_dispatcher:

Eventi
~~~~~~

Quando un evento viene distribuito, è identificato da un nome univoco (p.e.
``kernel.response``), a cui un numero qualsiasi di ascoltatori può ascoltare. Inoltre,
un'istanza di :class:`Symfony\\Component\\EventDispatcher\\Event` viene creata e
passata a tutti gli ascoltatori. Come vedremo più avanti, l'oggetto stesso ``Event``
spesso contiene dati sull'evento distribuito.

.. index::
   pair: Distributore di eventi; Convenzioni sui nomi

Convenzioni sui nomi
....................

Il nome univoco di un evento può essere una stringa qualsiasi, ma segue facoltativamente
alcune piccole convenzioni sui nomi:

* usa solo lettere minuscole, numeri, punti (``.``) e sotto-linee (``_``);

* ha un prefisso con uno spazio dei nomi, seguito da un punto (p.e. ``kernel.``);

* finisce con un verbo che indichi l'azione che sta per essere eseguita (p.e.
  ``request``).

Ecco alcuni esempi di buoni nomi di eventi:

* ``kernel.response``
* ``form.pre_set_data``

.. index::
   single: Distributore di eventi; Sotto-classi evento

Nomi di eventi e oggetti evento
...............................

Quando il distributore notifica agli ascoltatori, passa un oggetto ``Event`` a
questi ultimi. La classe base ``Event`` è molto semplice: contiene un metodo
per bloccare la :ref:`propagazione degli eventi<event_dispatcher-event-propagation>`,
non molto di più.

Spesso, occorre passare i dati su uno specifico evento insieme all'oggetto
``Event``, in modo che gli ascoltatori abbiano le informazioni necessarie. Nel caso
dell'evento ``kernel.response``, l'oggetto ``Event`` creato e passato a ciascun
ascoltatore è in realtà di tipo
:class:`Symfony\\Component\\HttpKernel\\Event\\FilterResponseEvent`, una sotto-classe
dell'oggetto base ``Event``. Questa classe contiene metodi come ``getResponse`` e
``setResponse``, che consentono agli ascoltatori di ottenere o anche sostituire
l'oggetto ``Response``.

La morale della storia è questa: quando si crea un ascoltatore di un evento, l'oggetto
``Event`` passato all'ascoltatore potrebbe essere una speciale sotto-classe, che possiede
ulteriori metodi per recuperare informazioni dall'evento e per rispondere
a esso.

Il distributore
~~~~~~~~~~~~~~~

Il distributore è l'oggetto centrale del sistema di distribuzione degli eventi. In
generale, viene creato un solo distributore di eventi, che mantiene un registro di
ascoltatori. Quando un evento viene distribuito tramite il distributore, esso notifica
a tutti gli ascoltatori registrati con tale evento.

.. code-block:: php

    use Symfony\Component\EventDispatcher\EventDispatcher;

    $dispatcher = new EventDispatcher();

.. index::
   single: Distributore di eventi; Ascoltatori

Connettere gli ascoltatori
~~~~~~~~~~~~~~~~~~~~~~~~~~

Per trarre vantaggio da un evento esistente, occorre connettere un ascoltatore
al distributore, in modo che possa essere notificato quando l'evento viene distribuito.
Un chiamata al metodo ``addListener()`` del distributore associa una funzione PHP
a un evento:

.. code-block:: php

    $listener = new AcmeListener();
    $dispatcher->addListener('pippo.azione', array($listener, 'allAzionePippo'));

Il metodo ``addListener()`` accetta fino a tre parametri:

* Il nome dell'evento (stringa) che questo ascoltatore vuole ascoltare;

* Una funzione PHP, che sarà notificata quando viene lanciato un evento che sta
  ascoltando;

* Un intero, opzionale, di priorità (più alto equivale a più importante), che determina
  quando un ascoltatore viene avvisato, rispetto ad altri ascoltatori (il valore predefinito
  è ``0``). Se due ascoltatori hanno la stessa priorità, sono eseguito nello stesso ordine
  con cui sono stati aggiunti al distributore.

.. note::

    Una `funzione PHP`_ è una variabile PHP che può essere usata dalla funzione
    ``call_user_func()`` e che restituisce ``true``, se passata alla funzione
    ``is_callable()``. Può essere un'istanza di una ``\Closure``, una stringa che
    rappresenta una funzione oppure un array che rappresenta un metodo di un oggetto
    o di una classe.

    Finora, abbiamo visto come oggetti PHP possano essere registrati come ascoltatori.
    Si possono anche registrare `Closure`_ PHP come ascoltatori di eventi:

    .. code-block:: php

        use Symfony\Component\EventDispatcher\Event;

        $dispatcher->addListener('pippo.azione', function (Event $event) {
            // sarà eseguito quando l'evento pippo.azione viene distribuito
        });

Una volta che un ascoltatore è registrato con il distributore, esso aspetta fino a che
l'evento non è notificato. Nell'esempio visto sopra, quando l'evento ``pippo.azione`` è
distribuito, il distributore richiama il metodo ``AcmeListener::allAzionePippo`` e passa
l'oggetto ``Event`` come unico parametro:

.. code-block:: php

    use Symfony\Component\EventDispatcher\Event;

    class AcmeListener
    {
        // ...

        public function allAzionePippo(Event $event)
        {
            // fare qualcosa
        }
    }

.. tip::

    Se si usa il framework MVC di Symfony2 MVC, gli ascoltatori possono essere
    registrati tramite la :ref:`configurazione <dic-tags-kernel-event-listener>`. Come
    bonus aggiuntivo, gli oggetti ascoltatori sono istanziati solo all'occorrenza.

In alcuni casi, una sotto-classe speciale ``Event``, specifica dell'evento dato, viene
passata all'ascoltatore. Questo dà accesso all'ascoltatore a informazioni speciali
sull'evento. Leggere la documentazione o l'implementazione di ogni evento per
determinare l'esatta istanza di ``Symfony\Component\EventDispatcher\Event``
passata. Per esempio, l'evento ``kernel.event`` passa un'istanza di
``Symfony\Component\HttpKernel\Event\FilterResponseEvent``:

.. code-block:: php

    use Symfony\Component\HttpKernel\Event\FilterResponseEvent

    public function onKernelResponse(FilterResponseEvent $event)
    {
        $response = $event->getResponse();
        $request = $event->getRequest();

        // ...
    }

.. _event_dispatcher-closures-as-listeners:

.. index::
   single: Distributore di eventi; Creare e distribuire un evento

Creare e distribuire un evento
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Oltre a registrare ascoltatori su eventi esistenti, si possono creare e lanciare
eventi propri. Questo è utile quando si creano librerie di terze parti e anche
quando si vogliono mantenere diversi componenti personalizzati nel proprio
sistema flessibili e disaccoppiati.

La classe statica ``Events``
............................

Si supponga di voler creare un nuovo evento, chiamato ``negozio.ordine``, distribuito
ogni volta che un ordine viene creato dentro la propria applicazione. Per mantenere le
cose organizzate, iniziamo a creare una classe ``StoreEvents`` all'interno della propria
applicazione, che serve a definire e documentare il proprio evento:

.. code-block:: php

    namespace Acme\StoreBundle;

    final class StoreEvents
    {
        /**
         * L'evento negozio.ordine è lanciato ogni volta che un ordine viene creato
         * nel sistema.
         *
         * L'ascoltatore dell'evento riceve un'istanza di Acme\StoreBundle\Event\FilterOrderEvent.
         * 
         *
         * @var string
         */
        const onStoreOrder = 'negozio.ordine';
    }

Si noti che la class in realtà non fa nulla. Lo scopo della classe
``StoreEvents`` è solo quello di essere un posto in cui le informazioni sugli eventi
comuni possano essere centralizzate. Si noti che anche che una classe speciale
``FilterOrderEvent`` sarà passata a ogni ascoltatore di questo evento.

Creare un oggetto evento
........................

Più avanti, quando si distribuirà questo nuovo evento, si creerà un'istanza di ``Event``
e la si passerà al distributore. Il distributore quindi passa questa stessa istanza
a ciascuno degli ascoltatori dell'evento. Se non si ha bisogno di passare informazioni
agli ascoltatori, si può usare la classe predefinita
``Symfony\Component\EventDispatcher\Event``. Tuttavia, la maggior parte delle volte, si
avrà bisogno di passare informazioni sull'evento a ogni ascoltatore. Per poterlo fare,
si creerà una nuova classe, che estende
``Symfony\Component\EventDispatcher\Event``.

In questo esempio, ogni ascoltatore avrà bisogno di accedere a un qualche oggetto
``Order``. Creare una classe ``Event`` che lo renda possibile:

.. code-block:: php

    namespace Acme\StoreBundle\Event;

    use Symfony\Component\EventDispatcher\Event;
    use Acme\StoreBundle\Order;

    class FilterOrderEvent extends Event
    {
        protected $order;

        public function __construct(Order $order)
        {
            $this->order = $order;
        }

        public function getOrder()
        {
            return $this->order;
        }
    }

Ogni ascoltatore ora ha accesso all'oggetto ``Order``, tramite il metodo
``getOrder``.

Distribuire l'evento
....................

Il metodo :method:`Symfony\\Component\\EventDispatcher\\EventDispatcher::dispatch`
notifica a tutti gli ascoltatori l'evento dato. Accetta due parametri: il nome
dell'evento da distribuire e l'istanza di ``Event`` da passare a ogni ascoltatore
di tale evento:

.. code-block:: php

    use Acme\StoreBundle\StoreEvents;
    use Acme\StoreBundle\Order;
    use Acme\StoreBundle\Event\FilterOrderEvent;

    // l'ordine viene in qualche modo creato o recuperato
    $order = new Order();
    // ...

    // creare FilterOrderEvent e distribuirlo
    $event = new FilterOrderEvent($order);
    $dispatcher->dispatch(StoreEvents::onStoreOrder, $event);

Si noti che l'oggetto speciale ``FilterOrderEvent`` è creato e passato al
metodo ``dispatch``. Ora ogni ascoltatore dell'evento ``negozio.ordino`` riceverà
``FilterOrderEvent`` e avrà accesso all'oggetto ``Order``, tramite il metodo
``getOrder``:

.. code-block:: php

    // una qualche classe ascoltatore che è stata registrata per onStoreOrder
    use Acme\StoreBundle\Event\FilterOrderEvent;

    public function onStoreOrder(FilterOrderEvent $event)
    {
        $order = $event->getOrder();
        // fare qualcosa con l'ordine
    }

Passare l'oggetto distributore di eventi
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Se si dà un'occhiata alla classe ``EventDispatcher``, si noterà che non agisce come un
singleton (non c'è un metodo statico ``getInstance()``).
Questa cosa è voluta, perché si potrebbe avere necessità di diversi distributori di eventi
contemporanei in una singola richiesta PHP. Ma vuol dire anche che serve un modo per
passare il distributore agli oggetti che hanno bisogno di connettersi o notificare eventi.

Il modo migliore è iniettare l'oggetto distributore di eventi nei propri oggetti,
quindi usare la dependency injection.

Si può usare una constructor injection::

    class Foo
    {
        protected $dispatcher = null;

        public function __construct(EventDispatcher $dispatcher)
        {
            $this->dispatcher = $dispatcher;
        }
    }

Oppure una setter injection::

    class Foo
    {
        protected $dispatcher = null;

        public function setEventDispatcher(EventDispatcher $dispatcher)
        {
            $this->dispatcher = $dispatcher;
        }
    }

La scelta tra i due alla fine è una questione di gusti. Alcuni preferiscono la
constructor injection, perché gli oggetti sono inizializzati in pieno al momento
della costruzione. Ma, quando si ha una lunga lista di dipendenza, usare la
setter injection può essere il modo migliore, specialmente per le dipendenze opzionali.

.. tip::

    Se si usa la dependency injection come negli esempi sopra, si può usare il
    `componente Dependency Injection di Symfony2`_ per gestire questi oggetti
    in modo elegante.

        .. code-block:: yaml

            # src/Acme/HelloBundle/Resources/config/services.yml
            services:
                foo_service:
                    class: Acme/HelloBundle/Foo/FooService
                    arguments: [@event_dispatcher]

.. index::
   single: Distributore di eventi; Sottoscrittori

Usare i sottoscrittori
~~~~~~~~~~~~~~~~~~~~~~

Il modo più comune per ascoltare un evento è registrare un *ascoltatore* con il
distributore. Questo ascoltatore può ascoltare uno o più eventi e viene
notificato ogni volta che tali eventi sono distribuiti.

Un altro modo per ascoltare gli eventi è tramite un *sottoscrittore*. Un sottoscrittore
di eventi è una classe PHP che è in grado di dire al distributore esattamente quale
evento dovrebbe sottoscrivere. Implementa l'interfaccia
:class:`Symfony\\Component\\EventDispatcher\\EventSubscriberInterface`,
che richiede un unico metodo statico, chiamato ``getSubscribedEvents``.
Si consideri il seguente esempio di un sottoscrittore, che sottoscrive gli eventi
``kernel.response`` e ``negozio.ordine``:

.. code-block:: php

    namespace Acme\StoreBundle\Event;

    use Symfony\Component\EventDispatcher\EventSubscriberInterface;
    use Symfony\Component\HttpKernel\Event\FilterResponseEvent;

    class StoreSubscriber implements EventSubscriberInterface
    {
        static public function getSubscribedEvents()
        {
            return array(
                'kernel.response' => 'onKernelResponse',
                'negozio.ordine'  => 'onStoreOrder',
            );
        }

        public function onKernelResponse(FilterResponseEvent $event)
        {
            // ...
        }

        public function onStoreOrder(FilterOrderEvent $event)
        {
            // ...
        }
    }

È molto simile a una classe ascoltatore, tranne che la classe stessa può
dire al distributore quali eventi dovrebbe ascoltare. Per registrare un
sottoscrittore con il distributore, usare il metodo
:method:`Symfony\\Component\\EventDispatcher\\EventDispatcher::addSubscriber`
:

.. code-block:: php

    use Acme\StoreBundle\Event\StoreSubscriber;

    $subscriber = new StoreSubscriber();
    $dispatcher->addSubscriber($subscriber);

Il distributore registrerà automaticamente il sottoscrittore per ciascun evento
restituito dal metodo ``getSubscribedEvents``. Questo metodo restituisce un array
indicizzata per nomi di eventi e i cui valori sono o i nomi dei metodi da chiamare o
array composti dal nome del metodo e da una priorità.

.. tip::

    Se si usa il framework MVC Symfony2, si possono registrare sottoscrittori tramite
    la propria :ref:`configurazione <dic-tags-kernel-event-subscriber>`. Come bonus
    aggiuntivo, gli oggetti sottoscrittori sono istanziati solo quando servono.

.. index::
   single: Distributore di eventi; Bloccare il flusso degli eventi

.. _event_dispatcher-event-propagation:

Bloccare il flusso e la propagazione degli eventi
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

In alcuni casi, potrebbe aver senso che un ascoltatore prevenga il richiamo di qualsiasi
altro ascoltatore. In altre parole, l'ascoltatore deve poter essere in grado di dire al
distributore di bloccare ogni propagazione dell'evento a futuri ascoltatori (cioè di non
notificare più altri ascoltatori). Lo si può fare da dentro un ascoltatore, tramite il
metodo :method:`Symfony\\Component\\EventDispatcher\\Event::stopPropagation`:


.. code-block:: php

   use Acme\StoreBundle\Event\FilterOrderEvent;

   public function onStoreOrder(FilterOrderEvent $event)
   {
       // ...

       $event->stopPropagation();
   }

Ora, tutti gli ascoltatori di ``negozio.ordine`` che non sono ancora stati richiamati
*non* saranno richiamati.

.. index::
   single: Profiler

Profiler
--------

Se abilitato, il profiler di Symfony2 raccoglie informazioni utili su ogni richiesta
fatta alla propria applicazione e le memorizza per analisi successive. L'uso del
profiler in ambienti di sviluppo aiuta il debug del proprio codice e a migliorare le
prestazioni. Lo si può usare anche in ambienti di produzione, per approfondire i
problemi che si presentano.

Raramente si avrà a che fare direttamente con il profiler, visto che Symfony2 fornisce
strumenti di visualizzazione, come la barra di web debug e il profiler web. Se si usa
Symfony2 Standard Edition, il profiler, la barra di web debug e il profiler
web sono già configurati con impostazioni appropriate.

.. note::

    Il profiler raccoglie informazioni per tutte le richieste (richieste semplici,
    rinvii, eccezioni, richieste Ajax, richieste ESI) e per tutti i metodi e formati
    HTTP. Questo vuol dire che per un singolo URL si possono avere diversi dati di
    profile associati (uno per ogni coppia richiesta/risposta
    esterna).

.. index::
   single: Profiler; Visualizzazione

Visualizzare i dati di profile
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Usare la barra di web debug
...........................

In ambiente di sviluppo, la barra di web debug è disponibile in fondo a
ogni pagina. Essa mostra un buon riassunto dei dati di profile, che danno
accesso immediato a moltissime informazioni utili, quando qualcosa non
funziona come ci si aspetta.

Se il riassunto fornito dalla barra di web debug non basta, cliccare sul
collegamento del token (una stringa di 13 caratteri casuali) per accedere al profiler web.

.. note::

    Se il token non è cliccabile, vuol dire che le rotte del profiler non sono state
    registrate (vedere sotto per le informazioni sulla configurazione).

Analizzare i dati di profile con il profiler web
................................................

Il profiler web è uno strumento di visualizzazione per i dati di profile, che può
essere usato in sviluppo per il debug del codice e l'aumento delle prestazioni. Ma lo
si può anche usare per approfondire problemi occorsi in produzione. Espone tutte le
informazioni raccolte dal profiler in un'interfaccia web.

.. index::
   single: Profiler; Usare il servizio del profiler

Accedere alle informazioni di profile
.....................................

Non occorre usare il visualizzatore predefinito per accedere alle informazioni di
profile. Ma come si possono recuperare informazioni di profile per una specifica
richiesta, dopo che è accaduta? Quando il profiler memorizza i dati su una richiesta, vi
associa anche un token. Questo token è disponibile nell'header HTTP ``X-Debug-Token``
della risposta::

    $profile = $container->get('profiler')->loadProfileFromResponse($response);

    $profile = $container->get('profiler')->loadProfile($token);

.. tip::

    Quando il profiler è abiliato, ma non lo è la barra di web debug, oppure quando si
    vuole il token di una richiesta Ajax, usare uno strumento come Firebug per ottenere
    il valore dell'header HTTP ``X-Debug-Token``.

Usare il metodo ``find()`` per accedere ai token, in base a determinati criteri::

    // gli ultimi 10 token
    $tokens = $container->get('profiler')->find('', '', 10);

    // gli ultimi 10 token per URL che contengono /admin/
    $tokens = $container->get('profiler')->find('', '/admin/', 10);

    // gli ultimi 10 token per richieste locali
    $tokens = $container->get('profiler')->find('127.0.0.1', '', 10);

Se si vogliono manipolare i dati di profile su macchine diverse da quella che
ha generato le informazioni, usare i metodi ``export()`` e
``import()``::

    // sulla macchina di produzione
    $profile = $container->get('profiler')->loadProfile($token);
    $data = $profiler->export($profile);

    // sulla macchina di sviluppo
    $profiler->import($data);

.. index::
   single: Profiler; Visualizzare

Configurazione
..............

La configurazione predefinita di Symfony2 ha delle impostazioni adeguate per il
profiler, la barra di web debug e il profiler web. Ecco per esempio
la configurazione per l'ambiente di sviluppo:

.. configuration-block::

    .. code-block:: yaml

        # load the profiler
        framework:
            profiler: { only_exceptions: false }

        # enable the web profiler
        web_profiler:
            toolbar: true
            intercept_redirects: true
            verbose: true

    .. code-block:: xml

        <!-- xmlns:webprofiler="http://symfony.com/schema/dic/webprofiler" -->
        <!-- xsi:schemaLocation="http://symfony.com/schema/dic/webprofiler http://symfony.com/schema/dic/webprofiler/webprofiler-1.0.xsd"> -->

        <!-- load the profiler -->
        <framework:config>
            <framework:profiler only-exceptions="false" />
        </framework:config>

        <!-- enable the web profiler -->
        <webprofiler:config
            toolbar="true"
            intercept-redirects="true"
            verbose="true"
        />

    .. code-block:: php

        // carica il profiler
        $container->loadFromExtension('framework', array(
            'profiler' => array('only-exceptions' => false),
        ));

        // abilita il profiler web
        $container->loadFromExtension('web_profiler', array(
            'toolbar' => true,
            'intercept-redirects' => true,
            'verbose' => true,
        ));

Quando ``only-exceptions`` è impostato a ``true``, il profiler raccoglie dati solo
quando l'applicazione solleva un'eccezione.

Quando ``intercept-redirects`` è impostata ``true``, il profiler web intercetta i
rinvii e dà l'opportunità di guardare i dati raccolti, prima di seguire il
rinvio.

Quando ``verbose`` è impostato a ``true``, la barra di web debug mostra diverse
informazioni. L'impostazione ``verbose`` a ``false`` nasconde alcune informazioni
secondarie, per rendere la barra più corta.

Se si abilita il profiler web, occorre anche montare le rotte del profiler:

.. configuration-block::

    .. code-block:: yaml

        _profiler:
            resource: @WebProfilerBundle/Resources/config/routing/profiler.xml
            prefix:   /_profiler

    .. code-block:: xml

        <import resource="@WebProfilerBundle/Resources/config/routing/profiler.xml" prefix="/_profiler" />

    .. code-block:: php

        $collection->addCollection($loader->import("@WebProfilerBundle/Resources/config/routing/profiler.xml"), '/_profiler');

Poiché il profiler aggiunge un po' di sovraccarico, probabilmente lo si abiliterà solo
in alcune circostanze in ambiente di produzione. L'impostazione ``only-exceptions``
limita il profile alle pagine 500, ma che succede se si vogliono più informazioni quando
il client ha uno specifico indirizzo IP, oppure per una parte limitata del sito? Si
può usare un matcher della richiesta:

.. configuration-block::

    .. code-block:: yaml

        # abilita il profiler solo per richieste provenienti dalla rete 192.168.0.0
        framework:
            profiler:
                matcher: { ip: 192.168.0.0/24 }

        # abilita il profiler solo per gli URL /admin
        framework:
            profiler:
                matcher: { path: "^/admin/" }

        # combina le regole
        framework:
            profiler:
                matcher: { ip: 192.168.0.0/24, path: "^/admin/" }

        # usa un matcher personalizzato, definito nel servizio "custom_matcher"
        framework:
            profiler:
                matcher: { service: custom_matcher }

    .. code-block:: xml

        <!-- abilita il profiler solo per richieste provenienti dalla rete 192.168.0.0 -->
        <framework:config>
            <framework:profiler>
                <framework:matcher ip="192.168.0.0/24" />
            </framework:profiler>
        </framework:config>

        <!-- abilita il profiler solo per gli URL /admin -->
        <framework:config>
            <framework:profiler>
                <framework:matcher path="^/admin/" />
            </framework:profiler>
        </framework:config>

        <!-- combina le regole -->
        <framework:config>
            <framework:profiler>
                <framework:matcher ip="192.168.0.0/24" path="^/admin/" />
            </framework:profiler>
        </framework:config>

        <!-- usa un matcher personalizzato, definito nel servizio "custom_matcher" -->
        <framework:config>
            <framework:profiler>
                <framework:matcher service="custom_matcher" />
            </framework:profiler>
        </framework:config>

    .. code-block:: php

        // abilita il profiler solo per richieste provenienti dalla rete 192.168.0.0
        $container->loadFromExtension('framework', array(
            'profiler' => array(
                'matcher' => array('ip' => '192.168.0.0/24'),
            ),
        ));

        // abilita il profiler solo per gli URL /admin
        $container->loadFromExtension('framework', array(
            'profiler' => array(
                'matcher' => array('path' => '^/admin/'),
            ),
        ));

        // combina le regole
        $container->loadFromExtension('framework', array(
            'profiler' => array(
                'matcher' => array('ip' => '192.168.0.0/24', 'path' => '^/admin/'),
            ),
        ));

        # usa un matcher personalizzato, definito nel servizio "custom_matcher"
        $container->loadFromExtension('framework', array(
            'profiler' => array(
                'matcher' => array('service' => 'custom_matcher'),
            ),
        ));

Imparare di più dal ricettario
------------------------------

* :doc:`/cookbook/testing/profiling`
* :doc:`/cookbook/profiler/data_collector`
* :doc:`/cookbook/event_dispatcher/class_extension`
* :doc:`/cookbook/event_dispatcher/method_behavior`

.. _Observer: http://it.wikipedia.org/wiki/Observer_pattern
.. _`componente HttpKernel di Symfony2`: https://github.com/symfony/HttpKernel
.. _Closure: http://php.net/manual/en/functions.anonymous.php
.. _`componente Dependency Injection di Symfony2`: https://github.com/symfony/DependencyInjection
.. _funzione PHP: http://php.net/manual/en/language.pseudo-types.php#language.types.callback
