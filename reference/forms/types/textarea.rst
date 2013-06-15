.. index::
   single: Form; Campi; textarea

Tipo di campo textarea
======================

Rende un elemento HTML ``textarea``.

+---------------+------------------------------------------------------------------------+
| Reso come     | tag ``textarea``                                                       |
+---------------+------------------------------------------------------------------------+
| Opzioni       | - `max_length`_                                                        |
| ereditate     | - `required`_                                                          |
|               | - `label`_                                                             |
|               | - `trim`_                                                              |
|               | - `read_only`_                                                         |
|               | - `disabled`_                                                          |
|               | - `error_bubbling`_                                                    |
|               | - `mapped`_                                                            |
+---------------+------------------------------------------------------------------------+
| Tipo genitore | :doc:`field</reference/forms/types/field>`                             |
+---------------+------------------------------------------------------------------------+
| Classe        | :class:`Symfony\\Component\\Form\\Extension\\Core\\Type\\TextareaType` |
+---------------+------------------------------------------------------------------------+

Opzioni ereditate
-----------------

Queste opzioni sono ereditate dal tipo :doc:`form</reference/forms/types/form>`:

.. include:: /reference/forms/types/options/max_length.rst.inc

.. include:: /reference/forms/types/options/required.rst.inc

.. include:: /reference/forms/types/options/label.rst.inc

.. include:: /reference/forms/types/options/trim.rst.inc

.. include:: /reference/forms/types/options/read_only.rst.inc

.. include:: /reference/forms/types/options/disabled.rst.inc

.. include:: /reference/forms/types/options/error_bubbling.rst.inc

.. include:: /reference/forms/types/options/mapped.rst.inc
