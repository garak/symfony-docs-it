IdenticalTo
===========

.. versionadded:: 2.3
    Questo vincolo è nuovo nella versione 2.3.

Valida che un valore sia identico a un altro valore, definito nelle
opzioni. Per forzare che un valore *non* sia identico, vedere
:doc:`/reference/constraints/NotIdenticalTo`.

.. caution::

    Questo vincolo confronta tramite ``===``, quindi ``3`` e ``"3"`` *non* sono considerati
    uguali. Usare :doc:`/reference/constraints/EqualTo` per confrontare
    con ``==``.

+----------------+--------------------------------------------------------------------------+
| Si applica a   | :ref:`proprietà o metodo<validation-property-target>`                    |
+----------------+--------------------------------------------------------------------------+
| Opzioni        | - `value`_                                                               |
|                | - `message`_                                                             |
+----------------+--------------------------------------------------------------------------+
| Classe         | :class:`Symfony\\Component\\Validator\\Constraints\\IdenticalTo`         |
+----------------+--------------------------------------------------------------------------+
| Validatore     | :class:`Symfony\\Component\\Validator\\Constraints\\IdenticalToValidator`|
+----------------+--------------------------------------------------------------------------+

Uso di base
-----------

Se ci si vuole assicurare che la proprietà ``age`` di una classe ``Person`` sia uguale a
``20`` e sia un intero, si può fare come segue:

.. configuration-block::

    .. code-block:: yaml

        # src/SocialBundle/Resources/config/validation.yml
        Acme\SocialBundle\Entity\Person:
            properties:
                age:
                    - IdenticalTo:
                        value: 20

    .. code-block:: php-annotations

        // src/Acme/SocialBundle/Entity/Person.php
        namespace Acme\SocialBundle\Entity;

        use Symfony\Component\Validator\Constraints as Assert;

        class Person
        {
            /**
             * @Assert\IdenticalTo(
             *     value = 20
             * )
             */
            protected $age;
        }

    .. code-block:: xml

        <!-- src/Acme/SocialBundle/Resources/config/validation.xml -->
        <class name="Acme\SocialBundle\Entity\Person">
            <property name="age">
                <constraint name="IdenticalTo">
                    <option name="value">20</option>
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
                $metadata->addPropertyConstraint('age', new Assert\IdenticalTo(array(
                    'value' => 20,
                )));
            }
        }

Opzioni
-------

.. include:: /reference/constraints/_comparison-value-option.rst.inc

message
~~~~~~~

**tipo**: ``stringa`` **predefinito**: ``This value should be identical to {{ compared_value_type }} {{ compared_value }}``

Messaggio mostrato se il valore non è identico.
