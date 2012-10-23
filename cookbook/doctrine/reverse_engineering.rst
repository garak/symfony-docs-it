.. index::
   single: Doctrine; Generare entità da una base dati esistente

Come generare entità da una base dati esistente
===============================================

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
    generate, perché tutto corrisponda alle specifiche del modello del proprio dominio.

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

Prima di addentrarsi nella ricetta, ci si assicuri di aver configurato correttamente
i propri parametri di connessione, nel file ``app/config/parameters.yml`` (o in qualsiasi
altro posto in cui la configurazione è memorizzata) e di aver inizializzato un bundle
che possa ospitare le future classi entità. In questa guida, si ipotizza che esista
un ``AcmeBlogBundle``, posto nella cartella
``src/Acme/BlogBundle``.

Il primo passo nella costruzione di classi entità da una base dati esistente è quello di
chiedere a Doctrine un'introspezione della base dati e una generazione dei file dei
meta-dati corrispondenti. I file dei meta-dati descrivono le classi entità da generare
in base ai campi delle tabelle.

.. code-block:: bash

    php app/console doctrine:mapping:convert xml ./src/Acme/BlogBundle/Resources/config/doctrine/metadata/orm --from-database --force

Questo comando del terminale chiede a Doctrine l'introspezione della base dati e la
generazione dei file di meta-dati XML sotto la cartella
``src/Acme/BlogBundle/Resources/config/doctrine/metadata/orm`` del bundle.

.. tip::

    Le classi dei meta-dati possono anche essere generate in YAML, modificando il
    primo parametro in `yml`.

Il file dei meta-dati ``BlogPost.dcm.xml`` assomiglia a questo:

.. code-block:: xml

    <?xml version="1.0" encoding="utf-8"?>
    <doctrine-mapping>
      <entity name="BlogPost" table="blog_post">
        <change-tracking-policy>DEFERRED_IMPLICIT</change-tracking-policy>
        <id name="id" type="bigint" column="id">
          <generator strategy="IDENTITY"/>
        </id>
        <field name="title" type="string" column="title" length="100"/>
        <field name="content" type="text" column="content"/>
        <field name="isPublished" type="boolean" column="is_published"/>
        <field name="createdAt" type="datetime" column="created_at"/>
        <field name="updatedAt" type="datetime" column="updated_at"/>
        <field name="slug" type="string" column="slug" length="255"/>
        <lifecycle-callbacks/>
      </entity>
    </doctrine-mapping>

.. note::

    Se si hanno relazioni ``oneToMany`` tra entità,
    occorrerà modificare i file ``xml`` o ``yml`` generati, per aggiungere
    una sezione sulle entità specifiche ``oneToMany``, che definiscono le parti
    ``inversedBy`` e ``mappedBy``.

Una volta generati i file dei meta-dati, si può chiedere a Doctrine di importare lo
schema e costruire le relative classi entità, eseguendo i seguenti comandi.

.. code-block:: bash

    $ php app/console doctrine:mapping:import AcmeBlogBundle annotation
    $ php app/console doctrine:generate:entities AcmeBlogBundle

Il primo comando genera le classi delle entità con annotazioni, ma ovviamente
si può cambiare il parametro ``annotation`` in ``xml`` o ``yml``.
La nuva classe entità ``BlogComment`` è simile a questa:

.. code-block:: php

    <?php

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
         * @var bigint $id
         *
         * @ORM\Column(name="id", type="bigint", nullable=false)
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

Il secondo comando genera tutti i getter e i setter per le proprietà delle classi entità
``BlogPost`` e ``BlogComment``. Le entità generate sono ora pronte per essere
usate.

.. _`documentazione sugli strumenti di Doctrine`: http://docs.doctrine-project.org/projects/doctrine-orm/en/latest/reference/tools.html#reverse-engineering
