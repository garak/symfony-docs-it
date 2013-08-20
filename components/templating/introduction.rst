.. index::
   single: Templating
   single: Componenti; Templating

Il componente Templating
========================

    Il componente Templating fornisce tutti gli strumenti necessari per costruire
    un sistema di template.

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
(:class:`Symfony\\Component\\templating\\TemplateNameParserInterface`),
per convertire il nome di un template in un riferimento a un template
(:class:`Symfony\\Component\\Templating\\TemplateReferenceInterface`).
Ha inoltre bisogno di un caricatore di template (:class:`Symfony\\Component\\templating\\Loader\\LoaderInterface`),
per trovare il template associato a un riferimento::

    use Symfony\Component\Templating\PhpEngine;
    use Symfony\Component\Templating\TemplateNameParser;
    use Symfony\Component\Templating\Loader\FilesystemLoader;

    $loader = new FilesystemLoader(__DIR__.'/views/%name%');

    $templating = new PhpEngine(new TemplateNameParser(), $loader);

    echo $templating->render('hello.php', array('firstname' => 'Fabien'));

.. code-block:: html+php

    <!-- views/hello.php -->
    Ciao, <?php echo $firstname ?>!

Il metodo :method:`Symfony\\Component\\Templating\\PhpEngine::render` esegue il
file `views/hello.php` e restituisce il testo di output. Il secondo parametro
di ``render`` è un array di variabili da usare nel template. In questo
esempio, il risultato sarà ``Ciao, Fabien!``.

La variabile ``$view``
----------------------

In tutti i template analizzati da ``PhpEngine`` si ha accesso a una misteriosa
variabile, chiamata ``$view``. Questa variabile contiene l'istanza corrente di ``PhpEngine``.
Questo vuol dire che si ha accesso a un sacco di metodi che rendono facile
la vita.

Includere template
------------------

Il modo migliore per condividere una porzione di codice è creare un template che
possa essere incluso da altri template. Siccome la variabile ``$view`` è
un'istanza di ``PhpEngine``, si può usare il metodo ``render`` (usato per
rendere originariamente il template) all'interno del template, per rendere un altro template::

    <?php $names = array('Fabien', ...) ?>
    <?php foreach ($names as $name) : ?>
        <?php echo $view->render('hello.php', array('firstname' => $name)) ?>
    <?php endforeach ?>

Escape dell'output
------------------

Quando si rendono delle variabili, probabilmente si vorrà un escape, in modo che il codice HTML o
JavaScript non venga scritto nella pagina. In questo modo si prevengono attacchi come
XSS. Per poterlo fare, usare il metodo
:method:`Symfony\\Component\\Templating\\PhpEngine::escape`::

    <?php echo $view->escape($firstname) ?>

Per impostazione predefinita, il metodo ``escape()`` ipotizza che la variabile sia mostrata
in un contesto HTML, Il secondo parametro dà la possibilità di cambiare tale contesto. Per
esempio, per mostrare una variabile in JavaScript, usare il contesto ``js``::

    <?php echo $view->escape($var, 'js') ?>

Il componente fornisce escape in HTML e JS. Si può registrare un escape
personalizzato, usando il metodo
:method:`Symfony\\Component\\Templating\\PhpEngine::setEscaper`::

    $templating->setEscaper('css', function ($value) {
        // ... escape CSS

        return $escapedValue;
    });

Aiutanti
--------

Il componente Templating può essere facilmente esteso, tramite aiutanti. Il componente ha
due aiutanti predefiniti:

* :doc:`/components/templating/helpers/assetshelper`
* :doc:`/components/templating/helpers/slotshelper`

Prima di poterli usare, occorre registrare tali aiutanti, usando
:method:`Symfony\\Component\\Templating\\PhpEngine::set`::

    use Symfony\Component\Templating\Helper\AssetsHelper;
    // ...

    $templating->set(new AssetsHelper());

Aiutanti personalizzati
~~~~~~~~~~~~~~~~~~~~~~~

Si può creare un proprio aiutante, creando una classe che implementi
:class:`Symfony\\Component\\Templating\\Helper\\HelperInterface`. Tuttavia,
la maggior parte delle volte si estenderà
:class:`Symfony\\Component\\Templating\\Helper\\Helper`.

La classe ``Helper`` ha un metodo obbligatorio:
:method:`Symfony\\Component\\Templating\\Helper\\HelperInterface::getName`.
Resituisce il nome da usare per ottenere l'aiutante dall'oggetto ``$view``.

.. _Packagist: https://packagist.org/packages/symfony/templating
