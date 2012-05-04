.. index::
   single: Event Dispatcher

Oggetto evento generico
=======================

.. versionadded:: 2.1
    La classe ``GenericEvent`` è stata aggiunta in Symfony 2.1

La classe base :class:`Symfony\\Component\\EventDispatcher\\Event` fornita dal
componente ``Event Dispatcher`` è deliberatamente breve, per consentire la creazione
di oggetti evento con API specifiche, usando l'ereditarietà. Questo consente un codice
elegante e leggibile, anche in applicazioni complesse.

La classe :class:`Symfony\\Component\\EventDispatcher\\GenericEvent` è disponibile
per comodità per chi volesse usare un solo oggetto evento in tutta la propria
applicazione. È adatta alla maggior parte degli scopi, senza modifiche, perché segue
il pattern observer standard, in cui gli oggetti evento incapsulano il soggetto ("subject")
di un evento, ma anche alcuni parametri in
più.

:class:`Symfony\\Component\\EventDispatcher\\GenericEvent` ha una semplice API, in
aggiunta alla classe base :class:`Symfony\\Component\\EventDispatcher\\Event`

* :method:`Symfony\\Component\\EventDispatcher\\GenericEvent::__construct`:
  il costruttore accetta il soggetto dell'evento e qualsiasi parametro;

* :method:`Symfony\\Component\\EventDispatcher\\GenericEvent::getSubject`:
  restituisce il soggetto;

* :method:`Symfony\\Component\\EventDispatcher\\GenericEvent::setArg`:
  imposta un parametro per chiave;

* :method:`Symfony\\Component\\EventDispatcher\\GenericEvent::setArgs`:
  imposta un array di parametri;

* :method:`Symfony\\Component\\EventDispatcher\\GenericEvent::getArg`:
  restituisce un parametro per chiave;

* :method:`Symfony\\Component\\EventDispatcher\\GenericEvent::getArgs`:
  restituisce un array di parametri;

* :method:`Symfony\\Component\\EventDispatcher\\GenericEvent::hasArg`:
  restituisce ``true`` se il parametro esiste;

``GenericEvent`` implementa anche :phpclass:`ArrayAccess` sui parametri dell'evento,
il che lo rende molto utile per passare parametri ulteriori, che riguardino il soggetto
dell'evento.

Gli esempi seguenti mostrano dei casi d'uso, che danno un'idea generale della flessibilità.
Gli esempi presumono che gli ascoltatori siano stati aggiunti al distributore di eventi.

Passare semplicemente un soggetto::

    use Symfony\Component\EventDispatcher\GenericEvent;

    $event = GenericEvent($subject);
    $dispatcher->dispatch('pippo', $event);

    class PippoListener
    {
        public function handler(GenericEvent $event)
        {
            if ($event->getSubject() instanceof Pippo) {
                // ...
            }
        }
    }

Passare e processare parametri usando l'API :phpclass:`ArrayAccess` per accedere ai
parametri dell'evento::

    use Symfony\Component\EventDispatcher\GenericEvent;

    $event = new GenericEvent($subject, array('type' => 'pippo', 'counter' => 0)));
    $dispatcher->dispatch('pippo', $event);

    echo $event['counter'];

    class PippoListener
    {
        public function handler(GenericEvent $event)
        {
            if (isset($event['type']) && $event['type'] === 'pippo') {
                // ... fare qualcosa
            }

            $event['counter']++;
        }
    }

Filtrare i dati::

    use Symfony\Component\EventDispatcher\GenericEvent;

    $event = new GenericEvent($subject, array('data' => 'pippo'));
    $dispatcher->dispatch('pippo', $event);

    echo $event['data'];

    class PippoListener
    {
        public function filter(GenericEvent $event)
        {
            strtolower($event['data']);
        }
    }
