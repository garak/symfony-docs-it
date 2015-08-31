.. index::
   single: Email; In sviluppo

Lavorare con le email durante lo sviluppo
=========================================

Durante lo sviluppo di applicazioni che inviino email, non sempre è 
desiderabile che le email vengano inviate all'effettivo 
destinatario del messaggio. Se si utilizza SwiftmailerBundle con Symfony,
è possibile evitarlo semplicemente modificano i parametri di 
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
a essere inviate negli ambienti ``prod`` e ``dev``:

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
            delivery_address: dev@example.com

    .. code-block:: xml

        <!-- app/config/config_dev.xml -->

        <!--
            xmlns:swiftmailer="http://symfony.com/schema/dic/swiftmailer"
            http://symfony.com/schema/dic/swiftmailer http://symfony.com/schema/dic/swiftmailer/swiftmailer-1.0.xsd
        -->

        <swiftmailer:config delivery-address="dev@example.com" />

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
            ->setBody(
                $this->renderView(
                    'HelloBundle:Hello:email.txt.twig',
                    array('name' => $name)
                )
            )
        ;
        $this->get('mailer')->send($message);

        return $this->render(...);
    }

Nell'ambiente ``dev``, l'email verrà in realtà inviata a ``dev@example.com``.
Swiftmailer aggiungerà un'ulteriore intestazione nell'email, ``X-Swift-To``, contenente
l'indirizzo sostituito, così da poter vedere a chi sarebbe stata inviata l'email in realtà.

.. note::

    Oltre alle email inviate all'indirizzo ``to``, questa configurazione 
    blocca anche quelle inviate a qualsiasi indirizzo ``CC`` e ``BCC``. Swift Mailer
    aggiungerà ulteriori intestazioni contenenti gli indirizzi ignorati.
    Le intestazioni usate saranno ``X-Swift-Cc`` e ``X-Swift-Bcc`` 
    rispettivamente per gli indirizzi in ``CC`` e per quelli in ``BCC``.

.. _sending-to-a-specified-address-but-with-exceptions:

Invio a un indirizzo specifico ma con eccezioni
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Si supponga di volere che tutte le email siano rinviate a un indirizzo specifico
(come nello scenario precedente, a ``dev@example.com``). Mi si vorrebbe anche
che alcune email, indirizzate a specifici indirizzi, passino normalmente non
siano rinviate (anche in ambiente di sviluppo). Lo si può fare tramite
l'opzione ``delivery_whitelist``:

.. configuration-block::

    .. code-block:: yaml

        # app/config/config_dev.yml
        swiftmailer:
            delivery_address: dev@example.com
            delivery_whitelist:
               # per tutti gli indirizzi corrispondenti all'espressione regolare *non*
               # ci sarà rinvio a dev@example.com
               - "/@dominiospeciale.com$/"

               # anche i messaggi inviati ad admin@miomdominio.com non
               # saranno rinviati a dev@example.com
               - "/^admin@mydomain.com$/"

    .. code-block:: xml

        <!-- app/config/config_dev.xml -->

        <?xml version="1.0" charset="UTF-8" ?>
        <container xmlns="http://symfony.com/schema/dic/services"
            xmlns:swiftmailer="http://symfony.com/schema/dic/swiftmailer">

        <swiftmailer:config delivery-address="dev@example.com">
            <!-- per tutti gli indirizzi corrispondenti all'espressione regolare *non* ci sarà rinvio a dev@example.com -->
            <swiftmailer:delivery-whitelist-pattern>/@dominiospeciale.com$/</swiftmailer:delivery-whitelist-pattern>

            <!-- anche i messaggi inviati ad admin@miomdominio.com non saranno rinviati a dev@example.com -->
            <swiftmailer:delivery-whitelist-pattern>/^admin@miomdominio.com$/</swiftmailer:delivery-whitelist-pattern>
        </swiftmailer:config>

    .. code-block:: php

        // app/config/config_dev.php
        $container->loadFromExtension('swiftmailer', array(
            'delivery_address'  => "dev@example.com",
            'delivery_whitelist' => array(
                // per tutti gli indirizzi corrispondenti all'espressione regolare *non*
                // ci sarà rinvio a dev@example.com
                '/@dominiospeciale.com$/',

                // anche i messaggi inviati ad admin@miomdominio.com non 
                // saranno rinviati a dev@example.com
                '/^admin@miomdominio.com$/',
            ),
        ));

Nell'esempio appena visto, tutti i messaggi saranno rinviati a ``dev@example.com``,
tranne quelli inviati ad ``admin@miomdominio.com`` o a qualsiasi indirizzo
appartenente al dominio ``dominiospeciale.com``, che saranno consegnate normalmente.

Visualizzazione tramite barra di debug
--------------------------------------

Utilizzando la barra di debug, è possibile visualizzare le email inviate 
durante la singola risposta nell'ambiente ``dev``. L'icona dell'email 
apparirà nella barra mostrando quante email sono state spedite. Cliccandoci 
sopra, un rapporto mostrerà il dettaglio delle email inviate.

Se si invia un'email e immediatamente si esegue un rinvio a un'altra pagina,
la barra di debug del web non mostrerà né l'icona delle email né alcun rapporto
nella pagina finale.

È però possibile, configurando a ``true`` l'opzione ``intercept_redirects`` nel 
file ``config_dev.yml``, fermare il rinvio, in modo da permettere la visualizzazione
del rapporto con il dettaglio delle email inviate.

.. configuration-block::

    .. code-block:: yaml

        # app/config/config_dev.yml
        web_profiler:
            intercept_redirects: true

    .. code-block:: xml

        <!-- app/config/config_dev.xml -->

        <!--
            xmlns:webprofiler="http://symfony.com/schema/dic/webprofiler"
            xsi:schemaLocation="http://symfony.com/schema/dic/webprofiler
            http://symfony.com/schema/dic/webprofiler/webprofiler-1.0.xsd">
        -->

        <webprofiler:config
            intercept-redirects="true"
        />

    .. code-block:: php

        // app/config/config_dev.php
        $container->loadFromExtension('web_profiler', array(
            'intercept_redirects' => 'true',
        ));

.. tip::

    Alternativamente, è possibile aprire il profilatore in seguito al rinvio e
    cercare l'URL utilizzato nella richiesta precedente (p.e. ``/contatti/gestione``).
    Questa funzionalità di ricerca del profilatore permette di ottenere informazioni relative
    a qualsiasi richiesta pregressa.
