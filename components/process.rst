.. index::
   single: Process

Il componente Process
=====================

    Il componente Process esegue i comandi nei sotto-processi.

Installazione
-------------

Si può installare il componente in molti modi diversi:

* Usare il repository ufficiale su Git (https://github.com/symfony/Process);
* Installarlo via PEAR (`pear.symfony.com/Process`);
* Installarlo via Composer (`symfony/process` su Packagist).

Uso
---

La classe :class:`Symfony\\Component\\Process\\Process` consente di eseguire un
comando in un sotto-processo::

    use Symfony\Component\Process\Process;

    $process = new Process('ls -lsa');
    $process->setTimeout(3600);
    $process->run();
    if (!$process->isSuccessful()) {
        throw new RuntimeException($process->getErrorOutput());
    }

    print $process->getOutput();

Il metodo :method:`Symfony\\Component\\Process\\Process::run` si prende cura delle
sottili differenze tra le varie piattaforme, durante l'esecuzione del
comando.

Quando si esegue un comando che gira a lungo (come la sincronizzazione di file con un
server remoto), si può dare un feedback all'utente finale in tempo reale, passando una
funzione anonima al metodo
:method:`Symfony\\Component\\Process\\Process::run`::

    use Symfony\Component\Process\Process;

    $process = new Process('ls -lsa');
    $process->run(function ($type, $buffer) {
        if ('err' === $type) {
            echo 'ERR > '.$buffer;
        } else {
            echo 'OUT > '.$buffer;
        }
    });

Se si vuole eseguire del codice PHP in isolamento, usare invece
``PhpProcess``::

    use Symfony\Component\Process\PhpProcess;

    $process = new PhpProcess(<<<EOF
        <?php echo 'Ciao mondo'; ?>
    EOF);
    $process->run();

.. versionadded:: 2.1
    La classe ``ProcessBuilder`` è stata aggiunta nella 2.1.

Per far funzionare meglio il proprio codice su tutte le piattaforme, potrebbe essere
preferibile usare la classe :class:`Symfony\\Component\\Process\\ProcessBuilder`::

    use Symfony\Component\Process\ProcessBuilder;

    $builder = new ProcessBuilder(array('ls', '-lsa'));
    $builder->getProcess()->run();
