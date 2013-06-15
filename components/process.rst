.. index::
   single: Process
   single: Componenti; Process

Il componente Process
=====================

    Il componente Process esegue i comandi nei sotto-processi.

Installazione
-------------

Si può installare il componente in molti modi diversi:

* Usare il repository ufficiale su Git (https://github.com/symfony/Process);
* Installarlo via Composer (``symfony/process`` su `Packagist`_).

Uso
---

La classe :class:`Symfony\\Component\\Process\\Process` consente di eseguire un
comando in un sotto-processo::

    use Symfony\Component\Process\Process;

    $process = new Process('ls -lsa');
    $process->run();

    // eseguito deopo la fine del comando
    if (!$process->isSuccessful()) {
        throw new \RuntimeException($process->getErrorOutput());
    }

    print $process->getOutput();

Il metodo si prende cura delle sottili differenze tra le varie piattaforme, durante
l'esecuzione del comando.

.. versionadded:: 2.2
    I metodi ``getIncrementalOutput()`` e ``getIncrementalErrorOutput()`` sono stati aggiunti in Symfony 2.2.

Il metodo ``getOutput()`` restituisce sempre l'intero contenuto dell'output standard
del comando, mentre ``getErrorOutput()`` restituisce l'intero contenuto dell'output
di errore. In alternativa, i metodi :method:`Symfony\\Component\\Process\\Process::getIncrementalOutput`
e :method:`Symfony\\Component\\Process\\Process::getIncrementalErrorOutput`
restituiscono i nuovi output dall'ultima chiamata.

Output  del processo in tempo reale
-----------------------------------

Quando si esegue un comando che gira a lungo (come la sincronizzazione di file con un
server remoto), si può dare un feedback all'utente finale in tempo reale, passando una
funzione anonima al metodo
:method:`Symfony\\Component\\Process\\Process::run`::

    use Symfony\Component\Process\Process;

    $process = new Process('ls -lsa');
    $process->run(function ($type, $buffer) {
        if (Process::ERR === $type) {
            echo 'ERR > '.$buffer;
        } else {
            echo 'OUT > '.$buffer;
        }
    });

.. versionadded:: 2.1
    La caratteristica di non bloccagio è stata aggiunta in 2.1.

Esecuzione asincrona dei processi
---------------------------------

Si può anche iniziare il sotto-processo e lasciarlo girare in modo asincrono, recuperando
l'output e lo stato nel processo principali, quando occorre. Usare il metodo
:method:`Symfony\\Component\\Process\\Process::start` per iniziare un processo asincrono,
il metodo :method:`Symfony\\Component\\Process\\Process::isRunning` per
verificare che il processo sia finito e il metodo
:method:`Symfony\\Component\\Process\\Process::getOutput` per ottenere l'output::

    $process = new Process('ls -lsa');
    $process->start();
    
    while ($process->isRunning()) {
        // aspetta che il processo finisca
    }

    echo $process->getOutput();
    
Si può anche aspettare che un processo finisca, se è stato fatto partire in modo asincrono e
si sta facendo altro::

    $process = new Process('ls -lsa');
    $process->start();
    
    // ... fare altre cose
    
    $process->wait(function ($type, $buffer) {
        if (Process:ERR === $type) {
            echo 'ERR > '.$buffer;
        } else {
            echo 'OUT > '.$buffer;
        }
    });

Fermare un processo
-------------------

Any asynchronous process can be stopped at any time with the
:method:`Symfony\\Component\\Process\\Process::stop` method. This method takes
a timeout as its argument. Once the timeout is reached, the process is terminated.

    $process = new Process('ls -lsa');
    $process->start();

    // ... do other things

    $process->stop(3);

Executing PHP Code in Isolation
-------------------------------

Se si vuole eseguire del codice PHP in isolamento, usare invece
``PhpProcess``::

    use Symfony\Component\Process\PhpProcess;

    $process = new PhpProcess(<<<EOF
        <?php echo 'Ciao mondo'; ?>
    EOF
    );
    $process->run();

.. versionadded:: 2.1
    La classe ``ProcessBuilder`` è stata aggiunta nella 2.1.

Per far funzionare meglio il proprio codice su tutte le piattaforme, potrebbe essere
preferibile usare la classe :class:`Symfony\\Component\\Process\\ProcessBuilder`::

    use Symfony\Component\Process\ProcessBuilder;

    $builder = new ProcessBuilder(array('ls', '-lsa'));
    $builder->getProcess()->run();

Timeout del processo
--------------------

Si può limitare il tempo a disposizione di un processo per essere completato, impostando
un timeout (in secondi)::

    use Symfony\Component\Process\Process;

    $process = new Process('ls -lsa');
    $process->setTimeout(3600);
    $process->run();

Se questo tempo viene raggiunto, viene lanciata una
:class:`Symfony\\Process\\Exception\\RuntimeException`.

Per comandi che richiedono molto tempo, è responsabilità dello sviluppatore contollare
il timeout a intervalli regolari::

    $process->setTimeout(3600);
    $process->start();

    while ($condition) {
        // ...

        // verifica se è stato raggiunto il timeout
        $process->checkTimeout();

        usleep(200000);
    }

.. _Packagist: https://packagist.org/packages/symfony/process
