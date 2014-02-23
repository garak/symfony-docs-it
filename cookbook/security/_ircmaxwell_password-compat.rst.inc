.. caution::

    Se si usa PHP 5.4 o precedenti, occorrerà installare la libreria ``ircmaxell/password-compat``
    tramite Composer, per poter usare il codificatore ``bcrypt``:

    .. code-block:: json

        {
            "require": {
                ...
                "ircmaxell/password-compat": "~1.0.3"
            }
        }
