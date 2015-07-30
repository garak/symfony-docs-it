.. index::
   single: HTTP
   single: HttpKernel
   single: Componenti; HttpKernel

Il componente HttpKernel
========================

    Il componente HttpKernel fornisce un processo strutturato per convertire una
    ``Request`` in una ``Response``, usando il distributore di eventi.
    È abbastanza flessibile per creare un framework completo (Symfony), un micro-framework
    (Silex) o un CMS avanzato (Drupal).

Installazione
-------------

È possibile installare il componente in due modi:

* Installandolo :doc:`via Composer</components/using_components>` (``symfony/http-kernel`` su Packagist);
* Utilizzando il repository ufficiale su Git (https://github.com/symfony/HttpKernel).

.. include:: /components/require_autoload.rst.inc

Il flusso di una richiesta
--------------------------

Ogni interazione HTTP inizia con una richiesta e finisce con una risposta.
Il compito dello sviluppatore è quello di creare codice PHP che legga le informazioni della richiesta
(p.e. l'URL) e crei e restituisca una risposta (p.e. una pagina HTML o una stringa JSON).

.. image:: /images/components/http_kernel/request-response-flow.png
   :align: center

Di solito, si costruisce una sorta di framework o sistema per gestire tutte le operazioni
ripetitive (come rotte, sicurezza, ecc.), in modo che uno sviluppatore possa costruire
facilmente ogni *pagina* dell'applicazione. *Come* esattamente tali sistemi siano costruiti varia
enormemente, Il componente HttpKernel fornisce un'interfaccia che formalizza il
processo di iniziare con una richiesta e creare la risposta appropriata.
Il componente è pensato per essere il cuore di qualsiasi applicazione o framework, non
importa quanto vari l'architettura di tale sistema::

    namespace Symfony\Component\HttpKernel;

    use Symfony\Component\HttpFoundation\Request;

    interface HttpKernelInterface
    {
        // ...

        /**
         * @return Response Un'istanza di Response
         */
        public function handle(
            Request $request,
            $type = self::MASTER_REQUEST,
            $catch = true
        );
    }

Internamente, :method:`HttpKernel::handle()<Symfony\\Component\\HttpKernel\\HttpKernel::handle>`,
l'implementazione concreta di :method:`HttpKernelInterface::handle() <Symfony\\Component\\HttpKernel\\HttpKernelInterface::handle>`,
definisce un flusso che inizia con una :class:`Symfony\\Component\\HttpFoundation\\Request`
e finisce con una :class:`Symfony\\Component\\HttpFoundation\\Response`.

.. image:: /images/components/http_kernel/01-workflow.png
   :align: center

I dettagli precisi di tale flusso sono la chiave per capire come funzioni il kernel
(e il framework Symfony o qualsiasi altra libreria che usi il kernel).

HttpKernel: guidato da eventi
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Il metodo ``HttpKernel::handle()`` funziona internamente distribuendo eventi.
Questo rende il metodo flessibile, ma anche un po' astratto, poiché tutto il
"lavoro" di un framework/applicazione costruiti con HttpKernel è effettivamente svolto
da ascoltatori di eventi.

Per aiutare nella spiegazione di questo processo, questo documento ne analizza ogni passo
e spiega come funziona una specifica implementazione di HttpKernel, il framework
Symfony.

All'inizio, l'uso di :class:`Symfony\\Component\\HttpKernel\\HttpKernel`
è molto semplice e implica la creazione un
:doc:`distributore di eventi </components/event_dispatcher/introduction>` e un
:ref:`risolutore di controllori <component-http-kernel-resolve-controller>` (spiegato
più avanti). Per completare un kernel funzionante, si aggiungeranno ulteriori
ascoltatori agli eventi discussi sotto::

    use Symfony\Component\HttpFoundation\Request;
    use Symfony\Component\HttpKernel\HttpKernel;
    use Symfony\Component\EventDispatcher\EventDispatcher;
    use Symfony\Component\HttpKernel\Controller\ControllerResolver;

    // creare l'oggetto Request
    $request = Request::createFromGlobals();

    $dispatcher = new EventDispatcher();
    // ... aggiungere degli ascoltatori

    // creare un risolutore di controllori
    $resolver = new ControllerResolver();
    // istanziare il kernel
    $kernel = new HttpKernel($dispatcher, $resolver);

    // esegue effettivamente il kernel, che trasforma la richiesta in una risposta
    // distribuendo eventi, richiamando un controllore e restituendo la risposta
    $response = $kernel->handle($request);

    // mostra il contenuto e invia gli header
    $response->send();

    // lancia l'evento kernel.terminate
    $kernel->terminate($request, $response);

Vedere ":ref:`http-kernel-working-example`" per un'implementazione più concreta.

Per informazioni generali sull'aggiunta di ascoltatori agli eventi qui sotto, vedere
:ref:`http-kernel-creating-listener`.

.. tip::

    Fabien Potencier ha anche scritto una bella serie sull'uso del componente HttpKernel
    e altri componenti di Symfony per creare un proprio framework. Vedere
    `Create your own framework... on top of the Symfony2 Components`_.

.. _component-http-kernel-kernel-request:

1) L'evento ``kernel.request``
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

**Scopi tipici**: Aggiungere più informazioni alla ``Request``, inizializzare
le parti del sistema oppure restituire una ``Response`` se possibile (p.e. un livello
di sicurezza che nega accesso)

:ref:`Tabella informativa sugli eventi del kernel <component-http-kernel-event-table>`

Il primo evento distribuito in :method:`HttpKernel::handle <Symfony\\Component\\HttpKernel\\HttpKernel::handle>`
è ``kernel.request``, che può avere vari ascoltatori.

.. image:: /images/components/http_kernel/02-kernel-request.png
   :align: center

Gli ascoltatori di questo evento possono essere alquanti vari. Alcuni, come un ascoltatore
di sicurezza, possono avere informazioni sufficienti a creare un oggetto ``Response`` immediatamente.
Per esempio, se un ascoltatore di sicurezza determina che un utente non deve accedere,
può restituire un :class:`Symfony\\Component\\HttpFoundation\\RedirectResponse`
alla pagina di login o una risposta 403 (accesso negato).

Se a questo punto viene restituita una ``Response``, il processo salta direttamente 
all'evento :ref:`kernel.response<component-http-kernel-kernel-response>`.

.. image:: /images/components/http_kernel/03-kernel-request-response.png
   :align: center

Altri ascoltatori inizializzano semplicemente alcune cose o aggiungono ulteriori informazioni alla richiesta.
Per esempio, un ascoltatore può determinare e impostare il locale sull'oggetto
``Request``.

Un altro ascoltatore comune è il routing. Un ascoltatore di rotte può processare la ``Request``
e determinare il controllore da rendere (vedere la sezione successiva).
In effetti, l'oggetto ``Request`` ha un insieme di ":ref:`attributi<component-foundation-attributes>`",
che è il posto ideale per memorizzare tali dati, ulteriori e specifici dell'applicazione,
sulla richiesta. Questo vuol dire che se un ascoltatore di rotte determina in qualche modo
il controllore, può memorizzarlo negli attributi di ``Request`` (che possono essere usati
dal risolutore di controllori).

Complessivamente, lo scopo dell'evento ``kernel.request`` è quello di creare e restituire
una ``Response`` direttamente oppure di aggiungere informazioni alla ``Request``
(p.e. impostando il locale o altre informazioni sugli attributi della
``Request``).

.. note::

    Quando si imposta una risposta per l'evento ``kernel.request``, la propagazione
    si ferma. Questo vuol dire che ascoltatori con priorità inferiore non saranno eseguiti.

.. sidebar:: ``kernel.request`` nel framework Symfony

    L'ascoltatore più importante di ``kernel.request`` nel framework Symfony
    è :class:`Symfony\\Component\\HttpKernel\\EventListener\\RouterListener`.
    Questa classe esegue il livello delle rotte, che restituisce un *array* di informazioni
    sulla richiesta corrispondente, incluso ``_controller`` e ogni segnaposto
    presente nello schema della rotta (p.e. ``{slug}``). Vedere il
    :doc:`componente Routing </components/routing/introduction>`.

    Questo array di informazioni è memorizzato nell'array ``attributes`` di :class:`Symfony\\Component\\HttpFoundation\\Request`.
    L'aggiunta di informazioni sulle rotte in questo punto non porta ancora a nulla,
    ma sarà usato durante la risoluzione del controllore.

.. _component-http-kernel-resolve-controller:

2) Risolvere il controllore
~~~~~~~~~~~~~~~~~~~~~~~~~~~

