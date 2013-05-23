Luhn
====

.. versionadded:: 2.2
    La validazione Luhn è nuova in Symfony 2.2.

Questo vincolo è usato per assicurarsi che un numero di carta di credito passi la `formula di Luhn`_.
È utile come primo passo per validare una carta di credito: prima di comunicare con
gateway di pagamento.

+----------------+-----------------------------------------------------------------------+
| Si applica a   | :ref:`proprietò o metodo<validation-property-target>`                 |
+----------------+-----------------------------------------------------------------------+
| Opzioni        | - `message`_                                                          |
+----------------+-----------------------------------------------------------------------+
| Classe         | :class:`Symfony\\Component\\Validator\\Constraints\\Luhn`             |
+----------------+-----------------------------------------------------------------------+
| Validatore     | :class:`Symfony\\Component\\Validator\\Constraints\\LuhnValidator`    |
+----------------+-----------------------------------------------------------------------+

Uso di base
-----------

Per usare il validatore Luhn, applicarlo semplicemente a una proprietà o a un oggetto che
conterrà un numero di carta di credito.

.. configuration-block::

    .. code-block:: yaml

        # src/Acme/SubscriptionBundle/Resources/config/validation.yml
        Acme\SubscriptionBundle\Entity\Transaction:
            properties:
                cardNumber:
                    - Luhn:
                        message: Verificare il numero di carta di credito.

    .. code-block:: xml

        <!-- src/Acme/SubscriptionBundle/Resources/config/validation.xml -->
        <class name="Acme\SubscriptionBundle\Entity\Transaction">
            <property name="cardNumber">
                <constraint name="Luhn">
                    <option name="message">Verificare il numero di carta di credito.</option>
                </constraint>
            </property>
        </class>

    .. code-block:: php-annotations

        // src/Acme/SubscriptionBundle/Entity/Transaction.php
        use Symfony\Component\Validator\Constraints as Assert;

        class Transaction
        {
            /**
             * @Assert\Luhn(message = "Verificare il numero di carta di credito.")
             */
            protected $cardNumber;
        }

    .. code-block:: php

        // src/Acme/SubscriptionBundle/Entity/Transaction.php
        use Symfony\Component\Validator\Mapping\ClassMetadata;
        use Symfony\Component\Validator\Constraints\Luhn;

        class Transaction
        {
            protected $cardNumber;

            public static function loadValidatorMetadata(ClassMetadata $metadata)
            {
                $metadata->addPropertyConstraint('luhn', new Luhn(array(
                    'message' => 'Verificare il numero di carta di credito.',
                )));
            }
        }

Opzioni disponibili
-------------------

message
~~~~~~~

**tipo**: ``stringa`` **predefinito**: ``Invalid card number``

Il messaggio predefinito fornito quando il valore non passa la formula di Luhn.

.. _`formula di Luhn`: http://it.wikipedia.org/wiki/Formula_di_Luhn