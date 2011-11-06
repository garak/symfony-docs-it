.. index::
   single: Form; Campi; repeated

Tipo di campo repeated
======================

Un campo speciale "group", che crea due campi identici, i cui valori devono
combaciare (altrimenti, viene lanciato un errore di validazionoe). L'uso più
comune è quando serve che l'utente ripeta la sua password o la sua email, per
verificarne l'accuratezza.

+---------------+------------------------------------------------------------------------+
| Reso come     | solitamente campo ``input`` ``text``, ma vedere l'opzione `type`_      |
+---------------+------------------------------------------------------------------------+
| Opzioni       | - `type`_                                                              |
|               | - `options`_                                                           |
|               | - `first_name`_                                                        |
|               | - `second_name`_                                                       |
+---------------+------------------------------------------------------------------------+
| Opzioni       | - `invalid_message`_                                                   |
| ereditate     | - `invalid_message_parameters`_                                        |
|               | - `error_bubbling`_                                                    |
+---------------+------------------------------------------------------------------------+
| Tipo genitore | :doc:`field</reference/forms/types/form>`                              |
+---------------+------------------------------------------------------------------------+
| Classe        | :class:`Symfony\\Component\\Form\\Extension\\Core\\Type\\RepeatedType` |
+---------------+------------------------------------------------------------------------+

Esempio di utilizzo
-------------------

.. code-block:: php

    $builder->add('password', 'repeated', array(
        'type' => 'password',
        'invalid_message' => 'Le password devono combaciare.',
        'options' => array('label' => 'Password'),
    ));

Dopo un invio di form con successo, il valore inserito in entrambi i campi "password"
diventa il dato della voce ``password``. In altre parole, anche se i due campi sono
effettivamente resi, i dati finali del form conterranno il singolo valore
(solitamente una stringa) necessario.

L'opzione più importante è ``type``, che può essere un qualsiasi tipo di campo e
determina il tipo effettivo dei due campi. L'opzione ``options`` è passata a ciascuno
dei due campi, il che vuol dire, in questo esempio, che è supportata qualsiasi opzione
supportata dal tipo ``password``.

Validazione
~~~~~~~~~~~

Una delle caratteristiche fondamentali del campo ``repeated`` è la validazione interna
(non occorre fare nulla per impostarla), che forza i due campi ad avere valori
combacianti. Se i due campi non combaciani, sarà mostrato un errore
all'utente.

si può usare l'opzione ``invalid_message`` per personalizzare l'errore che sarà
mostrato quando i due campi non combaciano.

Opzioni del campo
-----------------

type
~~~~

**tipo**: ``stringa`` **predefinito**: ``text``

I due campi sottostanti saranno di questo tipo. Per esempio, passare un tipo
``password`` renderà due campi password.

options
~~~~~~~

**tipo**: ``array`` **predefinito**: ``array()``

Questa opzione sarà passata a ciascuno dei due campi sottostanti. In altre
parole, queste opzioni personalizzano i singoli campi.
Per esempio, se l'opzione ``type`` è ``password``, questo array potrebbe contenere
le opzioni ``always_empty`` o ``required``, che sono opzioni supportate
dal tipo di campo ``password``.

first_name
~~~~~~~~~~

**tipo**: ``stringa`` **predefinito**: ``first``

L'effettivo nome del campo usato per il primo campo. Per lo più non ha significato,
tuttavia, essendo i dati effettivi inseriti in entrambi i campi disponibili sotto
la chiave associata al campo ``repeated`` medesimo (p.e
``password``). Tuttavia, se non si specifica una label, questo nome di campo è
usato per "indovinare" la label.

second_name
~~~~~~~~~~~

**tipo**: ``stringa`` **predefinito**: ``second``

Come ``first_name``, ma per il secondo campo.

Opzioni ereditate
-----------------

Queste opzioni sono ereditate dal tipo :doc:`field</reference/forms/types/field>`:

.. include:: /reference/forms/types/options/invalid_message.rst.inc

.. include:: /reference/forms/types/options/invalid_message_parameters.rst.inc

.. include:: /reference/forms/types/options/error_bubbling.rst.inc
