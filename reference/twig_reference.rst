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

+----------------------------------------------------+--------------------------------------------------------------------------------------------+
| Sintassi della funzione                            | Uso                                                                                        |
+====================================================+============================================================================================+
| ``asset(path, packageName = null)``                | Restituisce il percorso pubblico di una risorsa, maggiori informazioni in                  |
|                                                    | ":ref:`book-templating-assets`".                                                           |
+----------------------------------------------------+--------------------------------------------------------------------------------------------+
| ``asset_version(packageName = null)``              | Restituisce la versione attuale del pacchetto, maggiori informazioni in                    |
|                                                    | ":ref:`book-templating-assets`".                                                           |
+----------------------------------------------------+--------------------------------------------------------------------------------------------+
| ``form_enctype(form)``                             | Renderà l'attributo obbligatorio ``enctype="multipart/form-data"`` in un                   |
|                                                    | form con almeno un campo di caricamento di file, maggiori informazioni in                  |
|                                                    | :ref:`the Twig Form reference<reference-forms-twig-enctype>`.                              |
+----------------------------------------------------+--------------------------------------------------------------------------------------------+
| ``form_widget(form, variables = {})``              | Renderà un intero form o un widget specifico di un campo,                                  |
|                                                    | maggiori informazioni in :ref:`the Twig Form reference<reference-forms-twig-widget>`.      |
+----------------------------------------------------+--------------------------------------------------------------------------------------------+
| ``form_errors(form)``                              | Renderà gli errori per un dato campo o gli errori "globali",                               |
|                                                    | maggiori informazioni in :ref:`the Twig Form reference<reference-forms-twig-errors>`.      |
+----------------------------------------------------+--------------------------------------------------------------------------------------------+
| ``form_label(form, label = null, variables = {})`` | Renderà la label di un dato campo, maggiori informazioni in                                |
|                                                    | :ref:`the Twig Form reference<reference-forms-twig-label>`.                                |
+----------------------------------------------------+--------------------------------------------------------------------------------------------+
| ``form_row(form, variables = {})``                 | Renderà la riga (label, errori e widget del campo) del dato campo,                         |
|                                                    | maggiori informazioni in :ref:`the Twig Form reference<reference-forms-twig-row>`.         |
+----------------------------------------------------+--------------------------------------------------------------------------------------------+
| ``form_rest(form, variables = {})``                | Renderà tutti i campi non ancora resi, maggiori informazioni in                            |
|                                                    | :ref:`the Twig Form reference<reference-forms-twig-rest>`.                                 |
+----------------------------------------------------+--------------------------------------------------------------------------------------------+
| ``_form_is_choice_group(label)``                   | Restituirà ``true`` se la label è un gruppo di scelta.                                     |
+----------------------------------------------------+--------------------------------------------------------------------------------------------+
| ``_form_is_choice_selected(form, choice)``         | Restituirà ``true`` se la scelta data è selezionata.                                       |
+----------------------------------------------------+--------------------------------------------------------------------------------------------+
| ``is_granted(role, object = null, field = null)``  | Restituirà ``true`` se l'utente attuale ha il ruolo richiesto, maggiori                    |
|                                                    | informazioni in ":ref:`book-security-template`"                                            |
+----------------------------------------------------+--------------------------------------------------------------------------------------------+
| ``path(name, parameters = {})``                    | Restituisce l'URL relativo per la rotta data, maggiori informazioni in                     |
|                                                    | ":ref:`book-templating-pages`".                                                            |
+----------------------------------------------------+--------------------------------------------------------------------------------------------+
| ``url(name, parameters = {})``                     | Equivalente a ``path(...)``, ma genera un URL assoluto                                     |
+----------------------------------------------------+--------------------------------------------------------------------------------------------+

Filtri
------

+---------------------------------------------------------------------------------+-------------------------------------------------------------------+
| Sintassi del filtro                                                             | Uso                                                               |
+=================================================================================+===================================================================+
| ``text|trans(arguments = {}, domain = 'messages', locale = null)``              | Tradurrà il testo nella lingua attuale, maggiori                  |
|                                                                                 | informazioni in :ref:`book-translation-twig`.                     |
+---------------------------------------------------------------------------------+-------------------------------------------------------------------+
| ``text|transchoice(count, arguments = {}, domain = 'messages', locale = null)`` | Tradurrà il testo con il plurale, maggiori informazioni           |
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
| ``path|file_excerpt(line)``                                                     | Renderà un estratto di un file di codice intorno alla riga data.  |
+---------------------------------------------------------------------------------+-------------------------------------------------------------------+
| ``path|format_file(line, text)``                                                | Renderà il percorso di un file in un collegamento.                |
+---------------------------------------------------------------------------------+-------------------------------------------------------------------+
| ``exceptionMessage|format_file_from_text``                                      | Equivalente a ``format_file``, ma ha analizzato la stringa di     |
|                                                                                 | errore di PHP in un file (p.e. 'in pippo.php on line 45')         |
+---------------------------------------------------------------------------------+-------------------------------------------------------------------+
| ``path|file_link(line)``                                                        | Renderà un percorso al file (e numero di riga) corretto           |
+---------------------------------------------------------------------------------+-------------------------------------------------------------------+

Tag
---

+---------------------------------------------------+-------------------------------------------------------------------+
| Sintassi del tag                                  | Uso                                                               |
+===================================================+===================================================================+
| ``{% render 'controller' with {parameters} %}``   | Renderà il contenuto della risposta per il controllore dato,      |
|                                                   | maggiori informazioni in :ref:`templating-embedding-controller`.  |
+---------------------------------------------------+-------------------------------------------------------------------+
| ``{% form_theme form 'file' %}``                  | Cercherà in un dato file i blocchi di form ridefiniti,            |
|                                                   | maggiori informazioni in :doc:`/cookbook/form/form_customization`.|
+---------------------------------------------------+-------------------------------------------------------------------+
| ``{% trans with {variables} %}...{% endtrans %}`` | Tradurrà e renderà il testo, maggiori informazioni in             |
|                                                   | :ref:`book-translation-twig`                                      |
+---------------------------------------------------+-------------------------------------------------------------------+
| ``{% transchoice count with {variables} %}``      | Tradurrà e renderà il testo con il plurale, maggiori              |
| ...                                               | informazioni in :ref:`book-translation-twig`                      |
| ``{% endtranschoice %}``                          |                                                                   |
+---------------------------------------------------+-------------------------------------------------------------------+

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
