.. index::
   single: Cache

Cache HTTP
==========

Le applicazioni web sono dinamiche. Non importa quanto efficiente possa essere
un'applicazione, ogni richiesta conterrà sempre overhead rispetto a quando
si serve un file statico.

Per la maggior parte delle applicazioni, questo non è un problema. Symfony è
molto veloce e, a meno che non si stia facendo qualcosa di veramente molto pesante,
ogni richiesta sarà gestita rapidamente, senza stressare troppo il server.

Man mano che il sito cresce, però, quell'overhead può diventare un problema.
Il processo normalmente seguito a ogni richiesta andrebbe fatto una volta sola.
Questo è proprio lo scopo che si prefigge la cache.

La cache sulle spalle dei giganti
---------------------------------

Il modo più efficace per migliorare le prestazioni di un'applicazione è mettere in
cache l'intero output di una pagina e quindi aggirare interamente l'applicazione a
ogni richiesta successiva. Ovviamente, questo non è sempre possibile per siti altamente
dinamici, oppure sì? In questo capitolo, mostreremo come funziona il sistema di cache
di Symfony e perché pensiamo che sia il miglior approccio possibile.

Il sistema di cache di Symfony è diverso, perché si appoggia sulla semplicità e
sulla potenza della cache HTTP, definita nelle :term:`specifiche HTTP`.
Invence di inventare un altro metodo di cache, Symfony abbraccia lo standard
che definisce la comunicazione di base sul web. Una volta capiti i fondamenti
dei modelli di validazione e scadenza della cache HTTP, si sarà in grado di
padroneggiare il sistema di cache di Symfony.

Per poter imparare come funziona la cache in Symfony, procederemo in quattro
passi:

#. **Passo 1**: Un :ref:`gateway cache <gateway-caches>`, o reverse proxy, è un livello
   indipendente che si situa davanti all'applicazione. Il reverse proxy mette
   in cache le risposte non appena sono restituite dall'applicazione e risponde
   alle richieste con risposte in cache, prima che arrivino all'applicazione.
   Symfony fornisce il suo reverse proxy, ma se ne può usare uno qualsiasi.

#. **Passo 2**: Gli header di :ref:`cache HTTP <http-cache-introduction>` sono usati
   per comunicare col gateway cache e con ogni altra cache tra l'applicazione
   e il client. Symfony fornisce impostazioni predefinite appropriate e una potente
   interfaccia per interagire con gli header di cache.

