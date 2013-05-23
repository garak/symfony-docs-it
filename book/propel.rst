.. index::
   single: Propel

Basi di dati e Propel ("Il modello")
====================================

Ammettiamolo, uno dei compiti più comuni e impegnativi per qualsiasi applicazione
implica la persistenza e la lettura di informazioni da una base dati. Symfony2 
non è integrato nativamente con Propel, ma l'integrazione è alquanto semplice.
Per iniziare, leggere `Working With Symfony2`_.

Un semplice esempio: un prodotto
--------------------------------

In questa sezione, configureremo la nostra base dati, creeremo un oggetto ``Product``,
lo persisteremo nella base dati e lo recuperemo nuovamente.

.. sidebar:: Codice insieme all'esempio

    Se si vuole seguire il codice di questo capitolo, creare un
    ``AcmeStoreBundle``, tramite:
    
    .. code-block:: bash

        $ php app/console generate:bundle --namespace=Acme/StoreBundle

Configurare la base dati
~~~~~~~~~~~~~~~~~~~~~~~~

Prima di iniziare, occorre configurare le informazioni sulla connessione alla
base dati. Per convenzione, questa informazione solitamente è configurata in un
file ``app/config/parameters.ini``:

.. code-block:: yaml

    # app/config/parameters.yml
    parameters:
        database_driver:   mysql
        database_host:     localhost
        database_name:     test_project
        database_user:     root
        database_password: password
        database_charset:  UTF8

.. note::

    La definizione della configurazione tramite ``parameters.ini`` è solo una convenzione.
    I parametri definiti in tale file sono riferiti dal file di configurazione principale
    durante le impostazioni iniziali di Propel:

I parametri definiti in ``parameters.yml`` possono essere inclusi nel file di
configurazione (``config.yml``):

    .. code-block:: yaml

        propel:
            dbal:
                driver:     "%database_driver%"
                user:       "%database_user%"
                password:   "%database_password%"
                dsn:        "%database_driver%:host=%database_host%;dbname=%database_name%;charset=%database_charset%"

Ora che Propel ha informazioni sulla base dati, si può fare in modo che crei la
base dati al posto nostro:

.. code-block:: bash

    $ php app/console propel:database:create

.. note::

    In questo esempio, si ha una sola connessione configurata, di nome ``default``. Se
    si vogliono configurare più connessioni, leggere la sezione
    `configurazione di PropelBundle`_.

Creare una classe del modello
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Nel mondo di Propel, le classi ActiveRecord sono note come **modelli**, perché le classi
generate da Propel contengono della logica di business.

.. note::

    Per chi ha già usato Symfony2 con Doctrine2, i **modelli** sono equivalenti alle
    **entità**.

Si supponga di costruire un'applicazione in cui occorre mostrare dei prodotti.
Innanzitutto, creare un file ``schema.xml`` nella cartella ``Resources/config`` del
proprio ``AcmeStoreBundle``:

.. code-block:: xml

    <?xml version="1.0" encoding="UTF-8"?>
    <database name="default"
        namespace="Acme\StoreBundle\Model"
        defaultIdMethod="native"
    >
        <table name="product">
            <column name="id"
                type="integer"
                required="true"
                primaryKey="true"
                autoIncrement="true"
            />
            <column name="name"
                type="varchar"
                primaryString="true"
                size="100"
            />
            <column name="price"
                type="decimal"
            />
            <column name="description"
                type="longvarchar"
            />
        </table>
    </database>

Costruire il modello
~~~~~~~~~~~~~~~~~~~~

Dopo aver creato ``schema.xml``, generare il modello, eseguendo:

.. code-block:: bash

    $ php app/console propel:model:build

Questo comando genera ogni classe del modello, per sviluppare rapidamente la propria
applicazione, nella cartella ``Model/`` di ``AcmeStoreBundle``.

Creare schema e tabelle della base dati
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Ora si dispone di una classe ``Product`` e di tutto il necessario per poterla persistere.
Ovviamente, non si ha ancora una corrispondente tabella ``product`` nella propria base
dati. Per fortuna, Propel può creare automaticamente tutte le tabelle della base dati,
per ciascun modello dell'applicazione. Per farlo, eseguire:

.. code-block:: bash

    $ php app/console propel:sql:build
    $ php app/console propel:sql:insert --force

La base dati ora ha una tabella ``product``, con colonne corrispondenti allo
schema creato in precedenza.

.. tip::

    Si possono eseguire gli ultimi tre comandi in uno, usando il seguente:
    ``php app/console propel:build --insert-sql``.

