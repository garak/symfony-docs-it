Lavorare con le email durante lo sviluppo
=========================================

Durante lo sviluppo di applicazioni che inviino email, non sempre è 
desiderabile che le email vengano inviate all'effettivo 
destinatario del messaggio. Se si utilizza ``SwiftmailerBundle`` con 
Symfony2, è possibile evitarlo semplicemente modificano i parametri di 
configurazione, senza modificare alcuna parte del codice. Ci sono due 
possibili scelte quando si tratta di gestire le email in fase di 
sviluppo: (a) disabilitare del tutto l'invio delle email o (b) inviare 
tutte le email a uno specifico indirizzo.

Disabilitare l'invio
--------------------

È possibile disabilitare l'invio delle email, ponendo ``true`` nell'opzione
``disable_delivery``. Questa è la configurazione predefinita per l'ambiente test
della distribuzione Standard. Facendo questa modifica nell'ambiente ``test`` 
le email non verranno inviate durante l'esecuzione dei test ma continueranno 
ad essere inviate negli ambienti ``prod`` e ``dev``:

.. configuration-block::

    .. code-block:: yaml

        # app/config/config_test.yml
        swiftmailer:
            disable_delivery:  true

    .. code-block:: xml

        <!-- app/config/config_test.xml -->

        <!--
        xmlns:swiftmailer="http://symfony.com/schema/dic/swiftmailer"
        http://symfony.com/schema/dic/swiftmailer http://symfony.com/schema/dic/swiftmailer/swiftmailer-1.0.xsd
        -->

        <swiftmailer:config
            disable-delivery="true" />

    .. code-block:: php

        // app/config/config_test.php
        $container->loadFromExtension('swiftmailer', array(
            'disable_delivery'  => "true",
        ));

Se si preferisce disabilitare l'invio anche nell'ambiente ``dev``, basterà
aggiungere la stessa configurazione nel file ``config_dev.yml``.

Invio a uno specifico indirizzo
-------------------------------

È possibile anche scegliere di inviare le email a uno specifico indirizzo, invece
che a quello effettivamente specificato nell'invio del messaggio. Ciò si può
fare tramite l'opzione ``delivery_address``:

.. configuration-block::

    .. code-block:: yaml

        # app/config/config_dev.yml
        swiftmailer:
            delivery_address:  dev@example.com

    .. code-block:: xml

        <!-- app/config/config_dev.xml -->

        <!--
        xmlns:swiftmailer="http://symfony.com/schema/dic/swiftmailer"
        http://symfony.com/schema/dic/swiftmailer http://symfony.com/schema/dic/swiftmailer/swiftmailer-1.0.xsd
        -->

        <swiftmailer:config
            delivery-address="dev@example.com" />

    .. code-block:: php

        // app/config/config_dev.php
        $container->loadFromExtension('swiftmailer', array(
            'delivery_address'  => "dev@example.com",
        ));

Supponiamo di inviare un'email a ``destinatario@example.com``.

.. code-block:: php

    public function indexAction($name)
    {
        $message = \Swift_Message::newInstance()
            ->setSubject('Email di saluto')
            ->setFrom('mittente@example.com')
            ->setTo('destinatario@example.com')
            ->setBody($this->renderView('HelloBundle:Hello:email.txt.twig', array('name' => $name)))
        ;
        $this->get('mailer')->send($message);

        return $this->render(...);
    }

Nell'ambiente ``dev``, l'email verrà in realtà inviata a ``dev@example.com``.
Swiftmailer aggiungerà un'ulteriore intestazione nell'email, ``X-Swift-To``, contenente
l'indirizzo sostituito, così da poter vedere a chi sarebbe stata inviata l'email in realtà.

.. note::

    Oltre alle email inviate all'indirizzo ``to``, questa configurazione 
    blocca anche quelle inviate a qualsiasi indirizzo ``CC`` e ``BCC`. 
    Swiftmailer aggiungerà ulteriori intestazioni contenenti gli indirizzi 
    ignorati. Le intestazioni usate saranno ``X-Swift-Cc`` e ``X-Swift-Bcc`` 
    rispettivamente per gli indirizzi in ``CC`` e per quelli in ``BCC``.

Visualizzazione tramite Web Debug Toolbar
-----------------------------------------

Utilizzando la Web Debug Toolbar è possibile visualizzare le email inviate 
durante la singola risposta nell'ambiente ``dev``. L'icona dell'email 
apparirà nella barra mostrando quante email sono state spedite. Cliccandoci 
sopra, un report mostrerà il dettaglio delle email inviate.

Se si invia un'email e immediatamente si esegue un redirect a un'altra pagina,
la web debug toolbar non mostrerà né l'icona delle email né alcun report
nella pagina finale.

È però possibile, configurando a ``true`` l'opzione ``intercept_redirects`` nel 
file ``config_dev.yml``, fermare il redirect in modo da permettere la visualizzazione
del report con il dettaglio delle email inviate.

.. tip::

    Alternativamente è possibile aprire il profiler in seguito al redirect e
    cercare la URL utilizzata nella richiesta precedente (e.g. ``/contatti/gestione``).
    Questa funzionalità di ricerca del profiler permette di ottenere informazioni relative
    a qualsiasi richiesta pregressa.

.. configuration-block::

    .. code-block:: yaml

        # app/config/config_dev.yml
        web_profiler:
            intercept_redirects: true

    .. code-block:: xml

        <!-- app/config/config_dev.xml -->

        <!-- xmlns:webprofiler="http://symfony.com/schema/dic/webprofiler" -->
        <!-- xsi:schemaLocation="http://symfony.com/schema/dic/webprofiler http://symfony.com/schema/dic/webprofiler/webprofiler-1.0.xsd"> -->

        <webprofiler:config
            intercept-redirects="true"
        />

    .. code-block:: php

        // app/config/config_dev.php
        $container->loadFromExtension('web_profiler', array(
            'intercept_redirects' => 'true',
        ));
