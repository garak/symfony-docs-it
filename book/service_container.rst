.. index::
   single: Contenitore di servizi
   single: Dependency Injection Container

Contenitore di servizi
======================

Una moderna applicazione PHP è piena di oggetti. Un oggetto può facilitare la
consegna dei messaggi di posta elettronica, mentre un altro può consentire di salvare le informazioni
in una base dati. Nell'applicazione, è possibile creare un oggetto che gestisce
l'inventario dei prodotti, o un altro oggetto che elabora i dati da un'API di terze parti.
Il punto è che una moderna applicazione fa molte cose ed è organizzata
in molti oggetti che gestiscono ogni attività.

In questo capitolo si parlerà di un oggetto speciale PHP presente in Symfony2 che aiuta
a istanziare, organizzare e recuperare i tanti oggetti della propria applicazione.
Questo oggetto, chiamato contenitore di servizi, permetterà di standardizzare e
centralizzare il modo in cui sono costruiti gli oggetti nell'applicazione. Il contenitore
rende la vita più facile, è super veloce ed evidenzia un'architettura che
promuove codice riusabile e disaccoppiato. E poiché tutte le classi del nucleo di Symfony2
utilizzano il contenitore, si apprenderà come estendere, configurare e usare qualsiasi oggetto
in Symfony2. In gran parte, il contenitore dei servizi è il più grande contributore
riguardo la velocità e l'estensibilità di Symfony2.

Infine, la configurazione e l'utilizzo del contenitore di servizi è semplice. Entro la fine
di questo capitolo, si sarà in grado di creare i propri oggetti attraverso il
contenitore e personalizzare gli oggetti da un bundle di terze parti. Si inizierà a
scrivere codice che è più riutilizzabile, testabile e disaccoppiato, semplicemente perché
il contenitore di servizi consente di scrivere facilmente del buon codice.

.. tip::

    Per un approfondimento successivo alla lettura di questo capitolo, dare un'occhiata
    alla :doc:`documentazione del componente Dependency Injection</components/dependency_injection/introduction>`.

.. index::
   single: Contenitore di servizi; Cos'è un servizio?

Cos'è un servizio?
------------------

In parole povere, un :term:`servizio` è un qualsiasi oggetto PHP che esegue una sorta di
compito "globale". È un nome volutamente generico utilizzato in informatica
per descrivere un oggetto che è stato creato per uno scopo specifico (ad esempio spedire
email). Ogni servizio è utilizzato in tutta l'applicazione ogni volta che si ha bisogno
delle funzionalità specifiche che fornisce. Non bisogna fare nulla di speciale
per creare un servizio: è sufficiente scrivere una classe PHP con del codice che realizza
un compito specifico. Congratulazioni, si è appena creato un servizio!

.. note::

    Come regola generale, un oggetto PHP è un servizio se viene utilizzato a livello globale
    nell'applicazione. Un singolo servizio ``Mailer`` è usato globalmente per inviare
    messaggi email mentre i molti oggetti ``Message`` che spedisce
    *non* sono servizi. Allo stesso modo, un oggetto ``Product`` non è un servizio,
    ma un oggetto che persiste oggetti ``Product`` su una base dati *è* un servizio.

Qual è il discorso allora? Il vantaggio dei "servizi" è
che si comincia a pensare di separare ogni "pezzo di funzionalità" dell'applicazione
in una serie di servizi. Dal momento che ogni servizio fa solo un lavoro,
si può facilmente accedere a ogni servizio e utilizzare le sue funzionalità ovunque
ce ne sia bisogno. Ogni servizio può anche essere più facilmente testato e configurato essendo
separato dalle altre funzionalità dell'applicazione. Questa idea
si chiama `architettura orientata ai servizi`_ e non riguarda solo Symfony2
o il PHP. Strutturare la propria applicazione con una serie di classi indipendenti
di servizi è una nota best practice della programmazione a oggetti. Queste conoscenze
sono fondamentali per essere un buon sviluppatore in quasi tutti i linguaggi.

.. index::
   single: Contenitore di servizi; Cos'è?

Cos'è un contenitore di servizi?
--------------------------------

Un :term:`contenitore di servizi` (o *contenitore di dependency injection*) è semplicemente
un oggetto PHP che gestisce l'istanza di servizi (cioè gli oggetti).

Per esempio, supponiamo di avere una semplice classe PHP che spedisce messaggi email.
Senza un contenitore di servizi, bisogna creare manualmente l'oggetto ogni volta che
se ne ha bisogno::

    use Acme\HelloBundle\Mailer;

    $mailer = new Mailer('sendmail');
    $mailer->send('ryan@foobar.net', ...);

Questo è abbastanza facile. La classe immaginaria ``Mailer`` permette di configurare
il metodo utilizzato per inviare i messaggi email (per esempio ``sendmail``, ``smtp``, ecc).
Ma cosa succederebbe se volessimo utilizzare il servizio mailer da qualche altra parte? Certamente
non si vorrebbe ripetere la configurazione del mailer *ogni* volta che si ha bisogno
dell'oggetto ``Mailer``. Cosa succederebbe se avessimo bisogno di cambiare il ``transport`` da
``sendmail`` a ``smtp`` in ogni punto dell'applicazione? Avremo bisogno di cercare
ogni posto in cui si crea un servizio ``Mailer`` e cambiarlo.

.. index::
   single: Contenitore di servizi; Configurare i servizi

Creare/Configurare servizi nel contenitore
------------------------------------------

Una soluzione migliore è quella di lasciare che il contenitore di servizi crei l'oggetto ``Mailer``
per noi. Affinché questo funzioni, bisogna *insegnare* al contenitore come
creare il servizio ``Mailer``. Questo viene fatto tramite la configurazione, che può
essere specificata in YAML, XML o PHP:

