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

Sessioni
--------

Il componente HttpFoundation di Symfony2 ha un sotto-sistema per le sessioni molto potente
e flessibile, progettato per fornire una gestione delle sessioni tramite una semplice
interfaccia orientata agli oggetti, usando una varietà di driver per memorizzare la sessione.

.. versionadded:: 2.1
    L'interfaccia :class:`Symfony\\Component\\HttpFoundation\\Session\\SessionInterface`,
    insieme a molte altre modifiche, è stata aggiunta in Symfony 2.1.

Le sessioni sono usate tramite la semplice implementazione :class:`Symfony\\Component\\HttpFoundation\\Session\\Session`
dell'interfaccia :class:`Symfony\\Component\\HttpFoundation\\Session\\SessionInterface`.

Un rapido esempio::

    use Symfony\Component\HttpFoundation\Session\Session;

    $session = new Session();
    $session->start();

    // imposta e ottiene attributi di sessione
    $session->set('name', 'Drak');
    $session->get('name');

    // imposta messggi flash
    $session->getFlashBag()->add('notice', 'Profilo aggiornato');

    // recupera i messaggi
    foreach ($session->getFlashBag()->get('notice', array()) as $message) {
        echo "<div class='flash-notice'>$message</div>";
    }

API delle sessioni
~~~~~~~~~~~~~~~~~~

La classe :class:`Symfony\\Component\\HttpFoundation\\Session\\Session` implementa
:class:`Symfony\\Component\\HttpFoundation\\Session\\SessionInterface`.

La classe :class:`Symfony\\Component\\HttpFoundation\\Session\\Session` ha una semplice API,
suddivisa in un paio di gruppi.

Flusso della sessione

* :method:`Symfony\\Component\\HttpFoundation\\Session\\Session::start`:
  Fa partire la sessione. Non usare ``session_start()``.

* :method:`Symfony\\Component\\HttpFoundation\\Session\\Session::migrate`:
  Rigenera l'id di sessione. Non usare ``session_regenerate_id()``.
  Questo metodo, facoltativamente, può cambiare la scadenza del nuovo cookie, che sarà
  emesso alla chiamata di questo metodo.

* :method:`Symfony\\Component\\HttpFoundation\\Session\\Session::invalidate`:
  Pulisce i dati della sessione e rigenera la sessione. Non usare ``session_destroy()``.
  Questo non è altro che una scorciatoia per ``clear()`` e ``migrate()``.

* :method:`Symfony\\Component\\HttpFoundation\\Session\\Session::getId`: Restituisce
  l'id della sessione.

* :method:`Symfony\\Component\\HttpFoundation\\Session\\Session::setId`: Imposta
  l'id della sessione.

* :method:`Symfony\\Component\\HttpFoundation\\Session\\Session::getName`: Restituisce
  il nome della sessione.

* :method:`Symfony\\Component\\HttpFoundation\\Session\\Session::setName`: Imposta
  il nome della sessione.

Attributi della sessione

* :method:`Symfony\\Component\\HttpFoundation\\Session\\Session::set`:
  Imposta un attributo per chiave;

* :method:`Symfony\\Component\\HttpFoundation\\Session\\Session::get`:
  Restituisce un attributo per chiave;

* :method:`Symfony\\Component\\HttpFoundation\\Session\\Session::all`:
  Restituisce tutti gli attributi, come array chiave => valore;

* :method:`Symfony\\Component\\HttpFoundation\\Session\\Session::has`:
  Restituisce ``true`` se l'attributo esiste;

* :method:`Symfony\\Component\\HttpFoundation\\Session\\Session::keys`:
  Restituisce un array di chiavi di attributi;

* :method:`Symfony\\Component\\HttpFoundation\\Session\\Session::replace`:
  Imposta molti attributi contemporaneamente: accetta un array e imposta ogni coppia chiave => valore.

* :method:`Symfony\\Component\\HttpFoundation\\Session\\Session::remove`:
  Cancella un attributo per chiave;

