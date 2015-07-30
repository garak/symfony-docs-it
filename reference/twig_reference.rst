.. index::
    single: Estensioni Twig di Symfony

.. _symfony2-twig-extensions:

Estensioni Twig di Symfony
==========================

Twig è il motore di template predefinito di Symfony. Contiene già di suo diverse
funzioni di libreria, filtri e tag e test (si può
approfondire nella `guida a Twig`_).

Symfony aggiunge ulteriori estensioni personalizzate a Twig, per integrare alcuni
componenti nei template di Twig. Di seguito sono presenti le informazioni su tutte le
:ref:`funzioni <reference-twig-functions>`, i :ref:`filtri <reference-twig-filters>`,
i :ref:`tag <reference-twig-tags>` e :ref:`test <reference-twig-tests>`
aggiunti nel framework Symfony.

Ci sono anche alcuni tag nei bundle, non elencati qui.

.. _reference-twig-functions:

Funzioni
--------

.. _reference-twig-function-render:

render
~~~~~~

.. versionadded:: 2.2
    La funzione ``render()`` è stata introdotta in Symfony 2.2. Precedentemente,
    si usava il tag ``{% render %}``, che aveva una firma diversa.

.. code-block:: jinja

    {{ render(uri, opzioni) }}

``uri``
    **tipo**: ``stringa`` | ``ControllerReference``
``opzioni``
    **tipo**: ``array`` **predefinito**: ``[]``

Renderà il frammento per il controllore o l'URL dato.
Per maggiori informazioni, vedere :ref:`templating-embedding-controller`.

Si può specificare la strategia di resa nella chiave ``strategy`` delle opzioni.

.. tip::

    Si può generare l'URI con altre funzioni, come `path`_ e `url`_.

.. _reference-twig-function-render-esi:

render_esi
~~~~~~~~~~

.. code-block:: jinja

    {{ render_esi(uri, opzioni) }}

``uri``
    **tipo**: ``stringa`` | ``ControllerReference``
``opzioni``
    **tipo**: ``array`` **predefinito**: ``[]``

Genera un tag ESI, se possibile, altrimenti si comporta come la funzione
`render`_. Per maggiori informazioni, vedere
:ref:`templating-embedding-controller`.

.. tip::

    Si può generare l'URI con altre funzioni, come `path`_ e `url`_.

.. tip::

    La funzione ``render_esi()`` è un esempio di funzioni scorciatoia
    di ``render``. Imposta automaticamente la strategia in base al
    nome della funzione, p.e. ``render_hinclude()`` userà la strategia hinclude.js.
    Questo vale per tutte le funzioni ``render_*()``.

controller
~~~~~~~~~~

.. versionadded:: 2.2
    La funzione ``controller()`` è stata introdotta in Symfony 2.2.

.. code-block:: jinja

    {{ controller(controllore, attributi, query) }}

``controllore``
    **tipo**: ``stringa``
``attributi``
    **tipo**: ``array`` **predefinito**: ``[]``
``query``
    **tipo**: ``array`` **predefinito**: ``[]``

Restituisce un'istanza di ``ControllerReference``, da usare con funzioni come
:ref:`render() <reference-twig-function-render>` e 
:ref:`render_esi() <reference-twig-function-render-esi>`.

asset
~~~~~

.. code-block:: jinja

    {{ asset(percorso, nomePacchetto) }}

``percorso``
    **tipo**: ``stringa``
``nomePacchetto``
    **tipo**: ``stringa``|``null`` **predefinito**: ``null``

Restituisce un percorso pubblico a ``percorso``, che prende in considerazione il percorso base
impostato per il pacchetto e il percorso dell'URL. Maggiori informazioni su
:ref:`book-templating-assets`.

asset_version
~~~~~~~~~~~~~

.. code-block:: jinja

    {{ asset_version(nomePacchetto) }}

``nomePacchetto``
    **tipo**: ``stringa``|``null`` **predefinito**: ``null``

Restituisce la versione attuale del pacchetto, maggiori informazioni su
:ref:`book-templating-assets`.

form
~~~~

.. code-block:: jinja

    {{ form(vista, variabili) }}

``view``
    **tipo**: ``FormView``
``variables``
    **tipo**: ``array`` **predefinito**: ``[]``

Rende l'HTML di un form completo, maggiori informazioni sulla
:ref:`guida a Twig Form <reference-forms-twig-form>`.

form_start
~~~~~~~~~~

.. code-block:: jinja

    {{ form_start(view, variables) }}

``view``
    **tipo**: ``FormView``
``variables``
    **tipo**: ``array`` **predefinito**: ``[]``

