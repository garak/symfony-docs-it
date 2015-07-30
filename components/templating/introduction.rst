.. index::
   single: Templating
   single: Componenti; Templating

Il componente Templating
========================

    Il componente Templating fornisce tutti gli strumenti necessari per costruire
    un sistema di template.

    Fornisce un'infrastruttura per caricare file di template e, opzionalmente, monitorarne
    le modifiche. Fornisce anche l'implementazione concreta di un motore di template,
    usando PHP e strumenti aggiuntivi per l'escape e per la separazione dei template in
    blocchi e layout.

Installazione
-------------

Si può installare il componente in due modi:

* Installarlo tramite :doc:`Composer </components/using_components>` (``symfony/templating`` su `Packagist`_);
* Usare il repository ufficiale su Git (https://github.com/symfony/Templating).

.. include:: /components/require_autoload.rst.inc

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

.. note::

    I template saranno messi in cache all'interno della memoria del motore. Questo vuol dire
    che se si rende lo stesso template più volte nella stessa richiesta, il
    template sarà caricato un'unica volta dal filesystem.

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

Variabili globali
-----------------

A volte si ha bisogno di impostare una variabile che sia disponibile in tutti i template
resi da un motore (come la variabile ``$app`` quando si usa il framework Symfony).
Tali variabili possono essere impostate usando il metodo
:method:`Symfony\\Component\\Templating\\PhpEngine::addGlobal` e vi si può
accedere nel template come normali variabili::

    $templating->addGlobal('ga_tracking', 'UA-xxxxx-x');

In un template:

.. code-block:: html+php

    <p>Il codice di tracking di google è: <?php echo $ga_tracking ?></p>

.. caution::

    Le variabili globali non possono chiamarsi ``this`` o ``view``, poiché tali nomi
    sono usati dal motore PHP.

.. note::

    Le variabili globali possono essere sovrascritte da variabili locali nel template
    che abbiano lo stesso nome.

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

Il componente Templating può essere facilmente esteso, tramite aiutanti. Gli aiutanti sono oggetti PHP,
che forniscono caratteristiche utili nel contesto di un template. Il componente ha
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
Restituisce il nome da usare per ottenere l'aiutante dall'oggetto ``$view``.

Creare un motore personalizzato
-------------------------------

Oltre a fornire un motore di template PHP, si può anche creare un proprio motore,
usando il componente Templating. Per poterlo fare, creare una nuova classe che
implementi :class:`Symfony\\Component\\Templating\\EngineInterface`. L'interfaccia
richiede tre metodi:

* :method:`render($name, array $parameters = array()) <Symfony\\Component\\Templating\\EngineInterface::render>`
  - Rende un template
* :method:`exists($name) <Symfony\\Component\\Templating\\EngineInterface::exists>`
  - Verifica se il template esiste
* :method:`supports($name) <Symfony\\Component\\Templating\\EngineInterface::supports>`
  - Verifica se il template dato possa essere gestito dal motore.

Usare più motori
----------------

Si possono usare più motori contemporaneamente, usando la classe
:class:`Symfony\\Component\\Templating\\DelegatingEngine`. Questa classe
accetta una lista di motori e agisce come un normale motore di template. La
sola differenza è che delega le chiamate a uno degli altri motori. Per
scegliere quale motore usare per il template, viene usato il metodo
:method:`EngineInterface::supports() <Symfony\\Component\\Templating\\EngineInterface::supports>`.


.. code-block:: php

    use Acme\Templating\CustomEngine;
    use Symfony\Component\Templating\PhpEngine;
    use Symfony\Component\Templating\DelegatingEngine;

    $templating = new DelegatingEngine(array(
        new PhpEngine(...),
        new CustomEngine(...),
    ));

.. _Packagist: https://packagist.org/packages/symfony/templating
