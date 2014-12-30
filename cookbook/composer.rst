.. index::
    double: Composer; Installazione

Installare Composer
===================

`Composer`_ è il gestore di pacchetti usato dalle applicazioni PHP moderne ed lo
strumento raccomandato per installare Symfony.

Installare Composer su Linux e Mac OS X
---------------------------------------

Per installare Composer su Linux e Mac OS X, eseguire questi due comandi:

.. code-block:: bash

    $ curl -sS https://getcomposer.org/installer | php
    $ sudo mv composer.phar /usr/local/bin/composer

-.. note::

    Se non si dispone di ``curl``, si può anche semplicemente scaricare il file
    ``installer`` a mano, da http://getcomposer.org/installer, ed
    eseguire:

    .. code-block:: bash

        $ php installer
        $ sudo mv composer.phar /usr/local/bin/composer

Installre Composer su Windows
-----------------------------

Scaricare l'installatore da `getcomposer.org/download`_, eseguirlo e seguire
le istruzioni.

Saperne di più
--------------

Si può approfondire Composer nella sua `documentazione`_.

.. _`Composer`: https://getcomposer.org/
.. _`getcomposer.org/download`: https://getcomposer.org/download
.. _`documentazione`: https://getcomposer.org/doc/00-intro.md
