La vista
========

Dopo aver letto la prima parte di questa guida, avete deciso che Symfony2
vale altri dieci minuti. Bene! In questa seconda parte, si imparerà di più
sul sistema dei template di Symfony2, `Twig`_. Twig è un sistema di template veloce,
flessibile e sicuro per PHP. Rende i propri template più leggibili e concisi e anche
più amichevoli per i designer.

Familiarizzare con Twig
-----------------------

La `documentazione`_ ufficiale di Twig è la migliore risorsa per sapere tutto su
questo motore di template. Questa sezione è solo un rapido sguardo ai
concetti principali.

Un template Twig è un file di test che può generare ogni tipo di contenuto (HTML,
XML, CSV, LaTeX, ...). Gli elementi di Twig sono separati dal resto del contenuto
del template tramite alcuni delimitatori:

* ``{{ ... }}``: Stampa una variabile o il risultato di un'espressione;

* ``{% ... %}``: Controlla la logica del template; è usato per eseguire dei cicli
  ``for`` e delle istruzioni ``if``, per esempio.

* ``{# ... #}``: consente l'inserimento di commenti all'interno dei template.

Segue un template minimale, che illustra alcune caratteristiche di base, usando due
variabili, ``page_title`` e ``navigation``, che dovrebbero essere passate al template:

.. code-block:: html+jinja

    <!DOCTYPE html>
    <html>
        <head>
            <title>{{ page_title }}</title>
        </head>
        <body>
            <h1>{{ page_title }}</h1>

            <ul id="navigation">
                {% for item in navigation %}
                    <li><a href="{{ item.url }}">{{ item.label }}</a></li>
                {% endfor %}
            </ul>
        </body>
    </html>

Per rendere un template in Symfony, usare il metodo ``render`` dal controllore e passargli
qualsiasi variabile necessaria al template::

    $this->render('AcmeDemoBundle:Demo:hello.html.twig', array(
        'name' => $name,
    ));

Le variabili passate a un template possono essere stringhe, array o anche oggetti. Twig
astrae le differenze tra essi e consente di accedere agli "attributi" di una variabie
con la notazione del punto (``.``). Il codice seguente mostra come
visualizzare il contenuto di una variabile, a seconda del tipo di variabile passata
dal controllore:

.. code-block:: jinja

    {# 1. Variabile semplice #}
    {# array('name' => 'Fabien') #}
    {{ name }}

    {# 2. Array #}
    {# array('user' => array('name' => 'Fabien')) #}
    {{ user.name }}

    {# sintassi alternativa per array #}
    {{ user['name'] }}

    {# 3. Oggetti #}
    {# array('user' => new User('Fabien')) #}
    {{ user.name }}
    {{ user.getName }}

    {# sintassi alternativa per oggetti #}
    {{ user.name() }}
    {{ user.getName() }}

Decorare i template
-------------------

Molto spesso, i template in un progetto condividono alcuni elementi comuni,
come i ben noti header e footer. In Twig, il problema è affrontato in modo diverso,
chiamato "ereditarietà dei template". Questo consente
di costruire un template di base, chiamato "layout", che contiene tutti gli elementi comuni
di un sito e definisce dei "blocchi", che i template figli possono sovrascrivere.

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
    <div>
        {% block content %}
        {% endblock %}
    </div>

I tag ``{% block %}`` dicono al sistema di template che un template figlio può
sovrascrivere quelle porzioni di template. In questo esempio, il template ``hello.html.twig``
sovrascrive il blocco ``content``, quindi il testo "Hello Fabien" viene
reso all'interno dell'elemento ``div``.

Usare tag, filtri e funzioni
----------------------------

Una delle migliori caratteristiche di Twig è la sua estensibilità tramite tag, filtri e
funzioni. Si veda nell'esempio seguente un template che usa filtri in modo estensivo,
per modificare le informazioni prima che siano mostrate all'utente:

.. code-block:: jinja

    <h1>{{ article.title|trim|capitalize }}</h1>

    <p>{{ article.content|striptags|slice(0, 1024) }}</p>

    <p>Tag: {{ article.tags|sort|join(", ") }}</p>

    <p>Il prossimo articolo sarà pubblicato il {{ 'next Monday'|date('M j, Y')}}</p>

Non dimenticare di dare uno sguardo alla `documentazione`_ ufficiale di Twig, per imparare
tutto su filtri, funzioni e tag.

Includere altri template
------------------------

Il modo migliore per condividere una parte di codice di un template è quello
di definire un template che possa essere incluso in altri template.

Creare un template ``embedded.html.twig``:

.. code-block:: jinja

    {# src/Acme/DemoBundle/Resources/views/Demo/embedded.html.twig #}
    Hello {{ name }}

E cambiare il template ``hello.html.twig`` per includerlo:

.. code-block:: jinja

    {# src/Acme/DemoBundle/Resources/views/Demo/hello.html.twig #}
    {% extends "AcmeDemoBundle::layout.html.twig" %}

    {# override the body block from embedded.html.twig #}
    {% block content %}
        {{ include("AcmeDemoBundle:Demo:embedded.html.twig") }}
    {% endblock %}

Inserire altri controllori
--------------------------

Cosa fare se si vuole inserire il risultato di un altro controllore in un template?
Può essere molto utile quando si lavora con Ajax o quando il template incluso necessita
di alcune variabili, non disponibili nel template principale.

Supponiamo di aver creato un metodo ``topArticlesAction`` in un controllore e di volerlo
"rendere" dentro al template ``index``, che vuol dire inserire il risultato
(cioè il codice HTML) del controllore. Per farlo, si usa la funzione
``render``:

.. code-block:: jinja

    {# src/Acme/DemoBundle/Resources/views/Demo/index.html.twig #}
    {{ render(controller("AcmeDemoBundle:Demo:topArticles", {'num': 10})) }}

Qui, la stringa ``AcmeDemoBundle:Demo:topArticles`` si riferisce all'azione
``topArticlesAction`` del controllore ``Demo``. Il parametro ``num``
è reso disponibile al controllore::

    // src/Acme/DemoBundle/Controller/DemoController.php

    class DemoController extends Controller
    {
        public function topArticlesAction($num)
        {
            // cercare i $num articoli più popolari nella base dati
            $articles = ...;

            return $this->render('AcmeDemoBundle:Demo:topArticles.html.twig', array(
                'articles' => $articles,
            ));
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

    <a href="{{ path('_demo_hello', { 'name': 'Thomas' }) }}">Ciao Thomas!</a>

La funzione  ``path`` accetta come parametri un nome di rotta e un array di parametri.
Il nome della rotta è la chiave principale sotto cui le rotte sono elencate e
i parametri sono i valori dei segnaposto definiti nello schema della rotta::

    // src/Acme/DemoBundle/Controller/DemoController.php
    use Sensio\Bundle\FrameworkExtraBundle\Configuration\Route;
    use Sensio\Bundle\FrameworkExtraBundle\Configuration\Template;

    // ...

    /**
     * @Route("/hello/{name}", name="_demo_hello")
     * @Template()
     */
    public function helloAction($name)
    {
        return array('name' => $name);
    }

.. tip::

    La funzione ``url`` è simile alla funzione ``path``, ma genera
    URL *assoluti*, il che è utile per rendere email o file RSS:
    ``{{ url('_demo_hello', {'name': 'Thomas'}) }}``.

Includere risorse: immagini, JavaScript e fogli di stile
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Cosa sarebbe Internet senza immagini, JavaScript e fogli di stile?
Symfony2 fornisce la funzione ``asset`` per gestirli facilmente.

.. code-block:: jinja

    <link href="{{ asset('css/blog.css') }}" rel="stylesheet" type="text/css" />

    <img src="{{ asset('images/logo.png') }}" />

Lo scopo principale della funzione ``asset`` è quello di rendere le
applicazioni maggiormente portabili. Grazie a questa funzione, si
può spostare la cartella radice dell'applicazione ovunque, sotto la cartella
radice del web, senza cambiare nulla nel codice dei template.

Considerazioni finali
---------------------

Twig è semplice ma potente. Grazie a layout, blocchi, template e inclusioni
di azioni, è molto facile organizzare i template in un modo logico ed
estensibile. Tuttavia, chi non si trova a suo agio con Twig può sempre usare
i template PHP in Symfony, senza problemi.

Stiamo lavorando con Symfony2 da soli venti minuti e già siamo
in grado di fare cose incredibili. Questo è il potere di Symfony2.
Imparare le basi è facile e si imparerà presto che questa
facilità è nascosta sotto un'architettura molto flessibile.

Ma non corriamo troppo. Prima occorre imparare di più sul
controllore e questo è esattamente l'argomento della :doc:`prossima parte di questa guida <the_controller>`.
Pronti per altri dieci minuti di Symfony2?

.. _Twig:           http://twig.sensiolabs.org/
.. _documentazione: http://twig.sensiolabs.org/documentation
