Blank
=====

Valida che un valore sia vuoto, definito come uguale alla stringa vuota o uguale
a ``null``. Per forzare un valore per essere strettamente uguale a ``null``, vedere
il vincolo :doc:`/reference/constraints/Null`. Per forzare un valore a *non* essere
vuoto, vedere :doc:`/reference/constraints/NotBlank`.

+----------------+-----------------------------------------------------------------------+
| Si applica a   | :ref:`proprietà o metodo <validation-property-target>`                |
+----------------+-----------------------------------------------------------------------+
| Opzioni        | - `message`_                                                          |
+----------------+-----------------------------------------------------------------------+
| Classe         | :class:`Symfony\\Component\\Validator\\Constraints\\Blank`            |
+----------------+-----------------------------------------------------------------------+
| Validatore     | :class:`Symfony\\Component\\Validator\\Constraints\\BlankValidator`   |
+----------------+-----------------------------------------------------------------------+

Uso di base
-----------

Se, per qualche ragione, ci si vuole assicurare che la proprietà ``firstName`` di una
classe ``Author`` sia vuota, si può fare come segue:

.. configuration-block::

    .. code-block:: php-annotations

        // src/Acme/BlogBundle/Entity/Author.php
        namespace Acme\BlogBundle\Entity;

        use Symfony\Component\Validator\Constraints as Assert;

        class Author
        {
            /**
             * @Assert\Blank()
             */
            protected $firstName;
        }

    .. code-block:: yaml

        # src/Acme/BlogBundle/Resources/config/validation.yml
        Acme\BlogBundle\Entity\Author:
            properties:
                firstName:
                    - Blank: ~

    .. code-block:: xml

        <!-- src/Acme/BlogBundle/Resources/config/validation.xml -->
        <?xml version="1.0" encoding="UTF-8" ?>
        <constraint-mapping xmlns="http://symfony.com/schema/dic/constraint-mapping"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://symfony.com/schema/dic/constraint-mapping http://symfony.com/schema/dic/constraint-mapping/constraint-mapping-1.0.xsd">

            <class name="Acme\BlogBundle\Entity\Author">
                <property name="firstName">
                    <constraint name="Blank" />
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
                $metadata->addPropertyConstraint('firstName', new Assert\Blank());
            }
        }

Opzioni
-------

message
~~~~~~~

**tipo**: ``stringa`` **predefinito**: ``This value should be blank``

Messaggio che sarà mostrato se il valore non è vuoto.
