.. index::
   single: Form; Campi; reset

Tipo di campo reset
===================

.. versionadded:: 2.3
    Il tipo ``reset`` è stato aggiunto in Symfony 2.3

A button that resets all fields to their original values.

+----------------------+---------------------------------------------------------------------+
| Reso come            | tag  ``input`` ``reset``                                            |
+----------------------+---------------------------------------------------------------------+
| Opzioni              | - `attr`_                                                           |
| ereditate            | - `disabled`_                                                       |
|                      | - `label`_                                                          |
|                      | - `translation_domain`_                                             |
+----------------------+---------------------------------------------------------------------+
| Tipo genitore        | :doc:`button</reference/forms/types/button>`                        |
+----------------------+---------------------------------------------------------------------+
| Classe               | :class:`Symfony\\Component\\Form\\Extension\\Core\\Type\\ResetType` |
+----------------------+---------------------------------------------------------------------+

Opzioni ereditate
-----------------

.. include:: /reference/forms/types/options/button_attr.rst.inc

.. include:: /reference/forms/types/options/button_disabled.rst.inc

.. include:: /reference/forms/types/options/button_label.rst.inc

.. include:: /reference/forms/types/options/button_translation_domain.rst.inc
