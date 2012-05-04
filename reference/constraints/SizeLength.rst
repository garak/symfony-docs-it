SizeLength
==========

Valida che la lunghezza di una data stringa sia *tra* un minimo e un massimo, in base al charset fornito.

+----------------+--------------------------------------------------------------------------+
| Si applica a   | :ref:`proprietà o metodo<validation-property-target>`                    |
+----------------+--------------------------------------------------------------------------+
| Opzioni        | - `min`_                                                                 |
|                | - `max`_                                                                 |
|                | - `charset`_                                                             |
|                | - `minMessage`_                                                          |
|                | - `maxMessage`_                                                          |
|                | - `exactMessage`_                                                        |
+----------------+--------------------------------------------------------------------------+
| Classe         | :class:`Symfony\\Component\\Validator\\Constraints\\SizeLength`          |
+----------------+--------------------------------------------------------------------------+
| Validatore     | :class:`Symfony\\Component\\Validator\\Constraints\\SizeLengthValidator` |
+----------------+--------------------------------------------------------------------------+

Utilizzo di base
----------------

Per verificare che la lunghezza del campo ``firstName`` di una classe sia tra "2" e
"50", si potrebbe fare come segue:

.. configuration-block::

    .. code-block:: yaml

        # src/Acme/EventBundle/Resources/config/validation.yml
        Acme\EventBundle\Entity\Height:
            properties:
                firstName:
                    - SizeLength:
                        min: 2
                        max: 50
                        minMessage: Il nome deve essere lungo almeno 2 caratteri
                        maxMessage: Il nome non puà essere più lungo di 50 caratteri

    .. code-block:: php-annotations

        // src/Acme/EventBundle/Entity/Participant.php
        use Symfony\Component\Validator\Constraints as Assert;

        class Participant
        {
            /**
             * @Assert\SizeLength(
             *      min = "2",
             *      max = "50",
             *      minMessage = "Il nome deve essere lungo almeno 2 caratteri",
             *      maxMessage = "Il nome non puà essere più lungo di 50 caratteri"
             * )
             */
             protected $firstName;
        }

Opzioni
-------

min
~~~

**tipo**: ``intero`` [:ref:`default option<validation-default-option>`]

Questa opzione obbligatoria è il valore minimo di lunghezza. La validazione fallirà se la
lunghezza del valore dato è **inferiore** a questo valore.

max
~~~

**tipo**: ``intero`` [:ref:`default option<validation-default-option>`]

Questa opzione obbligatoria è il valore massimo di lunghezza. La validazione fallirà se la
lunghezza del valore dato è **superiore** a questo valore.

charset
~~~~~~~

**tipo**: ``stringa`` **predefinito**: ``UTF-8``

Il charset da usare nel calcolo della lunghezza del valore. Verrà usata, se disponibile,
la funzione PHP `grapheme_strlen`_. In caso contrario, verrà usata, se disponibile, la
funzione PHP `mb_strlen`_. Se nessuna delle due funzioni è disponibile, verrà usata
la funzione PHP `strlen`_.

.. _`grapheme_strlen`: http://www.php.net/manual/en/function.grapheme_strlen.php
.. _`mb_strlen`: http://www.php.net/manual/en/function.mb_strlen.php
.. _`strlen`: http://www.php.net/manual/en/function.strlen.php

minMessage
~~~~~~~~~~

**tipo**: ``stringa`` **predefinito**: ``This value is too short. It should have {{ limit }} characters or more.``

Messaggio mostrato se la lunghezza del valore sottostante è inferiore al valore
dell'opzione `min`_.

maxMessage
~~~~~~~~~~

**tipo**: ``stringa`` **predefinito**: ``This value is too long. It should have {{ limit }} characters or less.``

Messaggio mostrato se la lunghezza del valore sottostante è superiore al valore
dell'opzione `max`_.

exactMessage
~~~~~~~~~~~~

**tipo**: ``stringa`` **predefinito**: ``This value should have exactly {{ limit }} characters.``

Messaggio mostrato se i valori di massimo e minimo sono uguale e la lungheza del valore
sottostante non è uguale a tale valore.
