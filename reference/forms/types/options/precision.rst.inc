precision
~~~~~~~~~

**tipo**: ``intero`` **predefinito**: dipende dal locale (di solito intorno a ``3``)

Specifica quanti decimali saranno consentiti prima che il campo arrotondi
il valore inserito (tramite ``rounding_mode``). Per esempio, se ``precision``
è impostato a ``2``, un valore inserito di ``20.123`` sarà arrotondato,
per esempio, a ``20.12`` (a seconda di ``rounding_mode``).
