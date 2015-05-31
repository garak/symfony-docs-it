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
| Opzioni       | - `grouping`_                                                        |
|               | - `precision`_                                                       |
|               | - `rounding_mode`_                                                   |
+---------------+----------------------------------------------------------------------+
| Opzioni       | - `data`_                                                            |
| ereditate     | - `disabled`_                                                        |
|               | - `empty_data`_                                                      |
|               | - `error_bubbling`_                                                  |
|               | - `error_mapping`_                                                   |
|               | - `invalid_message`_                                                 |
|               | - `invalid_message_parameters`_                                      |
|               | - `label`_                                                           |
|               | - `label_attr`_                                                      |
|               | - `mapped`_                                                          |
|               | - `read_only`_                                                       |
|               | - `required`_                                                        |
+---------------+----------------------------------------------------------------------+
| Tipo genitore | :doc:`form </reference/forms/types/form>`                            |
+---------------+----------------------------------------------------------------------+
| Classe        | :class:`Symfony\\Component\\Form\\Extension\\Core\\Type\\NumberType` |
+---------------+----------------------------------------------------------------------+

Opzioni del campo
-----------------

.. include:: /reference/forms/types/options/grouping.rst.inc

.. include:: /reference/forms/types/options/precision.rst.inc

rounding_mode
~~~~~~~~~~~~~

**tipo**: ``intero`` **predefinito**: ``IntegerToLocalizedStringTransformer::ROUND_HALFUP``

Se un numero inviato ha bisogno di essere arrotondato (in base all'opzione ``precision``),
si dispone di varie opzioni configurabili per tale arrotondamento. Ogni opzione è una
costante di
:class:`Symfony\\Component\\Form\\Extension\\Core\\DataTransformer\\IntegerToLocalizedStringTransformer`:

* ``IntegerToLocalizedStringTransformer::ROUND_DOWN`` Arrotondamento verso lo
  zero.

* ``IntegerToLocalizedStringTransformer::ROUND_FLOOR`` Arrotondamento verso
  meno infinito.

* ``IntegerToLocalizedStringTransformer::ROUND_UP`` Arrotondamento verso
  l'alto.

* ``IntegerToLocalizedStringTransformer::ROUND_CEILING`` Arrotondamento verso
  infinito.

* ``IntegerToLocalizedStringTransformer::ROUND_HALFDOWN`` Arrotondamento verso
  il numero più vicino, a meno che entrambi i numeri più vicini siano equidistanti,
  nel qual caso arrotonda verso il basso.

* ``IntegerToLocalizedStringTransformer::ROUND_HALFEVEN`` Arrotondamento verso
  il numero più vicino, a meno che entrambi i numeri più vicini siano equidistanti,
  nel qual caso arrotonda verso numero pari più vicino.

* ``IntegerToLocalizedStringTransformer::ROUND_HALFUP`` Arrotondamento verso
  il numero più vicino, a meno che entrambi i numeri più vicini siano equidistanti,
  nel qual caso arrotonda verso l'alto.

Opzioni ereditate
-----------------

Queste opzioni sono ereditate dal tipo :doc:`form </reference/forms/types/form>`:


.. include:: /reference/forms/types/options/data.rst.inc

.. include:: /reference/forms/types/options/disabled.rst.inc

.. include:: /reference/forms/types/options/empty_data.rst.inc
    :end-before: DEFAULT_PLACEHOLDER

Il valore predefinito è ``''`` (stringa vuota).

.. include:: /reference/forms/types/options/empty_data.rst.inc
    :start-after: DEFAULT_PLACEHOLDER

.. include:: /reference/forms/types/options/error_bubbling.rst.inc

.. include:: /reference/forms/types/options/error_mapping.rst.inc

.. include:: /reference/forms/types/options/invalid_message.rst.inc

.. include:: /reference/forms/types/options/invalid_message_parameters.rst.inc

.. include:: /reference/forms/types/options/label.rst.inc

.. include:: /reference/forms/types/options/label_attr.rst.inc

.. include:: /reference/forms/types/options/mapped.rst.inc

.. include:: /reference/forms/types/options/read_only.rst.inc

.. include:: /reference/forms/types/options/required.rst.inc
