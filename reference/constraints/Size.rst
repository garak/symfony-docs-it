Size
====

Valida che un dato numero sia *tra* un minimo e un massimo.

+----------------+--------------------------------------------------------------------+
| Si applica a   | :ref:`proprietà o metodo<validation-property-target>`              |
+----------------+--------------------------------------------------------------------+
| Opzioni        | - `min`_                                                           |
|                | - `max`_                                                           |
|                | - `minMessage`_                                                    |
|                | - `maxMessage`_                                                    |
|                | - `invalidMessage`_                                                |
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
                height:
                    - Size:
                        min: 120
                        max: 180
                        minMessage: Devi essere alto almeno 120cm per entrare
                        maxMessage: Non puoi essere più alto di 180cm per entrare

    .. code-block:: php-annotations

        // src/Acme/EventBundle/Entity/Participant.php
        use Symfony\Component\Validator\Constraints as Assert;

        class Participant
        {
            /**
             * @Assert\Size(
             *      min = "120",
             *      max = "180",
             *      minMessage = "Devi essere alto almeno 120cm per entrare",
             *      maxMessage = "Non puoi essere più alto di 180cm per entrare"
             * )
             */
             protected $height;
        }

Opzioni
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

minMessage
~~~~~~~~~~

**tipo**: ``stringa`` **predefinito**: ``This value should be {{ limit }} or more.``

Il messaggio mostrato se il valore sottostante è inferiore a quello dell'opzione
`min`_.

maxMessage
~~~~~~~~~~

**tipo**: ``stringa`` **predefinito**: ``This value should be {{ limit }} or less.``

Il messaggio mostrato se il valore sottostante è superiore a quello dell'opzione
`max`_.

invalidMessage
~~~~~~~~~~~~~~

**tipo**: ``stringa`` **predefinito**: ``This value should be a valid number.``

Il messaggio mostrato se il valore sottostante non è un numero (in base
alla funzione PHP `is_numeric`_).

.. _`is_numeric`: http://www.php.net/manual/en/function.is-numeric.php
