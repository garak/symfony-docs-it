.. index::
    single: Console; Eventi

Usare gli eventi
================

.. versionadded:: 2.3
    Gli eventi della console sono stati aggiunti in Symfony 2.3.

La classe ``Application``  del componente Console consente di agganciarsi
al ciclo di vita di un'applicazione console, tramite gli eventi. Invece di reinventare
la ruota, usa il componente EventDispatcher di Symfony::

    use Symfony\Component\Console\Application;
    use Symfony\Component\EventDispatcher\EventDispatcher;

    $dispatcher = new EventDispatcher();

    $application = new Application();
    $application->setDispatcher($dispatcher);
    $application->run();

L'evento ``ConsoleEvents::COMMAND``
-----------------------------------

**Scopi tipici**: fare qualcosa prima che i comandi siano eseguiti (come scrivere nel log
quale comando sta per essere eseguito) o mostrare qualcosa sull'evento che sta per
essere eseguito.

L'evento ``ConsoleEvents::COMMAND`` è distribuito prima dell'esecuzione
dei comandi. Gli ascoltatori ricevono un evento
:class:`Symfony\\Component\\Console\\Event\\ConsoleCommandEvent`::

    use Symfony\Component\Console\Event\ConsoleCommandEvent;
    use Symfony\Component\Console\ConsoleEvents;

    $dispatcher->addListener(ConsoleEvents::COMMAND, function (ConsoleCommandEvent $event) {
        // prende l'istanza di input
        $input = $event->getInput();

        // prende l'istanza di output
        $output = $event->getOutput();

        // prende il comando da eseguire
        $command = $event->getCommand();

        // scrive qualcosa sul comando
        $output->writeln(sprintf('Prima di eseguire il comando <info>%s</info>', $command->getName()));

        // prende l'applicazione
        $application = $command->getApplication();
    });

L'evento ``ConsoleEvents::TERMINATE``
-------------------------------------

**Scopi tipici**: eseguire delle azioni di pulizia dopo l'esecuzione di un
comando.

L'evento ``ConsoleEvents::TERMINATE`` è distribuito dopo l'esecuzione
di un comando. Può essere usato per qualsiasi azione che debba essere eseguita per tutti
i comandi o per pulire qualcosa iniziato in un ascoltatore ``ConsoleEvents::COMMAND``
(come inviare log, chiudere connessioni a basi dati, inviare email,
...). Un ascoltatore potrebbe anche cambiare il codice di uscita.

Gli ascoltatori ricevono un evento
:class:`Symfony\\Component\\Console\\Event\\ConsoleTerminateEvent`::

    use Symfony\Component\Console\Event\ConsoleTerminateEvent;
    use Symfony\Component\Console\ConsoleEvents;

    $dispatcher->addListener(ConsoleEvents::TERMINATE, function (ConsoleTerminateEvent $event) {
        // prende l'output
        $output = $event->getOutput();

        // prende il comando che è stato eseguito
        $command = $event->getCommand();

        // mostra qualcosa
        $output->writeln(sprintf('Dopo l\'esecuzione del comando <info>%s</info>', $command->getName()));

        // cambia il codice di uscita
        $event->setExitCode(128);
    });

.. tip::

    Questo evento viene distribuito anche se il comando lancia un'eccezione.
    Viene distribuito appena prima dell'evento ``ConsoleEvents::EXCEPTION``.
    In questo caso, il codice di uscita ricevuto è il codice dell'eccezione.

L'evento ``ConsoleEvents::EXCEPTION``
-------------------------------------

**Scopi tipici**: gestire le eccezioni sollevate durante l'esecuzione di un
comando.

Ogni volta che un comando solleva un'eccezione, viene distribuito l'evento ``ConsoleEvents::EXCEPTION``.
Un ascoltatore può avvolgere o modificare l'eccezione o fare
qualcosa di utile che l'applicazione lanci l'eccezione.

Gli ascoltatori ricevono un evento
:class:`Symfony\\Component\\Console\\Event\\ConsoleExceptionEvent`::

    use Symfony\Component\Console\Event\ConsoleExceptionEvent;
    use Symfony\Component\Console\ConsoleEvents;

    $dispatcher->addListener(ConsoleEvents::EXCEPTION, function (ConsoleExceptionEvent $event) {
        $output = $event->getOutput();

        $command = $event->getCommand();

        $output->writeln(sprintf('Oops, eccezione lanciat durante l'\esecuzione del comando <info>%s</info>', $command->getName()));

        // prende il codice di uscita (il codice dell'eccezione o il codice di uscita impostato da un evento ConsoleEvents::TERMINATE)
        $exitCode = $event->getExitCode();

        // cambia l'eccezione con un'altra
        $event->setException(new \LogicException('Eccezione', $exitCode, $event->getException()));
    });
