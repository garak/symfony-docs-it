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
    use Symfony\Component\HttpKernel\Log\LoggerInterface;

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
                $logger->warn('Yelled: '.$text);
            }
            else {
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
eccezioni non catturate per ogni comando, servirà un po' di lavoro in più.

Primo, creare una nuova sotto-classe di :class:`Symfony\\Bundle\\FrameworkBundle\\Console\\Application`
e ridefinire il suo metodo :method:`Symfony\\Bundle\\FrameworkBundle\\Console\\Application::run`,
in cui sono gestite le eccezioni:

.. warning::

    A causa della natura della classe :class:`Symfony\\Component\\Console\\Application`,
    gran parte del metodo :method:`run<Symfony\\Bundle\\FrameworkBundle\\Console\\Application::run>`
    va duplicata, compresa la proprietà privata ``originalAutoExit``, che va
    reimplementata. Questo serve come esempio di ciò che si *potrebbe* fare nel proprio
    codice, ma c'è un alto rischio che qualcosa si rompa negli aggiornamenti
    futuri di Symfony.

.. code-block:: php

    // src/Acme/DemoBundle/Console/Application.php
    namespace Acme\DemoBundle\Console;

    use Symfony\Bundle\FrameworkBundle\Console\Application as BaseApplication;
    use Symfony\Component\Console\Input\InputInterface;
    use Symfony\Component\Console\Output\OutputInterface;
    use Symfony\Component\Console\Output\ConsoleOutputInterface;
    use Symfony\Component\HttpKernel\Log\LoggerInterface;
    use Symfony\Component\HttpKernel\KernelInterface;
    use Symfony\Component\Console\Output\ConsoleOutput;
    use Symfony\Component\Console\Input\ArgvInput;

    class Application extends BaseApplication
    {
        private $originalAutoExit;

        public function __construct(KernelInterface $kernel)
        {
            parent::__construct($kernel);
            $this->originalAutoExit = true;
        }

        /**
         * Runs the current application.
         *
         * @param InputInterface  $input  An Input instance
         * @param OutputInterface $output An Output instance
         *
         * @return integer 0 if everything went fine, or an error code
         *
         * @throws \Exception When doRun returns Exception
         *
         * @api
         */
        public function run(InputInterface $input = null, OutputInterface $output = null)
        {
            // make the parent method throw exceptions, so you can log it
            $this->setCatchExceptions(false);

            if (null === $input) {
                $input = new ArgvInput();
            }

            if (null === $output) {
                $output = new ConsoleOutput();
            }

            try {
                $statusCode = parent::run($input, $output);
            } catch (\Exception $e) {

                /** @var $logger LoggerInterface */
                $logger = $this->getKernel()->getContainer()->get('logger');

                $message = sprintf(
                    '%s: %s (uncaught exception) at %s line %s while running console command `%s`',
                    get_class($e),
                    $e->getMessage(),
                    $e->getFile(),
                    $e->getLine(),
                    $this->getCommandName($input)
                );
                $logger->crit($message);

                if ($output instanceof ConsoleOutputInterface) {
                    $this->renderException($e, $output->getErrorOutput());
                } else {
                    $this->renderException($e, $output);
                }
                $statusCode = $e->getCode();

                $statusCode = is_numeric($statusCode) && $statusCode ? $statusCode : 1;
            }

            if ($this->originalAutoExit) {
                if ($statusCode > 255) {
                    $statusCode = 255;
                }
                // @codeCoverageIgnoreStart
                exit($statusCode);
                // @codeCoverageIgnoreEnd
            }

            return $statusCode;
        }

        public function setAutoExit($bool)
        {
            // parent property is private, so we need to intercept it in a setter
            $this->originalAutoExit = (Boolean) $bool;
            parent::setAutoExit($bool);
        }

    }

Nel codice appena visto, si disabilita la cattura delle eccezioni, in modo che il metodo
``run`` del genitore lanci tutte le eccezioni. Quando un'eccezione viene catturata, la si
memorizza nel log accedendo al servizio ``logger`` del contenitore e quindi si gestisce
il resto della logica nello stesso modo del metodo genitore ``run``
(nello specifico, poiché il metodo genitore :method:`run<Symfony\\Bundle\\FrameworkBundle\\Console\\Application::run>`
non gestirà la resa delle eccezioni e il codice di stato quando
``catchExceptions`` è ``false``, occorre farlo nel metodo
ridefinito).

Per far sì che la classe estesa Application funzioni correttamente in modalità console,
serve un piccolo trucco per intercettare il setter ``autoExit`` e memorizzare
l'impostazione in un'altra proprietà, perché quella del genitore è privata.

Ora, per poter usare la classe ``Application`` estesa, occorre modificare
lo script ``app/console`` per usare la nuova classe al posto di quella predefinita::

    // app/console

    // ...
    // sostituire la riga seguente:
    // use Symfony\Bundle\FrameworkBundle\Console\Application;
    use Acme\DemoBundle\Console\Application;

    // ...

Ecco fatto! Grazie all'auto-caricamento, la nuova classe sarà usata al posto di
quella originaria.

Log degli sati di uscita diversi da 0
-------------------------------------

Le capacità di log della console possono essere estese ulteriormente, per mettere in log
stati di uscita diversi da 0. In questo modo, si saprà se un comando abbia avuto errori,
anche quando nessuna eccezione sia stata lanciata.

Per farlo, si deve modificare il metodo ``run()`` della classe
``Application`` estesa, in questo modo::

    public function run(InputInterface $input = null, OutputInterface $output = null)
    {
        // fare in modo che il metodo genitore lanci eccezioni, in modo da poterle mettere in log
        $this->setCatchExceptions(false);

        // memorizzare il valore di autoExit prima di cambiarlo, servirà più avanti
        $autoExit = $this->originalAutoExit;
        $this->setAutoExit(false);

        // ...

        if ($autoExit) {
            if ($statusCode > 255) {
                $statusCode = 255;
            }

            // log di stati di uscita diversi da 0, insieme al nome del comando
            if ($statusCode !== 0) {
                /** @var $logger LoggerInterface */
                $logger = $this->getKernel()->getContainer()->get('logger');
                $logger->warn(sprintf('Command `%s` exited with status code %d', $this->getCommandName($input), $statusCode));
            }

            // @codeCoverageIgnoreStart
            exit($statusCode);
            // @codeCoverageIgnoreEnd
        }

        return $statusCode;
    }
