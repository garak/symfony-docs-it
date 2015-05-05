.. _symfony2-versus-flat-php:

Symfony contro PHP puro
=======================

**Perché Symfony è meglio che aprire un file e scrivere PHP puro?**

Questo capitolo è per chi non ha mai usato un framework PHP, non ha familiarità con la
filosofia MVC, oppure semplicemente si chiede il motivo di tutto il *clamore* su
Symfony. Invece di *raccontare* che Symfony consente di sviluppare software più
rapidamente e in modo migliore che con PHP puro, ve lo faremo vedere.

In questo capitolo, scriveremo una semplice applicazione in PHP puro e poi la
rifattorizzeremo per essere più organizzata. Viaggeremo nel tempo, guardando le
decisioni che stanno dietro ai motivi per cui lo sviluppo web si è evoluto
durante gli ultimi anni per diventare quello che è ora.

Alla fine, vedremo come Symfony possa salvarci da compiti banali e consentirci di
riprendere il controllo del nostro codice.

Un semplice blog in PHP puro
----------------------------

In questo capitolo, costruiremo un'applicazione blog usando solo PHP puro.
Per iniziare, creiamo una singola pagina che mostra le voci del blog, che sono
state memorizzate nella base dati. La scrittura in puro PHP è sporca e veloce:

.. code-block:: html+php

    <?php
    // index.php
    $link = mysql_connect('localhost', 'mioutente', 'miapassword');
    mysql_select_db('blog_db', $link);

    $result = mysql_query('SELECT id, title FROM post', $link);
    ?>

    <!DOCTYPE html>
    <html>
        <head>
            <title>Lista dei post</title>
        </head>
        <body>
            <h1>Lista dei post</h1>
            <ul>
                <?php while ($row = mysql_fetch_assoc($result)): ?>
                <li>
                    <a href="/show.php?id=<?php echo $row['id'] ?>">
                        <?php echo $row['title'] ?>
                    </a>
                </li>
                <?php endwhile; ?>
            </ul>
        </body>
    </html>

    <?php
    mysql_close($link);
    ?>

Veloce da scrivere, rapido da eseguire e, al crescere dell'applicazione, impossibile
da mantenere. Ci sono diversi problemi che occorre considerare:

* **Niente verifica degli errori**: Che succede se la connessione alla base dati fallisce?

* **Scarsa organizzazione**: Se l'applicazione cresce, questo singolo file diventerà
  sempre più immantenibile. Dove inserire il codice per gestire la compilazione di un
  form? Come validare i dati? Dove mettere il codice per inviare delle
  email?

* **Difficoltà nel riusare il codice**: Essendo tutto in un solo file, non c'è modo di
  riusare alcuna parte dell'applicazione per altre "pagine" del blog.

.. note::

    Un altro problema non menzionato è il fatto che la base dati è legata a MySQL.
    Sebbene non affrontato qui, Symfony integra in pieno `Doctrine`_,
    una libreria dedicata all'astrazione e alla mappatura della base dati.

Isolare la presentazione
~~~~~~~~~~~~~~~~~~~~~~~~

Il codice può beneficiare immediatamente dalla separazione della "logica"
dell'applicazione dal codice che prepara la "presentazione" in HTML:

.. code-block:: html+php

    <?php
    // index.php
    $link = mysql_connect('localhost', 'mioutente', 'miapassword');
    mysql_select_db('blog_db', $link);

    $result = mysql_query('SELECT id, title FROM post', $link);

    $posts = array();
    while ($row = mysql_fetch_assoc($result)) {
        $posts[] = $row;
    }

    mysql_close($link);

    // include il codice HTML di presentazione
    require 'templates/list.php';

Il codice HTML ora è in un file separato (``templates/list.php``), che è
essenzialmente un file HTML che usa una sintassi PHP per template:

