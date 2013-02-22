Min
===

.. caution::

    Questo vincolo è deprecato dalla versione 2.1 e sarà rimosso
    in Symfony 2.3. Usare :doc:`/reference/constraints/Range` con opzione ``min``
    al suo posto.

Valida che un dato numero sia *maggiore* di un numero minimo.

+----------------+--------------------------------------------------------------------+
| Si applica a   | :ref:`proprietà o metodo<validation-property-target>`              |
+----------------+--------------------------------------------------------------------+
| Opzioni        | - `limit`_                                                         |
|                | - `message`_                                                       |
|                | - `invalidMessage`_                                                |
+----------------+--------------------------------------------------------------------+
| Classe         | :class:`Symfony\\Component\\Validator\\Constraints\\Min`           |
+----------------+--------------------------------------------------------------------+
| Validatore     | :class:`Symfony\\Component\\Validator\\Constraints\\MinValidator`  |
+----------------+--------------------------------------------------------------------+

Uso di base
-----------

Per verificare che il campo "age" di una classe sia "18" o più, si potrebbe
aggiungere il seguente:

.. configuration-block::

    .. code-block:: yaml

        # src/Acme/EventBundle/Resources/config/validation.yml
        Acme\EventBundle\Entity\Participant:
            properties:
                age:
                    - Min: { limit: 18, message: You must be 18 or older to enter. }

    .. code-block:: php-annotations

        // src/Acme/EventBundle/Entity/Participant.php
        namespace Acme\EventBundle\Entity;

        use Symfony\Component\Validator\Constraints as Assert;

        class Participant
        {
            /**
             * @Assert\Min(limit = "18", message = "You must be 18 or older to enter")
             */
             protected $age;
        }

    .. code-block:: xml

        <!-- src/Acme/EventBundle/Resources/config/validation.yml -->
        <class name="Acme\EventBundle\Entity\Participant">
            <property name="age">
                <constraint name="Min">
                    <option name="limit">18</option>
                    <option name="message">You must be 18 or older to enter.</option>
                </constraint>
            </property>
        </class>

    .. code-block:: php

        // src/Acme/EventBundle/Entity/Participant.php
        namespace Acme\EventBundle\Entity\Participant;

        use Symfony\Component\Validator\Mapping\ClassMetadata;
        use Symfony\Component\Validator\Constraints as Assert;

        class Participant
        {
            public static function loadValidatorMetadata(ClassMetadata $metadata)
            {
                $metadata->addPropertyConstraint('age', new Assert\Min(array(
                    'limit'   => '18',
                    'message' => 'You must be 18 or older to enter.',
                ));
            }
        }

Opzioni
-------

limit
~~~~~

**tipo**: ``intero`` [:ref:`opzione predefinita<validation-default-option>`]

Questa opzione obbligatoria è il valore minimo. La validazione fallisce se il valore
fornito è **minore** di questo.

message
~~~~~~~

**tipo**: ``stringa`` **predefinito**: ``This value should be {{ limit }} or more``

Messaggio mostrato se il valore sottostante è minore dell'opzione
`limit`_.

invalidMessage
~~~~~~~~~~~~~~

**tipo**: ``stringa`` **predefinito**: ``This value should be a valid number``

Messaggio mostrato se il valore sottostante non è un numero (in base alla funzione
:phpfunction:`is_numeric` di PHP).
