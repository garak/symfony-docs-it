.. index::
   single: Doctrine

Basi di dati e Doctrine
=======================

Uno dei compiti più comuni e impegnativi per qualsiasi applicazione
implica la persistenza e la lettura di informazioni da una base dati. Sebbene il
framework Symfony non si integri con un ORM in modo predefinito,
Symfony Standard Edition, la distribuzione più usata, dispone di
un'integrazione con `Doctrine`_, una libreria il cui unico scopo è quello di
fornire potenti strumenti per facilitare tali compiti. In questo capitolo, si imparerà
la filosofia alla base di Doctrine e si vedrà quanto possa essere facile lavorare
con una base dati.

.. note::

    Doctrine è totalmente disaccoppiato da Symfony e il suo utilizzo è facoltativo.
    Questo capitolo è tutto su Doctrine ORM, che si prefigge lo scopo di consentire una mappatura
    tra oggetti una base dati relazionale (come *MySQL*, *PostgreSQL* o
    *Microsoft SQL*). Se si preferisce l'uso di query grezze, lo si può fare facilmente,
    come spiegato nella ricetta ":doc:`/cookbook/doctrine/dbal`".

    Si possono anche persistere dati su `MongoDB`_ usando la libreria ODM Doctrine. Per
    ulteriori informazioni, leggere la documentazione di
    "DoctrineMongoDBBundle`_".

Un semplice esempio: un prodotto
--------------------------------

Il modo più facile per capire come funziona Doctrine è quello di vederlo in azione.
In questa sezione, configureremo una base dati, creeremo un oggetto ``Product``,
lo persisteremo nella base dati e lo recupereremo da esso.

Configurazione della base dati
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Prima di iniziare, occorre configurare le informazioni sulla connessione alla
base dati. Per convenzione, questa informazione solitamente è configurata in un
file ``app/config/parameters.yml``:

.. code-block:: yaml

    # app/config/parameters.yml
    parameters:
        database_driver:    pdo_mysql
        database_host:      localhost
        database_name:      progetto_test
        database_user:      root
        database_password:  password

    # ...

.. note::

    La definizione della configurazione tramite ``parameters.yml`` è solo una convenzione.
    I parametri definiti in tale file sono riferiti dal file di configurazione principale
    durante le impostazioni iniziali di Doctrine:

    .. configuration-block::

        .. code-block:: yaml

            # app/config/config.yml
            doctrine:
                dbal:
                    driver:   "%database_driver%"
                    host:     "%database_host%"
                    dbname:   "%database_name%"
                    user:     "%database_user%"
                    password: "%database_password%"

        .. code-block:: xml

            <!-- app/config/config.xml -->
            <?xml version="1.0" encoding="UTF-8" ?>
            <container xmlns="http://symfony.com/schema/dic/services"
                xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
                xmlns:doctrine="http://symfony.com/schema/dic/doctrine"
                xsi:schemaLocation="http://symfony.com/schema/dic/services
                    http://symfony.com/schema/dic/services/services-1.0.xsd
                    http://symfony.com/schema/dic/doctrine
                    http://symfony.com/schema/dic/doctrine/doctrine-1.0.xsd">

                <doctrine:config>
                    <doctrine:dbal
                        driver="%database_driver%"
                        host="%database_host%"
                        dbname="%database_name%"
                        user="%database_user%"
                        password="%database_password%" />
                </doctrine:config>
            </container>

        .. code-block:: php

            // app/config/config.php
            $configuration->loadFromExtension('doctrine', array(
                'dbal' => array(
                    'driver'   => '%database_driver%',
                    'host'     => '%database_host%',
                    'dbname'   => '%database_name%',
                    'user'     => '%database_user%',
                    'password' => '%database_password%',
                ),
            ));

    Separando le informazioni sula base dati in un file a parte, si possono mantenere
    facilmente diverse versioni del file su ogni server. Si possono anche facilmente
    memorizzare configurazioni di basi dati (o altre informazioni sensibili) fuori dal
    progetto, come per esempio dentro la configurazione di Apache. Per
    ulteriori informazioni, vedere :doc:`/cookbook/configuration/external_parameters`.

Ora che Doctrine ha informazioni sulla base dati, si può fare in modo che crei la
base dati al posto nostro:

.. code-block:: bash

    $ php app/console doctrine:database:create

.. sidebar:: Impostazioni dei caratteri della base dati

    Uno sbaglio che anche programmatori esperti commettono all'inizio di un progetto Symfony
    è dimenticare di impostare charset e collation nella base dati,
    finendo con collation di tipo latin, che sono predefinite la maggior parte delle volte.
    Lo si potrebbe fare anche solo all'inizio, ma spesso si dimentica che lo si
    può fare anche durante lo sviluppo, in modo abbastanza semplice:

    .. code-block:: bash

        $ php app/console doctrine:database:drop --force
        $ php app/console doctrine:database:create

    Non c'è modo di configurare tali valori predefiniti in Doctrine, che prova a essere
    il più agnostico possibile in termini di configurazione di ambienti. Un modo per risolvere
    la questione è usare dei valori definiti a livello di server.

    Impostare UTF8 come predefinito in MySQL è semplice, basta aggiungere poche righe 
    al file di configurazione (solitamente ``my.cnf``):

    .. code-block:: ini

        [mysqld]
        # La versione 5.5.3 ha introdotto "utf8mb4", che è raccomandato
        collation-server     = utf8mb4_general_ci # Sostituisce utf8_general_ci
        character-set-server = utf8mb4            # Sostituisce utf8

    Si raccomanda di non usare il set di caratteri ``utf8`` di MySQL, poiché non
    supporta caratteri unicode a 4 byte, quindi le stringhe che li contenessero sarebbero
    troncate. Il problema è stato risolta nel `nuovo set di caratteri utf8mb4`_.

.. note::

    Se si vuole usare SQLite come base dati, occorre impostare il percorso in cui
    si trova il relativo file:

    .. configuration-block::

        .. code-block:: yaml

            # app/config/config.yml
            doctrine:
                dbal:
                    driver: pdo_sqlite
                    path: "%kernel.root_dir%/sqlite.db"
                    charset: UTF8

        .. code-block:: xml

            <!-- app/config/config.xml -->
            <?xml version="1.0" encoding="UTF-8" ?>
            <container xmlns="http://symfony.com/schema/dic/services"
                xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
                xmlns:doctrine="http://symfony.com/schema/dic/doctrine"
                xsi:schemaLocation="http://symfony.com/schema/dic/services
                    http://symfony.com/schema/dic/services/services-1.0.xsd
                    http://symfony.com/schema/dic/doctrine
                    http://symfony.com/schema/dic/doctrine/doctrine-1.0.xsd">

                <doctrine:config>
                    <doctrine:dbal
                        driver="pdo_sqlite"
                        path="%kernel.root_dir%/sqlite.db"
                        charset="UTF-8" />
                </doctrine:config>
            </container>

        .. code-block:: php

            // app/config/config.php
            $container->loadFromExtension('doctrine', array(
                'dbal' => array(
                    'driver'  => 'pdo_sqlite',
                    'path'    => '%kernel.root_dir%/sqlite.db',
                    'charset' => 'UTF-8',
                ),
            ));

