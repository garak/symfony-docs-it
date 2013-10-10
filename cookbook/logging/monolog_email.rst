.. index::
   single: Log; Inviare errori per email

Come configurare Monolog con errori per email
=============================================

Monolog_ può essere configurato per inviare un'email quando accade un errore in
un'applicazione. La configurazione per farlo richiede alcuni gestori annidati,
per evitare di ricevere troppe email. Questa configurazione appare complicata a
prima vista, ma ogni gestore è abbastanza semplice, se visto
singolarmente.

.. configuration-block::

    .. code-block:: yaml

        # app/config/config_prod.yml
        monolog:
            handlers:
                mail:
                    type:         fingers_crossed
                    action_level: critical
                    handler:      buffered
                buffered:
                    type:    buffer
                    handler: swift
                swift:
                    type:       swift_mailer
                    from_email: error@example.com
                    to_email:   error@example.com
                    subject:    Si è verificato un errore!
                    level:      debug

    .. code-block:: xml

        <!-- app/config/config_prod.xml -->
        <container xmlns="http://symfony.com/schema/dic/services"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xmlns:monolog="http://symfony.com/schema/dic/monolog"
            xsi:schemaLocation="http://symfony.com/schema/dic/services http://symfony.com/schema/dic/services/services-1.0.xsd
                                http://symfony.com/schema/dic/monolog http://symfony.com/schema/dic/monolog/monolog-1.0.xsd">

            <monolog:config>
                <monolog:handler
                    name="mail"
                    type="fingers_crossed"
                    action-level="critical"
                    handler="buffered"
                />
                <monolog:handler
                    name="buffered"
                    type="buffer"
                    handler="swift"
                />
                <monolog:handler
                    name="swift"
                    from-email="error@example.com"
                    to-email="error@example.com"
                    subject="Si è verificato un errore!"
                    level="debug"
                />
            </monolog:config>
        </container>

    .. code-block:: php

        // app/config/config_prod.php
        $container->loadFromExtension('monolog', array(
            'handlers' => array(
                'mail' => array(
                    'type'         => 'fingers_crossed',
                    'action_level' => 'critical',
                    'handler'      => 'buffered',
                ),
                'buffered' => array(
                    'type'    => 'buffer',
                    'handler' => 'swift',
                ),
                'swift' => array(
                    'type'       => 'swift_mailer',
                    'from_email' => 'error@example.com',
                    'to_email'   => 'error@example.com',
                    'subject'    => 'Si è verificato un errore!',
                    'level'      => 'debug',
                ),
            ),
        ));

Il gestore ``mail`` è un gestore ``fingers_crossed``, che vuol dire che viene
evocato solo quando si raggiunge il livello di azione, in questo caso ``critical``.
Esso logga ogni cosa, inclusi i messaggi sotto il livello di azione. Il livello
``critical`` viene raggiunto solo per codici di errore HTTP 5xx. L'impostazione
``handler`` vuol dire che l'output è quindi passato nel gestore ``buffered``.

.. tip::

    Se si vuole che siano inviati per email sia gli errori 400 che i 500, impostare
    ``action_level`` a ``error``, invece che a ``critical``.

Il gestore ``buffered`` mantiene tutti i messaggi per una richiesta e quindi li passa
al gestore annidato in un colpo. Se non si usa questo gestore, ogni messaggio sarà
inviato separatamente. Viene quindi passato al gestore ``swift``. Questo gestore è
quello che si occupa effettivamente dell'invio della email con gli errori. Le
sue impostazioni sono semplici: gli indirizzi di mittente e destinatario e
l'oggetto.

Si possono combinare questi gestori con altri gestori, in modo che gli errori siano
comunque loggati sul server, oltre che inviati per email:

.. configuration-block::

    .. code-block:: yaml

        # app/config/config_prod.yml
        monolog:
            handlers:
                main:
                    type:         fingers_crossed
                    action_level: critical
                    handler:      grouped
                grouped:
                    type:    group
                    members: [streamed, buffered]
                streamed:
                    type:  stream
                    path:  "%kernel.logs_dir%/%kernel.environment%.log"
                    level: debug
                buffered:
                    type:    buffer
                    handler: swift
                swift:
                    type:       swift_mailer
                    from_email: error@example.com
                    to_email:   error@example.com
                    subject:    Si è verificato un errore!
                    level:      debug

    .. code-block:: xml

        <!-- app/config/config_prod.xml -->
        <container xmlns="http://symfony.com/schema/dic/services"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xmlns:monolog="http://symfony.com/schema/dic/monolog"
            xsi:schemaLocation="http://symfony.com/schema/dic/services http://symfony.com/schema/dic/services/services-1.0.xsd
                                http://symfony.com/schema/dic/monolog http://symfony.com/schema/dic/monolog/monolog-1.0.xsd">

            <monolog:config>
                <monolog:handler
                    name="main"
                    type="fingers_crossed"
                    action_level="critical"
                    handler="grouped"
                />
                <monolog:handler
                    name="grouped"
                    type="group"
                >
                    <member type="stream"/>
                    <member type="buffered"/>
                </monolog:handler>
                <monolog:handler
                    name="stream"
                    path="%kernel.logs_dir%/%kernel.environment%.log"
                    level="debug"
                />
                <monolog:handler
                    name="buffered"
                    type="buffer"
                    handler="swift"
                />
                <monolog:handler
                    name="swift"
                    from-email="error@example.com"
                    to-email="error@example.com"
                    subject="Si è verificato un errore!"
                    level="debug"
                />
            </monolog:config>
        </container>

    .. code-block:: php

        // app/config/config_prod.php
        $container->loadFromExtension('monolog', array(
            'handlers' => array(
                'main' => array(
                    'type'         => 'fingers_crossed',
                    'action_level' => 'critical',
                    'handler'      => 'grouped',
                ),
                'grouped' => array(
                    'type'    => 'group',
                    'members' => array('streamed', 'buffered'),
                ),
                'streamed'  => array(
                    'type'  => 'stream',
                    'path'  => '%kernel.logs_dir%/%kernel.environment%.log',
                    'level' => 'debug',
                ),
                'buffered'    => array(
                    'type'    => 'buffer',
                    'handler' => 'swift',
                ),
                'swift' => array(
                    'type'       => 'swift_mailer',
                    'from_email' => 'error@example.com',
                    'to_email'   => 'error@example.com',
                    'subject'    => 'Si è verificato un errore!',
                    'level'      => 'debug',
                ),
            ),
        ));

Qui è stato usato il gestore ``group``, per inviare i messaggi ai due membri del gruppo,
il gestore ``buffered`` e il gestore ``stream``. I messaggi saranno ora sia
scritti sul log che inviati per email.

.. _Monolog: https://github.com/Seldaek/monolog
