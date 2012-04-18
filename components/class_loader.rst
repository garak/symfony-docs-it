.. index::
   pair: Autoloader; Configurazione

Il componente ClassLoader
=========================

    Il componente ClassLoader carica le classi di un progetto automaticamente, purché
    seguano alcune convenzioni standard di PHP.

Ogni volta che si usa una classe non ancora definita, PHP utilizza il meccanismo di
auto-caricamento per delegare il caricamento di un file che definisca la classe. Symfony2
fornisce un autoloader "universale", capace di caricare classi da file che implementano
una delle seguenti convenzioni:

* Gli `standard`_ tecnici di interoperabilità per i nomi di classi e gli spazi dei nomi
  di PHP 5.3;

* La convenzione dei nomi delle classi di `PEAR`_.

Se le proprie classi e le librerie di terze parti usate per il proprio progetto seguono
questi standard, l'autoloader di Symfony2 è l'unico autoloader di cui si ha
bisogno.

Installazione
-------------

Si può installare il componente in molti modi diversi:

* Usare il repository ufficiale su Git (https://github.com/symfony/ClassLoader);
* Installarlo via PEAR (`pear.symfony.com/ClassLoader`);
* Installarlo via Composer (`symfony/class-loader` su Packagist).

Uso
---

.. versionadded:: 2.1
   Il metodo ``useIncludePath`` è stato aggiunto in Symfony 2.1.

La registrazione di :class:`Symfony\\Component\\ClassLoader\\UniversalClassLoader`
è molto semplice::

    require_once '/percorso/src/Symfony/Component/ClassLoader/UniversalClassLoader.php';

    use Symfony\Component\ClassLoader\UniversalClassLoader;

    $loader = new UniversalClassLoader();

    // Si può cercare in include_path come ultima risorsa.
    $loader->useIncludePath(true);

    $loader->register();

Per un minimo guadagno di prestazioni, i percorsi delle classi possono essere memorizzati
usando APC, registrando :class:`Symfony\\Component\\ClassLoader\\ApcUniversalClassLoader`::

    require_once '/percorso/src/Symfony/Component/ClassLoader/UniversalClassLoader.php';
    require_once '/percorso/src/Symfony/Component/ClassLoader/ApcUniversalClassLoader.php';

    use Symfony\Component\ClassLoader\ApcUniversalClassLoader;

    $loader = new ApcUniversalClassLoader('apc.prefix.');
    $loader->register();

L'autoloader è utile solo se si aggiungono delle librerie da auto-caricare.

.. note::

    L'autoloader è registrato automaticamente in ogni applicazione Symfony2 (si veda
    ``app/autoload.php``).

Se le classi da auto-caricare usano spazi dei nomi, usare i metodi
:method:`Symfony\\Component\\ClassLoader\\UniversalClassLoader::registerNamespace`
o
:method:`Symfony\\Component\\ClassLoader\\UniversalClassLoader::registerNamespaces`::


    $loader->registerNamespace('Symfony', __DIR__.'/vendor/symfony/symfony/src');

    $loader->registerNamespaces(array(
        'Symfony' => __DIR__.'/../vendor/symfony/symofny/src',
        'Monolog' => __DIR__.'/../vendor/monolog/monolog/src',
    ));

    $loader->register();

Per classi che seguono la convenzione dei nomi di PEAR, usare i metodi
:method:`Symfony\\Component\\ClassLoader\\UniversalClassLoader::registerPrefix`
o
:method:`Symfony\\Component\\ClassLoader\\UniversalClassLoader::registerPrefixes`::


    $loader->registerPrefix('Twig_', __DIR__.'/vendor/twig/twig/lib');

    $loader->registerPrefixes(array(
        'Swift_' => __DIR__.'/vendor/swiftmailer/swiftmailer/lib/classes',
        'Twig_'  => __DIR__.'/vendor/twig/twig/lib',
    ));

    $loader->register();

.. note::

    Alcune librerie richiedono anche che il loro percorso radice sia registrato
    nell'include_path di PHP (``set_include_path()``).

Le classi di un sotto-spazio dei nomi o di una sotto-gerarchia di PEAR possono essere
cercate in un elenco di posizioni, per facilitare i venditori di un sotto-insieme di classi
per grossi progetti::

    $loader->registerNamespaces(array(
        'Doctrine\\Common'           => __DIR__.'/vendor/doctrine/common/lib',
        'Doctrine\\DBAL\\Migrations' => __DIR__.'/vendor/doctrine/migrations/lib',
        'Doctrine\\DBAL'             => __DIR__.'/vendor/doctrine/dbal/lib',
        'Doctrine'                   => __DIR__.'/vendor/doctrine/orm/lib',
    ));

    $loader->register();

In questo esempio, se si prova a usare una classe nello spazio dei nomi ``Doctrine\Common``
o uno dei suoi figli, l'autoloader cercherà prima le classi sotto la cartella
``doctrine-common``, quindi, se non le trova, cercherà nella cartella
``Doctrine`` (l'ultima configurata), infine si arrenderà.
In questo caso, l'ordine di registrazione è significativo.

.. _standard: http://symfony.com/PSR0
.. _PEAR:     http://pear.php.net/manual/en/standards.php
