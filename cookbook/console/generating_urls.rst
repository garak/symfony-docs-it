.. index::
   single: Console; Generare URL

Generare URL con un host personalizzanto nei comandi da console
===============================================================

Sfortunatamente, la linea di comando non sa nulla degli host virtuali o dei
nomi di dominio. Questo vuol dire che, se si generano URL assoluti da un comando
da console, probabilmente si otterrà qualcosa come ``http://localhost/pippo/pluto``,
il che non è molto utile.

Per risolvere il problema, bisogna configrare il "contesto della richiesta", che un modo
particolare per dire che occorre configurare il proprio ambiente, in modo tale che sappia
quale URL va usata per la generazione.

Ci sono due modi per configurare il contesto della richiesta: a livello di applicazione
o per comando.

Configurare il contesto della richiesta globalmente
---------------------------------------------------

.. versionadded:: 2.1
    I parametri ``host`` e ``scheme`` sono disponibili da Symfony 2.1

Per configurare il contesto della richiesta, usato dal generatore di URL, si possono
ridefinire i parametri che usa come valori predefiniti, per cambiare l'host
(localhost) e lo schema (http) predefiniti. Si noti che ciò non impatta sugli URL
generati tramite normali richieste web, poiché sovrascriveranno tali valpori.

.. configuration-block::

    .. code-block:: yaml

        # app/config/parameters.yml
        parameters:
            router.request_context.host: example.org
            router.request_context.scheme: https

    .. code-block:: xml

        <!-- app/config/parameters.xml -->
        <?xml version="1.0" encoding="UTF-8"?>

        <container xmlns="http://symfony.com/schema/dic/services"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance">

            <parameters>
                <parameter key="router.request_context.host">example.org</parameter>
                <parameter key="router.request_context.scheme">https</parameter>
            </parameters>
        </container>

    .. code-block:: php

        // app/config/config_test.php
        $container->setParameter('router.request_context.host', 'example.org');
        $container->setParameter('router.request_context.scheme', 'https');

Configurare il contesto della richiesta per comando
---------------------------------------------------

Per cambiare un singolo comando, si può semplicemente recuperare il servizio del contesto
della richiesta e sovrascrivere le sue impostazioni::

    // src/Acme/DemoBundle/Command/DemoCommand.php

    // ...
    class DemoCommand extends ContainerAwareCommand
    {
        protected function execute(InputInterface $input, OutputInterface $output)
        {
            $context = $this->getContainer()->get('router')->getContext();
            $context->setHost('example.com');
            $context->setScheme('https');

            // ... your code here
        }
    }

