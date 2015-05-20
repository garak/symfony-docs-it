Isbn
====

.. versionadded:: 2.3
    Il vincolo Isbn è stato aggiunto in Symfony 2.3.

Questo vincolo valida che un `ISBN (International Standard Book Number)`_
si valido rispetto allo standard ISBN-10 o a quello ISBN-13 (o a entrambi).

+----------------+----------------------------------------------------------------------+
| Si applica a   | :ref:`proprietà o metodo<validation-property-target>`                |
+----------------+----------------------------------------------------------------------+
| Opzioni        | - `isbn10`_                                                          |
|                | - `isbn13`_                                                          |
|                | - `isbn10Message`_                                                   |
|                | - `isbn13Message`_                                                   |
|                | - `bothIsbnMessage`_                                                 |
+----------------+----------------------------------------------------------------------+
| Classe         | :class:`Symfony\\Component\\Validator\\Constraints\\Isbn`            |
+----------------+----------------------------------------------------------------------+
| Validatore     | :class:`Symfony\\Component\\Validator\\Constraints\\IsbnValidator`   |
+----------------+----------------------------------------------------------------------+

Uso di base
-----------

Per usare il validatore ``Isbn``, basta applicarlo a una proprietà o a un metodo
su un oggetto che conterrà un ISBN.

.. configuration-block::

    .. code-block:: php-annotations

        // src/Acme/BookcaseBundle/Entity/Book.php
        namespace Acme\BookcaseBundle\Entity;

        use Symfony\Component\Validator\Constraints as Assert;

        class Book
        {
            /**
             * @Assert\Isbn(
             *     isbn10 = true,
             *     isbn13 = true,
             *     bothIsbnMessage = "This value is neither a valid ISBN-10 nor a valid ISBN-13."
             * )
             */
            protected $isbn;
        }

    .. code-block:: yaml

        # src/Acme/BookcaseBundle/Resources/config/validation.yml
        Acme\BookcaseBundle\Entity\Book:
            properties:
                isbn:
                    - Isbn:
                        isbn10: true
                        isbn13: true
                        bothIsbnMessage: This value is neither a valid ISBN-10 nor a valid ISBN-13.

    .. code-block:: xml

        <!-- src/Acme/BookcaseBundle/Resources/config/validation.xml -->
        <?xml version="1.0" encoding="UTF-8" ?>
        <constraint-mapping xmlns="http://symfony.com/schema/dic/constraint-mapping"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://symfony.com/schema/dic/constraint-mapping http://symfony.com/schema/dic/constraint-mapping/constraint-mapping-1.0.xsd">

            <class name="Acme\BookcaseBundle\Entity\Book">
                <property name="isbn">
                    <constraint name="Isbn">
                        <option name="isbn10">true</option>
                        <option name="isbn13">true</option>
                        <option name="bothIsbnMessage">
                            This value is neither a valid ISBN-10 nor a valid ISBN-13.
                        </option>
                    </constraint>
                </property>
            </class>
        </constraint-mapping>

    .. code-block:: php

        // src/Acme/BookcaseBundle/Entity/Book.php
        namespace Acme\BookcaseBundle\Entity;

        use Symfony\Component\Validator\Mapping\ClassMetadata;
        use Symfony\Component\Validator\Constraints as Assert;

        class Book
        {
            protected $isbn;

            public static function loadValidatorMetadata(ClassMetadata $metadata)
            {
                $metadata->addPropertyConstraint('isbn', new Assert\Isbn(array(
                    'isbn10'          => true,
                    'isbn13'          => true,
                    'bothIsbnMessage' => 'This value is neither a valid ISBN-10 nor a valid ISBN-13.'
                )));
            }
        }

Opzioni disponibili
-------------------

isbn10
~~~~~~

**tipo**: ``booleano`` [:ref:`default option<validation-default-option>`]

Se questa opzione obbligatoria è ``true``, il vincolo verificherà che il codice
sia valido rispetto a ISBN-10.

isbn13
~~~~~~

**tipo**: ``booleano`` [:ref:`default option<validation-default-option>`]

Se questa opzione obbligatoria è ``true``, il vincolo verificherà che il codice
sia valido rispetto a ISBN-13.

isbn10Message
~~~~~~~~~~~~~

**tipo**: ``stringa`` **predefinito**: ``This value is not a valid ISBN-10.``

Messaggio mostrato se l'opzione `isbn10`_ è ``true`` e il valore dato
non passa la verifica ISBN-10.

isbn13Message
~~~~~~~~~~~~~

**tipo**: ``stringa`` **predefinito**: ``This value is not a valid ISBN-13.``

Messaggio mostrato se l'opzione `isbn13`_ è ``true`` e il valore dato
non passa la verifica ISBN-13.

bothIsbnMessage
~~~~~~~~~~~~~~~

**tipo**: ``stringa`` **predefinito**: ``This value is neither a valid ISBN-10 nor a valid ISBN-13.``

Messaggio mostrato se entrambe le opzioni `isbn10`_ e `isbn13`_ sono ``true``
e il valore dato non passa né la verifica ISBN-10 né quella ISBN-13.

.. _`ISBN (International Standard Book Number)`: https://it.wikipedia.org/wiki/ISBN
