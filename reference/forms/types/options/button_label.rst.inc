label
~~~~~

**tipo**: ``string`` **predefinito**: Il testo è "indovinato" dal nome del campo

Imposta il testo mostrato sul bottone. Il testo può anche essere
impostato direttamente all'interno del template:

.. configuration-block::

    .. code-block:: html+jinja

        {{ form_widget(form.save, { 'label': 'Cliccami' }) }}

    .. code-block:: html+php

        <?php echo $view['form']->widget($form['save'], array('label' => 'Cliccami')) ?>
