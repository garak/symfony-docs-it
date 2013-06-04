.. index::
   single: Form; Eventi

Come modificare dinamicamente form usando gli eventi
====================================================

Prima di addentrarci nella generazione dinamica dei form, diamo un'occhiata veloce 
alla classe dei form::

    // src/Acme/DemoBundle/Form/Type/ProductType.php
    namespace Acme\DemoBundle\Form\Type;

    use Symfony\Component\Form\AbstractType;
    use Symfony\Component\Form\FormBuilderInterface;

    class ProductType extends AbstractType
    {
        public function buildForm(FormBuilderInterface $builder, array $options)
        {
            $builder->add('nome');
            $builder->add('prezzo');
        }

        public function getName()
        {
            return 'prodotto';
        }
    }

.. note::

    Se questa particolare sezione di codice non è familiare,
    probabilmente è necessario tornare indietro e come prima cosa leggere il :doc:`capitolo sui form</book/forms>` 
    prima di andare avanti.

Si assuma per un momento che questo form utilizzi una classe immaginaria "prodotto"
questa ha solo due attributi rilevanti ("nome" e "prezzo"). Il form generato 
da questa classe avrà lo stesso aspetto, indipendentemente se sta per essere creato un nuovo prodotto
oppure se sta per essere modificato un prodotto esistente (cioè un prodotto ottenuto da base dati).

Si supponga ora, di non voler abilitare l'utente alla modifica del campo 'nome' 
una volta che l'oggetto è stato creato. Lo si può fare grazie al componente
:doc:`Event Dispatcher </components/event_dispatcher/introduction>`,
che analizza l'oggetto e modifica il form basato sull'
oggetto prodotto. In questa ricetta si imparerà come aggiungere questo livello di
flessibilità ai form.

.. _`cookbook-forms-event-subscriber`:

Aggiungere un evento sottoscrittore alla classe di un form
----------------------------------------------------------

Invece di aggiungere direttamente il widget "nome" tramite la  classe dei form ProductType 
si deleghi la responsabilità di creare questo particolare campo
a un evento sottoscrittore::

    // src/Acme/DemoBundle/Form/Type/ProductType.php
    namespace Acme\DemoBundle\Form\Type;

    use Symfony\Component\Form\AbstractType;
    use Symfony\Component\Form\FormBuilderInterface;
    use Acme\DemoBundle\Form\EventListener\AddNameFieldSubscriber;

    class ProductType extends AbstractType
    {
        public function buildForm(FormBuilderInterface $builder, array $options)
        {
            $builder->add('price');

            $builder->addEventSubscriber(new AddNameFieldSubscriber());
        }

        public function getName()
        {
            return 'prodotto';
        }
    }

.. _`cookbook-forms-inside-subscriber-class`:

Dentro la classe dell'evento sottoscrittore
-------------------------------------------

L'obiettivo è di creare un campo "nome" *solo* se l'oggetto Prodotto sottostante
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

            // Durante la creazione del form, setData è chiamata con parametri null
            // dal costruttore di FormBuilder. Si è interessati a quando 
            // setData è invocato con l'oggetto Entity attuale (se è nuovo,
            // oppure recuperato con Doctrine). Bisognerà uscire dal metodo 
            // se la condizione restituisce null.
            if (null === $data) {
                return;
            }

            // controlla se l'oggetto Prodotto è nuovo
            if (!$data->getId()) {
                $form->add('nome', 'text');
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

Anche se in questo esempio si potrebbe utilizzare l'evento ``form.post_set_data``,
utilizzando ``form.pre_set_data`` si garantisce che 
i dati saranno ottenuti dall'oggetto ``Event`` che non è stato modificato
da nessun altro sottoscrittore o ascoltatore, perché ``form.pre_set_data`` è
il primo evento distribuito.

.. note::

    È possibile consultare la lista completa degli eventi del form tramite la `classe FormEvents`_, 
    nel bundle dei form.

.. _`classe FormEvents`: https://github.com/symfony/Form/blob/master/FormEvents.php
.. _`classe Form`: https://github.com/symfony/symfony/blob/master/src/Symfony/Component/Form/Form.php
