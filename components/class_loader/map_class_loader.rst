.. index::
    single: ClassLoader; MapClassLoader

MapClassLoader
==============

La classe :class:`Symfony\\Component\\ClassLoader\\MapClassLoader` consente di
auto-caricare file tramite una mappa statica, dalle classi ai file. È utile se si
usano librerie di terze parti, che non seguono lo standard `PSR-0`_ e quindi non
possono usare :doc:`class loader PSR-0 </components/class_loader/class_loader>`.

Si può usare ``MapClassLoader`` insieme a :doc:`class loader PSR-0 </components/class_loader/class_loader>`,
configurando e richiamando su entrambi il metodo ``register()``.

.. note::

    Il comportamento predefinito è di appendere ``MapClassLoader`` alla pila di
    auto-caricamento. Se lo si vuole usare come primo autoloader, passare ``true``
    al metodo ``register()``. In questo caso, il class loader sarà messo in cima
    alla pila di auto-caricamento.

Uso
---

È facile, basta passare la mappa al costruttore, quando si crea
un'istanza della classe ``MapClassLoader``::

    require_once '/path/to/src/Symfony/Component/ClassLoader/MapClassLoader';

    $mapping = array(
        'Pippo' => '/percorso/di/Pippo',
        'Pluto' => '/percorso/di/Pluto',
    );

    $loader = new MapClassLoader($mapping);

    $loader->register();

.. _PSR-0: http://www.php-fig.org/psr/psr-0/