Ipotizzando che nessun ascoltatore di ``kernel.request`` sia stato in grado di creare una ``Response``,
il passo successivo in HttpKernel è determinare e preparare (cioè risolvere) il
controllore. Il controllore è la parte del codice dell'applicazione finale
responsabile di creare e restituire la ``Response`` per una pagina specifica.
L'unico requisito è che sia un callable PHP, cioè una funzione, un metodo su un
oggetto o una ``Closure``.

Ma *come* determinare l'esatto controllore per una richiesta è un compito che spetta
all'applicazione. Questo è il lavoro del "risolutore di controllori", una classe che
implementa :class:`Symfony\\Component\\HttpKernel\\Controller\\ControllerResolverInterface`
ed è uno dei parametri del costruttore di ``HttpKernel``.

.. image:: /images/components/http_kernel/04-resolve-controller.png
   :align: center

Il compito dello sviluppatore è creare una classe che implementi l'interfaccia e quindi
i suoi due metodi: ``getController`` e ``getArguments``. In effetti, esiste già
un'implementazione, che si può usare direttamente o a cui ci si può ispirare:
:class:`Symfony\\Component\\HttpKernel\\Controller\\ControllerResolver`.
Tale implementazione è spiegata qui sotto::

    namespace Symfony\Component\HttpKernel\Controller;

    use Symfony\Component\HttpFoundation\Request;

    interface ControllerResolverInterface
    {
        public function getController(Request $request);

        public function getArguments(Request $request, $controller);
    }

