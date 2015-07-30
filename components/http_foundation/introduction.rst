.. index::
   single: HTTP
   single: HttpFoundation
   single: Componenti; HttpFoundation

Il componente HttpFoundation
============================

    Il componente HttpFoundation definisce un livello orientato agli oggetti per le
    specifiche HTTP.

In PHP, la richiesta è rappresentata da alcune variabili globali (``$_GET``,
``$_POST``, ``$_FILE``, ``$_COOKIE``, ``$_SESSION``...) e la risposta è generata
da alcune funzioni (``echo``, ``header``, ``setcookie``, ...).

Il componente HttpFoundation di Symfony sostituisce queste variabili globali e queste
funzioni di PHP con un livello orientato agli oggetti.

Installazione
-------------

Si può installare il componente in due modi:

* Installarlo tramite :doc:`Composer </components/using_components>` (``symfony/http-foundation`` su `Packagist`_);
* Usare il repository ufficiale su Git (https://github.com/symfony/HttpFoundation).

.. include:: /components/require_autoload.rst.inc

.. _component-http-foundation-request:

Richiesta
---------

Il modo più comune per creare una richiesta è basarla sulle variabili attuali di PHP,
con
:method:`Symfony\\Component\\HttpFoundation\\Request::createFromGlobals`::

    use Symfony\Component\HttpFoundation\Request;

    $request = Request::createFromGlobals();

che è quasi equivalente al più verboso, ma anche più flessibile,
:method:`Symfony\\Component\\HttpFoundation\\Request::__construct`::

    $request = new Request(
        $_GET,
        $_POST,
        array(),
        $_COOKIE,
        $_FILES,
        $_SERVER
    );

Accedere ai dati della richiesta
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Un oggetto richiesta contiene informazioni sulla richiesta del client. Si può accedere a
queste informazioni tramite varie proprietà pubbliche:

* ``request``: equivalente di ``$_POST``;

* ``query``: equivalente di ``$_GET`` (``$request->query->get('name')``);

* ``cookies``: equivalente di ``$_COOKIE``;

* ``attributes``: non ha equivalenti, è usato dall'applicazione per memorizzare altri dati (vedere :ref:`sotto<component-foundation-attributes>`)

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
per allegare informazioni che appartengono alla richiesta e a cui si deve accedere
in diversi punti dell'applicazione. Per informazioni su come viene
usata nel framework Symfony, vedere
:ref:`il libro <book-fundamentals-attributes>`.

Infine, si può accedere ai dati grezzi inviati nel corpo della richiesta usando
:method:`Symfony\\Component\\HttpFoundation\\Request::getContent`::

    $content = $request->getContent();

Questo potrebbe essere utile, per esempio, per processare una stringa JSON inviata
all'applicazione da un servizio remoto tramite metodo HTTP POST.

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

    $request = Request::create(
        '/hello-world',
        'GET',
        array('name' => 'Fabien')
    );

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
:method:`Symfony\\Component\\HttpFoundation\\Request::getSession`. Il
metodo
:method:`Symfony\\Component\\HttpFoundation\\Request::hasPreviousSession`
dice se la richiesta contiene una sessione, che sia stata fatta partire in una delle
richieste precedenti.

Accedere ai dati degli header `Accept-*`
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Si può accedere facilmente ai dati di base estratti dagli header ``Accept-*``
usando i seguenti metodi:

* :method:`Symfony\\Component\\HttpFoundation\\Request::getAcceptableContentTypes`:
  restituisce la lista dei tipi di contenuto accettati, ordinata per qualità discendente;

* :method:`Symfony\\Component\\HttpFoundation\\Request::getLanguages`:
  restituisce la lista delle lingue accettate, ordinata per qualità discendente

* :method:`Symfony\\Component\\HttpFoundation\\Request::getCharsets`:
  restituisce la lista dei charset accettati, ordinata per qualità discendente

.. versionadded:: 2.2
    La classe :class:`Symfony\\Component\\HttpFoundation\\AcceptHeader` è stata
    introdotta in Symfony 2.2.

Se occorre pieno accesso ai dati analizzati da ``Accept``, ``Accept-Language``,
``Accept-Charset`` o ``Accept-Encoding``, si può usare la classe
:class:`Symfony\\Component\\HttpFoundation\\AcceptHeader`::

    use Symfony\Component\HttpFoundation\AcceptHeader;

    $accept = AcceptHeader::fromString($request->headers->get('Accept'));
    if ($accept->has('text/html')) {
        $item = $accept->get('text/html');
        $charset = $item->getAttribute('charset', 'utf-8');
        $quality = $item->getQuality();
    }

    // accepts items are sorted by descending quality
    $accepts = AcceptHeader::fromString($request->headers->get('Accept'))
        ->all();

Accedere ad altri dati
~~~~~~~~~~~~~~~~~~~~~~

La classe ``Request`` ha molti altri metodi, che si possono usare per accedere alle
informazioni della richiesta. Si dia uno sguardo alle 
:class:`API di Request<Symfony\\Component\\HttpFoundation\\Request>`
per maggiori informazioni.

.. _component-http-foundation-response:

Risposta
--------

Un oggetto :class:`Symfony\\Component\\HttpFoundation\\Response` contiene tutte le
informazioni che devono essere rimandate al client, per una data richiesta. Il
costruttore accetta fino a tre parametri: il contenuto della risposta, il codice di stato
e un array di header HTTP::

    use Symfony\Component\HttpFoundation\Response;

    $response = new Response(
        'Contenuto',
        200,
        array('content-type' => 'text/html')
    );

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

.. _streaming-response:

Flusso di risposta
~~~~~~~~~~~~~~~~~~

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

.. note::

    La funzione ``flush()`` non esegue il flush del buffer. Se è stato richiamato ``ob_start()``
    in precedenza oppure se l'opzione ``output_buffering`` è abilitata in php.ini,
    occorre richiamare ``ob_flush()`` prima di ``flush()``.

    Inoltre, PHP non è l'unico livello possibile di buffer dell'output. Il server web
    può anche eseguire un buffer, a seconda della configurazione. Ancora, se si
    usa fastcgi, non si può disabilitare affatto il buffer.

.. _component-http-foundation-serving-files:

Scaricare file
~~~~~~~~~~~~~~

Quando si scarica un file, occorre aggiungere un header ``Content-Disposition`` alla
risposta. Sebbene la creazione di questo header per scaricamenti di base sia facile,
l'uso di nomi di file non ASCII è più complesso. Il metodo
:method:`:Symfony\\Component\\HttpFoundation\\Response:makeDisposition`
astrae l'ingrato compito dietro una semplice API::

    use Symfony\Component\HttpFoundation\ResponseHeaderBag;

    $d = $response->headers->makeDisposition(
        ResponseHeaderBag::DISPOSITION_ATTACHMENT,
        'foo.pdf'
    );

    $response->headers->set('Content-Disposition', $d);

.. versionadded:: 2.2
    La classe :class:`Symfony\\Component\\HttpFoundation\\BinaryFileResponse`
    è stata aggiunta in Symfony 2.2.

In alternativa, se si sta servendo un file statico, si può usare
:class:`Symfony\\Component\\HttpFoundation\\BinaryFileResponse`::

    use Symfony\Component\HttpFoundation\BinaryFileResponse

    $file = 'percorrso/del/file.txt';
    $response = new BinaryFileResponse($file);

``BinaryFileResponse`` gestirà automaticamente gli header ``Range`` e
``If-Range`` della richiesta. Supporta anche ``X-Sendfile``
(vedere per `Nginx`_ e `Apache`_). Per poterlo usare, occorre determinare
se l'header ``X-Sendfile-Type`` sia fidato o meno e richiamare
:method:`Symfony\\Component\\HttpFoundation\\BinaryFileResponse::trustXSendfileTypeHeader`
in caso positivo::

    $response::trustXSendfileTypeHeader();

Si può ancora impostare il ``Content-Type`` del file inviato o cambiarne il ``Content-Disposition``::

    $response->headers->set('Content-Type', 'text/plain');
    $response->setContentDisposition(
        ResponseHeaderBag::DISPOSITION_ATTACHMENT,
        'nomefile.txt'
    );

.. _component-http-foundation-json-response:

Creare una risposta JSON
~~~~~~~~~~~~~~~~~~~~~~~~

Si può creare qualsiasi tipo di risposta tramite la classe
:class:`Symfony\\Component\\HttpFoundation\\Response`, impostando il contenuto
e gli header corretti. Una risposta JSON può essere come questa::

    use Symfony\Component\HttpFoundation\Response;

    $response = new Response();
    $response->setContent(json_encode(array(
        'data' => 123,
    )));
    $response->headers->set('Content-Type', 'application/json');

C'è anche un'utile classe :class:`Symfony\\Component\\HttpFoundation\\JsonResponse`,
che può rendere le cose ancora più semplici::

    use Symfony\Component\HttpFoundation\JsonResponse;

    $response = new JsonResponse();
    $response->setData(array(
        'data' => 123
    ));

Il risultato è una codifica dell'array di dati in JSON, con header ``Content-Type`` impostato
a ``application/json``.

.. caution::

    Per evitare un `JSON Hijacking`_ XSSI , bisogna passare un array associativo
    come parte più esterna dell'array a ``JsonResponse`` e non un array indicizzato, in modo
    che il risultato finale sia un oggetto (p.e. ``{"oggetto": "non dentro un array"}``)
    invece che un array (p.e. ``[{"oggetto": "dentro un array"}]``). Si leggano
    le `linee guida OWASP`_ per maggiori informazioni.

    Solo i metodi che rispondono a richieste GET sono vulnerabili a 'JSON Hijacking' XSSI.
    I metodi che rispondono a richieste POST restano immuni.

Callback JSONP
~~~~~~~~~~~~~~

Se si usa JSONP, si può impostare la funzione di callback
a cui i dati vanno passati::

    $response->setCallback('handleResponse');

In tal caso, l'header ``Content-Type`` sarà ``text/javascript`` e il
contenuto della risposta sarà come questo:

.. code-block:: javascript

    handleResponse({'data': 123});

Sessioni
--------

Le informazioni sulle sessioni sono nell'apposito documento: :doc:`/components/http_foundation/sessions`.

.. _Packagist: https://packagist.org/packages/symfony/http-foundation
.. _Nginx: http://wiki.nginx.org/XSendfile
.. _Apache: https://tn123.org/mod_xsendfile/
.. _`JSON Hijacking`: http://haacked.com/archive/2009/06/25/json-hijacking.aspx
.. _linee guida OWASP: https://www.owasp.org/index.php/OWASP_AJAX_Security_Guidelines#Always_return_JSON_with_an_Object_on_the_outside
