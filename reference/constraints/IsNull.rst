Null
====

Valida che un valore sia esattamente uguale a ``null``. Per forzare una proprietà a essere
vuota (stringa vuota o ``null``), vedere il vincolo :doc:`/reference/constraints/Blank`.
Per assicurarsi che una proprietà non sia ``null``, vedere :doc:`/reference/constraints/NotNull`.

Vedere anche :doc:`NotNull <NotNull>`.

+----------------+-----------------------------------------------------------------------+
| Si applica a   | :ref:`proprietà o metodo<validation-property-target>`                 |
+----------------+-----------------------------------------------------------------------+
| Opzioni        | - `message`_                                                          |
|                | - `payload`_                                                          |
+----------------+-----------------------------------------------------------------------+
| Classe         | :class:`Symfony\\Component\\Validator\\Constraints\\IsNull`           |
+----------------+-----------------------------------------------------------------------+
| Validatore     | :class:`Symfony\\Component\\Validator\\Constraints\\IsNullValidator`  |
+----------------+-----------------------------------------------------------------------+

Uso di base
-----------

Se, per qualche ragione, ci si vuole assicurare che la proprietà ``firstName`` di
una classe ``Author`` sia esttamente uguale a ``null``, si può fare come segue:

.. configuration-block::

    .. code-block:: php-annotations

        // src/AppBundle/Entity/Author.php
        namespace AppBundle\Entity;

        use Symfony\Component\Validator\Constraints as Assert;

        class Author
        {
            /**
             * @Assert\IsNull()
             */
            protected $firstName;
        }

    .. code-block:: yaml

        # src/AppBundle/Resources/config/validation.yml
        AppBundle\Entity\Author:
            properties:
                firstName:
                    - 'IsNull': ~

    .. code-block:: xml

        <!-- src/AppBundle/Resources/config/validation.xml -->
        <?xml version="1.0" encoding="UTF-8" ?>
        <constraint-mapping xmlns="http://symfony.com/schema/dic/constraint-mapping"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://symfony.com/schema/dic/constraint-mapping http://symfony.com/schema/dic/constraint-mapping/constraint-mapping-1.0.xsd">

            <class name="AppBundle\Entity\Author">
                <property name="firstName">
                    <constraint name="IsNull" />
                </property>
            </class>
        </constraint-mapping>

    .. code-block:: php

        // src/AppBundle/Entity/Author.php
        namespace AppBundle\Entity;

        use Symfony\Component\Validator\Mapping\ClassMetadata;
        use Symfony\Component\Validator\Constraints as Assert;

        class Author
        {
            public static function loadValidatorMetadata(ClassMetadata $metadata)
            {
                $metadata->addPropertyConstraint('firstName', Assert\IsNull());
            }
        }

Opzioni
-------

message
~~~~~~~

**tipo**: ``stringa`` **predefinito**: ``This value should be null``

Messaggio mostrato se il valore non è ``null``.

.. include:: /reference/constraints/_payload-option.rst.inc
