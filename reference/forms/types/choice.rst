.. index::
   single: Form; Campi; choice

Tipo di campo choice
====================

Un campo multi-funzione, usato per consentire all'utente di scegliere una o più opzioni.
Può essere reso come tag ``select``, bottone radio o checkbox.

Per usare questo campo, bisogna specificare l'opzione ``choice_list`` *oppure* l'opzione
``choices``.

+---------------+------------------------------------------------------------------------------+
| Reso come     | può essere vari tag (vedere sotto)                                           |
+---------------+------------------------------------------------------------------------------+
| Opzioni       | - `choices`_                                                                 |
|               | - `choice_list`_                                                             |
|               | - `placeholder`_                                                             |
|               | - `expanded`_                                                                |
|               | - `multiple`_                                                                |
|               | - `preferred_choices`_                                                       |
+---------------+------------------------------------------------------------------------------+
| Opzioni       | - `compound`_                                                                |
| ridefinite    | - `empty_data`_                                                              |
|               | - `error_bubbling`_                                                          |
+---------------+------------------------------------------------------------------------------+
| Opzioni       | - `by_reference`_                                                            |
| ereditate     | - `data`_                                                                    |
|               | - `disabled`_                                                                |
|               | - `error_mapping`_                                                           |
|               | - `inherit_data`_                                                            |
|               | - `label`_                                                                   |
|               | - `label_attr`_                                                              |
|               | - `mapped`_                                                                  |
|               | - `read_only`_                                                               |
|               | - `required`_                                                                |
+---------------+------------------------------------------------------------------------------+
| Tipo genitore | :doc:`form </reference/forms/types/form>`                                    |
+---------------+------------------------------------------------------------------------------+
| Classe        | :class:`Symfony\\Component\\Form\\Extension\\Core\\Type\\ChoiceType`         |
+---------------+------------------------------------------------------------------------------+

Esempio di utilizzo
-------------------

Il modo più facile per usare questo campo è specificare le scelte direttamente con
l'opzione ``choices``. La chiave dell'array diventa il valore effettivamente impostato
nell'oggetto (p.e. ``m``), mentre il valore è quello che l'utente vede nel
form (p.e. ``Maschio``).

.. code-block:: php

    $builder->add('gender', 'choice', array(
        'choices'   => array('m' => 'Maschio', 'f' => 'Femmina'),
        'required'  => false,
    ));

Impostando ``multiple`` a ``true``, si consente all'utente la scelta di più valori.
Il widget sarà reso come un un tag ``select`` con opzione ``mutliple`` oppure come una
serie di checkbox, a seconda dell'opzione ``expanded``:

.. code-block:: php

    $builder->add('availability', 'choice', array(
        'choices'   => array(
            'morning'   => 'Mattina',
            'afternoon' => 'Pomeriggio',
            'evening'   => 'Sera',
        ),
        'multiple'  => true,
    ));

Si può anche usare l'opzione ``choice_list``, che accetta un oggetto che può specificare
le scelte per il widget.

.. _forms-reference-choice-tags:

.. include:: /reference/forms/types/options/select_how_rendered.rst.inc

Opzioni del campo
-----------------

choices
~~~~~~~

**tipo**: ``array`` **predefinito**: ``array()``

Questo è il modo più semplice per specificare le scelte da usare per
questo campo. L'opzione ``choices`` è un array, in cui le chiavi sono il valore
dell'oggetto e i valori sono l'etichetta::

    $builder->add('gender', 'choice', array(
        'choices' => array('m' => 'Maschio', 'f' => 'Femmina')
    ));

.. tip::

    Quando i valori tra cui scegliere non sono interi o stringhe (ma per esempio float
    o booleani), si dovrebbe usare invece l'opzione `choice_list`_. Con tale
    opzione, si ha la possibilità di mantenere il formato originale dei dati, che è importante
    per assicurarsi una validazione corretta ed evitare inutili aggiornamenti sulla base dati,
    causati da una mancata corrispondenza tra tipi di dato.

choice_list
~~~~~~~~~~~

**tipo**: ``Symfony\Component\Form\Extension\Core\ChoiceList\ChoiceListInterface``

Questo è un modo per specificare le opzioni da usare per questo campo.
L'opzione ``choice_list`` deve essere un'istanza di ``ChoiceListInterface``.
Per classi avanzate, si può creare una classe personalizzata che implementi
questa interfaccia e fornisca le scelte.

