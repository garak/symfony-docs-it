Blank
=====

Valida che un valore sia vuoto, definito come uguale alla stringa vuota o uguale
a ``null``. Per forzare un valore per essere strettamente uguale a ``null``, vedere
il vincolo :doc:`/reference/constraints/Null`. Per forzare un valore a *non* essere
vuoto, vedere :doc:`/reference/constraints/NotBlank`.

+----------------+-----------------------------------------------------------------------+
| Si applica a   | :ref:`property or method<validation-property-target>`                 |
+----------------+-----------------------------------------------------------------------+
| Opzioni        | - `message`_                                                          |
+----------------+-----------------------------------------------------------------------+
| Classe         | :class:`Symfony\\Component\\Validator\\Constraints\\Blank`            |
+----------------+-----------------------------------------------------------------------+
| Validatore     | :class:`Symfony\\Component\\Validator\\Constraints\\BlankValidator`   |
+----------------+-----------------------------------------------------------------------+

Uso di base
-----------

Se, per qualche ragione, ci si vuole assicurare che la proprietà ``firstName`` di una
classe ``Author`` sia vuota, si può fare come segue:

.. configuration-block::

    .. code-block:: yaml

        properties:
            firstName:
                - Blank: ~

    .. code-block:: php-annotations

        // src/Acme/BlogBundle/Entity/Author.php
        use Symfony\Component\Validator\Constraints as Assert;

        class Author
        {
            /**
             * @Assert\Blank()
             */
            protected $firstName;
        }

Opzioni
-------

message
~~~~~~~

**tipo**: ``stringa`` **predefinito**: ``This value should be blank``

Messaggio che sarà mostrato se il valore non è vuoto.