Persistere oggetti nella base dati
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Ora che si ha un oggetto ``Product`` e una tabella ``product`` corrispondente,
si è pronti per persistere nella base dati. Da dentro un controllore, è molto
facile. Aggiungere il seguente metodo a ``DefaultController`` del
bundle::

    // src/Acme/StoreBundle/Controller/DefaultController.php

    // ...
    use Acme\StoreBundle\Model\Product;
    use Symfony\Component\HttpFoundation\Response;

    public function createAction()
    {
        $product = new Product();
        $product->setName('Un nome');
        $product->setPrice(19.99);
        $product->setDescription('Lorem ipsum dolor');

        $product->save();

        return new Response('Creato prodotto con id '.$product->getId());
    }

In questo pezzo di codice, abbiamo istanziato e usato un oggetto ``$product``.
Richiamando il suo metodo ``save()``, lo si persiste nella base dati. Non occorre
usare altri servizi, l'oggetto sa da solo come persistersi.

.. note::

    Se si segue il codice di questo esempio, occorre creare una
    :doc:`rotta <routing>` che punti a questa azione.

Recuperare oggetti dalla base dati
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Recuperare oggetti dalla base dati è anche più semplice. Per esempio, si supponga
di aver configurato una rotta per mostrare uno specifico ``Product``, in base al
valore del suo ``id``::
    
    // ...
    use Acme\StoreBundle\Model\ProductQuery;

    public function showAction($id)
    {
        $product = ProductQuery::create()
            ->findPk($id);

        if (!$product) {
            throw $this->createNotFoundException(
                'Nessun prodotto trovato con id '.$id
            );
        }

        // ... fare qualcosa, come passare l'oggetto $product a un template
    }

Aggiornare un oggetto
~~~~~~~~~~~~~~~~~~~~~

Una volta recuperato un oggetto con Propel, aggiornarlo è facile. Si supponga di avere
una rotta che mappi l'id di un prodotto all'azione di aggiornamento di un controllore::
    
    // ...
    use Acme\StoreBundle\Model\ProductQuery;

    public function updateAction($id)
    {
        $product = ProductQuery::create()
            ->findPk($id);

        if (!$product) {
            throw $this->createNotFoundException(
                'Nessun prodotto trovato con id '.$id
            );
        }

        $product->setName('Nuovo nome del prodotto!');
        $product->save();

        return $this->redirect($this->generateUrl('homepage'));
    }

L'aggiornamento di un oggetto si esegue in tre passi:

#. recupero dell'oggetto da Propel;
#. modifica dell'oggetto;
#. salvataggio.

Cancellare un oggetto
~~~~~~~~~~~~~~~~~~~~~

La cancellazione di un oggetto è molto simile, ma richiede una chiamata al metodo
``delete()`` dell'oggetto::

    $product->delete();

Cercare gli oggetti
-------------------

Propel fornisce delle classi ``Query``, per eseguire query, semplici o complesse,
senza sforzo::

    \Acme\StoreBundle\Model\ProductQuery::create()->findPk($id);

    \Acme\StoreBundle\Model\ProductQuery::create()
        ->filterByName('Pippo')
        ->findOne();

Si immagini di voler cercare prodotti che costino più di 19.99, ordinati dal più
economico al più costoso. Da dentro un controllore, fare come segue::

    $products = \Acme\StoreBundle\Model\ProductQuery::create()
        ->filterByPrice(array('min' => 19.99))
        ->orderByPrice()
        ->find();

In una sola riga, si ottengono i prodotti cercati in modo orientato agli oggetti. Non
serve perdere tempo con SQL o simili, Symfony2 offre una programmazione completamente orientata
agli oggetti e Propel rispetta la stessa filosofia, fornendo un incredibile livello di
astrazione.

Se si vogliono riutilizzare delle query, si possono aggiungere i propri metodi alla
classe ``ProductQuery``::

    // src/Acme/StoreBundle/Model/ProductQuery.php
    class ProductQuery extends BaseProductQuery
    {
        public function filterByExpensivePrice()
        {
            return $this
                ->filterByPrice(array('min' => 1000));
        }
    }

Ma si noti che Propel genera diversi metodi per noi e un semplice
``findAllOrderedByName()`` può essere scritto senza sforzi::

    \Acme\StoreBundle\Model\ProductQuery::create()
        ->orderByName()
        ->find();

Relazioni/associazioni
----------------------

Si supponga che tutti i prodotti dell'applicazione appartengano a una delle categorie.
In questo caso, occorrerà un oggetto ``Category`` e un modo per correlare un oggetto
``Product`` a un oggetto ``Category``.

Si inizi aggiungendo la definizione di ``category`` al file ``schema.xml``:

.. code-block:: xml

    <database name="default" namespace="Acme\StoreBundle\Model" defaultIdMethod="native">
        <table name="product">
            <column name="id" type="integer" required="true" primaryKey="true" autoIncrement="true" />
            <column name="name" type="varchar" primaryString="true" size="100" />
            <column name="price" type="decimal" />
            <column name="description" type="longvarchar" />

            <column name="category_id" type="integer" />
            <foreign-key foreignTable="category">
                <reference local="category_id" foreign="id" />
            </foreign-key>
        </table>

        <table name="category">
            <column name="id" type="integer" required="true" primaryKey="true" autoIncrement="true" />
            <column name="name" type="varchar" primaryString="true" size="100" />
       </table>
    </database>

