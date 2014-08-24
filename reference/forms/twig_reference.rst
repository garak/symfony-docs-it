.. index::
   single: Form; Riferimento delle funzioni per i form in Twig

Riferimento delle funzioni per i form nei template Twig
=======================================================

Lavorando con i form in un template, ci sono potenti strumenti a
disposizione:

* :ref:`Funzioni<reference-form-twig-functions>` per rendere ogni parte di un form
* :ref:`Variabili<twig-reference-form-variables>` per ottenere *qualsiasi* informazione su un campo

Spesso si useranno funzioni per rendere i campi. Le variabili, d'altro
canto, si usano meno frequentemente, ma hanno un grandissimo potere, perché danno
accesso a label, attributi id, errori e qualsiasi altra cosa riguardo ai campi.

.. _reference-form-twig-functions:

Funzioni per la resa dei form
-----------------------------

Questo manuale di riferimento copre tutte le possibili funzioni di Twig disponibili
per rendere i form. Ci sono diverse funzioni disponibili e ognuna è responsabile
della resa di diverse parti di un form (p.e. label, errori, widget,
eccetera).

.. _reference-forms-twig-form:

form(view, variables)
---------------------

Rende l'HTML di un form completo.

.. code-block:: jinja

    {# rende il form e cambia il metodo di invio #}
    {{ form(form, {'method': 'GET'}) }}

Si userà questo aiutante per lo più per prototipare o se si usano temi personalizzati
di form. Se occorre più flessibilità nella resa del form, bisogna usare
gli altri aiutanti, per rendere invece le singole parti del form:

.. code-block:: jinja

    {{ form_start(form) }}
        {{ form_errors(form) }}

        {{ form_row(form.name) }}
        {{ form_row(form.dueDate) }}

        <input type="submit" value="Submit me"/>
    {{ form_end(form) }}

.. _reference-forms-twig-start:

form_start(view, variables)
---------------------------

Rende il tag iniziale di un form. Questo aiutante si occupa di scrivere il
metodo e il target del form. Includerà anche la
proprietà ``enctype``, nel caso in cui il form contenga caricamenti di file.

.. code-block:: jinja

    {# rende il tag iniziale il metodo di invio #}
    {{ form_start(form, {'method': 'GET'}) }}

.. _reference-forms-twig-end:

form_end(view, variables)
-------------------------

Rende il tag finale di un form.

.. code-block:: jinja

    {{ form_end(form) }}

Questo aiutante include ``form_rest()``, a meno di non aver impostato ``render_rest`` a
``false``:

.. code-block:: jinja

    {# non rendere i campi non ancora resi #}
    {{ form_end(form, {'render_rest': false}) }}

.. _reference-forms-twig-label:

form_label(view, label, variables)
----------------------------------

Rende la label per un dato campo. Si può opzionalmente passare la label
specifica che si vuole mostrare, come secondo parametro.

.. code-block:: jinja

    {{ form_label(form.name) }}

    {# Le seguenti due sintassi sono equivalenti #}
    {{ form_label(form.name, 'Il tuo nome', {'label_attr': {'class': 'foo'}}) }}
    {{ form_label(form.name, null, {'label': 'Il tuo nome', 'label_attr': {'class': 'foo'}}) }}

Vedere ":ref:`twig-reference-form-variables`" per saperne di più sul parametro
``variables``.

.. _reference-forms-twig-errors:

form_errors(view)
-----------------

Renders any errors for the given field.

.. code-block:: jinja

    {{ form_errors(form.name) }}

    {# rende tutti gli errori "globali" #}
    {{ form_errors(form) }}

.. _reference-forms-twig-widget:

form_widget(view, variables)
----------------------------

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

form_row(view, variables)
-------------------------

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

form_rest(view, variables)
--------------------------

Rende tutti i campi che non sono ancora stati resi nel form dato. È sempre una
buona idea averlo da qualche parte nel form, perché renderà i campi
nascosti, oltre a tutti i campi che sono stati
dimenticati.

.. code-block:: jinja

    {{ form_rest(form) }}

.. _reference-forms-twig-enctype:

form_enctype(view)
------------------

.. note::

    Questo aiutante è stato deprecato in Symfony 2.3 e sarà rimosso in Symfony 3.0.
    Usare ``form_start()`` al suo posto.

Se il form contiene almeno un campo di caricamento file, renderà l'attributo
obbligatorio ``enctype="multipart/form-data"``. È sempre una buona idea includerlo
nel tag di un form:

.. code-block:: html+jinja

    <form action="{{ path('form_submit') }}" method="post" {{ form_enctype(form) }}>

Riferimento sui test dei form
-----------------------------

Si possono eseguire test, usando l'operatore ``is`` in Twig per creare una
condizione. Si veda la `documentazione di Twig`_ per maggiori informazioni.

.. _form-twig-selectedchoice:

selectedchoice(selected_value)
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

This test will check if the current choice is equal to the ``selected_value``
or if the current choice is in the array (when ``selected_value`` is an array).

.. code-block:: jinja

    <option {% if choice is selectedchoice(value) %} selected="selected"{% endif %} ...>

.. _`twig-reference-form-variables`:

Approfondimento sulle variabili dei form
----------------------------------------

.. tip::

    Per una lista completa di variabili, vedere: :ref:`reference-form-twig-variables`.

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

    {% block form_label %}
        {% if not compound %}
            {% set label_attr = label_attr|merge({'for': id}) %}
        {% endif %}
        {% if required %}
            {% set label_attr = label_attr|merge({'class': (label_attr.class|default('') ~ ' required')|trim}) %}
        {% endif %}
        {% if label is empty %}
            {% set label = name|humanize %}
        {% endif %}
        <label{% for attrname, attrvalue in label_attr %} {{ attrname }}="{{ attrvalue }}"{% endfor %}>{{ label|trans({}, translation_domain) }}</label>
    {% endblock form_label %}

Questo blocco fa uso di molte variabili: ``compound``, ``label_attr``, ``required``,
``label``, ``name`` e ``translation_domain``.
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

.. _reference-form-twig-variables:

Riferimento sulle variabili dei form
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Le seguenti variabili sono comuni a ogni tipo di campo. Alcuni tipi di campo
possono avere ulteriori variabili, mentre alcune variabili si applicano effettivamente
solo ad alcuni tipi.

Ipotizzando di avare una variabile di nome ``form`` in un template, e che si voglia
riferirsi alle variabili del campo ``name``, l'accesso alle variabili è eseguito tramite
una proprietà pubblica ``vars`` dell'oggetto :class:`Symfony\\Component\\Form\\FormView`:


.. configuration-block::

    .. code-block:: html+jinja

        <label for="{{ form.name.vars.id }}"
            class="{{ form.name.vars.required ? 'required' : '' }}">
            {{ form.name.vars.label }}
        </label>

    .. code-block:: html+php

        <label for="<?php echo $view['form']->get('name')->vars['id'] ?>"
            class="<?php echo $view['form']->get('name')->vars['required'] ? 'required' : '' ?>">
            <?php echo $view['form']->get('name')->vars['label'] ?>
        </label>

.. versionadded:: 2.3
    Le variabili ``method`` e ``action`` sono state introdotte in Symfony 2.3.

.. versionadded:: 2.4
    La variabile ``submitted`` è stata aggiunta in Symfony 2.4.

+------------------------+--------------------------------------------------------------------------------------+
| Variabile              | Uso                                                                                  |
+========================+======================================================================================+
| ``form``               | L'istanza corrente di ``FormView``.                                                  |
+------------------------+--------------------------------------------------------------------------------------+
| ``id``                 | Attributo HTML ``id`` da rendere                                                     |
+------------------------+--------------------------------------------------------------------------------------+
| ``name``               | Nome del campo (p.e. ``title``), ma non l'attributo HTML ``name``,                   |
|                        | che invece è ``full_name``                                                           |
+------------------------+--------------------------------------------------------------------------------------+
| ``full_name``          | Attributo HTML ``name`` da rendere                                                   |
+------------------------+--------------------------------------------------------------------------------------+
| ``errors``             | Un array di errori allegati a *questo* specifico campo (p.e. ``form.title.errors``). |
|                        | Si noti che non si può usare ``form.errors`` per stabilire se un form sia valido,    |
|                        | perché questo restituisce solo gli errori "globali": alcuni singoli campo possono    |
|                        | avere errori. Usare invece l'opzione ``valid``                                       |
|                        |                                                                                      |
+------------------------+--------------------------------------------------------------------------------------+
| ``submitted``          | Restituisce ``true`` o ``false`` a seconda che il form sia stato sottomesso o meno   | 
+------------------------+--------------------------------------------------------------------------------------+
| ``valid``              | ``true`` o ``false``, a seconda che il form sia valido o meno                        |
+------------------------+--------------------------------------------------------------------------------------+
| ``value``              | Valore che sarà usato per la resa (solitamente è l'attributo HTML ``value``)         |
+------------------------+--------------------------------------------------------------------------------------+
| ``read_only``          | Se ``true``, viene aggiunto ``readonly="readonly"`` al campo                         |
+------------------------+--------------------------------------------------------------------------------------+
| ``disabled``           | Se ``true``, viene aggiunto ``disabled="disabled"`` al campo                         |
+------------------------+--------------------------------------------------------------------------------------+
| ``required``           | Se ``true``, viene aggiunto un attributo ``required`` al campo, per attivare la      |
|                        | validazione HTML5. Inoltre, viene aggiunta una classe ``required`` alla label.       |
+------------------------+--------------------------------------------------------------------------------------+
| ``max_length``         | Aggiunge un attributo HTML ``maxlength`` all'elemento (deprecata dalla 2.5, sarà     |
|                        | rimossa nella 3.0, usare invece ``attr["maxlength"]``)                               |
+------------------------+--------------------------------------------------------------------------------------+
| ``pattern``            | Aggiunge un attributo HTML ``pattern`` all'elemento  (deprecata dalla 2.5, sarà      |
|                        | rimossa nella 3.0, usare invece ``attr["pattern"]``)                                 |
+------------------------+--------------------------------------------------------------------------------------+
| ``label``              | La stringa da rendere per la  label                                                  |
+------------------------+--------------------------------------------------------------------------------------+
| ``multipart``          | Se ``true``, ``form_enctype``renderà ``enctype="multipart/form-data"``.              |
|                        | Si applica solo all'elemento form principale.                                        |
+------------------------+--------------------------------------------------------------------------------------+
| ``attr``               | Un array chiave-valore resi come attributi HTML del campo                            |
+------------------------+--------------------------------------------------------------------------------------+
| ``label_attr``         | Un array chiave-valore resi come attributi HTML della label                          |
+------------------------+--------------------------------------------------------------------------------------+
| ``compound``           | Se il campo è effettivamente un contenitore di campi figli                           |
|                        | (per esempio, un campo ``choice``, che effettivamente è un grupo di checkbox         |
+------------------------+--------------------------------------------------------------------------------------+
| ``block_prefixes``     | Un array con tutti i nomi dei tipi genitore.                                         |
+------------------------+--------------------------------------------------------------------------------------+
| ``translation_domain`` | Il dominio di traduzione di questo form.                                             |
+------------------------+--------------------------------------------------------------------------------------+
| ``cache_key``          | Una chiave unica, usata per la cache.                                                |
+------------------------+--------------------------------------------------------------------------------------+
| ``data``               | I dati normalizzati del tipo.                                                        |
+------------------------+--------------------------------------------------------------------------------------+
| ``method``             | Il metodo del form (POST, GET, ecc.).                                                |
+------------------------+--------------------------------------------------------------------------------------+
| ``action``             | L'azione del form.                                                                   |
+------------------------+--------------------------------------------------------------------------------------+

.. _`form_div_layout.html.twig`: https://github.com/symfony/symfony/blob/master/src/Symfony/Bridge/Twig/Resources/views/Form/form_div_layout.html.twig
.. _`documentazione di Twig`: http://twig.sensiolabs.org/doc/templates.html#test-operator