.. code-block:: html+php

    <!DOCTYPE html>
    <html>
        <head>
            <title>Lista dei post</title>
        </head>
        <body>
            <h1>Lista dei post</h1>
            <ul>
                <?php foreach ($posts as $post): ?>
                <li>
                    <a href="/read?id=<?php echo $post['id'] ?>">
                        <?php echo $post['title'] ?>
                    </a>
                </li>
                <?php endforeach ?>
            </ul>
        </body>
    </html>

Per convenzione, il file che contiene tutta la logica dell'applicazione, cioè ``index.php``,
è noto come "controllore". Il termine :term:`controllore` è una parola che ricorrerà
spesso, quale che sia il linguaggio o il framework scelto. Si riferisce semplicemente
alla parte del *proprio* codice che processa l'input proveniente dall'utente e prepara la risposta.

In questo caso, il nostro controllore prepara i dati estratti dalla base dati e quindi include
un template, per presentare tali dati. Con il controllore isolato, è possibile cambiare
facilmente *solo* il file template necessario per rendere le voci del blog in un
qualche altro formato (p.e. ``list.json.php`` per il formato JSON). 

Isolare la logica dell'applicazione (il dominio)
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Finora l'applicazione contiene una singola pagina. Ma se una seconda pagina avesse
bisogno di usare la stessa connessione alla base dati, o anche lo stesso array di post
del blog? Rifattorizziamo il codice in modo che il comportamento centrale e le funzioni
di accesso ai dati dell'applicazioni siano isolati in un nuovo file, chiamato ``model.php``:

.. code-block:: html+php

    <?php
    // model.php
    function open_database_connection()
    {
        $link = mysql_connect('localhost', 'mioutente', 'miapassword');
        mysql_select_db('blog_db', $link);

        return $link;
    }

    function close_database_connection($link)
    {
        mysql_close($link);
    }

    function get_all_posts()
    {
        $link = open_database_connection();

        $result = mysql_query('SELECT id, title FROM post', $link);
        $posts = array();
        while ($row = mysql_fetch_assoc($result)) {
            $posts[] = $row;
        }
        close_database_connection($link);

        return $posts;
    }

.. tip::

   Il nome ``model.php`` è usato perché la logica e l'accesso ai dati di un'applicazione
   sono tradizionalmente noti come il livello del "modello". In un'applicazione ben
   organizzata la maggior parte del codice che rappresenta la "logica di business"
   dovrebbe stare nel modello (invece che stare in un controllore). Diversamente da
   questo esempio, solo una parte (o niente) del modello riguarda effettivamente
   l'accesso a una base dati.

Il controllore (``index.php``) è ora molto semplice:

.. code-block:: html+php

    <?php
    require_once 'model.php';

    $posts = get_all_posts();

    require 'templates/list.php';

Ora, l'unico compito del controllore è prendere i dati dal livello del modello
dell'applicazione (il modello) e richiamare un template per rendere tali dati.
Questo è un esempio molto semplice del pattern model-view-controller.

Isolare il layout
~~~~~~~~~~~~~~~~~

A questo punto, l'applicazione è stata rifattorizzata in tre parti distinte,
offrendo diversi vantaggi e l'opportunità di riusare quasi tutto su pagine
diverse.

L'unica parte del codice che *non può* essere riusata è il layout. Sistemiamo
questo aspetto, creando un nuovo file ``layout.php``:

.. code-block:: html+php

    <!-- templates/layout.php -->
    <!DOCTYPE html>
    <html>
        <head>
            <title><?php echo $title ?></title>
        </head>
        <body>
            <?php echo $content ?>
        </body>
    </html>

Il template (``templates/list.php``) ora può essere semplificato, per
"estendere" il layout:

.. code-block:: html+php

    <?php $title = 'Lista dei post' ?>

    <?php ob_start() ?>
        <h1>Lista dei post</h1>
        <ul>
            <?php foreach ($posts as $post): ?>
            <li>
                <a href="/read?id=<?php echo $post['id'] ?>">
                    <?php echo $post['title'] ?>
                </a>
            </li>
            <?php endforeach ?>
        </ul>
    <?php $content = ob_get_clean() ?>

    <?php include 'layout.php' ?>

