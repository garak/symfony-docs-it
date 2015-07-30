Un quadro generale
==================

Iniziare a usare Symfony in dieci minuti! Questa guida attraverserò
i concetti più importanti dietro a Symfony e spiegherà in che modo si può partire rapidamente
con Symfony, mostrando la struttura di un semplice progetto già pronto.

Chi ha già usato un framework per il web si troverà come a casa con Symfony. Altrimenti,
benvenuti in un nuovo mondo per sviluppare applicazioni web!

L'unico requisito tecnico per seguire questa guida è avere **PHP 5.4 o successivi
installato sulla propria macchina**. Se si usa una soluzione predefinita per PHP, come WAMP,
XAMP o MAMP, verificare di stare usando PHP 5.4 o una versione più recente.
Si può anche eseguire il seguente comando nel terminale o nella console,
per mostrare la versione di PHP installata:

.. code-block:: bash

    $ php --version

.. _installing-symfony2:

Installare Symfony
------------------

In passato, Symfony andava installato a mano su ogni nuovo progetto. Ora si può
usare l'**installatore di Symfony**, che deve essere installato solo la prima volta che
si usa Symfony su una macchina.

Su sistemi **Linux** e **Mac OS X**, eseguire i seguenti comandi da console:

.. code-block:: bash

    $ curl -LsS http://symfony.com/installer > symfony.phar
    $ sudo mv symfony.phar /usr/local/bin/symfony
    $ chmod a+x /usr/local/bin/symfony

Dopo aver installato l'installatore di Symfony, aprire una nuova console, per poter
eseguire il nuovo comando ``symfony``:

.. code-block:: bash

    $ symfony

Su sistemi **Windows**, eseguire il seguente comando da console:

.. code-block:: bash

    c:\> php -r "readfile('http://symfony.com/installer');" > symfony.phar

Questo comando scarica un file chiamato ``symfony.phar``, che contiene l'installatore di
Symfony. Salvare o spostare il file nella cartella in cui si vogliono creare i
progetti Symfony ed eseguire l'installatore di Symfony, con questo
comando:

.. code-block:: bash

    c:\> php symfony.phar

Creare il primo progetto Symfony
--------------------------------

Una volta pronto l'installatore di Symfony, usare il comando ``new`` per creare nuovi progetti
Symfony. Creiamo un progetto chiamato ``mioprogetto``:

.. code-block:: bash

    # Linux e Mac OS X
    $ symfony new mioprogetto

    # Windows
    c:\> php symfony.phar new mioprogetto

Questo comando scarica l'ultima versione stabile di Symfony e crea un progetto
vuoto nella cartella ``mioprogetto/``, in modo da poter iniziare lo sviluppo
di un'applicazione.

.. _running-symfony2:

Eseguire Symfony
----------------

Questa guida si appoggia al server web interno fornito da PHP, per eseguire applicazioni Symonfy.
Di conseguenza, per eseguire un'applicazione Symfony basta andare
nella cartella del progetto ed eseguire questo comando:

.. code-block:: bash

    $ cd mioprogetto/
    $ php app/console server:run

Aprire un browser e accedere all'URL ``http://localhost:8000`` per vedere
la pagina di benvenuto di Symfony:

.. image:: /images/quick_tour/welcome.png
   :align: center
   :alt: Pagina di benvenuto di Symfony

Congratulazioni! Il primo progetto Symfony sta funzionando!

.. note::

    Al posto della pagina di benvenuto, si potrebbe vedere una pagina vuota o di errore.
    La causa è una mancata configurazione dei permessi delle cartelle. Ci sono diverse
    possibili soluzioni, a seconda del sistema operativo. Sono tutte spiegate
    nella sezione
    :ref:`permessi <book-installation-permissions>`
    del libro ufficiale.

    Se la pagina di benvenuto sembra non avere stili o immagini, provare
    a installarli:
    
    .. code-block:: bash
    
        $ php app/console assets:install

Dopo aver finito di lavorare sull'applicazione Symfony, si può fermare il
server, con il comando ``server:stop``:

.. code-block:: bash

    $ php app/console server:stop

.. tip::

    Chi preferisce un server web tradizionale, come Apache o Nginx, può leggere la ricetta
    :doc:`/cookbook/configuration/web_server_configuration`.

Capire i fondamenti
-------------------

Uno degli obiettivi principali di un framework è quello di mantenere il codice organizzato e
consentire all'applicazione di evolvere facilmente nel tempo, evitando il miscuglio di chiamate
alla base dati, tag HTML e logica di business nello stesso script. Per raggiungere questo obiettivo
con Symfony, occorre prima imparare alcuni termini e concetti fondamentali.

