L'architettura
==============

Sei il mio eroe! Chi avrebbe pensato che tu fossi ancora qui dopo le prime
tre parti? I tuoi sforzi saranno presto ricompensati. Le prime tre parti
non danno uno sguardo approfondito all'architettura del framework. Poiché
essa rende unico Symfony2 nel panorama dei framework, vediamo in cosa consiste.


Capire la struttura delle cartelle
----------------------------------

La struttura delle cartelle di un':term:`applicazione` Symfony2 è alquanto flessibile,
ma la struttura delle cartelle della distribuzione *Standard Edition* riflette
la struttura tipica e raccomandata di un'applicazione Symfony2:

* ``app/``:    La configurazione dell'applicazione;
* ``src/``:    Il codice PHP del progetto;
* ``vendor/``: Le dipendenze di terze parti;
* ``web/``:    La cartella radice del web.

La cartella ``web/``
~~~~~~~~~~~~~~~~~~~~

La cartella radice del web è la casa di tutti i file pubblici e statici,
come immagini, fogli di stile, file JavaScript. È anche il posto in cui
stanno i :term:`front controller`::

    // web/app.php
    require_once __DIR__.'/../app/bootstrap.php.cache';
    require_once __DIR__.'/../app/AppKernel.php';

    use Symfony\Component\HttpFoundation\Request;

    $kernel = new AppKernel('prod', false);
    $kernel->loadClassCache();
    $kernel->handle(Request::createFromGlobals())->send();

Il kernel inizialmente richiede il file ``bootstrap.php.cache``, che lancia
l'applicazione e registra l'autoloader (vedi sotto).

Come ogni front controller, ``app.php`` usa una classe Kernel, ``AppKernel``,
per inizializzare l'applicazione.

.. _the-app-dir:

La cartella ``app/``
~~~~~~~~~~~~~~~~~~~~

La classe ``AppKernel`` è il punto di ingresso principale della configurazione
dell'applicazione e quindi è memorizzata nella cartella ``app/`.

Questa classe deve implementare due metodi:

* ``registerBundles()`` deve restituire un array di tutti i bundle necessari per
  eseguire l'applicazione;

* ``registerContainerConfiguration()``: carica la configurazione dell'applicazione
(approfondito più avanti);

L'autoloading di PHP può essere configurato tramite ``app/autoload.php``::

    // app/autoload.php
    use Symfony\Component\ClassLoader\UniversalClassLoader;

    $loader = new UniversalClassLoader();
    $loader->registerNamespaces(array(
        'Symfony'          => array(__DIR__.'/../vendor/symfony/src', __DIR__.'/../vendor/bundles'),
        'Sensio'           => __DIR__.'/../vendor/bundles',
        'JMS'              => __DIR__.'/../vendor/bundles',
        'Doctrine\\Common' => __DIR__.'/../vendor/doctrine-common/lib',
        'Doctrine\\DBAL'   => __DIR__.'/../vendor/doctrine-dbal/lib',
        'Doctrine'         => __DIR__.'/../vendor/doctrine/lib',
        'Monolog'          => __DIR__.'/../vendor/monolog/src',
        'Assetic'          => __DIR__.'/../vendor/assetic/src',
        'Metadata'         => __DIR__.'/../vendor/metadata/src',
    ));
    $loader->registerPrefixes(array(
        'Twig_Extensions_' => __DIR__.'/../vendor/twig-extensions/lib',
        'Twig_'            => __DIR__.'/../vendor/twig/lib',
    ));

    // ...

    $loader->registerNamespaceFallbacks(array(
        __DIR__.'/../src',
    ));
    $loader->register();

La classe `Symfony\\Component\\ClassLoader\\UniversalClassLoader` di Symfony2 è usata per auto-caricare i file che rispettano gli `standard`_  di interoperabilità  per i namespace di
PHP 5.3 oppure la `convenzione`_ dei nomi di PEAR per le classi. Come si può vedere,
tutte le dipendenze sono sotto la cartella ``vendor/``, ma questa è solo una convenzione. Si possono inserire in qualsiasi posto, globalmente sul proprio server o localmente nei propri progetti.




.. note::

    Se si vuole approfondire l'argomento flessibilità dell'autoloader di Symfony2,
    si può leggere la ricetta ":doc:`/cookbook/tools/autoloader`".


Capire il sistema dei bundle
----------------------------

Questa sezione è un'introduzione a una delle più grandi e
potenti caratteristiche di Symfony2, il sistema dei :term:`bundle`.

Un bundle è molto simile a un plugin in un altro software. Ma perché
allora si chiama *bundle e non *plugin*? Perché *ogni cosa* è un bundle
in Symfony2, dalle caratteristiche del nucleo del framework al codice
scritto per la propria applicazione. I bundle sono cittadini di prima classe in Symfony2.
Essi forniscono la flessibilità di usare delle caratteristiche pre-costruite impacchettate
in bundle di terze parti o di distribuire i propri bundle. Questo rende
molto facile scegliere quali caratteristiche abilitare nella propria
applicazione e ottimizzarle nel modo preferito. A fine giornata, il codice
della propria applicazione è *importante* quanto il nucleo stesso del framework.

