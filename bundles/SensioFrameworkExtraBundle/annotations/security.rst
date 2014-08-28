@Security
=========

.. caution::

    L'annotazione ``@Security`` è stata introdotta in SensioFrameworkExtraBundle
    3.0. Questa versione del bundle può essere usata solo con Symfony 2.4 o successivi (vedere
    :ref:`il ciclo di rilasci di SensioFrameworkExtraBundle <release-cycle-note>`).

Uso
---

L'annotazione ``@Security`` limita l'accesso nei controllori::

    use Sensio\Bundle\FrameworkExtraBundle\Configuration\Security;

    class PostController extends Controller
    {
        /**
         * @Security("has_role('ROLE_ADMIN')")
         */
        public function indexAction()
        {
            // ...
        }
    }

L'espressione può usare tutte le funzioni utilizzabili nella sezione ``access_control``
della configurazione della sicurezza, con l'aggiunta della funzione
``is_granted()``.

L'espressione ha accesso alle seguenti variabili:

* ``token``: il token di sicurezza corrente;
* ``user``: l'oggetto utente corrente;
* ``request``: l'istanza della richiesta;
* ``roles``: i ruoli dell'utente;
* e a tutti gli attributi della richiesta.

La funzione ``is_granted()`` consente di restringere l'accesso in base a variabili
passate al controllore::

    /**
     * @Security("is_granted('POST_SHOW', post)")
     */
    public function showAction(Post $post)
    {
    }

.. note::

    Definire un'annotazione ``Security`` ha lo stesso effetto di definire una regola
    di controllo di accesso, ma è più efficiente, perché la verifica viene eseguita
    solo all'accesso alla specifica rotta.

.. tip::

    Si può aggiungere un'annotazione ``@Security`` anche sull'intera classe del controllore.
