.. index::
   single: Form; Eventi

Modificare dinamicamente form usando gli eventi
===============================================

Capita spesso che un form non possa essere creato staticamente. In questa ricetta
vedremo come personalizzare un form, in base a tre casi d'uso.

1) :ref:`cookbook-form-events-underlying-data`

Esempio: si ha un form "Product" e occorre modificare/aggiungere/rimuovere un campo,
in base ai dati sull'oggetto Product sottostante.

2) :ref:`cookbook-form-events-user-data`

Esempio: si crea un form "Friend Message" e occorre costruire un menù a tendina,
che contenga solo utenti che sono amici dell'utente attualmente
autenticato.

3) :ref:`cookbook-form-events-submitted-data`

Esempio: in un form di registrazione, si ha un campo "country" e un campo "state",
che va popolato automaticamente in base al valore del campo
"country".

Se si vuole approfondire le basi che stanno dietro agli eventi dei form, si può
dare un'occhiata alla documentazione sugli
the :doc:`eventi dei form </components/form/form_events>`.

.. _cookbook-form-events-underlying-data:

Personalizzare un form in base ai dati sottostanti
--------------------------------------------------

Prima di addentrarci nella generazione dinamica dei form, diamo un'occhiata veloce 
alla classe dei form::

    // src/AppBundle/Form/Type/ProductType.php
    namespace AppBundle\Form\Type;

    use Symfony\Component\Form\AbstractType;
    use Symfony\Component\Form\FormBuilderInterface;
    use Symfony\Component\OptionsResolver\OptionsResolverInterface;

    class ProductType extends AbstractType
    {
        public function buildForm(FormBuilderInterface $builder, array $options)
        {
            $builder->add('name');
            $builder->add('price');
        }

        public function setDefaultOptions(OptionsResolverInterface $resolver)
        {
            $resolver->setDefaults(array(
                'data_class' => 'AppBundle\Entity\Product'
            ));
        }

        public function getName()
        {
            return 'product';
        }
    }

.. note::

    Se questa particolare sezione di codice non è familiare,
    probabilmente è necessario tornare indietro e come prima cosa leggere il :doc:`capitolo sui form</book/forms>` 
    prima di andare avanti.

Si assuma per un momento che questo form utilizzi una classe immaginaria "Product",
che ha solo due attributi rilevanti ("name" e "price"). Il form generato 
da questa classe avrà lo stesso aspetto, indipendentemente se sta per essere creato un nuovo prodotto
oppure se sta per essere modificato un prodotto esistente (cioè un prodotto ottenuto da base dati).

Si supponga ora di non voler abilitare l'utente alla modifica del campo ``name``,
una volta che l'oggetto è stato creato. Lo si può fare grazie al componente
:doc:`EventDispatcher </components/event_dispatcher/introduction>`,
che analizza l'oggetto e modifica il form basato sull'
oggetto Product. In questa ricetta si imparerà come aggiungere questo livello di
flessibilità ai form.

.. _`cookbook-forms-event-listener`:

Aggiungere un ascoltatore di eventi alla classe di un form
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Quindi, invece di aggiungere direttamente ``name``, la responsabilità di
creare tale campo è delegata a un ascoltatore di eventi::

    // src/AppBundle/Form/Type/ProductType.php
    namespace AppBundle\Form\Type;

    // ...
    use Symfony\Component\Form\FormEvent;
    use Symfony\Component\Form\FormEvents;

    class ProductType extends AbstractType
    {
        public function buildForm(FormBuilderInterface $builder, array $options)
        {
            $builder->add('price');

            $builder->addEventListener(FormEvents::PRE_SET_DATA, function(FormEvent $event) {
                // ... aggiungere il campo name, se necessario
            });
        }

        // ...
    }


