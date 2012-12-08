.. index::
   single: Form; Campi; language

Tipo di campo language
======================

Il tipo ``language`` è un sotto-insieme di ``ChoiceType``, che consente all'utente
di scegliere da un lungo elenco di lingue. Come bonus aggiuntivo, i nomi delle lingue
sono mostrati nella lingua dell'utente.

Il valore per ogni locale è il codice della *lingua* a due lettere ISO639-1
(p.e. ``it``).

.. note::

   Il locale degli utenti è indovinato tramite `Locale::getDefault()`_

Diversamente dal tipo ``choice``, non occorre specificare l'opzione
``choices`` o ``choice_list``, perché il tipo di campo usa automaticamente la lista
delle lingue. Si *può* specificare una di queste opzioni manualmente, ma allora si
dovrebbe usare il tipo ``choice`` direttamente.

+---------------+------------------------------------------------------------------------+
| Reso come     | possono essere diversi tag (vedere :ref:`forms-reference-choice-tags`) |
+---------------+------------------------------------------------------------------------+
| Opzioni       | - `multiple`_                                                          |
| ereditate     | - `expanded`_                                                          |
|               | - `preferred_choices`_                                                 |
|               | - `empty_value`_                                                       |
|               | - `error_bubbling`_                                                    |
|               | - `required`_                                                          |
|               | - `label`_                                                             |
|               | - `read_only`_                                                         |
+---------------+------------------------------------------------------------------------+
| Tipo genitore | :doc:`choice</reference/forms/types/choice>`                           |
+---------------+------------------------------------------------------------------------+
| Classe        | :class:`Symfony\\Component\\Form\\Extension\\Core\\Type\\LanguageType` |
+---------------+------------------------------------------------------------------------+

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

.. _`Locale::getDefault()`: http://php.net/manual/en/locale.getdefault.php