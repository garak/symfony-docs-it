.. index::
   single: Email; Uso del cloud

Usare il cloud per inviare email
================================

I requisiti per inviare email da un sistema di produzione sono diversi da quelli
di un sistema di sviluppo, perché non si desidera essere limitati nel numero di messaggi,
il tasso di invio o l'indirizzo del mittente. Quindi,
:doc:`usare Gmail </cookbook/email/gmail>` o servizi simili non è
un'opzione. Se configurare e mantenere un proprio sever di posta è troppo
complesso, c'è una semplice soluzione: sfruttare il cloud per inviare
email.

Questa ricetta mostra quanto sia facile integrare
`Simple Email Service (SES) di Amazon`_ in Symfony.

.. note::

    Si può usare la stessa tecnica per altri servizi di email, perché quasi
    sempre non c'è molto più che configurare un punto finale SMTP per
    SwiftMailer.

Nella configurazione di Symfony, cambiare le impostazioni di SwiftMailer ``transport``,
``host``, ``port`` ed ``encryption`` con le informazioni fornite
nella `console di SES`_. Creare le proprie credenziali SMTP nella console di SES
e completare la configurazione con ``username`` e ``password`` forniti:

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        swiftmailer:
            transport:  smtp
            host:       email-smtp.us-east-1.amazonaws.com
            port:       465 # ci sono varie porte a disposizione, vedere la console di SES
            encryption: tls # TLS è obbligatorio
            username:   CHIAVE_AWS  # da creare nella console di SES
            password:   SEGRETO_AWS # da creare nella console di SES

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <?xml version="1.0" encoding="UTF-8" ?>
        <container xmlns="http://symfony.com/schema/dic/services"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xmlns:swiftmailer="http://symfony.com/schema/dic/swiftmailer"
            xsi:schemaLocation="http://symfony.com/schema/dic/services http://symfony.com/schema/dic/services/services-1.0.xsd
                                http://symfony.com/schema/dic/swiftmailer http://symfony.com/schema/dic/swiftmailer/swiftmailer-1.0.xsd">

            <!-- ... -->
            <swiftmailer:config
                transport="smtp"
                host="email-smtp.us-east-1.amazonaws.com"
                port="465"
                encryption="tls"
                username="CHIAVE_AWS"
                password="SEGRETO_AWS"
            />
        </container>

    .. code-block:: php

        // app/config/config.php
        $container->loadFromExtension('swiftmailer', array(
            'transport'  => 'smtp',
            'host'       => 'email-smtp.us-east-1.amazonaws.com',
            'port'       => 465,
            'encryption' => 'tls',
            'username'   => 'CHIAVE_AWS',
            'password'   => 'SEGRETO_AWS',
        ));

Le voci ``port`` ed ``encryption`` non sono presenti nella configurazione predefinita
di Symfony Standard Edition, ma si possono semplicemente aggiungere.

Ecco fatto, si è pronti per iniziare a inviare email tramite il cloud!

.. tip::

    Se si usa Symfony Standard Edition, configurare i parametri in
    ``parameters.yml`` e usarli nei file di configurazione. Questo consente
    diverse configurazioni di SwiftMailer per le varie installazioni
    dell'applicazione. Per esempio, usare Gmail durante lo sviluppo e il cloud in
    produzione.

    .. code-block:: yaml

        # app/config/parameters.yml
        parameters:
            # ...
            mailer_transport:  smtp
            mailer_host:       email-smtp.us-east-1.amazonaws.com
            mailer_port:       465 # ci sono varie porte a disposizione, vedere la console di SES
            mailer_encryption: tls # TLS è obbligatorio
            mailer_user:       CHIAVE_AWS  # da creare nella console di SES
            mailer_password:   SEGRETO_AWS # da creare nella console di SES

.. note::

    Se si vuole usare SES di Amazon, si prenda nota di quanto segue:

    * Occorre iscriversi ad `Amazon Web Services (AWS)`_;

    * Ogni indirizzo mittente usato negli header ``From`` o ``Return-Path`` (indirizzo
      di bounce) deve essere confermato dal proprietario. Si può anche confermare
      un intero dominio;

    * All'inizio ci si trova in una modalità sandbox ristretta. Occorre richiedere
      l'accesso alla produzione prima di poter inviare a destinatari
      arbitrari;

    * SES potrebbe essere soggetto a una tariffa.

.. _`Simple Email Service (SES) di Amazon`: http://aws.amazon.com/ses
.. _`console di SES`: https://console.aws.amazon.com/ses
.. _`Amazon Web Services (AWS)`: http://aws.amazon.com
