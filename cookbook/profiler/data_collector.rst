.. index::
   single: Profilazione; Raccoglitore di dati

Come creare un raccoglitore di dati personalizzato
==================================================

Il :ref:`profilatore <internals-profiler>` di Symfony delega la raccolta di dati
ai raccoglitori di dati. Symfony dispone di un paio di raccoglitori, ma se ne
possono creare di personalizzati.

Creare un raccoglitore di dati personalizzato
---------------------------------------------

Creare un raccoglitore di dati personalizzato è semplice, basta implementare
:class:`Symfony\\Component\\HttpKernel\\DataCollector\\DataCollectorInterface`::

    interface DataCollectorInterface
    {
        /**
         * Collects data for the given Request and Response.
         *
         * @param Request    $request   A Request instance
         * @param Response   $response  A Response instance
         * @param \Exception $exception An Exception instance
         */
        function collect(Request $request, Response $response, \Exception $exception = null);

        /**
         * Returns the name of the collector.
         *
         * @return string The collector name
         */
        function getName();
    }

Il metodo ``getName()`` deve restituire un nome univoco. Viene usato per accedere
successivamente all'informazione (vedere :doc:`/cookbook/testing/profiling`, per
esempio).

Il metodo ``collect()`` è responsabile della memorizzazione dei dati, a cui vuole dare
accesso, in proprietà locali.

.. caution::

    Siccome il profilatore serializza istanze di raccoglitori di dati, non si dovrebbero
    memorizzare oggetti che non possono essere serializzati (come gli oggetti PDO),
    altrimenti occorre fornire il proprio metodo ``serialize()``.

La maggior parte delle volte, conviene estendere
:class:`Symfony\\Component\\HttpKernel\\DataCollector\\DataCollector` e
popolare la proprietà ``$this->data`` (che si occupa di serializzare la proprietà
``$this->data``)::

    class MemoryDataCollector extends DataCollector
    {
        public function collect(Request $request, Response $response, \Exception $exception = null)
        {
            $this->data = array(
                'memory' => memory_get_peak_usage(true),
            );
        }

        public function getMemory()
        {
            return $this->data['memory'];
        }

        public function getName()
        {
            return 'memory';
        }
    }

.. _data_collector_tag:

Abilitare i raccoglitori di dati personalizzati
-----------------------------------------------

Per abilitare un raccoglitore di dati, aggiungerlo come servizio in una delle proprie
configurazioni e assegnarli il tag ``data_collector``:

.. configuration-block::

    .. code-block:: yaml

        services:
            data_collector.nome_del_raccoglitore:
                class: Nome\Classe\Raccoglitore
                tags:
                    - { name: data_collector }

    .. code-block:: xml

        <service id="data_collector.nome_del_raccoglitore" class="Nome\Classe\Raccoglitore">
            <tag name="data_collector" />
        </service>

    .. code-block:: php

        $container
            ->register('data_collector.nome_del_raccoglitore', 'Nome\Classe\Raccoglitore')
            ->addTag('data_collector')
        ;

Aggiungere template al profilatore web
--------------------------------------

Quando si vogliono mostrare i dati raccolti dal proprio raccoglitore di dati nella
barra di debug del web, oppure nel profilatore web, creare un template Twig, seguendo
questo scheletro:

