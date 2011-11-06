.. index::
   single: Form; Campi; email

Tipo di campo email
===================

Il campo ``email`` è un campo di testo resto usando il tag
``<input type="email" />`` di HTML5.

+---------------+---------------------------------------------------------------------+
| Reso come     | campo ``input`` ``email`` (casella di testo)                        |
+---------------+---------------------------------------------------------------------+
| Opzioni       | - `max_length`_                                                     |
| ereditate     | - `required`_                                                       |
|               | - `label`_                                                          |
|               | - `trim`_                                                           |
|               | - `read_only`_                                                      |
|               | - `error_bubbling`_                                                 |
+---------------+---------------------------------------------------------------------+
| Tipo genitore | :doc:`field</reference/forms/types/field>`                          |
+---------------+---------------------------------------------------------------------+
| Classe        | :class:`Symfony\\Component\\Form\\Extension\\Core\\Type\\EmailType` |
+---------------+---------------------------------------------------------------------+

Opzioni ereditate
-----------------

Queste opzioni sono ereditate dal tipo :doc:`field</reference/forms/types/field>`:

.. include:: /reference/forms/types/options/max_length.rst.inc

.. include:: /reference/forms/types/options/required.rst.inc

.. include:: /reference/forms/types/options/label.rst.inc

.. include:: /reference/forms/types/options/trim.rst.inc

.. include:: /reference/forms/types/options/read_only.rst.inc

.. include:: /reference/forms/types/options/error_bubbling.rst.inc
