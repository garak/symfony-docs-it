.. index::
    single: Security; CSRF nel form di login

Uso di CSRF nel form di login
=============================

Quando si usa un form di login, ci dovrebbe assicurare di essere protetti contro CSRF
(`Cross-site request forgery`_). Il componente Security include già un supporto
per CSRF. In questa ricetta vedremo come lo si può usare nei form di login.

.. note::

    Gli attacchi CSRF ai login sono meno noti. Vedere `Forging Login Requests`_
    per maggiori dettagli.

Configurazione della protezione CSRF
------------------------------------

Innanzitutto, configurare il componente Security, in modo da poter usare la protezione CSRF.
Il componente Security ha bisogno di un fornitore di token CSRF. Lo si può impostare per usare il
fornitore predefinito, disponibile nel componente Form:

.. configuration-block::

    .. code-block:: yaml

        # app/config/security.yml
        security:
            firewalls:
                area_protetta:
                    # ...
                    form_login:
                        # ...
                        csrf_provider: form.csrf_provider

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <?xml version="1.0" encoding="UTF-8" ?>
        <srv:container xmlns="http://symfony.com/schema/dic/security"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xmlns:srv="http://symfony.com/schema/dic/services"
            xsi:schemaLocation="http://symfony.com/schema/dic/services http://symfony.com/schema/dic/services/services-1.0.xsd">

            <config>
                <firewall name="area_protetta">
                    <!-- ... -->

                    <form-login csrf-provider="form.csrf_provider" />
                </firewall>
            </config>
        </srv:container>

    .. code-block:: php

        // app/config/security.php
        $container->loadFromExtension('security', array(
            'firewalls' => array(
                'area_protetta' => array(
                    // ...
                    'form_login' => array(
                        // ...
                        'csrf_provider' => 'form.csrf_provider',
                    )
                )
            )
        ));

Il componente Security può essere ulteriormente configurato, ma queste informazioni sono
sufficienti per l'uso di CSRF nel form di login.

Rendere il campo CSRF
---------------------

Ora che il componente Security verificherà il token CSRF, occorre aggiungere
un campo *nascosto* al form di login, contenente il token CSRF. Per impostazione predefinita,
tale campo si chiama ``_csrf_token``. Questo campo nascosto deve contenere il token CSRF,
che può essere generato usando la funzione ``csrf_token``. Questa
funzione richiede un identificativo per il token, che deve essere impostato per l'autenticazione
quando si usa il form di login:

.. configuration-block::

    .. code-block:: html+jinja

        {# src/Acme/SecurityBundle/Resources/views/Security/login.html.twig #}

        {# ... #}
        <form action="{{ path('login_check') }}" method="post">
            {# ... campi del login #}

            <input type="hidden" name="_csrf_token"
                value="{{ csrf_token('authenticate') }}"
            >

            <button type="submit">login</button>
        </form>

    .. code-block:: html+php

        <!-- src/Acme/SecurityBundle/Resources/views/Security/login.html.php -->

        <!-- ... -->
        <form action="<?php echo $view['router']->generate('login_check') ?>" method="post">
            <!-- ... campi del login -->

            <input type="hidden" name="_csrf_token"
                value="<?php echo $view['form']->csrfToken('authenticate') ?>"
            >

            <button type="submit">login</button>
        </form>

Dopo di che, il form di login è protetto conto attacchi CSRF.

.. tip::

    Si può cambiare il nome del campo, impostando ``csrf_parameter`` e modificare
    l'identificativo del token, impostando la voce ``intention`` nella configurazione:

    .. configuration-block::

        .. code-block:: yaml

            # app/config/security.yml
            security:
                firewalls:
                    area_protetta:
                        # ...
                        form_login:
                            # ...
                            csrf_parameter: _csrf_security_token
                            intention: una_stringa_privata

        .. code-block:: xml

            <!-- app/config/config.xml -->
            <?xml version="1.0" encoding="UTF-8" ?>
            <srv:container xmlns="http://symfony.com/schema/dic/security"
                xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
                xmlns:srv="http://symfony.com/schema/dic/services"
                xsi:schemaLocation="http://symfony.com/schema/dic/services http://symfony.com/schema/dic/services/services-1.0.xsd">

                <config>
                    <firewall name="area_protetta">
                        <!-- ... -->

                        <form-login csrf-parameter="_csrf_security_token"
                            intention="una_stringa_privata" />
                    </firewall>
                </config>
            </srv:container>

        .. code-block:: php

            // app/config/security.php
            $container->loadFromExtension('security', array(
                'firewalls' => array(
                    'area_protetta' => array(
                        // ...
                        'form_login' => array(
                            // ...
                            'csrf_parameter' => '_csrf_security_token',
                            'intention'      => 'una_stringa_privata',
                        )
                    )
                )
            ));

.. _`Cross-site request forgery`: http://en.wikipedia.org/wiki/Cross-site_request_forgery
.. _`Forging Login Requests`: http://en.wikipedia.org/wiki/Cross-site_request_forgery#Forging_login_requests
