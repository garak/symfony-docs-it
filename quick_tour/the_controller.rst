Il controllore
==============

Ancora qui, dopo le prime due parti? State diventano dei Symfony2-dipendenti!
Senza ulteriori indugi, scopriamo cosa sono in grado di fare i controllori.

Usare i formati
---------------

Oggigiorno, un'applicazione web dovrebbe essere in grado di servire
più che semplici pagine HTML. Da XML per i feed RSS o per web service,
a JSON per le richieste Ajax, ci sono molti formati diversi tra cui
scegliere. Il supporto di tali formati in Symfony2 è semplice.
Modificare il file ``routing.yml`` e aggiungere un formato ``_format``, con valore ``xml``::

    // src/Acme/DemoBundle/Controller/DemoController.php
    use Sensio\Bundle\FrameworkExtraBundle\Configuration\Route;
    use Sensio\Bundle\FrameworkExtraBundle\Configuration\Template;

    /**
     * @Route("/hello/{name}", defaults={"_format"="xml"}, name="_demo_hello")
     * @Template()
     */
    public function helloAction($name)
    {
        return array('name' => $name);
    }

Usando il formato di richiesta (come definito nel valore ``_format``), Symfony2
sceglie automaticamente il template giusto, in questo caso ``hello.xml.twig``:

.. code-block:: xml+php

    <!-- src/Acme/DemoBundle/Resources/views/Demo/hello.xml.twig -->
    <hello>
        <name>{{ name }}</name>
    </hello>

È tutto. Per i formati standard, Symfony2 sceglierà anche l'header ``Content-Type``
migliore per la risposta. Se si vogliono supportare diversi formati per una
singola azione, usare invece il segnaposto ``{_format}`` nello schema della
rotta::

    // src/Acme/DemoBundle/Controller/DemoController.php
    use Sensio\Bundle\FrameworkExtraBundle\Configuration\Route;
    use Sensio\Bundle\FrameworkExtraBundle\Configuration\Template;

    /**
     * @Route("/hello/{name}.{_format}", defaults={"_format"="html"}, requirements={"_format"="html|xml|json"}, name="_demo_hello")
     * @Template()
     */
    public function helloAction($name)
    {
        return array('name' => $name);
    }

Ora il controller sarà richiamato per URL come ``/demo/hello/Fabien.xml`` o
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

Si può anche facilmente rimandare l'azione a un'altra, col metodo ``forward()``.
Internamente, Symfony effettua una "sotto-richiesta" e restituisce un oggetto ``Response``
da tale sotto-richiesta::

    $response = $this->forward('AcmeDemoBundle:Hello:fancy', array('name' => $name, 'color' => 'green'));

    // fare qualcosa con la risposta o restituirla direttamente

Ottenere informazioni dalla richiesta
-------------------------------------

Oltre ai valori dei segnaposto delle rotte, il controllore ha anche accesso
all'oggetto ``Request``::

    $request = $this->getRequest();

    $request->isXmlHttpRequest(); // è una richiesta Ajax?

    $request->getPreferredLanguage(array('en', 'fr'));

    $request->query->get('page'); // prende un parametro $_GET

    $request->request->get('page'); // prende un parametro $_POST

In un template, si può anche avere accesso all'oggetto ``Request`` tramite la
variabile ``app.request``:

.. code-block:: html+jinja

    {{ app.request.query.get('page') }}

    {{ app.request.parameter('page') }}

Persistere i dati nella sessione
--------------------------------

Anche se il protocollo HTTP non ha stato, Symfony2 fornisce un bell'oggetto sessione,
che rappresenta il client (sia esso una persona che usa un browser, un bot o un servizio
web). Tra due richieste, Symfony2 memorizza gli attributi in un cookie, usando
le sessioni native di PHP.

Si possono memorizzare e recuperare informazioni dalla sessione in modo facile, da
un qualsiasi controllore::

    $session = $this->getRequest()->getSession();

    // memorizza un attributo per riusarlo più avanti durante una richiesta utente
    $session->set('foo', 'bar');

    // in un altro controllore per un'altra richiesta
    $foo = $session->get('foo');

    // imposta la localizzazione dell'utente
    $session->setLocale('fr');

Si possono anche memorizzare piccoli messaggi che saranno disponibili solo per
la richiesta successiva::

    // memorizza un messaggio per la richiesta successiva (in un controllore)
    $session->setFlash('notice', 'Congratulazioni, azione eseguita con successo!');

    // mostra il messaggio nella richiesta successiva (in un template)
    {{ app.session.flash('notice') }}

Ciò risulta utile quando occorre impostare un messaggio di successo, prima di rinviare
l'utente a un'altra pagina (la quale mostrerà il messaggio).

