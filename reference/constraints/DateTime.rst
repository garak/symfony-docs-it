DateTime
========

Valida che un valore sia una data/ora valida, cioè o un oggetto ``DateTime``
o una stringa (o un oggetto che possa essere forzato a stringa) con un formato
valido YYYY-MM-DD HH:MM:SS.

+----------------+------------------------------------------------------------------------+
| Si applica a   | :ref:`property or method<validation-property-target>`                  |
+----------------+------------------------------------------------------------------------+
| Opzioni        | - `message`_                                                           |
+----------------+------------------------------------------------------------------------+
| Classe         | :class:`Symfony\\Component\\Validator\\Constraints\\DateTime`          |
+----------------+------------------------------------------------------------------------+
| Validatore     | :class:`Symfony\\Component\\Validator\\Constraints\\DateTimeValidator` |
+----------------+------------------------------------------------------------------------+

Uso di base
-----------

.. configuration-block::

    .. code-block:: yaml

        # src/Acme/EventBundle/Resources/config/validation.yml
        Acme\BlobBundle\Entity\Author:
            properties:
                createdAt:
                    - DateTime: ~

    .. code-block:: php-annotations

        // src/Acme/BlogBundle/Entity/Author.php
        namespace Acme\BlogBundle\Entity;

        use Symfony\Component\Validator\Constraints as Assert;

        class Author
        {
            /**
             * @Assert\DateTime()
             */
             protected $createdAt;
        }

Opzioni
-------

message
~~~~~~~

**tipo**: ``stringa`` **predefinito**: ``This value is not a valid datetime``

Messaggio mostrato se i dati sottostanti non sono una data/ora valida.