Internamente, il metodo ``HttpKernel::handle`` prima richiama
:method:`Symfony\\Component\\HttpKernel\\Controller\\ControllerResolverInterface::getController`
sul risolutore di controllori. A questo viene passata la ``Request`` ed è responsabile di
determinare in qualche modo e restituire un callable PHP (il controllore) in base
alle informazioni della richiesta.

Il secondo metodo, :method:`Symfony\\Component\\HttpKernel\\Controller\\ControllerResolverInterface::getArguments`,
sarà richiamato dopo che un altro evento, ``kernel.controller``, è stato distribuito.

.. sidebar:: Risolvere il controllore nel framework Symfony

    Il framework Symfony usa la classe
    :class:`Symfony\\Component\\HttpKernel\\Controller\\ControllerResolver`
    (in realtà, usa una sotto-classe, con alcune funzionalità aggiuntive menzionate
    più avanti). Questa classe sfrutta le informazioni che erano state inserite
    nella proprietà ``attributes`` dell'oggetto ``Request`` durante ``RouterListener``.

    **getController**

    ``ControllerResolver`` cerca una chiave ``_controller``
    nella proprietà ``attributes`` dell'oggetto ``Request`` (si ricordi che tale
    informazione solitamente è memorizzata nella ``Request`` tramite ``RouterListener``).
    Questa stringa viene trasformata in un callable PHP nei seguenti passi:

    a) Il formato ``AcmeDemoBundle:Default:index`` della chiave ``_controller``
    viene cambiato in un'altra stringa che contiene il nome completo di classe e metodo
    del controllore, seguendo la convenzione di Symfony, cioè
    ``Acme\DemoBundle\Controller\DefaultController::indexAction``. Questa trasformazione
    è specifica della sotto-classe :class:`Symfony\\Bundle\\FrameworkBundle\\Controller\\ControllerResolver`
    usata dal framework Symfony.

    b) Viene istanziata una nuova istanza della classe controllore, senza parametri al
    costruttore.

    c) Se il controllore implementa :class:`Symfony\\Component\\DependencyInjection\\ContainerAwareInterface`,
    viene richiamato ``setContainer`` sull'oggetto controllore e gli viene passato il
    contenitore. Anche questo passo è specifico della sotto-classe :class:`Symfony\\Bundle\\FrameworkBundle\\Controller\\ControllerResolver`
    usata dal framework Symfony.

    Ci possono essere alcune piccole variazioni nel processo appena visto, p.e. se
    i controllori sono stati registrati come servizi).

.. _component-http-kernel-kernel-controller:

3) L'evento ``kernel.controller``
---------------------------------

**Scopi tipici**: Inizializzare cose o cambiare il controllore subito prima che il
controllore venga eseguito.

:ref:`Tabella informativa sugli eventi del kernel<component-http-kernel-event-table>`

Dopo che il callable controllore è stato determinato, ``HttpKernel::handle``
distribuisce l'evento ``kernel.controller``. Gli ascoltatori di questo evento possono inizializzare
alcune parti del sistema che devono essere inizializzate dopo che alcune cose
sono state determinate (p.e. il controllore, informazioni sulle rotte) ma prima
che il controllore sia eseguito. Per alcuni esempi, vedere la sezione Symfony più avanti.

