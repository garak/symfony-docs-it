.. index::
   single: Assetic; UglifyJS

Minimizzare file CSS/JS (usando UglifyJS e UglifyCSS)
=====================================================

`UglifyJS`_ è una libreria per l'analisi/compressione/abbellimento di javascript. 
Può essere utilizzato per combinare e minimizzare le risorse javascript in modo da fare meno richieste HTTP
e far caricare più velocemente un sito web. `UglifyCss`_ è un compressore/abbellitore di css
molto simile a UglifyJs.

In questo ricettario verranno esaminati in dettaglio l'installazione, la configurazione
e l'utilizzo di UglifyJs. ``UglifyCss`` funziona praticamente nello stesso modo, motivo per il quale
se ne parlerà in modo meno approfondito.

Installare UglifyJS
-------------------

UglifyJS è disponibile come modulo npm di `Node.js`_. Per iniziare è necessario `installare node.js`_.
Successivamente si potrà decidere se eseguire un'installazione globale o locale.

Installazione globale
~~~~~~~~~~~~~~~~~~~~~

L'installazione globale consente a tutti i progetti di usare la stessa versione di UglifyJS,
il che semplifica la manutenzione. Aprire la console dei comandi ed eseguire
il seguente comando (potrebbero essere necessari privilegi di root):

.. code-block:: bash

    $ npm install -g uglify-js

Ora si può eseguire il comando globale ``uglifyjs``, da qualsiasi punto del sistema:

.. code-block:: bash

    $ uglifyjs --help

Installazione locale
--------------------

È anche possibile installare UglifyJs solo all'interno di un progett, che è utile
se si devono usare versioni differenti di UglifyJs. Per poterlo fare, sarà necessario
omettere l'opzione ``-g`` e specificare il percorso d'installazione del modulo:

.. code-block:: bash

    $ cd /percorso/di/symfony
    $ npm install uglify-js --prefix app/Resources

Si raccomanda di installare UglifyJs nella cartella ``app/Resources`` e
di aggiungere la cartella ``node_modules`` al controllo di versione. In alternativa,
si può creare un file `package.json`_ per npm e specificarne all'interno  le dipendenze.

Or si può eseguire il comando ``uglifyjs``, che si trova nella
cartella ``node_modules``:

.. code-block:: bash

    $ "./app/Resources/node_modules/.bin/uglifyjs" --help

Configurare il filtro uglifyjs2
-------------------------------

Vediamo ora come configurare Symfony per utilizzare il filtro ``uglifyjs2``
nel trattamento del codice javascript:

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        assetic:
            filters:
                uglifyjs2:
                    # percorso dell'eseguibile uglifyjs
                    bin: /usr/local/bin/uglifyjs

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <assetic:config>
            <!-- bin: percorso dell'eseguibile uglifyjs -->
            <assetic:filter
                name="uglifyjs2"
                bin="/usr/local/bin/uglifyjs" />
        </assetic:config>

    .. code-block:: php

        // app/config/config.php
        $container->loadFromExtension('assetic', array(
            'filters' => array(
                'uglifyjs2' => array(
                    // percorso dell'eseguibile uglifyjs
                    'bin' => '/usr/local/bin/uglifyjs',
                ),
            ),
        ));

.. note::

    Il percorso di installazione di UglifyJs può essere differente a seconda del sistema utilizzato.
    Per scoprire dove npm salvi la sua cartella ``bin``, si può usare il seguente comando:

    .. code-block:: bash

        $ npm bin -g

    Questo comando dovrebbe mostrare la cartella, all'interno del sistema, 
    nella quale risiede l'eseguibile di UglifyJS.

    Se si è installato UglifyJs localmente, la cartella bin si troverà
    all'interno della cartella ``node_modules``. In questo caso, il suo nome sarà ``.bin``.

A questo punto sarà possibile richiamare il filtro ``uglifyjs2`` dall'interno dell'applicazione.

Configurare il binario ``node``
-------------------------------

Assetic prova a trovare automaticamente il binario di node. Se non ci riesce,
si può configurare la sua posizione, usando la voce ``node``:

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        assetic:
            # the path to the node executable
            node: /usr/bin/nodejs
            filters:
                uglifyjs2:
                    # the path to the uglifyjs executable
                    bin: /usr/local/bin/uglifyjs

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <assetic:config
            node="/usr/bin/nodejs" >
            <assetic:filter
                name="uglifyjs2"
                bin="/usr/local/bin/uglifyjs" />
        </assetic:config>

    .. code-block:: php

        // app/config/config.php
        $container->loadFromExtension('assetic', array(
            'node' => '/usr/bin/nodejs',
            'uglifyjs2' => array(
                    // the path to the uglifyjs executable
                    'bin' => '/usr/local/bin/uglifyjs',
                ),
        ));

Minimizzare le risorse
----------------------

