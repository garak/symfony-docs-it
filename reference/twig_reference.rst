.. index::
    single: Estensioni Twig di Symfony2

Estensioni Twig di Symfony2
===========================

Twig è il motore di template predefinito di Symfony2. Contiene già di suo diverse
funzioni di libreria, filtri e tag (vedere `http://twig.sensiolabs.org/documentation`_,
in fondo alla pagina).

Symfony2 aggiunge ulteriori estensioni personalizzate a Twig, per integrare alcuni
componenti nei template di Twig. Di seguito sono presenti le informazioni su tutte le
funzioni personalizzate, i filtri e i tag aggiunti nel framework Symfony2.

Ci sono anche alcuni tag nei bundle, non elencati qui.

Funzioni
--------

.. versionadded:: 2.2
    Le funzioni ``render`` e ``controller`` sono nuove in Symfony 2.2. Precedentemente,
    si usava il tag ``{% render %}``, che aveva una firma diversa.

+----------------------------------------------------+--------------------------------------------------------------------------------------------+
| Sintassi della funzione                            | Uso                                                                                        |
+====================================================+============================================================================================+
| ``render(uri, options = {})``                      | Renderà il frammento per il controllore o l'URL dato.                                      |
| ``render(controller('B:C:a', {params}))``          | Per maggiori informazioni, vedere :ref:`templating-embedding-controller`.                  |
| ``render(path('rotta', {params}))``                |                                                                                            |
| ``render(url('rotta', {params}))``                 |                                                                                            |
+----------------------------------------------------+--------------------------------------------------------------------------------------------+
| ``render_esi(controller('B:C:a', {params}))``      | Genererà un tag ESI, quando possibile, altrimenti userà ``render``.                        |
| ``render_esi(url('rotta', {params}))``             | Per maggiori informazioni, vedere :ref:`templating-embedding-controller`.                  |
| ``render_esi(path('rotta', {params}))``            |                                                                                            |
+----------------------------------------------------+--------------------------------------------------------------------------------------------+
| ``render_hinclude(controller(...))``               | This will generates an Hinclude tag for the given controller or URL.                       |
| ``render_hinclude(url('rotta', {params}))``        | Per maggiori informazioni, vedere :ref:`templating-embedding-controller`.                  |
| ``render_hinclude(path('rotta', {params}))``       |                                                                                            |
+----------------------------------------------------+--------------------------------------------------------------------------------------------+
| ``controller(attributi = {}, query = {})``         | Usato con il tag ``render`` per fare riferimento al controllore che si vuole rendere.      |
+----------------------------------------------------+--------------------------------------------------------------------------------------------+
| ``asset(percorso, nomePacchetto = null)``          | Restituisce il percorso pubblico della risorsa, maggiori informazioni in                   |
|                                                    | ":ref:`book-templating-assets`".                                                           |
+----------------------------------------------------+--------------------------------------------------------------------------------------------+
| ``asset_version(nomePacchetto = null)``            | Restituisce la versione attuale del pacchetto, maggiori informazioni in                    |
|                                                    | ":ref:`book-templating-assets`".                                                           |
+----------------------------------------------------+--------------------------------------------------------------------------------------------+
| ``form(view, variabili = {})``                     | Renderà l'HTML di un form completo, maggiori informazioni                                  |
|                                                    | nel :ref:`riferimento Form di Twig <reference-forms-twig-form>`.                           |
+----------------------------------------------------+--------------------------------------------------------------------------------------------+
| ``form_start(view, variabili = {})``               | Renderà l'HTML del tag di apertura di un form, maggiori informazioni                       |
|                                                    | nel :ref:`riferimento Form di Twig <reference-forms-twig-start>`.                          |
+----------------------------------------------------+--------------------------------------------------------------------------------------------+
| ``form_end(view, variabili = {})``                 | Renderà l'HTML del tag di chiusura di un form, insieme a tutti i campi non                 |
|                                                    | ancora resi, maggiori informazioni                                                         |
|                                                    | nel :ref:`riferimento Form di Twig <reference-forms-twig-end>`.                            |
+----------------------------------------------------+--------------------------------------------------------------------------------------------+
| ``form_enctype(view)``                             | Renderà l'attributo obbligatorio ``enctype="multipart/form-data"`` in un                   |
|                                                    | form con almeno un campo di caricamento di file, maggiori informazioni in                  |
|                                                    | :ref:`riferimento Twig per i form<reference-forms-twig-enctype>`.                          |
+----------------------------------------------------+--------------------------------------------------------------------------------------------+
| ``form_widget(view, variabili = {})``              | Renderà un intero form o un widget specifico di un campo,                                  |
|                                                    | maggiori informazioni in :ref:`riferimento Twig per i form<reference-forms-twig-widget>`.  |
+----------------------------------------------------+--------------------------------------------------------------------------------------------+
| ``form_errors(view)``                              | Renderà gli errori per un dato campo o gli errori "globali",                               |
|                                                    | maggiori informazioni in :ref:`riferimento Twig per i form<reference-forms-twig-errors>`.  |
+----------------------------------------------------+--------------------------------------------------------------------------------------------+
| ``form_label(view, label = null, variabili = {})`` | Renderà la label di un dato campo, maggiori informazioni in                                |
|                                                    | :ref:`riferimento Twig per i form<reference-forms-twig-label>`.                            |
+----------------------------------------------------+--------------------------------------------------------------------------------------------+
| ``form_row(view, variabili = {})``                 | Renderà la riga (label, errori e widget del campo) del dato campo,                         |
|                                                    | maggiori informazioni in :ref:`riferimento Twig per i form<reference-forms-twig-row>`.     |
+----------------------------------------------------+--------------------------------------------------------------------------------------------+
| ``form_rest(view, variabili = {})``                | Renderà tutti i campi non ancora resi, maggiori informazioni in                            |
|                                                    | :ref:`riferimento Twig per i form<reference-forms-twig-rest>`.                             |
+----------------------------------------------------+--------------------------------------------------------------------------------------------+
| ``csrf_token(intention)``                          | Renderà un token CSRF. Funzione da usare se si vuole protezione CSRF senza                 |
|                                                    | creaew un form                                                                             |
+----------------------------------------------------+--------------------------------------------------------------------------------------------+
| ``is_granted(ruolo, ogg = null, campo = null)``    | Restituirà ``true`` se l'utente attuale ha il ruolo richiesto, maggiori                    |
|                                                    | informazioni in ":ref:`book-security-template`"                                            |
+----------------------------------------------------+--------------------------------------------------------------------------------------------+
| ``logout_path(chiave)``                            | Genererà l'URL relativo per il logout del firewall dato                                    |
+----------------------------------------------------+--------------------------------------------------------------------------------------------+
| ``logout_url(chiave)``                             | Equivalente a ``logout_path(...)``, ma genererà un URL assoluto                            |
+----------------------------------------------------+--------------------------------------------------------------------------------------------+
| ``path(nome, parametri = {})``                     | Restituisce l'URL relativo per la rotta data, maggiori informazioni in                     |
|                                                    | ":ref:`book-templating-pages`".                                                            |
+----------------------------------------------------+--------------------------------------------------------------------------------------------+
| ``url(nome, parametri = {})``                      | Equivalente a ``path(...)``, ma genera un URL assoluto                                     |
+----------------------------------------------------+--------------------------------------------------------------------------------------------+

