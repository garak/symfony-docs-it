L'architettura
==============

Sei il mio eroe! Chi avrebbe pensato che tu fossi ancora qui dopo le prime
tre parti? I tuoi sforzi saranno presto ricompensati. Le prime tre parti
non danno uno sguardo approfondito all'architettura del framework. Poiché
essa rende unico Symfony2 nel panorama dei framework, vediamo in cosa consiste.

.. index::
   single: Struttura delle cartelle

La struttura delle cartelle
---------------------------

La struttura delle cartelle di una :term:`application` di Symfony2 è
piuttosto flessibile, ma la struttura delle cartelle della sandbox riflette
la struttura tipica e raccomandata per un'applicazione di Symfony2:

* ``app/``: Questa cartella contiene la configurazione dell'applicazione;

* ``src/``: In questa cartella va tutto il codice PHP;

* ``web/``: Questa dovrebbe essere la cartella radice del web.

La cartella web
~~~~~~~~~~~~~~~

La cartella radice del web è la casa di tutti i file pubblici e statici,
come immagini, fogli di stile, file Javascript. È anche il posto in cui
stanno i front controller:

.. code-block:: html+php

    <!-- web/app.php -->
    <?php

    require_once __DIR__.'/../app/AppKernel.php';

    $kernel = new AppKernel('prod', false);
    $kernel->handle()->send();

Come ogni front controller, ``app.php`` usa una classe Kernel, ``AppKernel``,
per inizializzare l'applicazione.

.. index::
   single: Kernel

La cartella dell'applicazione
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

La classe ``AppKernel`` è il punto di ingresso principale della configurazione
dell'applicazione e quindi è memorizzata nella cartella ``app/`.

Questa classe deve implementare quattro metodi:

* ``registerRootDir()``: Restituisce la cartella radice della configurazione;

* ``registerBundles()``: Restituisce un array di tutti i bundle necessari per
  eseguire l'applicazione (si noti il riferimento a
  ``Application\HelloBundle\HelloBundle``);

* ``registerBundleDirs()``: Restituisce un array che associa i namespace e le
  rispettive cartelle;

* ``registerContainerConfiguration()``: Restituisce l'oggetto principale di
  configurazione (approfondito più avanti);

Guardando l'implementazione predefinita di questi metodi, si può avere una
comprensione migliore della flessibilità del framework.

Per far funzionare tutto insieme, il kernel richiede un file dalla cartella
``src/``::

    // app/AppKernel.php
    require_once __DIR__.'/../src/autoload.php';

La cartella dei sorgenti
~~~~~~~~~~~~~~~~~~~~~~~~

Il file ``src/autoload.php`` è responsabile dell'auto-caricamento di tutti i
file posti nella cartella ``src/``::

    // src/autoload.php
    $vendorDir = __DIR__.'/vendor';

    require_once $vendorDir.'/symfony/src/Symfony/Component/HttpFoundation/UniversalClassLoader.php';

    use Symfony\Component\HttpFoundation\UniversalClassLoader;

    $loader = new UniversalClassLoader();
    $loader->registerNamespaces(array(
        'Symfony'                    => $vendorDir.'/symfony/src',
        'Application'                => __DIR__,
        'Bundle'                     => __DIR__,
        'Doctrine\\Common'           => $vendorDir.'/doctrine-common/lib',
        'Doctrine\\DBAL\\Migrations' => $vendorDir.'/doctrine-migrations/lib',
        'Doctrine\\ODM\\MongoDB'     => $vendorDir.'/doctrine-mongodb/lib',
        'Doctrine\\DBAL'             => $vendorDir.'/doctrine-dbal/lib',
        'Doctrine'                   => $vendorDir.'/doctrine/lib',
        'Zend'                       => $vendorDir.'/zend/library',
    ));
    $loader->registerPrefixes(array(
        'Swift_' => $vendorDir.'/swiftmailer/lib/classes',
        'Twig_'  => $vendorDir.'/twig/lib',
    ));
    $loader->register();

