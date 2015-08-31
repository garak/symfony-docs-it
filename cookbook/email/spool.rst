.. index::
   single: Email; Spool

Lo spool della posta
====================

Quando si utilizza SwiftmailerBundle per l'invio delle email da un'applicazione
Symfony, queste vengono inviate immediatamente. È però possibile evitare il 
rallentamento dovuto dalla comunicazione tra ``Swiftmailer`` e  il servizio di
trasporto delle email, che potrebbe mettere l'utente in attesa del caricamento della
pagina durante l'invio. Per fare questo, basta scegliere di mettere le email 
in uno "spool", invece di spedirle direttamente. Questo vuol dire che ``Swiftmailer``
non cercherà di inviare le email, ma invece salverà i messaggi in qualche posto, come ad
esempio in un file. Un altro processo potrebbe poi leggere lo spool e prendersi
l'incarico di inviare le email in esso contenute. Attualmente ``Swiftmailer`` supporta solo
lo spool tramite file.

Spool in memoria
----------------

Se si usa lo spool per memorizzare email in memoria, saranno inviate subito prima del
termine del kernel. Questo vuol dire che le email sono inviate solamente se l'intera
richiesta viene eseguita, senza eccezioni o errori non gestiti. Per configurare
Swiftmailer con l'opzione memory, usare la seguente configurazione:

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        swiftmailer:
            # ...
            spool: { type: memory }

    .. code-block:: xml

        <!-- app/config/config.xml -->

        <!--
            xmlns:swiftmailer="http://symfony.com/schema/dic/swiftmailer"
            http://symfony.com/schema/dic/swiftmailer
            http://symfony.com/schema/dic/swiftmailer/swiftmailer-1.0.xsd
        -->

        <swiftmailer:config>
             <swiftmailer:spool type="memory" />
        </swiftmailer:config>

    .. code-block:: php

        // app/config/config.php
        $container->loadFromExtension('swiftmailer', array(
             // ...
            'spool' => array('type' => 'memory')
        ));

Spool in un file
----------------

Per usare lo spool con un file, usare la seguente configurazione:

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
            http://symfony.com/schema/dic/swiftmailer
            http://symfony.com/schema/dic/swiftmailer/swiftmailer-1.0.xsd
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
            ),
        ));

.. tip::

    Per creare lo spool all'interno delle cartelle del progetto, è possibile usare
    il parametro `%kernel.root_dir%` per indicare la cartella radice del
    progetto:

    .. code-block:: yaml

        path: "%kernel.root_dir%/spool"

Fatto questo, quando un'applicazione invia un'email, questa non verrà inviata subito
ma aggiunta allo spool. L'invio delle email dallo spool viene fatto da un processo separato.
Sarà un comando della console a inviare i messaggi dallo spool:

.. code-block:: bash

    $ php app/console swiftmailer:spool:send --env=prod

È possibile limitare il numero di messaggi da inviare con un'apposita opzione:

.. code-block:: bash

    $ php app/console swiftmailer:spool:send --message-limit=10 --env=prod

È anche possibile indicare un limite in secondi per l'invio:

.. code-block:: bash

    $ php app/console swiftmailer:spool:send --time-limit=10 --env=prod

Ovviamente questo comando non dovrà essere eseguito manualmente. Il comando
dovrebbe invece essere eseguito, a intervalli regolari, come un lavoro di 
cron o come un'operazione pianificata.
