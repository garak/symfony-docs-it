.. index::
   single: Form; Trasformatori di dati

Utilizzare i trasformatori di dati
==================================

Spesso si avrà la necessità di trasformare i dati che l'utente ha immesso in un form in
qualcosa di diverso da utilizzare nel programma. Tutto questo si potrebbe fare manualmente nel
controllore, ma nel caso in cui si volesse utilizzare il form in posti diversi?

Supponiamo di avere una relazione uno-a-uno tra Task e Issue, per esempio un Task può avere una
Issue associata. Avere una casella di riepilogo con la lista di tutte le issue può portare a
una casella di riepilogo molto lunga, nella quale risulterà impossibile cercare qualcosa. Si vorrebbe, piuttosto,
aggiungere un campo di testo nel quale l'utente può semplicemente inserire il numero della issue.

Lo si potrebbe fare nel controllore, ma non è la soluzione migliore.
Sarebbe meglio se questa issue fosse cercata automaticamente e convertita in un oggetto Issue.
In questi casi entrano in gioco i trasformatori di dati.

.. caution::

    Quando un campo ha l'opzione ``inherit_data`` impostata, i trasformatori di dati
    non saranno applicati.

Creare il trasformatore
-----------------------

Come prima cosa, creare una classe `IssueToNumberTransformer`, che sarà responsabile
della conversione da numero di issue a oggetto Issue e viceversa::

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
         * Trasforma un oggetto (issue) in una stringa (number).
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
         * Trasforma una stringa (number) in un oggetto (issue).
         *
         * @param  string $number
         *
         * @return Issue|null
         *
         * @throws TransformationFailedException se l'oggetto (issue) non viene trovato.
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

.. tip::

    Se si vuole che sia create una nuova issue all'inserimento di un numero non trovato, si
    può crearne l'istanza invece di lanciare ``TransformationFailedException``.

.. note::

    Se si passa ``null`` al metodo ``transform()``, il trasformatore dovrebbe
    restituire un valore equivalente al tipo in cui sta trasformando (p.e.
    una stringa vuota, 0 per gli interi o 0.0 per i numeri a virgola mobile).

Usare il trasformatore
----------------------

Dopo aver creato il trasformatore, basta aggiungerlo al campo issue in
un form.

    Si possono anche usare i trasformatori senza creare un nuovo tipo di form,
    richiamando ``addModelTransformer`` (o ``addViewTransformer``, vedere
    `Trasformatore per modelli e viste`_) sul builder di un campo::

    use Symfony\Component\Form\FormBuilderInterface;
    use Acme\TaskBundle\Form\DataTransformer\IssueToNumberTransformer;

    class TaskType extends AbstractType
    {
        public function buildForm(FormBuilderInterface $builder, array $options)
        {
            // ...

            // "em" è un'opzione da passare alla creazione del form. Vedere
            // il terzo parametro di createForm nel prossimo blocco di codice per capire
            // in  che modo è passata al form (vedere anche setDefaultOptions).
            $entityManager = $options['em'];
            $transformer = new IssueToNumberTransformer($entityManager);

            // aggiunge un normale campo testuale, ma vi aggiunge il trasformatore
            $builder->add(
                $builder->create('issue', 'text')
                    ->addModelTransformer($transformer)
            );
        }

        public function setDefaultOptions(OptionsResolverInterface $resolver)
        {
            $resolver
                ->setDefaults(array(
                    'data_class' => 'Acme\TaskBundle\Entity\Task',
                ))
                ->setRequired(array(
                    'em',
                ))
                ->setAllowedTypes(array(
                    'em' => 'Doctrine\Common\Persistence\ObjectManager',
                ));

            // ...
        }

        // ...
    }

Questo esempio richiede il passaggio del gestore di entità come opzione, al momento
di creare il form. Successivamente, si vedrà come si può creare un tipo di campo
``issue`` personalizzato, per evitare di doverlo fare nel controllore::

    $taskForm = $this->createForm(new TaskType(), $task, array(
        'em' => $this->getDoctrine()->getEntityManager(),
    ));

Ecco fatto! L'utente sarà in grado di inserire un numero di issue nel campo di
testo e di vederlo trasformato in un oggetto Issue. Questo vuol dire che,
dopo l'invio del form, il framework passerà un vero oggetto Issue
a ``Task::setIssue()`` e non un numero di issue.

Se la issue non viene trovata, sarà creato un errore per il campo, con un messaggio
controllabile dall'opzione del campo ``invalid_message``.

