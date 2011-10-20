Formato della documentazione
============================

La documentazione di Symfony2 utilizza `reStructuredText`_ come linguaggio e
`Sphinx`_ per la generazione dell'output (HTML, PDF, ...).

reStructuredText
----------------

reStructuredText "è una sintassi e un sistema di analisi testuale facile da leggere e
what-you-see-is-what-you-get."

Si può approfondire la sua sintassi, leggengo i `documenti`_ esistenti di Symfony2 o
leggendo la guida `reStructuredText Primer`_ sul sito di Sphinx.

Se si ha dimestichezza con Markdown, bisogna fare attenzione, perché alcune cose sono
simili, ma altre differenti: 

* Le liste cominciano all'inizio della riga (non hanno bisogno di indentazione)

* I blocchi di codice utilizzano i doppie apici singoli (````come questi````).

Sphinx
------

Sphinx è un sistema di compilazione, che aggiunge alcuni piacevoli strumenti per creare
documentazione da documenti reStructuredText. Come tale, aggiunge nuove direttive e
interpreta ruoli di testo definiti al ` markup`_ standard di reST. 

Colorazione della sintassi
~~~~~~~~~~~~~~~~~~~~~~~~~~

Tutti i blocchi di codice utilizzano PHP come linguaggio predefinito. È possibile
cambiarlo, con la direttiva ``code-block``:

.. code-block:: rst

    .. code-block:: yaml

        { pippo: pluto, pluto: { pippo: pluto, pluto: paperino } }

Se il proprio codice PHP comincia con ``<?php``, allora si avrà bisogno di
utilizzare ``html+php`` come pseudo-linguaggio:

.. code-block:: rst

    .. code-block:: html+php

        <?php echo $this->pippopluto(); ?>

.. note::

    La lista dei linguaggi supportati è disponibile nel `sito di Pygments`_.

Blocchi di configurazione
~~~~~~~~~~~~~~~~~~~~~~~~~

Ogni volta che si mostra una configurazione, per mostrarla in tutti i formati supportati
(``PHP``, ``YAML``, and ``XML``, bisogna utilizzare la direttiva
``configuration-block`` ):

.. code-block:: rst

    .. Configurazione-block::

        .. code-block:: yaml

            # Configurazione in YAML

        .. code-block:: xml

            <!-- Configurazione in XML //-->

        .. code-block:: php

            // Configurazione in PHP

Il precedente testo reST mostra un blocco come di seguito:

.. configuration-block::

    .. code-block:: yaml

        # Configurazione in YAML

    .. code-block:: xml

        <!-- Configurazione in XML //-->

    .. code-block:: php

        // Configurazione in PHP

La lista dei formati attualmente supportati è la seguente:

+-----------------+---------------+
| Formato         | Mostrato come |
+=================+===============+
| html            | HTML          |
+-----------------+---------------+
| xml             | XML           |
+-----------------+---------------+
| php             | PHP           |
+-----------------+---------------+
| yaml            | YAML          |
+-----------------+---------------+
| jinja           | Twig          |
+-----------------+---------------+
| html+jinja      | Twig          |
+-----------------+---------------+
| jinja+html      | Twig          |
+-----------------+---------------+
| php+html        | PHP           |
+-----------------+---------------+
| html+php        | PHP           |
+-----------------+---------------+
| ini             | INI           |
+-----------------+---------------+
| php-annotations | Annotazioni   |
+-----------------+---------------+

Test della documentazione
~~~~~~~~~~~~~~~~~~~~~~~~~

Per testare la documentazione, prima di un commit:

 * Installare `Sphinx`_;

 * Eseguire il `setup rapido di Sphinx`_;

 * Installare l'estensione configuration-block di Sphinx (vedere sotto);

 * Eseguire ``make html`` e controllare l'HTML generato nella cartella ``build``.

Installare l'estensione configuration-block di Sphinx
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

 * Scaricare l'estensione dal repository `sorgente configuration-block`_
  
 * Copiare ``configurationblock.py`` nella cartella ``_exts`` sotto la cartella dei
   sorgenti (il posto in cui si trova ``conf.py``)
   
 * Aggiungere il seguente codice al file ``conf.py``:

.. code-block:: py
    
    # ...
    sys.path.append(os.path.abspath('_exts'))
    
    # ...
    # add configurationblock to the list of extensions
    extensions = ['configurationblock']

.. _reStructuredText:             http://docutils.sf.net/rst.html
.. _Sphinx:                       http://sphinx.pocoo.org/
.. _documenti:                    http://github.com/symfony/symfony-docs
.. _reStructuredText Primer:      http://sphinx.pocoo.org/rest.html
.. _markup:                       http://sphinx.pocoo.org/markup/
.. _sito di Pygments:             http://pygments.org/languages/
.. _sorgente configuration-block: https://github.com/fabpot/sphinx-php
.. _setup rapido di Sphinx:       http://sphinx.pocoo.org/tutorial.html#setting-up-the-documentation-sources
