.. index::
   single: Form; Eventi

Come generare dinamicamente form usando gli eventi form
=======================================================

Prima di addentrarci nella generazione dinamica dei form, diamo un'occhiata veloce 
alla classe dei form::

    //src/Acme/DemoBundle/Form/ProductType.php
    namespace Acme\DemoBundle\Form

    use Symfony\Component\Form\AbstractType
    use Symfony\Component\Form\FormBuilder;
    
    class ProductType extends AbstractType
    {
        public function buildForm(FormBuilder $builder, array $options)
        {
            $builder->add('nome');
            $builder->add('prezzo');
        }

        public function getName()
        {
            return 'prodotto';
        }
    }

.. nota::

    Se questa particolare sezione di codice non è familiare,
    probabilmente è necessario tornare indietro e come prima cosa leggere il :doc:`capitolo sui form</book/forms>` 
    prima di andare avanti.

Si assuma per un momento che questo form utilizzi una classe immaginaria "prodotto"
questa ha solo due attributi rilevanti ("nome" e "prezzo"). Il form generato 
da questa classe avrà lo stesso aspetto, indipendentemente se un nuovo prodotto sta per essere creato
oppure se un prodotto esistente sta per essere modificato (es. un prodotto ottenuto da database).

Si supponga ora, di non voler abilitare l'utente alla modifica del campo 'nome' 
una volta che l'oggetto è stato creato. Per fare ciò si può dare un'occhiata al :ref:`Event Dispatcher <book-internals-event-dispatcher>` 
sistema che analizza l'oggetto e modifica il form basato sull'
oggetto 'prodotto'. In questa voce, si imparerà come aggiungere questo livello di
flessibilità ai form.

.. _`cookbook-forms-event-subscriber`:

Aggiungere un evento sottoscrittore alla classe di un form
----------------------------------------------------------

Invece di aggiungere direttamente il widget "nome" tramite la  classe dei form ProductType 
si deleghi la responsabilità di creare questo particolare campo
ad un evento sottoscrittore::

    //src/Acme/DemoBundle/Form/ProductType.php
    namespace Acme\DemoBundle\Form

    use Symfony\Component\Form\AbstractType
    use Symfony\Component\Form\FormBuilder;
    use Acme\DemoBundle\Form\EventListener\AddNameFieldSubscriber;

    class ProductType extends AbstractType
    {
        public function buildForm(FormBuilder $builder, array $options)
        {
            $subscriber = new AddNameFieldSubscriber($builder->getFormFactory());
            $builder->addEventSubscriber($subscriber);
            $builder->add('price');
        }

        public function getName()
        {
            return 'prodotto';
        }
    }

L'evento sottoscrittore è passato dall'oggetto FormFactory nel suo costruttore, quindi
il nuovo sottoscrittore è in grado di creare il widget del form una volta che 
viene notificata dall'evento inviato durante la creazione del form.

.. _`cookbook-forms-inside-subscriber-class`:

Dentro la classe dell'evento sottoscrittore
-------------------------------------------

L'obiettivo è di creare un campo "nome" *solo* se l'oggetto Prodotto sottostante
è nuovo (es. non è stato persistito nel database). Basandosi su questo, l'sottoscrittore
potrebbe essere simile a questo::

    // src/Acme/DemoBundle/Form/EventListener/AddNameFieldSubscriber.php
    namespace Acme\DemoBundle\Form\EventListener;

    use Symfony\Component\Form\Event\DataEvent;
    use Symfony\Component\Form\FormFactoryInterface;
    use Symfony\Component\EventDispatcher\EventSubscriberInterface;
    use Symfony\Component\Form\FormEvents;

    class AddNameFieldSubscriber implements EventSubscriberInterface
    {
        private $factory;
        
        public function __construct(FormFactoryInterface $factory)
        {
            $this->factory = $factory;
        }
        
        public static function getSubscribedEvents()
        {
            // Indica al dispacher che si vuole ascoltare l'evento form.pre_set_data
            // e che verrà invocato il metodo preSetData.
            return array(FormEvents::PRE_SET_DATA => 'preSetData');
        }

        public function preSetData(DataEvent $event)
        {
            $data = $event->getData();
            $form = $event->getForm();
            
            // Dutante la creazione del form, setData è chiamata con parametri null
            // dal costruttore di FormBuilder. Si è interessati a quando 
            // setData è invocato con l'oggetto Entity attuale (se è nuovo,
            // oppure recuperato con Doctrine). Bisognerà uscire dal metoro 
            // se la condizione restituisce null.
            if (null === $data) {
                return;
            }

            // controlla se l'oggetto Prodotto è nuovo
            if (!$data->getId()) {
                $form->add($this->factory->createNamed('text', 'name'));
            }
        }
    }

.. caution::

    È facile fraintendere lo scopo dell'istruzione ``if (null === $data)``  
    dell'evento sottoscrittore. Per comprendere appieno il suo ruolo, bisogna 
    dare uno sguardo alla `classe Form`_ e prestare attenzione a  
    dove setData() è invocato alla fine del costruttore, nonché
    al metodo setData() stesso.

La riga ``FormEvents::PRE_SET_DATA`` viene attualmente risolta nella stringa ``form.pre_set_data``. 
La `classe FormEvents`_ ha uno scopo organizzativo. Ha una posizione centralizzata
in quello che si può trovare tra i diversi eventi dei form disponibili.

Anche se in questo esempio si potrebbe utilizzare l'evento ``form.set_data`` o anche l'evento ``form.post_set_data``,
utilizzando ``form.pre_set_data`` si garantisce che 
i dati saranno ottenuti dall'oggetto ``Event`` che non è stato modificato
da nessun altro sottoscrittore o ascoltatore. Questo perché ``form.pre_set_data`` 
passa all'oggetto `DataEvent`_ invece dell'oggetto `FilterDataEvent`_ passato dall'evento
``form.set_data``. `DataEvent`_, a differenza del suo figlio `FilterDataEvent`_, 
non ha il metodo setData().

.. note::

    È possibile consultare la lista completa degli eventi del form tramite la `classe FormEvents`_, 
    nel bundle dei form.

.. _`DataEvent`: https://github.com/symfony/symfony/blob/master/src/Symfony/Component/Form/Event/DataEvent.php
.. _`classe FormEvents`: https://github.com/symfony/Form/blob/master/FormEvents.php
.. _`classe Form`: https://github.com/symfony/symfony/blob/master/src/Symfony/Component/Form/Form.php
.. _`FilterDataEvent`: https://github.com/symfony/symfony/blob/master/src/Symfony/Component/Form/Event/FilterDataEvent.php
