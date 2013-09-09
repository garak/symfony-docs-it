.. index::
   single: Security

Il componente Security
======================

Introduzione
------------

Il componente Security fornisce un sistema completo di sicurezza per un'applicazione
web. Dispone di strutture per l'autenticazione con HTTP basic authentication o con
digest authentication, form di login interattivo o login con certificato X.509,
ma consente anche di implementare strategie di autenticazione personalizzate.
Inoltre, il componente fornisce diversi modi per autorizzare gli utenti autenticati,
in base ai loro ruoli, e contiene un sistema di ACL avanzato.

Installazione
-------------

Il componente può essere installato in diversi modi:

* Utilizzando il repository Git ufficiale (https://github.com/symfony/Security);
* Installandolo :doc:`tramite Composer</components/using_components>` (`symfony/security`_ in Packagist).

Sezioni
-------

* :doc:`/components/security/firewall`
* :doc:`/components/security/authentication`
* :doc:`/components/security/authorization`

.. _symfony/security: https://packagist.org/packages/symfony/security
