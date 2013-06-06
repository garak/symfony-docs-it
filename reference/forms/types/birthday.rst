.. index::
   single: Form; Campi; birthday

Tipo di campo birthday
======================

Un campo :doc:`date</reference/forms/types/date>` specializzato nel gestire
i compleanni.

Può essere reso come singola casella di testo, tre caselle di testo (mese, giorno e anno)
oppure tre select.

Questo tipo essenzialmente è lo stesso di :doc:`date</reference/forms/types/date>`,
ma con valori predefiniti più appropriati per l'opzione `years`_. L'opzione
`years`_ ha come valori predefiniti fino a 120 anni nel passato.

+--------------------------+-------------------------------------------------------------------------------+
| Tipo di dato sottostante | può essere ``DateTime``, ``string``, ``timestamp`` o ``array``                |
|                          | (vedere :ref:`opzione input <form-reference-date-input>`)                     |
+--------------------------+-------------------------------------------------------------------------------+
| Reso come                | può essere tre select o 1 o 3 caselle di testo, in base all'opzione `widget`_ |
+--------------------------+-------------------------------------------------------------------------------+
| Opzioni                  | - `years`_                                                                    |
+--------------------------+-------------------------------------------------------------------------------+
| Opzioni ereditate        | - `widget`_                                                                   |
|                          | - `input`_                                                                    |
|                          | - `months`_                                                                   |
|                          | - `days`_                                                                     |
|                          | - `format`_                                                                   |
|                          | - `data_timezone`_                                                            |
|                          | - `user_timezone`_                                                            |
|                          | - `invalid_message`_                                                          |
|                          | - `invalid_message_parameters`_                                               |
|                          | - `read_only`_                                                                |
|                          | - `disabled`_                                                                 |
|                          | - `mapped`_                                                                   |
|                          | - `virtual`_                                                                  |
+--------------------------+-------------------------------------------------------------------------------+
| Tipo genitore            | :doc:`date</reference/forms/types/date>`                                      |
+--------------------------+-------------------------------------------------------------------------------+
| Classe                   | :class:`Symfony\\Component\\Form\\Extension\\Core\\Type\\BirthdayType`        |
+--------------------------+-------------------------------------------------------------------------------+

Opzioni del campo
-----------------

years
~~~~~

**tipo**: ``array`` **predefinito**: 120 anni nel passato

Lista di anni disponibili nel campo anno. Questa opzione è rilevante solo
quando l'opzione ``widget`` è impostata a ``choice``.

Opzioni ereditate
-----------------

Queste opzioni sono ereditate dal tipo :doc:`date</reference/forms/types/date>`:

.. include:: /reference/forms/types/options/date_widget.rst.inc

.. include:: /reference/forms/types/options/date_input.rst.inc

.. include:: /reference/forms/types/options/months.rst.inc

.. include:: /reference/forms/types/options/days.rst.inc

.. include:: /reference/forms/types/options/date_format.rst.inc

.. include:: /reference/forms/types/options/data_timezone.rst.inc

.. include:: /reference/forms/types/options/user_timezone.rst.inc

Queste opzioni sono ereditate dal tipo :doc:`form</reference/forms/types/form>`:

.. include:: /reference/forms/types/options/invalid_message.rst.inc

.. include:: /reference/forms/types/options/invalid_message_parameters.rst.inc

.. include:: /reference/forms/types/options/read_only.rst.inc

.. include:: /reference/forms/types/options/disabled.rst.inc

.. include:: /reference/forms/types/options/mapped.rst.inc

.. include:: /reference/forms/types/options/virtual.rst.inc
