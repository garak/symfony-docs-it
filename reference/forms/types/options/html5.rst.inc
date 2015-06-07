html5
~~~~~

.. versionadded:: 2.6
    L'opzione ``html5`` è stata introdotta in Symfony 2.6.

**tipo**: ``booleano`` **predefinito**: ``true``

Se impostata a ``true`` (valore predefinito), userà il tipo HTML5 (date, time
o datetime) per rendere il campo. Se impostata a ``false``, userà il tipo text.

Utile quando si vuole usare un calendario JavaScript personalizzato, che
spesso richiede un tipo text invece di un tipo HTML5.
