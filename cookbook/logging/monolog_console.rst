.. index::
   single: Log; Messaggi di console

Configurare Monolog per mostrare messaggi di console
====================================================

.. versionadded:: 2.4
    Questa caratteristica è stata introdotta in MonologBridge in Symfony 2.4.

Si può usare la console per mostrare messaggi per determinati
:ref:`livelli di verbosità <verbosity-levels>`, usando l'istanza di
:class:`Symfony\\Component\\Console\\Output\\OutputInterface` che è stata
passata durante l'esecuzione di un comando.

Quando ci sono molti log, non è facile mostrare informazioni
a seconda delle impostazioni di vebosità (``-v``, ``-vv``, ``-vvv``), perché
occorre inserire le chiamate all'interno di condizioni. Il codice diventa velocemente prolisso o sporco.
Per esempio::

    use Symfony\Component\Console\Input\InputInterface;
    use Symfony\Component\Console\Output\OutputInterface;

    protected function execute(InputInterface $input, OutputInterface $output)
    {
        if ($output->getVerbosity() >= OutputInterface::VERBOSITY_DEBUG) {
            $output->writeln('Un\'informazione');
        }

        if ($output->getVerbosity() >= OutputInterface::VERBOSITY_VERBOSE) {
            $output->writeln('Un\'altra informazione');
        }
    }

Invece di usare questi metodi semantici per verificare ogni livello di
verbosità, `MonologBridge` fornisce `ConsoleHandler`_, che ascolta gli
eventi di console e scrive messaggi di errore nell'output della console a seconda
del livello corrente di log e della verbosità della console.

L'esempio precedente può essere riscritto come::

    use Symfony\Component\Console\Input\InputInterface;
    use Symfony\Component\Console\Output\OutputInterface;

    protected function execute(InputInterface $input, OutputInterface $output)
    {
        // ipotizzando che Command estenda ContainerAwareCommand...
        $logger = $this->getContainer()->get('logger');
        $logger->debug('Un\'informazione');

        $logger->notice('Un\'altra informazione');
    }

A seconda del livello di verbosità con cui gira il comando e della configurazione
dell'utente (vedere sotto), questi messaggi possono essere o non essere mostrati
in console. Se sono mostrati, hanno data/ora e colore adeguato.
Inoltre, i log di errore sono scritti nell'output di errore (php://stderr).
Non c'è più bisogno di gestire in modo dinamico le impostazioni di verbosità.

Il gestore di console di Monolog è abilitato nella configurazione di Monolog. Questo è
il predefinito anche in Symfony Standard Edition.

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        monolog:
            handlers:
                console:
                    type: console

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <?xml version="1.0" encoding="UTF-8" ?>
        <container xmlns="http://symfony.com/schema/dic/services"
            xmlns:monolog="http://symfony.com/schema/dic/monolog">

            <monolog:config>
                <monolog:handler name="console" type="console" />
            </monolog:config>
        </container>

    .. code-block:: php

        // app/config/config.php
        $container->loadFromExtension('monolog', array(
            'handlers' => array(
                'console' => array(
                   'type' => 'console',
                ),
            ),
        ));

Con l'opzione ``verbosity_levels`` si può adattare la mappatura tra
verbosità e livello di log. Nell'esempio fornito mostrerà anche i notice in
modalità verbosa normale (invece che solo i warning). Inoltre, usa solo
messaggi di log con il canale personalizzato ``mio_canale`` e cambia lo
stile di visualizzazione tramite un formattatore personalizzato (vedere il
:doc:`riferimento di MonologBundle </reference/configuration/monolog>` per maggiori
informazioni):

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        monolog:
            handlers:
                console:
                    type:   console
                    verbosity_levels:
                        VERBOSITY_NORMAL: NOTICE
                    channels: mio_canale
                    formatter: mio_formattatore

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <?xml version="1.0" encoding="UTF-8" ?>
        <container xmlns="http://symfony.com/schema/dic/services"
            xmlns:monolog="http://symfony.com/schema/dic/monolog">

            <monolog:config>
                <monolog:handler name="console" type="console" formatter="mio_formattatore">
                    <monolog:verbosity-level verbosity-normal="NOTICE" />
                    <monolog:channel>mio_canale</monolog:channel>
                </monolog:handler>
            </monolog:config>
        </container>

    .. code-block:: php

        // app/config/config.php
        $container->loadFromExtension('monolog', array(
            'handlers' => array(
                'console' => array(
                    'type' => 'console',
                    'verbosity_levels' => array(
                        'VERBOSITY_NORMAL' => 'NOTICE',
                    ),
                    'channels' => 'mio_canale',
                    'formatter' => 'mio_formattatore',
                ),
            ),
        ));

.. configuration-block::

    .. code-block:: yaml

        # app/config/services.yml
        services:
            mio_formattatore:
                class: Symfony\Bridge\Monolog\Formatter\ConsoleFormatter
                arguments:
                    - "[%%datetime%%] %%start_tag%%%%message%%%%end_tag%% (%%level_name%%) %%context%% %%extra%%\n"

    .. code-block:: xml

        <!-- app/config/services.xml -->
        <?xml version="1.0" encoding="UTF-8" ?>
        <container xmlns="http://symfony.com/schema/dic/services"
                   xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
                   xsi:schemaLocation="http://symfony.com/schema/dic/services http://symfony.com/schema/dic/services/services-1.0.xsd">

             <services>
                <service id="mio_formattatore" class="Symfony\Bridge\Monolog\Formatter\ConsoleFormatter">
                    <argument>[%%datetime%%] %%start_tag%%%%message%%%%end_tag%% (%%level_name%%) %%context%% %%extra%%\n</argument>
                </service>
             </services>

        </container>

    .. code-block:: php

        // app/config/services.php
        $container
            ->register('mio_formattatore', 'Symfony\Bridge\Monolog\Formatter\ConsoleFormatter')
            ->addArgument('[%%datetime%%] %%start_tag%%%%message%%%%end_tag%% (%%level_name%%) %%context%% %%extra%%\n')
        ;

.. _ConsoleHandler: https://github.com/symfony/MonologBridge/blob/master/Handler/ConsoleHandler.php
.. _MonologBridge: https://github.com/symfony/MonologBridge
