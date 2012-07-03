NotNull
=======

Valida che un valore non sia esattamente uguale a ``null``. Per forzare un valore
a non essere vuoto (stringa vuota), vedere il vincolo
:doc:`/reference/constraints/NotBlank`.

+----------------+-----------------------------------------------------------------------+
| Si applica a   | :ref:`proprietà o metodo<validation-property-target>`                 |
+----------------+-----------------------------------------------------------------------+
| Opzioni        | - `message`_                                                          |
+----------------+-----------------------------------------------------------------------+
| Classe         | :class:`Symfony\\Component\\Validator\\Constraints\\NotNull`          |
+----------------+-----------------------------------------------------------------------+
| Validatore     | :class:`Symfony\\Component\\Validator\\Constraints\\NotNullValidator` |
+----------------+-----------------------------------------------------------------------+

Uso di base
-----------

Se ci si vuole assicurare che la proprietà ``firstName`` di una classe ``Author`` non
sia ``null``, si può fare come segue:

.. configuration-block::

    .. code-block:: yaml

        properties:
            firstName:
                - NotNull: ~

    .. code-block:: php-annotations

        // src/Acme/BlogBundle/Entity/Author.php
        use Symfony\Component\Validator\Constraints as Assert;

        class Author
        {
            /**
             * @Assert\NotNull()
             */
            protected $firstName;
        }

Opzioni
-------

message
~~~~~~~

**tipo**: ``stringa`` **predefinito**: ``This value should not be null``

Messaggio mostrato se il valore è ``null``.
