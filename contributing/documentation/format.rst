Formato della documentazione
============================

La documentazione di Symfony2 utilizza `reStructuredText`_, che usa come linguaggio di markup 
`Sphinx`_ per la generazione dell'output (HTML, PDF, ...).

reStructuredText
----------------

reStructuredText "è un sistema di analisi e una sintassi a marcatori facilmente
leggibile, in testo semplice e WYSIWYG".

Si può imparare di più su questa sintassi leggendo i `documenti`_ di Symfony2
oppure leggendo `reStructuredText Primer`_ nel sito web di Sphinx.

Se si ha dimestichezza con Markdown, bisogna fare attenzione alle cose simili, ma
differenti: 

* Le liste cominciano all'inizio della riga (non hanno bisogno di indentazione)

* I blocchi di codice utilizzano i doppi apici (````come questi````).

Sphinx
------

Sphinx è un sistema di compilazione che aggiunge alcuni piacevoli strumenti  per creare documentazione da documenti reStructuredText.
Come tale, aggiunge nuove direttive e
interpreta ruoli di testo definiti nel `markup`_ standard reST . 

Colorazione della sintassi
~~~~~~~~~~~~~~~~~~~~~~~~~~

Tutti i blocchi di codice utilizzano PHP come linguaggio predefinito. È possibile cambiarlo
con la direttiva ``code-block``:

.. code-block:: rst

    .. code-block:: yaml

        { foo: bar, bar: { foo: bar, bar: baz } }

Se il proprio codice PHP comincia con ``<?php``, allora si avrà bisogno di utilizzare ``html+php`` come
pseudo-linguaggio:

.. code-block:: rst

    .. code-block:: html+php

        <?php echo $this->foobar(); ?>

.. note::

    La lista dei linguaggi supportati è disponibile nel `sito di Pygments`_.

Blocchi di configurazione
~~~~~~~~~~~~~~~~~~~~~~~~~

Ogni volta che si mostra una configurazione, per mostrarla in tutti i formati supportati ,bisogna utilizzare la
direttiva ``configuration-block`` (``PHP``, ``YAML`` e
``XML``):

.. code-block:: rst

    .. configuration-block::

        .. code-block:: yaml

            # Configurazione in YAML

        .. code-block:: xml

            <!-- Configurazione in XML //-->

        .. code-block:: php

            // Configurazione in PHP

Il precedente snippet reST mostra un blocco come di seguito:

.. configuration-block::

    .. code-block:: yaml

        # Configurazione in YAML

    .. code-block:: xml

        <!-- Configurazione in XML //-->

    .. code-block:: php

        // Configurazione in PHP

Ecco la lista dei formati attualmente supportati:

+-----------------+-------------+
| Formato markup  | Mostrato    |
+=================+=============+
| html            | HTML        |
+-----------------+-------------+
| xml             | XML         |
+-----------------+-------------+
| php             | PHP         |
+-----------------+-------------+
| yaml            | YAML        |
+-----------------+-------------+
| jinja           | Twig        |
+-----------------+-------------+
| html+jinja      | Twig        |
+-----------------+-------------+
| html+php        | PHP         |
+-----------------+-------------+
| ini             | INI         |
+-----------------+-------------+
| php-annotations | Annotazioni |
+-----------------+-------------+

Collegamenti
~~~~~~~~~~~~

Per aggiungere collegamenti ad altre pagine nei documenti, usare la seguente sintassi:

.. code-block:: rst

    :doc:`/percorso/della/pagina`

Usando il percorso e il nome del file della pagina senza estensione, per esempio:

.. code-block:: rst

    :doc:`/book/controller`

    :doc:`/components/event_dispatcher/introduction`

    :doc:`/cookbook/configuration/environments`

Il testo del collegamento sarà il titolo principale del documento collegato. Si può
anche specificare un testo alternativo per il collegamento:

.. code-block:: rst

    :doc:`Spool di email</cookbook/email/spool>`

Si possono anche aggiungere collegamenti alla documentazione delle API:

.. code-block:: rst

    :namespace:`Symfony\\Component\\BrowserKit`

    :class:`Symfony\\Component\\Routing\\Matcher\\ApacheUrlMatcher`

    :method:`Symfony\\Component\\HttpKernel\\Bundle\\Bundle::build`

e alla documentazione di PHP:

.. code-block:: rst

    :phpclass:`SimpleXMLElement`

    `DateTime::createFromFormat`_

    :phpfunction:`iterator_to_array`

Test della documentazione
~~~~~~~~~~~~~~~~~~~~~~~~~

Per fare un test della documentazione, prima di un commit:

 * Installare `Sphinx`_;

 * Eseguire la `preparazione rapida di Sphinx`_;

 * Installare le estensioni di Sphinx (vedere sotto);

 * Eseguire ``make html`` e controllare l'HTML generato nella cartella ``build``.

Installare le estensioni di Sphinx
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

 * Scaricare l'estensione dal repository dei `sorgenti`_
  
 * Copiare la cartella ``sensio`` nella cartella ``_exts`` della propria
   cartella dei sorgenti (in cui si trova ``conf.py``)
   
 * Aggiungere le righe seguenti al file ``conf.py``:

.. code-block:: py
    
    # ...
    sys.path.append(os.path.abspath('_exts'))
    
    # aggiunge PhpLexer
    from sphinx.highlighting import lexers
    from pygments.lexers.web import PhpLexer

    # ...
    # aggiunge le estensioni alla lista di estensioni
    extensions = [..., 'sensio.sphinx.refinclude', 'sensio.sphinx.configurationblock', 'sensio.sphinx.phpcode']

    # abilita la colorazione per il codice PHP non compreso tra ``<?php ... ?>``
    lexers['php'] = PhpLexer(startinline=True)
    lexers['php-annotations'] = PhpLexer(startinline=True)

    # usa PHP come dominio primario
    primary_domain = 'php'
    
    # imposta url per collegamenti alle API
    api_url = 'http://api.symfony.com/master/%s'

.. _reStructuredText:        http://docutils.sourceforge.net/rst.html
.. _Sphinx:                  http://sphinx-doc.org/
.. _documenti:               http://github.com/symfony/symfony-docs
.. _reStructuredText Primer: http://sphinx-doc.org/rest.html
.. _markup:                  http://sphinx-doc.org/markup/
.. _sito di Pygments:        http://pygments.org/languages/
.. _sorgenti:                https://github.com/fabpot/sphinx-php
.. _preparazione rapida di Sphinx:  http://sphinx-doc.org/tutorial.html#setting-up-the-documentation-sources
.. _`DateTime::createFromFormat`: http://php.net/manual/en/datetime.createfromformat.php