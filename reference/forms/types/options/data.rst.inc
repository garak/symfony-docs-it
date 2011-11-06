data
~~~~

**tipo**: mixed **predefinito**: Predefinito al campo dell'oggetto sottostante (se presente)

Quando si crea un form, ogni campo inizialmente mostra il valore della proprietà
corrispondente dell'oggetto del dominio del form (se un oggetto è legato al
form). Se si vuole sovrascrivere il valore iniziale per il form o solo per un singolo
campo, lo si può fare con l'opzione ``data``::

    $builder->add('token', 'hidden', array(
        'data' => 'abcdef',
    ));

