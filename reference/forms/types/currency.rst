.. index::
    single: Form; Campi; currency

Tipo di campo currency
======================

Il tipo ``currency`` è un sottoinsieme del
:doc:`campo choice </reference/forms/types/choice>` che permette all'utente
di selezionare da una grande lista di valute `ISO 4217 a 3 lettere`_.

A differenza del tipo ``choice``, non c'è bisogno di specificare le opzioni
``choices`` o ``choice_list`` dato che il campo usa automaticamente una grande lista di
valute. *È possibile* anche specificare queste opzioni manualmente, ma in questo caso
si dovrebbe usare il tipo ``choice`` direttamente.

+---------------+------------------------------------------------------------------------+
| Reso come     | possono essere vari tag (vedi :ref:`forms-reference-choice-tags`)      |
+---------------+------------------------------------------------------------------------+
| Opzioni       | - `choices`_                                                           |
| Sovrascritte  |                                                                        |
+---------------+------------------------------------------------------------------------+
| Opzioni       | dal tipo :doc:`choice </reference/forms/types/choice>`                 |
| ereditate     |                                                                        |
|               | - `empty_value`_                                                       |
|               | - `error_bubbling`_                                                    |
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
| Classe        | :class:`Symfony\\Component\\Form\\Extension\\Core\\Type\\CurrencyType` |
+---------------+------------------------------------------------------------------------+

Opzioni ridefinite
------------------

choices
~~~~~~~

**default**: ``Symfony\Component\Intl\Intl::getCurrencyBundle()->getCurrencyNames()``

L'opzione ``choices`` è impostata in modo predefinito a tutte le valute.

Opzioni ereditate
-----------------

Queste opzioni sono ereditate dal tipo :doc:`choice </reference/forms/types/choice>`:


.. include:: /reference/forms/types/options/empty_value.rst.inc

.. include:: /reference/forms/types/options/error_bubbling.rst.inc

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

.. _`ISO 4217 a 3 lettere`: http://en.wikipedia.org/wiki/ISO_4217
