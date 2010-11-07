.. index::
   single: Email

Email
=====

Symfony2 sfrutta la potenza di `Swiftmailer`_ per l'invio delle email.

Installazione
-------------

Abilitare ``SwiftmailerBundle`` nel kernel::

    public function registerBundles()
    {
        $bundles = array(
            // ...
            new Symfony\Bundle\SwiftmailerBundle\SwiftmailerBundle(),
        );

        // ...
    }

Configurazione
--------------

L'unico parametro obbligatorio nella configurazione è ``transport``:

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        swiftmailer.config:
            transport:  smtp
            encryption: ssl
            auth_mode:  login
            host:       smtp.gmail.com
            username:   your_username
            password:   your_password

    .. code-block:: xml

        <!-- app/config/config.xml -->

        <!--
        xmlns:swiftmailer="http://www.symfony-project.org/schema/dic/swiftmailer"
        http://www.symfony-project.org/schema/dic/swiftmailer http://www.symfony-project.org/schema/dic/swiftmailer/swiftmailer-1.0.xsd
        -->

        <swiftmailer:config
            transport="smtp"
            encryption="ssl"
            auth_mode="login"
            host="smtp.gmail.com"
            username="your_username"
            password="your_password" />

    .. code-block:: php

        // app/config/config.php
        $container->loadFromExtension('swift', 'mailer', array(
            'transport'  => "smtp",
            'encryption' => "ssl",
            'auth_mode'  => "login",
            'host'       => "smtp.gmail.com",
            'username'   => "your_username",
            'password'   => "your_password",
        ));

I seguenti parametri di configurazione sono disponibili:

* ``transport`` (``smtp``, ``mail``, ``sendmail``, or ``gmail``)
* ``username``
* ``password``
* ``host``
* ``port``
* ``encryption`` (``tls``, or ``ssl``)
* ``auth_mode`` (``plain``, ``login``, or ``cram-md5``)
* ``type``
* ``delivery_strategy`` (``realtime``, ``spool``, ``single_address``, or ``none``)
* ``delivery_address`` (un indirizzo email a cui inviare TUTTE le email)
* ``disable_delivery``

Inviare email
-------------

Il mailer è accessibile tramite il servizio ``mailer``; in un'azione::

    public function indexAction($name)
    {
        // recuperare il mailer (obbligatorio per l'inizializzazione di Swift Mailer)
        $mailer = $this['mailer'];

        $message = \Swift_Message::newInstance()
            ->setSubject('Hello Email')
            ->setFrom('send@example.com')
            ->setTo('recipient@example.com')
            ->setBody($this->renderView('HelloBundle:Hello:email', array('name' => $name)))
        ;
        $mailer->send($message);

        return $this->render(...);
    }

.. note::
   Per garantire il disaccoppiamento il corpo delle email viene memorizzato
   in un template renderizzato utilizzando il metodo ``renderView()``.

Utilizzare Gmail
----------------

Per utilizzare il proprio account Gmail per inviare le email utilizzare il 
trasport speciale ``gmail``:

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        swiftmailer.config:
            transport: gmail
            username:  your_gmail_username
            password:  your_gmail_password

    .. code-block:: xml

        <!-- app/config/config.xml -->

        <!--
        xmlns:swift="http://www.symfony-project.org/schema/dic/swiftmailer"
        http://www.symfony-project.org/schema/dic/swiftmailer http://www.symfony-project.org/schema/dic/swiftmailer/swiftmailer-1.0.xsd
        -->

        <swiftmailer:config
            transport="gmail"
            username="your_gmail_username"
            password="your_gmail_password" />

    .. code-block:: php

        // app/config/config.php
        $container->loadFromExtension('swiftmailer', 'config', array(
            'transport' => "gmail",
            'username'  => "your_gmail_username",
            'password'  => "your_gmail_password",
        ));

.. _`Swiftmailer`: http://www.swiftmailer.org/
