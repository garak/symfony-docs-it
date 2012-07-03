.. index::
   single: Riferimento configurazione; WebProfiler

Configurazione di WebProfilerBundle
===================================

Configurazione completa predefinita
-----------------------------------

.. configuration-block::

    .. code-block:: yaml

        web_profiler:
            
            # DEPRECATO, non è più utile e può essere rimosso senza problemi
            verbose:             true

            # mostra la barra di web debug in fondo alle pagine con un riassunto delle info del profilatore
            toolbar:             false
            position:             bottom
            intercept_redirects:  false

