.. index::
   single: Event Dispatcher; Ricette

Ricette per Event Dispatcher
============================

Distribuire l'oggetto Event Dispatcher
--------------------------------------

Nel caso in cui si fosse data un'occhiata alla classe ``EventDispatcher`` ci si
sarà resi conto che la classe non agisce come un Singleton (non c'è alcun
metodo statico ``getInstance()``).
Questa è una cosa intenzionale dato che si vorrebbe poter utilizzare diversi
event dispatcher concorrenti in una singola richiesta PHP. Ma questo significa
aver bisogno di un modo per passare il dispatcher agli oggetti che necessitano
di esservi connessi o che devono notificare eventi.

La best practice è quella di iniettare l'oggetto event dispatcher negli oggetti,
si parla di dependency injection.

Si può sfruttare l'injection tramite costrutture::

    class Foo
    {
        protected $dispatcher = null;

        public function __construct(EventDispatcher $dispatcher)
        {
            $this->dispatcher = $dispatcher;
        }
    }

O la setter injection::

    class Foo
    {
        protected $dispatcher = null;

        public function setEventDispatcher(EventDispatcher $dispatcher)
        {
            $this->dispatcher = $dispatcher;
        }
    }

Scegliere tra le due è davvero questione di gusti. Personalmente preferisco
l'iniezione da costruttore dato che gli oggetti vengono completamente 
inizializzati al momento della costruzione. Ma quando si ha una lunga lista
di dipendenze l'utilizzo dell'iniezione via setter può essere la strada da
seguire specialmente per le dipendenze opzionali.

.. tip::
   Se si utilizza la dependency injection come nei due esempi precedenti si
   potrà utilizzare facilmente il componente Symfony2 Dependency Injection
   per gestire in modo elegante questi oggetti.

Fare qualcosa prima o dopo l'invocazione di un metodo
-----------------------------------------------------

Se si vuole fare qualcosa subito prima, o subito dopo, la chiamata ad un metodo
è possibile notificare rispettivamente un evento all'inizio o alla fine del
metodo::

    class Foo
    {
        // ...

        public function send($foo, $bar)
        {
            // fai qualcosa prima del metodo
            $event = new Event($this, 'foo.do_before_send', array('foo' => $foo, 'bar' => $bar));
            $this->dispatcher->notify($event);

            // la reale implementazione del metodo è qui
            // $ret = ...;

            // fai qualcosa dopo il metodo
            $event = new Event($this, 'foo.do_after_send', array('ret' => $ret));
            $this->dispatcher->notify($event);

            return $ret;
        }
    }

Aggiungere metodi ad una classe
-------------------------------

Per permettere a più classi di aggiungere metodi ad un'altra si può definire
il metodo magico ``__call()`` nella classe che si vuole estendere in questo
modo::

    class Foo
    {
        // ...

        public function __call($method, $arguments)
        {
            // creare un evento chiamato 'foo.method_is_not_found'
            // e passare il nome del metodo ed i parametri passati a questo metodo
            $event = new Event($this, 'foo.method_is_not_found', array('method' => $method, 'arguments' => $arguments));

            // chiama tutti gli ascoltatori fino a che uno sia in grado di implementare $method
            $this->dispatcher->notifyUntil($event);

            // nessun ascoltatore è stato in grado di processare l'evento? Il metodo non esiste
            if (!$event->isProcessed()) {
                throw new \Exception(sprintf('Call to undefined method %s::%s.', get_class($this), $method));
            }

            // restituire il valore restituito dall'ascoltatore
            return $event->getReturnValue();
        }
    }

Poi creare la classe che conterrà l'ascoltatore::

    class Bar
    {
        public function addBarMethodToFoo(Event $event)
        {
            // vogliamo solo rispondere alle chiamate al metodo 'bar'
            if ('bar' != $event['method']) {
                // permettere ad un altro ascoltatore di occuparsi di questo metodo sconosciuto
                return false;
            }

            // il soggetto dell'oggetto (istanza foo)
            $foo = $event->getSubject();

            // i parametri del metodo bar
            $arguments = $event['parameters'];

            // fai qualcosa
            // ...

            // imposta il valore di ritorno
            $event->setReturnValue($someValue);

            // racconta a tutti che l'evento è stato processato
            return true;
        }
    }

Infine aggiungere il nuovo metodo ``bar`` alla classe ``Foo``::

    $dispatcher->connect('foo.method_is_not_found', array($bar, 'addBarMethodToFoo'));

Modificare parametri
--------------------

Se si volesse permettere a classe di terze parti di modificare i parametri passati
ad un metodo subito prima dell'esecuzione del metodo aggiungere un evento ``filter``
all'inizio del metodo::

    class Foo
    {
        // ...

        public function render($template, $arguments = array())
        {
            // filtra i parametri
            $event = new Event($this, 'foo.filter_arguments');
            $this->dispatcher->filter($event, $arguments);

            // prende i parametri filtrati
            $arguments = $event->getReturnValue();
            // il metodo inizia qui
        }
    }

Di seguito un esempio per il filter::

    class Bar
    {
        public function filterFooArguments(Event $event, $arguments)
        {
            $arguments['processed'] = true;

            return $arguments;
        }
    }
