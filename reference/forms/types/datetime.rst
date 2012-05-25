.. index::
   single: Form; Campi; datetime

Tipo di campo datetime
======================

Questo tipo di campo consente all'utente di modicare dati che rappresentano
una data e un'ora (p.e. ``1984-06-05 12:15:30``).

Può essere reso come una casella di testo o con tag select. Il formato sottostante dei
dati può essere un oggetto ``DateTime``, una stringa, un timestamp o un array.

+--------------------------+-----------------------------------------------------------------------------+
| Tipo di dato sottostante | uno tra ``DateTime``, stringa, timestamp o array (vedere opzione ``input``) |
+--------------------------+-----------------------------------------------------------------------------+
| Reso come                | casella di testo o tre select                                               |
+--------------------------+-----------------------------------------------------------------------------+
| Opzioni                  | - `date_widget`_                                                            |
|                          | - `time_widget`_                                                            |
|                          | - `input`_                                                                  |
|                          | - `date_format`_                                                            |
|                          | - `hours`_                                                                  |
|                          | - `minutes`_                                                                |
|                          | - `seconds`_                                                                |
|                          | - `years`_                                                                  |
|                          | - `months`_                                                                 |
|                          | - `days`_                                                                   |
|                          | - `with_seconds`_                                                           |
|                          | - `data_timezone`_                                                          |
|                          | - `user_timezone`_                                                          |
+--------------------------+-----------------------------------------------------------------------------+
| Opzioni                  | - `invalid_message`_                                                        |
| ereditate                | - `invalid_message_parameters`_                                             |
+--------------------------+-----------------------------------------------------------------------------+
| Tipo genitore            | :doc:`form</reference/forms/types/form>`                                    |
+--------------------------+-----------------------------------------------------------------------------+
| Classe                   | :class:`Symfony\\Component\\Form\\Extension\\Core\\Type\\DateTimeType`      |
+--------------------------+-----------------------------------------------------------------------------+

Opzioni del campo
-----------------

date_widget
~~~~~~~~~~~

**tipo**: ``stringa`` **predefinito**: ``choice``

Definisce l'opzione ``widget`` per il tipo :doc:`date</reference/forms/types/date>`

time_widget
~~~~~~~~~~~

**tipo**: ``stringa`` **predefinito**: ``choice``

Definisce l'opzione ``widget`` per il tipo :doc:`time</reference/forms/types/time>`

input
~~~~~

**tipo**: ``stringa`` **predefinito**: ``datetime``

IL formato dei dati di *input*, cioè il formato in cui la data è memorizzata
nell'oggetto sottostante. Valori validi sono:

* ``string`` (p.e. ``2011-06-05 12:15:00``)
* ``datetime`` (un oggetto ``DateTime``)
* ``array`` (p.e. ``array(2011, 06, 05, 12, 15, 0)``)
* ``timestamp`` (p.e. ``1307276100``)

Il valore che arriva dal form sarà anche normalizzato in questo
formato.

date_format
~~~~~~~~~~~

**tipo**: ``intero`` o ``stringa`` **predefinito**: ``IntlDateFormatter::MEDIUM``

Definisce l'opzione ``format`` che sarà passata al campo date.
Vedere :ref:`date type's format option<reference-forms-type-date-format>`
per maggiori dettagli.

.. include:: /reference/forms/types/options/hours.rst.inc

.. include:: /reference/forms/types/options/minutes.rst.inc

.. include:: /reference/forms/types/options/seconds.rst.inc

.. include:: /reference/forms/types/options/years.rst.inc

.. include:: /reference/forms/types/options/months.rst.inc

.. include:: /reference/forms/types/options/days.rst.inc

.. include:: /reference/forms/types/options/with_seconds.rst.inc

.. include:: /reference/forms/types/options/data_timezone.rst.inc

.. include:: /reference/forms/types/options/user_timezone.rst.inc

Opzioni ereditate
-----------------

Queste opzioni sono ereditate dal tipo :doc:`field</reference/forms/types/field>`:

.. include:: /reference/forms/types/options/invalid_message.rst.inc

.. include:: /reference/forms/types/options/invalid_message_parameters.rst.inc