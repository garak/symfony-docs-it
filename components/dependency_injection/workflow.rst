.. index::
   single: DependencyInjection; Flusso

Flusso di costruzione del contenitore
=====================================

Nelle pagine precedenti di questa sezioni, è stato detto poco sulle posizioni in
cui i vari file e le classi dovrebbero trovarsi. Questo perché ciò dipende
dall'applicazione, libreria o framework in cui si vuole usare
il contenitore. Vedere come il contenitore è configurato e costruito nel
framework Symfony2 aiuterà a capire come tutti questi file si incastrino insieme,
sia che si voglia usare il framework, sia che si cerchi solo di usare il contenitore
di servizi in un'altra applicazione.

Il framework usa il componente HttpKernel per gestire il caricamento della
configurazione del contenitore di servizi dall'applicazione e dai bundle, inoltre
gestisce la compilazione e la cache. Anche se non si usa HttpKernel,
dovrebbe dare un'idea del modo in cui organizzare la configurazione in
un'applicazione modulare.

Lavorare con il contenitore in cache
------------------------------------

Il kernel verifica se c'è una versione in cache del contenitore, prima di
costruirlo. HttpKernel ha un'impostazione di debug, per cui la versione in cache
viene usata se tale impostazione vale ``false``. Se invece debug è ``true``, il kernel
:doc:`verifica se la configurazione è fresca </components/config/caching>` e,
se lo è, la versione in cache è quella del contenitore. Se non lo è, il contenitore viene
costruito a partire dalla configurazione a livello di applicazione e da quella dei
bundle.

Leggere :ref:`esportare la configurazione per le prestazioni <components-dependency-injection-dumping>`
per maggiori dettagli.

Configurazione a livello di applicazione
----------------------------------------

La configurazione a livello di applicazione è caricata dalla cartella ``app/config``.
Vengono caricati più file e quindi fusi, quando le estensioni vengono processate. Ciò
consente di avere configurazioni diverse per ambienti diversi, p.e. dev, prod, ecc.

Questi file contengono parametri e servizi, che sono caricati direttamente nel
contenitore, come in :ref:`impostare il contenitore con file di configurazione<components-dependency-injection-loading-config>`.
Contengono anche configurazioni che sono processate da estensioni, come in
:ref:`gestire la configurazione con le estensioni<components-dependency-injection-extension>`.
Sono considerate configurazioni di bundle, perché ogni bundle contiene una classe
an Extension.

Configurazione a livello di bundle
----------------------------------

Per convenzione, ogni bundle contiene una classe Extension, nella cartella
``DependencyInjection`` del bundle stesso. Queste classi vengono registrare da ``ContainerBuilder``,
al boot del kernel. Quando ``ContainerBuilder`` viene :doc:`compilato</components/dependency_injection/compilation>`,
la configurazione a livello di applicazione rilevante per l'estensione del bundle viene
passata alla classe Extension, che solitamente carica anche i propri file di configurazione, tipicamente
dalla cartella ``Resources/config`` del bundle. La configurazione a livello di applicazione
è solitamente processata con un :doc:`oggetto Configuration </components/config/definition>`, anch'esso
memorizzato nella cartella ``DependencyInjection`` del bundle.

Passi di compilatore per consentire interazioni tra bundle
----------------------------------------------------------

I :ref:`passi di compilatore <components-dependency-injection-compiler-passes>` sono
usati per consentire interazioni tra diversi bundle, poiché non possono influire
a vicenda sulla configurazione nelle classi estensione. Uno degli usi principali è
il processamento dei servizi con tag, consentendo ai bundle di registrare servizi che
possono essere presi da altri bundle, come i logger di Monolog, le estensioni di Twig e
i collettori di dati del Web Profiler. I passi di compilatore sono solitamente posti nella
cartella ``DependencyInjection/Compiler`` del bundle.

Compilazione e cache
--------------------

Dopo che il processo di compilazione ha caricato i servizi dalla configurazione,
dalle estensioni e dai passi di compilatore, viene esportato, in modo che possa essere
usata la cache nella volta successiva. La versione esportata è usata nelle richieste
successive, essendo più efficiente.
