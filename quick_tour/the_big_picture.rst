Un quadro generale
==================

Volete provare Symfony2 avendo a disposizione solo dieci minuti? Questa prima
parte di questa guida è stata scritta appositamente: spiega come
partire subito con Symfony2, mostrando la struttura di un semplice progetto già pronto.

Chi ha già usato un framework per il web si troverà come a casa con Symfony2. Altrimenti,
benvenuti in un nuovo mondo per sviluppare applicazioni web!

Installare Symfony2
-------------------

Prima di tutto, verificare di aver installato una versione di PHP compatibile con i requisiti
di Symfony2: 5.3.3 o successivi. Quindi, apire una finestra di terminale ed eseguire il comando
seguente, per installare l'ultima versione di Symfony2 nella cartella
``progetto/``:

.. code-block:: bash

    $ composer create-project symfony/framework-standard-edition progetto/ '~2.5'

.. note::

    `Composer`_ è il gestore di pacchetti usato dalle applicazioni PHP recenti ed è
    l'unico modo raccomandato di installare Symfony2. Per installare Composer su
    Linux o Mac, eseguire i seguenti comandi:

    .. code-block:: bash

        $ curl -sS https://getcomposer.org/installer | php
        $ sudo mv composer.phar /usr/local/bin/composer

    Per installare Composer su Windows, scaricare l'`installer`_.

La prima installazione di Symfony2 potrebbe richiedere vari minuti, per
scaricare tutti i componenti. Alla fine del processo di installazione,
l'installatore porrà alcune domande:

1. **Would you like to use Symfony 3 directory structure? [y/N]** La futura versione
   Symfony 3 modificherà la struttura predefinita delle cartelle per le applicazioni Symfony.
   Se si vuole provare la nuova struttura, rispondere ``y``.
   Per poter seguire questa guida, premere "Invio" per accettare il valore
   predefinito ``N`` e mantenere la struttura predefinita di Symfony2.
2. **Would you like to install Acme demo bundle? [y/N]** Le versioni di Symfony precedenti
   a 2.5 includevano un'applicazione dimostrativa, per provare alcune caratteristiche del
   framework. Tuttavia, poiché l'applicazione dimostrativa è utile solo per i principianti,
   la sua installazione ora è facoltativa. Per poter seguire questa guida, rispondere
   ``y`` e installare l'applicazione dimostrativa.
3. **Some parameters are missing. Please provide them.** Symfony2 chiede i
   valori dei parametri di configurazione. Per questo primo progetto,
   si può tranquillamente sorvolare e premere "Invio"
   ripetutamente.
4. **Do you want to remove the existing VCS (.git, .svn..) history? [Y,n]?**
   La cronologia di sviluppo di grandi progetti, come Symfony, può occupare
   molto spazio. Premere "Invio" per rimuovere senza problemi tutti i dati della cronologia.

Eseguire Symfony2
-----------------

Prima di eseguire Symfony2 per la prima volta, eseguire il comando seguente,
per assicurarsi che il sistema soddisfi i requisiti tecnici:

.. code-block:: bash

    $ cd progetto/
    $ php app/check.php

Sistemare ogni eventuale problema segnato, quindi usare il server web incluso in PHP
per far girare Symfony:

.. code-block:: bash

    $ php app/console server:run

.. seealso::

    Si può sapere di più sul server interno sul :doc:`ricettario </cookbook/web_server/built_in>`.

In caso di errore `There are no commands defined in the "server" namespace.`,
probabilmente si sta usando PHP 5.3. Va bene lo stesso, la il server web è
disponibile solo da PHP 5.4.0 in poi. Per vecchie versioni di PHP o se si
preferisce un server web tradizionale, come Apache o Nginx, leggere la ricetta
:doc:`/cookbook/configuration/web_server_configuration`.

Aprire un browser e accedere all'URL ``http://localhost:8000`` per vedere
la pagina di benvenuto di Symfony2:

.. image:: /images/quick_tour/welcome.png
   :align: center
   :alt:   Pagina di benvenuto di Symfony2

Capire i fondamenti
-------------------

Uno degli obiettivi principali di un framework è quello di mantenere il codice organizzato e
consentire all'applicazione di evolvere facilmente nel tempo, evitando il miscuglio di chiamate
alla base dati, tag HTML e logica di business nello stesso script. Per raggiungere questo obiettivo
con Symfony, occorre prima imparare alcuni termini e concetti fondamentali.

