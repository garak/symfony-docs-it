.. index::
   single: Log

Usare Monolog per scrivere log
==============================

Monolog_ è una libreria di log per PHP 5.3 usata da Symfony. È
ispirata dalla libreria LogBook di Python.

Utilizzo
--------

Per registrare un messaggio di log, basta prendere il servizio logger dal contenitore,
in un controllore::

    public function indexAction()
    {
        $logger = $this->get('logger');
        $logger->info('Ho preso il logger');
        $logger->error('SI è verificato un errore');

        // ...
    }

Il servizio ``logger`` ha vari metodi, per diversi livelli di log.
Vedere :class:`Symfony\\Component\\HttpKernel\\Log\\LoggerInterface` per maggiori dettagli
sui metodi disponibili.

Gestori e canali: scrivere log in posizioni diverse
---------------------------------------------------

In Monolog, ogni logger definisce un canale di log, il che consente di organizzare i messaggi
di log in diverse "categorie". Ogni canale ha una pila di gestori
per scrivere i log (i gestori possono essere condivisi).

.. tip::

    Quando si inietta il logger in un servizio, si può
    :ref:`usare un canale personalizzato <dic_tags-monolog>` per vedere facilmente
    quale parte dell'applicazione ha loggato il messaggio.

Il gestore di base è ``StreamHandler``, che scrive log in un flusso
(per impostazione definita, in ``app/logs/prod.log`` in ambiente di produzione e in
``app/logs/dev.log`` in quello di sviluppo).

Monolog dispone anche di un potente gestore per il log in ambiente di
produzione: ``FingersCrossedHandler``. Esso consente di memorizzare i
messaggi in un buffer e di loggarli solo se un messaggio raggiunge il livello
di azione (ERROR, nella configurazione fornita con la Standard
Edition), inoltrando i messaggi a un altro gestore.

Usare diversi gestori
~~~~~~~~~~~~~~~~~~~~~

Il logger usa una pila di gestori, che sono richiamati in successione. Ciò
consente di loggare facilmente i messaggi in molti modi.

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        monolog:
            handlers:
                applog:
                    type: stream
                    path: /var/log/symfony.log
                    level: error
                main:
                    type: fingers_crossed
                    action_level: warning
                    handler: file
                file:
                    type: stream
                    level: debug
                syslog:
                    type: syslog
                    level: error

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <?xml version="1.0" encoding="UTF-8" ?>
        <container xmlns="http://symfony.com/schema/dic/services"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xmlns:monolog="http://symfony.com/schema/dic/monolog"
            xsi:schemaLocation="http://symfony.com/schema/dic/services
                http://symfony.com/schema/dic/services/services-1.0.xsd
                http://symfony.com/schema/dic/monolog
                http://symfony.com/schema/dic/monolog/monolog-1.0.xsd">

            <monolog:config>
                <monolog:handler
                    name="applog"
                    type="stream"
                    path="/var/log/symfony.log"
                    level="error"
                />
                <monolog:handler
                    name="main"
                    type="fingers_crossed"
                    action-level="warning"
                    handler="file"
                />
                <monolog:handler
                    name="file"
                    type="stream"
                    level="debug"
                />
                <monolog:handler
                    name="syslog"
                    type="syslog"
                    level="error"
                />
            </monolog:config>
        </container>

    .. code-block:: php

        // app/config/config.php
        $container->loadFromExtension('monolog', array(
            'handlers' => array(
                'applog' => array(
                    'type'  => 'stream',
                    'path'  => '/var/log/symfony.log',
                    'level' => 'error',
                ),
                'main' => array(
                    'type'         => 'fingers_crossed',
                    'action_level' => 'warning',
                    'handler'      => 'file',
                ),
                'file' => array(
                    'type'  => 'stream',
                    'level' => 'debug',
                ),
                'syslog' => array(
                    'type'  => 'syslog',
                    'level' => 'error',
                ),
            ),
        ));

La configurazione appena vista definisce una pila di gestori, che saranno richiamati
nell'ordine in cui sono stati definiti.

.. tip::

    Il gestore chiamato "file" non sarà incluso nella pila, perché è usato
    come gestore annidato del gestore ``fingers_crossed``.

.. note::

    Se si vuole cambiare la configurazione di MonologBundle con un altro file di
    configurazione, occorre ridefinire l'intera pila. Non si possono fondere,
    perché l'ordine conta e una fusione non consente di controllare
    l'ordine.

Cambiare il formattatore
~~~~~~~~~~~~~~~~~~~~~~~~

