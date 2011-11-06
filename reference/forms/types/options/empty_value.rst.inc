empty_value
~~~~~~~~~~~

**tipo**: ``stringa`` o ``booleano``

Questa opzione determina se apparirà o meno una speciale opzione vuota (p.e. "Scegliere
un'opzione") in cima al select. Questa opzione si applica solamente se entrambe le
opzioni ``expanded`` e ``multiple`` sono impstate a ``false``.

* Aggiungere un valore vuoto con "Scegliere un'opzione" come testo::

    $builder->add('states', 'choice', array(
        'empty_value' => 'Scegliere un\'opzione',
    ));

* Non mostrare alcun valore vuoto::

    $builder->add('states', 'choice', array(
        'empty_value' => false,
    ));

Se non si imposta l'opzione ``empty_value``, sarà aggiunta automaticamente
un'opzione vuota (senza testo), ma solo se l'opzione ``required`` è
``false``::

    // sarà aggiunta un'opzione vuota (senza testo)
    $builder->add('states', 'choice', array(
        'required' => false,
    ));