Symfony offre alcuni esempi di codice, che possono essere usati per capire meglio
i concetti fondamentali di Symfony. Si vada al seguente URL per essere salutati da Symfony2
(sostituire *Fabien* col proprio nome):

.. code-block:: text

    http://localhost:8000/app_dev.php/demo/hello/Fabien

.. image:: /images/quick_tour/hello_fabien.png
   :align: center

.. note::

    Al posto della pagina con il saluto, si potrebbe vedere una pagina di errore.
    La causa è una configurazione errata dei permessi delle cartelle. Ci sono varie
    soluzioni possibili, a seconda del sistema operativo. Tutte queste soluzioni sono
    spiegate nella sezione :ref:`impostazione dei permessi <book-installation-permissions>`
    del libro.

Cosa sta accadendo? Dissezioniamo l'URL:

* ``app_dev.php``: È un :term:`front controller`. È l'unico punto di ingresso
  dell'applicazione e risponde a ogni richiesta dell'utente;

* ``/demo/hello/Fabien``: È il *percorso virtuale* alla risorsa a cui l'utente
  vuole accedere .

È responsabilità dello sviluppatore scrivere il codice che mappa la *richiesta*
dell'utente (``/demo/hello/Fabien``) alla *risorsa* a essa associata
(la pagina HTML ``Hello Fabien!``).

Rotte
~~~~~

Symfony2 dirige la richiesta al codice che la gestisce, cercando la corrispondenza
tra l'URL richiesto e alcuni schemi configurati. Le rotte delle pagine di demo
sono nel file di configurazione ``app/config/routing_dev.yml``:

.. code-block:: yaml

    # app/config/routing_dev.yml
    # ...

    # rotte AcmeDemoBundle (da rimuovere)
    _acme_demo:
        resource: "@AcmeDemoBundle/Resources/config/routing.yml"

Questo importa un file ``routing.yml``, che si trova in AcmeDemoBundle:

.. code-block:: yaml

    # src/Acme/DemoBundle/Resources/config/routing.yml
    _welcome:
        path:     /
        defaults: { _controller: AcmeDemoBundle:Welcome:index }

    _demo:
        resource: "@AcmeDemoBundle/Controller/DemoController.php"
        type:     annotation
        prefix:   /demo

    # ...

Le prime tre righe (dopo il commento) definiscono quale codice
richiamare quanto l'utente richiede la risorsa "``/``" (come la pagina di benvenuto
vista prima). Quando richiesto, il controllore ``AcmeDemoBundle:Welcome:index`` sarà
eseguito. Nella prossima sezione, si imparerà esattamente quello che significa.

.. tip::

    Oltre ai file YAML, Symfony2 supporta nativamente anche XML, PHP e
    le annotazioni. Questa flessibilità è uno dei punti di forza di
    Symfony2, un framework che non impone mai un formato di configurazione
    particolare.

Controllori
~~~~~~~~~~~

Il controllore è una funzione o un metodo PHP che gestisce le *richieste* in entrata
e restituisce delle *risposte* (spesso codice HTML). Invece di usare variabili e
funzioni globali di PHP (come ``$_GET`` o ``header()``) per gestire questi messaggi
HTTP, Symfony usa degli oggetti: :class:`Symfony\\Component\\HttpFoundation\\Request`
e :class:`Symfony\\Component\\HttpFoundation\\Response`.  Il controllore più semplice
possibile potrebbe creare la risposta a mano, basandosi sulla richiesta::

    use Symfony\Component\HttpFoundation\Response;

    $name = $request->get('name');

    return new Response('Hello '.$name);

Symfony2 sceglie il controllore basandosi sul valore ``_controller`` della configurazione
delle rotte: ``AcmeDemoBundle:Welcome:index``. Questa stringa è il *nome logico* del
controllore e fa riferimento al metodo ``indexAction`` della classe
``Acme\DemoBundle\Controller\WelcomeController``::

    // src/Acme/DemoBundle/Controller/WelcomeController.php
    namespace Acme\DemoBundle\Controller;

    use Symfony\Bundle\FrameworkBundle\Controller\Controller;

    class WelcomeController extends Controller
    {
        public function indexAction()
        {
            return $this->render('AcmeDemoBundle:Welcome:index.html.twig');
        }
    }

