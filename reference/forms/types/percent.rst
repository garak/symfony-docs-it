.. index::
   single: Form; Campi; percent

Tipo di campo percent
=====================


Il tipo ``percent`` rende un campo testuale specializzato nella gestione di dati
percentuali. Se i dati percentuali sono memorizzati come decimali (p.e. ``.95``),
si può usare questo campo senza modifiche. Se si memorizzano i dati come numeri
(p.e. ``95``), bisogna impostare l'opzione ``type`` a ``integer``.

Questo campo aggiunge un simbolo di percentuale,  "``%``", dopo l'input.

+---------------+-----------------------------------------------------------------------+
| Reso come     | campo ``input`` ``text``                                              |
+---------------+-----------------------------------------------------------------------+
| Opzioni       | - `type`_                                                             |
|               | - `precision`_                                                        |
+---------------+-----------------------------------------------------------------------+
| Opzioni       | - `required`_                                                         |
| ereditate     | - `label`_                                                            |
|               | - `read_only`_                                                        |
|               | - `disabled`_                                                         |
|               | - `error_bubbling`_                                                   |
|               | - `invalid_message`_                                                  |
|               | - `invalid_message_parameters`_                                       |
|               | - `mapped`_                                                           |
+---------------+-----------------------------------------------------------------------+
| Tipo genitore | :doc:`field</reference/forms/types/field>`                            |
+---------------+-----------------------------------------------------------------------+
| Classe        | :class:`Symfony\\Component\\Form\\Extension\\Core\\Type\\PercentType` |
+---------------+-----------------------------------------------------------------------+

Opzioni
-------

type
~~~~

**tipo**: ``stringa`` **predefinito**: ``fractional``

Controlla come i dati sono memorizzati nel proprio oggetto. Per esempio, una
percentuale di "55%" potrebbe essere memorizzata come ``.55`` o ``55`` nel proprio
oggetto. I due tipi gestiscono questi due casi:

*   ``fractional``
    Se i dati sono memorizzati come decimale (p.e. ``.55``), usare questo tipo.
    I dati saranno moltiplicati per ``100`` prima di essere mostrati all'utente
    (p.e. ``55``). I dati inviati saranno divisi per ``100`` dopo l'invio
    del form, in modo che il valore sia memorizzato come decimale (``.55``);

*   ``integer``
    Se i dati sono memorizzati come intero (p.e. 55), usare questa opzione.
    Il valore grezzo (``55``) è mostrato all'utente e memorizzato nel proprio oggetto.
    Notare che ciò funziona solo per valori interi.

precision
~~~~~~~~~

**tipo**: ``intero`` **predefinito**: ``0``

Per impostazione predefinita, i numeri inseriti sono arrotondanti. Per consentire
ultetiori posizioni decimali, usare questa opzione.

Opzioni ereditate
-----------------

Queste opzioni sono ereditate dal tipo :doc:`form</reference/forms/types/form>`:

.. include:: /reference/forms/types/options/required.rst.inc

.. include:: /reference/forms/types/options/label.rst.inc

.. include:: /reference/forms/types/options/read_only.rst.inc

.. include:: /reference/forms/types/options/disabled.rst.inc

.. include:: /reference/forms/types/options/error_bubbling.rst.inc

.. include:: /reference/forms/types/options/invalid_message.rst.inc

.. include:: /reference/forms/types/options/invalid_message_parameters.rst.inc

.. include:: /reference/forms/types/options/mapped.rst.inc
