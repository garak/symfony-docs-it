.. index::
   single: Form; Campi; form

Tipo di campo form
==================

Il tipo ``form`` predefinisce alcune opzioni, che sono quindi disponibili
su tutti i campi che hanno ``form`` come genitore:

+-----------+--------------------------------------------------------------------+
| Opzioni   | - `data`_                                                          |
|           | - `data_class`_                                                    |
|           | - `empty_data`_                                                    |
|           | - `compound`_                                                      |
|           | - `required`_                                                      |
|           | - `label_attr`_                                                    |
|           | - `constraints`_                                                   |
|           | - `cascade_validation`_                                            |
|           | - `read_only`_                                                     |
|           | - `trim`_                                                          |
|           | - `mapped`_                                                        |
|           | - `property_path`_                                                 |
|           | - `max_length`_                                                    |
|           | - `by_reference`_                                                  |
|           | - `error_bubbling`_                                                |
|           | - `inherit_data`_                                                  |
|           | - `error_mapping`_                                                 |
|           | - `invalid_message`_                                               |
|           | - `invalid_message_parameters`_                                    |
|           | - `extra_fields_message`_                                          |
|           | - `post_max_size_message`_                                         |
|           | - `pattern`_                                                       |
|           | - `action`_                                                        |
|           | - `method`_                                                        |
+-----------+--------------------------------------------------------------------+
| Opzioni   | - `block_name`_                                                    |
| ereditate | - `disabled`_                                                      |
|           | - `label`_                                                         |
|           | - `attr`_                                                          |
|           | - `translation_domain`_                                            |
|           | - `auto_initialize`_                                               |
+-----------+--------------------------------------------------------------------+
| Genitore  | nessuno                                                            |
+-----------+--------------------------------------------------------------------+
| Classe    | :class:`Symfony\\Component\\Form\\Extension\\Core\\Type\\FormType` |
+-----------+--------------------------------------------------------------------+

Opzioni del campo
-----------------

.. include:: /reference/forms/types/options/data.rst.inc

.. include:: /reference/forms/types/options/data_class.rst.inc

.. include:: /reference/forms/types/options/empty_data.rst.inc

.. include:: /reference/forms/types/options/compound.rst.inc

.. _reference-form-option-required:

.. include:: /reference/forms/types/options/required.rst.inc

.. include:: /reference/forms/types/options/label_attr.rst.inc

.. include:: /reference/forms/types/options/constraints.rst.inc

.. include:: /reference/forms/types/options/cascade_validation.rst.inc

.. include:: /reference/forms/types/options/read_only.rst.inc

.. include:: /reference/forms/types/options/trim.rst.inc

.. include:: /reference/forms/types/options/mapped.rst.inc

.. include:: /reference/forms/types/options/property_path.rst.inc

.. _reference-form-option-max_length:

.. include:: /reference/forms/types/options/max_length.rst.inc

.. include:: /reference/forms/types/options/by_reference.rst.inc

.. include:: /reference/forms/types/options/error_bubbling.rst.inc

.. include:: /reference/forms/types/options/inherit_data.rst.inc

.. include:: /reference/forms/types/options/error_mapping.rst.inc

.. include:: /reference/forms/types/options/invalid_message.rst.inc

.. include:: /reference/forms/types/options/invalid_message_parameters.rst.inc

.. include:: /reference/forms/types/options/extra_fields_message.rst.inc

.. include:: /reference/forms/types/options/post_max_size_message.rst.inc

.. _reference-form-option-pattern:

.. include:: /reference/forms/types/options/pattern.rst.inc

.. include:: /reference/forms/types/options/action.rst.inc

.. include:: /reference/forms/types/options/method.rst.inc

Opzioni ereditate
-----------------

Le seguenti opzioni sono definite nella classe
:class:`Symfony\\Component\\Form\\Extension\\Core\\Type\\BaseType`.
La classe ``BaseType`` è la classe genitore sia del tipo ``form`` sia del
tipo :doc:`button </reference/forms/types/button>`, ma non fa parte
dell'albero dei tipi di form (cioè non può essere usata a sua volta come tipo di form).

.. include:: /reference/forms/types/options/block_name.rst.inc

.. include:: /reference/forms/types/options/disabled.rst.inc

.. include:: /reference/forms/types/options/label.rst.inc

.. include:: /reference/forms/types/options/attr.rst.inc

.. include:: /reference/forms/types/options/translation_domain.rst.inc

.. include:: /reference/forms/types/options/auto_initialize.rst.inc
