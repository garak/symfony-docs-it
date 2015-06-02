Type
====

Valida che un valore sia di uno specifico tipo. Per esempio, se una variabile
deve essere un array, si può usare questo vincolo con l'opzione tipo ``array``,
per validarla.

+----------------+---------------------------------------------------------------------+
| Si applica a   | :ref:`proprietà o metodo <validation-property-target>`              |
+----------------+---------------------------------------------------------------------+
| Opzioni        | - :ref:`type <reference-constraint-type-type>`                      |
|                | - `message`_                                                        |
+----------------+---------------------------------------------------------------------+
| Classe         | :class:`Symfony\\Component\\Validator\\Constraints\\Type`           |
+----------------+---------------------------------------------------------------------+
| Validatore     | :class:`Symfony\\Component\\Validator\\Constraints\\TypeValidator`  |
+----------------+---------------------------------------------------------------------+

Uso di base
-----------

.. configuration-block::

    .. code-block:: php-annotations

        // src/Acme/BlogBundle/Entity/Author.php
        namespace Acme\BlogBundle\Entity;

        use Symfony\Component\Validator\Constraints as Assert;

        class Author
        {
            /**
             * @Assert\Type(
             *     type="integer",
             *     message="Il valore {{ value }} non è un {{ type }} valido."
             * )
             */
            protected $age;
        }

    .. code-block:: yaml

        # src/Acme/BlogBundle/Resources/config/validation.yml
        Acme\BlogBundle\Entity\Author:
            properties:
                age:
                    - Type:
                        type: integer
                        message: Il valore {{ value }} non è un {{ type }} valido.

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

* :phpfunction:`array <is_array>`
* :phpfunction:`bool <is_bool>`
* :phpfunction:`callable <is_callable>`
* :phpfunction:`float <is_float>`
* :phpfunction:`double <is_double>`
* :phpfunction:`int <is_int>`
* :phpfunction:`integer <is_integer>`
* :phpfunction:`long <is_long>`
* :phpfunction:`null <is_null>`
* :phpfunction:`numeric <is_numeric>`
* :phpfunction:`object <is_object>`
* :phpfunction:`real <is_real>`
* :phpfunction:`resource <is_resource>`
* :phpfunction:`scalar <is_scalar>`
* :phpfunction:`string <is_string>`

Si possono anche usare le funzioni ``ctype_`` della corrispondente
`estensione di PHP <http://php.net/manual/it/book.ctype.php>`_. Si consideri
`una lista di funzioni ctype <http://php.net/manual/it/ref.ctype.php>`_:

* :phpfunction:`alnum <ctype_alnum>`
* :phpfunction:`alpha <ctype_alpha>`
* :phpfunction:`cntrl <ctype_cntrl>`
* :phpfunction:`digit <ctype_digit>`
* :phpfunction:`graph <ctype_graph>`
* :phpfunction:`lower <ctype_lower>`
* :phpfunction:`print <ctype_print>`
* :phpfunction:`punct <ctype_punct>`
* :phpfunction:`space <ctype_space>`
* :phpfunction:`upper <ctype_upper>`
* :phpfunction:`xdigit <ctype_xdigit>`

Assicurarsi che il :phpfunction:`locale <setlocale>` adatto sia impostato, prima
dell'uso.

message
~~~~~~~

**tipo**: ``stringa`` **predefinito**: ``This value should be of type {{ type }}``

Messaggio mostrato se i dati sottostanti non sono del tipo dato.
