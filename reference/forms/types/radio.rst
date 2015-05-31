.. index::
   single: Form; Campi; radio

Tipo di campo radio
===================

Crea un singolo bottone radio. Se il bottone radio è selezionato, il campo
sarà impostato al valore specificato. I bottoni radio non possono essere deselezionati,
il valore cambia solo se un altro bottone radio con lo stesso nome viene
selezionato.

Il tipo ``radio`` solitamente non è usato direttamente. Più comunemente, è usato
internamente da altri tipo, come :doc:`choice </reference/forms/types/choice>`.
Se si desidera un campo booleano, usare :doc:`checkbox </reference/forms/types/checkbox>`.

+---------------+---------------------------------------------------------------------+
| Reso come     | campo ``input`` ``radio``                                           |
+---------------+---------------------------------------------------------------------+
| Opzioni       | dal tipo :doc:`checkbox </reference/forms/types/checkbox>`:         |
| ereditate     |                                                                     |
|               | - `value`_                                                          |
|               |                                                                     |
|               | dal tipo :doc:`form </reference/forms/types/form>`:                 |
|               |                                                                     |
|               | - `data`_                                                           |
|               | - `disabled`_                                                       |
|               | - `empty_data`_                                                     |
|               | - `error_bubbling`_                                                 |
|               | - `error_mapping`_                                                  |
|               | - `label`_                                                          |
|               | - `label_attr`_                                                     |
|               | - `mapped`_                                                         |
|               | - `read_only`_                                                      |
|               | - `required`_                                                       |
+---------------+---------------------------------------------------------------------+
| Tipo genitore | :doc:`checkbox </reference/forms/types/checkbox>`                   |
+---------------+---------------------------------------------------------------------+
| Classe        | :class:`Symfony\\Component\\Form\\Extension\\Core\\Type\\RadioType` |
+---------------+---------------------------------------------------------------------+

Opzioni ereditate
-----------------

Queste opzioni sono ereditate dal tipo :doc:`checkbox </reference/forms/types/checkbox>`:


.. include:: /reference/forms/types/options/value.rst.inc

Queste opzioni sono ereditate dal tipo :doc:`form </reference/forms/types/form>`:


.. include:: /reference/forms/types/options/data.rst.inc

.. include:: /reference/forms/types/options/disabled.rst.inc

.. include:: /reference/forms/types/options/checkbox_empty_data.rst.inc

.. include:: /reference/forms/types/options/error_bubbling.rst.inc

.. include:: /reference/forms/types/options/error_mapping.rst.inc

.. include:: /reference/forms/types/options/label.rst.inc

.. include:: /reference/forms/types/options/label_attr.rst.inc

.. include:: /reference/forms/types/options/mapped.rst.inc

.. include:: /reference/forms/types/options/read_only.rst.inc

.. include:: /reference/forms/types/options/required.rst.inc

Variabili di form
-----------------

.. include:: /reference/forms/types/variables/check_or_radio_table.rst.inc
