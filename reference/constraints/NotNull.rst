NotNull
=======

Valida che un valore is not strictly equal to ``null``. To ensure that
a value is simply not blank (not a blank string), see the  :doc:`/reference/constraints/NotBlank`
constraint.

+----------------+-----------------------------------------------------------------------+
| Si applica a   | :ref:`property or method<validation-property-target>`                 |
+----------------+-----------------------------------------------------------------------+
| Opzioni        | - `message`_                                                          |
+----------------+-----------------------------------------------------------------------+
| Classe         | :class:`Symfony\\Component\\Validator\\Constraints\\NotNull`          |
+----------------+-----------------------------------------------------------------------+
| Validatore     | :class:`Symfony\\Component\\Validator\\Constraints\\NotNullValidator` |
+----------------+-----------------------------------------------------------------------+

Uso di base
-----------

If you wanted to ensure that the ``firstName`` property of an ``Author`` class
were not strictly equal to ``null``, you would:

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

Messaggio mostrato se the value is ``null``.
