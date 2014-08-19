.. index::
   single: Form; Campi; form

Tipo di campo form
==================

Il tipo ``form`` predefinisce alcune opzioni, che sono quindi disponibili
su tutti i campi che hanno ``form`` come genitore:

+-----------+--------------------------------------------------------------------+
| Opzioni   | - `action`_                                                        |
|           | - `by_reference`_                                                  |
|           | - `cascade_validation`_                                            |
|           | - `compound`_                                                      |
|           | - `constraints`_                                                   |
|           | - `data`_                                                          |
|           | - `data_class`_                                                    |
|           | - `empty_data`_                                                    |
|           | - `error_bubbling`_                                                |
|           | - `error_mapping`_                                                 |
|           | - `extra_fields_message`_                                          |
|           | - `inherit_data`_                                                  |
|           | - `invalid_message`_                                               |
|           | - `invalid_message_parameters`_                                    |
|           | - `label_attr`_                                                    |
|           | - `mapped`_                                                        |
|           | - `max_length`_ (deprecata da 2.5)                                 |
|           | - `method`_                                                        |
|           | - `pattern`_ (deprecata da 2.5)                                    |
|           | - `post_max_size_message`_                                         |
|           | - `property_path`_                                                 |
|           | - `read_only`_                                                     |
|           | - `required`_                                                      |
|           | - `trim`_                                                          |
+-----------+--------------------------------------------------------------------+
| Opzioni   | - `attr`_                                                          |
| ereditate | - `auto_initialize`_                                               |
|           | - `block_name`_                                                    |
|           | - `disabled`_                                                      |
|           | - `label`_                                                         |
|           | - `translation_domain`_                                            |
+-----------+--------------------------------------------------------------------+
| Genitore  | nessuno                                                            |
+-----------+--------------------------------------------------------------------+
| Classe    | :class:`Symfony\\Component\\Form\\Extension\\Core\\Type\\FormType` |
+-----------+--------------------------------------------------------------------+

Opzioni del campo
-----------------

.. include:: /reference/forms/types/options/action.rst.inc

.. include:: /reference/forms/types/options/by_reference.rst.inc

.. include:: /reference/forms/types/options/cascade_validation.rst.inc

.. include:: /reference/forms/types/options/compound.rst.inc

.. include:: /reference/forms/types/options/constraints.rst.inc

.. include:: /reference/forms/types/options/data.rst.inc

.. include:: /reference/forms/types/options/data_class.rst.inc

.. include:: /reference/forms/types/options/empty_data.rst.inc
    :end-before: DEFAULT_PLACEHOLDER

Il valore predefinito effettivo di questa opzione dipende da altre opzioni:

* Se ``data_class`` è impostato e ``required`` è ``true``, allora ``new $data_class()``;
* Se ``data_class`` è impostato e ``required`` è ``false``, allora ``null``;
* Se ``data_class`` non è impostato e ``compound`` è ``true``, allora ``array()``
  (empty array);
* Se ``data_class`` non è impostato e ``compound`` è ``false``, allora ``''`` (stringa vuota).

.. include:: /reference/forms/types/options/empty_data.rst.inc
    :start-after: DEFAULT_PLACEHOLDER

.. include:: /reference/forms/types/options/error_bubbling.rst.inc

.. include:: /reference/forms/types/options/error_mapping.rst.inc

.. include:: /reference/forms/types/options/extra_fields_message.rst.inc

.. include:: /reference/forms/types/options/inherit_data.rst.inc

.. include:: /reference/forms/types/options/invalid_message.rst.inc

.. include:: /reference/forms/types/options/invalid_message_parameters.rst.inc

.. include:: /reference/forms/types/options/label_attr.rst.inc

.. include:: /reference/forms/types/options/mapped.rst.inc

.. _reference-form-option-max_length:

.. include:: /reference/forms/types/options/max_length.rst.inc

.. include:: /reference/forms/types/options/method.rst.inc

.. _reference-form-option-pattern:

.. include:: /reference/forms/types/options/pattern.rst.inc

.. include:: /reference/forms/types/options/post_max_size_message.rst.inc

.. include:: /reference/forms/types/options/property_path.rst.inc

.. include:: /reference/forms/types/options/read_only.rst.inc

.. _reference-form-option-required:

.. include:: /reference/forms/types/options/required.rst.inc

.. include:: /reference/forms/types/options/trim.rst.inc

Opzioni ereditate
-----------------

Le seguenti opzioni sono definite nella classe
:class:`Symfony\\Component\\Form\\Extension\\Core\\Type\\BaseType`.
La classe ``BaseType`` è la classe genitore sia del tipo ``form`` sia del
tipo :doc:`button </reference/forms/types/button>`, ma non fa parte
dell'albero dei tipi di form (cioè non può essere usata a sua volta come tipo di form).

.. include:: /reference/forms/types/options/attr.rst.inc

.. include:: /reference/forms/types/options/auto_initialize.rst.inc

.. include:: /reference/forms/types/options/block_name.rst.inc

.. include:: /reference/forms/types/options/disabled.rst.inc

.. include:: /reference/forms/types/options/label.rst.inc

.. include:: /reference/forms/types/options/translation_domain.rst.inc
