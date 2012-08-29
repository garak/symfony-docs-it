.. index::
   single: Log

Come scrivere messaggi di log su file diversi
=============================================

.. versionadded:: 2.1
    La possibilità di specificare canali per gestori specifici è stata aggiunta
    in MonologBundle per Symfony 2.1.

La Standard Edition di Symfony contiene molti canali per i log: ``doctrine``,
``event``, ``security`` e ``request``. Ogni canale corrisponde a un servizio di
log (``monolog.logger.XXX``) nel contenitore ed è iniettato nel servizio
interessato. Lo scopo dei canali è quello di poter organizzare diversi
tipi di messaggi di log.

Per impostazione predefinita, Symfony2 scrive ogni messaggio di log in un singolo file
(indipendentemente dal canale).

Spostare un canale su un gestore diverso
----------------------------------------

Si supponga ora di voler loggare il canale ``doctrine`` in un altro file.

Per farlo, basta creare un nuovo gestore e configurarlo in questo modo:

.. configuration-block::

    .. code-block:: yaml

        monolog:
            handlers:
                main:
                    type: stream
                    path: /var/log/symfony.log
                    channels: !doctrine
                doctrine:
                    type: stream
                    path: /var/log/doctrine.log
                    channels: doctrine

    .. code-block:: xml

        <monolog:config>
            <monolog:handlers>
                <monolog:handler name="main" type="stream" path="/var/log/symfony.log">
                    <monolog:channels>
                        <type>exclusive</type>
                        <channel>doctrine</channel>
                    </monolog:channels>
                </monolog:handler>

                <monolog:handler name="doctrine" type="stream" path="/var/log/doctrine.log" />
                    <monolog:channels>
                        <type>inclusive</type>
                        <channel>doctrine</channel>
                    </monolog:channels>
                </monolog:handler>
            </monolog:handlers>
        </monolog:config>

Specifiche YAML
---------------

Si può specificare la configurazione in molte forme:

.. code-block:: yaml

    channels: ~    # Include tutti i canali

    channels: pippo  # Include solo il canale "pippo"
    channels: !pippo # Include tutti i canali, tranne "pippo"

    channels: [pippo, pluto]   # Include solo i canali "pippo" e "pluto"
    channels: [!pippo, !pluto] # Include tutti i canali, tranne "pippo" e "pluto"

    channels:
        type:     inclusive # Include solo quelli elencati sotto
        elements: [ pippo, pluto ]
    channels:
        type:     exclusive # Include tutto, tranne quelli elencati sotto
        elements: [ pippo, pluto ]

Creare il proprio canale
------------------------

Si può cambiare il canale usato da monolog su un servizio alla volta. Lo si può fare
aggiungendo il tag ``monolog.logger`` a un servizio e specificando quale canale il
servizio dovrebbe usare per i log. In questo modo, il logger iniettato in questo
servizio viene preconfigurarto per usare il canale specificato.

Per maggiori informazioni, incluso un esempio completo, leggere ":ref:`dic_tags-monolog`",
nella sezione di riferimento sui tag della dependency injection.

Imparare di più con il ricettario
---------------------------------

* :doc:`/cookbook/logging/monolog`
