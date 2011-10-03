.. index::
   single: Controller

Il controllore
==============

Un controllore è una funzione PHP che bisogna creare, che prende le informazioni dalla
richiesta HTTP e costruttori e restituisce una risposta HTTP (come oggetto
``Response`` di Symfony2). La risposta potrebbe essere una pagina HTML, un documento XML,
un array serializzato JSON, una immagine, una redirezione, un errore 404 o qualsiasi altra cosa
possa venire in mente. Il controllore contiene una qualunque logica arbitraria di cui la
*propria applicazione* necessita per rendere il contenuto di una pagina.

Per vedere come questo è semplice, diamo un'occhiata  ad un controllore di Symfony2 in azione.
Il seguente controllore renderebbe una pagina che stampa semplicemente ``Ciao mondo!``::

    use Symfony\Component\HttpFoundation\Response;

    public function helloAction()
    {
        return new Response('Ciao mondo!');
    }

L'obiettivo di un controllore è sempre lo stesso: creare e restituire un oggetto
``Response``. Lungo il percorso, potrebbe leggere le informazioni dalla richiesta, caricare una
risorsa da un database, inviare una e-mail, o impostare informazioni sulla sessione dell'utente.
Ma in ogni caso, il controllore alla fine restituirà un oggetto ``Response``
che verrà restituito al client.
	
Non c'è nessuna magia e nessun altro requisito di cui preoccuparsi! Di seguito alcuni
esempi comuni:

* Il *controllore A* prepara un oggetto ``Response`` che rappresenta il contenuto
  della homepage di un sito.

* Il *controllore B* legge il parametro ``slug`` da una richiesta per caricare un
  blog da un database e creare un oggetto ``Response`` che visualizza
  quel blog. Se lo ``slug`` non viene trovato nel database, crea e
  restituisce un oggetto ``Response`` con codice di stato 404.

* Il *controllore C* gestisce l'invio form di un form contatti. Legge le
  informazioni del form da dalla richiesta, salva le informazioni del contatto nel
  database ed invia una email con le informazioni del contatto al webmaster. Infine,
  crea un oggetto ``Response``che reindirizza il browser del client al
  alla pagina di ringraziamento del form contatti.

.. index::
   single: Controller; Request-controller-response lifecycle

Richieste, controllori, ciclo di vita della risposta
----------------------------------------------------

Ogni richiesta gestita da un progetto Symfony2 passa attraverso lo stesso semplice ciclo di vita.
Il framework si occupa dei compiti ripetitivi ed infine esegue un
controllore, che ospita il codice personalizzato dell'applicazione:

#. Ogni richiesta è gestita da un singolo file con il controllore principale (ad esempio ``app.php``
   o ``app_dev.php``) che è il bootstrap dell'applicazione;

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

    Anche se ha un nome simile, un "controllore principale" è diverso dagli altri
    "controllori" di cui si parla in questo capitolo. Un controllore principale
    è un breve file PHP che è presente nella vostra cartella web e sul quale sono
    dirette tutte le richieste. Una tipica applicazione avrà un controllore
    principale di produzione (ad esempio ``app.php``) e un controllore principale per lo sviluppo
    (ad esempio ``app_dev.php``). Probabilmente non si avrà mai bisogno di modificare, visualizzare o preoccuparsi
    del controllore principale dell'applicazione.

.. index::
   single: Controller; Simple example

Un semplice controllore
-----------------------

Mentre un controllore può essere un qualsiasi PHP callable (una funzione, un metodo di un oggetto,
o una ``Closure``), in Symfony2, un controllore di solito è un unico metodo all'interno
di un oggetto controllore. I controllori sono anche chiamati *azioni*.

.. code-block:: php
    :linenos:

    // src/Acme/HelloBundle/Controller/HelloController.php

    namespace Acme\HelloBundle\Controller;
    use Symfony\Component\HttpFoundation\Response;

    class HelloController
    {
        public function indexAction($name)
        {
          return new Response('<html><body>Ciao '.$name.'!</body></html>');
        }
    }

.. tip::

    Si noti che il *controllore* è il metodo ``indexAction``, che vive
    all'interno di una *classe controllore* (``HelloController``). Non bisogna confondersi
    con i nomi: una *classe controllore* è semplicemente un modo comodo per raggruppare
    insieme vari controllori/azioni. Tipicamente, la classe controllore
    ospiterà diversi controllori/azioni (ad esempio ``updateAction``, ``deleteAction``,
    ecc).