.. image:: /images/components/http_kernel/06-kernel-controller.png
   :align: center

Gli ascoltatori di questo evento possono anche cambiare completamente il callable controllore,
richiamando :method:`FilterControllerEvent::setController<Symfony\\Component\\HttpKernel\\Event\\FilterControllerEvent::setController>`
sull'oggetto evento che viene passato agli ascoltatori di questo evento.

.. sidebar:: ``kernel.controller`` in the Symfony Framework

    Ci sono alcuni ascoltatori minori dell'evento ``kernel.controller`` nel
    framework Symfony, la maggior parte dei quali ha a che vedere con la raccolta di dati per il
    profilatore, se è abilitato.

    Un interessante ascoltatore è presente in `SensioFrameworkExtraBundle`_,
    incluso in Symfony Standard Edition. La funzionalità
    `@ParamConverter`_ di questo bundle consente di passare un oggetto completo (p.e. un oggetto
    ``Post``) a un controllore, al posto di uno scalare (p.e. un parametro 
    ``id``presente su una rotta). L'ascoltatore,
    ``ParamConverterListener``, usa reflection per cercare ogni parametro del controllore
    e prova a usare vari metodi per convertirli in oggetti, che sono quindi memorizzati
    nella proprietà ``attributes`` dell'oggetto ``Request``. Leggere la
    prossima sezione per vedere perché questo è importante.

4) Ottenere i parametri del controllore
---------------------------------------

Quindi, ``HttpKernel::handle`` richiama
:method:`Symfony\\Component\\HttpKernel\\Controller\\ControllerResolverInterface::getArguments`.
Si ricordi che il controllore restituito in ``getController`` è un callable.
Lo scopo di ``getArguments`` è restituire l'array di parametri che vanno
passati a tale controllore. Il modo esatto in cui ciò sia realizzato spetta completamente
alla progettazione dello sviluppatore, sebbene la classe :class:`Symfony\\Component\\HttpKernel\\Controller\\ControllerResolver`
ne sia un buon esempio.

.. image:: /images/components/http_kernel/07-controller-arguments.png
   :align: center

A questo punto il kernel ha un callable PHP (il controllore) e un array
di parametri che vanno passati durante l'esecuzione di tale callable.

.. sidebar:: Ottenere i parametri del controllore nel framework Symfony 

    Ora che sappiamo esattamente cosa sia il callable controllore (solitamente un metodo
    dentro all'oggetto controllore), ``ControllerResolver`` usa `reflection`_
    sul callable per restituire un array di *nomi* di ciascun parametro.
    Quindi itera su ogni parametro e usa il trucco seguente per
    determinare quale valore sia da passare per ogni parametro:

    a) Se gli attributi di ``Request`` contengono una chiave che corrisponde al nome
    del parametro, viene usato quel valore. Per esempio, se il primo parametro
    di un controllore è ``$slug`` e c'è una chiave ``slug`` nella chiave
    ``attributes`` di ``Request``, tale valore viene usato (e tipicamente questo valore
    viene da ``RouterListener``).

    b) Se il parametro nel controllore ha specificato come tipo
    :class:`Symfony\\Component\\HttpFoundation\\Request` , viene passata la
    ``Request`` come valore.

.. _component-http-kernel-calling-controller:

5) Richiamare il controllore
~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Il prossimo passo è semplice! ``HttpKernel::handle`` esegue il controllore.

.. image:: /images/components/http_kernel/08-call-controller.png
   :align: center

Il compito del controllore è costruire la risposta per la risorsa data.
Potrebbe essere una pagina HTML, una stringa JSON o qualsiasi altra cosa. Diversamente dalle
altre parti del processo viste finora, questo passo è implementato dallo sviluppatore,
per ogni pagina da costruire.

Di solito, il controllore restituirà un oggetto ``Response``. Se questo è vero,
il lavoro del kernel sta per finire! In questo caso, il prossimo passo
è l'evento :ref:`kernel.response<component-http-kernel-kernel-response>`.

.. image:: /images/components/http_kernel/09-controller-returns-response.png
   :align: center

Se invece il controllore restituisce qualcosa che non sia una ``Response``, il kernel deve
fare ancora un piccolo lavoro, :ref:`kernel.view<component-http-kernel-kernel-view>` 
(perché lo scopo finale è *sempre* generare un oggetto ``Response``).

