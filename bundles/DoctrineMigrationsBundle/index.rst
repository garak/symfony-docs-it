DoctrineMigrationsBundle
========================

Le migrazioni delle basi dati sono un'estensione del livello di astrazione della base dati
e offrono l'opportunità di fare deploy programmatici di nuove versioni del proprio schema
di base dati in modo sicuro, facile e standardizzato.

.. tip::

    Si possono approfondire le migrazioni di Doctrine nella `documentazione`_ del
    progetto.

Installazione
-------------

Le migrazioni di Doctrine per Symfony sono mantenute in `DoctrineMigrationsBundle`_.
Assicurarsi di avere entrambe le librerie ``doctrine-migrations`` e ``DoctrineMigrationsBundle``
configurate nel proprio progetto. Seguire questi passi per installare le librerie
nella distribuzione Symfony Standard.

Aggiungere i seguenti a ``deps``. Ciò registrarà il bundle delle migrazioni e la libreria
doctrine-migrations come dipendenze della propria applicazione:

.. code-block:: text

    [doctrine-migrations]
        git=http://github.com/doctrine/migrations.git

    [DoctrineMigrationsBundle]
        git=http://github.com/symfony/DoctrineMigrationsBundle.git
        target=/bundles/Symfony/Bundle/DoctrineMigrationsBundle

Aggiornare le librerie dei venditori:

.. code-block:: bash

    $ php bin/vendors install

Quindi, assicurarsi che lo spazio dei nomi ``Doctrine\DBAL\Migrations`` sia caricato
tramite ``autoload.php``. Lo spazio dei nomi ``Migrations`` *deve* essere posto prima
di ``Doctrine\\DBAL``, in modo che l'autoloader cerchi tali classi nella cartella
delle migrazioni:

.. code-block:: php

    // app/autoload.php
    $loader->registerNamespaces(array(
        //...
        'Doctrine\\DBAL\\Migrations' => __DIR__.'/../vendor/doctrine-migrations/lib',
        'Doctrine\\DBAL'             => __DIR__.'/../vendor/doctrine-dbal/lib',
    ));

Infine, assicurarsi di abilitare il bundle in ``AppKernel.php``, includendo il
seguente:

.. code-block:: php

    // app/AppKernel.php
    public function registerBundles()
    {
        $bundles = array(
            //...
            new Symfony\Bundle\DoctrineMigrationsBundle\DoctrineMigrationsBundle(),
        );
    }

Utilizzo
--------

Tutte le funzionalità di migrazione sono contenuto in alcuni comandi:

.. code-block:: bash

    doctrine:migrations
      :diff     Genera una migrazione, confrontando la base dati attuale con le informazioni di mappatura.
      :execute  Esegue una singola migrazione manualmente, in su o in giù.
      :generate Genera una classe migrazione vuota.
      :migrate  Esegue una migrazione a una specifica versione o all'ultima versione disponibile.
      :status   Mostra lo stato di un insieme di migrazioni.
      :version  Aggiunge e cancella manualmente versioni di migrazione nella tabella delle versioni.

Iniziare verificando lo stato delle migrazioni della propria applicazione, eseguendo il
comando ``status``:

.. code-block:: bash

    php app/console doctrine:migrations:status

     == Configuration

        >> Name:                                               Application Migrations
        >> Configuration Source:                               manually configured
        >> Version Table Name:                                 migration_versions
        >> Migrations Namespace:                               Application\Migrations
        >> Migrations Directory:                               /percorso/del/progetto/app/DoctrineMigrations
        >> Current Version:                                    0
        >> Latest Version:                                     0
        >> Executed Migrations:                                0
        >> Available Migrations:                               0
        >> New Migrations:                                     0

Ora si può iniziare a lavorare con le migrazioni, generando una nuova classe migrazione
vuota. Successivamente, si vedrà come Doctrine può generare automaticamente migrazioni
al posto nostro.

.. code-block:: bash

    php app/console doctrine:migrations:generate
    Generated new migration class to "/percorso/del/progetto/app/DoctrineMigrations/Version20100621140655.php"

Aprendo la classe migrazione appena generata, si vedrà qualcosa di simile a
questo::

    namespace Application\Migrations;

    use Doctrine\DBAL\Migrations\AbstractMigration,
        Doctrine\DBAL\Schema\Schema;

    class Version20100621140655 extends AbstractMigration
    {
        public function up(Schema $schema)
        {

        }

        public function down(Schema $schema)
        {

        }
    }

Se si esegue il comando ``status``, esso ora mostrerà che sia ha una nuova migrazione
da eseguire:

.. code-block:: bash

    php app/console doctrine:migrations:status

     == Configuration

       >> Name:                                               Application Migrations
       >> Configuration Source:                               manually configured
       >> Version Table Name:                                 migration_versions
       >> Migrations Namespace:                               Application\Migrations
       >> Migrations Directory:                               /percorso/del/progetto/app/DoctrineMigrations
       >> Current Version:                                    0
       >> Latest Version:                                     2010-06-21 14:06:55 (20100621140655)
       >> Executed Migrations:                                0
       >> Available Migrations:                               1
       >> New Migrations:                                     1

    == Migration Versions

       >> 2010-06-21 14:06:55 (20100621140655)                not migrated

