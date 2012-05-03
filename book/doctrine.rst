.. index::
   single: Doctrine

Database e Doctrine ("Il modello")
==================================

Ammettiamolo, uno dei compiti più comuni e impegnativi per qualsiasi applicazione
implica la persistenza e la lettura di informazioni da un database. Fortunatamente,
Symfony è integrato con `Doctrine`_, una libreria il cui unico scopo è quello di
fornire potenti strumenti per facilitare tali compiti. In questo capitolo, si imparerà
la filosofia alla base di Doctrine e si vedrà quanto possa essere facile lavorare
con un database.

.. note::

    Doctrine è totalmente disaccoppiato da Symfony e il suo utilizzo è facoltativo.
    Questo capitolo è tutto su Doctrine, che si prefigge lo scopo di consentire una mappatura
    tra oggetti un database relazionale (come *MySQL*, *PostgreSQL* o *Microsoft SQL*).
    Se si preferisce l'uso di query grezze, lo si può fare facilmente, come spiegato
    nella ricetta ":doc:`/cookbook/doctrine/dbal`".

    Si possono anche persistere dati su `MongoDB`_ usando la libreria ODM Doctrine. Per
    ulteriori informazioni, leggere la documentazione
    ":doc:`/bundles/DoctrineMongoDBBundle/index`".

Un semplice esempio: un prodotto
--------------------------------

Il modo più facile per capire come funziona Doctrine è quello di vederlo in azione.
In questa sezione, configureremo un database, creeremo un oggetto ``Product``,
lo persisteremo nel database e lo recupereremo da esso.

.. sidebar:: Codice con l'esempio

    Se si vuole seguire l'esempio in questo capitolo, creare
    un bundle ``AcmeStoreBundle`` tramite:
    
    .. code-block:: bash
    
        php app/console generate:bundle --namespace=Acme/StoreBundle

Configurazione del database
~~~~~~~~~~~~~~~~~~~~~~~~~~~

Prima di iniziare, occorre configurare le informazioni sulla connessione al
database. Per convenzione, questa informazione solitamente è configurata in un
file ``app/config/parameters.yml``:

.. code-block:: yaml

    # app/config/parameters.yml
    parameters:
        database_driver:   pdo_mysql
        database_host:     localhost
        database_name:     test_project
        database_user:     root
        database_password: password

.. note::

    La definizione della configurazione tramite ``parameters.yml`` è solo una convenzione.
    I parametri definiti in tale file sono riferiti dal file di configurazione principale
    durante le impostazioni iniziali di Doctrine:
    
    .. code-block:: yaml
    
        doctrine:
            dbal:
                driver:   %database_driver%
                host:     %database_host%
                dbname:   %database_name%
                user:     %database_user%
                password: %database_password%
    
    Separando le informazioni sul database in un file a parte, si possono mantenere
    facilmente diverse versioni del file su ogni server. Si possono anche facilmente
    memorizzare configurazioni di database (o altre informazioni sensibili) fuori dal
    proprio progetto, come per esempio dentro la configurazione di Apache. Per
    ulteriori informazioni, vedere :doc:`/cookbook/configuration/external_parameters`.

Ora che Doctrine ha informazioni sul proprio database, si può fare in modo che crei il
database al posto nostro:

.. code-block:: bash

    php app/console doctrine:database:create

Creare una classe entità
~~~~~~~~~~~~~~~~~~~~~~~~

Supponiamo di star costruendo un'applicazione in cui i prodotti devono essere mostrati.
Senza nemmeno pensare a Doctrine o ai database, già sappiamo di aver bisogno di
un oggetto ``Product`` che rappresenti questi prodotti. Creare questa classe dentro
la cartella ``Entity`` del proprio ``AcmeStoreBundle``::

    // src/Acme/StoreBundle/Entity/Product.php    
    namespace Acme\StoreBundle\Entity;

    class Product
    {
        protected $name;

        protected $price;

        protected $description;
    }

La classe, spesso chiamata "entità" (che vuol dire *una classe di base che contiene dati*),
è semplice e aiuta a soddisfare i requisiti di business di necessità di prodotti della
propria applicazione. Questa classe non può ancora essere persistita in un database, è
solo una semplice classe PHP.

.. tip::

    Una volta imparati i concetti dietro a Doctrine, si può fare in modo che Doctrine
    crei questa classe entità al posto nostro:
    
    .. code-block:: bash
        
        php app/console doctrine:generate:entity --entity="AcmeStoreBundle:Product" --fields="name:string(255) price:float description:text"

.. index::
    single: Doctrine; Aggiungere meta-dati di mappatura

.. _book-doctrine-adding-mapping:

Aggiungere informazioni di mappatura
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Doctrine consente di lavorare con i database in un modo molto più interessante rispetto
al semplice recupero di righe da tabelle basate su colonne in un array. Invece, Doctrine
consente di persistere interi *oggetti* sul database e di recuperare interi oggetti
dal database. Funziona mappando una classe PHP su una tabella di database e le
proprietà della classe PHP sulle colonne della tabella:

.. image:: /images/book/doctrine_image_1.png
   :align: center

