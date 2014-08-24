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
estendere AcmeDemoBundle per mandare un saluto dalla linea di comando, creare
``GreetCommand.php`` e aggiungervi il codice seguente::

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

Proprio come i controllori, i comandi possono essere dichiarati come servizi. Vedere la
:doc:`ricetta dedicata </cookbook/console/commands_as_services>`
per i dettagli.

Recuperare i servizi dal contenitore
------------------------------------

Usando :class:`Symfony\\Bundle\\FrameworkBundle\\Command\\ContainerAwareCommand`
come classe base per il comando (invece della più basica
:class:`Symfony\\Component\\Console\\Command\\Command`), si ha accesso al
contenitore di servizi. In altre parole, si ha accesso a qualsiasi servizio configurato::

    protected function execute(InputInterface $input, OutputInterface $output)
    {
        $name = $input->getArgument('name');
        $logger = $this->getContainer()->get('logger');

        $logger->info('Esecuzione del comando per '.$name);
        // ...
    }

Tuttavia, a causae degli `scope del contenitore </cookbook/service_container/scopes>`_, questo
codice non funziona per alcuni servizi. Per esempio, se si prova a prednere il servizio ``request``
o un altro servizio correlato, si otterrà il seguente errore:

.. code-block:: text

    You cannot create a service ("request") of an inactive scope ("request").

Si consideri il seguente esempio, che usa il servizio ``translator`` per
tradurre dei contenuti usando un comando di console::

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

Se si dà un'occhiata alle classi del componente Translator, si vedrà che il servizio ``request``
è necessario per ottenere il locale in cui tradurre i contenuti::

    // vendor/symfony/symfony/src/Symfony/Bundle/FrameworkBundle/Translation/Translator.php
    public function getLocale()
    {
        if (null === $this->locale && $this->container->isScopeActive('request')
            && $this->container->has('request')) {
            $this->locale = $this->container->get('request')->getLocale();
        }

        return $this->locale;
    }

Quindi, quando si usa il servizio ``translator`` dall'interno di un comando, si otterrà,
come prima, l'errore *"You cannot create a service of an inactive scope"*.
In questo caso, la soluzione è facile, basta impostare il valore del locale
prima di tradurre i contenuti::

    protected function execute(InputInterface $input, OutputInterface $output)
    {
        $name = $input->getArgument('name');
        $locale = $input->getArgument('locale');

        $translator = $this->getContainer()->get('translator');
        $translator->setLocale($locale);

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
