.. index::
   single: Form; Campi; textarea

Tipo di campo textarea
======================

Rende un elemento HTML ``textarea``.

+---------------+------------------------------------------------------------------------+
| Reso come     | tag ``textarea``                                                       |
+---------------+------------------------------------------------------------------------+
| Opzioni       | - `data`_                                                              |
| ereditate     | - `disabled`_                                                          |
|               | - `empty_data`_                                                        |
|               | - `error_bubbling`_                                                    |
|               | - `error_mapping`_                                                     |
|               | - `label`_                                                             |
|               | - `label_attr`_                                                        |
|               | - `mapped`_                                                            |
|               | - `max_length`_                                                        |
|               | - `read_only`_                                                         |
|               | - `required`_                                                          |
|               | - `trim`_                                                              |
+---------------+------------------------------------------------------------------------+
| Tipo genitore | :doc:`text </reference/forms/types/text>`                              |
+---------------+------------------------------------------------------------------------+
| Classe        | :class:`Symfony\\Component\\Form\\Extension\\Core\\Type\\TextareaType` |
+---------------+------------------------------------------------------------------------+

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

.. include:: /reference/forms/types/options/label.rst.inc

.. include:: /reference/forms/types/options/label_attr.rst.inc

.. include:: /reference/forms/types/options/mapped.rst.inc

.. include:: /reference/forms/types/options/max_length.rst.inc

.. include:: /reference/forms/types/options/read_only.rst.inc

.. include:: /reference/forms/types/options/required.rst.inc

.. include:: /reference/forms/types/options/trim.rst.inc
