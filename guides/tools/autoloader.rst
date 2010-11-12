.. index::
   pair: Autoloader; Configuration

Autoloader
==========

Ogni volta che si utilizza una classe indefinita, PHP utilizza il meccanismo di
caricamento automatico per delegare il caricamento del file dove è definita la classe.
Symfony2 fornisce un autoloader "universale", che è in grado di caricare le classi da
file che implementano una delle seguenti convenzioni:

* La tecnica di interoperabilità `standards`_ per namespace e nomi di classi di PHP 5.3;

* La convenzione `PEAR`_ per i nomi delle classi.

Se le classi e le librerie di terze parti che vengono utilizzate per il progetto
seguono questi standard, l'autoloader di Symfony2 è l'unico autoloader di cui si ha
bisogno.

Utilizzo
--------

Registrare la classe autoloader
:class:`Symfony\\Component\\HttpFoundation\\UniversalClassLoader` è
semplice::

    require_once '/path/to/src/Symfony/Component/HttpFoundation/UniversalClassLoader.php';

    use Symfony\Component\HttpFoundation\UniversalClassLoader;

    $loader = new UniversalClassLoader();
    $loader->register();

L'autoloader è utile solamente quando si aggiungono alcune librerie da autocaricare.

.. note::
   L'autoloader è registrato automaticamente nelle applicazioni di Symfony2 (vedere
   ``src/autoload.php``).

Se le classi da auto caricare utilizzano i namespace, usare i metodi
:method:`Symfony\\Component\\HttpFoundation\\UniversalClassLoader::registerNamespace` o
:method:`Symfony\\Component\\HttpFoundation\\UniversalClassLoader::registerNamespaces`::

    $loader->registerNamespace('Symfony', __DIR__.'/vendor/symfony/src');

    $loader->registerNamespaces(array(
        'Symfony' => __DIR__.'/vendor/symfony/src',
        'Zend'    => __DIR__.'/vendor/zend/library',
    ));

Se le classi seguono la convenzione PEAR per i nomi, usare i metodi
:method:`Symfony\\Component\\HttpFoundation\\UniversalClassLoader::registerPrefix` o
:method:`Symfony\\Component\\HttpFoundation\\UniversalClassLoader::registerPrefixes`::

    $loader->registerPrefix('Twig_', __DIR__.'/vendor/twig/lib');

    $loader->registerPrefixes(array(
        'Swift_' => __DIR__.'/vendor/swiftmailer/lib/classes',
        'Twig_'  => __DIR__.'/vendor/twig/lib',
    ));

.. note::
   Alcune librerie hanno anche bisogno che il loro percorso radice sia registrato
   nell'include path di PHP (``set_include_path()``).

Le classi di sotto-namespace o di una sotto gerarchia di classi PEAR possono essere cercate
in un elenco di percorsi per facilitare gli utilizzatori di un sotto insieme di classi per grossi
progetti::

    $loader->registerNamespaces(array(
        'Doctrine\\Common'           => __DIR__.'/vendor/doctrine-common/lib',
        'Doctrine\\DBAL\\Migrations' => __DIR__.'/vendor/doctrine-migrations/lib',
        'Doctrine\\DBAL'             => __DIR__.'/vendor/doctrine-dbal/lib',
        'Doctrine'                   => __DIR__.'/vendor/doctrine/lib',
    ));

In questo esempio, se si prova a usare una classe nel namespace ``Doctrine\Common``
o in uno dei suoi figli, l'autoloader prima cercherà nelle classi presenti nella cartella
``doctrine-common``,poi se non lo trova tornerà indietro fino alla cartella predefinita
``Doctrine`` (l'ultima configurata), prima di "arrendersi".
In questo caso l'ordine di registrazione è significativo.

.. _standards: http://groups.google.com/group/php-standards/web/psr-0-final-proposal
.. _PEAR:      http://pear.php.net/manual/en/standards.php
