.. index::
   single: Form; Eventi

Come modificare dinamicamente form usando gli eventi
====================================================

Capita spesso che non un form non possa essere creato staticamente. In questa ricetta
vedremo come personalizzare un form, in base a tre casi d'uso.

1) :ref:`cookbook-form-events-underlying-data`

Eesempio: si ha un form "Product" e occorre modificare/aggiungere/rimuovere un campo,
in base ai dati sull'oggetto Product sottostante.

2) :ref:`cookbook-form-events-user-data`

Eesempio: si crea un form "Friend Message" e occorre costruire un menù a tendina,
che contenga solo utenti che sono amici con l'utente attualmente
autenticato.

3) :ref:`cookbook-form-events-submitted-data`

Eesempio: in un form di registrazione, si ha un campo "country" e un campo "state",
che va popolato automaticamente in base al valore del campo
"country".

.. _cookbook-form-events-underlying-data:

Personalizzare un form in base ai dati sottostanti
--------------------------------------------------

Prima di addentrarci nella generazione dinamica dei form, diamo un'occhiata veloce 
alla classe dei form::

    // src/Acme/DemoBundle/Form/Type/ProductType.php
    namespace Acme\DemoBundle\Form\Type;

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
                'data_class' => 'Acme\DemoBundle\Entity\Product'
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

Si assuma per un momento che questo form utilizzi una classe immaginaria "Product"
questa ha solo due attributi rilevanti ("name" e "price"). Il form generato 
da questa classe avrà lo stesso aspetto, indipendentemente se sta per essere creato un nuovo prodotto
oppure se sta per essere modificato un prodotto esistente (cioè un prodotto ottenuto da base dati).

Si supponga ora, di non voler abilitare l'utente alla modifica del campo ``name``
una volta che l'oggetto è stato creato. Lo si può fare grazie al componente
:doc:`Event Dispatcher </components/event_dispatcher/introduction>`,
che analizza l'oggetto e modifica il form basato sull'
oggetto Product. In questa ricetta si imparerà come aggiungere questo livello di
flessibilità ai form.

.. _`cookbook-forms-event-subscriber`:

Aggiungere un evento sottoscrittore alla classe di un form
----------------------------------------------------------

Invece di aggiungere direttamente il widget "name" tramite la  classe dei form ProductType 
si deleghi la responsabilità di creare questo particolare campo
a un evento sottoscrittore::

    // src/Acme/DemoBundle/Form/Type/ProductType.php
    namespace Acme\DemoBundle\Form\Type;

    // ...
    use Acme\DemoBundle\Form\EventListener\AddNameFieldSubscriber;

    class ProductType extends AbstractType
    {
        public function buildForm(FormBuilderInterface $builder, array $options)
        {
            $builder->add('price');

            $builder->addEventSubscriber(new AddNameFieldSubscriber());
        }

        // ...
    }

.. _`cookbook-forms-inside-subscriber-class`:

Dentro la classe dell'evento sottoscrittore
-------------------------------------------

L'obiettivo è di creare un campo "name" *solo* se l'oggetto Prodotto sottostante
è nuovo (cioè non è stato persistito nella base dati). Basandosi su questo, l'sottoscrittore
potrebbe essere simile a questo:

.. versionadded:: 2.2
    La possibilità di passare una stringa in :method:`FormInterface::add<Symfony\\Component\\Form\\FormInterface::add>`
    è stata aggiunta in Symfony 2.2.

.. code-block:: php

    // src/Acme/DemoBundle/Form/EventListener/AddNameFieldSubscriber.php
    namespace Acme\DemoBundle\Form\EventListener;

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
            $data = $event->getData();
            $form = $event->getForm();

            // verifica se l'oggetto product sia "nuovo"
            // Se non si passano dati al form, $data è "null".
            // Questo va considerato un nuovo "Product"
            if (!$data || !$data->getId()) {
                $form->add('name', 'text');
            }
        }
    }

.. tip::

    La riga ``FormEvents::PRE_SET_DATA`` viene risolta in
    ``form.pre_set_data``. :class:`Symfony\\Component\\Form\\FormEvents` ha uno scopo
    organizzativo. È un posto centralizzato in cui si possono trovare
    tutti i vari eventi disponibili per i form.

