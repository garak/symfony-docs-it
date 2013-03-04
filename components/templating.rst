.. index::
   single: Templating
   single: Componenti; Templating

Il componente Templating
========================

    Templating fornisce tutti gli strumenti necessari per costruire un sistema di
    template.

    Fornisce un'infrastruttra per caricare file di template e, opzionalmente, monitorarne
    le modifiche. Fornisce anche l'implementazione concreta di un motore di template,
    usando PHP e strumenti aggiuntivi per l'escape e per la separazione dei template in
    blocchi e layout.

Installazione
-------------

Si può installare il componente in molti modi diversi:

* Usare il repository ufficiale su Git (https://github.com/symfony/Templating);
* Installarlo via :doc:`Composer</components/using_components>` (``symfony/templating`` su `Packagist`_).

Uso
---

La classe :class:`Symfony\\Component\\Templating\\PhpEngine` è il punto di ingresso
del componente. Ha bisogno di un analizzatore di nomi di template
(:class:`Symfony\\Component\\templating\\TemplateNameParserInterface`), per
convertire il nome di un template in un riferimento a un template, e di un caricatore di template
(:class:`Symfony\\Component\\templating\\Loader\\LoaderInterface`), per trovare il
template associato a un riferimento::

    use Symfony\Component\Templating\PhpEngine;
    use Symfony\Component\Templating\TemplateNameParser;
    use Symfony\Component\Templating\Loader\FilesystemLoader;

    $loader = new FilesystemLoader(__DIR__ . '/views/%name%');

    $view = new PhpEngine(new TemplateNameParser(), $loader);

    echo $view->render('hello.php', array('firstname' => 'Fabien'));

Il metodo :method:`Symfony\\Component\\Templating\\PhpEngine::render` esegue il
file `views/hello.php` e restituisce il testo di output.

.. code-block:: html+php

    <!-- views/hello.php -->
    Hello, <?php echo $firstname ?>!

Ereditarietà dei template con gli slot
--------------------------------------

L'ereditarietà dei template è pensata per condividere dei layout tra template diversi.

.. code-block:: php

    <!-- views/layout.php -->
    <html>
        <head>
            <title><?php $view['slots']->output('title', 'Default title') ?></title>
        </head>
        <body>
            <?php $view['slots']->output('_content') ?>
        </body>
    </html>

Il metodo :method:`Symfony\\Templating\\PhpEngine::extend` è richiamato nel
sotto-template, per impostare il suo template padre.

.. code-block:: html+php

    <!-- views/page.php -->
    <?php $view->extend('layout.php') ?>

    <?php $view['slots']->set('title', $page->title) ?>

    <h1>
        <?php echo $page->title ?>
    </h1>
    <p>
        <?php echo $page->body ?>
    </p>

Per usare l'ereditarietà dei template, l'helper :class:`Symfony\\Templating\\Helper\\SlotsHelper`
deve essere registrato::

    use Symfony\Templating\Helper\SlotsHelper;

    $view->set(new SlotsHelper());

    // Recupera l'oggetto $page
    $page = ...;

    echo $view->render('page.php', array('page' => $page));

.. note::

    Si possono avere più livelli di ereditarietà: un layout può estendere un
    altro layout.

Escape dell'output
------------------

Questa documentazione è ancora da scrivere.

L'helper Asset
--------------

Questa documentazione è ancora da scrivere.

.. _Packagist: https://packagist.org/packages/symfony/templating