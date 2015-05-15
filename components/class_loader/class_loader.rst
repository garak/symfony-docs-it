.. index::
   single: ClassLoader; Class Loader PSR-0

Class Loader PSR-0
==================

.. versionadded:: 2.1
    La classe ``ClassLoader`` è stata aggiunta in Symfony 2.1.

Se si usano classi e librerie di terze parti che seguono lo standard `PSR-0`_,
si può usare la classe :class:`Symfony\\Component\\ClassLoader\\ClassLoader`
per caricare tutte le classi del progetto.

.. tip::

    Si possono usare sia ``ApcClassLoader`` sia ``XcacheClassLoader`` per mettere in
    :doc:`cache </components/class_loader/cache_class_loader>` un'istanza di ``ClassLoader``
    o di ``DebugClassLoader`` per il :doc:`debug </components/class_loader/debug_class_loader>`.


Uso
---

La registrazione dell'autoloader :class:`Symfony\\Component\\ClassLoader\\ClassLoader`
è semplice::

    require_once '/path/to/src/Symfony/Component/ClassLoader/ClassLoader.php';

    use Symfony\Component\ClassLoader\ClassLoader;

    $loader = new ClassLoader();

    // per abilitare la ricerca in include_path (per esempio per i pacchetti PEAR)
    $loader->useIncludePath(true);

    // ... registrare qui spazi di nomi e prefissi, vedere più avanti

    $loader->register();

.. note::

    In un'applicazione Symfony, l'autoloader è registrato automaticamente (vedere
    ``app/autoload.php``).

Usare i metodi :method:`Symfony\\Component\\ClassLoader\\ClassLoader::addPrefix` o
:method:`Symfony\\Component\\ClassLoader\\ClassLoader::addPrefixes` per
registrare le classi::

    // registra un singolo spazio di nomi
    $loader->addPrefix('Symfony', __DIR__.'/vendor/symfony/symfony/src');

    // registra più spazi di nomi
    $loader->addPrefixes(array(
        'Symfony' => __DIR__.'/../vendor/symfony/symfony/src',
        'Monolog' => __DIR__.'/../vendor/monolog/monolog/src',
    ));

    // registra un prefisso di una classe che segue le convenzioni di PEAR
    $loader->addPrefix('Twig_', __DIR__.'/vendor/twig/twig/lib');

    $loader->addPrefixes(array(
        'Swift_' => __DIR__.'/vendor/swiftmailer/swiftmailer/lib/classes',
        'Twig_'  => __DIR__.'/vendor/twig/twig/lib',
    ));

Si possono cercare le classi di un sotto-spazio dei nomi o di una sotto-gerarchia di classi `PEAR`_
in una lista di posizioni, per facilitare la gestione dei venditori di un sottoinsieme di classi per
grandi progetti::

    $loader->addPrefixes(array(
        'Doctrine\\Common'           => __DIR__.'/vendor/doctrine/common/lib',
        'Doctrine\\DBAL\\Migrations' => __DIR__.'/vendor/doctrine/migrations/lib',
        'Doctrine\\DBAL'             => __DIR__.'/vendor/doctrine/dbal/lib',
        'Doctrine'                   => __DIR__.'/vendor/doctrine/orm/lib',
    ));

In questo esempio, se si prova a usare una classe nello spazio dei nomi ``Doctrine\Common``
o uno dei suoi figli, l'autoloader cercherà prima la classe sotto la cartella
``doctrine-common``. Se non trovata, ripiegherà alla cartella predefinita
``Doctrine`` (l'ultima configurata), prima di arrendersi. L'ordine
delle registrazioni dei prefissi, in questo caso, è significativo.

.. _PEAR:  http://pear.php.net/manual/en/standards.naming.php
.. _PSR-0: http://www.php-fig.org/psr/psr-0/
