.. index::
   single: Form; Unire una collezione di form

Come unire una collezione di form
==================================

Con questa ricetta si apprenderà come creare un form che unisce una collezione
di altri form. Ciò può essere utile, ad esempio, se si ha una classe ``Task``
e si vuole modificare/creare/cancellare oggetti ``Tag`` connessi a
questo Task, all'interno dello stesso form.

.. note::

    Con questa ricetta, si assume di utilizzare Doctrine come
    ORM. Se non si utilizza Doctrine (es. Propel o semplicemente
    una connessione a database), il tutto è pressapoco simile.
    
    Se si utilizza Doctrine, si avrà la necessità di aggiungere meta-dati Doctrine,
    includendo una relazione ``ManyToMany`` sulla colonna ``tags`` di Task.

Iniziamo: supponiamo che ogni ``Task`` appartiene a più oggetti ``Tags``.
Si crei una semplice classe ``Task``::

    // src/Acme/TaskBundle/Entity/Task.php
    namespace Acme\TaskBundle\Entity;
    
    use Doctrine\Common\Collections\ArrayCollection;

    class Task
    {
        protected $description;

        protected $tags;

        public function __construct()
        {
            $this->tags = new ArrayCollection();
        }
        
        public function getDescription()
        {
            return $this->description;
        }

        public function setDescription($description)
        {
            $this->description = $description;
        }

        public function getTags()
        {
            return $this->tags;
        }

        public function setTags(ArrayCollection $tags)
        {
            $this->tags = $tags;
        }
    }

.. note::

    ``ArrayCollection`` è specifica per Doctrine ed è fondamentalmente la
    stessa cosa di utilizzare un ``array`` (ma deve essere un ``ArrayCollection``) se
    si utilizza Doctrine.

Ora, si crei una classe ``Tag``. Come è possibile verificare, un ``Task`` può avere più oggetti
``Tag``::

    // src/Acme/TaskBundle/Entity/Tag.php
    namespace Acme\TaskBundle\Entity;

    class Tag
    {
        public $name;
    }

.. tip::

    La proprietà ``name`` qui è pubblica, ma può essere facilmente protetta
    o privata (ma in questo caso si avrebbe bisogno dei metodi ``getName`` e ``setName``).

Si crei ora una classe di form cosicché un oggetto ``Tag``
può essere modificato dall'utente::

    // src/Acme/TaskBundle/Form/Type/TagType.php
    namespace Acme\TaskBundle\Form\Type;

    use Symfony\Component\Form\AbstractType;
    use Symfony\Component\Form\FormBuilder;

    class TagType extends AbstractType
    {
        public function buildForm(FormBuilder $builder, array $options)
        {
            $builder->add('name');
        }

        public function getDefaultOptions(array $options)
        {
            return array(
                'data_class' => 'Acme\TaskBundle\Entity\Tag',
            );
        }

        public function getName()
        {
            return 'tag';
        }
    }

Questo è sufficiente per rendere un form tag. Ma dal momento che l'obiettivo
finale è permettere la modifica dei tag di un task nello stesso form 
del task, bisogna creare un form per la classe ``Task``.

Da notare che si unisce una collezione di form ``TagType`` utilizzando
il tipo di campo :doc:`collection</reference/forms/types/collection>`::

    // src/Acme/TaskBundle/Form/Type/TaskType.php
    namespace Acme\TaskBundle\Form\Type;

    use Symfony\Component\Form\AbstractType;
    use Symfony\Component\Form\FormBuilder;

    class TaskType extends AbstractType
    {
        public function buildForm(FormBuilder $builder, array $options)
        {
            $builder->add('description');

            $builder->add('tags', 'collection', array('type' => new TagType()));
        }

        public function getDefaultOptions(array $options)
        {
            return array(
                'data_class' => 'Acme\TaskBundle\Entity\Task',
            );
        }

        public function getName()
        {
            return 'task';
        }
    }

