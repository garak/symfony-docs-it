Si potrebbero voler aggiornare anche le altre librerie. Per chi ha fatto un
buon lavoro con i `vincoli di versione`_ in ``composer.json``, si può fare
in sicurezza, eseguendo:

.. code-block:: bash

    $ composer update

.. caution::

    Attenzione, però: se si hanno `vincoli di versione`_ incorretti in
    ``composer.json``, (come ``dev-master``), alcune librerie esterne
    potrebbero aggiornarsi a versioni che contengono modifiche non
    retrocompatibili.

.. _`version constraints`: https://getcomposer.org/doc/01-basic-usage.md#package-versions
