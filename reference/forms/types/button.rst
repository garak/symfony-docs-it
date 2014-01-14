.. index::
   single: Form; Campi; button

Tipo di campo button
====================

.. versionadded:: 2.3
    Il tipo ``button`` è stato aggiunto in Symfony 2.3

Un semplice bottone, non interattivo.

+----------------------+----------------------------------------------------------------------+
| Reso come            | tag ``button``                                                       |
+----------------------+----------------------------------------------------------------------+
| Opzioni              | - `attr`_                                                            |
|                      | - `disabled`_                                                        |
|                      | - `label`_                                                           |
|                      | - `label_attr`_                                                      |
|                      | - `translation_domain`_                                              |
+----------------------+----------------------------------------------------------------------+
| Opzioni ridefinite   | - `auto_initialize`                                                  |
+----------------------+----------------------------------------------------------------------+
| Tipo genitore        | nessuno                                                              |
+----------------------+----------------------------------------------------------------------+
| Classe               | :class:`Symfony\\Component\\Form\\Extension\\Core\\Type\\ButtonType` |
+----------------------+----------------------------------------------------------------------+

Opzioni
-------

.. include:: /reference/forms/types/options/button_attr.rst.inc

.. include:: /reference/forms/types/options/button_disabled.rst.inc

.. include:: /reference/forms/types/options/button_label.rst.inc

.. include:: /reference/forms/types/options/label_attr.rst.inc

.. include:: /reference/forms/types/options/button_translation_domain.rst.inc

Opzioni ridefinite
------------------

.. include:: /reference/forms/types/options/button_auto_initialize.rst.inc
