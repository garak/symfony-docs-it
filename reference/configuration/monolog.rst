.. index::
   pair: Monolog; Riferimento configurazione

Riferimento configurazione
==========================

.. configuration-block::

    .. code-block:: yaml

        monolog:
            handlers:

                # Esempi:
                syslog:
                    type:                stream
                    path:                /var/log/symfony.log
                    level:               ERROR
                    bubble:              false
                    formatter:           mio_formattatore
                    processors:
                        - un_callable
                main:
                    type:                fingers_crossed
                    action_level:        WARNING
                    buffer_size:         30
                    handler:             custom
                custom:
                    type:                service
                    id:                  my_handler

                # Opzioni e valori predfiniti per un "mio_gestore"
                mio_gestore:
                    type:                 ~ # Obbligatorio
                    id:                   ~
                    priority:             0
                    level:                DEBUG
                    bubble:               true
                    path:                 "%kernel.logs_dir%/%kernel.environment%.log"
                    ident:                false
                    facility:             user
                    max_files:            0
                    action_level:         WARNING
                    activation_strategy:  ~
                    stop_buffering:       true
                    buffer_size:          0
                    handler:              ~
                    members:              []
                    channels:
                        type:     ~
                        elements: ~
                    from_email:           ~
                    to_email:             ~
                    subject:              ~
                    email_prototype:
                        id:     ~ # Obbligatorio (se si usa email_prototype)
                        factory-method:       ~
                    channels:
                        type:                 ~
                        elements:             []
                    formatter:            ~

    .. code-block:: xml

        <container xmlns="http://symfony.com/schema/dic/services"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xmlns:monolog="http://symfony.com/schema/dic/monolog"
            xsi:schemaLocation="http://symfony.com/schema/dic/services http://symfony.com/schema/dic/services/services-1.0.xsd
                                http://symfony.com/schema/dic/monolog http://symfony.com/schema/dic/monolog/monolog-1.0.xsd">

            <monolog:config>
                <monolog:handler
                    name="syslog"
                    type="stream"
                    path="/var/log/symfony.log"
                    level="error"
                    bubble="false"
                    formatter="my_formatter"
                />
                <monolog:handler
                    name="main"
                    type="fingers_crossed"
                    action-level="warning"
                    handler="custom"
                />
                <monolog:handler
                    name="custom"
                    type="service"
                    id="my_handler"
                />
            </monolog:config>
        </container>

.. note::

    Quando il profilatore è abilitato, viene aggiunto un gestore per memorizzare i messaggi
    di log nel profilatore. Il profilatore usa il nome "debug", quindi il nome è riservato e
    non può essere usato nella configurazione.
