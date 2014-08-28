.. caution::

    L'opzione ``max_length`` è stata deprecata e sarà rimossa in 3.0.
    Usare invece l'opzione ``attr``, impostandola a un array con una chiave ``maxlength``.

max_length
~~~~~~~~~~

**tipo**: ``intero``

Se questa opzione non è nulla, aggiunge un attributo ``max_length``, usato da alcuni
browser per limitare la lunghezza del testo in un campo.

La validazione è solo su browser, quindi i dati devono essere comunque validati
lato server.

