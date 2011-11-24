Null
====

Valida che un valore is exactly equal to ``null``. To force that a property
is simply blank (blank string or ``null``), see the  :doc:`/reference/constraints/Blank`
constraint. To ensure that a property is not null, see :doc:`/reference/constraints/NotNull`.

+----------------+-----------------------------------------------------------------------+
| Si applica a   | :ref:`property or method<validation-property-target>`                 |
+----------------+-----------------------------------------------------------------------+
| Opzioni        | - `message`_                                                          |
+----------------+-----------------------------------------------------------------------+
| Classe         | :class:`Symfony\\Component\\Validator\\Constraints\\Null`             |
+----------------+-----------------------------------------------------------------------+
| Validatore     | :class:`Symfony\\Component\\Validator\\Constraints\\NullValidator`    |
+----------------+-----------------------------------------------------------------------+

Uso di base
-----------

If, for some reason, you wanted to ensure that the ``firstName`` property
of an ``Author`` class exactly equal to ``null``, you could do the following:

.. configuration-block::

    .. code-block:: yaml

        # src/Acme/BlogBundle/Resources/config/validation.yml
        Acme\BlogBundle\Entity\Author:
            properties:
                firstName:
                    - Null: ~

    .. code-block:: php-annotations

        // src/Acme/BlogBundle/Entity/Author.php
        namespace Acme\BlogBundle\Entity;
        
        use Symfony\Component\Validator\Constraints as Assert;

        class Author
        {
            /**
             * @Assert\Null()
             */
            protected $firstName;
        }

Opzioni
-------

message
~~~~~~~

**tipo**: ``stringa`` **predefinito**: ``This value should be null``

Messaggio mostrato se the value is not ``null``.
