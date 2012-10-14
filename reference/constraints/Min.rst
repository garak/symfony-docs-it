Min
===

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
        use Symfony\Component\Validator\Constraints as Assert;

        class Participant
        {
            /**
             * @Assert\Min(limit = "18", message = "You must be 18 or older to enter")
             */
             protected $age;
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
`is_numeric`_ di PHP).

.. _`is_numeric`: http://www.php.net/manual/en/function.is-numeric.php