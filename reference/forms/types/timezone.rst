.. index::
   single: Form; Campi; timezone

Tipo di campo timezone
======================

Il tipo ``timezone`` è un sotto-insieme di ``ChoiceType``, che consente all'utente
di scegliere da tutti i possibili fusi orari.

Il valore di ogni fuso orario è il nome completo del fuso orario, come ``America/Chicago``
o ``Europe/Istanbul``.

Diversamente dal tipo ``choice``, non occorre specificare l'opzione ``choices`` o
``choice_list``, perché il campo usa automaticamente una lunga lista di
locale. Si *può* specificare una di queste opzioni manualmente, ma allora si
dovrebbe usare direttamente il tipo ``choice``.

+---------------+------------------------------------------------------------------------+
| Reso come     | può essere vari tag (vedere :ref:`forms-reference-choice-tags`)        |
+---------------+------------------------------------------------------------------------+
| Opzioni       | - `choices`_                                                           |
| ridefinite    |                                                                        |
+---------------+------------------------------------------------------------------------+
| Opzioni       | dal tipo :doc:`choice </reference/forms/types/choice>`                 |
| ereditate     |                                                                        |
|               | - `empty_value`_                                                       |
|               | - `expanded`_                                                          |
|               | - `multiple`_                                                          |
|               | - `preferred_choices`_                                                 |
|               |                                                                        |
|               | dal tipo :doc:`form </reference/forms/types/form>`                     |
|               |                                                                        |
|               | - `data`_                                                              |
|               | - `disabled`_                                                          |
|               | - `empty_data`_                                                        |
|               | - `error_bubbling`_                                                    |
|               | - `error_mapping`_                                                     |
|               | - `label`_                                                             |
|               | - `label_attr`_                                                        |
|               | - `mapped`_                                                            |
|               | - `read_only`_                                                         |
|               | - `required`_                                                          |
+---------------+------------------------------------------------------------------------+
| Tipo genitore | :doc:`choice </reference/forms/types/choice>`                          |
+---------------+------------------------------------------------------------------------+
| Classe        | :class:`Symfony\\Component\\Form\\Extension\\Core\\Type\\TimezoneType` |
+---------------+------------------------------------------------------------------------+

Opzioni ridefinite
------------------

choices
~~~~~~~

**predefinito**: :class:`Symfony\\Component\\Form\\Extension\\Core\\ChoiceList\\TimezoneChoiceList`

Tutti i fusi orari restituiti da
:phpmethod:`DateTimeZone::listIdentifiers`, separati per continente.

Opzioni ereditate
-----------------

Queste opzioni sono ereditate dal tipo :doc:`choice </reference/forms/types/choice>`:


.. include:: /reference/forms/types/options/empty_value.rst.inc

.. include:: /reference/forms/types/options/expanded.rst.inc

.. include:: /reference/forms/types/options/multiple.rst.inc

.. include:: /reference/forms/types/options/preferred_choices.rst.inc

Queste opzioni sono ereditate dal tipo :doc:`form </reference/forms/types/form>`:


.. include:: /reference/forms/types/options/data.rst.inc

.. include:: /reference/forms/types/options/disabled.rst.inc

.. include:: /reference/forms/types/options/empty_data.rst.inc
    :end-before: DEFAULT_PLACEHOLDER

Il valore predefinito effettivo di questa opzione dipende da altre opzioni:

* Se ``multiple`` è ``false`` ed ``expanded`` è ``false``, allora ``''``
  (stringa vuota);
* Altrimenti ``array()`` (array vuoto).

.. include:: /reference/forms/types/options/empty_data.rst.inc
    :start-after: DEFAULT_PLACEHOLDER

.. include:: /reference/forms/types/options/error_bubbling.rst.inc

.. include:: /reference/forms/types/options/error_mapping.rst.inc

.. include:: /reference/forms/types/options/label.rst.inc

.. include:: /reference/forms/types/options/label_attr.rst.inc

.. include:: /reference/forms/types/options/mapped.rst.inc

.. include:: /reference/forms/types/options/read_only.rst.inc

.. include:: /reference/forms/types/options/required.rst.inc
