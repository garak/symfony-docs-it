.. index::
    single: Console; Cambiare comando predefinito

Cambiare comando predefinito
============================

.. versionadded:: 2.5
    Il metodo :method:`Symfony\\Component\\Console\\Application::setDefaultCommand`
    è stato introdotto in Symfony 2.5.

eseguirà sempre ``ListCommand`` se non si passa un nome di comando. Per cambiare il comando
predefinito, basta passare il nome del comando che si vuole eseguire
al metodo ``setDefaultCommand``::

    namespace Acme\Console\Command;

    use Symfony\Component\Console\Command\Command;
    use Symfony\Component\Console\Input\InputInterface;
    use Symfony\Component\Console\Output\OutputInterface;

    class HelloWorldCommand extends Command
    {
        protected function configure()
        {
            $this->setName('ciao:mondo')
                ->setDescription('Mostra \'Ciao mondo\'');
        }

        protected function execute(InputInterface $input, OutputInterface $output)
        {
            $output->writeln('Ciao mondo');
        }
    }

Eseguire l'applicazione e cambiare il comando predefinito::

    // application.php

    use Acme\Console\Command\HelloWorldCommand;
    use Symfony\Component\Console\Application;

    $command = new HelloWorldCommand();
    $application = new Application();
    $application->add($command);
    $application->setDefaultCommand($command->getName());
    $application->run();

Verificare il nuovo comando predefinito, eseguendo:

.. code-block:: bash

    $ php application.php

Mostrerà il seguente:

.. code-block:: text

    Ciao Fabien

.. tip::

    Questa caratteristica ha una limitazione: non la si può usare con i parametri dei comandi.

Saperne di più
--------------

* :doc:`/components/console/single_command_tool`
