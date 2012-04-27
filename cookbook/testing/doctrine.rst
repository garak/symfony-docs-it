.. index::
   single: Test; Doctrine

Come testare i repository Doctrine
==================================

I test unitari dei repository Doctrine in un progetto Symfony non sono un compito
facile. Infatti, per caricare un repository occorre caricare le entità, un gestore
di entità e un po' di altre cose, come una connessione.

Per testare i propri repository, ci sono due opzioni diverse:

1) **Test funzionali**: includono l'uso di una vera connessione alla base dati, con veri
   oggetti della base dati. Sono facili da preparare e possono testare tutto, ma sono lenti
   da eseguire. Vedere :ref:`cookbook-doctrine-repo-functional-test`.

2) **Test unitari**: i test unitari sono più veloci da eseguire e più precisi su
   cosa testare. Richiedono un po' più di preparazione, come vedremo in questo
   documento. Possono testare solo metodi che, per esempio, costruiscono query,
   non metodi che le eseguono effettivamente.

Test unitari
------------

Poiché Symfony e Doctrine condividono lo stesso framework di test, è facile implementare
test unitari nel proprio progetto Symfony. L'ORM ha il suo insieme di strumenti, che
facilitano i test unitari e i mock di ogni cosa di cui si abbia bisogno, come una
connessione, un gestore di entità, ecc. Usando i componenti dei test forniti da
Doctrine, con un po' di preparazione di base, si possono sfruttare gli strumenti di
Doctrine per testare i propri repository.

Si tenga a mente che, se si vuole testare la reale esecuzione delle proprie query,
occorrerà un test funzionale (vedere :ref:`cookbook-doctrine-repo-functional-test`).
I test unitari consentono solo di testare un metodo che costruisce una query.

Preparazione
~~~~~~~~~~~~

Innanzitutto, occorre aggiungere lo spazio dei nomi ``Doctrine\Tests`` al proprio autoloader::

    // app/autoload.php
    $loader->registerNamespaces(array(
        // ...
        'Doctrine\\Tests'                => __DIR__.'/../vendor/doctrine/tests',
        // ...
    ));

Poi, occorrerà preparare un gestore di entità in ogni test, in modo che Doctrine
possa caricare le entità e i repository.

Poiché Doctrine da solo non è in grado di caricare i meta-dati delle annotazioni dalle
entità, occorrerà configurare il lettore di annotazioni per poter analizzare e
caricare le entità::

    // src/Acme/ProductBundle/Tests/Entity/ProductRepositoryTest.php
    namespace Acme\ProductBundle\Tests\Entity;

    use Doctrine\Tests\OrmTestCase;
    use Doctrine\Common\Annotations\AnnotationReader;
    use Doctrine\ORM\Mapping\Driver\DriverChain;
    use Doctrine\ORM\Mapping\Driver\AnnotationDriver;

    class ProductRepositoryTest extends OrmTestCase
    {
        private $_em;

        protected function setUp()
        {
            $reader = new AnnotationReader();
            $reader->setIgnoreNotImportedAnnotations(true);
            $reader->setEnableParsePhpImports(true);

            $metadataDriver = new AnnotationDriver(
                $reader, 
                // fornisce lo spazio dei nomi delle entità che si vogliono testare
                'Acme\\ProductBundle\\Entity'
            );

            $this->_em = $this->_getTestEntityManager();

            $this->_em->getConfiguration()
              ->setMetadataDriverImpl($metadataDriver);

            // consente di usare la sintassi AcmeProductBundle:Product
            $this->_em->getConfiguration()->setEntityNamespaces(array(
                'AcmeProductBundle' => 'Acme\\ProductBundle\\Entity'
            ));
        }
    }

Guardando il codice, si può notare:

* Si estende da ``\Doctrine\Tests\OrmTestCase``, che fornisce metodi utili per i
  test unitari;

* Occorre preparare ``AnnotationReader`` per poter analizzare e caricare le
  entità;

* Si crea il gestore di entità, richiamando ``_getTestEntityManager``, che
  restituisce il mock di un gestore di entità, con il mock di una connessione.

Ecco fatto! Si è pronti per scrivere test unitari per i repository Doctrine.

Scrivere i test unitari
~~~~~~~~~~~~~~~~~~~~~~~

Tenere a mente che i metodi dei repository Doctrine possono essere testati solo se
costruiscono e restituiscono una query (senza eseguirla). Si consideri il
seguente esempio::

    // src/Acme/StoreBundle/Entity/ProductRepository
    namespace Acme\StoreBundle\Entity;

    use Doctrine\ORM\EntityRepository;

    class ProductRepository extends EntityRepository
    {
        public function createSearchByNameQueryBuilder($name)
        {
            return $this->createQueryBuilder('p')
                ->where('p.name LIKE :name')
                ->setParameter('name', $name);
        }
    }

In questo esempio, il metodo restituisce un'istanza di ``QueryBuilder``. Si può
testare il risultato di questo metodo in molti modi::

    class ProductRepositoryTest extends \Doctrine\Tests\OrmTestCase
    {
        /* ... */

        public function testCreateSearchByNameQueryBuilder()
        {
            $queryBuilder = $this->_em->getRepository('AcmeProductBundle:Product')
                ->createSearchByNameQueryBuilder('foo');

            $this->assertEquals('p.name LIKE :name', (string) $queryBuilder->getDqlPart('where'));
            $this->assertEquals(array('name' => 'foo'), $queryBuilder->getParameters());
        }
     }

In questo test, si disseziona l'oggetto ``QueryBuilder``, cercando che ogni parte sia
come ci si aspetta. Se si aggiungessero altre cose al costruttore di query,
si potrebbero verificare le parti DQL: ``select``, ``from``, ``join``, ``set``,
``groupBy``, ``having`` o ``orderBy``.

Se si ha solo un oggetto ``Query`` grezzo o se si preferisce testare la vera query,
si può testare direttamente la query DQL::

    public function testCreateSearchByNameQueryBuilder()
    {
        $queryBuilder = $this->_em->getRepository('AcmeProductBundle:Product')
            ->createSearchByNameQueryBuilder('foo');

        $query = $queryBuilder->getQuery();

        // testa la DQL
        $this->assertEquals(
            'SELECT p FROM Acme\ProductBundle\Entity\Product p WHERE p.name LIKE :name',
            $query->getDql()
        );
    }

.. _cookbook-doctrine-repo-functional-test:

Test funzionali
---------------

Se occorre eseguire effettivamente una query, occorrerò far partire il kernel, per
ottenere una connessione valida. In questo caso, si estenderà ``WebTestCase``,
che rende tutto alquanto facile::

    // src/Acme/ProductBundle/Tests/Entity/ProductRepositoryFunctionalTest.php
    namespace Acme\ProductBundle\Tests\Entity;

    use Symfony\Bundle\FrameworkBundle\Test\WebTestCase;

    class ProductRepositoryFunctionalTest extends WebTestCase
    {
        /**
         * @var \Doctrine\ORM\EntityManager
         */
        private $_em;

        public function setUp()
        {
          $kernel = static::createKernel();
          $kernel->boot();
            $this->_em = $kernel->getContainer()
                ->get('doctrine.orm.entity_manager');
        }

        public function testProductByCategoryName()
        {
            $results = $this->_em->getRepository('AcmeProductBundle:Product')
                ->searchProductsByNameQuery('foo')
                ->getResult();

            $this->assertEquals(count($results), 1);
        }
    }
