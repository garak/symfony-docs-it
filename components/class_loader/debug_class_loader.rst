.. index::
    single: Class Loader; DebugClassLoader
    
Debug di un Class Loader
========================

La classe :class:`Symfony\\Component\\ClassLoader\\DebugClassLoader` tenta di
lanciare eccezioni più utitli, quando gli autoloader registrati non trovano
una classe. Tutti gli autoloader che implementano un metodo ``findFile()`` sono sostituiti
con un wrapper ``DebugClassLoader``.

L'uso di ``DebugClassLoader`` è facile, basta richiamare il suo metodo statico
:method:`Symfony\\Component\\ClassLoader\\DebugClassLoader::enable`::

    use Symfony\Component\ClassLoader\DebugClassLoader;
    
    DebugClassLoader::enable();