.. include includes/_service_container_my_mailer.rst.inc

.. note::

    Durante l'inizializzazione di Symfony2, viene costruito il contenitore di servizi utilizzando
    la configurazione dell'applicazione (per impostazione predefinita ``app/config/config.yml``). Il
    file esatto che viene caricato è indicato dal metodo ``AppKernel::registerContainerConfiguration()``,
    che carica un file di configurazione specifico per l'ambiente (ad esempio
    ``config_dev.yml`` per l'ambiente ``dev`` o ``config_prod.yml``
    per ``prod``).

Un'istanza dell'oggetto ``Acme\HelloBundle\Mailer`` è ora disponibile tramite
il contenitore di servizio. Il contenitore è disponibile in qualsiasi normale controllore di Symfony2
in cui è possibile accedere ai servizi del contenitore  attraverso il
metodo scorciatoia ``get()``::

    class HelloController extends Controller
    {
        // ...

        public function sendEmailAction()
        {
            // ...
            $mailer = $this->get('my_mailer');
            $mailer->send('ryan@foobar.net', ...);
        }
    }

Quando si chiede il servizio ``my_mailer`` del contenitore, il contenitore
costruisce l'oggetto e lo restituisce. Questo è un altro grande vantaggio che
si ha utilizzando il contenitore di servizi. Questo significa che un servizio non è *mai* costruito fino
a che non ce n'è bisogno. Se si definisce un servizio e non lo si usa mai su una richiesta, il servizio
non verrà mai creato. Ciò consente di risparmiare memoria e aumentare la velocità dell'applicazione.
Questo significa anche che c'è un calo di prestazioni basso o inesistente quando si definiscono
molti servizi. I servizi che non vengono mai utilizzati non sono mai costruite.

Come bonus aggiuntivo, il servizio ``Mailer`` è creato una sola volta e
ogni volta che si chiede per il servizio viene restituita la stessa istanza. Questo è quasi sempre
il comportamento di cui si ha bisogno (è più flessibile e potente), ma si imparerà
come configurare un servizio che ha istanze multiple nella ricetta
":doc:`/cookbook/service_container/scopes`".

.. note::

    In questo esempio, il controllore estende quello base di Symfony, il quale fornisce
    accesso al contenitore di servizi. Si può quindi usare il metodo
    ``get`` per recuperare il servizio ``my_mailer`` dal
    contenitore. Si possono anche definire i :doc:`controllori come servizi</cookbook/controller/service>`.
    Questo è un po' più avanzato e non sempre necessario, ma consente di iniettare solo
    i servizi che serviranno nel controllore.

.. _book-service-container-parameters:

I parametri del servizio
------------------------

La creazione di nuovi servizi (cioè oggetti) attraverso il contenitore è abbastanza
semplice. Con i parametri si possono definire servizi più organizzati e flessibili:

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        parameters:
            my_mailer.class:      Acme\HelloBundle\Mailer
            my_mailer.transport:  sendmail

        services:
            my_mailer:
                class:        "%my_mailer.class%"
                arguments:    ["%my_mailer.transport%"]

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <?xml version="1.0" encoding="UTF-8" ?>
        <container xmlns="http://symfony.com/schema/dic/services"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://symfony.com/schema/dic/services http://symfony.com/schema/dic/services/services-1.0.xsd">

            <parameters>
                <parameter key="my_mailer.class">Acme\HelloBundle\Mailer</parameter>
                <parameter key="my_mailer.transport">sendmail</parameter>
            </parameters>

            <services>
                <service id="my_mailer" class="%my_mailer.class%">
                    <argument>%my_mailer.transport%</argument>
                </service>
            </services>
        </container>

    .. code-block:: php

        // app/config/config.php
        use Symfony\Component\DependencyInjection\Definition;

        $container->setParameter('my_mailer.class', 'Acme\HelloBundle\Mailer');
        $container->setParameter('my_mailer.transport', 'sendmail');

        $container->setDefinition('my_mailer', new Definition(
            '%my_mailer.class%',
            array('%my_mailer.transport%')
        ));

Il risultato finale è esattamente lo stesso di prima, la differenza è solo nel
*come* è stato definito il servizio. Circondando le stringhe ``my_mailer.class`` e
``my_mailer.transport`` con il segno di percentuale (``%``), il contenitore sa
di dover cercare per parametri con questi nomi. Quando il contenitore è costruito,
cerca il valore di ogni parametro e lo usa nella definizione del servizio.

.. note::

    Se si vuole usare una stringa che inizi con il simbolo ``@`` come valore di un
    parametro (p.e. una password) in un file yaml, occorre un escape tramite
    un ulteriore simbolo ``@`` (si applica solo al formato YAML):

    .. code-block:: yaml

        # app/config/parameters.yml
        parameters:
            # Questo valore sarà analizzato come "@passwordsicura"
            mailer_password: "@@passwordsicura"

.. note::

    Il simbolo di percentuale dentro a un parametro, come parte della stringa, deve
    subire un escape tramite un ulteriore simbolo di percentuale:

    .. code-block:: xml

        <argument type="string">http://symfony.com/?pippo=%%s&pluto=%%d</argument>

Lo scopo dei parametri è quello di inserire informazioni dei servizi. Naturalmente
non c'è nulla di sbagliato a definire il servizio senza l'uso di parametri.
I parametri, tuttavia, hanno diversi vantaggi:

* separazione e organizzazione di tutte le "opzioni" del servizio sotto un'unica
  chiave ``parameters``;

* i valori dei parametri possono essere utilizzati in molteplici definizioni di servizi;