La classe ``UniversalClassLoader`` di Symfony2 è usata per auto-caricare
i file che rispettano gli `standard`_  di interoperabilità  per i namespace di
PHP 5.3 oppure la `convenzione`_ dei nomi di PEAR per le classi. Come si
può vedere, tutte le dipendenze sono sotto la cartella ``vendor/``, ma
questa è solo una convenzione. Si possono inserire in qualsiasi posto,
globalmente sul proprio server o localmente nei propri progetti.

.. index::
   single: Bundle

Il sistema dei bundle
---------------------

Questa sezione è solo una piccola introduzione a una delle più grandi e
potenti caratteristiche di Symfony2, il sistema dei :term:`bundle`.

Un bundle è molto simile a un plugin in un altro software. Ma perché
allora si chiama "bundle" e non "plugin"? Perché ogni cosa è un bundle
in Symfony2, dalle caratteristiche del nucleo del framework al codice
scritto per la propria applicazione.
I bundle sono cittadini di prima classe in Symfony2. Essi forniscono la
flessibilità di usare delle caratteristiche pre-costruite impacchettate
in bundle di terze parti o di distribuire i propri bundle. Questo rende
molto facile scegliere quali caratteristiche abilitare nella propria
applicazione e ottimizzarle nel modo preferito.

Un'applicazione è composta di bundle, come definito nel metodo ``registerBundles()``
della classe ``AppKernel``::

    // app/AppKernel.php
    public function registerBundles()
    {
        $bundles = array(
            new Symfony\Bundle\FrameworkBundle\FrameworkBundle(),

            // abilita i bundle di terze parti
            new Symfony\Bundle\ZendBundle\ZendBundle(),
            new Symfony\Bundle\SwiftmailerBundle\SwiftmailerBundle(),
            new Symfony\Bundle\DoctrineBundle\DoctrineBundle(),
            //new Symfony\Bundle\DoctrineMigrationsBundle\DoctrineMigrationsBundle(),
            //new Symfony\Bundle\DoctrineMongoDBBundle\DoctrineMongoDBBundle(),
            //new Symfony\Bundle\TwigBundle\TwigBundle(),

            // registra i bundle personali
            new Application\AppBundle\AppBundle(),
        );

        if ($this->isDebug()) {
            $bundles[] = new Symfony\Bundle\WebProfilerBundle\WebProfilerBundle();
        }

        return $bundles;
    }

Oltra a ``HelloBundle``, di cui abbiamo già parlato, si noti che il kernel
abilita anche ``FrameworkBundle``, ``DoctrineBundle``,
``SwiftmailerBundle`` e ``ZendBundle``. Fanno tutti parte del nucleo del
framework.

