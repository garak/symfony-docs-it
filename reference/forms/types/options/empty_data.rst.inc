empty_data
~~~~~~~~~~

**tipo**: ``mixed``

.. Questo file va incluso solo con start-after o end-before, impostato al
   valore di questo segnaposto. Lo scopo è di includere solo parte di questo file.

DEFAULT_PLACEHOLDER

Questa opzione determina il valore restituito dal campo quando viene selezionato
``placeholder``.

Si può personalizzare a seconda delle esigenze. Per esempio, se si vuole
che il campo ``gender`` sia impostato a ``null`` quando non viene scelto
alcun valore, lo si può fare in questo modo:

.. code-block:: php

    $builder->add('gender', 'choice', array(
        'choices' => array(
            'm' => 'Maschio',
            'f' => 'Femmina'
        ),
        'required'    => false,
        'placeholder' => 'Scegliere un genere',
        'empty_data'  => null
    ));

.. note::

    Se si vuole impostare l'opzione ``empty_data`` per l'intera classe del form,
    vedere la ricetta :doc:`/cookbook/form/use_empty_data`.