Nel controllore, è possibile inizializzare una nuova istanza di ``TaskType``::

    // src/Acme/TaskBundle/Controller/TaskController.php
    namespace Acme\TaskBundle\Controller;
    
    use Acme\TaskBundle\Entity\Task;
    use Acme\TaskBundle\Entity\Tag;
    use Acme\TaskBundle\Form\TaskType;
    use Symfony\Component\HttpFoundation\Request;
    use Symfony\Bundle\FrameworkBundle\Controller\Controller;
    
    class TaskController extends Controller
    {
        public function newAction(Request $request)
        {
            $task = new Task();
            
            // codice fittizio - è qui solo perché il Task ha alcuni tag
            // altrimenti, questo non è un esempio interessante
            $tag1 = new Tag()
            $tag1->name = 'tag1';
            $task->getTags()->add($tag1);
            $tag2 = new Tag()
            $tag2->name = 'tag2';
            $task->getTags()->add($tag2);
            // fine del codice fittizio
            
            $form = $this->createForm(new TaskType(), $task);
            
            // fare qualche processo del form qui, in una richiesta POST
            
            return $this->render('AcmeTaskBundle:Task:new.html.twig', array(
                'form' => $form->createView(),
            ));
        }
    }

Il template corrispondente ora è abilitato a rendere entrambi i campi ``description``
per il form dei task, oltre tutti i form ``TagType``
che sono relazionati a questo ``Task``. Nel controllore sottostante, viene aggiunto
del codice fittizio così da poterlo vedere in azione (dato che un ``Task`` non
ha tags appena viene creato).

