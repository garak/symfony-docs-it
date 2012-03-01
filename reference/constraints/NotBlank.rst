NotBlank
========

Valida che un valore non sia vuoto, cioè sia diverso da una stringa vuota
e anche diverso da ``null``. Per forzare un valore a essere solo diverso da
``null``, vedere il vincolo :doc:`/reference/constraints/NotNull`.

+----------------+------------------------------------------------------------------------+
| Si applica a   | :ref:`proprietà o metodo<validation-property-target>`                  |
+----------------+------------------------------------------------------------------------+
| Opzioni        | - `message`_                                                           |
+----------------+------------------------------------------------------------------------+
| Classe         | :class:`Symfony\\Component\\Validator\\Constraints\\NotBlank`          |
+----------------+------------------------------------------------------------------------+
| Validatore     | :class:`Symfony\\Component\\Validator\\Constraints\\NotBlankValidator` |
+----------------+------------------------------------------------------------------------+

Uso di base
-----------

Se ci si vuole assicurare che la proprietà ``firstName`` di una classe ``Author`` non
sia vuota, si può fare come segue:

.. configuration-block::

    .. code-block:: yaml

        properties:
            firstName:
                - NotBlank: ~

    .. code-block:: php-annotations

        // src/Acme/BlogBundle/Entity/Author.php
        use Symfony\Component\Validator\Constraints as Assert;

        class Author
        {
            /**
             * @Assert\NotBlank()
             */
            protected $firstName;
        }

Opzioni
-------

message
~~~~~~~

**tipo**: ``stringa`` **predefinito**: ``This value should not be blank``

Messaggio mostrato se il valore è vuoto.
