Regex
=====

Valida che un valore corrisponda a un'espressione regolare.

+----------------+-----------------------------------------------------------------------+
| Si applica a   | :ref:`proprietà o metodo<validation-property-target>`                 |
+----------------+-----------------------------------------------------------------------+
| Opzioni        | - `pattern`_                                                          |
|                | - `htmlPattern`_                                                      |
|                | - `match`_                                                            |
|                | - `message`_                                                          |
+----------------+-----------------------------------------------------------------------+
| Classe         | :class:`Symfony\\Component\\Validator\\Constraints\\Regex`            |
+----------------+-----------------------------------------------------------------------+
| Validatore     | :class:`Symfony\\Component\\Validator\\Constraints\\RegexValidator`   |
+----------------+-----------------------------------------------------------------------+

Uso di base
-----------

Si supponga di avere un campo ``description`` e di voler verificare che inizi con un
carattere alfanumerico valido. L'espressione regolare da testare sarebbe
``/^\w+/``, che indica che si sta cercando almeno uno o più caratteri alfanumerici
all'inizio della stringa:

.. configuration-block::

    .. code-block:: yaml

        # src/Acme/BlogBundle/Resources/config/validation.yml
        Acme\BlogBundle\Entity\Author:
            properties:
                description:
                    - Regex: "/^\w+/"

    .. code-block:: php-annotations

        // src/Acme/BlogBundle/Entity/Author.php
        namespace Acme\BlogBundle\Entity;

        use Symfony\Component\Validator\Constraints as Assert;

        class Author
        {
            /**
             * @Assert\Regex("/^\w+/")
             */
            protected $description;
        }

    .. code-block:: xml

        <!-- src/Acme/BlogBundle/Resources/config/validation.xml -->
        <?xml version="1.0" encoding="UTF-8" ?>
        <constraint-mapping xmlns="http://symfony.com/schema/dic/constraint-mapping"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://symfony.com/schema/dic/constraint-mapping http://symfony.com/schema/dic/constraint-mapping/constraint-mapping-1.0.xsd">

            <class name="Acme\BlogBundle\Entity\Author">
                <property name="description">
                    <constraint name="Regex">
                        <option name="pattern">/^\w+/</option>
                    </constraint>
                </property>
            </class>
        </constraint-mapping>

    .. code-block:: php

        // src/Acme/BlogBundle/Entity/Author.php
        namespace Acme\BlogBundle\Entity;

        use Symfony\Component\Validator\Mapping\ClassMetadata;
        use Symfony\Component\Validator\Constraints as Assert;

        class Author
        {
            public static function loadValidatorMetadata(ClassMetadata $metadata)
            {
                $metadata->addPropertyConstraint('description', new Assert\Regex(array(
                    'pattern' => '/^\w+/',
                )));
            }
        }

In alternativa, si può impostare l'opzione `match`_ a ``false``, per asserire che
una stringa data *non* debba corrispondere. Nell'esempio seguente, si asserisce che
il campo ``firstName`` non contenga numeri e si imposta un messaggio
personalizzato:

.. configuration-block::

    .. code-block:: yaml

        # src/Acme/BlogBundle/Resources/config/validation.yml
        Acme\BlogBundle\Entity\Author:
            properties:
                firstName:
                    - Regex:
                        pattern: "/\d/"
                        match:   false
                        message: Il nome non può contenere numeri

    .. code-block:: php-annotations

        // src/Acme/BlogBundle/Entity/Author.php
        namespace Acme\BlogBundle\Entity;

        use Symfony\Component\Validator\Constraints as Assert;

        class Author
        {
            /**
             * @Assert\Regex(
             *     pattern="/\d/",
             *     match=false,
             *     message="Il nome non può contenere numeri"
             * )
             */
            protected $firstName;
        }

    .. code-block:: xml

        <!-- src/Acme/BlogBundle/Resources/config/validation.xml -->
        <?xml version="1.0" encoding="UTF-8" ?>
        <constraint-mapping xmlns="http://symfony.com/schema/dic/constraint-mapping"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://symfony.com/schema/dic/constraint-mapping http://symfony.com/schema/dic/constraint-mapping/constraint-mapping-1.0.xsd">

            <class name="Acme\BlogBundle\Entity\Author">
                <property name="firstName">
                    <constraint name="Regex">
                        <option name="pattern">/\d/</option>
                        <option name="match">false</option>
                        <option name="message">Il nome non può contenere numeri</option>
                    </constraint>
                </property>
            </class>
        </constraint-mapping>

    .. code-block:: php

        // src/Acme/BlogBundle/Entity/Author.php
        namespace Acme\BlogBundle\Entity;

        use Symfony\Component\Validator\Mapping\ClassMetadata;
        use Symfony\Component\Validator\Constraints as Assert;

        class Author
        {
            public static function loadValidatorMetadata(ClassMetadata $metadata)
            {
                $metadata->addPropertyConstraint('firstName', new Assert\Regex(array(
                    'pattern' => '/\d/',
                    'match'   => false,
                    'message' => 'Il nome non può contenere numeri',
                )));
            }
        }

