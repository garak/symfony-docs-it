Null
====

Valida che un valore sia esattamente uguale a ``null``. Per forzare una proprietà a essere
vuota (stringa vuota o ``null``), vedere il vincolo :doc:`/reference/constraints/Blank`.
Per assicurarsi che una proprietà non sia ``null``, vedere :doc:`/reference/constraints/NotNull`.

+----------------+-----------------------------------------------------------------------+
| Si applica a   | :ref:`proprietà o metodo<validation-property-target>`                 |
+----------------+-----------------------------------------------------------------------+
| Opzioni        | - `message`_                                                          |
+----------------+-----------------------------------------------------------------------+
| Classe         | :class:`Symfony\\Component\\Validator\\Constraints\\Null`             |
+----------------+-----------------------------------------------------------------------+
| Validatore     | :class:`Symfony\\Component\\Validator\\Constraints\\NullValidator`    |
+----------------+-----------------------------------------------------------------------+

Uso di base
-----------

Se, per qualche ragione, ci si vuole assicurare che la proprietà ``firstName`` di
una classe ``Author`` sia esttamente uguale a ``null``, si può fare come segue:

.. configuration-block::

    .. code-block:: yaml

        # src/Acme/BlogBundle/Resources/config/validation.yml
        Acme\BlogBundle\Entity\Author:
            properties:
                firstName:
                    - 'Null': ~

    .. code-block:: php-annotations

        // src/Acme/BlogBundle/Entity/Author.php
        namespace Acme\BlogBundle\Entity;
        
        use Symfony\Component\Validator\Constraints as Assert;

        class Author
        {
            /**
             * @Assert\Null()
             */
            protected $firstName;
        }

    .. code-block:: xml

        <!-- src/Acme/BlogBundle/Resources/config/validation.xml -->
        <class name="Acme\BlogBundle\Entity\Author">
            <property name="firstName">
                <constraint name="Null" />
            </property>
        </class>

    .. code-block:: php

        // src/Acme/BlogBundle/Entity/Author.php
        namespace Acme\BlogBundle\Entity;
        
        use Symfony\Component\Validator\Mapping\ClassMetadata;
        use Symfony\Component\Validator\Constraints as Assert;

        class Author
        {
            public static function loadValidatorMetadata(ClassMetadata $metadata)
            {
                $metadata->addPropertyConstraint('firstName', Assert\Null());
            }
        }

Opzioni
-------

message
~~~~~~~

**tipo**: ``stringa`` **predefinito**: ``This value should be null``

Messaggio mostrato se il valore non è ``null``.
