NotEqualTo
==========

.. versionadded:: 2.3
    Questo vincolo è nuovo nella versione 2.3.

Valida che un valore **non** sia uguale a un altro valore, definito nelle
opzioni. Per forzare che un valore sia uguale, vedere
:doc:`/reference/constraints/EqualTo`.

.. caution::
    
    Questo vincolo confronta tramite ``!=``, quindi ``3`` e ``"3"`` sono considerati
    uguali. Usare :doc:`/reference/constraints/NotIdenticalTo` per confrontare con
    ``!==``.

+----------------+-------------------------------------------------------------------------+
| Si applica a   | :ref:`proprietà o metodo<validation-property-target>`                   |
+----------------+-------------------------------------------------------------------------+
| Opzioni        | - `value`_                                                              |
|                | - `message`_                                                            |
+----------------+-------------------------------------------------------------------------+
| Classe         | :class:`Symfony\\Component\\Validator\\Constraints\\NotEqualTo`         |
+----------------+-------------------------------------------------------------------------+
| Validatore     | :class:`Symfony\\Component\\Validator\\Constraints\\NotEqualToValidator`|
+----------------+-------------------------------------------------------------------------+

Uso di base
-----------

Se ci si vuole assicurare che la proprietà ``age`` di un oggetto ``Person`` non sia uguale a
``15``, si può fare come segue:

.. configuration-block::

    .. code-block:: yaml

        # src/SocialBundle/Resources/config/validation.yml
        Acme\SocialBundle\Entity\Person:
            properties:
                age:
                    - NotEqualTo:
                        value: 15

    .. code-block:: php-annotations

        // src/Acme/SocialBundle/Entity/Person.php
        namespace Acme\SocialBundle\Entity;

        use Symfony\Component\Validator\Constraints as Assert;

        class Person
        {
            /**
             * @Assert\NotEqualTo(
             *     value = 15
             * )
             */
            protected $age;
        }

    .. code-block:: xml

        <!-- src/Acme/SocialBundle/Resources/config/validation.xml -->
        <class name="Acme\SocialBundle\Entity\Person">
            <property name="age">
                <constraint name="NotEqualTo">
                    <option name="value">15</option>
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
                $metadata->addPropertyConstraint('age', new Assert\NotEqualTo(array(
                    'value' => 15,
                )));
            }
        }

Opzioni
-------

.. include:: /reference/constraints/_comparison-value-option.rst.inc

message
~~~~~~~

**tipo**: ``stringa`` **predefinito**: ``This value should not be equal to {{ compared_value }}``

Messaggio mostrato se il valore non è diverso.