Lo scopo è quello di creare un campo ``name`` *solo* se l'oggetto ``Product`` sottostante
è nuovo (cioè se non è stato persistito sulla base dati). In base a questo,
l'ascoltatore di eventi potrebbe somigliare a questo::

    // ...
    public function buildForm(FormBuilderInterface $builder, array $options)
    {
        // ...
        $builder->addEventListener(FormEvents::PRE_SET_DATA, function (FormEvent $event) {
            $product = $event->getData();
            $form = $event->getForm();

            // verifica se l'oggetto Product sia "nuovo"
            // Se non sono stati passati dati al form, i dati sono "null".
            // Questo va considerato un nuovo Product
            if (!$product || null === $product->getId()) {
                $form->add('name', 'text');
            }
        });
    }

.. versionadded:: 2.2
    La possibilità di passare una stringa a
    :method:`FormInterface::add <Symfony\\Component\\Form\\FormInterface::add>`
    è stata aggiunta in Symfony 2.2.

.. note::

    La riga ``FormEvents::PRE_SET_DATA`` viene risolta in
    ``form.pre_set_data``. :class:`Symfony\\Component\\Form\\FormEvents`
    ha uno scopo organizzativo. È un posto centralizzato in cui
    si possono trovare tutti i vari eventi disponibili per i form. La lista
    completa degli eventi è nella classe
    class:`Symfony\\Component\\Form\\FormEvents`.

.. _`cookbook-forms-event-subscriber`:

Aggiungere un sottoscrittore di eventi alla classe di un form
-------------------------------------------------------------

Per una migliore riusabilità o se c'è della logica in un ascoltatore di eventi,
si può spostare la logica per creare il campo ``name`` in un
:ref:`sottoscrittore di eventi <event_dispatcher-using-event-subscribers>`::

    // src/AppBundle/Form/Type/ProductType.php
    namespace AppBundle\Form\Type;

    // ...
    use AppBundle\Form\EventListener\AddNameFieldSubscriber;

    class ProductType extends AbstractType
    {
        public function buildForm(FormBuilderInterface $builder, array $options)
        {
            $builder->add('price');

            $builder->addEventSubscriber(new AddNameFieldSubscriber());
        }

        // ...
    }

Ora la logica per creare il campo ``name`` si trova nella propria classe
sottoscrittore::

    // src/AppBundle/Form/EventListener/AddNameFieldSubscriber.php
    namespace AppBundle\Form\EventListener;

    use Symfony\Component\Form\FormEvent;
    use Symfony\Component\Form\FormEvents;
    use Symfony\Component\EventDispatcher\EventSubscriberInterface;

    class AddNameFieldSubscriber implements EventSubscriberInterface
    {
        public static function getSubscribedEvents()
        {
            // Indica al distributore che si vuole ascoltare l'evento form.pre_set_data
            // e che verrà invocato il metodo preSetData.
            return array(FormEvents::PRE_SET_DATA => 'preSetData');
        }

        public function preSetData(FormEvent $event)
        {
            $product = $event->getData();
            $form = $event->getForm();

            if (!$product || null === $product->getId()) {
                $form->add('name', 'text');
            }
        }
    }


.. _cookbook-form-events-user-data:

Generare dinamicamente form in base ai dati dell'utente
-------------------------------------------------------

A volte si vuole che un form sia generato dinamicamente, non solo in base ai dati
del form, ma anche in base ad altro, come dati provenienti dall'utente attuale.
Si supponga di avere un sito sociale, in cui un utente può inviare messaggi solo ai
suo amici. In questo caso, una lista per scegliere a chi inviare il messaggio
dovrebbe contenere solo utenti che siano amici dell'utente attuale.

Creare il form Type
~~~~~~~~~~~~~~~~~~~

Usando un ascoltatore di eventi, il form potrebbe assomigliare a questo::

    // src/AppBundle/Form/Type/FriendMessageFormType.php
    namespace AppBundle\Form\Type;

    use Symfony\Component\Form\AbstractType;
    use Symfony\Component\Form\FormBuilderInterface;
    use Symfony\Component\Form\FormEvents;
    use Symfony\Component\Form\FormEvent;
    use Symfony\Component\Security\Core\SecurityContext;
    use Symfony\Component\OptionsResolver\OptionsResolverInterface;

    class FriendMessageFormType extends AbstractType
    {
        public function buildForm(FormBuilderInterface $builder, array $options)
        {
            $builder
                ->add('subject', 'text')
                ->add('body', 'textarea')
            ;
            $builder->addEventListener(FormEvents::PRE_SET_DATA, function(FormEvent $event) {
                // ... aggiungere una lista di amici dell'utente attuale
            });
        }

        public function getName()
        {
            return 'friend_message';
        }

        public function setDefaultOptions(OptionsResolverInterface $resolver)
        {
        }
    }

