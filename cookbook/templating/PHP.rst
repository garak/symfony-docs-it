.. index::
   single: Template PHP

Usare PHP al posto di Twig nei template
=======================================

Anche se Twig è il motore predefinito di template in Symfony2, si può sempre usare
PHP, se lo si preferisce. Entrambi i motori di template sono supportati in ugual modo
in Symfony2. Symfony2 aggiunge alcune caratteristiche interessanti sopra PHP, per rendere
la scrittura dei template più potente.

Rendere i template PHP
----------------------

Se si vuole usare il motore di template PHP, occorre prima di tutto assicurarsi
di abilitarlo nel file di configurazione dell'applicazione:

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        framework:
            # ...
            templating:
                engines: ['twig', 'php']

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <framework:config>
            <!-- ... -->
            <framework:templating>
                <framework:engine id="twig" />
                <framework:engine id="php" />
            </framework:templating>
        </framework:config>

    .. code-block:: php

        $container->loadFromExtension('framework', array(
            // ...
            'templating' => array(
                'engines' => array('twig', 'php'),
            ),
        ));

Ora si può rendere un template PHP invece di uno Twig, semplicemente usando nel nome
del template l'estensione ``.php`` al posto di ``.twig``. Il controllore sottostante
rende il template ``index.html.php``::

    // src/Acme/HelloBundle/Controller/HelloController.php

    // ...
    public function indexAction($name)
    {
        return $this->render(
            'AcmeHelloBundle:Hello:index.html.php',
            array('name' => $name)
        );
    }

Si può anche usare la scorciatoia `@Template`_ per rendere il template
``AcmeHelloBundle:Hello:index.html.php``::

    // src/Acme/HelloBundle/Controller/HelloController.php
    use Sensio\Bundle\FrameworkExtraBundle\Configuration\Template;

    // ...

    /**
     * @Template(engine="php")
     */
    public function indexAction($name)
    {
        return array('name' => $name);
    }

