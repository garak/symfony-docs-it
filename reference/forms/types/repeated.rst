.. index::
   single: Form; Campi; repeated

Tipo di campo repeated
======================

Uno speciale "gruppo" di campi, che crea due campi identici, i cui valori devono
combaciare (altrimenti, viene lanciato un errore di validazione). L'uso più
comune è quando serve che l'utente ripeta la sua password o la sua email, per
verificarne l'accuratezza.

+---------------+------------------------------------------------------------------------+
| Reso come     | solitamente campo input ``text``, ma vedere l'opzione `type`_          |
+---------------+------------------------------------------------------------------------+
| Opzioni       | - `type`_                                                              |
|               | - `options`_                                                           |
|               | - `first_options`_                                                     |
|               | - `second_options`_                                                    |
|               | - `first_name`_                                                        |
|               | - `second_name`_                                                       |
+---------------+------------------------------------------------------------------------+
| Opzioni       | - `error_bubbling`_                                                    |
| ridefinite    |                                                                        |
+---------------+------------------------------------------------------------------------+
| Opzioni       | - `data`_                                                              |
| ereditate     | - `invalid_message`_                                                   |
|               | - `invalid_message_parameters`_                                        |
|               | - `mapped`_                                                            |
|               | - `error_mapping`_                                                     |
+---------------+------------------------------------------------------------------------+
| Tipo genitore | :doc:`form </reference/forms/types/form>`                              |
+---------------+------------------------------------------------------------------------+
| Classe        | :class:`Symfony\\Component\\Form\\Extension\\Core\\Type\\RepeatedType` |
+---------------+------------------------------------------------------------------------+

Esempio di utilizzo
-------------------

.. code-block:: php

    $builder->add('password', 'repeated', array(
        'type' => 'password',
        'invalid_message' => 'Le password devono combaciare.',
        'options' => array('attr' => array('class' => 'password-field')),
        'required' => true,
        'first_options'  => array('label' => 'Password'),
        'second_options' => array('label' => 'Ripetere Password'),
    ));

Dopo un invio di form con successo, il valore inserito in entrambi i campi "password"
diventa il dato della voce ``password``. In altre parole, anche se i due campi sono
effettivamente resi, i dati finali del form conterranno il singolo valore
(solitamente una stringa) necessario.

L'opzione più importante è ``type``, che può essere un qualsiasi tipo di campo e
determina il tipo effettivo dei due campi. L'opzione ``options`` è passata a ciascuno
dei due campi, il che vuol dire, in questo esempio, che è supportata qualsiasi opzione
supportata dal tipo ``password``.

Resa
~~~~

Il tipo di campo ``repeated`` consiste in realtà di due campi, che possono essere
resi in una volta sola o singolarmente. Per renderli in una volta sola, usare qualcosa
come:

.. configuration-block::

    .. code-block:: jinja

        {{ form_row(form.password) }}

    .. code-block:: php

        <?php echo $view['form']->row($form['password']) ?>

Per rendere ogni campo singolarmente, usare qualcosa come:

.. configuration-block::

    .. code-block:: jinja

        {# .first e .second possono variare, vedere nota più avanti #}
        {{ form_row(form.password.first) }}
        {{ form_row(form.password.second) }}

    .. code-block:: php

        <?php echo $view['form']->row($form['password']['first']) ?>
        <?php echo $view['form']->row($form['password']['second']) ?>

.. note::

    I nomi ``first`` e ``second`` sono i predefiniti per i due
    sottocampi. Tuttavia, tali nomi possono essere controllati tramite opzioni `first_name`_
    e `second_name`_. Se sei impostano tali opzioni, occorre usare i valori impostati,
    al posto di ``first`` e ``second``, durante la resa.

Validazione
~~~~~~~~~~~

Una delle caratteristiche fondamentali del campo ``repeated`` è la validazione interna
(non occorre fare nulla per impostarla), che forza i due campi ad avere valori
combacianti. Se i due campi non combaciano, sarà mostrato un errore
all'utente.

Si può usare l'opzione ``invalid_message`` per personalizzare l'errore che sarà
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

first_options
~~~~~~~~~~~~~

**tipo**: ``array`` **predefinito**: ``array()``

Ulteriori opzioni (saranno fuse in `options`) da passare
*solo* al primo campo. Particolarmente utile per personalizzare la
label::

    $builder->add('password', 'repeated', array(
        'first_options'  => array('label' => 'Password'),
        'second_options' => array('label' => 'Ripetere Password'),
    ));

second_options
~~~~~~~~~~~~~~

**tipo**: ``array`` **predefinito**: ``array()``

Ulteriori opzioni (saranno fuse in `options`) da passare
*solo* al secondo campo. Particolarmente utile per personalizzare la
label (vedere `first_options`_).

first_name
~~~~~~~~~~

**tipo**: ``stringa`` **predefinito**: ``first``

L'effettivo nome del campo usato per il primo campo. Per lo più non ha significato,
tuttavia, essendo i dati effettivi inseriti in entrambi i campi disponibili sotto
la chiave associata al campo ``repeated`` medesimo (p.e.
``password``). Tuttavia, se non si specifica una label, questo nome di campo è
usato per "indovinare" la label.

second_name
~~~~~~~~~~~

**tipo**: ``stringa`` **predefinito**: ``second``

Come ``first_name``, ma per il secondo campo.

Opzioni ridefinite
------------------

error_bubbling
~~~~~~~~~~~~~~

**predefinito**: ``false``

Opzioni ereditate
-----------------

Queste opzioni sono ereditate dal tipo :doc:`form </reference/forms/types/form>`:

.. include:: /reference/forms/types/options/data.rst.inc

.. include:: /reference/forms/types/options/invalid_message.rst.inc

.. include:: /reference/forms/types/options/invalid_message_parameters.rst.inc

.. include:: /reference/forms/types/options/mapped.rst.inc

.. include:: /reference/forms/types/options/error_mapping.rst.inc
