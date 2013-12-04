.. index::
   single: Log
   single: Log; Escludere errori 404
   single: Monolog; Escludere errori 404

Configurare Monolog per escludere gli errori 404 dal log
========================================================

.. versionadded:: 2.3
    Questa caratteristica è stata introdotta in MonologBundle versione 2.4. Questa
    versione è compatibile con Symfony 2.3, ma la versione predefinita installata è
    MonologBundle 2.3. Per usare questa caratteristica, aggiornare il bundle a mano.

A volte, i log si riempiono di errori HTTP 404 indesiderati, per esempio se
un attaccante esegue uno scan dell'applicazione, cercando percorsi noti (come
`/phpmyadmin`). Quando si usa il gestore ``fingers_crossed``, si possono escludere
dai log questi errori 404, in base a espressioni regolari nella configurazione
di MonologBundle:

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        monolog:
            handlers:
                main:
                    # ...
                    type: fingers_crossed
                    handler: ...
                    excluded_404s:
                        - ^/phpmyadmin

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <container xmlns="http://symfony.com/schema/dic/services"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xmlns:monolog="http://symfony.com/schema/dic/monolog"
            xsi:schemaLocation="http://symfony.com/schema/dic/services
                http://symfony.com/schema/dic/services/services-1.0.xsd
                http://symfony.com/schema/dic/monolog
                http://symfony.com/schema/dic/monolog/monolog-1.0.xsd"
        >
            <monolog:config>
                <monolog:handler type="fingers_crossed" name="main" handler="...">
                    <!-- ... -->
                    <monolog:excluded-404>^/phpmyadmin</monolog:excluded-404>
                </monolog:handler>
            </monolog:config>
        </container>

    .. code-block:: php

        // app/config/config.php
        $container->loadFromExtension('monolog', array(
            'handlers' => array(
                'main' => array(
                    // ...
                    'type'          => 'fingers_crossed',
                    'handler'       => ...,
                    'excluded_404s' => array(
                        '^/phpmyadmin',
                    ),
                ),
            ),
        ));
