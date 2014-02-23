post_max_size_message
~~~~~~~~~~~~~~~~~~~~~

**tipo**: ``stringa`` **predefinito**: ``The uploaded file was too large. Please try to upload a smaller file.``

Il messaggio di errore usato se i dati inviati in POST
eccedono la direttiva ``post_max_size`` di ``php.ini``. Si può usare il
segnaposto ``{{ max }}`` per mostrare la dimensione consentita.

.. note:

    La validazione di ``post_max_size`` è solo per il form radice.
