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

.. tip::

    Si pul disabilitare la conversione automatica dei parametri con tipo,
    impostando ``auto_convert`` a ``false``:

    .. configuration-block::

        .. code-block:: yaml

            # app/config/config.yml
            sensio_framework_extra:
                request:
                    converters: true
                    auto_convert: false

        .. code-block:: xml

            <sensio-framework-extra:config>
                <request converters="true" auto-convert="true" />
            </sensio-framework-extra:config>

Per determinare quale convertitore eseguire su un parametro, viene eseguito il seguente procedimento:

* Se c'è una scelta esplicitia di convertitore, tramite
  ``@ParamConverter(convertitore="nome")``, viene scelto il convertitore con il nome
  fornito.
* Altrimenti, si itera su tutti i convertitori di parametri registrati, per priorità.
  Viene invocato il metodo ``supports()``, per verificare se un convertitore possa
  convertire la richiesta nel parametro desiderato. Se il metodo restituisce ``true``,
  il convertitore viene richiamato.

Convertitori predefiniti
------------------------

Il bundle ha due convertitori predefinito, quello di Doctrine e il
DateTime.

Convertitore Doctrine
~~~~~~~~~~~~~~~~~~~~~

Nome del convertitore: ``doctrine.orm``

Il convertitore di Doctrine tenta di convertire gli attributi della richiesta in entità Doctrine,
recuperate dalla base dati. Sono possibili due diversi approcci:

- Recuperare un oggetto in base alla chiave primaria
- Recuperare un oggetto in base a uno o più campi che contengano valori univoci
  nella base dati.

Il seguente algoritmo determina quale operazione eseguire.

- Se nella rotta è presente un parametro ``{id}``, trova l'oggetto per chiave primaria.
- Se è configurata un'opzione ``'id'`` e corrisponde ai parametri della rotta, trova l'oggetto per chiave primaria.
- Se le regole precedenti non si applicano, prova a trovare un'entità in cui i campi corrispondano
  ai parametri della rotta. Si può controllare questo processo configurando
  i parametri ``exclude`` o il parametro ``mapping``.

Il convertitore Doctrine usa il gestore di entità predefinito (*default*). Si può
configurare con l'opzione ``entity_manager``::

    use Sensio\Bundle\FrameworkExtraBundle\Configuration\Route;
    use Sensio\Bundle\FrameworkExtraBundle\Configuration\ParamConverter;

    /**
     * @Route("/blog/{id}")
     * @ParamConverter("post", class="SensioBlogBundle:Post", options={"entity_manager" = "foo"})
     */
    public function showAction(Post $post)
    {
    }

Se il segnaposto non ha lo stesso nome della chiave primaria, passare l'opzione
``id``::

    /**
     * @Route("/blog/{post_id}")
     * @ParamConverter("post", class="SensioBlogBundle:Post", options={"id" = "post_id"})
     */
    public function showAction(Post $post)
    {
    }

.. tip::

   L'opzione ``id`` specifica quale segnaposto della rotta viene passato al metodo del
   repository. Se non si specifica alcun metodo, viene usato ``find()``.

Si possono anche avere più convertitori in una sola azione::

    /**
     * @Route("/blog/{id}/comments/{comment_id}")
     * @ParamConverter("comment", class="SensioBlogBundle:Comment", options={"id" = "comment_id"})
     */
    public function showAction(Post $post, Comment $comment)
    {
    }

In questo esempio, il parametro ``$post`` è gestito automaticamente, mentre ``$comment`` è
configurato tramite annotazione, poiché non possono seguire entrambi la convenzione predefinita.

Se si vuole cercare un'entità per più campi, usare l'opzione ``mapping``: un array in cui
la chiave è il nome del segnaposto della rotte e il valore è il nome
del campo Doctrine::

    /**
     * @Route("/blog/{date}/{slug}/comments/{comment_slug}")
     * @ParamConverter("post", options={"mapping": {"date": "date", "slug": "slug"}})
     * @ParamConverter("comment", options={"mapping": {"comment_slug": "slug"}})
     */
    public function showAction(Post $post, Comment $comment)
    {
    }