Rende il tag HTML di apertura di un form, maggiori informazioni sulla
:ref:`guida a Twig Form <reference-forms-twig-start>`.

form_end
~~~~~~~~

.. code-block:: jinja

    {{ form_end(view, variables) }}

``view``
    **tipo**: ``FormView``
``variables``
    **tipo**: ``array`` **predefinito**: ``[]``

Rende il tag HTML di chiusura del form, insieme a tutti i campi che non sono ancora stati
resi, maggiori informazioni sulla :ref:`guida a Twig Form <reference-forms-twig-end>`.

form_enctype
~~~~~~~~~~~~

.. code-block:: jinja

    {{ form_enctype(view) }}

``view``
    **tipo**: ``FormView``

Rende l'attributo ``enctype="multipart/form-data"``, necessario se il form
contiene almeno un campo di caricamento file, maggiori informazioni sulla
:ref:`guida a Twig Form <reference-forms-twig-enctype>`.

form_widget
~~~~~~~~~~~

.. code-block:: jinja

    {{ form_widget(view, variables) }}

``view``
    **tipo**: ``FormView``
``variables``
    **tipo**: ``array`` **predefinito**: ``[]``

Rende un form completo o uno specifico widget HTML di un campo, maggiori informazioni
sulla :ref:`guida a Twig Form <reference-forms-twig-widget>`.

form_errors
~~~~~~~~~~~

.. code-block:: jinja

    {{ form_errors(view) }}

``view``
    **tipo**: ``FormView``

Rende gli errori di un dato campo o gli errori globali, maggiori informazioni
sulla :ref:`guida a Twig Form <reference-forms-twig-errors>`.

form_label
~~~~~~~~~~

.. code-block:: jinja

    {{ form_label(view, label, variabili) }}

``view``
    **tipo**: ``FormView``
``label``
    **tipo**: ``stringa`` **predefinito**: ``null``
``variabili``
    **tipo**: ``array`` **predefinito**: ``[]``

Rende the label for the given field, mre information in
:ref:`guida a Twig Form <reference-forms-twig-label>`.

form_row
~~~~~~~~

.. code-block:: jinja

    {{ form_row(view, variabili) }}

``view``
    **tipo**: ``FormView``
``variabili``
    **tipo**: ``array`` **predefinito**: ``[]``

Rende la riga (label, errori e widget) del campo dato, maggiori
informazioni sulla :ref:`guida a Twig Form <reference-forms-twig-row>`.

form_rest
~~~~~~~~~

.. code-block:: jinja

    {{ form_rest(view, variabili) }}

``view``
    **tipo**: ``FormView``
``variabili``
    **tipo**: ``array`` **predefinito**: ``[]``

Rende tutti campi non ancora resi, maggiori informazioni sulla
:ref:`guida a Twig Form <reference-forms-twig-rest>`.

csrf_token
~~~~~~~~~~

.. code-block:: jinja

    {{ csrf_token(intenzione) }}

``intenzione``
    **tipo**: ``stringa``

Rende un token CSRF. Usare questa funzione se si vuole protezione CSRF senza
creare un form.

is_granted
~~~~~~~~~~

.. code-block:: jinja

    {{ is_granted(ruolo, oggetto, campo) }}

``ruolo``
    **tipo**: ``stringa``
``oggetto``
    **tipo**: ``object``
``campo``
    **tipo**: ``stringa``

Restituisce ``true`` se l'utente corrente ha il ruolo richiesto. Si può anche passare un
oggetto, che verrà usato dal votante. Maggiori informazioni su
:ref:`book-security-template`.

.. note::

    Si può anche passare il campo per usare un ACE per uno specifico campo. Approfondire
    su :ref:`cookbook-security-acl-field_scope`.


logout_path
~~~~~~~~~~~

.. code-block:: jinja

    {{ logout_path(chiave) }}

``chiave``
    **tipo**: ``stringa``

Genera un URL relativo di logout per il firewall dato.

logout_url
~~~~~~~~~~

.. code-block:: jinja

    {{ logout_url(chiave) }}

``chiave``
    **tipo**: ``stringa``

Uguale alla funzione `logout_path`_, ma genera un URL assoluto
invece che relativo.

path
~~~~

.. code-block:: jinja

    {{ path(nome, parametri, relativo) }}

``nome``
    **tipo**: ``stringa``
``parametri``
    **tipo**: ``array`` **predefinito**: ``[]``
``relativo``
    **tipo**: ``booleano`` **predefinito**: ``false``

Restituisce l'URL relativo (senza schema e host) per la rotta data. Se
``relative`` è abilitato, crea un percorso relativo al percorso attuale. Maggiori
informazioni su :ref:`book-templating-pages`.

