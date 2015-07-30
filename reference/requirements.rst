.. index::
   single: Requisiti

.. _requirements-for-running-symfony2:

Requisiti per far girare Symfony
================================

Per far girare Symfony, un sistema deve soddisfare un elenco di requisiti.
Si può verificare facilmente se un sistema abbia tutti i requisiti, richiamando ``web/config.php``
in una distribuzione di Symfony. Poiché la CLI spesso usa un file di configurazione ``php.ini``
diverso, è una buona idea verificare i requisiti anche tramite
riga di comando, con:

.. code-block:: bash

    $ php app/check.php

Di seguito la lista dei requisiti e dei requisiti opzionali.

Requisiti
---------

* PHP deve essere almeno alla versione 5.3.3
* JSON deve essere abilitato
* ctype deve essere abilitato
* ``php.ini`` deve avere l'impostazione ``date.timezone``

.. caution::

    Fare attenzione, perché Symfony ha alcuni limiti noti con versioni precedenti
    a PHP 5.3.8 o uguali a 5.3.16. Per maggiori informazioni, vedere la
    `sezione Requisiti del README`_.

Opzionali
---------

* Occorre avere il modulo PHP-XML installato
* Occorre avere almeno la versione 2.6.21 di libxml
* Il tokenizer di PHP deve essere abilitato
* Le funzioni mbstring devono essere abilitate
* iconv deve essere abilitato
* POSIX deve essere abilitato (solo su \*nix)
* Intl deve essere installato con ICU 4+
* APC 3.0.17+ (o un'altra cache di opcode) deve essere installato
* Impostazioni raccomandate di ``php.ini``

  * ``short_open_tag = Off``
  * ``magic_quotes_gpc = Off``
  * ``register_globals = Off``
  * ``session.auto_start = Off``

Doctrine
--------

Se si vuole usare Doctrine, bisogna avere PDO installato. Inoltre, bisogna avere
installato il driver PDO per la base dati che si vuole
utilizzare.

.. _`sezione Requisiti del README`: https://github.com/symfony/symfony#requirements
