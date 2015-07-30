Form
====

I form sono uno dei componenti più abusati di Symfony; questo è dovuto sia al suo vasto campo
di applicazione sia alla sua lista infinita di funzionalità. In questo capitolo,
mostreremo alcune best practices in modo da poterli sfruttare al meglio.

Creazione dei form
------------------

.. best-practice::

    Definire i form come classi PHP.

Il componente `Form` consente di creare form direttamente dal controllore.
A meno che non si voglia riusare il form da qualche altra parte, quest'abitudine
non è del tutto sbagliata. Nonostante ciò, per form più complessi da poter riutilizzare in altri controllori
si raccomanda di definire ogni form nella propria classe PHP::

    namespace AppBundle\Form;

    use Symfony\Component\Form\AbstractType;
    use Symfony\Component\Form\FormBuilderInterface;
    use Symfony\Component\OptionsResolver\OptionsResolverInterface;

    class PostType extends AbstractType
    {
        public function buildForm(FormBuilderInterface $builder, array $options)
        {
            $builder
                ->add('title')
                ->add('summary', 'textarea')
                ->add('content', 'textarea')
                ->add('authorEmail', 'email')
                ->add('publishedAt', 'datetime')
            ;
        }

        public function setDefaultOptions(OptionsResolverInterface $resolver)
        {
            $resolver->setDefaults(array(
                'data_class' => 'AppBundle\Entity\Post'
            ));
        }

        public function getName()
        {
            return 'post';
        }
    }

Per creare il form, chiamare il metodo `createForm` e instanziare la nuova classe::

    use AppBundle\Form\PostType;
    // ...

    public function newAction(Request $request)
    {
        $post = new Post();
        $form = $this->createForm(new PostType(), $post);

        // ...
    }

Registrazione dei form come servizi
-----------------------------------

È possibile
:ref:`registrare i tipi di form come servizi <form-cookbook-form-field-service>`,
anche se non si consiglia di farlo a meno che non si pianifichi di riusare lo stesso
form in altri posti o di incorporarlo all'interno di altri form, usando il
:doc:`tipo collection </reference/forms/types/collection>`.

Per la maggior parte dei casi in cui il form viene usato solo per modifica o creazione, la
registrazione come servizio è eccessiva e rende più difficile capire esattamente quale classe
sia usata nel controllore.

Configurazione dei bottoni
--------------------------

Le classe dei form dovrebbe essere agnostica rispetto a *dove* viene utilizzata. In questo modo,
i form possono essere riusati più facilmente.

.. best-practice::

    Aggiungere i bottoni nel template, non nelle classi dei form o nei controllori.

A partire da Symfony 2.5 è possibile aggiungere campi button all'interno del form.
Il vantaggio è che si semplifica il codice del template che visualizza il form.
Lo svantaggio è che aggiungere il bottone alla classe form ne limita la sua riusabilità.

.. code-block:: php

    class PostType extends AbstractType
    {
        public function buildForm(FormBuilderInterface $builder, array $options)
        {
            $builder
                // ...
                ->add('save', 'submit', array('label' => 'Create Post'))
            ;
        }

        // ...
    }

Questo form *potrebbe* essere stato progettato per la creazione di post, ma, se si volesse
riusarlo anche per la modifica, la label del bottone sarebbe sbagliata.
Alcuni sviluppatori configurano invece i bottoni del form nel controllore::

    namespace AppBundle\Controller\Admin;

    use Symfony\Component\HttpFoundation\Request;
    use Symfony\Bundle\FrameworkBundle\Controller\Controller;
    use AppBundle\Entity\Post;
    use AppBundle\Form\PostType;

    class PostController extends Controller
    {
        // ...

        public function newAction(Request $request)
        {
            $post = new Post();
            $form = $this->createForm(new PostType(), $post);
            $form->add('submit', 'submit', array(
                'label' => 'Crea',
                'attr'  => array('class' => 'btn btn-default pull-right')
            ));

            // ...
        }
    }

Anche questa soluzione è sbagliata, perché si sta mischiando codice markup relativo
alla presentazione (etichette, classi CSS, ecc.) con codice PHP. La separazione delle
competenze è una buona regola da seguire sempre, quindi tutto ciò che è relativo alla vista
deve essere messo nel livello della vista:

.. code-block:: html+jinja

    {{ form_start(form) }}
        {{ form_widget(form) }}

        <input type="submit" value="Create"
               class="btn btn-default pull-right" />
    {{ form_end(form) }}

Rendere il Form
---------------

Symfony mette a disposizione diversi modi per rendere un form, dal
rendere tutto il form con un unico comando al rendere ogni singolo campo in modo indipendente.
Il modo migliore dipende dalla quantità di personalizzazione necessaria nel form.

Il modo più semplice, utile specialmente durante lo sviluppo, è la
funzione ``form_widget()`` per rendere tutti i campi
insieme:

.. code-block:: html+jinja

    {{ form_start(form, {'attr': {'class': 'my-form-class'} }) }}
        {{ form_widget(form) }}
    {{ form_end(form) }}

Se si ha bisogno di un controllo più preciso sulla renderizzazione del form
non usare la funzione ``form_widget(form)`` e rendere i campi individualmente.
Consultare la ricetta :doc:`/cookbook/form/form_customization`
per maggiori informazioni su come rendere i form e su come impostare un tema
in modo globale.

Gestire l'invio
---------------

La gestione di un form in Symonfy generalmente segue la seguente struttura:

.. code-block:: php

    public function newAction(Request $request)
    {
        // costruire il form ...

        $form->handleRequest($request);

        if ($form->isSubmitted() && $form->isValid()) {
            $em = $this->getDoctrine()->getManager();
            $em->persist($post);
            $em->flush();

            return $this->redirect($this->generateUrl(
                'admin_post_show',
                array('id' => $post->getId())
            ));
        }

        // rendere il template
    }

Nel codice precedente è importante evidenziare due cose. In primo luogo, si
raccomanda di usare un'unica azione sia per rendere il form che per la gestione dell'invio.
Per esempio, si potrebbe avere ``newAction`` *solo* per rendere il form
e ``createAction`` *solo* per gestire l'invio. Entrambe le azioni però sono quasi identiche,
quindi è più semplice lasciare che sia ``newAction`` a gestire il
tutto.

In secondo luogo si raccomand di usare ``$form->isSubmitted()`` nel costrutto ``if``,
per rendere il codice più chiaro. Tecnicamente non è necessario, dato che ``isValid()``  esegue prima
``isSubmitted()``. Senza questo, tuttavia, il flusso risulterebbe un po' strano
e il form sembrerebbe *sempre* processato, anche per le richieste GET.

TIpi di campo personalizzati
----------------------------

.. best-practice::

    Aggiungere il prefisso ``app_`` ai campi personalizzati, per evitare collisioni.

I tipi di campo personalizzati ereditano dalla classe ``AbstractType``, che definisce il metodo
``getName()`` per configurare il nome del tipo. Tali nomi devono essere univoci
nell'applicazione.

Se un tipo personalizzato usa lo stesso nome di uno dei tipi di Symfony,
lo sovrascriverà. Lo stesso accade quando il tipo personalizzato corrisponde
a un qualsiasi tipo definito da bundle di terze parti installati nell'applicazione.

Aggiungere il prefisso ``app_`` ai campi personalizzati, per evitare collisioni
che potrebbero portare a errori.