.. note::

    Un controllore deve restituire *qualcosa*. Se un controllore restituisce ``null``,
    viene immediatamente lanciata un'eccezione.

.. _component-http-kernel-kernel-view:

6) L'evento ``kernel.view``
~~~~~~~~~~~~~~~~~~~~~~~~~~~

**Scopi tipici**: Trasformare un valore diverso da ``Response``  restituito da un
controllore in una ``Response``

:ref:`Tabella informativa sugli eventi del kernel<component-http-kernel-event-table>`

Se il controllore non restituisce un oggetto ``Response``, iol kernel distribuisce un
altro evento, ``kernel.view``. Il compito di un ascoltatore di tale evento è di usare
il valore restituito dal controllore (p.e. un array di dati o un oggetto) per
creare una ``Response``.

.. image:: /images/components/http_kernel/10-kernel-view.png
   :align: center

Questo può essere utile se si vuole usare un livello "vista": invece di restituire
una ``Response`` dal controllore, si restituiscono dati che rappresentano la pagina.
Un ascoltatore di questo evento può quindi usare tali dati per creare una ``Response``
che sia nel formato corretto (p.e. HTML, JSON, ecc.).

A questo punto, ne nessun ascoltatore imposta una risposta sull'evento, viene lanciata
un'eccezione: o il controllore *o* uno degli ascoltatori della vista devono sempre
restituire una ``Response``.

.. note::

    Quando si imposta una risposta per l'evento ``kernel.request``, la propagazione
    si ferma. Questo vuol dire che ascoltatori con priorità inferiore non saranno eseguiti.

.. sidebar:: ``kernel.view`` in the Symfony Framework

    Non c'è un ascoltatore predefinito nel framework Symfony per l'evento ``kernel.view``.
    Tuttavia, un bundle del nucleo, `SensioFrameworkExtraBundle`_,
    *aggiunge* un ascoltatore a questo evento. Se un controllore restituisce un array,
    e se si inserisce l'annotazione `@Template`_
    in cima al controllore, questo ascoltatore rende un template,
    passa l'array restituita dal controllore a tale template
    e crea una ``Response`` con il contenuto restituito da tale template.

    Inoltre, un bundle popolare della comunità, `FOSRestBundle`_, implementa
    un ascoltatore di questo evento, con lo scopo di fornire un robusto livello vista,
    capace di usare un singolo controllore per restituire molti differenti tipi di
    risposta (p.e. HTML, JSON, XML, ecc).

.. _component-http-kernel-kernel-response:

7) L'evento ``kernel.response``
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

**Scopi tipici**: Modificare l'oggetto ``Response`` subito prima che sia inviato

:ref:`Tabella informativa sugli eventi del kernel<component-http-kernel-event-table>`

Lo scopo finale del kernel è trasformare una ``Request`` in una ``Response``. La
``Response`` può essere creata durante l'evento :ref:`kernel.request<component-http-kernel-kernel-request>`,
restituita dal :ref:`controllore<component-http-kernel-calling-controller>` oppure
restituita da uno degli ascoltatori dell'evento
:ref:`kernel.view<component-http-kernel-kernel-view>`.

Indipendentemente da chi abbia creato la ``Response``, un altro evento, ``kernel.response``,
viene distribuito subito dopo. Un tipico ascoltatore di questo evento modificherà
l'oggetto ``Response`` in qualche modo, modificando header, aggiungendo cookie o anche
cambiando il contenuto della ``Response`` stessa (p.e. iniettando del codice
JavaScript prima della chiusura del tag ``</body>`` di una risposta HTML).

Dopo la distribuzione di questo evento, l'oggetto finale ``Response`` viene restituito
da :method:`Symfony\\Component\\HttpKernel\\HttpKernel::handle`. Nel caso d'uso
più tipico, si può quindi richiamare il metodo :method:`Symfony\\Component\\HttpFoundation\\Response::send`,
che invia gli header e stampa il contenuto della ``Response``.