.. caution::

    Notare che l'aggiunta di un trasformatore richiede l'uso di una sintassi leggermente più
    complessa, durante l'aggiunta del campo. Quello che segue è *errato*, perché il trasformatore
    sarebbe applicato all'intero form, invece che solo a un campo::

        // QUESTO NON VA BENE: IL TRASFORMATORE SARÀ APPLICATO ALL'INTERO FORM
        // vedere l'esempio precedente per il codice corretto
        $builder->add('issue', 'text')
            ->addModelTransformer($transformer);

Trasformatore per modelli e viste
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Nell'esempio precedente, il trasformatore è stato usato come trasformatore di modello.
Infatti, ci sono due diversi tipi di trasformatori e tre diversi tipi di
dati sottostanti.

.. image:: /images/cookbook/form/DataTransformersTypes.png
   :align: center

In un form, i tre diversi tipi di dati sono:

1) **Dati di modello** - sono i dati nel formato usato dall'applicazione
(p.e. un oggetto ``Issue``). Richiamando ``Form::getData`` o ``Form::setData``, 
si ha a che fare con dati di "modello".

2) **Dati normali** - la versione normalizzata dei dati, solitamente gli stessi dati
del "modello" (ma non nel nostro esempio). Solitamente non sono usati
direttamente.

3) **Dati di vista** - il formato usato per riempire i campi stessi del form.
È anche il formato in cui l'utente invierà i dati. Quando si richiama
``Form::bind($data)``, ``$data`` + nel formato di dati "vista".

I due diversi tipi di trasformatori aiutano a convertire da e per ciascuno di questi
tipi di dati:

**Trasformatori di modello**:
    - ``transform``: "dati modello" => "dati normali"
    - ``reverseTransform``: "dati normali" => "dati modello"

**Trasformatori di vista**:
    - ``transform``: "dati normali" => "dati vista"
    - ``reverseTransform``: "dati vista" => "dati normali"

A seconda della situazione, occorrerà un tasformatore diverso.

Per usare un trasformatore di vista, richiamare ``addViewTransformer``.

Perché abbiamo usato i trasformatori di modello?
------------------------------------------------

Nel nostro esempio, il campo è di tipo ``text`` e ci aspettiamo sempre che un campo testo
sia in formato semplice e scalare, nei formati "normale" e "vista". Per tale
ragione, il trasformatore più adeguato era il trasformatore "modello"
(che converte da/a formato *normale*, il numero di issue, al formato *modello*,
l'oggetto Issue.

La differenza tra i trasformatori è sottile, si dovrebbe sempre pensare quali
dati "normali" un campo dovrebbe avere realmente. Per esempio, i dati
"normali" per un campo ``text`` sono stringhe, ma c'è un oggetto ``DateTime``
per un campo ``date``.

Usare i trasformatori in un tipo di campo personalizzato
--------------------------------------------------------

Nell'esempio precedente, abbiamo applicato i trasformatori a un normale campo ``text``.
È stato facile, ma con due svantaggi:

1) Si deve sempre ricordare di applicare i trasformatori ogni volta che si aggiunge un campo
per i numeri di issue

2) Ci si deve preoccupare di passare l'opzione ``em`` ogni volta che si crea un form
che usi i trasformatori.

Per questi motivi, si potrebbe scegliere di :doc:`creare un tipo di campo personalizzato</cookbook/form/create_custom_field_type>`.
Prima di tutto, creare una classe::

    // src/Acme/TaskBundle/Form/Type/IssueSelectorType.php
    namespace Acme\TaskBundle\Form\Type;

    use Symfony\Component\Form\AbstractType;
    use Symfony\Component\Form\FormBuilderInterface;
    use Acme\TaskBundle\Form\DataTransformer\IssueToNumberTransformer;
    use Doctrine\Common\Persistence\ObjectManager;
    use Symfony\Component\OptionsResolver\OptionsResolverInterface;

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
            $builder->addModelTransformer($transformer);
        }

        public function setDefaultOptions(OptionsResolverInterface $resolver)
        {
            $resolver->setDefaults(array(
                'invalid_message' => 'La issue scelta non esiste',
            ));
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

Quindi, registrare il proprio tipo come servizio, con il tag ``form.type``, in modo che sia
riconosciuto come tipo di campo personalizzato:

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

    .. code-block:: php

        $container
            ->setDefinition('acme_demo.type.issue_selector', array(
                new Reference('doctrine.orm.entity_manager'),
            ))
            ->addTag('form.type', array(
                'alias' => 'issue_selector',
            ))
        ;

Ora, ogni volta che serve il tipo ``issue_selector``,
è molto facile::

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
                ->add('dueDate', null, array('widget' => 'single_text'))
                ->add('issue', 'issue_selector');
        }

        public function getName()
        {
            return 'task';
        }
    }
