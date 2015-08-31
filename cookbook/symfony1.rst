.. index::
   single: symfony1

Differenze tra Symfony2 e symfony1
==================================

Il framework Symfony2 rappresenta un'importante evoluzione rispetto alla sua versione
precedente. Fortunatamente, con l'architettura MVC al suo interno, le abilità
usate per padroneggiare un progetto symfony1 continuano a essere molto importanti
per lo sviluppo con Symfony2. Certo, non c'è più ``app.yml``, ma le rotte, i controllori
e i template ci sono ancora tutti.

In questo capitolo analizzeremo le differenze tra symfony1 e Symfony2.
Come vedremo, diverse cose sono implementate in modo un po' diverso. Si imparerà
ad apprezzare tali differenze, in quanto esse promuovono in
un'applicazione Symfony2 un codice stabile, prevedibile, testabile e disaccoppiato.

Prendiamoci dunque un po' di relax, per andare da "allora" ad "adesso".

Struttura delle cartelle
------------------------

Guardando un progetto Symfony2, per esempio `Symfony2 Standard`_, si noterà
una struttura di cartelle molto diversa rispetto a symfony1. Le differenze, tuttavia,
sono in qualche modo superficiali.

La cartella ``app/``
~~~~~~~~~~~~~~~~~~~~

In symfony1, un progetto ha una o più applicazioni, ognuna delle quali risiede
nella cartella ``apps/`` (per esempio ``apps/frontend``). La configurazione
predefinita di Symfony2 è di avere un'unica applicazione, nella cartella ``app/``.
Come in symfony1, la cartella ``app/`` contiene una configurazione specifica per
quell'applicazione. Contiene inoltre cartelle di cache, log e template specifiche
dell'applicazione, come anche una classe ``Kernel`` (``AppKernel``), che è l'oggetto
di base che rappresenta l'applicazione.

Diversamente da symfony1, c'è pochissimo codice PHP nella cartella ``app/``. Questa
cartella non è pensata per ospitare moduli o file di librerie, come era in symfony1.
Invece, è semplicemente il posto in cui risiedono la configurazione e altre risorse
(template, file di traduzione).

La cartella ``src/``
~~~~~~~~~~~~~~~~~~~~

Semplicemente, il codice va messo qui. In Symfony, tutto il codice relativo
alle applicazioni risiede in un bundle (pressappoco equivalente a un plugin di symfony1)
e ogni bundle risiede, per impostazione predefinita, nella cartella ``src``. In questo
modo, la cartella ``src`` è un po' come la cartella ``plugins`` di symfony1, ma molto
più flessibile. Inoltre, mentre i *propri* bundle risiedono nella cartella ``src``, i
bundle di terze parti risiederanno nella cartella ``vendor/``.

Per avere un quadro più completo della cartella ``src/``, pensiamo a un'applicazione
symfony1. Innanzitutto, parte del codice probabilmente risiede in una o più
applicazioni. Solitamente questo include dei moduli, ma potrebbe anche includere altre
classi PHP inserite in un'applicazione. Si potrebbe anche aver creato un file
``schema.yml`` nella cartella ``config`` del progetto e costruito diversi file di modello.
Infine, per aiutarsi con alcune funzionalità comuni, si usano diversi plugin di terze
parti, che stanno nella cartella ``plugins/``.
In altre parole, il codice che guida un'applicazione risiede in molti posti
diversi.

In Symfony2, la vite è molto più semplice, perché *tutto* il codice di Symfony2 deve
risiedere in un bundle. Nel nostro ipotetico progetto symfony1, tutto il codice
*potrebbe* essere spostato in uno o più plugin (che in effetti è una buona pratica).
Ipotizzando che tutti i moduli, le classi PHP, lo schema, la configurazione delle rotte,
eccetera siano spostate in un plugin, la cartella ``plugins/`` di symfony1 sarebbe
molto simile alla cartella ``src/`` di Symfony2.

