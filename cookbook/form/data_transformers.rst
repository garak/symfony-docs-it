.. index::
   single: Form; Data transformer

Utilizzare i data transformer
=============================

Spesso si avrà la necessità di trasformare i dati che l'utente ha immesso in un form in
qualcosa di diverso da utilizzare nel programma. Tutto questo si potrebbe fare manualmente nel
controllore, ma nel caso in cui si volesse utilizzare il form in posti diversi?

Supponiamo di avere una relazione uno-a-uno tra Task e Issue, per esempio un Task può avere una
Issue associata. Avere una casella di riepilogo con la lista di tuttr le issue può portare a
una casella di riepilogo molto lunga, nella quale risulterà impossibile cercare qualcosa. Si vorrebbe, piuttosto,
aggiungere un campo di testo nel quale l'utente può semplicemente inserire il numero della issue. Nel
controllore si può convertire questo numero di issue in un task ed eventualmente aggiungere
errori al form, se non è stato trovato, ma questo non è il modo migliore di procedere.

Sarebbe meglio se questa issue fosse cercata automaticamente e convertita in un
oggetto Issue, in modo da poterlo utilizzare nell'azione. In questi casi entrano in gioco i data transformer.

Come prima cosa, bisogna creare un form che abbia un data transformer collegato che,
dato un numero, ritorni un oggetto Issue: il tipo selettore di issue. Eventualmente sarà semplicemente 
un campo di testo, dato che la configurazione dei campi che estendono è impostata come campo di testo, nel quale
si potrà inserire il numero di issue. Il campo di testo farà comparire un errore se verrà inserito
un numero di issue che non esiste::

    // src/Acme/TaskBundle/Form/Type/IssueSelectorType.php
    namespace Acme\TaskBundle\Form\Type;
    
    use Symfony\Component\Form\AbstractType;
    use Symfony\Component\Form\FormBuilderInterface;
    use Symfony\Component\OptionsResolver\OptionsResolverInterface;
    use Acme\TaskBundle\Form\DataTransformer\IssueToNumberTransformer;
    use Doctrine\Common\Persistence\ObjectManager;

    class IssueSelectorType extends AbstractType
    {
        /**
         * @var ObjectManager
         */
        private $om;
    
        /**
         * @param ObjectManager $om
         */
        public function __construct(ObjectManager $om)
        {
            $this->om = $om;
        }
    
        public function buildForm(FormBuilderInterface $builder, array $options)
        {
            $transformer = new IssueToNumberTransformer($this->om);
            $builder->addViewTransformer($transformer);
        }
    
        public function getParent()
        {
            return 'text';
        }
    
        public function getName()
        {
            return 'issue_selector';
        }
    }

.. tip::

    È possibile utilizzare i transformer senza necessariamente creare un nuovo form
    personalizzato invocando la funzione ``appendClientTransformer`` su qualsiasi field builder::

        use Symfony\Component\Form\FormBuilderInterface;
        use Acme\TaskBundle\Form\DataTransformer\IssueToNumberTransformer;

        class TaskType extends AbstractType
        {
            public function buildForm(FormBuilderInterface $builder, array $options)
            {
                // ...
            
                // si assume che l'entity manager è stato passato come opzione
                $entityManager = $options['em'];
                $transformer = new IssueToNumberTransformer($entityManager);

                // utilizza un campo di testo ma trasforma il testo in un oggetto issue
                $builder
                    ->add('issue', 'text')
                    ->addViewTransformer($transformer)
                ;
            }
            
            // ...
        }

quindi, creiamo il data transformer che effettua la vera e propria conversione::

    // src/Acme/TaskBundle/Form/DataTransformer/IssueToNumberTransformer.php

    namespace Acme\TaskBundle\Form\DataTransformer;
    
    use Symfony\Component\Form\DataTransformerInterface;
    use Symfony\Component\Form\Exception\TransformationFailedException;
    use Doctrine\Common\Persistence\ObjectManager;
    use Acme\TaskBundle\Entity\Issue;
    
    class IssueToNumberTransformer implements DataTransformerInterface
    {
        /**
         * @var ObjectManager
         */
        private $om;

        /**
         * @param ObjectManager $om
         */
        public function __construct(ObjectManager $om)
        {
            $this->om = $om;
        }

        /**
         * Trasforma un oggetto (issue) in una stringa (number)
         *
         * @param  Issue|null $issue
         * @return string
         */
        public function transform($issue)
        {
            if (null === $issue) {
                return "";
            }

            return $issue->getNumber();
        }

        /**
         * Trasforma una  stringa (number) in un oggetto (issue).
         *
         * @param  string $number
         * @return Issue|null
         * @throws TransformationFailedException if object (issue) is not found.
         */
        public function reverseTransform($number)
        {
            if (!$number) {
                return null;
            }

            $issue = $this->om
                ->getRepository('AcmeTaskBundle:Issue')
                ->findOneBy(array('number' => $number))
            ;

            if (null === $issue) {
                throw new TransformationFailedException(sprintf(
                    'Non esiste una issue con numero "%s"!',
                    $number
                ));
            }

            return $issue;
        }
    }

Infine, poiché abbiamo deciso di creare un campo di testo personalizzato che utilizza
il data transformer, bisogna registrare il tipo nel service container, in modo che l'entity
manager può essere automaticamente iniettato:

.. configuration-block::

    .. code-block:: yaml

        services:
            acme_demo.type.issue_selector:
                class: Acme\TaskBundle\Form\Type\IssueSelectorType
                arguments: ["@doctrine.orm.entity_manager"]
                tags:
                    - { name: form.type, alias: issue_selector }

    .. code-block:: xml
    
        <service id="acme_demo.type.issue_selector" class="Acme\TaskBundle\Form\Type\IssueSelectorType">
            <argument type="service" id="doctrine.orm.entity_manager"/>
            <tag name="form.type" alias="issue_selector" />
        </service>

Ora è possibile aggiungere il tipo al form dal suo alias come segue::

    // src/Acme/TaskBundle/Form/Type/TaskType.php
    
    namespace Acme\TaskBundle\Form\Type;
    
    use Symfony\Component\Form\AbstractType;
    use Symfony\Component\Form\FormBuilderInterface;
    
    class TaskType extends AbstractType
    {
        public function buildForm(FormBuilderInterface $builder, array $options)
        {
            $builder
                ->add('task')
                ->add('dueDate', null, array('widget' => 'single_text'));
                ->add('issue', 'issue_selector')
            ;
        }
    
        public function getName()
        {
            return 'task';
        }
    }

Ora sarà molto facile in qualsiasi punto dell'applicazione, usare questo
tipo selettore per selezionare una issue da un numero. Tutto questo, senza aggiungere nessuna logica 
al controllore.

Se si vuole creare una nuova issue quando viene inserito un numero di issue sconosciuto,
è possibile istanziarlo piuttosto che lanciare l'eccezione TransformationFailedException e
inoltre persiste nel proprio entity manager se il task non ha opzioni di cascata
per la issue.