.. note::

    La lista completa degli eventi dei form è nella classe :class:`Symfony\\Component\\Form\\FormEvents`.


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

    // src/Acme/DemoBundle/Form/Type/FriendMessageFormType.php
    namespace Acme\DemoBundle\Form\Type;

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
            $builder->addEventListener(FormEvents::PRE_SET_DATA, function(FormEvent $event){
                // ... aggiungere una lista di amici dell'utente attuale
            });
        }

        public function getName()
        {
            return 'acme_friend_message';
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

    // src/Acme/DemoBundle/FormType/FriendMessageFormType.php

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
                function(FormEvent $event) use ($user) {
                    $form = $event->getForm();

                    $formOptions = array(
                        'class' => 'Acme\DemoBundle\Entity\User',
                        'property' => 'fullName',
                        'query_builder' => function(EntityRepository $er) use ($user) {
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

Per definire il form come servizio, creare un normale serizio e aggiungere il tag
:ref:`dic-tags-form-type`.

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        services:
            acme.form.friend_message:
                class: Acme\DemoBundle\Form\Type\FriendMessageFormType
                arguments: [@security.context]
                tags:
                    -
                        name: form.type
                        alias: acme_friend_message

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <services>
            <service id="acme.form.friend_message" class="Acme\DemoBundle\Form\Type\FriendMessageFormType">
                <argument type="service" id="security.context" />
                <tag name="form.type" alias="acme_friend_message" />
            </service>
        </services>

    .. code-block:: php

        // app/config/config.php
        $definition = new Definition('Acme\DemoBundle\Form\Type\FriendMessageFormType');
        $definition->addTag('form.type', array('alias' => 'acme_friend_message'));
        $container->setDefinition(
            'acme.form.friend_message',
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
            $form = $this->get('form.factory')->create('acme_friend_message');

            // ...
        }
    }

Se si estende la classe ``Symfony\Bundle\FrameworkBundle\Controller\Controller``, basta chiamare::

    $form = $this->createForm('acme_friend_message');

Si può anche includere il form type in un altro form::

    // dentro un'altra classe "form type"
    public function buildForm(FormBuilderInterface $builder, array $options)
    {
        $builder->add('message', 'acme_friend_message');
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

    // src/Acme/DemoBundle/Form/Type/SportMeetupType.php
    namespace Acme\DemoBundle\Form\Type;

    use Symfony\Component\Form\FormBuilderInterface;
    use Symfony\Component\Form\FormEvent;
    use Symfony\Component\Form\FormEvents;
    // ...

    class SportMeetupType extends AbstractType
    {
        public function buildForm(FormBuilderInterface $builder, array $options)
        {
            $builder
                ->add('sport', 'entity', array(...))
            ;

            $builder->addEventListener(
                FormEvents::PRE_SET_DATA,
                function(FormEvent $event) {
                    $form = $event->getForm();

                    // questa sarà l'entità, p.e. SportMeetup
                    $data = $event->getData();

                    $positions = $data->getSport()->getAvailablePositions();

                    $form->add('position', 'entity', array('choices' => $positions));
                }
            );
        }
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
    Il comportamento dell'evento ``POST_SUBMIT`` è cambiato leggermento in 2.2.6, usato
    dall'esempio seguente.

La chiave sta nell'aggiungere un ascoltatore ``POST_SUBMIT`` al campo da cui dipende il nuovo
campo. Se si aggiunge un ascoltatore ``POST_SUBMIT`` a un form figlio (p.e. ``sport``),
e si aggiungono nuovi figli al form genitore, il componente Form individuerà il
nuovo campo automaticamente e lo mapperà ai dati inviati dal client.

La classe ora sarà così::

    // src/Acme/DemoBundle/Form/Type/SportMeetupType.php
    namespace Acme\DemoBundle\Form\Type;

    // ...
    use Acme\DemoBundle\Entity\Sport;
    use Symfony\Component\Form\FormInterface;

    class SportMeetupType extends AbstractType
    {
        public function buildForm(FormBuilderInterface $builder, array $options)
        {
            $builder
                ->add('sport', 'entity', array(...))
            ;

            $formModifier = function(FormInterface $form, Sport $sport) {
                $positions = $sport->getAvailablePositions();

                $form->add('position', 'entity', array('choices' => $positions));
            };

            $builder->addEventListener(
                FormEvents::PRE_SET_DATA,
                function(FormEvent $event) use ($formModifier) {
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
    }

Si può vedere come occorra scoltare questi due eventi e avere callback diversi,
solo perché in due scenari diversi i dati che si possono usare vengono restituiti in eventi diversi.
Oltre a questo, gli ascoltatori eseguono esattamente le stesse cose su un form dato.

Un pezzo ancora mancante è l'aggiornamento lato client del form, dopo la
scelta dello sport. Lo si può gestire tramite una chiamata AJAX
all'applicazione. Nel controllore, si può eseguire il bind del form e,
invece di processarlo, usare semplicemente il form per rendere i campi
aggiornati. La risposta della chiamata AJAX può quindi essere usata per aggiornare la vista.