Questo controllore è piuttosto semplice, ma vediamo di analizzarlo:

* *line 3*: Symfony2 sfrutta la funzionalità namespace di PHP 5.3 per
  utilizzarla nell'intera classe dei controllori. La parola chiave ``use`` importa la
  classe ``Response``, che il controllore deve restituire.

* *line 6*: Il nome della classe è la concatenazione di un nome per la classe
  controllore (ad esempio ``Hello``) e la parola ``Controller``. Questa è una convenzione
  che fornisce consistenza ai controllori e permette loro di essere referenziati
  solo dalla prima parte del nome (ad esempio ``Hello``) nella configurazione delle rotte.

* *line 8*: Ad ogni azione in una classe controllore viene aggiunto il suffisso ``Action``
  mentre nella configurazione delle rotte viene utilizzato come riferimento il solo nome dell'azione (``index``).
  In the next section, you'll create a route that maps a URI to this action.
  Si imparerà come i segnaposto delle rotte (``{name}``) diventano argomenti
  del metodo dell'azione (``$name``).

* *line 10*: Il controllore crea e restituisce un oggetto ``Response``.

.. index::
   single: Controller; Routes and controllers

Mappare un URL in un controllore
--------------------------------

Il nuovo controllore restituisce una semplice pagina HTML. Per visualizzare questa pagina
nel browser, è necessario creare una rotta che mappa uno specifico pattern URL
nel controllore:

.. configuration-block::

    .. code-block:: yaml

        # app/config/routing.yml
        hello:
            pattern:      /hello/{name}
            defaults:     { _controller: AcmeHelloBundle:Hello:index }

    .. code-block:: xml

        <!-- app/config/routing.xml -->
        <route id="hello" pattern="/hello/{name}">
            <default key="_controller">AcmeHelloBundle:Hello:index</default>
        </route>

    .. code-block:: php

        // app/config/routing.php
        $collection->add('hello', new Route('/hello/{name}', array(
            '_controller' => 'AcmeHelloBundle:Hello:index',
        )));

Andando in ``/hello/ryan`` ora viene eseguito il controllore ``HelloController::indexAction()``
e passa ``ryan`` nella variabile ``$name``. Creare una
"pagina" significa semplicemente creare un metodo controllore e associargli una rotta.

Si noti la sintassi utilizzata per fare riferimento al controllore: ``AcmeHelloBundle:Hello:index``.
Symfony2 utilizza una notazione flessibile per le stringhe per fare riferimento a diversi controllori.
Questa è la sintassi più comune e dice a Symfony2 di cercare una classe
controllore chiamata ``HelloController`` dentro un bundle chiamato ``AcmeHelloBundle``. Il
metodo ``indexAction()`` viene quindi eseguito.

Per maggiori dettagli sul formato stringa utilizzato per fare riferimento ai controllori differenti,
vedere :ref:`controller-string-syntax`.

.. note::

    Questo esempio pone la configurazione delle rotte direttamente nella cartella ``app/config/``.
    Un modo migliore per organizzare le proprie rotte è quello di posizionare ogni rotta
    nel bundle a cui appartiene. Per ulteriori informazioni si veda
    :ref:`routing-include-external-resources`.

.. tip::

    Si può imparare molto di più sul sistema delle rotte leggendo il :doc:`Capitolo sulla rotte</book/routing>`.

.. index::
   single: Controller; Controller arguments

.. _route-parameters-controller-arguments:

I parametri delle rotte come argomenti del controllore
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Si è già appreso che il parametro ``AcmeHelloBundle:Hello:index`` di ``_controller``
fa riferimento a un metodo ``HelloController::indexAction()`` che vive all'interno di un
bundle ``AcmeHelloBundle``. La cosa più interessante è che gli argomenti vengono
passati a tale metodo:

.. code-block:: php

    <?php
    // src/Acme/HelloBundle/Controller/HelloController.php

    namespace Acme\HelloBundle\Controller;
    use Symfony\Bundle\FrameworkBundle\Controller\Controller;

    class HelloController extends Controller
    {
        public function indexAction($name)
        {
          // ...
        }
    }

Il controllore ha un solo argomento, ``$name``, che corrisponde al
parametro ``{name}`` della rotta corrispondente (``ryan`` nel nostro esempio).
Infatti, quando viene eseguito il controllore, Symfony2 verifica ogni argomento del
controllore con un parametro della rotta abbinata. Vedere il seguente
esempio:

