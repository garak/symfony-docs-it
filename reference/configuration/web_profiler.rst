.. index::
   single: Riferimento configurazione; WebProfiler

Configurazione di WebProfilerBundle
===================================

Configurazione completa predefinita
-----------------------------------

.. configuration-block::

    .. code-block:: yaml

        web_profiler:
            
            # mostra informazioni secondarie per rendere la barra più corta
            verbose:             true

            # mostra la barra di web debug in fondo alle pagine con un riassunto delle info del profilatore
            toolbar:             false

            # dà la possibilità di guardare i dati raccolti prima di seguire il rinvio
            intercept_redirects: false
