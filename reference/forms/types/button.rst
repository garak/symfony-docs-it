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
| ereditate            | - `disabled`_                                                        |
|                      | - `label`_                                                           |
|                      | - `translation_domain`_                                              |
+----------------------+----------------------------------------------------------------------+
| Tipo genitore        | nessuno                                                              |
+----------------------+----------------------------------------------------------------------+
| Classe               | :class:`Symfony\\Component\\Form\\Extension\\Core\\Type\\ButtonType` |
+----------------------+----------------------------------------------------------------------+

Opzioni ereditate
-----------------

Le seguenti opzioni sono definite nella classe
:class:`Symfony\\Component\\Form\\Extension\\Core\\Type\\BaseType`.
La classe ``BaseType`` è il genitore sia del tipo ``button`` sia del
tipo :doc:`form </reference/forms/types/form>`, ma non fa parte
dell'albero dei tipi di form (cioè non può essere usato come tipo form a sé stante).

.. include:: /reference/forms/types/options/button_attr.rst.inc

.. include:: /reference/forms/types/options/button_disabled.rst.inc

.. include:: /reference/forms/types/options/button_label.rst.inc

.. include:: /reference/forms/types/options/button_translation_domain.rst.inc
