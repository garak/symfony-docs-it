Il controllore
==============

Ancora qui, dopo le prime due parti? State diventando dei Symfony-dipendenti!
Senza ulteriori indugi, scopriamo cosa sono in grado di fare i controllori.

Usare i formati
---------------

Oggigiorno, un'applicazione web dovrebbe essere in grado di servire
più che semplici pagine HTML. Da XML per i feed RSS o per web service,
a JSON per le richieste Ajax, ci sono molti formati diversi tra cui
scegliere. Il supporto di tali formati in Symfony è semplice.
Modificare il file ``routing.yml`` e aggiungere un formato ``_format``, con valore ``xml``::

    // src/Acme/DemoBundle/Controller/DemoController.php
    use Sensio\Bundle\FrameworkExtraBundle\Configuration\Route;
    use Sensio\Bundle\FrameworkExtraBundle\Configuration\Template;

    // ...

    /**
     * @Route("/hello/{name}", defaults={"_format"="xml"}, name="_demo_hello")
     * @Template()
     */
    public function helloAction($name)
    {
        return array('name' => $name);
    }

Usando il formato di richiesta (come definito nel valore ``_format``), Symfony
sceglie automaticamente il template giusto, in questo caso ``hello.xml.twig``:

.. code-block:: xml+php

    <!-- src/Acme/DemoBundle/Resources/views/Demo/hello.xml.twig -->
    <hello>
        <name>{{ name }}</name>
    </hello>

È tutto. Per i formati standard, Symfony sceglierà anche l'header ``Content-Type``
migliore per la risposta. Se si vogliono supportare diversi formati per una
singola azione, usare invece il segnaposto ``{_format}`` nello schema della
rotta::

    // src/Acme/DemoBundle/Controller/DemoController.php
    use Sensio\Bundle\FrameworkExtraBundle\Configuration\Route;
    use Sensio\Bundle\FrameworkExtraBundle\Configuration\Template;

    // ...

    /**
     * @Route(
     *     "/hello/{name}.{_format}",
     *     defaults = { "_format" = "html" },
     *     requirements = { "_format" = "html|xml|json" },
     *     name = "_demo_hello"
     * )
     * @Template()
     */
    public function helloAction($name)
    {
        return array('name' => $name);
    }

Ora il controllore sarà richiamato per URL come ``/demo/hello/Fabien.xml`` o
``/demo/hello/Fabien.json``.

La voce ``requirements`` definisce delle espressioni regolari che i segnaposto
devono soddisfare. In questo esempio, se si prova a richiedere la risorsa
``/demo/hello/Fabien.js``, si otterrà un errore 404, poiché essa non corrisponde al
requisito di ``_format``.

Rinvii e rimandi
----------------

Se si vuole rinviare l'utente a un'altra pagina, usare il metodo
``redirect()``::

    return $this->redirect($this->generateUrl('_demo_hello', array('name' => 'Lucas')));

Il metodo ``generateUrl()`` è lo stesso della funzione ``path()`` che abbiamo usato nei
template. Accetta come parametri il nome della rotta e un array di parametri e restituisce
l'URL amichevole associato.

Si può anche rimandare internamente l'azione a un'altra, col metodo
``forward()``::

    return $this->forward('AcmeDemoBundle:Hello:fancy', array(
        'name'  => $name,
        'color' => 'green'
    ));

Mostrare pagine di errore
-------------------------

Inevitabilmente, accadono degli errori durante l'esecuzione di un'applicazione.
In caso di errori ``404``, Symfony include una comoda scorciatoia da usare
nei controllori::

    throw $this->createNotFoundException();

Per gli errori ``500``, basta sollevare una normale eccezione PHP all'interno del controllore,
Symfony la trasformerà in una pagina di errore ``500``::

    throw new \Exception('Qualcosa è andato storto!');

Ottenere informazioni dalla richiesta
-------------------------------------

Symfony inietta l'oggetto ``Request`` nel controllolre, se
una variabile è forzata a ``Symfony\Component\HttpFoundation\Request``::

    use Symfony\Component\HttpFoundation\Request;

    public function indexAction(Request $request)
    {
        $request->isXmlHttpRequest(); // è una richiesta Ajax?

        $request->getPreferredLanguage(array('en', 'fr'));

        $request->query->get('page');   // prende un parametro $_GET

        $request->request->get('page'); // prende un parametro $_POST
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
        $session->set('foo', 'bar');

        // in un altro controllore per un'altra richiesta
        $foo = $session->get('foo');

        // usa una valore predefinito se la chiave non esiste
        $filters = $session->get('filters', array());
    }

Si possono anche memorizzare piccoli messaggi che saranno disponibili solo per
la richiesta successiva. Sono utili quando occorre impostare un messaggio prima di rimandare
l'utente a un'altra pagina (che mostrerà il messaggio)::

    // memorizza un messaggio per la richiesta successiva (in un controllore)
    $session->getFlashBag()->set('notice', 'Congratulazioni, azione eseguita con successo!');

.. code-block:: html+jinja

    {# mostra il messaggio nella richiesta successiva (in un template) #}
    <div>{{ app.session.flashbag.get('notice') }}</div>

Mettere in cache le risorse
---------------------------

Non appena il sito inizia a generare più traffico, si vorrà evitare di
dover generare la stessa risorsa più volte. Symfony usa gli header di cache
HTTP per gestire la cache delle risorse. Per semplici strategie di cache, si può
usare l'annotazione ``@Cache()``::

    use Sensio\Bundle\FrameworkExtraBundle\Configuration\Route;
    use Sensio\Bundle\FrameworkExtraBundle\Configuration\Template;
    use Sensio\Bundle\FrameworkExtraBundle\Configuration\Cache;

    /**
     * @Route("/hello/{name}", name="_demo_hello")
     * @Template()
     * @Cache(maxage="86400")
     */
    public function helloAction($name)
    {
        return array('name' => $name);
    }

In questo esempio, la risorsa sarà in cache per un giorno (``86400`` secondi).
La cache delle risorse è gestita dal reverse proxy predefinito di Symfony. Ma, poiché la
cache è gestita usando i normali header di cache di HTTP, è possibile rimpiazzare il
reverse proxy predefinito con Varnish o Squid e far scalare facilmente un'applicazione.

Considerazioni finali
---------------------

È tutto, e forse non abbiamo nemmeno speso tutti e dieci i minuti previsti.
Nella prima parte abbiamo introdotto brevemente i bundle e tutte le caratteristiche
apprese finora fanno parte del bundle del nucleo del framework. Ma, grazie ai bundle,
ogni cosa in Symfony può essere estesa o sostituita. Questo è l'argomento della
:doc:`prossima parte di questa guida<the_architecture>`.
