Max
===

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
        use Symfony\Component\Validator\Constraints as Assert;

        class Participant
        {
            /**
             * @Assert\Max(limit = 50, message = "You must be 50 or under to enter.")
             */
             protected $age;
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
`is_numeric`_ di PHP).

.. _`is_numeric`: http://www.php.net/manual/en/function.is-numeric.php
