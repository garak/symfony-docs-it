.. index::
   single: Controllore; Come servizi

Definire i controllori come servizi
===================================

Nel libro, si è visto quanto sia facile usare un controllore che estenda la
classe base :class:`Symfony\\Bundle\\FrameworkBundle\\Controller\\Controller`. Oltre
a questo metodo, i controllori possono anche essere specificati come
servizi.

.. note::

    Specificare un controllore come servizio richiede un po' di lavoro in più. Il
    vantaggio principale è che l'intero controllore o qualsiasi servizio a esso
    passato può essere modificato tramite la configurazione del contenitore dei servizi.
    Questo risulta particolarmente utile nello sviluppo di un bundle open source o di
    un bundle da usare in progetti diversi.

    Un secondo vantaggio è che i controllori sono più isolati. Guardando
    i parametri del costruttore, si può vedere facilmente che tipo di cose
    tale controllore possa o non possa fare. Poiché ogni dipendenza deve essere
    iniettata a mano, è più ovvio (cioè se si hanno molti parametri del
    costruttore) quando il controllore diventa troppo grosso. La raccomandazione delle
    the :doc:`best practice </best_practices/controllers>` è valida anche per i
    controllori definiti come servizi: evitare di inserire logica di business nei
    controllori. Iniettare invece servizi e far svolgere loro il grosso del lavoro.

    Quindi, anche se non si specificano i controllori come servizi, probabilmente lo
    si vedrà fatto in qualche bundle open source di Symfony. È anche importante
    capire i pro e i contro di entrambi gli approcci.

Definire il controllore come servizio
-------------------------------------

Un controllore può essere definito come servizio nello stesso modo di altre classi.
Per esempio, se si ha il seguente controllore::

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

Lo si può definire come servizio in questo modo:

.. configuration-block::

    .. code-block:: yaml

        # app/config/services.yml
        services:
            app.hello_controller:
                class: AppBundle\Controller\HelloController

    .. code-block:: xml

        <!-- app/config/services.xml -->
        <services>
            <service id="app.hello_controller" class="AppBundle\Controller\HelloController" />
        </services>

    .. code-block:: php

        // app/config/services.php
        use Symfony\Component\DependencyInjection\Definition;

        $container->setDefinition('app.hello_controller', new Definition(
            'AppBundle\Controller\HelloController'
        ));

Fare riferimento al servizio
----------------------------

Per fare riferimento a un controllore definito come servizio, usare la notazione con due punti (:).
Per esempio, per rinviare al metodo ``indexAction()`` del servizio
definito sopra con id ``app.hello.controller``::

    $this->forward('app.hello_controller:indexAction', array('name' => $name));

.. note::

    Non si può omettere la parte ``Action`` del metodo, quando si usa questa
    sintassi.

Ci si può anche riferire al servizio usando la stessa notazione, definendo
il valore ``_controller`` di una rotta:

.. configuration-block::

    .. code-block:: yaml

        # app/config/routing.yml
        hello:
            path:     /hello
            defaults: { _controller: app.hello_controller:indexAction }

    .. code-block:: xml

        <!-- app/config/routing.xml -->
        <route id="hello" path="/hello">
            <default key="_controller">app.hello_controller:indexAction</default>
        </route>

    .. code-block:: php

        // app/config/routing.php
        $collection->add('hello', new Route('/hello', array(
            '_controller' => 'app.hello_controller:indexAction',
        )));

.. tip::

    Si possono anche usare annotazioni per configurare le rotte in un controllore
    definito come servizio. Vedere la `documentazione di FrameworkExtraBundle`_
    per maggiori dettagli.

Alternative ai metodi del controllore base
------------------------------------------

Quando si usa un controllore definito come servizio, probabilmente non si estenderà
la classe base ``Controller``. Invece di appoggiarsi ai metodi scorciatoia,
si interagirà direttamente con i servizi necessari. Per fortuna, farlo è alquanto
facile e il `codice della classe Controller`_ fornisce un ottimo spunto
su come eseguire compiti comuni.

Per esempio, se si vuole rendere un template invece di creare direttamente l'oggetto ``Response``,
il codice somiglierà a questo, se si estende
il controllore base di Symfony::

    // src/AppBundle/Controller/HelloController.php
    namespace AppBundle\Controller;

    use Symfony\Bundle\FrameworkBundle\Controller\Controller;

    class HelloController extends Controller
    {
        public function indexAction($name)
        {
            return $this->render(
                'AppBundle:Hello:index.html.twig',
                array('name' => $name)
            );
        }
    }

Se si guarda il codice della funzione ``render`` nella
`classe Controller base`_ di Symfony, si vedrà che tale metodo in realtà usa il
servizio ``templating``::

    public function render($view, array $parameters = array(), Response $response = null)
    {
        return $this->container->get('templating')->renderResponse($view, $parameters, $response);
    }

In un controllore definito come servizio, si può invece iniettare il servizio ``templating``
e usarlo direttamente::

    // src/AppBundle/Controller/HelloController.php
    namespace AppBundle\Controller;

    use Symfony\Bundle\FrameworkBundle\Templating\EngineInterface;
    use Symfony\Component\HttpFoundation\Response;

    class HelloController
    {
        private $templating;

        public function __construct(EngineInterface $templating)
        {
            $this->templating = $templating;
        }

        public function indexAction($name)
        {
            return $this->templating->renderResponse(
                'AppBundle:Hello:index.html.twig',
                array('name' => $name)
            );
        }
    }

