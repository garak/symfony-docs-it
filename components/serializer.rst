.. index::
   single: Serializer 
   single: Componenti; Serializer

Il componente Serializer
========================

   Il componente Serializer è pensato per essere usato per trasformare oggetti
   in formati specifici (XML, JSON, Yaml, ...) e viceversa.

Per raggiungere questo scopo, il componente Serializer segue il semplice
schema seguente.

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

Si può installare il componente in molti modi diversi:

* Usare il repository ufficiale su Git (https://github.com/symfony/Serializer);
* :doc:`Installarlo via Composer</components/using_components>` (``symfony/serializer`` su `Packagist`_).

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
    $person->setName('foo');
    $person->setAge(99);

    $serializer->serialize($person, 'json'); // Output: {"name":"foo","age":99}

Il primo parametro di :method:`Symfony\\Component\\Serializer\\Serializer::serialize`
è l'oggetto da serializzare e il secondo è usato per scegliere l'Encoder giusto,
in questo caso :class:`Symfony\\Component\\Serializer\\Encoder\\JsonEncoder`.

Deserializzare un oggetto
~~~~~~~~~~~~~~~~~~~~~~~~~

Vediamo ora l'operazione inversa. Questa volta, l'informazione della classe
`People` sarà codificata in formato in XML::

    $data = <<<EOF
    <person>
        <name>foo</name>
        <age>99</age>
    </person>
    EOF;

    $person = $serializer->deserialize($data,'Acme\Person','xml');

In questo caso, :method:`Symfony\\Component\\Serializer\\Serializer::deserialize`
ha bisogno di tre parametri:

1. l'informazione da decodificare
2. il nome della classe in cui questa informazione sarà decodificata
3. l'Encoder usato per convertire questa informazione in un array

JMSSerializationBundle
----------------------

Esiste un popolare bundle, `JMSSerializationBundle`_, che estende
(e a volte sostituisce) la funzionalità della serializzazione. Questo include la
possibilità di configurare il modo in cui gli oggetto debbano essere serializzati/deserializzati
tramite annotazioni (oltre che YML, XML e PHP), integrazione con l'ORM di Doctrine
e gestione di altri casi complessi (p.e. riferimenti circolari).

.. _`JMSSerializationBundle`: https://github.com/schmittjoh/JMSSerializerBundle
.. _Packagist: https://packagist.org/packages/symfony/serializer
