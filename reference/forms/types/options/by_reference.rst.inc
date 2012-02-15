by_reference
~~~~~~~~~~~~

**tipo**: ``Booleano`` **predefinito**: ``true``

Se il valore sottostante a un campo è un oggetto e questa opzione è
``true``, l'oggetto risultante non sarà effettivamente impostato al bind del
form. Per esempio, se si ha un campo ``author`` nell'oggetto sottostante,
istanza di una classe ``Author``, allora se ``by_reference`` è
``false``, l'oggetto ``Author`` sarà aggiornato con i dati sottostanti,
ma ``setAuthor`` non sarà effettivamente richiamata sull'oggetto principale. Poiché
l'oggetto ``Author`` è un riferimento, questo fa veramente differenza solo se si ha
nel metodo ``setAuthor``  una logica personalizzata, che si vuol essere sicuri
sia eseguita. In questo caso, impostare questa opzione a ``false``.
