.. index::
    single: Aiutanti di console; Aiutante Question

Aiutante Question
=================

.. versionadded:: 2.5
    L'aiutante Question è stato introdotto in Symfony 2.5.

:class:`Symfony\\Component\\Console\\Helper\\QuestionHelper` fornisce
funzioni per chiedere all'utente maggiori informazioni. Fa parte degli
aiutanti predefiniti, il cui elenco si può ottenere richiamando
:method:`Symfony\\Component\\Console\\Command\\Command::getHelperSet`::

    $helper = $this->getHelper('question');

L'aiutante Question ha un solo metodo,
:method:`Symfony\\Component\\Console\\Command\\Command::ask`, che ha bisogno di un'istanza di
:class:`Symfony\\Component\\Console\\Output\\InputInterface` come
primo parametro, una istanza di :class:`Symfony\\Component\\Console\\Output\\OutputInterface`
come secondo parametro e una
:class:`Symfony\\Component\\Console\\Question\\Question` come ultimo parametro.

Chiedere conferma all'utente
----------------------------

Si supponga di voler confermare un'azione, prima di esegurila. Aggiungere
a un comando::

    use Symfony\Component\Console\Question\ConfirmationQuestion;
    // ...

    $helper = $this->getHelper('question');
    $question = new ConfirmationQuestion('Continuare con questa azione?', false);

    if (!$helper->ask($input, $output, $question)) {
        return;
    }

In questo caso, all'utente sarà posta la domanda "Continuare con questa azione?". Se l'utente
risponde con ``y``, restituirà ``true``, se invece risponderà ``n`` restituirà ``false``.
Il secondo parametro di
:method:`Symfony\\Component\\Console\\Question\\ConfirmationQuestion::__construct`
è il valore predefinito da restituire se l'utente non risponde nulla. Qualsiasi risposta
diversa farà sì che la domanda venga posta nuovamente.

Chiedere informazioni all'utente
--------------------------------

Si possono anche porre domande con risposte più elaborate. Per esempio,
se si vuole sapere il nome di un bundle, si può aggiungere a un comando::

    use Symfony\Component\Console\Question\Question;
    // ...

    $question = new Question('Si prega di inserire il nome del bundle', 'AcmeDemoBundle');

    $bundle = $helper->ask($input, $output, $question);

All'utente sarà chiesto "Si prega di inserire il nome del bundle". L'utente può inserire
un nome, che sarà restituito dal metodo
:method:`Symfony\\Component\\Console\\Helper\\QuestionHelper::ask`.
Se la risposta sarà vuota, sarà restituito il valore predefinito (``AcmeDemoBundle``, in questo caso).

Far scegliere da una lista di risposte
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Se si ha un insieme predefinito di risposte, tra cui l'utente può scegliere, si
può usare :class:`Symfony\\Component\\Console\\Question\\ChoiceQuestion`,
che si assicura che l'utente inserisca una stringa valida, cioè compresa
nella lista::

    use Symfony\Component\Console\Question\ChoiceQuestion;
    // ...

    $helper = $this->getHelper('question');
    $question = new ChoiceQuestion(
        'Si prega di scegliere un colore (predefinito: rosso)',
        array('rosso', 'blu', 'giallo'),
        0
    );
    $question->setErrorMessage('Il colore %s non è valido.');

    $color = $helper->ask($input, $output, $question);
    $output->writeln('Colore scelto: '.$color);

    // ... fare qualcosa con il colore

L'opzione predefinita viene fornità dal terzo parametro
del costruttore. Il valore predefinito è ``null``, che significa che
nessuna opzione sarà quella predefinita.

Se l'utente inserisce una stringa non valida, viene mostrato un messaggio di errore e
chiesta nuovamente la domanda, fino a che l'utente non inserirà una stringa valida
o raggiungerà il numero massimo di tentativi. Il valore predefinito per il numero massimo
di tentativi è ``null``, che significa un numero infinito di tentativi. Si può definire
un messaggio di errore, usando
:method:`Symfony\\Component\\Console\\Question\\ChoiceQuestion::setErrorMessage`.

Scelte multiple
...............

A volte, si possono dare più risposte. ``ChoiceQuestion`` fornisce questa
caratteristica, usando valori separati da virgole. Per abilitarla, 
usare :method:`Symfony\\Component\\Console\\Question\\ChoiceQuestion::setMultiselect`::

    use Symfony\Component\Console\Question\ChoiceQuestion;
    // ...

    $helper = $this->getHelper('question');
    $question = new ChoiceQuestion(
        'Si prega di scegliere dei colori (predefiniti: rosso e blu)',
        array('rosso', 'blu', 'giallo'),
        '0,1'
    );
    $question->setMultiselect(true);

    $colors = $helper->ask($input, $output, $question);
    $output->writeln('Colori scelti: ' . implode(', ', $colors));