Il problema ora è ottenere l'utente attuale e creare un campo di scelta, che
contenga solo i suoi amici.

Fortunatamente, è alquanto facile iniettare un servizio nel form. Lo si può
fare nel costruttore::

    private $securityContext;

    public function __construct(SecurityContext $securityContext)
    {
        $this->securityContext = $securityContext;
    }

.. note::

    Ci si potrebbe chiedere, ora che si ha accesso all'utente (attraverso
    SecurityContext), perché non usarlo direttamente in ``buildForm``, senza
    usare un ascoltatore. La risposta è che, così facendo, l'intero form type
    sarebbe modificato, non solamente questa singola istanza
    del form. Di solito questo non sarebbe un problema, ma tecnicamente
    un singolo form type potrebbe essere usato in una singola richiesta per creare molti
    form o molti campi.

Personalizzare il Form Type
~~~~~~~~~~~~~~~~~~~~~~~~~~~

Ora che si dispone di tutto il necessario, si può sfruttare ``securityContext``
e scrivere la logica dell'ascoltatore::

    // src/AppBundle/FormType/FriendMessageFormType.php

    use Symfony\Component\Security\Core\SecurityContext;
    use Doctrine\ORM\EntityRepository;
    // ...

    class FriendMessageFormType extends AbstractType
    {
        private $securityContext;

        public function __construct(SecurityContext $securityContext)
        {
            $this->securityContext = $securityContext;
        }

        public function buildForm(FormBuilderInterface $builder, array $options)
        {
            $builder
                ->add('subject', 'text')
                ->add('body', 'textarea')
            ;

            // prende l'utente, fa un rapido controllo che esista
            $user = $this->securityContext->getToken()->getUser();
            if (!$user) {
                throw new \LogicException(
                    'The FriendMessageFormType cannot be used without an authenticated user!'
                );
            }

            $builder->addEventListener(
                FormEvents::PRE_SET_DATA,
                function (FormEvent $event) use ($user) {
                    $form = $event->getForm();

                    $formOptions = array(
                        'class' => 'AppBundle\Entity\User',
                        'property' => 'fullName',
                        'query_builder' => function (EntityRepository $er) use ($user) {
                            // usare una query personalizzata 
                            // return $er->createQueryBuilder('u')->addOrderBy('fullName', 'DESC');

                            // o richiamare un metodo del repository che restituisce un query builder
                            // $er è un'istanza di UserRepository
                            // return $er->createOrderByFullNameQueryBuilder();
                        },
                    );

                    // creare il campo, similmente a $builder->add()
                    // nome del campo, tipo di campo, dati, opzioni
                    $form->add('friend', 'entity', $formOptions);
                }
            );
        }

        // ...
    }

.. note::

    Le opzioni ``multiple`` ed ``expanded`` varranno ``false``,
    perché il tipo del campo ``friend`` è ``entity``.

Usare il form
~~~~~~~~~~~~~

Il form ora è pronto da usare e ci sono due modi possibili per usarlo in un
controllore:

a) crearlo a mano e ricordarsi di passargli SecurityContext;

oppure

b) definirlo come servizio.

a) Creare il form a mano
........................

È molto semplice e probabilmente l'approccio migliore, a meno di non usare
il nuovo form type in molti posti o includerlo in altri form::

    class FriendMessageController extends Controller
    {
        public function newAction(Request $request)
        {
            $securityContext = $this->container->get('security.context');
            $form = $this->createForm(
                new FriendMessageFormType($securityContext)
            );

            // ...
        }
    }

b) Definire il form come servizio
.................................

