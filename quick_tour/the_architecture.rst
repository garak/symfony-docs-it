L'architettura
==============

Sei un eroe! Chi avrebbe pensato che tu fossi ancora qui dopo le prime
tre parti? I tuoi sforzi saranno presto ricompensati. Le prime tre parti
non danno uno sguardo approfondito all'architettura del framework. Poiché
essa rende unico Symfony nel panorama dei framework, vediamo in cosa
consiste.

Capire la struttura delle cartelle
----------------------------------

La struttura delle cartelle di un':term:`applicazione` Symfony è alquanto flessibile,
ma la struttura raccomandata è la seguente:

``app/``
    La configurazione dell'applicazione;
``src/``
    Il codice PHP del progetto;
``vendor/``
    Le dipendenze di terze parti;
``web/``
    La cartella radice del web.

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
    $request = Request::createFromGlobals();
    $response = $kernel->handle($request);
    $response->send();

Il front controller inizializza l'applicazione, usando una classe kernel (``AppKernel``,
in questo caso). Quindi, crea l'oggetto ``Request``, usando le variabili globali di PHP,
e lo passa al kernel. L'ultimo passo è l'invio del contenuto della risposta,
restituito dal kernel all'utente.

.. _the-app-dir:

La cartella ``app/``
~~~~~~~~~~~~~~~~~~~~

La classe ``AppKernel`` è il punto di ingresso principale della configurazione
dell'applicazione e quindi è memorizzata nella cartella ``app/``.

Questa classe deve implementare due metodi:

``registerBundles()``
    deve restituire un array di tutti i bundle necessari per
    eseguire l'applicazione;
``registerContainerConfiguration()``
    carica la configurazione dell'applicazione (approfondito più avanti);

Il caricamento automatico essere configurato tramite `Composer`_, che vuol dire che si
può usare qualsiasi classe PHP senza dover far nulla! Tutte le dipendenze sono
memorizzate sotto la cartella ``vendor/``, ma è solo una convenzione.
Si possono memorizzare dove si preferisce, globalmente sul proprio server o localmente
nei propri progetti.

Capire il sistema dei bundle
----------------------------

Questa sezione è un'introduzione a una delle più grandi e
potenti caratteristiche di Symfony, il sistema dei :term:`bundle`.

Un bundle è molto simile a un plugin in un altro software. Ma perché
allora si chiama *bundle* e non *plugin*? Perché *ogni cosa* è un bundle
in Symfony, dalle caratteristiche del nucleo del framework al codice
scritto per un'applicazione.

Tutto il codice scritto per un'applicazione è organizzato in bundle. Nel linguaggio di Symfony,
un bundle è un insieme strutturato di file (file PHP, fogli di stile, JavaScript,
immagini, ...) che implementano una singola caratteristica (un blog, un forum, ...) e che
possono essere facilmente condivisi con altri sviluppatori.

I bundle sono cittadini di prima classe in Symfony. Essi forniscono la flessibilità
di usare delle caratteristiche pre-costruite impacchettate in bundle di terze parti o di distribuire 
i propri bundle. Questo rende molto facile scegliere quali caratteristiche abilitare in
un'applicazione e ottimizzarle nel modo preferito. A fine giornata, il codice
dell'applicazione è *importante* quanto il nucleo stesso del
framework.

Symfony include un AppBundle, che si può usare per iniziare lo sviluppo
di un'applicazione. Quindi, se occorre dividere l'applicazione in componenti
riutilizzabili, si possono creare altri bundle.

Registrare un bundle
~~~~~~~~~~~~~~~~~~~~

Un'applicazione è composta di bundle, come definito nel metodo ``registerBundles()``
della classe ``AppKernel`` . Ogni bundle è una cartella che contiene una singola classe
Bundle che la descrive::

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
            new AppBundle\AppBundle();
        );

        if (in_array($this->getEnvironment(), array('dev', 'test'))) {
            $bundles[] = new Symfony\Bundle\WebProfilerBundle\WebProfilerBundle();
            $bundles[] = new Sensio\Bundle\DistributionBundle\SensioDistributionBundle();
            $bundles[] = new Sensio\Bundle\GeneratorBundle\SensioGeneratorBundle();
        }

        return $bundles;
    }

Oltre ad AppBundle, di cui abbiamo già parlato, si noti che il kernel
abilita anche FrameworkBundle, DoctrineBundle,
SwiftmailerBundle e AsseticBundle. Fanno tutti parte del nucleo del framework.

Configurare un bundle
~~~~~~~~~~~~~~~~~~~~~

Ogni bundle può essere personalizzato tramite file di configurazione scritti in YAML,
XML o PHP. Si veda la configurazione predefinita:

.. code-block:: yaml

    # app/config/config.yml
    imports:
        - { resource: parameters.yml }
        - { resource: security.yml }
        - { resource: services.yml }

    framework:
        #esi:             ~
        #translator:      { fallbacks: ["%locale%"] }
        secret:          "%secret%"
        router:
            resource: "%kernel.root_dir%/config/routing.yml"
            strict_requirements: "%kernel.debug%"
        form:            true
        csrf_protection: true
        validation:      { enable_annotations: true }
        templating:      { engines: ['twig'] }
        default_locale:  "%locale%"
        trusted_proxies: ~
        session:         ~

    # Configurazione di Twig
    twig:
        debug:            "%kernel.debug%"
        strict_variables: "%kernel.debug%"

    # Configurazione di Swift Mailer
    swiftmailer:
        transport: "%mailer_transport%"
        host:      "%mailer_host%"
        username:  "%mailer_user%"
        password:  "%mailer_password%"
        spool:     { type: memory }

    # ...