La definizione del servizio va modificata, per specificare il parametro
del costruttore:

.. configuration-block::

    .. code-block:: yaml

        # app/config/services.yml
        services:
            app.hello_controller:
                class:     AppBundle\Controller\HelloController
                arguments: ["@templating"]

    .. code-block:: xml

        <!-- app/config/services.xml -->
        <services>
            <service id="app.hello_controller" class="AppBundle\Controller\HelloController">
                <argument type="service" id="templating"/>
            </service>
        </services>

    .. code-block:: php

        // app/config/services.php
        use Symfony\Component\DependencyInjection\Definition;
        use Symfony\Component\DependencyInjection\Reference;

        $container->setDefinition('app.hello_controller', new Definition(
            'AppBundle\Controller\HelloController',
            array(new Reference('templating'))
        ));

Invece che recuperare il servizio ``templating`` dal contenitore, si può
iniettare *solamente* il servizio o i servizi necessari direttamente nel controllore.

.. note::

   Questo non vuol dire che non si possano estendere questi controllori da un proprio
   controllore base. La rinuncia al controllore base standard è dovuta al fatto che
   i metodi aiutanti si appoggiano al contenitore disponibile, che non è il caso
   dei controllori definiti come servizi. Può essere una buona
   idea estrarre del codice comune in un servizio che sia iniettato, piuttosto che
   inserire tale codice in un controllore base da estendere. Entrambi gli approcci
   sono validi, il modo preciso con cui si vuole organizzare il codice è una scelta
   che spetta allo sviluppatore.

Metodi del controllore base e servizi sostitutivi
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Questo elenco mostra come sostituire i metodi del controllore di
base:

:method:`Symfony\\Bundle\\FrameworkBundle\\Controller\\Controller::createForm` (servizio: ``form.factory``)
    .. code-block:: php

        $formFactory->create($type, $data, $options);

:method:`Symfony\\Bundle\\FrameworkBundle\\Controller\\Controller::createFormBuilder` (servizio: ``form.factory``)
    .. code-block:: php

        $formFactory->createBuilder('form', $data, $options);

:method:`Symfony\\Bundle\\FrameworkBundle\\Controller\\Controller::createNotFoundException`
    .. code-block:: php

        new NotFoundHttpException($message, $previous);

:method:`Symfony\\Bundle\\FrameworkBundle\\Controller\\Controller::forward` (servizio: ``http_kernel``)
    .. code-block:: php

        use Symfony\Component\HttpKernel\HttpKernelInterface;
        // ...

        $request = ...;
        $attributes = array_merge($path, array('_controller' => $controller));
        $subRequest = $request->duplicate($query, null, $attributes);
        $httpKernel->handle($subRequest, HttpKernelInterface::SUB_REQUEST);

:method:`Symfony\\Bundle\\FrameworkBundle\\Controller\\Controller::generateUrl` (servizio: ``router``)
    .. code-block:: php

       $router->generate($route, $params, $absolute);

:method:`Symfony\\Bundle\\FrameworkBundle\\Controller\\Controller::getDoctrine` (servizio: ``doctrine``)

    *Iniettare doctrine invece di recuperarlo dal contenitore*

:method:`Symfony\\Bundle\\FrameworkBundle\\Controller\\Controller::getUser` (servizio: ``security.context``)
    .. code-block:: php

        $user = null;
        $token = $securityContext->getToken();
        if (null !== $token && is_object($token->getUser())) {
             $user = $token->getUser();
        }

:method:`Symfony\\Bundle\\FrameworkBundle\\Controller\\Controller::isGranted` (servizio: ``security.context``)
    .. code-block:: php

        $securityContext->isGranted($attributes, $object);

:method:`Symfony\\Bundle\\FrameworkBundle\\Controller\\Controller::redirect`
    .. code-block:: php

        use Symfony\Component\HttpFoundation\RedirectResponse;

        return new RedirectResponse($url, $status);

:method:`Symfony\\Bundle\\FrameworkBundle\\Controller\\Controller::render` (servizio: ``templating``)
    .. code-block:: php

        $templating->renderResponse($view, $parameters, $response);

:method:`Symfony\\Bundle\\FrameworkBundle\\Controller\\Controller::renderView` (servizio: ``templating``)
    .. code-block:: php

       $templating->render($view, $parameters);

:method:`Symfony\\Bundle\\FrameworkBundle\\Controller\\Controller::stream` (servizio: ``templating``)
    .. code-block:: php

        use Symfony\Component\HttpFoundation\StreamedResponse;

        $templating = $this->templating;
        $callback = function () use ($templating, $view, $parameters) {
            $templating->stream($view, $parameters);
        }

        return new StreamedResponse($callback);

.. tip::

    ``getRequest`` è stato deprecato. Inserire invece un parametro nell'azione del
    controllore, chiamato ``Request $request``. L'ordine dei
    parametri non è rilevante, ma occorre specificare il tipo.


.. _`codice della classe Controller`: https://github.com/symfony/symfony/blob/master/src/Symfony/Bundle/FrameworkBundle/Controller/Controller.php
.. _`classe Controller base`: https://github.com/symfony/symfony/blob/master/src/Symfony/Bundle/FrameworkBundle/Controller/Controller.php
.. _`documentazione di FrameworkExtraBundle`: http://symfony.com/doc/current/bundles/SensioFrameworkExtraBundle/annotations/routing.html
