.. index::
   single: Form; Campi; timezone

Tipo di campo timezone
======================

Il tipo ``timezone`` è un sotto-insieme di ``ChoiceType``, che consente all'utente
di scegliere da tutti i possibili fusi orari.

Il valore di ogni fuso orario è il nome completo del fuso orario, come ``America/Chicago``
o ``Europe/Istanbul``.

Diversamente dal tipo ``choice``, non occorre specificare l'opzione ``choices`` o
``choice_list``, percheé il campo usa automaticamente una lunga lista di
locale. Si *può* specificare una di queste opzioni manualmente, ma allora si
dovrebbe usare direttamente il tipo ``choice``.

+---------------+------------------------------------------------------------------------+
| Reso come     | può essere vari tag (vedere :ref:`forms-reference-choice-tags`)        |
+---------------+------------------------------------------------------------------------+
| Opzioni       | - `choice_list`_                                                       |
| ridefinite    |                                                                        |
+---------------+------------------------------------------------------------------------+
| Opzioni       | - `multiple`_                                                          |
| ereditate     | - `expanded`_                                                          |
|               | - `preferred_choices`_                                                 |
|               | - `empty_value`_                                                       |
|               | - `required`_                                                          |
|               | - `label`_                                                             |
|               | - `read_only`_                                                         |
|               | - `disabled`_                                                          |
|               | - `error_bubbling`_                                                    |
|               | - `mapped`_                                                            |
+---------------+------------------------------------------------------------------------+
| Tipo genitore | :doc:`choice</reference/forms/types/choice>`                           |
+---------------+------------------------------------------------------------------------+
| Classe        | :class:`Symfony\\Component\\Form\\Extension\\Core\\Type\\TimezoneType` |
+---------------+------------------------------------------------------------------------+

Opzioni ridefinite
------------------

choice_list
~~~~~~~~~~~

**predefinito**: :class:`Symfony\\Component\\Form\\Extension\\Core\\ChoiceList\\TimezoneChoiceList`

Tutti i fusi orari restituiti da
:phpmethod:`DateTimeZone::listIdentifiers`, separati per continente.

Opzioni ereditate
-----------------

Queste opzioni sono ereditate dal tipo :doc:`choice</reference/forms/types/choice>`:

.. include:: /reference/forms/types/options/multiple.rst.inc

.. include:: /reference/forms/types/options/expanded.rst.inc

.. include:: /reference/forms/types/options/preferred_choices.rst.inc

.. include:: /reference/forms/types/options/empty_value.rst.inc

Queste opzioni sono ereditate dal tipo :doc:`form</reference/forms/types/form>`:

.. include:: /reference/forms/types/options/required.rst.inc

.. include:: /reference/forms/types/options/label.rst.inc

.. include:: /reference/forms/types/options/read_only.rst.inc

.. include:: /reference/forms/types/options/disabled.rst.inc

.. include:: /reference/forms/types/options/error_bubbling.rst.inc

.. include:: /reference/forms/types/options/mapped.rst.inc
