.. index::
   single: Form; Test dei form

Test unitari sui form
=====================

Il componente Form è formato da tre oggetti principali: il tipo (che implementa
:class:`Symfony\\Component\\Form\\FormTypeInterface`), il
:class:`Symfony\\Component\\Form\\Form` e il
:class:`Symfony\\Component\\Form\\FormView`.

Di solito lo sviluppatore manipola solamente la classe tipo,
che funge da canovaccio per un form. Essa viene usata per generare gli oggetti ``Form`` e
``FormView``. Potrebbe essere testata direttamente, con dei mock delle sue interazioni con il
factory, ma sarebbe complesso. È meglio passarla al FormFactory, come viene fatto
in un'applicazione reale. È semplice da preparare e ci si può fidare abbastanza
dei componenti di Symfony per usarla come base per i test.

C'è già una classe che può beneficiare di semplici test dei FormType:
:class:`Symfony\\Component\\Form\\Tests\\Extension\\Core\\Type\\TypeTestCase`.
Questa classe viene usata per testare i tipi del nucleo e la si può usare per testare i propri tipi.

.. note::

    A seconda del modo in cui si sono installati i componenti di Symfony o il framework,
    i test potrebbero non essere stati scaricati. Usare l'opzione ``--prefer-source`` di
    composer, nel caso mancassero.

I fondamentali
--------------

L'implementazione più semplice di ``TypeTestCase`` assomiglia a questa::

    // src/Acme/TestBundle/Tests/Form/Type/TestedTypeTests.php
    namespace Acme\TestBundle\Tests\Form\Type;

    use Acme\TestBundle\Form\Type\TestedType;
    use Acme\TestBundle\Model\TestObject;
    use Symfony\Component\Form\Tests\Extension\Core\Type\TypeTestCase;

    class TestedTypeTest extends TypeTestCase
    {
        public function testBindValidData()
        {
            $formData = array(
                'test' => 'test',
                'test2' => 'test2',
            );

            $type = new TestedType();
            $form = $this->factory->create($type);

            $object = new TestObject();
            $object->fromArray($formData);

            $form->bind($formData);

            $this->assertTrue($form->isSynchronized());
            $this->assertEquals($object, $form->getData());

            $view = $form->createView();
            $children = $view->children;

            foreach (array_keys($formData) as $key) {
                $this->assertArrayHasKey($key, $children);
            }
        }
    }

Che cosa testa? Vediamolo riga per riga.

Innanzitutto, si verifica che ``FormType`` compili. Ciò include l'ereditarietà di base
della classe, la funzione ``buildForm`` e la risoluzione delle opzioni. Questo dovrebbe
essere il primo test a essere scritto::

    $type = new TestedType();
    $form = $this->factory->create($type);

