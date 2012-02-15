attr
~~~~

**tipo**: array **predefinito**: array vuoto

Se si vogliono aggiungere attributi extra a un campo HTML, si
può usare l'opzione ``attr``. È un array associativo con attributi HTML come
chiavi. Può essere utile quando occorre impostare una classe personalizzata per un widget::

    $builder->add('body', 'textarea', array(
        'attr' => array('class' => 'tinymce'),
    ));



