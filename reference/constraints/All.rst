All
===

Quando applicato a un array (o a un oggetto Traversable), questo vincolo consente di
applicare un insieme di vincoli a ogni elemento dell'array.

+----------------+------------------------------------------------------------------------+
| Si applica a   | :ref:`proprietà o metodo<validation-property-target>`                  |
+----------------+------------------------------------------------------------------------+
| Opzioni        | - `vincoli`_                                                           |
+----------------+------------------------------------------------------------------------+
| Classe         | :class:`Symfony\\Component\\Validator\\Constraints\\All`               |
+----------------+------------------------------------------------------------------------+
| Validatore     | :class:`Symfony\\Component\\Validator\\Constraints\\AllValidator`      |
+----------------+------------------------------------------------------------------------+

Uso di base
-----------

Si supponga di avere un array di stringhe e di voler validare ogni elemento
dell'array:

.. configuration-block::

    .. code-block:: yaml

        # src/UserBundle/Resources/config/validation.yml
        Acme\UserBundle\Entity\User:
            properties:
                favoriteColors:
                    - All:
                        - NotBlank:  ~
                        - Length:
                            min: 5

    .. code-block:: php-annotations

       // src/Acme/UserBundle/Entity/User.php
       namespace Acme\UserBundle\Entity;
       
       use Symfony\Component\Validator\Constraints as Assert;

       class User
       {
           /**
            * @Assert\All({
            *     @Assert\NotBlank
            *     @Assert\Length(min = "5"),
            * })
            */
            protected $favoriteColors = array();
       }

Ora, ogni elemento nell'array ``favoriteColors`` sarà validato per non essere
vuoto e per avere almeno 5 caratteri.

Opzioni
-------

vincoli
~~~~~~~

**tipo**: ``array`` [:ref:`opzione predefinita<validation-default-option>`]

Questa opzione obbligatoria è l'array dei vincoli di validazione che si vuole
applicre a ogni elemento dell'array sottostante.
