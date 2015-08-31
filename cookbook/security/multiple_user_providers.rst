Usare più fornitori di utenti
=============================

Ogni meccanismo di autenticazione (autenticazione HTTP, form di login, ecc.)
usa esattamente un fornitore di utente, il primo fornitore di utenti
dichiarato. E se invece si volessero specificare alcuni utenti nella configurazione
e tutti gli altri nella base dati? Lo si può fare, creando un nuovo
fornitore, che formi una catena degli altri fornitori esistenti:

.. configuration-block::

    .. code-block:: yaml

        # app/config/security.yml
        security:
            providers:
                chain_provider:
                    chain:
                        providers: [in_memory, user_db]
                in_memory:
                    memory:
                        users:
                            foo: { password: test }
                user_db:
                    entity: { class: Acme\UserBundle\Entity\User, property: username }

    .. code-block:: xml

        <!-- app/config/security.xml -->
        <?xml version="1.0" encoding="UTF-8"?>
        <srv:container xmlns="http://symfony.com/schema/dic/security"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xmlns:srv="http://symfony.com/schema/dic/services"
            xsi:schemaLocation="http://symfony.com/schema/dic/services
                http://symfony.com/schema/dic/services/services-1.0.xsd">

            <config>
                <provider name="chain_provider">
                    <chain>
                        <provider>in_memory</provider>
                        <provider>user_db</provider>
                    </chain>
                </provider>
                <provider name="in_memory">
                    <memory>
                        <user name="foo" password="test" />
                    </memory>
                </provider>
                <provider name="user_db">
                    <entity class="Acme\UserBundle\Entity\User" property="username" />
                </provider>
            </config>
        </srv:container>

    .. code-block:: php

        // app/config/security.php
        $container->loadFromExtension('security', array(
            'providers' => array(
                'chain_provider' => array(
                    'chain' => array(
                        'providers' => array('in_memory', 'user_db'),
                    ),
                ),
                'in_memory' => array(
                    'memory' => array(
                       'users' => array(
                           'foo' => array('password' => 'test'),
                       ),
                    ),
                ),
                'user_db' => array(
                    'entity' => array(
                        'class' => 'Acme\UserBundle\Entity\User',
                        'property' => 'username',
                    ),
                ),
            ),
        ));

Ora, tutti i meccanismi di autenticazione useranno ``chain_provider``, perché
è il primo specificato. ``chain_provider`` proverà a caricare
l'utente sia dal fornitore ``in_memory`` sia dal fornitore ``user_db``.

Si può anche configurare il firewall o singoli meccanismi di autenticazione
per usare uno specifico fornitore. Anche qui, a meno di specificare un fornitore,
sarà sempre usato il primo fornitore:

.. configuration-block::

    .. code-block:: yaml

        # app/config/security.yml
        security:
            firewalls:
                secured_area:
                    # ...
                    pattern: ^/
                    provider: user_db
                    http_basic:
                        realm: "Area demo protetta"
                        provider: in_memory
                    form_login: ~

    .. code-block:: xml

        <!-- app/config/security.xml -->
        <?xml version="1.0" encoding="UTF-8"?>
        <srv:container xmlns="http://symfony.com/schema/dic/security"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xmlns:srv="http://symfony.com/schema/dic/services"
            xsi:schemaLocation="http://symfony.com/schema/dic/services
                http://symfony.com/schema/dic/services/services-1.0.xsd">

            <config>
                <firewall name="secured_area" pattern="^/" provider="user_db">
                    <!-- ... -->
                    <http-basic realm="Area demo protetta" provider="in_memory" />
                    <form-login />
                </firewall>
            </config>
        </srv:container>

    .. code-block:: php

        // app/config/security.php
        $container->loadFromExtension('security', array(
            'firewalls' => array(
                'secured_area' => array(
                    // ...
                    'pattern' => '^/',
                    'provider' => 'user_db',
                    'http_basic' => array(
                        // ...
                        'provider' => 'in_memory',
                    ),
                    'form_login' => array(),
                ),
            ),
        ));

In questo esempio, se un utente prova a connettersi tramite autenticazione HTTP, il sistema di
autenticazione userà il fornitore di utenti ``in_memory``. Se invece l'utente prova a connettersi
tramite il form di login, sarà usato il fornitore ``user_db`` (essendo quello
predefinito per l'intero firewall).

Per maggiori informazioni sulla configurazione del fornitore di utenti e del firewall, si
veda :doc:`/reference/configuration/security`.