#. **Passo 3**: La :ref:`scadenza e la validazione <http-expiration-validation>` HTTP sono
   due modelli usati per determinare se il contenuto in cache è *fresco* (può
   essere riusato dalla cache) o *vecchio* (andrebbe rigenerato
   dall'applicazione):

#. **Passo 4**: Gli :ref:`Edge Side Include <edge-side-includes>` (ESI) consentono alla
   cache HTTP di essere usata per mettere in cache frammenti di pagine (anche frammenti
   annidati) in modo indipendente. Con ESI, si può anche mettere in cache una pagina intera
   per 60 minuti, ma una barra laterale interna per soli 5 minuti.

Poiché la cache con HTTP non è esclusiva di Symfony, esistono già molti articoli a
riguardo. Se si è nuovi con la cache HTTP, raccomandiamo *caldamente* l'articolo
di Ryan Tomayko `Things Caches Do`_. Un'altra risorsa importante è il `Cache Tutorial`_
di Mark Nottingham.

.. index::
   single: Cache; Proxy
   single: Cache; Reverse proxy
   single: Cache; Gateway

.. _gateway-caches:

Cache con gateway cache
-----------------------

Quando si usa la cache con HTTP, la *cache* è completamente separata dall'applicazione
e si trova in mezzo tra applicazione e client che effettua la richiesta.

Il compito della cache è accettare le richieste dal client e passarle
all'applicazione. La cache riceverà anche risposte dall'applicazione e le girerà
al client. La cache è un "uomo in mezzo" nella comunicazione richiesta-risposta tra
il client e l'applicazione.

Lungo la via, la cache memorizzerà ogni risposta ritenuta "cacheable"
(vedere :ref:`http-cache-introduction`). Se la stessa risorsa viene richiesta nuovamente,
la cache invia la risposta in cache al client, ignorando completamente
l'applicazione.

Questo tipo di cache è nota come HTTP gateway cache e ne esistono diverse, come
`Varnish`_, `Squid in modalità reverse proxy`_ e il reverse proxy di Symfony.

.. index::
   single: Cache; Tipi

Tipi di cache
~~~~~~~~~~~~~

Ma il gateway cache non è l'unico tipo di cache. Infatti, gli header HTTP di cache
inviati dall'applicazione sono analizzati e interpretati da tre diversi
tipi di cache:

* *Cache del browser*: Ogni browser ha la sua cache locale, usata principalmente
  quando si clicca sul pulsante "indietro" per immagini e altre risorse.
  La cache del browser è una cache *privata*, perché le risorse in cache non sono
  condivise con nessun altro.

* *Proxy cache*: Un proxy è una cache *condivisa*, perché molte persone possono stare
  dietro a un singolo proxy. Solitamente si trova nelle grandi aziende e negli ISP, per
  ridurre la latenza e il traffico di rete.

* *Gateway cache*: Come il proxy, anche questa è una cache *condivisa*, ma dalla parte
  del server. Installata dai sistemisti di rete, rende i siti più scalabili, affidabili
  e performanti.

.. tip::

    Le gateway cache sono a volte chiamate reverse proxy cache,
    cache surrogate o anche acceleratori HTTP.

.. note::

    I significati di cache *privata* e *condivisa* saranno più chiari quando
    si parlerà di mettere in cache risposte che contengono contenuti specifici
    per un singolo utente (p.e. informazioni sull'account).

Ogni risposta dall'applicazione probabilmente attraverserà una o più
cache dei primi due tipi. Queste cache sono fuori dal nostro controllo, ma seguono
le indicazioni di cache HTTP impostate nella risposta.

.. index::
   single: Cache; Reverse proxy di Symfony

.. _`symfony-gateway-cache`:
.. _symfony2-reverse-proxy:

Il reverse proxy di Symfony 
~~~~~~~~~~~~~~~~~~~~~~~~~~~

Symfony ha un suo reverse proxy (detto anche gateway cache) scritto
in PHP. Abilitandolo, le risposte in cache dall'applicazione
inizieranno a essere messe in cache. L'installazione è altrettanto facile.
Ogni una applicazione Symfony ha la cache già configurata in ``AppCache``, che
estende ``AppKernel``. Il kernel della cache *è* il reverse
proxy.

Per abilitare la cache, modificare il codice di un front controller, per usare
il kernel della cache::

    // web/app.php
    require_once __DIR__.'/../app/bootstrap.php.cache';
    require_once __DIR__.'/../app/AppKernel.php';
    require_once __DIR__.'/../app/AppCache.php';

    use Symfony\Component\HttpFoundation\Request;

    $kernel = new AppKernel('prod', false);
    $kernel->loadClassCache();
    // inserisce AppKernel all'interno di AppCache
    $kernel = new AppCache($kernel);

    $request = Request::createFromGlobals();

    $response = $kernel->handle($request);
    $response->send();

    $kernel->terminate($request, $response);

Il kernel della cache agirà immediatamente da reverse proxy, mettendo in cache
le risposte dell'applicazione e restituendole al client.

.. caution::

    Se si usa l'opzione :ref:`framework.http_method_override <configuration-framework-http_method_override>`
    per leggere il metodo HTTP da un parametro ``_method``, vedere il collegamento precedente
    per un trucco da applicare.

.. tip::

    Il kernel della cache ha uno speciale metodo ``getLog()``, che restituisce una
    rappresentazione in stringa di ciò che avviene a livello di cache. Nell'ambiente
    di sviluppo, lo si può usare per il debug e la verifica della strategia di cache::

        error_log($kernel->getLog());

L'oggetto ``AppCache`` una una configurazione predefinita adeguata, ma può essere
regolato tramite un insieme di opzioni impostabili sovrascrivendo il
metodo
:method:`Symfony\\Bundle\\FrameworkBundle\\HttpCache\\HttpCache::getOptions`::

    // app/AppCache.php
    use Symfony\Bundle\FrameworkBundle\HttpCache\HttpCache;

    class AppCache extends HttpCache
    {
        protected function getOptions()
        {
            return array(
                'debug'                  => false,
                'default_ttl'            => 0,
                'private_headers'        => array('Authorization', 'Cookie'),
                'allow_reload'           => false,
                'allow_revalidate'       => false,
                'stale_while_revalidate' => 2,
                'stale_if_error'         => 60,
            );
        }
    }

.. tip::

    A meno che non sia sovrascritta in ``getOptions()``, l'opzione ``debug`` sarà
    impostata automaticamente al valore di debug di ``AppKernel`` circostante.

Ecco una lista delle opzioni principali:

``default_ttl``
    Il numero di secondi per cui un elemento in cache va considerato
    fresco, quando nessuna informazione esplicita sulla freschezza viene fornita in
    una risposta. Header espliciti ``Cache-Control`` o ``Expires`` sovrascrivono questo valore (predefinito: ``0``);

``private_headers``
    Insieme di header di richiesta che fanno scattare il comportamento
    "privato" ``Cache-Control`` sulle risposte che non stabiliscono esplicitamente il loro
    stato di ``public`` o ``private``, tramite una direttiva ``Cache-Control``.
    (predefinito: ``Authorization`` e ``Cookie``);

``allow_reload``
    Specifica se il client possa forzare un ricaricamento della cache
    includendo una direttiva ``Cache-Control`` "no-cache" nella richiesta. Impostare a
    ``true`` per aderire alla RFC 2616 (predefinito: ``false``);

``allow_revalidate``
    Specifica se il client possa forzare una rivalidazione della
    cache includendo una direttiva ``Cache-Control`` "max-age=0" nella richiesta. Impostare
    a ``true`` per aderire alla RFC 2616 (predefinito: false);

``stale_while_revalidate``
    Specifica il numero predefinito di secondi (la
    granularità è il secondo, perché la precisione del TTL della risposta è un secondo)
    durante il quale la cache può restituire immediatamente una risposta vecchia mentre
    si rivalida in background (predefinito: ``2``); questa impostazione è sovrascritta
    dall'estensione ``stale-while-revalidate`` ``Cache-Control`` di HTTP (vedere RFC 5861);

``stale_if_error``
    Specifica il numero predefinito di secondi (la granularità
    è il secondo) durante il quale la cache può servire una risposta vecchia quando si
    incontra un errore (predefinito: ``60``). Questa impostazione è sovrascritta
    dall'estensione ``stale-if-error`` ``Cache-Control`` di HTTP (vedere RFC 5861).

Se ``debug`` è ``true``, Symfony aggiunge automaticamente un header
``X-Symfony-Cache`` alla risposta, con dentro informazioni utili su hit e miss della
cache.

.. sidebar:: Cambiare da un reverse proxy a un altro

    Il reverse proxy di Symfony è un grande strumento da usare durante lo sviluppo
    di un sito oppure quando il deploy di un sito è su un host condiviso,
    dove non si può installare altro che codice PHP. Ma, essendo scritto in PHP, non può
    essere veloce quando un proxy scritto in C. Per questo si raccomanda caldamente di
    usare Varnish o Squid sul server di produzione, se possibile. La buona notizia
    è che il cambio da un proxy a un altro è facile e trasparente, non implicando alcuna
    modifica al codice dell'applicazione. Si può iniziare semplicemente con il
    reverse proxy di Symfony e aggiornare successivamente a Varnish, quando il traffico
    aumenta.

    Per maggiori informazioni sull'uso di Varnish con Symfony, vedere la ricetta
    :doc:`usare Varnish </cookbook/cache/varnish>`.

.. note::

    Le prestazioni del reverse proxy di Symfony non dipendono dalla complessità
    dell'applicazione. Questo perché il kernel dell'applicazione parte solo quando
    ha una richiesta a cui deve essere rigirato.

.. index::
   single: Cache; HTTP

.. _http-cache-introduction:

Introduzione alla cache HTTP
----------------------------

Per sfruttare i livelli di cache disponibili, un'applicazione deve poter
comunicare quale risposta può essere messa in cache e le regole che stabiliscono
quando e come tale cache debba essere considerata vecchia. Lo si può fare impostando
gli header di cache HTTP nella risposta.

.. tip::

    Si tenga a mente che "HTTP" non è altro che il linguaggio (un semplice linguaggio
    testuale) usato dai client web (p.e. i browser) e i server web per comunicare
    tra loro. La cache HTTP è la parte di tale linguaggio che consente a client
    e server di scambiarsi informazioni riguardo alla cache.

HTTP specifica quattro header di cache per la risposta di cui ci occupiamo:

* ``Cache-Control``
* ``Expires``
* ``ETag``
* ``Last-Modified``

L'header più importante e versatile è l'header ``Cache-Control``,
che in realtà è un insieme di varie informazioni sulla cache.

.. note::

    Ciascun header sarà spiegato in dettaglio nella sezione
    :ref:`http-expiration-validation`.

.. index::
   single: Cache; Header Cache-Control
   single: Header HTTP; Cache-Control

L'header Cache-Control
~~~~~~~~~~~~~~~~~~~~~~

L'header ``Cache-Control`` è unico, perché non contiene una, ma vari pezzi
di informazione sulla possibilità di una risposta di essere messa in cache.
Ogni pezzo di informazione è separato da una virgola:

.. code-block:: text

    Cache-Control: private, max-age=0, must-revalidate

    Cache-Control: max-age=3600, must-revalidate

Symfony fornisce un'astrazione sull'header ``Cache-Control``, per rendere la sua
creazione più gestibile::

    // ...

    use Symfony\Component\HttpFoundation\Response;

    $response = new Response();

    // segna la risposta come pubblica o privata
    $response->setPublic();
    $response->setPrivate();

    // imposta max age privata o condivisa
    $response->setMaxAge(600);
    $response->setSharedMaxAge(600);

    // imposta una direttiva personalizzata Cache-Control
    $response->headers->addCacheControlDirective('must-revalidate', true);

.. tip::

    Se occorre impostare header di cache per molte azioni diverse,
    si potrebbe usare FOSHttpCacheBundle_. Questo bundle fornisce un modo
    per definire header di cache in base allo schema dell'URL e ad altre proprietà
    della richiesta.

Risposte pubbliche e risposte private
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Sia la gateway cache che la proxy cache sono considerate cache "condivise", perché
il contenuto della cache è condiviso da più di un utente. Se una risposta specifica per un
utente venisse per errore inserita in una cache condivisa, potrebbe successivamente essere
restituita a diversi altri utenti. Si immagini se delle informazioni su un account
venissero messe in cache e poi restituite a ogni utente successivo che richiede la sua pagina dell'account!

Per gestire questa situazione, ogni risposta può essere impostata a pubblica o privata:

*pubblica*
    Indica che la risposta può essere messa in cache sia da che private che da cache condivise;

*privata*
    Indica che tutta la risposta, o una sua parte, è per un singolo utente
    e quindi non deve essere messa in una cache condivisa.

Symfony è conservativo e ha come predefinita una risposta privata. Per sfruttare le
cache condivise (come il reverse proxy di Symfony), la risposta deve essere
impostata esplicitamente come pubblica.

.. index::
   single: Cache; Metodi sicuri

Metodi sicuri
~~~~~~~~~~~~~

La cache HTTP funziona solo per metodi HTTP "sicuri" (come GET e HEAD). Essere
sicuri vuol dire che lo stato dell'applicazione sul server non cambia mai quando
si serve la richiesta (si può, certamente, memorizzare un'informazione sui log, mettere
in cache dati, eccetera). Questo ha due conseguenze molto ragionevoli:

* Non si dovrebbe *mai* cambiare lo stato dell'applicazione quando si risponde
  a una richiesta GET o HEAD. Anche se non si usa una gateway cache, la presenza di
  proxy cache vuol dire che ogni richiesta GET o HEAD potrebbe arrivare al server,
  ma potrebbe anche non arrivare.

* Non aspettarsi la cache dei metodi PUT, POST o DELETE. Questi metodi sono fatti per
  essere usati quando si cambia lo stato dell'applicazione (p.e. si cancella un
  post di un blog). Metterli in cache impedirebbe ad alcune richieste di arrivare
  all'applicazione o di modificarla.

.. _http-cache-defaults:

Regole e valori predefiniti della cache
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

HTTP 1.1 consente per impostazione predefinita la cache di tutto, a meno che non ci sia un
header esplicito ``Cache-Control``. In pratica, la maggior parte delle cache non fanno
nulla quando la richiesta ha un cookie, un header di autorizzazione, usa un metodo non
sicuro (PUT, POST, DELETE) o quando la risposta ha un codice di stato di rinvio.

Symfony imposta automaticamente un header ``Cache-Control`` conservativo, quando
nessun header è impostato dallo sviluppatore, seguendo queste regole:

* Se non è definito nessun header di cache (``Cache-Control``, ``Expires``, ``ETag``
  o ``Last-Modified``), ``Cache-Control`` è impostato a ``no-cache``, il che vuol dire
  che la risposta non sarà messa in cache;

* Se ``Cache-Control`` è vuoto (ma uno degli altri header di cache è presente),
  il suo valore è impostato a ``private, must-revalidate``;

* Se invece almeno una direttiva ``Cache-Control`` è impostata e nessuna direttiva
  ``public`` o ``private`` è stata aggiunta esplicitamente, Symfony aggiunge
  automaticamente la direttiva ``private`` (tranne quando è impostato ``s-maxage``).

.. _http-expiration-validation:
.. _http-expiration-and-validation:

Scadenza e validazione HTTP
---------------------------

Le specifiche HTTP definiscono due modelli di cache:

* Con il `modello a scadenza`_, si specifica semplicemente quanto a lungo una risposta
  debba essere considerata "fresca", includendo un header ``Cache-Control`` e/o uno
  ``Expires``. Le cache che capiscono la scadenza non faranno di nuovo la stessa richiesta
  finché la versione in cache non raggiunge la sua scadenza e diventa "vecchia".

* Quando le pagine sono molto dinamiche (cioè quando la loro rappresentazione varia spesso),
  il `modello a validazione`_ è spesso necessario. Con questo modello, la cache memorizza
  la risposta, ma chiede al serve a ogni richiesta se la risposta in cache sia ancora
  valida o meno. L'applicazione usa un identificatore univoco per la risposta (l'header
  ``Etag``) e/o un timestamp (come l'header ``Last-Modified``) per verificare se la
  pagina sia cambiata da quanto è stata messa in cache.

Lo scopo di entrambi i modelli è quello di non generare mai la stessa risposta due volte,
appoggiandosi a una cache per memorizzare e restituire risposte "fresche". Per ottenere tempi di cache lunghi,
ma fornire comunque contenuti aggiornati immediatamente, a volte si usa
l'*invalidazione della cache*.

.. sidebar:: Leggere le specifiche HTTP

    Le specifiche HTTP definiscono un linguaggio semplice, ma potente, in cui client e
    server possono comunicare. Come sviluppatori web, il modello richiesta-risposta
    delle specifiche domina il nostro lavoro. Sfortunatamente, il documento delle
    specifiche, la `RFC 2616`_, può risultare di difficile lettura.

    C'è uno sforzo in atto (`HTTP Bis`_) per riscrivere la RFC 2616. Non descrive
    una nuova versione di HTTP, ma per lo più chiarisce le specifiche HTTP
    originali. Anche l'organizzazione è migliore, essendo le specifiche separate in
    sette parti; tutto ciò che riguarda la cache HTTP si trova in due parti
    dedicate (`P4 - Richieste condizionali`_ e `P6 - Cache: Browser
    e cache intermedie`_).

    Come sviluppatori web, dovremmo leggere tutti le specifiche. Possiedono un chiarezza e
    una potenza, anche dopo oltre dieci anni dalla creazione, inestimabili.
    Non ci si spaventi dalle apparenze delle specifiche, il contenuto è molto
    più bello della copertina.

.. index::
   single: Cache; Scadenza HTTP

Scadenza
~~~~~~~~

Il modello a scadenza è il più efficiente e il più chiaro dei due modelli di cache
e andrebbe usato ogni volta che è possibile. Quando una risposta è messa in cache con
una scadenza, la cache memorizzerà la risposta e la restituirà direttamente,
senza arrivare all'applicazione, finché non scade.

Il modello a scadenza può essere implementato con l'uso di due header HTTP, quasi
identici: ``Expires`` o ``Cache-Control``.

.. index::
   single: Cache; Header Expires
   single: Header HTTP; Expires

Scadenza con l'header ``Expires``
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Secondo le specifiche HTTP, "l'header ``Expires`` dà la data e l'ora dopo la quale
la risposta è considerata vecchia". L'header ``Expires`` può essere impostato
con il metodo ``setExpires()`` di ``Response``. Accetta un'istanza di ``DateTime``
come parametro::

    $date = new DateTime();
    $date->modify('+600 seconds');

    $response->setExpires($date);

Il risultante header HTTP sarà simile a questo:

.. code-block:: text

    Expires: Thu, 01 Mar 2011 16:00:00 GMT

.. note::

    Il metodo ``setExpires()`` converte automaticamente la data al fuso orario GMT,
    come richiesto dalle specifiche.

Si noti che, nelle versioni di HTTP precedenti alla 1.1, non era richiesto al server di origine di inviare
l'header ``Date``. Di conseguenza, la cache (p.e. il browser) potrebbe aver bisogno di
appoggiarsi all'orologio locale per valutare l'header ``Expires``, rendendo il calcolo del ciclo di vita
vulnerabile a difformità di ore. L'header ``Expires`` soffre di un'altra limitazione: le
specifiche stabiliscono che "i server HTTP/1.1 non dovrebbero inviare header ``Expires``
oltre un anno nel futuro."

.. index::
   single: Cache; Header Cache-Control
   single: Header HTTP; Cache-Control

Scadenza con l'header ``Cache-Control``
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

A causa dei limiti dell'header ``Expires``, la maggior parte delle volte si userà
al suo posto l'header ``Cache-Control``. Si ricordi che l'header ``Cache-Control``
è usato per specificare molte differenti direttive di cache. Per la scadenza, ci
sono due direttive, ``max-age`` e ``s-maxage``.  La prima è usata da tutte le
cache, mentre la seconda viene considerata solo dalla cache
condivise::

    // Imposta il numero di secondi dopo cui la risposta
    // non dovrebbe più essere considerata fresca
    $response->setMaxAge(600);

    // Come sopra, ma solo per cache condivise
    $response->setSharedMaxAge(600);

L'header ``Cache-Control`` avrebbe il seguente formato (potrebbe contenere
direttive aggiuntive):