.. tip::

    Si sarebbero potuti usare i nomi completi di classe e metodi,
    ``Acme\DemoBundle\Controller\WelcomeController::indexAction``, per il valore
    di ``_controller``. Ma se si seguono alcune semplici convenzioni, il nome logico
    è più breve e consente maggiore flessibilità.

La classe ``WelcomeController`` estende la classe predefinita ``Controller``,
che fornisce alcuni utili metodi scorciatoia, come il metodo
:ref:`render()<controller-rendering-templates>`, che carica e rende
un template (``AcmeDemoBundle:Welcome:index.html.twig``). Il valore restituito
è un oggetto risposta, popolato con il contenuto resto. Quindi, se ci sono nuove
necessità, l'oggetto risposta può essere manipolato prima di essere inviato al browser::

    public function indexAction()
    {
        $response = $this->render('AcmeDemoBundle:Welcome:index.txt.twig');
        $response->headers->set('Content-Type', 'text/plain');

        return $response;
    }

Indipendentemente da come lo si raggiunge, lo scopo finale di un controllore
è sempre quello di restituire l'oggetto ``Response`` da inviare all'utente. Questo
oggetto ``Response`` può essere popolato con codice HTML, rappresentare un rinvio del
client o anche restituire il contenuto di un'immagine JPG, con un header ``Content-Type`` del valore ``image/jpg``.

Il nome del template, ``AcmeDemoBundle:Welcome:index.html.twig``, è il
*nome logico* del template e fa riferimento al file ``Resources/views/Welcome/index.html.twig``
dentro AcmeDemoBundle (localizzato in ``src/Acme/DemoBundle``). La sezione successiva
sui bundle ne spiega l'utilità.

Diamo ora un altro sguardo al file di configurazione delle rotte e cerchiamo la voce
``_demo``:

.. code-block:: yaml

    # src/Acme/DemoBundle/Resources/config/routing.yml
    # ...
    _demo:
        resource: "@AcmeDemoBundle/Controller/DemoController.php"
        type:     annotation
        prefix:   /demo

Il *nome logico* del file che contiene le rotte ``_demo`` è
``@AcmeDemoBundle/Controller/DemoController.php`` e si riferisce al
file ``src/Acme/DemoBundle/Controller/DemoController.php``. In
questo file, le rotte sono definite come annotazioni sui metodi delle azioni::

    // src/Acme/DemoBundle/Controller/DemoController.php
    use Sensio\Bundle\FrameworkExtraBundle\Configuration\Route;
    use Sensio\Bundle\FrameworkExtraBundle\Configuration\Template;

    class DemoController extends Controller
    {
        /**
         * @Route("/hello/{name}", name="_demo_hello")
         * @Template()
         */
        public function helloAction($name)
        {
            return array('name' => $name);
        }

        // ...
    }

L'annotazione ``@Route()`` definisce una nuova rotta con uno schema
``/hello/{name}``, che esegue il metodo ``helloAction`` quando trovato.
Una stringa racchiusa tra parentesi graffe, come ``{name}``, è chiamata segnaposto.
Come si può vedere, il suo valore può essere recuperato tramite il parametro ``$name`` del metodo.

Dando un'occhiata più attenta al codice del controllore, si può vedere che invece di
rendere un template e restituire un oggetto ``Response`` come prima, esso restituisce
solo un array di parametri. L'annotazione ``@Template()`` dice a Symfony di rendere
il template al posto nostro, passando ogni variabili dell'array al template. Il nome
del template resto segue il nome del controllore. Quindi, nel nostro esempio, viene
reso il template ``AcmeDemoBundle:Demo:hello.html.twig`` (localizzato in
``src/Acme/DemoBundle/Resources/views/Demo/hello.html.twig``).

Template
~~~~~~~~

Il controllore rende il template ``src/Acme/DemoBundle/Resources/views/Demo/hello.html.twig``
(oppure ``AcmeDemoBundle:Demo:hello.html.twig``, se si usa il nome logico):

