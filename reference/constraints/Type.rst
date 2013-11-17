Type
====

Valida che un valore sia di uno specifico tipo. Per esempio, se una variabile
deve essere un array, si può usare questo vincolo con l'opzione tipo ``array``,
per validarla.

+----------------+---------------------------------------------------------------------+
| Si applica a   | :ref:`proprietà o metodo<validation-property-target>`               |
+----------------+---------------------------------------------------------------------+
| Opzioni        | - :ref:`type<reference-constraint-type-type>`                       |
|                | - `message`_                                                        |
+----------------+---------------------------------------------------------------------+
| Classe         | :class:`Symfony\\Component\\Validator\\Constraints\\Type`           |
+----------------+---------------------------------------------------------------------+
| Validatore     | :class:`Symfony\\Component\\Validator\\Constraints\\TypeValidator`  |
+----------------+---------------------------------------------------------------------+

Uso di base
-----------

.. configuration-block::

    .. code-block:: yaml

        # src/BlogBundle/Resources/config/validation.yml
        Acme\BlogBundle\Entity\Author:
            properties:
                age:
                    - Type:
                        type: integer
                        message: Il valore {{ value }} non è un {{ type }} valido.

    .. code-block:: php-annotations

        // src/Acme/BlogBundle/Entity/Author.php
        namespace Acme\BlogBundle\Entity;

        use Symfony\Component\Validator\Constraints as Assert;

        class Author
        {
            /**
             * @Assert\Type(type="integer", message="Il valore {{ value }} non è un {{ type }} valido.")
             */
            protected $age;
        }

    .. code-block:: xml

        <!-- src/Acme/BlogBundle/Resources/config/validation.xml -->
        <?xml version="1.0" encoding="UTF-8" ?>
        <constraint-mapping xmlns="http://symfony.com/schema/dic/constraint-mapping"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://symfony.com/schema/dic/constraint-mapping http://symfony.com/schema/dic/constraint-mapping/constraint-mapping-1.0.xsd">

            <class name="Acme\BlogBundle\Entity\Author">
                <property name="age">
                    <constraint name="Type">
                        <option name="type">integer</option>
                        <option name="message">Il valore {{ value }} non è un {{ type }} valido.</option>
                    </constraint>
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
                $metadata->addPropertyConstraint('age', new Assert\Type(array(
                    'type'    => 'integer',
                    'message' => 'Il valore {{ value }} non è un {{ type }} valido.',
                )));
            }
        }

Opzioni
-------

.. _reference-constraint-type-type:

type
~~~~

**tipo**: ``stringa`` [:ref:`opzione predefinita<validation-default-option>`]

Questa opzione obbligatoria è il nome pienamente qualificato della classe oppure uno
dei tipi di dato di PHP, come stabilito dalle funzioni ``is_`` di PHP.

* `array <http://php.net/is_array>`_
* `bool <http://php.net/is_bool>`_
* `callable <http://php.net/is_callable>`_
* `float <http://php.net/is_float>`_
* `double <http://php.net/is_double>`_
* `int <http://php.net/is_int>`_
* `integer <http://php.net/is_integer>`_
* `long <http://php.net/is_long>`_
* `null <http://php.net/is_null>`_
* `numeric <http://php.net/is_numeric>`_
* `object <http://php.net/is_object>`_
* `real <http://php.net/is_real>`_
* `resource <http://php.net/is_resource>`_
* `scalar <http://php.net/is_scalar>`_
* `string <http://php.net/is_string>`_

message
~~~~~~~

**tipo**: ``stringa`` **predefinito**: ``This value should be of type {{ type }}``

Messaggio mostrato se i dati sottostanti non sono del tipo dato.
