.. index::
    single: Security; Impersonare utenti

Impersonare un utente
=====================

A volte, può essere utile poter cambiare da un utente all'altro, senza
dover uscire ed entrare (per esempio, quando si deve eseguire un debug o provare
a capire un bug che un utente vede e che non si riesce a riprodurre). Lo si può fare facilmente,
attivando l'ascoltatore ``switch_user`` in un firewall:

.. configuration-block::

    .. code-block:: yaml

        # app/config/security.yml
        security:
            firewalls:
                main:
                    # ...
                    switch_user: true

    .. code-block:: xml

        <!-- app/config/security.xml -->
        <?xml version="1.0" encoding="UTF-8"?>
        <srv:container xmlns="http://symfony.com/schema/dic/security"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xmlns:srv="http://symfony.com/schema/dic/services"
            xsi:schemaLocation="http://symfony.com/schema/dic/services
                http://symfony.com/schema/dic/services/services-1.0.xsd">
            <config>
                <firewall>
                    <!-- ... -->
                    <switch-user />
                </firewall>
            </config>
        </srv:container>

    .. code-block:: php

        // app/config/security.php
        $container->loadFromExtension('security', array(
            'firewalls' => array(
                'main'=> array(
                    // ...
                    'switch_user' => true
                ),
            ),
        ));

Per passare a un altro utente, basta aggiungere il parametro ``_switch_user``
alla query string e il nome utente come valore dell'URL corrente:

.. code-block:: text

    http://example.com/un_url?_switch_user=thomas

Per tornare all'utente precedente, usare il nome utente speciale ``_exit``:

.. code-block:: text

    http://example.com/un_url?_switch_user=_exit

Mentre si impersona un utente, questo dispone di un ruolo speciale, chiamato
``ROLE_PREVIOUS_ADMIN``. In un template, per esempio, si può usare tale ruolo
per mostrare un collegamento che riporti all'utente precedente:

.. configuration-block::

    .. code-block:: html+jinja

        {% if is_granted('ROLE_PREVIOUS_ADMIN') %}
            <a href="{{ path('homepage', {'_switch_user': '_exit'}) }}">Torna a utente precedente</a>
        {% endif %}

    .. code-block:: html+php

        <?php if ($view['security']->isGranted('ROLE_PREVIOUS_ADMIN')): ?>
            <a
                href="<?php echo $view['router']->generate('homepage', array(
                    '_switch_user' => '_exit',
                ) ?>"
            >
                Torna a utente precedente
            </a>
        <?php endif; ?>

Ovviamente, si dovrebbe rendere disponibile questa opzione a un gruppo ristretto di utenti.
Per impostazione predefinita, l'accesso è ristretto a utenti con il ruolo ``ROLE_ALLOWED_TO_SWITCH``.
Il nome di questo ruolo può essere modificato tramite l'impostazione ``role``. Per
estrema sicurezza, si può anche cambiare il nome del parametro della query, tramite
l'impostazione ``parameter``:

.. configuration-block::

    .. code-block:: yaml

        # app/config/security.yml
        security:
            firewalls:
                main:
                    # ...
                    switch_user: { role: ROLE_ADMIN, parameter: _voglio_cambiare_utente }

    .. code-block:: xml

        <!-- app/config/security.xml -->
        <?xml version="1.0" encoding="UTF-8"?>
        <srv:container xmlns="http://symfony.com/schema/dic/security"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xmlns:srv="http://symfony.com/schema/dic/services"
            xsi:schemaLocation="http://symfony.com/schema/dic/services
                http://symfony.com/schema/dic/services/services-1.0.xsd">
            <config>
                <firewall>
                    <!-- ... -->
                    <switch-user role="ROLE_ADMIN" parameter="_voglio_cambiare_utente" />
                </firewall>
            </config>
        </srv:container>

    .. code-block:: php

        // app/config/security.php
        $container->loadFromExtension('security', array(
            'firewalls' => array(
                'main'=> array(
                    // ...
                    'switch_user' => array(
                        'role' => 'ROLE_ADMIN',
                        'parameter' => '_voglio_cambiare_utente',
                    ),
                ),
            ),
        ));
