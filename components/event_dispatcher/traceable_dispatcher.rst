.. index::
    single: EventDispatcher; Debug
    single: EventDispatcher; Traceable

Distributore di eventi tracciabile
==================================

.. versionadded:: 2.5
    La classe ``TraceableEventDispatcher`` è stata spostata nel componente EventDispatcher
    in Symfony 2.5. In precedenza, si trovava nel componente HttpKernel.

:class:`Symfony\\Component\\EventDispatcher\\Debug\\TraceableEventDispatcher`
è un distributore di eventi che avvolge ogni altro distributore di eventi e può quindi
essere usato per determinare quale ascoltatori di eventi siano stati richiamati dal distributore.
Passare il distributore di eventi da avvolgere e un'istanza di
:class:`Symfony\\Component\\Stopwatch\\Stopwatch` al suo costruttore::

    use Symfony\Component\EventDispatcher\Debug\TraceableEventDispatcher;
    use Symfony\Component\Stopwatch\Stopwatch;

    // distributore di eventi su cui fare debug
    $eventDispatcher = ...;

    $traceableEventDispatcher = new TraceableEventDispatcher(
        $eventDispatcher,
        new Stopwatch()
    );

Si può quindi usare ``TraceableEventDispatcher`` come qualsiasi altro distributore di eventi,
per registrare ascoltatori di eventi e distribuire eventi::

    // ...

    // registrare un ascoltatore di eventi
    $eventListener = ...;
    $priority = ...;
    $traceableEventDispatcher->addListener('nome-evento, $eventListener, $priority);

    // distribuire un evento
    $event = ...;
    $traceableEventDispatcher->dispatch('nome-evento', $event);

Dopo che un'applicazione è stata processata, si può usare il metodo
:method:`Symfony\\Component\\EventDispatcher\\Debug\\TraceableEventDispatcherInterface::getCalledListeners`
per recuperare un array di ascoltatori di eventi che sono stati richiamati
nell'applicazione. In modo simile il metodo
:method:`Symfony\\Component\\EventDispatcher\\Debug\\TraceableEventDispatcherInterface::getNotCalledListeners`
restituisce un array di ascoltatori di eventi che non sono stati chiamati::

    // ...

    $calledListeners = $traceableEventDispatcher->getCalledListeners();
    $notCalledListeners = $traceableEventDispatcher->getNotCalledListeners();