* la creazione di un servizio in un bundle (lo mostreremo a breve), usando i parametri
  consente al servizio di essere facilmente personalizzabile nell'applicazione.

La scelta di usare o non usare i parametri è personale. I bundle
di alta qualità di terze parti li utilizzeranno *sempre*, perché rendono i servizi
memorizzati nel contenitore più configurabili. Per i servizi della propria applicazione,
tuttavia, potrebbe non essere necessaria la flessibilità dei parametri.

Parametri array
~~~~~~~~~~~~~~~

I parametri possono anche contenere array. Vedere :ref:`component-di-parameters-array`.

Importare altre risorse di configurazione del contenitore
---------------------------------------------------------

.. tip::

    In questa sezione, si farà riferimento ai file di configurazione del servizio come *risorse*.
    Questo per sottolineare il fatto che, mentre la maggior parte delle risorse di configurazione
    saranno file (ad esempio YAML, XML, PHP), Symfony2 è così flessibile che la configurazione
    potrebbe essere caricata da qualunque parte (per esempio in una base dati o tramite un
    servizio web esterno).

Il contenitore dei servizi è costruito utilizzando una singola risorsa di configurazione
(per impostazione predefinita ``app/config/config.yml``). Tutte le altre configurazioni di servizi
(comprese le configurazioni del nucleo di Symfony2 e dei bundle di terze parti) devono
essere importate da dentro questo file in un modo o nell'altro. Questo dà una assoluta
flessibilità sui servizi dell'applicazione.

La configurazione esterna di servizi può essere importata in due modi differenti. Il primo,
e più comune, è la direttiva ``imports``. Nella sezione seguente, si introdurrà il
secondo metodo, che è il metodo più flessibile e privilegiato per importare
la configurazione di servizi in bundle di terze parti.

.. index::
   single: Contenitore di servizi; imports

.. _service-container-imports-directive:

Importare la configurazione con ``imports``
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Finora, si è messo la definizione di contenitore del servizio ``my_mailer`` direttamente
nel file di configurazione dell'applicazione (ad esempio ``app/config/config.yml``).
Naturalmente, poiché la classe ``Mailer`` stessa vive all'interno di ``AcmeHelloBundle``,
ha più senso mettere la definizione ``my_mailer`` del contenitore dentro il
bundle stesso.

In primo luogo, spostare la definizione ``my_mailer`` del contenitore, in un nuovo file risorse
del contenitore in ``AcmeHelloBundle``. Se le cartelle ``Resources`` o ``Resources/config``
non esistono, crearle.

.. configuration-block::

    .. code-block:: yaml

        # src/Acme/HelloBundle/Resources/config/services.yml
        parameters:
            my_mailer.class:      Acme\HelloBundle\Mailer
            my_mailer.transport:  sendmail

        services:
            my_mailer:
                class:        "%my_mailer.class%"
                arguments:    ["%my_mailer.transport%"]

    .. code-block:: xml

        <!-- src/Acme/HelloBundle/Resources/config/services.xml -->
        <?xml version="1.0" encoding="UTF-8" ?>
        <container xmlns="http://symfony.com/schema/dic/services"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://symfony.com/schema/dic/services http://symfony.com/schema/dic/services/services-1.0.xsd">

            <parameters>
                <parameter key="my_mailer.class">Acme\HelloBundle\Mailer</parameter>
                <parameter key="my_mailer.transport">sendmail</parameter>
            </parameters>

            <services>
                <service id="my_mailer" class="%my_mailer.class%">
                    <argument>%my_mailer.transport%</argument>
                </service>
            </services>
        </container>

    .. code-block:: php

        // src/Acme/HelloBundle/Resources/config/services.php
        use Symfony\Component\DependencyInjection\Definition;

        $container->setParameter('my_mailer.class', 'Acme\HelloBundle\Mailer');
        $container->setParameter('my_mailer.transport', 'sendmail');

        $container->setDefinition('my_mailer', new Definition(
            '%my_mailer.class%',
            array('%my_mailer.transport%')
        ));

Non è cambiata la definizione, solo la sua posizione. Naturalmente il servizio
contenitore non conosce il nuovo file di risorse. Fortunatamente, si può
facilmente importare il file risorse utilizzando la chiave ``imports`` nella configurazione
dell'applicazione.

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        imports:
            - { resource: "@AcmeHelloBundle/Resources/config/services.yml" }

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <?xml version="1.0" encoding="UTF-8" ?>
        <container xmlns="http://symfony.com/schema/dic/services"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://symfony.com/schema/dic/services http://symfony.com/schema/dic/services/services-1.0.xsd">

            <imports>
                <import resource="@AcmeHelloBundle/Resources/config/services.xml"/>
            </imports>
        </container>

    .. code-block:: php

        // app/config/config.php
        $this->import('@AcmeHelloBundle/Resources/config/services.php');

La direttiva ``imports`` consente all'applicazione  di includere risorse di configurazione per il
contenitore di servizi da qualsiasi altro posto (in genere da bundle).
La locazione ``resource``, per i file, è il percorso assoluto al file
risorse. La speciale sintassi ``@AcmeHello`` risolve il percorso della cartella del
bundle ``AcmeHelloBundle``. Questo aiuta a specificare il percorso alla risorsa
senza preoccuparsi in seguito, se si sposta ``AcmeHelloBundle`` in una cartella
diversa.

.. index::
   single: Contenitore di servizi; Configurazione delle estensioni

.. _service-container-extension-configuration:

