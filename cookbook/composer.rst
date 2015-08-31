.. index::
    double: Composer; Installazione

Installare Composer
===================

`Composer`_ è il gestore di pacchetti usato dalle applicazioni PHP moderne. Usare Composer
per gestire dipendenze nelle applicazioni Symfony per per installare componenti Symfony
nei progetti PHP.

Si raccomanda di installare Composer globalmente nel proprio sistema, come spiegato
nelle sezioni seguenti.

Installare Composer su Linux e Mac OS X
---------------------------------------

Per installare Composer su Linux e Mac OS X, eseguire questi due comandi:

.. code-block:: bash

    $ curl -sS https://getcomposer.org/installer | php
    $ sudo mv composer.phar /usr/local/bin/composer

.. note::

    Se non si dispone di ``curl``, si può anche semplicemente scaricare il file
    ``installer`` a mano, da http://getcomposer.org/installer, ed
    eseguire:

    .. code-block:: bash

        $ php installer
        $ sudo mv composer.phar /usr/local/bin/composer

Installare Composer su Windows
------------------------------

Scaricare l'installatore da `getcomposer.org/download`_, eseguirlo e seguire
le istruzioni.

Saperne di più
--------------

Si può approfondire Composer nella sua `documentazione`_.

.. _`Composer`: https://getcomposer.org/
.. _`getcomposer.org/download`: https://getcomposer.org/download
.. _`documentazione`: https://getcomposer.org/doc/00-intro.md
