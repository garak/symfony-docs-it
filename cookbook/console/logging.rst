.. index::
   single: Console; Abilitare i log

Abilitare i log nei comandi da console
======================================

Il componente Console non dispone di capacità di log predefinite.
Solitamente, si esegue un comando a mano per osservarne l'esito, per cui non
occorre un log. Tuttavia, possono esserci casi in cui il log sia
necessario. Per esempio, se si eseguono automaticamente dei comandi, come
in un cron o in uno script di deploy, può essere più facile usare le capacità
di log di Symfony, piuttosto che configurare altri strumenti che memorizzino
l'output e lo processino. Questo può essere particolarmente utile se si hanno
già pronti strumenti per aggregare e analizzare i log di Symfony.

Di base, ci sono due casi di log che possono essere necessari:
 * Log manuale di alcune informazioni dal proprio comando;
 * Log di eccezioni non catturate.

Log manuale da un comando di console;
-------------------------------------

Questa parte è molto semplice. Quando si crea un comando usando il
framework, come descritto in ":doc:`/cookbook/console/console_command`", il comando
estende :class:`Symfony\\Bundle\\FrameworkBundle\\Command\\ContainerAwareCommand`.
Questo vuol dire che si può accedere facilmente al servizio predefinito di log attravaerso
il contenitore e usarlo per i log::

    // src/Acme/DemoBundle/Command/GreetCommand.php
    namespace Acme\DemoBundle\Command;

    use Symfony\Bundle\FrameworkBundle\Command\ContainerAwareCommand;
    use Symfony\Component\Console\Input\InputArgument;
    use Symfony\Component\Console\Input\InputInterface;
    use Symfony\Component\Console\Input\InputOption;
    use Symfony\Component\Console\Output\OutputInterface;
    use Psr\Log\LoggerInterface;

    class GreetCommand extends ContainerAwareCommand
    {
        // ...

        protected function execute(InputInterface $input, OutputInterface $output)
        {
            /** @var $logger LoggerInterface */
            $logger = $this->getContainer()->get('logger');

            $name = $input->getArgument('name');
            if ($name) {
                $text = 'Hello '.$name;
            } else {
                $text = 'Hello';
            }

            if ($input->getOption('yell')) {
                $text = strtoupper($text);
                $logger->warning('Yelled: '.$text);
            } else {
                $logger->info('Greeted: '.$text);
            }

            $output->writeln($text);
        }
    }

A seconda dell'ambiente in cui si esegue il comando (e a seconda della configurazione),
si dovrebbero avere delle voci di log in ``app/logs/dev.log`` o ``app/logs/prod.log``.

Abilitare i log automatici delle eccezioni
------------------------------------------

Per consentire all'applicazione di console di salvare automaticamente nei log le
eccezioni non catturate per ogni comando, si possono usare gli :doc:`eventi </components/console/events>`.

.. versionadded:: 2.3
    Gli eventi della console sono stati introdotti inSymfony 2.3.

Prima di tutto, configurare un ascoltatore per gli eventi delle eccezioni della console nel contenitore:

.. configuration-block::

    .. code-block:: yaml

        # app/config/services.yml
        services:
            kernel.listener.command_dispatch:
                class: Acme\DemoBundle\EventListener\ConsoleExceptionListener
                arguments:
                    logger: "@logger"
                tags:
                    - { name: kernel.event_listener, event: console.exception }

    .. code-block:: xml

        <!-- app/config/services.xml -->
        <?xml version="1.0" encoding="UTF-8" ?>
        <container xmlns="http://symfony.com/schema/dic/services"
                   xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
                   xsi:schemaLocation="http://symfony.com/schema/dic/services http://symfony.com/schema/dic/services/services-1.0.xsd">

            <parameters>
                <parameter key="console_exception_listener.class">Acme\DemoBundle\EventListener\ConsoleExceptionListener</parameter>
            </parameters>

            <services>
                <service id="kernel.listener.command_dispatch" class="%console_exception_listener.class%">
                    <argument type="service" id="logger"/>
                    <tag name="kernel.event_listener" event="console.exception" />
                </service>
            </services>
        </container>

    .. code-block:: php

        // app/config/services.php
        use Symfony\Component\DependencyInjection\Definition;
        use Symfony\Component\DependencyInjection\Reference;

        $container->setParameter(
            'console_exception_listener.class',
            'Acme\DemoBundle\EventListener\ConsoleExceptionListener'
        );
        $definitionConsoleExceptionListener = new Definition(
            '%console_exception_listener.class%',
            array(new Reference('logger'))
        );
        $definitionConsoleExceptionListener->addTag(
            'kernel.event_listener',
            array('event' => 'console.exception')
        );
        $container->setDefinition(
            'kernel.listener.command_dispatch',
            $definitionConsoleExceptionListener
        );

