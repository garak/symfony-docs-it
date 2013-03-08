.. index::
   single: Test; Autenticazione HTTP

Come simulare un'autenticazione HTTP in un test funzionale
==========================================================

Se la propria applicazione necessita di autenticazione HTTP, passare il nome utente e
la password come variabili di ``createClient()``::

    $client = static::createClient(array(), array(
        'PHP_AUTH_USER' => 'nome_utente',
        'PHP_AUTH_PW'   => 'pa$$word',
    ));

Si possono anche sovrascrivere per ogni richiesta::

    $client->request('DELETE', '/post/12', array(), array(
        'PHP_AUTH_USER' => 'nome_utente',
        'PHP_AUTH_PW'   => 'pa$$word',
    ));

Quando l'applicazione usa un ``form_login``, si possono semplificare i test,
consentendo nella configurazione l'utilizzo dell'autenticazione HTTP. In questo modo
si può usare il codice precedente per l'autenticazione nei test, mantenendo il normale
login per gli utenti. Il trucco sta nell'includere la chiave ``http_basic``
nel firewall, insieme alla chiave ``form_login``:

.. configuration-block::

    .. code-block:: yaml

        # app/config/config_test.yml
        security:
            firewalls:
                mio_firewall:
                    http_basic:

    .. code-block:: xml

        <!-- app/config/config_test.xml -->
        <security:config>
            <security:firewall name="mio_firewall">
              <security:http-basic />
           </security:firewall>
        </security:config>

    .. code-block:: php

        // app/config/config_test.php
        $container->loadFromExtension('security', array(
            'firewalls' => array(
                'mio_firewall' => array(
                    'http_basic' => array(),
                ),
            ),
        ));
