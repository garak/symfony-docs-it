.. index::
   single: Assetic; UglifyJs

Minimizzare file CSS/JS (usando UglifyJs e UglifyCss)
=====================================================

`UglifyJs`_ è una libreria per l'analisi/compressione/abbellimento di javascript. 
Può essere utilizzato per combinare e minimizzare le risorse javascript in modo da fare meno richieste HTTP
e far caricare più velocemente un sito web. `UglifyCss`_ è un compressore/abbellitore di css
molto simile a UglifyJs.

In questo ricettario verranno esaminati in dettaglio l'installazione, la configurazione
e l'utilizzo di UglifyJs. ``UglifyCss`` funziona praticamente nello stesso modo, motivo per il quale
se ne parlerà in modo meno approfondito.

Installare UglifyJs
-------------------

UglifyJs è disponibile come modulo npm di `Node.js`_ e può essere installato utilizzando
npm. Per iniziare è necessario `installare node.js`_. Successivamente si potrà installare UglifyJs
utilizzando npm:

.. code-block:: bash

    $ npm install -g uglify-js

Questo comando installerà UglifyJs globalmente e potrebbe quindi richiedere l'esecuzione con
privilegi di root.

.. note::

    È anche possibile installare UglifyJs solo all'interno di un progetto. Per poterlo
    fare, sarà necessario omettere l'opzione ``-g`` e specificare il percorso d'installazione
    del modulo:

    .. code-block:: bash

        $ cd /percorso/di/symfony
        $ mkdir app/Resources/node_modules
        $ npm install uglify-js --prefix app/Resources

    Si raccomanda di installare UglifyJs nella cartella ``app/Resources``
    e di aggiungere la cartella ``node_modules`` al controllo di versione. In alternativ,
    si può creare un file `package.json`_ per npm e specificarne all'interno
    le dipendenze.

A seconda del metodo di installazione utilizzato, sarà possibile eseguire il
programma globale ``uglifyjs`` o eseguire il file fisico presente all'interno
della cartella ``node_modules``:

.. code-block:: bash

    $ uglifyjs --help

    $ ./app/Resources/node_modules/.bin/uglifyjs --help

Configurare il filtro uglifyjs2
-------------------------------

Vediamo ora come configurare Symfony2 per utilizzare il filtro ``uglifyjs2``
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
            <assetic:filter
                name="uglifyjs2"
                bin="/usr/local/bin/uglifyjs" />
        </assetic:config>

    .. code-block:: php

        // app/config/config.php
        $container->loadFromExtension('assetic', array(
            'filters' => array(
                'uglifyjs2' => array(
                    'bin' => '/usr/local/bin/uglifyjs',
                ),
            ),
        ));

.. note::

    Il percorso di installazione di UglifyJs può essere differente a seconda del sistema utilizzato.
    Per scoprire dove npm salvi la sua cartella ``bin``, si può usare il seguente
    comando:

    .. code-block:: bash

        $ npm bin -g

    Questo comando dovrebbe mostrare la cartella, all'interno del sistema, 
    nella quale risiede l'eseguibile di UglifyJs.

    Se si è installato UglifyJs localmente, la cartella bin si troverà
    all'interno della cartella ``node_modules``. In questo caso, il suo nome sarà ``.bin``.

A questo punto sarà possibile richiamare il filtro ``uglifyjs2`` dall'interno dell'applicazione.

Minimizzare le risorse
----------------------

Per utilizzare UglifyJs è necessario applicarlo alle proprie risorse. Siccome
le risorse fanno parte del livello della vista, questo lavoro deve essere svolto nei template:

.. configuration-block::

    .. code-block:: html+jinja

        {% javascripts '@AcmePippoBundle/Resources/public/js/*' filter='uglifyjs2' %}
            <script src="{{ asset_url }}"></script>
        {% endjavascripts %}

    .. code-block:: html+php

        <?php foreach ($view['assetic']->javascripts(
            array('@AcmePippoBundle/Resources/public/js/*'),
            array('uglifyj2s')
        ) as $url): ?>
            <script src="<?php echo $view->escape($url) ?>"></script>
        <?php endforeach; ?>

.. note::

    L'esempio precedente presuppone l'esistenza di un bundle chiamato ``AcmePippoBundle``
    e che i file JavaScript si trovino nella cartella ``Resources/public/js`` all'interno
    del bundle. Tutto ciò non è comunque fondamentale, dato che è possibile includere i file JavaScript
    indipendentemente dal loro posizionamento.

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

        {% javascripts '@AcmePippoBundle/Resources/public/js/*' filter='?uglifyjs2' %}
            <script src="{{ asset_url }}"></script>
        {% endjavascripts %}

    .. code-block:: html+php

        <?php foreach ($view['assetic']->javascripts(
            array('@AcmePippoBundle/Resources/public/js/*'),
            array('?uglifyjs2')
        ) as $url): ?>
            <script src="<?php echo $view->escape($url) ?>"></script>
        <?php endforeach; ?>

Per provarlo, basta passare all'ambiente ``prod`` (``app.php``). Ma prima non
bisogna scordarsi di :ref:`pulire la cache<book-page-creation-prod-cache-clear>`
e di :ref:`esportare le risorse di assetic<cookbook-asetic-dump-prod>`.

.. tip::

    Invece di aggiungere il filtro all'interno dei tag delle risorse, è possibile 
    abilitarlo globalmente aggiungendo l'attributo applay-to nella configurazione. 
    Ad esempio, per il filtro ``uglifyjs2``, ``apply_to: "\.js$"``. Per abilitare
    il filtro nel solo ambiente di produzione, è possibile aggiungere il precedente
    attributo nel file ``config_prod`` piuttosto che nel file di configurazione comune. Per ulteriori dettagli
    sull'applicazione dei filtri, si veda :ref:`cookbook-assetic-apply-to`.

Installare, configurare e utilizzare UglifyCss
----------------------------------------------

L'utilizzo di UglifyCss segue le stesse regole di UglifyJs. Per iniziare,
si installa il pacchetto npm:

.. code-block:: bash

    $ npm install -g uglifycss

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

Per utilizzare il filtro sui file css, si aggiunge il filtro all'helper ``stylesheets``
di Assetic:

.. configuration-block::

    .. code-block:: html+jinja

        {% javascripts '@AcmePippoBundle/Resources/public/css/*' filter='uglifycss' %}
             <link rel="stylesheet" href="{{ asset_url }}" />
        {% endjavascripts %}

    .. code-block:: html+php

        <?php foreach ($view['assetic']->stylesheets(
            array('@AcmePippoBundle/Resources/public/css/*'),
            array('uglifycss')
        ) as $url): ?>
            <link rel="stylesheet" href="<?php echo $view->escape($url) ?>" />
        <?php endforeach; ?>

Così come per il filtro ``uglifyjs2``, se si premette ``?`` al nome del filtro
(come in ``?uglifycss``), la minimizzazione avverrà solamente quando non si è
in modalità debug.

.. _`UglifyJs`: https://github.com/mishoo/UglifyJS
.. _`UglifyCss`: https://github.com/fmarcia/UglifyCSS
.. _`Node.js`: http://nodejs.org/
.. _`installare node.js`: http://nodejs.org/
.. _`package.json`: http://package.json.nodejitsu.com/