.. code-block:: jinja

    {# src/Acme/DemoBundle/Resources/views/Demo/hello.html.twig #}
    {% extends "AcmeDemoBundle::layout.html.twig" %}

    {% block title "Hello " ~ name %}

    {% block content %}
        <h1>Hello {{ name }}!</h1>
    {% endblock %}

Per impostazione predefinita, Symfony2 usa `Twig`_ come sistema di template, ma si
possono anche usare i tradizionali template PHP, se si preferisce. Il
:doc:`prossimo capitolo</quick_tour/the_view>` introdurrà
il modo in cui funzionano i template in in Symfony2.

Bundle
~~~~~~

Forse ci si sta chiedendo perché il termine :term:`bundle` sia stato usato così tante volte
finora. Tutto il codice che si scrive per un'applicazione è organizzato in
bundle. Nel linguaggio di Symfony2, un bundle è un insieme strutturato di file (file
PHP, fogli di stile, JavaScript, immagini, ...) che implementano una singola
caratteristica (un blog, un forum, ...) e che può essere condivisa facilmente con
altri sviluppatori. Finora è stato trattato un solo bundle, ``AcmeDemoBundle``.
Si vedrà di più sui bundle nell'ultimo capitolo di questa guida.

.. _quick-tour-big-picture-environments:

Lavorare con gli ambienti
-------------------------

Ora che si possiede una migliore comprensione di come funziona Symfony2, è
ora di dare un'occhiata più da vicino al fondo della pagina: si noterà
una piccola barra con il logo di Symfony2. Questa barra è chiamata
"barra di debug del web" ed è il miglior amico dello sviluppatore.

.. image:: /images/quick_tour/web_debug_toolbar.png
   :align: center

Ma quello che si vede all'inizio è solo la punta dell'iceberg: cliccando
sullo strano numero esadecimale, si rivelerà un altro strumento di debug veramente
utile di Symfony2: il profilatore.

.. image:: /images/quick_tour/profiler.png
   :align: center

Ovviamente, questo strumento non deve essere mostrato quando si rilascia l'applicazione
su un server di produzione. Per questo motivo, si troverà un altro front controller (``app.php``)
nella cartella ``web/``, ottimizzato per l'ambiente di produzione:

.. _quick-tour-big-picture-environments-intro:

Che cos'è un ambiente?
~~~~~~~~~~~~~~~~~~~~~~

Un :term:`Ambiente` è una stringa che rappresenta un gruppo di configurazioni
usate per far girare un'applicazione. Symfony2 definisce due ambienti di base: ``dev``
(adatto per lo sviluppo in locale) e ``prod`` (ottimizzato
per eseguire l'applicazione in produzione).

Di solito, gli ambienti contengono una grande quantità di opzioni di configurazione. Per
questo motivo, si tiene la configurazione comune ``config.yml`` e si sovrascrive,
ove necessario, la configurazione per ciascun ambiente:

.. code-block:: yaml

    # app/config/config_dev.yml
    imports:
        - { resource: config.yml }

    web_profiler:
        toolbar: true
        intercept_redirects: false

In questo esempio, l'ambiente ``dev`` carica il file di configurazione ``config_dev.yml``,
che importa il file globale ``config.yml`` e quindi lo modifica,
abilitando la barra di debug del web.

Richiamando il file ``app_dev.php`` nel browser, si esegue
l'applicazione Symfony in ambiente ``dev``. Per vedere l'applicazione
in ambiente ``prod``, richiamare invece ``app.php``.

Le rotte di demo nell'applicazione sono disponibili solo in ambiente ``dev``.
Quindi se si provar ad accedere all'URL ``http://localhost/app.php/demo/hello/Fabien``,
si ottiene un errore 404.

.. tip::

    Se, invece di usare il server web di PHP, si usa Apache con
    ``mod_rewrite`` abilitato, sfruttando il file ``.htaccess`` fornito da
    Symfony2  in ``web/``, si può anche omettere la parte ``app.php`` dell'URL.
    Il file ``.htaccess`` punta tutte le richieste al front controller
    ``app.php``:

    .. code-block:: text

        http://localhost/demo/hello/Fabien

Per maggiori dettagli sugli ambienti, vedere la pagina
":ref:`ambienti e front controller <page-creation-environments>`".

Considerazioni finali
---------------------

Congratulazioni! Avete avuto il vostro primo assaggio di codice di Symfony2.
Non era così difficile, vero? C'è ancora molto da esplorare, ma dovreste
già vedere come Symfony2 rende veramente facile implementare siti web in modo
migliore e più veloce. Se siete ansiosi di saperne di più, andate alla prossima
sezione: ":doc:`la vista<the_view>`".

.. _Composer:             https://getcomposer.org/
.. _installer:            http://getcomposer.org/download
.. _Twig:                 http://twig.sensiolabs.org/
