.. index::
   single: Form; Campi; date

Tipo di campo date
==================

Un campo che consente all'utente di modificare le informazioni sulle date tramite
una serie di diversi elementi HTML.

I dati sottostanti usati per questo campo possono essere un oggetto ``DateTime``,
una stringa, un timestamp o un array. Se l'opzione `input`_ è impostata correttamente,
il campo si occuperà di tutti i dettagli.

Il campo può essere reso come una singola casella di testo, tre caselle di testo (mese,
giorno e anno) oppure tre select (vedere l'opzione `widget_`).

+--------------------------+-----------------------------------------------------------------------------+
| Tipo di dato sottostante | uno tra ``DateTime``, stringa, timestamp o array (vedere opzione ``input``) |
+--------------------------+-----------------------------------------------------------------------------+
| Reso come                | singolo campo testo o tre campi select                                      |
+--------------------------+-----------------------------------------------------------------------------+
| Opzioni                  | - `days`_                                                                   |
|                          | - `empty_value`_                                                            |
|                          | - `format`_                                                                 |
|                          | - `input`_                                                                  |
|                          | - `model_timezone`_                                                         |
|                          | - `months`_                                                                 |
|                          | - `view_timezone`_                                                          |
|                          | - `widget`_                                                                 |
|                          | - `years`_                                                                  |
+--------------------------+-----------------------------------------------------------------------------+
| Opzioni                  | - `by_reference`_                                                           |
| ridefinite               | - `error_bubbling`_                                                         |
+--------------------------+-----------------------------------------------------------------------------+
| Opzioni                  | - `data`_                                                                   |
| ereditate                | - `disabled`_                                                               |
|                          | - `error_mapping`_                                                          |
|                          | - `inherit_data`_                                                           |
|                          | - `invalid_message`_                                                        |
|                          | - `invalid_message_parameters`_                                             |
|                          | - `mapped`_                                                                 |
|                          | - `read_only`_                                                              |
+--------------------------+-----------------------------------------------------------------------------+
| Tipo genitore            | :doc:`form </reference/forms/types/form>`                                   |
+--------------------------+-----------------------------------------------------------------------------+
| Classe                   | :class:`Symfony\\Component\\Form\\Extension\\Core\\Type\\DateType`          |
+--------------------------+-----------------------------------------------------------------------------+

Uso di base
-----------

Questo tipo di campo è altamente configurabile, ma facile da usare. Le opzioni più
importanti sono ``input`` e ``widget``.

Si supponga di avere un campo ``publishedAt``, la cui data sottostante sia un oggetto
``DateTime``. Il codice seguente configura il tipo ``date`` per tale campo come
tre diversi campi di scelta::

    $builder->add('publishedAt', 'date', array(
        'input'  => 'datetime',
        'widget' => 'choice',
    ));

L'opzione ``input`` *deve* essere cambiata per corrispondere al tipo di dato della data
sottostante. Per esempio, se i dati del campo ``publishedAt`` fossero un timestamp,
si dovrebbe impostare ``input`` a ``timestamp``::

    $builder->add('publishedAt', 'date', array(
        'input'  => 'timestamp',
        'widget' => 'choice',
    ));

Il campo supporta anche ``array`` e ``string`` come valori validi dell'opzione
``input``.

Opzioni del campo
-----------------

.. include:: /reference/forms/types/options/days.rst.inc

empty_value
~~~~~~~~~~~

**tipo**: ``stringa`` o ``array``

Se l'opzione del widget è ``choice``, il campo sarà rappresentato come una serie
di ``select``. L'opzione ``empty_value`` può essere usata per aggiungere una voce
vuota in cima a ogni select::

    $builder->add('dueDate', 'date', array(
        'empty_value' => '',
    ));

In alternativa, si può specificare una stringa da mostrare per ogni voce vuota::

    $builder->add('dueDate', 'date', array(
        'empty_value' => array('year' => 'Anno', 'month' => 'Mese', 'day' => 'Giorno')
    ));

.. _reference-forms-type-date-format:

.. include:: /reference/forms/types/options/date_format.rst.inc

.. _form-reference-date-input:

.. include:: /reference/forms/types/options/date_input.rst.inc

.. include:: /reference/forms/types/options/model_timezone.rst.inc

.. include:: /reference/forms/types/options/months.rst.inc

.. include:: /reference/forms/types/options/view_timezone.rst.inc

.. include:: /reference/forms/types/options/date_widget.rst.inc

.. include:: /reference/forms/types/options/years.rst.inc

Opzioni ridefinite
------------------

by_reference
~~~~~~~~~~~~

**predefinito**: ``false``

Le classi ``DateTime`` sono trattate come oggetti immutabili.

error_bubbling
~~~~~~~~~~~~~~

**predefinito**: ``false``

Opzioni ereditate
-----------------

Queste opzioni sono ereditate dal tipo :doc:`form </reference/forms/types/form>`:


.. include:: /reference/forms/types/options/data.rst.inc

.. include:: /reference/forms/types/options/disabled.rst.inc

.. include:: /reference/forms/types/options/error_mapping.rst.inc

.. include:: /reference/forms/types/options/inherit_data.rst.inc

.. include:: /reference/forms/types/options/invalid_message.rst.inc

.. include:: /reference/forms/types/options/invalid_message_parameters.rst.inc

.. include:: /reference/forms/types/options/mapped.rst.inc

.. include:: /reference/forms/types/options/read_only.rst.inc

Variabili del campo
-------------------

+--------------+------------+----------------------------------------------------------------------+
| Variabile    | Tipo       | Uso                                                                  |
+==============+============+======================================================================+
| widget       | ``mixed``  | Il valore dell'opzione `widget`_.                                    |
+--------------+------------+----------------------------------------------------------------------+
| type         | ``string`` | Presente solo quando widget è ``single_text`` e HTML5 è attivo,      |
|              |            | contiene il tipo di input (``datetime``, ``date`` o ``time``).       |
+--------------+------------+----------------------------------------------------------------------+
| date_pattern | ``string`` | Una stringa con il formato di data.                                  |
+--------------+------------+----------------------------------------------------------------------+
