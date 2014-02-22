label_attr
~~~~~~~~~~

**tipo**: ``array`` **predefinito**: ``array()``

Imposta gli attributi HTML per l'elemento ``<label>``, che saranno usati durante
la resa della label di un campo. È un array associativo con gli attributi HTML
come chiavi. Questi attributi possono anche essere impostati all'interno del template:

.. configuration-block::

    .. code-block:: jinja

        {{ form_label(form.name, 'Nome', {'label_attr': {'class': 'CUSTOM_LABEL_CLASS'}}) }}

    .. code-block:: php

        echo $view['form']->label(
            $form['name'],
            'Nome',
            array('class', 'CUSTOM_LABEL_CLASS')
        );
