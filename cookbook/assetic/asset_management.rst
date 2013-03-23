.. index::
   single: Assetic; Introduzione

Come usare Assetic per la gestione delle risorse
================================================

Assetic unisce due idee principali: :ref:`risorse<cookbook-assetic-assets>` e
:ref:`filtri<cookbook-assetic-filters>`. Le risorse sono file come CSS,
JavaScript e file di immagini. I filtri sono cose che possono essere applicate
a questi file prima di essere serviti al browser. Questo permette una separazione
tra i file delle risorse memorizzati nell'applicazione e i file effettivamente presentati
all'utente.

Senza Assetic, basta servire direttamente i file che sono memorizzati
nell'applicazione:

.. configuration-block::

    .. code-block:: html+jinja

        <script src="{{ asset('js/script.js') }}" type="text/javascript" />

    .. code-block:: php

        <script src="<?php echo $view['assets']->getUrl('js/script.js') ?>" type="text/javascript" />

Ma *con* Assetic, è possibile manipolare queste risorse nel modo che si preferisce (o
caricarle da qualunque parte) prima di servirli. Questo significa che si può:

* Minimizzare e combinare tutti i file CSS e JS

* Eseguire tutti (o solo alcuni) dei file CSS o JS attraverso una sorta di compilatore,
  come LESS, SASS o CoffeeScript

* Eseguire ottimizzazioni delle immagini

.. _cookbook-assetic-assets:

Risorse
-------

L'utilizzo di Assetic consente molti vantaggi rispetto a servire direttamente i file.
I file non devono essere memorizzati dove vengono serviti e possono
provenire da varie fonti come quelle all'interno di un bundle.

Si può usare Assetic per processare sia :ref:`fogli di stile CSS<cookbook-assetic-including-css>`
che :ref:`file JavaScript<cookbook-assetic-including-javascript>`. La filosofia
dietro all'aggiunta di entrambi è di base la stessa, ma con una sintassi leggermente diversa.

.. _cookbook-assetic-including-javascript:

Includere file JavaScript
~~~~~~~~~~~~~~~~~~~~~~~~~

Per includere file JavaScript, usare il tag ``javascript`` in un template.
Solitamente lo si farà nel blocck ``javascripts``, se si usano i nomi
di blocchi predefiniti in Symfony Standard Distribution:

.. configuration-block::

    .. code-block:: html+jinja

        {% javascripts '@AcmeFooBundle/Resources/public/js/*' %}
        <script type="text/javascript" src="{{ asset_url }}"></script>
        {% endjavascripts %}

    .. code-block:: html+php

        <?php foreach ($view['assetic']->javascripts(
            array('@AcmeFooBundle/Resources/public/js/*')
        ) as $url): ?>
        <script type="text/javascript" src="<?php echo $view->escape($url) ?>"></script>
        <?php endforeach; ?>

.. tip::

    Si possono anche includere fogli di stile CSS: vedere :ref:`cookbook-assetic-including-css`.

In questo esempio, tutti i file della cartella ``Resources/public/js/``
di ``AcmeFooBundle`` saranno caricati e serviti da una posizione diversa.
Il tag di effettiva resa assomiglierà a questo:

.. code-block:: html

    <script src="/app_dev.php/js/abcd123.js"></script>

Questo è un punto chiave. una volta che Assetic gestisce le risorse, i file sono
serviti da una posizione diversa. Questo *causerà* problemi con i file CSS
che fanno riferimento a immagini con percorsi relativi. Vedere :ref:`cookbook-assetic-cssrewrite`.

.. _cookbook-assetic-including-css:

Includere fogli di stile CSS
~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Per usare fogli di stile CSS, si può usare la stessa metodologia vista
sopra, tranne per l'uso del tag ``stylesheets``. Se si usano i nomi di blocchi
predefiniti in Symfony Standard Distribution, si troverà di solito
in un blocco ``stylesheets``:

    .. configuration-block::

        .. code-block:: html+jinja

            {% stylesheets 'bundles/acme_foo/css/*' filter='cssrewrite' %}
                <link rel="stylesheet" href="{{ asset_url }}" />
            {% endstylesheets %}

        .. code-block:: html+php

            <?php foreach ($view['assetic']->stylesheets(
                array('bundles/acme_foo/css/*'),
                array('cssrewrite')
            ) as $url): ?>
                <link rel="stylesheet" href="<?php echo $view->escape($url) ?>" />
            <?php endforeach; ?>

