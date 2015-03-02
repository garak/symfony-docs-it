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
    costruttore) quando il controllore diventa troppo grosso e potrebbe aver bisogno
    di essere separato in più controllori.

    Quindi, anche se non si specificano i controllori come servizi, probabilmente lo
    si vedrà fatto in qualche bundle open source di Symfony. È anche importante
    capire i pro e i contro di entrambi gli approcci.

Definire il controllore come servizio
-------------------------------------

Un controllore può essere definito come servizio nello stesso modo di altre classi.
Per esempio, se si ha il seguente controllore::

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

Lo si può definire come servizio in questo modo:

.. configuration-block::

    .. code-block:: yaml

        # src/Acme/HelloBundle/Resources/config/services.yml
        parameters:
            # ...
            acme.controller.hello.class: Acme\HelloBundle\Controller\HelloController

        services:
            acme.hello.controller:
                class: "%acme.controller.hello.class%"

    .. code-block:: xml

        <!-- src/Acme/HelloBundle/Resources/config/services.xml -->
        <parameters>
            <!-- ... -->
            <parameter key="acme.controller.hello.class">Acme\HelloBundle\Controller\HelloController</parameter>
        </parameters>

        <services>
            <service id="acme.hello.controller" class="%acme.controller.hello.class%" />
        </services>

    .. code-block:: php

        // src/Acme/HelloBundle/Resources/config/services.php
        use Symfony\Component\DependencyInjection\Definition;

        // ...
        $container->setParameter(
            'acme.controller.hello.class',
            'Acme\HelloBundle\Controller\HelloController'
        );

        $container->setDefinition('acme.hello.controller', new Definition(
            '%acme.controller.hello.class%'
        ));

Fare riferimento al servizio
----------------------------

Per fare riferimento a un controllore definito come servizio, usare la notazione con due punti (:).
Per esempio, per rinviare al metodo ``indexAction()`` del servizio
definito sopra con id ``acme.hello.controller``::

    $this->forward('acme.hello.controller:indexAction');

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
            defaults: { _controller: acme.hello.controller:indexAction }

    .. code-block:: xml

        <!-- app/config/routing.xml -->
        <route id="hello" path="/hello">
            <default key="_controller">acme.hello.controller:indexAction</default>
        </route>

    .. code-block:: php

        // app/config/routing.php
        $collection->add('hello', new Route('/hello', array(
            '_controller' => 'acme.hello.controller:indexAction',
        )));

.. tip::

    Si possono anche usare annotazioni per configurare le rotte in un controllore
    definito come servizio. Vedere la `documentazione di FrameworkExtraBundle`_
    per maggiori dettagli.

.. versionadded:: 2.6
    Se il servizio controllore implementa il metodo ``__invoke``, basta fare riferimento all'id del servizio
    (``acme.hello.controller``).

Alternative ai metodi del controllore base
------------------------------------------

Quando si usa un controllore definito come servizio, probabilmente non si estenderà
la classe base ``Controller``. Invece di appoggiarsi ai metodi scorciatoria,
si interagirà direttamente con i servizi necessari. Per fortuna, farlo è alquanto
facile e il `codice della classe Controller`_ fornisce un ottimo spunto
su come eseguire compiti comuni.

Per esempio, se si vuole rendere un template invece di creare direttamente l'oggetto ``Response``,
il codice somiglierà a questo, se si estende
il controllore base di Symfony::

    // src/Acme/HelloBundle/Controller/HelloController.php
    namespace Acme\HelloBundle\Controller;

    use Symfony\Bundle\FrameworkBundle\Controller\Controller;

    class HelloController extends Controller
    {
        public function indexAction($name)
        {
            return $this->render(
                'AcmeHelloBundle:Hello:index.html.twig',
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

    // src/Acme/HelloBundle/Controller/HelloController.php
    namespace Acme\HelloBundle\Controller;

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
                'AcmeHelloBundle:Hello:index.html.twig',
                array('name' => $name)
            );
        }
    }

La definizione del servizio va modificata, per specificare il parametro
del costruttore:

.. configuration-block::

    .. code-block:: yaml

        # src/Acme/HelloBundle/Resources/config/services.yml
        parameters:
            # ...
            acme.controller.hello.class: Acme\HelloBundle\Controller\HelloController

        services:
            acme.hello.controller:
                class:     "%acme.controller.hello.class%"
                arguments: ["@templating"]

    .. code-block:: xml

        <!-- src/Acme/HelloBundle/Resources/config/services.xml -->
        <parameters>
            <!-- ... -->
            <parameter
                key="acme.controller.hello.class"
            >Acme\HelloBundle\Controller\HelloController</parameter>
        </parameters>

        <services>
            <service id="acme.hello.controller" class="%acme.controller.hello.class%">
                <argument type="service" id="templating"/>
            </service>
        </services>

    .. code-block:: php

        // src/Acme/HelloBundle/Resources/config/services.php
        use Symfony\Component\DependencyInjection\Definition;
        use Symfony\Component\DependencyInjection\Reference;

        // ...
        $container->setParameter(
            'acme.controller.hello.class',
            'Acme\HelloBundle\Controller\HelloController'
        );

        $container->setDefinition('acme.hello.controller', new Definition(
            '%acme.controller.hello.class%',
            array(new Reference('templating'))
        ));

Invece che recuperare il servizio ``templating`` dal contenitore, si può
iniettare *solamente* il servizio o i servizi necessari direttamente nel controllore.

.. note::

   Questo non vuol dire che non si possano estendere questi controllori da un proprio
   controllore base. La rinuncia al controllore base standard è dovuta al fatto che
   i metodi aiutanti si appoggiano al conenitore disponibile, che non è il caso
   dei controllori definiti come servizi. Può essere una buona
   idea estrarre del codice comune in un servizio che sia iniettato, piuttosto che
   inserire tale codice in un controllore base da estendere. Entrambi gli approcci
   sono validi, il modo preciso con cui si vuole organizzare il codice è una scelta
   che spetta allo sviluppatore.

.. _`codice della classe Controller`: https://github.com/symfony/symfony/blob/master/src/Symfony/Bundle/FrameworkBundle/Controller/Controller.php
.. _`classe Controller base`: https://github.com/symfony/symfony/blob/master/src/Symfony/Bundle/FrameworkBundle/Controller/Controller.php
.. _`documentazione di FrameworkExtraBundle`: http://symfony.com/doc/current/bundles/SensioFrameworkExtraBundle/annotations/routing.html
