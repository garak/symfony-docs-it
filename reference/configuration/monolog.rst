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
                    formatter:           my_formatter
                main:
                    type:                fingerscrossed
                    action_level:        WARNING
                    buffer_size:         30
                    handler:             custom
                custom:
                    type:                service
                    id:                  my_handler

                # Prototype
                name:
                    type:                 ~ # Required
                    id:                   ~
                    priority:             0
                    level:                DEBUG
                    bubble:               true
                    path:                 %kernel.logs_dir%/%kernel.environment%.log
                    ident:                false
                    facility:             user
                    max_files:            0
                    action_level:         WARNING
                    stop_buffering:       true
                    buffer_size:          0
                    handler:              ~
                    members:              []
                    from_email:           ~
                    to_email:             ~
                    subject:              ~
                    email_prototype:
                        id:     ~ # Required (when the email_prototype is used)
                        method: ~
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
                    type="fingerscrossed"
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

    Quando il profiler è abilitato, viene aggiunto un gestore per memorizzare i messaggi
    di log nel profiler. Il profiler usa il nome "debug", quindi il nome è riservato e
    non può essere usato nella configurazione.
