.. index::
   single: Rotte

Le rotte
========

URL ben realizzati sono una cosa assolutamente da avere per qualsiasi applicazione web seria. Questo
significa lasciarsi alle spalle  URL del tipo ``index.php?article_id=57`` in favore
di qualcosa come ``/read/intro-to-symfony``.

Avere flessibilità è ancora più importante. Che cosa succede se è necessario modificare
l'URL di una pagina da ``/blog`` a ``/news``? Quanti collegamenti bisogna cercare
e aggiornare per realizzare la modifica? Se si stanno utilizzando le rotte di Symfony
la modifica è semplice.

Le rotte di Symfony2 consentono di definire URL creativi che possono essere mappati
in differenti aree dell'applicazione. Entro la fine del capitolo, si sarà in grado di:

* Creare rotte complesse che mappano i controllori
* Generare URL all'interno di template e controllori
* Caricare le risorse delle rotte dai bundle (o da altre parti) 
* Eseguire il debug delle rotte

.. index::
   single: Rotte; Basi

Le rotte in azione
------------------

Una *rotta* è una mappatura tra uno schema di URL e un controllore. Per esempio, supponiamo
che si voglia gestire un qualsiasi URL tipo ``/blog/my-post`` o ``/blog/all-about-symfony``
e inviarlo a un controllore che cerchi e visualizzi quel post del blog.
La rotta è semplice:

.. configuration-block::

    .. code-block:: yaml

        # app/config/routing.yml
        blog_show:
            pattern:   /blog/{slug}
            defaults:  { _controller: AcmeBlogBundle:Blog:show }

    .. code-block:: xml

        <!-- app/config/routing.xml -->
        <?xml version="1.0" encoding="UTF-8" ?>
        <routes xmlns="http://symfony.com/schema/routing"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://symfony.com/schema/routing http://symfony.com/schema/routing/routing-1.0.xsd">

            <route id="blog_show" pattern="/blog/{slug}">
                <default key="_controller">AcmeBlogBundle:Blog:show</default>
            </route>
        </routes>

    .. code-block:: php

        // app/config/routing.php
        use Symfony\Component\Routing\RouteCollection;
        use Symfony\Component\Routing\Route;

        $collection = new RouteCollection();
        $collection->add('blog_show', new Route('/blog/{slug}', array(
            '_controller' => 'AcmeBlogBundle:Blog:show',
        )));

        return $collection;

