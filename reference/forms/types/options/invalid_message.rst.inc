invalid_message
~~~~~~~~~~~~~~~

**tipo**: ``stringa`` **predefinito**: ``This value is not valid``

Questo è il messaggio di errore di validazione usato quando i dati inseriti
sono determinati dalla validazione interna di un tipo di campo. Questo può accadere,
per esempio, se l'utente inserire una stringa dentro un campo 
:doc:`time</reference/forms/types/time>` che non può essere convertito in un tempo reale.
Per i normali messaggi di validazione (come quando si imposta la lunghezza minima per un
campo), impostare i messaggi di validazione con le proprie regole di validazione
(:ref:`riferimento<book-validation-constraint-configuration>`).