Creare una classe entità
~~~~~~~~~~~~~~~~~~~~~~~~

Supponiamo di star costruendo un'applicazione in cui si devono elencare dei prodotti.
Senza nemmeno pensare a Doctrine o alle basi dati, già sappiamo di aver bisogno di
un oggetto ``Product`` che rappresenti questi prodotti. Creare questa classe dentro
la cartella ``Entity`` di AppBundle::

    // src/AppBundle/Entity/Product.php
    namespace AppBundle\Entity;

    class Product
    {
        protected $name;
        protected $price;
        protected $description;
    }

La classe, spesso chiamata "entità" (che vuol dire *una classe di base che contiene dati*),
è semplice e aiuta a soddisfare i requisiti di business di necessità di prodotti
dell'applicazione. Questa classe non può ancora essere persistita in una base dati, è
solo una semplice classe PHP.

.. tip::

    Una volta imparati i concetti dietro a Doctrine, si può fare in modo che Doctrine
    crei questa classe entità al posto nostro. Questo comando porrà delle domande, per
    aiutare nella costruzione dell'entità:

    .. code-block:: bash

        $ php app/console doctrine:generate:entity

.. index::
    single: Doctrine; Aggiungere metadati di mappatura

.. _book-doctrine-adding-mapping:

Aggiungere informazioni di mappatura
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Doctrine consente di lavorare con le basi dati in un modo molto più interessante rispetto
al semplice recupero di righe da tabelle basate su colonne in un array. Invece, Doctrine
consente di persistere interi *oggetti* sula base dati e di recuperare interi oggetti
dalla base dati. Funziona mappando una classe PHP su una tabella di base dati e le
proprietà della classe PHP sulle colonne della tabella:

.. image:: /images/book/doctrine_image_1.png
   :align: center

Per fare in modo che Doctrine possa fare ciò, occorre solo creare dei "metadati", ovvero
la configurazione che dice esattamente a Doctrine come la classe ``Product`` e le sue
proprietà debbano essere *mappate* sula base dati. Questi metadati possono essere specificati
in diversi formati, inclusi YAML, XML o direttamente dentro la classe
``Product``, tramite annotazioni:

.. configuration-block::

    .. code-block:: php-annotations

        // src/AppBundle/Entity/Product.php
        namespace AppBundle\Entity;

        use Doctrine\ORM\Mapping as ORM;

        /**
         * @ORM\Entity
         * @ORM\Table(name="product")
         */
        class Product
        {
            /**
             * @ORM\Column(type="integer")
             * @ORM\Id
             * @ORM\GeneratedValue(strategy="AUTO")
             */
            protected $id;

            /**
             * @ORM\Column(type="string", length=100)
             */
            protected $name;

            /**
             * @ORM\Column(type="decimal", scale=2)
             */
            protected $price;

            /**
             * @ORM\Column(type="text")
             */
            protected $description;
        }

    .. code-block:: yaml

        # src/AppBundle/Resources/config/doctrine/Product.orm.yml
        AppBundle\Entity\Product:
            type: entity
            table: product
            id:
                id:
                    type: integer
                    generator: { strategy: AUTO }
            fields:
                name:
                    type: string
                    length: 100
                price:
                    type: decimal
                    scale: 2
                description:
                    type: text

    .. code-block:: xml

        <!-- src/AppBundle/Resources/config/doctrine/Product.orm.xml -->
        <?xml version="1.0" encoding="UTF-8" ?>
        <doctrine-mapping xmlns="http://doctrine-project.org/schemas/orm/doctrine-mapping"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://doctrine-project.org/schemas/orm/doctrine-mapping
                http://doctrine-project.org/schemas/orm/doctrine-mapping.xsd">

            <entity name="AppBundle\Entity\Product" table="product">
                <id name="id" type="integer">
                    <generator strategy="AUTO" />
                </id>
                <field name="name" type="string" length="100" />
                <field name="price" type="decimal" scale="2" />
                <field name="description" type="text" />
            </entity>
        </doctrine-mapping>

.. note::

    Un bundle può accettare un solo formato di definizione dei metadati. Per esempio, non
    è possibile mischiare definizioni di metadati in YAML con definizioni tramite
    annotazioni.

.. tip::

    Il nome della tabella è facoltativo e, se omesso, sarà determinato automaticamente
    in base al nome della classe entità.

Doctrine consente di scegliere tra una grande varietà di tipi di campo, ognuno
con le sue opzioni Per informazioni sui tipi disponibili, vedere la sezione
:ref:`book-doctrine-field-types`.

.. seealso::

    Si può anche consultare `Basic Mapping Documentation`_ di Doctrine
    per tutti i dettagli sulla mappatura. Se si usano le annotazioni, occorrerà
    aggiungere a ogni annotazione il prefisso ``ORM\`` (p.e. ``ORM\Column(..)``),
    che non è mostrato nella documentazione di Doctrine. Occorrerà anche includere
    l'istruzione ``use Doctrine\ORM\Mapping as ORM;``, che *importa* il prefisso
    ``ORM`` delle annotazioni.

.. caution::

    Si faccia attenzione che il nome della classe e delle proprietà scelti non siano
    mappati a delle parole riservate di SQL (come ``group`` o ``user``). Per esempio,
    se il nome di una classe entità è ``Group``, allora il nome predefinito della
    tabella sarà ``group``, che causerà un errore SQL in alcuni sistemi di basi dati.
    Vedere `Reserved SQL keywords documentation`_ di Doctrine per sapere come fare
    correttamente escape di tali nomi. In alternativa, se si può scegliere liberamente lo schema della
    base dati, usare semplicemente un nome diverso di tabella o di colonna. Vedere
    `Persistent classes`_ e `Property Mapping`_ nella documentazione di Doctrine.

.. note::

    Se si usa un'altra libreria o programma che utilizza le annotazioni (come Doxygen),
    si dovrebbe inserire l'annotazione ``@IgnoreAnnotation`` nella classe, per indicare
    a Symfony quali annotazioni ignorare.

    Per esempio, per evitare che l'annotazione ``@fn`` sollevi un'eccezione, aggiungere
    il seguente::

        /**
         * @IgnoreAnnotation("fn")
         */
        class Product
        // ...

.. _book-doctrine-generating-getters-and-setters:

