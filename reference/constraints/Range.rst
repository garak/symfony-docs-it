Range
=====

Valida che un dato numero sia *tra* un minimo e un massimo.

+----------------+---------------------------------------------------------------------+
| Si applica a   | :ref:`property or method<validation-property-target>`               |
+----------------+---------------------------------------------------------------------+
| Opzioni        | - `min`_                                                            |
|                | - `max`_                                                            |
|                | - `minMessage`_                                                     |
|                | - `maxMessage`_                                                     |
|                | - `invalidMessage`_                                                 |
+----------------+---------------------------------------------------------------------+
| Classe         | :class:`Symfony\\Component\\Validator\\Constraints\\Range`          |
+----------------+---------------------------------------------------------------------+
| Validatore     | :class:`Symfony\\Component\\Validator\\Constraints\\RangeValidator` |
+----------------+---------------------------------------------------------------------+

Uso di base
-----------

Per verificare che un campo "height" di una classe sia tra "120" e "180", si può aggiungere
quanto segue:

.. configuration-block::

    .. code-block:: yaml

        # src/Acme/EventBundle/Resources/config/validation.yml
        Acme\EventBundle\Entity\Participant:
            properties:
                height:
                    - Range:
                        min: 120
                        max: 180
                        minMessage: Devi essere alto almeno 120cm per entrare
                        maxMessage: Non puoi essere più alto di 180cm per entrare

    .. code-block:: php-annotations

        // src/Acme/EventBundle/Entity/Participant.php
        namespace Acme\EventBundle\Entity;

        use Symfony\Component\Validator\Constraints as Assert;

        class Participant
        {
            /**
             * @Assert\Range(
             *      min = "120",
             *      max = "180",
             *      minMessage = "Devi essere alto almeno 120cm per entrare",
             *      maxMessage="Non puoi essere più alto di 180cm per entrare"
             * )
             */
             protected $height;
        }

    .. code-block:: xml

        <!-- src/Acme/EventBundle/Resources/config/validation.xml -->
        <?xml version="1.0" encoding="UTF-8" ?>
        <constraint-mapping xmlns="http://symfony.com/schema/dic/constraint-mapping"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://symfony.com/schema/dic/constraint-mapping http://symfony.com/schema/dic/constraint-mapping/constraint-mapping-1.0.xsd">

            <class name="Acme\EventBundle\Entity\Participant">
                <property name="height">
                    <constraint name="Range">
                        <option name="min">120</option>
                        <option name="max">180</option>
                        <option name="minMessage">Devi essere alto almeno 120cm per entrare</option>
                        <option name="maxMessage">Non puoi essere più alto di 180cm per entrare</option>
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
                $metadata->addPropertyConstraint('height', new Assert\Range(array(
                    'min'        => 120,
                    'max'        => 180,
                    'minMessage' => 'Devi essere alto almeno 120cm per entrare',
                    'maxMessage' => 'Non puoi essere più alto di 180cm per entrare',
                )));
            }
        }

Opzioni
-------

min
~~~

**tipo**: ``intero`` [:ref:`default option<validation-default-option>`]

Questa opzione obbligatoria è il valore minimo. La validazione fallirà se il valore dato
è *inferiore* a questo valore.

max
~~~

**tipo**: ``intero`` [:ref:`default option<validation-default-option>`]

Questa opzione obbligatoria è il valore massimo. La validazione fallirà se il valore dato
è *superiore* a questo valore.

minMessage
~~~~~~~~~~

**tipo**: ``stringa`` **predefinito**: ``This value should be {{ limit }} or more.``

Il messaggio mostrato se il valore sottostante è inferiore all'opzione
`min`_.

maxMessage
~~~~~~~~~~

**tipo**: ``string`` **default**: ``This value should be {{ limit }} or less.``

Il messaggio mostrato se il valore sottostante è superiore all'opzione
`max`_.

invalidMessage
~~~~~~~~~~~~~~

**tipo**: ``string`` **default**: ``This value should be a valid number.``

Il messaggio mostrato se il valore sottostante non è un numero (in base
alla funzione `is_numeric`_ di PHP).

.. _`is_numeric`: http://php.net/manual/it/function.is-numeric.php
