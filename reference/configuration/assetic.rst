.. index::
    pair: Assetic; Riferimento configurazione

Configurazione di AsseticBundle ("assetic")
===========================================

Configurazione predefinita completa
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

.. configuration-block::

    .. code-block:: yaml

        assetic:
            debug:                "%kernel.debug%"
            use_controller:
                enabled:              "%kernel.debug%"
                profiler:             false
            read_from:            "%kernel.root_dir%/../web"
            write_to:             "%assetic.read_from%"
            java:                 /usr/bin/java
            node:                 /usr/bin/node
            ruby:                 /usr/bin/ruby
            sass:                 /usr/bin/sass
            # Una coppia chiave-valore di un numero di elementi
            variables:
                un_nome:                 []
            bundles:

                # Predefiniti (tutti i bundle attualmente registrati):
                - FrameworkBundle
                - SecurityBundle
                - TwigBundle
                - MonologBundle
                - SwiftmailerBundle
                - DoctrineBundle
                - AsseticBundle
                - ...
            assets:
                # Un array di risorse (p.e. una_risorsa, un_altra_risorsa)
                una_risorsa:
                    inputs:               []
                    filters:              []
                    options:
                        # Un array chiave-valore di opzioni e valori
                        una_opzione: []
            filters:

                # Un array di filtri (p.e. un_filtro, un_altro_filtro)
                un_filtro:                []
            twig:
                functions:
                    # Un array di funzioni (p.e. una_funzione, un_altra_funzione)
                    una_funzione:         []

    .. code-block:: xml

        <assetic:config
            debug="%kernel.debug%"
            use-controller="%kernel.debug%"
            read-from="%kernel.root_dir%/../web"
            write-to="%assetic.read_from%"
            java="/usr/bin/java"
            node="/usr/bin/node"
            sass="/usr/bin/sass"
        >
            <!-- Predefiniti (tutti i bundle attualmente registrati) -->
            <assetic:bundle>FrameworkBundle</assetic:bundle>
            <assetic:bundle>SecurityBundle</assetic:bundle>
            <assetic:bundle>TwigBundle</assetic:bundle>
            <assetic:bundle>MonologBundle</assetic:bundle>
            <assetic:bundle>SwiftmailerBundle</assetic:bundle>
            <assetic:bundle>DoctrineBundle</assetic:bundle>
            <assetic:bundle>AsseticBundle</assetic:bundle>
            <assetic:bundle>...</assetic:bundle>

            <assetic:asset>
                <!-- prototype -->
                <assetic:name>
                    <assetic:input />

                    <assetic:filter />

                    <assetic:option>
                        <!-- prototype -->
                        <assetic:name />
                    </assetic:option>
                </assetic:name>
            </assetic:asset>

            <assetic:filter>
                <!-- prototype -->
                <assetic:name />
            </assetic:filter>

            <assetic:twig>
                <assetic:functions>
                    <!-- prototype -->
                    <assetic:name />
                </assetic:functions>
            </assetic:twig>

        </assetic:config>