Qui abbiamo introdotto una metodologia che consente il riuso del layout.
Sfortunatamente, per poterlo fare, si è costretti a usare alcune brutte
funzioni PHP (``ob_start()``, ``ob_get_clean()``) nel template. Symfony
usa un componente Templating, che consente di poter fare ciò in modo
pulito e facile. Lo vedremo in azione tra poco.

Aggiungere al blog una pagina "show"
------------------------------------

La pagina "elenco" del blog è stata ora rifattorizzata in modo che il codice
sia meglio organizzato e riusabile. Per provarlo, aggiungiamo al blog una pagina
"mostra", che mostra un singolo post del blog identificato dal parametro ``id``.

Per iniziare, creiamo nel file ``model.php``  una nuova funzione, che recupera
un singolo risultato del blog a partire da un id dato::

    // model.php
    function get_post_by_id($id)
    {
        $link = open_database_connection();

        $id = intval($id);
        $query = 'SELECT date, title, body FROM post WHERE id = '.$id;
        $result = mysql_query($query);
        $row = mysql_fetch_assoc($result);

        close_database_connection($link);

        return $row;
    }

Quindi, creiamo un file chiamato ``show.php``, il controllore per questa nuova
pagina:

.. code-block:: html+php

    <?php
    require_once 'model.php';

    $post = get_post_by_id($_GET['id']);

    require 'templates/show.php';

Infine, creiamo un nuovo file template, ``templates/show.php``, per rendere
il singolo post del blog:

.. code-block:: html+php

    <?php $title = $post['title'] ?>

    <?php ob_start() ?>
        <h1><?php echo $post['title'] ?></h1>

        <div class="date"><?php echo $post['date'] ?></div>
        <div class="body">
            <?php echo $post['body'] ?>
        </div>
    <?php $content = ob_get_clean() ?>

    <?php include 'layout.php' ?>

La creazione della seconda pagina è stata molto facile e non ha implicato alcuna
duplicazione di codice. Tuttavia, questa pagina introduce alcuni altri problemi, che
un framework può risolvere. Per esempio, un parametro ``id`` mancante o non valido
causerà un errore nella pagina. Sarebbe meglio se facesse rendere una pagina 404,
ma non possiamo ancora farlo in modo facile. Inoltre, avendo dimenticato di pulire
il parametro ``id`` con la funzione ``mysql_real_escape_string()``, la base dati
è a rischio di attacchi di tipo SQL injection.

Un altro grosso problema è che ogni singolo controllore deve includere il file
``model.php``. Che fare se poi occorresse includere un secondo file o eseguire
un altro compito globale (p.e. garantire la sicurezza)? Nella situazione
attuale, tale codice dovrebbe essere aggiunto a ogni singolo file. Se lo si
dimentica in un file, speriamo che non sia qualcosa legato alla
sicurezza.

Un "front controller" alla riscossa
-----------------------------------

La soluzione è usare un :term:`front controller`: un singolo file PHP attraverso
il quale *tutte* le richieste sono processate. Con un front controller, gli URI
dell'applicazione cambiano un poco, ma iniziano a diventare più flessibili:

.. code-block:: text

    Senza un front controller
    /index.php          => Pagina della lista dei post (index.php eseguito)
    /show.php           => Pagina che mostra il singolo post (show.php eseguito)

    Con index.php come front controller
    /index.php          => Pagina della lista dei post (index.php eseguito)
    /index.php/show     => Pagina che mostra il singolo post (index.php eseguito)

.. tip::
    La parte dell'URI ``index.php`` può essere rimossa se si usano le regole di
    riscrittura di Apache (o equivalente). In questo caso, l'URI risultante della
    pagina che mostra il post sarebbe semplicemente ``/show``.

