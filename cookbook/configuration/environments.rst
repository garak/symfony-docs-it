.. index::
   single: Ambienti;

Come padroneggiare e creare nuovi ambienti
==========================================

Ogni applicazione è la combinazione di codice e di un insieme di configurazioni
che determinano come il codice dovrà lavorare. La configurazione può definire
il database da utilizzare, cosa dovrà essere messo in cache e cosa non, o quanto
esaustivi dovranno essere i log. In Symfony2, l'idea di ambiente è quella di
eseguire il codice, utilizzando differenti configurazioni. Per esempio,
l'ambiente ``dev`` dovrebbe usare una configurazione che renda lo sviluppo
semplice e ricco di informazioni, mentre l'ambiente ``prod`` dovrebbe usare un
insieme di configurazioni che ottimizzino la velocità.

.. index::
   single: Ambienti; File di configurzione

Ambienti differenti, differenti file di configurazione
------------------------------------------------------

Una tipica applicazione Symfony2 inizia con tre ambienti: ``dev``, ``prod``
e ``test``. Come si è già detto, ogni "ambiente " rappresenta un modo in cui
eseguire l'intero codice con differenti configurazioni. Non dovrebbe destare
sorpresa il fatto che ogni ambiente carichi i suoi propri file di configurazione.
Se si utilizza il formato di configurazione YAML, verranno utilizzati
i seguenti file:

 * per l'ambiente ``dev``: ``app/config/config_dev.yml``
 * per l'ambiente ``prod``: ``app/config/config_prod.yml``
 * per l'ambiente ``test``: ``app/config/config_test.yml``

Il funzionamento si basa su di un semplice comportamento predefinito all'interno
della classe ``AppKernel``:

.. code-block:: php

    // app/AppKernel.php

    // ...
    
    class AppKernel extends Kernel
    {
        // ...

        public function registerContainerConfiguration(LoaderInterface $loader)
        {
            $loader->load(__DIR__.'/config/config_'.$this->getEnvironment().'.yml');
        }
    }

Come si può vedere, quando Symfony2 viene caricato, utilizza l'ambiente
per determinare quale file di configurazione caricare. Questo permette 
di avere ambienti differenti in modo elegante, efficace e trasparente.

Ovviamente, in realtà, ogni ambiente differisce solo per alcuni aspetti dagli altri.
Generalmente, gli ambienti condividono gran parte della loro configurazione.
Aprendo il file di configurazione di ``dev``, si può vedere come questo venga 
ottenuto facilmente e in modo trasparente:

.. configuration-block::

    .. code-block:: yaml

        imports:
            - { resource: config.yml }
        # ...

    .. code-block:: xml

        <imports>
            <import resource="config.xml" />
        </imports>
        <!-- ... -->

    .. code-block:: php

        $loader->import('config.php');
        // ...

Per condividere una configurazione comune, i file di configurazione di ogni ambiente
importano, per iniziare, un file di configurazione comune (``config.yml``).
Il resto del file potrà deviare dalla configurazione predefinita, sovrascrivendo
i singoli parametri. Ad esempio, nell'ambiente ``dev``, la barra delle applicazioni
viene attivata modificando, nel file di configurazione di ``dev``, il relativo 
parametro predefinito:

.. configuration-block::

    .. code-block:: yaml

        # app/config/config_dev.yml
        imports:
            - { resource: config.yml }

        web_profiler:
            toolbar: true
            # ...

    .. code-block:: xml

        <!-- app/config/config_dev.xml -->
        <imports>
            <import resource="config.xml" />
        </imports>

        <webprofiler:config
            toolbar="true"
            ... />

    .. code-block:: php

        // app/config/config_dev.php
        $loader->import('config.php');

        $container->loadFromExtension('web_profiler', array(
            'toolbar' => true,
            ...,
        ));

.. index::
   single: Ambienti; Eseguire ambienti diversi

Eseguire un'applicazione in ambienti differenti
-----------------------------------------------

