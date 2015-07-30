.. index::
    single: Aiutanti della console; Aiutante Dialog

Aiutante Dialog 
===============

:class:`Symfony\\Component\\Console\\Helper\\DialogHelper` fornisce 
funzioni per chiedere informazioni all'utente. È incluso nell'insieme
predefinito degli aiutanti, ottenibile richiamando
:method:`Symfony\\Component\\Console\\Command\\Command::getHelperSet`::

    $dialog = $this->getHelperSet()->get('dialog');

Tutti i metodi all'interno dell'aiutante Dialog hanno
:class:`Symfony\\Component\\Console\\Output\\OutputInterface` come primo
parametro, la domanda come secondo parametro e il valore predefinito come ultimo
parametro.

Chiedere conferma all'utente
----------------------------

Si supponga di voler confermare un'azione prima di eseguirla effettivamente.
Aggiungere al comando il codice seguente::

    // ...
    if (!$dialog->askConfirmation(
            $output,
            '<question>Continuare con questa azione?</question>',
            false
        )) {
        return;
    }

In questo caso, all'utente sarà chiesto "Continuare con questa azione", e si otterrà
``true`` se l'utente risponde ``y`` o ``false`` se l'utente risponde
``n``. Il terzo parametro di
:method:`Symfony\\Component\\Console\\Helper\\DialogHelper::askConfirmation`
è il valore predefinito da restituire se l'utente non inserisce alcun input. Qualsiasi
altro input farà ripetere la domanda.

Chiedere informazioni all'utente
--------------------------------

Si possono anche porre domande con risposte più complesse di sì/no. Per esempio,
se si vuole sapere il nome di un bundle, si può aggiungere al comando::

    // ...
    $bundle = $dialog->ask(
        $output,
        'Prego inserire il nome del bundle',
        'AcmeDemoBundle'
    );

All'utente sarà chiesto "Prego inserire il nome del bundle". L'utente potrà inserire
un nome, che sarà restituito dal metodo
:method:`Symfony\\Component\\Console\\Helper\\DialogHelper::ask`. Se
lasciato vuoto, sarà restituito il valore predefinito (qui ``AcmeDemoBundle``).

Autcompletamento
~~~~~~~~~~~~~~~~

.. versionadded:: 2.2
    L'autocompletamento è stato aggiunto in Symfony 2.2.

Si possono anche specificare delle possibili risposte alla domanda data. Saranno
completate man mano che l'utente scrive::

    $dialog = $this->getHelperSet()->get('dialog');
    $bundleNames = array('AcmeDemoBundle', 'AcmeBlogBundle', 'AcmeStoreBundle');
    $name = $dialog->ask(
        $output,
        'Prego inserire il nome del bundle',
        'PippoBundle',
        $bundleNames
    );

Nascondere la risposta dell'utente
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

.. versionadded:: 2.2
    Il metodo ``askHiddenResponse`` è stato aggiunto in Symfony 2.2.

Si può anche fare una domanda e nascondere la risposta. Ciò risulta utile
in particolare per le password::

    $dialog = $this->getHelperSet()->get('dialog');
    $password = $dialog->askHiddenResponse(
        $output,
        'Inserire la password della base dati',
        false
    );

.. caution::

    Quando si richiede una risposta nascosta, Symfony userà un binario, cambierà
    la modalità stty oppure userà un altro trucco per nascondere la risposta. Se nessuna opzione è
    disponibile, si arrenderà e mostrerà la risposta, a meno che non si passi ``false``
    come terzo parametro, come nell'esempio appena visto. In questo caso, sarà sollevata
    una ``RuntimeException``.

Validare la risposta
--------------------

Si può anche validare la risposta. Per esempio, nell'ultimo esempio è stato
chiesto il nome di un bundle. Seguendo le convenzioni di Symfony, il nome dovrebbe
avere il suffisso ``Bundle``. Lo si può validare, usando il metodo
:method:`Symfony\\Component\\Console\\Helper\\DialogHelper::askAndValidate`::


    // ...
    $bundle = $dialog->askAndValidate(
        $output,
        'Prego inserire il nome del bundle',
        function ($answer) {
            if ('Bundle' !== substr($answer, -6)) {
                throw new \RuntimeException(
                    'Il nome del bundle deve avere \'Bundle\' come suffisso'
                );
            }

            return $answer;
        },
        false,
        'AcmeDemoBundle'
    );

Il metodo ha due nuovi parametri. La sua firma completa è::

    askAndValidate(
        OutputInterface $output,
        string|array $question,
        callback $validator,
        integer $attempts = false,
        string $default = null,
        array $autocomplete = null
    )

Il parametro ``$validator`` è un callback, che gestisce la validazione. Dovrebbe
lanciare un'eccezione se qualcosa va storto. Il messaggio dell'eccezione è mostrato
nella console, quindi è una buona pratica inserirvi delle informazioni
rilevanti.

Si può impostare il numero massimo di volte in cui fare la domanda, nel parametro ``$attempts``.
Una volta raggiunto tale numero, sarà usato il valore predefinito, fornito
nell'ultimo parametro. Usando ``false`` si indica che il numero di tentativi è infinito.
L'utente vedrà la domanda finché inserisce una risposta non valida e potrà
procedere solo in caso di risposta valida.