Per utilizzare UglifyJs è necessario applicarlo alle proprie risorse. Siccome
le risorse fanno parte del livello della vista, questo lavoro deve essere svolto nei template:

.. configuration-block::

    .. code-block:: html+jinja

        {% javascripts '@AppBundle/Resources/public/js/*' filter='uglifyjs2' %}
            <script src="{{ asset_url }}"></script>
        {% endjavascripts %}

    .. code-block:: html+php

        <?php foreach ($view['assetic']->javascripts(
            array('@AppBundle/Resources/public/js/*'),
            array('uglifyj2s')
        ) as $url): ?>
            <script src="<?php echo $view->escape($url) ?>"></script>
        <?php endforeach ?>

.. note::

    L'esempio precedente presuppone l'esistenza di un bundle chiamato AppBundle e che i file
    JavaScript si trovino nella cartella ``Resources/public/js`` all'interno del
    bundle. Tuttavia è possibile includere i file JavaScript indipendentemente dal loro posizionamento.

Con l'aggiunta del filtro ``uglifyjs2`` ai tag delle risorse precedenti, si vedranno
i file JavaScript minimizzati fluire molto più velocemente sulla rete.

Disabilitare la minimizzazione nella modalità debug
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

I file javascript minimizzati sono difficili da leggere e, a maggior ragione, da debuggare. Per questo
motivo Assetic permette di disabilitare alcuni filtri quando l'applicazione è eseguita
in modalità debug (ad esempio ``app_dev.php``). Per fare ciò è possibile aggiungere un 
punto interrogativo ``?`` come prefisso del filtro all'interno del template. In questo modo Assetic viene
informato di applicare i filtri solo quando la modalità debug è spenta (come in ``app.php``):

.. configuration-block::

    .. code-block:: html+jinja

        {% javascripts '@AppBundle/Resources/public/js/*' filter='?uglifyjs2' %}
            <script src="{{ asset_url }}"></script>
        {% endjavascripts %}

    .. code-block:: html+php

        <?php foreach ($view['assetic']->javascripts(
            array('@AppBundle/Resources/public/js/*'),
            array('?uglifyjs2')
        ) as $url): ?>
            <script src="<?php echo $view->escape($url) ?>"></script>
        <?php endforeach ?>

Per provarlo, basta passare all'ambiente ``prod`` (``app.php``). Ma prima non
bisogna scordarsi di :ref:`pulire la cache<book-page-creation-prod-cache-clear>`
e di :ref:`esportare le risorse di assetic<cookbook-asetic-dump-prod>`.

.. tip::

    Invece di aggiungere il filtro all'interno dei tag delle risorse, è possibile configurare
    i filtri da applicare per ogni file, nella configurazione dell'applicazione.
    Vedere :ref:`cookbook-assetic-apply-to` per maggiori dettagli.

Installare, configurare e utilizzare UglifyCSS
----------------------------------------------

L'utilizzo di UglifyCSS segue le stesse regole di UglifyJS. Per iniziare,
si installa il pacchetto npm:

.. code-block:: bash

    # installazione globale
    $ npm install -g uglifycss

    # installazione locale
    $ cd /percorso/del/progetto/symfony
    $ npm install uglifycss --prefix app/Resources

Successivamente, aggiungere il filtro alla configurazione:

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        assetic:
            filters:
                uglifycss:
                    bin: /usr/local/bin/uglifycss

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <assetic:config>
            <assetic:filter
                name="uglifycss"
                bin="/usr/local/bin/uglifycss" />
        </assetic:config>

    .. code-block:: php

        // app/config/config.php
        $container->loadFromExtension('assetic', array(
            'filters' => array(
                'uglifycss' => array(
                    'bin' => '/usr/local/bin/uglifycss',
                ),
            ),
        ));

Per utilizzare il filtro sui file CSS, si aggiunge il filtro all'helper ``stylesheets``
di Assetic:

.. configuration-block::

    .. code-block:: html+jinja

        {% stylesheets 'bundles/App/css/*' filter='uglifycss' filter='cssrewrite' %}
             <link rel="stylesheet" href="{{ asset_url }}" />
        {% endstylesheets %}

    .. code-block:: html+php

        <?php foreach ($view['assetic']->stylesheets(
            array('bundles/App/css/*'),
            array('uglifycss'),
            array('cssrewrite')
        ) as $url): ?>
            <link rel="stylesheet" href="<?php echo $view->escape($url) ?>" />
        <?php endforeach ?>

Così come per il filtro ``uglifyjs2``, se si premette ``?`` al nome del filtro
(come in ``?uglifycss``), la minimizzazione avverrà solamente quando non si è
in modalità debug.

.. _`UglifyJS`: https://github.com/mishoo/UglifyJS
.. _`UglifyCSS`: https://github.com/fmarcia/UglifyCSS
.. _`Node.js`: http://nodejs.org/
.. _`installare node.js`: http://nodejs.org/
.. _`package.json`: http://package.json.nodejitsu.com/
