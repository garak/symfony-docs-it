.. index::
   single: Form; Campi; language

Tipo di campo language
======================

Il tipo ``language`` è un sotto-insieme di ``ChoiceType``, che consente all'utente
di scegliere da un lungo elenco di lingue. Come bonus aggiuntivo, i nomi delle lingue
sono mostrati nella lingua dell'utente.

Il valore per ogni lingua è *l'identificativo Unicode della lingua*
(p.e. ``it`` o ``zh-Hant``).

.. note::

   Il locale degli utenti è indovinato tramite :phpmethod:`Locale::getDefault`

Diversamente dal tipo ``choice``, non occorre specificare l'opzione
``choices`` o ``choice_list``, perché il tipo di campo usa automaticamente la lista
delle lingue. Si *può* specificare una di queste opzioni manualmente, ma allora si
dovrebbe usare il tipo ``choice`` direttamente.

+---------------+------------------------------------------------------------------------+
| Reso come     | possono essere diversi tag (vedere :ref:`forms-reference-choice-tags`) |
+---------------+------------------------------------------------------------------------+
| Opzioni       | - `choices`_                                                           |
| ridefinite    |                                                                        |
+---------------+------------------------------------------------------------------------+
| Opzioni       | - `multiple`_                                                          |
| ereditate     | - `expanded`_                                                          |
|               | - `preferred_choices`_                                                 |
|               | - `empty_value`_                                                       |
|               | - `error_bubbling`_                                                    |
|               | - `error_mapping`_                                                     |
|               | - `required`_                                                          |
|               | - `label`_                                                             |
|               | - `read_only`_                                                         |
|               | - `disabled`_                                                          |
|               | - `mapped`_                                                            |
+---------------+------------------------------------------------------------------------+
| Tipo genitore | :doc:`choice</reference/forms/types/choice>`                           |
+---------------+------------------------------------------------------------------------+
| Classe        | :class:`Symfony\\Component\\Form\\Extension\\Core\\Type\\LanguageType` |
+---------------+------------------------------------------------------------------------+

Opzioni ridefinite
------------------

choices
~~~~~~~

**predefinito**: :method:`Symfony\\Component\\Locale\\Locale::getDisplayLanguages`

Questa opzione ha come valore predefinito tutte le lingue. Usa il
locale predefinito per specificare la lingua.

Opzioni ereditate
-----------------

Queste opzioni sono ereditate dal tipo :doc:`choice</reference/forms/types/choice>`:

.. include:: /reference/forms/types/options/multiple.rst.inc

.. include:: /reference/forms/types/options/expanded.rst.inc

.. include:: /reference/forms/types/options/preferred_choices.rst.inc

.. include:: /reference/forms/types/options/empty_value.rst.inc

.. include:: /reference/forms/types/options/error_bubbling.rst.inc

.. include:: /reference/forms/types/options/error_mapping.rst.inc

Queste opzioni sono ereditate dal tipo :doc:`form</reference/forms/types/form>`:

.. include:: /reference/forms/types/options/required.rst.inc

.. include:: /reference/forms/types/options/label.rst.inc

.. include:: /reference/forms/types/options/read_only.rst.inc

.. include:: /reference/forms/types/options/disabled.rst.inc

.. include:: /reference/forms/types/options/mapped.rst.inc
