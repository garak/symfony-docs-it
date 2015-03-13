.. index::
   single: Form; Campi; time

Tipo di campo time
==================

Un campo per inserire un tempo.

Può essere reso come campo testuale, una serie di campi testuali (p.e. ora,
minuto, secondo) o una serie di select. I dati sottostanti possono essere memorizzati
come oggetto ``DateTime``, stringa, timestamp o array.

+--------------------------+---------------------------------------------------------------------+
| Tipo di dato sottostante | ``DateTime``, stringa, timestamp o array (vedere opzione ``input``) |
+--------------------------+---------------------------------------------------------------------+
| Reso come                | può essere vari tag (vedere sotto)                                  |
+--------------------------+---------------------------------------------------------------------+
| Opzioni                  | - `placeholder`_                                                    |
|                          | - `hours`_                                                          |
|                          | - `input`_                                                          |
|                          | - `minutes`_                                                        |
|                          | - `model_timezone`_                                                 |
|                          | - `seconds`_                                                        |
|                          | - `view_timezone`_                                                  |
|                          | - `widget`_                                                         |
|                          | - `with_minutes`_                                                   |
|                          | - `with_seconds`_                                                   |
+--------------------------+---------------------------------------------------------------------+
| Opzioni                  | - `by_reference`_                                                   |
| ridefinite               | - `error_bubbling`_                                                 |
+--------------------------+---------------------------------------------------------------------+
| Opzioni                  | - `data`_                                                           |
| ereditate                | - `disabled`_                                                       |
|                          | - `error_mapping`_                                                  |
|                          | - `inherit_data`_                                                   |
|                          | - `invalid_message`_                                                |
|                          | - `invalid_message_parameters`_                                     |
|                          | - `mapped`_                                                         |
|                          | - `read_only`_                                                      |
+--------------------------+---------------------------------------------------------------------+
| Tipo genitore            | form                                                                |
+--------------------------+---------------------------------------------------------------------+
| Classe                   | :class:`Symfony\\Component\\Form\\Extension\\Core\\Type\\TimeType`  |
+--------------------------+---------------------------------------------------------------------+

Utilizzo di base
----------------

Questo tipo di campo è altamente configurabile, ma facile da usare. Le opzioni più
importanti sono ``input`` e ``widget``.

Si supponga di avere un campo ``startTime``, il cui datp sottostante sia un oggetto
``DateTime``. Il codice seguente configura il tipo ``time`` per il campo come tre
campi di scelta separati:

.. code-block:: php

    $builder->add('startTime', 'time', array(
        'input'  => 'datetime',
        'widget' => 'choice',
    ));

L'opzione ``input`` *deve* essere cambiata per corrispondere al tipo di dato
sottostante. Per esempio, se il campo ``startTime`` fosse un timestamp,
bisognerebbe impostare ``input`` a ``timestamp``:

.. code-block:: php

    $builder->add('startTime', 'time', array(
        'input'  => 'timestamp',
        'widget' => 'choice',
    ));

Il campo supporta anche i valori ``array`` e ``string`` per l'opzione
``input``.

Opzioni del campo
-----------------

.. include:: /reference/forms/types/options/placeholder.rst.inc

.. include:: /reference/forms/types/options/hours.rst.inc

input
~~~~~

**tipo**: ``stringa`` **predefinito**: ``datetime``

IL formato dei dati di *ingresso*, cioè il formato in cui la data è memorizzata
nell'oggetto sottostante. Valori validi sono:

* ``stringa`` (p.e. ``12:17:26``)
* ``datetime`` (un oggetto ``DateTime``)
* ``array`` (p.e. ``array('hour' => 12, 'minute' => 17, 'second' => 26)``)
* ``timestamp`` (p.e. ``1307232000``)

Il valore proveniente dal form sarà normalizzato nello stesso
formato.

.. include:: /reference/forms/types/options/minutes.rst.inc

.. include:: /reference/forms/types/options/model_timezone.rst.inc

.. include:: /reference/forms/types/options/seconds.rst.inc

.. include:: /reference/forms/types/options/view_timezone.rst.inc

widget
~~~~~~

**tipo**: ``stringa`` **predefinito**: ``choice``

Il modo di base in cui il campo andrebbe reso. Può essere uno dei seguenti:

* ``choice``: rende uno, due (predefinito) o tre input select (ore, minuti,
  secondi), a seconda delle opzioni `with_minutes`_ e `with_seconds`_.

* ``text``: rende uno, due (predefinito) o tre input testuali (ore, minuti,
  secondi), a seconda delle opzioni `with_minutes`_ e `with_seconds`_.

* ``single_text``: rende un singolo input testuale. Il valore inserito dall'utente
  sarà validato nella forma ``hh:mm`` (o ``hh:mm:ss``, se si usano i secondi).

.. caution::

    la combinazione dell'opzione widget a ``single_text`` e `with_minutes`_
    a ``false`` può portare a comportamenti inattesi, perché l'input
    ``time`` potrebbe non supportare la selezione solo dell'ora.

.. include:: /reference/forms/types/options/with_minutes.rst.inc

.. include:: /reference/forms/types/options/with_seconds.rst.inc

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

Variabili di form
-----------------

+--------------+--------------+---------------------------------------------------------------------+
| Variabile    | Tipo         | Uso                                                                 |
+==============+==============+=====================================================================+
| widget       | ``mixed``    | Il valore dell'opzione `widget`_.                                   |
+--------------+--------------+---------------------------------------------------------------------+
| with_minutes | ``Booleano`` | Il valore dell'opzione `with_minutes`_.                             |
+--------------+--------------+---------------------------------------------------------------------+
| with_seconds | ``Booleano`` | Il valore dell'opzione `with_seconds`_ .                            |
+--------------+--------------+---------------------------------------------------------------------+
| type         | ``stringa``  | Presente solo se widget è ``single_text`` HTML5 è attivo, contiene  |
|              |              | il tipo di input da usare (``datetime``, ``date`` o ``time``).      |
+--------------+--------------+---------------------------------------------------------------------+
