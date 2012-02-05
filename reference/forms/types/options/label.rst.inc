label
~~~~~

**tipo**: ``stringa`` **predefinito**: "indovinato" dal nome del campo

Imposta la label usata per la resa del campo. La label può anche essere inserita
direttamente all'interno del template:
    
.. code-block:: jinja

    {{ form_label(form.name, 'Il tuo nome') }}