Il gestore usa un ``Formatter`` per formattare un record, prima di loggarlo.
Tutti i gestori di Monolog usano, per impostazione predefinita, un'istanza di
``Monolog\Formatter\LineFormatter``, ma la si può sostituire facilmente.
Il proprio formattatore deve implementare
``Monolog\Formatter\FormatterInterface``.

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        services:
            my_formatter:
                class: Monolog\Formatter\JsonFormatter
        monolog:
            handlers:
                file:
                    type: stream
                    level: debug
                    formatter: my_formatter

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <?xml version="1.0" encoding="UTF-8" ?>
        <container xmlns="http://symfony.com/schema/dic/services"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xmlns:monolog="http://symfony.com/schema/dic/monolog"
            xsi:schemaLocation="http://symfony.com/schema/dic/services
                http://symfony.com/schema/dic/services/services-1.0.xsd
                http://symfony.com/schema/dic/monolog
                http://symfony.com/schema/dic/monolog/monolog-1.0.xsd">

            <services>
                <service id="my_formatter" class="Monolog\Formatter\JsonFormatter" />
            </services>

            <monolog:config>
                <monolog:handler
                    name="file"
                    type="stream"
                    level="debug"
                    formatter="my_formatter"
                />
            </monolog:config>
        </container>

    .. code-block:: php

        // app/config/config.php
        $container
            ->register('my_formatter', 'Monolog\Formatter\JsonFormatter');

        $container->loadFromExtension('monolog', array(
            'handlers' => array(
                'file' => array(
                    'type'      => 'stream',
                    'level'     => 'debug',
                    'formatter' => 'my_formatter',
                ),
            ),
        ));

Ruotare i file di log
---------------------

Nel tempo, i file di log possono crescere molto, sia durante lo sviluppo sia
in produzione. Una soluzione frequente è quella di usare uno strumento come `logrotate`_,
un comando Linux per ruotare i file di log, prima che diventino troppo grossi.

Un'altra opzione possibile e di lasciare la rotazione a Monolog, usando il gestore
``rotating_file``. Questo gestore crea un nuovo file di log ogni giorno e
può anche rimuovere i vecchi file automaticamente. Per usarlo, basta impostare l'opzione ``type``
del gestore su ``rotating_file``:

.. configuration-block::

    .. code-block:: yaml

        # app/config/config_dev.yml
        monolog:
            handlers:
                main:
                    type:  rotating_file
                    path:  %kernel.logs_dir%/%kernel.environment%.log
                    level: debug
                    # numero massimo di file di log da mantenere
                    # predefinito a zero, che vuol dire file infiniti
                    max_files: 10

    .. code-block:: xml

        <!-- app/config/config_dev.xml -->
        <?xml version="1.0" charset="UTF-8" ?>
        <container xmlns=''http://symfony.com/schema/dic/services"
            xmlns:monolog="http://symfony.com/schema/dic/monolog">

            <monolog:config>
                <monolog:handler name="main"
                    type="rotating_file"
                    path="%kernel.logs_dir%/%kernel.environment%.log"
                    level="debug"
                    <!-- numero massimo di file di log da mantenere
                         predefinito a zero, che vuol dire file infiniti -->
                    max_files="10"
                />
            </monolog:config>
        </container>

    .. code-block:: php

        // app/config/config_dev.php
        $container->loadFromExtension('monolog', array(
            'handlers' => array(
                'main' => array(
                    'type'  => 'rotating_file',
                    'path'  => '%kernel.logs_dir%/%kernel.environment%.log',
                    'level' => 'debug',
                    // numero massimo di file di log da mantenere
                    // predefinito a zero, che vuol dire file infiniti
                    'max_files' => 10,
                ),
            ),
        ));

Aggiungere dati extra nei messaggi di log
-----------------------------------------

Monolog consente di processare il record prima di loggarlo, per aggiungere
alcuni dati extra. Un processore può essere applicato all'intera pila dei
gestori oppure solo a un gestore specifico.

Un processore è semplicemente una funzione che riceve il record come primo parametro.
I processori sono configurati con il tag ``monolog.processor`` del DIC. Vedere il
:ref:`riferimento <dic_tags-monolog-processor>`.

Aggiungere un token di sessione/richiesta
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

A volte è difficile dire quali voci nel log appartengano a quale sessione e/o
richiesta. L'esempio seguente aggiunge un token univoco per ogni richiesta,
usando un processore.

