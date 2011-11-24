Locale
======

Valida che un valore sia un valid locale.

The "value" for each locale is either the two letter ISO639-1 *language* code
(p.e. ``fr``), or the language code followed by an underscore (``_``), then
the ISO3166 *country* code (p.e. ``fr_FR`` for French/France).

+----------------+------------------------------------------------------------------------+
| Si applica a   | :ref:`property or method<validation-property-target>`                  |
+----------------+------------------------------------------------------------------------+
| Opzioni        | - `message`_                                                           |
+----------------+------------------------------------------------------------------------+
| Classe         | :class:`Symfony\\Component\\Validator\\Constraints\\Locale`            |
+----------------+------------------------------------------------------------------------+
| Validatore     | :class:`Symfony\\Component\\Validator\\Constraints\\LocaleValidator`   |
+----------------+------------------------------------------------------------------------+

Uso di base
-----------

.. configuration-block::

    .. code-block:: yaml

        # src/UserBundle/Resources/config/validation.yml
        Acme\UserBundle\Entity\User:
            properties:
                locale:
                    - Locale:

    .. code-block:: php-annotations

       // src/Acme/UserBundle/Entity/User.php
       namespace Acme\UserBundle\Entity;
       
       use Symfony\Component\Validator\Constraints as Assert;

       class User
       {
           /**
            * @Assert\Locale
            */
            protected $locale;
       }

Opzioni
-------

message
~~~~~~~

**tipo**: ``stringa`` **predefinito**: ``This value is not a valid locale``

Messaggio mostrato se the string is not a valid locale.
