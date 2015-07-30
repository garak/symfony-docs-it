.. index::
   single: Controllore

Il controllore
==============

Un controllore è una funzione PHP da creare, che prende le informazioni dalla
richiesta HTTP e crea e restituisce una risposta HTTP (come oggetto
``Response`` di Symfony). La risposta potrebbe essere una pagina HTML, un documento XML,
un array serializzato JSON, una immagine, un rinvio, un errore 404 o qualsiasi altra cosa
possa venire in mente. Il controllore contiene una qualunque logica arbitraria di cui la
*propria applicazione* necessita per rendere il contenuto di una pagina.

Per vedere quanto questo è semplice, diamo un'occhiata a un controllore di Symfony in azione.
Il seguente controllore renderebbe una pagina che stampa semplicemente ``Ciao mondo!``::

    use Symfony\Component\HttpFoundation\Response;

    public function helloAction()
    {
        return new Response('Ciao mondo!');
    }

L'obiettivo di un controllore è sempre lo stesso: creare e restituire un oggetto
``Response``. Lungo il percorso, potrebbe leggere le informazioni dalla richiesta, caricare una
risorsa da una base dati, inviare un'email, o impostare informazioni sulla sessione dell'utente.
Ma in ogni caso, il controllore alla fine restituirà un oggetto ``Response``
che verrà restituito al client.

Non c'è nessuna magia e nessun altro requisito di cui preoccuparsi! Di seguito alcuni
esempi comuni:

* Il *controllore A* prepara un oggetto ``Response`` che rappresenta il contenuto
  della homepage di un sito.

* Il *controllore B* legge il parametro ``slug`` da una richiesta per caricare un
  blog da una base dati  e creare un oggetto ``Response`` che visualizza
  quel blog. Se lo ``slug`` non viene trovato nella base dati, crea e
  restituisce un oggetto ``Response`` con codice di stato 404.

* Il *controllore C* gestisce l'invio di un form contatti. Legge le
  informazioni del form dalla richiesta, salva le informazioni del contatto nella
  base dati e invia una email con le informazioni del contatto al webmaster. Infine,
  crea un oggetto ``Response``, che rinvia il browser del client
  alla pagina di ringraziamento del form contatti.

.. index::
   single: Controllore; Ciclo di vita richiesta-controllore-risposta

Richieste, controllori, ciclo di vita della risposta
----------------------------------------------------

Ogni richiesta gestita da un progetto Symfony passa attraverso lo stesso semplice ciclo di vita.
Il framework si occupa dei compiti ripetitivi e infine esegue un
controllore, che ospita il codice personalizzato dell'applicazione:

#. Ogni richiesta è gestita da un singolo file con il controllore principale (ad esempio ``app.php``
   o ``app_dev.php``) che inizializza l'applicazione;

#. Il ``Router`` legge le informazioni dalla richiesta (ad esempio l'URI), trova
   una rotta che corrisponde a tali informazioni e legge il parametro ``_controller``
   dalla rotta;

#. Viene eseguito il controllore della rotta corrispondente e il codice all'interno del
   controllore crea e restituisce un oggetto ``Response``;

#. Le intestazioni HTTP e il contenuto dell'oggetto ``Response`` vengono rispedite
   al client.

