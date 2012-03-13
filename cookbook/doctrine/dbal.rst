.. index::
   pair: Doctrine; DBAL

Come usare il livello DBAL di Doctrine
======================================

.. note::

    Questo articolo riguarda il livello DBAL di Doctrine. Di solito si lavora con il livello
    dell'ORM di Doctrine, che è un livello più astratto e usa il DBAL dietro le
    quinte, per comunicare con il database. Per saperne di più sull'ORM
    di Docrine, si veda ":doc:`/book/doctrine`".

Il livello di astrazione del database (Database Abstraction Layer o DBAL) di `Doctrine`_
è un livello posto sopra `PDO`_ e offre un'API intuitiva e flessibile per comunicare
con i database relazionali più diffusi. In altre parole, la libreria DBAL
facilita l'esecuzione delle query ed esegue altre azioni sul database.

.. tip::

    Leggere la documentazione di Doctrine `DBAL Documentation`_ per conoscere tutti i dettagli
    e le capacità della libreria DBAL di Doctrine.

Per iniziare, configurare i parametri di connessione al database:

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        doctrine:
            dbal:
                driver:   pdo_mysql
                dbname:   Symfony2
                user:     root
                password: null
                charset:  UTF8

    .. code-block:: xml

        // app/config/config.xml
        <doctrine:config>
            <doctrine:dbal
                name="default"
                dbname="Symfony2"
                user="root"
                password="null"
                driver="pdo_mysql"
            />
        </doctrine:config>

    .. code-block:: php

        // app/config/config.php
        $container->loadFromExtension('doctrine', array(
            'dbal' => array(
                'driver'    => 'pdo_mysql',
                'dbname'    => 'Symfony2',
                'user'      => 'root',
                'password'  => null,
            ),
        ));

Per un elenco completo delle opzioni di configurazione, vedere :ref:`reference-dbal-configuration`.

Si può quindi accedere alla connessione del DBAL di Doctrine usando il
servizio ``database_connection``:

.. code-block:: php

    class UserController extends Controller
    {
        public function indexAction()
        {
            $conn = $this->get('database_connection');
            $users = $conn->fetchAll('SELECT * FROM users');

            // ...
        }
    }

Registrare tipi di mappatura personalizzati
-------------------------------------------

Si possono registrare tipi di mappatura personalizzati attraverso la configurazione di
Symfony. Saranno aggiunti a tutte le configurazioni configurate. Per maggiori informazioni sui
tipi di mappatura personalizzati, leggere la sezione `Custom Mapping Types`_ della documentazione di Doctrine.

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        doctrine:
            dbal:
                types:
                    custom_first: Acme\HelloBundle\Type\CustomFirst
                    custom_second: Acme\HelloBundle\Type\CustomSecond

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <container xmlns="http://symfony.com/schema/dic/services"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xmlns:doctrine="http://symfony.com/schema/dic/doctrine"
            xsi:schemaLocation="http://symfony.com/schema/dic/services http://symfony.com/schema/dic/services/services-1.0.xsd
                                http://symfony.com/schema/dic/doctrine http://symfony.com/schema/dic/doctrine/doctrine-1.0.xsd">

            <doctrine:config>
                <doctrine:dbal>
                <doctrine:dbal default-connection="default">
                    <doctrine:connection>
                        <doctrine:mapping-type name="enum">string</doctrine:mapping-type>
                    </doctrine:connection>
                </doctrine:dbal>
            </doctrine:config>
        </container>

    .. code-block:: php

        // app/config/config.php
        $container->loadFromExtension('doctrine', array(
            'dbal' => array(
                'connections' => array(
                    'default' => array(
                        'mapping_types' => array(
                            'enum'  => 'string',
                        ),
                    ),
                ),
            ),
        ));

Registrare tipi di mappatura personalizzati in SchemaTool
---------------------------------------------------------

SchemaTool è usato per ispezionare il database per confrontare lo schema. Per assolvere
a questo compito, ha bisogno di sapere quale tipo di mappatura deve essere usato
per ogni tipo di database. Se ne possono registrare di nuovi attraverso la configurazione.

Mappiamo il tipo ENUM (non supportato di base dal DBAL) sul tipo di mappatura
``string``:

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        doctrine:
            dbal:
                connection:
                    default:
                        // Other connections parameters
                        mapping_types:
                            enum: string

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <container xmlns="http://symfony.com/schema/dic/services"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xmlns:doctrine="http://symfony.com/schema/dic/doctrine"
            xsi:schemaLocation="http://symfony.com/schema/dic/services http://symfony.com/schema/dic/services/services-1.0.xsd
                                http://symfony.com/schema/dic/doctrine http://symfony.com/schema/dic/doctrine/doctrine-1.0.xsd">

            <doctrine:config>
                <doctrine:dbal>
                    <doctrine:type name="custom_first" class="Acme\HelloBundle\Type\CustomFirst" />
                    <doctrine:type name="custom_second" class="Acme\HelloBundle\Type\CustomSecond" />
                </doctrine:dbal>
            </doctrine:config>
        </container>

    .. code-block:: php

        // app/config/config.php
        $container->loadFromExtension('doctrine', array(
            'dbal' => array(
                'types' => array(
                    'custom_first'  => 'Acme\HelloBundle\Type\CustomFirst',
                    'custom_second' => 'Acme\HelloBundle\Type\CustomSecond',
                ),
            ),
        ));

.. _`PDO`:           http://www.php.net/pdo
.. _`Doctrine`:      http://www.doctrine-project.org
.. _`DBAL Documentation`: http://docs.doctrine-project.org/projects/doctrine-dbal/en/latest/index.html
.. _`Custom Mapping Types`: http://docs.doctrine-project.org/projects/doctrine-dbal/en/latest/reference/types.html#custom-mapping-types