.. configuration-block::

    .. code-block:: yaml

        # app/config/routing.yml
        hello:
            pattern:      /hello/{first_name}/{last_name}
            defaults:     { _controller: AcmeHelloBundle:Hello:index, color: green }

    .. code-block:: xml

        <!-- app/config/routing.xml -->
        <route id="hello" pattern="/hello/{first_name}/{last_name}">
            <default key="_controller">AcmeHelloBundle:Hello:index</default>
            <default key="color">green</default>
        </route>

    .. code-block:: php

        // app/config/routing.php
        $collection->add('hello', new Route('/hello/{first_name}/{last_name}', array(
            '_controller' => 'AcmeHelloBundle:Hello:index',
            'color'       => 'green',
        )));

Il controllore per questo può richiedere diversi argomenti::

    public function indexAction($first_name, $last_name, $color)
    {
        // ...
    }

Si noti che entrambe le variabili segnaposto (``{first_name}``, ``{last_name}``)
così come la variabile predefinita ``color`` sono disponibili come argomenti nel
controllore. Quando una rotta viene abbinata, le variabili segnaposto vengono unite
con le ``impostazioni predefinite`` per creare un array che è disponibile al controllore.

La mappatura dei parametri delle rotte negli argomenti del controllore è semplice e flessibile. Tenere
in mente le seguenti linee guida mentre si sviluppa.

* **L'ordine degli argomenti del controllore non ha importanza**

    Symfony è in grado di abbinare i nomi dei parametri delle rotte nei nomi delle variabili
    dei metodi dei controllori. In altre parole, si vuole dire che
    il parametro ``{last_name}`` corrisponde all'argomento ``$last_name``.
    Gli argomenti del controllore possono essere totalmente riordinati e continuare a funzionare
    perfettamente::

        public function indexAction($last_name, $color, $first_name)
        {
            // ..
        }

* **Ogni argomento richiesto del controllore, deve corrispondere ad uno dei parametri della rotta**

    Il codice seguente genererebbe un ``RuntimeException``perché non c'è nessun parametro ``foo``
    definito nella rotta::

        public function indexAction($first_name, $last_name, $color, $foo)
        {
            // ..
        }

    Rendere l'argomento facoltativo, metterebbe le cose a posto. Il seguente
    esempio non lancerebbe un'eccezione::

        public function indexAction($first_name, $last_name, $color, $foo = 'bar')
        {
            // ..
        }

* **Non tutti i parametri delle rotte devono essere argomenti del controllore**

    Se, per esempio, ``last_name`` non è importante per il controllore,
    si può ometterlo del tutto::

        public function indexAction($first_name, $color)
        {
            // ..
        }

.. tip::

    Ogni rotta ha anche un parametro speciale ``_route``, che è equivalente al
    nome della rotta che è stata abbinata (ad esempio ``hello``). Anche se di solito non è
    utile, questa è ugualmente disponibile come argomento del controllore.

La ``Request`` come argomento del controllore
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Per comodità, è anche possibile far passare a Symfony l'oggetto ``Request``
come argomento al controllore. E' particolarmente utile quando si
lavora con i form, ad esempio::

    use Symfony\Component\HttpFoundation\Request;

    public function updateAction(Request $request)
    {
        $form = $this->createForm(...);
        
        $form->bindRequest($request);
        // ...
    }

.. index::
   single: Controller; Base controller class

La classe base del controllore
------------------------------

Per comodità, Symfony2 ha una classe base ``Controller`` che aiuta
nelle attività più comuni del controllore e dà alla classe controllore
l'accesso a qualsiasi risorsa che potrebbe essere necessaria. Estendendo questa classe ``Controller``,
è possibile usufruire di numerosi metodi helper.

Aggiungere la dichiarazione ``use`` sopra alla classe ``Controller`` e modificare
``HelloController`` per estenderla:

.. code-block:: php

    // src/Acme/HelloBundle/Controller/HelloController.php

    namespace Acme\HelloBundle\Controller;
    use Symfony\Bundle\FrameworkBundle\Controller\Controller;
    use Symfony\Component\HttpFoundation\Response;

    class HelloController extends Controller
    {
        public function indexAction($name)
        {
          return new Response('<html><body>Hello '.$name.'!</body></html>');
        }
    }

Questo in realtà non cambia nulla su come lavora il controllore. Nella
prossima sezione, si imparerà a conoscere i metodi helper che rende disponibili
la classe base del controllore. Questi metodi sono solo scorciatoie per usare funzionalità
del core di Symfony2 che sono a disposizione con o senza la classe
base di ``Controller``. Un ottimo modo per vedere le funzionalità core in azione
è quello di guardare nella classe
:class:`Symfony\\Bundle\\FrameworkBundle\\Controller\\Controller`
stessa.

