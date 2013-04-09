.. index::
    single: PropertyAccess
    single: Componenti; PropertyAccess

Il componente PropertyAccess
============================

    Il componente PropertyAccess fornisce funzioni per leggere e scrivere da/a
    oggetti o array, usando una semplice notazione stringa.

.. versionadded:: 2.2
    Il componente PropertyAccess è nuovo in Symfony 2.2. In precedenza, la
    classe ``PropertyPath`` si trovava nel componente ``Form``.

Installazione
-------------

Si può installare il componente in due modi:

* Usare il repository Git ufficiale (https://github.com/symfony/PropertyAccess);
* Installarlo :doc:`tramite Composer</components/using_components>` (``symfony/property-access`` su `Packagist`_).

Uso
---

Il punto di ingresso di questo componente è il factory
:method:`PropertyAccess::getPropertyAccessor<Symfony\\Component\\PropertyAccess\\PropertyAccess::getPropertyAccessor>`.
Questo factory creerà una nuova istanza della classe
:class:`Symfony\\Component\\PropertyAccess\\PropertyAccessor` con la
configurazione predefinita::

    use Symfony\Component\PropertyAccess\PropertyAccess;

    $accessor = PropertyAccess::getPropertyAccessor();

Lettura da array
----------------

Si può leggere un array con il metodo
:method:`PropertyAccessor::getValue<Symfony\\Component\\PropertyAccess\\PropertyAccessor::getValue>`.
Lo si può fare usando la notazione indice, usata in PHP::

    // ...
    $person = array(
        'first_name' => 'Wouter',
    );

    echo $accessor->getValue($person, '[first_name]'); // 'Wouter'
    echo $accessor->getValue($person, '[age]'); // null

Come si può vedere, il metodo restituisce ``null`` quando l'indice non esiste.

Si possono usare anche array a più dimensioni::

    // ...
    $persons = array(
        array(
            'first_name' => 'Wouter',
        ),
        array(
            'first_name' => 'Ryan',
        )
    );

    echo $accessor->getValue($persons, '[0][first_name]'); // 'Wouter'
    echo $accessor->getValue($persons, '[1][first_name]'); // 'Ryan'

Lettura da oggetti
------------------

Il metodo ``getValue`` è molto robusto, lo si può vedere quando
si ha a che fare con oggetti.

Accesso a proprietà pubbliche
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Per leggere da proprietà, usare la notazione con il punto::

    // ...
    $person = new Person();
    $person->firstName = 'Wouter';

    echo $accessor->getValue($person, 'firstName'); // 'Wouter'

    $child = new Person();
    $child->firstName = 'Pluto';
    $person->children = array($child);

    echo $accessor->getValue($person, 'children[0].firstName'); // 'Pluto'

.. caution::

    L'accesso a proprietà pubbliche è l'ultima opzione usata da ``PropertyAccessor``.
    Prima prova ad accedere al valore usando i metodi, prima di usare
    direttamente la proprietà. Per esempio, se si ha una proprietà pubblica con
    un metodo gettere, sarà usato il getter.

Uso dei getter
~~~~~~~~~~~~~~

Il metodo ``getValue`` supporta anche la lettura tramite getter. Il metodo
sarà creato usando le comuni convenzioni di nomenclatura dei getter. Mette in
maiuscolo il nome (``first_name`` diventa ``FirstName``) e aggiunge il prefisso
``get``. Il metodo diventa quindi ``getFirstName``::

    // ...
    class Person
    {
        private $firstName = 'Wouter';

        public function getFirstName()
        {
            return $this->firstName;
        }
    }

    $person = new Person();

    echo $accessor->getValue($person, 'first_name'); // 'Wouter'

Uso di Hasser/Isser
~~~~~~~~~~~~~~~~~~~

Se non viene trovato un getter, l'accessor cercherà
un isser o un hasser. Tale metodo è creto nello stesso modo dei
getter, quindi si può fare qualcosa come::

    // ...
    class Person
    {
        private $author = true;
        private $children = array();

        public function isAuthor()
        {
            return $this->author;
        }

        public function hasChildren()
        {
            return 0 !== count($this->children);
        }
    }

    $person = new Person();

    if ($accessor->getValue($person, 'author')) {
        echo 'È un autore';
    }
    if ($accessor->getValue($person, 'children')) {
        echo 'Ha dei figli';
    }

Produrrà: ``È un autore``

Metodi magici
~~~~~~~~~~~~~

Infine, ``getValue`` può usare anche il metodo magico ``__get``::

    // ...
    class Person
    {
        private $children = array(
            'wouter' => array(...),
        );

        public function __get($id)
        {
            return $this->children[$id];
        }
    }

    $person = new Person();

    echo $accessor->getValue($person, 'Wouter'); // array(...)

Scrittura su array
------------------

La classe ``PropertyAccessor`` può far più che leggere semplicemente un array, può
anche scrivere in un array. Lo si può fare usando il metodo
:method:`PropertyAccessor::setValue<Symfony\\Component\\PropertyAccess\\PropertyAccessor::setValue>`::


    // ...
    $person = array();

    $accessor->setValue($person, '[first_name]', 'Wouter');

    echo $accessor->getValue($person, '[first_name]'); // 'Wouter'
    // oppure
    // echo $person['first_name']; // 'Wouter'

Scrittura su oggetti
--------------------

Il metodo ``setValue`` ha le stesse caratteristiche del metodo ``getValue``. Si possono
usare i setter, il metodo magico ``__set`` o le proprietà, per impostare i valori::

    // ...
    class Person
    {
        public $firstName;
        private $lastName;
        private $children = array();

        public function setLastName($name)
        {
            $this->lastName = $name;
        }

        public function __set($property, $value)
        {
            $this->$property = $value;
        }

        // ...
    }

    $person = new Person();
    
    $accessor->setValue($person, 'firstName', 'Wouter');
    $accessor->setValue($person, 'lastName', 'de Jong');
    $accessor->setValue($person, 'children', array(new Person()));

    echo $person->firstName; // 'Wouter'
    echo $person->getLastName(); // 'de Jong'
    echo $person->children; // array(Person());

Mischiare oggetti e array
-------------------------

Si possono anche mischiare oggetti e array::

    // ...
    class Person
    {
        public $firstName;
        private $children = array();

        public function setChildren($children)
        {
            return $this->children;
        }

        public function getChildren()
        {
            return $this->children;
        }
    }

    $person = new Person();

    $accessor->setValue($person, 'children[0]', new Person);
    // equivale a $person->getChildren()[0] = new Person()

    $accessor->setValue($person, 'children[0].firstName', 'Wouter');
    // equivale a $person->getChildren()[0]->firstName = 'Wouter'

    echo 'Hello '.$accessor->getValue($person, 'children[0].firstName'); // 'Wouter'
    // equivale a $person->getChildren()[0]->firstName

.. _Packagist: https://packagist.org/packages/symfony/property-access
