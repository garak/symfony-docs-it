.. index::
   single: HTTP
   single: HttpFoundation

Il componente HttpFoundation
============================

    Il componente HttpFoundation definisce un livello orientato agli oggetti per le
    specifiche HTTP.

In PHP, la richiesta è rappresentata da alcune variabili globali (``$_GET``,
``$_POST``, ``$_FILE``, ``$_COOKIE``, ``$_SESSION``...) e la risposta è generata
da alcune funzioni (``echo``, ``header``, ``setcookie``, ...).

Il componente HttpFoundation di Symfony2 sostituisce queste variabili globali e queste
funzioni di PHP con un livello orientato agli oggetti.

Installazione
-------------

Si può installare il componente in molti modi diversi:

* Usare il repository ufficiale su Git (https://github.com/symfony/HttpFoundation);
* Installarlo via PEAR (`pear.symfony.com/HttpFoundation`);
* Installarlo via Composer (`symfony/http-foundation` su Packagist).

Richiesta
---------

Il modo più comune per creare una richiesta è basarla sulle variabili attuali di PHP,
con
:method:`Symfony\\Component\\HttpFoundation\\Request::createFromGlobals`::

    use Symfony\Component\HttpFoundation\Request;

    $request = Request::createFromGlobals();

che è quasi equivalente al più verboso, ma anche più flessibile,
:method:`Symfony\\Component\\HttpFoundation\\Request::__construct`::

    $request = new Request($_GET, $_POST, array(), $_COOKIE, $_FILES, $_SERVER);

Accedere ai dati della richiesta
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Un oggetto richiesta contiene informazioni sulla richiesta del client. Si può accedere a
queste informazioni tramite varie proprietà pubbliche:

* ``request``: equivalente di ``$_POST``;

* ``query``: equivalente di ``$_GET`` (``$request->query->get('name')``);

* ``cookies``: equivalente di ``$_COOKIE``;

* ``attributes``: non ha equivalenti, è usato dall'applicazione per memorizzare alrti dati (vedere :ref:`sotto<component-foundation-attributes>`)

* ``files``: equivalente di ``$_FILE``;

* ``server``: equivalente di ``$_SERVER``;

* ``headers``: quasi equivalente di un sottinsieme di ``$_SERVER``
  (``$request->headers->get('Content-Type')``).

Ogni proprietà è un'istanza di :class:`Symfony\\Component\\HttpFoundation\\ParameterBag`
(o di una sua sotto-classe), che è una classe contenitore:

* ``request``: :class:`Symfony\\Component\\HttpFoundation\\ParameterBag`;

* ``query``:   :class:`Symfony\\Component\\HttpFoundation\\ParameterBag`;

* ``cookies``: :class:`Symfony\\Component\\HttpFoundation\\ParameterBag`;

* ``attributes``: :class:`Symfony\\Component\\HttpFoundation\\ParameterBag`;

* ``files``:   :class:`Symfony\\Component\\HttpFoundation\\FileBag`;

* ``server``:  :class:`Symfony\\Component\\HttpFoundation\\ServerBag`;

* ``headers``: :class:`Symfony\\Component\\HttpFoundation\\HeaderBag`.

Tutte le istanze di :class:`Symfony\\Component\\HttpFoundation\\ParameterBag` hanno metodi
per recuperare e aggiornare i propri dati:

* :method:`Symfony\\Component\\HttpFoundation\\ParameterBag::all`: Restituisce
  i parametri;

* :method:`Symfony\\Component\\HttpFoundation\\ParameterBag::keys`: Restituisce
  le chiavi dei parametri;

* :method:`Symfony\\Component\\HttpFoundation\\ParameterBag::replace`:
  Sostituisce i parametri attuali con dei nuovi;

* :method:`Symfony\\Component\\HttpFoundation\\ParameterBag::add`: Aggiunge
  parametri;

* :method:`Symfony\\Component\\HttpFoundation\\ParameterBag::get`: Restituisce un
  parametro per nome;

* :method:`Symfony\\Component\\HttpFoundation\\ParameterBag::set`: Imposta un
  parametro per nome;

* :method:`Symfony\\Component\\HttpFoundation\\ParameterBag::has`: Restituisce
  ``true`` se il parametro è definito;

* :method:`Symfony\\Component\\HttpFoundation\\ParameterBag::remove`: Rimuove
  un parametro.

La classe :class:`Symfony\\Component\\HttpFoundation\\ParameterBag` ha anche
alcuni metodi per filtrare i valori in entrata:

* :method:`Symfony\\Component\\HttpFoundation\\Request::getAlpha`: Restituisce
  i caratteri alfabetici nel valore del parametro;

* :method:`Symfony\\Component\\HttpFoundation\\Request::getAlnum`: Restituisce
  i caratteri alfabetici e i numeri nel valore del parametro;

* :method:`Symfony\\Component\\HttpFoundation\\Request::getDigits`: Restituisce
  i numeri nel valore del parametro;

* :method:`Symfony\\Component\\HttpFoundation\\Request::getInt`: Restituisce il
  valore del parametro convertito in intero;

* :method:`Symfony\\Component\\HttpFoundation\\Request::filter`: Filtra il
  parametro, usando la funzione PHP ``filter_var()``.

Tutti i getter accettano tre parametri: il primo è il nome del parametro e
il secondo è il valore predefinito, da restituire se il parametro non
esiste::

    // la query string è '?foo=bar'

    $request->query->get('foo');
    // restituisce bar

    $request->query->get('bar');
    // restituisce null

    $request->query->get('bar', 'bar');
    // restituisce 'bar'


Quando PHP importa la query della richiesta, gestisce i parametri della richiesta, come
``foo[bar]=bar``, in modo speciale, creando un array. In questo modo, si può richiedere il
parametro ``foo`` e ottenere un array con un elemento ``bar``. A volte, però,
si potrebbe volere il valore del nome "originale" del parametro:
``foo[bar]``. Ciò è possibile con tutti i getter di `ParameterBag`, come
:method:`Symfony\\Component\\HttpFoundation\\Request::get`, tramite il terzo
parametro::

        // la query string è '?foo[bar]=bar'

        $request->query->get('foo');
        // restituisce array('bar' => 'bar')

        $request->query->get('foo[bar]');
        // restituisce null

        $request->query->get('foo[bar]', null, true);
        // restituisce 'bar'

.. _component-foundation-attributes:

Infine, ma non meno importante, si possono anche memorizzare dati aggiuntivi nella
richiesta, grazie alla proprietà pubblica ``attributes``, che è anche un'istanza di
:class:`Symfony\\Component\\HttpFoundation\\ParameterBag`. La si usa soprattutto
per allegare informazioni che appartengono alla richiesta e a cui si deve accedere in
diversi punti della propria applicazione. Per informazioni su come viene usata
nel framework Symfony2, vedere :ref:`saperne di più<book-fundamentals-attributes>`.

Identificare una richiesta
~~~~~~~~~~~~~~~~~~~~~~~~~~

Nella propria applicazione, serve un modo per identificare una richiesta. La maggior
parte delle volte, lo si fa tramite il "path info" della richiesta, a cui si può accedere
tramite il metodo :method:`Symfony\\Component\\HttpFoundation\\Request::getPathInfo`::

    // per una richiesta a http://example.com/blog/index.php/post/hello-world
    // path info è "/post/hello-world"
    $request->getPathInfo();

Simulare una richiesta
~~~~~~~~~~~~~~~~~~~~~~

Invece di creare una richiesta basata sulle variabili di PHP, si può anche simulare
una richiesta::

    $request = Request::create('/hello-world', 'GET', array('name' => 'Fabien'));

Il metodo :method:`Symfony\\Component\\HttpFoundation\\Request::create`
crea una richiesta in base a path info, un metodo e alcuni parametri (i parametri
della query o quelli della richiesta, a seconda del metodo HTTP) e, ovviamente,
si possono forzare anche tutte le altre variabili (Symfony crea dei
valori predefiniti adeguati per ogni variabile globale di PHP).

In base a tale richiesta, si possono forzare le variabili globali di PHP tramite
:method:`Symfony\\Component\\HttpFoundation\\Request::overrideGlobals`::

    $request->overrideGlobals();

.. tip::

    Si può anche duplicare una query esistente, tramite
    :method:`Symfony\\Component\\HttpFoundation\\Request::duplicate`, o
    cambiare molti parametri con una singola chiamata a
    :method:`Symfony\\Component\\HttpFoundation\\Request::initialize`.

Accedere alla sessione
~~~~~~~~~~~~~~~~~~~~~~

Se si ha una sessione allegata alla richiesta, vi si può accedere tramite il metodo
:method:`Symfony\\Component\\HttpFoundation\\Request::getSession`. Il metodo
:method:`Symfony\\Component\\HttpFoundation\\Request::hasPreviousSession`
dice se la richiesta contiene una sessione, che sia stata fatta partire in una delle
richieste
precedenti.

Accedere ad altri dati
~~~~~~~~~~~~~~~~~~~~~~

La classe Request ha molti altri metodi, che si possono usare per accedere alle
informazioni della richiesta. Si dia uno sguardo alle API per maggiori informazioni.

Risposta
--------

Un oggetto :class:`Symfony\\Component\\HttpFoundation\\Response` contiene tutte le
informazioni che devono essere rimandate al client, per una data richiesta. Il
costruttore accetta fino a tre parametri: il contenuto della risposta, il codice di stato
e un array di header HTTP::

    use Symfony\Component\HttpFoundation\Response;

    $response = new Response('Contenuto', 200, array('content-type' => 'text/html'));

Queste informazioni possono anche essere manipolate dopo la creazione di Response::

    $response->setContent('Ciao mondo');

    // l'attributo pubblico headers è un ResponseHeaderBag
    $response->headers->set('Content-Type', 'text/plain');

    $response->setStatusCode(404);

Quando si imposta il ``Content-Type`` di Response, si può impostare il charset,
ma è meglio impostarlo tramite il metodo
:method:`Symfony\\Component\\HttpFoundation\\Response::setCharset`::

    $response->setCharset('ISO-8859-1');

Si noti che Symfony presume che le risposte siano codificate in
UTF-8.

Inviare la risposta
~~~~~~~~~~~~~~~~~~~

Prima di inviare la risposta, ci si può assicurare che rispetti le specifiche HTTP,
richiamando il metodo
:method:`Symfony\\Component\\HttpFoundation\\Response::prepare`::

    $response->prepare($request);

Inviare la risposta al client è quindi semplice, basta richiamare
:method:`Symfony\\Component\\HttpFoundation\\Response::send`::

    $response->send();

Impostare cookie
~~~~~~~~~~~~~~~~

Si possono manipolare i cookie della risposta attraverso l'attributo pubblico
``headers``::

    use Symfony\Component\HttpFoundation\Cookie;

    $response->headers->setCookie(new Cookie('pippo', 'pluto'));

Il metodo
:method:`Symfony\\Component\\HttpFoundation\\ResponseHeaderBag::setCookie`
accetta un'istanza di
:class:`Symfony\\Component\\HttpFoundation\\Cookie` come parametro.

Si può pulire un cookie tramite il metodo
:method:`Symfony\\Component\\HttpFoundation\\Response::clearCookie`.

Gestire la cache HTTP
~~~~~~~~~~~~~~~~~~~~~

La classe :class:`Symfony\\Component\\HttpFoundation\\Response` ha un corposo insieme
di metodi per manipolare gli header HTTP relativi alla cache:

* :method:`Symfony\\Component\\HttpFoundation\\Response::setPublic`;
* :method:`Symfony\\Component\\HttpFoundation\\Response::setPrivate`;
* :method:`Symfony\\Component\\HttpFoundation\\Response::expire`;
* :method:`Symfony\\Component\\HttpFoundation\\Response::setExpires`;
* :method:`Symfony\\Component\\HttpFoundation\\Response::setMaxAge`;
* :method:`Symfony\\Component\\HttpFoundation\\Response::setSharedMaxAge`;
* :method:`Symfony\\Component\\HttpFoundation\\Response::setTtl`;
* :method:`Symfony\\Component\\HttpFoundation\\Response::setClientTtl`;
* :method:`Symfony\\Component\\HttpFoundation\\Response::setLastModified`;
* :method:`Symfony\\Component\\HttpFoundation\\Response::setEtag`;
* :method:`Symfony\\Component\\HttpFoundation\\Response::setVary`;

Il metodo :method:`Symfony\\Component\\HttpFoundation\\Response::setCache` può
essere usato per impostare le informazioni di cache più comuni, con un'unica
chiamata::

    $response->setCache(array(
        'etag'          => 'abcdef',
        'last_modified' => new \DateTime(),
        'max_age'       => 600,
        's_maxage'      => 600,
        'private'       => false,
        'public'        => true,
    ));

Per verificare che i validatori della risposta (``ETag``, ``Last-Modified``) corrispondano
a un valore condizionale specificato nella richiesta del client, usare il metodo
:method:`Symfony\\Component\\HttpFoundation\\Response::isNotModified`::


    if ($response->isNotModified($request)) {
        $response->send();
    }

Se la risposta non è stata modificata, imposta il codice di stato a 304 e rimuove
il contenuto effettivo della risposta.

Rinviare l'utente
~~~~~~~~~~~~~~~~~

Per rinviare il client a un altro URL, si può usare la classe
:class:`Symfony\\Component\\HttpFoundation\\RedirectResponse`::

    use Symfony\Component\HttpFoundation\RedirectResponse;

    $response = new RedirectResponse('http://example.com/');

Flusso di risposta
~~~~~~~~~~~~~~~~~~

.. versionadded:: 2.1
    Il supporto per i flussi di risposte è stato aggiunto in Symfony 2.1.

La classe :class:`Symfony\\Component\\HttpFoundation\\StreamedResponse` consente
di inviare flussi di risposte al client. Il contenuto della risposta viene
rappresentato da un callable PHP, invece che da una stringa::

    use Symfony\Component\HttpFoundation\StreamedResponse;

    $response = new StreamedResponse();
    $response->setCallback(function () {
        echo 'Ciao mondo';
        flush();
        sleep(2);
        echo 'Ciao mondo';
        flush();
    });
    $response->send();

Scaricare file
~~~~~~~~~~~~~~

.. versionadded:: 2.1
    Il metodo ``makeDisposition`` è stato aggiunto in Symfony 2.1.

Quando si carica un file, occorre aggiungere un header ``Content-Disposition`` alla
risposta. Sebbene la creazione di questo header per scaricamenti di base sia facile,
l'uso di nomi di file non ASCII è più complesso. Il metodo
:method:`:Symfony\\Component\\HttpFoundation\\Response:makeDisposition`
astrae l'ingrato compito dietro una semplice API::

    use Symfony\\Component\\HttpFoundation\\ResponseHeaderBag;

    $d = $response->headers->makeDisposition(ResponseHeaderBag::DISPOSITION_ATTACHMENT, 'foo.pdf');

    $response->headers->set('Content-Disposition', $d);

Sessione
--------

TBD -- Questa parte non è ancora stata scritta, perché probabilmente sarà presto
rifattorizzata in Symfony 2.1.
