.. index::
   single: Form

Form
=====

L'utilizzo dei form HTML è una delle attività più comuni e stimolanti per
uno sviluppatore web. Symfony integra un componente Form che permette di gestire
facilmente i form. Con l'aiuto di questo capitolo si potrà creare da zero un form complesso,
e imparare le caratteristiche più importanti della libreria dei form.

.. note::

   Il componente form di Symfony è una libreria autonoma che può essere usata al di fuori
   dei progetti Symfony. Per maggiori informazioni, vedere
   la :doc:`documentazione del componente Form </components/form/introduction>`
   su GitHub.

.. index::
   single: Form; Creazione di un form semplice

Creazione di un form semplice
-----------------------------

Supponiamo che si stia costruendo un semplice applicazione "elenco delle cose da fare" che dovrà
visualizzare le "attività". Poiché gli utenti avranno bisogno di modificare e creare attività, sarà
necessario costruire un form. Ma prima di iniziare, si andrà a vedere la generica
classe ``Task`` che rappresenta e memorizza i dati di una singola attività::

    // src/AppBundle/Entity/Task.php
    namespace AppBundle\Entity;

    class Task
    {
        protected $task;
        protected $dueDate;

        public function getTask()
        {
            return $this->task;
        }

        public function setTask($task)
        {
            $this->task = $task;
        }

        public function getDueDate()
        {
            return $this->dueDate;
        }

        public function setDueDate(\DateTime $dueDate = null)
        {
            $this->dueDate = $dueDate;
        }
    }

Questa classe è un "vecchio-semplice-oggetto-PHP", perché finora non ha nulla
a che fare con Symfony o qualsiasi altra libreria. È semplicemente un normale oggetto PHP,
che risolve un problema direttamente dentro la *propria* applicazione (cioè la necessità di
rappresentare un task nella propria applicazione). Naturalmente, alla fine di questo capitolo,
si sarà in grado di inviare dati all'istanza di un ``Task`` (tramite un form HTML), validare
i suoi dati e persisterli nella base dati.

.. index::
   single: Form; Creare un form in un controllore

Costruire il Form
~~~~~~~~~~~~~~~~~

Ora che la classe ``Task`` è stata creata, il prossimo passo è creare e
visualizzare il form HTML. In Symfony, lo si fa costruendo un oggetto
form e poi visualizzandolo in un template. Per ora, lo si può fare
all'interno di un controllore::

    // src/AppBundle/Controller/DefaultController.php
    namespace AppBundle\Controller;

    use AppBundle\Entity\Task;
    use Symfony\Bundle\FrameworkBundle\Controller\Controller;
    use Symfony\Component\HttpFoundation\Request;

    class DefaultController extends Controller
    {
        public function newAction(Request $request)
        {
            // crea un task fornendo alcuni dati fittizi per questo esempio
            $task = new Task();
            $task->setTask('Scrivere un post sul blog');
            $task->setDueDate(new \DateTime('tomorrow'));

            $form = $this->createFormBuilder($task)
                ->add('task', 'text')
                ->add('dueDate', 'date')
                ->add('save', 'submit', array('label' => 'Crea post'))
                ->getForm();

            return $this->render('default/new.html.twig', array(
                'form' => $form->createView(),
            ));
        }
    }

.. tip::

   Questo esempio mostra come costruire il form direttamente nel controllore.
   Più tardi, nella sezione ":ref:`book-form-creating-form-classes`", si imparerà
   come costruire il form in una classe autonoma, metodo consigliato perché in questo modo
   il form diventa riutilizzabile.

La creazione di un form richiede relativamente poco codice, perché gli oggetti form di Symfony
sono costruiti con un "costruttore di form". Lo scopo del costruttore di form è quello di consentire
di scrivere una semplice "ricetta" per il form e fargli fare tutto il lavoro pesante della
costruzione del form.

In questo esempio sono stati aggiunti due campi al form, ``task`` e ``dueDate``,
corrispondenti alle proprietà ``task`` e ``dueDate`` della classe ``Task``.
È stato anche assegnato un "tipo" ciascuno (ad esempio ``text``, ``date``), che, tra
le altre cose, determina quale tag form HTML viene utilizzato per tale campo.

Infine, è stato aggiunto un bottone submit, con un'etichetta personalizzata, per
l'invio del form.

.. versionadded:: 2.3
    Il supporto per i bottoni submit è stato aggiunto in Symfony 2.3. Precedentemente,
    era necessario aggiungere i bottoni manualmente nel codice HTML.

Symfony ha molti tipi predefiniti che verranno trattati a breve
(see :ref:`book-forms-type-reference`).

.. index::
  single: Form; Visualizzazione di base nel template

Visualizzare il Form
~~~~~~~~~~~~~~~~~~~~

Ora che il modulo è stato creato, il passo successivo è quello di visualizzarlo. Questo viene
fatto passando uno speciale oggetto form "view" al template (notare il
``$form->createView()`` nel controllore sopra) e utilizzando una serie di funzioni
aiutanti per i form:

.. configuration-block::

    .. code-block:: html+jinja

        {# app/Resources/views/default/new.html.twig #}
        {{ form_start(form) }}
        {{ form_widget(form) }}
        {{ form_end(form) }}

    .. code-block:: html+php

        <!-- app/Resources/views/default/new.html.php -->
        <?php echo $view['form']->start($form) ?>
        <?php echo $view['form']->widget($form) ?>
        <?php echo $view['form']->end($form) ?>

.. image:: /images/book/form-simple.png
    :align: center

.. note::

    Questo esempio presuppone che il form sia inviato in POST allo
    stesso URL in cui viene mostrato. Si vedrà successivamente come modificare
    il metodo della richiesta e l'URL di destinazione del form.

Questo è tutto! Bastano tre righe per rendere completamente il form:

``form_start(form)``
    Rende il tag iniziale del form, incluso l'attributo
    ``enctype``, se si usa un caricamento di file;

``form_widget(form)``
    Rende tutti i campi, inclusi l'elemento stesso,
    un'etichetta ed eventuali messaggi di errori;

``form_end(form)``
    Rende il tag finale del form e ogni campo che non sia ancora
    stato reso, nel caso in cui i campi siano stati resti singolarmante a mano. È utile
    per rendere campi nascosti e sfruttare la
    :ref:`protezione CSRF <forms-csrf>` automatica.

.. seealso::

    Pur essendo facile, non è (ancora) flessibile. Di solito, si vorranno
    rendere i singoli campi, in modo da poter controllare l'aspetto del form.
    Si vedrà come fare nella sezione ":ref:`form-rendering-template`".

Prima di andare avanti, notare come il campo input ``task`` reso ha il value
della proprietà ``task`` dall'oggetto ``$task`` (ad esempio "Scrivere un post sul blog").
Questo è il primo compito di un form: prendere i dati da un oggetto e tradurli
in un formato adatto a essere reso in un form HTML.

.. tip::

   Il sistema dei form è abbastanza intelligente da accedere al valore della proprietà
   protetta ``task`` attraverso i metodi ``getTask()`` e ``setTask()`` della
   classe ``Task``. A meno che una proprietà non sia privata, *deve* avere un metodo "getter" e uno
   "setter", in modo che il componente form possa ottenere e mettere dati nella
   proprietà. Per una proprietà booleana, è possibile utilizzare un metodo "isser" o "hasser"
   (per esempio ``isPublished()`` o ``hasReminder``) invece di un getter (per esempio
   ``getPublished()`` o ``getReminder()``).

.. index::
  single: Form; Gestione dell'invio del form

.. _book-form-handling-form-submissions:

Gestione dell'invio del form
~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Il secondo compito di un form è quello di tradurre i dati inviati dall'utente alle
proprietà di un oggetto. Affinché ciò avvenga, i dati inviati
dall'utente devono essere associati al form. Aggiungere le seguenti funzionalità al
controllore::

    // ...
    use Symfony\Component\HttpFoundation\Request;

    public function newAction(Request $request)
    {
        // crea un nuovo oggetto $task (rimuove i dati fittizi)
        $task = new Task();

        $form = $this->createFormBuilder($task)
            ->add('task', 'text')
            ->add('dueDate', 'date')
            ->add('save', 'submit', array('label' => 'Crea post'))
            ->getForm();

        $form->handleRequest($request);

        if ($form->isValid()) {
            // esegue alcune azioni, come ad esempio salvare il task nella base dati

            return $this->redirect($this->generateUrl('task_success'));
        }

        // ...
    }

.. versionadded:: 2.3
    Il metodo :method:`Symfony\\Component\\Form\\FormInterface::handleRequest` è stato
    aggiunto in Symfony 2.3. In precedenza, veniva passata ``$request`` al
    metodo ``submit``, una strategia deprecata, che sarà rimossa
    in Symfony 3.0. Per dettagli sul metodo, vedere :ref:`cookbook-form-submit-request`.

Questo controllore segue uno schema comune per gestire i form e ha tre
possibili percorsi:

#. Quando in un browser inizia il caricamento di una pagina, il form viene creato
   e reso. :method:`Symfony\\Component\\Form\\FormInterface::handleRequest`
   capisce che il form non è stato inviato e non fa nulla.
   :method:`Symfony\\Component\\Form\\FormInterface::isValid` restituisce ``false``
   se il form non è stato inviato.

#. Quando l'utente invia il form, :method:`Symfony\\Component\\Form\\FormInterface::handleRequest`
   lo capisce e scrive immediatamente i dati nelle proprietà
   ``task`` e ``dueDate`` dell'oggetto ``$task``. Quindi tale oggetto
   viene validato. Se non è valido (la validazione è trattata nella prossima sezione),
   :method:`Symfony\\Component\\Form\\FormInterface::isValid` restituisce ``false``
   di nuovo, quindi il form viene reso insieme agli errori di validazione;

   .. note::

       Si può usare il metodo :method:`Symfony\\Component\\Form\\FormInterface::isSubmitted`
       per verificare se il form sia stato inviato, indipendentemente dal fatto
       che i dati inviati siano validi o meno.

#. Quando l'utente invia il form con dati validi, i dati inviati sono scritti
   nuovamente nel form, ma stavolta :method:`Symfony\\Component\\Form\\FormInterface::isValid`
   restituisce ``true``. Ora si ha la possibilità di eseguire alcune azioni usando l'oggetto
   ``$task`` (ad esempio persistendolo nella base dati) prima di rinviare l'utente
   a un'altra pagina (ad esempio una pagina "thank you" o "success").

.. note::

   Reindirizzare un utente dopo aver inviato con successo un form impedisce l'utente
   di essere in grado di premere il tasto "aggiorna" del suo browser e reinviare
   i dati.

.. seealso::

    Se occorre maggior controllo su quando esattamente il form è inviato o su quali dati
    siano passati, si può usare il metodo :method:`Symfony\\Component\\Form\\FormInterface::submit`.
    Si può approfondire :ref:`nel ricettario <cookbook-form-call-submit-directly>`.

.. index::
   single: Form; Bottoni di submit multipli

.. _book-form-submitting-multiple-buttons:

Inviare form con bottoni di submit multipli
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

.. versionadded:: 2.3
    Il supporto per i bottoni nei form è stato aggiunto in Symfony 2.3.

Quando un form contiene più di un bottone di submit, si vuole sapere
quale dei bottoni sia stato cliccato, per adattare il flusso del controllore.
Aggiungiamo un secondo bottone "Salva e aggiungi" al form::

    $form = $this->createFormBuilder($task)
        ->add('task', 'text')
        ->add('dueDate', 'date')
        ->add('save', 'submit', array('label' => 'Crea post'))
        ->add('saveAndAdd', 'submit', array('label' => 'Salva e aggiungi'))
        ->getForm();

Nel controllore, usare il metodo
:method:`Symfony\\Component\\Form\\ClickableInterface::isClicked` del bottone
per sapere se sia stato cliccato il bottone "Salva e aggiungi"::

    if ($form->isValid()) {
        // ... eseguire un'azione, come salvare il task nella base dati

        $nextAction = $form->get('saveAndAdd')->isClicked()
            ? 'task_new'
            : 'task_success';

        return $this->redirect($this->generateUrl($nextAction));
    }

.. index::
   single: Form; Validazione

.. _book-forms-form-validation:

Validare un form
----------------

Nella sezione precedente, si è appreso come un form può essere inviato con dati
validi o invalidi. In Symfony, la validazione viene applicata all'oggetto sottostante
(per esempio ``Task``). In altre parole, la questione non è se il "form" è
valido, ma se l'oggetto ``$task`` è valido o meno dopo che al form sono stati
applicati i dati inviati. La chiamata di ``$form->isValid()`` è una scorciatoia
che chiede all'oggetto ``$task`` se ha dati validi o meno.

La validazione è fatta aggiungendo di una serie di regole (chiamate vincoli) a una classe. Per
vederla in azione, verranno aggiunti vincoli di validazione in modo che il campo ``task`` non possa
essere vuoto e il campo ``dueDate`` non possa essere vuoto e debba essere un oggetto \DateTime
valido.

.. configuration-block::

    .. code-block:: php-annotations

        // AppBundle/Entity/Task.php
        use Symfony\Component\Validator\Constraints as Assert;

        class Task
        {
            /**
             * @Assert\NotBlank()
             */
            public $task;

            /**
             * @Assert\NotBlank()
             * @Assert\Type("\DateTime")
             */
            protected $dueDate;
        }

    .. code-block:: yaml

        # AppBundle/Resources/config/validation.yml
        AppBundle\Entity\Task:
            properties:
                task:
                    - NotBlank: ~
                dueDate:
                    - NotBlank: ~
                    - Type: \DateTime

    .. code-block:: xml

        <!-- AppBundle/Resources/config/validation.xml -->
        <?xml version="1.0" encoding="UTF-8"?>
        <constraint-mapping xmlns="http://symfony.com/schema/dic/constraint-mapping"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://symfony.com/schema/dic/constraint-mapping
                http://symfony.com/schema/dic/constraint-mapping/constraint-mapping-1.0.xsd">

            <class name="AppBundle\Entity\Task">
                <property name="task">
                    <constraint name="NotBlank" />
                </property>
                <property name="dueDate">
                    <constraint name="NotBlank" />
                    <constraint name="Type">\DateTime</constraint>
                </property>
            </class>
        </constraint-mapping>

    .. code-block:: php

        // AppBundle/Entity/Task.php
        use Symfony\Component\Validator\Mapping\ClassMetadata;
        use Symfony\Component\Validator\Constraints\NotBlank;
        use Symfony\Component\Validator\Constraints\Type;

        class Task
        {
            // ...

            public static function loadValidatorMetadata(ClassMetadata $metadata)
            {
                $metadata->addPropertyConstraint('task', new NotBlank());

                $metadata->addPropertyConstraint('dueDate', new NotBlank());
                $metadata->addPropertyConstraint(
                    'dueDate',
                    new Type('\DateTime')
                );
            }
        }

Questo è tutto! Se si re-invia il form con i dati non validi, si vedranno i
rispettivi errori visualizzati nel form.

.. _book-forms-html5-validation-disable:

.. sidebar:: Validazione HTML5

   Dall'HTML5, molti browser possono nativamente imporre alcuni vincoli di validazione
   sul lato client. La validazione più comune è attivata con la resa
   di un attributo ``required`` sui campi che sono obbligatori. Per i browser che
   supportano HTML5, questo si tradurrà in un messaggio nativo del browser che verrà visualizzato
   se l'utente tenta di inviare il form con quel campo vuoto.

   I form generati traggono il massimo vantaggio di questa nuova funzionalità con l'aggiunta di appropriati
   attributi HTML che verifichino la convalida. La convalida lato client,
   tuttavia, può essere disabilitata aggiungendo l'attributo ``novalidate`` al
   tag ``form`` o ``formnovalidate`` al tag submit. Ciò è particolarmente
   utile quando si desidera testare i propri vincoli di convalida lato server,
   ma viene impedito dal browser, per esempio, inviando
   campi vuoti.

   .. configuration-block::

       .. code-block:: html+jinja

           {# app/Resources/views/default/new.html.twig #}
           {{ form(form, {'attr': {'novalidate': 'novalidate'}}) }}

       .. code-block:: html+php

           <!-- app/Resources/views/default/new.html.php -->
           <?php echo $view['form']->form($form, array(
               'attr' => array('novalidate' => 'novalidate'),
           )) ?>

La validazione è una caratteristica molto potente di Symfony e dispone di un proprio
:doc:`capitolo dedicato </book/validation>`.

.. index::
   single: Form; Gruppi di validatori

.. _book-forms-validation-groups:

Gruppi di validatori
~~~~~~~~~~~~~~~~~~~~

Se un oggetto si avvale dei :ref:`gruppi di validatori <book-validation-validation-groups>`,
occorrerà specificare quali gruppi di convalida deve usare il form::

    $form = $this->createFormBuilder($users, array(
        'validation_groups' => array('registrazione'),
    ))->add(...);

Se si stanno creando :ref:`classi per i form<book-form-creating-form-classes>` (una
buona pratica), allora si avrà bisogno di aggiungere quanto segue al metodo
``setDefaultOptions()``::

    use Symfony\Component\OptionsResolver\OptionsResolverInterface;

    public function setDefaultOptions(OptionsResolverInterface $resolver)
    {
        $resolver->setDefaults(array(
            'validation_groups' => array('registrazione'),
        ));
    }

In entrambi i casi, *solo* il gruppo di validazione ``registrazione`` verrà
utilizzato per validare l'oggetto sottostante.

.. index::
   single: Form; Disabilitare la validazione

Disabilitare la validazione
~~~~~~~~~~~~~~~~~~~~~~~~~~~

.. versionadded:: 2.3
    La possibilità di impostare ``validation_groups`` a ``false`` è stata aggiunta in Symfony 2.3.

A volte è utile sopprimere la validazione per un intero form. Per questi
casi, si può impostare l'opzione ``validation_groups`` a ``false``::

    use Symfony\Component\OptionsResolver\OptionsResolverInterface;

    public function setDefaultOptions(OptionsResolverInterface $resolver)
    {
        $resolver->setDefaults(array(
            'validation_groups' => false,
        ));
    }

Notare che in questo caso il form eseguirà comunque alcune verifiche basilari di integrità,
per esempio se un file caricato è troppo grande o se dei campi non esistenti
sono stati inviati. Se si vuole sopprimere completamente la validazione, si può usare
:ref:`l'evento POST_SUBMIT <cookbook-dynamic-form-modification-suppressing-form-validation>`.

.. index::
   single: Form; Gruppi di validazione basati su dati inseriti

.. _book-form-validation-groups:

Gruppi basati su dati inseriti
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Se si ha bisogno di una logica avanzata per determinare i gruppi di validazione (p.e.
basandosi sui dati inseriti), si può impostare l'opzione ``validation_groups`` a
un callback o a una ``Closure``::

    use Symfony\Component\OptionsResolver\OptionsResolverInterface;

    // ...
    public function setDefaultOptions(OptionsResolverInterface $resolver)
    {
        $resolver->setDefaults(array(
            'validation_groups' => array(
                'AppBundle\Entity\Client',
                'determineValidationGroups',
            ),
        ));
    }

Questo richiamerà il metodo statico ``determineValidationGroups()`` della classe
``Client``, dopo il bind del form ma prima dell'esecuzione della validazione.
L'oggetto Form è passato come parametro del metodo (vedere l'esempio successivo).
Si può anche definire l'intera logica con una Closure::

    use AppBundle\Entity\Client;
    use Symfony\Component\Form\FormInterface;
    use Symfony\Component\OptionsResolver\OptionsResolverInterface;

    // ...
    public function setDefaultOptions(OptionsResolverInterface $resolver)
    {
        $resolver->setDefaults(array(
            'validation_groups' => function (FormInterface $form) {
                $data = $form->getData();

                if (Client::TYPE_PERSON == $data->getType()) {
                    return array('person');
                }

                return array('company');
            },
        ));
    }

L'uso dell'opzione ``validation_groups`` sovrascrive il gruppo predefinito di validazione
in uso. Se si vogliono validare anche i vincoli predefiniti
dell'entità, occorre modificare l'opzione in questo modo::

    use AppBundle\Entity\Client;
    use Symfony\Component\Form\FormInterface;
    use Symfony\Component\OptionsResolver\OptionsResolverInterface;

    // ...
    public function setDefaultOptions(OptionsResolverInterface $resolver)
    {
        $resolver->setDefaults(array(
            'validation_groups' => function (FormInterface $form) {
                $data = $form->getData();

                if (Client::TYPE_PERSON == $data->getType()) {
                    return array('Default', 'person');
                }

                return array('Default', 'company');
            },
        ));
    }

Si possono trovare maggiori informazioni sui gruppi di validazione e sui vincoli predefiniti
nella sezione del libro sui :ref:`gruppi di validazione <book-validation-validation-groups>`.

.. index::
   single: Form; Gruppi di validazione basati sul bottone cliccato

Gruppi basati sul bottone cliccato
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

.. versionadded:: 2.3
    Il supporto per i bottoni nei form è stato aggiunto in Symfony 2.3.

Se un form contiene più bottoni submit, si può modificare il gruppo di validazione,
a seconda di quale bottone sia stato usato per inviare il form. Per esempi,
consideriamo un form in sequenza, in cui si può avanzare al passo successivo o tornare
al passo precedente. Ipotizziamo anche che, quando si torna al passo precedente,
i dati del form debbano essere salvati, ma non validati.

Prima di tutto, bisogna aggiungere i due bottoni al form::

    $form = $this->createFormBuilder($task)
        // ...
        ->add('nextStep', 'submit')
        ->add('previousStep', 'submit')
        ->getForm();

Quindi, occorre configurare il bottone che torna al passo precedente per eseguire
specifici gruppi di validazione. In questo esempio, vogliamo sopprimere la validazione,
quindi impostiamo l'opzione ``validation_groups`` a ``false``::

    $form = $this->createFormBuilder($task)
        // ...
        ->add('previousStep', 'submit', array(
            'validation_groups' => false,
        ))
        ->getForm();

Ora il form salterà i controlli di validazione. Validerà comunque i vincoli basilari
di integrità, come il controllo se un file caricato sia troppo grande
o se si sia tentato di inserire del testo in un campo numerico.

.. index::
   single: Form; Tipi di campo predefiniti

.. _book-forms-type-reference:

Tipi di campo predefiniti
-------------------------

Symfony dispone di un folto gruppo di tipi di campi che coprono tutti i
campi più comuni e i tipi di dati di cui necessitano i form:

.. include:: /reference/forms/types/map.rst.inc

È anche possibile creare dei tipi di campi personalizzati. Questo argomento è trattato
nell'articolo ":doc:`/cookbook/form/create_custom_field_type`" del ricettario.

.. index::
   single: Form; Opzioni dei tipi di campo

Opzioni dei tipi di campo
~~~~~~~~~~~~~~~~~~~~~~~~~

Ogni tipo di campo ha un numero di opzioni che può essere utilizzato per la configurazione.
Ad esempio, il campo ``dueDate`` è attualmente reso con 3 menu
select. Tuttavia, il :doc:`campo data</reference/forms/types/date>` può essere
configurato per essere reso come una singola casella di testo (in cui l'utente deve inserire
la data nella casella come una stringa)::

    ->add('dueDate', 'date', array('widget' => 'single_text'))

.. image:: /images/book/form-simple2.png
    :align: center

Ogni tipo di campo ha un numero di opzioni differente che possono essere passate a esso.
Molte di queste sono specifiche per il tipo di campo e i dettagli possono essere trovati
nella documentazione di ciascun tipo.

.. sidebar:: L'opzione ``required``

    L'opzione più comune è l'opzione ``required``, che può essere applicata a
    qualsiasi campo. Per impostazione predefinita, l'opzione ``required`` è impostata a ``true`` e questo significa
    che i browser che interpretano l'HTML5 applicheranno la validazione lato client se il campo
    viene lasciato vuoto. Se non si desidera questo comportamento, impostare l'opzione ``required``
    del campo a ``false`` oppure
    :ref:`disabilitare la validazione HTML5 <book-forms-html5-validation-disable>`.

    Si noti inoltre che l'impostazione dell'opzione ``required`` a ``true`` **non**
    farà applicare la validazione lato server. In altre parole, se un
    utente invia un valore vuoto per il campo (sia con un browser vecchio
    o un servizio web, per esempio), sarà accettata come valore valido a meno 
    che si utilizzi il vincolo di validazione ``NotBlank`` o ``NotNull``.

    In altre parole, l'opzione ``required`` è "bella", ma la vera validazione lato server
    dovrebbe *sempre* essere utilizzata.

.. sidebar:: L'opzione ``label``

    La label per il campo del form può essere impostata con l'opzione ``label``,
    applicabile a qualsiasi campo::

        ->add('dueDate', 'date', array(
            'widget' => 'single_text',
            'label'  => 'Due Date',
        ))

    La label per un campo può anche essere impostata nel template che rende il
    form, vedere sotto. Se non occorre alcuna label, la si
    può disabilitare impostandone il valore a ``false``.

.. index::
   single: Form; Indovinare il tipo di campo

.. _book-forms-field-guessing:

Indovinare il tipo di campo
---------------------------

Ora che sono stati aggiunti i metadati di validazione alla classe ``Task``, Symfony
sa già un po' dei campi. Se lo si vuole permettere, Symfony può "indovinare"
il tipo del campo e impostarlo al posto vostro. In questo esempio, Symfony può
indovinare dalle regole di validazione che il campo ``task`` è un normale
campo ``text`` e che il campo ``dueDate`` è un campo ``date``::

    public function newAction()
    {
        $task = new Task();

        $form = $this->createFormBuilder($task)
            ->add('task')
            ->add('dueDate', null, array('widget' => 'single_text'))
            ->add('save', 'submit')
            ->getForm();
    }

Questa funzionalità si attiva quando si omette il secondo parametro del metodo
``add()`` (o se si passa ``null`` a esso). Se si passa un array di opzioni come
terzo parametro (fatto sopra per ``dueDate``), queste opzioni vengono applicate
al campo indovinato.

.. caution::

    Se il form utilizza un gruppo specifico di validazione, la funzionalità che indovina il tipo di campo
    prenderà ancora in considerazione *tutti* i vincoli di validazione quando andrà a indovinare i
    tipi di campi (compresi i vincoli che non fanno parte del processo di convalida
    dei gruppi in uso).

.. index::
   single: Form; Indovinare il tipo di campo

Indovinare le opzioni dei tipi di campo
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Oltre a indovinare il "tipo" di un campo, Symfony può anche provare a indovinare
i valori corretti di una serie di opzioni del campo.

.. tip::

    Quando queste opzioni vengono impostate, il campo sarà reso con speciali attributi
    HTML che forniscono la validazione HTML5 lato client. Tuttavia,
    non genera i vincoli equivalenti lato server (ad esempio ``Assert\MaxLength``).
    E anche se si ha bisogno di aggiungere manualmente la validazione lato server, queste
    opzioni dei tipi di campo possono essere ricavate da queste informazioni.

``required``
    L'opzione ``required`` può essere indovinata in base alle regole
    di validazione (cioè se il campo è ``NotBlank`` o ``NotNull``) o dai metadati di Doctrine
    (vale a dire se il campo è ``nullable``). Questo è molto utile, perché la validazione
    lato client corrisponderà automaticamente alle vostre regole di validazione.   

``max_length``
    Se il campo è un qualche tipo di campo di testo, allora l'opzione ``max_length``
    può essere indovinata dai vincoli di validazione (se viene utilizzato ``Length`` o ``Range``)
    o dai metadati Doctrine (tramite la lunghezza del campo).

.. note::

  Queste opzioni di campi vengono indovinate *solo* se si sta usando Symfony per ricavare
  il tipo di campo (ovvero omettendo o passando ``null`` nel secondo parametro di ``add()``).

Se si desidera modificare uno dei valori indovinati, è possibile sovrascriverlo
passando l'opzione nell'array di opzioni del campo::

    ->add('task', null, array('max_length' => 4))

.. index::
   single: Form; Rendere un form in un template

.. _form-rendering-template:

Rendere un form in un template
------------------------------

Finora si è visto come un intero form può essere reso con una sola linea
di codice. Naturalmente, solitamente si ha bisogno di molta più flessibilità:

.. configuration-block::

    .. code-block:: html+jinja

        {# app/Resources/views/default/new.html.twig #}
        {{ form_start(form) }}
            {{ form_errors(form) }}

            {{ form_row(form.task) }}
            {{ form_row(form.dueDate) }}
        {{ form_end(form) }}

    .. code-block:: html+php

        <!-- app/Resources/views/default/newAction.html.php -->
        <?php echo $view['form']->start($form) ?>
            <?php echo $view['form']->errors($form) ?>

            <?php echo $view['form']->row($form['task']) ?>
            <?php echo $view['form']->row($form['dueDate']) ?>
        <?php echo $view['form']->end($form) ?>

Abbiamo già visto le funzioni ``form_start()`` e ``form_end()``, ma cosa fanno
le altre funzioni?

``form_errors(form)``
    Rende eventuali errori globali per l'intero modulo
    (gli errori specifici dei campi vengono visualizzati accanto a ciascun campo);

``form_row(form.dueDate)``
    Rende l'etichetta, eventuali errori e il widget HTML del form per il dato
    campo (p.e. ``dueDate``) all'interno, per impostazione predefinita, di un elemento ``div``;

La maggior parte del lavoro viene fatto dall'helper ``form_row``, che rende
l'etichetta, gli errori e i widget HTML del form di ogni campo all'interno di un tag ``div``
per impostazione predefinita. Nella sezione :ref:`form-theming`, si apprenderà come l'output
di ``form_row`` possa essere personalizzato su diversi livelli.

.. tip::

    Si può accedere ai dati attuali del form tramite ``form.vars.value``:

    .. configuration-block::

        .. code-block:: jinja

            {{ form.vars.value.task }}

        .. code-block:: html+php

            <?php echo $form->vars['value']->getTask() ?>

.. index::
   single: Form; Rendere manualmente ciascun campo

Rendere manualmente ciascun campo
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

L'aiutante ``form_row`` è utile, perché si può rendere ciascun campo del form
molto facilmente (e il markup utilizzato per la "riga" può essere personalizzato
a piacere). Ma poiché la vita non è sempre così semplice, è anche possibile rendere ogni campo
interamente a mano. Il risultato finale del codice che segue è lo stesso di quando
si è utilizzato l'aiutante ``form_row``:

.. configuration-block::

    .. code-block:: html+jinja

        {{ form_start(form) }}
            {{ form_errors(form) }}

            <div>
                {{ form_label(form.task) }}
                {{ form_errors(form.task) }}
                {{ form_widget(form.task) }}
            </div>

            <div>
                {{ form_label(form.dueDate) }}
                {{ form_errors(form.dueDate) }}
                {{ form_widget(form.dueDate) }}
            </div>

            <div>
                {{ form_widget(form.save) }}
            </div>

        {{ form_end(form) }}

    .. code-block:: html+php

        <?php echo $view['form']->start($form) ?>

            <?php echo $view['form']->errors($form) ?>

            <div>
                <?php echo $view['form']->label($form['task']) ?>
                <?php echo $view['form']->errors($form['task']) ?>
                <?php echo $view['form']->widget($form['task']) ?>
            </div>

            <div>
                <?php echo $view['form']->label($form['dueDate']) ?>
                <?php echo $view['form']->errors($form['dueDate']) ?>
                <?php echo $view['form']->widget($form['dueDate']) ?>
            </div>

            <div>
                <?php echo $view['form']->widget($form['save']) ?>
            </div>

        <?php echo $view['form']->end($form) ?>

Se la label auto-generata di un campo non è giusta, si può specificarla
esplicitamente:

.. configuration-block::

    .. code-block:: html+jinja

        {{ form_label(form.task, 'Task Description') }}

    .. code-block:: html+php

        <?php echo $view['form']->label($form['task'], 'Task Description') ?>

Alcuni tipi di campi hanno opzioni di resa aggiuntive che possono essere passate
al widget. Queste opzioni sono documentate con ogni tipo, ma un'opzione
comune è ``attr``, che permette di modificare gli attributi dell'elemento form.
Di seguito viene aggiunta la classe ``task_field`` al resa del campo
casella di testo:

.. configuration-block::

    .. code-block:: html+jinja

        {{ form_widget(form.task, {'attr': {'class': 'task_field'}}) }}

    .. code-block:: html+php

        <?php echo $view['form']->widget($form['task'], array(
            'attr' => array('class' => 'task_field'),
        )) ?>

Se occorre rendere dei campi "a mano", si può accedere ai singoli valori dei campi,
come ``id``, ``name`` e ``label``. Per esempio, per ottenere
``id``:

.. configuration-block::

    .. code-block:: html+jinja

        {{ form.task.vars.id }}

    .. code-block:: html+php

        <?php echo $form['task']->vars['id']?>

Per ottenere il valore usato per l'attributo nome dei campi del form, occorre usare
il valore ``full_name``:

.. configuration-block::

    .. code-block:: html+jinja

        {{ form.task.vars.full_name }}

    .. code-block:: html+php

        <?php echo $form['task']->vars['full_name'] ?>

Riferimento alle funzioni del template Twig
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Se si utilizza Twig, un riferimento completo alle funzioni di resa è
disponibile nel :doc:`manuale di riferimento</reference/forms/twig_reference>`.
Leggendolo si può sapere tutto sugli helper disponibili e le opzioni
che possono essere usate con ciascuno di essi.

.. index::
   single: Form; Cambiare azione e metodo

.. _book-forms-changing-action-and-method:

Cambiare azione e metodo di un form
-----------------------------------

Finora, è stato usato l'helper ``form_start()`` per rendere il tag di aperture del form,
ipotizzando che ogni form sia inviato allo stesso URL in POST.
A volte si vogliono cambiare questi parametri. Lo si può fare in modi diversi.
Se si costruisce il form nel controllore, si può usare ``setAction()`` e
``setMethod()``::

    $form = $this->createFormBuilder($task)
        ->setAction($this->generateUrl('target_route'))
        ->setMethod('GET')
        ->add('task', 'text')
        ->add('dueDate', 'date')
        ->add('save', 'submit')
        ->getForm();

.. note::

    Questo esempio ipotizza la presenza di una rotta ``target_route``,
    che punti al controllore che processerà il form.

In :ref:`book-form-creating-form-classes`, vedremo come spostare il codice di costruzione
del form in una classe separata. Quando si usa una classe form esterna
nel controllore, si possono passare azione e metodo come opzioni::

    $form = $this->createForm(new TaskType(), $task, array(
        'action' => $this->generateUrl('target_route'),
        'method' => 'GET',
    ));

Infine, si possono sovrascrivere azione e metodo nel template, passandoli all'aiutante
``form()`` o ``form_start()``:

.. configuration-block::

    .. code-block:: html+jinja

        {# app/Resources/views/default/new.html.twig #}
        {{ form_start(form, {'action': path('target_route'), 'method': 'GET'}) }}

    .. code-block:: html+php

        <!-- app/Resources/views/default/newAction.html.php -->
        <?php echo $view['form']->start($form, array(
            'action' => $view['router']->generate('target_route'),
            'method' => 'GET',
        )) ?>

.. note::

    Se il metodo del form non è GET o POST, ma PUT, PATCH o DELETE, Symfony
    inserirà un campo nascosto chiamato "_method", per memorizzare il metodo.
    Il form sarà inviato in POST, ma il router di Symfony's è in
    grado di rilevare il parametro "_method" e interpretare la richiesta 
    come PUT, PATCH o DELETE. Si veda la ricetta
    ":doc:`/cookbook/routing/method_parameters`" per maggiori informazioni.

.. index::
   single: Form; Creare classi form

.. _book-form-creating-form-classes:

Creare classi per i form
------------------------

Come si è visto, un form può essere creato e utilizzato direttamente in un controllore.
Tuttavia, una pratica migliore è quella di costruire il form in una apposita classe
PHP, che può essere riutilizzata in qualsiasi punto dell'applicazione. Creare una nuova classe
che ospiterà la logica per la costruzione del form task::

    // src/AppBundle/Form/Type/TaskType.php
    namespace AppBundle\Form\Type;

    use Symfony\Component\Form\AbstractType;
    use Symfony\Component\Form\FormBuilderInterface;

    class TaskType extends AbstractType
    {
        public function buildForm(FormBuilderInterface $builder, array $options)
        {
            $builder
                ->add('task')
                ->add('dueDate', null, array('widget' => 'single_text'))
                ->add('save', 'submit')
            ;
        }

        public function getName()
        {
            return 'task';
        }
    }

.. caution::

    Il metodo ``getName()`` restituisce l'identificatore per questo "tipo" di form. Questi
    identificatori devono essere univoci nell'applicazione. A meno che non si voglia sovrascrivere
    un tipo predefinito, devono essere diversi dai tipi di Symfony e da ogni
    tipo definito da bundle di terze parti installati nell'applicazione.
    Si consideri l'uso di un prefisso ``app_``, per evitare collisioni.

Questa nuova classe contiene tutte le indicazioni necessarie per creare il form
task. Può essere usato per costruire rapidamente un oggetto form nel controllore::

    // src/AppBundle/Controller/DefaultController.php

    // add this new use statement at the top of the class
    use AppBundle\Form\Type\TaskType;

    public function newAction()
    {
        $task = ...;
        $form = $this->createForm(new TaskType(), $task);

        // ...
    }

Porre la logica del form in una classe a parte significa che il form può essere facilmente
riutilizzato in altre parti del progetto. Questo è il modo migliore per creare form, ma
la scelta in ultima analisi, spetta allo sviluppatore.

.. _book-forms-data-class:

.. sidebar:: Impostare ``data_class``

    Ogni form ha bisogno di sapere il nome della classe che detiene i dati
    sottostanti (ad esempio ``AppBundle\Entity\Task``). Di solito, questo viene indovinato
    in base all'oggetto passato al secondo parametro di ``createForm``
    (vale a dire ``$task``). Dopo, quando si inizia a incorporare i form, questo
    non sarà più sufficiente. Così, anche se non sempre necessario, è in genere una
    buona idea specificare esplicitamente l'opzione ``data_class`` aggiungendo
    il codice seguente alla classe del tipo di form::

        use Symfony\Component\OptionsResolver\OptionsResolverInterface;

        public function setDefaultOptions(OptionsResolverInterface $resolver)
        {
            $resolver->setDefaults(array(
                'data_class' => 'AppBundle\Entity\Task',
            ));
        }

.. tip::

    Quando si mappano form su oggetti, tutti i campi vengono mappati. Ogni campo nel
    form che non esiste nell'oggetto mappato causerà il lancio di
    un'eccezione.

    Nel caso in cui servano campi extra nel form (per esempio, un checkbox "accetto
    i termini"), che non saranno mappati nell'oggetto sottostante,
    occorre impostare l'opzione ``mapped`` a ``false``::

        use Symfony\Component\Form\FormBuilderInterface;

        public function buildForm(FormBuilderInterface $builder, array $options)
        {
            $builder
                ->add('task')
                ->add('dueDate', null, array('mapped' => false))
                ->add('save', 'submit')
            ;
        }

    Inoltre, se ci sono campi nel form che non sono inclusi nei dati inviati,
    tali campi saranno impostati esplicitamente a ``null``.

    Si può accedere ai dati del campo in un controllore con::

        $form->get('dueDate')->getData();

    Inoltre, anche i dati di un campo non mappato si possono modificare direttamente::

        $form->get('dueDate')->setData(new \DateTime());

.. _form-as-services:

Definire i form come servizi
~~~~~~~~~~~~~~~~~~~~~~~~~~~~

La definizione dei form type come servizi è una buona pratica e li rende riusabili 
facilmente in un'applicazione.

.. note::

    I servizi e il contenitore di servizi saranno trattati
    :doc:`più avanti nel libro </book/service_container>`. Le cose saranno
    più chiaro dopo aver letto quel capitolo.

.. configuration-block::

    .. code-block:: yaml

        # src/AppBundle/Resources/config/services.yml
        services:
            app.form.type.task:
                class: AppBundle\Form\Type\TaskType
                tags:
                    - { name: form.type, alias: task }

    .. code-block:: xml

        <!-- src/AppBundle/Resources/config/services.xml -->
        <?xml version="1.0" encoding="UTF-8" ?>
        <container xmlns="http://symfony.com/schema/dic/services"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://symfony.com/schema/dic/services http://symfony.com/schema/dic/services/services-1.0.xsd">

            <services>
                <service id="app.form.type.task" class="AppBundle\Form\Type\TaskType">
                    <tag name="form.type" alias="task" />
                </service>
            </services>
        </container>

    .. code-block:: php

        // src/AppBundle/Resources/config/services.php
        $container
            ->register(
                'app.form.type.task',
                'AppBundle\Form\Type\TaskType'
            )
            ->addTag('form.type', array(
                'alias' => 'task',
            ))
        ;

Ecco fatto! Ora si può usare il form type direttamente in un controllore::

    // src/AppBundle/Controller/DefaultController.php
    // ...

    public function newAction()
    {
        $task = ...;
        $form = $this->createForm('task', $task);

        // ...
    }

o anche usarlo in un altro form::

    // src/AppBundle/Form/Type/ListType.php
    // ...

    class ListType extends AbstractType
    {
        public function buildForm(FormBuilderInterface $builder, array $options)
        {
            // ...

            $builder->add('someTask', 'task');
        }
    }

Si veda :ref:`form-cookbook-form-field-service` per maggiori informazioni.

.. index::
   pair: Form; Doctrine

I form e Doctrine
-----------------

L'obiettivo di un form è quello di tradurre i dati da un oggetto (ad esempio ``Task``) a un
form HTML e quindi tradurre i dati inviati dall'utente indietro all'oggetto originale. Come
tale, il tema della persistenza dell'oggetto ``Task`` nella base dati è interamente
non correlato al tema dei form. Ma, se la classe ``Task`` è stata configurata
per essere salvata attraverso Doctrine (vale a dire che per farlo si è aggiunta la
:ref:`mappatura dei metadati <book-doctrine-adding-mapping>`), allora si può salvare 
dopo l'invio di un form, quando il form stesso è valido::

    if ($form->isValid()) {
        $em = $this->getDoctrine()->getManager();
        $em->persist($task);
        $em->flush();

        return $this->redirect($this->generateUrl('task_success'));
    }

Se, per qualche motivo, non si ha accesso all'oggetto originale ``$task``,
è possibile recuperarlo dal form::

    $task = $form->getData();

Per maggiori informazioni, vedere il :doc:`capitolo ORM Doctrine</book/doctrine>`.

La cosa fondamentale da capire è che quando il form viene riempito, i dati
inviati vengono trasferiti immediatamente all'oggetto sottostante. Se si vuole
persistere i dati, è sufficiente persistere l'oggetto stesso (che già
contiene i dati inviati).

.. index::
   single: Form; Incorporare form

Incorporare form
----------------

Spesso, si vuole costruire form che includono campi provenienti da oggetti
diversi. Ad esempio, un form di registrazione può contenere dati appartenenti
a un oggetto ``User`` così come a molti oggetti ``Address``. Fortunatamente, questo
è semplice e naturale con il componente per i form.

.. _forms-embedding-single-object:

Incorporare un oggetto singolo
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Supponiamo che ogni ``Task`` appartenga a un semplice oggetto ``Category``. Si parte,
naturalmente, con la creazione di un oggetto ``Category``::

    // src/AppBundle/Entity/Category.php
    namespace AppBundle\Entity;

    use Symfony\Component\Validator\Constraints as Assert;

    class Category
    {
        /**
         * @Assert\NotBlank()
         */
        public $name;
    }

Poi, aggiungere una nuova proprietà ``category`` alla classe ``Task``::

    // ...

    class Task
    {
        // ...

        /**
         * @Assert\Type(type="AppBundle\Entity\Category")
         * @Assert\Valid()
         */
        protected $category;

        // ...

        public function getCategory()
        {
            return $this->category;
        }

        public function setCategory(Category $category = null)
        {
            $this->category = $category;
        }
    }

.. tip::

    Il vincolo ``Valid`` è stato aggiunto alla proprietà ``category``. In questo modo si valida a
    cascata l'entità corrispondente. Se si omette tale vincolo, l'entità
    figlia non sarà validata.

Ora che l'applicazione è stata aggiornata per riflettere le nuove esigenze,
creare una classe di form in modo che l'oggetto ``Category`` possa essere modificato dall'utente::

    // src/AppBundle/Form/Type/CategoryType.php
    namespace AppBundle\Form\Type;

    use Symfony\Component\Form\AbstractType;
    use Symfony\Component\Form\FormBuilderInterface;
    use Symfony\Component\OptionsResolver\OptionsResolverInterface;

    class CategoryType extends AbstractType
    {
        public function buildForm(FormBuilderInterface $builder, array $options)
        {
            $builder->add('name');
        }

        public function setDefaultOptions(OptionsResolverInterface $resolver)
        {
            $resolver->setDefaults(array(
                'data_class' => 'AppBundle\Entity\Category',
            ));
        }

        public function getName()
        {
            return 'category';
        }
    }

L'obiettivo finale è quello di far si che la ``Category`` di un ``Task`` possa essere correttamente modificata
all'interno dello stesso form task. Per farlo, aggiungere il campo ``category``
all'oggetto ``TaskType``, il cui tipo è un'istanza della nuova classe
``CategoryType``:

.. code-block:: php

    use Symfony\Component\Form\FormBuilderInterface;

    public function buildForm(FormBuilderInterface $builder, array $options)
    {
        // ...

        $builder->add('category', new CategoryType());
    }

I campi di ``CategoryType`` ora possono essere resi accanto a quelli
della classe ``TaskType``.

Rendere i campi di ``Category`` allo stesso modo dei campi ``Task`` originali:

.. configuration-block::

    .. code-block:: html+jinja

        {# ... #}

        <h3>Category</h3>
        <div class="category">
            {{ form_row(form.category.name) }}
        </div>

        {# ... #}

    .. code-block:: html+php

        <!-- ... -->

        <h3>Category</h3>
        <div class="category">
            <?php echo $view['form']->row($form['category']['name']) ?>
        </div>

        <!-- ... -->

Quando l'utente invia il form, i dati inviati con i campi ``Category``
sono utilizzati per costruire un'istanza di ``Category``, che viene poi impostata sul
campo ``category`` dell'istanza ``Task``.

L'istanza ``Category`` è accessibile naturalmente attraverso ``$task->getCategory()``
e può essere memorizzata nella base dati o utilizzata quando serve.

Incorporare un insieme di form
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

È anche possibile incorporare un insieme di form in un form (si immagini un form
``Category`` con tanti sotto-form ``Product``).
Lo si può fare utilizzando il tipo di campo ``collection``.

Per maggiori informazioni, vedere la ricetta ":doc:`/cookbook/form/form_collections`" e il
:doc:`riferimento al tipo collection</reference/forms/types/collection>`.

.. index::
   single: Form; Temi
   single: Form; Personalizzare i campi

.. _form-theming:

Temi con i form
---------------

Ogni parte nel modo in cui un form viene reso può essere personalizzata. Si è liberi di cambiare
come ogni "riga" del form viene resa, modificare il markup utilizzato per rendere gli errori, o
anche personalizzare la modalità con cui un tag ``textarea`` dovrebbe essere rappresentato. Nulla è off-limits,
e personalizzazioni differenti possono essere utilizzate in posti diversi.

Symfony utilizza i template per rendere ogni singola parte di un form, come ad esempio
i tag ``label``, i tag ``input``, i messaggi di errore e ogni altra cosa.

In Twig, ogni "frammento" di form è rappresentato da un blocco Twig. Per personalizzare
una qualunque parte di come un form è reso, basta sovrascrivere il blocco appropriato.

In PHP, ogni "frammento" è reso tramite un file template individuale.
Per personalizzare una qualunque parte del modo in cui un form viene reso, basta sovrascrivere il
template esistente creandone uno nuovo.

Per capire come funziona, cerchiamo di personalizzare il frammento ``form_row`` e
aggiungere un attributo class all'elemento ``div`` che circonda ogni riga. Per
farlo, creare un nuovo file template per salvare il nuovo codice:

.. configuration-block::

    .. code-block:: html+jinja

        {# app/Resources/views/form/fields.html.twig #}
        {% block form_row %}
        {% spaceless %}
            <div class="form_row">
                {{ form_label(form) }}
                {{ form_errors(form) }}
                {{ form_widget(form) }}
            </div>
        {% endspaceless %}
        {% endblock form_row %}

    .. code-block:: html+php

        <!-- app/Resources/views/form/form_row.html.php -->
        <div class="form_row">
            <?php echo $view['form']->label($form, $label) ?>
            <?php echo $view['form']->errors($form) ?>
            <?php echo $view['form']->widget($form, $parameters) ?>
        </div>

Il frammento di form ``field_row`` è utilizzato per rendere la maggior parte dei campi attraverso la
funzione ``form_row``. Per dire al componente form di utilizzare il nuovo frammento
``field_row`` definito sopra, aggiungere il codice seguente all'inizio del template che
rende il form:

.. configuration-block::

    .. code-block:: html+jinja

        {# app/Resources/views/default/new.html.twig #}
        {% form_theme form 'form/fields.html.twig' %}

        {# oppure, se si vogliono usare più temi #}
        {% form_theme form 'form/fields.html.twig' 'form/fields2.html.twig' %}

        {# ... rendere il form #}

    .. code-block:: html+php

        <!-- app/Resources/views/default/new.html.php -->
        <?php $view['form']->setTheme($form, array('form')) ?>

        <!-- oppure, se si vogliono usare più temi -->
        <?php $view['form']->setTheme($form, array('form', 'form2')) ?>

        <!-- ... rendere il form -->

Il tag ``form_theme`` (in Twig) "importa" i frammenti definiti nel dato
template e li usa quando deve rendere il form. In altre parole, quando la
funzione ``form_row`` è successivamente chiamata in questo template, utilizzerà il
blocco ``field_row`` dal tema personalizzato (al posto del blocco predefinito ``field_row``
fornito con Symfony).

Non è necessario che il tema personalizzato sovrascriva tutti i blocchi. Quando viene reso un blocco
non sovrascrritto nel tema personalizzato, il sistema dei temi userà il
tema globale (definito a livello di bundle).

Se vengono forniti più temi personalizzati, saranno analizzati nell'ordine elencato,
prima di usare il tema globale.

Per personalizzare una qualsiasi parte di un form, basta sovrascrivere il frammento
appropriato. Sapere esattamente qual è il blocco o il file da sovrascrivere è l'oggetto
della sezione successiva.

Per una trattazione più ampia, vedere :doc:`/cookbook/form/form_customization`.

.. index::
   single: Form; Nomi per i frammenti di form

.. _form-template-blocks:

Nomi per i frammenti di form
~~~~~~~~~~~~~~~~~~~~~~~~~~~~

In Symfony, ogni parte di un form che viene reso (elementi HTML del form, errori,
etichette, ecc.) è definito in un tema base, che in Twig è una raccolta
di blocchi e in PHP una collezione di file template.

In Twig, ogni blocco necessario è definito in un singolo file template (p.e.
`form_div_layout.html.twig`_) che si trova all'interno di `Twig Bridge`_. Dentro
questo file, è possibile ogni blocco necessario alla resa del form e ogni tipo predefinito di
campo.

In PHP, i frammenti sono file template individuali. Per impostazione predefinita sono posizionati
nella cartella `Resources/views/Form` del bundle framework (`vedere su GitHub`_).

Ogni nome di frammento segue lo stesso schema di base ed è suddiviso in due pezzi,
separati da un singolo carattere di sottolineatura (``_``). Alcuni esempi sono:

* ``field_row`` - usato da ``form_row`` per rendere la maggior parte dei campi;
* ``textarea_widget`` - usato da ``form_widget`` per rendere un campo di tipo
  ``textarea``;
* ``field_errors`` - usato da ``form_errors`` per rendere gli errori di un campo;

Ogni frammento segue lo stesso schema di base: ``type_part``. La parte ``type``
corrisponde al campo *type* che viene reso (es. ``textarea``, ``checkbox``,
``date``, ecc) mentre la parte ``part`` corrisponde a *cosa* si sta
rendendo (es. ``label``, ``widget``, ``errors``, ecc). Per impostazione predefinita, ci
sono 4 possibili *parti* di un form che possono essere rese:

+-------------+------------------------+-------------------------------------------------------------+
| ``label``   | (es. ``form_label``)   | rende l'etichetta dei campi                                 |
+-------------+------------------------+-------------------------------------------------------------+
| ``widget``  | (es. ``form_widget``)  | rende la rappresentazione HTML dei campi                    |
+-------------+------------------------+-------------------------------------------------------------+
| ``errors``  | (es. ``form_errors``)  | rende gli errori dei campi                                  |
+-------------+------------------------+-------------------------------------------------------------+
| ``row``     | (es. ``form_row``)     | rende l'intera riga del campo (etichetta, widget ed errori) |
+-------------+------------------------+-------------------------------------------------------------+

.. note::

    In realtà ci sono altre 3 *parti* (``rows``, ``rest`` e ``enctype``),
    ma raramente c'è la necessità di sovrascriverle.

Conoscendo il tipo di campo (ad esempio ``textarea``) e che parte si vuole
personalizzare (ad esempio ``widget``), si può costruire il nome del frammento che
deve essere sovrascritto (esempio ``textarea_widget``).

.. index::
   single: Form; Ereditarietà dei frammenti di template

Ereditarietà dei frammenti di template
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

In alcuni casi, il frammento che si vuole personalizzare sembrerà mancare.
Ad esempio, non c'è nessun frammento ``textarea_errors`` nei temi predefiniti
forniti con Symfony. Quindi dove sono gli errori di un campo textarea che deve essere reso?

La risposta è: nel frammento ``field_errors``. Quando Symfony rende gli errori
per un tipo textarea, prima cerca un frammento ``textarea_errors``, poi cerca
un frammento ``form_errors``. Ogni tipo di campo ha un tipo *genitore*
(il tipo genitore di ``textarea`` è ``text``) e Symfony utilizza il
frammento per il tipo del genitore se il frammento di base non
esiste.

Quindi, per ignorare gli errori dei *soli* campi ``textarea``, copiare il
frammento ``form_errors``, rinominarlo in ``textarea_errors`` e personalizzarlo. Per
sovrascrivere la resa degli errori predefiniti di *tutti* i campi, copiare e personalizzare
direttamente il frammento ``form_errors``.

.. tip::

    Il tipo "genitore" di ogni tipo di campo è disponibile
    per ogni tipo di campo in :doc:`form type reference</reference/forms/types>`

.. index::
   single: Form; Temi globali

.. _book-forms-theming-global:

Temi globali per i form
~~~~~~~~~~~~~~~~~~~~~~~

Nell'esempio sopra, è stato utilizzato l'helper ``form_theme`` (in Twig) per "importare"
i frammenti personalizzati *solo* in quel form. Si può anche dire a Symfony
di importare personalizzazioni del form nell'intero progetto.

.. _book-forms-theming-twig:

Twig
....

Per includere automaticamente i blocchi personalizzati del template
``fields.html.twig`` creato in precedenza, in *tutti* i template, modificare il file
della configurazione dell'applicazione:

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        twig:
            form:
                resources:
                    - 'form/fields.html.twig'
            # ...

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <?xml version="1.0" encoding="UTF-8" ?>
        <container xmlns="http://symfony.com/schema/dic/services"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xmlns:twig="http://symfony.com/schema/dic/twig"
            xsi:schemaLocation="http://symfony.com/schema/dic/services http://symfony.com/schema/dic/services/services-1.0.xsd
                http://symfony.com/schema/dic/twig http://symfony.com/schema/dic/twig/twig-1.0.xsd">

            <twig:config>
                <twig:form>
                    <twig:resource>form/fields.html.twig</twig:resource>
                </twig:form>
                <!-- ... -->
            </twig:config>
        </container>

    .. code-block:: php

        // app/config/config.php
        $container->loadFromExtension('twig', array(
            'form' => array(
                'resources' => array(
                    'form/fields.html.twig',
                ),
            ),
            // ...
        ));

Tutti i blocchi all'interno del template ``fields.html.twig`` vengono ora utilizzati a livello globale
per definire l'output del form.

.. sidebar::  Personalizzare tutti gli output del form in un singolo file con Twig

    Con Twig, si può anche personalizzare il blocco di un form all'interno del template
    in cui questa personalizzazione è necessaria:

    .. code-block:: html+jinja

        {% extends 'base.html.twig' %}

        {# import "_self" as the form theme #}
        {% form_theme form _self %}

        {# make the form fragment customization #}
        {% block form_row %}
            {# custom field row output #}
        {% endblock form_row %}

        {% block content %}
            {# ... #}

            {{ form_row(form.task) }}
        {% endblock %}

    Il tag ``{% form_theme form _self %}`` consente ai blocchi del form di essere personalizzati
    direttamente all'interno del template che utilizzerà tali personalizzazioni. Utilizzare
    questo metodo per creare velocemente personalizzazioni del form che saranno
    utilizzate solo in un singolo template.

    .. caution::

        La funzionalità ``{% form_theme form _self %}`` funziona *solo*
        se un template estende un altro. Se un template non estende, occorre
        far puntare ``form_theme`` a un template separato.

PHP
...

Per includere automaticamente i template personalizzati dalla cartella ``app/Resources/views/Form``
creata in precedenza in *tutti* i template, modificare il file con la configurazione
dell'applicazione:

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        framework:
            templating:
                form:
                    resources:
                        - 'Form'
        # ...

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <?xml version="1.0" encoding="UTF-8" ?>
        <container xmlns="http://symfony.com/schema/dic/services"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xmlns:framework="http://symfony.com/schema/dic/symfony"
            xsi:schemaLocation="http://symfony.com/schema/dic/services http://symfony.com/schema/dic/services/services-1.0.xsd
                http://symfony.com/schema/dic/symfony http://symfony.com/schema/dic/symfony/symfony-1.0.xsd">

            <framework:config>
                <framework:templating>
                    <framework:form>
                        <framework:resource>Form</framework:resource>
                    </framework:form>
                </framework:templating>
                <!-- ... -->
            </framework:config>
        </container>

    .. code-block:: php

        // app/config/config.php
        $container->loadFromExtension('framework', array(
            'templating' => array(
                'form' => array(
                    'resources' => array(
                        'Form',
                    ),
                ),
            ),
            // ...
        ));

Ogni frammento all'interno della cartella ``app/Resources/views/Form`` è ora
usato globalmente per definire l'output del form.

.. index::
   single: Form; Protezione CSRF

.. _forms-csrf:

Protezione da CSRF
------------------

CSRF, o `Cross-site request forgery`_, è un metodo mediante il quale un utente
malintenzionato cerca di fare inviare inconsapevolmente agli utenti legittimi dati che
non vorrebbero inviare. Fortunatamente, gli attacchi CSRF possono essere prevenuti,
utilizzando un token CSRF all'interno dei form.

La buona notizia è che, per impostazione predefinita, Symfony integra e convalida i token CSRF
automaticamente. Questo significa che è possibile usufruire della protezione
CSRF, senza dover far nulla. Infatti, ogni form in questo capitolo
sfrutta la protezione CSRF!

La protezione CSRF funziona con l'aggiunta al form di un campo nascosto, il cui nome
predefinito è ``_token``, che contiene un valore noto solo allo sviluppatore e all'utente. Questo
garantisce che proprio l'utente, e non qualcun altro, stia inviando i dati.
Symfony valida automaticamente la presenza e l'esattezza di questo token.

Il campo ``_token`` è un campo nascosto e sarà reso automaticamente
se si include la funzione ``form_end()`` nel template, perché questa assicura
che tutti i campi non ancora resi vengano visualizzati.

Il token CSRF può essere personalizzato specificatamente per ciascun form. Per esempio::

    use Symfony\Component\OptionsResolver\OptionsResolverInterface;

    class TaskType extends AbstractType
    {
        // ...

        public function setDefaultOptions(OptionsResolverInterface $resolver)
        {
            $resolver->setDefaults(array(
                'data_class'      => 'AppBundle\Entity\Task',
                'csrf_protection' => true,
                'csrf_field_name' => '_token',
                // una chiave univoca per generare il token
                'intention'       => 'task_item',
            ));
        }

        // ...
    }

.. _form-disable-csrf:

Per disabilitare la protezione CSRF, impostare l'opzione ``csrf_protection`` a ``false``.
Si può anche personalizzare a livello globale nel progetto. Per ulteriori informazioni,
vedere la sezione
:ref:`riferimento della configurazione dei form <reference-framework-form>`.

.. note::

    L'opzione ``intention`` è facoltativa, ma migliora notevolmente la sicurezza
    del token generato, rendendolo diverso per ogni modulo.

.. caution::

    I token CSRF sono pensati per essere diversi per ciascun utente. Per questo motivo,
    occorre cautela nel provare a mettere in cache pagine con form che includano questo
    tipo di protezione. Per maggiori informazioni, vedere
    :doc:`/cookbook/cache/form_csrf_caching`.

.. index:
   single: Form; Senza classe

Usare un form senza una classe
------------------------------

Nella maggior parte dei casi, un form è legato a un oggetto e i campi del form prendono i
loro dati dalle proprietà di tale oggetto. Questo è quanto visto finora in
questo capitolo, con la classe `Task`.

A volte, però, si vuole solo usare un form senza classi, per ottenere un
array di dati inseriti. Lo si può fare in modo molto facile::

    // assicurarsi di aver importato lo spazio dei nomi Request all'inizio della classe
    use Symfony\Component\HttpFoundation\Request
    // ...

    public function contactAction(Request $request)
    {
        $defaultData = array('message' => 'Type your message here');
        $form = $this->createFormBuilder($defaultData)
            ->add('name', 'text')
            ->add('email', 'email')
            ->add('message', 'textarea')
            ->add('send', 'submit')
            ->getForm();

        $form->handleRequest($request);

        if ($form->isValid()) {
            // data è un array con "name", "email", e "message" come chiavi
            $data = $form->getData();
        }

        // ... rendere il form
    }

Per impostazione predefinita, un form ipotizza che si voglia lavorare con array
di dati, invece che con oggetti. Ci sono due modi per modificare questo comportamento
e legare un form a un oggetto:

#. Passare un oggetto alla creazione del form (come primo parametro di
   ``createFormBuilder`` o come secondo parametro di ``createForm``);

#. Dichiarare l'opzione ``data_class`` nel form.

Se *non* si fa nessuna di queste due cose, il form restituirà i dati come
array. In questo esempio, poiché ``$defaultData`` non è un oggetto (e
l'opzione ``data_class`` è omessa), ``$form->getData()`` restituirà
un array.

.. tip::

    Si può anche accedere ai valori POST ("name", in questo caso) direttamente tramite
    l'oggetto ``Request``, in questo modo::

        $request->request->get('name');

    Tuttavia, si faccia attenzione che in molti casi l'uso del metodo ``getData()`` è
    preferibile, poiché restituisce i dati (solitamente un oggetto) dopo
    che sono stati manipolati dal sistema dei form.

Aggiungere la validazione
~~~~~~~~~~~~~~~~~~~~~~~~~

L'ultima parte mancante è la validazione. Solitamente, quando si richiama ``$form->isValid()``,
l'oggetto viene validato dalla lettura dei vincoli applicati alla
classe. Se il form è legato a un oggetto (cioè se si sta usando l'opzione ``data_class``
o passando un oggetto al form), questo è quasi sempre l'approccio
desiderato. Vedere :doc:`/book/validation` per maggiori dettagli.

.. _form-option-constraints:

Ma se il form non è legato a un oggetto e invece si sta recuperando un semplice array
di dati inviati, come si possono aggiungere vincoli al
form?

La risposta è: impostare i vincoli in modo autonomo e passarli al form.
L'approccio generale è spiegato meglio nel :ref:`capitolo sulla validazione<book-validation-raw-values>`,
ma ecco un breve esempio:

.. versionadded:: 2.1
   L'opzione ``constraints``, che accetta un singolo vincolo o un array
   di vincoli (prima della 2.1, l'opzione era chiamata ``validation_constraint``
   e accettava solo un singolo vincolo), è nuova in Symfony 2.1.

.. code-block:: php

    use Symfony\Component\Validator\Constraints\Length;
    use Symfony\Component\Validator\Constraints\NotBlank;

    $builder
       ->add('firstName', 'text', array(
           'constraints' => new Length(array('min' => 3)),
       ))
       ->add('lastName', 'text', array(
           'constraints' => array(
               new NotBlank(),
               new Length(array('min' => 3)),
           ),
       ))
    ;

.. tip::

    Se si usano i gruppi di validazione, occorre fare riferimento al gruppo
    ``Default`` quando si crea il form, oppure impostare il gruppo corretto
    nel vincolo che si sta aggiungendo.

.. code-block:: php

    new NotBlank(array('groups' => array('create', 'update'))

Considerazioni finali
---------------------

Ora si è a conoscenza di tutti i mattoni necessari per costruire form complessi e
funzionali per la propria applicazione. Quando si costruiscono form, bisogna tenere presente che
il primo obiettivo di un form è quello di tradurre i dati da un oggetto (``Task``) a un
form HTML in modo che l'utente possa modificare i dati. Il secondo obiettivo di un form è quello di
prendere i dati inviati dall'utente e ri-applicarli all'oggetto.

Ci sono altre cose da imparare sul potente mondo dei form, ad esempio
come gestire :doc:`il caricamento di file con Doctrine
</cookbook/doctrine/file_uploads>` o come creare un form dove un numero
dinamico di sub-form possono essere aggiunti (ad esempio una todo list in cui è possibile continuare ad aggiungere
più campi tramite Javascript prima di inviare). Vedere il ricettario per questi
argomenti. Inoltre, assicurarsi di basarsi sulla
:doc:`documentazione di riferimento sui tipi di campo</reference/forms/types>`, che
comprende esempi di come usare ogni tipo di campo e le relative opzioni.

Saperne di più con il ricettario
--------------------------------

* :doc:`/cookbook/doctrine/file_uploads`
* :doc:`Riferimento del tipo di campo file </reference/forms/types/file>`
* :doc:`Creare tipi di campo personalizzati </cookbook/form/create_custom_field_type>`
* :doc:`/cookbook/form/form_customization`
* :doc:`/cookbook/form/dynamic_form_modification`
* :doc:`/cookbook/form/data_transformers`
* :doc:`/cookbook/security/csrf_in_login_form`
* :doc:`/cookbook/cache/form_csrf_caching`

.. _`Componente Form di Symfony`: https://github.com/symfony/Form
.. _`DateTime`: http://php.net/manual/it/class.datetime.php
.. _`Twig Bridge`: https://github.com/symfony/symfony/tree/2.3/src/Symfony/Bridge/Twig
.. _`form_div_layout.html.twig`: https://github.com/symfony/symfony/blob/2.3/src/Symfony/Bridge/Twig/Resources/views/Form/form_div_layout.html.twig
.. _`Cross-site request forgery`: http://it.wikipedia.org/wiki/Cross-site_request_forgery
.. _`vedere su GitHub`: https://github.com/symfony/symfony/tree/2.3/src/Symfony/Bundle/FrameworkBundle/Resources/views/Form