Creare le classi:

.. code-block:: bash

    $ php app/console propel:model:build

Ipotizziamo di avere già dei prodotti nella base dati e che non si voglia perderli. Grazie
alle migrazioni, Propel sarà in grado di aggiornare la base dati, senza perdere alcun
dato esistente.

.. code-block:: bash

    $ php app/console propel:migration:generate-diff
    $ php app/console propel:migration:migrate

La base dati è stata aggiornata, si può continuare nella scrittura dell'applicazione.

Salvare oggetti correlati
~~~~~~~~~~~~~~~~~~~~~~~~~

Vediamo ora un po' di codice in azione. Immaginiamo di essere dentro un controllore::

    // ...
    use Acme\StoreBundle\Model\Category;
    use Acme\StoreBundle\Model\Product;
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
            // mette in relazione questo prodotto alla categoria
            $product->setCategory($category);

            // salva tutto
            $product->save();

            return new Response(
                'Creato prodotto con id: '.$product->getId().' e categoria con id: '.$category->getId()
            );
        }
    }

Una singola riga è stata aggiunta alle tabelle ``category`` e ``product``. La colonna
``product.category_id`` del nuovo prodotto è stata impostata all'id della nuova
categoria. Propel gestisce la persistenza di questa relazione al posto
nostro.

Recuperare oggetti correlati
~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Quando serve recuperare oggetti correlati, il flusso di lavoro assomiglia del tutto al
precedente. Prima, recuperare un oggetto ``$product`` e quindi accedere alla ``Category``
relativa::

    // ...
    use Acme\StoreBundle\Model\ProductQuery;

    public function showAction($id)
    {
        $product = ProductQuery::create()
            ->joinWithCategory()
            ->findPk($id);

        $categoryName = $product->getCategory()->getName();

        // ...
    }

Si noti che, nell'esempio qui sopra, è stata eseguita una sola query.

Maggior informazioni sulle associazioni
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Si possono trovare maggiori informazioni sulle relazioni, leggendo il capitolo
dedicato alle `relazioni`_.

Callback del ciclo di vita
--------------------------

A volte, occorre eseguire un'azione appena prima (o appena dopo) che l'oggetto sia
inserito, aggiornato o cancellato. Questi tipi di azioni sono noti come "callback del
ciclo di vita" oppure come "hook", perché sono metodi callback che occorre eseguire
durante i diversi stadi del ciclo di vita di un oggetto (p.e. quando l'oggetto viene
inserito, aggiornato, cancellato, eccetera).

Per aggiungere un hook, basta aggiungere un nuovo metodo alla classe::

    // src/Acme/StoreBundle/Model/Product.php

    // ...
    class Product extends BaseProduct
    {
        public function preInsert(\PropelPDO $con = null)
        {
            // fare qualcosa prima che l'oggetto sia inserito
        }
    }

Propel fornisce i seguenti hook:

* ``preInsert()`` codice eseguito prima dell'inserimento di un nuovo oggetto
* ``postInsert()`` codice eseguito dopo l'inserimento di un nuovo oggetto
* ``preUpdate()`` codice eseguito prima dell'aggiornamento di un oggetto esistente
* ``postUpdate()`` codice eseguito dopo l'aggiornamento di un oggetto esistente
* ``preSave()`` codice eseguito prima di salvare un oggetto (nuovo o esistente)
* ``postSave()`` codice eseguito dopo il salvataggio di un oggetto (nuovo o esistente)
* ``preDelete()`` codice eseguito prima di cancellare un oggetto
* ``postDelete()`` codice eseguito dopo la cancellazione di un oggetto


Comportamenti
-------------

Tutti i comportamenti distribuiti con Propel funzionano in Symfony2. Per ottenere
maggiori informazioni su come usare i comportamenti di Propel, fare riferimento alla
sezione sui `behavior`_.

Comandi
-------

Leggere la sezione dedicata ai `comandi Propel in Symfony2`_.

.. _`Working With Symfony2`: http://propelorm.org/cookbook/symfony2/working-with-symfony2.html#installation
.. _`configurazione di PropelBundle`: http://propelorm.org/cookbook/symfony2/working-with-symfony2.html#configuration
.. _`relazioni`: http://propelorm.org/documentation/04-relationships.html
.. _`behavior`: http://propelorm.org/documentation/#behaviors_reference
.. _`comandi Propel in Symfony2`: http://propelorm.org/cookbook/symfony2/working-with-symfony2#the_commands
