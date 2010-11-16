.. index::
   single: Controllore
   single: MVC; Controllore

Il controllore
==============

Ancora qui, dopo le prime due parti? State diventano dei Symfony2-dipendenti!
Senza ulteriori indugi, scopriamo cosa sono in grado di fare i controllori.

.. index::
   single: Formati
   single: Controllore; Formati
   single: Rotte; Formati
   single: Vista; Formati

Usare i formati
---------------

Oggigiorno, un'applicazione web dovrebbe essere in grado di servire
più che semplici pagine HTML. Da XML per i feed RSS o per webservice,
a JSON per le richieste Ajax, ci sono molti formati diversi tra cui
scegliere. Il supporto di tali formati in Symfony2 è semplice.
Modificare il file ``routing.yml`` e aggiungere un formato
``_format``, con valore ``xml``:

.. configuration-block::

    .. code-block:: yaml

        # src/Application/HelloBundle/Resources/config/routing.yml
        hello:
            pattern:  /hello/:name
            defaults: { _controller: HelloBundle:Hello:index, _format: xml }

    .. code-block:: xml

        <!-- src/Application/HelloBundle/Resources/config/routing.xml -->
        <route id="hello" pattern="/hello/:name">
            <default key="_controller">HelloBundle:Hello:index</default>
            <default key="_format">xml</default>
        </route>

    .. code-block:: php

        // src/Application/HelloBundle/Resources/config/routing.php
        $collection->addRoute('hello', new Route('/hello/:name', array(
            '_controller' => 'HelloBundle:Hello:index',
            '_format'     => 'xml',
        )));

Quindi, aggiungere un template ``index.xml.php`` insieme a ``index.php``:

.. code-block:: xml+php

    # src/Application/HelloBundle/Resources/views/Hello/index.xml.php
    <hello>
        <name><?php echo $name ?></name>
    </hello>

È tutto. Nessun bisogno di modificare il controllore. Per i formati
standard, Symfony2 sceglierà anche il miglior header ``Content-Type``
per la risposta. Se si vogliono supportare formati diversi per una
singola azione, usare invece il segnaposto ``:_format`` nello schema:

.. configuration-block::

    .. code-block:: yaml

        # src/Application/HelloBundle/Resources/config/routing.yml
        hello:
            pattern:      /hello/:name.:_format
            defaults:     { _controller: HelloBundle:Hello:index, _format: html }
            requirements: { _format: (html|xml|json) }

    .. code-block:: xml

        <!-- src/Application/HelloBundle/Resources/config/routing.xml -->
        <route id="hello" pattern="/hello/:name.:_format">
            <default key="_controller">HelloBundle:Hello:index</default>
            <default key="_format">html</default>
            <requirement key="_format">(html|xml|json)</requirement>
        </route>

    .. code-block:: php

        // src/Application/HelloBundle/Resources/config/routing.php
        $collection->addRoute('hello', new Route('/hello/:name.:_format', array(
            '_controller' => 'HelloBundle:Hello:index',
            '_format'     => 'html',
        ), array(
            '_format' => '(html|xml|json)',
        )));

Ora il controller sarà richiamato per URL come ``/hello/Fabien.xml`` o
``/hello/Fabien.json``. Essendo ``html`` il valore predefinito per
``_format``, sia ``/hello/Fabien`` che ``/hello/Fabien.html`` corrisponderanno
al formato ``html``.

La voce ``requirements`` definisce delle espressioni regolari che i segnaposto
devono soddisfare. In questo esempio, se si prova a richiedere la risorsa
``/hello/Fabien.js``, si otterrà un errore 404, poiché essa non corrisponde al
requisito di ``_format``.

.. index::
   single: Risposta

L'oggetto risposta
------------------

Torniamo ora al controllore ``Hello``::

    // src/Application/HelloBundle/Controller/HelloController.php

    public function indexAction($name)
    {
        return $this->render('HelloBundle:Hello:index.php', array('name' => $name));
    }

Il metodo ``render()`` rende un template e restituisce un oggetto ``Response``.
La risposta può essere rimaneggiata prima che venga inviata al browser, per
esempio per cambiare il ``Content-Type`` predefinito::

    public function indexAction($name)
    {
        $response = $this->render('HelloBundle:Hello:index.php', array('name' => $name));
        $response->headers->set('Content-Type', 'text/plain');

        return $response;
    }

