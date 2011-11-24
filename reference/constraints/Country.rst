Country
=======

Valida che un valore sia un codice valido per un paese.

+----------------+------------------------------------------------------------------------+
| Si applica a   | :ref:`property or method<validation-property-target>`                  |
+----------------+------------------------------------------------------------------------+
| Opzioni        | - `message`_                                                           |
+----------------+------------------------------------------------------------------------+
| Classe         | :class:`Symfony\\Component\\Validator\\Constraints\\Country`           |
+----------------+------------------------------------------------------------------------+
| Validatore     | :class:`Symfony\\Component\\Validator\\Constraints\\CountryValidator`  |
+----------------+------------------------------------------------------------------------+

Uso di base
-----------

.. configuration-block::

    .. code-block:: yaml

        # src/UserBundle/Resources/config/validation.yml
        Acme\UserBundle\Entity\User:
            properties:
                country:
                    - Country:

    .. code-block:: php-annotations

       // src/Acme/UserBundle/Entity/User.php
       namespace Acme\UserBundle\Entity;
       
       use Symfony\Component\Validator\Constraints as Assert;

       class User
       {
           /**
            * @Assert\Country
            */
            protected $country;
       }

Opzioni
-------

message
~~~~~~~

**tipo**: ``stringa`` **predefinito**: ``This value is not a valid country``

Messaggio mostrato se la stringa non è un codice valido per un paese.
