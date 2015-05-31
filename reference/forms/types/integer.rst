.. index::
   single: Form; Campi; integer

Tipo di campo integer
=====================

Rende un campo input per un numero. Di base, è un campo testo che va bene per
gestire dati che siano in forma di interi. Il campo ``number`` appare come
una casella di testo, ma con alcune funzionalità aggiuntive, a patto che il browser
dell'utente supporti HTML5.

Questo campo ha diverse opzioni su come gestire i valori ricevuti che non siano interi.
Per impostazione predefinita, tutti i valori non interi (p.e. 6.78) saranno arrotondati per difetto
(p.e. 6).

+---------------+-----------------------------------------------------------------------+
| Reso come     | campo ``input`` ``number``                                            |
+---------------+-----------------------------------------------------------------------+
| Opzioni       | - `grouping`_                                                         |
|               | - `precision`_                                                        |
|               | - `rounding_mode`_                                                    |
+---------------+-----------------------------------------------------------------------+
| Opzioni       | - `data`_                                                             |
| ereditate     | - `disabled`_                                                         |
|               | - `empty_data`_                                                       |
|               | - `error_bubbling`_                                                   |
|               | - `error_mapping`_                                                    |
|               | - `invalid_message`_                                                  |
|               | - `invalid_message_parameters`_                                       |
|               | - `label`_                                                            |
|               | - `label_attr`_                                                       |
|               | - `mapped`_                                                           |
|               | - `read_only`_                                                        |
|               | - `required`_                                                         |
+---------------+-----------------------------------------------------------------------+
| Tipo genitore | :doc:`form </reference/forms/types/form>`                             |
+---------------+-----------------------------------------------------------------------+
| Classe        | :class:`Symfony\\Component\\Form\\Extension\\Core\\Type\\IntegerType` |
+---------------+-----------------------------------------------------------------------+

Opzioni del campo
-----------------

.. include:: /reference/forms/types/options/grouping.rst.inc

.. include:: /reference/forms/types/options/precision.rst.inc

rounding_mode
~~~~~~~~~~~~~

**tipo**: ``intero`` **predefinito**: ``IntegerToLocalizedStringTransformer::ROUND_DOWN``

Per impostazione predefinita, se l'utente inserisce un numero non intero, sarà arrotondato
per difetto. Ci sono molti altri metodi di arrotondamento e ognuno è una costante di
:class:`Symfony\\Component\\Form\\Extension\\Core\\DataTransformer\\IntegerToLocalizedStringTransformer`:

*   ``IntegerToLocalizedStringTransformer::ROUND_DOWN`` Arrotondamento verso
    lo zero.

*   ``IntegerToLocalizedStringTransformer::ROUND_FLOOR`` Arrotondamento verso
    meno infinito.

*   ``IntegerToLocalizedStringTransformer::ROUND_UP`` Arrotondamento verso
    l'alto.

*   ``IntegerToLocalizedStringTransformer::ROUND_CEILING`` Arrotondamento
    verso infinito.

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
