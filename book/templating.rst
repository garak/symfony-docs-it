.. index::
   single: Template

Creare e usare i template
=========================

Come noto, il :doc:`controllore </book/controller>` è responsabile della
gestione di ogni richiesta che arriva a un'applicazione Symfony2. In realtà,
il controllore delega la maggior parte del lavoro pesante ad altri punti, in modo
che il codice possa essere testato e riusato. Quando un controllore ha bisogno di generare
HTML, CSS o ogni altro contenuto, passa il lavoro al motore dei template.
In questo capitolo si imparerà come scrivere potenti template, che possano essere
riusati per restituire del contenuto all'utente, popolare corpi di email e altro.
Si impareranno scorciatoie, modi intelligenti di estendere template e come riusare
il codice di un template

.. index::
   single: Template; Cos'è un template?

Template
--------

Un template è un semplice file testuale che può generare qualsiasi formato basato sul testo
(HTML, XML, CSV, LaTeX ...). Il tipo più familiare di template è un template *PHP*, un
file testuale analizzato da PHP che contiene un misto di testo e codice PHP:

.. code-block:: html+php

    <!DOCTYPE html>
    <html>
        <head>
            <title>Benvenuto in Symfony!</title>
        </head>
        <body>
            <h1><?php echo $page_title ?></h1>

            <ul id="navigation">
                <?php foreach ($navigation as $item): ?>
                    <li>
                        <a href="<?php echo $item->getHref() ?>">
                            <?php echo $item->getCaption() ?>
                        </a>
                    </li>
                <?php endforeach; ?>
            </ul>
        </body>
    </html>

.. index:: Twig; Introduzione

Ma Symfony2 possiede un linguaggio di template ancora più potente, chiamato `Twig`_.
Twig consente di scrivere template concisi e leggibili, più amichevoli per i grafici e,
in molti modi, più potenti dei template PHP:

.. code-block:: html+jinja

    <!DOCTYPE html>
    <html>
        <head>
            <title>Benvenuto in Symfony!</title>
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

Twig definisce due tipi di sintassi speciali:

* ``{{ ... }}``: "Dice qualcosa": stampa una variabile o il risultato di
  un'espressione nel template;

* ``{% ... %}``: "Fa qualcosa": un **tag** che controlla la logica del
  template; è usato per eseguire istruzioni, come il ciclo ``for`` dell'esempio.

.. note::

   C'è una terza sintassi usata per creare commenti: ``{# questo è un commento #}``.
   Questa sintassi può essere usata su righe multiple, come il suo equivalente PHP
   ``/* commento */``.

Twig contiene anche dei **filtri**, che modificano il contenuto prima che sia reso.
L'esempio seguente rende la variabile ``title`` tutta maiuscola, prima di
renderla:

.. code-block:: jinja

    {{ title | upper }}

Twig ha una lunga lista di `tag`_ e `filtri`_, disponibili in maniera
predefinita. Si possono anche `aggiungere le proprie estensioni`_ a Twig, se necessario.

.. tip::

    È facile registrare un'estensione di Twig: basta creare un nuovo servizio e
    assegnarli il :ref:`tag<book-service-container-tags>` ``twig.extension``.

Come vedremo nella documentazione, Twig supporta anche le funzioni e si possono
aggiungere facilmente nuove funzioni. Per esempio, di seguito viene usato un tag
standard ``for`` e la funzione ``cycle`` per stampare dieci tag div, con classi
alternate ``odd`` e ``even``:

.. code-block:: html+jinja

    {% for i in 0..10 %}
      <div class="{{ cycle(['odd', 'even'], i) }}">
        <!-- un po' di codice HTML -->
      </div>
    {% endfor %}

In questo capitolo, gli esempi dei template saranno mostrati sia in Twig che in PHP.

.. sidebar:: Perché Twig?

    I template di Twig sono pensati per essere semplici e non considerano i tag PHP. Questo
    è intenzionale: il sistema di template di Twig è fatto per esprimere una presentazione,
    non logica di programmazione. Più si usa Twig, più se ne può apprezzare benefici e
    distinzione. E, ovviamente, essere amati da tutti i grafici
    del mondo.

    Twig può anche far cose che PHP non può fare, come una vera ereditarietà dei template
    (i template Twig compilano in classi PHP che ereditano tra di loro), controllo
    degli spazi vuoti, sandbox e inclusione di funzioni e filtri personalizzati, che
    hanno effetti solo sui template. Twig possiede poche caratteristiche che rendono la
    scrittura di template più facile e concisa. Si prenda il seguente esempio,
    che combina un ciclo con un'istruzione logica ``if``:
    
    .. code-block:: html+jinja
    
        <ul>
            {% for user in users %}
                <li>{{ user.username }}</li>
            {% else %}
                <li>Nessun utente trovato</li>
            {% endfor %}
        </ul>

.. index::
   pair: Twig; Cache

Cache di template Twig
~~~~~~~~~~~~~~~~~~~~~~

Twig è veloce. Ogni template Twig è compilato in una classe nativa PHP, che viene resa
a runtime. Le classi compilate sono situate nella cartella
``app/cache/{environment}/twig`` (dove ``{environment}`` è l'ambiente, come
``dev`` o ``prod``) e in alcuni casi possono essere utili durante il
debug. Vedere :ref:`environments-summary` per maggiori informazioni sugli
ambienti.

Quando si abilita la modalità di ``debug`` (tipicamente in ambiente ``dev``), un
template Twig viene automaticamente ricompilato a ogni modifica subita. Questo
vuol dire che durante lo sviluppo si possono tranquillamente effettuare cambiamenti a
un template Twig e vedere immediatamente le modifiche, senza doversi preoccupare di
pulire la cache.

Quando la modalità di ``debug`` è disabilitata (tipicamente in ambiente ``prod``),
tuttavia, occorre pulire la cache di Twig, in modo che i template Twig siano
rigenerati. Si ricordi di farlo al deploy della propria applicazione.

.. index::
   single: Template; Ereditarietà

Ereditarietà dei template e layout
----------------------------------

Molto spesso, i template di un progetto condividono elementi comuni, come la
testata, il piè di pagina, una barra laterale e altro. In Symfony2, ci piace
pensare a questo problema in modo differente: un template può essere decorato da un
altro template. Funziona esattamente come per le classi PHP: l'ereditarietà dei template
consente di costruire un template "layout" di base, che contiene tutti gli elementi comuni
del proprio sito, definiti come **blocchi** (li si pensi come "classi PHP con metodi base").
Un template figlio può estendere un layout di base e sovrascrivere uno qualsiasi dei suoi
blocchi (li si pensi come "sottoclassi PHP che sovrascrivono alcuni metodi della classe genitrice").