.. code-block:: text

    Cache-Control: max-age=600, s-maxage=600

.. index::
   single: Cache; Validazione

Validazione
~~~~~~~~~~~

Quando una risorsa ha bisogno di essere aggiornata non appena i dati sottostanti
subiscono una modifica, il modello a scadenza non raggiunge lo scopo. Con il modello
a scadenza, all'applicazione non sarà chiesto di restituire la risposta aggiornata,
finché la cache non diventa vecchia.

Il modello a validazione si occupa di questo problema. Con questo modello, la cache
continua a memorizzare risposte. La differenza è che, per ogni richiesta, la cache
chiede all'applicazione se la risposta in cache è ancora valida. Se la cache *è*
ancora valida, l'applicazione dovrebbe restituire un codice di stato 304 e
nessun contenuto. Questo dice alla cache che è va bene restituire la risposta in cache.

Con questo modello, si risparmiare solo CPU, se si è in grado di determinare che la
risposta in cache sia ancora  valida, facendo *meno* lavoro rispetto alla generazione
dell'intera pagina (vedere sotto per un esempio di implementazione).

.. tip::

    Il codice di stato 304 significa "non modificato". È importante, perché questo
    codice di stato *non* contiene il vero contenuto richiesto.
    La risposta è invece un semplice e leggero insieme di istruzioni che dicono alla
    cache che dovrebbe usare la sua versione memorizzata.

