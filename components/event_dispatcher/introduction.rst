.. index::
   single: Event Dispatcher
   single: Componenti; EventDispatcher

Il componente Event Dispatcher
==============================

Introduzione
------------

L'approccio orientato agli oggetti da tempo assicura estensibilità del codice. Creando
classi con responsabilità ben definite, il codice diventa più flessibile, consentendo allo
sviluppatore di estenderlo con sotto-classi, che ne modifichino il comportamento. Se
tuttavia si vogliono condividere le proprie modifiche con altri sviluppatori, che a loro
volta abbiano costruito le proprie sotto-classi, l'ereditarietà non è più la risposta giusta.

Consideriamo un esempio concreato, in cui si voglia fornire un sistema di plugin per un
progetto. Un plugin dovrebbe essere in grado di aggiungere metodi o di fare qualcosa prima
o dopo che un metodo sia eseguito, senza interferire con altri plugin. Questo problema non
si risolve facilmente con l'ereditarietà singola, mentre l'ereditarietà multipla
(dove sia possibile con PHP) ha i suoi difetti.

Il componente Event Dispatcher di Symfony2 implementa il pattern `Observer`_ in modo
semplice ed efficace, per rendere possibile tutto questo e per rendere un progetto
veramente estensibile.

Prendiamo un semplice esempio dal `componente HttpKernel di Symfony2`_. Una volta creato
un oggetto ``Response``, può essere utile consentirne la modifica ad altri elementi del
sistema (p.e. aggiungere header di cache) prima del suo utilizzo effettivo.
Per poterlo fare, il kernel di Symfony2 lancia un evento,
``kernel.response``. Ecco come funziona:

* Un *ascoltatore* (oggetto PHP) dice a un oggetto *distributore* centrale che vuole
  ascoltare l'evento ``kernel.response``;

* A un certo punto, il kernel di Symfony2 dice all'oggetto *distributore* di distribuire
  l'evento ``kernel.response``, passando un oggetto ``Event``, che ha accesso
  all'oggetto ``Response``;

* Il distributore notifica a (ovvero chiama un metodo su) tutti gli ascoltatori
  dell'evento ``kernel.response``, consentendo a ciascuno di essi di modificare
  l'oggetto ``Response``.

.. index::
   single: Event Dispatcher; Eventi

Installazione
-------------

Si può installare il componente in molti modi diversi:

* Usare il repository ufficiale su Git (https://github.com/symfony/EventDispatcher);
* Installarlo via PEAR (`pear.symfony.com/EventDispatcher`);
* Installarlo via Composer (`symfony/event-dispatcher` su Packagist).

Uso
---

Eventi
~~~~~~

Quando un evento viene distribuito, è identificato da un nome univoco (p.e.
``kernel.response``), che può essere ascoltato da un numero qualsiasi di ascoltatori.
Inoltre un'istanza di :class:`Symfony\\Component\\EventDispatcher\\Event`  viene creata
e passata a tutti gli ascoltatori. Come vedremo più aventi, l'oggetto ``Event`` stesso
spesso contiene dei dati sull'evento distribuito.

.. index::
   pair: Event Dispatcher; Convenzioni sui nomi

Convenzioni sui nomi
....................

Il nome univoco dell'evento può essere una stringa qualsiasi, ma segue facoltativamente
alcune semplici convenzioni di nomenclatura:

* usa solo lettere minuscole, numeri, punti (``.``) e trattini bassi (``_``);

* ha un pressi con uno spazio dei nomi, seguito da un punto (p.e. ``kernel.``);

* termina con un verbo, che indica l'azione intrapresa (p.e.
  ``request``).

Ecco alcuni buoni esempi di nomi di eventi:

* ``kernel.response``
* ``form.pre_set_data``

.. index::
   single: Event Dispatcher; Sotto-classi di eventi

Nomi di eventi e oggetti Event
..............................

Quando il distributore notifica gli ascoltatori, passa loro un oggetto ``Event``.
La classe base ``Event`` è molto semplice: contiene un metodo per fermare la
:ref:`propagazione degli eventi<event_dispatcher-event-propagation>`, non molto
di più.

Spesso, i dati su uno specifico evento devono essere passati insieme all'oggetto
``Event``, in modo che gli ascoltatori ottengano le informazioni necessarie. Nel caso
dell'evento ``kernel.response``, l'oggetto ``Event`` creato e passato a ciascun
ascoltatore è in effetti di tipo
:class:`Symfony\\Component\\HttpKernel\\Event\\FilterResponseEvent`, una sotto-classe
dell'oggetto base ``Event``. Questa classe contiene metodi come
``getResponse`` e ``setResponse``, che consentono agli ascoltatori di ottenere, o anche
sostituire, l'oggetto ``Response``.