url
~~~

.. code-block:: jinja

    {{ url(nome, parametri, schemaRelativo) }}

``nome``
    **tipo**: ``stringa``
``parametri``
    **tipo**: ``array`` **predefinito**: ``[]``
``schemaRelativo``
    **tipo**: ``booleano`` **predefinito**: ``false``

Restituisce l'URL assoluto (con schema e host) per la rotta data. Se
``schemaRelativo`` è abilitato, crea un URL relativo allo schema. Maggiori
informazioni su :ref:`book-templating-pages`.

.. _reference-twig-filters:

Filtri
------

humanize
~~~~~~~~

.. versionadded:: 2.1
    Il filtro ``humanize`` è stato introdotto in Symfony 2.1

.. code-block:: jinja

    {{ testo|humanize }}

``testo``
    **tipo**: ``stringa``

Rende leggibile a un umano un nome tecnico (cioè sostituisce i trattini bassi con spazi e
mette in maiuscolo le stringhe).

trans
~~~~~

.. code-block:: jinja

    {{ messaggio|trans(parametri, dominio, locale) }}

``messaggio``
    **tipo**: ``stringa``
``parametri``
    **tipo**: ``array`` **predefinito**: ``[]``
``dominio``
    **tipo**: ``stringa`` **predefinito**: ``null``
``locale``
    **tipo**: ``stringa`` **predefinito**: ``null``

Traduce il testo nella lingua attuale. Maggiori informazioni su
:ref:`Translation Filters <book-translation-filters>`.

transchoice
~~~~~~~~~~~

.. code-block:: jinja

    {{ message|transchoice(conteggio, parametri, dominio, locale) }}

``message``
    **tipo**: ``stringa``
``conteggio``
    **tipo**: ``intero``
``parametri``
    **tipo**: ``array`` **predefinito**: ``[]``
``dominio``
    **tipo**: ``stringa`` **predefinito**: ``null``
``locale``
    **tipo**: ``stringa`` **predefinito**: ``null``

Traduce il testo con supporto alla pluralizzazione. Maggiori informazioni su
:ref:`Translation Filters <book-translation-filters>`.

yaml_encode
~~~~~~~~~~~

.. code-block:: jinja

    {{ input|yaml_encode(inline, dumpObjects) }}

``input``
    **tipo**: ``mixed``
``inline``
    **tipo**: ``intero`` **predefinito**: ``0``
``dumpObjects``
    **tipo**: ``booleano`` **predefinito**: ``false``

Trasforma l'input in sintassi YAML. Vedere :ref:`components-yaml-dump` per maggiori
informazioni.

yaml_dump
~~~~~~~~~

.. code-block:: jinja

    {{ value|yaml_dump(inline, dumpObjects) }}

``value``
    **tipo**: ``mixed``
``inline``
    **tipo**: ``intero`` **predefinito**: ``0``
``dumpObjects``
    **tipo**: ``booleano`` **predefinito**: ``false``

Fa lo stesso di `yaml_encode() <yaml_encode>`_, ma include il tipo
nell'output.

abbr_class
~~~~~~~~~~

.. code-block:: jinja

    {{ classe|abbr_class }}

``classe``
    **tipo**: ``stringa``

Genera un elemento ``<abbr>`` con il nome breve di una classe PHP (il nome FQCN
sarà mostrato in un tooltip quando l'utente va sopra all'elemento).

abbr_method
~~~~~~~~~~~

.. code-block:: jinja

    {{ metodo|abbr_method }}

``metodo``
    **tipo**: ``stringa``

Genera un elemento ``<abbr>`` usando la sintassi ``FQCN::method()``. Se ``metodo``
è una ``Closure``, verrà invece usata la ``Closure`` e se ``metodo`` non ha un
nome di classe, è mostrato come una funzione (``metodo()``).

format_args
~~~~~~~~~~~

.. code-block:: jinja

    {{ parametri|format_args }}

``parametri``
    **tipo**: ``array``

Genera una stringa con i parametri e i rispettivi tipi (in elementi ``<em>``).

format_args_as_text
~~~~~~~~~~~~~~~~~~~

.. code-block:: jinja

    {{ parametri|format_args_as_text }}

``parametri``
    **tipo**: ``array``

Uguale al filtro `format_args`_, ma non usa tag.

file_excerpt
~~~~~~~~~~~~

.. code-block:: jinja

    {{ file|file_excerpt(riga) }}

``file``
    **tipo**: ``stringa``
``riga``
    **tipo**: ``intero``

