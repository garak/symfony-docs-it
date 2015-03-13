placeholder
~~~~~~~~~~~

.. versionadded:: 2.6
    L'opzione ``placeholder`` è stata introdotta in Symfony 2.6, al posto di
    ``placeholder``, che è disponibile per le versioni precedenti.

.. versionadded:: 2.3
    Da Symfony 2.3, si possono usare valori vuoti anche se l'opzione ``expanded``
    è impostata a ``true``.

**tipo**: ``stringa`` o ``booleano``

Questa opzione determina se apparirà o meno una speciale opzione vuota (p.e. "Scegliere
un'opzione") in cima al select. Questa opzione si applica solamente se entrambe le
opzioni ``expanded`` e ``multiple`` sono impostate a ``false``.

* Aggiungere un valore vuoto con "Scegliere un'opzione" come testo::

    $builder->add('states', 'choice', array(
        'placeholder' => 'Scegliere un\'opzione',
    ));

* Non mostrare alcun valore vuoto::

    $builder->add('states', 'choice', array(
        'placeholder' => false,
    ));

Se non si imposta l'opzione ``placeholder``, sarà aggiunta automaticamente
un'opzione vuota (senza testo), ma solo se l'opzione ``required`` è
``false``::

    // sarà aggiunta un'opzione vuota (senza testo)
    $builder->add('states', 'choice', array(
        'required' => false,
    ));
