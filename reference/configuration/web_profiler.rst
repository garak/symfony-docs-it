.. index::
    single: Riferimento configurazione; WebProfiler

Configurazione di WebProfilerBundle ("web_profiler")
====================================================

Configurazione completa predefinita
-----------------------------------

.. configuration-block::

    .. code-block:: yaml

        web_profiler:

            # DEPRECATO, non è più utile e può essere rimosso senza problemi
            verbose:              true

            # mostra la barra di web debug in fondo alle pagine con un riassunto delle info del profilatore
            toolbar:              false
            position:             bottom

            # dà l'opportunità di guardare i dati raccolti prima di seguire il rinvio
            intercept_redirects: false

    .. code-block:: xml

        <web-profiler:config
            toolbar="false"
            verbose="true"
            intercept_redirects="false"
        />