Se l'utente inserisce ``1,2``, il risultato sarà:
``Colori scelti: blu, giallo``.

Se l'utente non inserisce niente, il risultato sarà:
``Colori scelti: rosso, blu``.

Completamento
~~~~~~~~~~~~~

Si può anche specificare un array di potenziali risposte per una data domanda. Questi
forniranno un completamento automatico, mentre l'utente scrive::

    use Symfony\Component\Console\Question\Question;
    // ...

    $bundles = array('AcmeDemoBundle', 'AcmeBlogBundle', 'AcmeStoreBundle');
    $question = new Question('Si prega di inserire il nome di un bundle', 'PippoBundle');
    $question->setAutocompleterValues($bundles);

    $name = $helper->ask($input, $output, $question);

Nascondere la risposta
~~~~~~~~~~~~~~~~~~~~~~

Si può anche fare una domanda e nascondere la risposta. Questo è particolarmente
utile per le password::

    use Symfony\Component\Console\Question\Question;
    // ...

    $question = new Question('Inserire una password');
    $question->setHidden(true);
    $question->setHiddenFallback(false);

    $password = $helper->ask($input, $output, $question);

.. caution::

    Quando si usa una risposta nascosta, Symfony userà un binario, cambierà modalità
    stty o userà un altro trucco per nascodere la risposta. Se nessuno di questi è disponibile,
    si arrenderà e consentirà alla risposta di essere visibile, a meno che non si
    imposti questo comportamento a ``false``, usando
    :method:`Symfony\\Component\\Console\\Question\\Question::setHiddenFallback`,
    come nell'esempio precedente. In questo caso, sarà sollevata
    una``RuntimeException``.

Validare la risposta
--------------------

Si può anche validare la risposta. Per esempio, nell'ultimo esempio è stato
chiesto il nome di un bundle. Seguendo le convenzioni di Symfony2, il nome dovrebbe
avere il suffisso ``Bundle``. Lo si può validare, usando il
metodo
:method:`Symfony\\Component\\Console\\Question\\Question::setValidator`::

    use Symfony\Component\Console\Question\Question;
    // ...

    $question = new Question('Si prega di inserire il nome del bundle', 'AcmeDemoBundle');
    $question->setValidator(function ($answer) {
        if ('Bundle' !== substr($answer, -6)) {
            throw new \RuntimeException(
                'Il nome del bundle deve terminare con \'Bundle\''
            );
        }
        return $answer;
    });
    $question->setMaxAttempts(2);

    $name = $helper->ask($input, $output, $question);

Il parametro ``$validator`` è un callback, che gestisce la validazione. Dovrebbe
lanciare un'eccezione se qualcosa va storto. Il messaggio dell'eccezione è mostrato
nella console, quindi è una buona pratica inserirvi delle informazioni
rilevanti. IL callback dovrebbe anche restituire il valore inserito dall'utente, in
caso di successo.

Si può impostare il numero massimo di volte in cui fare la domanda, con il metodo
:method:`Symfony\\Component\\Console\\Question\\Question::setMaxAttempts`.
Una volta raggiunto tale numero, sarà usato il valore predefinito. Usando ``null`` si
indica che il numero di tentativi è infinito. L'utente vedrà la domanda finché inserisce
una risposta non valida e potrà procedere solo in caso di risposta valida.

Validare una risposta nascosta
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Si può anche usare un validatore con una risposta nascosta::

    use Symfony\Component\Console\Question\Question;
    // ...

    $helper = $this->getHelper('question');

    $question = new Question('Inserire la password');
    $question->setValidator(function ($value) {
        if (trim($value) == '') {
            throw new \Exception('La password non può essere vuota');
        }

        return $value;
    });
    $question->setHidden(true);
    $question->setMaxAttempts(20);

    $password = $helper->ask($input, $output, $question);


Testare un comando con un input atteso
--------------------------------------

Se si vuole scrivere un test per un comando che si aspetta un qualche tipo di input
da linea di omando, occorre sovrascrivere HelperSet usato dal comando::

    use Symfony\Component\Console\Helper\QuestionHelper;
    use Symfony\Component\Console\Helper\HelperSet;
    use Symfony\Component\Console\Tester\CommandTester;

    // ...
    public function testExecute()
    {
        // ...
        $commandTester = new CommandTester($command);

        $helper = $command->getHelper('question');
        $helper->setInputStream($this->getInputStream('Test\\n'));
        // Equivale all'utente che inserisce "Test" e preme invio
        // Se serve una conferma, va bene "yes\n"

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

Impostando il flusso di input di ``QuestionHelper``, si imita ciò che la
console farebbe internamente con l'input dell'utente tramite cli. In questo modo,
si può testare ogni interazione, anche complessa, passando un appropriato
flusso di input.
