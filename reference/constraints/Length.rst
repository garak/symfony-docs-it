Size
====

Valida che la lunghezza di una data stringa sia *tra* un minimo e un massimo.

.. versionadded:: 2.1
    Il vincolo Size è stato aggiunto in Symfony 2.1.

+----------------+--------------------------------------------------------------------+
| Si applica a   | :ref:`proprietà o metodo<validation-property-target>`              |
+----------------+--------------------------------------------------------------------+
| Opzioni        | - `min`_                                                           |
|                | - `max`_                                                           |
|                | - `charset`_                                                       |
|                | - `minMessage`_                                                    |
|                | - `maxMessage`_                                                    |
|                | - `exactMessage`_                                                  |
+----------------+--------------------------------------------------------------------+
| Classe         | :class:`Symfony\\Component\\Validator\\Constraints\\Size`          |
+----------------+--------------------------------------------------------------------+
| Validatore     | :class:`Symfony\\Component\\Validator\\Constraints\\SizeValidator` |
+----------------+--------------------------------------------------------------------+

Utilizzo di base
----------------

Per verificare che il campo "height" di una classe sia tra "120" e "180", si potrebbe
fare come segue:

.. configuration-block::

    .. code-block:: yaml

        # src/Acme/EventBundle/Resources/config/validation.yml
        Acme\EventBundle\Entity\Participant:
            properties:
                firstName:
                    - Length:
                        min: 2
                        max: 50
                        minMessage: Il nome deve essere lungo almeno 2 caratteri
                        maxMessage: Il nome non può essere più lungo di 50 caratteri

    .. code-block:: php-annotations

        // src/Acme/EventBundle/Entity/Participant.php
        namespace Acme\EventBundle\Entity;

        use Symfony\Component\Validator\Constraints as Assert;

        class Participant
        {
            /**
             * @Assert\Length(
             *      min = "2",
             *      max = "50",
             *      minMessage = "Il nome deve essere lungo almeno 2 caratteri",
             *      maxMessage = "Il nome non può essere più lungo di 50 caratteri"
             * )
             */
             protected $firstName;
        }

    .. code-block:: xml

        <!-- src/Acme/EventBundle/Resources/config/validation.xml -->
        <class name="Acme\EventBundle\Entity\Participant">
            <property name="firstName">
                <constraint name="Length">
                    <option name="min">2</option>
                    <option name="max">50</option>
                    <option name="minMessage">Your first name must be at least {{ limit }} characters length</option>
                    <option name="maxMessage">Your first name cannot be longer than than {{ limit }} characters length</option>
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
            public static function loadValidatorMetadata(ClassMetadata $metadata)
            {
                $metadata->addPropertyConstraint('firstName', new Assert\Length(array(
                    'min'        => 2,
                    'max'        => 50,
                    'minMessage' => 'Your first name must be at least {{ limit }} characters length',
                    'maxMessage' => 'Your first name cannot be longer than than {{ limit }} characters length',
                )));
            }
        }

Options
-------

min
~~~

**tipo**: ``intero`` [:ref:`opzione predefinita<validation-default-option>`]

Questa opzione obbligatoria è il valore minimo. La validazione fallirà se il
valore dato è **inferiore** a questo valore.

max
~~~

**tipo**: ``intero`` [:ref:`opzione predefinita<validation-default-option>`]

Questa opzione obbligatoria è il valore massimo. La validazione fallirà se il
valore dato è **superiore** a questo valore.

charset
~~~~~~~

**tipo**: ``stringa``  **predefinito**: ``UTF-8``

Il set di caratteri da usare nel calcolo della lunghezza del valore. Se disponibili, viene
usata la funzione :phpfunction:`grapheme_strlen` di PHP. Altrimenti, viene usata la funzione
:phpfunction:`mb_strlen` di PHP, se disponibile. Se nessuna delle due è disponibile. viene
usta la funzione :phpfunction:`strlen` di PHP.

minMessage
~~~~~~~~~~

**tipo**: ``stringa`` **predefinito**: ``This value should be {{ limit }} or more.``

Il messaggio mostrato se il valore sottostante è inferiore a quello dell'opzione `min`_.

maxMessage
~~~~~~~~~~

**tipo**: ``stringa`` **predefinito**: ``This value should be {{ limit }} or less.``

Il messaggio mostrato se il valore sottostante è superiore a quello dell'opzione `max`_.

exactMessage
~~~~~~~~~~~~

**tipo**: ``string`` **predefinito**: ``This value should have exactly {{ limit }} characters.`` when validating a string, or ``This collection should contain exactly {{ limit }} elements.`` when validating a collection.

Il messaggio mostrato se i valori minimo e massimo sono uguali e la lunghezza del valore
sottostante o il numero di elementi dell'insieme non è esattamente tale valore.