* :method:`Symfony\\Component\\HttpFoundation\\Session\\Session::clear`:
  Pulisce tutti gli attributi;

Gli attributi sono memorizzati internamente in un "Bag", un oggetto PHP che agisce come
un array. Ci sono alcuni metodi per la gestione del "Bag":

* :method:`Symfony\\Component\\HttpFoundation\\Session\\Session::registerBag`:
  Registra una :class:`Symfony\\Component\\HttpFoundation\\Session\\SessionBagInterface`

* :method:`Symfony\\Component\\HttpFoundation\\Session\\Session::getBag`:
  Restituisce una :class:`Symfony\\Component\\HttpFoundation\\Session\\SessionBagInterface` per
  nome del bag.

* :method:`Symfony\\Component\\HttpFoundation\\Session\\Session::getFlashBag`:
  Restituisce la :class:`Symfony\\Component\\HttpFoundation\\Session\\Flash\\FlashBagInterface`.
  Questa è solo una scorciatoia.

Meta-dati della sessione

* :method:`Symfony\\Component\\HttpFoundation\\Session\\Session::getMetadataBag`:
  Restituisce la :class:`Symfony\\Component\\HttpFoundation\\Session\\Storage\MetadataBag`,
  che contiene informazioni sulla sessione.

Gestori del salvataggio
~~~~~~~~~~~~~~~~~~~~~~~

