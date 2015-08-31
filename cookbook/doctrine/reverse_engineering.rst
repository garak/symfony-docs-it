.. index::
   single: Doctrine; Generare entità da una base dati esistente

Generare entità da una base dati esistente
==========================================

Quando si inizia a lavorare su un nuovo progetto, che usa una base dati, si pongono
due situazioni diverse. Nella maggior parte dei casi, il modello della base dati è
progettato e costruito da zero. A volte, tuttavia, si inizia con un modello di base
dati esistente e probabilmente non modificabile. Per fortuna, Doctrine dispone di molti
strumenti che aiutano a generare classi del modello da una base dati esistente.

.. note::

    Come dice la `documentazione sugli strumenti di Doctrine`_, il reverse engineering
    è un processo da eseguire una sola volta su un progetto. Doctrine è in grado di
    convertire circa il 70-80% delle informazioni di mappatura necessarie, in base a
    campi, indici e vincoli di integrità referenziale. Doctrine non può scoprire le
    associazioni inverse, i tipi di ereditarietà, le entità con chiavi esterne come
    chiavi primarie, né operazioni semantiche sulle associazioni, come le cascate o gli
    eventi del ciclo di vita. Sarà necessario un successivo lavoro manuale sulle entità
    generate, perché tutto corrisponda alle specifiche del modello del dominio.

Questa guida ipotizza che si stia usando una semplice applicazione blog, con le seguenti
due tabelle: ``blog_post`` e ``blog_comment``. Una riga di un commento è collegata alla
riga di un post tramite una chiave esterna.

.. code-block:: sql

    CREATE TABLE `blog_post` (
      `id` bigint(20) NOT NULL AUTO_INCREMENT,
      `title` varchar(100) COLLATE utf8_unicode_ci NOT NULL,
      `content` longtext COLLATE utf8_unicode_ci NOT NULL,
      `created_at` datetime NOT NULL,
      PRIMARY KEY (`id`)
    ) ENGINE=InnoDB AUTO_INCREMENT=1 DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci;

    CREATE TABLE `blog_comment` (
      `id` bigint(20) NOT NULL AUTO_INCREMENT,
      `post_id` bigint(20) NOT NULL,
      `author` varchar(20) COLLATE utf8_unicode_ci NOT NULL,
      `content` longtext COLLATE utf8_unicode_ci NOT NULL,
      `created_at` datetime NOT NULL,
      PRIMARY KEY (`id`),
      KEY `blog_comment_post_id_idx` (`post_id`),
      CONSTRAINT `blog_post_id` FOREIGN KEY (`post_id`) REFERENCES `blog_post` (`id`) ON DELETE CASCADE
    ) ENGINE=InnoDB AUTO_INCREMENT=1 DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci;

Prima di addentrarsi nella ricetta, assicurarsi di aver configurato correttamente
i parametri di connessione, nel file ``app/config/parameters.yml`` (o in qualsiasi
altro posto in cui la configurazione è memorizzata) e di aver inizializzato un bundle
che possa ospitare le future classi entità. In questa guida, si ipotizza che esista un
AcmeBlogBundle, posto nella cartella ``src/Acme/BlogBundle``.

Il primo passo nella costruzione di classi entità da una base dati esistente è quello di
chiedere a Doctrine un'introspezione della base dati e una generazione dei file dei
metadati corrispondenti. I file dei metadati descrivono le classi entità da generare
in base ai campi delle tabelle.

.. code-block:: bash

    $ php app/console doctrine:mapping:import --force AcmeBlogBundle xml

Questo comando del terminale chiede a Doctrine l'introspezione della base dati e la
generazione dei file di metadati XML sotto la cartella ``src/Acme/BlogBundle/Resources/config/doctrine``
del bundle. I file generati sono ``BlogPost.orm.xml`` e
``BlogComment.orm.xml``.

.. tip::

    Le classi dei metadati possono anche essere generate in YAML, modificando il
    primo parametro in ``yml``.

Il file dei metadati ``BlogPost.orm.xml`` è simile a questo:

.. code-block:: xml

    <?xml version="1.0" encoding="utf-8"?>
    <doctrine-mapping xmlns="http://doctrine-project.org/schemas/orm/doctrine-mapping" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" xsi:schemaLocation="http://doctrine-project.org/schemas/orm/doctrine-mapping http://doctrine-project.org/schemas/orm/doctrine-mapping.xsd">
      <entity name="Acme\BlogBundle\Entity\BlogPost" table="blog_post">
        <id name="id" type="bigint" column="id">
          <generator strategy="IDENTITY"/>
        </id>
        <field name="title" type="string" column="title" length="100" nullable="false"/>
        <field name="content" type="text" column="content" nullable="false"/>
        <field name="createdAt" type="datetime" column="created_at" nullable="false"/>
      </entity>
    </doctrine-mapping>

Una volta generati i file dei metadati, si può chiedere a Doctrine di importare lo
schema e costruire le relative classi entità, eseguendo i seguenti comandi.

.. code-block:: bash

    $ php app/console doctrine:mapping:convert annotation ./src
    $ php app/console doctrine:generate:entities AcmeBlogBundle

Il primo comando genera le classi delle entità con annotazioni. Se invece
si vuole usare la mappature yml o xml, basta eseguire il secondo
comando.

.. tip::

    Se si vogliono usare le annotazioni, si possono tranquillamente eliminare i file XML,
    dopo l'esecuzione dei due comandi.

Per esempio, la nuova classe entità ``BlogComment`` è simile a questa::

    // src/Acme/BlogBundle/Entity/BlogComment.php
    namespace Acme\BlogBundle\Entity;

    use Doctrine\ORM\Mapping as ORM;

    /**
     * Acme\BlogBundle\Entity\BlogComment
     *
     * @ORM\Table(name="blog_comment")
     * @ORM\Entity
     */
    class BlogComment
    {
        /**
         * @var integer $id
         *
         * @ORM\Column(name="id", type="bigint")
         * @ORM\Id
         * @ORM\GeneratedValue(strategy="IDENTITY")
         */
        private $id;

        /**
         * @var string $author
         *
         * @ORM\Column(name="author", type="string", length=100, nullable=false)
         */
        private $author;

        /**
         * @var text $content
         *
         * @ORM\Column(name="content", type="text", nullable=false)
         */
        private $content;

        /**
         * @var datetime $createdAt
         *
         * @ORM\Column(name="created_at", type="datetime", nullable=false)
         */
        private $createdAt;

        /**
         * @var BlogPost
         *
         * @ORM\ManyToOne(targetEntity="BlogPost")
         * @ORM\JoinColumn(name="post_id", referencedColumnName="id")
         */
        private $post;
    }

Come si può vedere, Doctrine converte tutti i campi delle tabelle in proprietà della
classe. La cosa più notevole è che scopre anche la relazione con la classe entità
``BlogPost``, basandosi sulla chiave esterna.
Di conseguenza, si può trovare una proprietà ``$post``, mappata con l'entità ``BlogPost``
nella classe ``BlogComment``.

.. note::

    Se si vuole una relazione ``oneToMany``, occorrerà aggiungerla manualmente
    nell'entità o nei file XML o YAML.
    Aggiungere una sezione nelle specifiche entità per ``oneToMany``, definendo
    le parti ``inversedBy`` e ``mappedBy``.

Le entità generate sono pronte per l'uso. Buon divertimento!

.. _`documentazione sugli strumenti di Doctrine`: http://docs.doctrine-project.org/projects/doctrine-orm/en/latest/reference/tools.html#reverse-engineering