.. configuration-block::

    .. code-block:: html+jinja

        {# src/Acme/TaskBundle/Resources/views/Task/new.html.twig #}
        {# ... #}

        {# rende solo il campo: description #}
        {{ form_row(form.description) }}

        <h3>Tags</h3>
        <ul class="tags">
            {# itera per ogni tag esistente e rende solo il campo: nome #}
			{% for tag in form.tags %}
            	<li>{{ form_row(tag.name) }}</li>
			{% endfor %}
        </ul>

        {{ form_rest(form) }}
        {# ... #}

    .. code-block:: html+php

        <!-- src/Acme/TaskBundle/Resources/views/Task/new.html.php -->
        <!-- ... -->

        <h3>Tags</h3>
        <ul class="tags">
			<?php foreach($form['tags'] as $tag): ?>
            	<li><?php echo $view['form']->row($tag['name']) ?></li>
			<?php endforeach; ?>
        </ul>

        <?php echo $view['form']->rest($form) ?>
        <!-- ... -->

Quando l'utente invia il form, i dati inviati per i campi di ``Tags``
sono utilizzato per costruire un ArrayCollection di oggetti ``Tag``,che viene poi
impostato sul campo ``tag`` dell'istanza ``Task``.

La collezione ``Tags``è acessibile tramite ``$task->getTags()``
e può essere persistita nel database oppure utilizzata dove se ne ha bisogno.

Finora, tutto ciò funziona bene, ma questo non permette di aggiungere nuovi dinamicamente 
todo o eliminare todo esistenti. Quindi, la modifica dei todo esistenti funziona 
bene ma ancora non si possono aggiungere nuovi todo.

.. _cookbook-form-collections-new-prototype:

Permettere "nuovi" todo con "prototipo"
---------------------------------------

Permettere all'utente di inserire dinamicamente nuovi todo significa che abbiamo la necessità di
utilizzare Javascript. Precedentemente sono stati aggiunti due tags al nostro form nel controllore.
Ora si ha la necessità che l'utente possa aggiungere diversi form di tag secondo le sue necessità direttamente dal browser.
Questo può essere fatto attraverso un po' di Javascript.

La prima cosa di cui si ha bisogno è di far capire alla collezione di form che
riceverà un numero indeterminato di tag. Finora sono stati aggiunti due tag e il form
si aspetta di riceverne esattamente due, altrimenti verrà lanciato un errore:
``Questo form non può contenere campi extra``. Per rendere flessibile il form,
bisognerà aggiungere l'opzione ``allow_add`` alla collezione di campi::

    // ...
    
    public function buildForm(FormBuilder $builder, array $options)
    {
        $builder->add('description');

        $builder->add('tags', 'collection', array(
            'type' => new TagType(),
            'allow_add' => true,
            'by_reference' => false,
        ));
    }

Da notare che è stata aggiunto  ``'by_reference' => false``. Questo perché
non si sta inviando una referenza ad un tag esistente ma piuttosto si sta creando
un nuovo tag quando si salva insieme il todo e i suoi tag.

L'opzione ``allow_add`` effettua anche un'altra cosa. Aggiunge la proprietà ``data-prototype``
al ``div`` che contiene la collezione del tag. Questa proprietà
contiene html da aggiungere all'elemento Tag nella pagina, come il seguente esempio:

.. code-block:: html

    <div data-prototype="&lt;div&gt;&lt;label class=&quot; required&quot;&gt;$$name$$&lt;/label&gt;&lt;div id=&quot;khepin_productbundle_producttype_tags_$$name$$&quot;&gt;&lt;div&gt;&lt;label for=&quot;khepin_productbundle_producttype_tags_$$name$$_name&quot; class=&quot; required&quot;&gt;Name&lt;/label&gt;&lt;input type=&quot;text&quot; id=&quot;khepin_productbundle_producttype_tags_$$name$$_name&quot; name=&quot;khepin_productbundle_producttype[tags][$$name$$][name]&quot; required=&quot;required&quot; maxlength=&quot;255&quot; /&gt;&lt;/div&gt;&lt;/div&gt;&lt;/div&gt;" id="khepin_productbundle_producttype_tags">
    </div>

Sarà, quindi, possibile ottenere questa proprietà da Javascript ed utilizzarla per visualizzare
U nuovo form di Tag. Per rendere le cose semplici, verrà incorporato jQuery nella pagina 
dato che permette la manipolazione della pagina in modalità cross-browser..

Come prima cosa, si aggiunga un ``nuovo`` form con la classe ``add_tag_link``.
Ogni volta che viene cliccato dall'utente, verrà aggiunto un tag vuoto:

.. code-block:: javascript

    $('.record_action').append('<li><a href="#" class="add_tag_link">Add a tag</a></li>');

Inoltre, bisognerà includere un template che contenga il codice Javascript necessario per aggiungere gli elementi
del form quando il link verrà premuto..

.. note:

    È meglio separare Javascript in un file a sé stante piuttosto che
    includerlo nell'HTML come viene fatto qui.

Il codice può essere semplice:

.. code-block:: javascript

    function addTagForm() {
        // Ottieni il div che detiene la collezione di tag
        var collectionHolder = $('#task_tags');
        // prendi il data-prototype 
        var prototype = collectionHolder.attr('data-prototype');
        // Sostituisci '$$name$$' nell'html del prototype in the prototype's HTML 
        // affiché sia un nummero basato sulla lunghezza corrente della collezione.
        form = prototype.replace(/\$\$name\$\$/g, collectionHolder.children().length);
        // Visualizza il form nella pagina
        collectionHolder.append(form);
    }
    // Aggiungi il link per aggiungere ulteriori tag
    $('.record_action').append('<li><a href="#" class="add_tag_link">Aggiungi un tag</a></li>');
    // Quando il link viene premuto aggiunge un campo per immettere un nuovo tag
    $('a.jslink').click(function(event){
        addTagForm();
    });

Ora, ogni volta che un utente clicca sul link  ``Aggiungi un tag``, apparirà un nuovo
form nella pagina. Il form lato server  è consapevole di tutto e non
si aspetterà nessuna specifica dimensione per la collezione ``Tag``. Tutti i tag
verranno aggiunti creando un nuovo ``Todo`` salvandolo insieme a esso.

Per ulteriori dettagli, guarda :doc:`collection form type reference</reference/forms/types/collection>`.

.. _cookbook-form-collections-remove:

Permettere la rimozione di todo
-------------------------------

Questa sezione non è ancora stata scritta, ma lo sarà presto. Se si è interessati
a scrivere questa sezione, si guardi :doc:`/contributing/documentation/overview`.