.. code-block:: php

    namespace Acme\MyBundle;

    use Symfony\Component\HttpFoundation\Session\Session;

    class SessionRequestProcessor
    {
        private $session;
        private $token;

        public function __construct(Session $session)
        {
            $this->session = $session;
        }

        public function processRecord(array $record)
        {
            if (null === $this->token) {
                try {
                    $this->token = substr($this->session->getId(), 0, 8);
                } catch (\RuntimeException $e) {
                    $this->token = '????????';
                }
                $this->token .= '-' . substr(uniqid(), -8);
            }
            $record['extra']['token'] = $this->token;

            return $record;
        }
    }

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        services:
            monolog.formatter.session_request:
                class: Monolog\Formatter\LineFormatter
                arguments:
                    - "[%%datetime%%] [%%extra.token%%] %%channel%%.%%level_name%%: %%message%%\n"

            monolog.processor.session_request:
                class: Acme\MyBundle\SessionRequestProcessor
                arguments:  ["@session"]
                tags:
                    - { name: monolog.processor, method: processRecord }

        monolog:
            handlers:
                main:
                    type: stream
                    path: "%kernel.logs_dir%/%kernel.environment%.log"
                    level: debug
                    formatter: monolog.formatter.session_request

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <?xml version="1.0" encoding="UTF-8" ?>
        <container xmlns="http://symfony.com/schema/dic/services"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xmlns:monolog="http://symfony.com/schema/dic/monolog"
            xsi:schemaLocation="http://symfony.com/schema/dic/services
                http://symfony.com/schema/dic/services/services-1.0.xsd
                http://symfony.com/schema/dic/monolog
                http://symfony.com/schema/dic/monolog/monolog-1.0.xsd">

            <services>
                <service id="monolog.formatter.session_request"
                    class="Monolog\Formatter\LineFormatter">

                    <argument>[%%datetime%%] [%%extra.token%%] %%channel%%.%%level_name%%: %%message%%&#xA;</argument>
                </service>

                <service id="monolog.processor.session_request"
                    class="Acme\MyBundle\SessionRequestProcessor">

                    <argument type="service" id="session" />
                    <tag name="monolog.processor" method="processRecord" />
                </service>
            </services>

            <monolog:config>
                <monolog:handler
                    name="main"
                    type="stream"
                    path="%kernel.logs_dir%/%kernel.environment%.log"
                    level="debug"
                    formatter="monolog.formatter.session_request"
                />
            </monolog:config>
        </container>

    .. code-block:: php

        // app/config/config.php
        $container
            ->register(
                'monolog.formatter.session_request',
                'Monolog\Formatter\LineFormatter'
            )
            ->addArgument('[%%datetime%%] [%%extra.token%%] %%channel%%.%%level_name%%: %%message%%\n');

        $container
            ->register(
                'monolog.processor.session_request',
                'Acme\MyBundle\SessionRequestProcessor'
            )
            ->addArgument(new Reference('session'))
            ->addTag('monolog.processor', array('method' => 'processRecord'));

        $container->loadFromExtension('monolog', array(
            'handlers' => array(
                'main' => array(
                    'type'      => 'stream',
                    'path'      => '%kernel.logs_dir%/%kernel.environment%.log',
                    'level'     => 'debug',
                    'formatter' => 'monolog.formatter.session_request',
                ),
            ),
        ));

.. note::

    Se si usano molti gestori, si può anche registrare il processore a livello
    di gestore o a livello di canale, invece che globalmente
    (vedere le sezioni seguenti).

Registrare processori per gestore
---------------------------------

Si può registrare processori per gestore, usando l'opzione ``handler`` del
tag ``monolog.processor``:

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        services:
            monolog.processor.session_request:
                class: Acme\MyBundle\SessionRequestProcessor
                arguments:  ["@session"]
                tags:
                    - { name: monolog.processor, method: processRecord, handler: main }

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <?xml version="1.0" encoding="UTF-8" ?>
        <container xmlns="http://symfony.com/schema/dic/services"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xmlns:monolog="http://symfony.com/schema/dic/monolog"
            xsi:schemaLocation="http://symfony.com/schema/dic/services
                http://symfony.com/schema/dic/services/services-1.0.xsd
                http://symfony.com/schema/dic/monolog
                http://symfony.com/schema/dic/monolog/monolog-1.0.xsd">

            <services>
                <service id="monolog.processor.session_request"
                    class="Acme\MyBundle\SessionRequestProcessor">

                    <argument type="service" id="session" />
                    <tag name="monolog.processor" method="processRecord" handler="main" />
                </service>
            </services>
        </container>

    .. code-block:: php

        // app/config/config.php
        $container
            ->register(
                'monolog.processor.session_request',
                'Acme\MyBundle\SessionRequestProcessor'
            )
            ->addArgument(new Reference('session'))
            ->addTag('monolog.processor', array('method' => 'processRecord', 'handler' => 'main'));

Registrare processori per canale
--------------------------------

Si può registrare processori per canale, usando l'opzione ``channel`` del
tag ``monolog.processor``:

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        services:
            monolog.processor.session_request:
                class: Acme\MyBundle\SessionRequestProcessor
                arguments:  ["@session"]
                tags:
                    - { name: monolog.processor, method: processRecord, channel: main }

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <?xml version="1.0" encoding="UTF-8" ?>
        <container xmlns="http://symfony.com/schema/dic/services"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xmlns:monolog="http://symfony.com/schema/dic/monolog"
            xsi:schemaLocation="http://symfony.com/schema/dic/services
                http://symfony.com/schema/dic/services/services-1.0.xsd
                http://symfony.com/schema/dic/monolog
                http://symfony.com/schema/dic/monolog/monolog-1.0.xsd">

            <services>
                <service id="monolog.processor.session_request"
                    class="Acme\MyBundle\SessionRequestProcessor">

                    <argument type="service" id="session" />
                    <tag name="monolog.processor" method="processRecord" channel="main" />
                </service>
            </services>
        </container>

    .. code-block:: php

        // app/config/config.php
        $container
            ->register(
                'monolog.processor.session_request',
                'Acme\MyBundle\SessionRequestProcessor'
            )
            ->addArgument(new Reference('session'))
            ->addTag('monolog.processor', array('method' => 'processRecord', 'channel' => 'main'));

.. _`Monolog`: https://github.com/Seldaek/monolog
.. _`logrotate`: https://fedorahosted.org/logrotate/
