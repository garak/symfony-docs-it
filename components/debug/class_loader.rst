.. index::
    single: ClassLoader; DebugClassLoader
    single: Debug; DebugClassLoader

Debug di ClassLoader
====================

.. versionadded:: 2.4
    ``DebugClassLoader`` del componente Debug è stato introdotto in Symfony 2.4.
    In precedenza, si trovava nel componente ClassLoader.

:class:`Symfony\\Component\\Debug\\DebugClassLoader` tenta di
lanciare eccezioni più utili, quando gli autoloader registrati non trovano
una classe. Tutti gli autoloader che implementano un metodo ``findFile()`` vengono sostituiti
da un ``DebugClassLoader``.

L'uso di ``DebugClassLoader`` è facile, basta richiamare il metodo statico
:method:`Symfony\\Component\\Debug\\DebugClassLoader::enable`::

    use Symfony\Component\ClassLoader\DebugClassLoader;

    DebugClassLoader::enable();
