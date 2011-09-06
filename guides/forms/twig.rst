.. index::
   pair: Forms; Twig

I form nei template Twig
========================

Un :doc:`Form </guides/forms/overview>` di Symfony2 è fatto di campi. I campi
descrivono la forma semantica, non la loro rappresentazione per l'utente finale;
questo significa che un form non è legato all'HTML. Al contrario, è responsabilità
del grafico web la visualizzazione di ogni campo del form nel modo che vuole. Quindi
in Symfony2 la visualizzazione di un form in un template, può essere fatta
manualmente in modo semplice. Ma Twig facilita l'integrazione e la personalizzazione
dei form, fornendo una raccolta di filtri che possono essere applicati al foorm e
alle istanze dei campi.

Visualizzare "manualmente" un form
----------------------------------

Prima di addentrarsi nei filtri di Twig e vedere come possono aiutare a visualizzare
i form, in modo facile, sicuro e veloce, bisogna sapere che niente di speciale accade sotto
il cofano. Per visualizzare un form di Symfony2, è possibile utilizzare un qualsiasi codice HTML:

.. code-block:: html

    <form action="#" method="post">
        <input type="text" name="name" />

        <input type="submit" />
    </form>

Se c'è un errore di validazione, bisognerebbe visualizzarlo e compilare i campi con
i valori inviati, in modo da rendere più facile risolvere velocemente i problemi. Basta
usare i metodi dedicati ai form:

.. code-block:: jinja

    <form action="#" method="post">
        <ul>
            {% for error in form.name.errors %}
                <li>{{ error.0 }}</li>
            {% endfor %}
        </ul>
        <input type="text" name="name" value="{{ form.name.data }}" />

        <input type="submit" />
    </form>

I filtri di Twig aiutano a mantenere più corto il codice nei template,
rendono la visualizzazione del form facilmente personalizzabile, supportano
l'internazionalizzazione, la protezione CSRF, l'upload dei file e altro
out of the box. Le sezioni seguenti descrivono tutto quello che c'è da sapere.

Visualizzare un form
--------------------

Poiché la struttura globale di un form (il tag form, il pulsante di invio, ...)
non è definita dall'istanza del form, si è liberi di utilizzare il codice HTML che si desidera.
Un semplice template per un form, appare come segue:

.. code-block:: html

    <form action="#" method="post">
        <!-- Display the form fields -->

        <input type="submit" />
    </form>

Oltre alla struttura globale del form, è necessario un modo per visualizzare gli errori
a livello globale e i campi nascosti; questo è il lavoro rispettivamente dei filtri
``render_errors``e ``render_hidden``:

.. code-block:: jinja

    <form action="#" method="post">
        {{ form|render_errors }}

        <!-- Visualizza i campi del form -->

        {{ form|render_hidden }}
        <input type="submit" />
    </form>

.. note::
   Per impostazione predefinita, ``render_errors`` genera una lista ``<ul>``, ma
   come si vedrà più avanti, può essere facilmente personalizzato.

Ultimo ma non meno importante, un form contenente un input di tipo file, deve avere
l'attributo ``enctype``; utilizzare il filtro ``render_enctype`` per tenere conto di questo:

.. code-block:: jinja

    <form action="#" {{ form|render_enctype }} method="post">

Visualizzare i campi
--------------------

Accedere ai campi di un form è facile, perché Symfony2 agisce sui form con la sintassi degli array:

.. code-block:: jinja

    {{ form.title }}

    {# accede ad un campo (first_name) nidificato in un gruppo (user) #}
    {{ form.user.first_name }}

Essendo che ogni campo è una istanza di Field, non può essere visualizzato come mostrato sopra;
bisogna usare invece uno dei filtri per il campo.

Il filtro ``render_widget`` visualizza una rappresentazione HTML di un campo:

.. code-block:: jinja

    {{ form.title|render_widget }}

.. note::
   Il widget dei campi è selezionato in base al nome della classe del campo (sotto
   ci sono maggiori informazioni).

``render_label`` visualizza il tag ``<label>`` associato con il campo:

.. code-block:: jinja

    {{ form.title|render_label }}

Per impostazione predefinita, Symfony2 "umanizza" il nome del campo, ma si può anche
dare la propria etichetta:

.. code-block:: jinja

    {{ form.title|render_label('Dammi un titolo') }}

.. note::
   Symfony2 internazionalizza automaticamente tutte le etichette e i messaggi di errore.

Il filtro ``render_errors`` visualizza gli errori del campo:

.. code-block:: jinja

    {{ form.title|render_errors }}

.. tip::
   Il filtro ``render_errors`` può essere usato su un form o su un campo.

È anche possibile ottenere i dati associati con il campo (i dati predefiniti o i
dati inviati dall'utente), attraverso il filtro ``render_data``:

.. code-block:: jinja

    {{ form.title|render_data }}

    {{ form.created_at|render_data|date('Y-m-d') }}

Definire la rappresentazione HTML
---------------------------------

Tutti i filtri si basano sui blocchi template di Twig per la visualizzazione HTML.
Per impostazione predefinita, Symfony2 viene distribuito con due template che definiscono
tutti i blocchi necessari, uno per le istanze dei form (``form.twig``) e uno per le istanze
dei campi (``widgets.twig``).

Ogni filtro è associato a un blocco di template. Ad esempio, il
filtro ``render_errors`` cerca un blocco ``errors``. Quello predefinito
è del tipo:

.. code-block:: jinja

    {# TwigBundle::form.twig #}

    {% block errors %}
        {% if errors %}
        <ul>
            {% for error in errors %}
                <li>{% trans error.0 with error.1 from validators %}</li>
            {% endfor %}
        </ul>
        {% endif %}
    {% endblock errors %}

La tabella seguente mostra un elenco completo dei filtri e dei relativi blocchi associati:

================= ==================
Filtro            Nome blocco
================= ==================
``render_errors`` ``errors``
``render_hidden`` ``hidden``
``render_label``  ``label``
``render``        ``group`` o ``field`` (vedere sotto)
================= ==================

Il filtro ``render_widget`` è un po' diverso, in quanto sceglie il blocco da
visualizzare in base alla versione con sottolineatura del nome della classe
del campo. Per esempio, cerca per un blocco ``input_field`` quando
deve visualizzare una istanza di ``InputField``:

.. code-block:: jinja

    {# TwigBundle::widgets.twig #}

    {% block input_field %}
        {% tag "input" with attributes %}
    {% endblock input_field %}

Se il blocco non esiste, il filtro cerca un blocco per una delle
classi genitrici del campo. Ecco perché non vi è alcun blocco predefinito
``password_field``, dal momento che la sua rappresentazione è esattamente la
stessa della sua classe padre (``input_field``).

Personalizzare la rappresentazione di un campo
----------------------------------------------

Il modo più semplice per personalizzare un campo è quello di passare attributi HTML
personalizzati come argomento di ``render_widget``:

.. code-block:: jinja

    {{ form.title|render_widget(['class': 'important']) }}

Se si vuole sovrascrivere completamente la rappresentazione HTML di un widget, passare
un template Twig che definisce il necessario blocco template:

.. code-block:: jinja

    {{ form.title|render_widget([], 'HelloBundle::widgets.twig') }}

``HelloBundle::widgets.twig`` è un normale template Twig contenente blocchi
che definiscono la rappresentazione HTML per i widget che si vogliono sovrascrivere:

.. code-block:: jinja

    {# HelloBundle/Resources/views/widgets.twig #}

    {% block input_field %}
        <div class="input_field">
            {% tag "input" with attributes %}
        </div>
    {% endblock input_field %}

In questo esempio, il blocco ``input_field`` viene ridefinito. Invece di cambiare
la rappresentazione predefinita, si può anche estendere quella predefinita utilizzando
la caratteristica nativa di ereditarietà di Twig:

.. code-block:: jinja

    {# HelloBundle/Resources/views/widgets.twig #}

    {% extends 'TwigBundle::widgets.twig' %}

    {% block date_time_field %}
        <div class="important_date_field">
            {% parent %}
        </div>
    {% endblock date_time_field %}

Se si vogliono personalizzare tutti i campi di un dato form, usare il tag ``form_theme``:

.. code-block:: jinja

    {% form_theme form 'HelloBundle::widgets.twig' %}

Ogni volta che si chiama il filtro ``render_widget`` sul form, dopo questa chiamata
Symfony2 cercherà una rappresentazione nel template, prima di ricadere su
quella predefinita.
 
Se i blocchi widget sono definiti in diversi template, si possono aggiungere come
array ordinati:

.. code-block:: jinja

    {% form_theme form ['HelloBundle::form.twig', 'HelloBundle::widgets.twig', 'HelloBundle::hello_widgets.twig'] %}

Un tema può essere collegato a un intero modulo (come sopra) o solo ad un gruppo di campi:

.. code-block:: jinja

    {% form_theme form.user 'HelloBundle::widgets.twig' %}

In ultimo, la personalizzazione della rappresentazione di tutti i form di una applicazione, è
possibile attraverso la configurazione:

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        twig.config:
            form:
                resources: [BlogBundle::widgets.twig]

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <twig:config>
            <twig:form>
                <twig:resource>BlogBundle::widgets.twig</twig:resource>
            </twig:form>
        </twig:config>

    .. code-block:: php

        // app/config/config.php
        $container->loadFromExtension('twig', 'config', array('form' => array(
            'resources' => array('BlogBundle::widgets.twig'),
        )));

Prototipazione
--------------

Quando si vuole realizzare il prototipo di un form, si può usare il filtro ``render``
invece di visualizzare manualmente tutti i campi:

.. code-block:: jinja

    <form action="#" {{ form|render_enctype }} method="post">
        {{ form|render }}
        <input type="submit" />
    </form>

Il filtro ``render`` può essere anche usato per visualizzare la "riga" di un campo:

.. code-block:: jinja

    <form action="#" {{ form|render_enctype }} method="post">
        {{ form|render_errors }}
        <table>
            {{ form.first_name|render }}
            {{ form.last_name|render }}
        </table>
        {{ form|render_hidden }}
        <input type="submit" />
    </form>

Il filtro ``render`` per la visualizzazione utilizza i blocchi ``group`` e ``field``:

.. code-block:: jinja

    {# TwigBundle::form.twig #}

    {% block group %}
        {{ group|render_errors }}
        <table>
            {% for field in group %}
                {% if not field.ishidden %}
                    {{ field|render }}
                {% endif %}
            {% endfor %}
        </table>
        {{ group|render_hidden }}
    {% endblock group %}

    {% block field %}
        <tr>
            <th>{{ field|render_label }}</th>
            <td>
                {{ field|render_errors }}
                {{ field|render_widget }}
            </td>
        </tr>
    {% endblock field %}

Così come ogni altro filtro, ``render`` accetta un template come argomento,
per sovrascrivere la rappresentazione predefinita:

.. code-block:: jinja

    {{ form|render("HelloBundle::form.twig") }}

.. caution::
    Il filtro ``render`` non è molto flessibile e dovrebbe essere usato solo
    per costruire prototipi.