Sviluppando un'applicazione Symfony, la responsabilità dello sviluppatore è scrivere
codice che mappi una *richiesta* dell'utente (come ``http://localhost:8000/``)
su una *risorsa* a essa associata (la pagina HTML ``Benvenuto in Symfony!``).

Il codice da eseguire è definito in **azioni** e **controllori**, La mappatura
tra richiesta utente e tale codice è definita tramite la configurazione delle **rotte**.
Il contenuto mostrato nel browser solitamente viene reso usando dei **template**.

Aprendo in precedenza ``http://localhost:8000/``, Symfony ha eseguito il
controllore definito nel file ``src/AppBundle/Controller/DefaultController.php``
e reso il template ``app/Resources/views/default/index.html.twig`` template.
Nelle sezioni successive, si vedrà in dettaglio le operazioni interne di
controllori, rotte e template.

Azioni e controllori
~~~~~~~~~~~~~~~~~~~~

Aprendo il file ``src/AppBundle/Controller/DefaultController.php``, si vedrà il
codice seguente (per ora, non far caso alla configurazione ``@Route``, sarà
spiegata nella prossima sezione)::

    namespace AppBundle\Controller;

    use Sensio\Bundle\FrameworkExtraBundle\Configuration\Route;
    use Symfony\Bundle\FrameworkBundle\Controller\Controller;

    class DefaultController extends Controller
    {
        /**
         * @Route("/app/example", name="homepage")
         */
        public function indexAction()
        {
            return $this->render('default/index.html.twig');
        }
    }

Nelle applicazioni Symfony, i **controllori** sono solitamente classi PHP, il cui nome
finisce per ``Controller``. In questo esempio, il controllore si chiama
``Default`` e la classe PHP si chiama ``DefaultController``.

I metodi definiti in un controllore si chiamano **azioni**, sono solitamente
associati con un URL dell'applicazione e hanno nomi che finiscono per
``Action``. In questo esempio, il controllore ``Default`` ha un'unica azione
chiamata ``index`` e definita nel metodo ``indexAction``.

Le azioni sono solitamente molto brevi, tra le 10 e le 15 linee di codice, perché devono
solo richiamare altre parti dell'applicazione, per ottenere o generare le informazioni necessarie
e quindi rendere un template, per mostrare i risultati all'utente.

In questo esempio, l'azione ``index`` è praticamente vuota, perché non ha bisogno di
richiamare altri metodi. L'azione si limita a rendere un template, con il contenuto
*Benvenuto in Symfony!*.

Rotte
~~~~~

Le rotte di Symfony mappano ogni richiesta all'azione che la gestisce, facendo corrispondere
un URL a un percorso configurato dall'applicazione. Aprire di nuovo il file
``src/AppBundle/Controller/DefaultController.php`` e dare un'occhiata
alle tre linee di codice sopra al metodo ``indexAction``::

    // src/AppBundle/Controller/DefaultController.php
    namespace AppBundle\Controller;

    use Sensio\Bundle\FrameworkExtraBundle\Configuration\Route;
    use Symfony\Bundle\FrameworkBundle\Controller\Controller;

    class DefaultController extends Controller
    {
        /**
         * @Route("/app/example", name="homepage")
         */
        public function indexAction()
        {
            return $this->render('default/index.html.twig');
        }
    }