Generare getter e setter
~~~~~~~~~~~~~~~~~~~~~~~~

Sebbene ora Doctrine sappia come persistere un oggetto ``Product`` nella base dati,
la classe stessa non è molto utile. Poiché ``Product`` è solo una normale classe
PHP, occorre creare dei metodi getter e setter (p.e. ``getName()``,
``setName()``) per poter accedere alle sue proprietà (essendo le proprietà protette).
Fortunatamente, Doctrine può farlo al posto nostro, basta eseguire:

.. code-block:: bash

    $ php app/console doctrine:generate:entities AppBundle/Entity/Product

Il comando si assicura che i getter e i setter siano generati per la classe
``Product``. È un comando sicuro, lo si può eseguire diverse volte: genererà i
getter e i setter solamente se non esistono (ovvero non sostituirà eventuali
metodi già presenti).

.. caution::

    Si tenga a mente che il generatore di entità di Doctrine produce semplici getter e setter. 
    Si dovrebbero controllare le entità generate e sistemare getter e setter per adattarli
    alle proprie necessità.

.. sidebar:: Di più su ``doctrine:generate:entities``

    Con il comando ``doctrine:generate:entities`` si può:

    * generare getter e setter;

    * generare classi repository configurate con l'annotazione
      ``@ORM\Entity(repositoryClass="...")``;

    * generare il costruttore appropriato per relazioni 1:n e n:m.

    Il comando ``doctrine:generate:entities`` salva una copia di backup del file
    originale ``Product.php``, chiamata ``Product.php~``. In alcuni casi, la presenza
    di questo file può causare un errore "Cannot redeclare class". Il file può
    essere rimosso senza problemi. Si può anche usare l'opzione ``--no-backup``, per prevenire
    la generazione di questi file di backup.

    Si noti che non è *necessario* usare questo comando. Doctrine non si appoggia alla
    generazione di codice. Come con le normali classi PHP, occorre solo assicurarsi
    che le proprietà protected/private abbiano metodi getter e setter.
    Questo comando è stato creato perché è una cosa comune da fare quando si usa
    Doctrine.

Si possono anche generare tutte le entità note (cioè ogni classe PHP con informazioni di
mappatura di Doctrine) di un bundle o di un intero spazio dei nomi:

.. code-block:: bash

    # genera tutte le entità in AppBundle
    $ php app/console doctrine:generate:entities AppBundle

    # genera tutte le entità dei bundle nello spazio dei nomi Acme
    $ php app/console doctrine:generate:entities Acme

.. note::

    Doctrine non si cura se le proprietà siano protette o private,
    o se siano o meno presenti getter o setter per una proprietà.
    I getter e i setter sono generati qui solo perché necessari per
    interagire col l'oggetto PHP.

.. _book-doctrine-creating-the-database-tables-schema:

Creare tabelle e schema della base dati
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Ora si ha una classe ``Product`` usabile, con informazioni di mappatura che consentono
a Doctrine di sapere esattamente come persisterla. Ovviamente, non si ha ancora la
corrispondente tabella ``product`` nella propria base dati. Fortunatamente, Doctrine può
creare automaticamente tutte le tabelle della base dati necessarie a ogni entità nota
nella propria applicazione. Per farlo, eseguire:

.. code-block:: bash

    $ php app/console doctrine:schema:update --force

.. tip::

    Questo comando è incredibilmente potente. Confronta ciò che la propria base dati
    *dovrebbe* essere (basandosi sulle informazioni di mappatura delle entità) con
    ciò che *effettivamente* è, quindi genera le istruzioni SQL necessarie per
    *aggiornare* la base dati e portarlo a ciò che dovrebbe essere. In altre parole,
    se si aggiunge una nuova proprietà con metadati di mappatura a ``Product`` e si
    esegue nuovamente il task, esso genererà l'istruzione "alter table" necessaria
    per aggiungere questa nuova colonna alla tabella ``product`` esistente.

    Un modo ancora migliore per trarre vantaggio da questa funzionalità è tramite
    le `migrazioni`_, che consentono di
    generare queste istruzioni SQL e di memorizzarle in classi di migrazione, che
    possono essere eseguite sistematicamente sul server di produzione, per
    poter tracciare e migrare lo schema della base dati in modo sicuro e affidabile.

La propria base dati ora ha una tabella ``product`` pienamente funzionante, con le colonne
corrispondenti ai metadati specificati.

Persistere gli oggetti nella base dati
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Ora che l'entità ``Product`` è stata mappata alla corrispondente tabella ``product``,
si è pronti per persistere i dati nella base dati. Da dentro un controllore, è
molto facile. Aggiungere il seguente metodo a ``DefaultController``
del bundle::


    // src/AppBundle/Controller/DefaultController.php

    // ...
    use AppBundle\Entity\Product;
    use Symfony\Component\HttpFoundation\Response;

    // ...
    public function createAction()
    {
        $product = new Product();
        $product->setName('Pippo Pluto');
        $product->setPrice('19.99');
        $product->setDescription('Lorem ipsum dolor');

        $em = $this->getDoctrine()->getManager();

        $em->persist($product);
        $em->flush();

        return new Response('Creato prodotto con id '.$product->getId());
    }

.. note::

    Se si sta seguendo questo esempio, occorrerà creare una
    rotta che punti a questa azione, per poterla vedere in azione.

.. tip::

    Questo articolo mostra come si interagisce con Doctrine dall'interno di un controllore, usando
    il metodo :method:`Symfony\\Bundle\\FrameworkBundle\\Controller\\Controller::getDoctrine`
    del controllore. Tale metodo è una scorciatoia per ottenere il servizio
    ``doctrine``. Si può interagire con Doctrine in altri contesti,
    iniettandolo come servizio. Vedere
    :doc:`/book/service_container` per maggiori informazioni sulla creazione di servizi.

Analizziamo questo esempio:

* **righe 10-13** In questa sezione, si istanzia e si lavora con l'oggetto ``$product``,
  come qualsiasi altro normale oggetto PHP;

* **riga 14** Questa riga recupera l'oggetto *gestore di entità* di Doctrine,
  responsabile della gestione del processo di persistenza e del recupero di
  oggetti dalla base dati;

* **riga 16** Il metodo ``persist()`` dice a Doctrine di "gestire" l'oggetto ``$product``.
  Questo non fa (ancora) eseguire una query sula base dati.

* **riga 17** Quando il metodo ``flush()`` è richiamato, Doctrine cerca tutti
  gli oggetti che sta gestendo, per vedere se hanno bisogno di essere persistiti
  sulla base dati. In questo esempio, l'oggetto ``$product`` non è stato ancora
  persistito, quindi il gestore di entità esegue una query ``INSERT`` e crea
  una riga nella tabella ``product``.

