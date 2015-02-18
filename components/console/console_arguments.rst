.. index::
    single: Console; Parametri della console

Capire come sono gestiti i parametri della console
==================================================

Potrebbe risultare difficile capire il modo in cui le applicazione di console gestiscono i parametri.
Un'applicazione di console Symony, come molti altri strumenti di CLI, segue il comportamento
descritto negli standard `docopt`_.

Diamo uno sguardo al seguente comando, che ha tre opzioni::

    namespace Acme\Console\Command;

    use Symfony\Component\Console\Command\Command;
    use Symfony\Component\Console\Input\InputArgument;
    use Symfony\Component\Console\Input\InputInterface;
    use Symfony\Component\Console\Input\InputOption;
    use Symfony\Component\Console\Output\OutputInterface;

    class DemoArgsCommand extends Command
    {
        protected function configure()
        {
            $this
                ->setName('demo:args')
                ->setDescription('Descrive i comportamenti dei parametri')
                ->setDefinition(
                    new InputDefinition(array(
                        new InputOption('foo', 'f'),
                        new InputOption('bar', 'b', InputOption::VALUE_REQUIRED),
                        new InputOption('cat', 'c', InputOption::VALUE_OPTIONAL),
                    ))
                );
        }

        protected function execute(InputInterface $input, OutputInterface $output)
        {
           // ...
        }
    }

Poiché l'opzione ``foo`` non accetta valori, sarà o ``false``
(se non passata al comando) o ``true`` (se l'utente passa l'opzione ``--foo``).
Il valore dell'opzione ``bar`` (o della scorciatoia ``b``)
è obbligatorio. Può essere separato dal nome dell'opzione da spazi o dal carattere
``=``. L'opzione ``cat`` (e la sua scorciatoia ``c``) si comporta in modo simile,
ma non ha un valore obbligatorio. Uno sguardo alla seguente tabella offre
una panoramica dei modi possibili di passare queste opzioni:

===================== ========= =========== ============
Input                 ``foo``   ``bar``     ``cat``
===================== ========= =========== ============
``--bar=Hello``       ``false`` ``"Hello"`` ``null``
``--bar Hello``       ``false`` ``"Hello"`` ``null``
``-b=Hello``          ``false`` ``"Hello"`` ``null``
``-b Hello``          ``false`` ``"Hello"`` ``null``
``-bHello``           ``false`` ``"Hello"`` ``null``
``-fcWorld -b Hello`` ``true``  ``"Hello"`` ``"World"``
``-cfWorld -b Hello`` ``false`` ``"Hello"`` ``"fWorld"``
``-cbWorld``          ``false`` ``null``    ``"bWorld"``
===================== ========= =========== ============

Le cose si complicano un po' se il comando accetta anche un parametro
facoltativo::

    // ...

    new InputDefinition(array(
        // ...
        new InputArgument('arg', InputArgument::OPTIONAL),
    ));

Si potrebbe dover usare il separatore ``--`` per separare le opzioni dai
parametri. Diamo uno sguardo al quinto esempio nella tabella seguente, in cui
è usato per dire al comando che ``World`` è il valore di ``arg`` e non il
valore dell'opzione facoltativa ``cat``:

============================== ================= =========== ===========
Input                          ``bar``           ``cat``     ``arg``
============================== ================= =========== ===========
``--bar Hello``                ``"Hello"``       ``null``    ``null``
``--bar Hello World``          ``"Hello"``       ``null``    ``"World"``
``--bar "Hello World"``        ``"Hello World"`` ``null``    ``null``
``--bar Hello --cat World``    ``"Hello"``       ``"World"`` ``null``
``--bar Hello --cat -- World`` ``"Hello"``       ``null``    ``"World"``
``-b Hello -c World``          ``"Hello"``       ``"World"`` ``null``
============================== ================= =========== ===========

.. _docopt: http://docopt.org/
