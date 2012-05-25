.. index::
   single: Form; Campi; money

Tipo di campo money
====================

Rende un campo di testo specializzato nella gestione di dati di
valuta (money).

Questo tipo di campo consente di specificare una valuta, il cui simbolo viene reso
accanto al campo testuale. Ci sono diverse altre opzioni per personalizzare la
gestione dell'input e dell'output dei dati.

+---------------+---------------------------------------------------------------------+
| Reso come     | campo ``input`` ``text``                                            |
+---------------+---------------------------------------------------------------------+
| Opzioni       | - `currency`_                                                       |
|               | - `divisor`_                                                        |
|               | - `precision`_                                                      |
|               | - `grouping`_                                                       |
+---------------+---------------------------------------------------------------------+
| Opzioni       | - `required`_                                                       |
| ereditate     | - `label`_                                                          |
|               | - `read_only`_                                                      |
|               | - `error_bubbling`_                                                 |
|               | - `invalid_message`_                                                |
|               | - `invalid_message_parameters`_                                     |
+---------------+---------------------------------------------------------------------+
| Tipo genitore | :doc:`field</reference/forms/types/field>`                          |
+---------------+---------------------------------------------------------------------+
| Classe        | :class:`Symfony\\Component\\Form\\Extension\\Core\\Type\\MoneyType` |
+---------------+---------------------------------------------------------------------+

Opzioni del campo
-----------------

currency
~~~~~~~~

**tipo**: ``stringa`` **predefinito**: ``EUR``

Specifica la valuta. Ciò determina il simbolo di valuta che dovrebbe essere
mostrato accanto alla casella di testo. A seconda della valuta, il simbolo potrebbe
essere mostrato prima o dopo la casella di
testo.

Può anche essere impostato a ``false``, per nascondere il simbolo della valuta.

divisor
~~~~~~~

**tipo**: ``intero`` **predefinito**: ``1``

Se, per qualche ragione, occorre dividere il valore di partenza per un numero, prima
di renderlo all'utente, si può usare l'opzione ``divisor``.
Per esempio::

    $builder->add('price', 'money', array(
        'divisor' => 100,
    ));

In questo caso, se il campo ``price`` è impostato a ``9900``, allora il valore reso
all'utente sarà ``99``. Quando l'utente invia il valore
``99``, sarà moltiplicato per ``100`` e ``9900`` sarà infine inviato
al proprio oggetto.

precision
~~~~~~~~~

**tipo**: ``intero`` **predefinito**: ``2``

Se, per qualche ragione, occore una precisione diversa da due cifre decimali,
si può modificare questo valore. Probabilmente non se ne avrà bisogno, a meno che,
per esempio, non si voglia arrotondare all'unità più vicina (impostare in questo caso
a ``0``).

.. include:: /reference/forms/types/options/grouping.rst.inc

Opzioni ereditate
-----------------

Queste opzioni sono ereditate dal tipo :doc:`field</reference/forms/types/field>`:

.. include:: /reference/forms/types/options/required.rst.inc

.. include:: /reference/forms/types/options/label.rst.inc

.. include:: /reference/forms/types/options/read_only.rst.inc

.. include:: /reference/forms/types/options/error_bubbling.rst.inc

.. include:: /reference/forms/types/options/invalid_message.rst.inc

.. include:: /reference/forms/types/options/invalid_message_parameters.rst.inc