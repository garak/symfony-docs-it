Max
===

.. caution::

    Questo vincolo è deprecato dalla versione 2.1 e sarà rimosso
    in Symfony 2.3. Usare :doc:`/reference/constraints/Range` con opzione ``max``
    al suo posto.

Valida che un dato numero sia *minore* di un numero massimo.

+----------------+--------------------------------------------------------------------+
| Si applica a   | :ref:`proprietà o metodo<validation-property-target>`              |
+----------------+--------------------------------------------------------------------+
| Opzioni        | - `limit`_                                                         |
|                | - `message`_                                                       |
|                | - `invalidMessage`_                                                |
+----------------+--------------------------------------------------------------------+
| Classe         | :class:`Symfony\\Component\\Validator\\Constraints\\Max`           |
+----------------+--------------------------------------------------------------------+
| Validatore     | :class:`Symfony\\Component\\Validator\\Constraints\\MaxValidator`  |
+----------------+--------------------------------------------------------------------+

Uso di base
-----------

Per verificare che il campo "age" di una classe non sia maggiore di "50", si potrebbe
aggiungere il seguente:

.. configuration-block::

    .. code-block:: yaml

        # src/Acme/EventBundle/Resources/config/validation.yml
        Acme\EventBundle\Entity\Participant:
            properties:
                age:
                    - Max: { limit: 50, message: You must be 50 or under to enter. }

    .. code-block:: php-annotations

        // src/Acme/EventBundle/Entity/Participant.php
        namespace Acme\EventBundle\Entity;

        use Symfony\Component\Validator\Constraints as Assert;

        class Participant
        {
            /**
             * @Assert\Max(limit = 50, message = "You must be 50 or under to enter.")
             */
             protected $age;
        }

    .. code-block:: xml

        <!-- src/Acme/EventBundle/Resources/config/validation.yml -->
        <?xml version="1.0" encoding="UTF-8" ?>
        <constraint-mapping xmlns="http://symfony.com/schema/dic/constraint-mapping"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://symfony.com/schema/dic/constraint-mapping http://symfony.com/schema/dic/constraint-mapping/constraint-mapping-1.0.xsd">

            <class name="Acme\EventBundle\Entity\Participant">
                <property name="age">
                    <constraint name="Max">
                        <option name="limit">50</option>
                        <option name="message">You must be 50 or under to enter.</option>
                    </constraint>
                </property>
            </class>
        </constraint-mapping>

    .. code-block:: php

        // src/Acme/EventBundle/Entity/Participant.php
        namespace Acme\EventBundle\Entity;

        use Symfony\Component\Validator\Mapping\ClassMetadata;
        use Symfony\Component\Validator\Constraints as Assert;

        class Participant
        {
            public static function loadValidatorMetadata(ClassMetadata $metadata)
            {
                $metadata->addPropertyConstraint('age', new Assert\Max(array(
                    'limit'   => 50,
                    'message' => 'You must be 50 or under to enter.',
                )));
            }
        }

Opzioni
-------

limit
~~~~~

**tipo**: ``intero`` [:ref:`opzione predefinita<validation-default-option>`]

Questa opzione obbligatoria è il valore massimo. La validazione fallisce se il valore
fornito è **maggiore** di questo.

message
~~~~~~~

**tipo**: ``stringa`` **predefinito**: ``This value should be {{ limit }} or less``

Messaggio mostrato se il valore sottostante è maggiore dell'opzione
`limit`_.

invalidMessage
~~~~~~~~~~~~~~

**tipo**: ``stringa`` **predefinito**: ``This value should be a valid number``

Messaggio mostrato se il valore sottostante non è un numero (in base alla funzione
:phpfunction:`is_numeric` di PHP).
