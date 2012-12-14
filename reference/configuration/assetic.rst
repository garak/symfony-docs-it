.. index::
   pair: Assetic; Riferimento configurazione

Riferimento configurazione AsseticBundle
========================================

Configurazione predefinita completa
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

.. configuration-block::

    .. code-block:: yaml

        assetic:
            debug:                %kernel.debug%
            use_controller:
                enabled:              %kernel.debug%
                profiler:             false
            read_from:            %kernel.root_dir%/../web
            write_to:             %assetic.read_from%
            java:                 /usr/bin/java
            node:                 /usr/bin/node
            ruby:                 /usr/bin/ruby
            sass:                 /usr/bin/sass
            # Una coppia chiave-valore di un numero di elementi
            variables:
                some_name:                 []
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
                some_asset:
                    inputs:               []
                    filters:              []
                    options:
                        # Un array chiave-valore di opzioni e valori
                        some_option_name: []
            filters:

                # Un array di filtri (p.e. un_filtro, un_altro_filtro)
                some_filter:                 []
            twig:
                functions:
                    # Un array di funzioni (p.e. una_funzione, un_altra_funzione)
                    some_function:                 []
