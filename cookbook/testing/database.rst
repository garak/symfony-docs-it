.. index::
   single: Test; Base dati

Testare codice che interagisce con la base dati
===============================================

Se si ha un codice che interagisce con la base dati, cioè legge o memorizza dati,
occorre modificare i test per considerare la questione. Ci sono molti
modi per poterlo fare. In un test unitario, si può creare un mock
per un ``Repository`` e usarlo per restituire gli oggetti attesi. In un test funzionale,
potrebbe essere necessario preparare una base dati di test con valori predefiniti, per assicurarsi
di testare sempre gli stessi dati.

.. note::

    Se si vogliono testare direttamente le query, vedere :doc:`/cookbook/testing/doctrine`.

Mock di ``Repository`` in un test unitario
------------------------------------------

Se si vuole testare codice che dipende da un ``Repository`` Doctrine in isolamento,
occorre un  mock di ``Repository``. Normalmente, si inietta ``EntityManager``
nella propria classe e lo si usa per ottenere il repository. Questo rende le cose un po'
più difficili, perché occorrono mock sia di ``EntityManager`` che della classe
repository.

.. tip::

    È possibile (ed è una buona idea) iniiettare il repository direttamente
    registrandolo come :doc:`servizio factory </components/dependency_injection/factories>`
    C'è da fare un po' di lavoro in più per la preparazione, ma facilita i test, perché
    serve solo il mock del repository.

Supponiamo che la classe da testare sia come questa::

    namespace Acme\DemoBundle\Salary;

    use Doctrine\Common\Persistence\ObjectManager;

    class SalaryCalculator
    {
        private $entityManager;

        public function __construct(ObjectManager $entityManager)
        {
            $this->entityManager = $entityManager;
        }

        public function calculateTotalSalary($id)
        {
            $employeeRepository = $this->entityManager->getRepository('AcmeDemoBundle::Employee');
            $employee = $userRepository->find($id);

            return $employee->getSalary() + $employee->getBonus();
        }
    }

Poiché ``ObjectManager`` viene iniettato nella classe tramite il costruttore,
è facile passare un oggetto mock in un test::

    use Acme\DemoBundle\Salary\SalaryCalculator;

    class SalaryCalculatorTest extends \PHPUnit_Framework_TestCase
    {

        public function testCalculateTotalSalary()
        {
            // il primo mock che serve è quello da usare nel test
            $employee = $this->getMock('\Acme\DemoBundle\Entity\Employee');
            $employee->expects($this->once())
                ->method('getSalary')
                ->will($this->returnValue(1000));
            $employee->expects($this->once())
                ->method('getBonus')
                ->will($this->returnValue(1100));

            // ora serve il mock del repository, in modo che restitsuica il mock di Employee
            $employeeRepository = $this->getMockBuilder('\Doctrine\ORM\EntityRepository')
                ->disableOriginalConstructor()
                ->getMock();
            $employeeRepository->expects($this->once())
                ->method('find')
                ->will($this->returnValue($employee));

            // infine, serve il mock di EntityManager, per restituire il mock del repository
            $entityManager = $this->getMockBuilder('\Doctrine\Common\Persistence\ObjectManager')
                ->disableOriginalConstructor()
                ->getMock();
            $entityManager->expects($this->once())
                ->method('getRepository')
                ->will($this->returnValue($employeeRepository));

            $salaryCalculator = new SalaryCalculator($entityManager);
            $this->assertEquals(2100, $salaryCalculator->calculateTotalSalary(1));
        }
    }

In questo esempio, i mock sono stati costruiti partendo dall'interno, creando prima
Employee, restituito  da ``Repository``, restituito a sua volta
da ``EntityManager``. IN questo modo, nessuna classe reale è stata coinvolta nel
test.

Modifica delle impostazioni per test funzionali    
-----------------------------------------------

In caso di test funzionali, si vuole che interagiscano con una base dati reale.
La maggior parte delle volte si vuole usare una connessione dedicata, per assicurarsi
di non sovrascrivere dati inseriti durante lo sviluppo dell'applicazione e anche
per poter pulire la base dati prima di ogni test.

Per poterlo fare, si può specificare una configurazione, che sovrascriva quella
predefinita:

.. configuration-block::

    .. code-block:: yaml

        # app/config/config_test.yml
        doctrine:
            # ...
            dbal:
                host: localhost
                dbname: testdb
                user: testdb
                password: testdb

    .. code-block:: xml

        <!-- app/config/config_test.xml -->
        <doctrine:config>
            <doctrine:dbal
                host="localhost"
                dbname="testdb"
                user="testdb"
                password="testdb"
            />
        </doctrine:config>

    .. code-block:: php

        // app/config/config_test.php
        $configuration->loadFromExtension('doctrine', array(
            'dbal' => array(
                'host'     => 'localhost',
                'dbname'   => 'testdb',
                'user'     => 'testdb',
                'password' => 'testdb',
            ),
        ));

Assicurarsi che la base dati sia in esecuzione su localhost, che la base dati esista
e che le credenziali siano corrette.