Proteggere le risorse
---------------------

La Standard Edition di Symfony possiede una semplice configurazione di sicurezza, che
soddisfa i bisogni più comuni:

.. code-block:: yaml

    # app/config/security.yml
    security:
        encoders:
            Symfony\Component\Security\Core\User\User: plaintext

        role_hierarchy:
            ROLE_ADMIN:       ROLE_USER
            ROLE_SUPER_ADMIN: [ROLE_USER, ROLE_ADMIN, ROLE_ALLOWED_TO_SWITCH]

        providers:
            in_memory:
                users:
                    user:  { password: userpass, roles: [ 'ROLE_USER' ] }
                    admin: { password: adminpass, roles: [ 'ROLE_ADMIN' ] }

        firewalls:
            dev:
                pattern:  ^/(_(profiler|wdt)|css|images|js)/
                security: false

            login:
                pattern:  ^/demo/secured/login$
                security: false

            secured_area:
                pattern:    ^/demo/secured/
                form_login:
                    check_path: /demo/secured/login_check
                    login_path: /demo/secured/login
                logout:
                    path:   /demo/secured/logout
                    target: /demo/

Questa configurazione richiede agli utenti di effettuare login per ogni URL che inizi
per ``/demo/secured/`` e definisce due utenti validi: ``user`` e ``admin``.
Inoltre, l'utente ``admin`` ha il ruolo ``ROLE_ADMIN``, che include il ruolo
``ROLE_USER`` (si veda l'impostazione ``role_hierarchy``).

.. tip::

    Per leggibilità, le password sono memorizzate in chiaro in questa semplice
    configurazione, ma si può usare un qualsiasi algoritmo di hash, modificando
    la sezione ``encoders``.

Andando all'URL ``http://localhost/Symfony/web/app_dev.php/demo/secured/hello``,
si verrà automaticamente rinviati al form di login, perché questa risorsa è
protetta da un ``firewall``.

Si può anche forzare l'azione a richiedere un dato ruolo, usando l'annotazione
``@Secure`` nel controllore::

    use Sensio\Bundle\FrameworkExtraBundle\Configuration\Route;
    use Sensio\Bundle\FrameworkExtraBundle\Configuration\Template;
    use JMS\SecurityExtraBundle\Annotation\Secure;

    /**
     * @Route("/hello/admin/{name}", name="_demo_secured_hello_admin")
     * @Secure(roles="ROLE_ADMIN")
     * @Template()
     */
    public function helloAdminAction($name)
    {
        return array('name' => $name);
    }

Ora, si entri come utente ``user`` (che *non* ha il ruolo ``ROLE_ADMIN``) e,
dalla pagina sicura "hello", si clicchi sul collegamento "Hello resource secured".
Symfony2 dovrebbe restituire un codice di stato HTTP 403 ("forbidden"), indicando che
l'utente non è autorizzato ad accedere a tale risorsa.

.. note::

    Il livello di sicurezza di Symfony2 è molto flessibile e fornisce diversi provider
    per gli utenti (come quello per l'ORM Doctrine) e provider di autenticazione
    (come HTTP basic, HTTP digest o certificati X509). Si legga il capitolo
    ":doc:`/book/security`" del libro per maggiori informazioni su come
    usarli e configurarli.

Mettere in cache le risorse
---------------------------

Non appena il proprio sito inizia a generare più traffico, si vorrà evitare di
dover generare la stessa risorsa più volte. Symfony2 usa gli header di cache
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

In questo esempio, la risorsa sarà in cache per un giorno. Ma si può anche usare
la validazione invece della scadenza o una combinazione di entrambe, se questo
soddisfa meglio le proprie esigenze.

La cache delle risorse è gestita dal reverse proxy predefinito di Symfony2. Ma poiché la
cache è gestita usando i normali header di cache di HTTP, è possibile rimpiazzare il
reverse proxy predefinito con Varnish o Squid e scalare facilmente la propria applicazione.

.. note::

    E se non si volesse mettere in cache l'intera pagina? Symfony2 ha una soluzione,
    tramite Edge Side Includes (ESI), supportate nativamente. Si possono avere
    maggiori informazioni nel capitolo ":doc:`/book/http_cache`" del libro.

Considerazioni finali
---------------------

È tutto, e forse non abbiamo nemmeno speso tutti e dieci i minuti previsti.
Nella prima parte abbiamo introdotto brevemente i bundle e tutte le caratteristiche
apprese finora fanno parte del bundle del nucleo del framework. Ma, grazie ai bundle,
ogni cosa in Symfony2 può essere estesa o sostituita. Questo è l'argomento della
:doc:`prossima parte di questa guida<the_architecture>`.