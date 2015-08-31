.. index::
   pair: Doctrine; DBAL

Usare il livello DBAL di Doctrine
=================================

.. note::

    Questa ricetta riguarda il livello DBAL di Doctrine. Di solito si lavora con il livello
    dell'ORM di Doctrine, che è un livello più astratto e usa il DBAL dietro le
    quinte, per comunicare con la base dati. Per saperne di più sull'ORM
    di Doctrine, si veda ":doc:`/book/doctrine`".

Il livello di astrazione della base dati (Database Abstraction Layer o DBAL) di `Doctrine`_
è un livello posto sopra `PDO`_ e offre un'API intuitiva e flessibile per comunicare
con le basi dati relazionali più diffuse. In altre parole, la libreria DBAL
facilita l'esecuzione delle query ed esegue altre azioni sulla base dati.

.. tip::

    Leggere la documentazione di Doctrine `DBAL Documentation`_ per conoscere tutti i dettagli
    e le capacità della libreria DBAL di Doctrine.

Per iniziare, configurare i parametri di connessione alla base dati:

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        doctrine:
            dbal:
                driver:   pdo_mysql
                dbname:   Symfony
                user:     root
                password: null
                charset:  UTF8
                server_version: 5.6

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <doctrine:config>
            <doctrine:dbal
                name="default"
                dbname="Symfony"
                user="root"
                password="null"
                charset="UTF8"
                server-version="5.6"
                driver="pdo_mysql"
            />
        </doctrine:config>

    .. code-block:: php

        // app/config/config.php
        $container->loadFromExtension('doctrine', array(
            'dbal' => array(
                'driver'    => 'pdo_mysql',
                'dbname'    => 'Symfony',
                'user'      => 'root',
                'password'  => null,
                'charset'   => 'UTF8',
                'server_version' => '5.6',
            ),
        ));

Per un elenco completo delle opzioni di configurazione di DBAL o per sapere come con
connessioni multiple, vedere :ref:`reference-dbal-configuration`.

Si può quindi accedere alla connessione del DBAL di Doctrine usando il
servizio ``database_connection``::

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
                    primo:   AppBundle\Type\Primo
                    secondo: AppBundle\Type\Secondo

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <container xmlns="http://symfony.com/schema/dic/services"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xmlns:doctrine="http://symfony.com/schema/dic/doctrine"
            xsi:schemaLocation="http://symfony.com/schema/dic/services http://symfony.com/schema/dic/services/services-1.0.xsd
                                http://symfony.com/schema/dic/doctrine http://symfony.com/schema/dic/doctrine/doctrine-1.0.xsd">

            <doctrine:config>
                <doctrine:dbal>
                    <doctrine:type name="primo" class="AppBundle\Type\Primo" />
                    <doctrine:type name="secondo" class="AppBundle\Type\Secondo" />
                </doctrine:dbal>
            </doctrine:config>
        </container>

    .. code-block:: php

        // app/config/config.php
        $container->loadFromExtension('doctrine', array(
            'dbal' => array(
                'types' => array(
                    'primo'   => 'AppBundle\Type\Primo',
                    'secondo' => 'AppBundle\Type\Secondo',
                ),
            ),
        ));

Registrare tipi di mappatura personalizzati in SchemaTool
---------------------------------------------------------

SchemaTool è usato per ispezionare la base dati per confrontare lo schema. Per assolvere
a questo compito, ha bisogno di sapere quale tipo di mappatura deve essere usato
per ogni tipo di base dati. Se ne possono registrare di nuovi attraverso la configurazione.

Mappiamo il tipo ENUM (non supportato di base dal DBAL) sul tipo di mappatura
``string``:

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        doctrine:
            dbal:
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
                     <doctrine:mapping-type name="enum">string</doctrine:mapping-type>
                </doctrine:dbal>
            </doctrine:config>
        </container>

    .. code-block:: php

        // app/config/config.php
        $container->loadFromExtension('doctrine', array(
            'dbal' => array(
               'mapping_types' => array(
                  'enum'  => 'string',
               ),
            ),
        ));

.. _`PDO`:           http://php.net/manual/it/book.pdo.php
.. _`Doctrine`:      http://www.doctrine-project.org
.. _`DBAL Documentation`: http://docs.doctrine-project.org/projects/doctrine-dbal/en/latest/index.html
.. _`Custom Mapping Types`: http://docs.doctrine-project.org/projects/doctrine-dbal/en/latest/reference/types.html#custom-mapping-types