Ogni bundle può essere personalizzato tramite file di configurazione scritti inYAML, XML
o PHP. Si consideri la configurazione predefinita:

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        app.config:
            charset:       UTF-8
            error_handler: null
            csrf_secret:   xxxxxxxxxx
            router:        { resource: "%kernel.root_dir%/config/routing.yml" }
            validation:    { enabled: true, annotations: true }
            templating:
                escaping:       htmlspecialchars
                #assets_version: SomeVersionScheme
            #user:
            #    default_locale: it
            #    session:
            #        name:     SYMFONY
            #        type:     Native
            #        lifetime: 3600

        ## Twig Configuration
        #twig.config:
        #    auto_reload: true

        ## Doctrine Configuration
        #doctrine.dbal:
        #    dbname:   xxxxxxxx
        #    user:     xxxxxxxx
        #    password: ~
        #doctrine.orm: ~

        ## Swiftmailer Configuration
        #swiftmailer.config:
        #    transport:  smtp
        #    encryption: ssl
        #    auth_mode:  login
        #    host:       smtp.gmail.com
        #    username:   xxxxxxxx
        #    password:   xxxxxxxx

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <app:config csrf-secret="xxxxxxxxxx" charset="UTF-8" error-handler="null">
            <app:router resource="%kernel.root_dir%/config/routing.xml" />
            <app:validation enabled="true" annotations="true" />
            <app:templating escaping="htmlspecialchars" />
            <!--
            <app:user default-locale="it">
                <app:session name="SYMFONY" type="Native" lifetime="3600" />
            </app:user>
            //-->
        </app:config>

        <!-- Twig Configuration -->
        <!--
        <twig:config auto_reload="true" />
        -->

        <!-- Doctrine Configuration -->
        <!--
        <doctrine:dbal dbname="xxxxxxxx" user="xxxxxxxx" password="" />
        <doctrine:orm />
        -->

        <!-- Swiftmailer Configuration -->
        <!--
        <swiftmailer:config
            transport="smtp"
            encryption="ssl"
            auth_mode="login"
            host="smtp.gmail.com"
            username="xxxxxxxx"
            password="xxxxxxxx" />
        -->

    .. code-block:: php

        // app/config/config.php
        $container->loadFromExtension('app', 'config', array(
            'charset'       => 'UTF-8',
            'error_handler' => null,
            'csrf-secret'   => 'xxxxxxxxxx',
            'router'        => array('resource' => '%kernel.root_dir%/config/routing.php'),
            'validation'    => array('enabled' => true, 'annotations' => true),
            'templating'    => array(
                'escaping'        => 'htmlspecialchars'
                #'assets_version' => "SomeVersionScheme",
            ),
            #'user' => array(
            #    'default_locale' => "it",
            #    'session' => array(
            #        'name' => "SYMFONY",
            #        'type' => "Native",
            #        'lifetime' => "3600",
            #    )
            #),
        ));

        // Twig Configuration
        /*
        $container->loadFromExtension('twig', 'config', array('auto_reload' => true));
        */

        // Doctrine Configuration
        /*
        $container->loadFromExtension('doctrine', 'dbal', array(
            'dbname'   => 'xxxxxxxx',
            'user'     => 'xxxxxxxx',
            'password' => '',
        ));
        $container->loadFromExtension('doctrine', 'orm');
        */

        // Swiftmailer Configuration
        /*
        $container->loadFromExtension('swiftmailer', 'config', array(
            'transport'  => "smtp",
            'encryption' => "ssl",
            'auth_mode'  => "login",
            'host'       => "smtp.gmail.com",
            'username'   => "xxxxxxxx",
            'password'   => "xxxxxxxx",
        ));
        */

Ogni voce com ``app.config`` definisce la configurazione per un bundle.

Ogni :term:`environment` può ridefinire la configurazione predefinita, fornendo
uno specifico file di configurazione:

.. configuration-block::

    .. code-block:: yaml

        # app/config/config_dev.yml
        imports:
            - { resource: config.yml }

        app.config:
            router:   { resource: "%kernel.root_dir%/config/routing_dev.yml" }
            profiler: { only_exceptions: false }

        webprofiler.config:
            toolbar: true
            intercept_redirects: true

        zend.config:
            logger:
                priority: debug
                path:     %kernel.root_dir%/logs/%kernel.environment%.log

    .. code-block:: xml

        <!-- app/config/config_dev.xml -->
        <imports>
            <import resource="config.xml" />
        </imports>

        <app:config>
            <app:router resource="%kernel.root_dir%/config/routing_dev.xml" />
            <app:profiler only-exceptions="false" />
        </app:config>

        <webprofiler:config
            toolbar="true"
            intercept-redirects="true"
        />

        <zend:config>
            <zend:logger priority="info" path="%kernel.logs_dir%/%kernel.environment%.log" />
        </zend:config>

    .. code-block:: php

        // app/config/config.php
        $loader->import('config.php');

        $container->loadFromExtension('app', 'config', array(
            'router'   => array('resource' => '%kernel.root_dir%/config/routing_dev.php'),
            'profiler' => array('only-exceptions' => false),
        ));

        $container->loadFromExtension('webprofiler', 'config', array(
            'toolbar' => true,
            'intercept-redirects' => true,
        ));

        $container->loadFromExtension('zend', 'config', array(
            'logger' => array(
                'priority' => 'info',
                'path'     => '%kernel.logs_dir%/%kernel.environment%.log',
            ),
        ));

