payload
~~~~~~~

**tipo**: ``mixed`` **predefinito**: ``null``

.. versionadded:: 2.6
    L'opzione ``payload`` è stata introdotta in Symfony 2.6.

Si può usare questa opzione per allegare dati arbitrari, specifici del dominio, a un vincolo.
Il payload configurato non è usato dal componente Validator, il suo trattamento è 
completamente a carico dello sviluppatore.

Per esempio, si potrebbero voler usare
:doc:`vari livelli di errore </cookbook/validation/severity>` per presentare i vincoli
falliti in modo diverso nel frontend, a seconda della graivtà
dell'errore.
