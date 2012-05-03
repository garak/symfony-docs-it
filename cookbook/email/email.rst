.. index::
   single: Email

Come spedire un'email
=====================

Spedire le email è un delle azioni classiche di ogni applicazione web, ma 
rappresenta anche l'origine di potenziali problemi e complicazioni. Invece 
di reinventare la ruota, una soluzione per l'invio di email è l'uso di 
``SwiftmailerBundle``, il quale sfrutta la potenza della libreria `Swiftmailer`_ .

.. note::

    Non dimenticare di abilitare il bundle all'interno del kernel prima di utilizzarlo::

        public function registerBundles()
        {
            $bundles = array(
                // ...
                new Symfony\Bundle\SwiftmailerBundle\SwiftmailerBundle(),
            );

            // ...
        }

.. _swift-mailer-configuration:

Configurazione
--------------

Prima di utilizzare Swiftmailer, assicurarsi di includerne la configurazione. 
L'unico parametro obbligatorio della configurazione è il parametro ``transport``:

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        swiftmailer:
            transport:  smtp
            encryption: ssl
            auth_mode:  login
            host:       smtp.gmail.com
            username:   tuo_nome_utente
            password:   tua_password

    .. code-block:: xml

        <!-- app/config/config.xml -->

        <!--
        xmlns:swiftmailer="http://symfony.com/schema/dic/swiftmailer"
        http://symfony.com/schema/dic/swiftmailer http://symfony.com/schema/dic/swiftmailer/swiftmailer-1.0.xsd
        -->

        <swiftmailer:config
            transport="smtp"
            encryption="ssl"
            auth-mode="login"
            host="smtp.gmail.com"
            username="tuo_nome_utente"
            password="tua_password" />

    .. code-block:: php

        // app/config/config.php
        $container->loadFromExtension('swiftmailer', array(
            'transport'  => "smtp",
            'encryption' => "ssl",
            'auth_mode'  => "login",
            'host'       => "smtp.gmail.com",
            'username'   => "tuo_nome_utente",
            'password'   => "tua_password",
        ));

La maggior parte della configurazione di Swiftmailer è relativa al come
i messaggi debbano essere inoltrati.

Sono disponibili i seguenti parametri di configurazione:

* ``transport``         (``smtp``, ``mail``, ``sendmail`` o ``gmail``)
* ``username``
* ``password``
* ``host``
* ``port``
* ``encryption``        (``tls`` o ``ssl``)
* ``auth_mode``         (``plain``, ``login`` o ``cram-md5``)
* ``spool``

  * ``type`` (come accodare i messaggi: attualmente solo l'opzione ``file`` è supportata)
  * ``path`` (dove salvare i messaggi)
* ``delivery_address``  (indirizzo email dove spedire TUTTE le email)
* ``disable_delivery``  (impostare a true per disabilitare completamente l'invio)

L'invio delle email
-------------------

Per lavorare con la libreria Swiftmailer occorre creare, configurare e quindi 
spedire oggetti di tipo ``Swift_Message``. Il "mailer" è il vero responsabile 
dell'invio dei messaggi ed è accessibile tramite il servizio ``mailer``. 
In generale, spedire un'email è abbastanza intuitivo::

    public function indexAction($name)
    {
        $messaggio = \Swift_Message::newInstance()
            ->setSubject('Hello Email')
            ->setFrom('mittente@example.com')
            ->setTo('destinatario@example.com')
            ->setBody($this->renderView('HelloBundle:Hello:email.txt.twig', array('nome' => $nome)))
        ;
        $this->get('mailer')->send($messaggio);

        return $this->render(...);
    }

Per tenere i vari aspetti separati, il corpo del messaggio è stato salvato
in un template che viene poi restituito tramite il metodo ``renderView()``.

L'oggetto ``$messaggio`` supporta molte altre opzioni, come l'aggiunta di allegati, 
l'inserimento di HTML e molto altro. Fortunatamente la documentazione di Swiftmailer affronta 
questo argomento dettagliatamente nel capitolo sulla `Creazione di Messaggi`_ .

.. tip::

    Diversi altri articoli di questo ricettario spiegano come spedire le 
    email grazie Symfony2:

    * :doc:`gmail`
    * :doc:`dev_environment`
    * :doc:`spool`

.. _`Swiftmailer`: http://www.swiftmailer.org/
.. _`Creazione di Messaggi`: http://swiftmailer.org/docs/messages