Primo, costruire un file per il layout di base:

.. configuration-block::

    .. code-block:: html+jinja

        {# app/Resources/views/base.html.twig #}
        <!DOCTYPE html>
        <html>
            <head>
                <meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
                <title>{% block title %}Applicazione di test{% endblock %}</title>
            </head>
            <body>
                <div id="sidebar">
                    {% block sidebar %}
                    <ul>
                        <li><a href="/">Home</a></li>
                        <li><a href="/blog">Blog</a></li>
                    </ul>
                    {% endblock %}
                </div>

                <div id="content">
                    {% block body %}{% endblock %}
                </div>
            </body>
        </html>

    .. code-block:: php

        <!-- app/Resources/views/base.html.php -->
        <!DOCTYPE html>
        <html>
            <head>
                <meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
                <title><?php $view['slots']->output('title', 'Applicazione di test') ?></title>
            </head>
            <body>
                <div id="sidebar">
                    <?php if ($view['slots']->has('sidebar'): ?>
                        <?php $view['slots']->output('sidebar') ?>
                    <?php else: ?>
                        <ul>
                            <li><a href="/">Home</a></li>
                            <li><a href="/blog">Blog</a></li>
                        </ul>
                    <?php endif; ?>
                </div>

                <div id="content">
                    <?php $view['slots']->output('body') ?>
                </div>
            </body>
        </html>

.. note::

    Sebbene la discussione sull'ereditarietà dei template sia relativa a Twig,
    la filosofia è condivisa tra template Twig e template PHP.

Questo template definisce lo scheletro del documento HTML di base di una semplice pagina
a due colonne. In questo esempio, tre aree ``{% block %}`` sono definite (``title``,
``sidebar`` e ``body``). Ciascun blocco può essere sovrascritto da un template figlio o
lasciato alla sua implementazione predefinita. Questo template potrebbe anche essere
reso direttamente. In questo caso, i blocchi ``title``, ``sidebar`` e ``body``
manterrebbero semplicemente i valori predefiniti usati in questo template.

Un template figlio potrebbe assomigliare a questo:

.. configuration-block::

    .. code-block:: html+jinja

        {# src/Acme/BlogBundle/Resources/views/Blog/index.html.twig #}
        {% extends '::base.html.twig' %}

        {% block title %}I post fighi del mio blog{% endblock %}

        {% block body %}
            {% for entry in blog_entries %}
                <h2>{{ entry.title }}</h2>
                <p>{{ entry.body }}</p>
            {% endfor %}
        {% endblock %}

    .. code-block:: php

        <!-- src/Acme/BlogBundle/Resources/views/Blog/index.html.php -->
        <?php $view->extend('::base.html.php') ?>

        <?php $view['slots']->set('title', 'I post fighi del mio blog') ?>

        <?php $view['slots']->start('body') ?>
            <?php foreach ($blog_entries as $entry): ?>
                <h2><?php echo $entry->getTitle() ?></h2>
                <p><?php echo $entry->getBody() ?></p>
            <?php endforeach; ?>
        <?php $view['slots']->stop() ?>

.. note::

   Il template padre è identificato da una speciale sintassi di stringa
   (``::base.html.twig``) che indica che il template si trova nella cartella
   ``app/Resources/views`` del progetto. Questa convenzione di nomi è spiegata
   nel dettaglio in :ref:`template-naming-locations`.

La chiave dell'ereditarietà dei template è il tag ``{% extends %}``. Questo dice
al motore dei template di valutare prima il template base, che imposta il
layout e definisce i vari blocchi. Quindi viene reso il template figlio e i
blocchi ``title`` e ``body`` del padre vengono rimpiazzati da quelli del figlio.
A seconda del valore di ``blog_entries``, l'output potrebbe assomigliare a
questo:

.. code-block:: html

    <!DOCTYPE html>
    <html>
        <head>
            <meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
            <title>I post fighi del mio blog</title>
        </head>
        <body>
            <div id="sidebar">
                <ul>
                    <li><a href="/">Home</a></li>
                    <li><a href="/blog">Blog</a></li>
                </ul>
            </div>

            <div id="content">
                <h2>Il mio primo post</h2>
                <p>Il testo del primo post.</p>

                <h2>Un altro post</h2>
                <p>Il testo del secondo post.</p>
            </div>
        </body>
    </html>

Si noti che, siccome il template figlio non definisce un blocco ``sidebar``, viene
usato al suo posto il valore del template padre. Il contenuto di un tag ``{% block %}``
in un template padre è sempre usato come valore predefinito.

Si possono usare tanti livelli di ereditarietà quanti se ne desiderano. Nella prossima
sezione, sarà spiegato un modello comune a tre livelli di ereditarietà, insieme al modo
in cui i template sono organizzati in un progetto Symfony2.

Quando si lavora con l'ereditarietà dei template, ci sono alcuni concetti da tenere a mente:

* se si usa ``{% extends %}`` in un template, deve essere il primo tag di quel
  template.

* Più tag ``{% block %}`` si hanno in un template, meglio è.
  Si ricordi che i template figli non devono definire tutti i blocchi del padre,
  quindi si possono creare molti blocchi nei template base e dar loro dei valori
  predefiniti adeguati. Più blocchi si hanno in un template base, più sarà
  flessibile il layout.

* Se ci si trova ad aver duplicato del contenuto in un certo numero di template, vuol
  dire che probabilmente si dovrebbe spostare tale contenuto in un ``{% block %}`` di un
  template padre. In alcuni casi, una soluzione migliore potrebbe essere spostare il
  contenuto in un nuovo template e usare ``include`` (vedere :ref:`including-templates`).

* Se occorre prendere il contenuto di un blocco da un template padre, si può usare la
  funzione ``{{ parent() }}``. È utile quando si vuole aggiungere il contenuto di un
  template padre, invece di sovrascriverlo completamente:

    .. code-block:: html+jinja

        {% block sidebar %}
            <h3>Sommario</h3>
            ...
            {{ parent() }}
        {% endblock %}

.. index::
   single: Template; Convenzioni dei nomi
   single: Template; Posizioni dei file

.. _template-naming-locations:

Nomi e posizioni dei template
-----------------------------

Per impostazione predefinita, i template possono stare in una di queste posizioni:

* ``app/Resources/views/``: La cartella ``views`` di un'applicazione può contenere
  template di base a livello di applicazione (p.e. i layout dell'applicazione), ma anche
  template che sovrascrivono template di bundle (vedere
  :ref:`overriding-bundle-templates`);

* ``percorso/bundle/Resources/views/``: Ogni bundle ha i suoi template, nella sua
  cartella ``Resources/views`` (e nelle sotto-cartelle). La maggior parte dei template è
  dentro a un bundle.

Symfony2 usa una sintassi stringa **bundle**:**controllore**:**template** per i
template. Questo consente diversi tipi di template, ciascuno in un posto
specifico:

* ``AcmeBlogBundle:Blog:index.html.twig``: Questa sintassi è usata per specificare un
  template per una determinata pagina. Le tre parti della stringa, ognuna separata da
  due-punti (``:``), hanno il seguente significato:

    * ``AcmeBlogBundle``: (*bundle*) il template è dentro
      ``AcmeBlogBundle`` (p.e. ``src/Acme/BlogBundle``);

    * ``Blog``: (*controllore*) indica che il template è nella sotto-cartella
      ``Blog`` di ``Resources/views``;

    * ``index.html.twig``: (*template*) il nome del file è
      ``index.html.twig``.

  Ipotizzando che ``AcmeBlogBundle`` sia dentro ``src/Acme/BlogBundle``, il percorso
  finale del layout sarebbe ``src/Acme/BlogBundle/Resources/views/Blog/index.html.twig``.

* ``AcmeBlogBundle::layout.html.twig``: Questa sintassi si riferisce a un template di base
  specifico di ``AcmeBlogBundle``. Poiché la parte centrale, "controllore", manca,
  (p.e. ``Blog``), il template è ``Resources/views/layout.html.twig``
  dentro ``AcmeBlogBundle``.

* ``::base.html.twig``: Questa sintassi si riferisce a un template di base o a un
  layout di applicazione. Si noti che la stringa inizia con un doppio due-punti (``::``),
  il che vuol dire che mancano sia la parte del *bundle* che quella del *controllore*.
  Questo significa che il template non è in alcun bundle, ma invece nella cartella
  radice ``app/Resources/views/``.

Nella sezione :ref:`overriding-bundle-templates` si potrà trovare come ogni template
dentro ``AcmeBlogBundle``, per esempio, possa essere sovrascritto mettendo un
template con lo stesso nome nella cartella ``app/Resources/AcmeBlogBundle/views/``.
Questo dà la possibilità di sovrascrivere template di qualsiasi bundle.

.. tip::

    Si spera che la sintassi dei nomi risulti familiare: è la stessa convenzione di
    nomi usata per lo :ref:`controller-string-syntax`.

Suffissi dei template
~~~~~~~~~~~~~~~~~~~~~

Il formato **bundle**:**controllore**:**template** di ogni template specifica
*dove* il file del template si trova. Ogni nome di template ha anche due estensioni,
che specificano il *formato* e il *motore* per quel template.

* **AcmeBlogBundle:Blog:index.html.twig** - formato HTML, motore Twig

* **AcmeBlogBundle:Blog:index.html.php** - formato HTML, motore PHP

* **AcmeBlogBundle:Blog:index.css.twig** - formato CSS, motore Twig

Per impostazione predefinita, ogni template Symfony2 può essere scritto in Twig o in PHP,
e l'ultima parte dell'estensione (p.e. ``.twig`` o ``.php``) specifica quale
di questi due *motori* va usata. La prima parte dell'estensione,
(p.e. ``.html``, ``.css``, ecc.) è il formato finale che il template
genererà. Diversamente dal motore, che determina il modo in cui Symfony2 analizza il
template, si tratta di una tattica organizzativa usata nel caso in cui alcune risorse
debbano essere rese come HTML (``index.html.twig``), XML (``index.xml.twig``) o
in altri formati. Per maggiori informazioni, leggere la sezione
:ref:`template-formats`.

.. note::

   I "motori" disponibili possono essere configurati e se ne possono aggiungere di nuovi.
   Vedere :ref:`Configurazione dei template<template-configuration>` per maggiori dettagli.

.. index::
   single: Template Tag e helper
   single: Template; Helper

Tag e helper
------------

Dopo aver parlato delle basi dei template, di che nomi abbiano e di come si
possa usare l'ereditarietà, la parte più difficile è passata. In questa
sezione, si potranno conoscere un gran numero di strumenti disponibili per
aiutare a compiere i compiti più comuni sui template, come includere altri
template, collegare pagine e inserire immagini.

Symfony2 dispone di molti tag di Twig specializzati e di molte funzioni, che facilitano
il lavoro del progettista di template. In PHP, il sistema di template fornisce un
sistema estensibile di *helper*, che fornisce utili caratteristiche nel contesto
dei template.

Abbiamo già visto i tag predefiniti (``{% block %}`` e ``{% extends %}``),
così come un esempio di helper PHP (``$view['slots']``). Vediamone alcuni
altri.

.. index::
   single: Template; Includere altri template

.. _including-templates:

Includere altri template
~~~~~~~~~~~~~~~~~~~~~~~~

Spesso si vorranno includere lo stesso template o lo stesso pezzo di codice in
pagine diverse. Per esempio, in un'applicazione con "nuovi articoli", il codice
del template che mostra un articolo potrebbe essere usato sulla pagina dei dettagli
dell'articolo, un una pagina che mostra gli articoli più popolari o in una lista
degli articoli più recenti.

Quando occorre riusare un pezzo di codice PHP, tipicamente si posta il codice in una
nuova classe o funzione PHP. Lo stesso vale per i template. Spostando il codice del
template da riusare in un template a parte, può essere incluso in qualsiasi altro
template. Primo, creare il template che occorrerà riusare.

.. configuration-block::

    .. code-block:: html+jinja

        {# src/Acme/ArticleBundle/Resources/views/Article/articleDetails.html.twig #}
        <h2>{{ article.title }}</h2>
        <h3 class="byline">by {{ article.authorName }}</h3>

        <p>
          {{ article.body }}
        </p>

    .. code-block:: php

        <!-- src/Acme/ArticleBundle/Resources/views/Article/articleDetails.html.php -->
        <h2><?php echo $article->getTitle() ?></h2>
        <h3 class="byline">by <?php echo $article->getAuthorName() ?></h3>

        <p>
          <?php echo $article->getBody() ?>
        </p>

Includere questo template da un altro template è semplice:

.. configuration-block::

    .. code-block:: html+jinja

        {# src/Acme/ArticleBundle/Resources/Article/list.html.twig #}
        {% extends 'AcmeArticleBundle::layout.html.twig' %}

        {% block body %}
            <h1>Articoli recenti<h1>

            {% for article in articles %}
                {% include 'AcmeArticleBundle:Article:articleDetails.html.twig' with {'article': article} %}
            {% endfor %}
        {% endblock %}

    .. code-block:: php

        <!-- src/Acme/ArticleBundle/Resources/Article/list.html.php -->
        <?php $view->extend('AcmeArticleBundle::layout.html.php') ?>

        <?php $view['slots']->start('body') ?>
            <h1>Articoli recenti</h1>

            <?php foreach ($articles as $article): ?>
                <?php echo $view->render('AcmeArticleBundle:Article:articleDetails.html.php', array('article' => $article)) ?>
            <?php endforeach; ?>
        <?php $view['slots']->stop() ?>

Il template è incluso usando il tag ``{% include %}``. Si noti che il nome del
template segue le stesse tipiche convenzioni. Il template ``articleDetails.html.twig``
usa una variabile ``article``. Questa viene passata nel template ``list.html.twig``
usando il comando ``with``.

.. tip::

    La sintassi ``{'article': article}`` è la sintassi standard di Twig per gli
    array associativi (con chiavi non numeriche). Se avessimo avuto bisogno di passare più
    elementi, sarebbe stato così: ``{'pippo': pippo, 'pluto': pluto}``.

.. index::
   single: Template; Inserire azioni

.. _templating-embedding-controller:

Inserire controllori
~~~~~~~~~~~~~~~~~~~~

A volte occorre fare di più che includere semplici template. Si supponga di avere nel
proprio layout una barra laterale, che contiene i tre articoli più recenti.
Recuperare i tre articoli potrebbe implicare una query al database, o l'esecuzione
di altra logica, che non si può fare dentro a un template.

La soluzione è semplicemente l'inserimento del risultato di un intero controllore dal
proprio template. Primo, creare un controllore che rende un certo numero di
articoli recenti:

.. code-block:: php

    // src/Acme/ArticleBundle/Controller/ArticleController.php

    class ArticleController extends Controller
    {
        public function recentArticlesAction($max = 3)
        {
            // chiamare il database o altra logica per ottenere "$max" articoli recenti
            $articles = ...;

            return $this->render('AcmeArticleBundle:Article:recentList.html.twig', array('articles' => $articles));
        }
    }

Il template ``recentList`` è molto semplice:

.. configuration-block::

    .. code-block:: html+jinja

        {# src/Acme/ArticleBundle/Resources/views/Article/recentList.html.twig #}
        {% for article in articles %}
          <a href="/article/{{ article.slug }}">
              {{ article.title }}
          </a>
        {% endfor %}

    .. code-block:: php

        <!-- src/Acme/ArticleBundle/Resources/views/Article/recentList.html.php -->
        <?php foreach ($articles in $article): ?>
            <a href="/article/<?php echo $article->getSlug() ?>">
                <?php echo $article->getTitle() ?>
            </a>
        <?php endforeach; ?>

.. note::

    Si noti che abbiamo barato e inserito a mano l'URL dell'articolo in questo esempio
    (p.e. ``/article/*slug*``). Questa non è una buona pratica. Nella prossima sezione,
    vedremo come farlo correttamente.

Per includere il controllore, occorrerà fare riferimento a esso usando la sintassi
standard per i controllori (cioè **bundle**:**controllore**:**azione**):

.. configuration-block::

    .. code-block:: html+jinja

        {# app/Resources/views/base.html.twig #}
        ...

        <div id="sidebar">
            {% render "AcmeArticleBundle:Article:recentArticles" with {'max': 3} %}
        </div>

    .. code-block:: html+php

        <!-- app/Resources/views/base.html.php -->
        ...

        <div id="sidebar">
            <?php echo $view['actions']->render('AcmeArticleBundle:Article:recentArticles', array('max' => 3)) ?>
        </div>

Ogni volta che ci si trova ad aver bisogno di una variabile o di un pezzo di informazione
a cui non si ha accesso in un template, considerare di rendere un controllore.
I controllori sono veloci da eseguire e promuovono buona organizzazione e riuso del codice.

Contenuto asincrono con hinclude.js
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

.. versionadded:: 2.1
    il supporto per hinclude.js è stato aggiunto in Symfony 2.1

Si possono inserire controllori in modo asincrono, con la libreria hinclude.js_.
Poiché il contenuto incluso proviene da un'altra pagina (o da un altro controllore),
Symfony2 usa l'helper standard ``render`` per configurare i tag ``hinclude``:

.. configuration-block::

    .. code-block:: jinja

        {% render '...:news' with {}, {'standalone': 'js'} %}

    .. code-block:: php

        <?php echo $view['actions']->render('...:news', array(), array('standalone' => 'js')) ?>

.. note::

   hinclude.js_ deve essere incluso nella pagina.

Il contenuto predefinito (visibile durante il caricamento o senza JavaScript) può
essere impostato in modo globale nella configurazione dell'applicazione:

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        framework:
            # ...
            templating:
                hinclude_default_template: AcmeDemoBundle::hinclude.html.twig

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <framework:config>
            <framework:templating hinclude-default-template="AcmeDemoBundle::hinclude.html.twig" />
        </framework:config>

    .. code-block:: php

        // app/config/config.php
        $container->loadFromExtension('framework', array(
            // ...
            'templating'      => array(
                'hinclude_default_template' => array('AcmeDemoBundle::hinclude.html.twig'),
            ),
        ));

.. index::
   single: Template; Collegare le pagine

Collegare le pagine
~~~~~~~~~~~~~~~~~~~

Creare collegamenti alle altre pagine della propria applicazione è uno dei lavori più
comuni per un template. Invece di inserire a mano URL nei template, usare la funzione
``path`` di Twig (o l'helper ``router`` in PHP)  per generare URL basati sulla
configurazione delle rotte. Più avanti, se si vuole modificare l'URL di una particolare
pagina, tutto ciò di cui si avrà bisogno è cambiare la configurazione delle rotte: i
template genereranno automaticamente il nuovo URL.

Primo, collegare la pagina "_welcome", accessibile tramite la seguente configurazione
delle rotte:

.. configuration-block::

    .. code-block:: yaml

        _welcome:
            pattern:  /
            defaults: { _controller: AcmeDemoBundle:Welcome:index }

    .. code-block:: xml

        <route id="_welcome" pattern="/">
            <default key="_controller">AcmeDemoBundle:Welcome:index</default>
        </route>

    .. code-block:: php

        $collection = new RouteCollection();
        $collection->add('_welcome', new Route('/', array(
            '_controller' => 'AcmeDemoBundle:Welcome:index',
        )));

        return $collection;

Per collegare la pagina, usare la funzione ``path`` di Twig e riferirsi alla rotta:

.. configuration-block::

    .. code-block:: html+jinja

        <a href="{{ path('_welcome') }}">Home</a>

    .. code-block:: php

        <a href="<?php echo $view['router']->generate('_welcome') ?>">Home</a>

Come ci si aspettava, questo genererà l'URL ``/``. Vediamo come funziona con una
rotta più complessa:

.. configuration-block::

    .. code-block:: yaml

        article_show:
            pattern:  /article/{slug}
            defaults: { _controller: AcmeArticleBundle:Article:show }

    .. code-block:: xml

        <route id="article_show" pattern="/article/{slug}">
            <default key="_controller">AcmeArticleBundle:Article:show</default>
        </route>

    .. code-block:: php

        $collection = new RouteCollection();
        $collection->add('article_show', new Route('/article/{slug}', array(
            '_controller' => 'AcmeArticleBundle:Article:show',
        )));

        return $collection;

In questo caso, occorre specificare sia il nome della rotta (``article_show``) che
il valore del parametro ``{slug}``. Usando questa rotta, rivisitiamo il template
``recentList`` della sezione precedente e colleghiamo correttamente gli
articoli:

.. configuration-block::

    .. code-block:: html+jinja

        {# src/Acme/ArticleBundle/Resources/views/Article/recentList.html.twig #}
        {% for article in articles %}
          <a href="{{ path('article_show', { 'slug': article.slug }) }}">
              {{ article.title }}
          </a>
        {% endfor %}

    .. code-block:: html+php

        <!-- src/Acme/ArticleBundle/Resources/views/Article/recentList.html.php -->
        <?php foreach ($articles in $article): ?>
            <a href="<?php echo $view['router']->generate('article_show', array('slug' => $article->getSlug()) ?>">
                <?php echo $article->getTitle() ?>
            </a>
        <?php endforeach; ?>

.. tip::

    Si può anche generare un URL assoluto, usando la funzione ``url`` di Twig:

    .. code-block:: html+jinja

        <a href="{{ url('_welcome') }}">Home</a>

    Lo stesso si può fare nei template PHP, passando un terzo parametro al metodo
    ``generate()``:

    .. code-block:: php

        <a href="<?php echo $view['router']->generate('_welcome', array(), true) ?>">Home</a>

.. index::
   single: Template; Collegare le risorse

Collegare le risorse
~~~~~~~~~~~~~~~~~~~~

I template solitamente hanno anche riferimenti a immagini, Javascript, fogli di stile e
altre risorse. Certamente, si potrebbe inserire manualmente il percorso a tali risorse
(p.e. ``/images/logo.png``), ma Symfony2 fornisce un'opzione più dinamica, tramite la funzione ``asset`` di Twig:

.. configuration-block::

    .. code-block:: html+jinja

        <img src="{{ asset('images/logo.png') }}" alt="Symfony!" />

        <link href="{{ asset('css/blog.css') }}" rel="stylesheet" type="text/css" />

    .. code-block:: html+php

        <img src="<?php echo $view['assets']->getUrl('images/logo.png') ?>" alt="Symfony!" />

        <link href="<?php echo $view['assets']->getUrl('css/blog.css') ?>" rel="stylesheet" type="text/css" />

Lo scopo principale della funzione ``asset`` è rendere più portabile la propria
applicazione. Se la propria applicazione si trova nella radice del proprio host
(p.e. http://example.com), i percorsi resi dovrebbero essere del tipo ``/images/logo.png``. 
Se invece la propria applicazione si trova in una sotto-cartella (p.e.
http://example.com/my_app), ogni percorso dovrebbe includere la sotto-cartella
(p.e. ``/my_app/images/logo.png``). La funzione ``asset`` si prende cura di questi aspetti,
determinando in che modo è usata la propria applicazione e generando i percorsi adeguati.

Inoltre, se si usa la funzione ``asset``, Symfony può aggiungere automaticamente
un parametro all'URL della risorsa, per garantire che le risorse statiche aggiornate
non siano messe in cache. Per esempio, ``/images/logo.png`` potrebbe comparire come
``/images/logo.png?v2``. Per ulteriori informazioni, vedere l'opzione di
configurazione :ref:`ref-framework-assets-version`.

.. index::
   single: Template; Includere fogli di stile e Javascript
   single: Fogli di stile; Includere fogli di stile
   single: Javascript; Includere Javascript

Includere fogli di stile e Javascript in Twig
---------------------------------------------

Nessun sito sarebbe completo senza l'inclusione di file Javascript e fogli di stile.
In Symfony, l'inclusione di tali risorse è gestita elegantemente sfruttando
l'ereditarietà dei template.

.. tip::

    Questa sezione insegnerà la filosofia che sta dietro l'inclusione di fogli di stile
    e Javascript in Symfony. Symfony dispone di un'altra libreria, chiamata Assetic,
    che segue la stessa filosofia, ma consente di fare cose molto più interessanti
    con queste risorse. Per maggiori informazioni sull'uso di Assetic, vedere
    :doc:`/cookbook/assetic/asset_management`.


Iniziamo aggiungendo due blocchi al template di base, che conterranno le risorse:
uno chiamato ``stylesheets``, dentro al tag ``head``, e l'altro chiamato ``javascripts``,
appena prima della chiusura del tag ``body``. Questi blocchi conterranno tutti i fogli
di stile e i Javascript che occorrerano al sito:

.. code-block:: html+jinja

    {# 'app/Resources/views/base.html.twig' #}
    <html>
        <head>
            {# ... #}

            {% block stylesheets %}
                <link href="{{ asset('/css/main.css') }}" type="text/css" rel="stylesheet" />
            {% endblock %}
        </head>
        <body>
            {# ... #}

            {% block javascripts %}
                <script src="{{ asset('/js/main.js') }}" type="text/javascript"></script>
            {% endblock %}
        </body>
    </html>

È così facile! Ma che succede quando si ha bisogno di includere un foglio di stile o un
Javascript aggiuntivo in un template figlio? Per esempio, supponiamo di avere una pagina
di contatti e che occorra includere un foglio di stile ``contact.css`` *solo* su tale
pagina. Da dentro il template della pagina di contatti, fare come segue:

.. code-block:: html+jinja

    {# src/Acme/DemoBundle/Resources/views/Contact/contact.html.twig #}
    {# extends '::base.html.twig' #}

    {% block stylesheets %}
        {{ parent() }}
        
        <link href="{{ asset('/css/contact.css') }}" type="text/css" rel="stylesheet" />
    {% endblock %}
    
    {# ... #}

Nel template figlio, basta sovrascrivere il blocco ``stylesheets`` ed inserire
il nuovo tag del foglio di stile nel blocco stesso. Ovviamente, poiché vogliamo
aggiungere contenuto al blocco padre (e non *sostituirlo*), occorre usare la funzione
``parent()`` di Twig, per includere tutto ciò che sta nel blocco ``stylesheets``
del template di base.

Si possono anche includere risorse dalla cartella ``Resources/public`` del proprio bundle.
Occorre poi eseguire il comando ``php app/console assets:install target [--symlink]``,
che copia (o collega) i file nella posizione corretta (la posizione predefinita è sotto la
cartella "web").

.. code-block:: html+jinja

   <link href="{{ asset('bundles/acmedemo/css/contact.css') }}" type="text/css" rel="stylesheet" />

Il risultato finale è una pagina che include i fogli di stile ``main.css`` e
``contact.css``.

Variabili globali nei template
------------------------------

Durante ogni richiesta, Symfony2 imposta una variabile globale ``app``,
sia nei template Twig che in quelli PHP. La variabile ``app``
è un'istanza di :class:`Symfony\\Bundle\\FrameworkBundle\\Templating\\GlobalVariables`,
che dà accesso automaticamente ad alcune variabili specifiche
dell'applicazione:

* ``app.security`` - Il contesto della sicurezza.
* ``app.user`` - L'oggetto dell'utente attuale.
* ``app.request`` - L'oggetto richiesta.
* ``app.session`` - L'oggetto sessione.
* ``app.environment`` - L'ambiente attuale (dev, prod, ecc).
* ``app.debug`` - True se in debug. False altrimenti.

.. configuration-block::

    .. code-block:: html+jinja

        <p>Nome utente: {{ app.user.username }}</p>
        {% if app.debug %}
            <p>Metodo richiesta: {{ app.request.method }}</p>
            <p>Ambiente: {{ app.environment }}</p>
        {% endif %}

    .. code-block:: html+php

        <p>Nome utente: <?php echo $app->getUser()->getUsername() ?></p>
        <?php if ($app->getDebug()): ?>
            <p>Metodo richiesta: <?php echo $app->getRequest()->getMethod() ?></p>
            <p>Ambiente: <?php echo $app->getEnvironment() ?></p>
        <?php endif; ?>

.. tip::

    Si possono aggiungere le proprie variabili globali ai template. Si veda la
    ricetta :doc:`Variabili globali</cookbook/templating/global_variables>`.

.. index::
   single: Template; Il servizio templating

Configurare e usare il servizio ``templating``
----------------------------------------------

Il cuore del sistema dei template di Symfony2 è il motore dei template.
L'oggetto speciale ``Engine`` è responsabile della resa dei template e della
restituzione del loro contenuto. Quando si rende un template in un controllore,
per esempio, si sta in realtà usando il servizio del motore dei template. Per esempio:

.. code-block:: php

    return $this->render('AcmeArticleBundle:Article:index.html.twig');

equivale a

.. code-block:: php

    $engine = $this->container->get('templating');
    $content = $engine->render('AcmeArticleBundle:Article:index.html.twig');

    return $response = new Response($content);

.. _template-configuration:

Il motore (o "servizio") dei template è pre-configurato per funzionare automaticamente
dentro a Symfony2. Può anche essere ulteriormente configurato nel file di configurazione
dell'applicazione:

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        framework:
            # ...
            templating: { engines: ['twig'] }

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <framework:templating>
            <framework:engine id="twig" />
        </framework:templating>

    .. code-block:: php

        // app/config/config.php
        $container->loadFromExtension('framework', array(
            // ...
            'templating'      => array(
                'engines' => array('twig'),
            ),
        ));

Sono disponibili diverse opzioni di configurazione, coperte
nell':doc:`Appendice: configurazione</reference/configuration/framework>`.

.. note::

   Il motore ``twig`` è obbligatorio per poter usare il webprofiler (così come
   molti altri bundle di terze parti).

.. index::
    single; Template; Sovrascrivere template

.. _overriding-bundle-templates:

Sovrascrivere template dei bundle
---------------------------------

La comunità di Symfony2 si vanta di creare e mantenere bundle di alta
qualità (vedere `KnpBundles.com`_) per un gran numero di diverse caratteristiche.
Quando si usa un bundle di terze parti, probabilmente occorrerà sovrascrivere e
personalizzare uno o più dei suoi template.

Supponiamo di aver incluso l'immaginario bundle ``AcmeBlogBundle`` nel nostro
progetto (p.e. nella cartella ``src/Acme/BlogBundle``). Pur essendo soddisfatti,
vogliamo sovrascrivere la pagina "list" del blog, per personalizzare il codice e
renderlo specifico per la nostra applicazione. Analizzando il controllore
``Blog`` di ``AcmeBlogBundle``, troviamo::

    public function indexAction()
    {
        $blogs = // logica per recuperare i blog

        $this->render('AcmeBlogBundle:Blog:index.html.twig', array('blogs' => $blogs));
    }

Quando viene reso ``AcmeBlogBundle:Blog:index.html.twig``, Symfony2 cerca il template
in due diversi posti:

#. ``app/Resources/AcmeBlogBundle/views/Blog/index.html.twig``
#. ``src/Acme/BlogBundle/Resources/views/Blog/index.html.twig``

Per sovrascrivere il template del bundle, basta copiare il file ``index.html.twig``
dal bundle a ``app/Resources/AcmeBlogBundle/views/Blog/index.html.twig``
(la cartella ``app/Resources/AcmeBlogBundle`` non esiste ancora, quindi occorre
crearla). Ora si può personalizzare il template.

Questa logica si applica anche ai template base dei bundle. Supponiamo che ogni
template in ``AcmeBlogBundle`` erediti da un template base chiamato
``AcmeBlogBundle::layout.html.twig``. Esattamente come prima, Symfony2 cercherà
il template i questi due posti:

#. ``app/Resources/AcmeBlogBundle/views/layout.html.twig``
#. ``src/Acme/BlogBundle/Resources/views/layout.html.twig``

Anche qui, per sovrascrivere il template, basta copiarlo dal bundle a
``app/Resources/AcmeBlogBundle/views/layout.html.twig``. Ora lo si può
personalizzare.

Facendo un passo indietro, si vedrà che Symfony2 inizia sempre a cercare un
template nella cartella ``app/Resources/{NOME_BUNDLE}/views/``. Se il template
non c'è, continua verificando nella cartella ``Resources/views`` del bundle stesso.
Questo vuol dire che ogni template di bundle può essere sovrascritto, inserendolo
nella sotto-cartella ``app/Resources``
appropriata.

.. _templating-overriding-core-templates:

.. index::
    single; Template; Sovrascrivere template di eccezioni

Sovrascrivere template del nucleo
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Essendo il framework Symfony2 esso stesso un bundle, i template del nucleo
possono essere sovrascritti allo stesso modo. Per esempio, ``TwigBundle``
contiene diversi template "exception" ed "error", che possono essere sovrascritti,
copiandoli dalla cartella ``Resources/views/Exception`` di ``TwigBundle`` a,
come si può immaginare, la cartella
``app/Resources/TwigBundle/views/Exception``.

.. index::
   single: Template; Lo schema di ereditarietà a tre livelli

Ereditarietà a tre livelli
--------------------------

Un modo comune per usare l'ereditarietà è l'approccio a tre livelli.
Questo metodo funziona perfettamente con i tre diversi tipi di template
di cui abbiamo appena parlato:

* Creare un file ``app/Resources/views/base.html.twig`` che contenga il layout
  principale per la propria applicazione (come nell'esempio precedente). Internamente,
  questo template si chiama ``::base.html.twig``;

* Creare un template per ogni "sezione" del proprio sito. Per esempio, ``AcmeBlogBundle``
  avrebbe un template di nome ``AcmeBlogBundle::layout.html.twig``, che contiene solo
  elementi specifici alla sezione blog;

    .. code-block:: html+jinja

        {# src/Acme/BlogBundle/Resources/views/layout.html.twig #}
        {% extends '::base.html.twig' %}

        {% block body %}
            <h1>Applicazione blog</h1>

            {% block content %}{% endblock %}
        {% endblock %}

* Creare i singoli template per ogni pagina, facendo estendere il template della sezione
  appropriata. Per esempio, la pagina "index" avrebbe un nome come
  ``AcmeBlogBundle:Blog:index.html.twig`` e mostrerebbe la lista dei post del blog.

    .. code-block:: html+jinja

        {# src/Acme/BlogBundle/Resources/views/Blog/index.html.twig #}
        {% extends 'AcmeBlogBundle::layout.html.twig' %}

        {% block content %}
            {% for entry in blog_entries %}
                <h2>{{ entry.title }}</h2>
                <p>{{ entry.body }}</p>
            {% endfor %}
        {% endblock %}

Si noti che questo template estende il template di sezione (``AcmeBlogBundle::layout.html.twig``),
che a sua volte estende il layout base dell'applicazione (``::base.html.twig``).
Questo è il modello di ereditarietà a tre livelli.

Durante la costruzione della propria applicazione, si può scegliere di seguire questo
metodo oppure semplicemente far estendere direttamente a ogni template di pagina il
template base dell'applicazione (p.e. ``{% extends '::base.html.twig' %}``). Il modello
a tre template è una best practice usata dai bundle dei venditori, in modo che il
template base di un bundle possa essere facilmente sovrascritto per estendere correttamente
il layout base della propria applicazione.

.. index::
   single: Template; Escape dell'output

Escape dell'output
------------------

Quando si genera HTML da un template, c'è sempre il rischio che una variabile
possa mostrare HTML indesiderato o codice pericoloso lato client. Il risultato
è che il contenuto dinamico potrebbe rompere il codice HTML della pagina risultante
o consentire a un utente malintenzionato di eseguire un attacco `Cross Site Scripting`_
(XSS). Consideriamo questo classico esempio:

.. configuration-block::

    .. code-block:: jinja

        Ciao {{ name }}

    .. code-block:: php

        Ciao <?php echo $name ?>

Si immagini che l'utente inserisca nel suo nome il seguente codice::

    <script>alert('ciao!')</script>

Senza alcun escape dell'output, il template risultante causerebbe la comparsa
di una finestra di alert Javascript::

    Ciao <script>alert('ciao!')</script>

Sebbene possa sembrare innocuo, se un utente arriva a tal punto, lo stesso
utente sarebbe in grado di scrivere Javascript che esegua azioni dannose
all'interno dell'area di un utente legittimo e ignaro.

La risposta a questo problema è l'escape dell'output. Con l'escape attivo,
lo stesso template verrebbe reso in modo innocuo e scriverebbe alla lettera
il tag ``script`` su schermo::

    Ciao &lt;script&gt;alert(&#39;ciao!&#39;)&lt;/script&gt;

L'approccio dei sistemi di template Twig e PHP a questo problema sono diversi.
Se si usa Twig, l'escape è attivo in modo predefinito e si è al sicuro.
In PHP, l'escape dell'output non è automatico, il che vuol dire che occorre
applicarlo a mano, dove necessario.

Escape dell'output in Twig
~~~~~~~~~~~~~~~~~~~~~~~~~~

Se si usano i template Twig, l'escape dell'output è attivo in modo predefinito.
Questo vuol dire che si è protetti dalle conseguenze non intenzionali del codice
inviato dall'utente. Per impostazione predefinita, l'escape dell'output assume che
il contenuto sia sotto escape per l'output HTML.

In alcuni casi, si avrà bisogno di disabilitare l'escape dell'output, quando si avrà
bisogno di rendere una variabile affidabile che contiene markup. Supponiamo che gli
utenti amministratori siano abilitati a scrivere articoli che contengano codice HTML.
Per impostazione predefinita, Twig mostrerà l'articolo con escape. Per renderlo
normalmente, aggiungere il filtro ``raw``: ``{{ article.body | raw }}``.

Si può anche disabilitare l'escape dell'output dentro a un ``{% block %}`` o
per un intero template. Per maggiori informazioni, vedere `Escape dell'output`_ nella
documentazione di Twig.

Escape dell'output in PHP
~~~~~~~~~~~~~~~~~~~~~~~~~

L'escape dell'output non è automatico, se si usano i template PHP. Questo vuol dire che,
a meno che non scelga esplicitamente di passare una variabile sotto escape, non si è
protetti. Per usare l'escape, usare il metodo speciale ``escape()``::

    Ciao <?php echo $view->escape($name) ?>

Per impostazione predefinita, il metodo ``escape()`` assume che la variabile sia resa
in un contesto HTML (quindi l'escape renderà la variabile sicura per HTML).
Il secondo parametro consente di cambiare contesto. Per esempio per mostrare qualcosa
in una stringa Javascript, usare il contesto ``js``:

.. code-block:: js

    var myMsg = 'Ciao <?php echo $view->escape($name, 'js') ?>';

.. index::
   single: Template; Formati

.. _template-formats:

Debug
-----

.. versionadded:: 2.0.9
    Questa caratteristica è disponibile da Twig ``1.5.x``, che è stato aggiunto
    in Symfony 2.0.9.

Quando si usa PHP, si può ricorrere a ``var_dump()``, se occorre trovare rapidamente il
valore di una variabile passata. Può essere utile, per esempio, nel proprio controllore.
Si può ottenere lo stesso risultato con Twig, usando l'estensione debug. Occorre
abilitarla nella configurazione:

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        services:
            acme_hello.twig.extension.debug:
                class:        Twig_Extension_Debug
                tags:
                     - { name: 'twig.extension' }

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <services>
            <service id="acme_hello.twig.extension.debug" class="Twig_Extension_Debug">
                <tag name="twig.extension" />
            </service>
        </services>

    .. code-block:: php

        // app/config/config.php
        use Symfony\Component\DependencyInjection\Definition;

        $definition = new Definition('Twig_Extension_Debug');
        $definition->addTag('twig.extension');
        $container->setDefinition('acme_hello.twig.extension.debug', $definition);

Si può quindi fare un dump dei parametri nei template, usando la funzione ``dump``:

.. code-block:: html+jinja

    {# src/Acme/ArticleBundle/Resources/views/Article/recentList.html.twig #}

    {{ dump(articles) }}

    {% for article in articles %}
        <a href="/article/{{ article.slug }}">
            {{ article.title }}
        </a>
    {% endfor %}


Il dump delle variabili avverrà solo se l'impostazione ``debug`` (in ``config.yml``)
è ``true``. Questo vuol dire che, per impostazione predefinita, il dump avverrà in
ambiente ``dev``, ma non in ``prod``.

Formati di template
-------------------

I template sono un modo generico per rendere contenuti in *qualsiasi* formato. Pur usando
nella maggior parte dei casi i template per rendere contenuti HTML, un template può
generare altrettanto facilmente Javascript, CSS, XML o qualsiasi altro formato desiderato.

Per esempio, la stessa "risorsa" spesso è resa in molti formati diversi.
Per rendere una pagina in XML, basta includere il formato nel nome del
template:

* *nome del template XML*: ``AcmeArticleBundle:Article:index.xml.twig``
* *nome del file del template XML*: ``index.xml.twig``

In realtà, questo non è niente più che una convenzione sui nomi e il template
non è effettivamente resto in modo diverso in base al suo formato.

In molti casi, si potrebbe voler consentire a un singolo controllore di rendere
formati diversi, in base al "formato di richiesta". Per questa ragione, una
soluzione comune è fare come segue:

.. code-block:: php

    public function indexAction()
    {
        $format = $this->getRequest()->getRequestFormat();
    
        return $this->render('AcmeBlogBundle:Blog:index.'.$format.'.twig');
    }

Il metodo ``getRequestFormat`` dell'oggetto ``Request`` ha come valore predefinito ``html``,
ma può restituire qualsiasi altro formato, in base al formato richiesto dall'utente.
Il formato di richiesta è spesso gestito dalle rotte, quando una rotta è
configurata in modo che ``/contact`` imposti il formato di richiesta a ``html``,
mentre ``/contact.xml`` lo imposti a ``xml``. Per maggiori informazioni, vedere
:ref:`Esempi avanzati nel capitolo delle rotte <advanced-routing-example>`.

Per creare collegamenti che includano il formato, usare la chiave ``_format``
come parametro:

.. configuration-block::

    .. code-block:: html+jinja

        <a href="{{ path('article_show', {'id': 123, '_format': 'pdf'}) }}">
            versione PDF
        </a>

    .. code-block:: html+php

        <a href="<?php echo $view['router']->generate('article_show', array('id' => 123, '_format' => 'pdf')) ?>">
            versione PDF
        </a>

Considerazioni finali
---------------------

Il motore dei template in Symfony è un potente strumento, che può essere usato ogni
volta che occorre generare contenuto relativo alla presentazione in HTML, XML o altri
formati. Sebbene i template siano un modo comune per generare contenuti in un
controllore, i loro utilizzo non è obbligatorio. L'oggetto ``Response`` restituito da
un controllore può essere creato con o senza l'uso di un template:

.. code-block:: php

    // crea un oggetto Response il cui contenuto è il template reso
    $response = $this->render('AcmeArticleBundle:Article:index.html.twig');

    // crea un oggetto Response il cui contenuto è semplice testo
    $response = new Response('contenuto della risposta');

Il motore dei template di Symfony è molto flessibile e mette a disposizione due
sistemi di template: i tradizionali template *PHP* e i potenti e raffinati
template *Twig*. Entrambi supportano una gerarchia di template e sono distribuiti
con un ricco insieme di funzioni helper, capaci di eseguire i compiti più
comuni.

Complessivamente, l'argomento template dovrebbe essere considerato come un potente
strumento a disposizione. In alcuni casi, si potrebbe non aver bisogno di rendere un
template , in Symfony2, questo non è assolutamente un problema.

Imparare di più con il ricettario
---------------------------------

* :doc:`/cookbook/templating/PHP`
* :doc:`/cookbook/controller/error_pages`
* :doc:`/cookbook/templating/twig_extension`

.. _`Twig`: http://twig.sensiolabs.org
.. _`KnpBundles.com`: http://knpbundles.com
.. _`Cross Site Scripting`: http://it.wikipedia.org/wiki/Cross-site_scripting
.. _`Escape dell'output`: http://twig.sensiolabs.org
.. _`tag`: http://twig.sensiolabs.org/doc/tags/index.html
.. _`filtri`: http://twig.sensiolabs.org/doc/filters/index.html
.. _`aggiungere le proprie estensioni`: http://twig.sensiolabs.org/doc/extensions.html
.. _`hinclude.js`: http://mnot.github.com/hinclude/