Questo test verifica che nessun trasformatore di dati usato dal form
fallisca. Il metodo :method:`Symfony\\Component\\Form\\FormInterface::isSynchronized``
è impostato a ``false`` solo se un trasformatore di dati lancia un'eccezione::

    $form->bind($formData);
    $this->assertTrue($form->isSynchronized());

.. note::

    Non testare la validazione: viene applicata da un ascoltatore, c he non
    è attivo in caso di test ed è basata sulla configurazione della validazione.
    Invece, testare unitariamente i propri vincoli personalizzati, direttamente.

Il passo successivo consiste nel verificare il bind e la mappatura del form. Il test
seguente verifica se tutti i campi siano specificati correttamente::

    $this->assertEquals($object, $form->getData());

Infine, verificare la creazione di ``FormView``. Si deve verificare se tutti i
widget che si vogliono mostrare siano disponibili nella proprietà ``children``::

    $view = $form->createView();
    $children = $view->children;

    foreach (array_keys($formData) as $key) {
        $this->assertArrayHasKey($key, $children);
    }

Aggiungere un tipo da cui il form dipende
-----------------------------------------

Un form potrebbe dipendere da altri tipi, definiti come servizi. Una
cosa del genere::

    // src/Acme/TestBundle/Form/Type/TestedType.php

    // ... il metodo buildForm
    $builder->add('acme_test_child_type');

Per creare correttamente il form, occorre rendere il tipo disponibile al
form factory del test. Il modo più facile è registrarlo manualmente,
prima di creare il form genitore::

    // src/Acme/TestBundle/Tests/Form/Type/TestedTypeTests.php
    namespace Acme\TestBundle\Tests\Form\Type;

    use Acme\TestBundle\Form\Type\TestedType;
    use Acme\TestBundle\Model\TestObject;
    use Symfony\Component\Form\Tests\Extension\Core\Type\TypeTestCase;

    class TestedTypeTest extends TypeTestCase
    {
        public function testBindValidData()
        {
            $this->factory->addType(new TestChildType());

            $type = new TestedType();
            $form = $this->factory->create($type);

            // ... il test
        }
    }

.. caution::

    Assicurarsi che il tipo figlio che si aggiunge sia ben testato. In caso contrario,
    si potrebbero avere errori che non dipendono dal form che si sta testando
    attualmente, ma dai suoi figli.

Aggiungere estensioni personalizzate
------------------------------------

Spesso accade di usare alcune opzioni aggiunte da
:doc:`estensioni di form</cookbook/form/create_form_type_extension>`. Uno dei casi può
essere ``ValidatorExtension``, con la sua opzione ``invalid_message``.
``TypeTestCase`` carica solo le estensioni base del form, quindi sarà lanciata
un'eccezione "Invalid option", se si prova a usarlo per testare una classe che dipenda
da altre estensioni. Occorre aggiungere tali estensioni all'oggetto factory::

    // src/Acme/TestBundle/Tests/Form/Type/TestedTypeTests.php
    namespace Acme\TestBundle\Tests\Form\Type;

    use Acme\TestBundle\Form\Type\TestedType;
    use Acme\TestBundle\Model\TestObject;
    use Symfony\Component\Form\Tests\Extension\Core\Type\TypeTestCase;

    class TestedTypeTest extends TypeTestCase
    {
        protected function setUp()
        {
            parent::setUp();

            $this->factory = Forms::createFormFactoryBuilder()
                ->addTypeExtension(
                    new FormTypeValidatorExtension(
                        $this->getMock('Symfony\Component\Validator\ValidatorInterface')
                    )
                )
                ->addTypeGuesser(
                    $this->getMockBuilder(
                        'Symfony\Component\Form\Extension\Validator\ValidatorTypeGuesser'
                    )
                        ->disableOriginalConstructor()
                        ->getMock()
                )
                ->getFormFactory();

            $this->dispatcher = $this->getMock('Symfony\Component\EventDispatcher\EventDispatcherInterface');
            $this->builder = new FormBuilder(null, null, $this->dispatcher, $this->factory);
        }

        // ... i test
    } 

Testare diversi insiemi di dati
-------------------------------

Se non si è mai provato a usare i `data provider`_ di PHPUnit, questa può
essere una buona occasione::

    // src/Acme/TestBundle/Tests/Form/Type/TestedTypeTests.php
    namespace Acme\TestBundle\Tests\Form\Type;

    use Acme\TestBundle\Form\Type\TestedType;
    use Acme\TestBundle\Model\TestObject;
    use Symfony\Component\Form\Tests\Extension\Core\Type\TypeTestCase;

    class TestedTypeTest extends TypeTestCase
    {

        /**
         * @dataProvider getValidTestData
         */
        public function testForm($data)
        {
            // ... il test
        }

        public function getValidTestData()
        {
            return array(
                array(
                    'data' => array(
                        'test' => 'test',
                        'test2' => 'test2',
                    ),
                ),
                array(
                    'data' => array(),
                ),
                array(
                    'data' => array(
                        'test' => null,
                        'test2' => null,
                    ),
                ),
            );
        }
    }

Qeusto codice eseguira il test tre volte, con tre diversi insiemi di
dati. Questo consente di disaccoppiare le fixture dei test dai test stessi e
di testare facilmente insiemi diversi di dati.

Si può anche passare un altro parametro, come un booleano che dice se il form debba
essere sincronizzato con il dato insiemi di dati o meno.

.. _`data provider`: http://www.phpunit.de/manual/current/en/writing-tests-for-phpunit.html#writing-tests-for-phpunit.data-providers
