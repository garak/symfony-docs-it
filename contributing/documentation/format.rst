Formato della documentazione
============================

La documentazione di Symfony utilizza `reStructuredText`_, che usa come linguaggio di markup 
`Sphinx`_ per la generazione di documentazione in formati leggibili dagli utenti finali,
come HTML e PDF.

reStructuredText
----------------

reStructuredText ha una sintassi in testo semplice con marcatori, simile a Markdown, ma molto
più rigida nella sintassi. Chi non conosce reStructuredText può prenderne
familiarità, leggendo il codice sorgente della `documentazione di Symfony`_
esistente.

Si può imparare di più su questo formato leggendo le guide `reStructuredText Primer`_
e `reStructuredText Reference`_.

.. caution::

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

        { pippo: pluto, pluto: { pippo: pluto, pluto: paperino } }

.. note::

    Oltre ai maggiori linguaggi di programmazione, l'evidenziatore di sintassi
    supporta tutti i tipi di marcatori e linguaggi di configurazione. Si veda la
    lista di `linguaggi supportati`_ sul sito relativo.

.. _docs-configuration-blocks:

Blocchi di configurazione
~~~~~~~~~~~~~~~~~~~~~~~~~

Ogni volta che si mostra una configurazione, per mostrarla in tutti i formati supportati,
bisogna utilizzare la direttiva ``configuration-block``
(``PHP``, ``YAML`` e ``XML``)

.. code-block:: rst

    .. configuration-block::

        .. code-block:: yaml

            # Configurazione in YAML

        .. code-block:: xml

            <!-- Configurazione in XML -->

        .. code-block:: php

            // Configurazione in PHP

Il precedente snippet reST mostra un blocco come di seguito:

.. configuration-block::

    .. code-block:: yaml

        # Configurazione in YAML

    .. code-block:: xml

        <!-- Configurazione in XML -->

    .. code-block:: php

        // Configurazione in PHP

Ecco la lista dei formati attualmente supportati:

===================  =========================
Formato markup       Mostrato
===================  =========================
``html``             HTML
``xml``              XML
``php``              PHP
``yaml``             YAML
``jinja``            Twig puro
``html+jinja``       Twig mescolato con HTML
``html+php``         PHP mescolato con HTML
``ini``              INI
``php-annotations``  Annotazioni PHP
===================  =========================

Collegamenti
~~~~~~~~~~~~

La maggior parte dei collegamenti sono **interni** e usano
la seguente sintassi:

.. code-block:: rst

    :doc:`/percorso/della/pagina`

Usando il percorso e il nome del file della pagina senza estensione (``.rst``). Per esempio:

.. code-block:: rst

    :doc:`/book/controller`

    :doc:`/components/event_dispatcher/introduction`

    :doc:`/cookbook/configuration/environments`

Il testo del collegamento sarà il titolo principale del documento collegato. Si può
anche specificare un testo alternativo per il collegamento:

.. code-block:: rst

    :doc:`Spool di email </cookbook/email/spool>`

.. note::

    Sebbene tecnicamente corretti, evitare l'uso di collegamenti interni relativi,
    come i seguenti, perché infrangono i riferimenti nella documentazione
    generata in PDF:

    .. code-block:: rst

        :doc:`controller`

        :doc:`event_dispatcher/introduction`

        :doc:`environments`

I **collegamenti alle API** seguono una sintassi diversa, in cui si deve specificare
il tipo di risorsa (``namespace``, ``class`` o ``method``):

.. code-block:: rst

    :namespace:`Symfony\\Component\\BrowserKit`

    :class:`Symfony\\Component\\Routing\\Matcher\\ApacheUrlMatcher`

    :method:`Symfony\\Component\\HttpKernel\\Bundle\\Bundle::build`

I **collegamenti alla documentazione di PHP** seguono una sintassi simile:

.. code-block:: rst

    :phpclass:`SimpleXMLElement`

    :phpmethod:`DateTime::createFromFormat`

    :phpfunction:`iterator_to_array`

Nuove caratteristiche o modifiche di comportamento
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Se si sta documentando una nuova caratteristica o una modifica eseguita in
Symfony, occorre anteporre la descrizione della modifica con una direttiva
``.. versionadded:: 2.X`` e una breve descrizione:

.. code-block:: rst

    .. versionadded:: 2.3
        Il metodo ``askHiddenResponse`` è stato introdotto in Symfony 2.3.

    Si possono anche porre domande e nascondere le risposte. [...]

Se si sta modificando una modifica di comportamento, può essere utile descrivere *brevemente*
in che modo il comportamento sia cambiato.

.. code-block:: rst

    .. versionadded:: 2.3
        La funzione ``include()`` è una nuova caratteristica di Twig, disponibile in
        Symfony 2.3. In precedenza, si usava il tag ``{% include %}``.

A ogni rilascio di una versione minore di Symfony (p.e. 2.4, 2.5, ecc),
viene creato un nuovo ramo della documentazione, a partire da ``master``.
A questo punto, i tag ``versionadded`` per versioni di Symfony che hanno raggiunto il
fine vita saranno rimossi. Per esempio, se Symfony 2.5 fosse rilasciato oggi e
se il 2.2 avesse appena raggiunto il suo fine vita, i tag ``versionadded`` per 2.2
sarebbero rimossi dal ramo ``2.5``.

Test della documentazione
~~~~~~~~~~~~~~~~~~~~~~~~~

Quando si inviano nuovi contenuti al repository della documentazione o quando si modificano
risorse esistenti, un processo automatico controllerà l'eventuale presenza di
errori di sintassi..

Se tuttavia si preferisce verificare in locale, prima di inviare documentazione,
seguire questi passi:

* Installare `Sphinx`_;
* Installare le estensioni di Sphinx, eseguendo ``$ git submodule update --init``;
* Eseguire ``make html`` e controllare l'HTML generato nella cartella ``build/``.

.. _reStructuredText:        http://docutils.sourceforge.net/rst.html
.. _Sphinx:                  http://sphinx-doc.org/
.. _documentazione di Symfony: https://github.com/symfony/symfony-docs-it
.. _reStructuredText Primer: http://sphinx-doc.org/rest.html
.. _`reStructuredText Reference`: http://docutils.sourceforge.net/docs/user/rst/quickref.html
.. _markup:                  http://sphinx-doc.org/markup/
.. _linguaggi supportati:    http://pygments.org/languages/
