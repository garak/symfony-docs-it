.. index::
   single: Form; Campi; time

Tipo di campo time
==================

Un campo per inserire un tempo.

Può essere reso come campo testuale, una serie di campi testuali (p.e. ora,
minuto, secondo) o una serie di select. I dati sottostanti possono essere memorizzati
come oggetto ``DateTime``, stringa, timestamp o array.

+-------------------------+---------------------------------------------------------------------+
| Tipo di dato sottstante | ``DateTime``, stringa, timestamp o array (vedere opzione ``input``) |
+-------------------------+---------------------------------------------------------------------+
| Reso come               | può essere vari tag (vedere sotto)                                  |
+-------------------------+---------------------------------------------------------------------+
| Opzioni                 | - `widget`_                                                         |
|                         | - `input`_                                                          |
|                         | - `with_seconds`_                                                   |
|                         | - `hours`_                                                          |
|                         | - `minutes`_                                                        |
|                         | - `seconds`_                                                        |
|                         | - `model_timezone`_                                                 |
|                         | - `view_timezone`_                                                  |
|                         | - `empty_value`_                                                    |
+-------------------------+---------------------------------------------------------------------+
| Opzioni                 | - `by_reference`_                                                   |
| ridefinite              | - `error_bubbling`_                                                 |
+-------------------------+---------------------------------------------------------------------+
| Opzioni                 | - `invalid_message`_                                                |
| ereditate               | - `invalid_message_parameters`_                                     |
|                         | - `read_only`_                                                      |
|                         | - `disabled`_                                                       |
|                         | - `mapped`_                                                         |
|                         | - `inherit_data`_                                                   |
|                         | - `error_mapping`_                                                  |
+-------------------------+---------------------------------------------------------------------+
| Tipo genitore           | :doc:`form</reference/forms/types/form>`                            |
+-------------------------+---------------------------------------------------------------------+
| Classe                  | :class:`Symfony\\Component\\Form\\Extension\\Core\\Type\\TimeType`  |
+-------------------------+---------------------------------------------------------------------+

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

widget
~~~~~~

**tipo**: ``stringa`` **predefinito**: ``choice``

Il modo di base in cui il campo andrebbe reso. Può essere uno dei seguenti:

* ``choice``: rende due (o tre, se `with_seconds`_ è ``true``) select.

* ``text``: rende due o tre input testuali (ora, minuto, secondo).

* ``single_text``: rende un singolo input testuale. Il valore inserito dall'utente
  sarà validato nella forma ``hh:mm`` (o ``hh:mm:ss``, se si usano i secondi).

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

.. include:: /reference/forms/types/options/with_seconds.rst.inc

.. include:: /reference/forms/types/options/hours.rst.inc

.. include:: /reference/forms/types/options/minutes.rst.inc

.. include:: /reference/forms/types/options/seconds.rst.inc

.. include:: /reference/forms/types/options/model_timezone.rst.inc

.. include:: /reference/forms/types/options/view_timezone.rst.inc

.. include:: /reference/forms/types/options/empty_value.rst.inc

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

Queste opzioni sono ereditate dal tipo :doc:`form</reference/forms/types/form>`:

.. include:: /reference/forms/types/options/invalid_message.rst.inc

.. include:: /reference/forms/types/options/invalid_message_parameters.rst.inc

.. include:: /reference/forms/types/options/read_only.rst.inc

.. include:: /reference/forms/types/options/disabled.rst.inc

.. include:: /reference/forms/types/options/mapped.rst.inc

.. include:: /reference/forms/types/options/inherit_data.rst.inc

.. include:: /reference/forms/types/options/error_mapping.rst.inc
