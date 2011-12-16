@ParamConverter
===============

Utilizzo
--------

L'annotazione ``@ParamConverter`` richiama dei *convertitori*, per convertire parametri
della richiesta in oggetti. Tali oggetti sono memorizzati come attributi della richiesta
e quindi possono essere iniettati come parametri dei metodi del controllore::

    use Sensio\Bundle\FrameworkExtraBundle\Configuration\Route;
    use Sensio\Bundle\FrameworkExtraBundle\Configuration\ParamConverter;

    /**
     * @Route("/blog/{id}")
     * @ParamConverter("post", class="SensioBlogBundle:Post")
     */
    public function showAction(Post $post)
    {
    }

Dietro le quinte, succedono diverse cose:

* Il convertitore prova a prendere un oggetto ``SensioBlogBundle:Post`` dagli attributi
  della richiesta (gli attributi della richiesta arrivano dai segnaposto delle rotte,
  in questo caso ``id``);

* Se non si trova nessun oggetto ``Post``, viene generata una risposta ``404``;

* Se si trova un oggetto ``Post``, viene definito un nuovo attributo ``post`` della
  richiesta (accessibile tramite ``$request->attributes->get('post')``);

* Come ogni altro attributo della richiesta, è iniettato automaticamente nel
  controllore, quando presente nella firma del metodo.

Se si usa il type hinting, come nell'esempio precedente, si può anche omettere
del tutto l'annotazione ``@ParamConverter``::

    // automatico, con la firma del metodo
    public function showAction(Post $post)
    {
    }

Convertitori predefiniti
------------------------

Il bundle ha un solo convertitore predefinito, quello di Doctrine.

Convertitore Doctrine
~~~~~~~~~~~~~~~~~~~~~

Il convertitore di Doctrine usa il gestore di entità *predefinito*. Si può modificare
tale comportamento con l'opzione ``entity_manager``::

    use Sensio\Bundle\FrameworkExtraBundle\Configuration\Route;

    use Sensio\Bundle\FrameworkExtraBundle\Configuration\ParamConverter;
    /**
     * @Route("/blog/{id}")
     * @ParamConverter("post", class="SensioBlogBundle:Post", options={"entity_manager" = "foo"})
     */
    public function showAction(Post $post)
    {
    }

Creare un convertitore
----------------------

Ogni convertitore deve implementare
:class:`Sensio\\Bundle\\FrameworkExtraBundle\\Request\\ParamConverter\\ParamConverterInterface`::

    namespace Sensio\Bundle\FrameworkExtraBundle\Request\ParamConverter;

    use Sensio\Bundle\FrameworkExtraBundle\Configuration\ConfigurationInterface;
    use Symfony\Component\HttpFoundation\Request;

    interface ParamConverterInterface
    {
        function apply(Request $request, ConfigurationInterface $configuration);

        function supports(ConfigurationInterface $configuration);
    }

Il metodo ``supports()`` deve restituire ``true`` quando è in grando di convertire la
configurazione data (un'istanza di ``ParamConverter``).

L'istanza ``ParamConverter`` ha tre informazioni sull'annotazione:

* ``name``: Il nome dell'attributo;
* ``class``: Il nome della classe dell'attributo (può essere una qualsiasi stringa che
  rappresenti il nome di una classe);
* ``options``: Un array di opzioni

Il metodo ``apply()`` è richiamato per ogni configurazione supportata. In base agli
attributi della richiesta, dovrebbe impostare un attributo chiamato
``$configuration->getName()``, che memorizza un oggetto di classe
``$configuration->getClass()``.

.. tip::

   Si può usare la classe ``DoctrineConverter`` come modello per i propri convertitori.

