.. index::
    single: Console; CLI
    single: Componenti; Console

Il componente Console
=====================

    Il componente Console semplifica la creazione di eleganti e testabili comandi
    da terminale.

Symfony viene distribuito con un componente Console, che permette di creare
comandi da terminale. I comandi da terminale possono essere utilizzati per qualsiasi
lavoro ripetitivo, come i lavori di cron, importazioni o lavori batch.

Installazione
-------------

Il componente può essere installato in due modi:

* Installandolo :doc:`tramite Composer </components/using_components>` (``symfony/console`` su `Packagist`_);
* Utilizzando il repository Git ufficiale (https://github.com/symfony/Console).

.. include:: /components/require_autoload.rst.inc

Creazione di comandi di base
----------------------------

Per creare un comando che porga il saluto dal terminale, creare il file  ``SalutaCommand.php``,
contenente il seguente codice::

    namespace Acme\Console\Command;

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
    <?php
    // application.php

    require __DIR__.'/vendor/autoload.php';

    use Acme\Console\Command\SalutaCommand;
    use Symfony\Component\Console\Application;

    $application = new Application();
    $application->add(new SalutaCommand());
    $application->run();

È possibile provare il programma nel modo seguente

.. code-block:: bash

    $ php application.php demo:saluta Fabien

Il comando scriverà, nel terminale, quello che segue:

.. code-block:: text

    Ciao Fabien

È anche possibile usare l'opzione ``--urla`` per stampare il saluto in lettere maiuscole:

.. code-block:: bash

    $ php application.php demo:saluta Fabien --urla

Il cui risultato sarà::

    CIAO FABIEN

.. _components-console-coloring:

Colorare l'output
~~~~~~~~~~~~~~~~~

.. note::

    Windows non supporta i colori ANSI in modo predefinito, quindi il componente Console individua e
    disabilita i colori quando Windows non dà supporto. Tuttavia, se Windows non è
    configurato con un driver ANSI e i propri comandi di console invocano altri script che
    emettono sequenze di colori ANSI, saranno mostrati come sequenze di caratteri grezzi.
    Per abilitare il supporto ai colori ANSI su Windows, si può installare `ConEmu`_ o `ANSICON`_.

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

    use Symfony\Component\Console\Formatter\OutputFormatterStyle;

    // ...
    $style = new OutputFormatterStyle('red', 'yellow', array('bold', 'blink'));
    $output->getFormatter()->setStyle('fire', $style);
    $output->writeln('<fire>pippo</fire>');

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

Livelli di verbosità
~~~~~~~~~~~~~~~~~~~~

.. versionadded:: 2.3
   Le costanti ``VERBOSITY_VERY_VERBOSE`` e ``VERBOSITY_DEBUG`` sono state introdotte
   nella versione 2.3

La console dispone di tre livelli di verbosità. Tali livelli sono definiti in
:class:`Symfony\\Component\\Console\\Output\\OutputInterface`:

=======================================  ===================================
Opzione                                     Valore
=======================================  ===================================
OutputInterface::VERBOSITY_QUIET         Nessun messaggio in output
OutputInterface::VERBOSITY_NORMAL        Livello predefinito di verbosità
OutputInterface::VERBOSITY_VERBOSE       Verbosità maggiore
OutputInterface::VERBOSITY_VERY_VERBOSE  Messaggi informativi non essenziali
OutputInterface::VERBOSITY_DEBUG         Messaggi di debug
=======================================  ===================================

Si può specificare il livello quieto di verbosità con l'opzione ``--quiet`` o ``-q``.
L'opzione ``--verbose`` o ``-v`` si usa quando si vuole un livello di verbosità
maggiore.

.. tip::

    Se si usa il livello ``VERBOSITY_VERBOSE``, viene mostrato lo stacktrace
    completo delle eccezioni.

È anche possibile mostrare un messaggio in un comando solo per uno specifico livello
di verbosità. Per esempio::

    if (OutputInterface::VERBOSITY_VERBOSE <= $output->getVerbosity()) {
        $output->writeln(...);
    }

Quando si usa il livello quieto, viene soppresso ogni output, poiché iol metodo
:method:`Symfony\\Component\\Console\\Output::write`
esce senza stampare nulla.

Utilizzo dei parametri nei comandi
----------------------------------

La parte più interessante dei comandi è data dalla possibilità di mettere a disposizione 
parametri e opzioni. I parametri sono delle stringhe, separate da spazi, che seguono
il nome stesso del comando. Devono essere inseriti in un ordine preciso e possono essere opzionali o 
obbligatori. Ad esempio, per aggiungere un parametro opzionale ``cognome`` al precedente
comando e rendere il parametro ``nome`` obbligatorio, si dovrà scrivere::

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
        );

