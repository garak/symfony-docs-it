.. index::
    single: Console; CLI
    single: Componenti; Console
    
Il componente Console
=====================

    Il componente Console semplifica la creazione di eleganti e testabili comandi
    da terminale.

Symfony2 viene distribuito con un componente Console che permette di creare
comandi da terminale. I comandi da terminale possono essere utilizzati per qualsiasi
lavoro ripetivo come i lavori di cron, le importazioni o lavori batch.

Installazione
-------------

Il componente può essere installato in diversi modi:

* Utilizzando il repository Git ufficiale (https://github.com/symfony/Console);
* Installandolo :doc:`via Composer</components/using_components>` (``symfony/console`` in `Packagist`_).

.. note::

    Windows non supporta i colori ANSI in modo predefinito, quindi il componente Console individua e
    disabilita i colori quando Windows non dà supporto. Tuttavia, se Windows non è
    configurato con un driver ANSI e i propri comandi di console invocano altri scipt che
    emetttono sequenze di colori ANSI, saranno mostrati come sequenze di caratteri grezzi.

    Per abilitare il supporto ai colori ANSI su Windows, si può installare `ANSICON`_.

Creazione di comandi di base
----------------------------

Per creare un comando che porga il saluto dal terminale, creare il file  ``SalutaCommand.php``,
contenente il seguente codice::

    namespace Acme\DemoBundle\Command;

    use Symfony\Component\Console\Command\Command;
    use Symfony\Component\Console\Input\InputArgument;
    use Symfony\Component\Console\Input\InputInterface;
    use Symfony\Component\Console\Input\InputOption;
    use Symfony\Component\Console\Output\OutputInterface;

    class SalutaCommand extends Command
    {
        protected function configure()
        {
            $this
                ->setName('demo:saluta')
                ->setDescription('Saluta qualcuno')
                ->addArgument(
                    'nome',
                    InputArgument::OPTIONAL,
                    'Chi vuoi salutare?'
                )
                ->addOption(
                   'urla',
                   null,
                   InputOption::VALUE_NONE,
                   'Se impostato, il saluto verrà urlato con caratteri maiuscoli'
                )
            ;
        }

        protected function execute(InputInterface $input, OutputInterface $output)
        {
            $nome = $input->getArgument('nome');
            if ($nome) {
                $testo = 'Ciao '.$nome;
            } else {
                $testo = 'Ciao';
            }

            if ($input->getOption('urla')) {
                $testo = strtoupper($testo);
            }

            $output->writeln($testo);
        }
    }

Occorre anche creare il file da eseguire in linea di comando, che crea
una ``Application`` e vi aggiunge comandi::

    #!/usr/bin/env php
    # app/console
    <?php 

    use Acme\DemoBundle\Command\GreetCommand;
    use Symfony\Component\Console\Application;

    $application = new Application();
    $application->add(new GreetCommand);
    $application->run();

È possibile provare il programma nel modo seguente

.. code-block:: bash

    app/console demo:saluta Fabien

Il comando scriverà, nel terminale, quello che segue:

.. code-block:: text

    Ciao Fabien

È anche possibile usare l'opzione ``--urla`` per stampare il saluto in lettere maiuscole:

.. code-block:: bash

    app/console demo:saluta Fabien --urla

Il cui risultato sarà::

    CIAO FABIEN

.. _components-console-coloring:

Colorare l'output
~~~~~~~~~~~~~~~~~

È possibile inserire il testo da stampare, all'interno di speciali tag per colorare 
l'output. Ad esempio::

    // testo verde
    $output->writeln('<info>pippo</info>');

    // testo giallo
    $output->writeln('<comment>pippo</comment>');

    // testo nero su sfondo ciano
    $output->writeln('<question>pippo</question>');

    // testo nero su sfondo rosso
    $output->writeln('<error>pippo</error>');

Si può definire un proprio stile, usando la classe
:class:`Symfony\\Component\\Console\\Formatter\\OutputFormatterStyle`::

    $style = new OutputFormatterStyle('red', 'yellow', array('bold', 'blink'));
    $output->getFormatter()->setStyle('fire', $style);
    $output->writeln('<fire>foo</fire>');

I colori di sfondo e di testo disponibili sono: ``black``, ``red``, ``green``,
``yellow``, ``blue``, ``magenta``, ``cyan`` e ``white``.

Le opzioni disponibili sono: ``bold``, ``underscore``, ``blink``, ``reverse`` e ``conceal``.

Si possono anche impostare colori e opzioni dentro il tag::

    // testo verde
    $output->writeln('<fg=green>pippo</fg=green>');

    // testo nero su sfondo ciano
    $output->writeln('<fg=black;bg=cyan>pippo</fg=black;bg=cyan>');

    // testo grassetto su sfondo giallo
    $output->writeln('<bg=yellow;options=bold>pippo</bg=yellow;options=bold>');


Utilizzo degli argomenti nei comandi
------------------------------------

La parte più interessante dei comandi è data dalla possibilità di mettere a disposizione 
parametri e argomenti. Gli argomenti sono delle stringhe, separate da spazi, che seguono
il nome stesso del comando. Devono essere inseriti in un ordine preciso e possono essere opzionali o 
obbligatori. Ad esempio, per aggiungere un argomento opzionale ``cognome`` al precedente
comando e rendere l'argomento ``nome`` obbligatorio, si dovrà scrivere::

    $this
        // ...
        ->addArgument(
            'nome',
            InputArgument::REQUIRED,
            'Chi vuoi salutare?'
        )
        ->addArgument(
            'cognome',
            InputArgument::OPTIONAL,
            'Il tuo cognome?'
        )

A questo punto si può accedere all'argomento ``cognome`` dal proprio codice::

    if ($cognome = $input->getArgument('cognome')) {
        $testo .= ' '.$cognome;
    }

Il comando potrà essere utilizzato in uno qualsiasi dei seguenti modi:

.. code-block:: bash

    $ app/console demo:saluta Fabien
    $ app/console demo:saluta Fabien Potencier

Utilizzo delle opzioni nei comandi
----------------------------------

Diversamente dagli argomenti, le opzioni non sono ordinate (cioè possono essere 
specificate in qualsiasi ordine) e sono identificate dal doppio trattino (come in --urla; è 
anche possibile dichiarare una scorciatoia a singola lettera preceduta da un solo  
trattino come in ``-u``). Le opzioni sono *sempre* opzionali e possono accettare valori 
(come in ``dir=src``) o essere semplici indicatori booleani senza alcuna assegnazione 
(come in ``urla``).

.. tip::

    È anche possibile fare in modo che un'opzione possa *opzionalmente* accettare un valore (ad esempio
    si potrebbe avere ``--urla`` o ``--urla=forte``). Le opzioni possono anche essere configurate per 
    accettare array di valori.

