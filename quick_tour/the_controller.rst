Il controllore
==============

Ancora qui, dopo le prime due parti? State diventando dei Symfony-dipendenti!
Senza ulteriori indugi, scopriamo cosa sono in grado di fare i controllori.

Risposte grezze
---------------

Symfony si definisce un framework Richiesta-Risposta. Quando l'utente fa una
richiesta all'applicazione, Symfony crea un oggetto ``Request``, per incapsulare
tutte le informazioni collegata a tale richiesta. Similmente, il risultato dell'esecuzione
di un'azione di un controllore è la creazione di un oggetto ``Response``, usato da
Symfony per generare contenuto HTML poi restituito
all'utente.

Finora, tutte le azioni mostrare in questa guida hanno usato la scorciatoia ``$this->render()``
per rendere una risposta come risultato. In caso di necessità, si può anche
creare un oggetto ``Response`` grezzo, per restituire un qualsiasi contenuto testuale::

    // src/AppBundle/Controller/DefaultController.php
    namespace AppBundle\Controller;

    use Sensio\Bundle\FrameworkExtraBundle\Configuration\Route;
    use Symfony\Bundle\FrameworkBundle\Controller\Controller;
    use Symfony\Component\HttpFoundation\Response;

    class DefaultController extends Controller
    {
        /**
         * @Route("/", name="homepage")
         */
        public function indexAction()
        {
            return new Response('Benvenuto in Symfony!');
        }
    }

Parametri delle rotte
---------------------

La maggior parte delle volte, gli URL di un'applicazione includono parti variabili. Se per
esempio si crea un'applicazione per un blog, l'URL per mostrare gli articoli dovrebbe
includere il titolo o altre parti identificative univoche, per far sapere all'applicazione
cosa mostrare esattamente.

Nelle applicazioni Symfony, le parti variabili delle rotte sono racchiuse tra parentesi
graffe (p.e. ``/blog/read/{article_title}/``). A ciascuna parte viene assegnato un
nome univoco, che può essere usato successivamente nel controllore, per recuperare
ciascun valore.

Creare una nuova azione con variabili di rotta, per vedere in azione questa caratteristica.
Aprire il file ``src/AppBundle/Controller/DefaultController.php`` e aggiungere un
metodo chiamato ``helloAction``, con il seguente contenuto::

    // src/AppBundle/Controller/DefaultController.php
    namespace AppBundle\Controller;

    use Sensio\Bundle\FrameworkExtraBundle\Configuration\Route;
    use Symfony\Bundle\FrameworkBundle\Controller\Controller;

    class DefaultController extends Controller
    {
        // ...

        /**
         * @Route("/hello/{name}", name="hello")
         */
        public function helloAction($name)
        {
            return $this->render('default/hello.html.twig', array(
                'name' => $name
            ));
        }
    }

Aprire un browser e andare all'URL ``http://localhost:8000/hello/fabien`` per
vedere il risultato dell'esecuzione di questa nuova a zione. Invece del risultato dell'azione,
si vedrà una pagina di errore. La causa di questo errore è che si sta cercando
di rendere un template
(``default/hello.html.twig``) che ancora non esiste.

Creare il template ``app/Resources/views/default/hello.html.twig``, con il
seguente contenuto:

