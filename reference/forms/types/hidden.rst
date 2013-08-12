.. index::
   single: Form; Campi; hidden

Tipo di campo hidden
====================

Il tipo hidden rappresenta un campo input nascosto.

+---------------+----------------------------------------------------------------------+
| Reso come     | campo ``input`` ``hidden``                                           |
+---------------+----------------------------------------------------------------------+
| Opzioni       | - `required`_                                                        |
| ridefinite    | - `error_bubbling`_                                                  |
+---------------+----------------------------------------------------------------------+
| Opzioni       | - `data`_                                                            |
| ereditate     | - `property_path`_                                                   |
|               | - `mapped`_                                                          |
|               | - `error_mapping`_                                                   |
+---------------+----------------------------------------------------------------------+
| Tipo genitore | :doc:`field</reference/forms/types/form>`                            |
+---------------+----------------------------------------------------------------------+
| Classe        | :class:`Symfony\\Component\\Form\\Extension\\Core\\Type\\HiddenType` |
+---------------+----------------------------------------------------------------------+

Opzioni ridefinite
------------------

required
~~~~~~~~

**predefinito**: ``false``

I campi nascosti non possono avere un attributo ``required``.

error_bubbling
~~~~~~~~~~~~~~

**predefinito**: ``true``

Passa gli errori al form radice, altrimenti non sarebbero visibili.

Opzioni ereditate
-----------------

Queste opzioni sono ereditate dal tipo :doc:`form</reference/forms/types/form>`:

.. include:: /reference/forms/types/options/data.rst.inc

.. include:: /reference/forms/types/options/property_path.rst.inc

.. include:: /reference/forms/types/options/mapped.rst.inc

.. include:: /reference/forms/types/options/error_mapping.rst.inc
