.. index::
   single: Dispatcher di eventi

Come estendere una classe senza usare l'ereditarietà
====================================================

Per consentire a molte classi di aggiungere metodi a un'altra classe, si può
definire il metodo magico ``__call()`` nella classe che si vuole estendere, in questo modo:

.. code-block:: php

    class Pippo
    {
        // ...

        public function __call($method, $arguments)
        {
            // crea un evento chiamato 'pippo.metodo_non_trovato'
            $event = new HandleUndefinedMethodEvent($this, $method, $arguments);
            $this->dispatcher->dispatch($this, 'pippo.metodo_non_trovato', $event);

            // nessun ascoltatore ha potuto processare l'evento? Il metodo non esiste
            if (!$event->isProcessed()) {
                throw new \Exception(sprintf('Metodo non definito %s::%s.', get_class($this), $method));
            }

            // restituisce all'ascoltatore il valore restituito
            return $event->getReturnValue();
        }
    }

Qui viene usato una classe speciale ``HandleUndefinedMethodEvent``, che va creata.
È una classe generica che potrebbe essere riusata ogni volta che si ha bisogno di
questo tipo di estensione di classe:

.. code-block:: php

    use Symfony\Component\EventDispatcher\Event;

    class HandleUndefinedMethodEvent extends Event
    {
        protected $subject;
        protected $method;
        protected $arguments;
        protected $returnValue;
        protected $isProcessed = false;

        public function __construct($subject, $method, $arguments)
        {
            $this->subject = $subject;
            $this->method = $method;
            $this->arguments = $arguments;
        }

        public function getSubject()
        {
            return $this->subject;
        }

        public function getMethod()
        {
            return $this->method;
        }

        public function getArguments()
        {
            return $this->arguments;
        }

        /**
         * Imposta il valore da restituire e ferma le notifiche agli altri ascoltatori
         */
        public function setReturnValue($val)
        {
            $this->returnValue = $val;
            $this->isProcessed = true;
            $this->stopPropagation();
        }

        public function getReturnValue($val)
        {
            return $this->returnValue;
        }

        public function isProcessed()
        {
            return $this->isProcessed;
        }
    }

Quindi, creare una classe che ascolterà l'evento ``pippo.metodo_non_trovato`` e
*aggiungere* il metodo ``pluto()``:

.. code-block:: php

    class Pluto
    {
        public function onPippoMethodIsNotFound(HandleUndefinedMethodEvent $event)
        {
            // vogliamo rispondere solo alle chiamate al metodo 'pluto'
            if ('pluto' != $event->getMethod()) {
                // consente agli altri ascoltatori di prendersi cura di questo metodo sconosciuto
                return;
            }

            // l'oggetto in questione (l'istanza di Pippo)
            $pippo = $event->getSubject();

            // i parametri del metodo 'pluto'
            $arguments = $event->getArguments();

            // fare qualcosa
            // ...

            // impostare il valore restituito
            $event->setReturnValue($someValue);
        }
    }

Infine, aggiungere il nuovo metodo ``pluto`` alla classe ``Pippo``, registrando un'istanza
di ``Pluto`` con l'evento ``pippo.metodo_non_trovato``:

.. code-block:: php

    $pluto = new Pluto();
    $dispatcher->addListener('pippo.metodo_non_trovato', $pluto);
