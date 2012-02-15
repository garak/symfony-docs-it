empty_data
~~~~~~~~~~

**tipo**: ``mixed`` **predefinito**: ``array()`` se ``multiple`` o ``expanded``, ``''`` altrimenti

Questa opzione determina il valore restituito dal campo quando viene selezionato
``empty_value``.

Per esempio, se si vuole che il campo ``gender`` sia impostato a ``null`` quando non
viene scelto alcun valore, lo si può fare in questo modo:

.. code-block:: php

    $builder->add('gender', 'choice', array(
        'choices' => array(
            'm' => 'Maschio',
            'f' => 'Femmina'
        ),
        'required'    => false,
        'empty_value' => 'Scegliere un genere',
        'empty_data'  => null
    ));