.. code-block:: jinja

    {% extends 'WebProfilerBundle:Profiler:layout.html.twig' %}

    {% block toolbar %}
        {# Questa barra può apparire in cima allo schermo.#}
        {% set icon %}
        <span class="icon"><img src="data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAABoAAAAcCAQAAADVGmdYAAAAAmJLR0QA/4ePzL8AAAAJcEhZcwAACxMAAAsTAQCanBgAAAAHdElNRQffAxkBCDStonIVAAAAGXRFWHRDb21tZW50AENyZWF0ZWQgd2l0aCBHSU1QV4EOFwAAAHpJREFUOMtj3PWfgXRAuqZd/5nIsIdhVBPFmgqIjCuYOrJsYtz1fxuUOYER2TQID8afwIiQ8YIkI4TzCv5D2AgaWSuExJKMIDbA7EEVhQEWXJ6FKUY4D48m7HYU/EcWZ8JlE6qfMELPDcUJuEMPxvYazYTDWRMjOcUyAEswO+VjeQQaAAAAAElFTkSuQmCC" alt=""/></span>
        <span class="sf-toolbar-status">Esempio</span>
        {% endset %}

        {% set text %}
        <div class="sf-toolbar-info-piece">
            <b>Un breve pezzo di dati</b>
            <span>100 unità</span>
        </div>
        <div class="sf-toolbar-info-piece">
            <b>Un altro pezzo</b>
            <span>300 unità</span>
        </div>
        {% endset %}

        {# Impostare il valore "link" a false se no si ha una gorssa sezione "panel" 
           a cui si vuole rimandare l'utente. #}
        {% include '@WebProfiler/Profiler/toolbar_item.html.twig' with { 'link': true } %}

    {% endblock %}

    {% block head %}
        {# facoltativo, se il profilatore web ha bisogno di file JS o CSS #}
        {{ parent() }} {# Usare parent() per mantenere gli stili predefiniti #}        
    {% endblock %}

    {% block menu %}
        {# Questo menu a sinistra appare usando il profilatore a pieno schermo. #}
        <span class="label">
            <span class="icon"><img src="data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAABoAAAAcCAQAAADVGmdYAAAAAmJLR0QA/4ePzL8AAAAJcEhZcwAACxMAAAsTAQCanBgAAAAHdElNRQffAxkBCDStonIVAAAAGXRFWHRDb21tZW50AENyZWF0ZWQgd2l0aCBHSU1QV4EOFwAAAHpJREFUOMtj3PWfgXRAuqZd/5nIsIdhVBPFmgqIjCuYOrJsYtz1fxuUOYER2TQID8afwIiQ8YIkI4TzCv5D2AgaWSuExJKMIDbA7EEVhQEWXJ6FKUY4D48m7HYU/EcWZ8JlE6qfMELPDcUJuEMPxvYazYTDWRMjOcUyAEswO+VjeQQaAAAAAElFTkSuQmCC" alt=""/></span>
            <strong>Esempio di raccoglitore</strong>
        </span>
    {% endblock %}

    {% block panel %}
        {# Facoltativo, per mostrare più dettagli. #} 
        <h2>Esempio</h2>
        <p>
            <em>Informazioni dettagliate vanno qui</em>
        </p>
    {% endblock %}

I blocchi sono tutti facoltativi. Il blocco ``toolbar`` è usato per la barra di debug
del web, mentre ``menu`` e ``panel`` sono usati per aggiungere un pannello al
profilatore web.

Tutti i blocchi hanno accesso all'oggetto ``collector``.

.. tip::

    I template predefiniti usano immagini codificate in base64 per la barra:

    .. code-block:: html

        <img src="data:image/png;base64,..." />

    Si può calcolare facilmente il valore base64 di un'immagine con questo
    piccolo script::

        #!/usr/bin/env php
        <?php
        echo base64_encode(file_get_contents($_SERVER['argv'][1]));

Per abilitare il template, aggiungere un attributo ``template`` al tag ``data_collector``
nella configurazione. Per esempio, ipotizzando che il template sia in un
``AcmeDebugBundle``:

.. configuration-block::

    .. code-block:: yaml

        services:
            data_collector.nome_del_raccoglitore:
                class: Acme\DebugBundle\Nome\Classe\Raccoglitore
                tags:
                    - { name: data_collector, template: "AcmeDebugBundle:Collector:nometemplate", id: "nome_del_raccoglitore" }

    .. code-block:: xml

        <service id="data_collector.your_collector_name" class="Acme\DebugBundle\Nome\Classe\Raccoglitore">
            <tag name="data_collector" template="AcmeDebugBundle:Collector:nometemplate" id="nome_del_raccoglitore" />
        </service>

    .. code-block:: php

        $container
            ->register('data_collector.nome_del_raccoglitore', 'Acme\DebugBundle\Nome\Classe\Raccoglitore')
            ->addTag('data_collector', array(
                'template' => 'AcmeDebugBundle:Collector:nometemplate',
                'id'       => 'nome_del_raccoglitore',
            ))
        ;

.. caution::

    Assicurarsi che l'attributo ``id`` sia la stessa stringa usata per il metodo
    ``getName()``.
