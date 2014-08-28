.. index::
   single: ClassLoader; Caricatore di classi PSR-4

Caricatore di classi PSR-4
==========================

.. versionadded:: 2.5
    :class:`Symfony\\Component\\ClassLoader\\Psr4ClassLoader` è stato
    introdotto in Symfony 2.5.

Si possono caricare le librerie che seguono lo standard `PSR-4`_ con ``Psr4ClassLoader``.

.. note::

    Se si gestiscono le dipendenza tramite Composer, si ha giò un autoloader comptabilie
    con PSR-4. Usare questo caricatore in ambienti in cui Composer
    non sia disponibile.

.. tip::

    Tutti i componenti di Symfony seguono PSR-4.

Uso
---

L'esempio seguente dimostra come si possa usare l'autoloader
:class:`Symfony\\Component\\ClassLoader\\Psr4ClassLoader` per il componente
Yaml di Symfony. Si immagini di aver scaricato sia ClassLoader sia il componente
Yaml come ZIP e di averli scompattati in una cartella ``libs``.
La struttura della cartella assomiglierà a questa:

.. code-block:: text

    libs/
        ClassLoader/
            Psr4ClassLoader.php
            ...
        Yaml/
            Yaml.php
            ...
    config.yml
    demo.php

In ``demo.php``, si analizzerà il file ``config.yml``. Per poterlo fare,
occorre prima configurare ``Psr4ClassLoader``:

.. code-block:: php

    use Symfony\Component\ClassLoader\Psr4ClassLoader;
    use Symfony\Component\Yaml\Yaml;

    require __DIR__.'/lib/ClassLoader/Psr4ClassLoader.php';

    $loader = new Psr4ClassLoader();
    $loader->addPrefix('Symfony\\Component\\Yaml\\', __DIR__.'/lib/Yaml');
    $loader->register();

    $data = Yaml::parse(__DIR__.'/config.yml');

Prima di tutto, il caricatore viene caricato manualmente, usando un'istruzione ``require``,
perché non c'è ancora un meccanismo di caricamento automatico. Con la chiamata a
:method:`Symfony\\Component\\ClassLoader\\Psr4ClassLoader::addPrefix`, si dice
al caricatore di classi di cercare classi con prefisso
``Symfony\Component\Yaml\`` nello spazio dei nomi. Dopo aver registrato l'autoloader,
il componente Yaml è pronto all'uso.

.. _PSR-4: http://www.php-fig.org/psr/psr-4/