Usando un front controller, un singolo file PHP (``index.php`` in questo caso)
rende *ogni* richiesta. Per la pagina che mostra il post, ``/index.php/show``
eseguirà in effetti il file ``index.php``, che ora è responsabile per gestire
internamente le richieste, in base all'URI. Come vedremo, un front controller
è uno strumento molto potente.

Creazione del front controller
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Stiamo per fare un **grosso** passo avanti con l'applicazione. Con un solo file
a gestire tutte le richieste, possiamo centralizzare cose come gestione della
sicurezza, caricamento della configurazione, rotte. In questa applicazione,
``index.php`` deve essere abbastanza intelligente da rendere la lista dei post
*oppure* il singolo post, in base all'URI richiesto:

.. code-block:: html+php

    <?php
    // index.php

    // carica e inizializza le librerie globali
    require_once 'model.php';
    require_once 'controllers.php';

    // dirotta internamente la richiesta
    $uri = parse_url($_SERVER['REQUEST_URI'], PHP_URL_PATH);
    if ('/index.php' == $uri) {
        list_action();
    } elseif ('/index.php/show' == $uri && isset($_GET['id'])) {
        show_action($_GET['id']);
    } else {
        header('Status: 404 Not Found');
        echo '<html><body><h1>Pagina non trovata</h1></body></html>';
    }

Per una migliore organizzazione, entrambi i controllori (precedentemente ``index.php`` e
``show.php``) sono ora funzioni PHP, entrambe spostate in un file separato, ``controllers.php``:

.. code-block:: php

    function list_action()
    {
        $posts = get_all_posts();
        require 'templates/list.php';
    }

    function show_action($id)
    {
        $post = get_post_by_id($id);
        require 'templates/show.php';
    }

Come front controller, ``index.php`` ha assunto un nuovo ruolo, che include il
caricamento delle librerie principali e la gestione delle rotte dell'applicazione, in
modo che sia richiamato uno dei due controllori (le funzioni ``list_action()`` e
``show_action()``). In realtà. il front controller inizia ad assomigliare molto al
meccanismo con cui Symfony gestisce le richieste.

.. tip::

   Un altro vantaggio di un front controller sono gli URL flessibili. Si noti che
   l'URL della pagina del singolo post può essere cambiato da ``/show`` a ``/read``
   solo cambiando un unico punto del codice. Prima, occorreva rinominare un file.
   In Symfony, gli URL sono ancora più flessibili.

Finora, l'applicazione si è evoluta da un singolo file PHP a una struttura
organizzata e che consente il riuso del codice. Dovremmo essere contenti, ma
non ancora soddisfatti. Per esempio, il sistema delle rotte è instabile e non
riconosce che la pagina della lista (``/index.php``) dovrebbe essere accessibile
anche tramite ``/`` (con le regole di riscrittura di Apache). Inoltre, invece di
sviluppare il blog, abbiamo speso diverso tempo sull'"architettura" del codice
(p.e. rotte, richiamo dei controllori, template, ecc.). Ulteriore tempo sarebbe
necessario per gestire l'invio di form, la validazione dell'input, i log e la
sicurezza. Perché dovremmo reinventare soluzioni a tutti questi problemi comuni?

.. _add-a-touch-of-symfony2:

Aggiungere un tocco di Symfony
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Symfony alla riscossa! Prima di usare effettivamente Symfony, occorre accertarsi che
PHP sappia come trovare le classi di Symfony. Possiamo farlo grazie all'autoloader
fornito da Symfony. Un autoloader è uno strumento che rende possibile l'utilizzo di
classi PHP senza includere esplicitamente il file che contiene la
classe.

Nella cartella radice, creare un file ``composer.json`` con il seguente
contenuto:

.. code-block:: json

    {
        "require": {
            "symfony/symfony": "2.3.*"
        },
        "autoload": {
            "files": ["model.php","controllers.php"]
        }
    }

