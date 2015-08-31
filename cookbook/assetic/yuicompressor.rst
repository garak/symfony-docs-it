.. index::
   single: Assetic; YUI Compressor

Minimizzare file JavaScript e fogli di stile con YUI Compressor
===============================================================

.. caution::

    YUI Compressor `non è più mantenuto da Yahoo`_. Per questo motivo, è
    **caldamente consigliato di evitare l'uso di YUI**, a meno che non sia strettamente
    necessario. Leggere :doc:`/cookbook/assetic/uglifyjs` per un'alternativa aggiornata.

Yahoo! mette a disposizione un eccellente strumento per minimizzare i file JavaScript
e i fogli di stile, che così possono viaggiare più velocemente sulla rete: lo `YUI Compressor`_. 
Grazie ad Assetic utilizzare questo strumento è semplicissimo.

Scaricare il JAR di YUI Compressor
----------------------------------

L'YUI Compressor è scritto in Java e viene distribuito in formato JAR. 
Si dovrà `scaricare il file JAR`_ e salvarlo in ``app/Resources/java/yuicompressor.jar``.

Configurare i filtri per YUI
----------------------------

È necessario configurare due filtri Assetic all'interno dell'applicazione. Uno
per minimizzare i file JavaScript e uno per minimizzare i fogli di stile 
con YUI Compressor:

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        assetic:
            # java: "/usr/bin/java"
            filters:
                yui_css:
                    jar: "%kernel.root_dir%/Resources/java/yuicompressor.jar"
                yui_js:
                    jar: "%kernel.root_dir%/Resources/java/yuicompressor.jar"

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <assetic:config>
            <assetic:filter
                name="yui_css"
                jar="%kernel.root_dir%/Resources/java/yuicompressor.jar" />
            <assetic:filter
                name="yui_js"
                jar="%kernel.root_dir%/Resources/java/yuicompressor.jar" />
        </assetic:config>

    .. code-block:: php

        // app/config/config.php
        $container->loadFromExtension('assetic', array(
            // 'java' => '/usr/bin/java',
            'filters' => array(
                'yui_css' => array(
                    'jar' => '%kernel.root_dir%/Resources/java/yuicompressor.jar',
                ),
                'yui_js' => array(
                    'jar' => '%kernel.root_dir%/Resources/java/yuicompressor.jar',
                ),
            ),
        ));

.. note::

    Gli utenti Windows ricordino di aggiornare la configurazione con il percorso corretto di java. 
    Quello predefinito in Windows7 x64 bit è ``C:\Program Files (x86)\Java\jre6\bin\java.exe``.

Dall'applicazione si ha ora accesso a due nuovi filtri di Assetic:
``yui_css`` e ``yui_js``. Questi filtri utilizzeranno YUI Compressor per
minimizzare, rispettivamente, i fogli di stile e i file JavaScript.

Minimizzare le risorse
----------------------

YUI Compressor è stato configurato, ma, prima di poter vedere i risultati, è
necessario applicare i filtri alle risorse. Visto che le risorse fanno parte del 
livello della vista, questo lavoro dovrà essere svolto nei template:

.. configuration-block::

    .. code-block:: html+jinja

        {% javascripts '@AppBundle/Resources/public/js/*' filter='yui_js' %}
            <script src="{{ asset_url }}"></script>
        {% endjavascripts %}

    .. code-block:: html+php

        <?php foreach ($view['assetic']->javascripts(
            array('@AppBundle/Resources/public/js/*'),
            array('yui_js')
        ) as $url): ?>
            <script src="<?php echo $view->escape($url) ?>"></script>
        <?php endforeach ?>

.. note::

    Il precedente esempio presuppone che ci sia un bundle chiamato ``AcmeFooBundle``
    e che i file JavaScript si trovino nella cartella ``Resources/public/js`` 
    all'interno del bundle. È comunque possibile includere file JavaScript
    che si trovino in posizioni differenti.

Con l'aggiunta del filtro ``yui_js`` dell'esempio precedente, i file minimizzati
viaggeranno molto più velocemente sulla rete. Lo stesso procedimento può essere
ripetuto per minimizzare i fogli di stile.

.. configuration-block::

    .. code-block:: html+jinja

        {% stylesheets '@AppBundle/Resources/public/css/*' filter='yui_css' %}
            <link rel="stylesheet" type="text/css" media="screen" href="{{ asset_url }}" />
        {% endstylesheets %}

    .. code-block:: html+php

        <?php foreach ($view['assetic']->stylesheets(
            array('@AppBundle/Resources/public/css/*'),
            array('yui_css')
        ) as $url): ?>
            <link rel="stylesheet" type="text/css" media="screen" href="<?php echo $view->escape($url) ?>" />
        <?php endforeach ?>

Disabilitare la minimizzazione in modalità debug
------------------------------------------------

I file JavaScript e i fogli di stile minimizzati sono difficili da leggere
e ancora più difficili da correggere. Per questo motivo Assetic permette di disabilitare 
determinati filtri quando l'applicazione viene eseguita in modalità debug.
Mettendo il prefisso punto interrogativo ``?`` al nome dei filtri, si chiede 
ad Assetic di applicarli solamente quando la modalità debug è inattiva.

.. configuration-block::

    .. code-block:: html+jinja

        {% javascripts '@AppBundle/Resources/public/js/*' filter='?yui_js' %}
            <script src="{{ asset_url }}"></script>
        {% endjavascripts %}

    .. code-block:: html+php

        <?php foreach ($view['assetic']->javascripts(
            array('@AppBundle/Resources/public/js/*'),
            array('?yui_js')
        ) as $url): ?>
            <script src="<?php echo $view->escape($url) ?>"></script>
        <?php endforeach ?>

.. tip::

    Invece di aggiungere il filtro ai tag degli asset, lo si può abilitare globalmente,
    aggiungendo l'attributo ``apply-to`` alla configurazione del filtro, per esempio
    nel filtro yui_js ``apply_to: "\.js$"``. Per avere un unico filtro applicato
    in produzione, aggiungerlo al file config_prod invece che al file comune
    config. Per dettagli sull'applicazione dei filtri per estensione di file,
    vedere :ref:`cookbook-assetic-apply-to`.

.. _`YUI Compressor`: http://developer.yahoo.com/yui/compressor/
.. _`scaricare il file JAR`: https://github.com/yui/yuicompressor/releases
.. _`non è più mantenuto da Yahoo`: http://www.yuiblog.com/blog/2013/01/24/yui-compressor-has-a-new-owner/
