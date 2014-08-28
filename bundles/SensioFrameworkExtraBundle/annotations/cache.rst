@Cache
======

L'annotazione ``@Cache`` rende facile la definizione degli header di cache HTTP per
scadenza e validazione.

Strategie di scadenza HTTP
--------------------------

L'annotazione ``@Cache`` rende facile la definizione degli header di cache HTTP::

    use Sensio\Bundle\FrameworkExtraBundle\Configuration\Cache;

    /**
     * @Cache(expires="tomorrow", public="true")
     */
    public function indexAction()
    {
    }

Si può anche usare l'annotazione su una classe, per definire la cache per tutti i metodi
di un controllore::

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

.. note::

   L'attributo ``expires`` accetta ogni data valida, secondo la funzione di PHP
   ``strtotime()``.

Strategie di validazione HTTP
-----------------------------

.. caution::

    Il supporto per le stretegie di validazione HTTP è stato introdotto in SensioFrameworkExtraBundle
    3.0. Questa versione del bundle si può usare solo con Symfony 2.4 o successivi (vedere
    :ref:`il ciclo di rilascio di SensioFrameworkExtraBundle <release-cycle-note>`).

Gli attributi ``lastModified`` ed ``ETag`` gestiscono gli header HTTO di validazione della
cache.. ``lastModified`` aggiunge un header ``Last-Modified`` alle risposte ed
``ETag`` aggiunge un header ``ETag``.

Entrambi fanno scattare automaticamente la logica per restituire una risposta 304 quando
la risposta non è modificata (in questo caso, il controllore *non* viene richiamato)::

    use Sensio\Bundle\FrameworkExtraBundle\Configuration\Cache;

    /**
     * @Cache(lastModified="post.getUpdatedAt()", ETag="'Post' ~ post.getId() ~ post.getUpdatedAt()")
     */
    public function indexAction(Post $post)
    {
        // codice...
        // non sarà richiamato in caso di 304
    }

Fa più o meno le stesse cose del codice seguente::

    public function myAction(Request $request, Post $post)
    {
        $response = new Response();
        $response->setLastModified($post->getUpdatedAt());
        if ($response->isNotModified($request)) {
            return $response;
        }

        // codice...
    }

.. note::

    Il valore dell'header HTTP ETag è il risultato dell'espressione, passato in hash con l'algoritmo
    ``sha256``.

Attributi
---------

Ecco una lista di attributi validi, con i rispettivi header HTTP:

===================================================== ================================
Annotazione                                           Metodo della risposta
===================================================== ================================
``@Cache(expires="tomorrow")``                        ``$response->setExpires()``
``@Cache(smaxage="15")``                              ``$response->setSharedMaxAge()``
``@Cache(maxage="15")``                               ``$response->setMaxAge()``
``@Cache(vary={"Cookie"})``                           ``$response->setVary()``
``@Cache(public=true)``                               ``$response->setPublic()``
``@Cache(lastModified="post.getUpdatedAt()")``        ``$response->setLastModified()``
``@Cache(ETag="post.getId() ~ post.getUpdatedAt()")`` ``$response->setETag()``
===================================================== ================================