Per eseguire l'applicazione in ogni ambiente, sarà necessario caricarla 
utilizzando il front controller ``app.php`` (per l'ambiente ``prod``) o 
utilizzando il front controller ``app_dev.php`` (per l'ambiente ``dev``):

.. code-block:: text

    http://localhost/app.php      -> ambiente *prod*
    http://localhost/app_dev.php  -> ambiente *dev*

.. note::

   Le precedenti URL presuppongono che il server web sia configurato in modo da
   usare la cartella ``web/`` dell'applicazione, come radice. Per approfondire, si legga
   :doc:`Installare Symfony2</book/installation>`.

Guardando il contenuto di questi file, si vede come l'ambiente utilizzato da entrambi,
sia definito in modo esplicito:

.. code-block:: php
   :linenos:

    <?php

    require_once __DIR__.'/../app/bootstrap_cache.php';
    require_once __DIR__.'/../app/AppCache.php';

    use Symfony\Component\HttpFoundation\Request;

    $kernel = new AppCache(new AppKernel('prod', false));
    $kernel->handle(Request::createFromGlobals())->send();

Si può vedere come la chiave ``prod`` specifica che l'ambiente di esecuzione
sarà l'ambiente ``prod``. Un'applicazione Symfony2 può essere esguita in qualsiasi
ambiente utilizzando lo stesso codice, cambiando la sola stringa relativa all'ambiente.

.. note::

   L'ambiente ``test`` è utilizzato quando si scrivono i test funzionali e
   non è perciò accessibile direttamente dal browser tramite un front controller. 
   In altre parole, diversamente dagli altri ambienti, non c'è alcun file,
   per il front controller, del tipo ``app_test.php``.

.. index::
   single: Configurazione; Modalità debug 

.. sidebar:: Modalità *debug*

    Importante, ma non collegato all'argomento *ambienti*, è il valore ``false``
    in riga 8 del precedente front controller. Questo valore specifica se
    l'applicazione dovrà essere eseguità in "modalità debug" o meno. Indipendentemente
    dall'ambiente, un'applicazione Symfony2 può essere eseguita con la modalità
    debug configurata a ``true`` o a ``false. Questo modifica diversi aspetti dell'applicazione, come
    il fatto che gli errori vengano mostrati o se la cache debba essere ricreata dinamicamente
    a ogni richiesta. Sebbene non sia obbligatorio, la modalità debug è sempre
    configurata a ``true`` negli ambienti ``dev`` e ``test`` e a ``false`` per 
    l'ambiente ``prod``.

    Internamente il valore della modalità debug diventa il parametro ``kernel.debug``
    utilizzato all'interno del  :doc:`contenitore di servizi </book/service_container>`.
    Dando uno sguardo al file di configurazione dell'applicazione, si vede come
    il parametro venga utilizzato, ad esempio, per avviare o interrompere il logging
    quando si utilizza il DBAL di Doctrine:

    .. configuration-block::

        .. code-block:: yaml

            doctrine:
               dbal:
                   logging:  "%kernel.debug%"
                   # ...

        .. code-block:: xml

            <doctrine:dbal logging="%kernel.debug%" ... />

        .. code-block:: php

            $container->loadFromExtension('doctrine', array(
                'dbal' => array(
                    'logging'  => '%kernel.debug%',
                    ...,
                ),
                ...
            ));

.. index::
   single: Ambienti; Creare un nuovo ambiente

Creare un nuovo ambiente
------------------------

Un'applicazione Symfony2 viene generata con tre ambienti preconfigurati per
gestire la maggior parte dei casi. Ovviamente, visto che un ambiente non è nient'altro
che una stringa  che corrisponde ad un insieme di configurazioni, creare un nuovo
ambiente è abbastanza semplice.

Supponiamo, per esempio, di voler misurare le prestazioni dell'applicazione
prima del suo invio in produzione. Un modo è quello di usare una configurazione
simile a quella del rilascio ma che utilizzasse il ``web_profiler`` di Symfony2.
Queso permetterebbe a Symfony2 di registrare le informazioni dell'applicazione mentre se ne misura le prestazioni.

Il modo migliore per ottenere tutto ciò è tramite un ambiente che si chiami, per esempio,
``benchmark``. Si parte creando un nuovo file di configurazione:

.. configuration-block::

    .. code-block:: yaml

        # app/config/config_benchmark.yml
        imports:
            - { resource: config_prod.yml }

        framework:
            profiler: { only_exceptions: false }

    .. code-block:: xml

        <!-- app/config/config_benchmark.xml -->
        <imports>
            <import resource="config_prod.xml" />
        </imports>

        <framework:config>
            <framework:profiler only-exceptions="false" />
        </framework:config>

    .. code-block:: php

        // app/config/config_benchmark.php     
        $loader->import('config_prod.php')

        $container->loadFromExtension('framework', array(
            'profiler' => array('only-exceptions' => false),
        ));

Con queste poche e semplici modifiche, l'applicazione supporta un nuovo
ambiente chiamato ``benchmark``.

Questa nuova configurazione importa la configurazione dell'ambiente ``prod`` 
e la modifica. Così si garantice che l'ambiente sia identico a quello 
``prod`` eccetto per le modifiche espressamente inserite in configurazione.

Siccome sarà necessario che l'ambiente sia accessibile tramite browser, sarà
necessario creare un apposito front controller. Basterà copiare il file ``web/app.php``
nel file ``web/app_benchmark.php`` e modificare l'ambiente in modo che punti a ``benchmark``:

.. code-block:: php

    <?php

    require_once __DIR__.'/../app/bootstrap.php';
    require_once __DIR__.'/../app/AppKernel.php';

    use Symfony\Component\HttpFoundation\Request;

    $kernel = new AppKernel('benchmark', false);
    $kernel->handle(Request::createFromGlobals())->send();

Il nuovo ambiente sarà accessibile tramite::

    http://localhost/app_benchmark.php

.. note::
   
   Alcuni ambienti, come il ``dev``, non dovrebbero mai essere accessibile su di
   un server pubblico di produzione. Questo perché alcuni ambienti, per facilitarne 
   il debug, potrebbero fornire troppe informazioni relative all'infrastruttura
   sottostante l'applicazione. Per essere sicuri che questi ambienti non siano
   accessibili, il front controller è solitamente protetto dall'accesso da parte di
   indirizzi IP esterni tramite il seguente codice, posto in cima al controllore:
   
    .. code-block:: php

        if (!in_array(@$_SERVER['REMOTE_ADDR'], array('127.0.0.1', '::1'))) {
            die('You are not allowed to access this file. Check '.basename(__FILE__).' for more information.');
        }

.. index::
   single: Ambienti; Cartella cache

Gli ambienti e la cartella della cache
--------------------------------------

Symfony2 sfrutta la cache in diversi modi: la configurazione dell'applicazione,
la configurazione delle rotte, i template di Twig vengono tutti immagazzinati
in oggetti PHP e salvati su file nella cartella della cache.

Normalmente questi file sono conservati principalmente nella cartella ``app/cache``.
Comunque ogni ambiente usa il suo proprio insieme di file della cache:

.. code-block:: text

    app/cache/dev   - cartella per la cache dell'ambiente *dev*
    app/cache/prod  - cartella per la cache dell'ambiente *prod*

Alcune volte, durante il debug, può essere utile poter controllare i file
salvati in cache, per capire come le cose stiano funzionando. In questi casi
bisogna ricordarsi di guardare nella cartella dell'ambiente che si sta utilizzando
(solitamente, in fase di sviluppo e debug, il ``dev``). Sebbene possa variare,
il contenuto della cartella ``app/cache/dev`` includerà i seguenti file:

* ``appDevDebugProjectContainer.php`` - il "contenitore di servizi" salvato in cache
  che rappresenta la configurazione dell'applicazione;

* ``appdevUrlGenerator.php`` - la classe PHP generata a partire dalla configurazione
  delle rotte e usata nella generazione degli URL;

* ``appdevUrlMatcher.php`` - la classe PHP utilizzata per ricercare le rotte: qui
  è possibile vedere le espressioni regolari utilizzate per associare gli URL in ingresso
  con le rotte disponibili;

* ``twig/`` - questa cartella contiene la cache dei template di Twig.


Approfondimenti
---------------

Si legga l'articolo :doc:`/cookbook/configuration/external_parameters`.
