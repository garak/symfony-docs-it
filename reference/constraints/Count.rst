Count
=====

Valida che un dato insieme (p.e. un array o un oggetto che implementi Countable)
abbia un conteggio *tra* un valore minimo e uno massimo.

.. versionadded:: 2.1
    Il vincolo Count è stato aggiunto in Symfony 2.1.

+----------------+---------------------------------------------------------------------+
| Si applica a   | :ref:`property or method<validation-property-target>`               |
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

    .. code-block:: yaml

        # src/Acme/EventBundle/Resources/config/validation.yml
        Acme\EventBundle\Entity\Participant:
            properties:
                emails:
                    - Count:
                        min: 1
                        max: 5
                        minMessage: Specificare almeno una email
                        maxMessage: Specificare non più di 5 email

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
             *      maxMessage = "Specificare non più di 5 email"
             * )
             */
             protected $emails = array();
        }

    .. code-block:: xml

        <!-- src/Acme/EventBundle/Resources/config/validation.xml -->
        <class name="Acme\EventBundle\Entity\Participant">
            <property name="emails">
                <constraint name="Count">       
                    <option name="min">1</option> 
                    <option name="max">5</option> 
                    <option name="minMessage">You must specify at least one email</option>
                    <option name="maxMessage">You cannot specify more than {{ limit }} emails</option>
                </constraint>
            </property>
        </class>

    .. code-block:: php

        // src/Acme/EventBundle/Entity/Participant.php
        namespace Acme\EventBundle\Entity;

        use Symfony\Component\Validator\Mapping\ClassMetadata;
        use Symfony\Component\Validator\Constraints as Assert;

        class Participant
        {
            public static function loadValidatorMetadata(ClassMetadata $data)
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

Questa opzione obbligatoria è il valore "min". La validazione fallità se gli elementi
dell'insieme dato sono in numero **inferiore** a questo valore.

max
~~~

**tipo**: ``intero`` [:ref:`opzione predefinita<validation-default-option>`]

Questa opzione obbligatoria è il valore "max". La validazione fallità se gli elementi
dell'insieme dato sono in numero **superiore** a questo valore.

minMessage
~~~~~~~~~~

**tipo**: ``stringa`` **predefinito**: ``This collection should contain {{ limit }} elements or more.``.

Messaggio mostrato se gli elementi dell'insieme sottostante sono meno dell'opzione `min`_.

maxMessage
~~~~~~~~~~

**tipo**: ``stringa`` **predefinito**: ``This collection should contain {{ limit }} elements or less.``.

Messaggio mostrato se gli elementi dell'insieme sottostante sono più dell'opzione `max`_.

exactMessage
~~~~~~~~~~~~

**tipo**: ``stringa`` **predefinito**: ``This collection should contain exactly {{ limit }} elements.``.

Messaggio mostrato se min e max sono uguali e gli elementi dell'insieme sottostante non
sono esattamente pari a tale valore.
