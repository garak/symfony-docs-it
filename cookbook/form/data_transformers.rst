Utilizzare i Data Transformers
=======================

Spesso si avrà la necessità di trasformare i dati che l'utente ha immesso in un form in
qualcosa di diverso da utilizzare nel programma. Tutto questo si potrebbe fare manualmente nel
controller ma nel caso in cui si volesse utilizzare il form in posti diversi?

Supponiamo di avere una relazione uno-a-uno tra Task e Rilasci, per esempio un Task può avere un
rilascio associato. Avere una casella di riepilogo con la lista di tutti i rilasci può portare ad
una casella di riepilogo molto lunga nella quale risulterà impossibile cercare qualcosa. Si vorrebbe, piuttosto,
aggiungere un campo di testo nel quale l'utente può semplicementere inserire il numero del rilascio. Nel
controller si può convertire questo numero di rilascio in un task attuale ed eventualmente aggiungere
errori al form se non è stato trovato ma questo non è il modo migliore di procedere.

Sarebbe meglio se questo rilascio fosse cercato automaticamente e convertito in un
oggetto Rilascio, in modo da poterlo utilizzare nell'azione. In questi casi entrano in gioco i Data Transformers.

Come prima cosa, bisogna creare un form che abbia un Data Transformer collegato che,
dato un numero, ritorni un oggetto Rilascio: il tipo selettore rilascio. Eventualmente sarà semplicemente 
un campo di testo, dato che la configurazione dei campi che estendono è impostata come campo di testo, nel quale
si potrà inserire il numero di rilascio. Il campo di testo farà comparire un errore se verrà inserito
un numero di rilascio che non esiste::

    // src/Acme/TaskBundle/Form/IssueSelectorType.php
    namespace Acme\TaskBundle\Form\Type;
    
    use Symfony\Component\Form\AbstractType;
    use Symfony\Component\Form\FormBuilder;
    use Acme\TaskBundle\Form\DataTransformer\IssueToNumberTransformer;
    use Doctrine\Common\Persistence\ObjectManager;

    class IssueSelectorType extends AbstractType
    {
        private $om;
    
        public function __construct(ObjectManager $om)
        {
            $this->om = $om;
        }
    
        public function buildForm(FormBuilder $builder, array $options)
        {
            $transformer = new IssueToNumberTransformer($this->om);
            $builder->appendClientTransformer($transformer);
        }
    
        public function getDefaultOptions(array $options)
        {
            return array(
                'invalid_message'=>'Il rilascio che cerchi non esiste.'
            );
        }
    
        public function getParent(array $options)
        {
            return 'text';
        }
    
        public function getName()
        {
            return 'issue_selector';
        }
    }

.. tip::

    È possibile utilizzare i transformers senza necessariamente creare un nuovo form
    personalizzato invocando la funzione ``appendClientTransformer`` su qualsiasi field builder::

        use Acme\TaskBundle\Form\DataTransformer\IssueToNumberTransformer;

        class TaskType extends AbstractType
        {
            public function buildForm(FormBuilder $builder, array $options)
            {
                // ...
            
                // si assume che l'entity manager è stato passato come option
                $entityManager = $options['em'];
                $transformer = new IssueToNumberTransformer($entityManager);

                // utilizza un campo di testo ma trasforma il testo in un oggetto rilascio
                $builder
                    ->add('issue', 'text')
                    ->appendClientTransformer($transformer)
                ;
            }
            
            // ...
        }

Quindi, creiamo il data transformer che effettua la vera e propria conversione::

    // src/Acme/TaskBundle/Form/DataTransformer/IssueToNumberTransformer.php
    namespace Acme\TaskBundle\Form\DataTransformer;
    
    use Symfony\Component\Form\Exception\TransformationFailedException;
    use Symfony\Component\Form\DataTransformerInterface;
    use Doctrine\Common\Persistence\ObjectManager;
    
    class IssueToNumberTransformer implements DataTransformerInterface
    {
        private $om;

        public function __construct(ObjectManager $om)
        {
            $this->om = $om;
        }

        // trasforma l'oggetto Rilascio in una stringa
        public function transform($val)
        {
            if (null === $val) {
                return '';
            }

            return $val->getNumber();
        }

        // trasforma il numero rilascio in un oggetto Rilascio
        public function reverseTransform($val)
        {
            if (!$val) {
                return null;
            }

            $issue = $this->om->getRepository('AcmeTaskBundle:Issue')->findOneBy(array('number' => $val));

            if (null === $issue) {
                throw new TransformationFailedException(sprintf('Un rilascio con numero %s non esiste', $val));
            }

            return $issue;
        }
    }

Infine, poichè abbiamo deciso di creare un campo di testo personalizzato che utilizza
il data transformer, bisogna registrare il tipo nel service container, in modo che l'entity
manager può essere automaticamente iniettato:

.. configuration-block::

    .. code-block:: yaml

        services:
            acme_demo.type.issue_selector:
                class: Acme\TaskBundle\Form\IssueSelectorType
                arguments: ["@doctrine.orm.entity_manager"]
                tags:
                    - { name: form.type, alias: issue_selector }

    .. code-block:: xml
    
        <service id="acme_demo.type.issue_selector" class="Acme\TaskBundle\Form\IssueSelectorType">
            <argument type="service" id="doctrine.orm.entity_manager"/>
            <tag name="form.type" alias="issue_selector" />
        </service>

Ora è possibile aggiungere il tipo al form dal suo alias come segue::

    // src/Acme/TaskBundle/Form/Type/TaskType.php
    
    namespace Acme\TaskBundle\Form\Type;
    
    use Symfony\Component\Form\AbstractType;
    use Symfony\Component\Form\FormBuilder;
    
    class TaskType extends AbstractType
    {
        public function buildForm(FormBuilder $builder, array $options)
        {
            $builder->add('task');
            $builder->add('dueDate', null, array('widget' => 'single_text'));
            $builder->add('issue', 'issue_selector');
        }
    
        public function getName()
        {
            return 'task';
        }
    }

Ora sarà molto facile in qualsiasi punto dell'applicazione, usare questo
tipo selettore per selezionare un rilascio da un numero. Tutto questo, senza aggiungere nessuna logica 
al Controller.

Se si vuole creare un nuovo rilascio quando viene inserito un numero di rilascio sconosciuto,
è possibile istanziarlo piuttosto che lanciare l'eccezione TransformationFailedException e
inoltre persiste nel tuo entity manager se il task non ha opzioni a cascata
per il rilascio.
