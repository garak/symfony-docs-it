*   ``disabled`` [tipo: booleano, predefinito: false]
    Se non si vuole che l'utente modifichi il valore di un campo, si può impostare
    questa opzione a ``true``. Ogni valore inserito sarà ignorato.

    .. code-block:: php

        use Symfony\Component\Form\TextField
      
        $field = new TextField('status', array(
            'data' => 'Vecchi dati',
            'disabled' => true,
        ));
        $field->submit('Nuovi dati');

        // mostra "Vecchi dati"
        echo $field->getData();