Ma poiché Assetic cambia i percorsi delle risorse, *non* funzioneranno tutte
le immagini di sfondo (o altri percorsi) che usano percorsi relativi, a meno di
non usare il filtro :ref:`cssrewrite<cookbook-assetic-cssrewrite>`.

.. note::

    Si noti che,  nell'esempio originale che includeva i file JavaScript files, abbiamo
    fatto riferimento ai file con un percorso come ``@AcmeFooBundle/Resources/public/file.js``,
    mentre in questo esempio, abbiamo fatto riferimento ai file CSS tramite il loro vero
    percorso, accessibile pubblicamente: ``bundles/acme_foo/css``. Si possono usare entrambi, tranne
    per il fatto che c'è un problema noto, che non fa funzionare ``cssrewrite`` quando
    si usa la sintassi ``@AcmeFooBundle`` per i fogli di stile CSS.

.. _cookbook-assetic-cssrewrite:

Aggiustare i percorsi del CSS con il filtro ``cssrewrite``
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Poiché Assetic generat nuovi URL per le risorse, qualsiasi percorso relativo dentro
ai file CSS non funzionerà. Per risolvere il problema, usare il filtro ``cssrewrite``
nel tag ``stylesheets``. Tale filtro analizza i file CSS e corregge
i percorsi interni, per riflettere la nuova posizione.

Un esempio è disponibile nella sezione precedente.

.. caution::

    Quando si usa il filtro ``cssrewrite``, non fare riferimento ai file cSS con la sintassi
    ``@AcmeFooBundle``. Vedere la nota nella sezione precedente per maggiori dettagli.

Combinare le risorse
~~~~~~~~~~~~~~~~~~~~

È anche possibile combinare più file in uno. Questo aiuta a ridurre il numero
delle richieste HTTP, una cosa molto utile per le prestazioni frontend. Permette
anche di mantenere i file più facilmente, dividendoli in gruppi maggiormente gestibili.
Questo può contribuire alla riusabilità in quanto si possono facilmente dividere file specifici del
progetto da quelli che possono essere utilizzati in altre applicazioni, ma servendoli ancora
come un unico file:

.. configuration-block::

    .. code-block:: html+jinja

        {% javascripts
            '@AcmeFooBundle/Resources/public/js/*'
            '@AcmeBarBundle/Resources/public/js/form.js'
            '@AcmeBarBundle/Resources/public/js/calendar.js' %}
        <script src="{{ asset_url }}"></script>
        {% endjavascripts %}

    .. code-block:: html+php

        <?php foreach ($view['assetic']->javascripts(
            array(
                '@AcmeFooBundle/Resources/public/js/*',
                '@AcmeBarBundle/Resources/public/js/form.js',
                '@AcmeBarBundle/Resources/public/js/calendar.js',
            )
        ) as $url): ?>
            <script src="<?php echo $view->escape($url) ?>"></script>
        <?php endforeach; ?>

Nell'ambiente ``dev``, ciascun file è ancora servito individualmente, in modo che
sia possibile eseguire il debug dei problemi più facilmente. Tuttavia, nell'ambiente ``prod``,
(o, più specificatamente, quando ``debug`` vale ``false``), questo verrà
reso come un unico tag ``script``, che contiene il contenuto di tutti i
file JavaScript.

.. tip::

    Se si è nuovi con Assetic e si prova a utilizzare la propria applicazione nell'ambiente
    ``prod`` (utilizzando il controllore ``app.php``), probabilmente si vedrà
    che mancano tutti i CSS e JS. Non bisogna preoccuparsi! Accade di proposito.
    Per informazioni dettagliate sull'utilizzo di Assetic in ambiente `prod`, vedere :ref:`cookbook-assetic-dumping`.

La combinazione dei file non si applica solo ai *propri* file. Si può anche utilizzare Assetic per
combinare risorse di terze parti (come jQuery) con i propri, in un singolo file:

