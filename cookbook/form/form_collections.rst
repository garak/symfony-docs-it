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
    una connessione a database), il tutto è pressapoco simile. Ci sono solo alcune parti
    di questa guida che si occupano effettivamente di "persistenza".
    
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
    use Acme\TaskBundle\Form\Type\TaskType;
    use Symfony\Component\HttpFoundation\Request;
    use Symfony\Bundle\FrameworkBundle\Controller\Controller;
    
    class TaskController extends Controller
    {
        public function newAction(Request $request)
        {
            $task = new Task();
            
            // codice fittizio: è qui solo perché il Task ha alcuni tag
            // altrimenti, questo non è un esempio interessante
            $tag1 = new Tag()
            $tag1->name = 'tag1';
            $task->getTags()->add($tag1);
            $tag2 = new Tag()
            $tag2->name = 'tag2';
            $task->getTags()->add($tag2);
            // fine del codice fittizio
            
            $form = $this->createForm(new TaskType(), $task);
            
            // processare il form, in una richiesta POST
            if ('POST' === $request->getMethod()) {
                $form->bindRequest($request);
                if ($form->isValid()) {
                    // fare qualcosa con il form,  come salvare oggetti Tag e Task
                }
            }
            
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

        <form action="..." method="POST" {{ form_enctype(form) }}>
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
        </form>

    .. code-block:: html+php

        <!-- src/Acme/TaskBundle/Resources/views/Task/new.html.php -->
        <!-- ... -->

        <form action="..." method="POST" ...>
            <h3>Tags</h3>
            <ul class="tags">
                <?php foreach($form['tags'] as $tag): ?>
                    <li><?php echo $view['form']->row($tag['name']) ?></li>
                <?php endforeach; ?>
            </ul>

            <?php echo $view['form']->rest($form) ?>
        </form>
        
        <!-- ... -->

Quando l'utente invia il form, i dati inviati per i campi di ``Tags``
sono utilizzato per costruire un ArrayCollection di oggetti ``Tag``,che viene poi
impostato sul campo ``tag`` dell'istanza ``Task``.

La collezione ``Tags``è acessibile tramite ``$task->getTags()``
e può essere persistita nella base dati, oppure utilizzata dove necessario.

Finora, tutto ciò funziona bene, ma questo non permette di aggiungere nuovi dinamicamente 
todo o eliminare todo esistenti. Quindi, la modifica dei todo esistenti funziona 
bene, ma ancora non si possono aggiungere nuovi tag.

.. _cookbook-form-collections-new-prototype:

Permettere "nuovi" tag con "prototipo"
--------------------------------------

Permettere all'utente di inserire dinamicamente nuovi tag significa che abbiamo la necessità di
utilizzare JavaScript. Precedentemente, sono stati aggiunti due tag al nostro form nel controllore.
Ora si ha la necessità che l'utente possa aggiungere diversi form di tag, secondo le sue necessità, direttamente dal browser.
Questo può essere fatto attraverso un po' di JavaScript.

La prima cosa di cui si ha bisogno è di far capire alla collezione di form, che
riceverà un numero indeterminato di tag. Finora sono stati aggiunti due tag e il form
si aspetta di riceverne esattamente due, altrimenti verrà lanciato un errore:
``Questo form non può contenere campi extra``. Per rendere flessibile il form,
bisognerà aggiungere l'opzione ``allow_add`` al campo collection::

    // src/Acme/TaskBundle/Form/Type/TaskType.php
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

Da notare che è stata aggiunto  ``'by_reference' => false``. Normalmente, il framework dei form
modificherebbe i tag su un oggetto `Task`, *senza* effettivamente nemmeno
richiamare `setTags`. Impostando :ref:`by_reference<reference-form-types-by-reference>`
a `false`, `setTags` sarà richiamato. Questo sarà importante più afvanti, come 
vedremo.

Oltre a dire al campo di accettare un numero qualsiasi di oggetti inviati, l'opzione
``allow_add` rende anche disponibile una variabile "prototipo". Quest "prototipo" è un
piccolo "template", che contiene il codice HTML necessario a rendere qualsiasi nuovo form
"tag". Per renderlo, eseguire la seguente modifica nel template:

.. configuration-block::

    .. code-block:: html+jinja
    
        <ul class="tags" data-prototype="{{ form_widget(form.tags.get('prototype')) | e }}">
            ...
        </ul>
    
    .. code-block:: html+php
    
        <ul class="tags" data-prototype="<?php echo $view->escape($view['form']->row($form['tags']->get('prototype'))) ?>">
            ...
        </ul>

.. note::

    Se si rende l'intero sotto-form "tags" insieme (p.e. ``form_row(form.tags)``),
    il prototipo sarà disponibile automaticamente nel ``div`` esterno, come
    attributo ``data-prototype``, similmente a quanto visto sopra.

.. tip::

    L'elemento ``form.tags.get('prototype')`` è un elemento del form che assomiglia molto
    ai singoli elementi ``form_widget(tag)`` dentro al proprio ciclo ``for``.
    Questo vuol dire che si può richiamare su di esso ``form_widget``, ``form_row`` o
    ``form_label``. Si può anche scegliere di rendere solo uno dei suoi campi (p.e. il
    campo ``name``):
    
    .. code-block:: html+jinja
    
        {{ form_widget(form.tags.get('prototype').name) | e }}

Nella pagina resa, il risultato assomiglierà a questo:

.. code-block:: html

    <ul class="tags" data-prototype="&lt;div&gt;&lt;label class=&quot; required&quot;&gt;$$name$$&lt;/label&gt;&lt;div id=&quot;task_tags_$$name$$&quot;&gt;&lt;div&gt;&lt;label for=&quot;task_tags_$$name$$_name&quot; class=&quot; required&quot;&gt;Name&lt;/label&gt;&lt;input type=&quot;text&quot; id=&quot;task_tags_$$name$$_name&quot; name=&quot;task[tags][$$name$$][name]&quot; required=&quot;required&quot; maxlength=&quot;255&quot; /&gt;&lt;/div&gt;&lt;/div&gt;&lt;/div&gt;">

Lo scopo di questa sezione sarà usare JavaScript per leggere questo attributo
e aggiungere dinamicamente nuovi form tag, quando l'utente clicca su "Aggiunti un tag".
Per facilitare le cose, useremo jQuery e ipotizzeremo di averlo incluso da qualche parte
nella nostra pagine.

Aggiunggere un tag ``script`` nella pagine, in modo da poter scrivere del codice JavaScript.

Prima di tutto, aggiungere un collegamento in fondo alla lista "tags", tramite JavaScript. Poi,
collegare l'evento "click" a tale collegamento, in modo da poter aggiungere un nuovo form tag
(``addTagForm`` sarà mostrato successivamente):

.. code-block:: javascript

    // Prendere il div che contiene la lista di tag
    var collectionHolder = $('ul.tags');

    // preparare un collegamento "aggiungere un tag"
    var $addTagLink = $('<a href="#" class="add_tag_link">Aggiungere un tag</a>');
    var $newLinkLi = $('<li></li>').append($addTagLink);

    jQuery(document).ready(function() {
        // aggiungere l'ancora "aggiungere un tag" e il li all'ul dei tag
        collectionHolder.append($newLinkLi);

        $addTagLink.on('click', function(e) {
            // prevenire il "#" nell'URL
            e.preventDefault();

            // aggiungere un nuovo form tag (vedere il prossimo blocco di codice)
            addTagForm();
        });
    });

Il compito della funzione ``addTagForm`` sarà usare l'attributo ``data-prototype`` per aggiungere
dinamicamente un nuovo form, al click sul collegamento. L'elemento ``data-prototype``
contiene l'input chiamato ``task[tags][$$name$$][name]`` e con id
``task_tags_$$name$$_name``. La stringa ``$$name`` è un piccolo "segnaposto",
che sostituiremo con un numero univoco e incrementale (p.e. ``task[tags][3][name]``).

Il vero codice necessario per far funzionare il tutto potrebbe variare un po', ma ecco
un esempio:

.. code-block:: javascript

    function addTagForm() {
        // Prendere data-prototype, come spiegato in precedenze
        var prototype = collectionHolder.attr('data-prototype');

        // Sostituire '$$name$$' nel prototipo, per essere invece un
        // numero, basato sulla lunghezza attuale dell'elenco.
        var newForm = prototype.replace(/\$\$name\$\$/g, collectionHolder.children().length);

        // Mostrare il form nella pagina, dentro un li, prima del collegamento "Aggiungere un tag"
        var $newFormLi = $('<li></li>').append(newForm);
        $newLinkLi.before($newFormLi);
    }

.. note:

    È meglio separare il codice JavaScript in un file a parte, piuttosto che scriverlo
    direttamente in mezzo al codice HTML, come fatto ora.

Ora, ogni volta che un utente clicca sul link ``Aggiungi un tag``, apparirà un nuovo
form nella pagina. All'invio del form, ogni nuovo form tag sarà convertito in nuovi oggetti
``Tag`` e aggiunto alla proprietà ``tags`` dell'oggetto ``Task``

.. sidebar:: Doctrine: relazioni a cascata e salvataggio del lato "opposto"

    Per avere i nuovi tag salvati in Doctrine, occore considerare un paio di altri aspetti.
    Primo, a meno di non iterare tutti i nuovi oggetti ``Tag`` e richiamare
    ``$em->persist($tag)`` su ciascuno, si riceverà un errore da
    Doctrine:
    
        A new entity was found through the relationship 'Acme\TaskBundle\Entity\Task#tags' that was not configured to cascade persist operations for entity...
    
    Per risolverlo, si può scegliere una "cascata" per persistere automaticamente l'operazione
    dall'oggetto  ``Task`` a ogni tag correlato. Per farlo, aggiungere l'opzione ``cascade``
    ai meta-dati ``ManyToMany``:
    
    .. configuration-block::
    
        .. code-block:: php-annotations

            /**
             * @ORM\ManyToMany(targetEntity="Tag", cascade={"persist"})
             */
            protected $tags;

        .. code-block:: yaml

            # src/Acme/TaskBundle/Resources/config/doctrine/Task.orm.yml
            Acme\TaskBundle\Entity\Task:
                type: entity
                # ...
                oneToMany:
                    tags:
                        targetEntity: Tag
                        cascade:      [persist]
    
    Un altro possibile problema riguarda il `lato di appartenenza e il lato inverso`_
    delle relazioni Doctrine. In questo esempio il lato di appartenenza della
    relazione è "Task", quindi la persistenza funzionerà finché i tag sono aggiunti
    in modo appropriato al Task. Tuttavia, se il lato di appartenenza è su "Tag", allora
    servirà un po' di lavoro in più, per assicurarsi che venga modificato il lato giusto
    della relazione.

    Il trucco sta nell'assicurarsi che un singolo "Task" sia impostato su ogni "Tag".   
    Un modo facile per farlo è aggiungere un po' di logica a ``setTags()``,
    che è richiamato dal framework dei form, poiché :ref:`by_reference<reference-form-types-by-reference>`
    è impostato a ``false``::
    
        // src/Acme/TaskBundle/Entity/Task.php
        // ...

        public function setTags(ArrayCollection $tags)
        {
            foreach ($tags as $tag) {
                $tag->addTask($this);
            }

            $this->tags = $tags;
        }

    Dentro ``Tag``, assicurarsi di avere un metodo ``addTask``::

        // src/Acme/TaskBundle/Entity/Tag.php
        // ...

        public function addTask(Task $task)
        {
            if (!$this->tasks->contains($task)) {
                $this->tasks->add($task);
            }
        }
    
    In caso di relazione ``OneToMany``, il trucco è simile, tranne che si
    può semplicemente richiamare ``setTask`` da dentro ``setTags``.

.. _cookbook-form-collections-remove:

Permettere la rimozione di tag
------------------------------

The next step is to allow the deletion of a particular item in the collection.
The solution is similar to allowing tags to be added.

Start by adding the ``allow_delete`` option in the form Type::
    
    // src/Acme/TaskBundle/Form/Type/TaskType.php
    // ...
    
    public function buildForm(FormBuilder $builder, array $options)
    {
        $builder->add('description');

        $builder->add('tags', 'collection', array(
            'type' => new TagType(),
            'allow_add' => true,
            'allow_delete' => true,
            'by_reference' => false,
        ));
    }

Templates Modifications
~~~~~~~~~~~~~~~~~~~~~~~
    
The ``allow_delete`` option has one consequence: if an item of a collection 
isn't sent on submission, the related data is removed from the collection
on the server. The solution is thus to remove the form element from the DOM.

First, add a "delete this tag" link to each tag form:

.. code-block:: javascript

    jQuery(document).ready(function() {
        // add a delete link to all of the existing tag form li elements
        collectionHolder.find('li').each(function() {
            addTagFormDeleteLink($(this));
        });
    
        // ... the rest of the block from above
    });
    
    function addTagForm() {
        // ...
        
        // add a delete link to the new form
        addTagFormDeleteLink($newFormLi);
    }

The ``addTagFormDeleteLink`` function will look something like this:

.. code-block:: javascript

    function addTagFormDeleteLink($tagFormLi) {
        var $removeFormA = $('<a href="#">delete this tag</a>');
        $tagFormLi.append($removeFormA);

        $removeFormA.on('click', function(e) {
            // prevent the link from creating a "#" on the URL
            e.preventDefault();

            // remove the li for the tag form
            $tagFormLi.remove();
        });
    }

When a tag form is removed from the DOM and submitted, the removed ``Tag`` object
will not be included in the collection passed to ``setTags``. Depending on
your persistence layer, this may or may not be enough to actually remove
the relationship between the removed ``Tag`` and ``Task`` object.

.. sidebar:: Doctrine: Ensuring the database persistence

    When removing objects in this way, you may need to do a little bit more
    work to ensure that the relationship between the Task and the removed Tag
    is properly removed.

    In Doctrine, you have two side of the relationship: the owning side and the
    inverse side. Normally in this case you'll have a ManyToMany relation
    and the deleted tags will disappear and persist correctly (adding new
    tags also works effortlessly).

    But if you have an ``OneToMany`` relation or a ``ManyToMany`` with a
    ``mappedBy`` on the Task entity (meaning Task is the "inverse" side),
    you'll need to do more work for the removed tags to persist correctly.

    In this case, you can modify the controller to remove the relationship
    on the removed tag. This assumes that you have some ``editAction`` which
    is handling the "update" of your Task::

        // src/Acme/TaskBundle/Controller/TaskController.php
        // ...

        public function editAction($id, Request $request)
        {
            $em = $this->getDoctrine()->getEntityManager();
            $task = $em->getRepository('AcmeTaskBundle:Task')->find($id);
    
            if (!$task) {
                throw $this->createNotFoundException('No task found for is '.$id);
            }

            // Create an array of the current Tag objects in the database
            foreach ($task->getTags() as $tag) $originalTags[] = $tag;
          
            $editForm = $this->createForm(new TaskType(), $task);

               if ('POST' === $request->getMethod()) {
                $editForm->bindRequest($this->getRequest());

                if ($editForm->isValid()) {
        
                    // filter $originalTags to contain tags no longer present
                    foreach ($task->getTags() as $tag) {
                        foreach ($originalTags as $key => $toDel) {
                            if ($toDel->getId() === $tag->getId()) {
                                unset($originalTags[$key]);
                            }
                        }
                    }

                    // remove the relationship between the tag and the Task
                    foreach ($originalTags as $tag) {
                        // remove the Task from the Tag
                        $tag->getTasks()->removeElement($task);
    
                        // if it were a ManyToOne relationship, remove the relationship like this
                        // $tag->setTask(null);
                        
                        $em->persist($tag);

                        // if you wanted to delete the Tag entirely, you can also do that
                        // $em->remove($tag);
                    }

                    $em->persist($task);
                    $em->flush();

                    // redirect back to some edit page
                    return $this->redirect($this->generateUrl('task_edit', array('id' => $id)));
                }
            }
            
            // render some form template
        }

    As you can see, adding and removing the elements correctly can be tricky.
    Unless you have a ManyToMany relationship where Task is the "owning" side,
    you'll need to do extra work to make sure that the relationship is properly
    updated (whether you're adding new tags or removing existing tags) on
    each Tag object itself.


.. _`lato di appartenenza e il lato inverso`: http://docs.doctrine-project.org/en/latest/reference/unitofwork-associations.html