Come visto nella parte precedente, un'applicazione è composta di bundle, come
definito nel metodo ``registerBundles()``. Ma come fa Symfony2 a sapere dove
cercare i bundle? Symfony2 è molto flessibile a riguardo. Il metodo
``registerBundleDirs()`` deve restituire un array associativo, che mappi i
namespace a una qualsiasi cartella valida (locale o globale)::

    public function registerBundleDirs()
    {
        return array(
            'Application'     => __DIR__.'/../src/Application',
            'Bundle'          => __DIR__.'/../src/Bundle',
            'Symfony\\Bundle' => __DIR__.'/../src/vendor/symfony/src/Symfony/Bundle',
        );
    }

Quindi, quando si fa riferimonto a ``HelloBundle`` nel nome di un controllore o nel
nome di un template, Symfony2 lo cercherà sotto le cartelle date.

Ora dovrebbe essere chiaro il motivo della flessibilità di Symfony2: condividere
i propri bundle tra le applicazioni, memorizzarli localmente o globalmente, a
propria scelta.

.. index::
   single: Vendor

Usare i vendor
--------------

Probabilmente la propria applicazione dipenderà da librerie di terze parti.
Queste ultime dovrebbero essere memorizzate nella cartella ``src/vendor/``.
Tale cartella contiene già le librerie di Symfony2, SwiftMailer, l'ORM Doctrine,
l'ORM Propel, il sistema di template Twig e una selezione di classi di
Zend Framework.

.. index::
   single: Cache di configurazione
   single: Log

Cache e log
-----------

Symfony2 è forse uno dei framework completi più veloci in circolazione.
Ma come può essere così veloce, se analizza e interpreta decine di file
YAML e XML a ogni richiesta? In parte, per il suo sistema di cache. La
configurazione dell'applicazione è analizzata solo per la prima richiesta
e poi compilata in semplice file PHP, memorizzato nella cartella ``cache/``
dell'applicazione. Nell'ambiente di sviluppo, Symfony2 è abbastanza
intelligente da pulire la cache quando cambiano dei file. In produzione,
invece, occorre pulire la cache manualmente quando si aggiorna il codice
o si modifica la configurazione.

Sviluppando un'applicazione web, le cose possono andar male in diversi modi.
I file di log nella cartella ``logs/`` dell'applicazione dicono tutto a
proposito delle richieste e aiutano a risolvere il problema in breve tempo.

.. index::
   single: CLI
   single: Linea di comando

L'interfaccia a linea di comando
--------------------------------

Ogni applicazione ha uno strumento di interfaccia a linea di comando, detta
anche ``console``, che aiuta nella manutenzione dell'applicazione stessa.
La console fornisce dei comandi che incrementano la produttività, automatizzando
dei compiti noiosi e ripetitivi.

Richiamandola senza parametri, si può sapere di più sulle sue capacità:

.. code-block:: bash

    $ php app/console

L'opzione ``--help`` aiuta a scoprire l'utilizzo di un comando:

.. code-block:: bash

    $ php app/console router:debug --help

Considerazioni finali
---------------------

Dopo aver letto questa parte, si dovrebbe essere in grado di muoversi facilmente
dentro Symfony2 e farlo funzionare. Ogni cosa in Symfony2 è fatta per
rispondere alle varie esigenze. Quindi, si possono rinominare e spostare le
varie cartelle, finché non si raggiunge il risultato voluto.

E questo è tutto per il giro veloce. Dai test all'invio di email, occorre ancora
imparare diverse cose per padroneggiare Symfony2. Pronti per approfondire questi
temi? Senza indugi, basta andare nella pagine delle `guide`_ ufficiali e
scegliere un argomento a piacere.

.. _standard:    http://groups.google.com/group/php-standards/web/psr-0-final-proposal
.. _convenzione: http://pear.php.net/
.. _guide:       http://www.symfony-reloaded.org/learn
