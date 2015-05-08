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

    .. code-block:: php-annotations

        // src/Acme/UserBundle/Entity/User.php
        namespace Acme\UserBundle\Entity;

        use Symfony\Component\Validator\Constraints as Assert;

        class User
        {
            /**
             * @Assert\All({
             *     @Assert\NotBlank,
             *     @Assert\Length(min = 5)
             * })
             */
             protected $favoriteColors = array();
        }

    .. code-block:: yaml

        # src/Acme/UserBundle/Resources/config/validation.yml
        Acme\UserBundle\Entity\User:
            properties:
                favoriteColors:
                    - All:
                        - NotBlank:  ~
                        - Length:
                            min: 5

    .. code-block:: xml

        <!-- src/Acme/UserBundle/Resources/config/validation.xml -->
        <?xml version="1.0" encoding="UTF-8" ?>
        <constraint-mapping xmlns="http://symfony.com/schema/dic/constraint-mapping"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://symfony.com/schema/dic/constraint-mapping http://symfony.com/schema/dic/constraint-mapping/constraint-mapping-1.0.xsd">

            <class name="Acme\UserBundle\Entity\User">
                <property name="favoriteColors">
                    <constraint name="All">
                        <option name="constraints">
                            <constraint name="NotBlank" />
                            <constraint name="Length">
                                <option name="min">5</option>
                            </constraint>
                        </option>
                    </constraint>
                </property>
            </class>
        </constraint-mapping>

    .. code-block:: php

        // src/Acme/UserBundle/Entity/User.php
        namespace Acme\UserBundle\Entity;

        use Symfony\Component\Validator\Mapping\ClassMetadata;
        use Symfony\Component\Validator\Constraints as Assert;

        class User
        {
            public static function loadValidatorMetadata(ClassMetadata $metadata)
            {
                $metadata->addPropertyConstraint('favoriteColors', new Assert\All(array(
                    'constraints' => array(
                        new Assert\NotBlank(),
                        new Assert\Length(array('min' => 5)),
                    ),
                )));
            }
        }

Ora, ogni elemento nell'array ``favoriteColors`` sarà validato per non essere
vuoto e per avere almeno 5 caratteri.

Opzioni
-------

vincoli
~~~~~~~

**tipo**: ``array`` [:ref:`opzione predefinita<validation-default-option>`]

Questa opzione obbligatoria è l'array dei vincoli di validazione che si vuole
applicare a ogni elemento dell'array sottostante.
