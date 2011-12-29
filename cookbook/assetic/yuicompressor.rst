Minimizzare i file JavaScript e i fogli di stile con YUI Compressor
===================================================================

Yahoo! mette a disposizione un'eccellente strumento per minimizzare i file JavaScipt
e i fogli di stile, che così possono viaggiare più velocemente sulla rete: lo `YUI Compressor`_. 
Grazie ad Assetic utilizzare questo strumento è semplicissimo.

Scaricare il JAR di YUI Compressor
----------------------------------

L'YUI Compresso è scritto in Java e viene distribuito in formato JAR. 
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
            'filters' => array(
                'yui_css' => array(
                    'jar' => '%kernel.root_dir%/Resources/java/yuicompressor.jar',
                ),
                'yui_js' => array(
                    'jar' => '%kernel.root_dir%/Resources/java/yuicompressor.jar',
                ),
            ),
        ));

Dall'applicazione si ha ora accesso a due nuovi filtri di Assetic:
``yui_css`` e ``yui_js``. Questi filtri utilizzeranno YUI Compressor per
minimizzare, rispettivamente, i fogli di stile e i file JavaScript.

Minimizzare le risorse
----------------------

YUI Compressor è stato configurato ma, prima di poter vedere i risultati, è
necessario applicare i filtri alle risorse. Visto che le risorse fanno parte del 
livello della vista, questo lavoro dovrà essere svolto nelle template:

.. configuration-block::

    .. code-block:: html+jinja

        {% javascripts '@AcmeFooBundle/Resources/public/js/*' filter='yui_js' %}
        <script src="{{ asset_url }}"></script>
        {% endjavascripts %}

    .. code-block:: html+php

        <?php foreach ($view['assetic']->javascripts(
            array('@AcmeFooBundle/Resources/public/js/*'),
            array('yui_js')) as $url): ?>
        <script src="<?php echo $view->escape($url) ?>"></script>
        <?php endforeach; ?>

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

        {% stylesheets '@AcmeFooBundle/Resources/public/css/*' filter='yui_css' %}
        <link rel="stylesheet" type="text/css" media="screen" href="{{ asset_url }}" />
        {% endstylesheets %}

    .. code-block:: html+php

        <?php foreach ($view['assetic']->stylesheets(
            array('@AcmeFooBundle/Resources/public/css/*'),
            array('yui_css')) as $url): ?>
        <link rel="stylesheet" type="text/css" media="screen" href="<?php echo $view->escape($url) ?>" />
        <?php endforeach; ?>

Disabilitare la minimizzazione in modalità debug
------------------------------------------------

I file JavaScript e i fogli di stile minimizzati sono difficili da leggere
e ancora più difficili da correggere. Per questo motivo Assetic permette di disabilitare 
determinati filtri  quando l'applicazione viene eseguita in modalità debug.
Mettendo il prefisso punto interrogativo ``?`` al nome dei filtri si chiede 
ad Assetic di applicarli solamente quando la modalità debug è spenta.

.. configuration-block::

    .. code-block:: html+jinja

        {% javascripts '@AcmeFooBundle/Resources/public/js/*' filter='?yui_js' %}
        <script src="{{ asset_url }}"></script>
        {% endjavascripts %}

    .. code-block:: html+php

        <?php foreach ($view['assetic']->javascripts(
            array('@AcmeFooBundle/Resources/public/js/*'),
            array('?yui_js')) as $url): ?>
        <script src="<?php echo $view->escape($url) ?>"></script>
        <?php endforeach; ?>

.. _`YUI Compressor`: http://developer.yahoo.com/yui/compressor/
.. _`scaricare il file JAR`: http://yuilibrary.com/downloads/#yuicompressor
