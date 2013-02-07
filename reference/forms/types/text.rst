.. index::
   single: Form; Campi; text

Tipo di campo text
==================

Il campo text rappresenta il campo testuale di base.

+---------------+--------------------------------------------------------------------+
| Reso come     | campo ``input`` ``text``                                           |
+---------------+--------------------------------------------------------------------+
| Opzioni       | - `max_length`_                                                    |
| ereditate     | - `required`_                                                      |
|               | - `label`_                                                         |
|               | - `trim`_                                                          |
|               | - `read_only`_                                                     |
|               | - `disabled`_                                                      |
|               | - `error_bubbling`_                                                |
+---------------+--------------------------------------------------------------------+
| Tipo genitore | :doc:`field</reference/forms/types/field>`                         |
+---------------+--------------------------------------------------------------------+
| Classe        | :class:`Symfony\\Component\\Form\\Extension\\Core\\Type\\TextType` |
+---------------+--------------------------------------------------------------------+


Opzioni ereditate
-----------------

Queste opzioni sono ereditate dal tipo :doc:`field</reference/forms/types/field>`:

.. include:: /reference/forms/types/options/max_length.rst.inc

.. include:: /reference/forms/types/options/required.rst.inc

.. include:: /reference/forms/types/options/label.rst.inc

.. include:: /reference/forms/types/options/trim.rst.inc

.. include:: /reference/forms/types/options/read_only.rst.inc

.. include:: /reference/forms/types/options/disabled.rst.inc

.. include:: /reference/forms/types/options/error_bubbling.rst.inc