.. note::

  Di fatto, essendo Doctrine consapevole di tutte le proprie entità gestite,
  quando si chiama il metodo ``flush()``, esso calcola un insieme globale di
  modifiche ed esegue le query nell'ordine corretto, usando dei prepared statement
  per migliorare le prestazioni. Per esempio, se si persiste
  un totale di 100 oggetti ``Product`` e quindi si richiama ``flush()``,
  Doctrine eseguirà 100 query ``INSERT`` in un singolo oggetto prepared statement.

Quando si creano o aggiornano oggetti, il flusso è sempre lo stesso. Nella prossima
sezione, si vedrà come Doctrine sia abbastanza intelligente da usare una query
``UPDATE`` se il record è già esistente nella base dati.

.. tip::

    Doctrine fornisce una libreria che consente di caricare dati di test
    in un progetto (le cosiddette "fixture"). Per informazioni, vedere la documentazione di
    "`DoctrineFixturesBundle`_".

Recuperare oggetti dalla base dati
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Recuperare un oggetto dalla base dati è ancora più facile. Per esempio,
supponiamo di aver configurato una rotta per mostrare uno specifico ``Product``,
in base al valore del suo ``id``::

    public function showAction($id)
    {
        $product = $this->getDoctrine()
            ->getRepository('AppBundle:Product')
            ->find($id);

        if (!$product) {
            throw $this->createNotFoundException(
                'Nessun prodotto trovato per l\'id '.$id
            );
        }

        // ... fare qualcosa, come passare l'oggetto $product a un template
    }

.. tip::

    Si può ottenere lo stesso risultato senza scrivere codice usando
    la scorciatoia ``@ParamConverter``. Vedere la `documentazione di FrameworkExtraBundle`_
    per maggiori dettagli.

Quando si cerca un particolare tipo di oggetto, si usa sempre quello che è noto
come il suo "repository". Si può pensare a un repository come a una classe PHP il cui
unico compito è quello di aiutare nel recuperare entità di una certa classe. Si può
accedere all'oggetto repository per una classe entità tramite::

    $repository = $this->getDoctrine()
        ->getRepository('AppBundle:Product');

.. note::

    La stringa ``AppBundle:Product`` è una scorciatoia utilizzabile ovunque in
    Doctrine al posto del nome intero della classe dell'entità (cioè ``AppBundle\Entity\Product``).
    Questo funzionerà finché le entità rimarranno sotto lo spazio dei nomi ``Entity``
    del bundle.

Una volta ottenuto il repository, si avrà accesso a tanti metodi utili::

    // cerca per chiave primaria (di solito "id")
    $product = $repository->find($id);

    // nomi di metodi dinamici per cercare in base al valore di una colonna
    $product = $repository->findOneById($id);
    $product = $repository->findOneByName('pippo');

    // trova *tutti* i prodotti
    $products = $repository->findAll();

    // trova un gruppo di prodotti in base a un valore arbitrario di una colonna
    $products = $repository->findByPrice(19.99);

.. note::

    Si possono ovviamente fare anche query complesse, su cui si può avere maggiori
    informazioni nella sezione :ref:`book-doctrine-queries`.

Si possono anche usare gli utili metodi ``findBy`` e ``findOneBy`` per
recuperare facilmente oggetti in base a condizioni multiple::

    // cerca un prodotto in base a nome e prezzo
    $product = $repository->findOneBy(
        array('name' => 'pippo', 'price' => 19.99)
    );

    // cerca tutti i prodotti in base al nome, ordinati per prezzo
    $product = $repository->findBy(
        array('name' => 'pippo'),
        array('price' => 'ASC')
    );

.. tip::

    Quando si rende una pagina, si può vedere il numero di query eseguite nell'angolo
    inferiore destro della barra di debug del web.

    .. image:: /images/book/doctrine_web_debug_toolbar.png
       :align: center
       :scale: 50
       :width: 350

    Cliccando sull'icona, si aprirà il profilatore, che mostrerà il numero esatto
    di query eseguite.

    L'icona diventa gialla se ci sono più di 50 query nella
    pagina. Questo potrebbe indicare che qualcosa non va.

Aggiornare un oggetto
~~~~~~~~~~~~~~~~~~~~~

Una volta che Doctrine ha recuperato un oggetto, il suo aggiornamento è facile. Supponiamo
di avere una rotta che mappi un id di prodotto a un'azione di aggiornamento in un controllore::

    public function updateAction($id)
    {
        $em = $this->getDoctrine()->getManager();
        $product = $em->getRepository('AppBundle:Product')->find($id);

        if (!$product) {
            throw $this->createNotFoundException(
                'Nessun prodotto trovato per l\'id '.$id
            );
        }

        $product->setName('Nome del nuovo prodotto!');
        $em->flush();

        return $this->redirect($this->generateUrl('homepage'));
    }

L'aggiornamento di un oggetto si svolge in tre passi:

#. recuperare l'oggetto da Doctrine;
#. modificare l'oggetto;
#. richiamare ``flush()`` sul gestore di entità

Si noti che non è necessario richiamare ``$em->persist($product)``. Ricordiamo che
questo metodo dice semplicemente a Doctrine di gestire o "osservare" l'oggetto ``$product``.
In questo caso, poiché l'oggetto ``$product`` è stato recuperato da Doctrine, è
già gestito.

Cancellare un oggetto
~~~~~~~~~~~~~~~~~~~~~

La cancellazione di un oggetto è molto simile, ma richiede una chiamata al metodo
``remove()`` del gestore delle entità::

    $em->remove($product);
    $em->flush();

Come ci si potrebbe aspettare, il metodo ``remove()`` rende noto a Doctrine che si
vorrebbe rimuovere la data entità dalla base dati. Tuttavia, la query ``DELETE`` non viene
realmente eseguita finché non si richiama il metodo ``flush()``.

.. _`book-doctrine-queries`:

Cercare gli oggetti
-------------------

Abbiamo già visto come l'oggetto repository consenta di eseguire query di base senza
alcuno sforzo::

    $repository->find($id);

    $repository->findOneByName('Pippo');

Ovviamente, Doctrine consente anche di scrivere query più complesse, usando
Doctrine Query Language (DQL). DQL è simile a SQL, tranne per il fatto che bisognerebbe
immaginare di stare cercando uno o più oggetti di una classe entità (p.e. ``Product``)
e non le righe di una tabella (p.e. ``product``).

Durante una ricerca in Doctrine, si hanno due opzioni: scrivere direttamente query
Doctrine, oppure usare il Query Builder di Doctrine.

Cercare oggetti con DQL
~~~~~~~~~~~~~~~~~~~~~~~

