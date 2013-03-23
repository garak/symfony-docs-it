Valid
=====

Questo vincolo è usato per abilitare la validazione su oggetti che sono inseriti
come proprietà su un oggetto in corso di validazione. Consente di validare un oggetto
e tutti i sotto-oggetti a esso associati.

+----------------+---------------------------------------------------------------------+
| Si applica a   | :ref:`proprietà o metodo<validation-property-target>`               |
+----------------+---------------------------------------------------------------------+
| Opzioni        | - `traverse`_                                                       |
+----------------+---------------------------------------------------------------------+
| Classe         | :class:`Symfony\\Component\\Validator\\Constraints\\Type`           |
+----------------+---------------------------------------------------------------------+

Uso di base
-----------

Nel seguente esempio, vengono create le due classi ``Author`` e ``Address``,
entrambe con vincoli sulle loro proprietà. Inoltre, ``Author`` memorizza
un'istanza di ``Address`` nella proprietà ``$address``.

.. code-block:: php

    // src/Acme/HelloBundle/Entity/Address.php
    namespace Amce\HelloBundle\Entity;

    class Address
    {
        protected $street;
        protected $zipCode;
    }

.. code-block:: php

    // src/Acme/HelloBundle/Entity/Author.php
    namespace Amce\HelloBundle\Entity;

    class Author
    {
        protected $firstName;
        protected $lastName;
        protected $address;
    }

.. configuration-block::

    .. code-block:: yaml

        # src/Acme/HelloBundle/Resources/config/validation.yml
        Acme\HelloBundle\Entity\Address:
            properties:
                street:
                    - NotBlank: ~
                zipCode:
                    - NotBlank: ~
                    - Length:
                        max: 5

        Acme\HelloBundle\Entity\Author:
            properties:
                firstName:
                    - NotBlank: ~
                    - Length:
                        min: 4
                lastName:
                    - NotBlank: ~

    .. code-block:: php-annotations

        // src/Acme/HelloBundle/Entity/Address.php
        namespace Acme\HelloBundle\Entity;

        use Symfony\Component\Validator\Constraints as Assert;

        class Address
        {
            /**
             * @Assert\NotBlank()
             */
            protected $street;

            /**
             * @Assert\NotBlank
             * @Assert\Length(max = "5")
             */
            protected $zipCode;
        }

        // src/Acme/HelloBundle/Entity/Author.php
        namespace Acme\HelloBundle\Entity;

        class Author
        {
            /**
             * @Assert\NotBlank
             * @Assert\Length(min = "4")
             */
            protected $firstName;

            /**
             * @Assert\NotBlank
             */
            protected $lastName;
            
            protected $address;
        }

    .. code-block:: xml

        <!-- src/Acme/HelloBundle/Resources/config/validation.xml -->
        <class name="Acme\HelloBundle\Entity\Address">
            <property name="street">
                <constraint name="NotBlank" />
            </property>
            <property name="zipCode">
                <constraint name="NotBlank" />
                <constraint name="MaxLength">5</constraint>
            </property>
        </class>

        <class name="Acme\HelloBundle\Entity\Author">
            <property name="firstName">
                <constraint name="NotBlank" />
                <constraint name="MinLength">4</constraint>
            </property>
            <property name="lastName">
                <constraint name="NotBlank" />
            </property>
        </class>

    .. code-block:: php

        // src/Acme/HelloBundle/Entity/Address.php
        namespace Acme\HelloBundle\Entity;

        use Symfony\Component\Validator\Mapping\ClassMetadata;
        use Symfony\Component\Validator\Constraints as Assert;
        
        class Address
        {
            protected $street;
            protected $zipCode;
            
            public static function loadValidatorMetadata(ClassMetadata $metadata)
            {
                $metadata->addPropertyConstraint('street', new Assert\NotBlank());
                $metadata->addPropertyConstraint('zipCode', new Assert\NotBlank());
                $metadata->addPropertyConstraint(
                    'zipCode',
                    new Assert\Length(array("max" => 5)));
            }
        }

        // src/Acme/HelloBundle/Entity/Author.php
        namespace Acme\HelloBundle\Entity;

        use Symfony\Component\Validator\Mapping\ClassMetadata;
        use Symfony\Component\Validator\Constraints as Assert;
        
        class Author
        {
            protected $firstName;
            protected $lastName;           
            protected $address;
            
            public static function loadValidatorMetadata(ClassMetadata $metadata)
            {
                $metadata->addPropertyConstraint('firstName', new Assert\NotBlank());
                $metadata->addPropertyConstraint('firstName', new Assert\Length(array("min" => 4)));
                $metadata->addPropertyConstraint('lastName', new Assert\NotBlank());
            }
        }

Con questa mappatura, è possibile validare con successo un autore con un indirizzo
non valido. Per prevenire questo problema, aggiungere il vincolo ``Valid`` alla
proprietà ``$address``.

.. configuration-block::

    .. code-block:: yaml

        # src/Acme/HelloBundle/Resources/config/validation.yml
        Acme\HelloBundle\Author:
            properties:
                address:
                    - Valid: ~

    .. code-block:: php-annotations

        // src/Acme/HelloBundle/Entity/Author.php
        namespace Acme\HelloBundle\Entity;

        use Symfony\Component\Validator\Constraints as Assert;

        class Author
        {
            /**
             * @Assert\Valid
             */
            protected $address;
        }

    .. code-block:: xml

        <!-- src/Acme/HelloBundle/Resources/config/validation.xml -->
        <class name="Acme\HelloBundle\Entity\Author">
            <property name="address">
                <constraint name="Valid" />
            </property>
        </class>

    .. code-block:: php

        // src/Acme/HelloBundle/Entity/Author.php
        namespace Acme\HelloBundle\Entity;

        use Symfony\Component\Validator\Mapping\ClassMetadata;
        use Symfony\Component\Validator\Constraints as Assert;
        
        class Author
        {
            protected $address;
            
            public static function loadValidatorMetadata(ClassMetadata $metadata)
            {
                $metadata->addPropertyConstraint('address', new Assert\Valid());
            }
        }

Se ora si valida un autore con indirizzo non valido, si può vedere che la
validazione dei campi ``Address`` non passa.

    Acme\HelloBundle\Author.address.zipCode:
    This value is too long. It should have 5 characters or less

Opzioni
-------

traverse
~~~~~~~~

**tipo**: ``stringa`` **predefinito**: ``true``

Se questo vincolo è applicato a una proprietà che contiene un array di oggetti,
allora ogni oggetto in tale array sarà validato solo se questa opzione è
``true``.