Quindi, `scaricare Composer`_ ed eseguire il seguente comando, che scaricherà Symfony
in una cartella ``vendor/``:

.. code-block:: bash

    $ composer install

Oltre a scaricare le dipendenza, Composer genera un file ``vendor/autoload.php``,
che si occupa di auto-caricare tutti i file del framework Symfony, nonché dei
file menzionati nella sezione autoload di ``composer.json``.

Una delle idee principali della filosofia di Symfony è che il compito principale di
un'applicazione sia quello di interpretare ogni richiesta e restituire una risposta. A
tal fine, Symfony fornice sia una classe :class:`Symfony\\Component\\HttpFoundation\\Request`
che una classe :class:`Symfony\\Component\\HttpFoundation\\Response`. Queste classi sono
rappresentazioni orientate agli oggetti delle richieste grezze HTTP processate e delle
risposte HTTP restituite. Usiamole per migliorare il nostro blog:

.. code-block:: html+php

    <?php
    // index.php
    require_once 'vendor/autoload.php';

    use Symfony\Component\HttpFoundation\Request;
    use Symfony\Component\HttpFoundation\Response;

    $request = Request::createFromGlobals();

    $uri = $request->getPathInfo();
    if ('/' == $uri) {
        $response = list_action();
    } elseif ('/show' == $uri && $request->query->has('id')) {
        $response = show_action($request->query->get('id'));
    } else {
        $html = '<html><body><h1>Pagina non trovata</h1></body></html>';
        $response = new Response($html, 404);
    }

    // mostra gli header e invia la risposta
    $response->send();

I controllori sono ora responsabili di restituire un oggetto ``Response``.
Per rendere le cose più facili, si può aggiungere una nuova funzione ``render_template()``,
che si comporta un po' come il sistema di template di Symfony:

.. code-block:: php

    // controllers.php
    use Symfony\Component\HttpFoundation\Response;

    function list_action()
    {
        $posts = get_all_posts();
        $html = render_template('templates/list.php', array('posts' => $posts));

        return new Response($html);
    }

    function show_action($id)
    {
        $post = get_post_by_id($id);
        $html = render_template('templates/show.php', array('post' => $post));

        return new Response($html);
    }

    // funzione aiutante per rendere i template
    function render_template($path, array $args)
    {
        extract($args);
        ob_start();
        require $path;
        $html = ob_get_clean();

        return $html;
    }

Prendendo una piccola parte di Symfony, l'applicazione è diventata più flessibile e
più affidabile. La classe ``Request`` fornisce un modo di accedere alle informazioni sulla
richiesta HTTP. Nello specifico, il metodo ``getPathInfo()`` restituisce un URI più
pulito (restituisce sempre ``/show`` e mai ``/index.php/show``).
In questo modo, anche se l'utente va su ``/index.php/show``, l'applicazione è abbastanza
intelligente per dirottare la richiesta a ``show_action()``.

L'oggetto ``Response`` dà flessibilità durante la costruzione della risposta HTTP,
consentendo di aggiungere header e contenuti HTTP tramite un'interfaccia orientata agli
oggetti. Mentre in questa applicazione le risposte molto semplici, tale flessibilità
ripagherà quando l'applicazione cresce.

.. _the-sample-application-in-symfony2:

L'applicazione di esempio in Symfony
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Il blog ha fatto *molta* strada, ma contiene ancora troppo codice per un'applicazione
così semplice. Durante il cammino, abbiamo anche inventato un semplice sistema di rotte
e un metodo che usa ``ob_start()`` e ``ob_get_clean()`` per rendere i template. Se, per
qualche ragione, si avesse bisogno di continuare a costruire questo "framework" da zero,
si potrebbero almeno utilizzare i componenti `Routing`_  e `Templating`_, che già
risolvono questi problemi.

