.. index::
    single: APC; ApcClassLoader
    single: ClassLoader; ApcClassLoader
    single: ClassLoader; Cache
    single: ClassLoader; XcacheClassLoader
    single: XCache; XcacheClassLoader
    
Cache di Class Loader
=====================

Introduzione
------------

Trovare un file per una classe specifica può essere pesante. Per fortuna,
il componente Class Loader dispone di due classi per la cache della mappatura
da classe a file. Sia :class:`Symfonfy\\Component\\ClassLoader\\ApcClassLoader`
che :class:`Symfony\\Component\\ClassLoader\\XcacheClassLoader` sono wrapper
intorno all'oggetto che implementa un metodo ``findFile()``, per trovare il file
di una classe.

.. note::

  Sia ``ApcClassLoader`` che ``XcacheClassLoader`` possono essere usati
  per la cache dell'`autoloader`_ di Composer.

ApcClassLoader
--------------

.. versionadded:: 2.1
    La classe ``ApcClassLoader`` è stata aggiunta in Symfony 2.1.

``ApcClassLoader`` è un wrapper di un class loader esistente e mette in cache le chiamate al suo
metodo ``findFile()``, usando `APC`_::

    require_once '/path/to/src/Symfony/Component/ClassLoader/ApcClassLoader.php';

    // istanza di una classe che implementa un metodo findFile(), come ClassLoader
    $loader = ...;

    // mio_prefisso è il prefisso da usare in APC
    $cachedLoader = new ApcClassLoader(sha1(__FILE__), $loader);

    // registra il class loader in cache
    $cachedLoader->register();

    // disattiva il loader originale, non in cache, se era stato precedentemente registrato
    $loader->unregister();

XcacheClassLoader
-----------------

.. versionadded:: 2.1
    La classe ``XcacheClassLoader`` è stata aggiunta in Symfony 2.1.

``XcacheClassLoader`` usa `XCache`_ per mettere in cache un class loader. La registrazione
è semplice::

    require_once '/path/to/src/Symfony/Component/ClassLoader/XcacheClassLoader.php';

    // istanza di una classe che implementa un metodo findFile(), come ClassLoader
    $loader = ...;

    // mio_prefisso è il prefisso da usare in XCache
    $cachedLoader = new XcacheClassLoader(sha1(__FILE__), $loader);

    // registra il class loader in cache
    $cachedLoader->register();

    // disattiva il loader originale, non in cache, se era stato precedentemente registrato
    $loader->unregister();

.. _APC:        http://php.net/manual/it/book.apc.php
.. _autoloader: http://getcomposer.org/doc/01-basic-usage.md#autoloading
.. _XCache:     http://xcache.lighttpd.net