.. tip::

    Estendere la classe base è *opzionale* in Symfony; essa contiene utili
    scorciatoie ma niente di obbligatorio. È inoltre possibile estendere
    ``Symfony\Component\DependencyInjection\ContainerAware``. L'oggetto
    service container sarà quindi accessibile tramite la proprietà ``container``.

.. note::

    È inoltre possibile definire i :doc:`Controllori come servizi
    </cookbook/controller/service>`.

.. index::
   single: Controller; Common Tasks

Attività comuni del controllore
-------------------------------

Anche se un controllore può fare praticamente qualsiasi cosa, la maggior parte dei controllori eseguiranno
gli stessi compiti di base più volte. Questi compiti, come il reindirizzamento,
l'inoltro, il rendere i template e l'accesso ai servizi core, sono molto semplici
da gestire con Symfony2.

.. index::
   single: Controller; Redirecting

Reindirizzamento
~~~~~~~~~~~~~~~~

Se si vuole reindirizzare l'utente ad un'altra pagina, usare il metodo ``redirect()``::

    public function indexAction()
    {
        return $this->redirect($this->generateUrl('homepage'));
    }

Il metodo ``generateUrl()`` è solo una funzione di supporto che genera l'URL
per una determinata rotta. Per maggiori informazioni, vedere il capitolo
:doc:`Routing </book/routing>`.

Per impostazione predefinita, il metodo ``redirect()`` esegue una redirezione 302 (temporanea). Per
eseguire una redirezione 301 (permanente), modificare il secondo argomento::

    public function indexAction()
    {
        return $this->redirect($this->generateUrl('homepage'), 301);
    }

.. tip::

    Il metodo ``redirect()`` è semplicemente una scorciatoia che crea un oggetto ``Response``
    specializzato nel reindirizzare l'utente. È equivalente a:

    .. code-block:: php

        use Symfony\Component\HttpFoundation\RedirectResponse;

        return new RedirectResponse($this->generateUrl('homepage'));

.. index::
   single: Controller; Forwarding

Inoltro
~~~~~~~~~~

Si può anche facilmente inoltrare internamente ad un altro controllore con il metodo
``forward()``. Invece di redirigere il browser dell'utente, fa una sotto richiesta interna
e chiama il controllore specificato. Il metodo ``forward()`` restituisce l'oggetto
``Response`` che è tornato da quel controllore::

    public function indexAction($name)
    {
        $response = $this->forward('AcmeHelloBundle:Hello:fancy', array(
            'name'  => $name,
            'color' => 'green'
        ));

        // modifica ulteriormente la risposta o ritorna direttamente
        
        return $response;
    }

Si noti che il metodo `forward()` utilizza la stessa rappresentazione stringa del
controllore utilizzato nella configurazione delle rotte. In questo caso, l'obiettivo
della classe del controllore sarà ``HelloController`` all'interno di un qualche ``AcmeHelloBundle``.
L'array passato al metodo diventa un insieme di argomenti sul controllore risultante.
La stessa interfaccia viene utilizzata quando si incorporano controllori nei template (vedere
:ref:`templating-embedding-controller`). L'obiettivo del metodo controllore dovrebbe
essere simile al seguente::

    public function fancyAction($name, $color)
    {
        // ... creare e restituire un oggetto Response
    }

E proprio come quando si crea un controllore per una rotta, l'ordine degli argomenti
di ``fancyAction`` non è importante. Symfony2 controlla i nomi degli indici chiave
(ad esempio ``name``) con i nomi degli argomenti del metodo (ad esempio ``$name``). Se
si modifica l'ordine degli argomenti, Symfony2 continuerà a passare il corretto
valore di ogni variabile.

.. tip::

    Come per gli altri metodi base di ``Controller``, il metodo ``forward`` è solo
    una scorciatoia per funzionalità core di Symfony2. Un inoltro può essere eseguito
    direttamente attraverso il servizio ``http_kernel``. Un inoltro restituisce un oggetto
    ``Response``::
    
        $httpKernel = $this->container->get('http_kernel');
        $response = $httpKernel->forward('AcmeHelloBundle:Hello:fancy', array(
            'name'  => $name,
            'color' => 'green',
        ));

.. index::
   single: Controller; Rendering templates

.. _controller-rendering-templates:

Rendere i template
~~~~~~~~~~~~~~~~~~

