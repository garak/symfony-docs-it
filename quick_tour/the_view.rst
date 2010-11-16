La vista
========

Dopo aver letto la prima parte di questa guida, avete deciso che Symfony2
vale altri dieci minuti. Bene! In questa seconda parte, si imparerà di più
sul sistema dei template di Symfony2. Come visto prima, Symfony2 usa PHP
come motore di template predefinito, ma vi aggiunge alcune caratteristiche
interessanti, per renderlo più potente.

Invece di PHP, si può anche usare `Twig`_ (rende i template più concisi e
più amichevoli per i grafici). Se si preferisce usare `Twig`, leggere il
capitolo alternativo :doc:`Vista con Twig <the_view_with_twig>`.

.. index::
  single: Template; Layout
  single: Layout

Decorare i template
-------------------

Molto spesso, i template in un progetto condividono alcuni elementi comuni,
come i ben noti header e footer. In Symfony2, il problema è affrontato in
modo diverso: un template può essere decorato da un altro template.

Il template ``index.php`` è decorato da ``layout.php``, grazie alla chiamata
a ``extend()``:

.. code-block:: html+php

    <!-- src/Application/HelloBundle/Resources/views/Hello/index.php -->
    <?php $view->extend('HelloBundle::layout.php') ?>

    Hello <?php echo $name ?>!

La notazione ``HelloBundle::layout.php`` suona familiare, non è vero? È la
stessa notazione usata per riferirsi a un template. La parte ``::`` vuol
dire semplicemente che l'elemento controllore è vuoto, quindi il file
corrispondente si trova direttamente sotto ``views/``.

Diamo ora un'occhiata al file ``layout.php``:

.. code-block:: html+php

    <!-- src/Application/HelloBundle/Resources/views/layout.php -->
    <?php $view->extend('::layout.php') ?>

    <h1>Hello Application</h1>

    <?php $view['slots']->output('_content') ?>

Il layout stesso è decorato da un altro layout (``::layout.php``). Symfony2
supporta livelli multipli di decorazione: un layout può essere decorato da
un altro. Quando la parte bundle di un nome di template è vuota, le viste
vengono cercate nella cartella ``app/views/``. Tale cartella conserva le
viste globali per l'intero progetto:

.. code-block:: html+php

    <!-- app/views/layout.php -->
    <!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
    <html>
        <head>
            <meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
            <title><?php $view['slots']->output('title', 'Hello Application') ?></title>
        </head>
        <body>
            <?php $view['slots']->output('_content') ?>
        </body>
    </html>

Per entrambi i layout, l'espressione ``$view['slots']->output('_content')``
viene sostituita dal contenuto del template figlio, rispettivamente
``index.php`` e ``layout.php`` (nella prossima sezione saranno approfonditi
gli slot).

Come si può vedere, Symfony2 fornisce dei metodi su un misterioso oggetto
``$view``. In un template, la variabile ``$view`` è sempre disponibile e
si riferisce a oggetti speciali che forniscono un sacco di metodi, per
mantenere snello il motore dei template.

.. index::
   single: Template; Slot
   single: Slot

Lavorare con gli slot
---------------------

Uno slot è una porzione di codice, definita in un template e riutilizzabile
in ogni layout che decora il template. Nel template ``index.php``,
definire uno slot ``title``:

.. code-block:: html+php

    <!-- src/Application/HelloBundle/Resources/views/Hello/index.php -->
    <?php $view->extend('HelloBundle::layout.php') ?>

    <?php $view['slots']->set('title', 'Hello World Application') ?>

    Hello <?php echo $name ?>!

Il layout di base ha già il codice per mostrare il titolo:

.. code-block:: html+php

    <!-- app/views/layout.php -->
    <head>
        <meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
        <title><?php $view['slots']->output('title', 'Hello Application') ?></title>
    </head>

Il metodo ``output()`` inserisce il contenuto di uno slot e accetta un
valore predefinito opzionale, se lo slot non è definito. E ``_content``
è solo uno slot speciale, che contiene la resa del template figlio.

Per slot molto grandi, c'è anche una sintassi estesa:

.. code-block:: html+php

    <?php $view['slots']->start('title') ?>
        Some large amount of HTML
    <?php $view['slots']->stop() ?>

.. index::
   single: Template; Inclusione

Includere altri template
------------------------

Il modo migliore per condividere una parte di codice di un template è quello
di definire un template che possa essere incluso in altri template.

Creare un template ``hello.php``:

.. code-block:: html+php

    <!-- src/Application/HelloBundle/Resources/views/Hello/hello.php -->
    Hello <?php echo $name ?>!

E cambiare ``index.php`` per includerlo:

.. code-block:: html+php

    <!-- src/Application/HelloBundle/Resources/views/Hello/index.php -->
    <?php $view->extend('HelloBundle::layout.php') ?>

    <?php echo $view->render('HelloBundle:Hello:hello.php', array('name' => $name)) ?>

