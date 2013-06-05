.. index::
   single: Form; Campi; number

Tipo di campo number
====================

Rende un campo testuale specializzato nella gestione di numeri. Questo tipo offre
diverse opzioni per la precisione, l'arrotondamento, il raggruppamento da usare
per i numeri.

+---------------+----------------------------------------------------------------------+
| Reso come     | campo ``input`` ``text``                                             |
+---------------+----------------------------------------------------------------------+
| Opzioni       | - `rounding_mode`_                                                   |
|               | - `precision`_                                                       |
|               | - `grouping`_                                                        |
+---------------+----------------------------------------------------------------------+
| Opzioni       | - `required`_                                                        |
| ereditate     | - `label`_                                                           |
|               | - `read_only`_                                                       |
|               | - `disabled`_                                                        |
|               | - `error_bubbling`_                                                  |
|               | - `invalid_message`_                                                 |
|               | - `invalid_message_parameters`_                                      |
|               | - `mapped`_                                                          |
+---------------+----------------------------------------------------------------------+
| Tipo genitore | :doc:`field</reference/forms/types/field>`                           |
+---------------+----------------------------------------------------------------------+
| Classe        | :class:`Symfony\\Component\\Form\\Extension\\Core\\Type\\NumberType` |
+---------------+----------------------------------------------------------------------+

Opzioni del campo
-----------------

precision
~~~~~~~~~

**tipo**: ``intero`` **predefinito**: dipende dal locale (solitamente ``3``)

Specifica quanti decimali saranno consentiti prima che il campo arrotondi il
valore inviato (tramite ``rounding_mode``). Per esempio, se ``precision`` è
impostato a ``2``, un valore inviato di ``20.123`` sarà arrotondato, per esempio,
a ``20.12`` (a seconda dell'opzione ``rounding_mode``).

rounding_mode
~~~~~~~~~~~~~

**tipo**: ``intero`` **predefinito**: ``IntegerToLocalizedStringTransformer::ROUND_DOWN``

Se un numero inviato ha bisogno di essere arrotondato (in base all'opzione ``precision``),
si dispone di varie opzioni configurabili per tale arrotondamento. Ogni opzione è una
costante di :class:`Symfony\\Component\\Form\\Extension\\Core\\DataTransformer\\IntegerToLocalizedStringTransformer`:
    
*   ``IntegerToLocalizedStringTransformer::ROUND_DOWN`` Arrotondamento verso lo
    zero.

*   ``IntegerToLocalizedStringTransformer::ROUND_FLOOR`` Arrotondamento verso
    meno infinito.

*   ``IntegerToLocalizedStringTransformer::ROUND_UP`` Arrotondamento verso
    l'alto.

*   ``IntegerToLocalizedStringTransformer::ROUND_CEILING`` Arrotondamento verso
    infinito.

*   ``IntegerToLocalizedStringTransformer::ROUND_HALFDOWN`` Arrotondamento verso
    il numero più vicino, a meno che entrambi i numeri più vicini siano equidistanti,
    nel qual caso arrotonda verso il basso.

*   ``IntegerToLocalizedStringTransformer::ROUND_HALFEVEN`` Arrotondamento verso
    il numero più vicino, a meno che entrambi i numeri più vicini siano equidistanti,
    nel qual caso arrotonda verso numero pari più vicino.

*   ``IntegerToLocalizedStringTransformer::ROUND_HALFUP`` Arrotondamento verso
    il numero più vicino, a meno che entrambi i numeri più vicini siano equidistanti,
    nel qual caso arrotonda verso l'alto.

.. include:: /reference/forms/types/options/grouping.rst.inc

Opzioni ereditate
-----------------

Queste opzioni sono ereditate dal tipo :doc:`form</reference/forms/types/form>`:

.. include:: /reference/forms/types/options/required.rst.inc

.. include:: /reference/forms/types/options/label.rst.inc

.. include:: /reference/forms/types/options/read_only.rst.inc

.. include:: /reference/forms/types/options/disabled.rst.inc

.. include:: /reference/forms/types/options/error_bubbling.rst.inc

.. include:: /reference/forms/types/options/invalid_message.rst.inc

.. include:: /reference/forms/types/options/invalid_message_parameters.rst.inc

.. include:: /reference/forms/types/options/mapped.rst.inc