.. caution::

    Si possono abilitare entrambi i motori di template, ``php`` e ``twig``,
    ma questo produrrà un effetto indesiderato nell'applicazione:
    la notazione ``@`` per gli spazi di nomi Twig non sarà più supportata dal metodo
    ``render()``::

        public function indexAction()
        {
            // ...

            // i template con spazi dei nomi non funzioneranno più nei controllori
            $this->render('@Acme/Default/index.html.twig');

            // si deve usare la notazione tradizionale dei template
            $this->render('AcmeBundle:Default:index.html.twig');
        }

    .. code-block:: jinja

        {# nei template Twig, i template con spazi dei nomi funzionano come ci si aspetta #}
        {{ include('@Acme/Default/index.html.twig') }}

        {# anche la notazione tradizionale funzionerà #}
        {{ include('AcmeBundle:Default:index.html.twig') }}


.. index::
  single: Template; Layout
  single: Layout

Decorare i template
-------------------

Spesso i template in un progetto condividono elementi comuni, come la testata e il pie'
di pagina. In Symfony2, ci piace pensare a questo problema in modo diverso: un template
può essere decorato da un altro template.

Il template ``index.html.php`` è decorato ``layout.html.php``, grazie alla chiamata
a ``extend()``:

.. code-block:: html+php

    <!-- src/Acme/HelloBundle/Resources/views/Hello/index.html.php -->
    <?php $view->extend('AcmeHelloBundle::layout.html.php') ?>

    Ciao <?php echo $name ?>!

La notazione ``HelloBundle::layout.html.php`` suona familiare, non è vero? È la
stessa notazione usata per fare riferimento a un template. La parte ``::`` vuol dire
semplicemente che l'elemento controllore è vuoto, quindi il file corrispondente
è memorizzato direttamente sotto ``views/``.

Diamo ora un'occhiata al file ``layout.html.php``:

.. code-block:: html+php

    <!-- src/Acme/HelloBundle/Resources/views/layout.html.php -->
    <?php $view->extend('::base.html.php') ?>

    <h1>Applicazione Ciao</h1>

    <?php $view['slots']->output('_content') ?>

Il layout stesso è decorato da un altro template (``::base.html.php``). Symfony2
supporta livelli molteplici di decorazione: un layout può esso stesso essere
decorato da un altro layout. Quando la parte bundle del nome del template è vuota,
le viste sono cercate nella cartella ``app/Resources/views/``. Questa cartella contiene
le viste globali del proprio progetto:

.. code-block:: html+php

    <!-- app/Resources/views/base.html.php -->
    <!DOCTYPE html>
    <html>
        <head>
            <meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
            <title><?php $view['slots']->output('title', 'Hello Application') ?></title>
        </head>
        <body>
            <?php $view['slots']->output('_content') ?>
        </body>
    </html>

Per entrambi i layout, l'espressione ``$view['slots']->output('_content')`` viene
sostituita dal contenuto del template figlio, rispettivamente ``index.html.php`` e
``layout.html.php`` (approfondiremo gli slot nella prossima sezione).

Come si può vedere, Symfony2 fornisce metodi su un misterioso oggetto ``$view``. In
un template, la variabile ``$view`` è sempre disponibile e fa riferimento a uno speciale
oggetto che fornisce un sacco di metodi, che mantengono snello il motore dei template.

.. index::
   single: Template; Slot
   single: Slot

Lavorare con gli slot
---------------------

Uno slot è un pezzetto di codice, definito in un template e riutilizzabile in qualsiasi
layout che decora il template. Nel template ``index.html.php``, definiamo uno
slot ``title``:

.. code-block:: html+php

    <!-- src/Acme/HelloBundle/Resources/views/Hello/index.html.php -->
    <?php $view->extend('AcmeHelloBundle::layout.html.php') ?>

    <?php $view['slots']->set('title', 'Applicazione Ciao mondo') ?>

    Ciao <?php echo $name ?>!

Il layout base ha già il codice per mostrare il titolo nella testata:

.. code-block:: html+php

    <!-- app/Resources/views/base.html.php -->
    <head>
        <meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
        <title><?php $view['slots']->output('title', 'Applicazione Ciao') ?></title>
    </head>

Il metodo ``output()`` inserisce il contenuto di uno slot e accetta un valore predefinito
opzionale, se lo slot non è definito. E ``_content`` è solo uno slot speciale che
contiene la resa del template figlio.

Per slot più grandi, si può usare una sintassi estesa:

.. code-block:: html+php

    <?php $view['slots']->start('title') ?>
        Un sacco di HTML
    <?php $view['slots']->stop() ?>

.. index::
   single: Template; Include

Includere altri template
------------------------

Il modo migliore di condividere un pezzo di codice di template è quello di definire un
template che possa essere incluso in altri template.

Creare un template ``hello.html.php``:

.. code-block:: html+php

    <!-- src/Acme/HelloBundle/Resources/views/Hello/hello.html.php -->
    Ciao <?php echo $name ?>!

E cambiare il template ``index.html.php`` per includerlo:

.. code-block:: html+php

    <!-- src/Acme/HelloBundle/Resources/views/Hello/index.html.php -->
    <?php $view->extend('AcmeHelloBundle::layout.html.php') ?>

    <?php echo $view->render('AcmeHello:Hello:hello.html.php', array('name' => $name)) ?>

Il metodo ``render()`` valuta e restituisce il contenuto di un altro template
(questo è esattamente lo stesso metodo usato nel controllore).

.. index::
   single: Template; Inserire pagine

Inserire altri controllori
--------------------------

Cosa fare se si vuole inserire il risultato di un altro controllore in un template?
Può essere molto utile lavorando con Ajax, oppure quando il template inserito ha bisogno
di variabili non disponibili nel template principale.

Se si crea un'azione ``fancy`` e la si vuole includere nel template
``index.html.php``, basta usare il seguente codice:

.. code-block:: html+php

    <!-- src/Acme/HelloBundle/Resources/views/Hello/index.html.php -->
    <?php echo $view['actions']->render(
        new \Symfony\Component\HttpKernel\Controller\ControllerReference('AcmeHelloBundle:Hello:fancy', array(
            'name'  => $name,
            'color' => 'green',
        ))
    ) ?>

Qui la stringa ``HelloBundle:Hello:fancy`` si riferisce all'azione ``fancy`` del
controllore ``Hello``::

    // src/Acme/HelloBundle/Controller/HelloController.php

    class HelloController extends Controller
    {
        public function fancyAction($name, $color)
        {
            // creare un oggetto basato sulla variabile $color
            $object = ...;

            return $this->render('AcmeHelloBundle:Hello:fancy.html.php', array(
                'name'   => $name,
                'object' => $object
            ));
        }

        // ...
    }

Ma dove è definito ``$view['actions']``? Come anche
``$view['slots']``, è chiamato aiutante dei template e sarà approfondito nella
prossima sezione.

.. index::
   single: Template; Aiutante

Usare gli aiutanti dei template
-------------------------------

Il sistema di template di Symfony2 può essere facilmente esteso tramite gli aiutanti.
Gli aiutanti sono oggetti PHP che forniscono caratteristiche utili nel contesto di un
template. ``actions`` e ``slots`` sono due degli aiutanti già disponibili in Symfony2.

Creare collegamenti tra le pagine
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Parlando di applicazioni web, non può mancare la creazione di collegamenti. Invece di
inserire a mano gli URL nei template, l'aiutante ``router`` sa come generare gli URL,
in base alla configurazione delle rotte. In questo modo, tutti gli URL possono essere
facilmente cambiati, cambiando la configurazione:

.. code-block:: html+php

    <a href="<?php echo $view['router']->generate('ciao', array('name' => 'Thomas')) ?>">
        Saluti Thomas!
    </a>

Il metodo ``generate()`` accetta come parametri il nome della rotta e un array di
parametri. Il nome della rotta è la chiave principale sotto cui le rotte sono
referenziate e i parametri sono i valori dei segnaposto definiti nello schema
della rotta:

.. code-block:: yaml

    # src/Acme/HelloBundle/Resources/config/routing.yml
    ciao: # Nome della rotta
        path:  /hello/{name}
        defaults: { _controller: AcmeHelloBundle:Hello:index }

Usare le risorse: immagini, JavaScript e fogli di stile
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Cosa sarebbe Internet senza immagini, JavaScript e fogli di stile?
Symfony2 fornisce il tag ``assets`` per gestirli facilmente:

.. code-block:: html+php

    <link href="<?php echo $view['assets']->getUrl('css/blog.css') ?>" rel="stylesheet" type="text/css" />

    <img src="<?php echo $view['assets']->getUrl('images/logo.png') ?>" />

Lo scopo principale dell'aiutante ``assets`` è quello di rendere l'applicazione più
portabile. Grazie a questo aiutante, si può spostare la cartella radice dell'applicazione
in qualsiasi punto sotto la cartella radice del web, senza dover cambiare nulla
nel codice dei template.

Profilare i template
~~~~~~~~~~~~~~~~~~~~

Usando l'aiutante ``stopwatch``, è possibile misurare i tempi di parti di template
e mostrarli sulla linea temporalre di WebProfilerBundle::

    <?php $view['stopwatch']->start('foo') ?>
    ... cose che richiedono tempo
    <?php $view['stopwatch']->stop('foo') ?>

.. tip::

    Se si usa lo stesso nome più volte in un template, i tempi
    saranno raggruppato su una stessa riga nella linea temporale.

Escape dell'output
------------------

Quando si usano i template PHP, occorre fare escape delle variabili mostrate
all'utente::

    <?php echo $view->escape($var) ?>

Per impostazione predefinita, il metodo ``escape()`` assume che la variabili sia inviata
in output in un contesto HTML. Il secondo parametro consente di cambiare il contesto.
Per esempio, per mandare in output qualcosa in uno script JavaScript, usare il contesto ``js``::

    <?php echo $view->escape($var, 'js') ?>

.. _`@Template`: http://symfony.com/doc/current/bundles/SensioFrameworkExtraBundle/annotations/view`
