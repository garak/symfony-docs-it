Date
====

Valida che un valore sia una data valida, cioà o un oggetto ``DateTime`` o
una strina (o un oggetto che possa essere forzato a stringa) che segue un formato
valido YYYY-MM-DD.

+----------------+--------------------------------------------------------------------+
| Si applica a   | :ref:`property or method<validation-property-target>`              |
+----------------+--------------------------------------------------------------------+
| Opzioni        | - `message`_                                                       |
+----------------+--------------------------------------------------------------------+
| Classe         | :class:`Symfony\\Component\\Validator\\Constraints\\Date`          |
+----------------+--------------------------------------------------------------------+
| Validatore     | :class:`Symfony\\Component\\Validator\\Constraints\\DateValidator` |
+----------------+--------------------------------------------------------------------+

Uso di base
-----------

.. configuration-block::

    .. code-block:: yaml

        # src/Acme/BlogBundle/Resources/config/validation.yml
        Acme\BlogBundle\Entity\Author:
            properties:
                birthday:
                    - Date: ~

    .. code-block:: php-annotations

        // src/Acme/BlogBundle/Entity/Author.php
        use Symfony\Component\Validator\Constraints as Assert;

        class Author
        {
            /**
             * @Assert\Date()
             */
             protected $birthday;
        }

Opzioni
-------

message
~~~~~~~

**tipo**: ``stringa`` **predefinito**: ``This value is not a valid date``

Messaggio mostrato se i dati sottostanti non sono una data valida.
