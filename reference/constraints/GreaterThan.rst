GreaterThan
===========

.. versionadded:: 2.3
    Questo vincolo è nuovo nella versione 2.3.

Valida che un valore sia maggiore di un altro valore, definito nelle opzioni. Per
forzare che un valore sia maggiore o uguale di un altro valore, vedere
:doc:`/reference/constraints/GreaterThanOrEqual`. Per forzare che un valore sia minore
di un altro valore, vedere :doc:`/reference/constraints/LessThan`.

+----------------+---------------------------------------------------------------------------+
| Si applica a   | :ref:`proprietà o metodo<validation-property-target>`                     |
+----------------+---------------------------------------------------------------------------+
| Opzioni        | - `value`_                                                                |
|                | - `message`_                                                              |
+----------------+---------------------------------------------------------------------------+
| Classe         | :class:`Symfony\\Component\\Validator\\Constraints\\GreaterThan`          |
+----------------+---------------------------------------------------------------------------+
| Validatore     | :class:`Symfony\\Component\\Validator\\Constraints\\GreaterThanValidator` |
+----------------+---------------------------------------------------------------------------+

Uso di base
-----------

Se ci si vuole assicurare che una proprietà  ``age`` di un oggetto ``Person`` sia maggiore di
``18``, si può fare come segue:

.. configuration-block::

    .. code-block:: yaml

        # src/SocialBundle/Resources/config/validation.yml
        Acme\SocialBundle\Entity\Person:
            properties:
                age:
                    - GreaterThan:
                        value: 18

    .. code-block:: php-annotations

        // src/Acme/SocialBundle/Entity/Person.php
        namespace Acme\SocialBundle\Entity;

        use Symfony\Component\Validator\Constraints as Assert;

        class Person
        {
            /**
             * @Assert\GreaterThan(
             *     value = 18
             * )
             */
            protected $age;
        }

    .. code-block:: xml

        <!-- src/Acme/SocialBundle/Resources/config/validation.xml -->
        <class name="Acme\SocialBundle\Entity\Person">
            <property name="age">
                <constraint name="GreaterThan">
                    <option name="value">18</option>
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
                $metadata->addPropertyConstraint('age', new Assert\GreaterThan(array(
                    'value' => 18,
                )));
            }
        }

Opzioni
-------

.. include:: /reference/constraints/_comparison-value-option.rst.inc

message
~~~~~~~

**tipo**: ``stringa`` **predefinito**: ``This value should be greater than {{ compared_value }}``

Messaggio mostrato se il valore non è maggiore del valore di
confronto.