La morale della favola è questa: quando si crea un ascoltatore per un evento, l'oggetto
``Event`` passato all'ascoltatore può essere una speciale sotto-classe, con metodi
aggiuntivi per recuperare informazioni dall'evento e per rispondere
all'evento.

Il distributore
~~~~~~~~~~~~~~~

Il distributore è l'oggetto centrale del sistema di distribuzione degli eventi.
In generale, viene creato un solo distributore, che mantiene un registro di
ascoltatori. Quando un evento viene distribuito dal distributore, esso notifica a tutti
gli ascoltatori registrati a tale evento.:

    use Symfony\Component\EventDispatcher\EventDispatcher;

    $dispatcher = new EventDispatcher();

.. index::
   single: Event Dispatcher; Ascoltatori

Connettere gli ascoltatori
~~~~~~~~~~~~~~~~~~~~~~~~~~

Per sfruttare un evento esistente, occorre connettere un ascoltatore al distributore,
in modo che riceva una notifica quando l'evento viene distribuito. Una chiamata al
metodo ``addListener()`` del distributore associa un qualsiasi callable PHP a un
evento::

    $listener = new AcmeListener();
    $dispatcher->addListener('pippo.action', array($listener, 'onPippoAction'));

Il metodo ``addListener()`` accetta fino a tre parametri:

* Il nome dell'evento (stringa) che questo ascoltatore vuole ascoltare;

* Un callabel PHP, che sarà notificato quando viene lanciato un evento che sta
  ascoltando;

* Un intero opzionale di priorità (più alto equivale a più importante), che determina
  quando far scattare un ascoltatore, rispetto ad altri (predefinito a ``0``). Se due
  ascoltatori hanno la medesima priorità, sono eseguiti nell'ordine in cui sono stati
  aggiunti al distributore.

.. note::

    Un `callable PHP`_ è una variabile PHP che possa essere usata dalla funzione
    ``call_user_func()`` e che restituisca ``true`` se passata alla funzione
    ``is_callable()``. Può essere un'istanza di ``\Closure``, un oggetto che implementi
    un metodo ``__invoke`` (che è ciò che in effetti sono le closure), una stringa
    che rappresenti una funzione, o infine un array che rappresenti il metodo di un oggetto
    o di una classe.

    Finora, abbiamo visto che oggetti PHP possano essere registrati come ascoltatori.
    Si possono anche registrare `Closure`_ PHP come ascoltatori di eventi::

        use Symfony\Component\EventDispatcher\Event;

        $dispatcher->addListener('pipo.action', function (Event $event) {
            // sarà eseguito quando l'evento pippo.action sarà distribuito
        });

Una volta registrato un evento sul distributore, esso aspetterà finché l'evento non
sarà notificato. Nell'esempio precedente, quando l'evento ``pippo.action`` viene
distribuito, il distributore richiama il metodo ``AcmeListener::onPippoAction`` e passa
l'oggetto ``Event`` come singolo parametro::

    use Symfony\Component\EventDispatcher\Event;

    class AcmeListener
    {
        // ...

        public function onPippoAction(Event $event)
        {
            // fa qualcosa
        }
    }

In molti casi, viene passata all'ascoltatore una speciale sotto-classe ``Event``, che
è specifica dell'evento dato. Questo dà accesso all'ascoltatore a informazioni speciali
sull'evento. Leggere la documentazione o l'implementazione di ciascun evento, per
determinare l'esatta istanza ``Symfony\Component\EventDispatcher\Event``
passata. Per esempio, l'evento ``kernel.event`` passa un'istanza di
``Symfony\Component\HttpKernel\Event\FilterResponseEvent``::

    use Symfony\Component\HttpKernel\Event\FilterResponseEvent

    public function onKernelResponse(FilterResponseEvent $event)
    {
        $response = $event->getResponse();
        $request = $event->getRequest();

        // ...
    }

.. _event_dispatcher-closures-as-listeners:

.. index::
   single: Event Dispatcher; Creare e distribuire un evento

Creare e distribuire un evento
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Oltre a registrare ascoltatori con eventi esistenti, si possono creare e distribuire
i propri eventi. Questo è utile quando si creano librerie di terze parti e anche
quando si vogliono mantenere i vari componenti dei propri sistemi flessibili e
disaccoppiati.

La classe statica ``Events``
............................