Il metodo ``render()`` valuta e restituisce il contenuto di un altro template
(è esattamente lo stesso metodo usato nel controllore).

.. index::
   single: Template; Includere pagine

Inserire altri controllori
--------------------------

Cosa fare se si vuole inserire il risultato di un altro controllore in un
template? Può essere molto utile quando si lavora con Ajax o quando il
template incluso necessita di alcune variabili, non disponibili nel
template principale.

Se si crea un'azione ``fancy`` e la si vuole includere nel template
``index.php``, basta usare il seguente codice:

.. code-block:: html+php

    <!-- src/Application/HelloBundle/Resources/views/Hello/index.php -->
    <?php echo $view['actions']->render('HelloBundle:Hello:fancy', array('name' => $name, 'color' => 'green')) ?>

Qui la stringa ``HelloBundle:Hello:fancy`` si riferisce all'azione ``fancy``
del controller ``Hello``::

    // src/Application/HelloBundle/Controller/HelloController.php

    class HelloController extends Controller
    {
        public function fancyAction($name, $color)
        {
            // crea un oggetto basato sulla variabile $color
            $object = ...;

            return $this->render('HelloBundle:Hello:fancy.php', array('name' => $name, 'object' => $object));
        }

        // ...
    }

Ma dove è definito l'elemento dell'array ``$view['actions']``?
Come ``$view['slots']``, è un cosiddetto helper di template e
verrà spiegato nella prossima sezione.

.. index::
   single: Template; Helper

Usare gli helper dei template
-----------------------------

Il sistema dei template di Symfony2 può essere facilmente esteso tramite
gli helper. Gli helper sono oggetti PHP che forniscono caratteristiche
utili in un contesto di template. Due degli helper distribuiti con
Symfony2 sono ``actions`` e ``slots``.

Creare collegamenti tra le pagine
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Parlando di applicazioni web, i collegamenti tra pagine sono una parte
essenziale. Invece di inserire a mano gli URL nei template, l'helper
``router`` sa come generare URL in base alla configurazione delle rotte.
In questo modo, tutti gli URL saranno facilmente cambiati al cambiare
della configurazione:

.. code-block:: html+php

    <a href="<?php echo $view['router']->generate('hello', array('name' => 'Thomas')) ?>">
        Greet Thomas!
    </a>

Il metodo ``generate()`` accetta come parametri un nome di rotta e un
array di parametri. Il nome della rotta è la chiave principale sotto
cui le rotte sono elencate e i parametri sono i valori dei segnaposto
definiti nello schema della rotta:

.. code-block:: yaml

    # src/Application/HelloBundle/Resources/config/routing.yml
    hello: # The route name
        pattern:  /hello/:name
        defaults: { _controller: HelloBundle:Hello:index }

Usare le risorse: immagini, JavaScript e fogli di stile
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Cosa sarebbe Internet senza immagini, JavaScript e fogli di stile?
Symfony2 fornisce tre helper per gestirli facilmente: ``assets``,
``javascripts`` e ``stylesheets``:

.. code-block:: html+php

    <link href="<?php echo $view['assets']->getUrl('css/blog.css') ?>" rel="stylesheet" type="text/css" />

    <img src="<?php echo $view['assets']->getUrl('images/logo.png') ?>" />

Lo scopo principale dell'helper ``assets`` è quello di rendere le
applicazioni maggiormente portabili. Grazie a questo helper, si
possono spostare la cartella radice dell'applicazione ovunque,
sotto la propria cartella radice del web, senza cambiare nulla
nel codice dei template

Similmente, si possono gestire fogli di stile e JavaScript con
gli helper ``stylesheets`` e ``javascripts``:

.. code-block:: html+php

    <?php $view['javascripts']->add('js/product.js') ?>
    <?php $view['stylesheets']->add('css/product.css') ?>

Il metodo ``add()`` definisce delle dipendenze. Per mostrare
veramente queste risorse, occorre anche aggiungere il codice
seguente nel layout principale:

.. code-block:: html+php

    <?php echo $view['javascripts'] ?>
    <?php echo $view['stylesheets'] ?>

Considerazioni finali
---------------------

Il sistema dei template di Symfony2 è semplice ma potente. Grazie
a layout, slot, template e inclusioni di azioni, è molto facile
organizzare i propri template in un modo logico ed estensibile.

Stiamo lavorando con Symfony2 da soli venti minuti e già siamo
in grado di fare cose incredibili. Questo è il potere di Symfony2.
Imparare le basi è facile e si imparerà presto che questa
facilità è nascosta sotto un'architettura molto flessibile.

Ma non corriamo troppo. Prima occorre imparare di più sul
controllore e questo è esattamente l'argomento della prossima
parte di questa guida. Pronti per altri dieci minuti di Symfony2?

.. _Twig: http://www.twig-project.org/
