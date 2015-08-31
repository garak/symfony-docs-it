.. index::
   single: Form; Personalizzare la resa dei form

Personalizzare la resa dei form
===============================

Symfony permette un'ampia varietà di modi per personalizzare la resa di un form.
In questa guida, si apprenderà come personalizzare ogni possibile parte del
form con il minimo sforzo possibile se si utilizza Twig o PHP come
motore di template.

Le basi della resa dei form
---------------------------

Si ricordi che le label, gli errori e i widget HTML di un campo del form possono essere facilmente
resi usando la funzione di Twig ``form_row`` oppure il metodo dell'helper PHP
``row``:

.. configuration-block::

    .. code-block:: jinja

        {{ form_row(form.age) }}

    .. code-block:: php

        <?php echo $view['form']->row($form['age']); ?>

È possibile anche rendere individualmente ogni parte dell'albero del campo:

.. configuration-block::

    .. code-block:: html+jinja

        <div>
            {{ form_label(form.age) }}
            {{ form_errors(form.age) }}
            {{ form_widget(form.age) }}
        </div>

    .. code-block:: php

        <div>
            <?php echo $view['form']->label($form['age']); ?>
            <?php echo $view['form']->errors($form['age']); ?>
            <?php echo $view['form']->widget($form['age']); ?>
        </div>

In entrambi i casi le label, gli errori e i widget HTML del form, sono resi utilizzando
un set di markup che sono standard con Symfony. Per esempio, entrambi i
template sopra renderebbero:

.. code-block:: html

    <div>
        <label for="form_age">Età</label>
        <ul>
            <li>Questo campo è obbligatorio</li>
        </ul>
        <input type="number" id="form_age" name="form[age]" />
    </div>

Per prototipare velocemente e testare un form, è possibile rendere l'intero form
semplicemente con una riga:

.. configuration-block::

    .. code-block:: jinja

        {# rende tutti i campi #}
        {{ form_widget(form) }}

        {# rende tutti i campi *e* i tag di apertura e chiusura del form #}
        {{ form(form) }}

    .. code-block:: php

        <!-- rende tutti i campi -->
        <?php echo $view['form']->widget($form) ?>

        <!-- rende tutti i campi *e* i tag di apertura e chiusura del form -->
        <?php echo $view['form']->form($form) ?>

Nella restante parte di questa ricetta, verrà mostrato come ogni parte del codice del form
può essere modificato a diversi livelli. Per maggiori informazioni sulla resa dei
form in generale, è disponibile :ref:`form-rendering-template`.

.. _cookbook-form-customization-form-themes:

Cosa sono i temi di un form?
----------------------------

Symfony usa frammenti di form, piccoli pezzi di template che rendono semplicemente
alcune parti, per rendere ogni parte di un form: la label del campo, gli errori,
campi di testo ``input``, tag ``select``, ecc.

I frammenti sono definiti come dei blocchi in Twig e come dei template in PHP.

Un *tema* non è nient'altro che un insieme di frammenti che si vuole utilizzare quando
si rende un form. In altre parole, se si vuole personalizzare una parte della
resa del form, è possibile importare un *tema* che contiene una personalizzazione
del frammento appropriato del form.

Symfony ha un tema predefinito (`form_div_layout.html.twig`_ in Twig e
``FrameworkBundle:Form`` in PHP), che definisce tutti i frammenti necessari 
per rendere ogni parte di un form.

Nella prossima sezione si potrà vedere come personalizzare un tema, sovrascrivendo
qualcuno o tutti i suoi frammenti.

Per esempio, quando è reso il widget di un campo ``integer``, è generato
un campo ``input`` ``number``

.. configuration-block::

    .. code-block:: html+jinja

        {{ form_widget(form.age) }}

    .. code-block:: php

        <?php echo $view['form']->widget($form['age']) ?>

rende:

.. code-block:: html

    <input type="number" id="form_age" name="form[age]" required="required" value="33" />

Internamente, Symfony utilizza il frammento ``integer_widget`` per rendere il campo.
Questo perché il tipo di campo è ``integer`` e si vuole rendere il ``widget``
(in contrapposizione alla sua ``label`` o ai suoi ``errors``).

In Twig per impostazione predefinita il blocco ``integer_widget`` dal template
`form_div_layout.html.twig`.

In PHP è il file ``integer_widget.html.php`` posizionato nella cartella
``FrameworkBundle/Resources/views/Form``.

L'implementazione del frammento ``integer_widget`` sarà simile a:

.. configuration-block::

    .. code-block:: jinja

        {# form_div_layout.html.twig #}
        {% block integer_widget %}
            {% set type = type|default('number') %}
            {{ block('form_widget_simple') }}
        {% endblock integer_widget %}

    .. code-block:: html+php

        <!-- integer_widget.html.php -->
        <?php echo $view['form']->block($form, 'form_widget_simple', array('type' => isset($type) ? $type : "number")) ?>

Come si può vedere, questo frammento rende un altro frammento: ``form_widget_simple``:

.. configuration-block::

    .. code-block:: html+jinja

        {# form_div_layout.html.twig #}
        {% block form_widget_simple %}
            {% set type = type|default('text') %}
            <input type="{{ type }}" {{ block('widget_attributes') }} {% if value is not empty %}value="{{ value }}" {% endif %}/>
        {% endblock form_widget_simple %}

    .. code-block:: html+php

        <!-- FrameworkBundle/Resources/views/Form/form_widget_simple.html.php -->
        <input
            type="<?php echo isset($type) ? $view->escape($type) : 'text' ?>"
            <?php if (!empty($value)): ?>value="<?php echo $view->escape($value) ?>"<?php endif ?>
            <?php echo $view['form']->block($form, 'widget_attributes') ?>
        />

Il punto è che il frammento detta l'output HTML di ogni parte del form. Per
personalizzare l'output del form, è necessario soltanto identificare e sovrascrivere il frammento
corretto. Un set di queste personalizzazioni di frammenti è conosciuto come "tema" di un form.
Quando viene reso un form, è possibile scegliere quale tema del form si vuole applicare.

In Twig un tema è un singolo file di template e i frammenti sono dei blocchi definiti
in questo file.

In PHP un tema è una cartella e i frammenti sono singoli file di template in
questa cartella.

.. _cookbook-form-customization-sidebar:

.. sidebar:: Sapere quale blocco personalizzare

    In questo esempio, il nome del frammento personalizzato è ``integer_widget`` perché
    si vuole sovrascrivere l'HTML del ``widget`` per tutti i tipi di campo ``integer``. Se
    si ha la necessità di personalizzare campi textarea, si deve personalizzare il widget ``textarea_widget``.

    Come è possibile vedere, il nome del frammento è una combinazione del tipo di campo e
    ogni parte del campo viene resa (es. ``widget``, ``label``,
    ``errors``, ``row``). Come tale, per personalizzare la resa degli errori solo per
    il campo input ``text``, bisogna personalizzare il frammento ``text_errors``.

    Più frequentemente, tuttavia, si vorrà personalizzare la visualizzazione degli errori
    attraverso  *tutti* i campi. È possibile fare questo personalizzando il frammento ``form_errors``.
    Questo si avvale delle ereditarietà del tipo di campo. Specificamente
    dato che il tipo ``text`` è esteso dal tipo ``field``, il componente del form
    guarderà per prima cosa al tipo-specifico di frammento (es. ``text_errors``) prima 
    di ricadere sul nome del frammento del suo genitore, se non esiste (es. ``form_errors``).

    Per maggiori informazioni sull'argomento, si veda :ref:`form-template-blocks`.

.. _cookbook-form-theming-methods:

Temi del Form
--------------

Per vedere la potenza dei temi di un form, si supponga di voler impacchettare ogni campo di input ``number``
in un tag ``div``. La chiave per fare questo è personalizzare
il frammento ``integer_widget``.

Temi del form in Twig
---------------------

Per personalizzare il blocco dei campi del form in Twig, si hanno due possibilità su *dove*
il blocco del form personalizzato può essere implementato:

+--------------------------------------+-----------------------------------+-------------------------------------------+
| Metodo                               | Pro                               | Contro                                    |
+======================================+===================================+===========================================+
| Nello stesso template del form       | Veloce e facile                   | Non utilizzabile in altri template        |
+--------------------------------------+-----------------------------------+-------------------------------------------+
| In un template separato              | Riutilizzabile in più template    | Richiede la creazione di un template extra|
+--------------------------------------+-----------------------------------+-------------------------------------------+

Entrambi i metodi hanno lo stesso effetto ma sono consigliati per situazioni differenti.

.. _cookbook-form-twig-theming-self:

Metodo 1: Nello stesso template del form
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Il modo più facile di personalizzare il blocco ``integer_widget`` è personalizzarlo
direttamente nel template che è sta attualmente rendendo il form.

.. code-block:: html+jinja

    {% extends '::base.html.twig' %}

    {% form_theme form _self %}

    {% block integer_widget %}
        <div class="integer_widget">
            {% set type = type|default('number') %}
            {{ block('form_widget_simple') }}
        </div>
    {% endblock %}

    {% block content %}
        {# ... rendere il form #}

        {{ form_row(form.age) }}
    {% endblock %}

Utilizzando il tag speciale ``{% form_theme form _self %}``, Twig guarda nello
stesso template per ogni blocco di form sovrascritto. Assumendo che il campo
``form.age`` è un tipo di campo ``integer``, quando il suo widget è reso, verrà utilizzato
il blocco personalizzato ``integer_widget``.

Lo svantaggio di questo metodo è che il blocco del form personalizzato non può essere
riutilizzato quando si rende un altro form in altri template. In altre parole, questo metodo
è molto utile quando si effettuano personalizzazioni che sono specifiche per singoli
form nell'applicazione. Se si vuole riutilizzare una personalizzazione attraverso
alcuni (o tutti) form nell'applicazione, si legga la prossima sezione.

.. _cookbook-form-twig-separate-template:

Metodo 2: In un template separato
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

È possibile scegliere di mettere il blocco del form personalizzato ``integer_widget`` in un
interamente in un template separato. Il codice e il risultato finale sono gli stessi, ma
ora è possibile riutilizzare la personalizzazione del formi in diversi template:

.. code-block:: html+jinja

    {# src/AppBundle/Resources/views/Form/fields.html.twig #}
    {% block integer_widget %}
        <div class="integer_widget">
            {% set type = type|default('number') %}
            {{ block('form_widget_simple') }}
        </div>
    {% endblock %}

Ora che è stato creato il blocco del form personalizzato, si ha la necessità di dire a Symfony
di utilizzarlo. Nel template dove si sta rendendo il form,
dire a Symfony di utilizzare il template attraverso il tag ``form_theme``:

.. _cookbook-form-twig-theme-import-template:

.. code-block:: html+jinja

    {% form_theme form 'AppBundle:Form:fields.html.twig' %}

    {{ form_widget(form.age) }}

Quando il widget ``form.age`` è reso, Symfony utilizzerà il blocco ``integer_widget``
dal nuovo template e il tag ``input`` sarà incorporato nel
``div`` specificato nel blocco personalizzato.

Template multipli
.................

Si può anche personalizzare un form applicando più template. Per poterlo fare, passare i
nomi di tutti i template come array, usando la parola chiave ``with``:

.. code-block:: html+jinja

    {% form_theme form with ['::common.html.twig', ':Form:fields.html.twig',
                             'AppBundle:Form:fields.html.twig'] %}

    {# ... #}

I template possono trovarsi in bundle differenti e possono anche trovarsi nella
cartella globale ``app/Resources/views/``.

Form figli
..........

Si può anche applicare un tema a uno specifico figlio del form:

.. code-block:: html+jinja

    {% form_theme form.child 'AppBundle:Form:fields.html.twig' %}

Questo torna utile quanto si vuole avere un tema personalizzato per un form innestato che
differisca da quello del form principale. Basta specificare entrambi i temi:

.. code-block:: html+jinja

    {% form_theme form 'AppBundle:Form:fields.html.twig' %}

    {% form_theme form.child 'AppBundle:Form:fields_child.html.twig' %}

.. _cookbook-form-php-theming:

Temi del form in PHP
--------------------

Quando si utilizza PHP come motore per i temi, l'unico metodo per personalizzare un frammento
è creare un nuovo file di tema, in modo simile al secondo metodo adottato per
Twig.

Bisogna nominare il file del tema dopo il frammento. Bisogna creare il file ``integer_widget.html.php``
per personalizzare il frammento ``integer_widget``.

.. code-block:: html+php

    <!-- src/AppBundle/Resources/views/Form/integer_widget.html.php -->
    <div class="integer_widget">
        <?php echo $view['form']->block($form, 'form_widget_simple', array('type' => isset($type) ? $type : "number")) ?>
    </div>

Ora che è stato creato il tema del form personalizzato, bisogna dire a Symfony
di utilizzarlo. Nel template dove viene attualmente reso il form,
dire a Symfony di utilizzare il tema attraverso il metodo ``setTheme`` dell'helper:

.. _cookbook-form-php-theme-import-template:

.. code-block:: php

    <?php $view['form']->setTheme($form, array('AppBundle:Form')); ?>

    <?php $view['form']->widget($form['age']) ?>

Quando il widget ``form.age`` viene reso, Symfony utilizzerà il tema personalizzato
``integer_widget.html.php`` e il tag ``input`` sarà contenuto in un
elemento ``div``.

Se si vuole applicare un tema a uno specifico form figlio, passarlo al metodo ``setTheme``:


.. code-block:: php

    <?php $view['form']->setTheme($form['child'], 'AppBundle:Form/Child'); ?>

.. _cookbook-form-twig-import-base-blocks:

Referenziare blocchi di form (specifico per Twig)
-------------------------------------------------

Finora, per sovrascrivere un particolare blocco del form, il metodo migliore è copiare
il blocco predefinito da  `form_div_layout.html.twig`_, incollarlo in un template differente,
e personalizzarlo. In molti casi, è possibile evitare di fare questo referenziando
il blocco di base quando lo si personalizza.

Tutto ciò è semplice da fare, ma varia leggermente a seconda se le personalizzazioni del blocco di form
sono nello stesso template del form o in un template separato.

Referenziare blocchi dall'interno dello stesso template del form
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Importare i blocchi aggiungendo un tag ``use`` nel template da dove si sta rendendo
il form:

.. code-block:: jinja

    {% use 'form_div_layout.html.twig' with integer_widget as base_integer_widget %}

Ora, quando sono importati i blocchi da `form_div_layout.html.twig`_, il
blocco ``integer_widget`` è chiamato ``base_integer_widget``. Questo significa che quando
viene ridefinito il blocco ``integer_widget``, è possibile referenziare il markup predefinito
tramite ``base_integer_widget``:

.. code-block:: html+jinja

    {% block integer_widget %}
        <div class="integer_widget">
            {{ block('base_integer_widget') }}
        </div>
    {% endblock %}

Referenziare blocchi base da un template esterno
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Se la personalizzazione è stata fatta su un template esterno, è possibile referenziare
il blocco base utilizzando la funzione di Twig ``parent()``:

.. code-block:: html+jinja

    {# src/AppBundle/Resources/views/Form/fields.html.twig #}
    {% extends 'form_div_layout.html.twig' %}

    {% block integer_widget %}
        <div class="integer_widget">
            {{ parent() }}
        </div>
    {% endblock %}

.. note::

    Non è possibile referenziare il blocco base quando si usa PHP come
    motore di template. Bisogna copiare manualmente il contenuto del blocco base
    nel nuovo file di template.

.. _cookbook-form-global-theming:

Personalizzare lo strato applicativo
------------------------------------

Se si vuole che una determinata personalizzazione del form sia globale nell'applicazione,
è possibile realizzare ciò effettuando personalizzazioni del form in un template
esterno e dopo importarlo nella configurazione dell'applicazione:

Twig
~~~~

Utilizzando la seguente configurazione, ogni blocco di form personalizzato nel template
``AppBundle:Form:fields.html.twig`` verrà utilizzato globalmente quando un
form verrà reso.

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        twig:
            form:
                resources:
                    - 'AppBundle:Form:fields.html.twig'
            # ...

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <twig:config>
            <twig:form>
                <resource>AppBundle:Form:fields.html.twig</resource>
            </twig:form>
            <!-- ... -->
        </twig:config>

    .. code-block:: php

        // app/config/config.php
        $container->loadFromExtension('twig', array(
            'form' => array(
                'resources' => array(
                    'AppBundle:Form:fields.html.twig',
                ),
            ),

            // ...
        ));

Per impostazione predefinita, Twig utilizza un layout a *div* quando rende i form. Qualcuno, tuttavia,
potrebbe preferire rendere i form in un layout a *tabella*. Utilizzare la risorsa ``form_table_layout.html.twig``
per ottenere questo tipo di layout:

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        twig:
            form:
                resources:
                    - 'form_table_layout.html.twig'
            # ...

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <twig:config>
            <twig:form>
                <resource>form_table_layout.html.twig</resource>
            </twig:form>
            <!-- ... -->
        </twig:config>

    .. code-block:: php

        // app/config/config.php
        $container->loadFromExtension('twig', array(
            'form' => array(
                'resources' => array(
                    'form_table_layout.html.twig',
                ),
            ),

            // ...
        ));

Se si vuole effettuare un cambiamento soltanto in un template, aggiungere la seguente riga al
file di template piuttosto che aggiungere un template come risorsa:

.. code-block:: html+jinja

    {% form_theme form 'form_table_layout.html.twig' %}

Si osservi che la variabile ``form`` nel codice sottostante è la variabile della vista form
che è stata passata al template.

PHP
~~~

Utilizzando la configurazione seguente, ogni frammento di form personalizzato nella
cartella ``src/Acme/DemoBundle/Resources/views/Form`` sarà utilizzato globalmente quando un
form viene reso.

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        framework:
            templating:
                form:
                    resources:
                        - 'AppBundle:Form'
            # ...

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <framework:config>
            <framework:templating>
                <framework:form>
                    <resource>AppBundle:Form</resource>
                </framework:form>
            </framework:templating>
            <!-- ... -->
        </framework:config>

    .. code-block:: php

        // app/config/config.php
        // PHP
        $container->loadFromExtension('framework', array(
            'templating' => array(
                'form' => array(
                    'resources' => array(
                        'AppBundle:Form',
                    ),
                ),
             ),

             // ...
        ));

Per impostazione predefinita, il motore PHP utilizza un layout a *div* quando rende i form. Qualcuno,
tuttavia, potrebbe preferire rendere i form in un layout a *tabella*. Usare la risorsa ``FrameworkBundle:FormTable``
per il layout:

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        framework:
            templating:
                form:
                    resources:
                        - 'FrameworkBundle:FormTable'

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <framework:config>
            <framework:templating>
                <framework:form>
                    <resource>FrameworkBundle:FormTable</resource>
                </framework:form>
            </framework:templating>
            <!-- ... -->
        </framework:config>

    .. code-block:: php

        // app/config/config.php
        $container->loadFromExtension('framework', array(
            'templating' => array(
                'form' => array(
                    'resources' => array(
                        'FrameworkBundle:FormTable',
                    ),
                ),
            ),

             // ...
        ));

Se si vuole effettuare un cambiamento soltanto in un template, aggiungere la seguente riga al
file di template piuttosto che aggiungere un template come risorsa:

.. code-block:: html+php

  <?php $view['form']->setTheme($form, array('FrameworkBundle:FormTable')); ?>

Si osservi che la variabile ``$form`` nel codice sottostante è la variabile della vista form
che è stata passata al template.

Personalizzare un singolo campo
---------------------------------

Finora, sono stati mostrati i vari modi per personalizzare l'output di un widget
di tutti i tipi di campo testuali. Ma è anche possibile personalizzare singoli campi. Per esempio,
si supponga di avere due campi testuali in un form ``product``, chiamati ``name`` e
``description``, ma di voler personalizzare solo uno dei campi. Lo si può fare
personalizzando un frammento, in cui il nome è una combinazione dell'attributo
``id`` del campo e in cui parte del campo viene personalizzato. Per esempio, per
personalizzare solo il campo ``name``:

.. configuration-block::

    .. code-block:: html+jinja

        {% form_theme form _self %}

        {% block _product_name_widget %}
            <div class="text_widget">
                {{ block('form_widget_simple') }}
            </div>
        {% endblock %}

        {{ form_widget(form.name) }}

    .. code-block:: html+php

        <!-- Main template -->
        <?php echo $view['form']->setTheme($form, array('AppBundle:Form')); ?>

        <?php echo $view['form']->widget($form['name']); ?>

        <!-- src/AppBundle/Resources/views/Form/_product_name_widget.html.php -->
        <div class="text_widget">
              echo $view['form']->block('form_widget_simple') ?>
        </div>

Qui, il frammento ``_product_name_widget`` definisce il template da utilizzare per il
campo del quale l'*id* è ``product_name`` (e il nome è ``product[name]``).

.. tip::

   La porzione del campo ``product`` è il nome del form, che può essere impostato
   manualmente o generato automaticamente basandosi sul tipo di nome del form (es.
   ``ProductType`` equivale a ``product``). Se non si è sicuri di cosa sia
   il nome del form, basta semplicemente vedere il sorgente del form generato.

   Se si vuole cambiare la porzione ``product`` o quella ``name`` del nome
   ``_product_name_widget`` del blocco, si può impostare l'opzione ``block_name`` nel
   tipo di form::

        use Symfony\Component\Form\FormBuilderInterface;

        public function buildForm(FormBuilderInterface $builder, array $options)
        {
            // ...

            $builder->add('name', 'text', array(
                'block_name' => 'nome_diverso',
            ));
        }

   Il nome del blocco sarà quindi ``_product_nome_diverso_widget``.

È possibile sovrascrivere il markup per un intera riga di campo utilizzando lo stesso metodo:

.. configuration-block::

    .. code-block:: html+jinja

        {% form_theme form _self %}

        {% block _product_name_row %}
            <div class="name_row">
                {{ form_label(form) }}
                {{ form_errors(form) }}
                {{ form_widget(form) }}
            </div>
        {% endblock %}

        {{ form_row(form.name) }}

    .. code-block:: html+php

        <!-- Template principale -->
        <?php echo $view['form']->setTheme($form, array('AppBundle:Form')); ?>

        <?php echo $view['form']->row($form['name']); ?>

        <!-- src/AppBundle/Resources/views/Form/_product_name_row.html.php -->
        <div class="name_row">
            <?php echo $view['form']->label($form) ?>
            <?php echo $view['form']->errors($form) ?>
            <?php echo $view['form']->widget($form) ?>
        </div>

Altre personalizzazioni comuni
------------------------------

Finora, questa ricetta ha illustrato diversi modi per personalizzare
la resa di un form. La chiave di tutto è personalizzare uno specifico frammento che
corrisponde alla porzione del form che si vuole controllare (si veda 
:ref:`nominare i blocchi dei form<cookbook-form-customization-sidebar>`).

Nella prossima sezione, si potrà vedere come è possibile effettuare diverse personalizzazioni comuni per il form.
Per applicare queste personalizzazioni, si utilizzi uno dei metodi descritti nella
sezione :ref:`cookbook-form-theming-methods`.

Personalizzare l'output degli errori
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

.. note::
   Il componente del form gestisce soltanto *come* gli errori di validazione vengono resi,
   e non gli attuali messaggi di errore di validazione. I messaggi d'errore
   sono determinati dai vincoli di validazione applicati agli oggetti.
   Per maggiori informazioni, si veda il capitolo :doc:`validazione</book/validation>`.

Ci sono diversi modi di personalizzare come gli errori sono resi quando un
form viene inviato con errori. I messaggi di errore per un campo sono resi
quando si utilizza l'helper ``form_errors``:

.. configuration-block::

    .. code-block:: jinja

        {{ form_errors(form.age) }}

    .. code-block:: php

        <?php echo $view['form']->errors($form['age']); ?>

Per impostazione predefinita, gli errori sono resi dentro una lista non ordinata:

.. code-block:: html

    <ul>
        <li>Questo campo è obbligatorio</li>
    </ul>

Per sovrascrivere il modo in cui gli errori sono resi per *tutti* i campi, basta semplicemente copiare,
incollare e personalizzare il frammento ``form_errors``.

.. configuration-block::

    .. code-block:: html+jinja

        {# form_errors.html.twig #}
        {% block form_errors %}
            {% spaceless %}
                {% if errors|length > 0 %}
                <ul>
                    {% for error in errors %}
                        <li>{{ error.message }}</li>
                    {% endfor %}
                </ul>
                {% endif %}
            {% endspaceless %}
        {% endblock form_errors %}

    .. code-block:: html+php

        <!-- form_errors.html.php -->
        <?php if ($errors): ?>
            <ul>
                <?php foreach ($errors as $error): ?>
                    <li><?php echo $error->getMessage() ?></li>
                <?php endforeach ?>
            </ul>
        <?php endif ?>

.. tip::

    Si veda :ref:`cookbook-form-theming-methods` per come applicare questa personalizzazione.

È anche possibile personalizzare l'output dell'errore per uno specifico tipo di campo.
Per personalizzare *solo* il markup usato per tali errori, seguire le stesse istruzioni
viste sopra, ma inserire i contenuti nel blocco ``_errors`` (o nel file, in caso
di template PHP). Per esempio, ``text_errors`` (o ``text_errors.html.php``).

.. tip::

    Vedere :ref:`form-template-blocks` per capire quale blocco o file occorra
    personalizzare.

Alcuni errori, che sono più globali in un form (cioè non sono specifici di un solo
campo), sono resi a parte, di solito in cima al form:

.. configuration-block::

    .. code-block:: jinja

        {{ form_errors(form) }}

    .. code-block:: php

        <?php echo $view['form']->render($form); ?>

Per personalizzare *solo* il markup utilizzato per questi errori, si segue la stesa strada
del codice sopra verificare che la variabile ``compound`` valga ``true``. Se è
``true``, vuol dire che ciò che viene reso al momento è una collezione di
campi (p.e. un form intero) e non solo un campo singolo.

.. configuration-block::

    .. code-block:: html+jinja

        {# form_errors.html.twig #}
        {% block form_errors %}
            {% spaceless %}
                {% if errors|length > 0 %}
                    {% if compound %}
                        <ul>
                            {% for error in errors %}
                                <li>{{ error.message }}</li>
                            {% endfor %}
                        </ul>
                    {% else %}
                        {# ... display the errors for a single field #}
                    {% endif %}
                {% endif %}
            {% endspaceless %}
        {% endblock form_errors %}

    .. code-block:: html+php

        <!-- form_errors.html.php -->
        <?php if ($errors): ?>
            <?php if ($compound): ?>
                <ul>
                    <?php foreach ($errors as $error): ?>
                        <li><?php echo $error->getMessage() ?></li>
                    <?php endforeach ?>
                </ul>
            <?php else: ?>
                <!-- ... render the errors for a single field -->
            <?php endif ?>
        <?php endif ?>


Personalizzare una "riga del form"
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Quando è possibile modificarlo, la strada più facile per rendere il campo di un form è attraverso
la funzione ``form_row``, che rende l'etichetta, gli errori e il widget HTML del
campo. Per personalizzare il markup utilizzato per rendere *tutte* le righe del campo di un form
bisogna sovrascrivere il frammento ``field_row``. Per esempio, si supponga di voler aggiungere una
classe all'elemento  ``div`` per ogni riga:

.. configuration-block::

    .. code-block:: html+jinja

        {# form_row.html.twig #}
        {% block form_row %}
            <div class="form_row">
                {{ form_label(form) }}
                {{ form_errors(form) }}
                {{ form_widget(form) }}
            </div>
        {% endblock form_row %}

    .. code-block:: html+php

        <!-- form_row.html.php -->
        <div class="form_row">
            <?php echo $view['form']->label($form) ?>
            <?php echo $view['form']->errors($form) ?>
            <?php echo $view['form']->widget($form) ?>
        </div>

.. tip::

    Si veda :ref:`cookbook-form-theming-methods` per capire come applicare questa personalizzazione.

Aggiungere un asterisco "obbligatorio" alle label del campo
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

È possibile denotare tutti i campi obbligatori con un asterisco (``*``),
semplicemente personalizzando il frammento ``field_label``.

In Twig, se si sta personalizzando il form all'interno dello stesso template del
form, basta modificare il tag ``use`` e aggiungere le seguenti righe:

.. code-block:: html+jinja

    {% use 'form_div_layout.html.twig' with form_label as base_form_label %}

    {% block form_label %}
        {{ block('base_form_label') }}

        {% if required %}
            <span class="required" title="This field is required">*</span>
        {% endif %}
    {% endblock %}

In Twig, se si sta personalizzando il form all'interno di un template separato, bisogna
utilizzare le seguenti righe:

.. code-block:: html+jinja

    {% extends 'form_div_layout.html.twig' %}

    {% block form_label %}
        {{ parent() }}

        {% if required %}
            <span class="required" title="Questo campo è obbligatorio">*</span>
        {% endif %}
    {% endblock %}

Quando si usa PHP come motore di template bisogna copiare il contenuto del 
template originale:

.. code-block:: html+php

    <!-- form_label.html.php -->

    <!-- contenuto originale -->
    <?php if ($required) { $label_attr['class'] = trim((isset($label_attr['class']) ? $label_attr['class'] : '').' required'); } ?>
    <?php if (!$compound) { $label_attr['for'] = $id; } ?>
    <?php if (!$label) { $label = $view['form']->humanize($name); } ?>
    <label <?php foreach ($label_attr as $k => $v) { printf('%s="%s" ', $view->escape($k), $view->escape($v)); } ?>><?php echo $view->escape($view['translator']->trans($label, array(), $translation_domain)) ?></label>

    <!-- personalizzazione -->
    <?php if ($required) : ?>
        <span class="required" title="This field is required">*</span>
    <?php endif ?>

.. tip::

    Si veda :ref:`cookbook-form-theming-methods` per sapere come effettuare questa personalizzazione.

.. sidebar:: Usare solo CSS

    La resa predefinita dei tag ``label`` dei campi obbligatori ha una classe CSS
    ``required``. Si può quindi aggiungere un asterisco usando solo CSS:

    .. code-block:: css

        label.required:before {
            content: "* ";
        }

Aggiungere messaggi di aiuto
~~~~~~~~~~~~~~~~~~~~~~~~~~~~

È possibile personalizzare i widget del form per ottenere un messaggio di aiuto opzionale.

In Twig, se si sta personalizzando il form all'interno dello stesso template del
form, basta modificare il tag ``use`` e aggiungere le seguenti righe:

.. code-block:: html+jinja

    {% use 'form_div_layout.html.twig' with form_widget_simple as base_form_widget_simple %}

    {% block form_widget_simple %}
        {{ block('base_form_widget_simple') }}

        {% if help is defined %}
            <span class="help">{{ help }}</span>
        {% endif %}
    {% endblock %}

In Twig, se si sta personalizzando il form all'interno di un template separato, bisogna
utilizzare le seguenti righe:

.. code-block:: html+jinja

    {% extends 'form_div_layout.html.twig' %}

    {% block form_widget_simple %}
        {{ parent() }}

        {% if help is defined %}
            <span class="help">{{ help }}</span>
        {% endif %}
    {% endblock %}

Quando si usa PHP come motore di template bisogna copiare il contenuto del 
template originale:

.. code-block:: html+php

    <!-- form_widget_simple.html.php -->

    <!-- Contenuto originale -->
    <input
        type="<?php echo isset($type) ? $view->escape($type) : 'text' ?>"
        <?php if (!empty($value)): ?>value="<?php echo $view->escape($value) ?>"<?php endif ?>
        <?php echo $view['form']->block($form, 'widget_attributes') ?>
    />

    <!-- Personalizzazione -->
    <?php if (isset($help)) : ?>
        <span class="help"><?php echo $view->escape($help) ?></span>
    <?php endif ?>

Per rendere un messaggio di aiuto sotto al campo, passare nella variabile ``help``:

.. configuration-block::

    .. code-block:: jinja

        {{ form_widget(form.title, {'help': 'foobar'}) }}

    .. code-block:: php

        <?php echo $view['form']->widget($form['title'], array('help' => 'foobar')) ?>

.. tip::

    Si veda :ref:`cookbook-form-theming-methods` per sapere come applicare questa configurazione.

Usare le variabili nei Form
---------------------------

La maggior parte delle funzioni disponibili per rendere le varie parti di un form (p.e.
il widget form, la label del form, etc.) consentono anche di eseguire direttamente alcune
personalizzazioni. Si veda l'esempio seguente:

.. configuration-block::

    .. code-block:: jinja

        {# rende un widget, ma con classe "pippo" #}
        {{ form_widget(form.name, {'attr': {'class': 'pippo'}}) }}

    .. code-block:: php

        <!-- rende un widget, ma con classe "pippo" -->
        <?php echo $view['form']->widget($form['name'], array(
            'attr' => array(
                'class' => 'pippo',
            ),
        )) ?>

L'array passato come secondo parametro contiene delle variabili del form. Per maggiori
dettagli su questo concetto in Twig, vedere :ref:`twig-reference-form-variables`.

.. _`form_div_layout.html.twig`: https://github.com/symfony/symfony/blob/2.3/src/Symfony/Bridge/Twig/Resources/views/Form/form_div_layout.html.twig
