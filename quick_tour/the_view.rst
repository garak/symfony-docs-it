La vista
========

Dopo aver letto la prima parte di questa guida, avete deciso che Symfony
vale altri dieci minuti. Bene! In questa seconda parte, si imparerà di più
sul sistema dei template di Symfony, `Twig`_. Twig è un sistema di template veloce,
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

``{{ ... }}``
    Stampa una variabile o il risultato di un'espressione;

``{% ... %}``
    Controlla la logica del template; è usato per eseguire dei cicli ``for``
    e delle istruzioni ``if``, per esempio.

``{# ... #}``
    consente l'inserimento di commenti all'interno dei template. Diversamente dai commenti HTML,
    non sono inclusi nel template reso.

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

Per rendere un template in Symfony, usare il metodo ``render`` dal controllore. Se il template
ha bisogno di variabili per generare i suoi contenuti, passarle usando un array
come secondo parametro::

    $this->render('default/index.html.twig', array(
        'variable_name' => 'variable_value',
    ));

Le variabili passate a un template possono essere stringhe, array o anche oggetti. Twig
astrae le differenze tra essi e consente di accedere agli "attributi" di una variabile
con la notazione del punto (``.``). Il codice seguente mostra come visualizzare il
contenuto di una variabile, a seconda del tipo di variabile passata dal controllore, a
seconda del tipo:

.. code-block:: jinja

    {# 1. Variabile semplice #}
    {# $this->render('template.html.twig', array(
           'name' => 'Fabien')
       ) #}
    {{ name }}

    {# 2. Array #}
    {# $this->render('template.html.twig', array(
           'user' => array('name' => 'Fabien'))
       ) #}
    {{ user.name }}

    {# sintassi alternativa per array #}
    {{ user['name'] }}

    {# 3. Oggetti #}
    {# $this->render('template.html.twig', array(
           'user' => new User('Fabien'))
       ) #}
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

Il template ``index.html.twig`` eredita da ``base.html.twig``, grazie al tag
``extends``:

.. code-block:: html+jinja

    {# app/Resources/views/default/index.html.twig #}
    {% extends 'base.html.twig' %}

    {% block body %}
        <h1>Benvenuto in Symfony!</h1>
    {% endblock %}

Nel file ``app/Resources/views/base.html.twig``, che corrisponde al template
``base.html.twig``, si troverà il seguente codice Twig:

.. code-block:: html+jinja

    {# app/Resources/views/base.html.twig #}
    <!DOCTYPE html>
    <html>
        <head>
            <meta charset="UTF-8" />
            <title>{% block title %}Welcome!{% endblock %}</title>
            {% block stylesheets %}{% endblock %}
            <link rel="icon" type="image/x-icon" href="{{ asset('favicon.ico') }}" />
        </head>
        <body>
            {% block body %}{% endblock %}
            {% block javascripts %}{% endblock %}
        </body>
    </html>

I tag ``{% block %}`` dicono al sistema di template che un template figlio può
sovrascrivere quelle porzioni di template. In questo esempio, il template
``index.html.twig`` sovrascrive il blocco ``body``, ma non il blocco
``title``, che mostrerà il contenuto definito nel template
``base.html.twig``.

Usare tag, filtri e funzioni
----------------------------

Una delle migliori caratteristiche di Twig è la sua estensibilità tramite tag, filtri e
funzioni. Si veda nell'esempio seguente un template che usa filtri in modo estensivo,
per modificare le informazioni prima che siano mostrate all'utente:

.. code-block:: jinja

    <h1>{{ article.title|capitalize }}</h1>

    <p>{{ article.content|striptags|slice(0, 255) }} ...</p>

    <p>Tag: {{ article.tags|sort|join(", ") }}</p>

    <p>Il prossimo articolo sarà pubblicato il {{ 'next Monday'|date('M j, Y') }}</p>

Non dimenticare di dare uno sguardo alla `documentazione`_ ufficiale di Twig, per imparare
tutto su filtri, funzioni e tag.

Includere altri template
------------------------

Il modo migliore per condividere una parte di codice di un template è quello
di definire un template che possa essere incluso in altri template.

Si immagini di voler mostrare delle pubblicità su alcune pagine dell'applicazione.
Creare un template ``banner.html.twig``:

.. code-block:: jinja

    {# app/Resources/views/ads/banner.html.twig #}
    <div id="ad-banner">
        ...
    </div>

Per mostrare la pubblicità, includere il template ``banner.html.twig``, usando la
funzione ``include()``:

.. code-block:: html+jinja

    {# app/Resources/views/default/index.html.twig #}
    {% extends 'base.html.twig' %}

    {% block body %}
        <h1>Benvenuto in Symfony!</h1>

        {{ include('ads/banner.html.twig') }}
    {% endblock %}

Inserire altri controllori
--------------------------

Cosa fare se si vuole inserire il risultato di un altro controllore in un template?
Può essere molto utile quando si lavora con Ajax o quando il template incluso necessita
di alcune variabili, non disponibili nel template principale.

Supponiamo di aver creato un metodo ``topArticlesAction`` in un controllore e di volerlo
"rendere" dentro al template ``index``, che vuol dire inserire il risultato
(cioè il codice HTML) del controllore. Per farlo, si usa la funzione
``render()``:

.. code-block:: jinja

    {# app/Resources/views/index.html.twig #}
    {{ render(controller('AppBundle:Default:topArticles')) }}

Qui, le funzioni ``render()`` e ``controller()`` usano la sintassi speciale
``AppBundle:Default:topArticles`` per riferirsi all'azione ``topArticlesAction``
del controllore ``Default`` (la parte ``AppBundle`` è spiegata in
seguito)::

    // src/AppBundle/Controller/DefaultController.php

    class DefaultController extends Controller
    {
        public function topArticlesAction()
        {
            // cercare i $num articoli più popolari nella base dati
            $articles = ...;

            return $this->render('default/top_articles.html.twig', array(
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

    <a href="{{ path('homepage') }}">Torna alla homepage</a>

La funzione ``path`` accetta come parametri un nome di rotta e un array di parametri
opzionali.

.. tip::

    La funzione ``url`` è simile alla funzione ``path``, ma genera
    URL *assoluti*, il che è utile per rendere email o file RSS:
    ``<a href="{{ url('homepage') }}">Visita il nostro sito</a>``.

Includere risorse: immagini, JavaScript e fogli di stile
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Cosa sarebbe Internet senza immagini, JavaScript e fogli di stile?
Symfony fornisce la funzione ``asset`` per gestirli facilmente.

.. code-block:: jinja

    <link href="{{ asset('css/blog.css') }}" rel="stylesheet" type="text/css" />

    <img src="{{ asset('images/logo.png') }}" />

La funzione ``asset()`` cerca risorse web nella cartella ``web/``.
Se si memorizzano risorse in altre cartelle, leggere :doc:`questa ricetta </cookbook/assetic/asset_management>`
per sapere come gestirle.

Usando la funzione ``asset``, l'applicazione è maggiormente portabile. Grazie a questa funzione, si
può spostare la cartella radice dell'applicazione ovunque, sotto la cartella
radice del web, senza cambiare nulla nel codice dei template.

Considerazioni finali
---------------------

Twig è semplice ma potente. Grazie a layout, blocchi, template e inclusioni
di azioni, è molto facile organizzare i template in un modo logico ed
estensibile.

Pur lavorando con Symfony da soli venti minuti, si e già
in grado di fare cose incredibili. Questo è il potere di Symfony.
Imparare le basi è facile e si imparerà presto che questa
facilità è nascosta sotto un'architettura molto flessibile.

Ma non corriamo troppo. Prima occorre imparare di più sul
controllore e questo è esattamente l'argomento della :doc:`prossima parte di questa guida <the_controller>`.
Pronti per altri dieci minuti di Symfony?

.. _Twig:           http://twig.sensiolabs.org/
.. _documentazione: http://twig.sensiolabs.org/documentation
