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
| Opzioni       | - `disabled`_                                                          |
| ereditate     | - `empty_data`_                                                        |
|               | - `error_bubbling`_                                                    |
|               | - `error_mapping`_                                                     |
|               | - `label`_                                                             |
|               | - `label_attr`_                                                        |
|               | - `mapped`_                                                            |
|               | - `max_length`_                                                        |
|               | - `read_only`_                                                         |
|               | - `required`_                                                          |
|               | - `trim`_                                                              |
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

.. include:: /reference/forms/types/options/disabled.rst.inc

.. include:: /reference/forms/types/options/empty_data.rst.inc
    :end-before: DEFAULT_PLACEHOLDER

Il valore predefinito è ``''`` (stringa vuota).

.. include:: /reference/forms/types/options/empty_data.rst.inc
    :start-after: DEFAULT_PLACEHOLDER

.. include:: /reference/forms/types/options/error_bubbling.rst.inc

.. include:: /reference/forms/types/options/error_mapping.rst.inc

.. include:: /reference/forms/types/options/label.rst.inc

.. include:: /reference/forms/types/options/label_attr.rst.inc

.. include:: /reference/forms/types/options/mapped.rst.inc

.. include:: /reference/forms/types/options/max_length.rst.inc

.. include:: /reference/forms/types/options/read_only.rst.inc

.. include:: /reference/forms/types/options/required.rst.inc

trim
~~~~

**tipo**: ``booleano`` **predefinito**: ``false``

Se ``true``, gli spazi vuoti della stringa inviata saranno eliminati tramite
la funzione :phpfunction:`trim`. Questo garantisce che, se un valore viene inserito con
spazi vuoti superflui, questi vengano rimossi prima che il valore sia
inserito nell'oggetto sottostante.
