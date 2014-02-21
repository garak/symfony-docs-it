.. index::
   single: Console; Creare comandi

Creare un comando di console
============================

La pagina Console della sezione dei componenti (:doc:`/components/console/introduction`) spiega
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

    $ app/console demo:greet Fabien

.. _cookbook-console-dic:

Registrare comandi nel contenitore di servizi
---------------------------------------------

.. versionadded:: 2.4
   IL supporto per registrare comandi nel contenitore di servizi è stato aggiunto nella
   versione 2.4.

Invece di inserire un comando nella cartella ``Command`` e farlo scoprire automaticamente
a Symfony, si possono registrare comandi nel contenitore di servizi,
usando il tag ``console.command``:

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        services:
            acme_hello.command.my_command:
                class: Acme\HelloBundle\Command\MyCommand
                tags:
                    -  { name: console.command }

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <?xml version="1.0" encoding="UTF-8" ?>
        <container xmlns="http://symfony.com/schema/dic/services"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://symfony.com/schema/dic/services http://symfony.com/schema/dic/services/services-1.0.xsd">

            <service id="acme_hello.command.my_command"
                class="Acme\HelloBundle\Command\MyCommand">
                <tag name="console.command" />
            </service>
        </container>

    .. code-block:: php

        // app/config/config.php

        $container
            ->register('acme_hello.command.my_command', 'Acme\HelloBundle\Command\MyCommand')
            ->addTag('console.command')
        ;

.. tip::

    La registrazione di un comando fornisce maggiore controllo sulla
    posizione e sui servizi inieettati. Non ci sono tuttavia
    vantaggi funzionali, quindi non occorre registrare un comando come servizio.

Recuperare servizi dal contenitore di servizi
---------------------------------------------

Usando :class:`Symfony\\Bundle\\FrameworkBundle\\Command\\ContainerAwareCommand`
come classe base per il comando (al posto della più basica
:class:`Symfony\\Component\\Console\\Command\\Command`), si ha accesso al contenitore
di servizi. In altre parole, si ha accesso a qualsiasi servizio configurato.
Per esempio, si può facilmente estendere il task per essere traducibile::

    protected function execute(InputInterface $input, OutputInterface $output)
    {
        $name = $input->getArgument('name');
        $translator = $this->getContainer()->get('translator');
        if ($name) {
            $output->writeln($translator->trans('Hello %name%!', array('%name%' => $name)));
        } else {
            $output->writeln($translator->trans('Hello!'));
        }
    }

Testare i comandi
-----------------

Quando si testano i comandi usati come parte di un framework, andrebbe usata
:class:`Symfony\\Bundle\\FrameworkBundle\\Console\\Application <Symfony\\Bundle\\FrameworkBundle\\Console\\Application>`
al posto di
:class:`Symfony\\Component\\Console\\Application <Symfony\\Component\\Console\\Application>`::

    use Symfony\Component\Console\Tester\CommandTester;
    use Symfony\Bundle\FrameworkBundle\Console\Application;
    use Acme\DemoBundle\Command\GreetCommand;

    class ListCommandTest extends \PHPUnit_Framework_TestCase
    {
        public function testExecute()
        {
            // fare un mock del Kernel o crearne uno, a seconda delle esigenze
            $application = new Application($kernel);
            $application->add(new GreetCommand());

            $command = $application->find('demo:greet');
            $commandTester = new CommandTester($command);
            $commandTester->execute(
                array(
                    'name'    => 'Fabien',
                    '--yell'  => true,
                )
            );

            $this->assertRegExp('/.../', $commandTester->getDisplay());

            // ...
        }
    }

.. versionadded:: 2.4
    A partire da Symfony 2.4, ``CommandTester`` individua automaticamente il nome
    del comando da eseguire. Non è quindi più necessario passarlo tramite la chiave
    ``command``.

.. note::

    Nel caso specifico appena visto, il parametro ``name`` e l'opzione ``--yell``
    non sono indispensabili al comando, ma sono mostrate per poter capire
    come personalizzarli quando si richiama il comando stesso.

Per poter usare il contenitore in modo completo per i test della console,
si può estendere il test da
:class:`Symfony\\Bundle\\FrameworkBundle\\Test\\WebTestCase`::

    use Symfony\Component\Console\Tester\CommandTester;
    use Symfony\Bundle\FrameworkBundle\Console\Application;
    use Symfony\Bundle\FrameworkBundle\Test\WebTestCase;
    use Acme\DemoBundle\Command\GreetCommand;

    class ListCommandTest extends WebTestCase
    {
        public function testExecute()
        {
            $kernel = $this->createKernel();
            $kernel->boot();

            $application = new Application($kernel);
            $application->add(new GreetCommand());

            $command = $application->find('demo:greet');
            $commandTester = new CommandTester($command);
            $commandTester->execute(
                array(
                    'name'    => 'Fabien',
                    '--yell'  => true,
                )
            );

            $this->assertRegExp('/.../', $commandTester->getDisplay());

            // ...
        }
    }
