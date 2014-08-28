.. index::
   single: Sicurezza; Limitare firewall a una richiesta

Limitare firewall a una specifica richiesta
===========================================

Usando il componente Security, si possono creare firewall che corrispondono ad alcune opzioni della richiesta.
In molti casi, è sufficiente una corrispondenza sugli URL, ma in altri casi speciali si può limitare
ulteriormente l'inizializzazione di un firewall, usando altre opzioni della richiesta.

.. note::

    Si possono usare tali restrizioni singolarmente oppure mescolarle insieme, per ottenere
    la configurazione desiderata 

Limitazione per schema
----------------------

Questa è la limitazione predefinita e limita un firewall a essere inizializzato solo se l'URL richiesto
corrisponde all'opzione ``pattern``. 

.. configuration-block::

    .. code-block:: yaml

        # app/config/security.yml

        # ...
        security:
            firewalls:
                secured_area:
                    pattern: ^/admin
                    # ...

    .. code-block:: xml

        <!-- app/config/security.xml -->
        <?xml version="1.0" encoding="UTF-8"?>
        <srv:container xmlns="http://symfony.com/schema/dic/security"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xmlns:srv="http://symfony.com/schema/dic/services"
            xsi:schemaLocation="http://symfony.com/schema/dic/services
                http://symfony.com/schema/dic/services/services-1.0.xsd">

            <config>
                <!-- ... -->
                <firewall name="secured_area" pattern="^/admin">
                    <!-- ... -->
                </firewall>
            </config>
        </srv:container>

    .. code-block:: php

        // app/config/security.php

        // ...
        $container->loadFromExtension('security', array(
            'firewalls' => array(
                'secured_area' => array(
                    'pattern' => '^/admin',
                    // ...
                ),
            ),
        ));

L'opzione ``pattern`` è un'espressione regolare. In questo esempio, il firewall sarà
attivato solo se l'URL inizia per ``/admin`` (essendo il carattere ``^``). Se
l'URL non soddisfa questo schema, il firewall non sarà attivato ed eventuali
firewall successivi avranno la possibilità di soddisfare la richiesta.

Limitazione per host
--------------------

.. versionadded:: 2.4
    Il supporto per limitare i firewall a specifici host è stato introdotto in
    Symfony 2.4.

Se la corrispondenza su ``pattern`` non è sufficiente, si può far corrispondere la richiesta su
``host``. Se si imposta l'opzione ``host``, il firewall sarà attivato solo
se l'host da cui la richiesta proviene corrisponde a quello configurato.

.. configuration-block::

    .. code-block:: yaml

        # app/config/security.yml

        # ...
        security:
            firewalls:
                secured_area:
                    host: ^admin\.example\.com$
                    # ...

    .. code-block:: xml

        <!-- app/config/security.xml -->
        <?xml version="1.0" encoding="UTF-8"?>
        <srv:container xmlns="http://symfony.com/schema/dic/security"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xmlns:srv="http://symfony.com/schema/dic/services"
            xsi:schemaLocation="http://symfony.com/schema/dic/services
                http://symfony.com/schema/dic/services/services-1.0.xsd">

            <config>
                <!-- ... -->
                <firewall name="secured_area" host="^admin\.example\.com$">
                    <!-- ... -->
                </firewall>
            </config>
        </srv:container>

    .. code-block:: php

        // app/config/security.php

        // ...
        $container->loadFromExtension('security', array(
            'firewalls' => array(
                'secured_area' => array(
                    'host' => '^admin\.example\.com$',
                    // ...
                ),
            ),
        ));

Come ``pattern``, anche ``host`` è un'espressione regolare. In questo esempio,
il firewall sarà attivato solo se l'host è uguale a ``admin.example.com``,
essendo presenti i caratteri ``^`` e ``$``. Se il nome dell'host non soddisfa
questo schema,il firewall non sarà attivato ed eventuali firewall
successivi avranno la possibilità di soddisfare la
richiesta.

Limitazione per metodi HTTP
---------------------------

.. versionadded:: 2.5
    Il supporto per limitare i firewall a specifici metodi HTTP è stato introdotto in
    Symfony 2.4.

L'opzione ``methods`` limita l'inizializzazione di un firewall ai metodi
HTTP specificati.

.. configuration-block::

    .. code-block:: yaml

        # app/config/security.yml

        # ...
        security:
            firewalls:
                secured_area:
                    methods: [GET, POST]
                    # ...

    .. code-block:: xml

        <!-- app/config/security.xml -->
        <?xml version="1.0" encoding="UTF-8"?>
        <srv:container xmlns="http://symfony.com/schema/dic/security"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xmlns:srv="http://symfony.com/schema/dic/services"
            xsi:schemaLocation="http://symfony.com/schema/dic/services
                http://symfony.com/schema/dic/services/services-1.0.xsd">

            <config>
                <!-- ... -->
                <firewall name="secured_area" methods="GET,POST">
                    <!-- ... -->
                </firewall>
            </config>
        </srv:container>

    .. code-block:: php

        // app/config/security.php

        // ...
        $container->loadFromExtension('security', array(
            'firewalls' => array(
                'secured_area' => array(
                    'methods' => array('GET', 'POST'),
                    // ...
                ),
            ),
        ));

In questo esempio, il firewall sarà attivato solo il metodo HTTP della richiesta
è ``GET`` o ``POST``. Se il metodo non rientra tra quelli specificati,
il firewall non sarà attivato ed eventuali firewall successivi
avranno la possibilità di soddisfare la richiesta.
