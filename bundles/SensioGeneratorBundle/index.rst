SensioGeneratorBundle
=====================

Il bundle ``SensioGeneratorBundle`` estende l'interfaccia a linea di comando di Symfony2,
fornendo nuovi comandi interattivi e intuitivi, per generare scheletri di codice, come
bundle, classi di form o controllori CRUD basati su uno schema di
Doctrine 2.

Installazione
-------------

`Scaricare`_ il bundle e metterlo sotto lo spazio dei nomi ``Sensio\\Bundle\\``.
Quindi, come per ogni altro bundle, includerlo nella propria classe Kernel::

    public function registerBundles()
    {
        $bundles = array(
            ...

            new Sensio\Bundle\GeneratorBundle\SensioGeneratorBundle(),
        );

        ...
    }

Elenco dei comandi disponibili
------------------------------

Il bundle ``SensioGeneratorBundle`` dispone di quattro nuovi comandi, eseguibili in
modalità interattiva o meno. La modalità interattiva pone alcune domande, per configurare
i parametri di generazione del codice. L'elenco dei nuovi comandi è il
seguente:

.. toctree::
   :maxdepth: 1

   commands/generate_bundle
   commands/generate_controller
   commands/generate_doctrine_crud
   commands/generate_doctrine_entity
   commands/generate_doctrine_form

.. _Scaricare: http://github.com/sensio/SensioGeneratorBundle

Sovrascrivere i template scheletro
----------------------------------

.. versionadded:: 2.3
  La possibilità di sovrascrivere i template scheletro è stata aggiunta nella versione 2.3.

Tutti i generatori usanto un template scheletro per generare file. Per impostazione predefinita,
i comandi usano i template forniti dal bundle, sotto la cartella
``Resources/skeleton``.

Si possono definire template scheletro personalizzati, creando la stessa cartella e
struttura di file in ``APP_PATH/Resources/SensioGeneratorBundle/skeleton`` o
``BUNDLE_PATH/Resources/SensioGeneratorBundle/skeleton``, se si vuole estendere
il bundle del generatore (per poter condividere i propri template in molti
progetti).

Per esempio, se si vuole sovrascrivere il template ``edit`` per il generatore di CRUD,
creare un file ``crud/views/edit.html.twig.twig`` sotto
``APP_PATH/Resources/SensioGeneratorBundle/skeleton``.

Quando si sovrascrive un template, dare un'occhiata ai template predefiniti, per capire gli
elementi a disposizione riguardo a template, percorsi e variabili a cui hanno accesso.

Invece di copiare e incollare il template originale per creare il proprio, si può anche
estenderlo e sovrascrivere solo le parti rilevanti:

.. code-block: jinja

  {# in app/Resources/SensioGeneratorBundle/skeleton/crud/actions/create.php.twig #}

  {# si noti il prefisso "skeleton", sarà approfondito successivamente #}
  {% extends "skeleton/crud/actions/create.php.twig" %}

  {% block phpdoc_header %}
       {{ parent() }}
       *
       * Questa parte sarà inserita dopo il titolo phpdoc
       * ma prima delle annotazioni.
  {% endblock phpdoc_header %}

I template più complessi nello scheletro predefinito sono separati in blocchi Twig, per consentire
facilmente l'ereditarietà e per evitare di copiare/incollare larghe parti di codice.

In alcuni casi, i template nello scheletro ne includono altri, come
per esempio nel template ``crud/views/edit.html.twig.twig``:

.. code-block: jinja

  {% include 'crud/views/others/record_actions.html.twig.twig' %}

Se è stato definito un template personalizzato per questo template, sarà
usato al posto di quello predefinito. Ma si può includere esplicitametne il template
scheletro originale, aggiungendo il prefisso ``skeleton/`` al suo percorso, come visto in precedenza:

.. code-block: jinja

  {% include 'skeleton/crud/views/others/record_actions.html.twig.twig' %}

Si può saperne di più su questo truccho nella `documentazione ufficiale di Twig
<http://twig.sensiolabs.org/doc/recipes.html#overriding-a-template-that-also-extends-itself>`_.