Se si vuole cercare un'entità per più campi, ma si vuole escludere un parametro
della rotta dai criteri di ricerca::

    /**
     * @Route("/blog/{date}/{slug}")
     * @ParamConverter("post", options={"exclude": {"date"}})
     */
    public function showAction(Post $post, \DateTime $date)
    {
    }

Se si vuole specificare un metodo del repository da usare per trovare le entità (per esempio,
per aggiungere join alla query), si può aggiungere l'opzione ``repository_method``::

    /**
     * @Route("/blog/{id}")
     * @ParamConverter("post", class="SensioBlogBundle:Post", options={"repository_method" = "findWithJoins"})
     */
    public function showAction(Post $post)
    {
    }

Convertitore DateTime
~~~~~~~~~~~~~~~~~~~~~

Nome del convertitore: ``datetime``

Il convertitore DateTime converte una rotta o un attributo della richiesta in un'istanza
di DateTime::

    /**
     * @Route("/blog/archive/{start}/{end}")
     */
    public function archiveAction(\DateTime $start, \DateTime $end)
    {
    }

Qualsiasi formato di data che possa essere analizzato dal costruttore di ``DateTime``
è accettabile. Si può restringere l'input tramite opzioni::

    /**
     * @Route("/blog/archive/{start}/{end}")
     * @ParamConverter("start", options={"format": "Y-m-d"})
     * @ParamConverter("end", options={"format": "Y-m-d"})
     */
    public function archiveAction(\DateTime $start, \DateTime $end)
    {
    }

Creare un convertitore
----------------------

Ogni convertitore deve implementare ``ParamConverterInterface``::

    namespace Sensio\Bundle\FrameworkExtraBundle\Request\ParamConverter;

    use Sensio\Bundle\FrameworkExtraBundle\Configuration\ParamConverter;
    use Symfony\Component\HttpFoundation\Request;

    interface ParamConverterInterface
    {
        function apply(Request $request, ParamConverter $configuration);

        function supports(ParamConverter $configuration);
    }

Il metodo ``supports()`` deve restituire ``true`` quando è in grado di convertire la
configurazione data (un'istanza di ``ParamConverter``).

L'istanza ``ParamConverter`` ha tre informazioni sull'annotazione:

* ``name``: il nome dell'attributo;
* ``class``: il nome della classe dell'attributo (può essere una qualsiasi stringa che
  rappresenti il nome di una classe);
* ``options``: un array di opzioni

Il metodo ``apply()`` è richiamato per ogni configurazione supportata. In base agli
attributi della richiesta, dovrebbe impostare un attributo chiamato
``$configuration->getName()``, che memorizza un oggetto di classe
``$configuration->getClass()``.

Per registrare il servizio contenitore, si deve aggiungere un tag

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        services:
            mio_convertitore:
                class:        MyBundle\Request\ParamConverter\MioConvertitore
                tags:
                    - { name: request.param_converter, priority: -2, converter: mio_convertitore }

    .. code-block:: xml

        <service id="mio_convertitore" class="MyBundle\Request\ParamConverter\MioConvertitore">
            <tag name="request.param_converter" priority="-2" converter="mio_convertitore" />
        </service>

Si può registrare un convertitore per priorità, per nome (attributo "converter") o
entrambi. Se non si specifica una priorità o un nome, il convertitore sarà aggiunto
alla pila dei convertitori con priorità `0`. Per disabilitare esplicitamente la
registrazione della priorità, occorre impostare `priority="false"` nella definizione
del tag.

.. tip::

   Se si voglione iniettare servizi o parametri aggiuntivi in un convertitore personalizzato, la priorità non dovrebbe essere
   maggiore di 1. Altrimenti, i servizio non sarà caricato.

.. tip::

   Si può usare la classe ``DoctrineParamConverter`` come modello per i propri convertitori.