Come per la scadenza, ci sono due diversi header HTTP che possono essere usati per
implementare il modello a validazione: ``ETag`` e ``Last-Modified``.

.. index::
   single: Cache; Header Etag
   single: Header HTTP; Etag

Validazione con header ``ETag``
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

L'header ``ETag`` è un header stringa (chiamato "tag entità") che identifica
univocamente una rappresentazione della risorsa in questione. È interamente
generato e impostato dall'applicazione, quindi si può dire, per esempio, se
la risorsa ``/about`` che è in cache sia aggiornata con ciò che
l'applicazione restituirebbe. Un ``ETag`` è come un'impronta digitale ed è usato per
confrontare rapidamente se due diverse versioni di una risorsa siano equivalenti. Come le
impronte digitali, ogni ``ETag`` deve essere univoco tra tutte le rappresentazioni della stessa risorsa.

Ecco una semplice implementazione, che genera l'ETag come un md5 del contenuto::

    // src/AppBundle/Controller/DefaultController.php
    namespace AppBundle\Controller;

    use Symfony\Component\HttpFoundation\Request;

    class DefaultController extends Controller
    {
        public function homepageAction(Request $request)
        {
            $response = $this->render('static/homepage.html.twig');
            $response->setETag(md5($response->getContent()));
            $response->setPublic(); // assicurarsi che la risposta sia pubblica
            $response->isNotModified($request);

            return $response;
        }
    }