Ogni voce di primo livello, come ``framework``, ``twig`` e ``swiftmailer``,
definisce la configurazione per uno specifico bundle. Per esempio, ``framework``
configura FrameworkBundle, mentre ``swiftmailer`` configura
SwiftmailerBundle.

Ogni :term:`ambiente` può sovrascrivere la configurazione predefinita, fornendo un file
di configurazione specifico. Per esempio, l'ambiente ``dev`` carica il file ``config_dev.yml``,
che carica la configurazione principale (cioè ``config.yml``) e quindi la modifica per
aggiungere alcuni strumenti di debug:

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

    # ...

Estendere un bundle
~~~~~~~~~~~~~~~~~~~

Oltre a essere un modo carino per organizzare e configurare il codice, un bundle
può estendere un altro bundle. L'ereditarietà dei bundle consente di sovrascrivere un bundle
esistente, per poter personalizzare i suoi controllori, i template o qualsiasi altro
suo file.

Nomi logici di file
...................

Quando si vuole fare riferimento a un file da un bundle, usare questa notazione:
``@NOME_BUNDLE/percorso/del/file``; Symfony risolverà ``@NOME_BUNDLE`` nel percorso
reale del bundle. Per esempio, il percorso logico
``@AppBundle/Controller/DefaultController.php`` verrebbe convertito in
``src/AppBundle/Controller/DefaultController.php``, perché Symfony conosce
la locazione di AppBundle.

Nomi logici di controllori
..........................

Per i controllori, occorre fare riferimento ai nomi dei metodi usando il formato
``NOME_BUNDLE:NOME_CONTROLLORE:NOME_AZIONE``. Per esempio,
``AppBundle:Default:index`` mappa il metodo ``indexAction`` della classe
``AppBundle\Controller\DefaultController``.

Estendere i bundle
..................

Se si seguono queste convenzioni, si può usare
l':doc:`ereditarietà dei bundle </cookbook/bundles/inheritance>`
per "sovrascrivere" file, controllori o template. Per esempio, se un nuovo bundle
chiamato NewBundle estende AppBundle, Symfony proverà, caricando il controllore
``AppBundle:Default:index``, a caricare prima il controllore
``DefaultController`` da NewBundle e poi cercherà in AppBundle, se il primo non esiste.
Questo vuol dire che un bundle può sovrascrivere
quasi ogni parte di un altro bundle!

È chiaro ora perché Symfony è così flessibile? Condividere bundle tra le
applicazioni, memorizzarli localmente o globalmente, a scelta.

.. _using-vendors:

Usare i venditori
-----------------

Probabilmente l'applicazione dipenderà da librerie di terze parti.
Queste ultime dovrebbero essere memorizzate nella cartella ``vendor/``. Non si dovrebbe
mai toccare questa cartella, perché è gestita esclusivamente da Composer. Tale cartella
contiene già le librerie di Symfony, SwiftMailer, l'ORM Doctrine,
il sistema di template Twig e alcune altre librerie e bundle di terze
parti.

Capire la cache e i log
-----------------------

Symfony è forse uno dei framework completi più veloci in circolazione.
Ma come può essere così veloce, se analizza e interpreta decine di file
YAML e XML a ogni richiesta? In parte, per il suo sistema di cache. La
configurazione dell'applicazione è analizzata solo per la prima richiesta
e poi compilata in semplice file PHP, memorizzato nella cartella ``app/cache/``.


Nell'ambiente di sviluppo, Symfony è abbastanza intelligente da pulire la cache
quando cambiano dei file. In produzione, invece, occorre pulire la cache
manualmente quando si aggiorna il codice o si modifica la
configurazione. Eseguire il seguente comando per pulire la cache in
ambiente ``prod``:

.. code-block:: bash

    $ php app/console cache:clear --env=prod

Sviluppando un'applicazione web, le cose possono andar male in diversi modi.
I file di log nella cartella ``app/logs/`` dicono tutto a proposito delle richieste
e aiutano a risolvere il problema in breve tempo.

Usare l'interfaccia a linea di comando
--------------------------------------

Ogni applicazione ha uno strumento di interfaccia a linea di comando (``app/console``),
che aiuta nella manutenzione dell'applicazione. La console fornisce dei comandi che incrementano la
produttività, automatizzando dei compiti noiosi e ripetitivi.

Richiamandola senza parametri, si può sapere di più sulle sue capacità:

.. code-block:: bash

    $ php app/console

L'opzione ``--help`` aiuta a scoprire l'utilizzo di un comando:

.. code-block:: bash

    $ php app/console router:debug --help

Considerazioni finali
---------------------

Dopo aver letto questa parte, si dovrebbe essere in grado di muoversi facilmente
dentro Symfony e farlo funzionare. Ogni cosa in Symfony è fatta per
rispondere alle varie esigenze. Quindi, si possono rinominare e spostare le
varie cartelle, finché non si raggiunge il risultato voluto.

E questo è tutto per il giro veloce. Dai test all'invio di email, occorre ancora
imparare diverse cose per padroneggiare Symfony. Pronti per approfondire questi
temi? Senza indugi, basta andare nella pagine del :doc:`libro </book/index>` e
scegliere un argomento a piacere.

.. _Composer:   http://getcomposer.org