Per template semplici, si può anche creare a mano un oggetto ``Response``
e risparmiare alcuni millisecondi::

    public function indexAction($name)
    {
        return $this->createResponse('Hello '.$name);
    }

Ciò torna molto utile quando un controllore deve inviare una risposta JSON
per una richiesta Ajax.

.. index::
   single: Eccezioni

Gestire gli errori
------------------

Quando qualcosa non viene trovato, si dovrebbe gestire correttamente il
protocollo HTTP e restituire una risposta 404. Lo si può fare
facilmente lanciando un'eccezione HTTP::

    use Symfony\Component\HttpKernel\Exception\NotFoundHttpException;

    public function indexAction()
    {
        $product = // retrieve the object from database
        if (!$product) {
            throw new NotFoundHttpException('The product does not exist.');
        }

        return $this->render(...);
    }

L'eccezione ``NotFoundHttpException`` restituirà un codice HTTP 404 al
browser. Similmente, ``ForbiddenHttpException`` restituisce un errore 403 e
``UnauthorizedHttpException`` un errore 401. Per altri codici di errore
HTTP, si può usare l'eccezione di base ``HttpException``, passando il codice:

    throw new HttpException('Unauthorized access.', 401);

.. index::
   single: Controllore; Redirezione
   single: Controllore; Inoltro

Redirezione e inoltro
---------------------

Se si vuole redirezionare l'utente a un'altra pagine, usare il metodo
``redirect()``::

    $this->redirect($this->generateUrl('hello', array('name' => 'Lucas')));

Il metodo ``generateUrl()`` è lo stesso del metodo ``generate()``
usato in precedenza nell'helper ``router``. Accetta come parametri un
nome di rotta e un array di parametri e restituisce l'URL associato.

Si può anche inoltrare facilmente l'azione verso un'altra, col metodo
``forward()``. Come per l'helper ``actions``, esegue una sotto-richiesta
interna, ma restituisce l'oggetto ``Response``, per consentire
eventuali altre modifiche::

    $response = $this->forward('HelloBundle:Hello:fancy', array('name' => $name, 'color' => 'green'));

    // fa qualcosa con la risposta o la restituisce direttamente

.. index::
   single: Richiesta

L'oggetto richiesta
-------------------

Accanto ai valori dei segnaposto delle rotte, il controllore ha anche
accesso all'oggetto ``Request``::

    $request = $this->get('request');

    $request->isXmlHttpRequest(); // è una richiesta Ajax?

    $request->getPreferredLanguage(array('en', 'it'));

    $request->query->get('page'); // prende un parametro da $_GET

    $request->request->get('page'); // prende un parametro da $_POST

In un template, si può anche accedere all'oggetto ``Request`` tramite
l'helper ``request``::

.. code-block:: html+php

    <?php echo $view['request']->getParameter('page') ?>

La sessione
-----------

Anche se il protocollo HTTP non ha stati, Symfony2 fornisce un oggetto
sessione, che rappresenta il client (sia esso una persona reale che usa
un browser, un bot o un webservice). Tra due richieste, Symfony2 memorizza
gli attributi in un cookie, usando le sessioni native di PHP.

La memorizzazione e il recupero delle informazioni dalla sessione
possono essere facilmente eseguite da ogni controllore::

    $session = $this->get('request')->getSession();

    // memorizza un attributo per riusarlo più tardi
    $session->set('foo', 'bar');

    // in un altro controllore per un'altra richiesta
    $foo = $session->get('foo');

    // imposta la localizzazione dell'utente
    $session->setLocale('it');

Si possono anche memorizzare brevi messaggi, che saranno disponibili
solo per la richiesta immediatamente successiva::

    // memorizza un messaggio per la richiesta successiva (in un controllore)
    $session->setFlash('notice', 'Congratulations, your action succeeded!');

    // mostra il messaggio nella richiesta successiva (in un template)
    <?php echo $view['session']->getFlash('notice') ?>

Considerazioni finali
---------------------

È tutto, e forse non abbiamo nemmeno speso tutti e dieci i minuti previsti.
Nella parte precedente, abbiamo visto come estendere il sistema dei
template con gli helper. Ma ogni cosa in Symfony2 può essere estesa o
sostituita, con i bundle. Questo è l'argomento della prossima parte di
questa guida.