Si supponga di voler creare un nuovo evento, chiamato ``negozio.ordine``, distribuito
ogni volta che un ordine viene creato dentro la propria applicazione. Per mantenere le
cose organizzate, iniziamo a creare una classe ``StoreEvents`` all'interno della propria
applicazione, che serve a definire e documentare il proprio evento::

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
``Order``. Creare una classe ``Event`` che lo renda possibile::

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
di tale evento::

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
``getOrder``::

    // una qualche classe ascoltatore che è stata registrata per onStoreOrder
    use Acme\StoreBundle\Event\FilterOrderEvent;

    public function onStoreOrder(FilterOrderEvent $event)
    {
        $order = $event->getOrder();
        // fare qualcosa con l'ordine
    }

.. index::
   single: Event Dispatcher; Sottoscrittori

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
``kernel.response`` e ``negozio.ordine``::

    namespace Acme\StoreBundle\Event;

    use Symfony\Component\EventDispatcher\EventSubscriberInterface;
    use Symfony\Component\HttpKernel\Event\FilterResponseEvent;

    class StoreSubscriber implements EventSubscriberInterface
    {
        static public function getSubscribedEvents()
        {
            return array(
                'kernel.response' => array(
                    array('onKernelResponsePre', 10),
                    array('onKernelResponseMid', 5),
                    array('onKernelResponsePost', 0),
                ),
                'negozio.ordine'  => array('onStoreOrder', 0),
            );
        }

        public function onKernelResponsePre(FilterResponseEvent $event)
        {
            // ...
        }

        public function onKernelResponseMid(FilterResponseEvent $event)
        {
            // ...
        }

        public function onKernelResponsePost(FilterResponseEvent $event)
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
::

    use Acme\StoreBundle\Event\StoreSubscriber;

    $subscriber = new StoreSubscriber();
    $dispatcher->addSubscriber($subscriber);

Il distributore registrerà automaticamente il sottoscrittore per ciascun evento
restituito dal metodo ``getSubscribedEvents``. Questo metodo restituisce un array
indicizzata per nomi di eventi e i cui valori sono o i nomi dei metodi da chiamare o
array composti dal nome del metodo e da una priorità. L'esempio precedentemostra come
registrare diversi metodi ascoltatori per lo stesso evento in un sottoscrittore e mostra
anche come passare una priorità a ciascun metodo ascoltatore.

.. index::
   single: Event Dispatcher; Bloccare il flusso degli eventi

.. _event_dispatcher-event-propagation:

Bloccare il flusso e la propagazione degli eventi
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

In alcuni casi, potrebbe aver senso che un ascoltatore prevenga il richiamo di qualsiasi
altro ascoltatore. In altre parole, l'ascoltatore deve poter essere in grado di dire al
distributore di bloccare ogni propagazione dell'evento a futuri ascoltatori (cioè di non
notificare più altri ascoltatori). Lo si può fare da dentro un ascoltatore, tramite il
metodo :method:`Symfony\\Component\\EventDispatcher\\Event::stopPropagation`
::

   use Acme\StoreBundle\Event\FilterOrderEvent;

   public function onStoreOrder(FilterOrderEvent $event)
   {
       // ...

       $event->stopPropagation();
   }

Ora, tutti gli ascoltatori di ``negozio.ordine`` che non sono ancora stati richiamati
*non* saranno richiamati.

Si può indidividuare se un evento è stato fermato, usando il metodo
:method:`Symfony\\Component\\EventDispatcher\\Event::isStoppedPropagation`,
che restituisce un booleano::

    $dispatcher->dispatch('foo.event', $event);
    if ($event->isStoppedPropagation()) {
        // ...
    }

.. index::
   single: Event Dispatcher; Eventi e ascoltatori consapevoli del distributore

.. _event_dispatcher-dispatcher-aware-events:

Eventi e ascolatori consapevoli del distributore
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

.. versionadded:: 2.1
    L'oggetto ``Event`` contiene un riferimento al distributore che lo invoca da Symfony 2.1

``EventDispatcher`` inietta sempre un riferimento a sé stesso nell'evento passato.
Questo vuol dire che tutti gli ascoltatori hanno accesso diretto all'oggetto
``EventDispatcher`` notificante, tramite il metodo
:method:`Symfony\\Component\\EventDispatcher\\Event::getDispatcher`
dell'oggetto ``Event`` passat .

Questo può portare ad applicazioni avanzate per ``EventDispatcher``, incluse la
possibilità per gli ascoltatori di distribuire altri eventi, il concatenamento degli eventi o anche il
caricamento pigro di più ascoltatori nell'oggetto distributore. Ecco degli esempi:

Caricamento pigro degli ascoltatori::

    use Symfony\Component\EventDispatcher\Event;
    use Acme\StoreBundle\Event\StoreSubscriber;

    class Foo
    {
        private $started = false;

        public function myLazyListener(Event $event)
        {
            if (false === $this->started) {
                $subscriber = new StoreSubscriber();
                $event->getDispatcher()->addSubscriber($subscriber);
            }

            $this->started = true;

            // ... eccetera
        }
    }

