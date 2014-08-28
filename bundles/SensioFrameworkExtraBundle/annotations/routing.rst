@Route e @Method
================

Utilizzo
--------

L'annotazione ``@Route`` mappa lo schema di una rotta con un controllore::

    use Sensio\Bundle\FrameworkExtraBundle\Configuration\Route;

    class PostController extends Controller
    {
        /**
         * @Route("/")
         */
        public function indexAction()
        {
            // ...
        }
    }

L'azione ``index`` del controllore ``Post`` viene mappata sull'URL ``/``.
Ciò equivale alla seguente configurazione YAML:

.. code-block:: yaml

    blog_home:
        path:     /
        defaults: { _controller: SensioBlogBundle:Post:index }

Come ogni altro schema di rotta, si possono definire segnaposto, requisiti e valori
predefiniti::

    /**
     * @Route("/{id}", requirements={"id" = "\d+"}, defaults={"id" = 1})
     */
    public function showAction($id)
    {
    }

Si può anche definire il valore predefinito per un segnaposto, usando
il valore predefinito di PHP::

    /**
     * @Route("/{id}", requirements={"id" = "\d+"})
     */
    public function showAction($id = 1)
    {
    }

Si può anche far corrispondere la rotta a più di un URL, definendo annotazioni
``@Route``::

    /**
     * @Route("/", defaults={"id" = 1})
     * @Route("/{id}")
     */
    public function showAction($id)
    {
    }

.. _frameworkextra-annotations-routing-activation:

Attivazione
-----------

Le rotte devono essere importate, per essere attive come ogni altra risorsa di rotta
(notare il tipo ``annotation``):

.. code-block:: yaml

    # app/config/routing.yml

    # importa rotte da una classe controllore
    post:
        resource: "@SensioBlogBundle/Controller/PostController.php"
        type:     annotation

Si può anche importare un'intera cartella:

.. code-block:: yaml

    # importa rotte da una cartella di controllori
    blog:
        resource: "@SensioBlogBundle/Controller"
        type:     annotation

Come per ogni altra risorsa, si possono "montare" le rotte sotto un prefisso:

.. code-block:: yaml

    post:
        resource: "@SensioBlogBundle/Controller/PostController.php"
        prefix:   /blog
        type:     annotation

Nome della rotta
----------------

Una rotta definita con l'annotazione ``@Route`` riceve un nome predefinito, composto dal
nome del bundle, il nome del controllore e il nome dell'azione. Per l'esempio precedente,
il nome è ``sensio_blog_post_index``.

Si può usare l'attributo ``name``, per ridefinire il nome predefinito::

    /**
     * @Route("/", name="blog_home")
     */
    public function indexAction()
    {
        // ...
    }

Prefisso della rotta
--------------------

Un'annotazione ``@Route`` in una classe controllore definisce un prefisso per le rotte
di tutte le azioni (notare che non si possono avere più annotazioni ``@Route`` sulla stessa
classe)::

    /**
     * @Route("/blog")
     */
    class PostController extends Controller
    {
        /**
         * @Route("/{id}")
         */
        public function showAction($id)
        {
        }
    }

L'azione ``show`` è mappata sullo schema ``/blog/{id}``.

Metodo della rotta
------------------

C'è un'annotazione scorciatoia ``@Method``, per specificare il metodo HTTP consentito per
la rotta. Per usarlo, importare lo spazio dei nomi ``Method``::

    use Sensio\Bundle\FrameworkExtraBundle\Configuration\Route;
    use Sensio\Bundle\FrameworkExtraBundle\Configuration\Method;

    /**
     * @Route("/blog")
     */
    class PostController extends Controller
    {
        /**
         * @Route("/edit/{id}")
         * @Method({"GET", "POST"})
         */
        public function editAction($id)
        {
        }
    }

L'azione ``edit`` è mappata sullo schema ``/blog/edit/{id}`` se il metodo usato è
GET o POST.

L'annotazione ``@Method`` viene considerata solo se un'azione è annotata con
``@Route``.

Controllore come servizio
-------------------------

L'annotazione ``@Route`` su una classe controllore può anche essere usata per assegnare
la classe a un servizio, in modo che il risolutore istanzi
il controllore recuperandolo dal contenitore, invece che richiamando ``new
PostController()``::

    /**
     * @Route(service="mio_servizio_post_controller")
     */
    class PostController
    {
        // ...
    }
