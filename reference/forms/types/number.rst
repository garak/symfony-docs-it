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
| Opzioni       | - `empty_data`_                                                      |
| ereditate     | - `required`_                                                        |
|               | - `label`_                                                           |
|               | - `label_attr`_                                                      |
|               | - `data`_                                                            |
|               | - `read_only`_                                                       |
|               | - `disabled`_                                                        |
|               | - `error_bubbling`_                                                  |
|               | - `error_mapping`_                                                   |
|               | - `invalid_message`_                                                 |
|               | - `invalid_message_parameters`_                                      |
|               | - `mapped`_                                                          |
+---------------+----------------------------------------------------------------------+
| Tipo genitore | :doc:`form </reference/forms/types/form>`                            |
+---------------+----------------------------------------------------------------------+
| Classe        | :class:`Symfony\\Component\\Form\\Extension\\Core\\Type\\NumberType` |
+---------------+----------------------------------------------------------------------+

Opzioni del campo
-----------------

.. include:: /reference/forms/types/options/precision.rst.inc

rounding_mode
~~~~~~~~~~~~~

**tipo**: ``intero`` **predefinito**: ``NumberToLocalizedStringTransformer::ROUND_HALFUP``

Se un numero inviato ha bisogno di essere arrotondato (in base all'opzione ``precision``),
si dispone di varie opzioni configurabili per tale arrotondamento. Ogni opzione è una
costante di :class:`Symfony\\Component\\Form\\Extension\\Core\\DataTransformer\\NumberToLocalizedStringTransformer`:

*   ``NumberToLocalizedStringTransformer::ROUND_DOWN`` Arrotondamento verso lo zero.

*   ``NumberToLocalizedStringTransformer::ROUND_FLOOR`` Arrotondamento verso
    meno infinito.

*   ``NumberToLocalizedStringTransformer::ROUND_UP`` Arrotondamento verso
    l'alto.

*   ``NumberToLocalizedStringTransformer::ROUND_CEILING`` Arrotondamento verso
    infinito.

*   ``NumberToLocalizedStringTransformer::ROUND_HALF_DOWN`` Arrotondamento verso
    il numero più vicino. Se entrambi i numeri più vicini sono equidistanti, arrotonda verso il basso.

*   ``NumberToLocalizedStringTransformer::ROUND_HALF_EVEN`` Arrotondamento verso
    il numero più vicino. Se entrambi i numeri più vicini sono equidistanti, arrotonda verso il numero pari più vicino.

*   ``NumberToLocalizedStringTransformer::ROUND_HALF_UP`` Arrotondamento verso
    il numero più vicino. Se entrambi i numeri più vicini sono equidistanti, arrotonda verso l'alto.

.. include:: /reference/forms/types/options/grouping.rst.inc

Opzioni ereditate
-----------------

Queste opzioni sono ereditate dal tipo :doc:`form </reference/forms/types/form>`:

.. include:: /reference/forms/types/options/empty_data.rst.inc

.. include:: /reference/forms/types/options/required.rst.inc

.. include:: /reference/forms/types/options/label.rst.inc

.. include:: /reference/forms/types/options/label_attr.rst.inc

.. include:: /reference/forms/types/options/data.rst.inc

.. include:: /reference/forms/types/options/read_only.rst.inc

.. include:: /reference/forms/types/options/disabled.rst.inc

.. include:: /reference/forms/types/options/error_bubbling.rst.inc

.. include:: /reference/forms/types/options/error_mapping.rst.inc

.. include:: /reference/forms/types/options/invalid_message.rst.inc

.. include:: /reference/forms/types/options/invalid_message_parameters.rst.inc

.. include:: /reference/forms/types/options/mapped.rst.inc