Con questa opzione si possono anche aggiungere valori float, per essere selezionati come dati.

.. code-block:: php

    use Symfony\Component\Form\Extension\Core\ChoiceList\ChoiceList;

    // ...
    $builder->add('status', 'choice', array(
      'choice_list' => new ChoiceList(array(1, 0.5), array('Pieno', 'Metà')
    ));

.. include:: /reference/forms/types/options/placeholder.rst.inc

.. include:: /reference/forms/types/options/expanded.rst.inc

.. include:: /reference/forms/types/options/multiple.rst.inc

.. include:: /reference/forms/types/options/preferred_choices.rst.inc

Opzioni ridefinite
------------------

compound
~~~~~~~~

**tipo**: ``booleano`` **predefinito**: stesso valore dell'opzione ``expanded``

Questa opzione specifica se il form è un composto. Il valore predefinito è
sovrascritto dal valore dell'opzione ``expanded``.

.. include:: /reference/forms/types/options/empty_data.rst.inc
    :end-before: DEFAULT_PLACEHOLDER

Il valore predefinito effettivo di questa opzione dipende da altre opzioni:

* Se ``multiple`` è ``false`` ed ``expanded`` è ``false``, allora è ``''``
  (stringa vuota);
* Altrimenti, è ``array()`` (array vuoto).

.. include:: /reference/forms/types/options/empty_data.rst.inc
    :start-after: DEFAULT_PLACEHOLDER

error_bubbling
~~~~~~~~~~~~~~

**tipo**: ``booleano`` **predefinito**: ``false``

Fa in modo che l'errore su questo campo sia allegato al campo stesso, invece che
al campo genitore (il form, nella maggior parte dei casi).

Opzioni ereditate
-----------------

Queste opzioni sono ereditate dal tipo :doc:`form </reference/forms/types/form>`:

.. include:: /reference/forms/types/options/by_reference.rst.inc

.. include:: /reference/forms/types/options/data.rst.inc

.. include:: /reference/forms/types/options/disabled.rst.inc

.. include:: /reference/forms/types/options/error_mapping.rst.inc

.. include:: /reference/forms/types/options/inherit_data.rst.inc

.. include:: /reference/forms/types/options/label.rst.inc

.. include:: /reference/forms/types/options/label_attr.rst.inc

.. include:: /reference/forms/types/options/mapped.rst.inc

.. include:: /reference/forms/types/options/read_only.rst.inc

.. include:: /reference/forms/types/options/required.rst.inc

Variabili di campo
------------------

+------------------------+--------------+-------------------------------------------------------------------+
| Variabile              | Tipo         | Uso                                                               |
+========================+==============+===================================================================+
| multiple               | ``Booleano`` | Il valore dell'opzione `multiple`_.                               |
+------------------------+--------------+-------------------------------------------------------------------+
| expanded               | ``Booleano`` | Il valore dell'opzione `expanded`_.                               |
+------------------------+--------------+-------------------------------------------------------------------+
| preferred_choices      | ``array``    | Un array con gli oggetti ``ChoiceView`` delle scelte              |
|                        |              | che vanno mostrare all'utente con precedenza.                     |
+------------------------+--------------+-------------------------------------------------------------------+
| choices                | ``array``    | Un array con gli oggetti ``ChoiceView`` delle scelte              |
|                        |              | restanti.                                                         |
+------------------------+--------------+-------------------------------------------------------------------+
| separator              | ``stringa``  | Il separatore da usare tra i gruppi.                              |
+------------------------+--------------+-------------------------------------------------------------------+
| placeholder            | ``mixed``    | Il valore vuoto, se non già presente nella lista, altrimenti      |
|                        |              | ``null``.                                                         |
+------------------------+--------------+-------------------------------------------------------------------+
| is_selected            | ``callable`` | Una funziona che accetta un ``ChoiceView`` e i valori selezionati |
|                        |              | e dice se la scelta fa parte dei valori selezionati.              |
+------------------------+--------------+-------------------------------------------------------------------+
| placeholder_in_choices | ``Booleano`` | Se il valore vuoto è nella lista di scelte                        |
+------------------------+--------------+-------------------------------------------------------------------+

.. tip::

    È molto più veloce usare invece il test :ref:`form-twig-selectedchoice`,
    quando si usa Twig.
