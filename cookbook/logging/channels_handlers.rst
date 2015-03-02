.. index::
   single: Log

Scrivere messaggi di log su file diversi
========================================

La Standard Edition di Symfony contiene molti canali per i log: ``doctrine``,
``event``, ``security`` e ``request``. Ogni canale corrisponde a un servizio di
log (``monolog.logger.XXX``) nel contenitore ed è iniettato nel servizio
interessato. Lo scopo dei canali è quello di poter organizzare diversi
tipi di messaggi di log.

Per impostazione predefinita, Symfony scrive ogni messaggio di log in un singolo file
(indipendentemente dal canale).

Spostare un canale su un gestore diverso
----------------------------------------

Si supponga ora di voler loggare il canale ``doctrine`` in un altro file.

Per farlo, basta creare un nuovo gestore e configurarlo in questo modo:

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        monolog:
            handlers:
                main:
                    type:     stream
                    path:     /var/log/symfony.log
                    channels: [!doctrine]
                doctrine:
                    type:     stream
                    path:     /var/log/doctrine.log
                    channels: [doctrine]

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <container xmlns="http://symfony.com/schema/dic/services"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xmlns:monolog="http://symfony.com/schema/dic/monolog"
            xsi:schemaLocation="http://symfony.com/schema/dic/services
                http://symfony.com/schema/dic/services/services-1.0.xsd
                http://symfony.com/schema/dic/monolog
                http://symfony.com/schema/dic/monolog/monolog-1.0.xsd"
        >
            <monolog:config>
                <monolog:handler name="main" type="stream" path="/var/log/symfony.log">
                    <monolog:channels>
                        <monolog:channel>!doctrine</monolog:channel>
                    </monolog:channels>
                </monolog:handler>

                <monolog:handler name="doctrine" type="stream" path="/var/log/doctrine.log">
                    <monolog:channels>
                        <monolog:channel>doctrine</monolog:channel>
                    </monolog:channels>
                </monolog:handler>
            </monolog:config>
        </container>

    .. code-block:: php

        // app/config/config.php
        $container->loadFromExtension('monolog', array(
            'handlers' => array(
                'main'     => array(
                    'type'     => 'stream',
                    'path'     => '/var/log/symfony.log',
                    'channels' => array(
                        '!doctrine',
                    ),
                ),
                'doctrine' => array(
                    'type'     => 'stream',
                    'path'     => '/var/log/doctrine.log',
                    'channels' => array(
                        'doctrine',
                    ),
                ),
            ),
        ));

Specifiche YAML
---------------

Si può specificare la configurazione in molte forme:

.. code-block:: yaml

    channels: ~    # Include tutti i canali

    channels: pippo  # Include solo il canale "pippo"
    channels: !pippo # Include tutti i canali, tranne "pippo"

    channels: [pippo, pluto]   # Include solo i canali "pippo" e "pluto"
    channels: [!pippo, !pluto] # Include tutti i canali, tranne "pippo" e "pluto"

Creare il proprio canale
------------------------

Si può cambiare il canale usato da monolog su un servizio alla volta. Lo si può fare
tramite :ref:`configurazione <cookbook-monolog-channels-config>`, come mostrato qui sotto,
o aggiungendo il tag :ref:`monolog.logger<dic_tags-monolog>` a un servizio e
specificando quale canale il servizio dovrebbe usare per i log. In questo modo, il logger
iniettato in questo servizio viene preconfigurato per usare il canale
specificato.

.. _cookbook-monolog-channels-config:

Configurare canali aggiuntivi senza tag dei servizi
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

.. versionadded:: 2.3
    Questa caratteristica è stata introdotto in MonologBundle nella versione 2.4. Questa
    versione è compatibile con Symfony 2.3, che però installa MonologBundle 2.3.
    Per usare questa caratteristica, occorre aggiornare il bundle a mano.

Con MonologBundle 2.4 si possono configurare canali aggiuntivi, senza aver
bisogno di tag per i servizi:

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        monolog:
            channels: ["pippo", "pluto"]

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <container xmlns="http://symfony.com/schema/dic/services"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xmlns:monolog="http://symfony.com/schema/dic/monolog"
            xsi:schemaLocation="http://symfony.com/schema/dic/services
                http://symfony.com/schema/dic/services/services-1.0.xsd
                http://symfony.com/schema/dic/monolog
                http://symfony.com/schema/dic/monolog/monolog-1.0.xsd"
        >
            <monolog:config>
                <monolog:channel>pippo</monolog:channel>
                <monolog:channel>pluto</monolog:channel>
            </monolog:config>
        </container>

    .. code-block:: php

        // app/config/config.php
        $container->loadFromExtension('monolog', array(
            'channels' => array(
                'pippo',
                'pluto',
            ),
        ));

In questo modo si possono loggare messaggi al canale ``pippo`` usando
il servizio logger, registrato automaticamente, ``monolog.logger.pippo``.

Imparare di più con il ricettario
---------------------------------

* :doc:`/cookbook/logging/monolog`
