LessThanOrEqual
===============

.. versionadded:: 2.3
    Questo vincolo è nuovo nella versione 2.3.

Valida che un valore sia minore o uguale a un altro valore, definito nelle
opzioni. Per forzare che un valore sia inferiore, vedere
:doc:`/reference/constraints/LessThan`.

+----------------+-------------------------------------------------------------------------------+
| Si applica a   | :ref:`proprietà o metodo<validation-property-target>`                         |
+----------------+-------------------------------------------------------------------------------+
| Opzioni        | - `value`_                                                                    |
|                | - `message`_                                                                  |
+----------------+-------------------------------------------------------------------------------+
| Classe         | :class:`Symfony\\Component\\Validator\\Constraints\\LessThanOrEqual`          |
+----------------+-------------------------------------------------------------------------------+
| Validatore     | :class:`Symfony\\Component\\Validator\\Constraints\\LessThanOrEqualValidator` |
+----------------+-------------------------------------------------------------------------------+

Basic Usage
-----------

Se ci si vuole assicurare che la proprietà ``age`` di una classe ``Person`` sia minore o
uguale a ``15``, si può fare come segue:

.. configuration-block::

    .. code-block:: yaml

        # src/SocialBundle/Resources/config/validation.yml
        Acme\SocialBundle\Entity\Person:
            properties:
                age:
                    - LessThanOrEqual:
                        value: 80

    .. code-block:: php-annotations

        // src/Acme/SocialBundle/Entity/Person.php
        namespace Acme\SocialBundle\Entity;

        use Symfony\Component\Validator\Constraints as Assert;

        class Person
        {
            /**
             * @Assert\LessThanOrEqual(
             *     value = 80
             * )
             */
            protected $age;
        }

    .. code-block:: xml

        <!-- src/Acme/SocialBundle/Resources/config/validation.xml -->
        <class name="Acme\SocialBundle\Entity\Person">
            <property name="age">
                <constraint name="LessThanOrEqual">
                    <option name="value">80</option>
                </constraint>
            </property>
        </class>

    .. code-block:: php

        // src/Acme/SocialBundle/Entity/Person.php
        namespace Acme\SocialBundle\Entity;

        use Symfony\Component\Validator\Mapping\ClassMetadata;
        use Symfony\Component\Validator\Constraints as Assert;

        class Person
        {
            public static function loadValidatorMetadata(ClassMetadata $metadata)
            {
                $metadata->addPropertyConstraint('age', new Assert\LessThanOrEqual(array(
                    'value' => 80,
                )));
            }
        }

Options
-------

.. include:: /reference/constraints/_comparison-value-option.rst.inc

message
~~~~~~~

**tipo**: ``stringa`` **predefinito**: ``This value should be less than or equal to {{ compared_value }}``

Messaggio mostrato se il valore non è minore o uguale a quello
di confronto.
