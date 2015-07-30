Ip
==

Valida che un valore sia un indirizzo IP valido. Per impostazione predefinita, valida
un valore come IPv4, ma ci sono diverse opzioni per validare come IPv6 e
altre combinazioni.

+----------------+---------------------------------------------------------------------+
| Si applica a   | :ref:`proprietà o metodo <validation-property-target>`              |
+----------------+---------------------------------------------------------------------+
| Opzioni        | - `version`_                                                        |
|                | - `message`_                                                        |
+----------------+---------------------------------------------------------------------+
| Classe         | :class:`Symfony\\Component\\Validator\\Constraints\\Ip`             |
+----------------+---------------------------------------------------------------------+
| Validatore     | :class:`Symfony\\Component\\Validator\\Constraints\\IpValidator`    |
+----------------+---------------------------------------------------------------------+

Uso di base
-----------

.. configuration-block::

    .. code-block:: php-annotations

        // src/Acme/BlogBundle/Entity/Author.php
        namespace Acme\BlogBundle\Entity;

        use Symfony\Component\Validator\Constraints as Assert;

        class Author
        {
            /**
             * @Assert\Ip
             */
             protected $ipAddress;
        }

    .. code-block:: yaml

        # src/Acme/BlogBundle/Resources/config/validation.yml
        Acme\BlogBundle\Entity\Author:
            properties:
                ipAddress:
                    - Ip: ~

    .. code-block:: xml

        <!-- src/Acme/BlogBundle/Resources/config/validation.xml -->
        <?xml version="1.0" encoding="UTF-8" ?>
        <constraint-mapping xmlns="http://symfony.com/schema/dic/constraint-mapping"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://symfony.com/schema/dic/constraint-mapping http://symfony.com/schema/dic/constraint-mapping/constraint-mapping-1.0.xsd">

            <class name="Acme\BlogBundle\Entity\Author">
                <property name="ipAddress">
                    <constraint name="Ip" />
                </property>
            </class>
        </constraint-mapping>

    .. code-block:: php

        // src/Acme/BlogBundle/Entity/Author.php
        namespace Acme\BlogBundle\Entity;

        use Symfony\Component\Validator\Mapping\ClassMetadata;
        use Symfony\Component\Validator\Constraints as Assert;

        class Author
        {
            public static function loadValidatorMetadata(ClassMetadata $metadata)
            {
                $metadata->addPropertyConstraint('ipAddress', new Assert\Ip());
            }
        }

Opzioni
-------

version
~~~~~~~

**tipo**: ``stringa`` **predefinito**: ``4``

Determina esattamente *come* è validato l'indirizzo IP e accetta uno dei
tanti valori a disposizione:

**Tutte le fasce**

``4``
    Valida indirizzi IPv4
``6``
    Valida indirizzi IPv6
``all``
    Valida tutti i formati IP

**Nessuna fascia privata**

``4_no_priv``
    Valida IPv4, ma senza le fasce IP private
``6_no_priv``
    Valida IPv6, ma senza le fasce IP private
``all_no_priv``
    Valida tutti i formati IP, ma senza le fasce IP private

**Nessuna fascia riservata**

``4_no_res``
    Valida IPv4, ma senza le fasce IP riservate
``6_no_res``
    Valida IPv6, ma senza le fasce IP riservate
``all_no_res``
    Valida tutti i formati IP, ma senza le fasce IP riservate

**Solo fasce pubbliche**

``4_public``
    Valida IPv4, ma senza fasce private e riservate
``6_public``
    Valida IPv6, ma senza fasce private e riservate
``all_public``
    Valida tutti i formati IP, ma senza le fasce IP private e riservate

message
~~~~~~~

**tipo**: ``stringa`` **predefinito**: ``This is not a valid IP address``

Messaggio mostrato se la stringa non è un indirizzo IP valido.
