Isbn
====

.. versionadded:: 2.3
    Il vincolo Isbn è stato aggiunto in Symfony 2.3.

.. caution::

    Le opzioni ``isbn10`` e ``isbn13`` sono deprecate da Symfony 2.5
    e saranno rimosse in Symfony 3.0. Usare invece l'opzione ``type``.
    Inoltre, quando si usa l'opzione ``type``, i caratteri minuscoli non sono
    più supportati a partire da Symfony 2.5, non essendo consentiti negli ISBN.

Questo vincolo valida che un `ISBN (International Standard Book Number)`_
si valido rispetto allo standard ISBN-10 o a quello ISBN-13 (o a entrambi).

+----------------+----------------------------------------------------------------------+
| Si applica a   | :ref:`proprietà o metodo<validation-property-target>`                |
+----------------+----------------------------------------------------------------------+
| Opzioni        | - `type`_                                                            |
|                | - `message`_                                                         |
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

    .. code-block:: yaml

        # src/Acme/BookcaseBundle/Resources/config/validation.yml
        Acme\BookcaseBundle\Entity\Book:
            properties:
                isbn:
                    - Isbn:
                        type: isbn10
                        message: This value is not  valid.

    .. code-block:: php-annotations

        // src/Acme/BookcaseBundle/Entity/Book.php
        namespace Acme\BookcaseBundle\Entity;

        use Symfony\Component\Validator\Constraints as Assert;

        class Book
        {
            /**
             * @Assert\Isbn(
             *     type = isbn10,
             *     message: This value is not  valid.
             * )
             */
            protected $isbn;
        }

    .. code-block:: xml

        <!-- src/Acme/BookcaseBundle/Resources/config/validation.xml -->
        <?xml version="1.0" encoding="UTF-8" ?>
        <constraint-mapping xmlns="http://symfony.com/schema/dic/constraint-mapping"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://symfony.com/schema/dic/constraint-mapping http://symfony.com/schema/dic/constraint-mapping/constraint-mapping-1.0.xsd">

            <class name="Acme\BookcaseBundle\Entity\Book">
                <property name="isbn">
                    <constraint name="Isbn">
                        <option name="type">isbn10</option>
                        <option name="message">This value is not  valid.</option>
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
                    'type'    => isbn10,
                    'message' => 'This value is not valid.'
                )));
            }
        }

Opzioni disponibili
-------------------

type
~~~~

**tipo**: ``stringa`` **predefinito**: ``null``

Il tipo di ISBN da validare.
Valori validi sono ``isbn10``, ``isbn13`` e ``null`` (per accettare ogni tipo di ISBN).

message
~~~~~~~

**tipo**: ``stringa`` **predefinito**: ``null``

Messaggio mostrato se il valore non è valido.
Se non ``null``, questo messaggio ha priorità sugli altri messaggi.

isbn10Message
~~~~~~~~~~~~~

**tipo**: ``stringa`` **predefinito**: ``This value is not a valid ISBN-10.``

Messaggio mostrato se l'opzione `type`_ è ``isbn10`` e il valore dato
non passa la verifica ISBN-10.

isbn13Message
~~~~~~~~~~~~~

**tipo**: ``stringa`` **predefinito**: ``This value is not a valid ISBN-13.``

Messaggio mostrato se l'opzione `type`_ è ``isbn13`` e il valore dato
non passa la verifica ISBN-13.

bothIsbnMessage
~~~~~~~~~~~~~~~

**tipo**: ``stringa`` **predefinito**: ``This value is neither a valid ISBN-10 nor a valid ISBN-13.``

Messaggio mostrato se l'opzione `type`_ è ``null``
e il valore dato non passa nessuna verifica ISBN.

.. _`ISBN (International Standard Book Number)`: https://it.wikipedia.org/wiki/ISBN