Invece di risolvere nuovamente problemi comuni, si può lasciare a Symfony il compito di
occuparsene. Ecco la stessa applicazione di esempio, ora costruita in Symfony::

    // src/AppBundle/Controller/BlogController.php
    namespace AppBundle\Controller;

    use Symfony\Bundle\FrameworkBundle\Controller\Controller;

    class BlogController extends Controller
    {
        public function listAction()
        {
            $posts = $this->get('doctrine')
                ->getManager()
                ->createQuery('SELECT p FROM AcmeBlogBundle:Post p')
                ->execute();

            return $this->render('Blog/list.html.php', array('posts' => $posts));
        }

        public function showAction($id)
        {
            $post = $this->get('doctrine')
                ->getManager()
                ->getRepository('AppBundle:Post')
                ->find($id);

            if (!$post) {
                // mostra la pagina 404 page not found
                throw $this->createNotFoundException();
            }

            return $this->render('Blog/show.html.php', array('post' => $post));
        }
    }

I due controllori sono ancora leggeri. Ognuno usa la libreria ORM Doctrine per
recuperare oggetti dalla base dati e il componente ``Templating`` per rendere un template
e restituire un oggetto ``Response``. Il template della lista è ora un po' più
semplice:

.. code-block:: html+php

    <!-- app/Resources/views/Blog/list.html.php -->
    <?php $view->extend('layout.html.php') ?>

    <?php $view['slots']->set('title', 'List of Posts') ?>

    <h1>Lista dei post</h1>
    <ul>
        <?php foreach ($posts as $post): ?>
        <li>
            <a href="<?php echo $view['router']->generate(
                'blog_show',
                array('id' => $post->getId())
            ) ?>">
                <?php echo $post->getTitle() ?>
            </a>
        </li>
        <?php endforeach ?>
    </ul>

Il layout è quasi identico:

.. code-block:: html+php

    <!-- app/Resources/views/layout.html.php -->
    <!DOCTYPE html>
    <html>
        <head>
            <title><?php echo $view['slots']->output(
                'title',
                'Titolo predefinito'
            ) ?></title>
        </head>
        <body>
            <?php echo $view['slots']->output('_content') ?>
        </body>
    </html>

.. note::

    Lasciamo il template di show come esercizio, visto che dovrebbe essere banale
    crearlo basandosi sul template della lista.

Quando il motore di Symfony (chiamato ``Kernel``) parte, ha bisogno di una mappa che gli
consenta di sapere quali controllori eseguire, in base alle informazioni della richiesta.
Una configurazione delle rotte fornisce tali informazioni in un formato leggibile:

.. code-block:: yaml

    # app/config/routing.yml
    blog_list:
        path:     /blog
        defaults: { _controller: AppBundle:Blog:list }

    blog_show:
        path:     /blog/show/{id}
        defaults: { _controller: AppBundle:Blog:show }

Ora che Symfony gestisce tutti i compiti più comuni, il front controller è
semplicissimo. E siccome fa così poco, non si avrà mai bisogno di modificarlo una
volta creato (e se si usa una `distribuzione di Symfony`_, non servirà nemmeno
crearlo!)::

    // web/app.php
    require_once __DIR__.'/../app/bootstrap.php';
    require_once __DIR__.'/../app/AppKernel.php';

    use Symfony\Component\HttpFoundation\Request;

    $kernel = new AppKernel('prod', false);
    $kernel->handle(Request::createFromGlobals())->send();

L'unico compito del front controller è inizializzare il motore di Symfony (il ``Kernel``)
e passargli un oggetto ``Request`` da gestire. Il nucleo di Symfony quindi usa la mappa
delle rotte per determinare quale controllore richiamare. Proprio come prima, il metodo
controllore è responsabile di restituire l'oggetto ``Response`` finale.
Non resta molto altro da fare.

Per una rappresentazione visuale di come Symfony gestisca ogni richiesta, si veda il
:ref:`diagramma di flusso della richiesta<request-flow-figure>`.

.. _where-symfony2-delivers:

