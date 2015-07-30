.. index::
   single: Fondamenti di Symfony

.. _symfony2-and-http-fundamentals:

Symfony e fondamenti di HTTP
============================

Congratulazioni! Imparando Symfony, si tende a essere sviluppatori web più
*produttivi*, *versatili* e *popolari* (in realtà, per quest'ultimo dovete
sbrigarvela da soli). Symfony è costruito per tornare alle basi: per sviluppare
strumenti che consentono di sviluppare più velocemente e costruire applicazioni
più robuste, anche andando fuori strada. Symfony è costruito sulle migliori idee
prese da diverse tecnologie: gli strumenti e i concetti che si stanno per apprendere
rappresentano lo sforzo di centinaia di persone, in molti anni. In altre parole,
non si sta semplicemente imparando "Symfony", si stanno imparando i fondamenti del web,
le pratiche migliori per lo sviluppo e come usare tante incredibili librerie PHP,
all'interno o dipendenti da Symfony. Tenetevi pronti.

Fedele alla filosofia di Symfony, questo capitolo inizia spiegando il concetto
fondamentale comune allo sviluppo web: HTTP. Indipendentemente dalla propria storia
o dal linguaggio di programmazione preferito, questo capitolo andrebbe letto da tutti.

HTTP è semplice
---------------

HTTP (Hypertext Transfer Protocol per i geek) è un linguaggio testuale che consente a
due macchine di comunicare tra loro. Tutto qui! Per esempio, se si controlla
l'ultima vignetta di `xkcd`_, ha luogo la seguente conversazione
(approssimata):

.. image:: /images/http-xkcd.png
   :align: center

E sebbene il linguaggio usato in realtà sia un po' più formale, è ancora assolutamente semplice.
HTTP è il termine usato per descrivere tale semplice linguaggio testuale. Non importa in
quale linguaggio si sviluppi sul web, lo scopo di un server è *sempre* quello di
interpretare semplici richieste testuali e restituire semplici risposte testuali.

Symfony è costruito fin dalle basi attorno a questa realtà. Che lo si comprenda o
meno, HTTP è qualcosa che si usa ogni giorno. Con Symfony, si imparerà come
padroneggiarlo.

.. index::
   single: HTTP; Paradigma richiesta-risposta