Detto semplicemente, la cartella ``src/`` è il posto in cui risiedono il codice,
le risorse, i template e quasi ogni altra cosa specifica di un progetto.

La cartella ``vendor/``
~~~~~~~~~~~~~~~~~~~~~~~

La cartella ``vendor/`` è essenzialmente equivalente alla cartella ``lib/vendor/``
in symfony1, che era la cartella convenzionale per tutte le librerie di terze
parti. Per impostazione predefinita, si troveranno le librerie di Symfony2 in
questa cartella, insieme a diverse altre librerie indipendenti, come Doctrine2,
Twig e Swiftmailer. I bundle di Symfony2 di terze parti risiedono da qualche parte
in ``vendor/``.

La cartella ``web/``
~~~~~~~~~~~~~~~~~~~~

Non è cambiato molto nella cartella ``web/``. La differenza più notevole è
l'assenza delle cartelle ``css/``, ``js/`` e ``images/``. La cosa è intenzionale.
Come il codice PHP, tutte le risorse dovrebbero risiedere all'interno di
un bundle. Con l'aiuto di un comando della console, la cartella ``Resources/public/``
di ogni bundle viene copiata o collegata alla cartella ``web/bundles/``.
Questo consente di mantenere le risorse organizzate nel bundle, ma ancora
disponibili pubblicamente. Per assicurarsi che tutti i bundle siano disponibili,
eseguire il seguente comando:

.. code-block:: bash

    $ php app/console assets:install web

.. note::

   Questo comando di Symfony2 è l'equivalente del comando ``plugin:publish-assets``
   di symfony1.

Auto-caricamento
----------------

Uno dei vantaggi dei framework moderni è il non doversi preoccupare di richiedere
i file. Utilizzando un autoloader, si può fare riferimento a qualsiasi classe
nel progetto e fidarsi che essa sia disponibile. L'auto-caricamento è
cambiato in Symfony2, per essere più universale, più veloce e indipendente
dalla pulizia della cache.

In symfony1, l'auto-caricamento era effettuato cercando nell'intero progetto la
presenza di file di classe PHP e mettendo in cache tale informazione in un
gigantesco array. Questo array diceva a symfony1 esattamente quale file
conteneva ciascuna classe. Nell'ambiente di produzione, questo causava la necessità
di dover pulire la cache quando una classe veniva aggiunta o spostata.

In Symfony2, uno strumento chiamato `Composer`_ gestisce questo processo.
L'idea dietro all'autoloader è semplice: il nome di una classe (incluso lo
spazio dei nomi) deve corrispondere al percorso del file che contiene tale classe.
Si prenda come esempio ``FrameworkExtraBundle``, nella Standard Edition di
Symfony2::

    namespace Sensio\Bundle\FrameworkExtraBundle;

    use Symfony\Component\HttpKernel\Bundle\Bundle;
    // ...

    class SensioFrameworkExtraBundle extends Bundle
    {
        // ...
    }

Il file stesso risiede in
``vendor/sensio/framework-extra-bundle/Sensio/Bundle/FrameworkExtraBundle/SensioFrameworkExtraBundle.php``.
Come si può vedere, la locazione del file segue lo spazio dei nomi della
classe. La prima parte è uguale al nome del pacchetto di SensioFrameworkExtraBundle.

Lo spazio dei nomi, ``Sensio\Bundle\FrameworkExtraBundle``, e il nome del pacchetto,
``sensio/framework-extra-bundle``, dicono che la cartella in cui il file
dovrebbe trovarsi
(``vendor/sensio/framework-extra-bundle/Sensio/Bundle/FrameworkExtraBundle/``).
Composer quindi può cercare il file nella specifica posizione e caricarlo molto
velocemente.

