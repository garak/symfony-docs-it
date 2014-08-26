Currency
========

.. versionadded:: 2.3
    Questo vincolo è nuovo nella versione 2.3.

Valida che un valore sia un nome di valuta valido rispetto allo standard `ISO 4217`_.

+----------------+---------------------------------------------------------------------------+
| Si applica a   | :ref:`proprietà o metodo<validation-property-target>`                     |
+----------------+---------------------------------------------------------------------------+
| Opzioni        | - `message`_                                                              |
+----------------+---------------------------------------------------------------------------+
| Classe         | :class:`Symfony\\Component\\Validator\\Constraints\\Currency`             |
+----------------+---------------------------------------------------------------------------+
| Validatore     | :class:`Symfony\\Component\\Validator\\Constraints\\CurrencyValidator`    |
+----------------+---------------------------------------------------------------------------+

Uso di base
-----------

Se ci si vuole assicurare che la proprietà ``currency`` di un oggetto ``Order`` sia una
valuta valida, si può fare come segue:

.. configuration-block::

    .. code-block:: yaml

        # src/Acme/EcommerceBundle/Resources/config/validation.yml
        Acme\EcommerceBundle\Entity\Order:
            properties:
                currency:
                    - Currency: ~

    .. code-block:: php-annotations

        // src/Acme/EcommerceBundle/Entity/Order.php
        namespace Acme\EcommerceBundle\Entity;

        use Symfony\Component\Validator\Constraints as Assert;

        class Order
        {
            /**
             * @Assert\Currency
             */
            protected $currency;
        }

    .. code-block:: xml

        <!-- src/Acme/EcommerceBundle/Resources/config/validation.xml -->
        <?xml version="1.0" encoding="UTF-8" ?>
        <constraint-mapping xmlns="http://symfony.com/schema/dic/constraint-mapping"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://symfony.com/schema/dic/constraint-mapping http://symfony.com/schema/dic/constraint-mapping/constraint-mapping-1.0.xsd">

            <class name="Acme\EcommerceBundle\Entity\Order">
                <property name="currency">
                    <constraint name="Currency" />
                </property>
            </class>
        </constraint-mapping>

    .. code-block:: php

        // src/Acme/EcommerceBundle/Entity/Order.php
        namespace Acme\SocialBundle\Entity;

        use Symfony\Component\Validator\Mapping\ClassMetadata;
        use Symfony\Component\Validator\Constraints as Assert;

        class Order
        {
            public static function loadValidatorMetadata(ClassMetadata $metadata)
            {
                $metadata->addPropertyConstraint('currency', new Assert\Currency());
            }
        }

Opzioni
-------

message
~~~~~~~

**tipo**: ``stringa`` **predefinito**: ``This value is not a valid currency.``

Messaggio mostrato se il valore non è una valuta valida.

.. _`ISO 4217`: http://it.wikipedia.org/wiki/ISO_4217 
