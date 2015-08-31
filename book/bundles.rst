.. index::
   single: Bundle

.. _page-creation-bundles:

Il sistema dei bundle
=====================

Un bundle è simile a un plugin in altri software, ma anche meglio. La differenza
fondamentale è che *tutto* è un bundle in Symfony, incluse le funzionalità
fondamentali del framework o il codice scritto per la propria applicazione.
I bundle sono cittadini di prima classe in Symfony. Questo fornisce la flessibilità
di usare caratteristiche già pronte impacchettate in `bundle di terze parti` o di
distribuire i propri bundle. Rende facile scegliere quali caratteristiche abilitare
nella propria applicazione per ottimizzarla nel modo preferito.

.. note::

   Pur trovando qui i fondamentali, un'intera ricetta è dedicata all'organizzazione e
   alle pratiche migliori in :doc:`bundle </cookbook/bundles/best_practices>`.

Un bundle è semplicemente un insieme strutturato di file dentro una cartella, che implementa
una singola caratteristica. Si potrebbe creare un ``BlogBundle``, un ``ForumBundle`` o un
bundle per la gestione degli utenti (molti di questi già esistono come bundle open source).
Ogni cartella contiene tutto ciò che è relativo a quella caratteristica, inclusi file
PHP, template, fogli di stile, JavaScript, test e tutto il resto.
Ogni aspetto di una caratteristica esiste in un bundle e ogni caratteristica risiede
in un bundle.

Un'applicazione è composta di bundle, come definito nel metodo ``registerBundles()``
della classe ``AppKernel``::

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
            new AppBundle\AppBundle(),
        );

        if (in_array($this->getEnvironment(), array('dev', 'test'))) {
            $bundles[] = new Symfony\Bundle\WebProfilerBundle\WebProfilerBundle();
            $bundles[] = new Sensio\Bundle\DistributionBundle\SensioDistributionBundle();
            $bundles[] = new Sensio\Bundle\GeneratorBundle\SensioGeneratorBundle();
        }

        return $bundles;
    }

Col metodo ``registerBundles()``, si ha il controllo totale su quali bundle siano usati
dalla propria applicazione (inclusi i bundle del nucleo di Symfony).

.. tip::

   Un bundle può stare *ovunque*, purché possa essere auto-caricato (tramite
   l'autoloader configurato in ``app/autoload.php``).

Creare un bundle
~~~~~~~~~~~~~~~~

Symfony Standard Edition contiene un task utile per creare un bundle pienamente
funzionante. Ma anche creare un bundle a mano è molto facile.

Per dimostrare quanto è semplice il sistema dei bundle, creiamo un nuovo bundle,
chiamato ``AcmeTestBundle``, e abilitiamolo.

.. tip::

    La parte ``Acme`` è solo un nome fittizio, che andrebbe sostituito da un nome di
    "venditore" che rappresenti la propria organizzazione (p.e. ``ABCTestBundle``
    per un'azienda chiamata ``ABC``).

Iniziamo creando una cartella ``src/Acme/TestBundle/`` e aggiungendo un nuovo file
chiamato ``AcmeTestBundle.php``::

    // src/Acme/TestBundle/AcmeTestBundle.php
    namespace Acme\TestBundle;

    use Symfony\Component\HttpKernel\Bundle\Bundle;

    class AcmeTestBundle extends Bundle
    {
    }

.. tip::

   Il nome AcmeTestBundle segue le
   :ref:`convenzioni sui nomi dei bundle<bundles-naming-conventions>`. Si
   potrebbe anche scegliere di accorciare il nome del bundle semplicemente a TestBundle,
   chiamando la classe ``TestBundle`` (e chiamando il file ``TestBundle.php``).

Questa classe vuota è l'unico pezzo necessario a creare un nuovo bundle. Sebbene solitamente
vuota, questa classe è potente e può essere usata per personalizzare il comportamento
del bundle.

Ora che il bundle è stato creato, va abilitato tramite la classe ``AppKernel``::

    // app/AppKernel.php
    public function registerBundles()
    {
        $bundles = array(
            // ...
            // registra il bundle
            new Acme\TestBundle\AcmeTestBundle(),
        );
        // ...

        return $bundles;
    }

Sebbene non faccia ancora nulla, AcmeTestBundle è ora pronto per essere usato.

Symfony fornisce anche un'interfaccia a linea di comando per generare
uno scheletro di base per un bundle:

.. code-block:: bash

    $ php app/console generate:bundle --namespace=Acme/TestBundle

Lo scheletro del bundle è generato con controllore, template e rotte, tutti
personalizzabili. Approfondiremo più avanti la linea di comando di
Symfony.

.. tip::

   Ogni volta che si crea un nuovo bundle o che si usa un bundle di terze parti,
   assicurarsi sempre che il bundle sia abilitato in ``registerBundles()``. Se si usa
   il comando ``generate:bundle``, l'abilitazione è automatica.

Struttura delle cartelle dei bundle
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

La struttura delle cartelle di un bundle è semplice e flessibile. Per impostazione
predefinita, il sistema dei bundle segue un insieme di convenzioni, che aiutano a
mantenere il codice coerente tra tutti i bundle di Symfony. Si dia un'occhiata a
``AcmeHelloBundle``, perché contiene alcuni degli elementi più comuni di un bundle:

``Controller/``
    contiene i controllori del bundle (p.e. ``HelloController.php``);

``DependencyInjection/``
    contiene alcune estensioni di classi,
    che possono importare configurazioni di servizi, registrare passi di compilatore o altro
    (tale cartella non è indispensabile);

``Resources/config/``
    contiene la configurazione, compresa la configurazione delle rotte (p.e. ``routing.yml``);

``Resources/views/``
    contiene i template, organizzati per nome di controllore (p.e. ``Hello/index.html.twig``);

``Resources/public/``
    contiene le risorse per il web (immagini, fogli di stile, ecc.)
    ed è copiata o collegata simbolicamente alla cartella ``web/`` del progetto, tramite
    il comando ``assets:install``;

``Tests/``
 contiene tutti i test del bundle.

Un bundle può essere grande o piccolo, come la caratteristica che implementa. Contiene
solo i file che occorrono e niente altro.

Andando avanti nel libro, si imparerà come persistere gli oggetti in una base dati,
creare e validare form, creare traduzioni per la propria applicazione, scrivere
test e molto altro. Ognuno di questi ha il suo posto e il suo ruolo dentro il
bundle.

.. _`bundle di terze parti`: http://knpbundles.com