Ora si può aggiungere del codice di migrazione ai metodi ``up()`` e ``down()`` e infine
migrare, quando si è pronti:

.. code-block:: bash

    php app/console doctrine:migrations:migrate

Per ulteriori informazioni su come scrivere le migrazioni (cioè su come riempire i
metodi ``up()`` e ``down()``), si veda la `documentazione`_ ufficiale di Doctrine sulle
migrazioni.

Eseguire migrazioni durante il deploy
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Ovviamente, il fine ultimo della scrittura delle migrazioni è la possibilità di usarle per
aggiornare la struttura della propria base dati al momento del deploy dell'applicazione.
Eseguendo le migrazioni localmente (o su un server di stage), ci si può assicurare che
esse funzionino come ci si aspetta.

Quando infine si esegue il deploy della propria applicazione, occorre solo ricordarsi di
eseguire il comando ``doctrine:migrations:migrate``. Internamente, Doctrine crea
una tabella ``migration_versions`` dentro la propria base dati e traccia le migrazioni
eseguite al suo interno. Quindi, non importa quante migrazioni sono state create ed
eseguite localmente, quando si esegue il comando durante il deploy, Doctrine saprà
esattamente quali migrazioni non sono ancora state eseguite, guardando la tabella
``migration_versions`` della base dati di produzione. Indipendentemente dal server su cui ci
si trova, si può sempre eseguire questo comando senza problemi, per eseguire solo le
migrazioni che non sono ancora state eseguite su *quella* particolare base dati.

Generare automaticamente le migrazioni
--------------------------------------

In realtà, raramente si avrà bisogno di scrivere migrazioni a mano, perché la libreria
delle migrazioni può generare automaticamente le classi delle migrazioni, confrontando
le informazioni di mappatura di Doctine (cioè come la propria base dati *dovrebbe*
essere) con l'attuale struttura della base dati.

Per esempio, si supponga di creare una nuova entità ``User`` e di aggiungere le
informazioni di mappatura per l'ORM di Doctrine:

.. configuration-block::

    .. code-block:: php-annotations

        // src/Acme/HelloBundle/Entity/User.php
        namespace Acme\HelloBundle\Entity;

        use Doctrine\ORM\Mapping as ORM;

        /**
         * @ORM\Entity
         * @ORM\Table(name="hello_user")
         */
        class User
        {
            /**
             * @ORM\Id
             * @ORM\Column(type="integer")
             * @ORM\GeneratedValue(strategy="AUTO")
             */
            protected $id;

            /**
             * @ORM\Column(type="string", length="255")
             */
            protected $name;
        }

    .. code-block:: yaml

        # src/Acme/HelloBundle/Resources/config/doctrine/User.orm.yml
        Acme\HelloBundle\Entity\User:
            type: entity
            table: hello_user
            id:
                id:
                    type: integer
                    generator:
                        strategy: AUTO
            fields:
                name:
                    type: string
                    length: 255

    .. code-block:: xml

        <!-- src/Acme/HelloBundle/Resources/config/doctrine/User.orm.xml -->
        <doctrine-mapping xmlns="http://doctrine-project.org/schemas/orm/doctrine-mapping"
              xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
              xsi:schemaLocation="http://doctrine-project.org/schemas/orm/doctrine-mapping
                            http://doctrine-project.org/schemas/orm/doctrine-mapping.xsd">

            <entity name="Acme\HelloBundle\Entity\User" table="hello_user">
                <id name="id" type="integer" column="id">
                    <generator strategy="AUTO"/>
                </id>
                <field name="name" column="name" type="string" length="255" />
            </entity>

        </doctrine-mapping>

Con queste informazioni, Doctrine è pronto per persistere il nuovo oggetto ``User`` nella
tabella ``hello_user``. Ovviamente, tale tabella non esiste ancora! Generare una nuova
migrazione per tale tabella automaticamente, eseguendo il seguente
comando:

.. code-block:: bash

    php app/console doctrine:migrations:diff

Un messaggio dovrebbe dire che una nuova classe migrazione è stata generata, in base
alle differenze con lo schema. Aprendo questo file, si troverà il codice SQL necessario
per creare la tabella ``hello_user``. Quindi, eseguire la migrazione per aggiungere
la tabella alla propria base dati:

.. code-block:: bash

    php app/console doctrine:migrations:migrate

La morale della favola è: dopo ogni modifica eseguita alle informazioni di mappatura di
Docrtine, eseguire il comando ``doctrine:migrations:diff``, per generare automaticamente
le proprie classi migrazione.

Se lo si fa già dall'inizio del proprio progetto (cioè in modo tale che anche le prime
tabelle siano caricate tramite una classe migrazione), si sarà sempre in grado di
creare una nuova base dati ed eseguire le proprie migrazioni per portare lo schema al
pieno aggiornamento. In effetti, è un modo di lavorare facile e affidabile per il
proprio progetto.

.. _documentazione: http://www.doctrine-project.org/projects/migrations/2.0/docs/reference/introduction/en
.. _DoctrineMigrationsBundle: https://github.com/symfony/DoctrineMigrationsBundle

