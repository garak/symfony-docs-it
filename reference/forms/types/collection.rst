.. index::
   single: Form; Campi; collection

Tipo di campo collection
========================

Questo tipo di campo è usato per rendere un insieme di campi o form. Nel senso più
semplice, potrebbe essere un array di campi ``text`` che popolano un array di
campi ``emails``. In esempi più complessi, si potrebbero includere interi form,
che è utile quando si creano form che espongono relazioni molti-a-molti
(p.e. un prodotto in cui si possono gestire molte foto
correlate).

+---------------+-----------------------------------------------------------------------------+
| Reso come     | dipende dall'opzione `type`_                                                |
+---------------+-----------------------------------------------------------------------------+
| Opzioni       | - `allow_add`_                                                              |
|               | - `allow_delete`_                                                           |
|               | - `options`_                                                                |
|               | - `prototype`_                                                              |
|               | - `prototype_name`_                                                         |
|               | - `type`_                                                                   |
+---------------+-----------------------------------------------------------------------------+
| Opzioni       | - `by_reference`_                                                           |
| ereditate     | - `cascade_validation`_                                                     |
|               | - `empty_data`_                                                             |
|               | - `error_bubbling`_                                                         |
|               | - `error_mapping`_                                                          |
|               | - `label`_                                                                  |
|               | - `label_attr`_                                                             |
|               | - `mapped`_                                                                 |
|               | - `required`_                                                               |
+---------------+-----------------------------------------------------------------------------+
| Tipo genitore | :doc:`form </reference/forms/types/form>`                                   |
+---------------+-----------------------------------------------------------------------------+
| Classe        | :class:`Symfony\\Component\\Form\\Extension\\Core\\Type\\CollectionType`    |
+---------------+-----------------------------------------------------------------------------+

.. note::

    Se si ha a che fare con collezioni di entità Doctrine, prestare particolare
    attenzione alle opzioni `allow_add`_, `allow_delete`_ e `by_reference`_.
    Si può vedere un esempio completo nella ricetta
    :doc:`/cookbook/form/form_collections`.

Uso di base
-----------

Questo tipo è usato quando si vuole gestire un insieme di elementi simili in un
form. Per esempio, si supponga di avere un campo ``emails``, che corrisponde
a un array di indirizzi email. Nel form, si vuole esporre ogni indirizzo email
con un campo testuale::

    $builder->add('emails', 'collection', array(
        // ogni elemento nell'array sarà un campo "email"
        'type'   => 'email',
        // queste opzioni sono passate a ogni tipo "email"
        'options'  => array(
            'required'  => false,
            'attr'      => array('class' => 'email-box')
        ),
    ));

Il modo più semplice di renderlo è tutto insieme:

.. configuration-block::

    .. code-block:: jinja

        {{ form_row(form.emails) }}

    .. code-block:: php

        <?php echo $view['form']->row($form['emails']) ?>

Un metodo molto più flessibile sarebbe questo:

.. configuration-block::

    .. code-block:: html+jinja

        {{ form_label(form.emails) }}
        {{ form_errors(form.emails) }}

        <ul>
        {% for emailField in form.emails %}
            <li>
                {{ form_errors(emailField) }}
                {{ form_widget(emailField) }}
            </li>
        {% endfor %}
        </ul>

    .. code-block:: html+php

        <?php echo $view['form']->label($form['emails']) ?>
        <?php echo $view['form']->errors($form['emails']) ?>

        <ul>
        <?php foreach ($form['emails'] as $emailField): ?>
            <li>
                <?php echo $view['form']->errors($emailField) ?>
                <?php echo $view['form']->widget($emailField) ?>
            </li>
        <?php endforeach ?>
        </ul>

In entrambi i casi, non sarebbe reso alcun campo, a meno che l'array ``emails``
non contenga almeno un'email.