Passo 1: il client invia una richiesta
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Ogni conversazione sul web inizia con una *richiesta*. La richiesta è un messaggio
testuale creato da un client (per esempio un browser, un'applicazione mobile, ecc.)
in uno speciale formato noto come HTTP. Il client invia la richiesta a un server e
quindi attende una risposta.

Diamo uno sguardo alla prima parte dell'interazione (la richiesta) tra un
browser e il server web di xkcd:

.. image:: /images/http-xkcd-request.png
   :align: center

Nel gergo di HTTP, questa richiesta apparirebbe in realtà in questo modo:

.. code-block:: text

    GET / HTTP/1.1
    Host: xkcd.com
    Accept: text/html
    User-Agent: Mozilla/5.0 (Macintosh)

Questo semplice messaggio comunica *ogni cosa* necessaria su quale risorsa
esattamente il client sta richiedendo. La prima riga di ogni richiesta HTTP
è la più importante e contiene due cose: l'URI e il metodo HTTP.

L'URI (p.e. ``/``, ``/contact``, ecc.) è l'indirizzo univoco o la locazione
che identifica la risorsa che il client vuole. Il metodo HTTP (p.e. ``GET``)
definisce cosa si vuole *fare* con la risorsa. I metodi HTTP sono *verbi*
della richiesta e definiscono i pochi modi comuni in cui si può agire
sulla risorsa:

+----------+---------------------------------+
| *GET*    | Recupera la risorsa dal server  |
+----------+---------------------------------+
| *POST*   | Crea una risorsa sul server     |
+----------+---------------------------------+
| *PUT*    | Aggiorna la risorsa sul server  |
+----------+---------------------------------+
| *DELETE* | Elimina la risorsa dal server   |
+----------+---------------------------------+

Tenendo questo a mente, si può immaginare come potrebbe apparire una richiesta HTTP
per cancellare una specifica voce di un blog, per esempio:

.. code-block:: text

    DELETE /blog/15 HTTP/1.1

.. note::

    Ci sono in realtà nove metodi HTTP definiti dalla specifica HTTP,
    ma molti di essi non sono molto usati o supportati. In realtà, molti
    browser moderni non supportano nemmeno i metodi ``PUT`` e ``DELETE``
    nei form. Molti altri sono invece supportati nelle richieste ajax,
    così come nelle rotte di Symfony.

In aggiunta alla prima linea, una richiesta HTTP contiene sempre altre linee
di informazioni, chiamate header. Gli header possono fornire un ampio raggio
di informazioni, come l'``Host`` richiesto, i formati di risposta accettati dal
client (``Accept``) e l'applicazione usata dal client per eseguire la richiesta
(``User-Agent``). Esistono molti altri header, che possono essere trovati nella
pagina di Wikipedia `Lista di header HTTP`_.

Passo 2: Il server restituisce una risposta
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Una volta che il server ha ricevuto la richiesta, sa esattamente la risorsa di
cui il client ha bisogno (tramite l'URI) e cosa vuole fare il client con tale
risorsa (tramite il metodo). Per esempio, nel caso di una richiesta GET, il server
prepara la risorsa e la restituisce in una risposta HTTP. Consideriamo la risposta
del server web di xkcd:

.. image:: /images/http-xkcd.png
   :align: center

Tradotto in HTTP, la risposta rimandata al browser assomiglierà a
questa: 

.. code-block:: text

    HTTP/1.1 200 OK
    Date: Sat, 02 Apr 2011 21:05:05 GMT
    Server: lighttpd/1.4.19
    Content-Type: text/html

    <html>
      <!-- ... HTML della vignetta di xkcd -->
    </html>

La risposta HTTP contiene la risorsa richiesta (il contenuto HTML, in questo caso).
oltre che altre informazioni sulla risposta. La prima riga è particolarmente
importante e contiene il codice di  stato della risposta HTTP (200, in questo caso).
Il codice di stato comunica il risultato globale della richiesta al client. La
richiesta è andata a buon fine? C'è stato un errore? Diversi codici di stato indicano
successo, errore o che il client deve fare qualcosa (p.e. rimandare a un'altra pagina).
Una lista completa può essere trovata nella pagina di Wikipedia
`Elenco dei codici di stato HTTP`_.

Come la richiesta, una risposta HTTP contiene parti aggiuntive di informazioni, note come
header. Per esempio, un importante header di risposta HTTP è ``Content-Type``. 
Il corpo della risorsa stessa potrebbe essere restituito in molti formati diversi, inclusi
HTML, XML o JSON, mentre l'header ``Content-Type`` usa i tipi di media di Internet, come ``text/html``, per
dire al client quale formato è restituito. Una lista di tipi di media comuni si può
trovare sulla voce di Wikipedia
`Lista di tipi di media comuni`_.

Esistono molti altri header, alcuni dei quali molto potenti. Per esempio, alcuni
header possono essere usati per creare un potente sistema di cache.

Richieste, risposte e sviluppo web
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Questa conversazione richiesta-risposta è il processo fondamentale che guida
tutta la comunicazione sul web. Questo processo è tanto importante e potente,
quanto inevitabilmente semplice.

L'aspetto più importante è questo: indipendentemente dal linguaggio usato, il
tipo di applicazione costruita (web, mobile, API JSON) o la filosofia di
sviluppo seguita, lo scopo finale di un'applicazione è **sempre** quello di capire
ogni richiesta e creare e restituire un'appropriata risposta.

L'architettura di Symfony è strutturata per corrispondere a questa realtà.

.. tip::

    Per saperne di più sulla specifica HTTP, si può leggere la `RFC HTTP 1.1`_ originale
    o la `HTTP Bis`_, che è uno sforzo attivo di chiarire la specifica originale. Un
    importante strumento per verificare sia gli header di richiesta che quelli di
    risposta durante la navigazione è l'estensione `Live HTTP Headers`_ di Firefox.

.. index::
   single: Fondamenti di Symfony; Richieste e risposte

Richieste e risposte in PHP
---------------------------

Dunque, come interagire con la "richiesta" e creare una "risposta" quando
si usa PHP? In realtà, PHP astrae un po' l'intero processo::

    $uri = $_SERVER['REQUEST_URI'];
    $pippo = $_GET['pippo'];

    header('Content-type: text/html');
    echo 'L\'URI richiesto è: '.$uri;
    echo 'Il valore del parametro "pippo" è: '.$pippo;

Per quanto possa sembrare strano, questa piccola applicazione di fatto prende
informazioni dalla richiesta HTTP e le usa per creare una risposta HTTP. Invece di
analizzare il messaggio di richiesta HTTP grezzo, PHP prepara della variabili superglobali,
come ``$_SERVER`` e ``$_GET``, che contengono tutte le informazioni dalla richiesta.
Similmente, invece di restituire un testo di risposta formattato come da HTTP, si può
usare la funzione ``header()`` per creare header di risposta e stampare semplicemente
il contenuto, che sarà la parte di contenuto del messaggio di risposta. PHP creerà una
vera risposta HTTP e la restituirà al client:

.. code-block:: text

    HTTP/1.1 200 OK
    Date: Sat, 03 Apr 2011 02:14:33 GMT
    Server: Apache/2.2.17 (Unix)
    Content-Type: text/html

    L'URI richiesto è: /testing?pippo=symfony
    Il valore del parametro "pippo" è: symfony

Richieste e risposte in Symfony
-------------------------------

Symfony fornisce un'alternativa all'approccio grezzo di PHP, tramite due classi
che consentono di interagire con richiesta e risposta HTTP in modo più facile.
La classe :class:`Symfony\\Component\\HttpFoundation\\Request` è una semplice
rappresentazione orientata agli oggetti del messaggio di richiesta HTTP. Con essa,
si hanno a portata di mano tutte le informazioni sulla richiesta::

    use Symfony\Component\HttpFoundation\Request;

    $request = Request::createFromGlobals();

    // l'URI richiesto (p.e. /about) tranne ogni parametro
    $request->getPathInfo();

    // recupera rispettivamente le variabili GET e POST
    $request->query->get('pippo');
    $request->request->get('pluto', 'valore predefinito se pluto non esiste');

    // recupera le variabili SERVER
    $request->server->get('HTTP_HOST');

    // recupera un'istanza di UploadedFile identificata da pippo
    $request->files->get('pippo');

    // recupera il valore di un COOKIE
    $request->cookies->get('PHPSESSID');

    // recupera un header di risposta HTTP, con chiavi normalizzate e minuscole
    $request->headers->get('host');
    $request->headers->get('content_type');

    $request->getMethod();    // GET, POST, PUT, DELETE, HEAD
    $request->getLanguages(); // un array di lingue accettate dal client

Come bonus, la classe ``Request`` fa un sacco di lavoro in sottofondo, di cui non ci si
dovrà mai preoccupare. Per esempio, il metodo ``isSecure()`` verifica **tre**
diversi valori in PHP che possono indicare se l'utente si stia connettendo o meno
tramite una connessione sicura (cioè HTTPS).

.. sidebar:: ParameterBags e attributi di Request

    Come visto in precedenza, le variabili ``$_GET`` e ``$_POST`` sono accessibili
    rispettivamente
    tramite le proprietà pubbliche ``query`` e ``request``. Entrambi questi oggetti
    sono oggetti della classe :class:`Symfony\\Component\\HttpFoundation\\ParameterBag`, che ha metodi come
    :method:`Symfony\\Component\\HttpFoundation\\ParameterBag::get`,
    :method:`Symfony\\Component\\HttpFoundation\\ParameterBag::has`,
    :method:`Symfony\\Component\\HttpFoundation\\ParameterBag::all` e altri.
    In effetti, ogni proprietà pubblica usata nell'esempio precedente è un'istanza
    di ParameterBag.

    .. _book-fundamentals-attributes:

    La classe Request ha anche una proprietà pubblica ``attributes``, che contiene
    dati speciali relativi a come l'applicazione funziona internamente. Per il
    framework Symfony, ``attributes`` contiene valori restituiti dalla rotta
    corrispondente, come ``_controller``, ``id`` (se si ha un parametro ``{id}``),
    e anche il nome della rotta stessa (``_route``). La proprietà
    ``attributes`` è pensata apposta per essere un posto in cui preparare
    e memorizzare informazioni sulla richiesta relative al contesto.

Symfony fornisce anche una classe ``Response``: una semplice rappresentazione PHP di un
messaggio di risposta HTTP. Questo consente a un'applicazione di usare un'interfaccia
orientata agli oggetti per costruire la risposta che occorre restituire al client::

    use Symfony\Component\HttpFoundation\Response;

    $response = new Response();

    $response->setContent('<html><body><h1>Ciao mondo!</h1></body></html>');
    $response->setStatusCode(200);
    $response->headers->set('Content-Type', 'text/html');

    // stampa gli header HTTP seguiti dal contenuto
    $response->send();

Se Symfony offrisse solo questo, si avrebbe già a disposizione un kit di strumenti per
accedere facilmente alle informazioni di richiesta e un'interfaccia orientata agli oggetti
per creare la risposta. Anche imparando le molte potenti caratteristiche di Symfony,
si tenga a mente che lo scopo di un'applicazione è sempre quello di *interpretare
una richiesta e creare l'appropriata risposta, basata sulla logica dell'applicazione*.

.. tip::

    Le classi ``Request`` e ``Response`` fanno parte di un componente a sé stante incluso
    con Symfony, chiamato ``HttpFoundation``. Questo componente può essere usato in modo
    completamente indipendente da Symfony e fornisce anche classi per gestire sessioni
    e caricamenti di file.

Il viaggio dalla richiesta alla risposta
----------------------------------------

Come lo stesso HTTP, gli oggetti ``Request`` e ``Response`` sono molto semplici.
La parte difficile nella costruzione di un'applicazione è la scrittura di quello che sta in
mezzo. In altre parole, il vero lavoro consiste nello scrivere il codice che interpreta
l'informazione della richiesta e crea la risposta.

Un'applicazione probabilmente deve fare molte cose, come inviare email, gestire
form, salvare dati in una base dati, rendere pagine HTML e proteggere contenuti. Come si
può gestire tutto questo e mantenere al contempo il codice organizzato e
mantenibile?

Symfony è stato creato per risolvere questi problemi.

Il front controller
~~~~~~~~~~~~~~~~~~~

Le applicazioni erano tradizionalmente costruite in modo che ogni "pagina" di un sito
fosse un file fisico:

.. code-block:: text

    index.php
    contact.php
    blog.php

Ci sono molti problemi con questo approccio, inclusa la flessibilità degli URL (che
succede se si vuole cambiare ``blog.php`` con ``news.php`` senza rompere tutti i
collegamenti?) e il fatto che ogni file *deve* includere manualmente alcuni file
necessari, in modo che la sicurezza, le connessioni alla base dati e l'aspetto del sito
possano rimanere coerenti.

Una soluzione molto migliore è usare un :term:`front controller`: un unico file PHP
che gestisce ogni richiesta che arriva all'applicazione. Per esempio:

+------------------------+----------------------+
| ``/index.php``         | esegue ``index.php`` |
+------------------------+----------------------+
| ``/index.php/contact`` | esegue ``index.php`` |
+------------------------+----------------------+
| ``/index.php/blog``    | esegue ``index.php`` |
+------------------------+----------------------+

.. tip::

    Usando il modulo ``mod_rewrite`` di Apache (o moduli equivalenti di altri server),
    gli URL possono essere facilmente puliti per essere semplicemente ``/``, ``/contact``
    e ``/blog``.

Ora ogni richiesta è gestita esattamente nello stesso modo. Invece di singoli URL che
eseguono diversi file PHP, è *sempre* eseguito il front controller, e il dirottamento
di URL diversi sulle diverse parti dell'applicazione è gestito internamente.
Questo risolve entrambi i problemi dell'approccio originario. Quasi tutte le applicazioni
web moderne fanno in questo modo, incluse applicazioni come WordPress.

Restare organizzati
~~~~~~~~~~~~~~~~~~~

Ma, all'interno del nostro front controller, come possiamo sapere quale pagina debba essere
resa e come poterla rendere in modo facile? In un modo o nell'altro, occorre verificare
l'URI in entrata ed eseguire parti diverse di codice, a seconda di tale valore. Le cose
possono peggiorare rapidamente::

    // index.php
    use Symfony\Component\HttpFoundation\Request;
    use Symfony\Component\HttpFoundation\Response;

    $request = Request::createFromGlobals();
    $path = $request->getPathInfo(); // il percorso dell'URI richiesto

    if (in_array($path, array('', '/'))) {
        $response = new Response('Benvenuto nella homepage.');
    } elseif ('/contact' === $path) {
        $response = new Response('Contattaci');
    } else {
        $response = new Response('Pagina non trovata.', 404);
    }
    $response->send();

La soluzione a questo problema può essere difficile. Fortunatamente, è *esattamente*
quello che Symfony è studiato per fare.

Il flusso di un'applicazione Symfony
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Quando si lascia a Symfony la gestione di ogni richiesta, la vita è molto più facile.
Symfony segue lo stesso semplice schema per ogni richiesta:

.. _request-flow-figure:

.. figure:: /images/request-flow.png
   :align: center
   :alt: flusso della richiesta di Symfony

   Le richieste in entrata sono interpretate dal routing e passate alle funzioni del
   controllore, che restituisce oggetti ``Response``.

Ogni "pagina" del proprio sito è definita in un file di configurazione delle rotte, che
mappa diversi URL su diverse funzioni PHP. Il compito di ogni funzione PHP, chiamata
:term:`controllore`, è di usare l'informazione della richiesta, insieme a molti altri
strumenti resi disponibili da Symfony, per creare e restituire un oggetto ``Response``.
In altre parole, il controllore è il posto in cui va il *proprio* codice: è dove
si interpreta la richiesta e si crea la risposta.

È così facile! Rivediamolo:

* Ogni richiesta esegue un file front controller;

* Il sistema delle rotte determina quale funzione PHP deve essere eseguita, in base
  all'informazione proveniente dalla richiesta e alla configurazione delle rotte creata;

* La giusta funzione PHP è eseguita, con il proprio codice che crea e restituisce l'oggetto
  ``Response`` appropriato.

Un richiesta Symfony in azione
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Senza entrare troppo in dettaglio, vediamo questo processo in azione. Supponiamo
di voler aggiungere una pagina ``/contact`` alla nostra applicazione Symfony. Primo,
iniziamo aggiungendo una voce per ``/contact`` nel file di configurazione delle rotte:

.. configuration-block::

    .. code-block:: yaml

        # app/config/routing.yml
        contact:
            path:     /contact
            defaults: { _controller: AppBundle:Main:contact }

    .. code-block:: xml

        <!-- app/config/routing.xml -->
        <?xml version="1.0" encoding="UTF-8" ?>
        <routes xmlns="http://symfony.com/schema/routing"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://symfony.com/schema/routing
                http://symfony.com/schema/routing/routing-1.0.xsd">

            <route id="contact" path="/contact">
                <default key="_controller">AppBundle:Main:contact</default>
            </route>
        </routes>

    .. code-block:: php

        // app/config/routing.php
        use Symfony\Component\Routing\Route;
        use Symfony\Component\Routing\RouteCollection;

        $collection = new RouteCollection();
        $collection->add('contact', new Route('/contact', array(
            '_controller' => 'AppBundle:Main:contact',
        )));

        return $collection;

Quando qualcuno vista la pagina ``/contact``, questa rotta viene corrisposta e il controllore
specificato è eseguito. Come si imparerà nel :doc:`capitolo delle rotte </book/routing>`,
la stringa ``AppBundle:Main:contact`` è una sintassi breve che punta a uno specifico
metodo PHP ``contactAction`` in una classe chiamata ``MainController``::

    // src/AppBundle/Controller/MainController.php
    namespace AppBundle\Controller;

    use Symfony\Component\HttpFoundation\Response;

    class MainController
    {
        public function contactAction()
        {
            return new Response('<h1>Contattaci!</h1>');
        }
    }

In questo semplice esempio, il controllore semplicemente crea un oggetto
:class:`Symfony\\Component\\HttpFoundation\\Response` con il codice HTML
``<h1>Contattaci!</h1>``. Nel :doc:`capitolo sul controllore </book/controller>`,
si imparerà come un controllore possa rendere dei template, consentendo al codice
di "presentazione" (cioè a qualsiasi cosa che scrive effettivamente HTML) di vivere in un
file template separato. Questo consente al controllore di preoccuparsi solo delle cose
difficili: interagire con la base dati, gestire l'invio di dati o l'invio di messaggi
email. 

.. _symfony2-build-your-app-not-your-tools:

Symfony: costruire un'applicazione, non degli strumenti
-------------------------------------------------------

Sappiamo dunque che lo scopo di un'applicazione è interpretare ogni richiesta in entrata
e creare un'appropriata risposta. Al crescere di un'applicazione, diventa sempre più
difficile mantenere il codice organizzato e mantenibile. Invariabilmente, gli
stessi complessi compiti continuano a presentarsi: persistere nella base dati, rendere e
riusare template, gestire form, inviare email, validare i dati degli utenti e
gestire la sicurezza.

La buona notizia è che nessuno di questi problemi è unico. Symfony fornisce un framework
pieno di strumenti che consentono di costruire un'applicazione, non di costruire degli
strumenti. Con Symfony, nulla viene imposto: si è liberi di usare l'intero framework
oppure un solo pezzo di Symfony.

.. index::
   single: Componenti di Symfony

.. _standalone-tools-the-symfony2-components:

Strumenti isolati: i *componenti* di Symfony
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Cos'*è* dunque Symfony? Primo, è un insieme di oltre venti librerie indipendenti, che
possono essere usate in *qualsiasi* progetto PHP. Queste librerie, chiamate
*componenti di Symfony*, contengono qualcosa di utile per quasi ogni situazione,
comunque sia sviluppato il proprio progetto. Solo per nominarne alcuni:

:doc:`HttpFoundation </components/http_foundation/introduction>`
    Contiene le classi ``Request`` e ``Response``, insieme ad altre  classi
    per gestire sessioni e caricamenti di file;

:doc:`Routing </components/routing/introduction>`
    Sistema di rotte potente e veloce, che
    consente di mappare uno specifico  URI (p.e. ``/contact``) ad alcune informazioni
    su come tale richiesta andrebbe gestita  (p.e. eseguendo il metodo ``contactAction()``);

:doc:`Form </components/form/introduction>`
    Un framework completo e flessibile per creare form e gestire invii di
    dati;

`Validator`_
    Un sistema per creare regole sui dati e quindi validarli, sia che i dati
    inviati dall'utente seguano o meno tali regole;

:doc:`Templating </components/templating/introduction>`
    Un insieme di strumenti per rendere template, gestire l'ereditarietà dei template (p.e.
    un template è decorato con un layout) ed eseguire altri compiti comuni sui template;

:doc:`Security </components/security/introduction>`
    Una potente libreria per gestire tutti i tipi di sicurezza all'interno di un'applicazione;

:doc:`Translation </components/translation/introduction>`
    Un framework per tradurre stringhe nella propria applicazione.

Tutti questi componenti sono disaccoppiati e possono essere usati in *qualsiasi* progetto
PHP, indipendentemente dall'uso del framework Symfony. Ogni parte di essi è stata
realizzata per essere usata se necessario e sostituita in caso contrario.

.. _the-full-solution-the-symfony2-framework:

La soluzione completa il *framework* Symfony
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Cos'*è* quindi il *framework* Symfony? Il *framework Symfony* è una libreria PHP
che esegue due compiti distinti:

#. Fornisce una selezione di componenti (cioè i componenti di Symfony) e
   librerie di terze parti (p.e. `Swiftmailer`_ per l'invio di email);

#. Fornisce una pratica configurazione e una libreria "collante", che lega insieme tutti
   i pezzi.

Lo scopo del framework è integrare molti strumenti indipendenti, per fornire
un'esperienza coerente allo sviluppatore. Anche il framework stesso è
un bundle (cioè un plugin) che può essere configurato o sostituito
interamente.

Symfony fornisce un potente insieme di strumenti per sviluppare rapidamente applicazioni web,
senza imposizioni sulla propria applicazione. Gli utenti normali possono iniziare velocemente
a sviluppare usando una distribuzione di Symfony, che fornisce uno scheletro di progetto
con configurazioni predefinite ragionevoli. Gli utenti avanzati hanno il cielo come limite.

.. _`xkcd`: http://xkcd.com/
.. _`RFC HTTP 1.1`: http://www.w3.org/Protocols/rfc2616/rfc2616.html
.. _`HTTP Bis`: http://datatracker.ietf.org/wg/httpbis/
.. _`Live HTTP Headers`: https://addons.mozilla.org/en-US/firefox/addon/live-http-headers/
.. _`Elenco dei codici di stato HTTP`: http://it.wikipedia.org/wiki/Elenco_dei_codici_di_stato_HTTP
.. _`Lista di header HTTP`: http://en.wikipedia.org/wiki/List_of_HTTP_header_fields
.. _`Lista di tipi di media comuni`: http://en.wikipedia.org/wiki/Internet_media_type#List_of_common_media_types
.. _`Validator`: https://github.com/symfony/Validator
.. _`Swiftmailer`: http://swiftmailer.org/
