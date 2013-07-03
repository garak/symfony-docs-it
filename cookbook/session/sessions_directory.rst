.. index::
   single: Sessioni, cartella delle sessioni

Configurare la cartella in cui salvare le sessioni
==================================================

Per impostazione predefinita, Symfony memorizza i dati delle sessioni nella cartella cache.
Questo vuol dire che quando si pulisce la cache, ogni sessione attiva viene
cancellata.

Usando una cartella diversa per salvare i dati delle sessioni è uno dei metodi per
assicurarsi che le sessioni attive non vadano perdute durante la pulizia della cache.

.. tip::

    Un metodo eccellente (ma più complesso) consiste nell'uso di un gestore di sessioni
    diverso, disponibile in Symfony. Si veda
    :doc:`/components/http_foundation/session_configuration` per un
    approfondimento sui gestori di sessioni. C'è anche una ricetta sulla
    memorizzazione delle sessioni, :doc:`database</cookbook/configuration/pdo_session_storage>`.

Per modificare la cartella in cui Symfony salva i dati di sessione, è sufficiente
cambiare la configurazione. In questo esempio, la cartella delle sessioni
sarà cambiata in ``app/sessions``:

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        framework:
            session:
                save_path: "%kernel.root_dir%/sessions"

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <framework:config>
            <framework:session save-path="%kernel.root_dir%/sessions" />
        </framework:config>

    .. code-block:: php

        // app/config/config.php
        $container->loadFromExtension('framework', array(
            'session' => array('save-path' => "%kernel.root_dir%/sessions"),
        ));