Importare la configurazione attraverso estensioni del contenitore
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Quando si sviluppa in Symfony2, si usa spesso la direttiva ``imports``
per importare la configurazione del contenitore dai bundle che sono stati creati appositamente
per l'applicazione. Le configurazioni dei contenitori di bundle di terze parti, includendo
i servizi del nucleo di Symfony2, di solito sono caricati utilizzando un altro metodo che è più
flessibile e facile da configurare nell'applicazione.

Ecco come funziona. Internamente, ogni bundle definisce i propri servizi in modo
molto simile a come si è visto finora. Un bundle utilizza uno o più file
di configurazione delle risorse (di solito XML) per specificare i parametri e i servizi del
bundle. Tuttavia, invece di importare ciascuna di queste risorse direttamente dalla
configurazione dell'applicazione utilizzando la direttiva ``imports``, si può semplicemente
richiamare una *estensione del contenitore di servizi* all'interno del bundle che fa il lavoro
per noi. Un'estensione del contenitore dei servizi è una classe PHP creata dall'autore del bundle
con lo scopo di realizzare due cose:

* importare tutte le risorse del contenitore dei servizi necessarie per configurare i servizi per
  il bundle;

* fornire una semplice configurazione semantica in modo che il bundle possa
  essere configurato senza interagire con i parametri "piatti" della configurazione del contenitore
  dei servizi del bundle.

In altre parole, una estensione dei contenitore dei servizi configura i servizi del
il bundle per lo sviluppatore. E, come si vedrà tra poco, l'estensione fornisce
un'interfaccia comoda e ad alto livello per configurare il bundle.

Si prenda FrameworkBundle, il bundle del nucleo del framework Symfony2, come
esempio. La presenza del seguente codice nella configurazione dell'applicazione
invoca l'estensione del contenitore dei servizi all'interno di FrameworkBundle:

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        framework:
            secret:          xxxxxxxxxx
            form:            true
            csrf_protection: true
            router:        { resource: "%kernel.root_dir%/config/routing.yml" }
            # ...

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <?xml version="1.0" encoding="UTF-8" ?>
        <container xmlns="http://symfony.com/schema/dic/services"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xmlns:framework="http://symfony.com/schema/dic/symfony"
            xsi:schemaLocation="http://symfony.com/schema/dic/services http://symfony.com/schema/dic/services/services-1.0.xsd
                                http://symfony.com/schema/dic/symfony http://symfony.com/schema/dic/symfony/symfony-1.0.xsd">

            <framework:config secret="xxxxxxxxxx">
                <framework:form />
                <framework:csrf-protection />
                <framework:router resource="%kernel.root_dir%/config/routing.xml" />
                <!-- ... -->
            </framework>
        </container>

    .. code-block:: php

        // app/config/config.php
        $container->loadFromExtension('framework', array(
            'secret'          => 'xxxxxxxxxx',
            'form'            => array(),
            'csrf-protection' => array(),
            'router'          => array(
                'resource' => '%kernel.root_dir%/config/routing.php',
            ),

            // ...
        ));

Quando viene analizzata la configurazione, il contenitore cerca un'estensione che
sia in grado di gestire la direttiva di configurazione ``framework``. L'estensione in questione,
che si trova in FrameworkBundle, viene invocata e la configurazione del servizio
per FrameworkBundle viene caricata. Se si rimuove del tutto la chiave ``framework``
dal file di configurazione dell'applicazione, i servizi del nucleo di Symfony2
non vengono caricati. Il punto è che è tutto sotto controllo: il framework Symfony2
non contiene nessuna magia e non esegue nessuna azione su cui non si abbia
il controllo.

Naturalmente è possibile fare molto di più della semplice "attivazione" dell'estensione
del contenitore dei servizi di ``FrameworkBundle``. Ogni estensione consente facilmente
di personalizzare il bundle, senza preoccuparsi di come i servizi interni siano
definiti.

In questo caso, l'estensione consente di personalizzare la configurazione di ``error_handler``,
``csrf_protection``, ``router`` e di molte altre. Internamente,
FrameworkBundle usa le opzioni qui specificate per definire e configurare
i servizi a esso specifici. Il bundle si occupa di creare tutte i necessari
``parameters`` e ``services`` per il contenitore dei servizi, pur consentendo
di personalizzare facilmente gran parte della configurazione. Come bonus aggiuntivo, la maggior parte
delle estensioni dei contenitori di servizi sono anche sufficientemente intelligenti da eseguire la validazione,
notificando le opzioni mancanti o con un tipo di dato sbagliato.

Durante l'installazione o la configurazione di un bundle, consultare la documentazione del bundle
per vedere come devono essere installati e configurati i suoi servizi. Le opzioni
disponibili per i  bundle del nucleo si possono trovare all'interno della :doc:`guida di riferimento </reference/index>`.

.. note::

   Nativamente, il contenitore dei servizi riconosce solo le direttive ``parameters``,
   ``services`` e ``imports``. Ogni altra direttiva
   è gestita dall'estensione del contenitore dei servizi.

Se si vogliono esporre in modo amichevole le configurazioni dei propri bundle, leggere la ricetta
":doc:`/cookbook/bundles/extension`".

.. index::
   single: Contenitore di servizi; Referenziare i servizi

Referenziare (iniettare) servizi
--------------------------------

Finora, il servizio ``my_mailer`` è semplice: accetta un solo parametro
nel suo costruttore, che è facilmente configurabile. Come si vedrà, la potenza
reale del contenitore viene fuori quando è necessario creare un servizio che
dipende da uno o più altri servizi nel contenitore.