.. sidebar:: ``kernel.response`` in the Symfony Framework

    Ci sono molti altri ascoltatori minori di questo evento nel framework Symfony,
    la maggior parte dei quali modifica la risposta in qualche modo. Per esempio,
    :class:`Symfony\\Bundle\\WebProfilerBundle\\EventListener\\WebDebugToolbarListener`
    inietta del codice JavaScript in fondo alla pagina in ambiente ``dev``,
    per mostrare la barra di debug del web. Un altro ascoltatore,
    :class:`Symfony\\Component\\Security\\Http\\Firewall\\ContextListener`,
    serializza le informazioni sull'utente corrente nella
    sessione, in modo che possano essere ricaricate alla richiesta successiva. 

.. _component-http-kernel-kernel-terminate:

8) L'evento ``kernel.terminate``
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

**Scopi tipici**: Eseguire qualche azione "pesante" dopo che la risposta sia stata
inviata all'utente

:ref:`Tabella informativa sugli eventi del kernel<component-http-kernel-event-table>`

L'evento finale processato da HttpKernel è ``kernel.terminate``, che è unico, perché
avviene *dopo* il metodo ``HttpKernel::handle`` e quindi dopo che la
risposta è stata inviata all'utente. Ripreso da sopra, il codice usato dal
kernel finisce in questo modo::

    // mostra il contenuto e invia gli header
    $response->send();

    // lancia l'evento kernel.terminate
    $kernel->terminate($request, $response);

Come si può vedere, richiamando ``$kernel->terminate`` dopo l'invio della risposta,
si lancerà l'evento ``kernel.terminate``, in cui si possono eseguire alcune
azioni che potrebbero essere state rimandate, per poter restituire la risposta nel modo
più veloce possibile al client (p.e. invio di email).

.. caution::

    Internamente, HttpKernel  fa uso della funzione :phpfunction:`fastcgi_finish_request` di
    PHP. Questo vuole dire che, al momento, solo le API del server `PHP FPM`_ sono
    in grado di inviare al cliente una risposta mentre il processo PHP del server
    esegue ancora alcuni compiti. Con le API di ogni altro server, gli ascoltatori di ``kernel.terminate``
    sono comunque eseguiti, ma la risposta non viene inviata al cliente finché non
    sono tutti completati.

.. note::

    L'uso dell'evento ``kernel.terminate`` è facoltativo e va limitato al caso in cui
    il kernel implementi :class:`Symfony\\Component\\HttpKernel\\TerminableInterface`.

.. sidebar:: ``kernel.terminate`` in the Symfony Framework

    Se si usa SwiftmailerBundle con Symfony e si usa lo spool ``memory``,
    viene attivato `EmailSenderListener`_, che invia effettivamente
    le email pianificate per essere inviate durante la richiesta.

.. _component-http-kernel-kernel-exception:

Gestire le eccezioni:: l'evento ``kernel.exception``
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

**Scopi tipici**: Gestire alcuni tipi di eccezioni e creare un'appropriata
``Response`` da restituire per l'eccezione

:ref:`Tabella informativa sugli eventi del kernel<component-http-kernel-event-table>`

Se a un certo punto in ``HttpKernel::handle`` viene lanciata un'eccezione, viene lanciato
un altro evento, ``kernel.exception``. Internamente, il corpo della funzione ``handle``
viene avvolto da un blocco try-catch. Quando viene lanciata un'eccezione, l'evento
``kernel.exception`` viene distribuito, in modo che il proprio sistema possa in qualche
modo rispondere all'eccezione.

.. image:: /images/components/http_kernel/11-kernel-exception.png
   :align: center

A ogni ascoltatore di questo evento viene passato un oggetto :class:`Symfony\\Component\\HttpKernel\\Event\\GetResponseForExceptionEvent`,
che si può usare per accedere all'eccezione originale, tramite il metodo
:method:`Symfony\\Component\\HttpKernel\\Event\\GetResponseForExceptionEvent::getException`.
Un tipico ascoltatore di questo evento verificherà un certo tipo di
eccezione e creerà un appropriata ``Response`` di errore.

Per esempio, per generare una pagina 404, si potrebbe lanciare uno speciale tipo di eccezione
e quindi aggiungere un ascoltatore a tale evento, che cerchi l'eccezione e
crei e restituisca una ``Response`` 404. In effetti, il componente HttpKernel
dispone di un :class:`Symfony\\Component\\HttpKernel\\EventListener\\ExceptionListener`,
che, se usato, farà questo e anche di più in modo predefinito (si vedano dettagli
più avanti).

.. note::

    Quando si imposta una risposta per l'evento ``kernel.request``, la propagazione
    si ferma. Questo vuol dire che ascoltatori con priorità inferiore non saranno eseguiti.