Per fare in modo che Doctrine possa fare ciò, occorre solo creare dei "meta-dati", ovvero
la configurazione che dice esattamente a Doctrine come la classe ``Product`` e le sue
proprietà debbano essere *mappate* sul database. Questi meta-dati possono essere specificati
in diversi formati, inclusi YAML, XML o direttamente dentro la classe ``Product``,
tramite annotazioni:

.. note::

    Un bundle può accettare un solo formato di definizione dei meta-dati. Per esempio, non
    è possibile mischiare definizioni di meta-dati in YAML con definizioni tramite
    annotazioni.

.. configuration-block::

    .. code-block:: php-annotations

        // src/Acme/StoreBundle/Entity/Product.php
        namespace Acme\StoreBundle\Entity;

        use Doctrine\ORM\Mapping as ORM;

        /**
         * @ORM\Entity
         * @ORM\Table(name="product")
         */
        class Product
        {
            /**
             * @ORM\Id
             * @ORM\Column(type="integer")
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

        # src/Acme/StoreBundle/Resources/config/doctrine/Product.orm.yml
        Acme\StoreBundle\Entity\Product:
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

        <!-- src/Acme/StoreBundle/Resources/config/doctrine/Product.orm.xml -->
        <doctrine-mapping xmlns="http://doctrine-project.org/schemas/orm/doctrine-mapping"
              xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
              xsi:schemaLocation="http://doctrine-project.org/schemas/orm/doctrine-mapping
                            http://doctrine-project.org/schemas/orm/doctrine-mapping.xsd">

            <entity name="Acme\StoreBundle\Entity\Product" table="product">
                <id name="id" type="integer" column="id">
                    <generator strategy="AUTO" />
                </id>
                <field name="name" column="name" type="string" length="100" />
                <field name="price" column="price" type="decimal" scale="2" />
                <field name="description" column="description" type="text" />
            </entity>
        </doctrine-mapping>

.. tip::

    Il nome della tabella è facoltativo e, se omesso, sarà determinato automaticamente
    in base al nome della classe entità.

Doctrine consente di scegliere tra una grande varietà di tipi di campo, ognuno
con le sue opzioni Per informazioni sui tipi disponibili, vedere la sezione
:ref:`book-doctrine-field-types`.

.. seealso::

    Si può anche consultare la `Documentazione di base sulla mappatura`_ di Doctrine
    per tutti i dettagli sulla mappatura. Se si usano le annotazioni, occorrerà
    aggiungere a ogni annotazione il prefisso ``ORM\`` (p.e. ``ORM\Column(..)``),
    che non è mostrato nella documentazione di Doctrine. Occorrerà anche includere
    l'istruzione ``use Doctrine\ORM\Mapping as ORM;``, che *importa* il prefisso
    ``ORM`` delle annotazioni.

.. caution::

    Si faccia attenzione che il nome della classe e delle proprietà scelti non siano
    mappati a delle parole riservate di SQL (come ``group`` o ``user``). Per esempio,
    se il proprio nome di classe entità è ``Group``, allora il nome predefinito della
    tabella sarà ``group``, che causerà un errore SQL in alcuni sistemi di database.
    Vedere la `Documentazione sulle parole riservate di SQL`_ per sapere come fare
    correttamente escape di tali nomi.

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

Generare getter e setter
~~~~~~~~~~~~~~~~~~~~~~~~

Sebbene ora Doctrine sappia come persistere un oggetto ``Product`` nel database,
la classe stessa non è molto utile. Poiché ``Product`` è solo una normale classe
PHP, occorre creare dei metodi getter e setter (p.e. ``getName()``, ``setName()``)
per poter accedere alle sue proprietà (essendo le proprietà protette).
Fortunatamente, Doctrine può farlo al posto nostro, basta eseguire:

.. code-block:: bash

    php app/console doctrine:generate:entities Acme/StoreBundle/Entity/Product

Il comando si assicura che i getter e i setter siano generati per la classe
``Product``. È un comando sicuro, lo si può eseguire diverse volte: genererà i
getter e i setter solamente se non esistono (ovvero non sostituirà eventuali
metodi già presenti).

.. sidebar:: Di più su ``doctrine:generate:entities``

    Con il comando ``doctrine:generate:entities`` si può:

    * generare getter e setter,

    * generare classi repository configurate con l'annotazione
      ``@ORM\Entity(repositoryClass="...")``,

    * generare il costruttore appropriato per relazioni 1:n e n:m.

    Il comando ``doctrine:generate:entities`` salva una copia di backup del file
    originale ``Product.php``, chiamata ``Product.php~``. In alcuni casi, la presenza
    di questo file può causare un errore "Cannot redeclare class". Il file può
    essere rimosso senza problemi.

    Si noti che non è *necessario* usare questo comando. Doctrine non si appoggia alla
    generazione di codice. Come con le normali classi PHP, occorre solo assicurarsi
    che le proprietà protected/private abbiano metodi getter e setter.
    Questo comando è stato creato perché è una cosa comune da fare quando si usa
    Doctrine.

Si possono anche generare tutte le entità note (cioè ogni classe PHP con informazioni di
mappatura di Doctrine) di un bundle o di un intero spazio dei nomi:

.. code-block:: bash

    php app/console doctrine:generate:entities AcmeStoreBundle
    php app/console doctrine:generate:entities Acme

.. note::

    Doctrine non si cura se le proprietà sono ``protected`` o ``private``,
    o se siano o meno presenti getter o setter per una proprietà.
    I getter e i setter sono generati qui solo perché necessari per
    interagire col proprio oggetto PHP.

Creare tabelle e schema del database
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Ora si ha una classe ``Product`` usabile, con informazioni di mappatura che consentono
a Doctrine di sapere esattamente come persisterla. Ovviamente, non si ha ancora la
corrispondente tabella ``product`` nel proprio database. Fortunatamente, Doctrine può
creare automaticamente tutte le tabelle del database necessarie a ogni entità nota
nella propria applicazione. Per farlo, eseguire:

.. code-block:: bash

    php app/console doctrine:schema:update --force

.. tip::

    Questo comando è incredibilmente potente. Confronta ciò che il proprio database
    *dovrebbe* essere (basandosi sulle informazioni di mappatura delle entità) con
    ciò che *effettivamente* è, quindi genera le istruzioni SQL necessarie per
    *aggiornare* il database e portarlo a ciò che dovrebbe essere. In altre parole,
    se si aggiunge una nuova proprietà con meta-dati di mappatura a ``Product`` e si
    esegue nuovamente il task, esso genererà l'istruzione "alter table" necessaria
    per aggiungere questa nuova colonna alla tabella ``product`` esistente.

    Un modo ancora migliore per trarre vantaggio da questa funzionalità è tramite
    le :doc:`migrazioni</bundles/DoctrineMigrationsBundle/index>`, che consentono di
    generare queste istruzioni SQL e di memorizzarle in classi di migrazione, che
    possono essere eseguite sistematicamente sul proprio server di produzione, per
    poter tracciare e migrare il proprio schema di database in modo sicuro e affidabile.

Il proprio database ora ha una tabella ``product`` pienamente funzionante, con le colonne
corrispondenti ai meta-dati specificati.

Persistere gli oggetti nel database
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Ora che l'entità ``Product`` è stata mappata alla corrispondente tabella ``product``,
si è pronti per persistere i dati nel database. Da dentro un controllore, è
molto facile. Aggiungere il seguente metodo a ``DefaultController``
del bundle:

.. code-block:: php
    :linenos:

    // src/Acme/StoreBundle/Controller/DefaultController.php
    use Acme\StoreBundle\Entity\Product;
    use Symfony\Component\HttpFoundation\Response;
    // ...
    
    public function createAction()
    {
        $product = new Product();
        $product->setName('Pippo Pluto');
        $product->setPrice('19.99');
        $product->setDescription('Lorem ipsum dolor');

        $em = $this->getDoctrine()->getEntityManager();
        $em->persist($product);
        $em->flush();

        return new Response('Creato prodotto con id '.$product->getId());
    }

.. note::

    Se si sta seguendo questo esempio, occorrerà creare una
    rotta che punti a questa azione, per poterla vedere in azione.

Analizziamo questo esempio:

* **righe 8-11** In questa sezione, si istanzia e si lavora con l'oggetto
  ``$product``, come qualsiasi altro normale oggetto PHP;

* **riga 13** Questa riga recupera l'oggetto *gestore di entità* di Doctrine,
  responsabile della gestione del processo di persistenza e del recupero di
  oggetti dal database;

* **riga 14** Il metodo ``persist()`` dice a Doctrine di "gestire" l'oggetto
  ``$product``. Questo non fa (ancora) eseguire una query sul database.

* **riga 15** Quando il metodo ``flush()`` è richiamato, Doctrine cerca tutti
  gli oggetti che sta gestendo, per vedere se hanno bisogno di essere persistiti
  sul database. In questo esempio, l'oggetto ``$product`` non è stato ancora
  persistito, quindi il gestore di entità esegue una query ``INSERT`` e crea
  una riga nella tabella ``product``.

.. note::

  Di fatto, essendo Doctrine consapevole di tutte le proprie entità gestite,
  quando si chiama il metodo ``flush()``, esso calcola un insieme globale di
  modifiche ed esegue le query più efficienti possibili. Per esempio, se si persiste
  un totale di 100 oggetti ``Product`` e quindi si richiama ``flush()``,
  Doctrine creerà una *singola* istruzione e la riuserà per ogni inserimento.
  Questo pattern si chiama *Unit of Work* ed è utilizzato in virtù della sua
  velocità ed efficienza.

Quando si creano o aggiornano oggetti, il flusso è sempre lo stesso. Nella prossima
sezione, si vedrà come Doctrine sia abbastanza intelligente da usare una query
``UPDATE`` se il record è già esistente nel database.

.. tip::

    Doctrine fornisce una libreria che consente di caricare dati di test
    nel proprio progetto (le cosiddette "fixture"). Per informazioni, vedere
    :doc:`/bundles/DoctrineFixturesBundle/index`.

Recuperare oggetti dal database
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Recuperare un oggetto dal database è ancora più facile. Per esempio,
supponiamo di aver configurato una rotta per mostrare uno specifico ``Product``,
in base al valore del suo ``id``::

    public function showAction($id)
    {
        $product = $this->getDoctrine()
            ->getRepository('AcmeStoreBundle:Product')
            ->find($id);
        
        if (!$product) {
            throw $this->createNotFoundException('Nessun prodotto trovato per l\'id '.$id);
        }

        // fare qualcosa, come passare l'oggetto $product a un template
    }

Quando si cerca un particolare tipo di oggetto, si usa sempre quello che è noto
come il suo "repository". Si può pensare a un repository come a una classe PHP il cui
unico compito è quello di aiutare nel recuperare entità di una certa classe. Si può
accedere all'oggetto repository per una classe entità tramite::

    $repository = $this->getDoctrine()
        ->getRepository('AcmeStoreBundle:Product');

.. note::

    La stringa ``AcmeStoreBundle:Product`` è una scorciatoia utilizzabile ovunque in
    Doctrine al posto del nome intero della classe dell'entità (cioè ``Acme\StoreBundle\Entity\Product``).
    Questo funzionerà finché le proprie entità rimarranno sotto lo spazio dei nomi ``Entity``
    del proprio bundle.

Una volta ottenuto il proprio repository, si avrà accesso a tanti metodi utili::

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
    $product = $repository->findOneBy(array('name' => 'pippo', 'price' => 19.99));

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

    Cliccando sull'icona, si aprirà il profiler, che mostrerà il numero esatto
    di query eseguite.

Aggiornare un oggetto
~~~~~~~~~~~~~~~~~~~~~

Una volta che Doctrine ha recuperato un oggetto, il suo aggiornamento è facile. Supponiamo
di avere una rotta che mappi un id di prodotto a un'azione di aggiornamento in un controllore::

    public function updateAction($id)
    {
        $em = $this->getDoctrine()->getEntityManager();
        $product = $em->getRepository('AcmeStoreBundle:Product')->find($id);

        if (!$product) {
            throw $this->createNotFoundException('Nessun prodotto trovato per l\'id '.$id);
        }

        $product->setName('Nome del nuovo prodotto!');
        $em->flush();

        return $this->redirect($this->generateUrl('homepage'));
    }

L'aggiornamento di un oggetto si svolge in tre passi:

1. recuperare l'oggetto da Doctrine;
2. modificare l'oggetto;
3. richiamare ``flush()`` sul gestore di entità

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
vorrebbe rimuovere la data entità dal database. Tuttavia, la query ``DELETE`` non viene
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

Si immagini di voler cercare dei prodotti, ma solo quelli che costino più di
``19.99``, ordinati dal più economico al più caro. Da dentro un controllore,
fare come segue::

    $em = $this->getDoctrine()->getEntityManager();
    $query = $em->createQuery(
        'SELECT p FROM AcmeStoreBundle:Product p WHERE p.price > :price ORDER BY p.price ASC'
    )->setParameter('price', '19.99');
    
    $products = $query->getResult();

Se ci si trova a proprio agio con SQL, DQL dovrebbe sembrare molto naturale. La
maggiore differenza è che occorre pensare in termini di "oggetti" invece che di
righe di database. Per questa ragione, si cerca *da* ``AcmeStoreBundle:Product``
e poi si usa ``p`` come suo alias.

Il metodo ``getResult()`` restituisce un array di risultati. Se si cerca un solo
oggetto, si può usare invece il metodo ``getSingleResult()``::

    $product = $query->getSingleResult();

.. caution::

    Il metodo ``getSingleResult()`` solleva un'eccezione ``Doctrine\ORM\NoResultException``
    se non ci sono risultati e una ``Doctrine\ORM\NonUniqueResultException``
    se c'è *più* di un risultato. Se si usa questo metodo, si potrebbe voler inserirlo
    in un blocco try-catch, per assicurarsi che sia restituito un solo risultato
    (nel caso in cui sia possibile che siano restituiti più
    risultati)::
    
        $query = $em->createQuery('SELECT ....')
            ->setMaxResults(1);
        
        try {
            $product = $query->getSingleResult();
        } catch (\Doctrine\Orm\NoResultException $e) {
            $product = null;
        }
        // ...

La sintassi DQL è incredibilmente potente e consente di fare join tra entità
(l'argomento :ref:`relazioni<book-doctrine-relations>` sarà affrontato
successivamente), raggruppare, ecc. Per maggiori informazioni, vedere la
documentazione ufficiale di Doctrine `Doctrine Query Language`_.

.. sidebar:: Impostare i parametri

    Si prenda nota del metodo ``setParameter()``. Lavorando con Doctrine,
    è sempre una buona idea impostare ogni valore esterno come "segnaposto",
    come è stato fatto nella query precedente:
    
    .. code-block:: text

        ... WHERE p.price > :price ...

    Si può quindi impostare il valore del segnaposto ``price``, richiamando il
    metodo ``setParameter()``::

        ->setParameter('price', '19.99')

    L'uso di parametri al posto dei valori diretti nella stringa della query 
    serve a prevenire attacchi di tipo SQL injection e andrebbe fatto *sempre*.
    Se si usano più parametri, si possono impostare i loro valori in una volta
    sola, usando il metodo ``setParameters()``::

        ->setParameters(array(
            'price' => '19.99',
            'name'  => 'Pippo',
        ))

Usare query builder di Doctrine
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Invece di scrivere direttamente le query, si può invece usare ``QueryBuilder``,
per fare lo stesso lavoro usando un'interfaccia elegante e orientata agli oggetti.
Se si usa un IDE, si può anche trarre vantaggio dall'auto-completamento durante
la scrittura dei nomi dei metodi. Da dentro un controllore::

    $repository = $this->getDoctrine()
        ->getRepository('AcmeStoreBundle:Product');

    $query = $repository->createQueryBuilder('p')
        ->where('p.price > :price')
        ->setParameter('price', '19.99')
        ->orderBy('p.price', 'ASC')
        ->getQuery();
    
    $products = $query->getResult();

L'oggetto ``QueryBuilder`` contiene tutti i metodi necessari per costruire la
propria query. Richiamando il metodo ``getQuery()``, query builder restituisce
un normale oggetto ``Query``, che è lo stesso oggetto costruito direttamente
nella sezione precedente.

Per maggiori informazioni su query builder, consultare la documentazione di
Doctrine `Query Builder`_.

Classi repository personalizzate
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Nelle sezioni precedenti, si è iniziato costruendo e usando query più complesse da
dentro un controllore. Per isolare, testare e riusare queste query, è una buona idea
creare una classe repository personalizzata per la propria entità e aggiungere
metodi, come la propria logica di query, al suo interno.

Per farlo, aggiungere il nome della classe del repository alla propria definizione di mappatura.

.. configuration-block::

    .. code-block:: php-annotations

        // src/Acme/StoreBundle/Entity/Product.php
        namespace Acme\StoreBundle\Entity;

        use Doctrine\ORM\Mapping as ORM;

        /**
         * @ORM\Entity(repositoryClass="Acme\StoreBundle\Repository\ProductRepository")
         */
        class Product
        {
            //...
        }

    .. code-block:: yaml

        # src/Acme/StoreBundle/Resources/config/doctrine/Product.orm.yml
        Acme\StoreBundle\Entity\Product:
            type: entity
            repositoryClass: Acme\StoreBundle\Repository\ProductRepository
            # ...

    .. code-block:: xml

        <!-- src/Acme/StoreBundle/Resources/config/doctrine/Product.orm.xml -->
        <!-- ... -->
        <doctrine-mapping>

            <entity name="Acme\StoreBundle\Entity\Product"
                    repository-class="Acme\StoreBundle\Repository\ProductRepository">
                    <!-- ... -->
            </entity>
        </doctrine-mapping>

Doctrine può generare la classe repository per noi, eseguendo lo stesso comando
usato precedentemente per generare i metodi getter e setter mancanti:

.. code-block:: bash

    php app/console doctrine:generate:entities Acme

Quindi, aggiungere un nuovo metodo, chiamato ``findAllOrderedByName()``, alla classe
repository appena generata. Questo metodo cercherà tutte le entità ``Product``,
ordinate alfabeticamente.

.. code-block:: php

    // src/Acme/StoreBundle/Repository/ProductRepository.php
    namespace Acme\StoreBundle\Repository;

    use Doctrine\ORM\EntityRepository;

    class ProductRepository extends EntityRepository
    {
        public function findAllOrderedByName()
        {
            return $this->getEntityManager()
                ->createQuery('SELECT p FROM AcmeStoreBundle:Product p ORDER BY p.name ASC')
                ->getResult();
        }
    }

.. tip::

    Si può accedere al gestore di entità tramite ``$this->getEntityManager()``
    da dentro il repository.

Si può usare il metodo appena creato proprio come i metodi predefiniti del repository::

    $em = $this->getDoctrine()->getEntityManager();
    $products = $em->getRepository('AcmeStoreBundle:Product')
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

    php app/console doctrine:generate:entity --entity="AcmeStoreBundle:Category" --fields="name:string(255)"

Questo task genera l'entità ``Category``, con un campo ``id``,
un campo ``name`` e le relative funzioni getter e setter.

Meta-dati di mappatura delle relazioni
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Per correlare le entità ``Category`` e ``Product``, iniziamo creando una proprietà
``products`` nella classe ``Category``:

.. configuration-block::

    .. code-block:: php-annotations

        // src/Acme/StoreBundle/Entity/Category.php
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

        # src/Acme/StoreBundle/Resources/config/doctrine/Category.orm.yml
        Acme\StoreBundle\Entity\Category:
            type: entity
            # ...
            oneToMany:
                products:
                    targetEntity: Product
                    mappedBy: category
            # non dimenticare di inizializzare la collection nel metodo __construct() dell'entità


Primo, poiché un oggetto ``Category`` sarà collegato a diversi oggetti ``Product``,
va aggiutna una proprietà array ``products``, per contenere questi oggetti ``Product``.
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

        // src/Acme/StoreBundle/Entity/Product.php
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

        # src/Acme/StoreBundle/Resources/config/doctrine/Product.orm.yml
        Acme\StoreBundle\Entity\Product:
            type: entity
            # ...
            manyToOne:
                category:
                    targetEntity: Category
                    inversedBy: products
                    joinColumn:
                        name: category_id
                        referencedColumnName: id

Infine, dopo aver aggiunto una nuova proprietà sia alla classe ``Category`` che a
quella ``Product``, dire a Doctrine di generare i metodi mancanti getter e
setter:

.. code-block:: bash

    php app/console doctrine:generate:entities Acme

Ignoriamo per un momento i meta-dati di Doctrine. Abbiamo ora due classi, ``Category``
e ``Product``, con una relazione naturale uno-a-molti. La classe ``Category``
contiene un array di oggetti ``Product`` e l'oggetto ``Product`` può contenere un
oggetto ``Category``. In altre parole, la classe è stata costruita in un modo che ha
senso per le proprie necessità. Il fatto che i dati necessitino di essere persistiti
su un database è sempre secondario.

Diamo ora uno sguardo ai meta-dati nella proprietà ``$category`` della classe
``Product``. Qui le informazioni dicono a Doctrine che la classe correlata è
``Category`` e che dovrebbe memorizzare il valore ``id`` della categoria in un campo
``category_id`` della tabella ``product``. In altre parole, l'oggetto ``Category``
correlato sarà memorizzato nella proprietà ``$category``, ma dietro le quinte Doctrine
persisterà questa relazione memorizzando il valore dell'id della categoria in una
colonna ``category_id`` della tabella ``product``.

.. image:: /images/book/doctrine_image_2.png
   :align: center

I meta-dati della proprietà ``$products`` dell'oggetto ``Category`` sono meno
importanti e dicono semplicemente a Doctrine di cercare la proprietà ``Product.category``
per sapere come mappare la relazione.

Prima di continuare, accertarsi di dire a Doctrine di aggiungere la nuova tabella
``category`` la nuova colonna ``product.category_id`` e la nuova chiave esterna:

.. code-block:: bash

    php app/console doctrine:schema:update --force

.. note::

    Questo task andrebbe usato solo durante lo sviluppo. Per un metodo più robusto
    di aggiornamento sistematico del proprio database di produzione, leggere
    :doc:`Migrazioni doctrine</bundles/DoctrineFixturesBundle/index>`.

Salvare le entità correlate
~~~~~~~~~~~~~~~~~~~~~~~~~~~

Vediamo ora il codice in azione. Immaginiamo di essere dentro un controllore::

    // ...
    use Acme\StoreBundle\Entity\Category;
    use Acme\StoreBundle\Entity\Product;
    use Symfony\Component\HttpFoundation\Response;
    // ...

    class DefaultController extends Controller
    {
        public function createProductAction()
        {
            $category = new Category();
            $category->setName('Prodotti principali');
            
            $product = new Product();
            $product->setName('Pippo');
            $product->setPrice(19.99);
            // correlare questo prodotto alla categoria
            $product->setCategory($category);
            
            $em = $this->getDoctrine()->getEntityManager();
            $em->persist($category);
            $em->persist($product);
            $em->flush();
            
            return new Response(
                'Creati prodotto con id: '.$product->getId().' e categoria con id: '.$category->getId()
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
            ->getRepository('AcmeStoreBundle:Product')
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

    public function showProductAction($id)
    {
        $category = $this->getDoctrine()
            ->getRepository('AcmeStoreBundle:Category')
            ->find($id);

        $products = $category->getProducts();
    
        // ...
    }

In questo caso succedono le stesse cose: prima si cerca un singolo oggetto
``Category``, poi Doctrine esegue una seconda query per recuperare l'oggetto
``Product`` correlato, ma solo quando/se richiesto (cioè al richiamo di
``->getProducts()``). La variabile ``$products`` è un array di tutti gli oggetti
``Product`` correlati con il dato oggetto ``Category`` tramite il loro valore ``category_id``.

.. sidebar:: Relazioni e classi proxy

    Questo "lazy load" è possibile perché, quando necessario, Doctrine restituisce
    un oggetto "proxy" al posto del vero oggetto. Guardiamo di nuovo l'esempio
    precedente::
    
        $product = $this->getDoctrine()
            ->getRepository('AcmeStoreBundle:Product')
            ->find($id);

        $category = $product->getCategory();

        // mostra "Proxies\AcmeStoreBundleEntityCategoryProxy"
        echo get_class($category);

    Questo oggetto proxy estende il vero oggetto ``Category`` e sembra e si comporta
    esattamente nello stesso modo. La differenza è che, usando un oggetto proxy,
    Doctrine può rimandare la query per i dati effettivi di ``Category`` fino a che
    non sia effettivamente necessario (cioè fino alla chiamata di ``$category->getName()``).

    Le classy proxy sono generate da Doctrine e memorizzate in cache.
    Sebbene probabilmente non si noterà mai che il proprio oggetto ``$category``
    sia in realtà un oggetto proxy, è importante tenerlo a mente.

    Nella prossima sezione, quando si recuperano i dati di prodotto e categoria
    in una volta sola (tramite una *join*), Doctrine restituirà il *vero* oggetto
    ``Category``, poiché non serve alcun lazy load.

Join di record correlati
~~~~~~~~~~~~~~~~~~~~~~~~

Negli esempi precedenti, sono state eseguite due query: una per l'oggetto originale
(p.e. una ``Category``) e una per gli oggetti correlati (p.e. gli oggetti
``Product``).

.. tip::

    Si ricordi che è possibile vedere tutte le query eseguite durante una richiesta,
    tramite la barra di web debug.

Ovviamente, se si sa in anticipo di aver bisogno di accedere a entrambi gli oggetti,
si può evitare la seconda query, usando una join nella query originale. Aggiungere
il seguente metodo alla classe ``ProductRepository``::

    // src/Acme/StoreBundle/Repository/ProductRepository.php
    
    public function findOneByIdJoinedToCategory($id)
    {
        $query = $this->getEntityManager()
            ->createQuery('
                SELECT p, c FROM AcmeStoreBundle:Product p
                JOIN p.category c
                WHERE p.id = :id'
            )->setParameter('id', $id);
        
        try {
            return $query->getSingleResult();
        } catch (\Doctrine\ORM\NoResultException $e) {
            return null;
        }
    }

Ora si può usare questo metodo nel proprio controllore per cercare un oggetto
``Product`` e la relativa ``Category`` con una sola query::

    public function showAction($id)
    {
        $product = $this->getDoctrine()
            ->getRepository('AcmeStoreBundle:Product')
            ->findOneByIdJoinedToCategory($id);

        $category = $product->getCategory();
    
        // ...
    }    

Ulteriori informazioni sulle associazioni
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Questa sezione è stata un'introduzione a un tipo comune di relazione tra entità,
la relazione uno-a-molti. Per dettagli ed esempi più avanzati su come usare altri
tipi di relazioni (p.e. uno-a-uno, molti-a-molti), vedere
la `Documentazione sulla mappatura delle associazioni`_.

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

Se si usano le annotazioni per i meta-dati, iniziare abilitando i callback del
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
ciclo di vita. Per esempio, supponiamo di voler impostare una colonna di data ``created``
alla data attuale, solo quando l'entità è persistita la prima volta (cioè è inserita):

.. configuration-block::

    .. code-block:: php-annotations

        /**
         * @ORM\prePersist
         */
        public function setCreatedValue()
        {
            $this->created = new \DateTime();
        }

    .. code-block:: yaml

        # src/Acme/StoreBundle/Resources/config/doctrine/Product.orm.yml
        Acme\StoreBundle\Entity\Product:
            type: entity
            # ...
            lifecycleCallbacks:
                prePersist: [ setCreatedValue ]

    .. code-block:: xml

        <!-- src/Acme/StoreBundle/Resources/config/doctrine/Product.orm.xml -->
        <!-- ... -->
        <doctrine-mapping>

            <entity name="Acme\StoreBundle\Entity\Product">
                    <!-- ... -->
                    <lifecycle-callbacks>
                        <lifecycle-callback type="prePersist" method="setCreatedValue" />
                    </lifecycle-callbacks>
            </entity>
        </doctrine-mapping>

.. note::

    L'esempio precedente presume che sia stata creata e mappata una proprietà ``created``
    (non mostrata qui).

Ora, appena prima che l'entità sia persistita per la prima volta, Doctrine richiamerà
automaticamente questo metodo e il campo ``created`` sarà valorizzato con la data attuale.

Si può ripetere questa operazione per ogni altro evento del ciclo di vita:

* ``preRemove``
* ``postRemove``
* ``prePersist``
* ``postPersist``
* ``preUpdate``
* ``postUpdate``
* ``postLoad``
* ``loadClassMetadata``

Per maggiori informazioni sul significato di questi eventi del ciclo di vita e in generale
sui callback del ciclo di vita, vedere la `Documentazione sugli eventi del ciclo di vita`_

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

Estensioni di Doctrine: Timestampable, Sluggable, ecc.
------------------------------------------------------

Doctrine è alquanto flessibile e diverse estensioni di terze parti sono disponibili,
consentendo di eseguire facilmente compiti comuni e ripetitivi sulle proprie entità.
Sono inclusi *Sluggable*, *Timestampable*, *Loggable*, *Translatable* e
*Tree*.

Per maggiori informazioni su come trovare e usare tali estensioni, vedere la ricetta
:doc:`usare le estensioni comuni di Doctrine</cookbook/doctrine/common_extensions>`.

.. _book-doctrine-field-types:

Riferimento sui tipi di campo di Doctrine
-----------------------------------------

Doctrine ha un gran numero di tipi di campo a disposizione. Ognuno di questi mappa
un tipo di dato PHP su un tipo specifico di colonna in qualsiasi database si
utilizzi. I seguenti tipi sono supportati in Doctrine:

* **Stringhe**

  * ``string`` (per stringhe più corte)
  * ``text`` (per stringhe più lunghe)

* **Numeri**

  * ``integer``
  * ``smallint``
  * ``bigint``
  * ``decimal``
  * ``float``

* **Date e ore** (usare un oggetto `DateTime`_ per questi campi in PHP)

  * ``date``
  * ``time``
  * ``datetime``

* **Altri tipi**

  * ``boolean``
  * ``object`` (serializzato e memorizzato in un campo ``CLOB``)
  * ``array`` (serializzato e memorizzato in un campo ``CLOB``)

Per maggiori informazioni, vedere `Documentazione sulla mappatura dei tipi`_.

Opzioni dei campi
~~~~~~~~~~~~~~~~~

Ogni campo può avere un insieme di opzioni da applicare. Le opzioni disponibili
includono ``type`` (predefinito ``string``), ``name``, ``length``, ``unique``
e ``nullable``. Vediamo alcuni esempi con le annotazioni:

.. configuration-block::

    .. code-block:: php-annotations

        /**
         * Un campo stringa con lunghezza 255 che non può essere nullo
         * (riflette i valori predefiniti per le opzioni "type", "length" e *nullable*)
         * 
         * @ORM\Column()
         */
        protected $name;

        /**
         * Un campo stringa con lunghezza 150 che persiste su una colonna "email_address"
         * e ha un vincolo di unicità.
         *
         * @ORM\Column(name="email_address", unique="true", length="150")
         */
        protected $email;

    .. code-block:: yaml

        fields:
            # Un campo stringa con lunghezza 255 che non può essere nullo
            # (riflette i valori predefiniti per le opzioni "type", "length" e *nullable*)
            # l'attributo type è necessario nelle definizioni yaml
            name:
                type: string

            # Un campo stringa con lunghezza 150 che persiste su una colonna "email_address"
            # e ha un vincolo di unicità.
            email:
                type: string
                column: email_address
                length: 150
                unique: true

.. note::

    Ci sono alcune altre opzioni, non elencate qui. Per maggiori dettagli,
    vedere la `Documentazione sulla mappatura delle proprietà`_

.. index::
   single: Doctrine; Comandi da console ORM
   single: CLI; ORM Doctrine

Comandi da console
------------------

L'integrazione con l'ORM Doctrine2 offre diversi comandi da console, sotto lo spazio
dei nomi ``doctrine``. Per vedere la lista dei comandi, si può eseguire la
console senza parametri:

.. code-block:: bash

    php app/console

Verrà mostrata una lista dei comandi disponibili, molti dei quali iniziano
col prefisso ``doctrine:``. Si possono trovare maggiori informazioni eseguendo il
comando ``help``. Per esempio, per ottenere dettagli sul task
``doctrine:database:create``, eseguire:

.. code-block:: bash

    php app/console help doctrine:database:create

Alcuni task interessanti sono:

* ``doctrine:ensure-production-settings`` - verifica se l'ambiente attuale
  sia configurato efficientemente per la produzione. Dovrebbe essere sempre
  eseguito nell'ambiente ``prod``:
  
  .. code-block:: bash
  
    php app/console doctrine:ensure-production-settings --env=prod

* ``doctrine:mapping:import`` - consente a Doctrine l'introspezione di un database
  esistente e di creare quindi le informazioni di mappatura. Per ulteriori informazioni,
  vedere :doc:`/cookbook/doctrine/reverse_engineering`.

* ``doctrine:mapping:info`` - elenca tutte le entità di cui Doctrine è a
  conoscenza e se ci sono o meno errori di base con la mappatura.

* ``doctrine:query:dql`` e ``doctrine:query:sql`` - consente l'esecuzione di
  query DQL o SQL direttamente dalla linea di comando.

.. note::

   Per poter caricare fixture nel proprio database, occorrerà avere il bundle
   ``DoctrineFixturesBundle`` installato. Per sapere come farlo, leggere
   la voce ":doc:`/bundles/DoctrineFixturesBundle/index`" della
   documentazione.

Riepilogo
---------

Con Doctrine, ci si può concentrare sui propri oggetti e su come siano utili nella
propria applicazione e preoccuparsi della persistenza su database in un secondo momento.
Questo perché Doctrine consente di usare qualsiasi oggetto PHP per tenere i propri dati e
si appoggia su meta-dati di mappatura per mappare i dati di un oggetto su una
particolare tabella di database.

Sebbene Doctrine giri intorno a un semplice concetto, è incredibilmente potente,
consentendo di creare query complesse e sottoscrivere eventi che consentono
di intraprendere diverse azioni, mentre gli oggetti viaggiano lungo il loro ciclo
di vita della persistenza.

Per maggiori informazioni su Doctrine, vedere la sezione *Doctrine* del
:doc:`ricettario</cookbook/index>`, che include i seguenti articoli:

* :doc:`/bundles/DoctrineFixturesBundle/index`
* :doc:`/cookbook/doctrine/common_extensions`

.. _`Doctrine`: http://www.doctrine-project.org/
.. _`MongoDB`: http://www.mongodb.org/
.. _`Documentazione di base sulla mappatura`: http://docs.doctrine-project.org/orm/en/latest/reference/basic-mapping.html
.. _`Query Builder`: http://docs.doctrine-project.org/orm/en/latest/reference/query-builder.html
.. _`Doctrine Query Language`: http://docs.doctrine-project.org/orm/en/latest/reference/dql-doctrine-query-language.html
.. _`Documentazione sulla mappatura delle associazioni`: http://docs.doctrine-project.org/orm/en/latest/reference/association-mapping.html
.. _`DateTime`: http://php.net/manual/en/class.datetime.php
.. _`Documentazione sulla mappatura dei tipi`: http://docs.doctrine-project.org/orm/en/latest/reference/basic-mapping.html#doctrine-mapping-types
.. _`Documentazione sulla mappatura delle proprietà`: http://docs.doctrine-project.org/orm/en/latest/reference/basic-mapping.html#property-mapping
.. _`Documentazione sugli eventi del ciclo di vita`: http://docs.doctrine-project.org/orm/en/latest/reference/events.html#lifecycle-events
.. _`Documentazione sulle parole riservate di SQL`: http://docs.doctrine-project.org/projects/doctrine-orm/en/latest/reference/basic-mapping.html#quoting-reserved-words
