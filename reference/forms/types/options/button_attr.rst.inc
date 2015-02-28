attr
~~~~

**tipo**: array **predefinito**: Array vuoto

Se si vogliono aggiungere attributi extra alla rappresentazione HTML del bottone,
si può usare l'opzione ``attr``. È un array associativo con gli attributi HTML
come chiavi. Può essere utile quando servono classi personalizzate per il bottone::

    $builder->add('save', 'button', array(
        'attr' => array('class' => 'save'),
    ));