Se il file *non* risiede in questa esatta locazione, si riceverà un errore
``Class "Sensio\Bundle\FrameworkExtraBundle\SensioFrameworkExtraBundle" does not exist.``.
In Symfony2, un errore "class does not exist" vuol dire che lo spazio dei nomi della
classe e la locazione fisica del file non corrispondono. Fondamentalmente, Symfony2
cerca in una specifica locazione quella classe, ma quella locazione non esiste
(oppure contiene una classe diversa). Per poter auto-caricare una classe, non
**è mai necessario pulire la cache** in Symfony2.

Come già accennato, per poter far funzionare l'autoloader, esso deve sapere che
lo spazio dei nomi ``Sensio`` risiede nella cartella ``vendor/bundles`` e che, per esempio,
lo spazio dei nomi ``Doctrine`` risiede nella cartella ``vendor/doctrine/orm/lib/``.
Questa mappatura è interamente controllata da Composer. Ogni
libreria di terze parti caricata tramite composer ha le sue impostazioni definite
e Composer si occupa di tutto al posto nostro.

Per poter funzionare, tutte le librerie di terze parti usate nel progetto devono
essere definite nel file ``composer.json``.

Se si dà un'occhiata a ``HelloController`` nella Standard Edition di Symfony2, si
vedrà che esso risiede nello spazio dei nomi ``Acme\DemoBundle\Controller``. Anche qui,
``AcmeBundle`` non è definito nel file ``composer.json``. I file vengono comunque caricati
automaticamente. Questo perché si può dire a Composer di auto-caricare i file da
cartelle specifiche, senza definire dipendenze:

.. code-block:: json

    "autoload": {
        "psr-0": { "": "src/" }
    }

Questo vuol dire che se una classe non viene trovata nella cartella ``vendor``, Composer
cercherà nella cartella ``src``, prima di lanciare un'eccezione "class does not exist".
Si può approfondire la configurazione dell'auto-caricamento di Composer nella
`documentazione di Composer`_

Uso della console
-----------------

In symfony1, la console è nella cartella radice del progetto ed è chiamata
 ``symfony``:

.. code-block:: bash

    $ php symfony

In Symfony2, la console è ora nella sotto-cartella ``app`` ed è chiamata
``console``:

.. code-block:: bash

    $ php app/console

Applicazioni
------------

In un progetto basato su symfony 1, è frequente avere diverse applicazioni: per
esempio, una per il frontend e una per il backend.

In un progetto basato su Symfony2, occorre creare una sola applicazione
(un'applicazione blog, un'applicazione intranet, ...). La maggior parte delle
volte, se si vuole creare una seconda applicazione, sarebbe meglio creare
un altro progetto e condividere alcuni bundle tra essi.

Se poi si ha bisogno di separare le caratteristiche di frontend e di backend
di alcuni bundle, creare dei sotto-spazi per controller, delle sotto-cartelle
per i template, configurazioni semantiche diverse, configurazioni di rotte
separate e così via.

Ovviamente non c'è nulla di sbagliato ad avere più di un'applicazione nel
progetto, questa scelta è lasciata allo sviluppatore. Una seconda applicazione
vorrebbe dire una nuova cartella, per esempio ``app2/``, con la stessa struttura di base della cartella ``app/``.

.. tip::

   Leggere la definizione di :term:`Progetto`, :term:`Applicazione` e
   :term:`Bundle` nel glossario.

Bundle e plugin
---------------

In un progetto symfony1, un plugin può contenere configurazioni, moduli, librerie PHP,
risorse e qualsiasi altra cosa relativa al progetto. In Symfony2,
l'idea di plugin è stata rimpiazzata con quella di "bundle". Un bundle è ancora più
potente di un plugin, perché il nucleo stesso del framework Symfony2 è costituito
da una serie di bundle. In Symfony2, i bundle sono cittadini di prima classe e sono
così flessibili che il nucleo stesso è un bundle.

In symfony1, un plugin deve essere abilitato nella classe
``ProjectConfiguration``::

    // config/ProjectConfiguration.class.php
    public function setup()
    {
        // qui ci sono alcuni plugin
        $this->enableAllPluginsExcept(array(...));
    }

