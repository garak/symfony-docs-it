Iban
====

.. versionadded:: 2.3
    Il vincolo Iban è stato aggiunto in Symfony 2.3.

Questo vincolo è usato per verificare che un numero di conto bancario abbia il giusto formato
di `International Bank Account Number (IBAN)`_.L'IBAN è un mezzo internazionale per
identificare i conti bancari, con un rischio ridotto di propagare
errori di trascrizione.

+----------------+-----------------------------------------------------------------------+
| Si applica     | :ref:`proprietà o metodo <validation-property-target>`                |
+----------------+-----------------------------------------------------------------------+
| Opzioni        | - `message`_                                                          |
+----------------+-----------------------------------------------------------------------+
| Classe         | :class:`Symfony\\Component\\Validator\\Constraints\\Iban`             |
+----------------+-----------------------------------------------------------------------+
| Validatore     | :class:`Symfony\\Component\\Validator\\Constraints\\IbanValidator`    |
+----------------+-----------------------------------------------------------------------+

Uso di base
-----------

Per usare il validatore Iban, applicarlo semplicemente a una proprietà di un oggetto
che conterrà un IBAN.

.. configuration-block::

    .. code-block:: php-annotations

        // src/Acme/SubscriptionBundle/Entity/Transaction.php
        namespace Acme\SubscriptionBundle\Entity;

        use Symfony\Component\Validator\Constraints as Assert;

        class Transaction
        {
            /**
             * @Assert\Iban(
             *     message="This is not a valid International Bank Account Number (IBAN)."
             * )
             */
            protected $bankAccountNumber;
        }

    .. code-block:: yaml

        # src/Acme/SubscriptionBundle/Resources/config/validation.yml
        Acme\SubscriptionBundle\Entity\Transaction:
            properties:
                bankAccountNumber:
                    - Iban:
                        message: This is not a valid International Bank Account Number (IBAN).

    .. code-block:: xml

        <!-- src/Acme/SubscriptionBundle/Resources/config/validation.xml -->
        <?xml version="1.0" encoding="UTF-8" ?>
        <constraint-mapping xmlns="http://symfony.com/schema/dic/constraint-mapping"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://symfony.com/schema/dic/constraint-mapping http://symfony.com/schema/dic/constraint-mapping/constraint-mapping-1.0.xsd">

            <class name="Acme\SubscriptionBundle\Entity\Transaction">
                <property name="bankAccountNumber">
                    <constraint name="Iban">
                        <option name="message">
                            This is not a valid International Bank Account Number (IBAN).
                        </option>
                    </constraint>
                </property>
            </class>
        </constraint-mapping>

    .. code-block:: php

        // src/Acme/SubscriptionBundle/Entity/Transaction.php
        namespace Acme\SubscriptionBundle\Entity;

        use Symfony\Component\Validator\Mapping\ClassMetadata;
        use Symfony\Component\Validator\Constraints as Assert;

        class Transaction
        {
            protected $bankAccountNumber;

            public static function loadValidatorMetadata(ClassMetadata $metadata)
            {
                $metadata->addPropertyConstraint('bankAccountNumber', new Assert\Iban(array(
                    'message' => 'This is not a valid International Bank Account Number (IBAN).',
                )));
            }
        }

Opzioni disponibili
-------------------

message
~~~~~~~

**tipo**: ``stringa`` **predefinito**: ``This is not a valid International Bank Account Number (IBAN).``

Messaggio fornito quando il valore non passa il controllo Iban.

.. _`International Bank Account Number (IBAN)`: http://it.wikipedia.org/wiki/International_Bank_Account_Number