Si immagini di voler cercare dei prodotti, ma solo quelli che costino più
di ``19.99``, ordinati dal più economico al più caro. Si può usare
``QueryBuilder`` di Doctrine, come segue::

    $em = $this->getDoctrine()->getManager();
    $query = $em->createQuery(
        'SELECT p
        FROM AppBundle:Product p
        WHERE p.price > :price
        ORDER BY p.price ASC'
    )->setParameter('price', '19.99');

    $products = $query->getResult();
    // per ottenere un singolo risultato:
    // $product = $query->setMaxResults(1)->getOneOrNullResult();

Chi si trova a suo agio con SQL troverà DQL molto naturale. La differenza maggiore
consiste nel pensare in termini di "oggetti", piuttosto che di righe in
una base dati. Per questa ragione, si seleziona dall'oggetto ``AppBundle:Product``
(una scorciatoia facoltativa per ``AppBundle\Entity\Product``) e si usa
come alias ``p``.

.. tip::

    Prendere nota del metodo ``setParameter()``. Interagendo con Doctrine,
    è sempre una buona idea impostare valori esterni tramite "segnaposto"
    (``:price`` nell'esempio appena visto), per prevenire attacchi di tipo SQL injection.

Il metodo ``getResult()`` restituisce un array di risultati. Se si cerca un solo
oggetto, si può usare invece il metodo ``getOneOrNullResult()``::

    $product = $query->setMaxResults(1)->getOneOrNullResult();

La sintassi DQL è molto potente e consente di eseguire facilmente join tra
entità (l'argomento :ref:`relazioni <book-doctrine-relations>` sarà affrontato
successivamente), group by, ecc. Per maggiori informazioni, consultare la documentazione
`Query Builder`_ di Doctrine.

Cercare oggetti usando DQL
~~~~~~~~~~~~~~~~~~~~~~~~~~

Invece di usare ``QueryBuilder``, is possono scrivere query direttamente,
usando DQL::

    $repository = $this->getDoctrine()
        ->getRepository('AppBundle:Product');

    // createQueryBuilder prende automaticamente FROM AppBundle:Product
    // e gli assegna "p" come alias
    $query = $repository->createQueryBuilder('p')
        ->where('p.price > :price')
        ->setParameter('price', '19.99')
        ->orderBy('p.price', 'ASC')
        ->getQuery();

    $products = $query->getResult();
    // per ottenere un singolo risultato:
    // $product = $query->setMaxResults(1)->getOneOrNullResult();

L'oggetto ``QueryBuilder`` contiene ogni metodo necessario per costruire una
query. Richiamando il metodo ``getQuery()``, QueryBuilder restituisce un normale
oggetto ``Query``, che può essere usato per ottenere il risultato della query.

Per maggiori informazioni, vedere la documentazione
ufficiale di Doctrine `Doctrine Query Language`_.

.. _book-doctrine-custom-repository-classes:

Classi repository personalizzate
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Nelle sezioni precedenti, si è iniziato costruendo e usando query più complesse da
dentro un controllore. Per isolare, testare e riusare queste query, è una buona idea
creare una classe repository personalizzata per la propria entità e aggiungere
metodi, come la propria logica di query, al suo interno.

Per farlo, aggiungere il nome della classe del repository alla propria definizione di mappatura.

.. configuration-block::

    .. code-block:: php-annotations

        // src/AppBundle/Entity/Product.php
        namespace AppBundle\Entity;

        use Doctrine\ORM\Mapping as ORM;

        /**
         * @ORM\Entity(repositoryClass="AppBundle\Entity\ProductRepository")
         */
        class Product
        {
            //...
        }

    .. code-block:: yaml

        # src/AppBundle/Resources/config/doctrine/Product.orm.yml
        AppBundle\Entity\Product:
            type: entity
            repositoryClass: AppBundle\Entity\ProductRepository
            # ...

    .. code-block:: xml

        <!-- src/AppBundle/Resources/config/doctrine/Product.orm.xml -->
        <?xml version="1.0" encoding="UTF-8" ?>
        <doctrine-mapping xmlns="http://doctrine-project.org/schemas/orm/doctrine-mapping"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://doctrine-project.org/schemas/orm/doctrine-mapping
                http://doctrine-project.org/schemas/orm/doctrine-mapping.xsd">

            <entity
                name="AppBundle\Entity\Product"
                repository-class="AppBundle\Entity\ProductRepository">

                <!-- ... -->
            </entity>
        </doctrine-mapping>

Doctrine può generare la classe repository per noi, eseguendo lo stesso comando
usato precedentemente per generare i metodi getter e setter mancanti:

.. code-block:: bash

    $ php app/console doctrine:generate:entities AppBundle

Quindi, aggiungere un nuovo metodo, chiamato ``findAllOrderedByName()``, alla classe
repository appena generata. Questo metodo cercherà tutte le entità ``Product``,
ordinate alfabeticamente.

.. code-block:: php

    // src/AppBundle/Entity/ProductRepository.php
    namespace AppBundle\Entity;

    use Doctrine\ORM\EntityRepository;

    class ProductRepository extends EntityRepository
    {
        public function findAllOrderedByName()
        {
            return $this->getEntityManager()
                ->createQuery(
                    'SELECT p FROM AppBundle:Product p ORDER BY p.name ASC'
                )
                ->getResult();
        }
    }

.. tip::

    Si può accedere al gestore di entità tramite ``$this->getEntityManager()``
    da dentro il repository.

Si può usare il metodo appena creato proprio come i metodi predefiniti del repository::

    $em = $this->getDoctrine()->getManager();
    $products = $em->getRepository('AppBundle:Product')
        ->findAllOrderedByName();

.. note::

    Quando si usa una classe repository personalizzata, si ha ancora accesso ai metodi
    predefiniti di ricerca, come ``find()`` e ``findAll()``.

.. _`book-doctrine-relations`:

Relazioni e associazioni tra entità
-----------------------------------

Supponiamo che i prodotti nella propria applicazione appartengano tutti a una "categoria".
In questo caso, occorrerà un oggetto ``Category`` e un modo per per mettere in relazione un
oggetto ``Product`` con un oggetto ``Category``. Iniziamo creando l'entità ``Category``.
Sapendo che probabilmente occorrerà persistere la classe tramite Doctrine, lasciamo che sia
Doctrine stesso a creare la classe.

.. code-block:: bash

    $ php app/console doctrine:generate:entity \
        --entity="AppBundle:Category" \
        --fields="name:string(255)"

Questo task genera l'entità ``Category``, con un campo ``id``,
un campo ``name`` e le relative funzioni getter e setter.

Metadati di mappatura delle relazioni
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Per correlare le entità ``Category`` e ``Product``, iniziamo creando una proprietà
``products`` nella classe ``Category``:

.. configuration-block::

    .. code-block:: php-annotations

        // src/AppBundle/Entity/Category.php

        // ...
        use Doctrine\Common\Collections\ArrayCollection;

        class Category
        {
            // ...

            /**
             * @ORM\OneToMany(targetEntity="Product", mappedBy="category")
             */
            protected $products;

            public function __construct()
            {
                $this->products = new ArrayCollection();
            }
        }

    .. code-block:: yaml

        # src/AppBundle/Resources/config/doctrine/Category.orm.yml
        AppBundle\Entity\Category:
            type: entity
            # ...
            oneToMany:
                products:
                    targetEntity: Product
                    mappedBy: category
            # non dimenticare di inizializzare la collection nel metodo __construct()
            # dell'entità

    .. code-block:: xml

        <!-- src/AppBundle/Resources/config/doctrine/Category.orm.xml -->
        <?xml version="1.0" encoding="UTF-8" ?>
        <doctrine-mapping xmlns="http://doctrine-project.org/schemas/orm/doctrine-mapping"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://doctrine-project.org/schemas/orm/doctrine-mapping
                http://doctrine-project.org/schemas/orm/doctrine-mapping.xsd">

            <entity name="AppBundle\Entity\Category">
                <!-- ... -->
                <one-to-many
                    field="products"
                    target-entity="Product"
                    mapped-by="category" />

                <!--
                    non dimenticare di inizializzare la collection
                    nel metodo __construct() dell'entità
                -->
            </entity>
        </doctrine-mapping>

Primo, poiché un oggetto ``Category`` sarà collegato a diversi oggetti ``Product``,
va aggiunta una proprietà array ``products``, per contenere questi oggetti ``Product``.
Di nuovo, non va fatto perché Doctrine ne abbia bisogno, ma perché ha senso
nell'applicazione che ogni ``Category`` contenga un array di oggetti
``Product``.

.. note::

    Il codice nel metodo ``__construct()`` è importante, perché Doctrine
    esige che la proprietà ``$products`` sia un oggetto ``ArrayCollection``.
    Questo oggetto sembra e si comporta quasi *esattamente* come un array, ma ha
    un po' di flessibilità in più. Se non sembra confortevole, niente paura.
    Si immagini solamente che sia un ``array``.

.. tip::

   Il valore ``targetEntity``, usato in precedenza sul decoratore, può riferirsi a qualsiasi entità
   con uno spazio dei nomi valido, non solo a entità definite nella stessa classe. Per
   riferirsi a entità definite in classi diverse, inserire uno spazio dei nomi completo come
   ``targetEntity``.

Poi, poiché ogni classe ``Product`` può essere in relazione esattamente con un oggetto
``Category``, si deve aggiungere una proprietà ``$category`` alla classe ``Product``:

.. configuration-block::

    .. code-block:: php-annotations

        // src/AppBundle/Entity/Product.php

        // ...
        class Product
        {
            // ...

            /**
             * @ORM\ManyToOne(targetEntity="Category", inversedBy="products")
             * @ORM\JoinColumn(name="category_id", referencedColumnName="id")
             */
            protected $category;
        }

    .. code-block:: yaml

        # src/AppBundle/Resources/config/doctrine/Product.orm.yml
        AppBundle\Entity\Product:
            type: entity
            # ...
            manyToOne:
                category:
                    targetEntity: Category
                    inversedBy: products
                    joinColumn:
                        name: category_id
                        referencedColumnName: id

    .. code-block:: xml

        <!-- src/AppBundle/Resources/config/doctrine/Product.orm.xml -->
        <?xml version="1.0" encoding="UTF-8" ?>
        <doctrine-mapping xmlns="http://doctrine-project.org/schemas/orm/doctrine-mapping"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://doctrine-project.org/schemas/orm/doctrine-mapping
                http://doctrine-project.org/schemas/orm/doctrine-mapping.xsd">

            <entity name="AppBundle\Entity\Product">
                <!-- ... -->
                <many-to-one
                    field="category"
                    target-entity="Category"
                    inversed-by="products"
                    join-column="category">

                    <join-column name="category_id" referenced-column-name="id" />
                </many-to-one>
            </entity>
        </doctrine-mapping>

Infine, dopo aver aggiunto una nuova proprietà sia alla classe ``Category`` che a
quella ``Product``, dire a Doctrine di generare i metodi mancanti getter e
setter:

.. code-block:: bash

    $ php app/console doctrine:generate:entities AppBundle

Ignoriamo per un momento i metadati di Doctrine. Abbiamo ora due classi, ``Category``
e ``Product``, con una relazione naturale uno-a-molti. La classe ``Category``
contiene un array di oggetti ``Product`` e l'oggetto ``Product`` può contenere un
oggetto ``Category``. In altre parole, la classe è stata costruita in un modo che ha
senso per le proprie necessità. Il fatto che i dati necessitino di essere persistiti
su una base dati è sempre secondario.

Diamo ora uno sguardo ai metadati nella proprietà ``$category`` della classe
``Product``. Qui le informazioni dicono a Doctrine che la classe correlata è
``Category`` e che dovrebbe memorizzare il valore ``id`` della categoria in un campo
``category_id`` della tabella ``product``. In altre parole, l'oggetto ``Category``
correlato sarà memorizzato nella proprietà ``$category``, ma dietro le quinte Doctrine
persisterà questa relazione memorizzando il valore dell'id della categoria in una
colonna ``category_id`` della tabella ``product``.

.. image:: /images/book/doctrine_image_2.png
   :align: center

I metadati della proprietà ``$products`` dell'oggetto ``Category`` sono meno
importanti e dicono semplicemente a Doctrine di cercare la proprietà ``Product.category``
per sapere come mappare la relazione.

Prima di continuare, accertarsi di dire a Doctrine di aggiungere la nuova tabella
``category`` la nuova colonna ``product.category_id`` e la nuova chiave esterna:

.. code-block:: bash

    $ php app/console doctrine:schema:update --force

.. note::

    Questo task andrebbe usato solo durante lo sviluppo. Per un metodo più robusto
    di aggiornamento sistematico della propria base dati di produzione, vedere le 
    `migrazioni`_.

Salvare le entità correlate
~~~~~~~~~~~~~~~~~~~~~~~~~~~

Vediamo ora il codice in azione. Immaginiamo di essere dentro un controllore::

    // ...

    use AppBundle\Entity\Category;
    use AppBundle\Entity\Product;
    use Symfony\Component\HttpFoundation\Response;

    class DefaultController extends Controller
    {
        public function createProductAction()
        {
            $category = new Category();
            $category->setName('Prodotti principali');

            $product = new Product();
            $product->setName('Pippo');
            $product->setPrice(19.99);
            $product->setDescription('Lorem ipsum dolor');
            // correlare questo prodotto alla categoria
            $product->setCategory($category);

            $em = $this->getDoctrine()->getManager();
            $em->persist($category);
            $em->persist($product);
            $em->flush();

            return new Response(
                'Creati prodotto con id: '.$product->getId()
                .' e categoria con id: '.$category->getId()
            );
        }
    }

Una riga è stata aggiunta alle tabelle ``category`` e ``product``.
La colonna ``product.category_id`` del nuovo prodotto è impostata allo stesso valore
di ``id`` della nuova categoria. Doctrine gestisce la persistenza di tale relazione
per noi.

Recuperare gli oggetti correlati
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Quando occorre recuperare gli oggetti correlati, il flusso è del tutto simile
a quello precedente. Recuperare prima un oggetto ``$product`` e poi accedere
alla sua ``Category`` correlata::

    public function showAction($id)
    {
        $product = $this->getDoctrine()
            ->getRepository('AppBundle:Product')
            ->find($id);

        $categoryName = $product->getCategory()->getName();

        // ...
    }

In questo esempio, prima di cerca un oggetto ``Product`` in base al suo ``id``.
Questo implica una query *solo* per i dati del prodotto e idrata l'oggetto
``$product`` con tali dati. Poi, quando si richiama ``$product->getCategory()->getName()``,
Doctrine effettua una seconda query, per trovare la ``Category`` correlata con il
``Product``. Prepara l'oggetto ``$category`` e lo
restituisce.

.. image:: /images/book/doctrine_image_3.png
   :align: center

Quello che è importante è il fatto che si ha facile accesso al prodotto correlato
con la categoria, ma i dati della categoria non sono recuperati finché la
categoria non viene richiesta (processo noto come "lazy load").

Si può anche cercare nella direzione opposta::

    public function showProductsAction($id)
    {
        $category = $this->getDoctrine()
            ->getRepository('AppBundle:Category')
            ->find($id);

        $products = $category->getProducts();

        // ...
    }

In questo caso succedono le stesse cose: prima si cerca un singolo oggetto ``Category``,
poi Doctrine esegue una seconda query per recuperare l'oggetto ``Product``
correlato, ma solo quando/se richiesto (cioè al richiamo di ``->getProducts()``).
La variabile ``$products`` è un array di tutti gli oggetti ``Product``
correlati con il dato oggetto ``Category`` tramite il loro valore ``category_id``.

.. sidebar:: Relazioni e classi proxy

    Questo "lazy load" è possibile perché, quando necessario, Doctrine restituisce
    un oggetto "proxy" al posto del vero oggetto. Guardiamo di nuovo l'esempio
    precedente::

        $product = $this->getDoctrine()
            ->getRepository('AppBundle:Product')
            ->find($id);

        $category = $product->getCategory();

        // prints "Proxies\AppBundleEntityCategoryProxy"
        echo get_class($category);

    Questo oggetto proxy estende il vero oggetto ``Category`` e sembra e si comporta
    esattamente nello stesso modo. La differenza è che, usando un oggetto proxy,
    Doctrine può rimandare la query per i dati effettivi di ``Category`` fino a che
    non sia effettivamente necessario (cioè fino alla chiamata di ``$category->getName()``).

    Le classy proxy sono generate da Doctrine e memorizzate in cache.
    Sebbene probabilmente non si noterà mai che l'oggetto ``$category``
    sia in realtà un oggetto proxy, è importante tenerlo a mente.

    Nella prossima sezione, quando si recuperano i dati di prodotto e categoria
    in una volta sola (tramite una *join*), Doctrine restituirà il *vero* oggetto ``Category``,
    poiché non serve alcun lazy load.

Join di record correlati
~~~~~~~~~~~~~~~~~~~~~~~~

Negli esempi precedenti, sono state eseguite due query: una per l'oggetto originale
(p.e. una ``Category``) e una per gli oggetti correlati (p.e. gli oggetti
``Product``).

.. tip::

    Si ricordi che è possibile vedere tutte le query eseguite durante una richiesta,
    tramite la barra di debug del web.

Ovviamente, se si sa in anticipo di aver bisogno di accedere a entrambi gli oggetti,
si può evitare la seconda query, usando una join nella query originale. Aggiungere
il seguente metodo alla classe ``ProductRepository``::

    // src/AppBundle/Entity/ProductRepository.php
    public function findOneByIdJoinedToCategory($id)
    {
        $query = $this->getEntityManager()
            ->createQuery(
                'SELECT p, c FROM AppBundle:Product p
                JOIN p.category c
                WHERE p.id = :id'
            )->setParameter('id', $id);

        try {
            return $query->getSingleResult();
        } catch (\Doctrine\ORM\NoResultException $e) {
            return null;
        }
    }

Ora si può usare questo metodo nel controllore, per cercare un oggetto
``Product`` e la relativa ``Category`` con una sola query::

    public function showAction($id)
    {
        $product = $this->getDoctrine()
            ->getRepository('AppBundle:Product')
            ->findOneByIdJoinedToCategory($id);

        $category = $product->getCategory();

        // ...
    }

Ulteriori informazioni sulle associazioni
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Questa sezione è stata un'introduzione a un tipo comune di relazione tra entità,
la relazione uno-a-molti. Per dettagli ed esempi più avanzati su come usare altri
tipi di relazioni (p.e. uno-a-uno, molti-a-molti), vedere
la `Association Mapping Documentation`_ di Doctrine.

.. note::

    Se si usano le annotazioni, occorrerà aggiungere a tutte le annotazioni il prefisso
    ``ORM\`` (p.e. ``ORM\OneToMany``), che non si trova nella documentazione di
    Doctrine. Occorrerà anche includere l'istruzione ``use Doctrine\ORM\Mapping as ORM;``,
    che *importa* il prefisso delle annotazioni ``ORM``.

Configurazione
--------------

Doctrine è altamente configurabile, sebbene probabilmente non si avrà nemmeno bisogno di
preoccuparsi di gran parte delle sue opzioni. Per saperne di più sulla configurazione di
Doctrine, vedere la sezione Doctrine del :doc:`manuale di riferimento</reference/configuration/doctrine>`.

Callback del ciclo di vita
--------------------------

A volte, occorre eseguire un'azione subito prima o subito dopo che un entità sia
inserita, aggiornata o cancellata. Questi tipi di azioni sono noti come callback
del "ciclo di vita", perché sono metodi callback che occorre eseguire durante i
diversi stadi del ciclo di vita di un'entità (p.e. l'entità è inserita, aggiornata,
cancellata, eccetera). 