.. configuration-block::

    .. code-block:: html+jinja

        {% javascripts
            '@AcmeFooBundle/Resources/public/js/thirdparty/jquery.js'
            '@AcmeFooBundle/Resources/public/js/*' %}
        <script src="{{ asset_url }}"></script>
        {% endjavascripts %}

    .. code-block:: html+php

        <?php foreach ($view['assetic']->javascripts(
            array(
                '@AcmeFooBundle/Resources/public/js/thirdparty/jquery.js',
                '@AcmeFooBundle/Resources/public/js/*',
            )
        ) as $url): ?>
            <script src="<?php echo $view->escape($url) ?>"></script>
        <?php endforeach; ?>

.. _cookbook-assetic-filters:

Filtri
------

Una volta che vengono gestite da Assetic, è possibile applicare i filtri alle proprie risorse prima
che siano servite. Questi includono filtri che comprimono l'output delle proprie risorse
per ottenere file di dimensioni inferiori (e migliore ottimizzazione nel frontend). Altri filtri
possono compilare i file JavaScript da file CoffeeScript e processare SASS in CSS.
Assetic ha una lunga lista di filtri disponibili.

Molti filtri non fanno direttamente il lavoro, ma usano librerie di terze
parti per fare il lavoro pesante. Questo significa che spesso si avrà la necessità di installare
una libreria di terze parti per usare un filtro. Il grande vantaggio di usare Assetic
per invocare queste librerie (invece di utilizzarle direttamente) è che invece
di doverle eseguire manualmente dopo aver lavorato sui file, sarà Assetic
a prendersene cura, rimuovendo del tutto questo punto dal processo di sviluppo
e di pubblicazione.

Per usare un filtro, è necessario specificarlo nella configurazione di Assetic.
L'aggiunta di un filtro qui non significa che venga utilizzato: significa solo che è
disponibile per l'uso.

Per esempio, per usare il compressore JavaScript YUI bisogna aggiungere la configurazione
seguente:

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        assetic:
            filters:
                yui_js:
                    jar: "%kernel.root_dir%/Resources/java/yuicompressor.jar"

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <assetic:config>
            <assetic:filter
                name="yui_js"
                jar="%kernel.root_dir%/Resources/java/yuicompressor.jar" />
        </assetic:config>

    .. code-block:: php

        // app/config/config.php
        $container->loadFromExtension('assetic', array(
            'filters' => array(
                'yui_js' => array(
                    'jar' => '%kernel.root_dir%/Resources/java/yuicompressor.jar',
                ),
            ),
        ));

Ora, per *utilizzare* effettivamente il filtro su un gruppo di file JavaScript, bisogna aggiungerlo
nel template:

.. configuration-block::

    .. code-block:: html+jinja

        {% javascripts '@AcmeFooBundle/Resources/public/js/*' filter='yui_js' %}
        <script src="{{ asset_url }}"></script>
        {% endjavascripts %}

    .. code-block:: html+php

        <?php foreach ($view['assetic']->javascripts(
            array('@AcmeFooBundle/Resources/public/js/*'),
            array('yui_js')
        ) as $url): ?>
            <script src="<?php echo $view->escape($url) ?>"></script>
        <?php endforeach; ?>

Una guida più dettagliata sulla configurazione e l'utilizzo dei filtri di Assetic, oltre a
dettagli della modalità di debug di Assetic, si trova in :doc:`/cookbook/assetic/yuicompressor`.

Controllare l'URL utilizzato
----------------------------

Se lo si desidera, è possibile controllare gli URL che produce Assetic. Questo è
fatto dal template ed è relativo alla radice del documento pubblico:

.. configuration-block::

    .. code-block:: html+jinja

        {% javascripts '@AcmeFooBundle/Resources/public/js/*' output='js/compiled/main.js' %}
        <script src="{{ asset_url }}"></script>
        {% endjavascripts %}

    .. code-block:: html+php

        <?php foreach ($view['assetic']->javascripts(
            array('@AcmeFooBundle/Resources/public/js/*'),
            array(),
            array('output' => 'js/compiled/main.js')
        ) as $url): ?>
        <script src="<?php echo $view->escape($url) ?>"></script>
        <?php endforeach; ?>

.. note::

    Symfony contiene anche un metodo per *accelerare* la cache, in cui l'URL finale
    generato da Assetic contiene un parametro di query che può essere incrementato
    tramite la configurazione di ogni pubblicazione. Per ulteriori informazioni, vedere
    l'opzione di configurazione :ref:`ref-framework-assets-version`.

.. _cookbook-assetic-dumping:

Copiare i file delle risorse
----------------------------