Sebbene non sia un requisito, la maggior parte dei controllori alla fine rendono un template
che è responsabile di generare il codice HTML (o un altro formato) per il controllore.
Il metodo ``renderView()`` rende un template e restituisce il suo contenuto. Il
contenuto di un template può essere usato per creare un oggetto ``Response``::

    $content = $this->renderView('AcmeHelloBundle:Hello:index.html.twig', array('name' => $name));

    return new Response($content);

Questo può anche essere fatto in un solo passaggio con con il metodo ``render()``, che
restituisce un oggetto ``Response`` contenente il contenuto di un template::

    return $this->render('AcmeHelloBundle:Hello:index.html.twig', array('name' => $name));

In entrambi i casi, verrà reso il template ``Resources/views/Hello/index.html.twig`` presente
all'interno di ``AcmeHelloBundle``.

Il motore per i template di Symfony è spiegato in dettaglio nel
capitolo :doc:`Template </book/templating>`.

.. tip::

    Il metodo ``renderView`` è una scorciatoia per utilizzare direttamente il servizio
    ``templating``. Il servizio ``templating`` può anche essere utilizzato in modo diretto::
    
        $templating = $this->get('templating');
        $content = $templating->render('AcmeHelloBundle:Hello:index.html.twig', array('name' => $name));

.. index::
   single: Controller; Accessing services

Accesso ad altri servizi
~~~~~~~~~~~~~~~~~~~~~~~~

Quando si estende la classe base del controllore, è possibile accedere a qualsiasi servizio di Symfony2
attraverso il metodo ``get()``. Di seguito ci sono alcuni servizi comuni che potrebbero essere utili::

    $request = $this->getRequest();

    $templating = $this->get('templating');

    $router = $this->get('router');

    $mailer = $this->get('mailer');

Ci sono innumerevoli altri servizi disponibili e si incoraggia a definirne
di propri. Per elencare tutti i servizi disponibili, utilizzare il comando di console
``container:debug``:

.. code-block:: bash

    php app/console container:debug

Per maggiori informazioni, vedere il capitolo :doc:`/book/service_container`.

.. index::
   single: Controller; Managing errors
   single: Controller; 404 pages

Gestire gli errori e le pagine 404
----------------------------------

Quando qualcosa non si trova, si dovrebbe utilizzare bene il protocollo HTTP e
restituire una risposta 404. Per fare questo, si lancia uno speciale tipo di eccezione.
Se si sta estendendo la classe base del controllore, procedere come segue::

    public function indexAction()
    {
        $product = // recuperare l'oggetto dal database
        if (!$product) {
            throw $this->createNotFoundException('Il prodotto non esiste');
        }

        return $this->render(...);
    }

Il metodo ``createNotFoundException()`` crea uno speciale oggetto ``NotFoundHttpException``,
che in ultima analisi innesca una risposta HTTP 404 all'interno di Symfony.

Naturalmente si è liberi di lanciare qualunque classe ``Exception`` nel controllor -
Symfony2 ritornerà automaticamente un codice di risposta HTTP 500.

.. code-block:: php

    throw new \Exception('Qualcosa è andato storto!');

In ogni caso, all'utente finale viene mostrata una pagina di errore predefinita e allo sviluppatore
vien mostrata una pagina di errore completa di debug (quando si visualizza la pagina in modalità debug).
Entrambe le pagine di errore possono essere personalizzate. Per ulteriori informazioni, leggere
nel ricettario ":doc:`/cookbook/controller/error_pages`".

.. index::
   single: Controller; The session
   single: Session

Gestione della sessione
-----------------------

Symfony2 fornisce un oggetto sessione che si può utilizzare per memorizzare le informazioni
sull'utente (che sia una persona reale che utilizza un browser, un bot, o un servizio web)
attraverso le richieste. Per impostazione predefinita, Symfony2 memorizza gli attributi in un cookie
utilizzando le sessioni PHP native.

Memorizzare e recuperare informazioni dalla sessione può essere fatto
da qualsiasi controllore::

    $session = $this->getRequest()->getSession();

    // memorizza un attributo per riutilizzarlo durante una successiva richiesta dell'utente
    $session->set('foo', 'bar');

    // in un altro controllore per un'altra richiesta
    $foo = $session->get('foo');

    // imposta il locale dell'utente
    $session->setLocale('fr');

Questi attributi rimarranno sull'utente per il resto della sessione
utente.

.. index::
   single Session; Flash messages

