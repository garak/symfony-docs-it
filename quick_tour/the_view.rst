La vista
========

Dopo aver letto la prima parte di questa guida, avete deciso che Symfony2
vale altri dieci minuti. Bene! In questa seconda parte, si imparerà di più
sul sistema dei template di Symfony2, `Twig`_. Twig è un sistema di template veloce,
flessibile e sicuro per PHP. Rende i propri template più leggibili e concisi e anche
più amichevoli per i designer.

.. note::

    Invece di Twig, si può anche usare :doc:`PHP </cookbook/templating/PHP>`
    per i proprio template. Entrambi i sistemi di template sono supportati da Symfony2.

Familiarizzare con Twig
-----------------------

.. tip::

    Se si vuole imparare Twig, suggeriamo caldamente di leggere la sua 
    `documentazione`_ ufficiale. Questa sezione è solo un rapido sguardo ai
    concetti principali.

Un template Twig è un file di test che può generare ogni tipo di contenuto (HTML,
XML, CSV, LaTeX, ...). Twig definisce due tipi di delimitatori:

* ``{{ ... }}``: Stampa una variabile o il risultato di un'espressione;

* ``{% ... %}``: Controlla la logica del template; è usato per eseguire dei cicli
  ``for`` e delle istruzioni ``if``, per esempio.

Segue un template minimale, che illustra alcune caratteristiche di base, usando due
variabili, ``page_title`` e ``navigation``, che dovrebbero essere passate al template:

.. code-block:: html+jinja

    <!DOCTYPE html>
    <html>
        <head>
            <title>My Webpage</title>
        </head>
        <body>
            <h1>{{ page_title }}</h1>

            <ul id="navigation">
                {% for item in navigation %}
                    <li><a href="{{ item.href }}">{{ item.caption }}</a></li>
                {% endfor %}
            </ul>
        </body>
    </html>


