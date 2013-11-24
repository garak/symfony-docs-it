.. index::
   single: Doctrine; Gestori di entità multipli

Lavorare con gestori di entità multipli
=======================================

Si possono usare gestori di entità multipli in un'applicazione Symfony2.
Questo si rende necessario quando si usano diverse basi dati o addirittura venditori
con insiemi di entità completamente differenti. In altre parole, un gestore di entità
che si connette a una base dati gestirà alcune entità, mentre un altro gestore di entità
che si connette a un'altra base dati potrebbe gestire il resto.

.. note::

    L'uso di molti gestori di entità è facile, ma più avanzato e solitamente non
    richiesto. Assicurarsi di avere effettivamente bisogno di gestori di entità
    multipli, prima di aggiungere un tale livello di complessità.

La configurazione seguente mostra come configurare due gestori di entità:

.. configuration-block::

    .. code-block:: yaml

        doctrine:
            dbal:
                default_connection:   default
                connections:
                    default:
                        driver:   %database_driver%
                        host:     %database_host%
                        port:     %database_port%
                        dbname:   %database_name%
                        user:     %database_user%
                        password: %database_password%
                        charset:  UTF8
                    customer:
                        driver:   %database_driver2%
                        host:     %database_host2%
                        port:     %database_port2%
                        dbname:   %database_name2%
                        user:     %database_user2%
                        password: %database_password2%
                        charset:  UTF8

            orm:
                default_entity_manager:   default
                entity_managers:
                    default:
                        connection:       default
                        mappings:
                            AcmeDemoBundle: ~
                            AcmeStoreBundle: ~
                    customer:
                        connection:       customer
                        mappings:
                            AcmeCustomerBundle: ~

    .. code-block:: xml

        <?xml version="1.0" encoding="UTF-8"?>

        <srv:container xmlns="http://symfony.com/schema/dic/doctrine"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xmlns:srv="http://symfony.com/schema/dic/services"
            xsi:schemaLocation="http://symfony.com/schema/dic/services http://symfony.com/schema/dic/services/services-1.0.xsd
                                http://symfony.com/schema/dic/doctrine http://symfony.com/schema/dic/doctrine/doctrine-1.0.xsd">

            <config>
                <dbal default-connection="default">
                    <connection name="default"
                        driver="%database_driver%"
                        host="%database_host%"
                        port="%database_port%"
                        dbname="%database_name%"
                        user="%database_user%"
                        password="%database_password%"
                        charset="UTF8"
                    />

                    <connection name="customer"
                        driver="%database_driver2%"
                        host="%database_host2%"
                        port="%database_port2%"
                        dbname="%database_name2%"
                        user="%database_user2%"
                        password="%database_password2%"
                        charset="UTF8"
                    />
                </dbal>

                <orm default-entity-manager="default">
                    <entity-manager name="default" connection="default">
                        <mapping name="AcmeDemoBundle" />
                        <mapping name="AcmeStoreBundle" />
                    </entity-manager>

                    <entity-manager name="customer" connection="customer">
                        <mapping name="AcmeCustomerBundle" />
                    </entity-manager>
                </orm>
            </config>
        </srv:container>

    .. code-block:: php

        $container->loadFromExtension('doctrine', array(
            'dbal' => array(
                'default_connection' => 'default',
                'connections' => array(
                    'default' => array(
                        'driver'   => '%database_driver%',
                        'host'     => '%database_host%',
                        'port'     => '%database_port%',
                        'dbname'   => '%database_name%',
                        'user'     => '%database_user%',
                        'password' => '%database_password%',
                        'charset'  => 'UTF8',
                    ),
                    'customer' => array(
                        'driver'   => '%database_driver2%',
                        'host'     => '%database_host2%',
                        'port'     => '%database_port2%',
                        'dbname'   => '%database_name2%',
                        'user'     => '%database_user2%',
                        'password' => '%database_password2%',
                        'charset'  => 'UTF8',
                    ),
                ),
            ),

            'orm' => array(
                'default_entity_manager' => 'default',
                'entity_managers' => array(
                    'default' => array(
                        'connection' => 'default',
                        'mappings'   => array(
                            'AcmeDemoBundle'  => null,
                            'AcmeStoreBundle' => null,
                        ),
                    ),
                    'customer' => array(
                        'connection' => 'customer',
                        'mappings'   => array(
                            'AcmeCustomerBundle' => null,
                        ),
                    ),
                ),
            ),
        ));

In questo caso, sono stati definiti due gestori di entità, chiamati ``default``
e ``customer``. Il gestore di entità ``default`` gestisce le entità in
``AcmeDemoBundle`` e ``AcmeStoreBundle``, mentre il gestore di entità ``customer``
gestisce le entità in ``AcmeCustomerBundle``. Sono state definite anche due
connessioni, una per ogni gestore di entità.

.. note::

    Lavorando con più connessioni e gestori di entità, si dovrebbe esplicitare
    la configurazione desiderata. Se si *omette* il nome della connessione
    o del gestore di entità, verrà usato quello predefinito (cioè ``default``).

Lavorando con connessioni multiple, per creare le basi dati::

.. code-block:: bash

    # Usa solo la connessione "default"
    $ php app/console doctrine:database:create

    # Usa solo la connessione "customer"
    $ php app/console doctrine:database:create --connection=customer

Lavorando con gestori di entità multipli, per aggiornare lo schema::

.. code-block:: bash

    # Usa solo la mappatura "default"
    $ php app/console doctrine:schema:update --force

    # Usa solo la mappatura "customer"
    $ php app/console doctrine:schema:update --force --em=customer

Se si *omette* il nome del gestore di entità quando lo si richiede,
si otterrà il gestore di entità predefinito (cioè ``default``)::

    class UserController extends Controller
    {
        public function indexAction()
        {
            // entrambi restiuiscono il gestore "default"
            $em = $this->get('doctrine')->getManager();
            $em = $this->get('doctrine')->getManager('default');

            $customerEm =  $this->get('doctrine')->getManager('customer');
        }
    }

Si può ora usare Doctrine come prima, usando il gestore di entità ``default`` per
persistere e recuperare le entità da esso gestite e il gestore di entità
``customer`` per persistere e recuperare le sue entità.

Lo stesso principio si applica alle chiamate ai repository::

    class UserController extends Controller
    {
        public function indexAction()
        {
            // Recupera un repository gestito da "default"
            $products = $this->get('doctrine')
                ->getRepository('AcmeStoreBundle:Product')
                ->findAll()
            ;

            // Esplicita la richiesta a "default"
            $products = $this->get('doctrine')
                ->getRepository('AcmeStoreBundle:Product', 'default')
                ->findAll()
            ;

            // Recupera un repository gestito da "customer"
            $customers = $this->get('doctrine')
                ->getRepository('AcmeCustomerBundle:Customer', 'customer')
                ->findAll()
            ;
        }
    }
