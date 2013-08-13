Time
====

Valida che un valore sia un tempo valido, cioè o un oggetto ``DateTime`` o
una stringa (o un oggetto che possa essere forzato a stringa) che segua il formato
"HH:MM:SS".

+----------------+------------------------------------------------------------------------+
| Si applica a   | :ref:`proprietà o metodo<validation-property-target>`                  |
+----------------+------------------------------------------------------------------------+
| Opzioni        | - `message`_                                                           |
+----------------+------------------------------------------------------------------------+
| Classe         | :class:`Symfony\\Component\\Validator\\Constraints\\Time`              |
+----------------+------------------------------------------------------------------------+
| Validatore     | :class:`Symfony\\Component\\Validator\\Constraints\\TimeValidator`     |
+----------------+------------------------------------------------------------------------+

Uso di base
-----------

Si supponga di avere una classe ``Event``, con un campo ``startAt``, che indica l'ora
del giorno in cui l'evento inizia:

.. configuration-block::

    .. code-block:: yaml

        # src/Acme/EventBundle/Resources/config/validation.yml
        Acme\EventBundle\Entity\Event:
            properties:
                startsAt:
                    - Time: ~

    .. code-block:: php-annotations

        // src/Acme/EventBundle/Entity/Event.php
        namespace Acme\EventBundle\Entity;
        
        use Symfony\Component\Validator\Constraints as Assert;

        class Event
        {
            /**
             * @Assert\Time()
             */
             protected $startsAt;
        }

    .. code-block:: xml

        <!-- src/Acme/EventBundle/Resources/config/validation.xml -->
        <?xml version="1.0" encoding="UTF-8" ?>
        <constraint-mapping xmlns="http://symfony.com/schema/dic/constraint-mapping"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://symfony.com/schema/dic/constraint-mapping http://symfony.com/schema/dic/constraint-mapping/constraint-mapping-1.0.xsd">

            <class name="Acme\EventBundle\Entity\Event">
                <property name="startsAt">
                    <constraint name="Time" />
                </property>
            </class>
        </constraint-mapping>

    .. code-block:: php
        
        // src/Acme/EventBundle/Entity/Event.php
        namespace Acme\EventBundle\Entity;
        
        use Symfony\Component\Validator\Mapping\ClassMetadata;
        use Symfony\Component\Validator\Constraints as Assert;

        class Event
        {
            public static function loadValidatorMetadata(ClassMetadata $metadata)
            {
                $metadata->addPropertyConstraint('startsAt', new Assert\Time());
            }
        }

Opzioni
-------

message
~~~~~~~

**tipo**: ``stringa`` **predefinito**: ``This value is not a valid time``

Messaggio mostrato se il dato sottostante non è un orario valido.
