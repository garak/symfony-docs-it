Creare comandi per console/terminale
====================================

Con Symfony2 viene distribuito un componente Console, che permette di creare
comandi per terminale. I comandi da terminale possono essere utilizzati per 
qualsiasi compito ripetitivo come i cronjob, le importazioni o altri gruppi di lavori.

Creazione di comandi di base
----------------------------

Per avere automaticamente a disposizione, sotto Symfony2, un comando a terminale, 
si dovrà creare una cartella ``Command`` all'interno del proprio bundle dentro la quale 
si inserirà un file, con il suffisso ``Command.php``, per ogni comando che si voglia realizzare. 
Ad esempio, per estendere l'``AcmeDemoBundle`` (disponibile in Symfony Standard Edition) con 
un programma che porga il saluto dal terminale, si dovrà creare il file  ``SalutaCommand.php`` 
contenente il seguente codice::

    // src/Acme/DemoBundle/Command/GreetCommand.php
    namespace Acme\DemoBundle\Command;

    use Symfony\Bundle\FrameworkBundle\Command\ContainerAwareCommand;
    use Symfony\Component\Console\Input\InputArgument;
    use Symfony\Component\Console\Input\InputInterface;
    use Symfony\Component\Console\Input\InputOption;
    use Symfony\Component\Console\Output\OutputInterface;

    class SalutaCommand extends ContainerAwareCommand
    {
        protected function configure()
        {
            $this
                ->setName('demo:saluta')
                ->setDescription('Saluta qualcuno')
                ->addArgument('nome', InputArgument::OPTIONAL, 'Chi vuoi salutare?')
                ->addOption('urla', null, InputOption::VALUE_NONE, 'Se impostato, il saluto verrà urlato con caratteri maiuscoli')
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

È possibile provare il programma nel modo seguente

.. code-block:: bash

    app/console demo:saluta Fabien

Che andrà a scrivere nel terminale quello che segue:

.. code-block:: text

    Ciao Fabien

È anche possibile usare l'opzione ``--urla`` per stampare il saluto in lettere maiuscole:

.. code-block:: bash

    app/console demo:saluta Fabien --urla

Il cui risultato sarà::

    CIAO FABIEN

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

Utilizzo degli argomenti nei comandi
------------------------------------

La parte più interessante dei comandi è data dalla possibilità di mettere a disposizione 
parametri e argomenti. Gli argomenti sono delle stringhe, separate da spazi, che seguono
il nome stesso del comando. Devono essere inseriti in un ordine preciso e possono essere opzionali o 
obbligatori. Ad esempio, per aggiungere un argomento opzionale ``cognome`` al precedente
comando e rendere l'argomento ``nome`` obbligatorio, si dovrà scrivere:

    $this
        // ...
        ->addArgument('nome', InputArgument::REQUIRED, 'Chi vuoi salutare?')
        ->addArgument('cognome', InputArgument::OPTIONAL, 'Il tuo cognome?')
        // ...

A qeusto punto si può accedere all'argomento ``cognome`` dal proprio codice::

    if ($cognome = $input->getArgument('cognome')) {
        $testo .= ' '.$cognome;
    }

Il comando potrà essere utilizzato in uno qualsiasi dei seguenti modi:

.. code-block:: bash

    app/console demo:saluta Fabien
    app/console demo:saluta Fabien Potencier

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
        ->addOption('ripetizioni', null, InputOption::VALUE_REQUIRED, 'Quante volte dovrà essere stampato il messaggio?', 1)

Ora è possibile usare l'opzione per stampare più volte il messaggio:

.. code-block:: php

    for ($i = 0; $i < $input->getOption('ripetizioni'); $i++) {
        $output->writeln($testo);
    }

In questo modo, quando si esegue il comando, sarà possibile specificare, opzionalmente, 
l'impostazione ``--ripetizioni``:

.. code-block:: bash

    app/console demo:saluta Fabien

    app/console demo:saluta Fabien --ripetizioni=5

Nel primo esempio, il saluto verrà stampata una sola volta, visto che ``ripetizioni`` è vuoto e
il suo valore predefinito è ``1`` (l'ultimo argomento di ``addOption``). Nel secondo esempio, il
saluto verrà stampato 5 volte.

Ricordiamo che le opzioni non devono essere specificate in un ordina predefinito. Perciò, entrambi i
seguenti esempi funzioneranno correttamente:

.. code-block:: bash

    app/console demo:saluta Fabien --ripetizioni=5 --urla
    app/console demo:saluta Fabien --urla --ripetizioni=5

Richiedere informazioni all'utente
----------------------------------

Nel creare comandi è possibile richiedere ulteriori informazioni dagli utenti 
rivolgendo loro domande. Ad esempio, si potrbbe richiedere la conferma 
prima di effettuare realmente una determinata azione. In questo caso si dovrà aggiungere 
il seguente codice al comando::

    $dialogo = $this->getHelperSet()->get('dialog');
    if (!$dialogo->askConfirmation($output, '<question>Vuoi proseguire con questa azione?</question>', false)) {
        return;
    }

In questo modo, all'utente verrà chiesto se vuole "proseguire con questa azione" e, a meno che 
la risposta non sia ``y``, l'azione non verrà eseguita. Il terzo argomento di 
``askConfirmation`` è il valore predefinito da restituire nel caso in cui l'utente non 
fornisca alcun input.

È possibile rivolgere domande che prevedano risposte più complesse di un semplice si/no. Ad esempio, 
se volessimo conoscere il nome di qualcosa, potremmo fare nel seguente modo::

    $dialogo = $this->getHelperSet()->get('dialog');
    $nome = $dialogo->ask($output, 'Insersci il nome del widget', 'pippo');

Testare i comandi
-----------------

Symfony2 mette a disposizione diversi strumenti a supporto del test dei comandi. Il più utile 
di questi è la classe :class:`Symfony\\Component\\Console\\Tester\\CommandTester`. Questa utilizza 
particolari classi per la gestione dell'input/output che semplificano lo svolgimento di 
test senza una reale interazione da terminale::

    use Symfony\Component\Console\Tester\CommandTester;
    use Symfony\Bundle\FrameworkBundle\Console\Application;

    class ListCommandTest extends \PHPUnit_Framework_TestCase
    {
        public function testExecute()
        {
            // simula il Kernel o ne crea uno a seconda delle esigenze
            $application = new Application($kernel);

            $comando = $application->find('demo:saluta');
            $testDelComando = new CommandTester($comando);
            $testDelComando->execute(array('command' => $comando->getFullName()));

            $this->assertRegExp('/.../', $testDelComando->getDisplay());

            // ...
        }
    }

Il metodo :method:`Symfony\\Component\\Console\\Tester\\CommandTester::getDisplay` 
restituisce ciò che sarebbe stato mostrato durante una normale chiamata dal 
terminale.

.. tip::

    È possibile testare un'intera applicazione da terminale utilizzando 
    :class:`Symfony\\Component\\Console\\Tester\\ApplicationTester`.

Ottenere i servizi dal contenitore dei servizi
----------------------------------------------

Utilizzando la classe :class:`Symfony\Bundle\FrameworkBundle\Command\ContainerAwareCommand` 
come classe base per i comandi (al posto della meno evoluta 
:class:`Symfony\Component\Console\Command\Command`) si ha la possibilità di accedere al 
contenitore dei servizi. In altre parole, è possibile accedere ad ogni servizio che sia stato 
configurato. Ad esempio, è possibile estendere facilmente la precedente azione affinchè sia traducibile::

    protected function execute(InputInterface $input, OutputInterface $output)
    {
        $nome = $input->getArgument('nome');
        $traduttore = $this->getContainer()->get('translator');
        if ($nome) {
            $output->writeln($traduttore->trans('Ciao %nome%!', array('%nome%' => $nome)));
        } else {
            $output->writeln($traduttore->trans('Ciao!'));
        }
    }

Richiamare un comando esistente
-------------------------------

Se un comando dipende da un'altro, che deve quindi essere eseguito per primo, invece 
di costringere l'utente a ricordarsi l'ordina di esecuzione, è possibile richiamarlo 
direttamente. Ciò risulta pratico anche nel caso si voglia creare dei "meta" comandi che 
non facciano altro che eseguire gruppi di altri comandi (ad esempio, l'insieme di comandi
da eseguire quando il codice del progetto viene modificato nel server di produzione: pulire
la cache, generare i metodi proxy di Doctrine2, eseguire il dump delle risorse di Assetic, ...).

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
	un comando al di fuori del terminale. Innanzitutto perchè l'output del 
	comando è ottimizzato per il terminale. Ma, anche più importante, un comando 
	è come un controllore: dovrebbe usare un modello per fare qualsiasi cosa e 
	restituire informazioni all'utente. Perciò, invece di eseguire un comando
	dal Web, sarebbe meglio provare a rifattorizzare il codice e spostare la logica
	all'interno di una nuova classe.