Il metodo :method:`Symfony\\Component\\HttpFoundation\\Response::isNotModified`
confronta l'``ETag`` inviato con la ``Request`` con quello impostato nella
``Response``. Se i due combaciano, il metodo imposta automaticamente il codice
di stato della ``Response`` a 304.

.. note::

    L'header ``If-None-Match`` della richiesta corrisponde all'header ``ETag``
    dell'ultima risposta inviata al client per una particolare risorsa. In questo modo
    il client e il server comunicano a vicenda e decidono se la
    risorsa sia stata aggiornata o meno, rispetto a quando è stata messa in cache.

Questo algoritmo è abbastanza semplice e molto generico, ma occorre creare
l'intera ``Response`` prima di poter calcolare l'ETag, che non è ottimale.
In altre parole, fa risparmiare banda, ma non cicli di CPU.

Nella sezione :ref:`optimizing-cache-validation`, mostreremo come si possa usare la
validazione in modo più intelligente, per determinare la validità di una cache senza
dover fare tanto lavoro.

.. tip::

    Symfony supporta anche gli ETag deboli, passando ``true`` come secondo
    parametro del metodo
    :method:`Symfony\\Component\\HttpFoundation\\Response::setETag`.

.. index::
   single: Cache; Header Last-Modified
   single: Header HTTP; Last-Modified

Validazione col metodo ``Last-Modified``
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

L'header ``Last-Modified`` è la seconda forma di validazione. Secondo le specifiche
HTTP, "l'header ``Last-Modified`` indica la data e l'ora in cui il server di origine
crede che la rappresentazione sia stata modificata l'ultima volta". In altre parole,
l'applicazione decide se il contenuto in cache sia stato modificato o meno, in base
al fatto se sia stato aggiornato o meno da quando la risposta è stata messa in
cache.

Per esempio, si può usare la data di ultimo aggiornamento per tutti gli oggetti
necessari per calcolare la rappresentazione della risorsa come valore dell'header
``Last-Modified``::

    // src/AppBundle/Controller/ArticleController.php
    namespace AppBundle\Controller;

    // ...
    use Symfony\Component\HttpFoundation\Request;
    use AppBundle\Entity\Article;

    class ArticleController extends Controller
    {
        public function showAction(Article $article, Request $request)
        {
            $author = $article->getAuthor();

            $articleDate = new \DateTime($article->getUpdatedAt());
            $authorDate = new \DateTime($author->getUpdatedAt());

            $date = $authorDate > $articleDate ? $authorDate : $articleDate;

            $response->setLastModified($date);
            // imposta la risposta come pubblica. Altrimenti, è privata come valore predefinito.
            $response->setPublic();

            if ($response->isNotModified($request)) {
                return $response;
            }

            // ... fare qualcosa per popolare la risposta con il contenuto completo

            return $response;
        }
    }

Il metodo method:`Symfony\\Component\\HttpFoundation\\Response::isNotModified`
confronta l'header ``If-Modified-Since`` inviato dalla richiesta con
l'header ``Last-Modified`` impostato nella risposta. Se sono equivalenti,
la ``Response`` sarà impostata a un codice di stato 304.

.. note::

    L'header della richiesta ``If-Modified-Since`` equivale all'header ``Last-Modified``
    dell'ultima risposta inviata al client per una determinata risorsa.
    In questo modo client e server comunicano l'uno con l'altro e decidono se
    la risorsa sia stata aggiornata o meno da quando è stata messa in cache.

.. index::
   single: Cache; Get condizionale
   single: HTTP; 304

.. _optimizing-cache-validation:

Ottimizzare il codice con la validazione
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Lo scopo principale di ogni strategia di cache è alleggerire il carico dell'applicazione.
In altre parole, meno un'applicazione fa per restituire una risposta 304,
meglio è. Il metodo ``Response::isNotModified()`` fa esattamente questo, esponendo
uno schema semplice ed efficiente::

    // src/AppBundle/Controller/ArticleController.php
    namespace AppBundle\Controller;

    // ...
    use Symfony\Component\HttpFoundation\Response;
    use Symfony\Component\HttpFoundation\Request;

    class ArticleController extends Controller
    {
        public function showAction($articleSlug, Request $request)
        {
            // Prende l'informazione minima per calcolare
            // l'ETag o o il valore di Last-Modified
            // (in base alla Request, i dati sono recuperati da una
            // base dati o da una memoria chiave-valore, per esempio)
            $article = ...;

            // crea una Response con un ETag e/o un header Last-Modified
            $response = new Response();
            $response->setETag($article->computeETag());
            $response->setLastModified($article->getPublishedAt());

            // imposta la risposta come pubblica. Altrimenti, è privata come valore predefinito.
            $response->setPublic();

            // Verifica che la Response non sia modificata per la Request data
            if ($response->isNotModified($request)) {
                // restituisce subito la Response 304
                return $response;
            }

            // qui fare qualcosa, come recuperare altri dati
            $comments = ...;

            // o rendere un template con la $response già iniziata
            return $this->render('article/show.html.twig', array(
                'article' => $article,
                'comments' => $comments
            ), $response);
        }
    }

Quando la ``Response`` non è stata modificata, ``isNotModified()`` imposta automaticamente
il codice di stato della risposta a ``304``, rimuove il contenuto e rimuove alcuni header
che non devono essere presenti in una risposta ``304`` (vedere
:method:`Symfony\\Component\\HttpFoundation\\Response::setNotModified`).

.. index::
   single: Cache; Vary
   single: Header HTTP; Vary

Variare la risposta
~~~~~~~~~~~~~~~~~~~

Finora abbiamo ipotizzato che ogni URI avesse esattamente una singola rappresentazione
della risorsa interessata. Per impostazione predefinita, la cache HTTP usa l'URI della
risorsa come chiave. Se due persone richiedono lo stesso URI di una risorsa che si può
mettere in cache, la seconda persona riceverà la versione in cache.

