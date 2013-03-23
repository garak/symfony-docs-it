cascade_validation
~~~~~~~~~~~~~~~~~~

**tipo**: booleano **predefinito**: false

Impostare questa opzione a ``true`` per forzare la validazione su form inclusi.
Per esempio, se si ha un ``ProductType`` con incluso ``CategoryType``,
impostare ``cascade_validation`` a ``true`` su ``ProductType`` farà validare
anche i dati di ``CategoryType``.

Invece di usare questa opzione, si può anche usare il vincolo ``Valid`` nel modello,
per forzare la validazione di un oggetto figlio memorizzato su una proprietà.