In questo semplice esempio, non è ancora possibile aggiungere nuovi indirizzi o
rimuoverne di esistenti. L'aggiunta di nuovi indirizzi è possibile tramite l'opzione
`allow_add`_ (e facoltativamente l'opzione `prototype`_) (vedere esempio sotto). La
rimozione di email è possibile tramite l'opzione `allow_delete`_.

Aggiungere e rimuovere elementi
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Se `allow_add`_ è ``true``, se vengono inviati elementi non riconosciuti,
saranno aggiunti all'array di elementi. Questo in teoria è buono,
ma in pratica richiede un po' di sforzi in più per far funzionare il JavaScript
lato client.

Proseguendo con l'esempio precedente, si supponga di partire con due email
nell'array ``emails``. In questo caso, saranno resi due campi input, che
assomiglieranno a questi (a seconda del nome del
form):

.. code-block:: html

    <input type="email" id="form_emails_0" name="form[emails][0]" value="foo@foo.com" />
    <input type="email" id="form_emails_1" name="form[emails][1]" value="bar@bar.com" />

Per consentire l'aggiunta di altre email, impostare `allow_add`_ a ``true``
e, tramite JavaScript, rendere un altro campo dal nome ``form[emails][2]``
(e così via per ulteriori campi).

Per facilitare le cose, impostare l'opzione `prototype`_ a ``true`` consente di
rendere un campo "template", utilizzabile poi nel codice JavaScript
per la creazione dinamica di questi nuovi campi. Un campo prototipo assomiglierà
a questo:

.. code-block:: html

    <input type="email"
        id="form_emails___name__"
        name="form[emails][__name__]"
        value=""
    />

Sostituendo ``__name__`` con un valore unico (p.e. ``2``),
si possono costruire e inserire nuovi campi HTML nel form.

Usando jQuery, un semplic esempio assomiglierebbe a questo. Se si rendono i campi
collection tutti insieme (p.e. ``form_row(form.emails)``), le cose si semplificano
ulteriormente, perché l'attributo ``data-prototype`` viene reso automaticamente
(con una piccola differenza, vedere la nota sotto) e tutto ciò che occorre
è il JavaScript:

.. configuration-block::

    .. code-block:: html+jinja

        {{ form_start(form) }}
            {# ... #}

            {# memorizza il prototipo nell'attributo data-prototype #}
            <ul id="email-fields-list"
                data-prototype="{{ form_widget(form.emails.vars.prototype)|e }}">
            {% for emailField in form.emails %}
                <li>
                    {{ form_errors(emailField) }}
                    {{ form_widget(emailField) }}
                </li>
            {% endfor %}
            </ul>

            <a href="#" id="add-another-email">Aggiungere email</a>

            {# ... #}
        {{ form_end(form) }}

        <script type="text/javascript">
            // tiene traccia di quanti campi email sono stati resi
            var emailCount = '{{ form.emails|length }}';

            jQuery(document).ready(function() {
                jQuery('#add-another-email').click(function(e) {
                    e.preventDefault();

                    var emailList = jQuery('#email-fields-list');

                    // prende il template prototipo
                    var newWidget = emailList.attr('data-prototype');
                    // sostiuisce "__name__" usato nell'id e il nome del prototipo
                    // con un numero univoco per le email
                    // l'attributo finale assomiglia a name="contact[emails][2]"
                    newWidget = newWidget.replace(/__name__/g, emailCount);
                    emailCount++;

                    // crea un nuovo elemento nella lista e lo aggiunge
                    var newLi = jQuery('<li></li>').html(newWidget);
                    newLi.appendTo(emailList);
                });
            })
        </script>

.. tip::

    Se si rende tutto l'insieme in una volta sola, il prototipo sarà disponibile
    automaticamente nell'attributo ``data-prototype`` dell'elemento
    (p.e. ``div`` o ``table``) che circonda l'insieme. L'unica differenza è
    che la riga del form viene resa automaticamente, quindi non occorre inserirla
    in un elemento contenitore, come è stato fatto in
    precedenza.

Opzioni del campo
-----------------

allow_add
~~~~~~~~~

**tipo**: ``Booleano`` **predefinito**: ``false``

Se ``true``, ogni elemento non riconosciuto inviato all'insieme sarà aggiunto
come nuovo elemento. L'array finale conterrà gli elementi esistenti e il nuovo
elemento, appena inviato. Si veda l'esempio sopra per maggiori
dettagli.

Si può usare l'opzione `prototype`_ per rendere un elemento prototipo, che può
essere usato, con JavaScript, per creare dinamicamente nuovi elementi lato
client. Per maggiori informazioni, vedere l'esempio sopra e
:ref:`cookbook-form-collections-new-prototype`.

.. caution::

    Se si includono altri form, per riflettere una relazione uno-a-molti nella base dati,
    potrebbe essere necessario assicurarsi a mano che la chiave esterna di questi nuovi
    oggetti sia impostata correttamente. Se si usa Doctrine, questo non avverrà
    automaticamente. Vedere il collegamento sopra per maggiori dettagli.    

allow_delete
~~~~~~~~~~~~

**tipo**: ``Booleano`` **predefinito**: ``false``

Se ``true``, se un elemento esistente non compare tra i dati inviati, sarà assente
dall'array finale di elementi. Questo vuol dire che si può implementare un bottone
"cancella" tramite JavaScript, che rimuove semplicemente un elemento del
form dal DOM. Quando l'utente invia il form, l'assenza dai dati inviati implicherà
una rimozione dall'array finale.

Per maggiori informazioni, vedere :ref:`cookbook-form-collections-remove`.

.. caution::

    Si faccia attenzione nell'usare questa opzione quando si include un insieme di
    oggetti. In questo caso, se un form incluso viene rimosso, *sarà*
    correttamente mancante dall'array finale di oggetti. Tuttavia, a seconda della
    logica dell'applicazione, quando uno di questi oggetti viene rimosso, si potrebbe
    volerlo cancellare o almeno rimuovere la sua chiave esterna riferita all'oggetto
    principale. Questi casi non sono gestiti automaticamente. Per maggiori informazioni,
    vedere :ref:`cookbook-form-collections-remove`.

options
~~~~~~~

**tipo**: ``array`` **predefinito**: ``array()``

L'array passato al tipo di form specificato nell'opzione `type`_.
Per esempio, se si è usato il tipo :doc:`choice</reference/forms/types/choice>`
come opzione `type`_ (p.e. per un insieme di menù a tendina), si dovrebbe
passare almeno l'opzione ``choices`` al tipo
sottostante::

    $builder->add('favorite_cities', 'collection', array(
        'type'   => 'choice',
        'options'  => array(
            'choices'  => array(
                'nashville' => 'Nashville',
                'paris'     => 'Paris',
                'berlin'    => 'Berlin',
                'london'    => 'London',
            ),
        ),
    ));

prototype
~~~~~~~~~

**tipo**: ``Booleano`` **predefinito**: ``true``

Questa opzione è utile quando si usa l'opzione `allow_add`_. Se ``true`` (e se
anche `allow_add`_ è ``true``), sarà disponibile uno speciale attributo "prototype",
in modo che si possa rendere nella pagina un "template" esempio di come ogni nuovo
elemento dovrebbe apparire. L'attributo ``name`` dato a tale elemento è
``__name__``. Questo consente l'aggiunta di un bottone "aggiungi" tramite JavaScript,
che legge il prototipo, sostituisce ``__name__`` con un nome o numero univoco
e lo rende all'interno del form. Quando inviato, sarà aggiunto all'array
sottostante, grazie all'opzione `allow_add`_.

Il campo prototipo può essere reso tramite la variabile ``prototype`` nel campo
collection:

.. configuration-block::

    .. code-block:: jinja

        {{ form_row(form.emails.vars.prototype) }}

    .. code-block:: php

        <?php echo $view['form']->row($form['emails']->vars['prototype']) ?>

Si noti che tutto quello di cui si ha effettivamente bisogno è il widget, ma a
seconda di come si rende il form, avere l'intera riga del form potrebbe essere più facile.

.. tip::

    Se si rende l'intero campo collection in una volta sola, la riga del prototipo
    sarà disponibile automaticamente nell'attributo ``data-prototype``
    dell'elemento (p.e. ``div`` o ``table``) che contiene l'insieme.

Per dettagli su come usare effettivamente questa opzione, vedere l'esempio sopra
o :ref:`cookbook-form-collections-new-prototype`.

prototype_name
~~~~~~~~~~~~~~

**tipo**: ``Stringa`` **predefinito**: ``__name__``

Se si hanno molti insiemi in un form o, peggio, si hanno insiemi annidati,
si potrebbe voler modificare il segnaposto, in modo che i segnaposto senza relazioni non
siano sostituiti con il medesimo valore.

type
~~~~

**tipo**: ``stringa`` o :class:`Symfony\\Component\\Form\\FormTypeInterface` **obbligatorio**

Il tipo per ogni elemento dell'insieme (p.e. ``text``, ``choice``,
ecc.). Per esempio, con un array di indirizzi email, si userebbe il tipo
:doc:`email</reference/forms/types/email>`. Se si vuole includere un insieme di
un qualche altro form, creare una nuova istanza del tipo di form e passarlo
in questa opzione.

Opzioni ereditate
-----------------

Queste opzioni sono ereditate dal tipo :doc:`form </reference/forms/types/form>`.
Non sono elencate tutte le opzioni, solo quelle più attinenti a questo
tipo:

.. _reference-form-types-by-reference:

.. include:: /reference/forms/types/options/by_reference.rst.inc

.. include:: /reference/forms/types/options/cascade_validation.rst.inc

.. include:: /reference/forms/types/options/empty_data.rst.inc
    :end-before: DEFAULT_PLACEHOLDER

Il valore predefinito è ``array()`` (array vuoto).

.. include:: /reference/forms/types/options/empty_data.rst.inc
    :start-after: DEFAULT_PLACEHOLDER

error_bubbling
~~~~~~~~~~~~~~

**tipo**: ``Booleano`` **predefinito**: ``true``

.. include:: /reference/forms/types/options/_error_bubbling_body.rst.inc

.. include:: /reference/forms/types/options/error_mapping.rst.inc

.. include:: /reference/forms/types/options/label.rst.inc

.. include:: /reference/forms/types/options/label_attr.rst.inc

.. include:: /reference/forms/types/options/mapped.rst.inc

.. include:: /reference/forms/types/options/required.rst.inc

Variabili di campo
------------------

============ ============ =======================================
Variabile    Tipo         Uso
============ ============ =======================================
allow_add    ``Booleano`` Il valore dell'opzione `allow_add`_.
allow_delete ``Booleano`` Il valore dell'opzione `allow_delete`_.
============ ============ =======================================
