.. index::
   single: Assetic; Applicare filtri

Applicare i filtri di Assetic a file con specifiche estensioni
==============================================================

I filtri di Assetic possono essere applicati a singoli file, gruppi di file o anche, 
come vedremo, a file che hanno una specifica estensione. Per mostrare 
l'utilizzo di ogni opzione, supponiamo di voler usare il filtro CoffeeScript 
di Assetic che compila i file CoffeeScript in Javascript.

La configurazione prevede semplicemente di definire i percorsi per coffee e per node.
I valori predefiniti sono ``/usr/bin/coffee`` e ``/usr/bin/node``:

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        assetic:
            filters:
                coffee:
                    bin:        /usr/bin/coffee
                    node:       /usr/bin/node
                    node_paths: [/usr/lib/node_modules/]

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <assetic:config>
            <assetic:filter
                name="coffee"
                bin="/usr/bin/coffee/"
                node="/usr/bin/node/">
                <assetic:node-path>/usr/lib/node_modules/</assetic:node-path>
            </assetic:filter>
        </assetic:config>

    .. code-block:: php

        // app/config/config.php
        $container->loadFromExtension('assetic', array(
            'filters' => array(
                'coffee' => array(
                    'bin'  => '/usr/bin/coffee',
                    'node' => '/usr/bin/node',
                    'node_paths' => array('/usr/lib/node_modules/'),
                ),
            ),
        ));

Filtrare un singolo file
------------------------

In questo modo sarà possibile inserire un singolo file CoffeScript nel template,
come se fosse un normale JavaScript:

.. configuration-block::

    .. code-block:: html+jinja

        {% javascripts '@AppBundle/Resources/public/js/esempio.coffee' filter='coffee' %}
            <script src="{{ asset_url }}"></script>
        {% endjavascripts %}

    .. code-block:: html+php

        <?php foreach ($view['assetic']->javascripts(
            array('@AppBundle/Resources/public/js/esempio.coffee'),
            array('coffee')
        ) as $url): ?>
            <script src="<?php echo $view->escape($url) ?>"></script>
        <?php endforeach ?>

Questo è tutto quel che serve per compilare il file CoffeeScript e restituirlo
come un normale JavaScript.

Filtrare file multpili
----------------------

È anche possibile combinare diversi file CoffeeScript in un singolo file:

.. configuration-block::

    .. code-block:: html+jinja

        {% javascripts '@AppBundle/Resources/public/js/esempio.coffee'
                       '@AppBundle/Resources/public/js/altro.coffee'
            filter='coffee' %}
            <script src="{{ asset_url }}"></script>
        {% endjavascripts %}

    .. code-block:: html+php

        <?php foreach ($view['assetic']->javascripts(
            array(
                '@AppBundle/Resources/public/js/esempio.coffee',
                '@AppBundle/Resources/public/js/altro.coffee',
            ),
            array('coffee')
        ) as $url): ?>
            <script src="<?php echo $view->escape($url) ?>"></script>
        <?php endforeach ?>

Tutti i file verranno restituiti e compilati in un unico, regolare file JavaScript.

.. _cookbook-assetic-apply-to:

Filtrare in base all'estensione del file
----------------------------------------

Uno dei grandi vantaggi nell'utilizzo di Assetic è quello di ridurre il numero
di file di risorse, riducendo così le richieste HTTP. Per massimizzarne 
i vantaggi, sarebbe utile combinare insieme *tutti* i file JavaScript e quelli CoffeeScript in uno unico, 
visto che verranno tutti serviti come file JavaScript. Sfortunatamente non è possibile aggiungere 
semplicemente un file JavaScript ai file precedenti, per via del fatto che il file 
JavaScript non supererebbe la compilazione di CoffeeScript.

Questo problema può essere ovviato utilizzando l'opzione ``apply_to`` nella configurazione,
in modo da specificare che il filtro dovrà essere applicato solo ai file con una 
determinata estensione. In questo caso si dovrà specificare che il filtro Coffee
dovrà applicarsi a tutti e soli i file ``.coffee``:

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        assetic:
            filters:
                coffee:
                    bin:        /usr/bin/coffee
                    node:       /usr/bin/node
                    node_paths: [/usr/lib/node_modules/]
                    apply_to:   "\.coffee$"

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <assetic:config>
            <assetic:filter
                name="coffee"
                bin="/usr/bin/coffee"
                node="/usr/bin/node"
                apply_to="\.coffee$" />
                <assetic:node-paths>/usr/lib/node_modules/</assetic:node-path>
        </assetic:config>

    .. code-block:: php

        // app/config/config.php
        $container->loadFromExtension('assetic', array(
            'filters' => array(
                'coffee' => array(
                    'bin'      => '/usr/bin/coffee',
                    'node'     => '/usr/bin/node',
                    'node_paths' => array('/usr/lib/node_modules/'),
                    'apply_to' => '\.coffee$',
                ),
            ),
        ));

In questo modo non è più necessario specificare il filtro ``coffee`` nel template.
È anche possibile elencare i normali file JavaScript, i quali verranno combinati e restituiti 
come un unico file JavaScript (e in modo tale che i soli file ``.coffee`` vengano elaborati
dal filtro CoffeeScript):

.. configuration-block::

    .. code-block:: html+jinja

        {% javascripts '@AppBundle/Resources/public/js/esempio.coffee'
                       '@AppBundle/Resources/public/js/altro.coffee'
                       '@AppBundle/Resources/public/js/regolare.js' %}
            <script src="{{ asset_url }}"></script>
        {% endjavascripts %}

    .. code-block:: html+php

        <?php foreach ($view['assetic']->javascripts(
            array(
                '@AppBundle/Resources/public/js/esempio.coffee',
                '@AppBundle/Resources/public/js/altro.coffee',
                '@AppBundle/Resources/public/js/regolare.js',
            )
        ) as $url): ?>
            <script src="<?php echo $view->escape($url) ?>"></script>
        <?php endforeach ?>