A questo punto si può accedere al parametro ``cognome`` dal codice::

    if ($cognome = $input->getArgument('cognome')) {
        $testo .= ' '.$cognome;
    }

Il comando potrà essere utilizzato in uno qualsiasi dei seguenti modi:

.. code-block:: bash

    $ php application.php demo:saluta Fabien
    $ php application.php demo:saluta Fabien Potencier

È anche possibile consentire una lista di valori a un parametro (si immagini di
voler salutare tutti gli amici). Lo si deve fare alla fine della lista dei
parametri::

    $this
        // ...
        ->addArgument(
            'nomi',
            InputArgument::IS_ARRAY,
            'Chi vuoi salutare (separare i nomi con uno spazio)?'
        );

In questo modo, si possono specificare più nomi:

.. code-block:: bash

    $ php application.php demo:saluta Fabien Ryan Bernhard

Si può accedere al parametro ``nomi`` come un array::

    if ($nomi = $input->getArgument('nomi')) {
        $testo .= ' '.implode(', ', $nomi);
    }

Ci sono tre varianti di parametro utilizzabili:

===========================  ====================================================================================================================
Modalità                     Valore
===========================  ====================================================================================================================
InputArgument::REQUIRED      Il parametro è obbligatorio
InputArgument::OPTIONAL      Il parametro è facoltativo, può essere omesso
InputArgument::IS_ARRAY      Il parametro può contenere un numero indefinito di parametri e deve essere usato alla fine della lista dei parametri
===========================  ====================================================================================================================

Si può combinare ``IS_ARRAY`` con ``REQUIRED`` e ``OPTIONAL``, per esempio::

    $this
        // ...
        ->addArgument(
            'nomi',
            InputArgument::IS_ARRAY | InputArgument::REQUIRED,
            'Chi vuoi salutare (separare i nomi con uno spazio)?'
        );

Utilizzo delle opzioni nei comandi
----------------------------------

Diversamente dagli argomenti, le opzioni non sono ordinate (cioè possono essere 
specificate in qualsiasi ordine) e sono identificate dal doppio trattino (come in --urla; è 
anche possibile dichiarare una scorciatoia a singola lettera preceduta da un solo  
trattino come in ``-u``). Le opzioni sono *sempre* opzionali e possono accettare valori 
(come in ``--dir=src``) o essere semplici indicatori booleani senza alcuna assegnazione 
(come in ``--urla``).

.. tip::

    Nulla impedisce la creazione di un comando con un'opzione che accetti in modo facoltativo
    un valore. Tuttavia, non c'è modo di distinguere quando l'opzione sia stata usata senza
    un valore (``comando --urla``) o quando non sia stata usata affatto
    (``comando``). In entrambi i casi, il valore recuperato per l'opzione
    sarebbe ``null``.

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

    $ php application.php demo:saluta Fabien
    $ php application.php demo:saluta Fabien --ripetizioni=5

