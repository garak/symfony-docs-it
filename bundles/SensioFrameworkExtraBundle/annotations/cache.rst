@Cache
======

Utilizzo
--------

L'annotazione ``@Cache`` rende facile la definizione della cache HTTP::

    use Sensio\Bundle\FrameworkExtraBundle\Configuration\Cache;

    /**
     * @Cache(expires="tomorrow", public="true")
     */
    public function indexAction()
    {
    }

Si può anche usare l'annotazione su una classe, per definire la cache per tutti i metodi::

    /**
     * @Cache(expires="tomorrow", public="true")
     */
    class BlogController extends Controller
    {
    }

Se ci dovesse essere un conflitto tra la configurazione della classe e quella del metodo,
l'ultima vincerebbe::

    /**
     * @Cache(expires="tomorrow")
     */
    class BlogController extends Controller
    {
        /**
         * @Cache(expires="+2 days")
         */
        public function indexAction()
        {
        }
    }

Attributi
---------

Ecco una lista di attributi validi, con i rispettivi header HTTP:

============================== ===============
Annotazione                    Metodo Response
============================== ===============
``@Cache(expires="tomorrow")`` ``$response->setExpires()``
``@Cache(smaxage="15")``       ``$response->setSharedMaxAge()``
``@Cache(maxage="15")``        ``$response->setMaxAge()``
``@Cache(vary=["Cookie"])``    ``$response->setVary()``
``@Cache(public="true")``      ``$response->setPublic()``
============================== ===============

.. note::

   L'attributo ``expires`` accetta qualsiasi data valida interpretabile dalla funzione
   ``strtotime()`` di PHP.
