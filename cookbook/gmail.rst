.. index::
   single: Emails; Gmail

Come usare Gmail per l'invio delle mail
=======================================

In fase di sviluppo, invece di utilizzare un normale server SMTP per l'invio delle mail, 
potreste trovare più semplice e pratico usare Gmail. Il bundle Swiftmailer ne rende 
facilissimo l'utilizzo.

.. tip::

    Invece di usare il vostro normale account di Gmail, sarebbe meglio
    crearne uno da usare appositamente per questo scopo.

Nel file di configurazione dell'ambiente di sviluppo, assegnate al parametro ``transport`` 
l'ozione ``gmail`` e ai parametri ``username`` e ``password le vostre credenziali di Google:

.. configuration-block::

    .. code-block:: yaml

        # app/config/config_dev.yml
        swiftmailer:
            transport: gmail
            username:  vostro_nome_utente_gmail
            password:  vostra_password_gmail

    .. code-block:: xml

        <!-- app/config/config_dev.xml -->

        <!--
        xmlns:swiftmailer="http://symfony.com/schema/dic/swiftmailer"
        http://symfony.com/schema/dic/swiftmailer http://symfony.com/schema/dic/swiftmailer/swiftmailer-1.0.xsd
        -->

        <swiftmailer:config
            transport="gmail"
            username="vostro_nome_utente_gmail"
            password="vostra_password_gmail" />

    .. code-block:: php

        // app/config/config_dev.php
        $container->loadFromExtension('swiftmailer', array(
            'transport' => "gmail",
            'username'  => "vostro_nome_utente_gmail",
            'password'  => "vostra_password_gmail",
        ));

E il gioco è fatto!

.. note::

    L'attributo di trasporto ``gmail`` è in realtà una scorciatoia che imposta a ``smtp`` il trasporto, e 
    modifica ``encryption``, ``auth_mode`` e ``host`` in modo da poter comunicare con Gmail.
