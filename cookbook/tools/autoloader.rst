.. index::
   pair: Autoloader; Configurazione

Come auto-caricare le classi
============================

Ogni volta che si usa una classe non definita, PHP usa il meccanismo di auto-caricamento
per delegare il caricamento di un file che definisce la classe. Symfony2 fornisce un
autoloader "universale", che è in grado di caricare classi dai file che implementano una
delle seguenti convenzioni:

* Gli `standard`_ tecnici di interoperabilità per gli spazi dei nomi e i nomi delle
  classi di PHP 5.3;

* La convezione dei nomi di `PEAR`_ per le classi.

Se le proprie classi e le librerie di terze parti usate per il proprio progetto seguono
tali standard, l'autoloader di Symfony2 è l'unico autoloader di cui si avrà
bisogno.

Utilizzo
--------

.. versionadded:: 2.1
   Il metodo ``useIncludePath`` è stato aggiunto in Symfony 2.1.

La registrazione dell'autoloader
:class:`Symfony\\Component\\ClassLoader\\UniversalClassLoader` è semplice::

    require_once '/percorso/src/Symfony/Component/ClassLoader/UniversalClassLoader.php';

    use Symfony\Component\ClassLoader\UniversalClassLoader;

    $loader = new UniversalClassLoader();

    // Si può cercare in include_path come ultima risorsa.
    $loader->useIncludePath(true);

    $loader->register();

Per piccoli miglioramenti delle prestazioni, i percorsi delle classi possono essere messi
in cache in memoria, usando APC o registrando
:class:`Symfony\\Component\\ClassLoader\\ApcUniversalClassLoader`::

    require_once '/percorso/src/Symfony/Component/ClassLoader/UniversalClassLoader.php';
    require_once '/percorso/src/Symfony/Component/ClassLoader/ApcUniversalClassLoader.php';

    use Symfony\Component\ClassLoader\ApcUniversalClassLoader;

    $loader = new ApcUniversalClassLoader('apc.prefix.');
    $loader->register();

L'autoloader è utile solo se si aggiungono delle librerie da auto-caricare.

.. note::

    L'autoloader è registrato automaticamente in un'applicazione Symfony2 (vedere
    ``app/autoload.php``).

Se le classi da auto-caricare usano spazi di nomi, usare il metodo
:method:`Symfony\\Component\\ClassLoader\\UniversalClassLoader::registerNamespace`
oppure il metodo
:method:`Symfony\\Component\\ClassLoader\\UniversalClassLoader::registerNamespaces`::

    $loader->registerNamespace('Symfony', __DIR__.'/vendor/symfony/src');

    $loader->registerNamespaces(array(
        'Symfony' => __DIR__.'/../vendor/symfony/src',
        'Monolog' => __DIR__.'/../vendor/monolog/src',
    ));

Per classi che seguono la convenzione dei nomi di PEAR, usare il metodo
:method:`Symfony\\Component\\ClassLoader\\UniversalClassLoader::registerPrefix`
oppure il metodo
:method:`Symfony\\Component\\ClassLoader\\UniversalClassLoader::registerPrefixes`::


    $loader->registerPrefix('Twig_', __DIR__.'/vendor/twig/lib');

    $loader->registerPrefixes(array(
        'Swift_' => __DIR__.'/vendor/swiftmailer/lib/classes',
        'Twig_'  => __DIR__.'/vendor/twig/lib',
    ));

.. note::

    Alcune librerie richiedono anche che il loro percorso radice sia registrato
    nell'include_path di PHP (``set_include_path()``).

Le classi in un sotto-spazio dei nomi o in una sotto-gerarchia di classi PEAR possono
essere cercato in una lista di posti, per facilitare l'aggiunta ai venditori di un
sotto-insieme di classi per grandi progetti::

    $loader->registerNamespaces(array(
        'Doctrine\\Common'           => __DIR__.'/vendor/doctrine-common/lib',
        'Doctrine\\DBAL\\Migrations' => __DIR__.'/vendor/doctrine-migrations/lib',
        'Doctrine\\DBAL'             => __DIR__.'/vendor/doctrine-dbal/lib',
        'Doctrine'                   => __DIR__.'/vendor/doctrine/lib',
    ));

In questo esempio, se si prova a usare una classe nello spazio dei nomi ``Doctrine\Common``
o uno dei suo figli, l'autoloader cercherà prima le classi nella cartella
``doctrine-common``, quindi proverà della cartella predefinita
``Doctrine`` (l'ultima configurata) se non trova nulla, infine rinuncerà.
In questo caso, l'ordine di registrazione è significativo.

.. _standard: http://groups.google.com/group/php-standards/web/psr-0-final-proposal
.. _PEAR:     http://pear.php.net/manual/en/standards.php
