.. index::
   single: Email; Gmail

Usare Gmail per l'invio delle email
===================================

In fase di sviluppo, invece di utilizzare un normale server SMTP per l'invio delle email, 
potrebbe essere più semplice e pratico usare Gmail. Il bundle Swiftmailer ne rende 
facilissimo l'utilizzo.

.. tip::

    Invece di usare un normale account di Gmail, sarebbe meglio
    crearne uno da usare appositamente per questo scopo.

Nel file di configurazione dell'ambiente di sviluppo, si assegna al parametro ``transport`` 
l'opzione ``gmail`` e ai parametri ``username`` e ``password`` le credenziali dell'account di Google:

.. configuration-block::

    .. code-block:: yaml

        # app/config/config_dev.yml
        swiftmailer:
            transport: gmail
            username:  nome_utente_gmail
            password:  password_gmail

    .. code-block:: xml

        <!-- app/config/config_dev.xml -->
        <?xml version="1.0" encoding="UTF-8" ?>
        <container xmlns="http://symfony.com/schema/dic/services"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xmlns:swiftmailer="http://symfony.com/schema/dic/swiftmailer"
            xsi:schemaLocation="http://symfony.com/schema/dic/services
                http://symfony.com/schema/dic/services/services-1.0.xsd
                http://symfony.com/schema/dic/swiftmailer
                http://symfony.com/schema/dic/swiftmailer/swiftmailer-1.0.xsd">

            <!-- ... -->
            <swiftmailer:config
                transport="gmail"
                username="nome_utente_gmail"
                password="password_gmail"
            />
        </container>

    .. code-block:: php

        // app/config/config_dev.php
        $container->loadFromExtension('swiftmailer', array(
            'transport' => 'gmail',
            'username'  => 'nome_utente_gmail',
            'password'  => 'password_gmail',
        ));

E il gioco è fatto!

.. tip::

    Se si usa Symfony Standard Edition, configurare i parametri in ``parameters.yml``:

    .. code-block:: yaml

        # app/config/parameters.yml
        parameters:
            # ...
            mailer_transport: gmail
            mailer_host:      ~
            mailer_user:      nome_utente_gmail
            mailer_password:  password_gmail

.. note::

    L'attributo di trasporto ``gmail`` è in realtà una scorciatoia che imposta a ``smtp`` il trasporto, e 
    modifica ``encryption``, ``auth_mode`` e ``host`` in modo da poter comunicare con Gmail.
