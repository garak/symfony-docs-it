NotBlank
========

Valida che un valore is not blank, defined as not equal to a blank string
and also not equal to ``null``. To force that a value is simply not equal to
``null``, see the :doc:`/reference/constraints/NotNull` constraint.

+----------------+------------------------------------------------------------------------+
| Si applica a   | :ref:`property or method<validation-property-target>`                  |
+----------------+------------------------------------------------------------------------+
| Opzioni        | - `message`_                                                           |
+----------------+------------------------------------------------------------------------+
| Classe         | :class:`Symfony\\Component\\Validator\\Constraints\\NotBlank`          |
+----------------+------------------------------------------------------------------------+
| Validatore     | :class:`Symfony\\Component\\Validator\\Constraints\\NotBlankValidator` |
+----------------+------------------------------------------------------------------------+

Uso di base
-----------

If you wanted to ensure that the ``firstName`` property of an ``Author`` class
were not blank, you could do the following:

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

Messaggio mostrato se the value is blank.