.. sidebar:: ``kernel.exception`` in the Symfony Framework

    Ci sono due ascoltatori principali di ``kernel.exception`` quando si usa il
    framework Symfony.

    **ExceptionListener in HttpKernel**

    Il primo fa parte del componente HttpKernel
    e si chiama :class:`Symfony\\Component\\HttpKernel\\EventListener\\ExceptionListener`.
    L'ascoltatore ha diversi scopi:

    1) L'eccezione lanciata è convertita in un oggetto
       :class:`Symfony\\Component\\HttpKernel\\Exception\\FlattenException`,
       che contiene tutte le informazioni sulla richiesta, ma che
       possa essere stampata e serializzata.

    2) Se l'eccezione originale implementa
       :class:`Symfony\\Component\\HttpKernel\\Exception\\HttpExceptionInterface`,
       allora sono richiamati ``getStatusCode`` e ``getHeaders`` sull'eccezione
       e usati per popolare gli header e il codice di stato dell'oggetto ``FlattenException``.
       L'idea è che siano usati nel passo successivo, quando si crea la
       risposta finale.

    3) Un controllore viene eseguito e gli viene passata l'eccezione appiattita.
       Il controllore esatto da rendere viene passato come parametro del costruttore a questo
       ascoltatore. Questo controllore restituirà la ``Response`` finale per questa pagina di errore.

    **ExceptionListener in Security**

    L'altro importante ascoltatore è
    :class:`Symfony\\Component\\Security\\Http\\Firewall\\ExceptionListener`.
    Lo scopo di questo ascoltatore è gestire le eccezioni di sicurezza e, quando
    appropriato, *aiutare* l'utente ad autenticarsi (p.e. rinviando alla pagina
    di login).

.. _http-kernel-creating-listener:

Creare un ascoltatore di eventi
-------------------------------

Come abbiamo visto, si possono creare e attaccare ascoltatori di eventi a qualsiasi evento
distribuito durante il ciclo ``HttpKernel::handle``. Un tipico ascoltatore è una classe PHP
con un metodo da eseguire, ma può essere qualsiasi cosa. Per maggiori informazioni
su come creare e attaccare ascoltatori di eventi, si veda :doc:`/components/event_dispatcher/introduction`.

Il nome di ogni evento del kernel è definito come costante della classe
:class:`Symfony\\Component\\HttpKernel\\KernelEvents`. Inoltre, a ogni ascoltatore
di evento viene passato un singolo parametro, che è una sotto-classe di :class:`Symfony\\Component\\HttpKernel\\Event\\KernelEvent`.
Questo oggetto contiene informazioni sullo stato attuale del sistema e
ogni vento ha il suo oggetto evento:

.. _component-http-kernel-event-table:

=====================  ================================  ===================================================================================
Nome                   Costante ``KernelEvents``         Parametro passato all'ascoltatore
=====================  ================================  ===================================================================================
kernel.request         ``KernelEvents::REQUEST``         :class:`Symfony\\Component\\HttpKernel\\Event\\GetResponseEvent`                    
kernel.controller      ``KernelEvents::CONTROLLER``      :class:`Symfony\\Component\\HttpKernel\\Event\\FilterControllerEvent`
kernel.view            ``KernelEvents::VIEW``            :class:`Symfony\\Component\\HttpKernel\\Event\\GetResponseForControllerResultEvent`
kernel.response        ``KernelEvents::RESPONSE``        :class:`Symfony\\Component\\HttpKernel\\Event\\FilterResponseEvent`
kernel.terminate       ``KernelEvents::TERMINATE``       :class:`Symfony\\Component\\HttpKernel\\Event\\PostResponseEvent`
kernel.exception       ``KernelEvents::EXCEPTION``       :class:`Symfony\\Component\\HttpKernel\\Event\\GetResponseForExceptionEvent`
=====================  ================================  ===================================================================================

.. _http-kernel-working-example:

Un esempio funzionante
----------------------

