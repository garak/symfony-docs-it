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
| Reso come                | single text box or three select fields                                      |
+--------------------------+-----------------------------------------------------------------------------+
| Opzioni                  | - `widget`_                                                                 |
|                          | - `input`_                                                                  |
|                          | - `empty_value`_                                                            |
|                          | - `years`_                                                                  |
|                          | - `months`_                                                                 |
|                          | - `days`_                                                                   |
|                          | - `format`_                                                                 |
|                          | - `pattern`_                                                                |
|                          | - `data_timezone`_                                                          |
|                          | - `user_timezone`_                                                          |
+--------------------------+-----------------------------------------------------------------------------+
| Tipo genitore            | ``field`` (se testo), ``form`` altrimenti                                   |
+--------------------------+-----------------------------------------------------------------------------+
| Classe                   | :class:`Symfony\\Component\\Form\\Extension\\Core\\Type\\DateType`          |
+--------------------------+-----------------------------------------------------------------------------+

Uso di base
-----------

Questo tipo di campo è altamente configurabile, ma facile da usare. Le opzioni più
importanti sono ``input`` e ``widget``.

Si supponga di avere un campo ``publishedAt``, la cui data sottostante sia un oggetto
``DateTime``. Il codice seguente configura il tipo ``date`` per tale campo come
tre diversi campi di scelta:

.. code-block:: php

    $builder->add('publishedAt', 'date', array(
        'input'  => 'datetime',
        'widget' => 'choice',
    ));

L'opzione ``input`` *deve* essere cambiata per corrispondere al tipo di dato della data
sottostante. Per esempio, se i dati del campo ``publishedAt`` fossero un timestamp,
si dovrebbe impostare ``input`` a ``timestamp``:

.. code-block:: php

    $builder->add('publishedAt', 'date', array(
        'input'  => 'timestamp',
        'widget' => 'choice',
    ));

Il campo supporta anche ``array`` e ``string`` come valori validi dell'opzione
``input``.

Opzioni del campo
-----------------

.. include:: /reference/forms/types/options/date_widget.rst.inc

.. _form-reference-date-input:

.. include:: /reference/forms/types/options/date_input.rst.inc

empty_value
~~~~~~~~~~~

**tipo**: ``stringa``|``array``

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

.. include:: /reference/forms/types/options/years.rst.inc

.. include:: /reference/forms/types/options/months.rst.inc

.. include:: /reference/forms/types/options/days.rst.inc

.. include:: /reference/forms/types/options/date_format.rst.inc

.. include:: /reference/forms/types/options/date_pattern.rst.inc

.. include:: /reference/forms/types/options/data_timezone.rst.inc

.. include:: /reference/forms/types/options/user_timezone.rst.inc
