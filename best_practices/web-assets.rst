Risorse pubbliche
=================

Le risorse web sono i fogli di stile CSS, file JavaScript e immagini che si utilizzano nel
frontend per renderlo accattivante. Gli sviluppatori Symfony, solitamente, mettevano le risorse
nella cartella ``Resources/public/`` di ogni bundle.

.. best-practice::

    Inserire gli asset nella cartella ``web/`` dell'applicazione.

Sparpagliare risorse tra decine di bundle rende il tutto più difficile da gestire.
La vita dei grafici sarebbe molto più facile se tutte le risorse dell'applicazione
fossero in un'unica posizione.

Centralizzando gli asset anche i template ne beneficerebbero; i link infatti
sarebbero molto più corti:

.. code-block:: html+jinja

    <link rel="stylesheet" href="{{ asset('css/bootstrap.min.css') }}" />
    <link rel="stylesheet" href="{{ asset('css/main.css') }}" />

    {# ... #}

    <script src="{{ asset('js/jquery.min.js') }}"></script>
    <script src="{{ asset('js/bootstrap.min.js') }}"></script>

.. note::

    Ricordarsi che la cartella ``web/`` è pubblica e che qualsiasi cosa salvata in essa
    sarà pubblicamente accessibile. Per questo motivo, si dovrebbero mettere qui tutte le risorse
    compilate, ma non i relativi file sorgente (come i file SASS).

Usare Assetic
-------------

Oggigiorno è praticamente impossibile trovare siti web che utilizzano solamente pochi file
statici CSS e JavaScript. È molto probabile che i progetto utilizzi invece molti file JavaScript
e diversi file Sass o LESS per la generazione dei CSS. Per migliorare le prestazioni lato client,
tutti questi file andrebbero quindi raggruppati e
minimizzati.

Esistono molti strumenti per risolvere questi problemi, come ad esempio GruntJS,
uno strumento progettato per il frontend (ma che non usa PHP).

.. best-practice::

    Usare Assetic per compilare, raggruppare e minimizzare i web asset, a meno che
    non si abbia dimestichezza con strumenti come GruntJS.

:doc:`Assetic </cookbook/assetic/asset_management>` è un asset manager in grado di
compilare risorse sviluppate con diverse tecnologie di frontend
come LESS, Sass e CoffeScript.
È possibile raggruppare tutti gli asset con Assetic, inserendoli in un
unico tag Twig:

.. code-block:: html+jinja

    {% stylesheets
        'css/bootstrap.min.css'
        'css/main.css'
        filter='cssrewrite' output='css/compiled/all.css' %}
        <link rel="stylesheet" href="{{ asset_url }}" />
    {% endstylesheets %}

    {# ... #}

    {% javascripts
        'js/jquery.min.js'
        'js/bootstrap.min.js'
        output='js/compiled/all.js' %}
        <script src="{{ asset_url }}"></script>
    {% endjavascripts %}

Applicazioni di Frontend
------------------------

Recentemente, tecnologie di frontend come AngularJS sono diventate molto popolari nello sviluppo
di applicazioni web. Tali applicazioni comunicano con il sistema tramite API.

Se si sta sviluppando un'applicazione come questa, si dovrebbero usare strumenti raccomandati
dalla tecnologia, come Bower e GruntJS. Inoltre, si dovrebbe sviluppare l'applicazione frontend
in modo del tutto separato dal backend Symfony (anche separando
i repository, eventualmente).

Saperne di più su Assetic
-------------------------

Assetic è in grado di migliorare la velocità dei siti mininizzarndo asset CSS e Javascript
tramite :doc:`UglifyCSS/UglifyJS </cookbook/assetic/uglifyjs>`.
È possibile anche :doc:`comprimere immagini </cookbook/assetic/jpeg_optimize>`,
riducendone la dimensione prima di essere restituiti nelle richieste.
Per saperne di più su tutte le funzionalità disponibili, fare riferimento alla
`documentazione ufficiale di Assetic`_.

.. _`documentazione ufficiale di Assetic`: https://github.com/kriswallsmith/assetic