Messaggi flash
~~~~~~~~~~~~~~

È anche possibile memorizzare messaggi di piccole dimensioni che vengono memorizzati sulla sessione utente
solo per una richiesta successiva. Ciò è utile quando si elabora un form:
si desidera reindirizzare e avere un messaggio speciale mostrato sulla richiesta *successiva*.
Questo tipo di messaggi sono chiamati messaggi "flash".

Per esempio, immaginiamo che si stia elaborando un form inviato::

    public function updateAction()
    {
        $form = $this->createForm(...);

        $form->bindRequest($this->getRequest());
        if ($form->isValid()) {
            // fare una qualche elaborazione

            $this->get('session')->setFlash('notice', 'Le modifiche sono state salvate!');

            return $this->redirect($this->generateUrl(...));
        }

        return $this->render(...);
    }

Dopo l'elaborazione della richiesta, il controllore imposta un messaggio flash ``notice``
e poi reindirizza. Il nome (``notice``) non è significativo - è solo quello che
si utilizza per identificare il tipo del messaggio.

Nel template dell'azione successiva, il seguente codice può essere utilizzato per rendere
il messaggio ``notice``:

.. configuration-block::

    .. code-block:: html+jinja

        {% if app.session.hasFlash('notice') %}
            <div class="flash-notice">
                {{ app.session.flash('notice') }}
            </div>
        {% endif %}

    .. code-block:: php
    
        <?php if ($view['session']->hasFlash('notice')): ?>
            <div class="flash-notice">
                <?php echo $view['session']->getFlash('notice') ?>
            </div>
        <?php endif; ?>

Per come sono stati progettati, i messaggi flash sono destinati a vivere esattamente per una richiesta (hanno la
"durata di un flash"). Sono progettati per essere utilizzati in redirect esattamente come
è stato fatto in questo esempio.

.. index::
   single: Controller; Response object

L'oggetto Response
------------------

L'unico requisito per un controllore è restituire un oggetto ``Response``. La
classe :class:`Symfony\\Component\\HttpFoundation\\Response` è una astrazione PHP
sulla risposta HTTP - il messaggio testuale che contiene gli header HTTP
headers e il contenuto che viene inviato al client::

    // crea una semplice risposta con un codice di stato 200 (il predefinito)
    $response = new Response('Hello '.$name, 200);
    
    // crea una risposta JSON con un codice di stato 200
    $response = new Response(json_encode(array('name' => $name)));
    $response->headers->set('Content-Type', 'application/json');

.. tip::

    La proprietà ``headers`` è un
    oggetto :class:`Symfony\\Component\\HttpFoundation\\HeaderBag` con alcuni
    utili metodi per leggere e modificare gli header ``Response``. I
    nomi degli header sono normalizzati in modo che l'utilizzo di ``Content-Type`` è equivalente
    a ``content-type`` o anche ``content_type``.

.. index::
   single: Controller; Request object

L'oggetto Request
-----------------

Oltre ai valori dei segnaposto delle rotte, il controllore ha anche accesso
all'oggetto ``Request`` quando si estende la classe base ``Controller``::

    $request = $this->getRequest();

    $request->isXmlHttpRequest(); // è una richiesta Ajax?

    $request->getPreferredLanguage(array('en', 'fr'));

    $request->query->get('page'); // recupera un parametro $_GET

    $request->request->get('page'); // recupera un parametro $_POST

Come l'oggetto ``Response``, le intestazioni della richiesta sono memorizzate in un oggetto
``HeaderBag`` e sono facilmente accessibili.

Considerazioni finali
---------------------

Ogni volta che si crea una pagina, è necessario scrivere del codice che
contiene la logica per quella pagina. In Symfony, questo codice si chiama controllore,
ed è una funzione PHP che può fare qualsiasi cosa di cui ha bisogno per tornare
l'oggetto finale ``Response`` che verrà restituito all'utente.

Per rendere la vita più facile, si può scegliere di estendere una classe base ``Controller``,
che contiene metodi scorciatoia per molti compiti comuni del controllore. Per esempio,
dal momento che non si vuole mettere il codice HTML nel controllore, è possibile utilizzare
il metodo ``render()`` per rendere e restituire il contenuto da un template.

In altri capitoli, si vedrà come il controllore può essere usato per persistere e
recuperare oggetti da un database, processare i form inviati, gestire la cache e
altro ancora.

Imparare di più dal ricettario
------------------------------

* :doc:`/cookbook/controller/error_pages`
* :doc:`/cookbook/controller/service`
