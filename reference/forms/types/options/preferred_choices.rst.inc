preferred_choices
~~~~~~~~~~~~~~~~~

**tipo**: ``array`` **predefinito**: ``array()``

Se questa opzione viene specificata, un sotto-insieme di tutte le opzioni sarà
spostato in cima al select. Il codice seguente sposterà l'opzione "Paperino" in
cima, con un separatore visuale tra essa e le opzioni restanti::

    $builder->add('scelte_pippo', 'choice', array(
        'choices' => array('pippo' => 'Pippo', 'pluto' => 'Pluto', 'paperino' => 'Paperino'),
        'preferred_choices' => array('paperino'),
    ));

Si noti che le scelte preferite hanno senso solo con la resa di un elemento select
(cioè se ``expanded`` è ``false``). Le scelte preferite e le scelte normali sono
separate visivamente da una serie di righe (``-------------------``).
Il separatore può essere personalizzato durante la resa:

.. configuration-block::

    .. code-block:: jinja

        {{ form_widget(form.scelte_pippo, { 'separator': '=====' }) }}

    .. code-block:: php

        <?php echo $view['form']->widget($form['scelte_pippo'], array('separator' => '=====')) ?>
