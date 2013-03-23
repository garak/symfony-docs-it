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
    una connessione a base dati), il tutto è pressapoco simile. Ci sono solo alcune parti
    di questa guida che si occupano effettivamente di "persistenza".

    Se si utilizza Doctrine, si avrà la necessità di aggiungere meta-dati Doctrine,
    includendo una la definizione della mappature della relazione ``ManyToMany`` sulla
    proprietà ``tags`` di Task.

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
    use Symfony\Component\Form\FormBuilderInterface;
    use Symfony\Component\OptionsResolver\OptionsResolverInterface;

    class TagType extends AbstractType
    {
        public function buildForm(FormBuilderInterface $builder, array $options)
        {
            $builder->add('name');
        }

        public function setDefaultOptions(OptionsResolverInterface $resolver)
        {
            $resolver->setDefaults(array(
                'data_class' => 'Acme\TaskBundle\Entity\Tag',
            ));
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
    use Symfony\Component\Form\FormBuilderInterface;
    use Symfony\Component\OptionsResolver\OptionsResolverInterface;

    class TaskType extends AbstractType
    {
        public function buildForm(FormBuilderInterface $builder, array $options)
        {
            $builder->add('description');

            $builder->add('tags', 'collection', array('type' => new TagType()));
        }

        public function setDefaultOptions(OptionsResolverInterface $resolver)
        {
            $resolver->setDefaults(array(
                'data_class' => 'Acme\TaskBundle\Entity\Task',
            ));
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
            $tag2 = new Tag();
            $tag2->name = 'tag2';
            $task->getTags()->add($tag2);
            // fine del codice fittizio

            $form = $this->createForm(new TaskType(), $task);

            // processare il form, in una richiesta POST
            if ($request->isMethod('POST')) {
                $form->bind($request);
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
ha tag, appena viene creato).

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

L'insieme ``Tags``è acessibile tramite ``$task->getTags()``
e può essere persistito nella base dati, oppure utilizzato. dove necessario.

Finora, tutto ciò funziona bene, ma questo non permette di aggiungere nuovi dinamicamente 
tag o eliminare tag esistenti. Quindi, la modifica dei tag esistenti funziona 
bene, ma ancora non si possono aggiungere nuovi tag.

.. caution::

    In questa ricetta, includiamo un solo insieme, ma non si è limitati
    a questo. Si possono anche includere insiemi innestati, in quanti livelli
    si desidera. Ma, se si usa Xdebug durante lo sviluppo, si potrebbe ricevere
    l'errore ``Maximum function nesting level of '100' reached, aborting!``.
    Questo a casua dell'impostazione ``xdebug.max_nesting_level`` di PHP setting, che
    ha come valore predefinito ``100``.

    Questa direttiva limita la ricorsione a 100 chiamate, che potrebbe non bastare per
    la resa del form nel template, se si rende l'intero form in una volta
    sola (p.e. con ``form_widget(form)``). Per risolvere, si può impostare la direttiva
    a un valore più alto (tramite il file ini di PHP o tramite :phpfunction:`ini_set`,
    per esempio in ``app/autoload.php``) opure si può rendere ogni campo del form a mano,
    usando ``form_row``.

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
    
    use Symfony\Component\Form\FormBuilderInterface;

    public function buildForm(FormBuilderInterface $builder, array $options)
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
a `false`, `setTags` sarà richiamato. Questo sarà importante più avanti, come 
vedremo.

Oltre a dire al campo di accettare un numero qualsiasi di oggetti inviati, l'opzione
``allow_add`` rende anche disponibile una variabile "prototipo". Questo "prototipo" è un
piccolo "template", che contiene il codice HTML necessario a rendere qualsiasi nuovo form
"tag". Per renderlo, eseguire la seguente modifica nel template:

.. configuration-block::

    .. code-block:: html+jinja

        <ul class="tags" data-prototype="{{ form_widget(form.tags.vars.prototype)|e }}">
            ...
        </ul>

    .. code-block:: html+php

        <ul class="tags" data-prototype="<?php echo $view->escape($view['form']->row($form['tags']->vars['prototype'])) ?>">
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
    
        {{ form_widget(form.tags.vars.prototype.name)|e }}

Nella pagina resa, il risultato assomiglierà a questo:

.. code-block:: html

    <ul class="tags" data-prototype="&lt;div&gt;&lt;label class=&quot; required&quot;&gt;__name__&lt;/label&gt;&lt;div id=&quot;task_tags___name__&quot;&gt;&lt;div&gt;&lt;label for=&quot;task_tags___name___name&quot; class=&quot; required&quot;&gt;Name&lt;/label&gt;&lt;input type=&quot;text&quot; id=&quot;task_tags___name___name&quot; name=&quot;task[tags][__name__][name]&quot; required=&quot;required&quot; maxlength=&quot;255&quot; /&gt;&lt;/div&gt;&lt;/div&gt;&lt;/div&gt;">

Lo scopo di questa sezione sarà usare JavaScript per leggere questo attributo
e aggiungere dinamicamente nuovi form tag, quando l'utente clicca su "Aggiunti un tag".
Per facilitare le cose, useremo jQuery e ipotizzeremo di averlo incluso da qualche parte
nella nostra pagine.

Aggiunggere un tag ``script`` nella pagine, in modo da poter scrivere del codice JavaScript.

Prima di tutto, aggiungere un collegamento in fondo alla lista "tags", tramite JavaScript. Poi,
collegare l'evento "click" a tale collegamento, in modo da poter aggiungere un nuovo form tag
(``addTagForm`` sarà mostrato successivamente):

.. code-block:: javascript

    // Prende il div che contiene la lista di tag
    var collectionHolder = $('ul.tags');

    // prepara un collegamento "aggiungere un tag"
    var $addTagLink = $('<a href="#" class="add_tag_link">Aggiungere un tag</a>');
    var $newLinkLi = $('<li></li>').append($addTagLink);

    jQuery(document).ready(function() {
        // aggiunge l'ancora "aggiungere un tag" e il li all'ul dei tag
        collectionHolder.append($newLinkLi);

        // contare gli input correnti (p.e. 2), usare il valore come nuovo
        // indice da usare per inserire un nuovo elemento (p.e. 2)
        collectionHolder.data('index', collectionHolder.find(':input').length);

        $addTagLink.on('click', function(e) {
            // previene il "#" nell'URL
            e.preventDefault();

            // aggiunge un nuovo form tag (vedere il prossimo blocco di codice)
            addTagForm(collectionHolder, $newLinkLi);
        });
    });

Il compito della funzione ``addTagForm`` sarà usare l'attributo ``data-prototype`` per aggiungere
dinamicamente un nuovo form, al click sul collegamento. L'elemento ``data-prototype``
contiene l'input chiamato ``task[tags][__name__][name]`` e con id
``task_tags___name___name``. La stringa ``__name__`` è un piccolo "segnaposto",
che sostituiremo con un numero univoco e incrementale (p.e. ``task[tags][3][name]``).

.. versionadded:: 2.1
    Il segnaposto è stato cambiato da ``$$name$$`` a ``__name__`` in Symfony 2.1

Il vero codice necessario per far funzionare il tutto potrebbe variare un po', ma ecco
un esempio:

.. code-block:: javascript

    function addTagForm() {
        // Prendere data-prototype, come spiegato in precedenze
        var prototype = collectionHolder.attr('data-prototype');

        // contare gli input nel form (p.e. 2), usarlo come nuovo indice (p.e. 2)
        var newIndex = collectionHolder.find(':input').length;

        // Sostituire '__name__' nell'HTML del prototipo per essere
        // invece un numero basato su quanti elementi ci sono
        var newForm = prototype.replace(/__name__/g, newIndex);

        // incrementare l'indice di 1 per l'elemento successivo
        collectionHolder.data('index', index + 1);

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

    Per avere i nuovi tag salvati in Doctrine, occorre considerare un paio di altri aspetti.
    Primo, a meno di non iterare tutti i nuovi oggetti ``Tag`` e richiamare
    ``$em->persist($tag)`` su ciascuno, si riceverà un errore da
    Doctrine:

        A new entity was found through the relationship `Acme\TaskBundle\Entity\Task#tags`
        that was not configured to cascade persist operations for entity...

    Per risolverlo, si può scegliere una "cascata" per persistere automaticamente l'operazione
    dall'oggetto  ``Task`` a ogni tag correlato. Per farlo, aggiungere l'opzione ``cascade``
    ai meta-dati ``ManyToMany``:

    .. configuration-block::

        .. code-block:: php-annotations

            // src/Acme/TaskBundle/Entity/Task.php

            // ...

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

        .. code-block:: xml

            <!-- src/Acme/TaskBundle/Resources/config/doctrine/Task.orm.xml -->
            <doctrine-mapping xmlns="http://doctrine-project.org/schemas/orm/doctrine-mapping"
                xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
                xsi:schemaLocation="http://doctrine-project.org/schemas/orm/doctrine-mapping
                                http://doctrine-project.org/schemas/orm/doctrine-mapping.xsd">

                <entity name="Acme\TaskBundle\Entity\Task" ...>
                    <!-- ... -->
                    <one-to-many field="tags" target-entity="Tag">
                        <cascade>
                            <cascade-persist />
                        </cascade>
                    </one-to-many>
                </entity>
            </doctrine-mapping>

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

Il passo successivo è consentire la cancellazione di un deterimato elemento dell'elenco.
La soluzione è simile a quella usata per consentire l'aggiunta di tag.

Iniziamo aggiungendo l'opzione ``allow_delete`` nel Type del form::
    
    // src/Acme/TaskBundle/Form/Type/TaskType.php

    // ...
    use Symfony\Component\Form\FormBuilderInterface;

    public function buildForm(FormBuilderInterface $builder, array $options)
    {
        $builder->add('description');

        $builder->add('tags', 'collection', array(
            'type' => new TagType(),
            'allow_add' => true,
            'allow_delete' => true,
            'by_reference' => false,
        ));
    }

Modifiche ai template
~~~~~~~~~~~~~~~~~~~~~
    
L'opzione ``allow_delete`` ha una conseguenza: se un elemento dell'elenco non viene
inviato, i dati relativi saranno rimossi dall'elenco. La soluzione quindi è quella di
rimuovere l'elemento dal DOM.

Primo, aggiungere un collegamento "eliminare questo tag" a ogni form tag:

.. code-block:: javascript

    jQuery(document).ready(function() {
        // aggiunge un collegamento di eliminazione a ogni elemento tag esistente
        collectionHolder.find('li').each(function() {
            addTagFormDeleteLink($(this));
        });

        // ... il resto del blocco visto in precedenza
    });

    function addTagForm() {
        // ...

        // aggiunge un collegamento di eliminazione al nuovo form
        addTagFormDeleteLink($newFormLi);
    }

La funzione ``addTagFormDeleteLink`` sarà simile a questa:

.. code-block:: javascript

    function addTagFormDeleteLink($tagFormLi) {
        var $removeFormA = $('<a href="#">delete this tag</a>');
        $tagFormLi.append($removeFormA);

        $removeFormA.on('click', function(e) {
            // previene il "#" nell'URL
            e.preventDefault();

            // rimuove l'elemento li per i form del tag
            $tagFormLi.remove();
        });
    }

Quando un form di un tag viene rimosso da DOM e inviato, l'oggetto ``Tag`` rimosso non
sarà incluso nell'elenco passato a ``setTags``. A seconda del livello di persistenza
usato, questo potrebbe essere o non essere sufficiente per rimuovere effettivamente la
relazione tra l'oggetto ``Tag`` rimosso e l'oggetto ``Task``.

.. sidebar:: Doctrine: assicurare la persistenza nella base dati

    Quando si rimuovono gli oggetti in questo modo, potrebbe essere necessario un po' di
    lavoro ulteriore per assicurare che la relazione tra il Task e il Tag rimosso sia
    propriamente eliminata.

    In Doctrine, si hanno due lati di una relazione: il lato di apparteneza e il lato
    inverso. Normalmente, in questo caso si avrà una relazione ``ManyToMany`` e i tag
    cancellati spariranno e saranno persistiti correttamente (e anche l'aggiunta di nuovi
    tag funzionerà senza sforzi ulteriori).

    Se invece si ha una relazione ``OneToMany``, o una ``ManyToMany`` con un
    ``mappedBy`` sull'entità Task (e quindi Task è il lato inverso),
    servirà del lavoro supplementare per persistere correttamente i tag rimossi.

    In questo caso, si può modificare il controllore per eliminare la relazione con il
    tag rimosso. Si ipotizza che si abbia un'azione ``editAction``, che gestisce
    l'aggiornamento del Task::

        // src/Acme/TaskBundle/Controller/TaskController.php

        // ...

        public function editAction($id, Request $request)
        {
            $em = $this->getDoctrine()->getManager();
            $task = $em->getRepository('AcmeTaskBundle:Task')->find($id);

            if (!$task) {
                throw $this->createNotFoundException('No task found for is '.$id);
            }

            $originalTags = array();

            // Crea un array degli oggetti Tag attualmente nella base dati
            foreach ($task->getTags() as $tag) {
                $originalTags[] = $tag;
            }

            $editForm = $this->createForm(new TaskType(), $task);

            if ($request->isMethod('POST')) {
                $editForm->bind($this->getRequest());

                if ($editForm->isValid()) {

                    // filtra $originalTags per contenere i tag non più presenti
                    foreach ($task->getTags() as $tag) {
                        foreach ($originalTags as $key => $toDel) {
                            if ($toDel->getId() === $tag->getId()) {
                                unset($originalTags[$key]);
                            }
                        }
                    }

                    // rimuove la relazione tra tag e Task
                    foreach ($originalTags as $tag) {
                        // rimuove il Task dal Tag
                        $tag->getTasks()->removeElement($task);

                        // se ci fosse una relazione ManyToOne, rimuoverla in questo modo
                        // $tag->setTask(null);

                        $em->persist($tag);

                        // se si vuole eliminare del tutto il Tag, si può anche fare così
                        // $em->remove($tag);
                    }

                    $em->persist($task);
                    $em->flush();

                    // ritorna a una pagina di modifica
                    return $this->redirect($this->generateUrl('task_edit', array('id' => $id)));
                }
            }

            // rendere un template del form
        }

    Come si può vedere, aggiungere e rimuovere correttamente gli elementi può non essere banale.
    A meno che non si abbia una relazione ``ManyToMany`` in cui il Task è il lato di appartenenza,
    occorrerà del lavoro ulteriore per assicurarsi che la relazione sia aggiornata
    correttamente (sia per l'aggiunta di nuovi tag che per la rimozione di tag esistenti)
    per ogni oggetto Tag.


.. _`lato di appartenenza e il lato inverso`: http://docs.doctrine-project.org/en/latest/reference/unitofwork-associations.html
