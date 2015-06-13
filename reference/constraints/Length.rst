Length
======

Valida che la lunghezza di una data stringa sia *tra* un minimo e un
massimo.

+----------------+----------------------------------------------------------------------+
| Si applica a   | :ref:`proprietà o metodo<validation-property-target>`                |
+----------------+----------------------------------------------------------------------+
| Opzioni        | - `min`_                                                             |
|                | - `max`_                                                             |
|                | - `charset`_                                                         |
|                | - `minMessage`_                                                      |
|                | - `maxMessage`_                                                      |
|                | - `exactMessage`_                                                    |
+----------------+----------------------------------------------------------------------+
| Classe         | :class:`Symfony\\Component\\Validator\\Constraints\\Length`          |
+----------------+----------------------------------------------------------------------+
| Validatore     | :class:`Symfony\\Component\\Validator\\Constraints\\LengthValidator` |
+----------------+----------------------------------------------------------------------+

Utilizzo di base
----------------

Per verificare che il campo ``firstName`` di una classe sia tra "2"
e "50", si può fare come segue:

.. configuration-block::

    .. code-block:: php-annotations

        // src/Acme/EventBundle/Entity/Participant.php
        namespace Acme\EventBundle\Entity;

        use Symfony\Component\Validator\Constraints as Assert;

        class Participant
        {
            /**
             * @Assert\Length(
             *      min = 2,
             *      max = 50,
             *      minMessage = "Il nome deve essere lungo almeno {{ limit }} caratteri.",
             *      maxMessage = "Il nome non può essere più lungo di {{ limit }} caratteri."
             * )
             */
             protected $firstName;
        }

    .. code-block:: yaml

        # src/Acme/EventBundle/Resources/config/validation.yml
        Acme\EventBundle\Entity\Participant:
            properties:
                firstName:
                    - Length:
                        min: 2
                        max: 50
                        minMessage: Il nome deve essere lungo almeno {{ limit }} caratteri.
                        maxMessage: Il nome non può essere più lungo di {{ limit }} caratteri.

    .. code-block:: xml

        <!-- src/Acme/EventBundle/Resources/config/validation.xml -->
        <?xml version="1.0" encoding="UTF-8" ?>
        <constraint-mapping xmlns="http://symfony.com/schema/dic/constraint-mapping"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://symfony.com/schema/dic/constraint-mapping http://symfony.com/schema/dic/constraint-mapping/constraint-mapping-1.0.xsd">

            <class name="Acme\EventBundle\Entity\Participant">
                <property name="firstName">
                    <constraint name="Length">
                        <option name="min">2</option>
                        <option name="max">50</option>
                        <option name="minMessage">
                            Il nome deve essere lungo almeno {{ limit }} caratteri.
                        </option>
                        <option name="maxMessage">
                            Il nome non può essere più lungo di {{ limit }} caratteri.
                        </option>
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
                $metadata->addPropertyConstraint('firstName', new Assert\Length(array(
                    'min'        => 2,
                    'max'        => 50,
                    'minMessage' => 'Il nome deve essere lungo almeno {{ limit }} caratteri.',
                    'maxMessage' => 'Il nome non può essere più lungo di {{ limit }} caratteri.',
                )));
            }
        }

Options
-------

min
~~~

**tipo**: ``intero``

Questa opzione obbligatoria è il valore minimo. La validazione fallirà se il
valore dato è **inferiore** a questo valore.

È importante notare che valori nulli e stringhe vuote sono considerati
validi, anche se il vincolo richiede una lunghezza minima. I validatori entrano in
azione solo se il valore non è vuoto.

max
~~~

**tipo**: ``intero``

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

**tipo**: ``stringa`` **predefinito**: ``This value is too short. It should have {{ limit }} characters or more.``

Il messaggio mostrato se il valore sottostante è inferiore a quello
dell'opzione `min`_.

maxMessage
~~~~~~~~~~

**tipo**: ``stringa`` **predefinito**: ``This value is too long. It should have {{ limit }} characters or less.``

Il messaggio mostrato se il valore sottostante è superiore a quello
dell'opzione `max`_.

exactMessage
~~~~~~~~~~~~

**tipo**: ``string`` **predefinito**: ``This value should have exactly {{ limit }} characters.``

Il messaggio mostrato se i valori minimo e massimo sono uguali e la lunghezza del valore
sottostante o il numero di elementi dell'insieme non è esattamente tale valore.