Dove consegna Symfony
~~~~~~~~~~~~~~~~~~~~~

Nei capitoli successivi, impareremo di più su come funziona ogni pezzo di Symfony e
sull'organizzazione raccomandata di un progetto. Per ora, vediamo come migrare il blog
da PHP puro a Symfony ci abbia migliorato la vita:

* L'applicazione ora ha un **codice organizzato chiaramente e coerentemente** (sebbene
  Symfony non obblighi a farlo). Questo promuove la **riusabilità** e consente
  a nuovi sviluppatori di essere produttivi nel progetto in modo più rapido.

* Il 100% del codice che si scrive è per la *propria* applicazione. **Non occorre
  sviluppare o mantenere utilità a basso livello**, come :ref:`autoload <autoloading-introduction-sidebar>`,
  :doc:`rotte </book/routing>` o rendere i :doc:`controllori </book/controller>`.

* Symfony dà **accesso a strumenti open source**, come  Doctrine e i componenti
  Templating, Security, Form, Validation e Translation (solo per nominarne
  alcuni).

* L'applicazione ora gode di **URL pienamente flessibili**, grazie al componente
  Routing.

* L'architettura HTTP-centrica di Symfony dà accesso a strumenti potenti, come
  la **cache HTTP** fornita dalla **cache HTTP interna di Symfony** o a strumenti ancora
  più potenti, come `Varnish`_. Questi aspetti sono coperti in un capitolo successivo,
  tutto dedicato alla :doc:`cache </book/http_cache>`.

Ma forse la parte migliore nell'usare Symfony è l'accesso all'intero insieme di
**strumenti open source di alta qualità sviluppati dalla comunità di Symfony**!
Si possono trovare dei buoni bundle su `KnpBundles.com`_.

Template migliori
-----------------

Se lo si vuole usare, Symfony ha un motore di template predefinito, chiamato
`Twig`_, che rende i template più veloci da scrivere e più facili da leggere.
Questo vuol dire che l'applicazione di esempio può contenere ancora meno codice!
Prendiamo per esempio il template della lista, scritto in Twig:

.. code-block:: html+jinja

    {# app/Resources/views/blog/list.html.twig #}
    {% extends "layout.html.twig" %}

    {% block title %}Lista dei post{% endblock %}

    {% block body %}
        <h1>Lista dei post</h1>
        <ul>
            {% for post in posts %}
            <li>
                <a href="{{ path('blog_show', {'id': post.id}) }}">
                    {{ post.title }}
                </a>
            </li>
            {% endfor %}
        </ul>
    {% endblock %}

Il template corrispondente ``layout.html.twig`` è anche più facile da scrivere:

.. code-block:: html+jinja

    {# app/Resources/views/layout.html.twig #}
    <!DOCTYPE html>
    <html>
        <head>
            <title>{% block title %}Titolo predefinito{% endblock %}</title>
        </head>
        <body>
            {% block body %}{% endblock %}
        </body>
    </html>

Twig è ben supportato in Symfony. Pur essendo sempre supportati i template PHP,
continueremo a discutere dei molti vantaggi offerti da Twig. Per ulteriori informazioni,
vedere il :doc:`capitolo dei template </book/templating>`.

Imparare di più con le ricette
------------------------------

* :doc:`/cookbook/templating/PHP`
* :doc:`/cookbook/controller/service`

.. _`Doctrine`: http://www.doctrine-project.org
.. _`scaricare Composer`: http://getcomposer.org/download/
.. _`Routing`: https://github.com/symfony/Routing
.. _`Templating`: https://github.com/symfony/Templating
.. _`KnpBundles.com`: http://knpbundles.com/
.. _`Twig`: http://twig.sensiolabs.org
.. _`Varnish`: https://www.varnish-cache.org/
.. _`PHPUnit`: http://www.phpunit.de
.. _`distribuzione di Symfony`: https://github.com/symfony/symfony-standard
