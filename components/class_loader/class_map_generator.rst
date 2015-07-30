.. index::
    single: Autoloading; Generatore di classi di mappe
    single: ClassLoader; Generatore di classi di mappe

Generatore di classi di mappe
=============================

Il caricamento di una classe è solitamente facile, con gli standard `PSR-0`_ e `PSR-4`_.
Grazie al componente ClassLoader di Symfony o al meccanismo fornito
da Composer, non occorre mappare manualmente i nomi di classi ai file PHP.
Oggigiorno, le librerie PHP solitamente dispongono di un supporto per il caricamento tramite Composer.

A volte però capita di usare librerie di terze parti che non dispongono
di un supporto per il caricamento, che costringono quindi a caricare ogni classe
a mano. Per esempio, si immagini una libreria con la seguente struttura di cartelle:

.. code-block:: text

    libreria/
    ├── pippo/
    │   ├── quiquoqua/
    │   │   └── Paperone.php
    │   └── Pippo.php
    └── pluto/
        ├── paperino/
        │   └── Pippo.php
        └── Paperrino.php

Questi file contengono le seguenti classi:

======================================== =======================
File                                     Nome classe
======================================== =======================
``libreria/pluto/paperino/Paperone.php`` ``Acme\Pluto\Paperino``
``libreria/pluto/Pippo.php``             ``Acme\Pluto``
``libreria/pippo/pluto/Pippo.php``       ``Acme\Pippo\Pluto``
``libreria/pippo/Pluto.php``             ``Acme\Pippo``
======================================== =======================

Per facilitare le cose, il componente ClassLoader dispone di una classe
:class:`Symfony\\Component\\ClassLoader\\ClassMapGenerator`, che rende
possibile creare una mappa di nomi di classi e file.

Generare una mappa di classi
----------------------------

Per generare una mappa di classi, basta passare la cartella radice dei file delle classi
al metodo :method:`Symfony\\Component\\ClassLoader\\ClassMapGenerator::createMap`::


    use Symfony\Component\ClassLoader\ClassMapGenerator;

    print_r(ClassMapGenerator::createMap(__DIR__.'/library'));

Dati file e classi della tabella precedente, si dovrebbe ottenere un output come
questo:

.. code-block:: text

    Array
    (
        [Acme\Pippo] => /var/www/library/pippo/Pluto.php
        [Acme\Pippo\Pluto] => /var/www/library/pippo/pluto/Pippo.php
        [Acme\Pluto\Paperino] => /var/www/library/pluto/paperino/Paperone.php
        [Acme\Pluto] => /var/www/library/pluto/Pippo.php
    )

Esportare la mappa di classi
----------------------------

La scrittura della mappa di classi sulla console non è sufficiente per
il caricamento automatico. Per fortuna, ``ClassMapGenerator`` dispone di un metodo
:method:`Symfony\\Component\\ClassLoader\\ClassMapGenerator::dump`, per
salvare la mappa di classi generata su filesystem::

    use Symfony\Component\ClassLoader\ClassMapGenerator;

    ClassMapGenerator::dump(__DIR__.'/library', __DIR__.'/class_map.php');

Questa chiamata a ``dump()`` genera la mappa di classi e la scrive nel file ``class_map.php``
nella stessa cartella, con il seguente contenuto::

    <?php return array (
    'Acme\\Pippo' => '/var/www/library/pippo/Pluto.php',
    'Acme\\Pippo\\Pluto' => '/var/www/library/pippo/pluto/Pippo.php',
    'Acme\\Pluto\\Baz' => '/var/www/library/pluto/paperino/Paperone.php',
    'Acme\\Pluto' => '/var/www/library/pluto/Pippo.php',
    );

Invece di caricare ogni file a mano, basta generare la mappa di classi generata,
per esempio usando :class:`Symfony\\Component\\ClassLoader\\MapClassLoader`::

    use Symfony\Component\ClassLoader\MapClassLoader;

    $mapping = include __DIR__.'/class_map.php';
    $loader = new MapClassLoader($mapping);
    $loader->register();

    // ora si possono usare le classi:
    use Acme\Pippo;

    $pippo = new Pippo();

    // ...

.. note::

    L'esempio ipotizza che si abbia già un autoloader funzionante (p.e.
    tramite `Composer`_ o uno dei caricatori di classi del componente
    ClassLoader.

Oltre a esportare la mappa di classi per una cartella, si può anche passare un array
di cartelle per cui generare la mappa di classi (il risultato è
lo stesso dell'esempio precedente)::

    use Symfony\Component\ClassLoader\ClassMapGenerator;

    ClassMapGenerator::dump(
        array(__DIR__.'/library/pluto', __DIR__.'/library/pippo'),
        __DIR__.'/class_map.php'
    );

.. _`PSR-0`: http://www.php-fig.org/psr/psr-0
.. _`PSR-4`: http://www.php-fig.org/psr/psr-4
.. _`Composer`: http://getcomposer.org
