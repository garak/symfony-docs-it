.. index::
   single: Form; Riferimento delle funzioni per i form in Twig

Riferimento delle funzioni per i form nei template Twig
=======================================================

Questo manuale di riferimento copre tutte le possibili funzioni di Twig disponibili
per rendere i form. Ci sono diverse funzioni disponibili e ognuna è responsabile
della resa di diverse parti di un form (p.e. label, errori, widget,
eccetera).

form_label(form.name, label, variables)
---------------------------------------

Rende la label per un dato campo. Si può opzionalmente passare la label
specifica che si vuole mostrare, come secondo parametro.

.. code-block:: jinja

    {{ form_label(form.name) }}

    {# Le seguenti due sintassi sono equivalenti #}
    {{ form_label(form.name, 'Your Name', { 'attr': {'class': 'foo'} }) }}
    {{ form_label(form.name, null, { 'label': 'Your name', 'attr': {'class': 'foo'} }) }}

form_errors(form.name)
----------------------

Renders any errors for the given field.

.. code-block:: jinja

    {{ form_errors(form.name) }}

    {# rende tutti gli errori "globali" #}
    {{ form_errors(form) }}

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

form_row(form.name, variables)
------------------------------

Rende la "riga" di un dato campo, cioè la combinazione di label, errori e widget
del campo.

.. code-block:: jinja

    {# rende la riga di un campo, ma con label "pippo" #}
    {{ form_row(form.name, { 'label': 'pippo' }) }}

Il secondo parametro di ``form_row`` è un array di variabili. I template forniti
in Symfony consentono solo di sovrascrivere la label come mostrato nell'esempio
precedente.

form_rest(form, variables)
--------------------------

Rende tutti i campi che non sono ancora stati resi nel form dato. È sempre una
buona idea averlo da qualche parte nel proprio form, perché renderà i campi
nascosti, oltre a tutti i campi che sono stati
dimenticati.

.. code-block:: jinja

    {{ form_rest(form) }}

form_enctype(form)
------------------

Se il form contiene almeno un campo di caricamento file, renderà l'attributo
obbligatorio ``enctype="multipart/form-data"``. È sempre una buona idea includerlo
nel tag del proprio form:

.. code-block:: html+jinja

    <form action="{{ path('form_submit') }}" method="post" {{ form_enctype(form) }}>