.. index::
   single: Console; Creare comandi

Come creare un comando di console
=================================

La pagina Console della sezione dei componenti (:doc:`/components/console`) spiega
come creare un comando di console. Questa ricetta spiega invece le differenze
nella creazione di comandi di console con il framework Symfony2.

Registrare comandi automaticamente
----------------------------------

Per rendere disponibili automaticamente i comandi in Symfony2, creare una cartella
``Command`` nel proprio bundle e creare un file php, con suffisso
``Command.php``, per ciascun comando che si vuole fornire. Per esempio, se si vuole
estendere ``AcmeDemoBundle`` (disponibile nella Standard Edition di Symfony),
per mandare un saluto dalla linea di comando, creare ``GreetCommand.php`` e
aggiungervi il codice seguente::

    // src/Acme/DemoBundle/Command/GreetCommand.php
    namespace Acme\DemoBundle\Command;

    use Symfony\Bundle\FrameworkBundle\Command\ContainerAwareCommand;
    use Symfony\Component\Console\Input\InputArgument;
    use Symfony\Component\Console\Input\InputInterface;
    use Symfony\Component\Console\Input\InputOption;
    use Symfony\Component\Console\Output\OutputInterface;

    class GreetCommand extends ContainerAwareCommand
    {
        protected function configure()
        {
            $this
                ->setName('demo:greet')
                ->setDescription('Saluta qualcuno')
                ->addArgument('name', InputArgument::OPTIONAL, 'Chi vuoi salutare?')
                ->addOption('yell', null, InputOption::VALUE_NONE, 'Se impostato, urlerà in lettere maiuscole')
            ;
        }

        protected function execute(InputInterface $input, OutputInterface $output)
        {
            $name = $input->getArgument('name');
            if ($name) {
                $text = 'Ciao '.$name;
            } else {
                $text = 'Ciao';
            }

            if ($input->getOption('yell')) {
                $text = strtoupper($text);
            }

            $output->writeln($text);
        }
    }

Ora il comando sarà automaticamente disponibile:

.. code-block:: bash

    app/console demo:greet Fabien

Testare i comandi
-----------------

Quando si testano i comandi usati come parte di un framework, andrebbe usata :class:`Symfony\\Bundle\\FrameworkBundle\\Console\\Application`
al posto di :class:`Symfony\\Component\\Console\\Application`::

    use Symfony\Component\Console\Tester\CommandTester;
    use Symfony\Bundle\FrameworkBundle\Console\Application;
    use Acme\DemoBundle\Command\GreetCommand;

    class ListCommandTest extends \PHPUnit_Framework_TestCase
    {
        public function testExecute()
        {
            // fare un mock del Kernel o crearme uno, a seconda delle esigenze
            $application = new Application($kernel);
            $application->add(new GreetCommand());

            $command = $application->find('demo:greet');
            $commandTester = new CommandTester($command);
            $commandTester->execute(array('command' => $command->getName()));

            $this->assertRegExp('/.../', $commandTester->getDisplay());

            // ...
        }
    }

Ottenere servizi dal contenitore di servizi
-------------------------------------------

Usando :class:`Symfony\\Bundle\\FrameworkBundle\\Command\\ContainerAwareCommand` 
come classe base per il comando (al posto della più basica
:class:`Symfony\\Component\\Console\\Command\\Command`), si ha accesso al contenitore
di servizi. In altre parole, si ha accesso a ogni servizio configurato.
Per esempio, si può estendere facilmente il task per essere traducibile::

    protected function execute(InputInterface $input, OutputInterface $output)
    {
        $name = $input->getArgument('name');
        $translator = $this->getContainer()->get('translator');
        if ($name) {
            $output->writeln($translator->trans('Ciao %name%!', array('%name%' => $name)));
        } else {
            $output->writeln($translator->trans('Ciao!'));
        }
    }