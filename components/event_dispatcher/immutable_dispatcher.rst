.. index::
    single: Event Dispatcher; Immutable

L'Event Dispatcher Immutable 
============================

.. versionadded:: 2.1
    Questa caratteristica è stata aggiunta in Symfony 2.1.

:class:`Symfony\\Component\\EventDispatcher\\ImmutableEventDispatcher` è un
distributore di eventi bloccato o congelato. Il distributore non può registrare nuovi
ascoltatori o sottoscrittori.

``ImmutableEventDispatcher`` accetta un altro distributore di eventi, con tutti gli
ascoltatori e i sottoscrittore. Il distributore immutabile è solo un proxy di tale
distributore originale.

Per poterlo usare, creare dapprima un distributore normale (``EventDispatcher`` o
``ContainerAwareEventDispatcher``) e registrare degli ascoltatori o dei
sottoscrittori::

    use Symfony\Component\EventDispatcher\EventDispatcher;

    $dispatcher = new EventDispatcher();
    $dispatcher->addListener('pippo.azione', function ($event) {
        // ...
    });

    // ...

Quindi, iniettarlo in un ``ImmutableEventDispatcher``::

    use Symfony\Component\EventDispatcher\ImmutableEventDispatcher;
    // ...

    $immutableDispatcher = new ImmutableEventDispatcher($dispatcher);

Si dovrà usare tale nuovo distributore nel proprio progetto.

Se si prova a eseguire uno dei metodi che modificano il distributore
(p.e. ``addListener``), verrà lanciata una ``BadMethodCallException``.
