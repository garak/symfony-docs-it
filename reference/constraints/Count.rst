Count
=====

Valida che un dato insieme (p.e. un array o un oggetto che implementi Countable)
abbia un conteggio *tra* un valore minimo e uno massimo.

+----------------+---------------------------------------------------------------------+
| Si applica a   | :ref:`proprietà o metodo<validation-property-target>`               |
+----------------+---------------------------------------------------------------------+
| Opzioni        | - `min`_                                                            |
|                | - `max`_                                                            |
|                | - `minMessage`_                                                     |
|                | - `maxMessage`_                                                     |
|                | - `exactMessage`_                                                   |
+----------------+---------------------------------------------------------------------+
| Classe         | :class:`Symfony\\Component\\Validator\\Constraints\\Count`          |
+----------------+---------------------------------------------------------------------+
| Validatore     | :class:`Symfony\\Component\\Validator\\Constraints\\CountValidator` |
+----------------+---------------------------------------------------------------------+

Uso di base
-----------

Per verificare che un campo array ``emails`` contenga tra 1 e 5 elementi, si potrebbe
fare come segue:

.. configuration-block::

    .. code-block:: php-annotations

        // src/Acme/EventBundle/Entity/Participant.php
        namespace Acme\EventBundle\Entity;

        use Symfony\Component\Validator\Constraints as Assert;

        class Participant
        {
            /**
             * @Assert\Count(
             *      min = "1",
             *      max = "5",
             *      minMessage = "Specificare almeno una email",
             *      maxMessage = "Specificare non più di {{ limit }} email"
             * )
             */
             protected $emails = array();
        }

    .. code-block:: yaml

        # src/Acme/EventBundle/Resources/config/validation.yml
        Acme\EventBundle\Entity\Participant:
            properties:
                emails:
                    - Count:
                        min: 1
                        max: 5
                        minMessage: Specificare almeno una email
                        maxMessage: Specificare non più di {{ limit }} email

    .. code-block:: xml

        <!-- src/Acme/EventBundle/Resources/config/validation.xml -->
        <?xml version="1.0" encoding="UTF-8" ?>
        <constraint-mapping xmlns="http://symfony.com/schema/dic/constraint-mapping"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://symfony.com/schema/dic/constraint-mapping http://symfony.com/schema/dic/constraint-mapping/constraint-mapping-1.0.xsd">

            <class name="Acme\EventBundle\Entity\Participant">
                <property name="emails">
                    <constraint name="Count">
                        <option name="min">1</option>
                        <option name="max">5</option>
                        <option name="minMessage">Specificare almeno una email</option>
                        <option name="maxMessage">Specificare non più di {{ limit }} email</option>
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
                $metadata->addPropertyConstraint('emails', new Assert\Count(array(
                    'min'        => 1,
                    'max'        => 5,
                    'minMessage' => 'You must specify at least one email',
                    'maxMessage' => 'You cannot specify more than {{ limit }} emails',
                )));
            }
        }

Opzioni
-------

min
~~~

**tipo**: ``intero`` [:ref:`opzione predefinita<validation-default-option>`]

Questa opzione obbligatoria è il valore "min". La validazione fallirà se gli elementi
dell'insieme dato sono in numero **inferiore** a questo valore.

max
~~~

**tipo**: ``intero`` [:ref:`opzione predefinita<validation-default-option>`]

Questa opzione obbligatoria è il valore "max". La validazione fallirà se gli elementi
dell'insieme dato sono in numero **superiore** a questo valore.

minMessage
~~~~~~~~~~

**tipo**: ``stringa`` **predefinito**: ``This collection should contain {{ limit }} elements or more.``

Messaggio mostrato se gli elementi dell'insieme sottostante sono meno dell'opzione
`min`_.

maxMessage
~~~~~~~~~~

**tipo**: ``stringa`` **predefinito**: ``This collection should contain {{ limit }} elements or less.``

Messaggio mostrato se gli elementi dell'insieme sottostante sono più dell'opzione
`max`_.

exactMessage
~~~~~~~~~~~~

**tipo**: ``stringa`` **predefinito**: ``This collection should contain exactly {{ limit }} elements.``

Messaggio mostrato se min e max sono uguali e gli elementi dell'insieme sottostante non
sono esattamente pari a tale valore.
