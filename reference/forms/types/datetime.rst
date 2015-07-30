.. index::
   single: Form; Campi; datetime

Tipo di campo datetime
======================

Questo tipo di campo consente all'utente di modificare dati che rappresentano
una data e un'ora (p.e. ``1984-06-05 12:15:30``).

Può essere reso come una casella di testo o con tag select. Il formato sottostante dei
dati può essere un oggetto ``DateTime``, una stringa, un timestamp o un array.

+--------------------------+-----------------------------------------------------------------------------+
| Tipo di dato sottostante | uno tra ``DateTime``, stringa, timestamp o array (vedere opzione ``input``) |
+--------------------------+-----------------------------------------------------------------------------+
| Reso come                | casella di testo o tre select                                               |
+--------------------------+-----------------------------------------------------------------------------+
| Opzioni                  | - `date_format`_                                                            |
|                          | - `date_widget`_                                                            |
|                          | - `days`_                                                                   |
|                          | - `empty_value`_                                                            |
|                          | - `format`_                                                                 |
|                          | - `hours`_                                                                  |
|                          | - `input`_                                                                  |
|                          | - `minutes`_                                                                |
|                          | - `model_timezone`_                                                         |
|                          | - `months`_                                                                 |
|                          | - `seconds`_                                                                |
|                          | - `time_widget`_                                                            |
|                          | - `view_timezone`_                                                          |
|                          | - `widget`_                                                                 |
|                          | - `with_minutes`_                                                           |
|                          | - `with_seconds`_                                                           |
|                          | - `years`_                                                                  |
+--------------------------+-----------------------------------------------------------------------------+
| Opzioni                  | - `data`_                                                                   |
| ereditate                | - `disabled`_                                                               |
|                          | - `inherit_data`_                                                           |
|                          | - `invalid_message`_                                                        |
|                          | - `invalid_message_parameters`_                                             |
|                          | - `mapped`_                                                                 |
|                          | - `read_only`_                                                              |
+--------------------------+-----------------------------------------------------------------------------+
| Tipo genitore            | :doc:`form </reference/forms/types/form>`                                   |
+--------------------------+-----------------------------------------------------------------------------+
| Classe                   | :class:`Symfony\\Component\\Form\\Extension\\Core\\Type\\DateTimeType`      |
+--------------------------+-----------------------------------------------------------------------------+

Opzioni del campo
-----------------

date_format
~~~~~~~~~~~

**tipo**: ``intero`` o ``stringa`` **predefinito**: ``IntlDateFormatter::MEDIUM``

Definisce l'opzione ``format`` che sarà passata al campo date.
Vedere le :ref:`opzioni per il formato date <reference-forms-type-date-format>`
per maggiori dettagli.

date_widget
~~~~~~~~~~~

.. include:: /reference/forms/types/options/date_widget_description.rst.inc

.. include:: /reference/forms/types/options/days.rst.inc

.. include:: /reference/forms/types/options/empty_value.rst.inc

format
~~~~~~

**tipo**: ``stringa`` **predefinito**: ``Symfony\Component\Form\Extension\Core\Type\DateTimeType::HTML5_FORMAT``

Se l'opzione ``widget`` è impostata a ``single_text``, questa opzione specifica
il formato del campo input, cioè il modo in cui Symfony interpreterà il dato fornito
come stringa temporale. Il formato predefinito è `RFC 3339`_, usato
dal campo ``datetime`` di HTML5. Con tale valore predefinito, il campo sarà
reso come ``input`` con ``type="datetime"``.

.. include:: /reference/forms/types/options/hours.rst.inc

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

.. include:: /reference/forms/types/options/_date_limitation.rst.inc

.. include:: /reference/forms/types/options/minutes.rst.inc

.. include:: /reference/forms/types/options/model_timezone.rst.inc

.. include:: /reference/forms/types/options/months.rst.inc

.. include:: /reference/forms/types/options/seconds.rst.inc

time_widget
~~~~~~~~~~~

**tipo**: ``stringa`` **predefinito**: ``choice``

Definisce l'opzione ``widget`` per il tipo :doc:`time</reference/forms/types/time>`


.. include:: /reference/forms/types/options/view_timezone.rst.inc

widget
~~~~~~

**tipo**: ``string`` **predefinito**: ``null``

Definisce l'opzione ``widget`` sia per il tipo :doc:`date </reference/forms/types/date>`
che per il tipo :doc:`time </reference/forms/types/time>`. Può essere ridefinito con
le opzioni `date_widget`_ e `time_widget`_.

.. include:: /reference/forms/types/options/with_minutes.rst.inc

.. include:: /reference/forms/types/options/with_seconds.rst.inc

.. include:: /reference/forms/types/options/years.rst.inc

Opzioni ereditate
-----------------

Queste opzioni sono ereditate dal tipo :doc:`form </reference/forms/types/form>`:


.. include:: /reference/forms/types/options/data.rst.inc

.. include:: /reference/forms/types/options/disabled.rst.inc

.. include:: /reference/forms/types/options/inherit_data.rst.inc

.. include:: /reference/forms/types/options/invalid_message.rst.inc

.. include:: /reference/forms/types/options/invalid_message_parameters.rst.inc

.. include:: /reference/forms/types/options/mapped.rst.inc

.. include:: /reference/forms/types/options/read_only.rst.inc

Variabili di form
-----------------

+-----------+------------+-------------------------------------------------------------------------+
| Variabile | Tipo       | Uso                                                                     |
+===========+============+=========================================================================+
| widget    | ``mixed``  | Il valore dell'opzione `widget`_.                                       |
+-----------+------------+-------------------------------------------------------------------------+
| type      | ``string`` | Presente solo se il widget è ``single_text`` e HTML5 è attivo,          |
|           |            | contiene il tipo di input da usare (``datetime``, ``date`` o ``time``). |
+-----------+------------+-------------------------------------------------------------------------+

.. _`RFC 3339`: http://tools.ietf.org/html/rfc3339