Creare una pagina è facile, basta creare un controllore (#3) e fare una rotta che
mappa un URL su un controllore (#2).

.. note::

    Anche se ha un nome simile, il "controllore principale" (front controller) è diverso dagli altri
    "controllori" di cui si parla in questo capitolo. Un controllore principale
    è un breve file PHP che è presente nella propria cartella web e sul quale sono
    dirette tutte le richieste. Una tipica applicazione avrà un front controller
    produzione (ad esempio ``app.php``) e un frot controller per lo sviluppo
    (ad esempio ``app_dev.php``). Probabilmente non si avrà mai bisogno di modificare, visualizzare o preoccuparsi
    dei front controller dell'applicazione.

.. index::
   single: Controllore; Semplice esempio

Un semplice controllore
-----------------------

Mentre un controllore può essere un qualsiasi callable PHP (una funzione, un metodo di un oggetto,
o una ``Closure``), in Symfony, un controllore di solito è un unico metodo all'interno
di un oggetto controllore. I controllori sono anche chiamati *azioni*.

.. code-block:: php

    // src/AppBundle/Controller/HelloController.php
    namespace AppBundle\Controller;

    use Symfony\Component\HttpFoundation\Response;

    class HelloController
    {
        public function indexAction($name)
        {
            return new Response('<html><body>Ciao '.$name.'!</body></html>');
        }
    }

.. tip::

    Si noti che il *controllore* è il metodo ``indexAction``, che si trova
    all'interno di una *classe controllore* (``HelloController``). Non bisogna confondersi
    con i nomi: una *classe controllore* è semplicemente un modo comodo per raggruppare
    insieme vari controllori/azioni. Tipicamente, la classe controllore
    ospiterà diversi controllori/azioni (ad esempio ``updateAction``, ``deleteAction``,
    ecc).

Questo controllore è piuttosto semplice, ma vediamo di analizzarlo:

* *linea 3*: Symfony sfrutta la funzionalità degli spazi dei nomi di PHP 5.3 per
  utilizzarla nell'intera classe dei controllori. La parola chiave ``use`` importa la
  classe ``Response``, che il controllore deve restituire.

* *linea 6*: Il nome della classe è la concatenazione di un nome per la classe
  controllore (ad esempio ``Hello``) e la parola ``Controller``. Questa è una convenzione
  che fornisce coerenza ai controllori e permette loro di essere referenziati
  solo dalla prima parte del nome (ad esempio ``Hello``) nella configurazione delle rotte.

* *linea 8*: A ogni azione in una classe controllore viene aggiunto il suffisso ``Action``
  mentre nella configurazione delle rotte viene utilizzato come riferimento il solo nome dell'azione (``index``).
  Nella sezione successiva, verrà creata una rotta che mappa un URI in questa azione.
  Si imparerà come i segnaposto delle rotte (``{name}``) diventano parametri
  del metodo dell'azione (``$name``).

* *linea 10*: Il controllore crea e restituisce un oggetto ``Response``.

.. index::
   single: Controllore; Rotte e controllori

Mappare un URL in un controllore
--------------------------------

Il nuovo controllore restituisce una semplice pagina HTML. Per visualizzare questa pagina
nel browser, è necessario creare una rotta che mappa uno specifico schema URL
nel controllore:

.. configuration-block::

    .. code-block:: php-annotations

        // src/AppBundle/Controller/HelloController.php
        namespace AppBundle\Controller;

        use Symfony\Component\HttpFoundation\Response;
        use Sensio\Bundle\FrameworkExtraBundle\Configuration\Route;

        class HelloController
        {
            /**
             * @Route("/hello/{name}", name="hello")
             */
            public function indexAction($name)
            {
                return new Response('<html><body>Ciao '.$name.'!</body></html>');
            }
        }

    .. code-block:: yaml

        # app/config/routing.yml
        hello:
            path:      /hello/{name}
            # usa una sintassi speciale per puntare al controllore, vedere sotto
            defaults:  { _controller: AppBundle:Hello:index }

    .. code-block:: xml

        <!-- app/config/routing.xml -->
        <?xml version="1.0" encoding="UTF-8" ?>
        <routes xmlns="http://symfony.com/schema/routing"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://symfony.com/schema/routing
                http://symfony.com/schema/routing/routing-1.0.xsd">

            <route id="hello" path="/hello/{name}">
                <!-- usa una sintassi speciale per puntare al controllore, vedere sotto -->
                <default key="_controller">AppBundle:Hello:index</default>
            </route>
        </routes>

    .. code-block:: php

        // app/config/routing.php
        use Symfony\Component\Routing\Route;
        use Symfony\Component\Routing\RouteCollection;

        $collection = new RouteCollection();
        $collection->add('hello', new Route('/hello/{name}', array(
            // usa una sintassi speciale per puntare al controllore, vedere sotto
            '_controller' => 'AppBundle:Hello:index',
        )));

        return $collection;

Andando in ``/hello/ryan`` (p.e. ``http://localhost:8000/app_dev.php/hello/ryan``
se si usa il :doc:`server web interno </cookbook/web_server/built_in>`)
Symfony esegue il controllore ``HelloController::indexAction()``
e passa ``ryan`` nella variabile ``$name``. Creare una "pagina" significa
semplicemente creare un metodo controllore e associargli una rotta.

Simple, right?

.. sidebar:: La sintassi AppBundle:Hello:index del controllore

    Se si usano i formati YML o XML, si farà riferimento al controllore usando
    una speciale sintassi abbrevviata: ``AppBundle:Hello:index``. Per maggiori dettagli
    sul formato del controllore, vedere :ref:`controller-string-syntax`.

.. seealso::

    Si può imparare molto di più sul sistema delle rotte
    leggendo il :doc:`capitolo sulle rotte </book/routing>`.

.. index::
   single: Controllore; Parametri del controllore

.. _route-parameters-controller-arguments:

I parametri delle rotte come parametri del controllore
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Si è già appreso che la rotta punta a un metodo ``HelloController::indexAction()``,
che si trova all'interno di un bundle ``AppBundle``. La cosa più interessante è
il parametro passato a tale metodo::

    // src/AppBundle/Controller/HelloController.php
    // ...
    use Sensio\Bundle\FrameworkExtraBundle\Configuration\Route;

    /**
     * @Route("/hello/{name}", name="hello")
     */
    public function indexAction($name)
    {
        // ...
    }

Il controllore ha un solo parametro, ``$name``, che corrisponde al
parametro ``{name}`` della rotta corrispondente (``ryan`` se si va su ``/hell/ryan``).
Infatti, quando viene eseguito il controllore, Symfony abbina ogni parametro del
controllore a un parametro della rotta. Quindi il valore di ``{name}`` viene passato a ``$name``.

Vedere il seguente esempio:

.. configuration-block::

    .. code-block:: php-annotations

        // src/AppBundle/Controller/HelloController.php
        // ...

        use Sensio\Bundle\FrameworkExtraBundle\Configuration\Route;

        class HelloController
        {
            /**
             * @Route("/hello/{firstName}/{lastName}", name="hello")
             */
            public function indexAction($firstName, $lastName)
            {
                // ...
            }
        }

    .. code-block:: yaml

        # app/config/routing.yml
        hello:
            path:      /hello/{firstName}/{lastName}
            defaults:  { _controller: AppBundle:Hello:index }

    .. code-block:: xml

        <!-- app/config/routing.xml -->
        <?xml version="1.0" encoding="UTF-8" ?>
        <routes xmlns="http://symfony.com/schema/routing"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://symfony.com/schema/routing
                http://symfony.com/schema/routing/routing-1.0.xsd">

            <route id="hello" path="/hello/{firstName}/{lastName}">
                <default key="_controller">AppBundle:Hello:index</default>
            </route>
        </routes>

    .. code-block:: php

        // app/config/routing.php
        use Symfony\Component\Routing\Route;
        use Symfony\Component\Routing\RouteCollection;

        $collection = new RouteCollection();
        $collection->add('hello', new Route('/hello/{firstName}/{lastName}', array(
            '_controller' => 'AppBundle:Hello:index',
        )));

        return $collection;

Per questo il controllore può richiedere diversi parametri::

    public function indexAction($firstName, $lastName)
    {
        // ...
    }

La mappatura dei parametri delle rotte nei parametri del controllore è semplice e flessibile. Tenere
in mente le seguenti linee guida mentre si sviluppa.

* **L'ordine dei parametri del controllore non ha importanza**

  Symfony abbina i **nomi** dei parametri delle rotte e i **nomi** delle variabili
  dei metodi dei controllori. I parametri del controllore possono essere totalmente riordinati e
  continuare a funzionare perfettamente::

      public function indexAction($lastName, $firstName)
      {
          // ...
      }

* **Ogni parametro richiesto del controllore, deve corrispondere a uno dei parametri della rotta**

  Il codice seguente genererebbe un ``RuntimeException``, perché non c'è nessun parametro ``foo``
  definito nella rotta::

      public function indexAction($firstName, $lastName, $foo)
      {
          // ...
      }

  Rendere il parametro facoltativo metterebbe le cose a posto. Il seguente
  esempio non lancerebbe un'eccezione::

      public function indexAction($firstName, $lastName, $foo = 'bar')
      {
          // ...
      }

* **Non tutti i parametri delle rotte devono essere parametri del controllore**

  Se, per esempio, ``last_name`` non è importante per il controllore,
  si può ometterlo del tutto::

      public function indexAction($firstName)
      {
          // ...
      }

.. tip::

    Ogni rotta ha anche un parametro speciale ``_route``, che è equivalente al
    nome della rotta che è stata abbinata (ad esempio ``hello``). Anche se di solito non è
    utile, questa è ugualmente disponibile come parametro del controllore. Si possono anche
    passare altre variabili alla rotta, dai parametri del controllore. Vedere
    :doc:`/cookbook/routing/extra_information`.

.. _book-controller-request-argument:

La ``Request`` come parametro del controllore
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Che fare se si ha bisogno di leggere i parametri della query string o un header o accedere
a un file caricato? Tutte queste informazioni sono memorizzate nell'oggetto ``Request`` di Symfony.
Per ottenerlo in un controllore, basta aggiungerlo come parametro e
**forzare il tipo a Request**::

    use Symfony\Component\HttpFoundation\Request;

    public function indexAction($firstName, $lastName, Request $request)
    {
        $page = $request->query->get('page', 1);

        // ...
    }

.. seealso::

    Per saperne di più su come ottenere informazioni dalla richiesta, si veda
    :ref:`accedere alla informazioni sulla richiesta <component-http-foundation-request>`.

.. index::
   single: Controllore; Classe base Controller

La classe base del controllore
------------------------------

Per comodità, Symfony ha una classe base ``Controller``, che aiuta
nelle attività più comuni del controllore e dà alla classe controllore
l'accesso ai servizi, tramite il contenitore (vedere :ref:`controller-accessing-services`).

Aggiungere la dichiarazione ``use`` sopra alla classe ``Controller`` e modificare
``HelloController`` per estenderla::

    // src/AppBundle/Controller/HelloController.php
    namespace AppBundle\Controller;

    use Symfony\Bundle\FrameworkBundle\Controller\Controller;

    class HelloController extends Controller
    {
        // ...
    }

Questo in realtà non cambia nulla su come lavora il controllore: dà solo
accesso a dei metodi aiutanti, resi disponibili dalla
classe base del controllore. Questi metodi sono solo scorciatoie per usare funzionalità
del nucleo di Symfony, che sono a disposizione con o senza la classe
base di ``Controller``. Un ottimo modo per vedere le funzionalità del nucleo in azione
è quello di guardare nella `classe Controller`_.

.. seealso::

    È inoltre possibile definire i :doc:`controllori come servizi </cookbook/controller/service>`.
    È opzionale, ma può dare maggiore controllo sulle esatte dipendenze e sugli oggetti
    iniettati dentro al
    controllore.

.. index::
   single: Controllore; Rinvio

Rinvio
~~~~~~

Se si vuole rinviare l'utente a un'altra pagina, usare il
metodo
:method:`Symfony\\Bundle\\FrameworkBundle\\Controller\\Controller::redirect`::

    public function indexAction()
    {
        return $this->redirect($this->generateUrl('homepage'));
    }

Il metodo ``generateUrl()`` è solo una funzione di supporto che genera l'URL
per una determinata rotta. Per maggiori informazioni, vedere il capitolo
:doc:`Rotte </book/routing>`.

Per impostazione predefinita, il metodo ``redirect()`` esegue un rinvio 302 (temporaneo). Per
eseguire un rinvio 301 (permanente), modificare il secondo parametro::

    public function indexAction()
    {
        return $this->redirect($this->generateUrl('homepage'), 301);
    }

.. tip::

    Il metodo ``redirect()`` è semplicemente una scorciatoia che crea un oggetto ``Response``
    specializzato nel rinviare l'utente. È equivalente a::

        use Symfony\Component\HttpFoundation\RedirectResponse;

        return new RedirectResponse($this->generateUrl('homepage'));

.. index::
   single: Controllore; Rendere i template

.. _controller-rendering-templates:

Rendere i template
~~~~~~~~~~~~~~~~~~

Se si serve dell'HTML, si vorrà rendere un template. Il metodo ``render()``
rende un template **e** ne inserisce il contenuto in un oggetto
``Response``::

    // rende app/Resources/views/hello/index.html.twig
    return $this->render('hello/index.html.twig', array('name' => $name));

Si possono anche mettere template in sottocartelle. Meglio però evitare di creare
strutture inutilmente profonde::

    // rende app/Resources/views/hello/greetings/index.html.twig
    return $this->render('hello/greetings/index.html.twig', array(
        'name' => $name
    ));

Il motore di template di Symfony è spiegato in gran dettaglio nel capitolo
:doc:`Template </book/templating>`.

.. sidebar:: Riferimenti a template che si trovano in un bundle

    Si possono anche mettere template nella cartella ``Resources/views`` di un
    bundle e farvi riferimento con la sintassi
    ``NomeBundle:NomeCartella:NomeFile``. Per esempio,
    ``AppBundle:Hello:index.html.twig`` si riferisce a un template collocato in
    ``src/AppBundle/Resources/views/Hello/index.html.twig``. Vedere :ref:`template-referencing-in-bundle`.

.. index::
   single: Controllore; Accedere ai servizi

.. _controller-accessing-services:

Accesso ad altri servizi
~~~~~~~~~~~~~~~~~~~~~~~~

Symfony dispone di vari oggetti utili, chiamati servizi. Si possono usare
per rendere template, inviare email, interrogare la base dati e per ogni altro
"lavoro" immaginabile. Quando si installa un nuovo bundle, probabilmente si avranno
a disposizione *ulteriori* servizi.

Quando si estende la classe base del controllore, è possibile accedere a qualsiasi servizio di Symfony
attraverso il metodo ``get()``. Di seguito si elencano alcuni servizi comuni che potrebbero essere utili::

    $templating = $this->get('templating');

    $router = $this->get('router');

    $mailer = $this->get('mailer');

Ci sono innumerevoli altri servizi disponibili. Per elencarli tutti, utilizzare il comando di console
``container:debug``:

.. code-block:: bash

    $ php app/console container:debug

Per maggiori informazioni, vedere il capitolo :doc:`/book/service_container`.

.. index::
   single: Controllore; Gestire gli errori
   single: Controllore; Pagine 404

Gestire gli errori e le pagine 404
----------------------------------

Quando qualcosa non si trova, si dovrebbe utilizzare bene il protocollo HTTP e
restituire una risposta 404. Per fare questo, si lancia uno speciale tipo di eccezione.
Se si sta estendendo la classe base del controllore, procedere come segue::

    public function indexAction()
    {
        // recuperare l'oggetto dalla base dati 
        $product = ...;
        if (!$product) {
            throw $this->createNotFoundException('Il prodotto non esiste');
        }

        return $this->render(...);
    }

Il metodo ``createNotFoundException()`` crea uno speciale oggetto
:class:`Symfony\\Component\\HttpKernel\\Exception\\NotFoundHttpException`,
che infine innesca una risposta HTTP 404 all'interno di Symfony.

Naturalmente si è liberi di lanciare qualunque classe ``Exception`` nel controllore:
Symfony restituirà automaticamente un codice di risposta HTTP 500.

.. code-block:: php

    throw new \Exception('Qualcosa è andato storto!');

In ogni caso, all'utente finale viene mostrata una pagina di errore predefinita e allo sviluppatore
viene mostrata una pagina di errore completa di debug (cioè usando ``app_dev.php``,
vedere :ref:`page-creation-environments`).

Entrambe le pagine di errore possono essere personalizzate. Per ulteriori informazioni, leggere
nel ricettario ":doc:`/cookbook/controller/error_pages`".

.. index::
   single: Controllore; La sessione
   single: Sessione

Gestione della sessione
-----------------------

Symfony fornisce un oggetto sessione che si può utilizzare per memorizzare le informazioni
sull'utente (che sia una persona reale che utilizza un browser, un bot, o un servizio web)
attraverso le richieste. Per impostazione predefinita, Symfony memorizza gli attributi in un cookie
utilizzando le sessioni PHP native.

Memorizzare e recuperare informazioni dalla sessione può essere fatto
da qualsiasi controllore::

    use Symfony\Component\HttpFoundation\Request;

    public function indexAction(Request $request)
    {
        $session = $request->getSession();

        // memorizza un attributo per riutilizzarlo durante una successiva richiesta dell'utente
        $session->set('pippo', 'pluto');

        // in un altro controllore per un'altra richiesta
        $pippo = $session->get('pippo');

        // usa un valore predefinito, se la chiave non esiste
        $filters = $session->get('filters', array());
    }

Questi attributi rimarranno sull'utente per il resto della sessione
utente.

.. index::
   single Sessione; Messaggi flash

Messaggi flash
~~~~~~~~~~~~~~

È anche possibile memorizzare messaggi di piccole dimensioni, all'interno della sessione dell'utente
e solo per la richiesta successiva. Ciò è utile quando si elabora un form:
si desidera rinviare e avere un messaggio speciale mostrato sulla richiesta *successiva*.
I messaggi di questo tipo sono chiamati messaggi "flash".

Per esempio, immaginiamo che si stia elaborando un form inviato::

    use Symfony\Component\HttpFoundation\Request;

    public function updateAction(Request $request)
    {
        $form = $this->createForm(...);

        $form->handleRequest($request);

        if ($form->isValid()) {
            // fare una qualche elaborazione

            $request->getSession()->getFlashBag()->add(
                'notice',
                'Le modifiche sono state salvate!'
            );

            return $this->redirect($this->generateUrl(...));
        }

        return $this->render(...);
    }

Dopo l'elaborazione della richiesta, il controllore imposta un messaggio flash ``notice``
e poi rinvia. Il nome (``notice``) non è significativo, è solo quello che
si utilizza per identificare il tipo del messaggio.

Nel template dell'azione successiva, il seguente codice può essere utilizzato per rendere
il messaggio ``notice``:

.. configuration-block::

    .. code-block:: html+jinja

        {% for flashMessage in app.session.flashbag.get('notice') %}
            <div class="flash-notice">
                {{ flashMessage }}
            </div>
        {% endfor %}

    .. code-block:: html+php

        <?php foreach ($view['session']->getFlash('notice') as $message): ?>
            <div class="flash-notice">
                <?php echo "<div class='flash-error'>$message</div>" ?>
            </div>
        <?php endforeach ?>

Per come sono stati progettati, i messaggi flash sono destinati a vivere esattamente per una richiesta (hanno la
"durata di un flash"). Sono progettati per essere utilizzati con un rinvio, esattamente come
è stato fatto in questo esempio.

.. index::
   single: Controllore; Oggetto Response

L'oggetto Response
------------------

L'unico requisito per un controllore è restituire un oggetto ``Response``. La
classe :class:`Symfony\\Component\\HttpFoundation\\Response` è una astrazione PHP
sulla risposta HTTP, il messaggio testuale che contiene gli header HTTP
e il contenuto che viene inviato al client::

    use Symfony\Component\HttpFoundation\Response;

    // crea una semplice risposta JSON con un codice di stato 200 (predefinito)
    $response = new Response('Ciao '.$name, 200);

    // crea una risposta JSON con un codice di stato 200
    $response = new Response(json_encode(array('name' => $name)));
    $response->headers->set('Content-Type', 'application/json');

La proprietà ``headers`` è un oggetto :class:`Symfony\\Component\\HttpFoundation\\HeaderBag`
con alcuni utili metodi per leggere e modificare gli header ``Response``. I nomi degli
header sono normalizzati in modo che l'utilizzo di ``Content-Type`` sia equivalente a
``content-type`` o anche a ``content_type``.

Ci sono anche alcune classi speciali, che facilitano alcuni tipi di risposta:

* Per JSON, :class:`Symfony\\Component\\HttpFoundation\\JsonResponse`.
  Vedere :ref:`component-http-foundation-json-response`.

* Per i file, :class:`Symfony\\Component\\HttpFoundation\\BinaryFileResponse`.
  Vedere :ref:`component-http-foundation-serving-files`.

* Per le risposte in flussi, :class:`Symfony\\Component\\HttpFoundation\\StreamedResponse`.
  Per :ref:`streaming-response`.

.. seealso::

    Niente paura! Ci sono molte altre informazioni nell'oggetto ``Response``
    nella documentazione sui componenti. Vedere :ref:`component-http-foundation-response`.

.. index::
   single: Controllore; Oggetto Request 

L'oggetto Request
-----------------

Oltre ai valori dei segnaposto delle rotte, il controllore ha anche accesso
all'oggetto ``Request``. Il framework inietta l'oggetto ``Request`` nel
controllore, se una variabile è forzata a
`Symfony\Component\HttpFoundation\Request`::

    use Symfony\Component\HttpFoundation\Request;

    public function indexAction(Request $request)
    {
        $request->isXmlHttpRequest(); // è una richiesta Ajax?

        $request->getPreferredLanguage(array('en', 'fr'));

        $request->query->get('page'); // recupera un parametro $_GET

        $request->request->get('page'); // recupera un parametro $_POST
    }

Come l'oggetto ``Response``, le intestazioni della richiesta sono memorizzate in un oggetto ``HeaderBag``
e sono facilmente accessibili.

.. seealso::

    Niente paura! Ci sono molte altre informazioni nell'oggetto ``Request``
    nella documentazione sui componenti. Vedere :ref:`component-http-foundation-response`.

Creare pagine statiche
----------------------

Si può creare una pagina statica, senza nemmeno creare un controllore (basta una rotta
e un template).

Vedere :doc:`/cookbook/templating/render_without_controller`.

.. index::
   single: Controllore; Inoltro

Inoltro a un altro controllore
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Si può anche facilmente inoltrare internamente a un altro controllore con il metodo
:method:`Symfony\\Bundle\\FrameworkBundle\\Controller\\Controller::forward`.
Invece di redirigere il browser dell'utente, fa una sotto richiesta interna
e chiama il controllore specificato. Il metodo ``forward()`` restituisce l'oggetto
``Response`` che è tornato da quel controllore::

    public function indexAction($name)
    {
        $response = $this->forward('AppBundle:Something:fancy', array(
            'name'  => $name,
            'color' => 'green',
        ));

        // ... modificare ulteriormente la risposta o restituirla direttamente

        return $response;
    }

Si noti che il metodo ``forward()`` utilizza la stessa rappresentazione stringa del
controllore (vedere :ref:`controller-string-syntax`). In questo caso, l'obiettivo
della classe del controllore sarà ``SomethingController::fancyAction()``
in ``AppBundle``. L'array passato al metodo diventa un insieme di parametri sul controllore risultante.
La stessa interfaccia viene utilizzata quando si incorporano controllori nei template (vedere
:ref:`templating-embedding-controller`). L'obiettivo del metodo controllore dovrebbe
essere simile al seguente::

    public function fancyAction($name, $color)
    {
        // ... creare e restituire un oggetto Response
    }

E proprio come quando si crea un controllore per una rotta, l'ordine dei parametri
di ``fancyAction`` non è importante. Symfony controlla i nomi degli indici chiave
(ad esempio ``name``) con i nomi dei parametri del metodo (ad esempio ``$name``). Se
si modifica l'ordine dei parametri, Symfony continuerà a passare il corretto
valore di ogni variabile.

Considerazioni finali
---------------------

Ogni volta che si crea una pagina, è necessario scrivere del codice che
contiene la logica per quella pagina. In Symfony, questo codice si chiama controllore,
ed è una funzione PHP che può fare qualsiasi cosa occorra per restituire
l'oggetto finale ``Response``, che verrà restituito all'utente.

Per rendere la vita più facile, si può scegliere di estendere una classe base ``Controller``,
che contiene metodi scorciatoia per molti compiti comuni del controllore. Per esempio,
dal momento che non si vuole mettere il codice HTML nel controllore, è possibile utilizzare
il metodo ``render()`` per rendere e restituire il contenuto da un template.

In altri capitoli, si vedrà come il controllore può essere usato per persistere e
recuperare oggetti da una base dati, processare i form inviati, gestire la cache e
altro ancora.

Imparare di più dal ricettario
------------------------------

* :doc:`/cookbook/controller/error_pages`
* :doc:`/cookbook/controller/service`

.. _`classe Controller`: https://github.com/symfony/symfony/blob/master/src/Symfony/Bundle/FrameworkBundle/Controller/Controller.php