A volte questo non basta e diverse versioni dello stesso URI hanno bisogno di stare in
cache in base a uno più header di richiesta. Per esempio, se si comprimono le pagine
per i client che supportano per la compressione, ogni URI ha due rappresentazioni:
una per i client col supporto e l'altra per i client senza supporto. Questo viene
determinato dal valore dell'header di richiesta ``Accept-Encoding``.

In questo caso, occorre mettere in cache sia una versione compressa che una non compressa
della risposta di un particolare URI e restituirle in base al valore ``Accept-Encoding``
della richiesta. Lo si può fare usando l'header di risposta ``Vary``, che è una lista
separata da virgole dei diversi header i cui valori causano rappresentazioni diverse
della risorsa richiesta:

.. code-block:: text

    Vary: Accept-Encoding, User-Agent

.. tip::

    Questo particolare header ``Vary`` fa mettere in cache versioni diverse di ogni
    risorsa in base all'URI, al valore di ``Accept-Encoding`` e all'header di richiesta
    ``User-Agent``.

L'oggetto ``Response`` offre un'interfaccia pulita per la gestione dell'header
``Vary``::

    // imposta un header Vary
    $response->setVary('Accept-Encoding');

    // imposta diversi header Vary
    $response->setVary(array('Accept-Encoding', 'User-Agent'));

Il metodo ``setVary()`` accetta un nome di header o un array di nomi di header per i
quali la risposta varia.

Scadenza e validazione
~~~~~~~~~~~~~~~~~~~~~~

Si può ovviamente usare sia la validazione che la scadenza nella stessa ``Response``.
Poiché la scadenza vince sulla validazione, si può beneficiare dei vantaggi di
entrambe. In altre parole, usando sia la scadenza che la validazione, si può
istruire la cache per servire il contenuto in cache, controllando ogni tanto
(la scadenza) per verificare che il contenuto sia ancora valido.

.. tip::

    Si possono anche definire header HTTP per la scadenza e la validazione della cache usando le
    annotazioni. Vedere la `documentazione di FrameworkExtraBundle`_.

.. index::
    pair: Cache; Configurazione

Altri metodi della risposta
~~~~~~~~~~~~~~~~~~~~~~~~~~~

La classe ``Response`` fornisce molti altri metodi per la cache. Ecco alcuni dei più
utili::

    // Segna la risposta come vecchia
    $response->expire();

    // Forza la risposta a restituire un 304 senza contenuti
    $response->setNotModified();

Inoltre, la maggior parte degli header HTTP relativi alla cache può essere impostata tramite il singolo
metodo :method:`Symfony\\Component\\HttpFoundation\\Response::setCache`::

    // Imposta le opzioni della cache in una sola chiamata
    $response->setCache(array(
        'etag'          => $etag,
        'last_modified' => $date,
        'max_age'       => 10,
        's_maxage'      => 10,
        'public'        => true,
        // 'private'    => true,
    ));

.. index::
   single: Cache; Invalidazione

.. _http-cache-invalidation:

Invalidazione della cache
-------------------------

    "Ci sono solo due cose difficili in informatica: invalidazione della cache e
    nomi delle cose." -- Phil Karlton

Una volta che un URL è memorizzato in una gateway cache, la cache non chiederà più
tale contenuto all'applicazione. Ciò consente alla cache di fornire risposte veloci
e ridurre il carico sull'applicazione. Tuttavia, si rischia di
fornire contenuti obsoleti. Un modo per uscire da questo dilemma è usare tempi di cache
lunghi, notificando alla gateway cache quando il contenuto cambia.
I reverse proxy solitamente forniscono un canale per ricevere tali notifiche,
tipicamente tramite speciali richieste HTTP.

.. caution::

    L'invalidazione della cache è potente, ma va evitata quando possibile. Se non si riesce
    a invalidare qualcosa, le cache obsolete saranno servite per un tempo potenzialmente
    molto lungo. Invece, usare tempi di cache brevi o usare il modello a validazione
    e adattare i controllori per eseguire validazioni efficienti, come
    spiegato in :ref:`optimizing-cache-validation`.

    Inoltre, essendo l'invalidazione un argomento specifico di ciascun tipo di reverse
    proxy, il suo utilizzo lega a un reverse proxy specifico oppure richiede
    sforzi aggiuntivi per supportare proxy diversi.

A volte, tuttavia, si ha bisogno di quelle prestazioni in più che si possono ottenere
invalidando esplicitamente. Per l'invalidazione, l'applicazione ha bisogno di individuare
quando il contenuto cambia e riferire alla cache di rimuovere gli URL che contengono
tali dati in cache.

.. tip::

    Se si vuole usare l'invalidazione della cache, si potrebbe usare
    `FOSHttpCacheBundle`_. Questo bundle fornisce servizi per l'aiuto in vari
    concetti di invalidazione della cache e documenta la configurazione per un
    paio di proxy comunemente usati.

Se un contenuto corrisponde a un URL, il modello ``PURGE`` funziona molto bene.
Si manda una richiesta al proxy con il metodo HTTP ``PURGE`` (l'uso
del verbo "PURGE" è una convenzione, tecnicamente si può usare una stringa qualsiasi) al
posto di ``GET`` e fare in modo che il proxy lo individui e rimuova i dati dall
cache, invece di passare a Symfony per avere una risposta.

Ecco come si può configurare il reverse proxy di Symfony per supportare il
metodo HTTP ``PURGE``::

    // app/AppCache.php

    use Symfony\Bundle\FrameworkBundle\HttpCache\HttpCache;
    use Symfony\Component\HttpFoundation\Request;
    use Symfony\Component\HttpFoundation\Response;
    // ...

    class AppCache extends HttpCache
    {
        protected function invalidate(Request $request, $catch = false)
        {
            if ('PURGE' !== $request->getMethod()) {
                return parent::invalidate($request, $catch);
            }

            if ('127.0.0.1' !== $request->getClientIp()) {
                return new Response(
                    'Invalid HTTP method',
                    Response::HTTP_BAD_REQUEST
                );
            }

            $response = new Response();
            if ($this->getStore()->purge($request->getUri())) {
                $response->setStatusCode(200, 'Purged');
            } else {
                $response->setStatusCode(200, 'Not found');
            }

            return $response;
        }
    }

