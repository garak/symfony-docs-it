.. index::
    single: Helper della console; Helper Dialog

Helper Dialog 
=============

:class:`Symfony\\Component\\Console\\Helper\\DialogHelper` fornisce 
funzioni per chiedere informazioni all'utente. È incluso nell'insieme
predefinito degli helper, ottenibile richiamando
:method:`Symfony\\Component\\Console\\Command\\Command::getHelperSet`::

    $dialog = $this->getHelperSet()->get('dialog');

Tutti i metodi all'interno dell'helper Dialog hanno
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
``true`` se l'utente risponde ``y`` o ``false`` in caso contrario. Il terzo
parametro di ``askConfirmation`` è il valore predefinito da restituire se l'utente
non inserisce alcun input.

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
un nome, che sarà restituito dal metodo ``ask``. Se lasciato vuoto, sarà
restituito il valore predefinito (``AcmeDemoBundle``).

Validare la risposta
--------------------

Si può anche validare la risposta. Per esempio, nell'ultimo esempio è stato
chiesto il nome di un bundle. Seguendo le convenzioni di Symfony2, il nome dovrebbe
avere il suffisso ``Bundle``. Lo si può validare, usando il metodo
:method:`Symfony\\Component\\Console\\Helper\\DialogHelper::askAndValidate`::


    // ...
    $bundle = $dialog->askAndValidate(
        $output,
        'Prego inserire il nome del bundle',
        function ($answer) {
            if ('Bundle' !== substr($answer, -6)) {
                throw new \RunTimeException(
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
        string $default = null
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