Registrare un bundle
~~~~~~~~~~~~~~~~~~~~

Un'applicazione è composta di bundle, come definito nel metodo ``registerBundles()``
della classe ``AppKernel`` . Ogni bundle è una cartella che contiene una singola classe
``Bundle`` che la descrive::

    // app/AppKernel.php
    public function registerBundles()
    {
        $bundles = array(
            new Symfony\Bundle\FrameworkBundle\FrameworkBundle(),
            new Symfony\Bundle\SecurityBundle\SecurityBundle(),
            new Symfony\Bundle\TwigBundle\TwigBundle(),
            new Symfony\Bundle\MonologBundle\MonologBundle(),
            new Symfony\Bundle\SwiftmailerBundle\SwiftmailerBundle(),
            new Symfony\Bundle\DoctrineBundle\DoctrineBundle(),
            new Symfony\Bundle\AsseticBundle\AsseticBundle(),
            new Sensio\Bundle\FrameworkExtraBundle\SensioFrameworkExtraBundle(),
            new JMS\SecurityExtraBundle\JMSSecurityExtraBundle(),
        );

        if (in_array($this->getEnvironment(), array('dev', 'test'))) {
            $bundles[] = new Acme\DemoBundle\AcmeDemoBundle();
            $bundles[] = new Symfony\Bundle\WebProfilerBundle\WebProfilerBundle();
            $bundles[] = new Sensio\Bundle\DistributionBundle\SensioDistributionBundle();
            $bundles[] = new Sensio\Bundle\GeneratorBundle\SensioGeneratorBundle();
        }

        return $bundles;
    }

Oltre a ``AcmeDemoBundle``, di cui abbiamo già parlato, si noti che il kernel
abilita anche ``FrameworkBundle``, ``DoctrineBundle``,
``SwiftmailerBundle`` e ``AsseticBundle``. Fanno tutti parte del nucleo del
framework.

Configurare un bundle
~~~~~~~~~~~~~~~~~~~~~

Ogni bundle può essere personalizzato tramite file di configurazione scritti in YAML,
XML o PHP. Si veda la configurazione predefinita::

    .. code-block:: yaml

    # app/config/config.yml
    imports:
        - { resource: parameters.ini }
        - { resource: security.yml }

    framework:
        secret:          %secret%
            charset:       UTF-8
            router:        { resource: "%kernel.root_dir%/config/routing.yml" }
        form:            true
        csrf_protection: true
        validation:      { enable_annotations: true }
        templating:      { engines: ['twig'] } #assets_version: SomeVersionScheme
        session:
            default_locale: %locale%
            auto_start:     true

    # Twig Configuration
    twig:
        debug:            %kernel.debug%
        strict_variables: %kernel.debug%

    # Assetic Configuration
    assetic:
        debug:          %kernel.debug%
        use_controller: false
        filters:
            cssrewrite: ~
            # closure:
            #     jar: %kernel.root_dir%/java/compiler.jar
            # yui_css:
            #     jar: %kernel.root_dir%/java/yuicompressor-2.4.2.jar

    # Doctrine Configuration
    doctrine:
        dbal:
            driver:   %database_driver%
            host:     %database_host%
            dbname:   %database_name%
            user:     %database_user%
            password: %database_password%
            charset:  UTF8

        orm:
            auto_generate_proxy_classes: %kernel.debug%
            auto_mapping: true

    # Swiftmailer Configuration
    swiftmailer:
        transport: %mailer_transport%
        host:      %mailer_host%
        username:  %mailer_user%
        password:  %mailer_password%

    jms_security_extra:
        secure_controllers:  true
        secure_all_services: false

Ogni voce come ``framework`` definisce la configurazione per uno specifico bundle.
Per esempio, ``framework`` configura ``FrameworkBundle``, mentre ``swiftmailer``
configura ``SwiftmailerBundle``.

Ogni :term:`ambiente` può sovrascrivere la configurazione predefinita, fornendo un file
di configurazione specifico. Per esempio, l'ambiente ``dev`` carica il file ``config_dev.yml``,
che carica la configurazione principale (cioè ``config.yml``) e quindi la modifica per
aggiungere alcuni strumenti di debug::

    .. code-block:: yaml

    # app/config/config_dev.yml
    imports:
        - { resource: config.yml }

    framework:
        router:   { resource: "%kernel.root_dir%/config/routing_dev.yml" }
        profiler: { only_exceptions: false }

    web_profiler:
        toolbar: true
        intercept_redirects: false

    monolog:
        handlers:
            main:
                type:  stream
                path:  %kernel.logs_dir%/%kernel.environment%.log
                level: debug
            firephp:
                type:  firephp
                level: info

    assetic:
        use_controller: true

Estendere un bundle
~~~~~~~~~~~~~~~~~~~