Il flusso della sessione di PHP ha sei possibili operazioni da eseguire. La sessione normale
segue `open`, `read`, `write` e `close`, con la possibilità di
`destroy` e `gc` (garbage collection, che fa scadere le vecchie sessioni: `gc`
viene richiamato in modo casuale, secondo la configurazione di PHP, e, se richiamato, è
invocato dopo l'operazione `open`). Si può approfondire l'argomento su
`php.net/session.customhandler`_


Gestori del salvataggio nativi di PHP
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

I gestori cosiddetti 'nativi' sono gestori di sessione che sono o compilati in
PHP o forniti da estensioni di PHP, come PHP-Sqlite, PHP-Memcached e così via.
I gestori sono compilato e possono essere attivati direttamente in PHP, usando
`ini_set('session.save_handler', $nome);` e solitamente sono configurati con
`ini_set('session.save_path', $percorso);` e, talvolta, diverse altre direttive
`ini`.

Symfony2 fornisce driver per i gestori nativi, facili da configurare:

  * :class:`Symfony\\Component\\HttpFoundation\\Session\\Storage\\Handler\\NativeFileSessionHandler`;
  * :class:`Symfony\\Component\\HttpFoundation\\Session\\Storage\\Handler\\NativeSqliteSessionHandler`;
  * :class:`Symfony\\Component\\HttpFoundation\\Session\\Storage\\Handler\\NativeMemcacheSessionHandler`;
  * :class:`Symfony\\Component\\HttpFoundation\\Session\\Storage\\Handler\\NativeMemcachedSessionHandler`;
  * :class:`Symfony\\Component\\HttpFoundation\\Session\\Storage\\Handler\\NativeRedisSessionHandler`;

Esempio di utilzzo::

    use Symfony\Component\HttpFoundation\Session\Session;
    use Symfony\Component\HttpFoundation\Session\Storage\NativeSessionStorage;
    use Symfony\Component\HttpFoundation\Session\Storage\Handler\NativeMemcachedSessionHandler;

    $storage = new NativeSessionStorage(array(), new NativeMemcachedSessionHandler());
    $session = new Session($storage);

Gestori del salvataggio personalizzati
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

I gestori personalizzati sono quelli che sostituiscono completamente i gestori del salvataggio
nativi di PHP, fornendo sei funzioni di callback, richiamate internamente da PHP in vari
punti del flusso della sessione.

HttpFoundation di Symfony2 ne fornisce alcuni predefiniti, che possono facilmente servire
da esempi, se se ne vuole scrivere uno.

  * :class:`Symfony\\Component\\HttpFoundation\\Session\\Storage\\Handler\\PdoSessionHandler`;
  * :class:`Symfony\\Component\\HttpFoundation\\Session\\Storage\\Handler\\MemcacheSessionHandler`;
  * :class:`Symfony\\Component\\HttpFoundation\\Session\\Storage\\Handler\\MemcachedSessionHandler`;
  * :class:`Symfony\\Component\\HttpFoundation\\Session\\Storage\\Handler\\NullSessionHandler`;

Esempio::

    use Symfony\Component\HttpFoundation\Session\Session;
    use Symfony\Component\HttpFoundation\Session\Storage\SessionStorage;
    use Symfony\Component\HttpFoundation\Session\Storage\Handler\PdoSessionHandler;

    $storage = new NativeSessionStorage(array(), new PdoSessionHandler());
    $session = new Session($storage);

Bag della sessione
------------------

La gestione della sessione di PHP richiede l'uso della variabile super-globale `$_SESSION`,
tuttavia questo interferisce in qualche modo con la testabilità del codice e con l'incapsulamento
in un paradigma OOP. Per superare il problema, Symfony2 usa dei 'bag' di sessione, collegati
alla sessione per incapsulare uno specifico insieme di 'attributi' o 'messaggi flash'.

Questo approccio mitiga anche l'inquinamento dello spazio dei nomi all'interno di `$_SESSION`,
perché ogni bas memorizza i suoi dati sotto uno spazio dei nomi univoco.
Questo consente a Symfony2 di coesistere in modo pacifico con altre applicazioni o librerie
che potrebbero usare `$_SESSION`, mantenendo tutti i dati completamente compatibili
con la gestione delle sessioni di Symfony2.

Symfony2 fornisce due tipi di bag, con due implementazioni separate.
Ogni cosa è scritta su interfacce, quindi si può estendere o creare i propri tipi di
bag, se necessario.

:class:`Symfony\\Component\\HttpFoundation\\Session\\SessionBagInterface` ha la
seguente API, intesa principalmente per scopi interni:

* :method:`Symfony\\Component\\HttpFoundation\\Session\\SessionBagInterface::getStorageKey`:
  restituisce la chiave che il bag memorizzerà nell'array sotto `$_SESSION`.
  In generale questo valore può essere lasciato al suo predefinito ed è per uso interno.

* :method:`Symfony\\Component\\HttpFoundation\\Session\\SessionBagInterface::initialize`:
  richiamato internamente dalle classi memorizzazione della sessione di Symfony2 per collegare
  i dati del bag alla sessione.

* :method:`Symfony\\Component\\HttpFoundation\\Session\\SessionBagInterface::getName`:
  Restituisce il nome del bag della sessione.

Attributi
~~~~~~~~~

Lo scopo dei bag che implementano :class:`Symfony\\Component\\HttpFoundation\\Session\\Attribute\\AttributeBagInterface`
è gestire la memorizzazione degli attributi di sessione. Questo potrebbe includer cose come l'id utente,
le impostazioni "ricordami" o altre informazioni basate sullo stato dell'utente.

* :class:`Symfony\\Component\\HttpFoundation\\Session\\Attribute\\AttributeBag`
  è l'implementazione standard predefinita.

* :class:`Symfony\\Component\\HttpFoundation\\Session\\Attribute\\NamespacedAttributeBag`
  consente agli attributi di essere memorizzati in uno spazio dei nomi strutturato.

Qualsiasi sistema di memorizzazione `chiave => valore` è limitato riguardo alla complessità
dei dati che possono essere memorizzati, perché ogni chiave deve essere univoca. Si può ottenere
una sorta di spazio di nomi, introducendo una convenzione di nomi nelle chiavi, in modo che
le varie parti dell'applicazioni possano operare senza interferenze. Per esempio, `modulo1.pippo`
e `modulo2.pippo`. Tuttavia, a volte questo non è molto pratico quando gli attributi sono
array, per esempio un insieme di token. In questo caso, gestire l'array diventa pesante,
perché di deve recuperare l'array e poi processarlo e memorizzarlo di
nuovo::

    $tokens = array('tokens' => array('a' => 'a6c1e0b6',
                                      'b' => 'f4a7b1f3'));

Quindi ogni processamento può rapidamente diventare brutto, persino la semplice aggiunta
di un token all'array::

    $tokens = $session->get('tokens');
    $tokens['c'] = $value;
    $session->set('tokens', $tokens);

Con uno spazio di nomi strutturato, la chiave può essere tradotta nella struttura
dell'array, usando un carattere che crei lo spazio dei nomi (predefinito a `/`)::

    $session->set('tokens/c', $value);

In questo modo si può accedere facilmente a una chiave nell'array direttamente e facilmente.

:class:`Symfony\\Component\\HttpFoundation\\Session\\Attribute\\AttributeBagInterface`
ha una semplice API

* :method:`Symfony\\Component\\HttpFoundation\\Session\\Attribute\\AttributeBagInterface::set`:
  Imposta un attributo per chiave;

* :method:`Symfony\\Component\\HttpFoundation\\Session\\Attribute\\AttributeBagInterface::get`:
  Restituisce un attributo per chiave;

* :method:`Symfony\\Component\\HttpFoundation\\Session\\Attribute\\AttributeBagInterface::all`:
  Restituisce tutti gli attributi come array chiave => valore;

* :method:`Symfony\\Component\\HttpFoundation\\Session\\Attribute\\AttributeBagInterface::has`:
  Restituisce ``true`` se l'attributo esiste;

* :method:`Symfony\\Component\\HttpFoundation\\Session\\Attribute\\AttributeBagInterface::keys`:
  Restituisce un array di chiavi di attributi;

* :method:`Symfony\\Component\\HttpFoundation\\Session\\Attribute\\AttributeBagInterface::replace`:
  Imposta molti attributi contemporaneamente: accetta un array e imposta ogni coppia chiave => valore.

* :method:`Symfony\\Component\\HttpFoundation\\Session\\Attribute\\AttributeBagInterface::remove`:
  Cancella un attributo per chiave;

* :method:`Symfony\\Component\\HttpFoundation\\Session\\Attribute\\AttributeBagInterface::clear`:
  Pulisce il bag;

Messaggi flash
~~~~~~~~~~~~~~

Lo scopo di :class:`Symfony\\Component\\HttpFoundation\\Session\\Flash\\FlashBagInterface`
è fornire un modo di impostare e recuperare messaggi basati sulla sessione.
Il flusso dei messaggi flash di solito è impostarli in una richiesta e mostrarli dopo
il rinvio di una pagina. Per esempio, un utente invia un form che esegue un controllore
che aggiorna un dato e dopo il processo il controllore rinvia o alla pagina di
aggiornamento o a quella di errore. I messaggi flash impostati nella pagina precedente
sarebbero mostrati immediatamente nella pagina successiva.
Tuttavia questa è solo una possibile applicazione per i messaggi flash.

* :class:`Symfony\\Component\\HttpFoundation\\Session\\Flash\\AutoExpireFlashBag`
   con questa implementazione, i messaggi impostati in una pagina saranno disponibili
   per essere mostrati sono al caricamento della pagina successiva. Tali messaggi
   scadranno automaticamente, che siano stati recuperati o meno.

* :class:`Symfony\\Component\\HttpFoundation\\Session\\Flash\\FlashBag`
   con questa implementazione, i messaggi rimarranno i sessione finché non saranno
   esplicitamente recuperati o rimossi. Questo rende possibile l'utilizzo della
   cache ESI.

:class:`Symfony\\Component\\HttpFoundation\\Session\\Flash\\FlashBagInterface`
ha una semplice API

* :method:`Symfony\\Component\\HttpFoundation\\Session\\Flash\\FlashBagInterface::add`:
  aggiunge un messaggio flash alla pila del tipo specificato;

* :method:`Symfony\\Component\\HttpFoundation\\Session\\Flash\\FlashBagInterface::set`:
  imposta i flash per tipo. Questo metodo accetta sia messaggi singoli come stringa,
  che messaggi multipli come array.

* :method:`Symfony\\Component\\HttpFoundation\\Session\\Flash\\FlashBagInterface::get`:
  restituisce i flash per tipo e cancella tali flash dal bag;

* :method:`Symfony\\Component\\HttpFoundation\\Session\\Flash\\FlashBagInterface::setAll`:
  imposta tutti i flash, accetta un array di array con chiavi ``tipo => array(messaggi)``;

* :method:`Symfony\\Component\\HttpFoundation\\Session\\Flash\\FlashBagInterface::all`:
  restituisce tutti i flash (come array di array con chiavi) e cancella i flash dal bag;

* :method:`Symfony\\Component\\HttpFoundation\\Session\\Flash\\FlashBagInterface::peek`:
  restituisce i flash per tipo (sola lettura);

* :method:`Symfony\\Component\\HttpFoundation\\Session\\Flash\\FlashBagInterface::peekAll`:
  restituisce tutti i flash (sola lettura) come array di array con chiavi;

* :method:`Symfony\\Component\\HttpFoundation\\Session\\Flash\\FlashBagInterface::has`:
  restituisce ``true`` se il tipo esiste, ``false`` altrimenti;

* :method:`Symfony\\Component\\HttpFoundation\\Session\\Flash\\FlashBagInterface::keys`:
  restituisce un array di tipi di flash memorizzati;

* :method:`Symfony\\Component\\HttpFoundation\\Session\\Flash\\FlashBagInterface::clear`:
  pulisce il bag;

Solitamente, per applicazioni semplici basta avere un solo messaggio flash per
tipo, per esempio una nota di conferma dopo l'invio di un form. Tuttavia,
i messaggi flash sono memorizzati in un array per ``$type``, il che vuol dire che
l'applicazione può inviare più messaggi di un dato tipo. Questo consente l'uso dell'API
per messaggi più complessi.

Esempi di impostazioni di flash multipli::

    use Symfony\Component\HttpFoundation\Session\Session;

    $session = new Session();
    $session->start();

    // aggiunge i messaggi flash
    $session->getFlashBag()->add('warning', 'Il file di config è scrivibile, dovrebbe essere in sola lettura');
    $session->getFlashBag()->add('error', 'Aggiornamento del nome fallito');
    $session->getFlashBag()->add('error', 'Un altro errore');

Si potrebbero mostrare i messaggi in questo modo:

Semplice, mostra un tipo di messaggio::

    // mostra avvertimenti
    foreach ($session->getFlashBag()->get('warning', array()) as $message) {
        echo "<div class='flash-warning'>$message</div>";
    }

    // mostra errori
    foreach ($session->getFlashBag()->get('error', array()) as $message) {
        echo "<div class='flash-error'>$message</div>";
    }

Metodo compatto per processare la visualizzazione di tutti i flash in un colpo solo::

    foreach ($session->getFlashBag()->all() as $type => $messages) {
        foreach ($messages as $message) {
            echo "<div class='flash-$type'>$message</div>\n";
        }
    }

Testabilità
-----------

Symfony2 è progettato dalla base con la testabilità del codice in mente. Per rendere il
proprio codice, che usa le sessioni, facilmente testabile, sono forniti due meccanismi
diversi di mock della memorizzazione, per i test unitari e per i test funzionali.

Il test del codice con sessioni vere è complicato, perché il flusso di stato di PHP è
globale e non è possibile avere sessioni multiple concorrenti nello stesso processo
PHP.

Il motore dei mock di memorizzazione simula il flusso della sessione di PHP senza
effettivamente iniziarne una, consentendo il test del codice senza complicazioni. Si
possono anche eseguire istanze multiple nello stesso processo PHP.

I driver dei mock di memorizzazione non leggono né scrivono `session_id()` e `session_name()`
di sistema. Ci sono dei metodi per simularli, se necessario:

* :method:`Symfony\\Component\\HttpFoundation\\Session\\SessionStorageInterface::getId`: restituisce
  l'id di sessione.

* :method:`Symfony\\Component\\HttpFoundation\\Session\\SessionStorageInterface::setId`: imposta
  l'id di sessione.

* :method:`Symfony\\Component\\HttpFoundation\\Session\\SessionStorageInterface::getName`: restituisce
  il nome della sessione.

* :method:`Symfony\\Component\\HttpFoundation\\Session\\SessionStorageInterface::setName`: imposta
  il nome della sessione.

Test unitari
~~~~~~~~~~~~

Per test unitari in cui non serve persistere la sessione, si dovrebbe semplicemente
scambiare il sistema di memorizzazione predefinito con
:class:`Symfony\\Component\\HttpFoundation\\Session\\Storage\\MockArraySessionStorage`::

    use Symfony\Component\HttpFoundation\Session\Storage\MockArraySessionStorage;
    use Symfony\Component\HttpFoundation\Session\Session;

    $session = new Session(new MockArraySessionStorage());

Test funzionali
~~~~~~~~~~~~~~~

Per test funzionali in cui potrebbe servire persistere dati di sessione tra processi PHP
separati, cambiare semplicemente il sistema di memorizzazione in
:class:`Symfony\\Component\\HttpFoundation\\Session\\Storage\\MockFileSessionStorage`::

    use Symfony\Component\HttpFoundation\Session\Session;
    use Symfony\Component\HttpFoundation\Session\Storage\MockFileSessionStorage;

    $session = new Session(new MockFileSessionStorage());

Compatibilità con PHP 5.4
~~~~~~~~~~~~~~~~~~~~~~~~~

A partire da PHP 5.4.0, sono disponibili :phpclass:`SessionHandler` e
:phpclass:`SessionHandlerInterface`. Symfony 2.1 fornisce compatibilità in avanti per
:phpclass:`SessionHandlerInterface`, in modo che possa essere usata con PHP 5.3.
Questo aumenta molto l'interoperabilità con altre
librerie.

:phpclass:`SessionHandler` è una classe interna speciale di PHP, che espone i gestori del
salvataggio nativi nello user space di PHP. Per poter fornire una soluzione a chi usa
PHP 5.4, Symfony2 ha una classe speciale, chiamata
:class:`Symfony\\Component\\HttpFoundation\\Session\\Storage\\Handler\\NativeSessionHandler`,
che sotto PHP 5.4 estende da `\SessionHandler` e sotto PHP 5.3 è solo una classe
di base vuota. Questo dà alcune interessanti opportunità, per sfruttare le
funzionalità di PHP 5.4, se disponibile.

Proxy per il gestore del salvataggio
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Ci sono due tpi di classi di proxy per il gestore del salvataggio, che ereditano da
:class:`Symfony\\Component\\HttpFoundation\\Session\\Storage\\Handler\\AbstractProxy`:
sono :class:`Symfony\\Component\\HttpFoundation\\Session\\Storage\\Handler\\NativeProxy`
e :class:`Symfony\\Component\\HttpFoundation\\Session\\Storage\\Handler\\SessionHandlerProxy`.

:class:`Symfony\\Component\\HttpFoundation\\Session\\Storage\\NativeSessionStorage`
inietta automaticamente i gestori della memorizzazione in un proxy per il gestore del
salvataggio, a meno che non ce ne sia giù uno che lo avvolge.

:class:`Symfony\\Component\\HttpFoundation\\Session\\Storage\\Handler\\NativeProxy`
è usato automaticamente sotto PHP 5.3, quando i gestori del salvataggio interni di PHP
vengono specificati tramite le classi `Native*SessionHandler` classes, mentre
:class:`Symfony\\Component\\HttpFoundation\\Session\\Storage\\Handler\\SessionHandlerProxy`
sarà usato per avvolgere qualsiasi gestore del salvataggio personalizzato, che implementi :phpclass:`SessionHandlerInterface`.

Sotto PHP 5.4 e successivi, tutti i gestori di sessione implementano :phpclass:`SessionHandlerInterface`,
incluse le classi `Native*SessionHandler` che ereditano da :phpclass:`SessionHandler`.

Il meccanismo del proxy consente di essere coinvolto in modo più approfondito nelle classi
dei gestori del salvataggio. Un proxy, per esempio, può essere usato per criptare ogni
transazione di una sessione, senza sapere nulla del gestore del salvataggio specifico.

Configurare le sessioni di PHP
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

:class:`Symfony\\Component\\HttpFoundation\\Session\\Storage\\NativeSessionStorage` può
configurare la maggior parte delle direttive di php.ini documentate su
`php.net/session.configuration`_.

Per configurare tali impostazioni, passare le chavi (omettendo la parte ``session.`` iniziale
della chiave) come array chiave-valore al parametro ``$options`` del costruttore.
Oppure impostarle tramite il metodo
:method:`Symfony\\Component\\HttpFoundation\\Session\\Storage\\NativeSessionStorage::setOptions`
.

Per ulteriore chiarezza, alcune chiavi di opzioni sono spiegate in questa documentazione.

Scadenza del cookie di sessione
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Per sicurezza, generalmente si raccomanda di inviare i token di sessione come cookie.
SI può configurare la scadenza dei cookie di sessione, specificando il tempo
(in secondi) usando la chiave ``cookie_lifetime`` nel parametro ``$options`` del
costruttore di :class:`Symfony\\Component\\HttpFoundation\\Session\\Storage\\NativeSessionStorage`.

Impostare ``cookie_lifetime`` a ``0`` farà sì che il cookie durerà solo finché il
browser non resta aperto. Generalmente, ``cookie_lifetime`` andrebbe impostato a
un numero relativamente grande di giorni, settimane o mesi. Non è raro impostare i
cookie per un anno o più, a seconda dell'applicazione.

Essendo i cookie di sessione dei token esclusivamente lato client, sono meno importanti
nel controllo dei dettagli rispetto alle impostazioni di sicurezza, che alla fine possono
essere controllate con tranquillità solamente lato server.

.. note::

    L'impostazione ``cookie_lifetime`` è il numero di secondi per cui il cookie sarà
    valido, non è un timestamp Unix. Il cookie di sessione risultante sarà emesso con
    un tempo di scadenza di ``time()``+``cookie_lifetime``, con riferimento alla
    data del server.

Configurare il garbage collector
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Quando si apre una sessione, PHP richiamerà ``gc`` in modo casuale, in base alla
probabilità impostata da ``session.gc_probability`` / ``session.gc_divisor``. Per
esempio, se impostati rispettivamente a ``5/100``, risulterebbe in una probabilità
del 5%. In modo simile, ``3/4`` vorrebbe dire 3 possibilità su 4 di essere richiamato,
quindi il 75%.

Se il garbage collector viene invocato, PHP passerà il valore memorizzato nella
direttiva php.ini ``session.gc_maxlifetime`. Il significato in questo contesto è
che ogni sessione memorizzata prima di ``maxlifetime`` secondi fa andrebbe
cancellata. Questo consente di far scadere le sessioni in base al tempo di inattività.

Si possono impostare queste impostazioni passando ``gc_probability``, ``gc_divisor``
e ``gc_maxlifetime`` in un array al costruttore di
:class:`Symfony\\Component\\HttpFoundation\\Session\\Storage\\NativeSessionStorage`
o al metodo :method:`Symfony\\Component\\HttpFoundation\\Session\\Storage\\NativeSessionStorage::setOptions()`
.

Scadenza della sessione
~~~~~~~~~~~~~~~~~~~~~~~

Quando viene creata una nuova sessione, quindi quando Symfony2 invia un nuovo cookie di
sessione al client, il cookie sarà emesso con un tempo di scadenza. Questo tempo è
calcolato aggiungendo il valore di configurazione di PHP in
``session.cookie_lifetime`` al tempo attuale del server.

.. note::

    PHP invierà un cookie una sola volta. Ci si aspetta che il client memorizzi tale
    cookie per l'intero periodo. Sarà inviato un nuovo cookie solo quando la sessione
    viene distrutta, il cookie viene cancellato dal browser oppure l'id della sessione
    viene rigenerato, usando i metodi ``migrate()`` o ``invalidate()`` della classe ``Session``.

    Il tempo di scadenza iniziale del cookie può essere impostato configurando ``NativeSessionStorage``,
    usando il metodo ``setOptions(array('cookie_lifetime' => 1234))``.

.. note::

    Un tempo di scadenza del cookie di ``0`` vuol dire che il cookie scadrà alla chiusura del browser.

Tempo di inattività della sessione
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Spesso, ci sono circostanze in cui si vuole proteggere l'uso della sessione oppure
minimizzarne l'uso non autorizzato, quando un utente si allontana dalla sua postazione
mentre è loggato, distruggendo la sessione dopo un certo periodo di inattività. Per
esempio, solitamente le applicazioni delle banche buttano fuori l'utente dopo appena 5
o 10 minuti di inattività. L'impostazione della scadenza del cookie, in questo caso, non
è appropriata, perché potrebbe essere manipolata dal client, quindi occorre farlo
scadere lato server. Il modo più facile di farlo è tramite il garbage collector, che viene
eseguito con una frequenza ragionevole. Il ``lifetime`` del cookie andrebbe impostato a
un valore relativamente alto e il ``maxlifetime`` del garbage collectore andrebbe impostato
per distruggere le sessioni al periodo di inattività desiderato.

L'altra opzione è verificare specificatamente se una sessione sia scaduta dopo che
la sessione parte. La sessione può essere distrutta, come richiesto. Questo metodo può
consentire di integrare la scadenza delle sessioni nell'esperienza utente, per esempio,
mostrando un messaggio.

Symfony2 registra alcuni meta-dati di base su ogni sessione, per dare completa libertà
in quest'area.

Meta-dati di sessione
~~~~~~~~~~~~~~~~~~~~~

Le sessioni vengono decorate da alcuni meta-dati di base, per consentire maggior controllo
sulle impostazioni di sicurezza. L'oggetto sessione ha un getter per i meta-dati,
:method:`Symfony\\Component\\HttpFoundation\\Session\\Session::getMetadataBag`, che
espone un'istanza di :class:`Symfony\\Component\\HttpFoundation\\Session\\Storage\\MetadataBag`::

    $session->getMetadataBag()->getCreated();
    $session->getMetadataBag()->getLastUsed();

Entrambi i metodi restituiscono un timestamp Unix (relativo al server).

Questi meta-dati possono essere usati per far scadere in modo espliciti una sessione all'accesso, p.e.::

    $session->start();
    if (time() - $session->getMetadataBag()->getLastUpdate() > $maxIdleTime) {
        $session->invalidate();
        throw new SessionExpired(); // rinvia alla pagina di sessione scaduta
    }

Si può anche specificare a cosa è stato impostato ``cookie_lifetime`` per un determinato
cookie, usando il metodo ``getLifetime()``::

    $session->getMetadataBag()->getLifetime();

Il tempo di scadenza del cookie può essere determinato aggiungendo il timestamp creato
e il lifetime.

.. _`php.net/session.customhandler`: http://php.net/session.customhandler
.. _`php.net/session.configuration`: http://php.net/session.configuration