Nell'ambiente ``dev``, Assetic genera percorsi a file CSS
e JavaScript che non esistono fisicamente sul computer. Ma vengono resi comunque,
perché un controllore interno di Symfony apre i file e ne restituisce il
contenuto (dopo aver eseguito eventuali filtri).

Questo tipo di pubblicazione dinamica delle risorse elaborate è ottima, perché significa
che si può immediatamente vedere il nuovo stato di tutti i file delle risorse modificate.
È anche un male, perché può essere molto lento. Se si stanno usando molti filtri,
potrebbe essere addirittura frustrante.

Fortunatamente, Assetic fornisce un modo per copiare le proprie risorse in file reali, anziché
farli generare dinamicamente.

Copiare i file delle risorse nell'ambiente ``prod``
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Nell'ambiente ``prod``, i file JS e CSS sono rappresentati da un unico
tag. In altre parole, invece di vedere ogni file JavaScript che che si sta includendo
nei sorgenti, è probabile che si veda qualcosa di questo tipo:

.. code-block:: html

    <script src="/app_dev.php/js/abcd123.js"></script>

Questo file in realtà **non** esiste, né viene reso dinamicamente
da Symfony (visto che i file di risorse sono nell'ambiente ``dev``).
Lasciare generare a Symfony questi file dinamicamente in un ambiente di
produzione sarebbe troppo lento.

Invece, ogni volta che si utilizza l'applicazione nell'ambiente ``prod`` (e quindi,
ogni volta che si fa un nuovo rilascio), è necessario eseguire il seguente task:

.. code-block:: bash

    $ php app/console assetic:dump --env=prod --no-debug

Questo genererà fisicamente e scriverà ogni file di cui si ha bisogno (ad esempio ``/js/abcd123.js``).
Se si aggiorna una qualsiasi delle risorse, sarà necessario eseguirlo di nuovo  per rigenerare
il file.

Copiare i file delle risorse nell'ambiente ``dev``
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Per impostazione predefinita, ogni percorso generato della risorsa nell'ambiente ``dev`` è gestito
dinamicamente da Symfony. Questo non ha alcun svantaggio (è possibile visualizzare immediatamente
le modifiche), salvo che le risorse verranno caricate sensibilmente lente. Se si ritiene che
le risorse vengano caricate troppo lentamente, seguire questa guida.

In primo luogo, dire a Symfony di smettere di cercare di elaborare questi file in modo dinamico. Fare
la seguente modifica nel file ``config_dev.yml``:

.. configuration-block::

    .. code-block:: yaml

        # app/config/config_dev.yml
        assetic:
            use_controller: false

    .. code-block:: xml

        <!-- app/config/config_dev.xml -->
        <assetic:config use-controller="false" />

    .. code-block:: php

        // app/config/config_dev.php
        $container->loadFromExtension('assetic', array(
            'use_controller' => false,
        ));

Poi, dato che Symfony non genererà più queste risorse dinamicamente,
bisognerà copiarle manualmente. Per fare ciò, eseguire il seguente comando:

.. code-block:: bash

    php app/console assetic:dump

Questo scrive fisicamente tutti i file delle risorse necessari per l'ambiente
``dev``. Il grande svantaggio è che è necessario eseguire questa operazione ogni volta
che si aggiorna una risorsa. Per fortuna, passando l'opzione ``--watch``, il
comando rigenererà automaticamente le risorse *che sono cambiate*:

.. code-block:: bash

    php app/console assetic:dump --watch

Dal momento che l'esecuzione di questo comando nell'ambiente ``dev`` può generare molti
file, di solito è una buona idea far puntare i file con le risorse generate in
una cartella separata (ad esempio ``/js/compiled``), per mantenere ordinate le cose:

.. configuration-block::

    .. code-block:: html+jinja

        {% javascripts '@AcmeFooBundle/Resources/public/js/*' output='js/compiled/main.js' %}
        <script src="{{ asset_url }}"></script>
        {% endjavascripts %}

    .. code-block:: html+php

        <?php foreach ($view['assetic']->javascripts(
            array('@AcmeFooBundle/Resources/public/js/*'),
            array(),
            array('output' => 'js/compiled/main.js')
        ) as $url): ?>
        <script src="<?php echo $view->escape($url) ?>"></script>
        <?php endforeach; ?>
