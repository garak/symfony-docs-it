.. index::
    single: Console; Logger

Uso del Logger
==============

.. versionadded:: 2.5
    :class:`Symfony\\Component\\Console\\Logger\\ConsoleLogger` è stato
    introdotto in Symfony 2.5.

Il componente Console dispone di un logger, che aderisce allo standard
`PSR-3`_. A seconda dell'impostazione sulla verbosità, i messaggi di log
saranno inviati all'istanza di :class:`Symfony\\Component\\Console\\Output\\OutputInterface`
passata come parametro al costruttore.

Il logger non ha dipendenze esterno, tranne ``php-fig/log``.
Questo è utile per applicazioni e comandi  di console che vogliono un logger
PSR-3 leggero::

    namespace Acme;

    use Psr\Log\LoggerInterface;

    class MyDependency
    {
        private $logger;

        public function __construct(LoggerInterface $logger)
        {
            $this->logger = $logger;
        }

        public function doStuff()
        {
            $this->logger->info('Mi piacciono i tagli di Tony Vairelles.');
        }
    }

Ci si può appoggiare al logger per usare questa dipendenza da dentro un comando::

    namespace Acme\Console\Command;

    use Acme\MyDependency;
    use Symfony\Component\Console\Command\Command;
    use Symfony\Component\Console\Input\InputInterface;
    use Symfony\Component\Console\Output\OutputInterface;
    use Symfony\Component\Console\Logger\ConsoleLogger;

    class MyCommand extends Command
    {
        protected function configure()
        {
            $this
                ->setName('mio:comando')
                ->setDescription(
                    'Usa una dipendenza esterna che richiede un logger PSR-3'
                )
            ;
        }

        protected function execute(InputInterface $input, OutputInterface $output)
        {
            $logger = new ConsoleLogger($output);

            $myDependency = new MyDependency($logger);
            $myDependency->doStuff();
        }
    }

La dipendenza userà l'istanza di
:class:`Symfony\\Component\\Console\\Logger\\ConsoleLogger` come logger.
I messaggi di log emessi saranno mostrati nell'output della console.

Verbosità
---------

A seconda del livello di verbosità con cui è eseguito il comando, i messaggi potrebbero
o meno essere inviati all'istanza di
:class:`Symfony\\Component\\Console\\Output\\OutputInterface`.

I logger della console si comporta come il
:doc:`gestore di console di Monolog </cookbook/logging/monolog_console>`.
L'associazione tra livello di log e verbosità può essere configurata
tramite il secondo parametro del costruttore di
:class:`Symfony\\Component\\Console\\ConsoleLogger`::

    // ...
    $verbosityLevelMap = array(
        LogLevel::NOTICE => OutputInterface::VERBOSITY_NORMAL,
        LogLevel::INFO   => OutputInterface::VERBOSITY_NORMAL,
    );
    $logger = new ConsoleLogger($output, $verbosityLevelMap);

Colori
------

Il logger mostra messaggi di log formattati con un colore che ne riflette il
livello, Questo comportamento è configurabile tramite il terzo parametro del
costruttore::

    // ...
    $formatLevelMap = array(
        LogLevel::CRITICAL => self::INFO,
        LogLevel::DEBUG    => self::ERROR,
    );
    $logger = new ConsoleLogger($output, array(), $formatLevelMap);

.. _PSR-3: http://www.php-fig.org/psr/psr-3/