Queste tre righe definiscono la configurazione delle rotte, tramite l'annotazione ``@Route()``.
Un'**annotazione PHP** è un modo conveniente di configurare un metodo, senza dover scrivere
codice PHP classico. Fare attenzione all'inizio del blocco dell'annotazione, che deve essere ``/**`` e
non il semplice ``/*``.

Il primo valore di ``@Route()`` definisce l'URL a cui corrisponderà
l'azione. Poiché non occorre aggiungere l'host dell'applicazione all'URL
(p.e. ```http://example.com``), tali URL sono sempre relativi e solitamente sono
chiamati *percorsi*. In questo caso, il percorso ``/`` si riferisce all'homepage dell'applicazione.
Il secondo valore di ``@Route()`` (come ``name="homepage"``) è facoltativo e imposta
il nome della rotta. Per ora tale nome non è necessario, ma più avanti si rivelerà utile
per collegare le pagine.

Considerando tutto ciò, l'annotazione ``@Route("/app/example", name="homepage")`` crea una
nuova rotta di nome ``homepage``, che fa eseguire a Symfony l'azione ``index`` del
controllore ``Default`` quando l'utente visita il percorso
``/app/example`` dell'applicazione.

.. tip::

    Oltre alle annotazioni PHP, si possono configurare le rotte in file YAML, XML
    o PHP, come spiegato nel
    :doc:`capitolo sulle rotte del libro di Symfony </book/routing>`. Tale
    flessibilità è una delle caratteristiche principali di Symfony, un framework
    che non impone mai determinati formati di configurazione.

Template
~~~~~~~~

Il contenuto dell'azione ``index`` è questa istruzione PHP::

    return $this->render('default/index.html.twig');

Il metodo ``$this->render()`` è un'utile scorciatoia per rendere un template.
Symfony fornisce alcune scorciatoie a ogni controllore che estenda la classe
``Controller``.

La posizione predefinita dei template è la cartella ``app/Resources/views/``.
Quindi, il template ``default/index.html.twig`` corrisponde a
``app/Resources/views/default/index.html.twig``. Aprire il file per vedere
il seguente codice:

.. code-block:: html+jinja

    {# app/Resources/views/default/index.html.twig #}
    {% extends 'base.html.twig' %}

    {% block body %}
        <h1>Benvenuto in Symfony!</h1>
    {% endblock %}

Questo template è scritto in `Twig`_, un motore di template creato per applicazioni
PHP moderne. La
:doc:`seconda parte di questa guida </quick_tour/the_view>` introduce
il modo in cui funzionano i template in Symfony.

.. _quick-tour-big-picture-environments:

Lavorare con gli ambienti
-------------------------

Ora che si possiede una migliore comprensione di come funziona Symfony, è
ora di dare un'occhiata più da vicino al fondo della pagina: si noterà
una piccola barra con il logo di Symfony. Questa barra è chiamata
"barra di debug del web" ed è il miglior amico dello sviluppatore.

.. image:: /images/quick_tour/web_debug_toolbar.png
   :align: center

Ma quello che si vede all'inizio è solo la punta dell'iceberg: cliccando
sullo strano numero esadecimale, si rivelerà un altro strumento di debug veramente
utile di Symfony: il profilatore.

.. image:: /images/quick_tour/profiler.png
   :align: center

Questo strumento fornisce così tante informazioni interne sull'applicazione che ci
si potrebbe preoccupare sulla loro visibilità pubblica. Symfony è
consapevole del problema e, per questo, non mostrerà tale barra quando
l'applicazione gira su un server di produzione.

Come fa Symfony a sapere se nun'applicazione stia girando localmente o su
un server di produzione? Nella prossima sezione si illustrerà il concetto di
**ambiente**.

.. _quick-tour-big-picture-environments-intro:

Che cos'è un ambiente?
~~~~~~~~~~~~~~~~~~~~~~

Un :term:`Ambiente` è una stringa che rappresenta un gruppo di configurazioni
usate per far girare un'applicazione. Symfony definisce due ambienti di base: ``dev``
(adatto per lo sviluppo in locale) e ``prod`` (ottimizzato
per eseguire l'applicazione in produzione).

Aprendo l'URL ``http://localhost:8000`` in un browser, si sta eseguendo
l'applicazione Symfony in ambiente ``dev``. Per visitare l'applicazione
in ambiente ``prod``, aprire invece l'URL ``http://localhost:8000/app.php``.
Se si preferisce mostrare sempre l'ambiente ``dev``, si può aprire l'URL
``http://localhost:8000/app_dev.php``.

La differenza principale tra gli ambienti è che ``dev`` è ottimizzato per fornire
varie informazioni allo sviluppatore, che vuol dire prestazioni peggiori.
Invece, ``prod`` è ottimizzato per ottenere migliori prestazioni, quindi le informazioni
di debug sono disabilitate, come anche la barra di
debug.

Un'altra differenza tra gli ambienti è rappresentata dalle opzioni di configurazione usate per
eseguire l'applicazione. Accedendo all'ambiente ``dev``, Symfony carica il file di
configurazione ``app/config/config_dev.yml``. Accedendo all'ambiente ``prod``,
Symfony carica il file ``app/config/config_prod.yml``.

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

Per maggiori dettagli sugli ambienti, vedere la pagina
":ref:`ambienti e front controller <page-creation-environments>`".

Considerazioni finali
---------------------

Congratulazioni! Avete avuto il vostro primo assaggio di codice di Symfony.
Non era così difficile, vero? C'è ancora molto da esplorare, ma dovreste
già vedere come Symfony rende veramente facile implementare siti web in modo
migliore e più veloce. Se siete ansiosi di saperne di più, andate alla prossima
sezione: ":doc:`la vista <the_view>`".

.. _Composer: https://getcomposer.org/
.. _installer: https://getcomposer.org/download
.. _Twig: http://twig.sensiolabs.org/
