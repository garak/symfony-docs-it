.. index::
   single: Form; Campi; locale

Tipo di campo locale
====================

Il tipo ``locale`` è un sotto-insieme di ``ChoiceType``, che consente all'utente di
scegliere tra una lunga lista di locale (lingua + paese). Come bonus aggiuntivo,
i nomi dei locale sono mostrati nella lingua dell'utente.

Il valore di ogni locale è un codice di *lingua* a due lettere ISO639-1
(p.e. ``it``) oppure il codice della lingua seguito da un trattino basso (``_``) e
dal codice del *paese* ISO3166 (p.e. ``it_IT`` per Italiano/Italia).

.. note::

   Il locale dell'utente è indovinato tramite :phpmethod:`Locale::getDefault`

Diversamente dal tipo ``choice``, non occorre specificare l'opzione
``choices`` o ``choice_list``, perché il tipo di campo usa automaticamente la lista
dei locale. Si *può* specificare una di queste opzioni manualmente, ma allora si
dovrebbe usare il tipo ``choice`` direttamente.

+---------------+------------------------------------------------------------------------+
| Reso come     | può essere diversi tag (vedere :ref:`forms-reference-choice-tags`)     |
+---------------+------------------------------------------------------------------------+
| Opzioni       | - `choices`_                                                           |
| ridefinite    |                                                                        |
+---------------+------------------------------------------------------------------------+
| Opzioni       | - `multiple`_                                                          |
| ereditate     | - `expanded`_                                                          |
|               | - `preferred_choices`_                                                 |
|               | - `empty_value`_                                                       |
|               | - `error_bubbling`_                                                    |
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

**predefinito**: :method:`Symfony\\Component\\Locale\\Locale::getDisplayLocales`

Questa opzione ha come valore predefinito tutti i locale restituiti da
:method:`Symfony\\Component\\Locale\\Locale::getDisplayLocales`. Usa il
locale predefinito per specificare la lingua.


Opzioni ereditate
-----------------

Queste opzioni sono ereditate dal tipo :doc:`choice</reference/forms/types/choice>`:

.. include:: /reference/forms/types/options/multiple.rst.inc

.. include:: /reference/forms/types/options/expanded.rst.inc

.. include:: /reference/forms/types/options/preferred_choices.rst.inc

.. include:: /reference/forms/types/options/empty_value.rst.inc

.. include:: /reference/forms/types/options/error_bubbling.rst.inc

Queste opzioni sono ereditate dal tipo :doc:`form</reference/forms/types/form>`:

.. include:: /reference/forms/types/options/required.rst.inc

.. include:: /reference/forms/types/options/label.rst.inc

.. include:: /reference/forms/types/options/read_only.rst.inc

.. include:: /reference/forms/types/options/disabled.rst.inc

.. include:: /reference/forms/types/options/mapped.rst.inc
