.. index::
   single: Test; Doctrine

Come testare i repository Doctrine
==================================

I test unitari dei repository Doctrine in un progetto Symfony non sono raccomandati.
Quando si ha a che fare con un repository, si sta trattando qualcosa che ha effettivamente
bisogno di essere testata con una versa connessione alla base dati.

Per fortuna, si possono facilmente testare le query su una base dati reale, come descritto
qui sotto.

.. _cookbook-doctrine-repo-functional-test:

Test funzionali
---------------

Se occorre eseguire effettivamente una query, occorrerò far partire il kernel, per
ottenere una connessione valida. In questo caso, si estenderà ``WebTestCase``,
che rende tutto alquanto facile::

    // src/Acme/StoreBundle/Tests/Entity/ProductRepositoryFunctionalTest.php

    namespace Acme\StoreBundle\Tests\Entity;

    use Symfony\Bundle\FrameworkBundle\Test\WebTestCase;

    class ProductRepositoryFunctionalTest extends WebTestCase
    {
        /**
         * @var \Doctrine\ORM\EntityManager
         */
        private $em;

        public function setUp()
        {
            $kernel = static::createKernel();
            $kernel->boot();
            $this->em = $kernel->getContainer()->get('doctrine.orm.entity_manager');
        }

        public function testProductByCategoryName()
        {
            $results = $this->em
                ->getRepository('AcmeStoreBundle:Product')
                ->searchProductsByNameQuery('foo')
                ->getResult()
            ;

            $this->assertCount(1, $results);
        }
    }
