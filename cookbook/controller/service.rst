.. index::
   single: Controllore; Come servizi

Definire i controllori come servizi
===================================

Nel libro, abbiamo imparato quanto è facile usare un controllore quando 
estende la classe base
:class:`Symfony\\Bundle\\FrameworkBundle\\Controller\\Controller`. Oltre
a questo metodo, i controllori possono anche essere specificati come servizi.

.. note::

    Specificare un controllore come servizio richiede un po' più di lavoro. Il
    vantaggio principale è che l'intero controllore o ogni servizio passato al
    controllore possono essere modificati tramite la configurazione del contenitore.
    Questo è particolarmente utile durante lo sviluppo di un bundle open source o di
    un bundle da usare in molti progetti diversi.

    Un secondo vantaggio è che il controllori sono più isolati. Guardando i
    parametri del costruttore, è facile capire che tipo di cose
    tale controllore possa o non possa fare. Poiché ogni dipendenza deve
    essere iniettata a mano, è più ovvio (p.e. se si hanno molti parametri del
    costruttore) quando un controllore sia diventato troppo grosso e quindi abbia
    bisogno di essere diviso in più controllori.

    Quindi, anche se non si specificano i controllori come servizi, probabilmente
    lo si vedrà in alcuni bundle open source di Symfony2. È anche importante
    capire pro e contro dei due approcci.

Definire il controllore come servizio
-------------------------------------

Si può definire un controllore come servizio nello stesso modo di qualsiasi altra classe.
Per esempio, se si ha questo semplice controllore::

    // src/Acme/HelloBundle/Controller/HelloController.php
    namespace Acme\HelloBundle\Controller;

    use Symfony\Component\HttpFoundation\Response;

    class HelloController
    {
        public function indexAction($name)
        {
            return new Response('<html><body>Hello '.$name.'!</body></html>');
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
                class:     "%acme.controller.hello.class%"

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

Per fare riferimento a un controllore definito come servizio, usare la notazione con i due punti (:).
Per esempio, per rinviare al metodo ``indexAction()`` del servizio
appena definito con l'id ``acme.hello.controller``::

    $this->forward('acme.hello.controller:indexAction');

.. note::

    Non è possibile omettere la parte ``Action`` del nome del metodo, se si usa
    questa sintassi.

Si può anche definire un rotta per il servizio, usando la stessa notazione
nel valore ``_controller`` della rotta:

.. configuration-block::

    .. code-block:: yaml

        # app/config/routing.yml
        hello:
            pattern:      /hello
            defaults:     { _controller: acme.hello.controller:indexAction }

    .. code-block:: xml

        <!-- app/config/routing.xml -->
        <route id="hello" pattern="/hello">
            <default key="_controller">acme.hello.controller:indexAction</default>
        </route>

    .. code-block:: php

        // app/config/routing.php
        $collection->add('hello', new Route('/hello', array(
            '_controller' => 'acme.hello.controller:indexAction',
        )));

.. tip::

    Si possono anche usare le annotazioni per configurare le rotte di un controllore
    definito come servizio. Vedere
    :doc:`FrameworkExtraBundle documentation</bundles/SensioFrameworkExtraBundle/annotations/routing>`
    per maggiori dettagli.

Alternative ai metodi del controllore base
-------------------------------------------

Quando si usa un controllore definito come servizio, molto probabilmente non si estenderà
la classe base ``Controller``. Invece di basarsi sui metodi scorciatoria,
si interagirà direttamente con i servizi necessari. Per fortuna, questo è
alquanto facile e il sorgente della `classe Controller base`_ è una buona
fonte su come eseguire questi compiti comuni.

Per esempio, se si vuole rendere un template, invece di creare direttamente l'oggetto  ``Response``,
il codice assomiglierà al seguente, se si estende
il controllore di base::

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

Se si guarda il codice sorgente del metodo ``render`` nella
`classe Controller base`_, si vedrà che tale metodo effettivamente usa il
servizio ``templating``::

    public function render($view, array $parameters = array(), Response $response = null)
    {
        return $this->container->get('templating')->renderResponse($view, $parameters, $response);
    }

In un controllore definito come servizio, si può invece iniettare il servizio ``templating``
e usarlo direttamente::

    // src/Acme/HelloBundle/Controller/HelloController.php
    namespace Acme\HelloBundle\Controller;

    use Symfony\Component\HttpFoundation\Response;

    class HelloController
    {
        private $templating;

        public function __construct($templating)
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

Occorre anche modificare la definizione del servizio, per specificare il parametro
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
iniettare *solo* il servizio (o i servizi) di cui si ha bisogno, direttamente nel controllore.

.. note::

   Questo non significa che non si possa estendere tali controllori da un proprio
   controllore di base. L'evitare il controllore di base standard è giustificato
   dal fatto che i suoi metodi si basano sull'aver a disposizione il contenitore, che non è
   il caso per i controllori definiti come servizi. Potrebbe essere una buona idea
   estrarre del codice comune in un servizio iniettato, piuttosto che
   inserire tale codice in un controllore base da estendere. Entrambi gli approcci
   sono validi, il modo esatto in cui organizzare il codice dipende dallo
   sviluppatore.

.. _`classe Controller base`: https://github.com/symfony/symfony/blob/master/src/Symfony/Bundle/FrameworkBundle/Controller/Controller.php