Cominciamo con un esempio. Supponiamo di avere un nuovo servizio, ``NewsletterManager``,
che aiuta a gestire la preparazione e la spedizione di un messaggio email a
un insieme di indirizzi. Naturalmente il servizio ``my_mailer`` è già
capace a inviare messaggi email, quindi verrà usato all'interno di ``NewsletterManager``
per gestire la spedizione effettiva dei messaggi. Questa classe potrebbe essere
qualcosa del genere::

    // src/Acme/HelloBundle/Newsletter/NewsletterManager.php
    namespace Acme\HelloBundle\Newsletter;

    use Acme\HelloBundle\Mailer;

    class NewsletterManager
    {
        protected $mailer;

        public function __construct(Mailer $mailer)
        {
            $this->mailer = $mailer;
        }

        // ...
    }

Senza utilizzare il contenitore di servizi, si può creare abbastanza facilmente
un nuovo ``NewsletterManager`` dentro a un controllore::

    use Acme\HelloBundle\Newsletter\NewsletterManager;

    // ...

    public function sendNewsletterAction()
    {
        $mailer = $this->get('my_mailer');
        $newsletter = new NewsletterManager($mailer);
        // ...
    }

Questo approccio va bene, ma cosa succede se più avanti si decide che la classe ``NewsletterManager``
ha bisogno di un secondo o terzo parametro nel costruttore? Che cosa succede se si decide di
rifattorizzare il codice e rinominare la classe? In entrambi i casi si avrà bisogno di cercare ogni
posto in cui viene istanziata ``NewsletterManager`` e fare le modifiche. Naturalmente,
il contenitore dei servizi fornisce una soluzione molto migliore:

.. configuration-block::

    .. code-block:: yaml

        # src/Acme/HelloBundle/Resources/config/services.yml
        parameters:
            # ...
            newsletter_manager.class: Acme\HelloBundle\Newsletter\NewsletterManager

        services:
            my_mailer:
                # ...
            newsletter_manager:
                class:     "%newsletter_manager.class%"
                arguments: ["@my_mailer"]

    .. code-block:: xml

        <!-- src/Acme/HelloBundle/Resources/config/services.xml -->
        <?xml version="1.0" encoding="UTF-8" ?>
        <container xmlns="http://symfony.com/schema/dic/services"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://symfony.com/schema/dic/services http://symfony.com/schema/dic/services/services-1.0.xsd">

            <parameters>
                <!-- ... -->
                <parameter key="newsletter_manager.class">Acme\HelloBundle\Newsletter\NewsletterManager</parameter>
            </parameters>

            <services>
                <service id="my_mailer" ...>
                <!-- ... -->
                </service>
                <service id="newsletter_manager" class="%newsletter_manager.class%">
                    <argument type="service" id="my_mailer"/>
                </service>
            </services>
        </container>

    .. code-block:: php

        // src/Acme/HelloBundle/Resources/config/services.php
        use Symfony\Component\DependencyInjection\Definition;
        use Symfony\Component\DependencyInjection\Reference;

        // ...
        $container->setParameter(
            'newsletter_manager.class',
            'Acme\HelloBundle\Newsletter\NewsletterManager'
        );

        $container->setDefinition('my_mailer', ...);
        $container->setDefinition('newsletter_manager', new Definition(
            '%newsletter_manager.class%',
            array(new Reference('my_mailer'))
        ));

In YAML, la sintassi speciale ``@my_mailer`` dice al contenitore di cercare
un servizio chiamato ``my_mailer`` e di passare l'oggetto nel costruttore
di ``NewsletterManager``. In questo caso, tuttavia, il servizio specificato ``my_mailer``
deve esistere. In caso contrario, verrà lanciata un'eccezione. È possibile contrassegnare le proprie
dipendenze come opzionali (sarà discusso nella prossima sezione).

L'utilizzo di riferimenti è uno strumento molto potente che permette di creare classi
di servizi indipendenti con dipendenze ben definite. In questo esempio, il servizio ``newsletter_manager``
ha bisogno del servizio ``my_mailer`` per poter funzionare. Quando si definisce
questa dipendenza nel contenitore dei servizi, il contenitore si prende cura di tutto
il lavoro di istanziare degli oggetti.

.. _book-services-expressions:

Usare Expression Language
~~~~~~~~~~~~~~~~~~~~~~~~~

.. versionadded:: 2.4
    La funzionalità Expression Language è stata introdotta in Symfony 2.4.

Il contenitore di servizi supporta anche un'"espressione", che consente di iniettare
valori molto specifici in un servizio.

Per esempio, su supponga di avere un terzo servizio (non mostrato qui), chiamato ``mailer_configuration``,
che ha un metodo ``getMailerMethod()``, che restituisce una stringa
come ``sendmail`` a seconda di una qualche configurazione. Si ricordi che il primo parametro del
servizio ``my_mailer`` è la semplice stringa ``sendmail``:

.. include includes/_service_container_my_mailer.rst.inc

Invece di scrivere direttamente la stringa, come si può ottenere tale valore da ``getMailerMethod()``
del servizio ``mailer_configuration``? Un possibile modo consiste nell'usare un'espressione:

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        services:
            my_mailer:
                class:        Acme\HelloBundle\Mailer
                arguments:    ["@=service('mailer_configuration').getMailerMethod()"]

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <?xml version="1.0" encoding="UTF-8" ?>
        <container xmlns="http://symfony.com/schema/dic/services"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://symfony.com/schema/dic/services
                http://symfony.com/schema/dic/services/services-1.0.xsd"
            >

            <services>
                <service id="my_mailer" class="Acme\HelloBundle\Mailer">
                    <argument type="expression">service('mailer_configuration').getMailerMethod()</argument>
                </service>
            </services>
        </container>

    .. code-block:: php

        // app/config/config.php
        use Symfony\Component\DependencyInjection\Definition;
        use Symfony\Component\ExpressionLanguage\Expression;

        $container->setDefinition('my_mailer', new Definition(
            'Acme\HelloBundle\Mailer',
            array(new Expression('service("mailer_configuration").getMailerMethod()'))
        ));

