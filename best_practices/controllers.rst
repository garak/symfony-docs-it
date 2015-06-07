Controllori
===========

Symfony segue la filosofia *"controllori magri e modelli grassi"*.
Ciò significa che i controllori dovrebbero contenere solo il codice strettamente necessario
per coordinare le diversi parti dell'applicazione.

La regola d'oro per i controllori è sintetizzata nella tripla 5-10-20.
Ogni controllore dovrebbe definire al massimo 5 variabili, contenere al massimo 10 azioni,
ciascuna delle quali formate al massimo da 20 righe di codice. Anche se possono
esserci eccezioni alla regola, essa mette in evidenza quando sarebbe opportuno
iniziare a rifattorizzare il codice del controllore e spostarlo in un servizio.

.. best-practice::

    Estendere il controllore dalla classe base fornita da FrameworkBundle
    e usare le annotazioni per configurare rotte, cache e sicurezza, quando possibile.

L'accoppiamento dei controllore al framework sottostante consente di sfruttare tutte
le funzionalità del framework stesso, aumentando la produttività.

Poiché i controllori dovrebbero essere leggeri e contenere niente
di più che poche linee di codice, per coordinare il flusso della richiesta, 
spendere ore provando a disaccoppiarlo dal framework non porterà grandi benefici nel lungo periodo.
La quantità di tempo *sprecato* non vale il beneficio.

Inoltre usare le annotazioni per le rotte, la cache e la sicurezza semplifica
enormemente la configurazione dell'applicazione.
Non sarà necessario esplorare decine di file di formati diversi
(YAML, XML, PHP): tutta la configurazione è lì dove serve e in un solo formato.

Complessivamente, quindi, si dovrebbe disaccoppiare totalmente la logica di business
dal framework e nello stesso tempo accoppiare totalmente al framework controllori e rotte,
in modo da ottenere il massimo da Symfony.

Configurazione delle rotte
--------------------------

Per caricare tutte le rotte definite nelle annotazioni dei controllori del bundle
`AppBundle` aggiungete la seguente configurazione al file principale delle rotte:

.. code-block:: yaml

    # app/config/routing.yml
    app:
        resource: "@AppBundle/Controller/"
        type:     annotation

Questa configurazione caricherà le annotazioni da ogni controllore presente sia nella
cartella ``src/AppBundle/Controller/`` sia nelle sue sottocartelle. Se
l'applicazione definisce molti controllori, è perfettamente lecito organizzare il
tutto in sottocartelle.

.. code-block:: text

    <progetto>/
    ├─ ...
    └─ src/
       └─ AppBundle/
          ├─ ...
          └─ Controller/
             ├─ DefaultController.php
             ├─ ...
             ├─ Api/
             │  ├─ ...
             │  └─ ...
             └─ Backend/
                ├─ ...
                └─ ...

Configurazione dei template
---------------------------

.. best-practice::

    Non usare l'annotazione ``@Template()`` per configurare il template
    usato dal controllore.

Anche se l'annotazione ``@Template()`` risulta molto utile, essa nasconde qualche trucco.
Ritenendo che il gioco non valga la candela, si raccomanda di non
usarla.

La maggior parte delle volte ``@Template`` è usato senza parametri, il che rende più difficile
sapere quale template viene reso. Il suo utilizzo inoltre rende meno ovvio
ai principianti che un controllore deve sempre restituire un oggetto Response (a meno che non si usi
il livello della vista).

Come dovrebbe essere il controllore
-----------------------------------

Considerando tutto, ecco un esempio di come dovrebbe essere il controllore
per l'homepage di un'applicazione:

.. code-block:: php

    namespace AppBundle\Controller;

    use Symfony\Bundle\FrameworkBundle\Controller\Controller;
    use Sensio\Bundle\FrameworkExtraBundle\Configuration\Route;

    class DefaultController extends Controller
    {
        /**
         * @Route("/", name="homepage")
         */
        public function indexAction()
        {
            $posts = $this->getDoctrine()
                ->getRepository('AppBundle:Post')
                ->findLatest();

            return $this->render('default/index.html.twig', array(
                'posts' => $posts
            ));
        }
    }

.. _best-practices-paramconverter:

Usare ParamConverter
--------------------

Se l'applicazione usa Doctrine, è possibile usare *opzionalmente* `ParamConverter`_
per effettuare la ricerca dell'entity in modo automatico e passarla come parametro del controllore.

.. best-practice::

    Usare ParamConverter per caricare automaticamente le entità di Doctrine,
    nei casi più semplici.

Per esempio:

.. code-block:: php

    use AppBundle\Entity\Post;
    use Sensio\Bundle\FrameworkExtraBundle\Configuration\Route;

    /**
     * @Route("/{id}", name="admin_post_show")
     */
    public function showAction(Post $post)
    {
        $deleteForm = $this->createDeleteForm($post);

        return $this->render('admin/post/show.html.twig', array(
            'post'        => $post,
            'delete_form' => $deleteForm->createView(),
        ));
    }

Solitamente ci si aspetterebbe un parametro ``$id`` nel metodo ``showAction``. Invece,
creando un nuovo parametro (``$post``) e specificando il tipo di classe ``Post``
(che è un'entità Doctrine), ParamConverter cercherà automaticamente
un oggetto la cui proprietà ``$id`` corrisponde al valore ``{id}``. Nel
caso in cui non venga trovato alcun ``Post``, verrà mostrato la pagina 404.

Esecuzione di ricerche più avanzate
-----------------------------------

Nell'esempio precedente tutto funziona senza nessuna configurazione, perché il nome del segnaposto ``{id}``
corrisponde esattamente al nome della proprietà dell'entità. Quando questo non succede, o se si ha
perfino una logica più complessa, la cosa più facile da fare è cercare l'entità manualmente.
Questo è per esempio quello che succede nella classe ``CommentController``  dell'applicazione:

.. code-block:: php

    /**
     * @Route("/comment/{postSlug}/new", name = "comment_new")
     */
    public function newAction(Request $request, $postSlug)
    {
        $post = $this->getDoctrine()
            ->getRepository('AppBundle:Post')
            ->findOneBy(array('slug' => $postSlug));

        if (!$post) {
            throw $this->createNotFoundException();
        }

        // ...
    }

Naturalmente è possibile configurare ``@ParamConverter`` in modo più avanzato,
perché è abbastanza flessibile:

.. code-block:: php

    use AppBundle\Entity\Post;
    use Sensio\Bundle\FrameworkExtraBundle\Configuration\Route;
    use Sensio\Bundle\FrameworkExtraBundle\Configuration\ParamConverter;
    use Symfony\Component\HttpFoundation\Request;

    /**
     * @Route("/comment/{postSlug}/new", name = "comment_new")
     * @ParamConverter("post", options={"mapping": {"postSlug": "slug"}})
     */
    public function newAction(Request $request, Post $post)
    {
        // ...
    }

Si può infine dire che la scorciatoia di ParamConverter è buona nelle situazioni semplici.
Nonostante ciò, non si dovrebbe mai dimenticare che la ricerca diretta di entity è un'operazione 
molto facile.

Eseguire codice prima e dopo
----------------------------

Se si ha la necessità di eseguire del codice prima o dopo l'esecuzione dei controllore,
è possibile usare il componente EventDispatcher
:doc:`configurando i filtri prima e dopo </cookbook/event_dispatcher/before_after_filters>`.

.. _`ParamConverter`: http://symfony.com/doc/current/bundles/SensioFrameworkExtraBundle/annotations/converters.html