.. tip::

   Si possono inserire commenti nei template, usando i delimitatori ``{# ... #}``.

Per rendere un template in Symfony, usare il metodo ``render`` dal controllore e passargli
qualsiasi variabile necessaria al template::

    $this->render('AcmeDemoBundle:Demo:hello.html.twig', array(
        'name' => $name,
    ));

Le variabili passate a un template possono essere stringhe, array o anche oggetti. Twig
astrae le differenze tra essi e consente di accedere agli "attributi" di una variabie
con la notazione del punto (``.``):

.. code-block:: jinja

    {# array('name' => 'Fabien') #}
    {{ name }}

    {# array('user' => array('name' => 'Fabien')) #}
    {{ user.name }}

    {# forza la ricerca nell'array #}
    {{ user['name'] }}

    {# array('user' => new User('Fabien')) #}
    {{ user.name }}
    {{ user.getName }}

    {# forza la ricerca del nome del metodo #}
    {{ user.name() }}
    {{ user.getName() }}

    {# passa parametri a un metodo #}
    {{ user.date('Y-m-d') }}

.. note::

    È importante sapere che le parentesi graffe non sono parte della variabile,
    ma istruzioni di stampa. Se si accede alle variabili dentro ai tag, non inserire
    le parentesi graffe.

Decorare i template
-------------------

Molto spesso, i template in un progetto condividono alcuni elementi comuni,
come i ben noti header e footer. In Symfony2, il problema è affrontato in
modo diverso: un template può essere decorato da un altro template.
Funziona esattamente come nelle classi di PHP: l'ereditarietà dei template consente
di costruire un template di base "layout", che contiene tutti gli elementi comuni
del proprio sito e definisce dei "blocchi", che i template figli possono sovrascrivere.

Il template ``hello.html.twig`` eredita da ``layout.html.twig``, grazie al tag
``extends``:

.. code-block:: html+jinja

    {# src/Acme/DemoBundle/Resources/views/Demo/hello.html.twig #}
    {% extends "AcmeDemoBundle::layout.html.twig" %}

    {% block title "Hello " ~ name %}

    {% block content %}
        <h1>Hello {{ name }}!</h1>
    {% endblock %}

La notazione ``AcmeDemoBundle::layout.html.twig`` suona familiare, non è vero? È la
stessa notazione usata per riferirsi a un template. La parte ``::`` vuol
dire semplicemente che l'elemento controllore è vuoto, quindi il file
corrispondente si trova direttamente sotto la cartella ``Resources/views/``.

Diamo ora un'occhiata a una versione semplificata di ``layout.html.twig``:

.. code-block:: jinja

    {# src/Acme/DemoBundle/Resources/views/layout.html.twig #}
    <div class="symfony-content">
        {% block content %}
        {% endblock %}
    </div>

I tag ``{% block %}`` definiscono blocchi che i template figli possono riempire.
Tutto ciò che fa un tag di blocco è dire al sistema di template che un template figlio
può sovrascrivere quelle porzioni di template.

In questo esempio, il template ``hello.html.twig`` sovrascrive il blocco ``content``,
quindi il testo "Hello Fabien" viene reso all'interno dell'elemento
``div.symfony-content``.

Usare tag, filtri e funzioni
----------------------------

Una delle migliori caratteristiche di Twig è la sua estensibilità tramite tag, filtri e
funzioni. Symfony2 ha dei bundle con molti di questi, per facilitare il lavoro dei
designer.

Includere altri template
------------------------

Il modo migliore per condividere una parte di codice di un template è quello
di definire un template che possa essere incluso in altri template.

Creare un template ``embedded.html.twig``:

.. code-block:: jinja

    {# src/Acme/DemoBundle/Resources/views/Demo/embedded.html.twig #}
    Hello {{ name }}

E cambiare il template ``index.html.twig`` per includerlo:

.. code-block:: jinja

    {# src/Acme/DemoBundle/Resources/views/Demo/hello.html.twig #}
    {% extends "AcmeDemoBundle::layout.html.twig" %}

    {# override the body block from embedded.html.twig #}
    {% block content %}
        {% include "AcmeDemoBundle:Demo:embedded.html.twig" %}
    {% endblock %}

Inserire altri controllori
--------------------------

Cosa fare se si vuole inserire il risultato di un altro controllore in un
template? Può essere molto utile quando si lavora con Ajax o quando il
template incluso necessita di alcune variabili, non disponibili nel template principale.

Se si crea un'azione ``fancy`` e la si vuole includere nel template
``index``, basta usare il tag ``render``:

.. code-block:: jinja

    {# src/Acme/DemoBundle/Resources/views/Demo/index.html.twig #}
    {% render "AcmeDemoBundle:Demo:fancy" with { 'name': name, 'color': 'green' } %}

Qui la stringa ``AcmeDemoBundle:Demo:fancy`` si riferisce all'azione ``fancy``
del controllore ``Demo``. I parametri (``name`` e ``color``) si comportano come
variabili di richiesta simulate (come se ``fancyAction`` stesse gestendo una richiesta
del tutto nuova) e sono rese disponibili al controllore::

    // src/Acme/DemoBundle/Controller/DemoController.php

    class DemoController extends Controller
    {
        public function fancyAction($name, $color)
        {
            // create some object, based on the $color variable
            $object = ...;

            return $this->render('AcmeDemoBundle:Demo:fancy.html.twig', array('name' => $name, 'object' => $object));
        }

        // ...
    }

Creare collegamenti tra le pagine
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Parlando di applicazioni web, i collegamenti tra pagine sono una parte
essenziale. Invece di inserire a mano gli URL nei template, la funzione
``path`` sa come generare URL in base alla configurazione delle rotte. In questo
modo, tutti gli URL saranno facilmente aggiornati al cambiare della configurazione:

.. code-block:: html+jinja

    <a href="{{ path('_demo_hello', { 'name': 'Thomas' }) }}">Greet Thomas!</a>

La funzione  ``path()`` accetta come parametri un nome di rotta e un
array di parametri. Il nome della rotta è la chiave principale sotto
cui le rotte sono elencate e i parametri sono i valori dei segnaposto
definiti nello schema della rotta::

    // src/Acme/DemoBundle/Controller/DemoController.php
    use Sensio\Bundle\FrameworkExtraBundle\Configuration\Route;
    use Sensio\Bundle\FrameworkExtraBundle\Configuration\Template;

    /**
     * @Route("/hello/{name}", name="_demo_hello")
     * @Template()
     */
    public function helloAction($name)
    {
        return array('name' => $name);
    }

.. tip::

    La funzione ``url`` genera URL *assoluti*: ``{{ url('_demo_hello', {
    'name': 'Thomas' }) }}``.

Includere risorse: immagini, JavaScript e fogli di stile
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Cosa sarebbe Internet senza immagini, JavaScript e fogli di stile?
Symfony2 fornisce la funzione ``asset`` per gestirli facilmente.

.. code-block:: jinja

    <link href="{{ asset('css/blog.css') }}" rel="stylesheet" type="text/css" />

    <img src="{{ asset('images/logo.png') }}" />

Lo scopo principale della funzione ``asset`` è quello di rendere le
applicazioni maggiormente portabili. Grazie a questa funzione, si
può spostare la cartella radice dell'applicazione ovunque, sotto la propria cartella
radice del web, senza cambiare nulla nel codice dei template.

Escape delle variabili
----------------------

Twig è configurato in modo predefinito per l'escape automatico di ogni output. Si legga
la `documentazione`_ di Twig per sapere di più sull'escape dell'output e sull'estensione
Escaper.

Considerazioni finali
---------------------

Twig è semplice ma potente. Grazie a layout, blocchi, template e inclusioni
di azioni, è molto facile organizzare i propri template in un modo logico ed
estensibile. Tuttavia, chi non si trova a proprio agio con Twig può sempre usare
i template PHP in Symfony, senza problemi.

Stiamo lavorando con Symfony2 da soli venti minuti e già siamo
in grado di fare cose incredibili. Questo è il potere di Symfony2.
Imparare le basi è facile e si imparerà presto che questa
facilità è nascosta sotto un'architettura molto flessibile.

Ma non corriamo troppo. Prima occorre imparare di più sul
controllore e questo è esattamente l'argomento della :doc:`prossima parte di questa guida<the_controller>`.
Pronti per altri dieci minuti di Symfony2?

.. _Twig:           http://twig.sensiolabs.org/
.. _documentazione: http://twig.sensiolabs.org/documentation
