.. index::
   single: Form; Events

Come generare dinamicamente form usando gli eventi form
===================================================

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

    Se questa particolare sezione di codeice non ti è familiare,
    probabilmente è necessario tornare indietro e come prima cosa leggere :doc:`Forms chapter </book/forms>` 
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

Adding An Event Subscriber To A Form Class
------------------------------------------

So, instead of directly adding that "name" widget via our ProductType form 
class, let's delegate the responsibility of creating that particular field 
to an Event Subscriber::

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
            return 'product';
        }
    }

The event subscriber is passed the FormFactory object in its constructor so 
that our new subscriber is capable of creating the form widget once it is 
notified of the dispatched event during form creation.

.. _`cookbook-forms-inside-subscriber-class`:

Inside the Event Subscriber Class
---------------------------------

The goal is to create a "name" field *only* if the underlying Product object
is new (e.g. hasn't been persisted to the database). Based on that, the subscriber
might look like the following::

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
            // Tells the dispatcher that we want to listen on the form.pre_set_data
            // event and that the preSetData method should be called.
            return array(FormEvents::PRE_SET_DATA => 'preSetData');
        }

        public function preSetData(DataEvent $event)
        {
            $data = $event->getData();
            $form = $event->getForm();
            
            // During form creation setData() is called with null as an argument 
            // by the FormBuilder constructor. We're only concerned with when 
            // setData is called with an actual Entity object in it (whether new,
            // or fetched with Doctrine). This if statement let's us skip right 
            // over the null condition.
            if (null === $data) {
                return;
            }

            // check if the product object is "new"
            if (!$data->getId()) {
                $form->add($this->factory->createNamed('text', 'name'));
            }
        }
    }

.. caution::

    It is easy to misunderstand the purpose of the ``if (null === $data)`` segment 
    of this event subscriber. To fully understand its role, you might consider 
    also taking a look at the `Form class`_ and paying special attention to 
    where setData() is called at the end of the constructor, as well as the 
    setData() method itself.

The ``FormEvents::PRE_SET_DATA`` line actually resolves to the string ``form.pre_set_data``. 
The `FormEvents class`_ serves an organizational purpose. It is a centralized  location
in which you can find all of the various form events available.

While this example could have used the ``form.set_data`` event or even the ``form.post_set_data`` 
events just as effectively, by using ``form.pre_set_data`` we guarantee that 
the data being retrieved from the ``Event`` object has in no way been modified 
by any other subscribers or listeners. This is because ``form.pre_set_data`` 
passes a `DataEvent`_ object instead of the `FilterDataEvent`_ object passed 
by the ``form.set_data`` event. `DataEvent`_, unlike its child `FilterDataEvent`_, 
lacks a setData() method.

.. note::

    You may view the full list of form events via the `FormEvents class`_, 
    found in the form bundle.

.. _`DataEvent`: https://github.com/symfony/symfony/blob/master/src/Symfony/Component/Form/Event/DataEvent.php
.. _`FormEvents class`: https://github.com/symfony/Form/blob/master/FormEvents.php
.. _`Form class`: https://github.com/symfony/symfony/blob/master/src/Symfony/Component/Form/Form.php
.. _`FilterDataEvent`: https://github.com/symfony/symfony/blob/master/src/Symfony/Component/Form/Event/FilterDataEvent.php
