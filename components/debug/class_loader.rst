.. index::
    single: ClassLoader; DebugClassLoader
    single: Debug; DebugClassLoader

Debug di ClassLoader
====================

:class:`Symfony\\Component\\Debug\\DebugClassLoader` tenta di
lanciare eccezioni più utili, quando gli autoloader registrati non trovano
una classe. Tutti gli autoloader che implementano un metodo ``findFile()`` vengono sostituiti
da un ``DebugClassLoader``.

L'uso di ``DebugClassLoader`` è facile, basta richiamare il metodo statico
:method:`Symfony\\Component\\Debug\\DebugClassLoader::enable`::

    use Symfony\Component\Debug\DebugClassLoader;

    DebugClassLoader::enable();
