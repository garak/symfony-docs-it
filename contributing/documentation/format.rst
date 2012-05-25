Formato della documentazione
============================

La documentazione di Symfony2 utilizza `reStructuredText`_ che utilizza come linguaggio di markup 
`Sphinx`_ per la generazione dell'output (HTML, PDF, ...).

reStructuredText
----------------

reStructuredText "is an easy-to-read, what-you-see-is-what-you-get plaintext
markup syntax and parser system."

Si può imparare di più su questa sintassi leggendo i `documenti`_ di Symfony2
oppure leggendo la `reStructuredText Primer`_ nel sito web di Sphinx.

Se si ha dimestichezza con Markdown, bisogna fare attenzione alle cose simili, ma
differenti: 

* Le liste cominciano all'inizio della linea (non hanno bisogno di indentazione)

* I blocchi di codice utilizzano le doppie apici (````come queste````).

Sphinx
------

Sphinx è un sistema di compilazione che aggiunge alcuni piacevoli strumenti  per creare documentazione da documenti reStructuredText.
Come tale, essa aggiunge nuove direttive e
interpreta ruoli di testo definiti nello standard reST `markup`_. 

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

            // Configuration in PHP

Il precedente snippet reST mostra un blocco come di seguito:

.. configuration-block::

    .. code-block:: yaml

        # Configurazione in YAML

    .. code-block:: xml

        <!-- Configurazione in XML //-->

    .. code-block:: php

        // Configuration in PHP

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
| jinja+html      | Twig        |
+-----------------+-------------+
| php+html        | PHP         |
+-----------------+-------------+
| html+php        | PHP         |
+-----------------+-------------+
| ini             | INI         |
+-----------------+-------------+
| php-annotations | Annotazioni |
+-----------------+-------------+

Test della documentazione
~~~~~~~~~~~~~~~~~~~~~~~~~

Per fare un test della documentazione, prima di un commit:

 * Installare `Sphinx`_;

 * Eseguire la `preparazione rapida di Sphinx`_;

 * Installare l'estensione configuration-block di Sphinx (vedere sotto);

 * Eseguire ``make html`` e controllare l'HTML generato nella cartella ``build``.

Installare l'estensione configuration-block di Sphinx
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

 * Scaricare l'estensione dal repository `configuration-block`_
  
 * Copiare il file ``configurationblock.py`` nella cartella ``_exts`` della propria
   cartella dei sorgenti (in cui si trova ``conf.py``)
   
 * Aggiungere le righe seguenti al file ``conf.py``:

.. code-block:: py
    
    # ...
    sys.path.append(os.path.abspath('_exts'))
    
    # ...
    # add the extensions to the list of extensions
    extensions = [..., 'sensio.sphinx.refinclude', 'sensio.sphinx.configurationblock', 'sensio.sphinx.phpcode']

    # enable highlighting for PHP code not between ``<?php ... ?>`` by default
    lexers['php'] = PhpLexer(startinline=True)
    lexers['php-annotations'] = PhpLexer(startinline=True)

    # use PHP as the primary domain
    primary_domain = 'php'
    
    # set url for API links
    api_url = 'http://api.symfony.com/master/%s'

.. _reStructuredText:        http://docutils.sf.net/rst.html
.. _Sphinx:                  http://sphinx.pocoo.org/
.. _documenti:               http://github.com/symfony/symfony-docs
.. _reStructuredText Primer: http://sphinx.pocoo.org/rest.html
.. _markup:                  http://sphinx.pocoo.org/markup/
.. _sito di Pygments:        http://pygments.org/languages/
.. _configuration-block:     https://github.com/fabpot/sphinx-php
.. _preparazione rapida di Sphinx:  http://sphinx.pocoo.org/tutorial.html#setting-up-the-documentation-sources
