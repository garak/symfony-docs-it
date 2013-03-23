.. index::
   single: Dispatcher di eventi

Come personalizzare il comportamento di un metodo senza usare l'ereditarietà
============================================================================

Fare qualcosa prima o dopo la chiamata a un metodo
--------------------------------------------------

Se si vuole fare qualcosa subito prima o subito dopo che un metodo sia chiamato,
si può inviare un evento rispettivamente all'inizio o alla fine del
metodo::

    class Pippo
    {
        // ...

        public function send($foo, $bar)
        {
            // fa qualcosa prima del metodo
            $event = new FilterBeforeSendEvent($foo, $bar);
            $this->dispatcher->dispatch('foo.pre_send', $event);

            // prende $foo e $bar dall'evento, potrebbero essere stati modificati
            $foo = $event->getFoo();
            $bar = $event->getBar();

            // la vera implementazione del metodo è qui
            // $ret = ...;

            // fare qualcosa dopo il metodo
            $event = new FilterSendReturnValue($ret);
            $this->dispatcher->dispatch('foo.post_send', $event);

            return $event->getReturnValue();
        }
    }

In questo esempio, vengono lanciati due eventi: ``foo.pre_send``, prima che il metodo
sia eseguito, e ``foo.post_send``, dopo che il metodo è eseguito. Ciascuno usa una
classe Event personalizzata per comunicare informazioni agli ascoltatori di questi
due eventi. Queste classi evento andrebbero create dallo sviluppatore e dovrebbero
consentire, in questo esempio, alle variabili ``$foo``, ``$bar`` e ``$ret`` di essere
recuperate e impostate dagli ascoltatori.

Per esempio, ipotizziamo che ``FilterSendReturnValue`` abbia un metodo ``setReturnValue``.
un ascoltatore potrebbe assomigliare a questo:

.. code-block:: php

    public function onFooPostSend(FilterSendReturnValue $event)
    {
        $ret = $event->getReturnValue();
        // modifica il valore originario di $ret

        $event->setReturnValue($ret);
    }