In Symfony2, i bundle sono attivati nel kernel dell'applicazione::

    // app/AppKernel.php
    public function registerBundles()
    {
        $bundles = array(
            new Symfony\Bundle\FrameworkBundle\FrameworkBundle(),
            new Symfony\Bundle\TwigBundle\TwigBundle(),
            ...,
            new Acme\DemoBundle\AcmeDemoBundle(),
        );

        return $bundles;
    }

Rotte (``routing.yml``) e configurazione (``config.yml``)
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

In symfony1, i file di configurazione ``routing.yml`` e ``app.yml`` sono
caricati automaticamente all'interno di un plugin. In Symfony2, le rotte e le
configurazioni dell'applicazioni all'interno di un bundle vanno incluse
a mano. Per esempio, per includere le rotte di un bundle chiamato ``AcmeDemoBundle``,
si può fare nel seguente modo:

.. configuration-block::

    .. code-block:: yaml

        # app/config/routing.yml
        _hello:
            resource: "@AcmeDemoBundle/Resources/config/routing.yml"

    .. code-block:: xml

        <!-- app/config/routing.yml -->
        <?xml version="1.0" encoding="UTF-8" ?>

        <routes xmlns="http://symfony.com/schema/routing"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://symfony.com/schema/routing http://symfony.com/schema/routing/routing-1.0.xsd">

            <import resource="@AcmeDemoBundle/Resources/config/routing.xml" />
        </routes>

    .. code-block:: php

        // app/config/routing.php
        use Symfony\Component\Routing\RouteCollection;

        $collection = new RouteCollection();
        $collection->addCollection($loader->import("@AcmeHelloBundle/Resources/config/routing.php"));

        return $collection;

Questo caricherà le rotte trovate nel file ``Resources/config/routing.yml`` di
``AcmeDemoBundle``. Il nome ``@AcmeDemoBundle`` è una sintassi abbreviata, risolta
internamente con il percorso completo di quel bundle.

Si può usare la stessa strategia per portare una configurazione da un bundle:

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        imports:
            - { resource: "@AcmeDemoBundle/Resources/config/config.yml" }

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <imports>
            <import resource="@AcmeDemoBundle/Resources/config/config.xml" />
        </imports>

    .. code-block:: php

        // app/config/config.php
        $this->import('@AcmeDemoBundle/Resources/config/config.php')

In Symfony2, la configurazione è un po' come ``app.yml`` in symfony1, ma più
sistematica. Con ``app.yml``, si poteva semplicemente creare le voci volute.
Per impostazione predefinita, queste voci erano prive di significato ed era
lasciato allo sviluppatore il compito di usarle nell'applicazione:

.. code-block:: yaml

    # un file app.yml di symfony1
    all:
      email:
        from_address:  pippo.pluto@example.com

In Symfony2, si possono anche creare voci arbitrarie sotto la voce ``parameters``
della configurazione:

.. configuration-block::

    .. code-block:: yaml

        parameters:
            email.from_address: pippo.pluto@example.com

    .. code-block:: xml

        <parameters>
            <parameter key="email.from_address">pippo.pluto@example.com</parameter>
        </parameters>

    .. code-block:: php

        $container->setParameter('email.from_address', 'pippo.pluto@example.com');

Si può ora accedervi da un controllore, per esempio::

    public function helloAction($name)
    {
        $fromAddress = $this->container->getParameter('email.from_address');
    }

In realtà, la configurazione di Symfony2 è molto più potente ed è usata principalmente
per configurare oggetti da poter usare. Per maggiori informazioni, vedere il capitolo
intitolato ":doc:`/book/service_container`".

.. _`Composer`: http://getcomposer.org
.. _`Symfony2 Standard`: https://github.com/symfony/symfony-standard
.. _`documentazione di Composer`: http://getcomposer.org/doc/04-schema.md#autoload