Opzioni
-------

pattern
~~~~~~~

**tipo**: ``stringa`` [:ref:`opzione predefinita<validation-default-option>`]

Questa opzione obbligatoria è l'espressione regolare a cui il valore inserito deve
corrispondere. Per impostazione predefinita, il validatore fallisce se la stringa inserita
*non* corrisponde a questa espressione regolare (tramite la funzione :phpfunction:`preg_match` di PHP).
Se tuttavia `match`_ è ``false``, la validazione fallisce se la stringa inserita
*corrisponde* a questo schema.

htmlPattern
~~~~~~~~~~~

.. versionadded:: 2.1
    L'opzione ``htmlPattern`` è stata aggiunta in Symfony 2.1

**tipo**: ``stringa|booleano`` **predefinito**: null

Questa opzione specifica lo schema da usare nell'attributo HTML5 ``pattern``.
Solitamente non si ha bisogno di specificare tale opzione, perché, per impostazione predefinita, il vincolo
convertirà lo schema fornito nell'opzione `pattern`_ in uno schema compatibile con HTML5.
Questo vuol dire che i delimitatori sono rimossi (p.e. ``/[a-z]+/`` diventa ``[a-z]+``).

Tuttavia, ci sono alcune incompatibilità, tra i due schemi, che non possono
essere risolte dal vincolo. Per esempio, l'attributo HTML5 ``pattern``
non supporta i flag. Se si ha uno schema come ``/[a-z]+/i``, occorre
specificare lo schema compatibile HTML5 nell'opzione ``htmlPattern``:

.. configuration-block::

    .. code-block:: yaml

        # src/Acme/BlogBundle/Resources/config/validation.yml
        Acme\BlogBundle\Entity\Author:
            properties:
                name:
                    - Regex:
                        pattern: "/^[a-z]+$/i"
                        htmlPattern: "^[a-zA-Z]+$"

    .. code-block:: php-annotations

        // src/Acme/BlogBundle/Entity/Author.php
        namespace Acme\BlogBundle\Entity;

        use Symfony\Component\Validator\Constraints as Assert;

        class Author
        {
            /**
             * @Assert\Regex(
             *     pattern     = "/^[a-z]+$/i",
             *     htmlPattern = "^[a-zA-Z]+$"
             * )
             */
            protected $name;
        }

    .. code-block:: xml

        <!-- src/Acme/BlogBundle/Resources/config/validation.xml -->
        <?xml version="1.0" encoding="UTF-8" ?>
        <constraint-mapping xmlns="http://symfony.com/schema/dic/constraint-mapping"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://symfony.com/schema/dic/constraint-mapping http://symfony.com/schema/dic/constraint-mapping/constraint-mapping-1.0.xsd">

            <class name="Acme\BlogBundle\Entity\Author">
                <property name="name">
                    <constraint name="Regex">
                        <option name="pattern">/^[a-z]+$/i</option>
                        <option name="htmlPattern">^[a-zA-Z]+$</option>
                    </constraint>
                </property>
            </class>
        </constraint-mapping>

    .. code-block:: php

        // src/Acme/BlogBundle/Entity/Author.php
        namespace Acme\BlogBundle\Entity;

        use Symfony\Component\Validator\Mapping\ClassMetadata;
        use Symfony\Component\Validator\Constraints as Assert;

        class Author
        {
            public static function loadValidatorMetadata(ClassMetadata $metadata)
            {
                $metadata->addPropertyConstraint('name', new Assert\Regex(array(
                    'pattern'     => '/^[a-z]+$/i',
                    'htmlPattern' => '^[a-zA-Z]+$',
                )));
            }
        }

Impostando ``htmlPattern`` a ``false``, disabiliterà la validazione lato client.

match
~~~~~

**tipo**: ``booleano`` default: ``true``

Se ``true`` (o non impostato), questo validatore passerà se la stringa data
corrisponde all'espressione regolare contenuta in `pattern`_. Se invece l'opzione è
``false``, sarà il contrario: la validazione passerà solo se la stringa data
**non** corrisponderà all'espressione regolare contenuta in `pattern`_.

message
~~~~~~~

**tipo**: ``stringa`` **predefinito**: ``This value is not valid``

Messaggio mostrato se il validatore fallisce.