Per approfondire la sintassi di Expression Language, vedere :doc:`/components/expression_language/syntax`.

In questo contesto, si ha accesso a due funzioni:

* ``service`` - restituisce un servizio dato (vedere l'esempio precedente);
* ``parameter`` - restituisce un parametro specifico (la sintassi è proprio come ``service``)

Si ha anche accesso a :class:`Symfony\\Component\\DependencyInjection\\ContainerBuilder`,
tramite una variabile ``container``. Ecco un altro esempio:

.. configuration-block::

    .. code-block:: yaml

        services:
            my_mailer:
                class:     Acme\HelloBundle\Mailer
                arguments: ["@=container.hasParameter('un_param') ? parameter('un_param') : 'valore_predefinito'"]

    .. code-block:: xml

        <?xml version="1.0" encoding="UTF-8" ?>
        <container xmlns="http://symfony.com/schema/dic/services"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://symfony.com/schema/dic/services
                http://symfony.com/schema/dic/services/services-1.0.xsd"
            >

            <services>
                <service id="my_mailer" class="Acme\HelloBundle\Mailer">
                    <argument type="expression">@=container.hasParameter('un_param') ? parameter('un_param') : 'valore_predefinito'</argument>
                </service>
            </services>
        </container>

    .. code-block:: php

        use Symfony\Component\DependencyInjection\Definition;
        use Symfony\Component\ExpressionLanguage\Expression;

        $container->setDefinition('my_mailer', new Definition(
            'Acme\HelloBundle\Mailer',
            array(new Expression(
                "@=container.hasParameter('un_param') ? parameter('un_param') : 'valore_predefinito'"
            ))
        ));

Si possono usare espressioni in ``arguments``, ``properties``, come parametri con
``configurator`` e come parametri di ``calls`` (chiamate di metodi).

Dipendenze opzionali: iniettare i setter
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Iniettare dipendenze nel costruttore è un eccellente modo
per essere sicuri che la dipendenza sia disponibile per l'uso. Se per una classe
si hanno dipendenze opzionali, allora l'"iniezione dei setter" può essere una scelta migliore.
Significa iniettare la dipendenza utilizzando una chiamata di metodo al posto del
costruttore. La classe sarà simile a questa::

    namespace Acme\HelloBundle\Newsletter;

    use Acme\HelloBundle\Mailer;

    class NewsletterManager
    {
        protected $mailer;

        public function setMailer(Mailer $mailer)
        {
            $this->mailer = $mailer;
        }

        // ...
    }

Iniettare la dipendenza con il metodo setter, necessita solo di un cambio di sintassi:

.. configuration-block::

    .. code-block:: yaml

        # src/Acme/HelloBundle/Resources/config/services.yml
        parameters:
            # ...
            newsletter_manager.class: Acme\HelloBundle\Newsletter\NewsletterManager

        services:
            my_mailer:
                # ...
            newsletter_manager:
                class:     "%newsletter_manager.class%"
                calls:
                    - [setMailer, ["@my_mailer"]]

    .. code-block:: xml

        <!-- src/Acme/HelloBundle/Resources/config/services.xml -->
        <?xml version="1.0" encoding="UTF-8" ?>
        <container xmlns="http://symfony.com/schema/dic/services"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://symfony.com/schema/dic/services http://symfony.com/schema/dic/services/services-1.0.xsd">

            <parameters>
                <!-- ... -->
                <parameter key="newsletter_manager.class">Acme\HelloBundle\Newsletter\NewsletterManager</parameter>
            </parameters>

            <services>
                <service id="my_mailer" ...>
                <!-- ... -->
                </service>
                <service id="newsletter_manager" class="%newsletter_manager.class%">
                    <call method="setMailer">
                        <argument type="service" id="my_mailer" />
                    </call>
                </service>
            </services>
        </container>

    .. code-block:: php

        // src/Acme/HelloBundle/Resources/config/services.php
        use Symfony\Component\DependencyInjection\Definition;
        use Symfony\Component\DependencyInjection\Reference;

        // ...
        $container->setParameter(
            'newsletter_manager.class',
            'Acme\HelloBundle\Newsletter\NewsletterManager'
        );

        $container->setDefinition('my_mailer', ...);
        $container->setDefinition('newsletter_manager', new Definition(
            '%newsletter_manager.class%'
        ))->addMethodCall('setMailer', array(
            new Reference('my_mailer'),
        ));

.. note::

    Gli approcci presentati in questa sezione sono chiamati "iniezione del costruttore"
    e "iniezione del setter". Il contenitore dei servizi di Symfony2  supporta anche
    "iniezione di proprietà".

.. _book-container-request-stack:

Iniettare la richiesta
~~~~~~~~~~~~~~~~~~~~~~

.. versionadded:: 2.4
    Il servizio ``request_stack`` è stato introdotto nella versione 2.4.

A partire da Symfony 2.4, invece di iniettare il servizio ``request``, si dovrebbe
iniettare il servizio ``request_stack`` e accedere alla richiesta con il
metodo
:method:`Symfony\\Component\\HttpFoundation\\RequestStack::getCurrentRequest`::

    namespace Acme\HelloBundle\Newsletter;

    use Symfony\Component\HttpFoundation\RequestStack;

    class NewsletterManager
    {
        protected $requestStack;

        public function __construct(RequestStack $requestStack)
        {
            $this->requestStack = $requestStack;
        }

        public function anyMethod()
        {
            $request = $this->requestStack->getCurrentRequest();
            // ... fare qualcosa con la richiesta
        }

        // ...
    }

Ora, basta iniettare ``request_stack``, che si comporta come un normale servizio:

.. configuration-block::

    .. code-block:: yaml

        # src/Acme/HelloBundle/Resources/config/services.yml
        services:
            newsletter_manager:
                class:     "Acme\HelloBundle\Newsletter\NewsletterManager"
                arguments: ["@request_stack"]

    .. code-block:: xml

        <!-- src/Acme/HelloBundle/Resources/config/services.xml -->
        <?xml version="1.0" encoding="UTF-8" ?>
        <container xmlns="http://symfony.com/schema/dic/services"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://symfony.com/schema/dic/services http://symfony.com/schema/dic/services/services-1.0.xsd">

            <services>
                <service
                    id="newsletter_manager"
                    class="Acme\HelloBundle\Newsletter\NewsletterManager"
                >
                    <argument type="service" id="request_stack"/>
                </service>
            </services>
        </container>

    .. code-block:: php

        // src/Acme/HelloBundle/Resources/config/services.php
        use Symfony\Component\DependencyInjection\Definition;
        use Symfony\Component\DependencyInjection\Reference;

        // ...
        $container->setDefinition('newsletter_manager', new Definition(
            'Acme\HelloBundle\Newsletter\NewsletterManager',
            array(new Reference('request_stack'))
        ));

.. sidebar: Perché non iniettare il servizio request?

    Quasi tutti i servizi presenti in Symfony2 si comportano allo stesso modo: viene creata
    una singola istanza dal contenitore, restituita ogni volta che venga richiesta o che
    venga iniettata in un altro servizio. C'è però un'eccezione in un'applicazione standard
    Symfony2: il servizio ``request``.

    Se si prova a iniettare ``request`` in un servizio, probabilmente si riceverà
    un'eccezione
    :class:`Symfony\\Component\\DependencyInjection\\Exception\\ScopeWideningInjectionException`.
    Questo perché ``request`` può **cambiare** durante il ciclo di vita
    di un contenitore (per esempio, quando viene creata una sottorichiesta).


.. tip::

    Se si definisce un controllore come servizio, si può ottenere l'oggetto ``Request``
    senza iniettare il contenitore, passandolo come parametro di un
    metodo azione. Vedere
    :ref:`book-controller-request-argument` per maggiori dettagli.

Rendere opzionali i riferimenti
-------------------------------

A volte, uno dei servizi può avere una dipendenza opzionale, il che significa
che la dipendenza non è richiesta al fine di fare funzionare correttamente il servizio.
Nell'esempio precedente, il servizio ``my_mailer`` *deve* esistere, altrimenti verrà
lanciata un'eccezione. Modificando la definizione del servizio ``newsletter_manager``,
è possibile rendere questo riferimento opzionale. Il contenitore inietterà se
esiste e in caso contrario non farà nulla:

.. configuration-block::

    .. code-block:: yaml

        # src/Acme/HelloBundle/Resources/config/services.yml
        parameters:
            # ...

        services:
            newsletter_manager:
                class:     "%newsletter_manager.class%"
                arguments: ["@?my_mailer"]

    .. code-block:: xml

        <!-- src/Acme/HelloBundle/Resources/config/services.xml -->
        <?xml version="1.0" encoding="UTF-8" ?>
        <container xmlns="http://symfony.com/schema/dic/services"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://symfony.com/schema/dic/services http://symfony.com/schema/dic/services/services-1.0.xsd">

            <services>
                <service id="my_mailer" ...>
                <!-- ... -->
                </service>
                <service id="newsletter_manager" class="%newsletter_manager.class%">
                    <argument type="service" id="my_mailer" on-invalid="ignore" />
                </service>
            </services>
        </container>

    .. code-block:: php

        // src/Acme/HelloBundle/Resources/config/services.php
        use Symfony\Component\DependencyInjection\Definition;
        use Symfony\Component\DependencyInjection\Reference;
        use Symfony\Component\DependencyInjection\ContainerInterface;

        // ...
        $container->setParameter(
            'newsletter_manager.class',
            'Acme\HelloBundle\Newsletter\NewsletterManager'
        );

        $container->setDefinition('my_mailer', ...);
        $container->setDefinition('newsletter_manager', new Definition(
            '%newsletter_manager.class%',
            array(
                new Reference(
                    'my_mailer',
                    ContainerInterface::IGNORE_ON_INVALID_REFERENCE
                )
            )
        ));

In YAML, la speciale sintassi ``@?`` dice al contenitore dei servizi che la dipendenza
è opzionale. Naturalmente, ``NewsletterManager`` deve essere scritto per
consentire una dipendenza opzionale::

        public function __construct(Mailer $mailer = null)
        {
            // ...
        }

Servizi del nucleo di Symfony e di terze parti
----------------------------------------------

Dal momento che Symfony2 e tutti i bundle di terze parti configurano e recuperano i loro servizi
attraverso il contenitore, si possono accedere facilmente o addirittura usarli nei propri
servizi. Per mantenere le cose semplici, Symfony2 per impostazione predefinita non richiede che
i controllori siano definiti come servizi. Inoltre Symfony2 inietta l'intero
contenitore dei servizi nel controllore. Ad esempio, per gestire la memorizzazione delle
informazioni su una sessione utente, Symfony2 fornisce un servizio ``session``,
a cui è possibile accedere dentro a un controllore standard, come segue::

    public function indexAction($bar)
    {
        $session = $this->get('session');
        $session->set('foo', $bar);

        // ...
    }

In Symfony2, si potranno sempre utilizzare i servizi forniti dal nucleo di Symfony o
dai bundle di terze parti per eseguire funzionalità come la resa di template (``templating``),
l'invio di email (``mailer``), o l'accesso a informazioni sulla richiesta (``request``).

Questo possiamo considerarlo come un ulteriore passo in avanti con l'utilizzo di questi servizi all'interno di servizi che
si è creato per l'applicazione. Andiamo a modificare ``NewsletterManager``
per usare il reale servizio ``mailer`` di Symfony2 (al posto del finto ``my_mailer``).
Si andrà anche a far passare il servizio con il motore dei template al ``NewsletterManager``
in modo che possa generare il contenuto dell'email tramite un template::

    namespace Acme\HelloBundle\Newsletter;

    use Symfony\Component\Templating\EngineInterface;

    class NewsletterManager
    {
        protected $mailer;

        protected $templating;

        public function __construct(
            \Swift_Mailer $mailer,
            EngineInterface $templating
        ) {
            $this->mailer = $mailer;
            $this->templating = $templating;
        }

        // ...
    }

La configurazione del contenitore dei servizi è semplice:

.. configuration-block::

    .. code-block:: yaml

        services:
            newsletter_manager:
                class:     "%newsletter_manager.class%"
                arguments: ["@mailer", "@templating"]

    .. code-block:: xml

        <?xml version="1.0" encoding="UTF-8" ?>
        <container xmlns="http://symfony.com/schema/dic/services"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://symfony.com/schema/dic/services http://symfony.com/schema/dic/services/services-1.0.xsd">

            <service id="newsletter_manager" class="%newsletter_manager.class%">
                <argument type="service" id="mailer"/>
                <argument type="service" id="templating"/>
            </service>
        </container>

    .. code-block:: php

        $container->setDefinition('newsletter_manager', new Definition(
            '%newsletter_manager.class%',
            array(
                new Reference('mailer'),
                new Reference('templating'),
            )
        ));

Il servizio ``newsletter_manager`` ora ha accesso ai servizi del nucleo ``mailer``
e ``templating``. Questo è un modo comune per creare servizi specifici
all'applicazione, in grado di sfruttare la potenza di numerosi servizi presenti
nel framework.

.. tip::

    Assicurarsi che la voce ``swiftmailer`` appaia nella configurazione
    dell'applicazione. Come è stato accennato in :ref:`service-container-extension-configuration`,
    la chiave ``swiftmailer`` invoca l'estensione del servizio da
    ``SwiftmailerBundle``, il quale registra il servizio ``mailer``.

.. _book-service-container-tags:

I tag
~~~~~

Allo stesso modo con cui il post di un blog su web viene etichettato con cose
tipo "Symfony" o "PHP", anche i servizi configurati nel contenitore possono 
essere etichettati. Nel contenitore dei servizi, un tag implica che si intende
utilizzare il servizio per uno scopo specifico. Si prenda il seguente esempio:

.. configuration-block::

    .. code-block:: yaml

        services:
            foo.twig.extension:
                class: Acme\HelloBundle\Extension\FooExtension
                tags:
                    -  { name: twig.extension }

    .. code-block:: xml

        <?xml version="1.0" encoding="UTF-8" ?>
        <container xmlns="http://symfony.com/schema/dic/services"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://symfony.com/schema/dic/services http://symfony.com/schema/dic/services/services-1.0.xsd">

            <service id="foo.twig.extension"
                class="Acme\HelloBundle\Extension\FooExtension">
                <tag name="twig.extension" />
            </service>
        </container>

    .. code-block:: php

        $definition = new Definition('Acme\HelloBundle\Extension\FooExtension');
        $definition->addTag('twig.extension');
        $container->setDefinition('foo.twig.extension', $definition);

Il tag ``twig.extension`` è un tag speciale che ``TwigBundle`` utilizza
durante la configurazione. Dando al servizio il tag ``twig.extension``,
il bundle sa che il servizio ``foo.twig.extension`` dovrebbe essere registrato
come estensione Twig. In altre parole, Twig cerca tutti i servizi etichettati
con ``twig.extension`` e li registra automaticamente come estensioni.

I tag, quindi, sono un modo per dire a Symfony2 o a un altro bundle di terze parti che
il servizio dovrebbe essere registrato o utilizzato in un qualche modo speciale dal bundle.

Per una lista completa dei tag disponibili in Symfony, dare un'occhiata
a :doc:`/reference/dic_tags`. Ognuno di essi ha un differente effetto sul
servizio e molti tag richiedono parametri aggiuntivi (oltre al solo ``name``
del parametro).

Debug dei servizi
-----------------

Si può sapere quali servizi sono registrati nel contenitore, usando la
console. Per mostrare tutti i servizi e le relative classi, eseguire:

.. code-block:: bash

    $ php app/console container:debug

Vengono mostrati solo i servizi pubblici, ma si possono vedere anche quelli privati:

.. code-block:: bash

    $ php app/console container:debug --show-private

Si possono ottenere informazioni più dettagliate su un singolo servizio, specificando
il suo id:

.. code-block:: bash

    $ php app/console container:debug my_mailer

Saperne di più
--------------

* :doc:`/components/dependency_injection/parameters`
* :doc:`/components/dependency_injection/compilation`
* :doc:`/components/dependency_injection/definitions`
* :doc:`/components/dependency_injection/factories`
* :doc:`/components/dependency_injection/parentservices`
* :doc:`/components/dependency_injection/tags`
* :doc:`/cookbook/controller/service`
* :doc:`/cookbook/service_container/scopes`
* :doc:`/cookbook/service_container/compiler_passes`
* :doc:`/components/dependency_injection/advanced`

.. _`architettura orientata ai servizi`: http://it.wikipedia.org/wiki/Service-oriented_architecture
