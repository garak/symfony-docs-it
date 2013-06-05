.. index::
   single: Form; Campi; country

Tipo di campo country
=====================

Il tipo ``country`` è un sotto-insieme di ``ChoiceType``, che mostra i paesi del mondo.
Come bonus aggiuntivo, i nomi dei paesi sono mostrati nella lingua
dell'utente.

Il valore di ogni paese è un codice da due lettere.

.. note::

   Il locale dell'utente è indovinato tramite :phpmethod:`Locale::getDefault`

Diversamente dal tipo ``choice``, non occorre specificare l'opzione ``choices`` o
``choice_list``, poiché il campo usaa automaticamente tutti i paesi del mondo.
Si *può* specificare una di queste opzioni manulmente, ma allora si dovrebbe
usare il tipo ``choice`` direttamente.

+---------------+-----------------------------------------------------------------------+
| Reso come     | possono essere varitag (vedere :ref:`forms-reference-choice-tags`)    |
+---------------+-----------------------------------------------------------------------+
| Opzioni       | - `choices`_                                                          |
| ridefinite    |                                                                       |
+---------------+-----------------------------------------------------------------------+
| Opzioni       | - `multiple`_                                                         |
| ereditate     | - `expanded`_                                                         |
|               | - `preferred_choices`_                                                |
|               | - `empty_value`_                                                      |
|               | - `error_bubbling`_                                                   |
|               | - `required`_                                                         |
|               | - `label`_                                                            |
|               | - `read_only`_                                                        |
|               | - `disabled`_                                                         |
|               | - `mapped`_                                                           |
+---------------+-----------------------------------------------------------------------+
| Tipo genitore | :doc:`choice</reference/forms/types/choice>`                          |
+---------------+-----------------------------------------------------------------------+
| Classe        | :class:`Symfony\\Component\\Form\\Extension\\Core\\Type\\CountryType` |
+---------------+-----------------------------------------------------------------------+

Opzioni ridefinite
------------------

choices
~~~~~~~

**predefinito**: :method:`Symfony\\Component\\Locale\\Locale::getDisplayCountries`

Questa opzione ha come valore predefinito tutti i locale restituiti da
:method:`Symfony\\Component\\Locale\\Locale::getDisplayCountries`.
Usa il locale predefinito per determinare la lingua.

Opzioni ereditate
-----------------

Queste opzioni sono ereditate dal tipo :doc:`choice</reference/forms/types/choice>`:

.. include:: /reference/forms/types/options/multiple.rst.inc

.. include:: /reference/forms/types/options/expanded.rst.inc

.. include:: /reference/forms/types/options/preferred_choices.rst.inc

.. include:: /reference/forms/types/options/empty_value.rst.inc

.. include:: /reference/forms/types/options/error_bubbling.rst.inc

Queste opzioni sono ereditate dal tipo :doc:`field</reference/forms/types/field>`:

.. include:: /reference/forms/types/options/required.rst.inc

.. include:: /reference/forms/types/options/label.rst.inc

.. include:: /reference/forms/types/options/read_only.rst.inc

.. include:: /reference/forms/types/options/disabled.rst.inc

.. include:: /reference/forms/types/options/mapped.rst.inc
