LessThan
========

.. versionadded:: 2.3
    Questo vincolo è nuovo nella versione 2.3.

Valida che un valore sia inferiore a un altro valore, definito nelle opzioni. Per
forzare che un valore si inferiore o uguale a un altro valore, vedere
:doc:`/reference/constraints/LessThanOrEqual`. Per forzare che un valore sia maggiore
di un altro valore, vedere :doc:`/reference/constraints/GreaterThan`.

+----------------+------------------------------------------------------------------------+
| Si applica a   | :ref:`proprietà o metodo<validation-property-target>`                  |
+----------------+------------------------------------------------------------------------+
| Opzioni        | - `value`_                                                             |
|                | - `message`_                                                           |
+----------------+------------------------------------------------------------------------+
| Classe         | :class:`Symfony\\Component\\Validator\\Constraints\\LessThan`          |
+----------------+------------------------------------------------------------------------+
| Validatore     | :class:`Symfony\\Component\\Validator\\Constraints\\LessThanValidator` |
+----------------+------------------------------------------------------------------------+

Uso di base
-----------

Se ci si vuole assicurare che la proprietà ``age`` di una classe ``Person`` sia inferiore a
``80``, si può fare come segue:

.. configuration-block::

    .. code-block:: yaml

        # src/Acme/SocialBundle/Resources/config/validation.yml
        Acme\SocialBundle\Entity\Person:
            properties:
                age:
                    - LessThan:
                        value: 80

    .. code-block:: php-annotations

        // src/Acme/SocialBundle/Entity/Person.php
        namespace Acme\SocialBundle\Entity;

        use Symfony\Component\Validator\Constraints as Assert;

        class Person
        {
            /**
             * @Assert\LessThan(
             *     value = 80
             * )
             */
            protected $age;
        }

    .. code-block:: xml

        <!-- src/Acme/SocialBundle/Resources/config/validation.xml -->
        <?xml version="1.0" encoding="UTF-8" ?>
        <constraint-mapping xmlns="http://symfony.com/schema/dic/constraint-mapping"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://symfony.com/schema/dic/constraint-mapping http://symfony.com/schema/dic/constraint-mapping/constraint-mapping-1.0.xsd">

            <class name="Acme\SocialBundle\Entity\Person">
                <property name="age">
                    <constraint name="LessThan">
                        <option name="value">80</option>
                    </constraint>
                </property>
            </class>
        </constraint-mapping>

    .. code-block:: php

        // src/Acme/SocialBundle/Entity/Person.php
        namespace Acme\SocialBundle\Entity;

        use Symfony\Component\Validator\Mapping\ClassMetadata;
        use Symfony\Component\Validator\Constraints as Assert;

        class Person
        {
            public static function loadValidatorMetadata(ClassMetadata $metadata)
            {
                $metadata->addPropertyConstraint('age', new Assert\LessThan(array(
                    'value' => 80,
                )));
            }
        }

Opzioni
-------

.. include:: /reference/constraints/_comparison-value-option.rst.inc

message
~~~~~~~

**tipo**: ``stringa`` **predefinito**: ``This value should be less than {{ compared_value }}``

Messaggio mostrato se il valore non inferiore al valore
di confronto.