Quindi, implementare l'ascoltatore::

    // src/Acme/DemoBundle/EventListener/ConsoleExceptionListener.php
    namespace Acme\DemoBundle\EventListener;

    use Symfony\Component\Console\Event\ConsoleExceptionEvent;
    use Psr\Log\LoggerInterface;

    class ConsoleExceptionListener
    {
        private $logger;

        public function __construct(LoggerInterface $logger)
        {
            $this->logger = $logger;
        }

        public function onConsoleException(ConsoleExceptionEvent $event)
        {
            $command = $event->getCommand();
            $exception = $event->getException();

            $message = sprintf(
                '%s: %s (eccezione) nel file %s alla riga %s  durante il comando `%s`',
                get_class($exception),
                $exception->getMessage(),
                $exception->getFile(),
                $exception->getLine(),
                $command->getName()
            );

            $this->logger->error($message);
        }
    }

Nel codice precedente, quando un comando lancia un'eccezione, l'ascoltatore
riceverà un evento. Lo si può loggare, semplicemente passando il servizio di log tramite
la configurazione del contenitore. Il metodo riceve un oggetto
:class:`Symfony\\Component\\Console\\Event\\ConsoleExceptionEvent`,
che ha metodi per ottenere informazioni sull'evento e sull'eccezione.

Log di stati di uscita diversi da 0
-----------------------------------

Le capacità di log della console possono essere espanse ulteriormente, loggando
stati di uscita diversi da 0. In questo modo si potrà sapere se un comando ha avuto un errore, anche
se non ha lanciato eccezioni.

Prima di tutto, configurare un ascoltatore per gli eventi di fine console nel contenitore:

.. configuration-block::

    .. code-block:: yaml

        # app/config/services.yml
        services:
            kernel.listener.command_dispatch:
                class: Acme\DemoBundle\EventListener\ConsoleTerminateListener
                arguments:
                    logger: "@logger"
                tags:
                    - { name: kernel.event_listener, event: console.terminate }

    .. code-block:: xml

        <!-- app/config/services.xml -->
        <?xml version="1.0" encoding="UTF-8" ?>
        <container xmlns="http://symfony.com/schema/dic/services"
                   xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
                   xsi:schemaLocation="http://symfony.com/schema/dic/services http://symfony.com/schema/dic/services/services-1.0.xsd">

            <parameters>
                <parameter key="console_exception_listener.class">Acme\DemoBundle\EventListener\ConsoleExceptionListener</parameter>
            </parameters>

            <services>
                <service id="kernel.listener.command_dispatch" class="%console_exception_listener.class%">
                    <argument type="service" id="logger"/>
                    <tag name="kernel.event_listener" event="console.terminate" />
                </service>
            </services>
        </container>

    .. code-block:: php

        // app/config/services.php
        use Symfony\Component\DependencyInjection\Definition;
        use Symfony\Component\DependencyInjection\Reference;

        $container->setParameter(
            'console_exception_listener.class',
            'Acme\DemoBundle\EventListener\ConsoleExceptionListener'
        );
        $definitionConsoleExceptionListener = new Definition(
            '%console_exception_listener.class%',
            array(new Reference('logger'))
        );
        $definitionConsoleExceptionListener->addTag(
            'kernel.event_listener',
            array('event' => 'console.terminate')
        );
        $container->setDefinition(
            'kernel.listener.command_dispatch',
            $definitionConsoleExceptionListener
        );

Quindi, implementare l'ascoltatore::

    // src/Acme/DemoBundle/EventListener/ConsoleExceptionListener.php
    namespace Acme\DemoBundle\EventListener;

    use Symfony\Component\Console\Event\ConsoleTerminateEvent;
    use Psr\Log\LoggerInterface;

    class ConsoleExceptionListener
    {
        private $logger;

        public function __construct(LoggerInterface $logger)
        {
            $this->logger = $logger;
        }

        public function onConsoleTerminate(ConsoleTerminateEvent $event)
        {
            $statusCode = $event->getExitCode();
            $command = $event->getCommand();

            if ($statusCode === 0) {
                return;
            }

            if ($statusCode > 255) {
                $statusCode = 255;
                $event->setExitCode($statusCode);
            }

            $this->logger->warning(sprintf(
                'Il comando `%s` è uscito con codice di stato %d',
                $command->getName(),
                $statusCode
            ));
        }
    }
