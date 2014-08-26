NotIdenticalTo
==============

.. versionadded:: 2.3
    Questo vincolo è nuovo nella versione 2.3.

Valida che un valore **non** sia identico a un altro valore, definito nelle
opzioni. Per forzare che un valore sia identico, vedere
:doc:`/reference/constraints/IdenticalTo`.

.. caution::

    Questo vincolo confronta tramite ``!==``, quindi ``3`` e ``"3"`` sono considerati
    diversi. Usare :doc:`/reference/constraints/NotEqualTo` per confrontare
    con ``!=``.

+----------------+-----------------------------------------------------------------------------+
| Si applica a   | :ref:`proprietà o metodo<validation-property-target>`                       |
+----------------+-----------------------------------------------------------------------------+
| Opzioni        | - `value`_                                                                  |
|                | - `message`_                                                                |
+----------------+-----------------------------------------------------------------------------+
| Classe         | :class:`Symfony\\Component\\Validator\\Constraints\\NotIdenticalTo`         |
+----------------+-----------------------------------------------------------------------------+
| Validatore     | :class:`Symfony\\Component\\Validator\\Constraints\\NotIdenticalToValidator`|
+----------------+-----------------------------------------------------------------------------+

Uso di base
-----------

Se ci si vuole assicurare che la proprietà ``age`` di una classe ``Person`` *non* sia uguale a
``15`` e *non* sia un intero, si può fare come segue:

.. configuration-block::

    .. code-block:: yaml

        # src/Acme/SocialBundle/Resources/config/validation.yml
        Acme\SocialBundle\Entity\Person:
            properties:
                age:
                    - NotIdenticalTo:
                        value: 15

    .. code-block:: php-annotations

        // src/Acme/SocialBundle/Entity/Person.php
        namespace Acme\SocialBundle\Entity;

        use Symfony\Component\Validator\Constraints as Assert;

        class Person
        {
            /**
             * @Assert\NotIdenticalTo(
             *     value = 15
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
                    <constraint name="NotIdenticalTo">
                        <option name="value">15</option>
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
                $metadata->addPropertyConstraint('age', new Assert\NotIdenticalTo(array(
                    'value' => 15,
                )));
            }
        }

Opzioni
-------

.. include:: /reference/constraints/_comparison-value-option.rst.inc

message
~~~~~~~

**tipo**: ``stringa`` **predefinito**: ``This value should not be identical to {{ compared_value_type }} {{ compared_value }}``

Messaggio mostrato se il valore è identico.