Ad esempio, per specificare il numero di volte in cui il messaggio di 
saluto sarà stampato, si può aggiungere la seguente opzione::

    $this
        // ...
        ->addOption(
            'ripetizioni',
            null,
            InputOption::VALUE_REQUIRED,
            'Quante volte dovrà essere stampato il messaggio?',
            1
        );

Ora è possibile usare l'opzione per stampare più volte il messaggio:

.. code-block:: php

    for ($i = 0; $i < $input->getOption('ripetizioni'); $i++) {
        $output->writeln($testo);
    }

In questo modo, quando si esegue il comando, sarà possibile specificare, opzionalmente, 
l'impostazione ``--ripetizioni``:

.. code-block:: bash

    $ app/console demo:saluta Fabien
    $ app/console demo:saluta Fabien --ripetizioni=5

Nel primo esempio, il saluto verrà stampata una sola volta, visto che ``ripetizioni`` è vuoto e
il suo valore predefinito è ``1`` (l'ultimo argomento di ``addOption``). Nel secondo esempio, il
saluto verrà stampato 5 volte.

Ricordiamo che le opzioni non devono essere specificate in un ordina predefinito. Perciò, entrambi i
seguenti esempi funzioneranno correttamente:

.. code-block:: bash

    $ app/console demo:saluta Fabien --ripetizioni=5 --urla
    $ app/console demo:saluta Fabien --urla --ripetizioni=5

Ci sono 4 possibili varianti per le opzioni:

===========================  =============================================================================================
Opzione                      Valore
===========================  =============================================================================================
InputOption::VALUE_IS_ARRAY  Questa opzione accetta valori multipli (p.e. ``--dir=/pippo --dir=/pluto``)
InputOption::VALUE_NONE      Non accettare alcun valore per questa opzione (come in ``--urla``)
InputOption::VALUE_REQUIRED  Il valore è obbligatorio (come in ``ripetizioni=5``), ò'opzione stessa è comunque facoltativa
InputOption::VALUE_OPTIONAL  L'opzione può avere un valore o meno (p.e. ``urla`` o ``urla=forte``)
===========================  =============================================================================================

È possibile combinare VALUE_IS_ARRAY con VALUE_REQUIRED o con VALUE_OPTIONAL nel seguente modo:

.. code-block:: php

    $this
        // ...
        ->addOption(
            'ripetizioni',
            null,
            InputOption::VALUE_REQUIRED | InputOption::VALUE_IS_ARRAY,
            'Quante volte dovrà essere stampato il messaggio?',
            1
        );

Helper di console
-----------------

Il componente Console contiene anche una serie di "helper", vari piccoli strumenti
in gradi di aiutare con diversi compiti:

* :doc:`/components/console/helpers/dialoghelper`: interactively ask the user for information
* :doc:`/components/console/helpers/formatterhelper`: customize the output colorization

Testare i comandi
-----------------

Symfony2 mette a disposizione diversi strumenti a supporto del test dei comandi. Il più utile 
di questi è la classe :class:`Symfony\\Component\\Console\\Tester\\CommandTester`. Questa utilizza 
particolari classi per la gestione dell'input/output che semplificano lo svolgimento di 
test senza una reale interazione da terminale::

    use Symfony\Component\Console\Application;
    use Symfony\Component\Console\Tester\CommandTester;
    use Acme\DemoBundle\Command\SalutaCommand;

    class ListCommandTest extends \PHPUnit_Framework_TestCase
    {
        public function testExecute()
        {
            $application = new Application();
            $application->add(new SalutaCommand());

            $comando = $application->find('demo:saluta');
            $testDelComando = new CommandTester($comando);
            $testDelComando->execute(array('command' => $comando->getName()));

            $this->assertRegExp('/.../', $testDelComando->getDisplay());

            // ...
        }
    }

Il metodo :method:`Symfony\\Component\\Console\\Tester\\CommandTester::getDisplay` 
restituisce ciò che sarebbe stato mostrato durante una normale chiamata dal 
terminale.

Si può testare l'invio di argomenti e opzioni al comando, passandoli come
array al metodo
:method:`Symfony\\Component\\Console\\Tester\\CommandTester::getDisplay`::

    use Symfony\Component\Console\Application;
    use Symfony\Component\Console\Tester\CommandTester;
    use Acme\DemoBundle\Command\GreetCommand;

    class ListCommandTest extends \PHPUnit_Framework_TestCase
    {
        // ...

        public function testNameIsOutput()
        {
            $application = new Application();
            $application->add(new GreetCommand());

            $command = $application->find('demo:saluta');
            $commandTester = new CommandTester($command);
            $commandTester->execute(
                array('command' => $command->getName(), 'name' => 'Fabien')
            );

            $this->assertRegExp('/Fabien/', $commandTester->getDisplay());
        }
    }

.. tip::

    È possibile testare un'intera applicazione da terminale utilizzando 
    :class:`Symfony\\Component\\Console\\Tester\\ApplicationTester`.

Richiamare un comando esistente
-------------------------------

Se un comando dipende da un altro, da eseguire prima, invece di chiedere all'utente
di ricordare l'ordine di esecuzione, lo si può richiamare direttamente.
Questo è utile anche quando si vuole creare un "meta" comando, che esegue solo una
serie di altri comandi (per esempio, tutti i comandi necessari quando il codice
del progetto è cambiato sui server di produzione: pulire la cache,
genereare i proxy di Doctrine, esportare le risorse di Assetic, ...).

Richiamare un comando da un altro è molto semplice::

    protected function execute(InputInterface $input, OutputInterface $output)
    {
        $comando = $this->getApplication()->find('demo:saluta');

        $argomenti = array(
            'command' => 'demo:saluta',
            'nome'    => 'Fabien',
            '--urla'  => true,
        );

        $input = new ArrayInput($argomenti);
        $codiceDiRitorno = $comando->run($input, $output);

        // ...
    }

Innanzitutto si dovrà trovare (:method:`Symfony\\Component\\Console\\Command\\Command::find`) il
comando da eseguire usandone il nome come parametro.

Quindi si dovrà creare un nuovo 
:class:`Symfony\\Component\\Console\\Input\\ArrayInput` che 
contenga gli argomenti e le opzioni da passare al comando.

Infine, la chiamata al metodo ``run()`` manderà effettivamente in esecuzione il comando e
restituirà il codice di ritorno del comando (``0`` se tutto è andato a buon fine, un qualsiasi altro 
intero negli altri altri casi).

.. note::

    Nella maggior parte dei casi, non è una buona idea quella di eseguire 
    un comando al di fuori del terminale. Innanzitutto perché l'output del 
    comando è ottimizzato per il terminale. Ma, anche più importante, un comando 
    è come un controllore: dovrebbe usare un modello per fare qualsiasi cosa e 
    restituire informazioni all'utente. Perciò, invece di eseguire un comando
    dal Web, sarebbe meglio provare a rifattorizzare il codice e spostare la logica
    all'interno di una nuova classe.

Sapene di più
-------------

* :doc:`/components/console/usage`
* :doc:`/components/console/single_command_tool`

.. _Packagist: https://packagist.org/packages/symfony/console
.. _ANSICON: http://adoxa.3eeweb.com/ansicon/