Se si usano le annotazioni per i metadati, iniziare abilitando i callback del
ciclo di vita. Questo non è necessario se si usa YAML o XML per la mappatura:

.. code-block:: php-annotations

    /**
     * @ORM\Entity()
     * @ORM\HasLifecycleCallbacks()
     */
    class Product
    {
        // ...
    }

Si può ora dire a Doctrine di eseguire un metodo su uno degli eventi disponibili del
ciclo di vita. Per esempio, supponiamo di voler impostare una colonna di data ``createdAt``
alla data attuale, solo quando l'entità è persistita la prima volta (cioè è inserita):

.. configuration-block::

    .. code-block:: php-annotations

        // src/AppBundle/Entity/Product.php

        /**
         * @ORM\PrePersist
         */
        public function setCreatedAtValue()
        {
            $this->createdAt = new \DateTime();
        }

    .. code-block:: yaml

        # src/AppBundle/Resources/config/doctrine/Product.orm.yml
        AppBundle\Entity\Product:
            type: entity
            # ...
            lifecycleCallbacks:
                prePersist: [setCreatedAtValue]

    .. code-block:: xml

        <!-- src/AppBundle/Resources/config/doctrine/Product.orm.xml -->
        <?xml version="1.0" encoding="UTF-8" ?>
        <doctrine-mapping xmlns="http://doctrine-project.org/schemas/orm/doctrine-mapping"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://doctrine-project.org/schemas/orm/doctrine-mapping
                http://doctrine-project.org/schemas/orm/doctrine-mapping.xsd">

            <entity name="AppBundle\Entity\Product">
                <!-- ... -->
                <lifecycle-callbacks>
                    <lifecycle-callback type="prePersist" method="setCreatedAtValue" />
                </lifecycle-callbacks>
            </entity>
        </doctrine-mapping>

