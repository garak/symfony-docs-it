.. index::
   single: Form; Riferimento delle funzioni per i form in Twig

Riferimento delle funzioni per i form nei template Twig
=======================================================

Questo manuale di riferimento copre tutte le possibili funzioni di Twig disponibili
per rendere i form. Ci sono diverse funzioni disponibili e ognuna è responsabile
della resa di diverse parti di un form (p.e. label, errori, widget,
eccetera).

.. _reference-forms-twig-label:

form_label(form.name, label, variables)
---------------------------------------

Rende la label per un dato campo. Si può opzionalmente passare la label
specifica che si vuole mostrare, come secondo parametro.

.. code-block:: jinja

    {{ form_label(form.name) }}

    {# Le seguenti due sintassi sono equivalenti #}
    {{ form_label(form.name, 'Il tuo nome', {'attr': {'class': 'foo'}}) }}
    {{ form_label(form.name, null, {'label': 'Il tuo nome', 'attr': {'class': 'foo'}}) }}

Vedere ":ref:`twig-reference-form-variables`" per saperne di più sul parametro
``variables``.

.. _reference-forms-twig-errors:

form_errors(form.name)
----------------------

Renders any errors for the given field.

.. code-block:: jinja

    {{ form_errors(form.name) }}

    {# rende tutti gli errori "globali" #}
    {{ form_errors(form) }}

.. _reference-forms-twig-widget:

form_widget(form.name, variables)
---------------------------------

Rende il widget HTML del campo dato. Se lo si applica all'intero form o a un
insieme di campi, ogni riga di form sottostante sarà resa.

.. code-block:: jinja

    {# rende un widget, ma gli aggiunge la classe "pippo" #}
    {{ form_widget(form.name, { 'attr': {'class': 'pippo'} }) }}

Il secondo parametro di ``form_widget`` è un array di variabili. La variabile più
comune è ``attr``, che è un array di attributi HTML da applicare al widget.
In alcuni casi, certi tipi hanno anche altre opzioni legate ai template, che possono
essere passate. Tali opzioni saranno discusse per ogni singolo tipo.
Gli attributi sono applicati ricorsivamente ai campi figli, se si stanno
rendendo molti campi contemporaneamente (p.e. ``form_widget(form)``).

Vedere ":ref:`twig-reference-form-variables`" per saperne di più sul parametro
``variables``.

.. _reference-forms-twig-row:

form_row(form.name, variables)
------------------------------

Rende la "riga" di un dato campo, cioè la combinazione di label, errori e widget
del campo.

.. code-block:: jinja

    {# rende la riga di un campo, ma con label "pippo" #}
    {{ form_row(form.name, {'label': 'pippo'}) }}

Il secondo parametro di ``form_row`` è un array di variabili. I template forniti
in Symfony consentono solo di sovrascrivere la label come mostrato nell'esempio
precedente.

Vedere ":ref:`twig-reference-form-variables`" per saperne di più sul parametro
``variables``.

.. _reference-forms-twig-rest:

form_rest(form, variables)
--------------------------

Rende tutti i campi che non sono ancora stati resi nel form dato. È sempre una
buona idea averlo da qualche parte nel proprio form, perché renderà i campi
nascosti, oltre a tutti i campi che sono stati
dimenticati.

.. code-block:: jinja

    {{ form_rest(form) }}

.. _reference-forms-twig-enctype:

form_enctype(form)
------------------

Se il form contiene almeno un campo di caricamento file, renderà l'attributo
obbligatorio ``enctype="multipart/form-data"``. È sempre una buona idea includerlo
nel tag del proprio form:

.. code-block:: html+jinja

    <form action="{{ path('form_submit') }}" method="post" {{ form_enctype(form) }}>

.. _`twig-reference-form-variables`:

Approfondimento sulle variabili dei form
----------------------------------------

In quasi tutte le funzioni di Twig viste in precedenza, l'ultimo parametro è un array
di variabili, che vengono usato per rendere una parte di un form. Per esempio, il
seguente renderà un "widget" per un campo, modificando i suoi attributi per
includere una classe particolare:

.. code-block:: jinja

    {# rende un widget, ma con una classe "pippo" #}
    {{ form_widget(form.name, { 'attr': {'class': 'pippo'} }) }}

Lo scopo di queste variabili, cosa fanno e da dove vengono, potrebbe non essere
immediatamente chiaro, ma sono incredibilmente potenti. Ogni volta che si rende
una parte di un form, il blocco che la rende fa uso di un certo numero di
variabili. Per impostazione predefinita, questi blocchi si trovano dentro `form_div_layout.html.twig`_.

Vediamo ``form_label`` come esempio:

.. code-block:: jinja

    {% block generic_label %}
        {% if required %}
            {% set attr = attr|merge({'class': attr.class|default('') ~ ' required'}) %}
        {% endif %}
        <label{% for attrname,attrvalue in attr %} {{attrname}}="{{attrvalue}}"{% endfor %}>{{ label|trans }}</label>
    {% endblock %}

Questo blocco usa tre variabili: ``required``, ``attr`` e ``label``.
Tali variabili sono rese disponibili dal sistema di resa dei form. Ma, più
importante, sono le variabili che si possono ridefinire durante la chiamata a ``form_label``
(poiché, in questo esempio, stiamo rendendo una label).

Le variabili esatte da ridefinire dipendono da quale parte del form si sta
rendendo (p.e. label o widget) e da quale campo si sta rendendo
(p.e. un widget ``choice`` ha un'opzione extra ``expanded``). Se ci si trova a proprio
agio guardando in `form_div_layout.html.twig`_, si potrà sempre vedere
quali opzioni sono disponibili.

.. tip::

    Dietro le quinte, queste variabili sono messe a disposizione dall'oggetto ``FormView``
    del form, quando il componente Form richiama ``buildView`` e ``buildViewBottomUp``
    su ogni "nodo" dell'albero del form. Per vedere quali variabili "view" ha un
    particolare campo, trovare il codice sorgente per tale campo (e i suoi campi genitori)
    e vedere le due funzioni di cui sopra.

.. note::

    Se si sta rendendo un intero form in una volta sola (oppure un intero form incluso),
    il parametro ``variables`` si applicherà solamente al form stesso, non ai
    suoi figli. In altre parole, il seguente **non** passerà una classe "pippo"
    a tutti i suoi campi figli nel form:

    .. code-block:: jinja

        {# **non** funziona, le variabili non sono ricorsive #}
        {{ form_widget(form, { 'attr': {'class': 'pippo'} }) }}

.. _`form_div_layout.html.twig`: https://github.com/symfony/symfony/blob/2.1/src/Symfony/Bridge/Twig/Resources/views/Form/form_div_layout.html.twig
