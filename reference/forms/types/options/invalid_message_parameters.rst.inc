invalid_message_parameters
~~~~~~~~~~~~~~~~~~~~~~~~~~

**tipo**: ``array`` **predefinito**: ``array()``

Impostando l'opzione ``invalid_message``, si potrebbe aver bisogno di includere
alcune variabili nella stringa. Lo si può fare aggiungendo dei segnaposto all'opzione
e includendo le variabili in questa opzione::

    $builder->add('un_campo', 'un_tipo', array(
        // ...
        'invalid_message'            => 'Valore inserito non valido: deve includere almeno %num% caratteri',
        'invalid_message_parameters' => array('%num%' => 6),
    ));
