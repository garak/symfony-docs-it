.. index::
   single: Routing
   single: Componenti; Routing

Il componente Routing
=====================

   Il componente Routing confronta una richiesta HTTP con un insieme di variabili
   di configurazione.

Installazione
-------------

È possibile installare il componente in diversi modi:

* Utilizzando il repository ufficiale su Git (https://github.com/symfony/Routing);
* Installandolo via Composer (``symfony/routing`` su `Packagist`_).

.. include:: /components/require_autoload.rst.inc

Utilizzo
--------

Per poter usare un sistema di rotte di base, sono necessari tre elementi:

* Una :class:`Symfony\\Component\\Routing\\RouteCollection`, che contiene la definizione delle rotte (un'istanza della classe :class:`Symfony\\Component\\Routing\\Route`)
* Un :class:`Symfony\\Component\\Routing\\RequestContext`, che contiene informazioni relative alla richiesta
* Un :class:`Symfony\\Component\\Routing\\Matcher\\UrlMatcher`, che associa la richiesta a una singola rotta

Il seguente esempio assume che l'autoloader sia già stato configurato 
in modo tale da caricare il componente Routing::

    use Symfony\Component\Routing\Matcher\UrlMatcher;
    use Symfony\Component\Routing\RequestContext;
    use Symfony\Component\Routing\RouteCollection;
    use Symfony\Component\Routing\Route;

    $rotta = new Route('/pippo', array('controller' => 'MioControllore'))
    $rotte = new RouteCollection();
    $rotte->add('nome_rotta', $rotta);

    $contesto = new RequestContext($_SERVER['REQUEST_URI']);

    $abbinatore = new UrlMatcher($rotte, $contesto);

    $parametri = $abbinatore->match( '/pippo');
    // array('controller' => 'MioControllore', '_route' => 'nome_rotta')

.. note::

    Particolare attenzione va data al'utilizzo di ``$_SERVER['REQUEST_URI']``,
    perché potrebbe contenere qualsiasi parametro della richiesta inserito nel'URL
    creando problemi con l'abbinamento alla rotta. Un semplice modo per risolvere
    il problema è usare il componente HTTPFoundation come spiegato :ref:`sotto<components-routing-http-foundation>`.

È possibile aggiungere un numero qualsiasi di rotte a una classe
:class:`Symfony\\Component\\Routing\\RouteCollection`.

Il metodo :method:`RouteCollection::add()<Symfony\\Component\\Routing\\RouteCollection::add>`
accetta due parametri. Il primo è il nome della rotta, il secondo è un oggetto
:class:`Symfony\\Component\\Routing\\Route`, il cui costruttore si aspetta di ricevere
un percorso URL e un array di variabili personalizzate. L'array di variabili personalizzate 
può essere *qualsiasi cosa* che  abbia senso per l'applicazione e viene restituito
quando la rotta viene abbinata.

Se non viene trovato alcun abbinamento con la rotta verrà lanciata una
:class:`Symfony\\Component\\Routing\\Exception\\ResourceNotFoundException`.

Oltre al'array di variabili personalizzate, viene aggiunta la chiave ``_route``
che conterrà il nome della rotta abbinata.

Definire le rotte
~~~~~~~~~~~~~~~~~

La definizione completa di una rotta può contenere fino a quattro parti:

#. Lo schema dell'URL della rotta. È questo il valore con il quale si confronta l'URL passato a `RequestContext`.
Può contenere diversi segnaposto (per esempio ``{segnaposto}``)
che possono abbinarsi a parti dinamiche dell'URL.

#. Un array di valori base. Contiene un array di valori arbitrari
che verranno restituiti quando la richiesta viene abbinata alla rotta.

#. Un array di requisiti. Definisce i requisiti per i valori dei segnaposto
in forma di espressione regolare.

#. Un array di opzioni. Questo array contiene configurazioni interne per le rotte e,
solitamente, sono la parte di cui meno ci si interessa.

#. Un host. Viene cercata corrispondenza con l'host della richiesta. Vedere
   :doc:`/components/routing/hostname_pattern` per ulteriori dettagli.

#. Un array di schemi. Restringe a specifici schemi HTTP (``http``, ``https``).

#. Un array di metodi. Restringe a specifici metodi di richiesta HTTP (``HEAD``,
   ``GET``, ``POST``, ...).

.. versionadded:: 2.2
   Il supporto per l'host è stato aggiunto in Symfony 2.2

Si prenda la seguente rotta, che combina diversi dei concetti esposti::

   $route = new Route(
       '/archivio/{mese}', // pattern per la rotta
       array('controller' => 'mostraArchivio'), // valori predefiniti
       array('mese' => '[0-9]{4}-[0-9]{2}'), // requisiti
       array(), // opzioni
       '{sottodominio}.example.com', // host
       array(), // schemi
       array() // metodi
   );

   // ...

   $parametri = $abbinatore->match('/archivio/2012-01');
   // array(
   //     'controller' => 'mostraArchivio',
   //     'mese' => 2012-01',
   //     'subdomain' => 'www',
   //     '_route' => ...
   //  )

   $parametri = $abbinatore->match('/archivio/pippo');
   // lancia una ResourceNotFoundException

In questo caso la rotta viene trovata con ``/archivio/2012/01``, perché il segnaposto
``{mese}`` è associabile alla espressione regolare definita. Invece, per ``/archivio/pippo``,
*non* verrà trovata nessuna corrispondenza perché "pippo" non rispetta i requisiti del segnaposto.

.. tip::

    Per creare una corrispondenza che trovi tutti gli URL che inizino con un determinato percorso e
    terminino con un suffisso arbitrario, è possibile usare la seguente definizione::

        $rotta = new Route(
            '/inizio/{suffisso}',
            array('suffisso' => ''),
            array('suffisso' => '.*')
        );
    
Usare i prefissi
~~~~~~~~~~~~~~~~

È possibile aggiungere sia rotte che nuove istanze di
:class:`Symfony\\Component\\Routing\\RouteCollection` ad *un'altra* collezione.
In questo modo si possono creare alberi di rotte. Inoltre è possibile definire dei prefissi,
requisiti predefiniti, opzioni predefinite, schemi predefiniti e host predefinito per
tutte le rotte di un sotto albero, con i metodi forniti dalla classe
``RouteCollection``::

    $collezioneRadice = new RouteCollection();

    $subCollezione = new RouteCollection();
    $subCollezione->add(...);
    $subCollezione->add(...);
    $subCollezione->addPrefix('/prefisso');
    $subCollezione->addDefaults(array(...));
    $subCollezione->addRequirements(array(...));
    $subCollezione->addOptions(array(...));
    $subCollezione->setHost('admin.example.com');
    $subCollezione->setMethods(array('POST'));
    $subCollezione->setSchemes(array('https'));

    $collezioneRadice->addCollection($subCollezione);


Configurare i parametri della richiesta
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

:class:`Symfony\\Component\\Routing\\RequestContext` fornisce informazioni
relative alla richiesta attuale. Con questa classe, tramite il suo costruttore,
è possibile definire tutti i parametri di una richiesta HTTP::

    public function __construct(
        $baseUrl = '',
        $method = 'GET',
        $host = 'localhost',
        $scheme = 'http',
        $httpPort = 80,
        $httpsPort = 443,
        $path = '/',
        $queryString = ''
    )

.. _components-routing-http-foundation:

È possibile passare i valori della variabile ``$_SERVER`` per popolare
:class:`Symfony\\Component\\Routing\\RequestContext`. Ma se si utilizza il
componente :doc:`HttpFoundation</components/http_foundation/index>`, è possibile usarne la classe
:class:`Symfony\\Component\\HttpFoundation\\Request` per riempire la
:class:`Symfony\\Component\\Routing\\RequestContext` con una scorciatoia::

    use Symfony\Component\HttpFoundation\Request;

    $context = new RequestContext();
    $context->fromRequest(Request::createFromGlobals());

Generare un URL
~~~~~~~~~~~~~~~

Mentre la classe :class:`Symfony\\Component\\Routing\\Matcher\\UrlMatcher` cerca
di trovare una rotta che sia adeguata a una determinata richiesta, è anche possibile creare degli URL
a partire da una determinata rotta::

    use Symfony\Component\Routing\Generator\UrlGenerator;

    $rotte = new RouteCollection();
    $rotte->add('mostra_articolo', new Route('/mostra/{slug}'));

    $contesto = new RequestContext($_SERVER['REQUEST_URI']);

    $generatore = new UrlGenerator($rotte, $contesto);

    $url = $generatore->generate('mostra_articolo', array(
        'slug' => 'articolo-sul-mio-blog'
    ));
    // /mostra/articolo-sul-mio-blog

.. note::

    Se fosse stato definito il requisito dello ``_scheme``, verrebbe generata un URL assoluto
    nel caso in cui lo schema corrente :class:`Symfony\\Component\\Routing\\RequestContext`
    non fosse coerente con i requisiti.

Caricare le rotte da un file
~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Si è visto come sia semplice aggiungere rotte a una collezione direttamente tramite
PHP. Ma è anche possibile caricare le rotte da diversi tipi di file differenti.

Il componente del Routing contiene diverse classi di caricamento, ognuna delle quali
fornisce l'abilità di caricare collezioni di definizioni di rotte da file esterni
di diverso formato.
Ogni caricatore si aspetta di ricevere un'istanza di :class:`Symfony\\Component\\Config\\FileLocator`
come argomento del costruttore. Si può usare il :class:`Symfony\\Component\\Config\\FileLocator`
per definire una array di percorsi nei quali il caricatore andrà a cercare i file richiesti.
Se il file viene trova, il caricatore restituisce una :class:`Symfony\\Component\\Routing\\RouteCollection`.

Si utilizza il caricatore ``YamlFileLoader``, allora la definizione delle rotte sarà simile alla seguente:

.. code-block:: yaml

    # routes.yml
    rotta1:
        path:     /pippo
        defaults: { controller: 'MioControllore::pippoAction' }

    rotta2:
        path:     /pippo/pluto
        defaults: { controller: 'MioControllore::pippoPlutoAction' }

Per caricare questo file, è possibile usare il seguente codice.  Si presume che il file
``routes.yml`` sia nella stessa cartella in cui si trova i codice::

    use Symfony\Component\Config\FileLocator;
    use Symfony\Component\Routing\Loader\YamlFileLoader;

    // controlla al'interno della cartella *corrente*
    $cercatore = new FileLocator(array(__DIR__));
    $caricatore = new YamlFileLoader($cercatore);
    $collezione = $caricatore->load('routes.yml');

Oltre a :class:`Symfony\\Component\\Routing\\Loader\\YamlFileLoader` ci sono 
altri due caricatori che funzionano nello stesso modo:

* :class:`Symfony\\Component\\Routing\\Loader\\XmlFileLoader`
* :class:`Symfony\\Component\\Routing\\Loader\\PhpFileLoader`

Se si usa :class:`Symfony\\Component\\Routing\\Loader\\PhpFileLoader` sarà necessario fornire
il nome del file php che restituirà una :class:`Symfony\\Component\\Routing\\RouteCollection`::

    // FornitoreDiRotte.php
    use Symfony\Component\Routing\RouteCollection;
    use Symfony\Component\Routing\Route;

    $collezione = new RouteCollection();
    $collezione->add(
        'nome_rotta',
        new Route('/pippo', array('controller' => 'ControlloreEsempio'))
    );
    // ...

    return $collezione;

Rotte e Closure
...............

Esiste anche un :class:`Symfony\\Component\\Routing\\Loader\\ClosureLoader`, il quale
chiama una closure e ne utilizza il risultato come una :class:`Symfony\\Component\\Routing\\RouteCollection`::

    use Symfony\Component\Routing\Loader\ClosureLoader;

    $closure = function() {
        return new RouteCollection();
    };

    $caricatore = new ClosureLoader();
    $collezione = $caricatore->load($closure);

Rotte e annotazioni
...................

Ultime, ma non meno importanti sono
:class:`Symfony\\Component\\Routing\\Loader\\AnnotationDirectoryLoader` e
:class:`Symfony\\Component\\Routing\\Loader\\AnnotationFileLoader` usate per
caricare le rotte a partire dalle annotazioni delle classi. Questo articolo non
tratterà i dettagli di queste classi.

Il router tutto-in-uno
~~~~~~~~~~~~~~~~~~~~~~

La classe :class:`Symfony\\Component\\Routing\\Router` è un pacchetto tutto-in-uno
che permette i usare rapidamente il componente Routing. Il costruttore si aspetta di ricevere
l'istanza di un caricatore, un percorso per la definizione della rotta principale e alcuni altri parametri::

    public function __construct(
        LoaderInterface $loader,
        $resource,
        array $options = array(),
        RequestContext $context = null,
        array $defaults = array()
    );

Tramite l'opzione ``cache_dir`` è possibile abilitare la cache delle rotte (cioè se si fornisce
un percorso) o disabilitarla (se viene configurata a ``null``). La cache è realizzata automaticamente
nello sfondo, qualora la si volesse utilizzare. Un semplice esempio di come sia fatta la classe
:class:`Symfony\\Component\\Routing\\Router` è riportato di seguito::

    $cercatore = new FileLocator(array(__DIR__));
    $contestoRichiesta = new RequestContext($_SERVER['REQUEST_URI']);

    $router = new Router(
        new YamlFileLoader($cercatore),
        'routes.yml',
        array('cache_dir' => __DIR__.'/cache'),
        $contestoRichiesta,
    );
    $router->match('/pippo/pluto');

.. note::

    Se si utilizza la cache, il componente Routing compilerà nuove classi che saranno
    salvate in ``cache_dir``. Questo significa che lo script deve avere i permessi di scrittura
    nella cartella indicata.

.. _Packagist: https://packagist.org/packages/symfony/routing