Distribuzione di altri eventi da dentro un ascoltatore::

    use Symfony\Component\EventDispatcher\Event;

    class Foo
    {
        public function myFooListener(Event $event)
        {
            $event->getDispatcher()->dispatch('log', $event);

            // ... eccetera
        }
    }

Questo è sufficiente per la maggior parte dei casi, ma, se si ha un'applicazione con
istanze multiple di ``EventDispatcher``, potrebbe essere necessario iniettare specificatamente un'istanza nota
di ``EventDispatcher`` nei propri ascoltatori. Questo è possibile tramite l'utilizzo
dell'iniezione per costruttore o per setter, come segue:

Iniezione per costruttore::

    use Symfony\Component\EventDispatcher\EventDispatcherInterface;

    class Foo
    {
        protected $dispatcher = null;

        public function __construct(EventDispatcherInterface $dispatcher)
        {
            $this->dispatcher = $dispatcher;
        }
    }

Iniezione per setter::

    use Symfony\Component\EventDispatcher\EventDispatcherInterface;

    class Foo
    {
        protected $dispatcher = null;

        public function setEventDispatcher(EventDispatcherInterface $dispatcher)
        {
            $this->dispatcher = $dispatcher;
        }
    }

La scelta tra i due è una questione di gusti. Molti preferiscono l'iniezione per
costruttore, perché l'oggetto in questo modo viene inizializzato durante la
costruzione. Ma quando si ha una lunga lista di dipendenze, l'utilizzo dell'iniezione
per settere può essere l'unico modo, specialmente per dipendenze opzionali.

.. index::
   single: Event Dispatcher; Scorciatoie del distributore

.. _event_dispatcher-shortcuts:

Scorciatoie del distributore
~~~~~~~~~~~~~~~~~~~~~~~~~~~~

.. versionadded:: 2.1
    Il metodo ``EventDispatcher::dispatch()`` restituisce l'evento da Symfony 2.1.

Il metodo :method:`EventDispatcher::dispatch<Symfony\\Component\\EventDispatcher\\EventDispatcher::dispatch>`
restiuisce sempre un oggetto :class:`Symfony\\Component\\EventDispatcher\\Event`.
Questo conente diverse scorciatoie. Per esempio, se non si ha bisogno di un oggetto
evento personalizzato, ci si può appoggiare semplicemente su un oggetto
:class:`Symfony\\Component\\EventDispatcher\\Event`. Non occorre nemmeno
passarlo al distributore, perché ne sarà creato uno per impostazione predefinita, a meno  che
non venga passato specificatamente::

    $dispatcher->dispatch('foo.event');

Inoltre, ``EventDispatcher`` restituisce sempre quale oggetto evento è stato
distribuito, cioè o l'evento passato o l'evento creato internamente dal
distributore. Questo consente utili scorciatoie::

    if (!$dispatcher->dispatch('foo.event')->isStoppedPropagation()) {
        // ...
    }

Oppure::

    $barEvent = new BarEvent();
    $bar = $dispatcher->dispatch('bar.event', $barEvent)->getBar();

Oppure::

    $response = $dispatcher->dispatch('bar.event', new BarEvent())->getBar();

e così via...

.. index::
   single: Event Dispatcher; Introspezione del nome dell'evento

.. _event_dispatcher-event-name-introspection:

Introspezione del nome dell'evento
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

.. versionadded:: 2.1
    Aggiunto il nome dell'evento all'oggetto ``Event`` da Symfony 2.1

Poiché ``EventDispatcher`` conosce già il nome dell'evento al momento della distribuzione,
il nome dell'evento è iniettato anche negli oggetti
:class:`Symfony\\Component\\EventDispatcher\\Event`, quindi è disponibile agli
ascoltatori dell'evento, tramite il metodo
:method:`Symfony\\Component\\EventDispatcher\\Event::getName`.

Il nome dell'evento (come ogni altro dato in un oggetto evento personalizzato) può essere
usato come parte della logica di processamento dell'ascoltatore::

    use Symfony\Component\EventDispatcher\Event;

    class Foo
    {
        public function myEventListener(Event $event)
        {
            echo $event->getName();
        }
    }

.. _Observer: http://en.wikipedia.org/wiki/Observer_pattern
.. _`componente HttpKernel di Symfony2`: https://github.com/symfony/HttpKernel
.. _Closure: http://php.net/manual/en/functions.anonymous.php
.. _callable PHP: http://www.php.net/manual/en/language.pseudo-types.php#language.types.callback
