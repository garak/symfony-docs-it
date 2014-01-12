.. index::
   single: Form; Campi; password

Tipo di campo password
======================

Il campo ``password`` rende una casella di testo per una password.

+---------------+------------------------------------------------------------------------+
| Reso come     | campo ``input`` ``password``                                           |
+---------------+------------------------------------------------------------------------+
| Opzioni       | - `always_empty`_                                                      |
+---------------+------------------------------------------------------------------------+
| Opzioni       | - `max_length`_                                                        |
| ereditate     | - `empty_data`_                                                        |
|               | - `required`_                                                          |
|               | - `label`_                                                             |
|               | - `label_attr`_                                                        |
|               | - `trim`_                                                              |
|               | - `read_only`_                                                         |
|               | - `disabled`_                                                          |
|               | - `error_bubbling`_                                                    |
|               | - `error_mapping`_                                                     |
|               | - `mapped`_                                                            |
+---------------+------------------------------------------------------------------------+
| Tipo genitore | :doc:`text </reference/forms/types/text>`                              |
+---------------+------------------------------------------------------------------------+
| Classe        | :class:`Symfony\\Component\\Form\\Extension\\Core\\Type\\PasswordType` |
+---------------+------------------------------------------------------------------------+

Opzioni del campo
-----------------

always_empty
~~~~~~~~~~~~

**tipo**: ``booleano`` **predefinito**: ``true``

Se impostato a ``true``, il campo sarà reso *sempre* vuoto, anche se il campo
corrispondente ha un valore. Quando impostato a ``false``, il campo password sarà
reso con l'attributo ``value`` impostate all'effettivo valore.

In parole povere, se per qualche ragione si vuole rendere il campo password
*con* la password già inserita nel campo, impostare questa opzione a ``false``
e inviare il form.


Opzioni ereditate
-----------------

Queste opzioni sono ereditate dal tipo :doc:`form </reference/forms/types/form>`:

.. include:: /reference/forms/types/options/max_length.rst.inc

.. include:: /reference/forms/types/options/empty_data.rst.inc

.. include:: /reference/forms/types/options/required.rst.inc

.. include:: /reference/forms/types/options/label.rst.inc

.. include:: /reference/forms/types/options/label_attr.rst.inc

.. include:: /reference/forms/types/options/trim.rst.inc

.. include:: /reference/forms/types/options/read_only.rst.inc

.. include:: /reference/forms/types/options/disabled.rst.inc

.. include:: /reference/forms/types/options/error_bubbling.rst.inc

.. include:: /reference/forms/types/options/error_mapping.rst.inc

.. include:: /reference/forms/types/options/mapped.rst.inc
