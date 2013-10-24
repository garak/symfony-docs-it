Issn
====

.. versionadded:: 2.3
    La validazione ISSN è nuova in Symfony 2.3.

Valida che un valore sia un `ISSN`_ valido.

+----------------+-----------------------------------------------------------------------+
| Si applica     | :ref:`proprietà o metodo<validation-property-target>`                 |
+----------------+-----------------------------------------------------------------------+
| Opzioni        | - `message`_                                                          |
|                | - `caseSensitive`_                                                    |
|                | - `requireHyphen`_                                                    |
+----------------+-----------------------------------------------------------------------+
| Classe         | :class:`Symfony\\Component\\Validator\\Constraints\\Issn`             |
+----------------+-----------------------------------------------------------------------+
| Validatore     | :class:`Symfony\\Component\\Validator\\Constraints\\IssnValidator`    |
+----------------+-----------------------------------------------------------------------+

Uso di base
-----------

.. configuration-block::

    .. code-block:: yaml

        # src/JournalBundle/Resources/config/validation.yml
        Acme\JournalBundle\Entity\Journal:
            properties:
                issn:
                    - Issn: ~

    .. code-block:: php-annotations

        // src/Acme/JournalBundle/Entity/Journal.php
        namespace Acme\JournalBundle\Entity;

        use Symfony\Component\Validator\Constraints as Assert;

        class Journal
        {
            /**
             * @Assert\Issn
             */
             protected $issn;
        }

    .. code-block:: xml

        <!-- src/Acme/JournalBundle/Resources/config/validation.xml -->
        <class name="Acme\JournalBundle\Entity\Journal">
            <property name="issn">
                <constraint name="Issn" />
            </property>
        </class>

    .. code-block:: php

        // src/Acme/JournalBundle/Entity/Journal.php
        namespace Acme\JournalBundle\Entity;

        use Symfony\Component\Validator\Mapping\ClassMetadata;
        use Symfony\Component\Validator\Constraints as Assert;

        class Journal
        {
            public static function loadValidatorMetadata(ClassMetadata $metadata)
            {
                $metadata->addPropertyConstraint('issn', new Assert\Issn());
            }
        }

Opzioni
-------

message
~~~~~~~

**tipo**: ``stringa`` predefinito: ``This value is not a valid ISSN.``

Messaggio mostrato se il valore dato non è un ISSN valido.

caseSensitive
~~~~~~~~~~~~~

**tipo**: ``booleano`` predefinito: ``false``

Il validatore consentià valori ISSN che terminano con una 'x' minuscola.
Se impostato a ``true``, il validatore richiederà una 'X' maiuscola.

requireHyphen
~~~~~~~~~~~~~

**tipo**: ``booleano`` predefinito: ``false``

Il validatore consentirà valori ISSN senza trattini. Se impostata
a ``true``, il validatore richiederà un valore ISSN con trattini.

.. _`ISSN`: http://it.wikipedia.org/wiki/ISSN

