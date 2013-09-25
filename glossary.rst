:orphan:

Glossario
=========

.. glossary::
   :sorted:

   Distribuzione
        Una *distribuzione* è una modalità di impacchettamento dei componenti di Symfony2,
        una selezione di bundle, una struttura di cartelle sensibile, una
        configurazione predefinita e un sistema di configurazione opzionale.

   Dependency Injection
        *Dependency Injection* è un design pattern molto usato nel framework Symfony2.
        Incoraggia un'architettura per le applicazioni poco accopiata e più manutenbile.
        Il principio di base di questo pattern sta nel consentire allo sviluppatore di *iniettare* oggetti
        (noti anche come "servizi") in altri oggetti, solitamente passandoli come parametri.
        Si possono stabilire vari livelli di accopiamento tra trali oggetti,
        a seconda del metodo usato per iniettare gli oggetti stessi.
        Il pattern Dependency Injection è spesso associato a un altro
        specifico tipo di oggetto: il :doc:`/book/service_container`.

   Progetto
        Un *progetto* è una cartella composta da un'applicazione, un insieme
        di bundle, librerie di terze parti, un autoloader e alcuni script per
        il front controller del web.

   Applicazione
        Una *applicazione* è una cartella contenente la *configurazione* per
        un dato insieme di bundle.

   Bundle
        Un *bundle* è una cartella con un insieme di file (file PHP, fogli di stile,
        JavaScript, immagini, ...) che *implementa* una singola caratteristica
        (un blog, un forum, eccetera). In Symfony2, (*quasi*) tutto risiede all'interno
        di un bundle. (si veda :ref:`page-creation-bundles`)

   Front controller
        Un *front controller* è un piccolo script PHP che risiede nella cartella web del
        proprio progetto. Solitamente, *ogni* richiesta è gestita eseguendo lo stesso
        front controller, il cui compito è quello di inizializzare l'applicazione
        Symfony.
   
   Controllore
        Un *controllore* è una funzione PHP che ospita tutta la logica necessaria a
        restituire un oggetto ``Response`` che rappresenta una particolare pagina.
        Solitamente, una rotta è mappata su un controllore, che quindi usa informazioni
        dalla richiesta per processare informazioni, eseguire azioni e infine
        costruire e restituire un oggetto ``Response``.

   Servizio
        Un *servizio* è un termine generico per qualsiasi oggetto PHP che esegua un
        compito specifico. Un servizio è solitamente usato "globalmente", come un oggetto
        di connessione a una base dati o un oggetto che invia messaggi email. In Symfony2,
        i servizi sono spesso configurati e recuperati da un contenitore di servizi.
        Un'applicazione con molti servizi non accoppiati segue una
        `architettura orientata ai servizi`_.
        
   Contenitore di servizi
        Un *contenitore di servizi*, conosciuto anche come *Dependency Injection Container*,
        è un oggetto speciale che gestisce le istanze di servizi all'interno di
        un'applicazione. Invece di creare direttamente servizi, lo sviluppatore
        *insegna* al contenitore di servizi (tramite configurazione) come creare
        i servizi. Il contenitore di servizi si occupa dell'istanza pigra e
        dell'inserimento dei servizi dipendenti. Si veda il capitolo
        :doc:`/book/service_container`.

   Specifiche HTTP
        Le *specifiche HTTP* sono contenute in un documento che descrive il protocollo
        di trasferimento, un insieme di regole che delineano le classiche comunicazioni
        di richiesta/risposta tra client e server. La specifica definisce il formato
        usato per una richiesta e una risposta, oltre che i possibili header HTTP che
        ognuna può avere. Per ulteriori informazioni, leggere l'articolo su
        `Wikipedia`_ o la `RFC HTTP 1.1`_.

   Ambiente
        Un ambiente è una stringa (come ``prod`` o ``dev``) che corrisponde a un insieme
        specifico di configurazione. La stessa applicazione può girare sulla stessa
        macchina usando una diversa configurazione, eseguendo l'applicazione in ambienti
        diversi. Questo è molto utile, perché consente a una singola applicazione di
        avere un ambiente ``dev``, costruito per il debug, e un ambiente ``prod``,
        ottimizzato per la velocità.

   Venditore
        Un *venditore* è un fornitore di librerie PHP e di bundle, incluso Symfony2
        stesso. A dispetto delle connotazioni commerciali del termine, i venditori
        in Symfony2 includono spesso e volentieri software libero. Qualsiasi libreria
        si aggiunga al proprio progetto Symfony2 dovrebbe andare nella cartella ``vendor``.
        Si veda :ref:`L'architettura: usare i venditori <using-vendors>`.

   Acme
        *Acme* è un nome fittizio di azienda usato nei demo e nella documentazione di
        Symfony. È usato come spazio dei nomi in punti in cui normalmente andrebbe usato il
        nome della propria azienda (per esempio ``Acme\BlogBundle``).

   Azione
        Un'*azione* è una funzione o un metodo PHP eseguito, per esempio, quando
        una data rotta viene trovata. Il termine "azione" è sinonimo di *controllore*,
        sebbene un controllore possa anche riferirsi a un'intera classe che include molte
        azioni. Si veda il :doc:`capitolo sul controllore </book/controller>`.

   Risorsa
        Una *risorsa* è un componente statico e non eseguibile di un'applicazione web,
        inclusi CSS, JavaScript, immagini e video. Le risorse possono essere inserite
        direttamente nella cartella ``web`` del progetto, oppure pubblicate da un
        :term:`bundle` nella cartella web, usando il task di console ``assets:install``.

   Kernel
        Il *kernel* è il nucleo di Symfony2. L'oggetto kernel gestisce le richieste HTTP,
        usando tutti i bundle e le librerie registrate. Si veda
        :ref:`L'architettura: La cartella delle applicazioni <the-app-dir>` e il
        capitolo :doc:`/book/internals`.

   Firewall
        In Symfony2, un *firewall* non ha a che fare con le reti. Definisce invece
        i meccanismi di autenticazione (ovvero gestisce il processo di determinazione
        dell'identità degli utenti), sia per l'intera applicazione che per le singole
        parti di essa. Si vedano i capitoli
       :doc:`/book/security`.

   Yaml 
        *YAML* è un acronimo ricorsivo, che sta per "YAML Ain't a Markup Language". È un
        linguaggio di serializzazione dei dati leggero e umano, molto usato nei file
        di configurazione di Symfony2. Si veda il capitolo
        :doc:`/components/yaml/introduction`.


.. _`architettura orientata ai servizi`: http://it.wikipedia.org/wiki/Service-oriented_architecture
.. _`Wikipedia`: http://it.wikipedia.org/wiki/Hypertext_Transfer_Protocol
.. _`RFC HTTP 1.1`: http://www.w3.org/Protocols/rfc2616/rfc2616.html