Nascondere la risposta dell'utente
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

.. versionadded:: 2.2
    Il metodo ``askHiddenResponseAndValidate`` è stato aggiunto in Symfony 2.2.

Si può anche fare una domanda e validare una risposta nascosta::

    $dialog = $this->getHelper('dialog');

    $validator = function ($value) {
        if ('' === trim($value)) {
            throw new \Exception('La password non può essere vuota');
        }

        return $value;
    };

    $password = $dialog->askHiddenResponseAndValidate(
        $output,
        'Si prega di inserire la password',
        $validator,
        20,
        false
    );

Se si vuole consentire che la risposta sia visibile, in caso non possa essere nascosta
per qualche ragione, passare ``true`` come quinto parametro.

Consentire una scelta da una lista di risposte
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

.. versionadded:: 2.2
    Il metodo :method:`Symfony\\Component\\Console\\Helper\\DialogHelper::select` è
    stato aggiunto in Symfony 2.2.

Se si ha un insieme predefinito di risposte tra cui l'utente può scegliere, si
può usare il metodo ``ask`` descritto in precedenza oppure, per assicurarsi che l'utente
fornisca una risposta corretta, il metodo ``askAndValidate``. Entrambi hanno
lo svantaggio di costringere lo sviluppatore a gestire i valori non corretti da solo.

Si può invece usare il metodo
:method:`Symfony\\Component\\Console\\Helper\\DialogHelper::select`,
che assicura che l'utente possa inserire solamente una stringa valida,
da una lista predefinita::

    $dialog = $this->getHelper('dialog');
    $colors = array('rosso', 'blu', 'giallo');

    $color = $dialog->select(
        $output,
        'Scegli il tuo colore preferito (predefinito: rosso)',
        $colors,
        0
    );
    $output->writeln('Hai scelto: ' . $colors[$color]);

    // ... fare qualcosa con il colore

L'opzione selezionata come predefinita va fornita come quarto
parametro. Il valore predefinito è ``null``, che significa che nessuna opzione è predefinita.

Se l'utente inserisce una stringa non valida, viene mostrato un errore e chiesto all'utente
di fornire una nuova risposta, finché non ne inserisce una valida o
raggiunge il numero massimo di tentativi (definibile nel quinto
parametro). Il valore predefinito per i tentativi è ``false``, che equivale a
infiniti tentativi. Si può definire un messaggio di errore personalizzato nel sesto parametro.

.. versionadded:: 2.3
    Il supporto alla selezione multipla è stato aggiunto in Symfony 2.3.

Scelte multiple
...............

A volte si possono dare più risposte. DialogHelper lo supporta tramite
l'uso di valori separati da virgole. Per abilitare questa possibilità,
occorre impostare il settimo parametro a ``true``::

    // ...

    $selected = $dialog->select(
        $output,
        'Scegli il tuo colore preferito (predefinito: rosso)',
        $colors,
        0,
        false,
        'Il valore "%s" non è valido',
        true // abilita la selezione multipla
    );

    $selectedColors = array_map(function($c) use ($colors) {
        return $colors[$c];
    }, $selected);

    $output->writeln(
        'Hai scelto: ' . implode(', ', $selectedColors)
    );

Se ora l'utente inserisce ``1,2``, il risultato sarà:
``Hai scelto: blu, giallo``.

Testare un comando con un input atteso
--------------------------------------

Se si vuole scrivere un test per un comando che si aspetta un qualche tipo di input
da linea di comando, occorre sovrascrivere HelperSet usato dal comando::

    use Symfony\Component\Console\Application;
    use Symfony\Component\Console\Helper\DialogHelper;
    use Symfony\Component\Console\Helper\HelperSet;
    use Symfony\Component\Console\Tester\CommandTester;

    // ...
    public function testExecute()
    {
        // ...
        $application = new Application();
        $application->add(new MyCommand());
        $command = $application->find('my:command:name');
        $commandTester = new CommandTester($command);

        $dialog = $command->getHelper('dialog');
        $dialog->setInputStream($this->getInputStream("Test\n"));
        // Equivale all'inserimento di "Test" e pressione di ENTER
        // Se occorre una conferma, va bene anche "yes\n"

        $commandTester->execute(array('command' => $command->getName()));

        // $this->assertRegExp('/.../', $commandTester->getDisplay());
    }

    protected function getInputStream($input)
    {
        $stream = fopen('php://memory', 'r+', false);
        fputs($stream, $input);
        rewind($stream);

        return $stream;
    }

Impostando il flusso di input di ``DialogHelper``, si imita ciò che la
console farebbe internamente con l'input dell'utente tramite cli. In questo modo,
si può testare ogni interazione, anche complessa, passando un appropriato
flusso di input.

.. seealso::

    Si possono trovare maggiori informazioni sui test dei comandi nella documentazione del
    componente console, in :ref:`test dei comandi console <component-console-testing-commands>`.