Filtri
------

+---------------------------------------------------------------------------------+-------------------------------------------------------------------+
| Sintassi del filtro                                                             | Uso                                                               |
+=================================================================================+===================================================================+
| ``text|humanize``                                                               | Rende un nome tecnico leggibile umanamente (sostituendo i         |
|                                                                                 | trattini bassi con spazi e mettendo la stringa in maiuscolo)      |
+---------------------------------------------------------------------------------+-------------------------------------------------------------------+
| ``text|trans(parametri = {}, dominio = 'messages', locale = null)``             | Tradurrà il testo nella lingua attuale, maggiori                  |
|                                                                                 | informazioni in                                                   |
|                                                                                 | :ref:`filtri di traduzione<book-translation-filters>`.            |
+---------------------------------------------------------------------------------+-------------------------------------------------------------------+
| ``text|transchoice(conta, parametri = {}, dominio = 'messages', locale = null)``| Tradurrà il testo con il plurale, maggiori informazioni           |
|                                                                                 | in :ref:`book-translation-twig`.                                  |
+---------------------------------------------------------------------------------+-------------------------------------------------------------------+
| ``variable|yaml_encode(inline = 0)``                                            | Trasformerà il testo della variabile in sintassi YAML.            |
+---------------------------------------------------------------------------------+-------------------------------------------------------------------+
| ``variable|yaml_dump``                                                          | Renderà una sintassi yaml con il suo tipo.                        |
+---------------------------------------------------------------------------------+-------------------------------------------------------------------+
| ``classname|abbr_class``                                                        | Renderà un elemento ``abbr`` con il nome breve di una             |
|                                                                                 | classe PHP.                                                       |
+---------------------------------------------------------------------------------+-------------------------------------------------------------------+
| ``methodname|abbr_method``                                                      | Renderà un metodo PHP dentro un elemento ``abbr``                 |
|                                                                                 | (p.e. ``Symfony\Component\HttpFoundation\Response::getContent``   |
+---------------------------------------------------------------------------------+-------------------------------------------------------------------+
| ``arguments|format_args``                                                       | Renderà una stringa con i parametri di una funzione e i suoi      |
|                                                                                 | tipi.                                                             |
+---------------------------------------------------------------------------------+-------------------------------------------------------------------+
| ``arguments|format_args_as_text``                                               | Equivalente a ``[...]|format_args``, ma elimina i tag.            |
+---------------------------------------------------------------------------------+-------------------------------------------------------------------+
| ``path|file_excerpt(riga)``                                                     | Renderà un estratto di un file di codice intorno alla riga data.  |
+---------------------------------------------------------------------------------+-------------------------------------------------------------------+
| ``path|format_file(riga, testo)``                                               | Renderà il percorso di un file in un collegamento.                |
+---------------------------------------------------------------------------------+-------------------------------------------------------------------+
| ``exceptionMessage|format_file_from_text``                                      | Equivalente a ``format_file``, ma ha analizzato la stringa di     |
|                                                                                 | errore di PHP in un file (p.e. 'in pippo.php on line 45')         |
+---------------------------------------------------------------------------------+-------------------------------------------------------------------+
| ``path|file_link(riga)``                                                        | Renderà un percorso al file (e numero di riga) corretto           |
+---------------------------------------------------------------------------------+-------------------------------------------------------------------+

Tag
---

+---------------------------------------------------+-------------------------------------------------------------------+
| Sintassi del tag                                  | Uso                                                               |
+===================================================+===================================================================+
| ``{% form_theme form 'file' %}``                  | Cercherà in un dato file i blocchi di form ridefiniti,            |
|                                                   | maggiori informazioni in :doc:`/cookbook/form/form_customization`.|
+---------------------------------------------------+-------------------------------------------------------------------+
| ``{% trans with {variabili} %}...{% endtrans %}`` | Tradurrà e renderà il testo, maggiori informazioni in             |
|                                                   | :ref:`book-translation-twig`                                      |
+---------------------------------------------------+-------------------------------------------------------------------+
| ``{% transchoice count with {variabili} %}``      | Tradurrà e renderà il testo con il plurale, maggiori              |
| ...                                               | informazioni in :ref:`book-translation-twig`                      |
| ``{% endtranschoice %}``                          |                                                                   |
+---------------------------------------------------+-------------------------------------------------------------------+
| ``{% trans_default_domain lingua %}``             | Imposterà il dominio predefinito per i cataloghi dei messaggi     |
|                                                   | nel template corrente                                             |
+---------------------------------------------------+-------------------------------------------------------------------+

Test
----

+---------------------------------------------------+------------------------------------------------------------------------------+
| Sintassi del test                                 | Uso                                                                          |
+===================================================+==============================================================================+
| ``selectedchoice(choice, selectedValue)``         | Restituirà ``true`` se la scelta è selezionata per il valore dato            |
+---------------------------------------------------+------------------------------------------------------------------------------+

Variabili globali
-----------------

+-------------------------------------------------------+------------------------------------------------------------------------------------+
| Variabile                                             | Uso                                                                                |
+=======================================================+====================================================================================+
| ``app`` *Attributi*: ``app.user``, ``app.request``    | La variabile ``app`` è disponibile ovunque e dà accesso rapido                     |
| ``app.session``, ``app.environment``, ``app.debug``   | a molti oggetti di uso comune. La variabile ``app`` è un'istanza                   |
| ``app.security``                                      | di :class:`Symfony\\Bundle\\FrameworkBundle\\Templating\\GlobalVariables`          |
+-------------------------------------------------------+------------------------------------------------------------------------------------+

Estensioni di Symfony Standard Edition
--------------------------------------

Symfony Standard Edition aggiunge alcuni bundle al nucleo di Symfony2.
Questi bundle possono avere altre estensioni di Twig:

* **Twig Extension** include tutte le estensioni che non appartengono al nucleo
  di Twig, ma che possono essere interessanti. Si può approfondire nella
  `documentazione ufficiale delle estensioni di Twig`_
* **Assetic** aggiunge i tag ``{% stylesheets %}``, ``{% javascripts %}`` e 
  ``{% image %}``. Si può approfondire nella 
  :doc:`documentazione di Assetic</cookbook/assetic/asset_management>`;

.. _`documentazione ufficiale delle estensioni di Twig`: http://twig.sensiolabs.org/doc/extensions/index.html
.. _`http://twig.sensiolabs.org/documentation`: http://twig.sensiolabs.org/documentation