Lo schema definito dalla rotta ``blog_show`` si comporta come ``/blog/*``, dove
al carattere jolly viene dato il nome ``slug``. Per l'URL ``/blog/my-blog-post``,
la variabile ``slug`` ottiene il valore ``my-blog-post``, che è disponibile
per l'utilizzo nel controllore (proseguire nella lettura).

Il parametro ``_controller`` è una chiave speciale che dice a Symfony quale controllore
dovrebbe essere eseguito quando un URL corrisponde a questa rotta. La stringa ``_controller``
è detta :ref:`nome logico<controller-string-syntax>`. Segue un
pattern che punta ad uno specifico metodo di una classe PHP:

.. code-block:: php

    // src/Acme/BlogBundle/Controller/BlogController.php
    
    namespace Acme\BlogBundle\Controller;
    use Symfony\Bundle\FrameworkBundle\Controller\Controller;

    class BlogController extends Controller
    {
        public function showAction($slug)
        {
            $blog = // usare la variabile $slug per interrogare il database
            
            return $this->render('AcmeBlogBundle:Blog:show.html.twig', array(
                'blog' => $blog,
            ));
        }
    }

Congratulazioni! Si è appena creata la prima rotta, collegandola ad
un controllore. Ora, quando si visita ``/blog/my-post``, verrà eseguito il
controllore ``showAction`` e la variabile ``$slug`` avrà valore ``my-post``.

Questo è l'obiettivo delle rotte di Symfony2: mappare l'URL di una richiesta in un
controllore. Lungo la strada, si impareranno tutti i trucchi per mappare facilmente
anche gli URL più complessi. 

.. index::
   single: Rotte; Sotto il cofano

Le rotte: funzionamento interno
-------------------------------

Quando all'applicazione viene fatta una richiesta, questa contiene un indirizzo alla
esatta "risorsa" che il client sta richiedendo. Questo indirizzo è chiamato
URL, (o URI) e potrebbe essere ``/contact``, ``/blog/read-me``, o qualunque
altra cosa. Prendere ad esempio la seguente richiesta HTTP:

.. code-block:: text

    GET /blog/my-blog-post

L'obiettivo del sistema delle rotte di Symfony2 è quello di analizzare questo URL e determinare
quale controller dovrebbe essere eseguito. L'intero processo è il seguente:

#. La richiesta è gestita dal front controller di Symfony2 (ad esempio ``app.php``);

#. Il nucleo di Symfony2 (ad es. il kernel) chiede al router di ispezionare la richiesta;

#. Il router verifica la corrispondenza dell'URL in arrivo con una specifica rotta e restituisce informazioni
   sulla rotta, tra le quali il controllore che deve essere eseguito;

#. Il kernel di Symfony2 esegue il controllore, che alla fine restituisce
   un oggetto ``Response``.

.. figure:: /images/request-flow.png
   :align: center
   :alt: flusso della richiesta di Symfony2

   Lo strato delle rotte è uno strumento che traduce l'URL in ingresso in uno specifico
   controllore da eseguire.

.. index::
   single: Rotte; Creazione di rotte

Creazione delle rotte
---------------------

Symfony carica tutte le rotte per l'applicazione da un singolo file con la configurazione
delle rotte. Il file generalmente è ``app/config/routing.yml``, ma può essere configurato
per essere qualunque cosa (compreso un file XML o PHP) tramite il file di configurazione
dell'applicazione:

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        framework:
            # ...
            router:        { resource: "%kernel.root_dir%/config/routing.yml" }

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <framework:config ...>
            <!-- ... -->
            <framework:router resource="%kernel.root_dir%/config/routing.xml" />
        </framework:config>

    .. code-block:: php

        // app/config/config.php
        $container->loadFromExtension('framework', array(
            // ...
            'router'        => array('resource' => '%kernel.root_dir%/config/routing.php'),
        ));

.. tip::

    Anche se tutte le rotte sono caricate da un singolo file, è una pratica comune
    includere ulteriori risorse di rotte all'interno del file. Vedere
    la sezione :ref:`routing-include-external-resources` per maggiori informazioni.

Configurazione di base delle rotte
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Definire una rotta è semplice e una tipica applicazione avrà molte rotte.
Una rotta di base è costituita da due parti: il ``pattern`` da confrontare e un
array ``defaults``:

.. configuration-block::

    .. code-block:: yaml

        _welcome:
            pattern:   /
            defaults:  { _controller: AcmeDemoBundle:Main:homepage }

    .. code-block:: xml

        <?xml version="1.0" encoding="UTF-8" ?>

        <routes xmlns="http://symfony.com/schema/routing"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://symfony.com/schema/routing http://symfony.com/schema/routing/routing-1.0.xsd">

            <route id="_welcome" pattern="/">
                <default key="_controller">AcmeDemoBundle:Main:homepage</default>
            </route>

        </routes>

    ..  code-block:: php

        use Symfony\Component\Routing\RouteCollection;
        use Symfony\Component\Routing\Route;

        $collection = new RouteCollection();
        $collection->add('_welcome', new Route('/', array(
            '_controller' => 'AcmeDemoBundle:Main:homepage',
        )));

        return $collection;

Questa rotta corrisponde alla homepage (``/``) e la mappa nel controllore ``AcmeDemoBundle:Main:homepage``.
La stringa ``_controller`` è tradotta da Symfony2 in una
funzione PHP effettiva, ed eseguita. Questo processo verrà spiegato a breve
nella sezione :ref:`controller-string-syntax`.

.. index::
   single: Rotte; Segnaposti

Rotte con segnaposti
~~~~~~~~~~~~~~~~~~~~

Naturalmente il sistema delle rotte supporta rotte molto più interessanti. Molte
rotte conterranno uno o più segnaposto "jolly":

.. configuration-block::

    .. code-block:: yaml

        blog_show:
            pattern:   /blog/{slug}
            defaults:  { _controller: AcmeBlogBundle:Blog:show }

    .. code-block:: xml

        <?xml version="1.0" encoding="UTF-8" ?>

        <routes xmlns="http://symfony.com/schema/routing"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://symfony.com/schema/routing http://symfony.com/schema/routing/routing-1.0.xsd">

            <route id="blog_show" pattern="/blog/{slug}">
                <default key="_controller">AcmeBlogBundle:Blog:show</default>
            </route>
        </routes>

    .. code-block:: php

        use Symfony\Component\Routing\RouteCollection;
        use Symfony\Component\Routing\Route;

        $collection = new RouteCollection();
        $collection->add('blog_show', new Route('/blog/{slug}', array(
            '_controller' => 'AcmeBlogBundle:Blog:show',
        )));

        return $collection;

Lo schema verrà soddisfatto da qualsiasi cosa del tipo ``/blog/*``. Meglio ancora,
il valore corrispondente il segnaposto ``{slug}`` sarà disponibile all'interno del
controllore. In altre parole, se l'URL è ``/blog/hello-world``, una variabile ``$slug``,
con un valore ``hello-world``, sarà disponibile nel controllore.
Questo può essere usato, ad esempio, per caricare il post sul blog che verifica questa stringa.

Tuttavia lo schema *non* deve corrispondere semplicemente a ``/blog``. Questo perché,
per impostazione predefinita, tutti i segnaposto sono obbligatori. Questo comportamento può essere cambiato aggiungendo
un valore segnaposto all'array ``defaults``.

Segnaposto obbligatori e opzionali
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Per rendere le cose più eccitanti, aggiungere una nuova rotta che visualizza un elenco di tutti
i post disponibili del blog per questa applicazione immaginaria di blog:

.. configuration-block::

    .. code-block:: yaml

        blog:
            pattern:   /blog
            defaults:  { _controller: AcmeBlogBundle:Blog:index }

    .. code-block:: xml

        <?xml version="1.0" encoding="UTF-8" ?>

        <routes xmlns="http://symfony.com/schema/routing"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://symfony.com/schema/routing http://symfony.com/schema/routing/routing-1.0.xsd">

            <route id="blog" pattern="/blog">
                <default key="_controller">AcmeBlogBundle:Blog:index</default>
            </route>
        </routes>

    .. code-block:: php

        use Symfony\Component\Routing\RouteCollection;
        use Symfony\Component\Routing\Route;

        $collection = new RouteCollection();
        $collection->add('blog', new Route('/blog', array(
            '_controller' => 'AcmeBlogBundle:Blog:index',
        )));

        return $collection;

Finora, questa rotta è la più semplice possibile: non contiene segnaposto
e corrisponde solo all'esatto URL ``/blog``. Ma cosa succede se si ha bisogno di questa rotta
per supportare l'impaginazione, dove ``/blog/2`` visualizza la seconda pagina dell'elenco post
del blog? Bisogna aggiornare la rotta per avere un nuovo segnaposto ``{page}``:

.. configuration-block::

    .. code-block:: yaml

        blog:
            pattern:   /blog/{page}
            defaults:  { _controller: AcmeBlogBundle:Blog:index }

    .. code-block:: xml

        <?xml version="1.0" encoding="UTF-8" ?>

        <routes xmlns="http://symfony.com/schema/routing"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://symfony.com/schema/routing http://symfony.com/schema/routing/routing-1.0.xsd">

            <route id="blog" pattern="/blog/{page}">
                <default key="_controller">AcmeBlogBundle:Blog:index</default>
            </route>
        </routes>

    .. code-block:: php

        use Symfony\Component\Routing\RouteCollection;
        use Symfony\Component\Routing\Route;

        $collection = new RouteCollection();
        $collection->add('blog', new Route('/blog/{page}', array(
            '_controller' => 'AcmeBlogBundle:Blog:index',
        )));

        return $collection;

Come il precedente segnaposto ``{slug}``, il valore che verifica ``{page}``
sarà disponibile all'interno del controllore. Il suo valore può essere usato per determinare quale
insieme di post del blog devono essere visualizzati per una data pagina.

Un attimo però! Dal momento che i segnaposto per impostazione predefinita sono obbligatori, questa rotta non
avrà più corrispondenza con il semplice ``/blog``. Invece, per vedere la pagina 1 del blog,
si avrà bisogno di utilizzare l'URL ``/blog/1``! Dal momento che non c'è soluzione per una complessa applicazione
web, modificare la rotta per rendere il parametro ``{page}`` opzionale.
Questo si fa includendolo nella collezione ``defaults``:

.. configuration-block::

    .. code-block:: yaml

        blog:
            pattern:   /blog/{page}
            defaults:  { _controller: AcmeBlogBundle:Blog:index, page: 1 }

    .. code-block:: xml

        <?xml version="1.0" encoding="UTF-8" ?>

        <routes xmlns="http://symfony.com/schema/routing"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://symfony.com/schema/routing http://symfony.com/schema/routing/routing-1.0.xsd">

            <route id="blog" pattern="/blog/{page}">
                <default key="_controller">AcmeBlogBundle:Blog:index</default>
                <default key="page">1</default>
            </route>
        </routes>

    .. code-block:: php

        use Symfony\Component\Routing\RouteCollection;
        use Symfony\Component\Routing\Route;

        $collection = new RouteCollection();
        $collection->add('blog', new Route('/blog/{page}', array(
            '_controller' => 'AcmeBlogBundle:Blog:index',
            'page' => 1,
        )));

        return $collection;

Aggiungendo ``page`` alla chiave ``defaults``, il segnaposto ``{page}`` non è
più obbligatorio. L'URL ``/blog`` corrisponderà a questa rotta e il valore del
parametro ``page`` verrà impostato a ``1``. Anche l'URL ``/blog/2`` avrà
corrispondenza, dando al parametro ``page`` il valore ``2``. Perfetto.

+---------+------------+
| /blog   | {page} = 1 |
+---------+------------+
| /blog/1 | {page} = 1 |
+---------+------------+
| /blog/2 | {page} = 2 |
+---------+------------+

.. index::
   single: Rotte; Requisiti

Aggiungere requisiti
~~~~~~~~~~~~~~~~~~~~

Si dia uno sguardo veloce alle rotte che sono state create finora:

.. configuration-block::

    .. code-block:: yaml

        blog:
            pattern:   /blog/{page}
            defaults:  { _controller: AcmeBlogBundle:Blog:index, page: 1 }

        blog_show:
            pattern:   /blog/{slug}
            defaults:  { _controller: AcmeBlogBundle:Blog:show }

    .. code-block:: xml

        <?xml version="1.0" encoding="UTF-8" ?>

        <routes xmlns="http://symfony.com/schema/routing"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://symfony.com/schema/routing http://symfony.com/schema/routing/routing-1.0.xsd">

            <route id="blog" pattern="/blog/{page}">
                <default key="_controller">AcmeBlogBundle:Blog:index</default>
                <default key="page">1</default>
            </route>

            <route id="blog_show" pattern="/blog/{slug}">
                <default key="_controller">AcmeBlogBundle:Blog:show</default>
            </route>
        </routes>

    .. code-block:: php

        use Symfony\Component\Routing\RouteCollection;
        use Symfony\Component\Routing\Route;

        $collection = new RouteCollection();
        $collection->add('blog', new Route('/blog/{page}', array(
            '_controller' => 'AcmeBlogBundle:Blog:index',
            'page' => 1,
        )));

        $collection->add('blog_show', new Route('/blog/{show}', array(
            '_controller' => 'AcmeBlogBundle:Blog:show',
        )));

        return $collection;

Si riesce a individuare il problema? Notare che entrambe le rotte hanno schemi che verificano
URL del tipo ``/blog/*``. Il router di Symfony sceglie sempre la
**prima** rotta corrispondente che trova. In altre parole, la rotta ``blog_show``
non sarà *mai* trovata. Invece, un URL del tipo ``/blog/my-blog-post`` verrà abbinato
alla prima rotta (``blog``) restituendo il valore senza senso ``my-blog-post``
per il parametro ``{page}``.

+--------------------+-------+-----------------------+
| URL                | rotta | parametri             |
+====================+=======+=======================+
| /blog/2            | blog  | {page} = 2            |
+--------------------+-------+-----------------------+
| /blog/my-blog-post | blog  | {page} = my-blog-post |
+--------------------+-------+-----------------------+

La risposta al problema è aggiungere rotte *obbligatorie*. Le rotte in questo
esempio potrebbero funzionare perfettamente se lo schema ``/blog/{page}`` fosse verificato *solo*
per gli URL dove ``{page}`` fosse un numero intero. Fortunatamente, i requisiti possono essere scritti tramite
espressioni regolari e aggiunti per ogni parametro. Per esempio:

.. configuration-block::

    .. code-block:: yaml

        blog:
            pattern:   /blog/{page}
            defaults:  { _controller: AcmeBlogBundle:Blog:index, page: 1 }
            requirements:
                page:  \d+

    .. code-block:: xml

        <?xml version="1.0" encoding="UTF-8" ?>

        <routes xmlns="http://symfony.com/schema/routing"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://symfony.com/schema/routing http://symfony.com/schema/routing/routing-1.0.xsd">

            <route id="blog" pattern="/blog/{page}">
                <default key="_controller">AcmeBlogBundle:Blog:index</default>
                <default key="page">1</default>
                <requirement key="page">\d+</requirement>
            </route>
        </routes>

    .. code-block:: php

        use Symfony\Component\Routing\RouteCollection;
        use Symfony\Component\Routing\Route;

        $collection = new RouteCollection();
        $collection->add('blog', new Route('/blog/{page}', array(
            '_controller' => 'AcmeBlogBundle:Blog:index',
            'page' => 1,
        ), array(
            'page' => '\d+',
        )));

        return $collection;

Il requisito ``\d+`` è una espressione regolare che dice che il valore del
parametro ``{page}`` deve essere una cifra (cioè un numero). La rotta ``blog``
sarà comunque abbinata a un URL del tipo ``/blog/2`` (perché 2 è un numero), ma
non sarà più abbinata a un URL tipo ``/blog/my-blog-post`` (perché ``my-blog-post``
*non* è un numero).

Come risultato, un URL tipo ``/blog/my-blog-post`` ora verrà correttamente abbinato alla
rotta ``blog_show``.

+--------------------+-----------+-----------------------+
| URL                | rotta     | parametri             |
+====================+===========+=======================+
| /blog/2            | blog      | {page} = 2            |
+--------------------+-----------+-----------------------+
| /blog/my-blog-post | blog_show | {slug} = my-blog-post |
+--------------------+-----------+-----------------------+

.. sidebar:: Vincono sempre le rotte che compaiono prima

    Il significato di tutto questo è che l'ordine delle rotte è molto importante.
    Se la rotta ``blog_show`` fosse stata collocata sopra la rotta ``blog``,
    l'URL ``/blog/2`` sarebbe stato abbinato a ``blog_show`` invece di ``blog`` perché
    il parametro ``{slug}`` di ``blog_show`` non ha requisiti. Utilizzando l'ordinamento
    appropriato e dei requisiti intelligenti, si può realizzare qualsiasi cosa.

Poiché i requisiti dei parametri sono espressioni regolari, la complessità
e la flessibilità di ogni requisito dipende da come li si scrive. Si supponga che la pagina
iniziale dell'applicazione sia disponibile in due diverse lingue, in base
all'URL:

.. configuration-block::

    .. code-block:: yaml

        homepage:
            pattern:   /{culture}
            defaults:  { _controller: AcmeDemoBundle:Main:homepage, culture: en }
            requirements:
                culture:  en|fr

    .. code-block:: xml

        <?xml version="1.0" encoding="UTF-8" ?>

        <routes xmlns="http://symfony.com/schema/routing"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://symfony.com/schema/routing http://symfony.com/schema/routing/routing-1.0.xsd">

            <route id="homepage" pattern="/{culture}">
                <default key="_controller">AcmeDemoBundle:Main:homepage</default>
                <default key="culture">en</default>
                <requirement key="culture">en|fr</requirement>
            </route>
        </routes>

    .. code-block:: php

        use Symfony\Component\Routing\RouteCollection;
        use Symfony\Component\Routing\Route;

        $collection = new RouteCollection();
        $collection->add('homepage', new Route('/{culture}', array(
            '_controller' => 'AcmeDemoBundle:Main:homepage',
            'culture' => 'en',
        ), array(
            'culture' => 'en|fr',
        )));

        return $collection;

Per le richieste in entrata, la porzione ``{culture}`` dell'URL viene controllata tramite
l'espressione regolare ``(en|fr)``.

+-----+--------------------------------+
| /   | {culture} = en                 |
+-----+--------------------------------+
| /en | {culture} = en                 |
+-----+--------------------------------+
| /fr | {culture} = fr                 |
+-----+--------------------------------+
| /es | *non si abbina a questa rotta* |
+-----+--------------------------------+

.. index::
   single: Rotte; Requisiti di metodi

Aggiungere requisiti al metodo HTTP
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

In aggiunta agli URL, si può anche verificare il *metodo* della richiesta
entrante (ad esempio GET, HEAD, POST, PUT, DELETE). Si supponga di avere un form contatti
con due controllori: uno per visualizzare il form (su una richiesta GET) e uno
per l'elaborazione del form dopo che è stato inviato (su una richiesta POST). Questo può
essere realizzato con la seguente configurazione per le rotte:

.. configuration-block::

    .. code-block:: yaml

        contact:
            pattern:  /contact
            defaults: { _controller: AcmeDemoBundle:Main:contact }
            requirements:
                _method:  GET

        contact_process:
            pattern:  /contact
            defaults: { _controller: AcmeDemoBundle:Main:contactProcess }
            requirements:
                _method:  POST

    .. code-block:: xml

        <?xml version="1.0" encoding="UTF-8" ?>

        <routes xmlns="http://symfony.com/schema/routing"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://symfony.com/schema/routing http://symfony.com/schema/routing/routing-1.0.xsd">

            <route id="contact" pattern="/contact">
                <default key="_controller">AcmeDemoBundle:Main:contact</default>
                <requirement key="_method">GET</requirement>
            </route>

            <route id="contact_process" pattern="/contact">
                <default key="_controller">AcmeDemoBundle:Main:contactProcess</default>
                <requirement key="_method">POST</requirement>
            </route>
        </routes>

    .. code-block:: php

        use Symfony\Component\Routing\RouteCollection;
        use Symfony\Component\Routing\Route;

        $collection = new RouteCollection();
        $collection->add('contact', new Route('/contact', array(
            '_controller' => 'AcmeDemoBundle:Main:contact',
        ), array(
            '_method' => 'GET',
        )));

        $collection->add('contact_process', new Route('/contact', array(
            '_controller' => 'AcmeDemoBundle:Main:contactProcess',
        ), array(
            '_method' => 'POST',
        )));

        return $collection;

Nonostante il fatto che queste due rotte abbiano schemi identici (``/contact``),
la prima rotta corrisponderà solo a richieste GET e la seconda rotta corrisponderà
solo a richieste POST. Questo significa che è possibile visualizzare il form e inviarlo
utilizzando lo stesso URL ma controllori distinti per le due azioni.

.. note::
    Se non viene specificato nessun requisito ``_method``, la rotta verrà abbinata
    con *tutti* i metodi.

Come avviene per gli altri requisiti, il requisito ``_method`` viene analizzato come una espressione
regolare. Per abbinare le richieste ``GET`` *o* ``POST``, si può utilizzare ``GET|POST``.

.. index::
   single: Rotte; Esempio avanzato
   single: Rotte; Parametro _format

.. _advanced-routing-example:

Esempio di rotte avanzate
~~~~~~~~~~~~~~~~~~~~~~~~~

A questo punto, si ha tutto il necessario per creare una complessa struttura
di rotte in Symfony. Quello che segue è un esempio di quanto flessibile
può essere il sistema delle rotte:

.. configuration-block::

    .. code-block:: yaml

        article_show:
          pattern:  /articles/{culture}/{year}/{title}.{_format}
          defaults: { _controller: AcmeDemoBundle:Article:show, _format: html }
          requirements:
              culture:  en|fr
              _format:  html|rss
              year:     \d+

    .. code-block:: xml

        <?xml version="1.0" encoding="UTF-8" ?>

        <routes xmlns="http://symfony.com/schema/routing"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://symfony.com/schema/routing http://symfony.com/schema/routing/routing-1.0.xsd">

            <route id="article_show" pattern="/articles/{culture}/{year}/{title}.{_format}">
                <default key="_controller">AcmeDemoBundle:Article:show</default>
                <default key="_format">html</default>
                <requirement key="culture">en|fr</requirement>
                <requirement key="_format">html|rss</requirement>
                <requirement key="year">\d+</requirement>
            </route>
        </routes>

    .. code-block:: php

        use Symfony\Component\Routing\RouteCollection;
        use Symfony\Component\Routing\Route;

        $collection = new RouteCollection();
        $collection->add('homepage', new Route('/articles/{culture}/{year}/{title}.{_format}', array(
            '_controller' => 'AcmeDemoBundle:Article:show',
            '_format' => 'html',
        ), array(
            'culture' => 'en|fr',
            '_format' => 'html|rss',
            'year' => '\d+',
        )));

        return $collection;

Come si sarà visto, questa rotta verrà soddisfatta solo quando la porzione ``{culture}``
dell'URL è ``en`` o ``fr`` e se ``{year}`` è un numero. Questa
rotta mostra anche come sia possibile utilizzare un punto tra i segnaposto al posto di
una barra. Gli URL corrispondenti a questa rotta potrebbero essere del tipo:

 * ``/articles/en/2010/my-post``
 * ``/articles/fr/2010/my-post.rss``

.. _book-routing-format-param:

.. sidebar:: Il parametro speciale ``_format`` per le rotte

    Questo esempio mette in evidenza lo speciale parametro per le rotte ``_format``.
    Quando si utilizza questo parametro, il valore cercato diventa il "formato della richiesta"
    dell'oggetto ``Request``. In definitiva, il formato della richiesta è usato per
    cose tipo impostare il ``Content-Type`` della risposta (per esempio una richiesta
    di formato ``json`` si traduce in un ``Content-Type`` con valore ``application/json``).
    Può essere utilizzato anche nel controllore per rendere un template diverso
    per ciascun valore di ``_format``. Il parametro ``_format`` è un modo molto potente
    per rendere lo stesso contenuto in formati diversi.

Parametri speciali per le rotte
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Come si è visto, ogni parametro della rotta o valore predefinito è disponibile
come parametro nel metodo del controllore. Inoltre, ci sono tre parametri
speciali: ciascuno aggiunge una funzionalità all'interno dell'applicazione:

* ``_controller``: Come si è visto, questo parametro viene utilizzato per determinare quale
  controllore viene eseguito quando viene trovata la rotta;

* ``_format``: Utilizzato per impostare il formato della richiesta (:ref:`per saperne di più<book-routing-format-param>`);

* ``_locale``: Utilizzato per impostare il locale sulla richiesta (:ref:`per saperne di più<book-translation-locale-url>`);

.. tip::

    Se si usa il parametro ``_locale`` in una rotta, il valore sarà memorizzato
    nella sessione, in modo che le richieste successive lo mantengano.

.. index::
   single: Rotte; Controllori
   single: Controllore; Formato dei nomi delle stringhe

.. _controller-string-syntax:

Schema per il nome dei controllori
----------------------------------

Ogni rotta deve avere un parametro ``_controller``, che determina quale
controllore dovrebbe essere eseguito quando si accoppia la rotta. Questo parametro
utilizza un semplice schema stringa, chiamato *nome logico del controllore*, che
Symfony mappa in uno specifico metodo PHP di una certa classe. Lo schema ha tre parti,
ciascuna separata da due punti:

    **bundle**:**controllore**:**azione**

Per esempio, se ``_controller`` ha valore ``AcmeBlogBundle:Blog:show`` significa:

+----------------+------------------------+-----------------+
| Bundle         | Classe del controllore | Nome del metodo |
+================+========================+=================+
| AcmeBlogBundle | BlogController         | showAction      |
+----------------+------------------------+-----------------+

Il controllore potrebbe essere simile a questo:

.. code-block:: php

    // src/Acme/BlogBundle/Controller/BlogController.php
    
    namespace Acme\BlogBundle\Controller;
    use Symfony\Bundle\FrameworkBundle\Controller\Controller;
    
    class BlogController extends Controller
    {
        public function showAction($slug)
        {
            // ...
        }
    }

Si noti che Symfony aggiunge la stringa ``Controller`` al nome della classe (``Blog``
=> ``BlogController``) e ``Action`` al nome del metodo (``show`` => ``showAction``).

Si potrebbe anche fare riferimento a questo controllore con il nome completo di classe
e metodo: ``Acme\BlogBundle\Controller\BlogController::showAction``.
Ma seguendo alcune semplici convenzioni, il nome logico è più conciso
e permette una maggiore flessibilità.

.. note::

   Oltre all'utilizzo del nome logico o il nome completo della classe,
   Symfony supporta un terzo modo per fare riferimento a un controllore. Questo metodo
   utilizza solo un separatore due punti (ad esempio ``nome_servizio:indexAction``) e
   fa riferimento al controllore come un servizio (vedere :doc:`/cookbook/controller/service`).

Parametri delle rotte e parametri del controllore
-------------------------------------------------

I parametri delle rotte (ad esempio ``{slug}``) sono particolarmente importanti perché
ciascuno è reso disponibile come parametro al metodo del controllore:

.. code-block:: php

    public function showAction($slug)
    {
      // ...
    }

In realtà, l'intera collezione ``defaults`` viene unita con i valori del
parametro per formare un singolo array. Ogni chiave di questo array è disponibile come
parametro sul controllore.

In altre parole, per ogni parametro del metodo del controllore, Symfony cerca
per un parametro della rotta con quel nome e assegna il suo valore a tale parametro.
Nell'esempio avanzato di cui sopra, qualsiasi combinazioni (in qualsiasi ordine) delle seguenti variabili
potrebbe essere usati come parametri per il metodo ``showAction()``:

* ``$culture``
* ``$year``
* ``$title``
* ``$_format``
* ``$_controller``

Dal momento che il segnaposto e la collezione ``defaults`` vengono uniti insieme, è disponibile
anche la variabile ``$_controller``. Per una trattazione più dettagliata,
vedere :ref:`route-parameters-controller-arguments`.

.. tip::

    È inoltre possibile utilizzare una variabile speciale ``$_route``, che è impostata sul
    nome della rotta che è stata abbinata.

.. index::
   single: Rotte; Importare risorse per le rotte

.. _routing-include-external-resources:

Includere risorse esterne per le rotte
--------------------------------------

Tutte le rotte vengono caricate attraverso un singolo file di configurazione, generalmente ``app/config/routing.yml``
(vedere `Creazione delle rotte`_ sopra). In genere, però, si desidera caricare le rotte
da altri posti, come un file di rotte presente all'interno di un bundle. Questo può
essere fatto "importando" il file:

.. configuration-block::

    .. code-block:: yaml

        # app/config/routing.yml
        acme_hello:
            resource: "@AcmeHelloBundle/Resources/config/routing.yml"

    .. code-block:: xml

        <!-- app/config/routing.xml -->
        <?xml version="1.0" encoding="UTF-8" ?>

        <routes xmlns="http://symfony.com/schema/routing"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://symfony.com/schema/routing http://symfony.com/schema/routing/routing-1.0.xsd">

            <import resource="@AcmeHelloBundle/Resources/config/routing.xml" />
        </routes>

    .. code-block:: php

        // app/config/routing.php
        use Symfony\Component\Routing\RouteCollection;

        $collection = new RouteCollection();
        $collection->addCollection($loader->import("@AcmeHelloBundle/Resources/config/routing.php"));

        return $collection;

.. note::

   Quando si importano le risorse in formato YAML, la chiave (ad esempio ``acme_hello``) non ha senso.
   Basta essere sicuri che sia unica, in modo che nessun'altra linea la sovrascriva.

La chiave ``resource`` carica la data risorsa di rotte. In questo esempio
la risorsa è il percorso completo di un file, dove la sintassi scorciatoia
``@AcmeHelloBundle`` viene risolta con il percorso del bundle. Il file importato potrebbe essere
tipo questo:

.. configuration-block::

    .. code-block:: yaml

        # src/Acme/HelloBundle/Resources/config/routing.yml
       acme_hello:
            pattern:  /hello/{name}
            defaults: { _controller: AcmeHelloBundle:Hello:index }

    .. code-block:: xml

        <!-- src/Acme/HelloBundle/Resources/config/routing.xml -->
        <?xml version="1.0" encoding="UTF-8" ?>

        <routes xmlns="http://symfony.com/schema/routing"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://symfony.com/schema/routing http://symfony.com/schema/routing/routing-1.0.xsd">

            <route id="acme_hello" pattern="/hello/{name}">
                <default key="_controller">AcmeHelloBundle:Hello:index</default>
            </route>
        </routes>

    .. code-block:: php

        // src/Acme/HelloBundle/Resources/config/routing.php
        use Symfony\Component\Routing\RouteCollection;
        use Symfony\Component\Routing\Route;

        $collection = new RouteCollection();
        $collection->add('acme_hello', new Route('/hello/{name}', array(
            '_controller' => 'AcmeHelloBundle:Hello:index',
        )));

        return $collection;

Le rotte di questo file sono analizzate e caricate nello stesso modo del file
principale delle rotte.

Prefissare le rotte importate
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Si può anche scegliere di fornire un "prefisso" per le rotte importate. Per esempio,
si supponga di volere che la rotta ``acme_hello`` abbia uno schema finale con ``/admin/hello/{name}``
invece di ``/hello/{name}``:

.. configuration-block::

    .. code-block:: yaml

        # app/config/routing.yml
        acme_hello:
            resource: "@AcmeHelloBundle/Resources/config/routing.yml"
            prefix:   /admin

    .. code-block:: xml

        <!-- app/config/routing.xml -->
        <?xml version="1.0" encoding="UTF-8" ?>

        <routes xmlns="http://symfony.com/schema/routing"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://symfony.com/schema/routing http://symfony.com/schema/routing/routing-1.0.xsd">

            <import resource="@AcmeHelloBundle/Resources/config/routing.xml" prefix="/admin" />
        </routes>

    .. code-block:: php

        // app/config/routing.php
        use Symfony\Component\Routing\RouteCollection;

        $collection = new RouteCollection();
        $collection->addCollection($loader->import("@AcmeHelloBundle/Resources/config/routing.php"), '/admin');

        return $collection;

La stringa ``/admin`` ora verrà preposta allo schema di ogni rotta
caricata dalla nuova risorsa delle rotte.

.. index::
   single: Rotte; Debug

Visualizzare e fare il debug delle rotte
----------------------------------------

L'aggiunta e la personalizzazione di rotte è utile, ma lo è anche essere in grado di visualizzare
e recuperare informazioni dettagliate sulle rotte. Il modo migliore per vedere tutte le rotte
dell'applicazione è tramite il comando di console ``router:debug``. Eseguire
il comando scrivendo il codice seguente dalla cartella radice del progetto

.. code-block:: bash

    php app/console router:debug

Il comando visualizzerà un utile elenco di *tutte* le rotte configurate
nell'applicazione:

.. code-block:: text

    homepage              ANY       /
    contact               GET       /contact
    contact_process       POST      /contact
    article_show          ANY       /articles/{culture}/{year}/{title}.{_format}
    blog                  ANY       /blog/{page}
    blog_show             ANY       /blog/{slug}

Inoltre è possibile ottenere informazioni molto specifiche su una singola rotta mettendo
il nome della rotta dopo il comando:

.. code-block:: bash

    php app/console router:debug article_show

.. index::
   single: Rotte; Generazione di URL

Generazione degli URL
---------------------

Il sistema delle rotte dovrebbe anche essere usato per generare gli URL. In realtà, il routing
è un sistema bidirezionale: mappa l'URL in un controllore + parametri e
una rotta + parametri di nuovo in un URL. I metodi
:method:`Symfony\\Component\\Routing\\Router::match` e
:method:`Symfony\\Component\\Routing\\Router::generate` formano questo sistema
bidirezionale. Si prenda la rotta dell'esempio precedente ``blog_show``::

    $params = $router->match('/blog/my-blog-post');
    // array('slug' => 'my-blog-post', '_controller' => 'AcmeBlogBundle:Blog:show')

    $uri = $router->generate('blog_show', array('slug' => 'my-blog-post'));
    // /blog/my-blog-post

Per generare un URL, è necessario specificare il nome della rotta (ad esempio ``blog_show``)
ed eventuali caratteri jolly (ad esempio ``slug = my-blog-post``) usati nello schema  per
questa rotta. Con queste informazioni, qualsiasi URL può essere generata facilmente:

.. code-block:: php

    class MainController extends Controller
    {
        public function showAction($slug)
        {
          // ...

          $url = $this->get('router')->generate('blog_show', array('slug' => 'my-blog-post'));
        }
    }

In una sezione successiva, si imparerà a generare URL dall'interno di un template.

.. tip::

    Se la propria applicazione usa richieste AJAX, si potrebbe voler
    generare URL in JavaScript, che siano basate sulla propria configurazione delle rotte.
    Usando `FOSJsRoutingBundle`_, lo si può fare:
    
    .. code-block:: javascript
    
        var url = Routing.generate('blog_show', { "slug": 'my-blog-post});

    Per ultetiori informazioni, vedere la documentazione del bundle.

.. index::
   single: Rotte; URL assoluti

Generare URL assoluti
~~~~~~~~~~~~~~~~~~~~~

Per impostazione predefinita, il router genera URL relativi (ad esempio ``/blog``). Per generare
un URL assoluto, è sufficiente passare ``true`` come terzo parametro del metodo
``generate()``:

.. code-block:: php

    $router->generate('blog_show', array('slug' => 'my-blog-post'), true);
    // http://www.example.com/blog/my-blog-post

.. note::

    L'host che viene usato quando si genera un URL assoluto è l'host
    dell'oggetto ``Request`` corrente. Questo viene rilevato automaticamente in base
    alle informazioni sul server fornite da PHP. Quando si generano URL assolute per
    script che devono essere eseguiti da riga di comando, sarà necessario impostare manualmente l'host
    desiderato sull'oggetto ``Request``:
    
    .. code-block:: php
    
        $request->headers->set('HOST', 'www.example.com');

.. index::
   single: Rotte; Generare URL in un template

Generare URL con query string
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Il metodo ``generate`` accetta un array di valori jolly per generare l'URI.
Ma se si passano quelli extra, saranno aggiunti all'URI come query string::

    $router->generate('blog', array('page' => 2, 'category' => 'Symfony'));
    // /blog/2?category=Symfony

Generare URL da un template
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Il luogo più comune per generare un URL è all'interno di un template quando si creano i collegamenti
tra le varie pagine dell'applicazione. Questo viene fatto esattamente come prima, ma utilizzando
una funzione helper per i template:

.. configuration-block::

    .. code-block:: html+jinja

        <a href="{{ path('blog_show', { 'slug': 'my-blog-post' }) }}">
          Read this blog post.
        </a>

    .. code-block:: php

        <a href="<?php echo $view['router']->generate('blog_show', array('slug' => 'my-blog-post')) ?>">
            Read this blog post.
        </a>

Possono anche essere generati gli URL assoluti.

.. configuration-block::

    .. code-block:: html+jinja

        <a href="{{ url('blog_show', { 'slug': 'my-blog-post' }) }}">
          Read this blog post.
        </a>

    .. code-block:: php

        <a href="<?php echo $view['router']->generate('blog_show', array('slug' => 'my-blog-post'), true) ?>">
            Read this blog post.
        </a>

Riassunto
---------

Il routing è un sistema per mappare l'URL delle richieste in arrivo in una funzione
controllore che dovrebbe essere chiamata a processare la richiesta. Il tutto
permette sia di creare URL "belle" che di mantenere la funzionalità dell'applicazione
disaccoppiata da questi URL. Il routing è un meccanismo bidirezionale, nel senso che
dovrebbe anche essere utilizzato per generare gli URL.

Imparare di più dal ricettario
------------------------------

* :doc:`/cookbook/routing/scheme`

.. _`FOSJsRoutingBundle`: https://github.com/FriendsOfSymfony/FOSJsRoutingBundle
