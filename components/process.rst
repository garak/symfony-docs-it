.. index::
   single: Process
   single: Componenti; Process

Il componente Process
=====================

    Il componente Process esegue i comandi nei sotto-processi.

Installazione
-------------

Si può installare il componente in due modi:

* Installarlo tramite :doc:`Composer </components/using_components>` (``symfony/process`` su `Packagist`_);
* Usare il repository ufficiale su Git (https://github.com/symfony/Process).

.. include:: /components/require_autoload.rst.inc

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
    I metodi ``getIncrementalOutput()`` e ``getIncrementalErrorOutput()`` sono stati
    aggiunti in Symfony 2.2.

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
    La caratteristica di non bloccaggio è stata aggiunta in 2.1.

Esecuzione asincrona dei processi
---------------------------------

Si può anche iniziare il sotto-processo e lasciarlo girare in modo asincrono, recuperando
l'output e lo stato nel processo principale, quando occorre. Usare il metodo
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
        if (Process::ERR === $type) {
            echo 'ERR > '.$buffer;
        } else {
            echo 'OUT > '.$buffer;
        }
    });

.. note::

    Il metodo :method:`Symfony\\Component\\Process\\Process::wait` è bloccante,
    il che vuol dire che il codice si fermerà a quella linea, finché il processo esterno
    non sarà finito.

Fermare un processo
-------------------

.. versionadded:: 2.3
    Il parametro ``signal`` del metodo ``stop`` è stato aggiunto in Symfony 2.3.

Ogni processo asincrono può essere fermato in qualsiasi momento, con il metodo
:method:`Symfony\\Component\\Process\\Process::stop`. Questo metodo accetta
due parametri: una scadenza e un segnale. Una volta raggiunta la scadenza, il segnale
viene inviato al processo in esecuzione. Il segnale predefinito inviato al processo è ``SIGKILL``.
Si legga la :ref:`documentazione sui segnali<reference-process-signal>`
per approfondire la gestione dei segnali nel componente Process::

    $process = new Process('ls -lsa');
    $process->start();

    // ... fare altre cose

    $process->stop(3, SIGINT);

Eseguire codice PHP in isolamento
---------------------------------

Se si vuole eseguire del codice PHP in isolamento, usare invece
``PhpProcess``::

    use Symfony\Component\Process\PhpProcess;

    $process = new PhpProcess(<<<EOF
        <?php echo 'Ciao mondo'; ?>
    EOF
    );
    $process->run();

Per far funzionare meglio il proprio codice su tutte le piattaforme, potrebbe essere
preferibile usare la classe :class:`Symfony\\Component\\Process\\ProcessBuilder`::

    use Symfony\Component\Process\ProcessBuilder;

    $builder = new ProcessBuilder(array('ls', '-lsa'));
    $builder->getProcess()->run();

.. versionadded:: 2.3
    Il metodo :method:`ProcessBuilder::setPrefix<Symfony\\Component\\Process\\ProcessBuilder::setPrefix>`
    è stato aggiunto in Symfony 2.3.

Se si sta costruendo un driver binario, si può usare il metodo
:method:`Symfony\\Component\\Process\\Process::setPrefix` per prefissare tutti
i comandi del processo generato.

L'esempio seguente genererà due comandi di processo per un adattatore binario
di tar::

    use Symfony\Component\Process\ProcessBuilder;

    $builder = new ProcessBuilder();
    $builder->setPrefix('/usr/bin/tar');

    // '/usr/bin/tar' '--list' '--file=archive.tar.gz'
    echo $builder
        ->setArguments(array('--list', '--file=archive.tar.gz'))
        ->getProcess()
        ->getCommandLine();

    // '/usr/bin/tar' '-xzf' 'archive.tar.gz'
    echo $builder
        ->setArguments(array('-xzf', 'archive.tar.gz'))
        ->getProcess()
        ->getCommandLine();

Scadenza del processo
---------------------

Si può limitare il tempo a disposizione di un processo per essere completato, impostando
una scadenza (in secondi)::

    use Symfony\Component\Process\Process;

    $process = new Process('ls -lsa');
    $process->setTimeout(3600);
    $process->run();

Se questo tempo viene raggiunto, viene lanciata una
:class:`Symfony\\Process\\Exception\\RuntimeException`.

Per comandi che richiedono molto tempo, è responsabilità dello sviluppatore controllare
il timeout a intervalli regolari::

    $process->setTimeout(3600);
    $process->start();

    while ($condition) {
        // ...

        // verifica se è stato raggiunto il timeout
        $process->checkTimeout();

        usleep(200000);
    }

.. _reference-process-signal:

Segnali di processo
-------------------

.. versionadded:: 2.3
    Il metodo ``signal`` è stato aggiunto in Symfony 2.3.

Durante l'esecuzione di un programma asincrono, si possono inviare segnali posix, con il metodo
:method:`Symfony\\Component\\Process\\Process::signal`::

    use Symfony\Component\Process\Process;

    $process = new Process('find / -name "rabbit"');
    $process->start();

    // invierà un SIGKILL al processo
    $process->signal(SIGKILL);

.. caution::

    A causa di alcune limitazioni in PHP,  se si usano segnali con il componente Process,
    potrebbe essere necessario prefissare i comandi con `exec`_. Si leggano la
    `issue #5759 di Symfony`_ e il `bug #39992 di PHP`_ per capire perché questo accada.

    I segnali POSIX non sono disponibili su piattaforme Windows, si faccia riferimento
    alla `documentazione di PHP`_ per i segnali disponibili.

Pid del processo
----------------

.. versionadded:: 2.3
    Il metodo ``getPid`` è stato aggiunto in Symfony 2.3.

Si può avere accesso al `pid`_ di un processo in esecuzione, con il metodo
:method:`Symfony\\Component\\Process\\Process::getPid`.

.. code-block:: php

    use Symfony\Component\Process\Process;

    $process = new Process('/usr/bin/php worker.php');
    $process->start();

    $pid = $process->getPid();

.. caution::

    A causa di alcune limitazioni in PHP, se si vuole ottenere il pid di un processo,
    potrebbe essere necessario prefissare i comandi con `exec`_. Si legga la
    `issue #5759 di Symfony`_ per capire perché questo accada.

.. _`issue #5759 di Symfony`: https://github.com/symfony/symfony/issues/5759
.. _`bug #39992 di PHP`: https://bugs.php.net/bug.php?id=39992
.. _`exec`: http://en.wikipedia.org/wiki/Exec_(operating_system)
.. _`pid`: http://en.wikipedia.org/wiki/Process_identifier
.. _`documentazione di PHP`: http://php.net/manual/it/pcntl.constants.php
.. _Packagist: https://packagist.org/packages/symfony/process