.. note::

    L'esempio precedente presume che sia stata creata e mappata una proprietà ``createdAt``
    (non mostrata qui).

Ora, appena prima che l'entità sia persistita per la prima volta, Doctrine richiamerà
automaticamente questo metodo e il campo ``created`` sarà valorizzato con la data attuale.

Ci sono molti altri eventi del ciclo di vita, a cui ci si può agganciare. Per maggiori
informazioni, vedere la documentazione di Doctrine
`Lifecycle Events documentation`_.

.. sidebar:: Callback del ciclo di vita e ascoltatori di eventi

    Si noti che il metodo ``setCreatedValue()`` non riceve parametri. Questo è sempre
    il caso di callback del ciclo di vita ed è intenzionale: i callback del ciclo di
    vita dovrebbero essere metodi semplici, riguardanti la trasformazione interna di dati
    nell'entità (p.e. impostare un campo di creazione/aggiornamento, generare un
    valore per uno slug).

    Se occorre un lavoro più pesante, come eseguire un log o inviare una email, si
    dovrebbe registrare una classe esterna come ascoltatore di eventi e darle accesso
    a qualsiasi risorsa necessaria. Per maggiori informazioni, vedere
    :doc:`/cookbook/doctrine/event_listeners_subscribers`.

.. _book-doctrine-field-types:

Riferimento sui tipi di campo di Doctrine
-----------------------------------------

Doctrine ha un gran numero di tipi di campo a disposizione. Ognuno di questi mappa
un tipo di dato PHP su un tipo specifico di colonna in qualsiasi base dati si
utilizzi. Per ciascun tipo di campo, si può configurare ulteriormente ``Column``, impostando
le opzioni ``length``, ``nullable``, ``name`` e altre ancora. Per una lista completa
di tipi e per maggiori informazioni vedere la documentazione di Doctrine
`Mapping Types documentation`_.

Riepilogo
---------

Con Doctrine, ci si può concentrare sui propri oggetti e su come siano utili nella
propria applicazione e preoccuparsi della persistenza su base dati in un secondo momento.
Questo perché Doctrine consente di usare qualsiasi oggetto PHP per tenere i propri dati e
si appoggia su metadati di mappatura per mappare i dati di un oggetto su una
particolare tabella di base dati.

Sebbene Doctrine giri intorno a un semplice concetto, è incredibilmente potente,
consentendo di creare query complesse e sottoscrivere eventi che consentono
di intraprendere diverse azioni, mentre gli oggetti viaggiano lungo il loro ciclo
di vita della persistenza.

Saperne di più
~~~~~~~~~~~~~~

Per maggiori informazioni su Doctrine, vedere la sezione *Doctrine* del
:doc:`ricettario </cookbook/index>`, che include i seguenti articoli:

* :doc:`/cookbook/doctrine/common_extensions`
* :doc:`/cookbook/doctrine/console`
* `DoctrineFixturesBundle`_
* `DoctrineMongoDBBundle`_

.. _`Doctrine`: http://www.doctrine-project.org/
.. _`MongoDB`: http://www.mongodb.org/
.. _`Basic Mapping Documentation`: http://docs.doctrine-project.org/projects/doctrine-orm/en/latest/reference/basic-mapping.html
.. _`Query Builder`: http://docs.doctrine-project.org/projects/doctrine-orm/en/latest/reference/query-builder.html
.. _`Doctrine Query Language`: http://docs.doctrine-project.org/projects/doctrine-orm/en/latest/reference/dql-doctrine-query-language.html
.. _`Association Mapping Documentation`: http://docs.doctrine-project.org/projects/doctrine-orm/en/latest/reference/association-mapping.html
.. _`Mapping Types Documentation`: http://docs.doctrine-project.org/projects/doctrine-orm/en/latest/reference/basic-mapping.html#property-mapping
.. _`Property Mapping`: http://docs.doctrine-project.org/projects/doctrine-orm/en/latest/reference/basic-mapping.html#property-mapping
.. _`Lifecycle Events documentation`: http://docs.doctrine-project.org/projects/doctrine-orm/en/latest/reference/events.html#lifecycle-events
.. _`Reserved SQL keywords documentation`: http://docs.doctrine-project.org/projects/doctrine-orm/en/latest/reference/basic-mapping.html#quoting-reserved-words
.. _`Persistent classes`: http://docs.doctrine-project.org/projects/doctrine-orm/en/latest/reference/basic-mapping.html#persistent-classes
.. _`DoctrineMongoDBBundle`: http://symfony.com/doc/current/bundles/DoctrineMongoDBBundle/index.html
.. _`migrazioni`: http://symfony.com/doc/current/bundles/DoctrineMigrationsBundle/index.html
.. _`DoctrineFixturesBundle`: http://symfony.com/doc/current/bundles/DoctrineFixturesBundle/index.html
.. _`documentazione di FrameworkExtraBundle`: http://symfony.com/doc/current/bundles/SensioFrameworkExtraBundle/annotations/converters.html
.. _`nuovo set di caratteri utf8mb4`: https://dev.mysql.com/doc/refman/5.5/en/charset-unicode-utf8mb4.html