.. code-block:: html+jinja

    {# app/Resources/views/default/hello.html.twig #}
    {% extends 'base.html.twig' %}

    {% block body %}
        <h1>Ciao {{ name }}! Benvenuto in Symfony!</h1>
    {% endblock %}

Andare di nuovo sull'URL ``http://localhost:8000/hello/fabien`` e si vedrà il
nuovo template reso, con le informazioni passate dal controllore. Se si
cambia l'ultima parte dell'URL (p.e.
``http://localhost:8000/hello/thomas``) e si ricarica la pagina, si vedrà
un messaggio diverso. Inoltre, se si rimuove l'ultima parte dell'URL
(p.e. ``http://localhost:8000/hello``), Symfony mostrerà un errore,
perché la rotta si aspetta un nome, che non è stato fornito.

Usare i formati
---------------

Oggigiorno, un'applicazione web dovrebbe essere in grado di servire più che semplici
pagine HTML. Da XML per i feed RSS o per servizi web, a JSON per le richieste Ajax,
ci sono molti formati diversi tra cui scegliere. Il supporto di tali formati
in Symfony è semplice, grazie a una speciale variabile chiamata ``_format``,
che memorizza il formato richiesto dall'utente.

Modificare la rotta ``hello``, aggiungendo una variabile ``_format``, con ``html``
come valore predefinito::

    // src/AppBundle/Controller/DefaultController.php
    use Sensio\Bundle\FrameworkExtraBundle\Configuration\Route;
    use Sensio\Bundle\FrameworkExtraBundle\Configuration\Template;

    // ...

    /**
     * @Route("/hello/{name}.{_format}", defaults={"_format"="html"}, name="hello")
     */
    public function helloAction($name, $_format)
    {
        return $this->render('default/hello.'.$_format.'.twig', array(
            'name' => $name
        ));
    }

Usando il formato di richiesta (come definito nel valore ``_format``), Symfony
sceglie automaticamente il template giusto, in questo caso
``hello.xml.twig``:

.. code-block:: xml+php

    <!-- app/Resources/views/default/hello.xml.twig -->
    <hello>
        <name>{{ name }}</name>
    </hello>

Ora, aprendo ``http://localhost:8000/hello/fabien``, si vedrà la normale
pagina HTML, perché ``html`` è il formato predefinito. Quando si apre
``http://localhost:8000/hello/fabien.html``, si vedrà ancora la pagina HTML, stavolta
perché è stato richiesto esplicitamente il forato ``html``. Infine, aprendo
``http://localhost:8000/hello/fabien.xml``, si vedrà il nuovo template XML reso
nel browser.

È tutto. Per i formati standard, Symfony sceglierà anche l'header ``Content-Type``
migliore per la risposta. Se si vogliono supportare diversi formati per una
singola azione, usare invece il segnaposto ``{_format}`` nello schema della
rotta::

    // src/AppBundle/Controller/DefaultController.php
    use Sensio\Bundle\FrameworkExtraBundle\Configuration\Route;
    use Sensio\Bundle\FrameworkExtraBundle\Configuration\Template;

    // ...

    /**
     * @Route("/hello/{name}.{_format}",
     *     defaults = {"_format"="html"},
     *     requirements = { "_format" = "html|xml|json" },
     *     name = "hello"
     * )
     */
    public function helloAction($name, $_format)
    {
        return $this->render('default/hello.'.$_format.'.twig', array(
            'name' => $name
        ));
    }

L'azione ``hello`` ora corrisponderà a URL come ``/hello/fabien.xml`` o
``/hello/fabien.json``, ma andrà ancora in errore per URL
come ``/hello/fabien.js``, poiché il valore della variabile ``_format`` non
soddisfa i requisiti.

.. _redirecting-and-forwarding:

Rinvii e rimandi
----------------

Se si vuole rinviare l'utente a un'altra pagina, usare il metodo
``redirectToRoute()``::

    // src/AppBundle/Controller/DefaultController.php
    class DefaultController extends Controller
    {
        /**
         * @Route("/", name="homepage")
         */
        public function indexAction()
        {
            return $this->redirectToRoute('hello', array('name' => 'Fabien'));
        }
    }

Il metodo ``redirectToRoute()`` accetta come parametri il nome della rotta e un array
opzionale di parametri e rinvia l'utente all'URL generato con
tali parametri.

Mostrare pagine di errore
-------------------------

Inevitabilmente, accadono degli errori durante l'esecuzione di un'applicazione.
In caso di errori ``404``, Symfony include una comoda scorciatoia da usare
nei controllori::

    // src/AppBundle/Controller/DefaultController.php
    // ...

    class DefaultController extends Controller
    {
        /**
         * @Route("/", name="homepage")
         */
        public function indexAction()
        {
            // ...
            throw $this->createNotFoundException();
        }
    }

Per gli errori ``500``, basta sollevare una normale eccezione PHP all'interno del controllore,
Symfony la trasformerà in una pagina di errore ``500``::

    // src/AppBundle/Controller/DefaultController.php
    // ...

    class DefaultController extends Controller
    {
        /**
         * @Route("/", name="homepage")
         */
        public function indexAction()
        {
            // ...
            throw new \Exception('Qualcosa è andato storto!');
        }
    }

Ottenere informazioni dalla richiesta
-------------------------------------

A  volte, un controllore ha bisogno di accedere a informazioni correlate alla
richiesta, come il linguaggio preferito, l'indirizzo IP o i parametri dell'URL.
Per accedere a tali informazioni, aggiungere un parametro di tipo ``Request``
all'azione. Il nome di tale parametro non ha importanza, ma deve essere preceduto
dal tipo ``Request`` per poter funzionare (non dimenticare di aggiungere un'istruzione ``use``
per importare la classe ``Request``)::

    // src/AppBundle/Controller/DefaultController.php
    namespace AppBundle\Controller;

    use Sensio\Bundle\FrameworkExtraBundle\Configuration\Route;
    use Symfony\Bundle\FrameworkBundle\Controller\Controller;
    use Symfony\Component\HttpFoundation\Request;

    class DefaultController extends Controller
    {
        /**
         * @Route("/", name="homepage")
         */
        public function indexAction(Request $request)
        {
            // è una richiesta Ajax?
            $isAjax = $request->isXmlHttpRequest();

            // quale linguaggio preferisce l'utente?
            $language = $request->getPreferredLanguage(array('en', 'fr'));

            // ottenere il valore di un parametro $_GET
            $pageName = $request->query->get('page');

            // ottenere il valore di un parametro $_POST
            $pageName = $request->request->get('page');
        }
    }

In un template, si può anche avere accesso all'oggetto ``Request`` tramite la
variabile ``app.request``:

.. code-block:: html+jinja

    {{ app.request.query.get('page') }}

    {{ app.request.parameter('page') }}

Persistere i dati nella sessione
--------------------------------

Anche se il protocollo HTTP non ha stato, Symfony fornisce un bell'oggetto sessione,
che rappresenta il client (sia esso una persona che usa un browser, un bot o un servizio
web). Tra due richieste, Symfony memorizza gli attributi in un cookie, usando
le sessioni native di PHP.

Si possono memorizzare e recuperare informazioni dalla sessione in modo facile, da
un qualsiasi controllore::

    use Symfony\Component\HttpFoundation\Request;

    public function indexAction(Request $request)
    {
        $session = $request->getSession();

        // memorizza un attributo per riusarlo più avanti durante una richiesta utente
        $session->set('pippo', 'pluto');

        // in un altro controllore per un'altra richiesta
        $pippo = $session->get('pippo');

        // usa una valore predefinito se la chiave non esiste
        $pippo = $session->get('pippo', 'valore_predefinito');
    }

Si possono anche memorizzare piccoli messaggi che saranno disponibili solo per
la richiesta successiva. Sono utili quando occorre impostare un messaggio prima di rimandare
l'utente a un'altra pagina (che mostrerà il messaggio)::

    public function indexAction(Request $request)
    {
        // ...

        // memorizza un messaggio per la richiesta successiva
        $this->addFlash('notice', 'Congratulazioni, azione eseguita con successo!');
    }

E si può mostrare il messaggio nella richiesta successiva in un template, così:

.. code-block:: html+jinja

    <div>
        {{ app.session.flashbag.get('notice') }}
    </div>

Considerazioni finali
---------------------

È tutto, e forse non abbiamo nemmeno speso tutti e dieci i minuti previsti.
Nella prima parte abbiamo introdotto brevemente i bundle e tutte le caratteristiche
apprese finora fanno parte del bundle del nucleo del framework. Ma, grazie ai bundle,
ogni cosa in Symfony può essere estesa o sostituita. Questo è l'argomento della
:doc:`prossima parte di questa guida<the_architecture>`.