Genera un estratto di 7 righe attorno alla riga data.

format_file
~~~~~~~~~~~

.. code-block:: jinja

    {{ file|format_file(riga, testo) }}

``file``
    **tipo**: ``stringa``
``riga``
    **tipo**: ``intero``
``testo``
    **tipo**: ``stringa`` **predefinito**: ``null``

Genera il percorso del file in un elemento ``<a>``. Se il percorso è all'interno della
cartella radice del kernel, il percorso della cartella radice del kernel è sostituito da
``kernel.root_dir`` (mostrando il percorso completo in un tooltip).

format_file_from_text
~~~~~~~~~~~~~~~~~~~~~

.. code-block:: jinja

    {{ testo|format_file_from_text }}

``testo``
    **tipo**: ``stringa``

Usa `|format_file <format_file>` per migliorare l'output predefinito degli errori PHP.

file_link
~~~~~~~~~

.. code-block:: jinja

    {{ file|file_link(riga) }}

``riga``
    **tipo**: ``intero``

Genera un collegamento al file fornito (eventualmente anche alla riga), usando uno
schema preconfigurato.

.. _reference-twig-tags:

Tag
---

form_theme
~~~~~~~~~~

.. code-block:: jinja

    {% form_theme form risorse %}

``form``
    **tipo**: ``FormView``
``risorse``
    **tipo**: ``array``|``stringa``

Imposta le risorse per sovrascrivere il tema del form per l'istanza data della vista del form.
Si può usare ``_self`` come risorse, per impostarli alla risorsa attuale. Maggiori
informazioni su :doc:`/cookbook/form/form_customization`.

trans
~~~~~

.. code-block:: jinja

    {% trans with variabili from dominio into locale %}{% endtrans %}

``variabili``
    **tipo**: ``array`` **predefinito**: ``[]``
``dominio``
    **tipo**: ``stringa`` **predefinito**: ``stringa``
``locale``
    **tipo**: ``stringa`` **predefinito**: ``stringa``

Rende la traduzione del contenuto. Maggiori informazioni su :ref:`book-translation-tags`.

transchoice
~~~~~~~~~~~

.. code-block:: jinja

    {% transchoice conteggio with variabili from dominio into locale %}{% endtranschoice %}

``conteggio``
    **tipo**: ``intero``
``variabili``
    **tipo**: ``array`` **predefinito**: ``[]``
``dominio``
    **tipo**: ``stringa`` **predefinito**: ``null``
``locale``
    **tipo**: ``stringa`` **predefinito**: ``null``

Rende la traduzione del contenuto con supporto alla pluralizzazione, maggiori
informazioni su :ref:`book-translation-tags`.

trans_default_domain
~~~~~~~~~~~~~~~~~~~~

.. code-block:: jinja

    {% trans_default_domain dominio %}

``dominio``
    **tipo**: ``stringa``

Imposta il dominio predefinito nel template attuale.

.. _reference-twig-tests:

Test
----

selectedchoice
~~~~~~~~~~~~~~

.. code-block:: jinja

    {% if choice is selectedchoice(valore) %}

``choice``
    **tipo**: ``ChoiceView``
``valore``
    **tipo**: ``stringa``

Verifica se ``valore`` era impostato per il campo di scelta fornito. L'uso di
questo è il modo più efficiente.

Variabili globali
-----------------

.. _reference-twig-global-app:

app
~~~

La variabile ``app`` è disponibile ovunque e dà accesso rapido a
molti oggetti di uso comune. La variabile ``app`` è un'istanza di
:class:`Symfony\\Bundle\\FrameworkBundle\\Templating\\GlobalVariables`.

The available attributes are:

* ``app.user``
* ``app.request``
* ``app.session``
* ``app.environment``
* ``app.debug``
* ``app.security``

Estensioni di Symfony Standard Edition
--------------------------------------

Symfony Standard Edition aggiunge alcuni bundle al nucleo di Symfony.
Questi bundle possono avere altre estensioni di Twig:

* **Twig Extension** include alcune estensioni interessanti, che non appartengono al nucleo
  di Twig. Si può approfondire nella
  `documentazione ufficiale delle estensioni di Twig`_
* **Assetic** aggiunge i tag ``{% stylesheets %}``, ``{% javascripts %}`` e 
  ``{% image %}``. Si può approfondire nella 
  :doc:`documentazione di Assetic </cookbook/assetic/asset_management>`.

.. _`guida a Twig`: http://twig.sensiolabs.org/documentation#reference
.. _`documentazione ufficiale delle estensioni di Twig`: http://twig.sensiolabs.org/doc/extensions/index.html