Oltre ad essere un modo carino per organizzare e configurare il proprio codice,
un bundle può estendere un altro bundle. L'ereditarietà dei bundle consente di
sovrascrivere un bundle esistente, per poter personalizzare i suoi controllori, i
template o qualsiasi altro suo file. Qui sono d'aiuto i nomi logici (come
``@AcmeDemoBundle/Controller/SecuredController.php``), che astraggono i posti in cui le risorse sono effettivamente memorizzate.

Nomi logici di file
...................

Quando si vuole fare riferimento a un file da un bundle, usare questa notazione:
``@NOME_BUNDLE/percorso/del/file``; Symfony2 risolverà ``@NOME_BUNDLE`` nel percorso
reale del bundle.
Per esempio, il percorso logico ``@AcmeDemoBundle/Controller/DemoController.php``
verrebbe converito in ``src/Acme/DemoBundle/Controller/DemoController.php``,
perché Symfony conosce la locazione di ``AcmeDemoBundle``.

Nomici logici di controllori
............................

Per i controllori, occorre fare riferimento ai nomi dei metodi usando il formato
``NOME_BUNDLE:NOME_CONTROLLORE:NOME_AZIONE``. Per esempio,
``AcmeDemoBundle:Welcome:index`` mappa il metodo ``indexAction`` della classe
``Acme\DemoBundle\Controller\WelcomeController``.

Nomi logici di template
.......................

Per i template, il nome logico ``AcmeDemoBundle:Welcome:index.html.twig`` è
convertito al percorso del file ``src/Acme/DemoBundle/Resources/views/Welcome/index.html.twig``.
I template diventano ancora più interessanti quando si realizza che i file non
hanno bisogno di essere memorizzati su filesystem. Si possono facilmente
memorizzare, per esempio, in una tabella di database.

Estendere i bundle
..................

Se si seguono queste convenzioni, si può usare l':doc:`ereditarietà dei bundle</cookbook/bundles/inheritance>` per "sovrascrivere" file, controllori o template.
Per esempio, se un nuovo bundle chiamato ``AcmeNewBundle`` estende
``AcmeDemoBundle``, Symfony proverà a caricare prima il controllore
``AcmeDemoBundle:Welcome:index`` da ``AcmeNewBundle`` e poi cercherà il
secondo ``AcmeDemoBundle``.

Capite ora perché Symfony2 è così flessibile? Condividere i propri bundle tra le
applicazioni, memorizzarli localmente o globalmente, a propria scelta.

.. _using-vendors:

Usare i venditori
-----------------

Probabilmente la propria applicazione dipenderà da librerie di terze parti.
Queste ultime dovrebbero essere memorizzate nella cartella ``vendor/``.
Tale cartella contiene già le librerie di Symfony2, SwiftMailer, l'ORM Doctrine,
il sistema di template Twig e alcune altre librerie e bundle di terze parti.

Capire la cache e i log
-----------------------

Symfony2 è forse uno dei framework completi più veloci in circolazione.
Ma come può essere così veloce, se analizza e interpreta decine di file
YAML e XML a ogni richiesta? In parte, per il suo sistema di cache. La
configurazione dell'applicazione è analizzata solo per la prima richiesta
e poi compilata in semplice file PHP, memorizzato nella cartella ``app/cache/``
dell'applicazione. Nell'ambiente di sviluppo, Symfony2 è abbastanza
intelligente da pulire la cache quando cambiano dei file. In produzione,
invece, occorre pulire la cache manualmente quando si aggiorna il codice
o si modifica la configurazione.

Sviluppando un'applicazione web, le cose possono andar male in diversi modi.
I file di log nella cartella ``app/logs/`` dicono tutto a proposito delle richieste e aiutano a risolvere il problema in breve tempo.

Usare l'interfaccia a linea di comando
--------------------------------------

Ogni applicazione ha uno strumento di interfaccia a linea di comando (``app/console``),
che aiuta nella manutenzione dell'applicazione. La console fornisce dei comandi che incrementano la produttività, automatizzando
dei compiti noiosi e ripetitivi.

Richiamandola senza parametri, si può sapere di più sulle sue capacità:

.. code-block:: bash

    php app/console

L'opzione ``--help`` aiuta a scoprire l'utilizzo di un comando:

.. code-block:: bash

    php app/console router:debug --help

Considerazioni finali
---------------------

Dopo aver letto questa parte, si dovrebbe essere in grado di muoversi facilmente
dentro Symfony2 e farlo funzionare. Ogni cosa in Symfony2 è fatta per
rispondere alle varie esigenze. Quindi, si possono rinominare e spostare le
varie cartelle, finché non si raggiunge il risultato voluto.

E questo è tutto per il giro veloce. Dai test all'invio di email, occorre ancora
imparare diverse cose per padroneggiare Symfony2. Pronti per approfondire questi
temi? Senza indugi, basta andare nella pagine del `libro<:doc:`/book/index`` e
scegliere un argomento a piacere.

.. _standard:    http://groups.google.com/group/php-standards/web/psr-0-final-proposal
.. _convenzione: http://pear.php.net/
