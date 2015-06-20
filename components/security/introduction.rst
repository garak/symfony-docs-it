.. index::
   single: Security

Il componente Security
======================

    Il componente Security fornisce un sistema completo di sicurezza per un'applicazione
    web. Dispone di strutture per l'autenticazione con HTTP basic authentication o con
    digest authentication, form di login interattivo o login con certificato X.509,
    ma consente anche di implementare strategie di autenticazione personalizzate.
    Inoltre, il componente fornisce diversi modi per autorizzare gli utenti autenticati,
    in base ai loro ruoli, e contiene un sistema di ACL avanzato.

Installazione
-------------

Il componente può essere installato in due modi:

* Installandolo :doc:`tramite Composer </components/using_components>` (``symfony/security`` su Packagist_);
* Utilizzando il repository Git ufficiale (https://github.com/symfony/Security).

.. include:: /components/require_autoload.rst.inc

Sezioni
-------

* :doc:`/components/security/firewall`
* :doc:`/components/security/authentication`
* :doc:`/components/security/authorization`
* :doc:`/components/security/secure_tools`

.. _Packagist: https://packagist.org/packages/symfony/security