Per definire il form come servizio, creare un normale servizio e aggiungere il tag
:ref:`dic-tags-form-type`.

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        services:
            app.form.friend_message:
                class: AppBundle\Form\Type\FriendMessageFormType
                arguments: ["@security.context"]
                tags:
                    - { name: form.type, alias: friend_message }

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <services>
            <service id="app.form.friend_message" class="AppBundle\Form\Type\FriendMessageFormType">
                <argument type="service" id="security.context" />
                <tag name="form.type" alias="friend_message" />
            </service>
        </services>

    .. code-block:: php

        // app/config/config.php
        $definition = new Definition('AppBundle\Form\Type\FriendMessageFormType');
        $definition->addTag('form.type', array('alias' => 'friend_message'));
        $container->setDefinition(
            'app.form.friend_message',
            $definition,
            array('security.context')
        );

Se si vuole crearlo da dentro un controllore o un altro servizio che abbia accesso
al form factory, si può usare::

    use Symfony\Component\DependencyInjection\ContainerAware;

    class FriendMessageController extends ContainerAware
    {
        public function newAction(Request $request)
        {
            $form = $this->get('form.factory')->create('friend_message');

            // ...
        }
    }

Se si estende la classe ``Symfony\Bundle\FrameworkBundle\Controller\Controller``, basta chiamare::

    $form = $this->createForm('friend_message');

Si può anche includere il form type in un altro form::

    // dentro un'altra classe "form type"
    public function buildForm(FormBuilderInterface $builder, array $options)
    {
        $builder->add('message', 'friend_message');
    }

.. _cookbook-form-events-submitted-data:

Generazione dinamica per form inviati
-------------------------------------

Un altro caso possibile è l'esigenza di personalizzare il form in base ai
dati inviati dall'utente. Per esempio, si immagini di avere un form di registrazione
per riunioni sportive. Alcuni eventi consentiranno di specificare la posizione preferita
sul campo. Questo, per esempio, sarebbe un campo ``choice``. Tuttavia, le scelte
possibili dipenderanno da ciascuno sport. Il calcio avrà attacco, difesa,
portiere, ecc... Il baseball avrà un lanciatore, ma non un portiere. Servono
le opzioni giuste impostate, per poter passare la validazione.

La riunione sarà passata come campo nascosto al form. In questo modo si può
accedere a ciascuno sport in questo modo::

    // src/AppBundle/Form/Type/SportMeetupType.php
    namespace AppBundle\Form\Type;

    use Symfony\Component\Form\AbstractType;
    use Symfony\Component\Form\FormBuilderInterface;
    use Symfony\Component\Form\FormEvent;
    use Symfony\Component\Form\FormEvents;
    // ...

    class SportMeetupType extends AbstractType
    {
        public function buildForm(FormBuilderInterface $builder, array $options)
        {
            $builder
                ->add('sport', 'entity', array(
                    'class'       => 'AppBundle:Sport',
                    'empty_value' => '',
                ))
            ;

            $builder->addEventListener(
                FormEvents::PRE_SET_DATA,
                function (FormEvent $event) {
                    $form = $event->getForm();

                    // questa sarà l'entità, p.e. SportMeetup
                    $data = $event->getData();

                    $sport = $data->getSport();
                    $positions = null === $sport ? array() : $sport->getAvailablePositions();

                    $form->add('position', 'entity', array(
                        'class'       => 'AppBundle:Position',
                        'empty_value' => '',
                        'choices'     => $positions,
                    ));
                }
            );
        }

        // ...
    }

Quando si costruisce il form per mostrarlo per la prima volta all'utente,
l'esempio funziona perfettamente.

Tuttavia, le cose si fanno più difficili quando si deve gestire l'invio del form.
Questo perché l'evento ``PRE_SET_DATA`` può riportare i dati con cui si inizia
(p.e. un oggetto ``SportMeetup`` vuoto), *non* ti dati inviati.

In un form, possiamo solitamente ascoltare questi eventi:

* ``PRE_SET_DATA``
* ``POST_SET_DATA``
* ``PRE_SUBMIT``
* ``SUBMIT``
* ``POST_SUBMIT``