Nel primo esempio, il saluto verrà stampato una sola volta, visto che ``ripetizioni`` è vuoto e
il suo valore predefinito è ``1`` (l'ultimo parametro di ``addOption``). Nel secondo esempio, il
saluto verrà stampato 5 volte.

Ricordiamo che le opzioni non devono essere specificate in un ordine predefinito. Perciò, entrambi i
seguenti esempi funzioneranno correttamente:

.. code-block:: bash

    $ php application.php demo:saluta Fabien --ripetizioni=5 --urla
    $ php application.php demo:saluta Fabien --urla --ripetizioni=5

Ci sono 4 possibili varianti per le opzioni:

===========================  =============================================================================================
Opzione                      Valore
===========================  =============================================================================================
InputOption::VALUE_IS_ARRAY  Questa opzione accetta valori multipli (p.e. ``--dir=/pippo --dir=/pluto``)
InputOption::VALUE_NONE      Non accettare alcun valore per questa opzione (come in ``--urla``)
InputOption::VALUE_REQUIRED  Il valore è obbligatorio (come in ``ripetizioni=5``), l'opzione stessa è comunque facoltativa
InputOption::VALUE_OPTIONAL  L'opzione può avere un valore o meno (p.e. ``urla`` o ``urla=forte``)
===========================  =============================================================================================

È possibile combinare ``VALUE_IS_ARRAY`` con ``VALUE_REQUIRED`` o con ``VALUE_OPTIONAL`` nel seguente modo:

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

Aiutanti di console
-------------------

Il componente Console contiene anche una serie di "aiutanti", vari piccoli strumenti
in grado di aiutare con diversi compiti:

* :doc:`/components/console/helpers/dialoghelper`: chiede informazioni interattive all'utente
* :doc:`/components/console/helpers/formatterhelper`: personalizza i colori dei testi
* :doc:`/components/console/helpers/progresshelper`: mostra una barra di progressione
* :doc:`/components/console/helpers/tablehelper`: mostra dati in una tabella

.. _component-console-testing-commands:

Testare i comandi
-----------------

Symfony mette a disposizione diversi strumenti a supporto del test dei comandi. Il più utile 
di questi è la classe :class:`Symfony\\Component\\Console\\Tester\\CommandTester`. Questa utilizza 
particolari classi per la gestione dell'input/output che semplificano lo svolgimento di 
test senza una reale interazione da terminale::

    use Acme\Console\Command\SalutaCommand;
    use Symfony\Component\Console\Application;
    use Symfony\Component\Console\Tester\CommandTester;

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
:method:`Symfony\\Component\\Console\\Tester\\CommandTester::execute`::

    use Acme\Console\Command\SalutaCommand;
    use Symfony\Component\Console\Application;
    use Symfony\Component\Console\Tester\CommandTester;

    class ListCommandTest extends \PHPUnit_Framework_TestCase
    {
        // ...

        public function testNameIsOutput()
        {
            $application = new Application();
            $application->add(new SalutaCommand());

            $comando = $application->find('demo:saluta');
            $testDelComando = new CommandTester($command);
            $testDelComando->execute(array(
                'command'       => $comando->getName(),
                'nome'          => 'Fabien',
                '--ripetizioni' => 5,
            ));

            $this->assertRegExp('/Fabien/', $testDelComando->getDisplay());
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
generare i proxy di Doctrine, esportare le risorse di Assetic, ...).

Richiamare un comando da un altro è molto semplice::

    protected function execute(InputInterface $input, OutputInterface $output)
    {
        $comando = $this->getApplication()->find('demo:saluta');

        $parametri = array(
            'command' => 'demo:saluta',
            'nome'    => 'Fabien',
            '--urla'  => true,
        );

        $input = new ArrayInput($parametri);
        $codiceDiRitorno = $comando->run($input, $output);

        // ...
    }

Innanzitutto si dovrà trovare (:method:`Symfony\\Component\\Console\\Application::find`) il
comando da eseguire usandone il nome come parametro.

Quindi si dovrà creare un nuovo 
:class:`Symfony\\Component\\Console\\Input\\ArrayInput` che 
contenga i parametri e le opzioni da passare al comando.

Infine, la chiamata al metodo ``run()`` manderà effettivamente in esecuzione il comando e
restituirà il codice di ritorno del comando (il valore restituito dal metodo
``execute`` del comando).

.. note::

    Nella maggior parte dei casi, non è una buona idea quella di eseguire 
    un comando al di fuori del terminale. Innanzitutto perché l'output del 
    comando è ottimizzato per il terminale. Ma, anche più importante, un comando 
    è come un controllore: dovrebbe usare un modello per fare qualsiasi cosa e 
    restituire informazioni all'utente. Perciò, invece di eseguire un comando
    dal Web, sarebbe meglio provare a rifattorizzare il codice e spostare la logica
    all'interno di una nuova classe.

Saperne di più
--------------

* :doc:`/components/console/usage`
* :doc:`/components/console/single_command_tool`
* :doc:`/components/console/events`
* :doc:`/components/console/console_arguments`

.. _Packagist: https://packagist.org/packages/symfony/console
.. _ConEmu: https://code.google.com/p/conemu-maximus5/
.. _ANSICON: https://github.com/adoxa/ansicon/releases