.. caution::

    Occorre proteggere in qualche modo il metodo HTTP ``PURGE``, per evitare che qualcuno
    pulisca casualmente i dati in cache.

**Purge** dice alla cache di eliminare una risorsa in *tutte le sue varianti*
(in accordo con l'header ``Vary``, vedere sopra). Un'alternativa a Purge è il
**refresh**. Refresh vuol dire che al proxy viene chiesto
di scartare la sua cache locale e recuperare nuovamente il contenuto. In questo modo,
il nuovo contenuto è già disponibile nella cache. Il difetto del refresh
è che le varianti non vengono invalidate.

In molte applicazioni, lo stesso bit di contenuto è usato su varie pagine con
URL diversi. In questo caso, esistono concetti più flessibili:

* Il *ban** invalida risposte che corrispondono a espressioni
  sull'URL o ad altri criteri;
* Il **tag** consente di marcare ciascun contenuto usato in una risposta,
  in modo da poter invalidare tutti gli URL che includono un determinato contenuto.

.. index::
   single: Cache; ESI
   single: ESI

.. _edge-side-includes:

Usare Edge Side Include
-----------------------

Le gateway cache sono un grande modo per rendere un sito più prestante. Ma hanno
una limitazione: possono mettere in cache solo pagine intere. Se non si possono mettere in
cache pagine intere o se le pagine hanno più parti dinamiche, non vanno bene.
Fortunatamente, Symfony fornisce una soluzione a questi casi, basata su una
tecnologia chiamata `ESI`_, o Edge Side Includes. Akamaï ha scritto le specifiche quasi
dieci anni fa, consentendo a determinate parti di una pagina di avere differenti
strategie di cache rispetto alla pagina principale.

Le specifiche ESI descrivono dei tag che si possono inserire nelle proprie pagine, per
comunicare col gateway cache. L'unico tag implementato in Symfony è ``include``,
poiché è l'unico utile nel contesto di Akamaï:

.. code-block:: html

    <!DOCTYPE html>
    <html>
        <body>
            <!-- ... del contenuto -->

            <!-- Inserisce qui il contenuto di un'altra pagina -->
            <esi:include src="http://..." />

            <!-- ... dell'altro contenuto -->
        </body>
    </html>

.. note::

    Si noti nell'esempio che ogni tag ESI ha un URL pienamente qualificato. Un tag
    ESI rappresenta un frammento di pagina che può essere recuperato tramite
    l'URL fornito.

Quando gestisce una richiesta, il gateway cache recupera l'intera pagina dalla sua cache
oppure la richiede dall'applicazione di backend. Se la risposta contiene uno o più
tag ESI, questi vengono processati nello stesso modo. In altre parole, la
gateway cache o recupera il frammento della pagina inclusa dalla sua cache oppure
richiede il frammento di pagina all'applicazione di backend. Quando tutti i tag ESI sono
stati risolti, il gateway cache li fonde nella pagina principale e invia il contenuto
finale al client.

Tutto questo avviene in modo trasparente a livello di gateway cache (quindi fuori
dall'applicazione). Come vedremo, se si scegli di avvalersi dei tag ESI,
Symfony rende quasi senza sforzo il processo di inclusione.

.. _using-esi-in-symfony2:

Usare ESI in Symfony
~~~~~~~~~~~~~~~~~~~~

Per usare ESI, assicurarsi prima di tutto di abilitarlo nella configurazione dell'applicazione:

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        framework:
            # ...
            esi: { enabled: true }

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <?xml version="1.0" encoding="UTF-8" ?>
        <container xmlns="http://symfony.com/schema/dic/symfony"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xmlns:framework="http://symfony.com/schema/dic/symfony"
            xsi:schemaLocation="http://symfony.com/schema/dic/services
                http://symfony.com/schema/dic/services/services-1.0.xsd
                http://symfony.com/schema/dic/symfony
                http://symfony.com/schema/dic/symfony/symfony-1.0.xsd">

            <framework:config>
                <!-- ... -->
                <framework:esi enabled="true" />
            </framework:config>
        </container>

    .. code-block:: php

        // app/config/config.php
        $container->loadFromExtension('framework', array(
            // ...
            'esi' => array('enabled' => true),
        ));

Supponiamo ora di avere una pagina relativamente statica, tranne per un elenco di
news in fondo al contenuto. Con ESI, si può mettere in cache l'elenco di news
indipendentemente dal resto della pagina.

.. code-block:: php

    // src/AppBundle/Controller/DefaultController.php

    // ...
    class DefaultController extends Controller
    {
        public function aboutAction()
        {
            $response = $this->render('static/about.html.twig');
            // imposta il tempo massimo condiviso, il che rende la risposta pubblica
            $response->setSharedMaxAge(600);

            return $response;
        }
    }

In questo esempio, abbiamo dato alla cache della pagina intera un tempo di vita di dieci
minuti. Successivamente, includiamo l'elenco di news nel template, includendolo in
un'azione. Possiamo farlo grazie all'aiutante ``render`` (vedere
:ref:`templating-embedding-controller` per maggiori dettagli).

Poiché il contenuto incluso proviene da un'altra pagina (o da un altro controllore),
Symfony usa l'aiutante ``render`` per configurare i tag ESI:

.. configuration-block::

    .. code-block:: jinja

        {# app/Resources/views/static/about.html.twig #}

        {# si può fare riferimento a un controllore #}
        {{ render_esi(controller('AppBundle:News:latest', { 'maxPerPage': 5 })) }}

        {# ... o a un URL #}
        {{ render_esi(url('latest_news', { 'maxPerPage': 5 })) }}

    .. code-block:: html+php

        <!-- app/Resources/views/static/about.html.php -->

        // si può fare riferimento a un controllore
        use Symfony\Component\HttpKernel\Controller\ControllerReference;
        <?php echo $view['actions']->render(
            new ControllerReference(
                'AppBundle:News:latest',
                array('maxPerPage' => 5)
            ),
            array('strategy' => 'esi')
        ) ?>

        // ... o a un URL
        use Symfony\Component\Routing\Generator\UrlGeneratorInterface;
        <?php echo $view['actions']->render(
            $view['router']->generate(
                'latest_news',
                array('maxPerPage' => 5),
                UrlGeneratorInterface::ABSOLUTE_URL
            ),
            array('strategy' => 'esi'),
        ) ?>

Usando l'opzione ``esi`` (che usa a sua volta la funzione Twig ``render_esi``), si dice
a Symfony che l'azione va resa come tag ESI. Ci si potrebbe chiedere
perché voler usare un aiutante invece di scrivere direttamente il tag ESI.
Il motivo è che un aiutante fa funzionare l'applicazione anche
se non ci sono gateway per la cache installati.

.. tip::

    Come si vedrà più avanti, la variabile passata ``maxPerPage`` è disponibile
    come parametro del controllore (come ``$maxPerPage``). Le variabili
    passate tramite ``render_esi`` diventano ugualmente parte della chiave di cache, in modo
    da avere cache uniche per ogni combinazione di variabili e valori.

Quando si usa la funzione ``render`` predefinita (o si usa l'opzione
``inline``), Symfony fonde il contenuto della pagina inclusa in quello principale,
prima di inviare la risposta al client. Se invece si usa l'opzione ``esi``
(che richiama ``render_esi``) *e* se Symfony capisce che sta parlando a un
gateway per la cache che supporti ESI, genera un tag ESI. Ma se non c'è
alcun gateway per la cache o se ce n'è uno che non supporta ESI, Symfony fonderà
il contenuto della pagina inclusa in quello principale, come se fosse state usata
``render``.

.. note::

    Symfony individua se una gateway cache supporta ESI tramite un'altra
    specifica di Akamaï, che è supportata nativamente dal reverse proxy di
    Symfony.

L'azione inclusa ora può specificare le  sue regole di cache, indipendentemente
dalla pagina principale.

.. code-block:: php

    // src/AppBundle/Controller/NewsController.php
    namespace AppBundle\Controller;

    // ...
    class NewsController extends Controller
    {
        public function latestAction($maxPerPage)
        {
            // ...
            $response->setSharedMaxAge(60);

            return $response;
        }
    }

Con ESI, la cache dell'intera pagina sarà valida per 600 secondi, mentre il
componente delle news avrà una cache che dura per soli 60 secondi.

.. _book-http_cache-fragments:

Quando si fa riferimento a un controllore, il tag ESI dovrebbe far riferimento all'azione
inclusa con un URL accessibile, in modo che il gateway della cache possa recuperarla indipendentemente
dal resto della pagina. Symfony si occupa di generare un URL univoco per ogni
riferimento a controllori ed è in grado di puntare correttamente le rotte, grazie all'ascoltatore
:class:`Symfony\\Component\\HttpKernel\\EventListener\\FragmentListener`,
che va abilitato nella configurazione:

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        framework:
            # ...
            fragments: { path: /_fragment }

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <?xml version="1.0" encoding="UTF-8" ?>
        <container xmlns="http://symfony.com/schema/dic/services"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xmlns:doctrine="http://symfony.com/schema/dic/framework"
            xsi:schemaLocation="http://symfony.com/schema/dic/services
                http://symfony.com/schema/dic/services/services-1.0.xsd
                http://symfony.com/schema/dic/symfony
                http://symfony.com/schema/dic/symfony/symfony-1.0.xsd">

            <!-- ... -->
            <framework:config>
                <framework:fragments path="/_fragment" />
            </framework:config>
        </container>

    .. code-block:: php

        // app/config/config.php
        $container->loadFromExtension('framework', array(
            // ...
            'fragments' => array('path' => '/_fragment'),
        ));

Un grosso vantaggio di questa strategia di cache è che si può rendere
l'applicazione tanto dinamica quanto necessario e, allo stesso tempo, mantenere gli
accessi al minimo.

.. tip::

    L'ascoltatore risponde solo agli indirizzi IP locali o ai
    :doc:`proxy fidati </cookbook/request/load_balancer_reverse_proxy>`.

.. note::

    Una volta iniziato a usare ESI, si ricordi di usare sempre la direttiva ``s-maxage``
    al posto di ``max-age``. Poiché il browser riceve la risorsa
    aggregata, non ha visibilità sui sotto-componenti, quindi obbedirà alla
    direttiva ``max-age`` e metterà in cache l'intera pagina. E questo non è quello che
    vogliamo.

L'aiutante ``render_esi`` supporta due utili opzioni:

``alt``
    usato come attributo ``alt`` nel tag ESI, che consente di specificare
    un URL alternativo da usare, nel caso in cui ``src`` non venga trovato;

``ignore_errors``
    se impostato a ``true``, un attributo ``onerror`` sarà aggiunto a
    ESI con il valore di ``continue``, a indicare che, in caso di fallimento, la
    gateway cache semplicemente rimuoverà il tag ESI senza produrre errori.

Riepilogo
---------

Symfony è stato progettato per seguire le regole sperimentate della strada: HTTP.
La cache non fa eccezione. Padroneggiare il sistema della cache di Symfony vuol dire
acquisire familiarità con i modelli di cache HTTP e usarli in modo efficace. Vuol dire
anche che, invece di basarsi solo su documentazione ed esempi di Symfony, si ha accesso
al mondo della conoscenza relativo alla cache HTTP e a gateway cache come
Varnish.

Imparare di più con le ricette
------------------------------

* :doc:`/cookbook/cache/varnish`

.. _`Things Caches Do`: http://tomayko.com/writings/things-caches-do
.. _`Cache Tutorial`: http://www.mnot.net/cache_docs/
.. _`Varnish`: https://www.varnish-cache.org/
.. _`Squid in modalità reverse proxy`: http://wiki.squid-cache.org/SquidFaq/ReverseProxy
.. _`modello a scadenza`: http://tools.ietf.org/html/rfc2616#section-13.2
.. _`modello a validazione`: http://tools.ietf.org/html/rfc2616#section-13.3
.. _`RFC 2616`: http://tools.ietf.org/html/rfc2616
.. _`HTTP Bis`: http://tools.ietf.org/wg/httpbis/
.. _`P4 - Richieste condizionali`: http://tools.ietf.org/html/draft-ietf-httpbis-p4-conditional
.. _`P6 - Cache: Browser e cache intermedie`: http://tools.ietf.org/html/draft-ietf-httpbis-p6-cache
.. _`documentazione di FrameworkExtraBundle`: http://symfony.com/doc/current/bundles/SensioFrameworkExtraBundle/annotations/cache.html
.. _`ESI`: http://www.w3.org/TR/esi-lang
.. _`FOSHttpCacheBundle`: http://foshttpcachebundle.readthedocs.org/
