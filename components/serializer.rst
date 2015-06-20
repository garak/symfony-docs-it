.. index::
   single: Serializer 
   single: Componenti; Serializer

Il componente Serializer
========================

   Il componente Serializer è pensato per essere usato per trasformare oggetti
   in formati specifici (XML, JSON, Yaml, ...) e viceversa.

Per raggiungere questo scopo, il componente Serializer segue il semplice
schema seguente.

.. _component-serializer-encoders:
.. _component-serializer-normalizers:

.. image:: /images/components/serializer/serializer_workflow.png

Come si può vedere nell'immagine, viene usato un array come tramite.
In questo modo, gli Encoder si occuperanno solo di trasformare specifici
**formati** in **arrays** e viceversa. Allo stesso modo, i Normalizer
si occuperanno di trasformare specifici **oggetti** in **arrays** e viceversa.

La serializzazione è un argomento complesso e, sebbene questo componente non possa
risolvere tutti i casi, può essere uno strumento utile per lo sviluppo di strumenti
che serializzano e deserializzano gli oggetti.

Installazione
-------------

Si può installare il componente in due modi:

* :doc:`Installarlo via Composer</components/using_components>` (``symfony/serializer`` su `Packagist`_);
* Usare il repository ufficiale su Git (https://github.com/symfony/Serializer).

.. include:: /components/require_autoload.rst.inc

Uso
---

L'uso del componente Serializer è molto semplice. Basta impostare la
classe :class:`Symfony\\Component\\Serializer\\Serializer`, specificando
quali Encoder e Normalizer saranno disponibili::

    use Symfony\Component\Serializer\Serializer;
    use Symfony\Component\Serializer\Encoder\XmlEncoder;
    use Symfony\Component\Serializer\Encoder\JsonEncoder;
    use Symfony\Component\Serializer\Normalizer\GetSetMethodNormalizer;

    $encoders = array(new XmlEncoder(), new JsonEncoder());
    $normalizers = array(new GetSetMethodNormalizer());

    $serializer = new Serializer($normalizers, $encoders);

Serializzare un oggetto
~~~~~~~~~~~~~~~~~~~~~~~

Per questo esempio, ipotizziamo che un progetto disponga della
classe seguente::

    namespace Acme;

    class Person
    {
        private $age;
        private $name;

        // Getter
        public function getName()
        {
            return $this->name;
        }

        public function getAge()
        {
            return $this->age;
        }

        // Setter
        public function setName($name)
        {
            $this->name = $name;
        }

        public function setAge($age)
        {
            $this->age = $age;
        }
    }

Se ora vogliamo serializzare questo oggetto in JSON, ci basta usare
il servizio Serializer creato in precedenza::

    $person = new Acme\Person();
    $person->setName('pippo');
    $person->setAge(99);

    $serializer->serialize($person, 'json');

    // $jsonContent contiene {"name":"pippo","age":99}

    echo $jsonContent; // o restituirlo in una risposta

Il primo parametro di :method:`Symfony\\Component\\Serializer\\Serializer::serialize`
è l'oggetto da serializzare e il secondo è usato per scegliere l'Encoder giusto,
in questo caso :class:`Symfony\\Component\\Serializer\\Encoder\\JsonEncoder`.

Ignorare attributi durante la serializzazione
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

.. versionadded:: 2.3
    Il metodo :method:`GetSetMethodNormalizer::setIgnoredAttributes<Symfony\\Component\\Serializer\\Normalizer\\GetSetMethodNormalizer::setIgnoredAttributes>`
    è stato introdotto in Symfony 2.3.

C'è un modo opzionale per ignorare attributi dall'oggetto originario, durante la
serializzazione. Per rimuovere attributi, usare il metodo
:method:`Symfony\\Component\\Serializer\\Normalizer\\GetSetMethodNormalizer::setIgnoredAttributes`
nella definizione del normalizzatore::

    use Symfony\Component\Serializer\Serializer;
    use Symfony\Component\Serializer\Encoder\JsonEncoder;
    use Symfony\Component\Serializer\Normalizer\GetSetMethodNormalizer;

    $normalizer = new GetSetMethodNormalizer();
    $normalizer->setIgnoredAttributes(array('age'));
    $encoder = new JsonEncoder();

    $serializer = new Serializer(array($normalizer), array($encoder));
    $serializer->serialize($person, 'json'); // Output: {"name":"foo"}

Deserializzare un oggetto
~~~~~~~~~~~~~~~~~~~~~~~~~

Vediamo ora l'operazione inversa. Questa volta, l'informazione della classe
`People` sarà codificata in formato in XML::

    $data = <<<EOF
    <person>
        <name>pippo</name>
        <age>99</age>
    </person>
    EOF;

    $person = $serializer->deserialize($data,'Acme\Person','xml');

In questo caso, :method:`Symfony\\Component\\Serializer\\Serializer::deserialize`
ha bisogno di tre parametri:

1. l'informazione da decodificare
2. il nome della classe in cui questa informazione sarà decodificata
3. l'Encoder usato per convertire questa informazione in un array

Usare nomi in CamelCase per attributi con trattini bassi
--------------------------------------------------------

.. versionadded:: 2.3
    Il metodo :method:`GetSetMethodNormalizer::setCamelizedAttributes<Symfony\\Component\\Serializer\\Normalizer\\GetSetMethodNormalizer::setCamelizedAttributes>`
    è stato aggiunto in Symfony 2.3.

A volte i nomi di proprietà del contenuto serializzato hanno trattini bassi (p.e.
``first_name``).  Di solito, questi attributi usano metodi get o set come
``getFirst_name``, mentre quello che si vuole è ``getFirstName``. Per cambiare
questo comportamento, usare il metodo
:method:`Symfony\\Component\\Serializer\\Normalizer\\GetSetMethodNormalizer::setCamelizedAttributes`
nella definizione del normalizzatore::

    $encoder = new JsonEncoder();
    $normalizer = new GetSetMethodNormalizer();
    $normalizer->setCamelizedAttributes(array('first_name'));

    $serializer = new Serializer(array($normalizer), array($encoder));

    $json = <<<EOT
    {
        "name":       "pippo",
        "age":        "19",
        "first_name": "pluto"
    }
    EOT;

    $person = $serializer->deserialize($json, 'Acme\Person', 'json');

Come risultato, il deserializzatore usa l'attributo ``first_name`` come se fosse
stato ``firstName`` e quindi usa i metodi ``getFirstName`` e ``setFirstName``.

Usare callback per serializzare proprietà con instanze di oggetti
-----------------------------------------------------------------

Quando si serializza, si può impostare un callback per formattare una determinata proprietà di un oggetto::

    use Acme\Person;
    use Symfony\Component\Serializer\Encoder\JsonEncoder;
    use Symfony\Component\Serializer\Normalizer\GetSetMethodNormalizer;
    use Symfony\Component\Serializer\Serializer;

    $encoder = new JsonEncoder();
    $normalizer = new GetSetMethodNormalizer();

    $callback = function ($dateTime) {
        return $dateTime instanceof \DateTime
            ? $dateTime->format(\DateTime::ISO8601)
            : '';
    };

    $normalizer->setCallbacks(array('createdAt' => $callback));

    $serializer = new Serializer(array($normalizer), array($encoder));

    $person = new Person();
    $person->setName('cordoval');
    $person->setAge(34);
    $person->setCreatedAt(new \DateTime('now'));

    $serializer->serialize($person, 'json');
    // Output: {"name":"cordoval", "age": 34, "createdAt": "2014-03-22T09:43:12-0500"}

JMSSerializer
-------------

Una popolare libreria, `JMS serializer`_, fornisce una soluzione
più sofisticata, sebbene più complessa. La libreria include la
possibilità di configurare il modo in cui gli oggetto debbano essere serializzati/deserializzati
tramite annotazioni (oltre che YML, XML e PHP), integrazione con l'ORM di Doctrine
e gestione di altri casi complessi (p.e. riferimenti circolari).

.. _`JMS serializer`: https://github.com/schmittjoh/serializer
.. _Packagist: https://packagist.org/packages/symfony/serializer