Quando si usa il componente HttpKernel, si è liberi di connettere qualsiasi ascoltatore
agli eventi del kernel e di usare qualsiasi risolutore di controllori che implementi
:class:`Symfony\\Component\\HttpKernel\\Controller\\ControllerResolverInterface`.
Tuttavia, il componente HttpKernel dispone di alcuni ascoltatori predefiniti e di un
ControllerResolver predefinito, utilizzabili per creare un esempio funzionante::

    use Symfony\Component\HttpFoundation\Request;
    use Symfony\Component\HttpFoundation\Response;
    use Symfony\Component\HttpKernel\HttpKernel;
    use Symfony\Component\EventDispatcher\EventDispatcher;
    use Symfony\Component\HttpKernel\Controller\ControllerResolver;
    use Symfony\Component\HttpKernel\EventListener\RouterListener;
    use Symfony\Component\Routing\RouteCollection;
    use Symfony\Component\Routing\Route;
    use Symfony\Component\Routing\Matcher\UrlMatcher;
    use Symfony\Component\Routing\RequestContext;

    $routes = new RouteCollection();
    $routes->add('hello', new Route('/hello/{name}', array(
            '_controller' => function (Request $request) {
                return new Response(
                    sprintf("Ciao %s", $request->get('name'))
                );
            }
        )
    ));

    $request = Request::createFromGlobals();

    $matcher = new UrlMatcher($routes, new RequestContext());

    $dispatcher = new EventDispatcher();
    $dispatcher->addSubscriber(new RouterListener($matcher));

    $resolver = new ControllerResolver();
    $kernel = new HttpKernel($dispatcher, $resolver);

    $response = $kernel->handle($request);
    $response->send();

    $kernel->terminate($request, $response);

.. _http-kernel-sub-requests:

Sotto-richieste
---------------

Oltre alla richiesta "principale", inviata a ``HttpKernel::handle``,
si possono anche inviare delle cosiddette "sotto-richieste". Una sotto-richiesta assomiglia e
si comporta come ogni altra richiesta, ma tipicamente serve a rendere solo una breve porzione
di una pagina, invece di una pagina completa. Solitamente si fanno sotto-richieste da un
controllore (o forse da dentro a un template, che viene reso da un
controllore).

.. image:: /images/components/http_kernel/sub-request.png
   :align: center

Per eseguire una sotto-richiesta, usare ``HttpKernel::handle``, ma cambiando il secondo
parametro, come segue::

    use Symfony\Component\HttpFoundation\Request;
    use Symfony\Component\HttpKernel\HttpKernelInterface;

    // ...

    // creare qualche altra richiesta a mano, come necessario
    $request = new Request();
    // per resempio, si potrebbe impostare a mano _controller
    $request->attributes->add('_controller', '...');

    $response = $kernel->handle($request, HttpKernelInterface::SUB_REQUEST);
    // fare qualcosa con questa risposta

Questo crea un altro ciclo richiesta-risposta, in cui la nuova ``Request`` è
trasformata in una ``Response``. L'unica differenza interna è che alcuni
ascoltatori (p.e. security) possono intervenire solo per la richiesta principale. A ogni
ascoltatore viene passata una sotto-classe di :class:`Symfony\\Component\\HttpKernel\\Event\\KernelEvent`,
il cui metodo :method:`Symfony\\Component\\HttpKernel\\Event\\KernelEvent::getRequestType`
può essere usato per capire se la richiesta corrente sia una richiesta principale o una sotto-richiesta.

Per esempio, un ascoltatore che deve agire solo sulla richiesta principale può
assomigliare a questo::

    use Symfony\Component\HttpKernel\HttpKernelInterface;
    // ...

    public function onKernelRequest(GetResponseEvent $event)
    {
        if (HttpKernelInterface::MASTER_REQUEST !== $event->getRequestType()) {
            return;
        }

        // ...
    }

.. _Packagist: https://packagist.org/packages/symfony/http-kernel
.. _reflection: http://php.net/manual/it/book.reflection.php
.. _FOSRestBundle: https://github.com/friendsofsymfony/FOSRestBundle
.. _`Create your own framework... on top of the Symfony2 Components`: http://fabien.potencier.org/article/50/create-your-own-framework-on-top-of-the-symfony2-components-part-1
.. _`PHP FPM`: http://php.net/manual/it/install.fpm.php
.. _`SensioFrameworkExtraBundle`: http://symfony.com/doc/current/bundles/SensioFrameworkExtraBundle/index.html
.. _`@ParamConverter`: http://symfony.com/doc/current/bundles/SensioFrameworkExtraBundle/annotations/converters.html
.. _`@Template`: http://symfony.com/doc/current/bundles/SensioFrameworkExtraBundle/annotations/view.html
.. _`EmailSenderListener`: https://github.com/symfony/SwiftmailerBundle/blob/master/EventListener/EmailSenderListener.php