.. versionadded:: 2.3
    Gli eventi ``PRE_SUBMIT``, ``SUBMIT`` e ``POST_SUBMIT`` sono stati aggiunti in
    Symfony 2.3. In precedenza, si chiamavano ``PRE_BIND``, ``BIND`` e ``POST_BIND``.

.. versionadded:: 2.2.6
    Il comportamento dell'evento ``POST_SUBMIT`` è cambiato leggermente in 2.2.6, usato
    dall'esempio seguente.

La chiave sta nell'aggiungere un ascoltatore ``POST_SUBMIT`` al campo da cui dipende il nuovo
campo. Se si aggiunge un ascoltatore ``POST_SUBMIT`` a un form figlio (p.e. ``sport``),
e si aggiungono nuovi figli al form genitore, il componente Form individuerà il
nuovo campo automaticamente e lo mapperà ai dati inviati dal client.

La classe ora sarà così::

    // src/AppBundle/Form/Type/SportMeetupType.php
    namespace AppBundle\Form\Type;

    // ...
    use Symfony\Component\Form\FormInterface;
    use AppBundle\Entity\Sport;

    class SportMeetupType extends AbstractType
    {
        public function buildForm(FormBuilderInterface $builder, array $options)
        {
            $builder
                ->add('sport', 'entity', array(
                    'class'       => 'AppBundle:Sport',
                    'empty_value' => '',
                ));
            ;

            $formModifier = function (FormInterface $form, Sport $sport = null) {
                $positions = null === $sport ? array() : $sport->getAvailablePositions();

                $form->add('position', 'entity', array(
                    'class'       => 'AppBundle:Position',
                    'empty_value' => '',
                    'choices'     => $positions,
                ));
            };

            $builder->addEventListener(
                FormEvents::PRE_SET_DATA,
                function (FormEvent $event) use ($formModifier) {
                    // questa sarebbe l'entità, p.e. SportMeetup
                    $data = $event->getData();

                    $formModifier($event->getForm(), $data->getSport());
                }
            );

            $builder->get('sport')->addEventListener(
                FormEvents::POST_SUBMIT,
                function(FormEvent $event) use ($formModifier) {
                    // è importante qui recuperare $event->getForm()->getData(), perché
                    // $event->getData() restituirà i dati del client (quindi l'ID)
                    $sport = $event->getForm()->getData();

                    // avendo aggiunto l'ascoltatore al figlio, dovremo passare
                    // il genitore alle funzioni callback!
                    $formModifier($event->getForm()->getParent(), $sport);
                }
            );
        }

        // ...
    }

Si può vedere come occorra ascoltare questi due eventi e avere callback diversi,
solo perché in due scenari diversi i dati che si possono usare vengono restituiti in
eventi diversi. Oltre a questo, gli ascoltatori eseguono esattamente le stesse cose
su un form dato.

Un pezzo ancora mancante è l'aggiornamento lato client del form, dopo la
scelta dello sport. Lo si può gestire tramite una chiamata AJAX
all'applicazione. Ipotizzando di avere un controllore per la creazione::

    // src/AppBundle/Controller/MeetupController.php
    namespace AppBundle\Controller;

    use Symfony\Bundle\FrameworkBundle\Controller\Controller;
    use Symfony\Component\HttpFoundation\Request;
    use AppBundle\Entity\SportMeetup;
    use AppBundle\Form\Type\SportMeetupType;
    // ...

    class MeetupController extends Controller
    {
        public function createAction(Request $request)
        {
            $meetup = new SportMeetup();
            $form = $this->createForm(new SportMeetupType(), $meetup);
            $form->handleRequest($request);
            if ($form->isValid()) {
                // ... salvare, rinviare, ecc.
            }

            return $this->render(
                'AppBundle:Meetup:create.html.twig',
                array('form' => $form->createView())
            );
        }

        // ...
    }

Il template associato usa un po' di JavaScript per aggiornare il campo ``position`` del form,
a seconda del valore selezionato nel campo ``sport``:

.. configuration-block::

    .. code-block:: html+jinja

        {# src/AppBundle/Resources/views/Meetup/create.html.twig #}
        {{ form_start(form) }}
            {{ form_row(form.sport) }}    {# <select id="meetup_sport" ... #}
            {{ form_row(form.position) }} {# <select id="meetup_position" ... #}
            {# ... #}
        {{ form_end(form) }}

        <script>
        var $sport = $('#meetup_sport');
        // Quando è stato selezionato lo sport ...
        $sport.change(function() {
          // ... recupera il form corrispondente.
          var $form = $(this).closest('form');
          // Simula i dati del form, ma include solo il valore selezionato di sport.
          var data = {};
          data[$sport.attr('name')] = $sport.val();
          // Invia i dati tramite AJAX al percorso dell'azione del form
          $.ajax({
            url : $form.attr('action'),
            type: $form.attr('method'),
            data : data,
            success: function(html) {
              // Sostituisce il campo della posizione attuale ...
              $('#meetup_position').replaceWith(
                // ... con quello restituito dalla risposta AJAX.
                $(html).find('#meetup_position')
              );
              // Il campo position ora mostra le posizioni appropriate.
            }
          });
        });
        </script>

    .. code-block:: html+php

        <!-- src/AppBundle/Resources/views/Meetup/create.html.php -->
        <?php echo $view['form']->start($form) ?>
            <?php echo $view['form']->row($form['sport']) ?>    <!-- <select id="meetup_sport" ... -->
            <?php echo $view['form']->row($form['position']) ?> <!-- <select id="meetup_position" ... -->
            <!-- ... -->
        <?php echo $view['form']->end($form) ?>

        <script>
        var $sport = $('#meetup_sport');
        // Quando è stato selezionato lo sport ...
        $sport.change(function() {
          // ... recupera il form corrispondente.
          var $form = $(this).closest('form');
          // Simula i dati del form, ma include solo il valore selezionato di sport.
          var data = {};
          data[$sport.attr('name')] = $sport.val();
          // Invia i dati tramite AJAX al percorso dell'azione del form
          $.ajax({
            url : $form.attr('action'),
            type: $form.attr('method'),
            data : data,
            success: function(html) {
              // Sostituisce il campo della posizione attuale ...
              $('#meetup_position').replaceWith(
                // ... con quello restituito dalla risposta AJAX.
                $(html).find('#meetup_position')
              );
              // Il campo position ora mostra le posizioni appropriate.
            }
          });
        });
        </script>

Il vantaggio maggiore di inviare l'intero form per estrarre solo il campo
``position`` aggiornato è che non serve alcun codice aggiuntivo lato server: tutto
il codice precedente per generare il form inviato può essere riutilizzato.

.. _cookbook-dynamic-form-modification-suppressing-form-validation:

Sopprimere la validazione
-------------------------

Per sopprimere la validazione di un form, si può usare l'evento ``POST_SUBMIT`` e impedire
che :class:`Symfony\\Component\\Form\\Extension\\Validator\\EventListener\\ValidationListener`
sia richiamato.

Una possibile ragione per farlo è che, pur avendo impostato ``group_validation``
a ``false``, ci sono alcune verifiche di integrità. Per esempio,
c'è una verifica che un file caricato non sia troppo grosso e il form
verificherà se siano stati inviati campi inesistenti. Per disabilitare
tutto ciò. usare un ascoltatore::

    use Symfony\Component\Form\FormBuilderInterface;
    use Symfony\Component\Form\FormEvents;
    use Symfony\Component\Form\FormEvent;

    public function buildForm(FormBuilderInterface $builder, array $options)
    {
        $builder->addEventListener(FormEvents::POST_SUBMIT, function (FormEvent $event) {
            $event->stopPropagation();
        }, 900); // Impostare sempre una priorità maggiore di ValidationListener

        // ...
    }

.. caution::

    In questo modo, si potrebbe disabilitare erroneamente qualcosa di più della
    sola validazione di form, perché l'evento ``POST_SUBMIT`` potrebbe avere altri ascoltatori.
