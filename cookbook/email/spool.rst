Lo spool della posta
====================

Quando si utilizza ``SwiftmailerBundle`` per l'invio delle email da un'applicazione
Symfony2, queste vengono inviate immediatamente. È però possibile evitare il 
rallentamento dovuto dalla comunicazione tra ``Swiftmailer`` e  il servizio di
trasporto delle mail che potrebbe mettere l'utente in attesa del caricamento della
pagina durante l'invio. Per fare questo basta scegliere di mettere le mail 
in uno "spool" invece di spedirle direttamente. Questo vuol dire che ``Swiftmailer``
non cerca di inviare le mail ma invece salva i messaggi in qualche posto come, ad
esempio, in un file. Un'altro processo potrebbe poi leggere lo spool e prendersi
l'incarico di inviare le mail in esso contenute. Attualmente ``Swiftmailer`` supporta solo
lo spool tramite file.

Per usare lo spool, si usa la seguente configurazione:

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        swiftmailer:
            # ...
            spool:
                type: file
                path: /percorso/file/di/spool

    .. code-block:: xml

        <!-- app/config/config.xml -->

        <!--
        xmlns:swiftmailer="http://symfony.com/schema/dic/swiftmailer"
        http://symfony.com/schema/dic/swiftmailer http://symfony.com/schema/dic/swiftmailer/swiftmailer-1.0.xsd
        -->

        <swiftmailer:config>
             <swiftmailer:spool
                 type="file"
                 path="/percorso/file/di/spool" />
        </swiftmailer:config>

    .. code-block:: php

        // app/config/config.php
        $container->loadFromExtension('swiftmailer', array(
             // ...
            'spool' => array(
                'type' => 'file',
                'path' => '/percorso/file/di/spool',
            )
        ));

.. tip::

    Per creare lo spool all'interno delle cartelle del progetto, è possibile usare
    il paramtreo `%kernel.root_dir%` per indicare la cartella radice del
    progetto:

    .. code-block:: yaml

        path: %kernel.root_dir%/spool

Fatto questo, quando un'applicazione invia una mail, questa non verrà inviata subito
ma aggiunta allo spool. L'invio delle mail dallo spool viene fatto da un processo separato.
Sarà un comando della console ad inviare i messaggi dallo spool:

.. code-block:: bash

    php app/console swiftmailer:spool:send

È possibili limitare il numero di messaggi da inviare con un'apposita opzione:

.. code-block:: bash

    php app/console swiftmailer:spool:send --message-limit=10

È anche possibile indicare un limite in secondi per l'invio:

.. code-block:: bash

    php app/console swiftmailer:spool:send --time-limit=10

Ovviamente questo comando non dovrà essere eseguito manualmente. Il comando
dovrebbe perciò essere eseguito, a intervalli regolari, come un lavoro di 
cron o come un'operazione pianificata.
