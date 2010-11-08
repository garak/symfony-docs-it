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
i file che rispettano l'interoperabilità `standards`_ per i namespace di
PHP 5.3 oppure la `convenzione`_ dei nomi di PEAR per le classi. Come si
può vedere, tutte le dipendenze sono sotto la cartella ``vendor/``, ma
questa è solo una convenzione. Si possono inserire in qualsiasi posto,
globalmente sul proprio server o localmente nei propri progetti.

.. index::
   single: Bundles

TODO.....
