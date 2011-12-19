Usare Assetic per l'ottimizzazione delle immagini con le funzioni di Twig
=========================================================================

Tra i vari filtri di Assetic, ve ne sono quattro che possono essere utilizzati per
ottimizzare le immagini al volo. Ciò permette di avere immagini di dimensioni inferiori
senza ricorrere ad un editor grafico per ogni modifica. Il risultato
dei filtri può essere messo in cache e usato in fase di produzione in modo da
eliminare problemi di performance per l'utente finale.

Usare Jpegoptim
---------------

`Jpegoptim`_ è uno strumento per ottimizzare i file JPEG. Per poterlo usare
si aggiunge il seguente codice alla configurazione di Assetic:

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        assetic:
            filters:
                jpegoptim:
                    bin: percorso/per/jpegoptim

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <assetic:config>
            <assetic:filter
                name="jpegoptim"
                bin="percorso/per/jpegoptim" />
        </assetic:config>

    .. code-block:: php

        // app/config/config.php
        $container->loadFromExtension('assetic', array(
            'filters' => array(
                'jpegoptim' => array(
                    'bin' => 'percorso/per/jpegoptim',
                ),
            ),
        ));

.. note::

    Per poter utilizzare jpegoptim è necessario che sia già installato sul
    proprio computer. L'opzione ``bin`` indica la posizione del programma eseguibile.

Sarà ora possibile usarlo nelle proprie template:

.. configuration-block::

    .. code-block:: html+jinja

        {% image '@AcmeFooBundle/Resources/public/images/esempio.jpg'
            filter='jpegoptim' output='/images/esempio.jpg'
        %}
        <img src="{{ asset_url }}" alt="Esempio"/>
        {% endimage %}

    .. code-block:: html+php

        <?php foreach ($view['assetic']->images(
            array('@AcmeFooBundle/Resources/public/images/esempio.jpg'),
            array('jpegoptim')) as $url): ?>
        <img src="<?php echo $view->escape($url) ?>" alt="Esempio"/>
        <?php endforeach; ?>

Rimozione dei dati EXIF 
~~~~~~~~~~~~~~~~~~~~~~~

Senza ulteirori opzioni, questo filtro rimuove solamente le meta informazioni
contenute nel file. I dati EXIF e i commenti non vengono eliminati: è comunque possibile
rimuoverli usando l'opzione ``strip_all``:

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        assetic:
            filters:
                jpegoptim:
                    bin: percorso/per/jpegoptim
                    strip_all: true

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <assetic:config>
            <assetic:filter
                name="jpegoptim"
                bin="percorso/per/jpegoptim"
                strip_all="true" />
        </assetic:config>

    .. code-block:: php

        // app/config/config.php
        $container->loadFromExtension('assetic', array(
            'filters' => array(
                'jpegoptim' => array(
                    'bin' => 'percorso/per/jpegoptim',
                    'strip_all' => 'true',
                ),
            ),
        ));

Diminuire la qualità massima
~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Senza ulteriori opzioni, la qualità dell'immagine JPEG non viene modificata. 
È però possibile ridurre ulteriormente la dimensione del file configurando il livello
di qualità massima per le immagini ad un livello inferiore di quello delle immagini stesse.
Ovviamente questo altererà la qualità dell'immagine:

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        assetic:
            filters:
                jpegoptim:
                    bin: percorso/per/jpegoptim
                    max: 70

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <assetic:config>
            <assetic:filter
                name="jpegoptim"
                bin="percorso/per/jpegoptim"
                max="70" />
        </assetic:config>

    .. code-block:: php

        // app/config/config.php
        $container->loadFromExtension('assetic', array(
            'filters' => array(
                'jpegoptim' => array(
                    'bin' => 'percorso/per/jpegoptim',
                    'max' => '70',
                ),
            ),
        ));

Abbreviare la sintassi: le funzioni di Twig
-------------------------------------------

Se si utilizza Twig è possibile inserire tutte queste opzioni con una sintassi
più concisa abilitando alcune speciali funzioni di Twig. Si inizia con
modificare la configurazione come nel seguito:

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        assetic:
            filters:
                jpegoptim:
                    bin: percorso/per/jpegoptim
            twig:
                functions:
                    jpegoptim: ~

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <assetic:config>
            <assetic:filter
                name="jpegoptim"
                bin="percorso/per/jpegoptim" />
            <assetic:twig>
                <assetic:twig_function
                    name="jpegoptim" />
            </assetic:twig>
        </assetic:config>

    .. code-block:: php

        // app/config/config.php
        $container->loadFromExtension('assetic', array(
            'filters' => array(
                'jpegoptim' => array(
                    'bin' => 'percorso/per/jpegoptim',
                ),
            ),
            'twig' => array(
                'functions' => array('jpegoptim'),
                ),
            ),
        ));

A questo punto il template di Twig può essere modificato nel seguente modo:

.. code-block:: html+jinja

    <img src="{{ jpegoptim('@AcmeFooBundle/Resources/public/images/esempio.jpg') }}"
         alt="Esempio"/>

È possibile specificare la cartella di output nel seguente modo:

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        assetic:
            filters:
                jpegoptim:
                    bin: percorso/per/jpegoptim
            twig:
                functions:
                    jpegoptim: { output: images/*.jpg }

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <assetic:config>
            <assetic:filter
                name="jpegoptim"
                bin="percorso/per/jpegoptim" />
            <assetic:twig>
                <assetic:twig_function
                    name="jpegoptim"
                    output="images/*.jpg" />
            </assetic:twig>
        </assetic:config>

    .. code-block:: php

        // app/config/config.php
        $container->loadFromExtension('assetic', array(
            'filters' => array(
                'jpegoptim' => array(
                    'bin' => 'percorso/per/jpegoptim',
                ),
            ),
            'twig' => array(
                'functions' => array(
                    'jpegoptim' => array(
                        output => 'images/*.jpg'
                    ),
                ),
            ),
        ));

.. _`Jpegoptim`: http://www.kokkonen.net/tjko/projects.html
