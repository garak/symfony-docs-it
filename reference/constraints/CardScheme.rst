CardScheme
==========

.. versionadded:: 2.2
    La validazione CardScheme è nuova in Symfony 2.2.

Questo vincolo assicura che un numero di carta di credito sia valido per una data compagnia
di carte di credito. Può essere usato per validare il numero prima di provare a iniziare un pagamento
tramite un gateway.

+----------------+--------------------------------------------------------------------------+
| Si applica a   | :ref:`proprietà o metodo <validation-property-target>`                   |
+----------------+--------------------------------------------------------------------------+
| Opzioni        | - `schemes`_                                                             |
|                | - `message`_                                                             |
+----------------+--------------------------------------------------------------------------+
| Classe         | :class:`Symfony\\Component\\Validator\\Constraints\\CardScheme`          |
+----------------+--------------------------------------------------------------------------+
| Validatore     | :class:`Symfony\\Component\\Validator\\Constraints\\CardSchemeValidator` |
+----------------+--------------------------------------------------------------------------+

Uso di base
-----------

Per usare il validatore ``CardScheme``, aggiungerlo semplicemente a una proprietà o a un metodo
su un oggetto che conterrà un numero di carta di credito.

.. configuration-block::

    .. code-block:: php-annotations

        // src/Acme/SubscriptionBundle/Entity/Transaction.php
        namespace Acme\SubscriptionBundle\Entity\Transaction;

        use Symfony\Component\Validator\Constraints as Assert;

        class Transaction
        {
            /**
             * @Assert\CardScheme(
             *     schemes={"VISA"},
             *     message="Numero di carta di credito non valido."
             * )
             */
            protected $cardNumber;
        }

    .. code-block:: yaml

        # src/Acme/SubscriptionBundle/Resources/config/validation.yml
        Acme\SubscriptionBundle\Entity\Transaction:
            properties:
                cardNumber:
                    - CardScheme:
                        schemes: [VISA]
                        message: Numero di carta di credito non valido.

    .. code-block:: xml

        <!-- src/Acme/SubscriptionBundle/Resources/config/validation.xml -->
        <?xml version="1.0" encoding="UTF-8" ?>
        <constraint-mapping xmlns="http://symfony.com/schema/dic/constraint-mapping"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://symfony.com/schema/dic/constraint-mapping http://symfony.com/schema/dic/constraint-mapping/constraint-mapping-1.0.xsd">

            <class name="Acme\SubscriptionBundle\Entity\Transaction">
                <property name="cardNumber">
                    <constraint name="CardScheme">
                        <option name="schemes">
                            <value>VISA</value>
                        </option>
                        <option name="message">Numero di carta di credito non valido.</option>
                    </constraint>
                </property>
            </class>
        </constraint-mapping>

    .. code-block:: php

        // src/Acme/SubscriptionBundle/Entity/Transaction.php
        namespace Acme\SubscriptionBundle\Entity\Transaction;

        use Symfony\Component\Validator\Mapping\ClassMetadata;
        use Symfony\Component\Validator\Constraints as Assert;

        class Transaction
        {
            protected $cardNumber;

            public static function loadValidatorMetadata(ClassMetadata $metadata)
            {
                $metadata->addPropertyConstraint('cardNumber', new Assert\CardScheme(array(
                    'schemes' => array(
                        'VISA'
                    ),
                    'message' => 'Numero di carta di credito non valido.',
                )));
            }
        }

Opzioni disponibili
-------------------

schemes
-------

**tipo**: ``mixed`` [:ref:`opzioni predefinite<validation-default-option>`]

Questa opzione è obbligatoria e rappresenta il nome dello schema usato per
validare la carta di credito, sia esso una stringa o un array. Valori
validi sono:

* ``AMEX``
* ``CHINA_UNIONPAY``
* ``DINERS``
* ``DISCOVER``
* ``INSTAPAYMENT``
* ``JCB``
* ``LASER``
* ``MAESTRO``
* ``MASTERCARD``
* ``VISA``

Per maggiori infomazioni sugli schemi usati, vedere
`Wikipedia: Issuer identification number (IIN)`_.

message
~~~~~~~

**tipo**: ``stringa`` **predefinito**: ``Unsupported card type or invalid card number``

Il messaggio mostrato quando il valore non passa il controllo ``CardScheme``.

.. _`Wikipedia: Issuer identification number (IIN)`: http://en.wikipedia.org/wiki/Bank_card_number#Issuer_identification_number_.28IIN.29
