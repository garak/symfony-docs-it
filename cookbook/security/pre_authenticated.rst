.. index::
   single: Sicurezza; Fornitori di pre-autenticazione

Uso di firewall di sicurezza pre-autenticati
============================================

Molti moduli di autenticazione sono già forniti da alcuni server web,
compreso Apache. Questi moduli generalmente impostano alcune variabili di ambiente,
che possono essere usate per determinare quale utente stia accedendo a un'applicazione. In modo
predefinito, Symfony supporta la maggior parte dei meccanismi di autenticazione.
Tali richieste sono chiamate richieste "pre autenticate*, perché l'utente è già
autenticato quando raggiunge l'applicazione.

Autenticazione con certificato client X.509
-------------------------------------------

Quando si usando certificati lato client, il server web si occupa di tutta l'autenticazione.
Con Apache, per esempio, si può usare la direttiva
``SSLVerifyClient Require``.

Abilitare l'autenticazione x509 per un determinato firewall nella configurazione della sicurezza:

.. configuration-block::

    .. code-block:: yaml

        # app/config/security.yml
        security:
            firewalls:
                area_protetta:
                    pattern: ^/
                    x509:
                        provider: fornitore_di_utenti

    .. code-block:: xml

        <?xml version="1.0" ?>
        <!-- app/config/security.xml -->
        <srv:container xmlns="http://symfony.com/schema/dic/security"
            xmlns:srv="http://symfony.com/schema/dic/services">

            <config>
                <firewall name="area_protetta" pattern="^/">
                    <x509 provider="fornitore_di_utenti"/>
                </firewall>
            </config>
        </srv:container>

    .. code-block:: php

        // app/config/security.php
        $container->loadFromExtension('security', array(
            'firewalls' => array(
                'area_protetta' => array(
                    'pattern' => '^/'
                    'x509'    => array(
                        'provider' => 'fornitore_di_utenti',
                    ),
                ),
            ),
        ));

Per impostazione predefinita, il firewall fornisce la variabile ``SSL_CLIENT_S_DN_Email`` al
fornitore di utenti e imposta ``SSL_CLIENT_S_DN`` come credenziali in
:class:`Symfony\\Component\\Security\\Core\\Authentication\\Token\\PreAuthenticatedToken`.
Si possono sovrascrivere tali opzioni, impostando le voci ``user`` e ``credentials``
nella configurazione del firewall x509.

.. note::

    Un fornitore di autenticazione informa solamente il fornitore di utenti del nome utente
    che ha effettuato una richiesta. Occorrerà creare (o usare) un "fornitore di utenti",
    referenziato dal parametro di configurazione ``provider`` (``fornitore_di_utenti``
    nell'esempio di configurazione). Tale fornitore convertirà il nome utente in un oggetto User
    a scelta. Per maggiori informazioni sulla creazione e configurazione di un fornitore di
    utenti, vedere:

    * :doc:`/cookbook/security/custom_provider`
    * :doc:`/cookbook/security/entity_provider`