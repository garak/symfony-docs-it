Time
====

Valida che un valore sia un valid time string with format "HH:MM:SS".

Valida che un valore sia un valid time, meaning either a ``DateTime`` object
or a string (or an object that can be cast into a string) that follows
a valid "HH:MM:SS" format.

+----------------+------------------------------------------------------------------------+
| Si applica a   | :ref:`property or method<validation-property-target>`                  |
+----------------+------------------------------------------------------------------------+
| Opzioni        | - `message`_                                                           |
+----------------+------------------------------------------------------------------------+
| Classe         | :class:`Symfony\\Component\\Validator\\Constraints\\Time`              |
+----------------+------------------------------------------------------------------------+
| Validatore     | :class:`Symfony\\Component\\Validator\\Constraints\\TimeValidator`     |
+----------------+------------------------------------------------------------------------+

Uso di base
-----------

Suppose you have an Event class, with a ``startAt`` field that is the time
of the day when the event starts:

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

Opzioni
-------

message
~~~~~~~

**tipo**: ``stringa`` **predefinito**: ``This value is not a valid time``

Messaggio mostrato se the underlying data is not a valid time.
