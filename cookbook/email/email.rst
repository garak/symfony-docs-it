.. index::
   single: Email

Spedire un'email
================

Spedire email è una delle azioni classiche di ogni applicazione web, ma 
rappresenta anche l'origine di potenziali problemi e complicazioni. Invece 
di reinventare la ruota, una soluzione per l'invio di email è l'uso di 
``SwiftmailerBundle``, il quale sfrutta la potenza della libreria `Swift Mailer`_.
Questo bundle fa parte di Symfony Standard Edition.

.. _swift-mailer-configuration:

Configurazione
--------------

Per usare Swift Mailer, occorre configurarlo per il server di posta.

.. tip::

    Invece di usare un server di posta interno, si potrebbe voler usare
    un fornitore con hosting, come `Mandrill`_, `SendGrid`_, `Amazon SES`_
    o altri. In questo modo si ottiene un server SMTP, username e password (a volte
    chiamate chiavi), da usare con la configurazione di Swift Mailer.

In una tipica installazione di Symfony è già inclusa una configurazione
per ``swiftmailer``:

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        swiftmailer:
            transport: "%mailer_transport%"
            host:      "%mailer_host%"
            username:  "%mailer_user%"
            password:  "%mailer_password%"

    .. code-block:: xml

        <!-- app/config/config.xml -->

        <!--
            xmlns:swiftmailer="http://symfony.com/schema/dic/swiftmailer"
            http://symfony.com/schema/dic/swiftmailer http://symfony.com/schema/dic/swiftmailer/swiftmailer-1.0.xsd
        -->

        <swiftmailer:config
            transport="%mailer_transport%"
            host="%mailer_host%"
            username="%mailer_user%"
            password="%mailer_password%" />

    .. code-block:: php

        // app/config/config.php
        $container->loadFromExtension('swiftmailer', array(
            'transport'  => "%mailer_transport%",
            'host'       => "%mailer_host%",
            'username'   => "%mailer_user%",
            'password'   => "%mailer_password%",
        ));

Questi valori (come ``%mailer_transport%``) sono presi dai parametri,
impostati nel file :ref:`parameters.yml <config-parameters.yml>`. Si possono
modificare i valori lì o direttamente qui.

Sono disponibili i seguenti parametri di configurazione:

* ``transport``         (``smtp``, ``mail``, ``sendmail`` o ``gmail``)
* ``username``
* ``password``
* ``host``
* ``port``
* ``encryption``        (``tls`` o ``ssl``)
* ``auth_mode``         (``plain``, ``login`` o ``cram-md5``)
* ``spool``

  * ``type`` (come accodare i messaggi: sono supportate le opzioni ``file`` e ``memory``, vedere :doc:`/cookbook/email/spool`)
  * ``path`` (dove salvare i messaggi)
* ``delivery_address``  (indirizzo email dove spedire TUTTE le email)
* ``disable_delivery``  (impostare a true per disabilitare completamente l'invio)

Invio delle email
-----------------

Per lavorare con la libreria Swiftmailer occorre creare, configurare e quindi 
spedire oggetti di tipo ``Swift_Message``. Il "mailer" è il vero responsabile 
dell'invio dei messaggi ed è accessibile tramite il servizio ``mailer``. 
In generale, spedire un'email è abbastanza intuitivo::

    public function indexAction($nome)
    {
        $messaggio = \Swift_Message::newInstance()
            ->setSubject('Ciao')
            ->setFrom('mittente@example.com')
            ->setTo('destinatario@example.com')
            ->setBody(
                $this->renderView(
                    // app/Resources/views/Emails/registrazione.html.twig
                    'Emails/registration.html.twig',
                    array('nome' => $nome)
                ),
                'text/html'
            )
            /*
             * Se si vuole includere anche una parte in testo semplice
            ->addPart(
                $this->renderView(
                    'Emails/registrazione.txt.twig',
                    array('nome' => $nome)
                ),
                'text/plain'
            )
            */
        ;
        $mailer->send($messaggio);

        return $this->render(...);
    }

Per tenere i vari aspetti separati, il corpo del messaggio è stato salvato
in un template che viene poi restituito tramite il metodo ``renderView()``.

L'oggetto ``$messaggio`` supporta molte altre opzioni, come l'aggiunta di allegati, 
l'inserimento di HTML e molto altro. Fortunatamente la documentazione di Swift Mailer affronta 
questo argomento dettagliatamente nel capitolo sulla `creazione di messaggi`_ .

.. tip::

    Diversi altri articoli di questo ricettario spiegano come spedire le 
    email in Symfony:

    * :doc:`gmail`
    * :doc:`dev_environment`
    * :doc:`spool`

.. _`Swift Mailer`: http://swiftmailer.org/
.. _`creazione di messaggi`: http://swiftmailer.org/docs/messages.html
.. _`Mandrill`: https://mandrill.com/
.. _`SendGrid`: https://sendgrid.com/
.. _`Amazon SES`: http://aws.amazon.com/ses/
