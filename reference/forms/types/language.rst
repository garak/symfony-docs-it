.. index::
   single: Form; Campi; language

Tipo di campo language
======================

Il tipo ``language`` è un sotto-insieme di ``ChoiceType``, che consente all'utente
di scegliere da un lungo elenco di lingue. Come bonus aggiuntivo, i nomi delle lingue
sono mostrati nella lingua dell'utente.

Il valore per ogni lingua è *l'identificativo Unicode della lingua*, usato
nei `componenti internazionali per Unicode`_ (p.e. ``it`` o ``zh-Hant``).

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
| Opzioni       | dal tipo :doc:`choice </reference/forms/types/choice>`                 |
| ereditate     |                                                                        |
|               | - `empty_value`_                                                       |
|               | - `error_bubbling`_                                                    |
|               | - `error_mapping`_                                                     |
|               | - `expanded`_                                                          |
|               | - `multiple`_                                                          |
|               | - `preferred_choices`_                                                 |
|               |                                                                        |
|               | dal tipo :doc:`form </reference/forms/types/form>`                     |
|               |                                                                        |
|               | - `data`_                                                              |
|               | - `disabled`_                                                          |
|               | - `empty_data`_                                                        |
|               | - `label`_                                                             |
|               | - `label_attr`_                                                        |
|               | - `mapped`_                                                            |
|               | - `read_only`_                                                         |
|               | - `required`_                                                          |
+---------------+------------------------------------------------------------------------+
| Tipo genitore | :doc:`choice </reference/forms/types/choice>`                          |
+---------------+------------------------------------------------------------------------+
| Classe        | :class:`Symfony\\Component\\Form\\Extension\\Core\\Type\\LanguageType` |
+---------------+------------------------------------------------------------------------+

Opzioni ridefinite
------------------

choices
~~~~~~~

**predefinito**: ``Symfony\Component\Intl\Intl::getLanguageBundle()->getLanguageNames()``.

Questa opzione ha come valore predefinito tutte le lingue.
Usa il locale predefinito per specificare la lingua.

Opzioni ereditate
-----------------

Queste opzioni sono ereditate dal tipo :doc:`choice </reference/forms/types/choice>`:


.. include:: /reference/forms/types/options/empty_value.rst.inc

.. include:: /reference/forms/types/options/error_bubbling.rst.inc

.. include:: /reference/forms/types/options/error_mapping.rst.inc

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

.. include:: /reference/forms/types/options/label.rst.inc

.. include:: /reference/forms/types/options/label_attr.rst.inc

.. include:: /reference/forms/types/options/mapped.rst.inc

.. include:: /reference/forms/types/options/read_only.rst.inc

.. include:: /reference/forms/types/options/required.rst.inc

.. _`componenti internazionali per Unicode`: http://site.icu-project.org
