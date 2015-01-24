.. index::
    single: PropertyAccess
    single: Componenti; PropertyAccess

Il componente PropertyAccess
============================

    Il componente PropertyAccess fornisce funzioni per leggere e scrivere da/a
    oggetti o array, usando una semplice notazione stringa.

Installazione
-------------

Si può installare il componente in due modi:

* Installarlo :doc:`tramite Composer</components/using_components>` (``symfony/property-access`` su `Packagist`_);
* Usare il repository Git ufficiale (https://github.com/symfony/PropertyAccess).

Uso
---

Il punto di ingresso di questo componente è il factory
:method:`PropertyAccess::createPropertyAccessor<Symfony\\Component\\PropertyAccess\\PropertyAccess::createPropertyAccessor>`.
Questo factory creerà una nuova istanza della classe
:class:`Symfony\\Component\\PropertyAccess\\PropertyAccessor` con la
configurazione predefinita::

    use Symfony\Component\PropertyAccess\PropertyAccess;

    $accessor = PropertyAccess::createPropertyAccessor();

.. versionadded:: 2.3
    Il metodo :method:`Symfony\\Component\\PropertyAccess\\PropertyAccess::createPropertyAccessor`
    è stato introdotto in Symfony 2.3. In precedenza si chiamava ``getPropertyAccessor()``.

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

Uso di hasser/isser
~~~~~~~~~~~~~~~~~~~

Se non viene trovato un getter, l'accessor cercherà
un isser o un hasser. Tale metodo è creato nello stesso modo dei
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

Metodo magico ``__get()``
~~~~~~~~~~~~~~~~~~~~~~~~~

Infine, ``getValue`` può usare anche il metodo magico ``__get``::

    // ...
    class Person
    {
        private $children = array(
            'Wouter' => array(...),
        );

        public function __get($id)
        {
            return $this->children[$id];
        }
    }

    $person = new Person();

    echo $accessor->getValue($person, 'Wouter'); // array(...)

.. _components-property-access-magic-call:

Metodo magico ``__call()``
~~~~~~~~~~~~~~~~~~~~~~~~~~

Alla fine, ``getValue`` può usare il metodo magico ``__call``, ma occorre abilitare
questa caratteristica, usando :class:`Symfony\\Component\\PropertyAccess\\PropertyAccessorBuilder`::

    // ...
    class Person
    {
        private $children = array(
            'wouter' => array(...),
        );

        public function __call($name, $args)
        {
            $property = lcfirst(substr($name, 3));
            if ('get' === substr($name, 0, 3)) {
                return isset($this->children[$property])
                    ? $this->children[$property]
                    : null;
            } elseif ('set' === substr($name, 0, 3)) {
                $value = 1 == count($args) ? $args[0] : null;
                $this->children[$property] = $value;
            }
        }
    }

    $person = new Person();

    // Abilita __call
    $accessor = PropertyAccess::createPropertyAccessorBuilder()
        ->enableMagicCall()
        ->getPropertyAccessor();

    echo $accessor->getValue($person, 'wouter'); // array(...)

.. versionadded:: 2.3
    L'uso del metodo magico ``__call()`` è stato aggiunto in Symfony 2.3.

.. caution::

    Per impostazione predefinita, ``__call`` è disabilitato, lo si può abilitare richiamando
    :method:`PropertyAccessorBuilder::enableMagicCallEnabled<Symfony\\Component\\PropertyAccess\\PropertyAccessorBuilder::enableMagicCallEnabled>`,
    vedere `Abilitare altre caratteristiche`_.

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

Si può anche usare ``__call`` per impostare valori, ma occorre abilitarlo,
vedere `Abilitare altre caratteristiche`_.

.. code-block:: php

    // ...
    class Person
    {
        private $children = array();

        public function __call($name, $args)
        {
            $property = lcfirst(substr($name, 3));
            if ('get' === substr($name, 0, 3)) {
                return isset($this->children[$property])
                    ? $this->children[$property]
                    : null;
            } elseif ('set' === substr($name, 0, 3)) {
                $value = 1 == count($args) ? $args[0] : null;
                $this->children[$property] = $value;
            }
        }

    }

    $person = new Person();

    // Abilita __call
    $accessor = PropertyAccess::createPropertyAccessorBuilder()
        ->enableMagicCall()
        ->getPropertyAccessor();

    $accessor->setValue($person, 'wouter', array(...));

    echo $person->getWouter(); // array(...)

Verificare i percorsi della proprietà
-------------------------------------

.. versionadded:: 2.5
    I metodi
    :method:`PropertyAccessor::isReadable <Symfony\\Component\\PropertyAccess\\PropertyAccessor::isReadable>`
    e
    :method:`PropertyAccessor::isWritable <Symfony\\Component\\PropertyAccess\\PropertyAccessor::isWritable>`
    sono stati introdotti in Symfony 2.5.

Se si vuole verificare se il metodo
:method:`PropertyAccessor::getValue<Symfony\\Component\\PropertyAccess\\PropertyAccessor::getValue>`
possa essere richamato senza doverlo effettivamente richiamare, si può usare
:method:`PropertyAccessor::isReadable<Symfony\\Component\\PropertyAccess\\PropertyAccessor::isReadable>`::


    $person = new Person();

    if ($accessor->isReadable($person, 'firstName') {
        // ...
    }

Lo stesso è possibile per :method:`PropertyAccessor::setValue<Symfony\\Component\\PropertyAccess\\PropertyAccessor::setValue>`:
Richiamare il metodo
:method:`PropertyAccessor::isWritable<Symfony\\Component\\PropertyAccess\\PropertyAccessor::isWritable>`
per sapere se un percorso di proprietà possa essere aggiornato::

    $person = new Person();

    if ($accessor->isWritable($person, 'firstName') {
        // ...
    }

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
            $this->children = $children;
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

Abilitare altre caratteristiche
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Si può configurare :class:`Symfony\\Component\\PropertyAccess\\PropertyAccessor`
per abilitare caratteristiche extra. Per poterlo fare, si può usare
:class:`Symfony\\Component\\PropertyAccess\\PropertyAccessorBuilder`::

    // ...
    $accessorBuilder = PropertyAccess::createPropertyAccessorBuilder();

    // Abilita __call
    $accessorBuilder->enableMagicCall();

    // Disabilita __call
    $accessorBuilder->disableMagicCall();

    // Verifica se la gestione di __call è abilitata
    $accessorBuilder->isMagicCallEnabled() // true o false

    // Alla fine ottiene l'accessor alla proprietà configurato
    $accessor = $accessorBuilder->getPropertyAccessor();

    // Oppure tutto insieme
    $accessor = PropertyAccess::createPropertyAccessorBuilder()
        ->enableMagicCall()
        ->getPropertyAccessor();

Oppure si possono passsare parametri direttamente al costruttore (non raccomandato)::

    // ...
    $accessor = new PropertyAccessor(true) // abilita la gestione di __call


.. _Packagist: https://packagist.org/packages/symfony/property-access
