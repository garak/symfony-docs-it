.. index::
    single: Console; Comandi come servizi

Definire comandi come servizi
=============================

.. versionadded:: 2.4
   Il supporto per registrare comandi nel contenitore di servizi è stato introdotto in
   Symfony 2.4.

Symfony cerca una cartella ``Command`` in ogni
bundle e registra automaticamente i comandi. Se un comando estende la classe
:class:`Symfony\\Bundle\\FrameworkBundle\\Command\\ContainerAwareCommand`,
Symfony vi inietta il contenitore.
Pur essendo facile, questo approccio ha alcuni limiti:

* Il comando deve trovarsi nella cartella ``Command``;
* Non ci sono modi per registrare in modo condizionale un servizio, in base all'ambiente
  o alla disponibilità di alcune dipendenze;
* Non si può accedere al contenitore nel metodo ``configure()`` (perché
  ``setContainer`` non è ancora stato richiamato);
* Non si può usare la stessa classe per creare molti comandi (p.e. ciascuno con
  una diversa configurazione).

Per ovviare a questi problemi, si può registrare un comando come servizio e assegnargli
il tag ``console.command``:

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        services:
            acme_hello.command.mio_comando:
                class: Acme\HelloBundle\Command\MioComando
                tags:
                    -  { name: console.command }

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <?xml version="1.0" encoding="UTF-8" ?>
        <container xmlns="http://symfony.com/schema/dic/services"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://symfony.com/schema/dic/services http://symfony.com/schema/dic/services/services-1.0.xsd">

            <services>
                <service id="acme_hello.command.mio_comando"
                    class="Acme\HelloBundle\Command\MioComando">
                    <tag name="console.command" />
                </service>
            </services>
        </container>

    .. code-block:: php

        // app/config/config.php
        $container
            ->register('acme_hello.command.mio_comando', 'Acme\HelloBundle\Command\MioComando')
            ->addTag('console.command')
        ;

Usare dipendenze e parametri per impostare valori predefiniti per le opzioni
----------------------------------------------------------------------------

Si immagini di voler fornire un valore predefinito per l'opzione ``nome``. Si potrebbe
passare uno dei seguenti come quinto parametro di ``addOption()``:

* una stringa fissa;
* un parametro del contenitore (p.e. qualcosa che provenga da parameters.yml);
* un valore calcolato da un servizio (p.e. un repository).

Se si estende ``ContainerAwareCommand``, solo la prima è possibile, perché non si può
accedere al contenitore dal metodo ``configure()``. Invece, si può iniettare
un parametro o un servizio nel costruttore. Per esempio, si supponga di
avere un servizio ``NameRepository``, da usare per ricavare il valore predefinito::

    // src/Acme/DemoBundle/Command/GreetCommand.php
    namespace Acme\DemoBundle\Command;

    use Acme\DemoBundle\Entity\NameRepository;
    use Symfony\Component\Console\Command\Command;
    use Symfony\Component\Console\Input\InputInterface;
    use Symfony\Component\Console\Input\InputOption;
    use Symfony\Component\Console\Output\OutputInterface;

    class GreetCommand extends Command
    {
        protected $nameRepository;

        public function __construct(NameRepository $nameRepository)
        {
            $this->nameRepository = $nameRepository;
            
            parent::__construct();
        }

        protected function configure()
        {
            $defaultName = $this->nameRepository->findLastOne();

            $this
                ->setName('demo:greet')
                ->setDescription('Saluta qualcuno')
                ->addOption('nome', '-n', InputOption::VALUE_REQUIRED, 'Chi vuoi salutare?', $defaultName)
            ;
        }

        protected function execute(InputInterface $input, OutputInterface $output)
        {
            $nome = $input->getOption('nome');

            $output->writeln($oame);
        }
    }

Ora, basta aggiornare i parametri della configurazione del servizio per
iniettare ``NameRepository``. Ottimo, ora si ha a disposizione un valore predefinito dinamico!

.. caution::

    Fare attenzione a non fare troppe cose in ``configure`` (come query alla base
    dati), perché il codice viene eseguito anche se si usa la console per
    eseguire un comando diverso.
