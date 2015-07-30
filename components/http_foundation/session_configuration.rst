.. index::
   single: HTTP
   single: HttpFoundation, Sessioni

Configurare sessioni e gestori di salvataggio
=============================================

Questa sezione tratta la configurazione della gestione della sessione e la messa a punto
secondo esigenze specifiche. Questa documentazione copre alcuni gestori, che memorizzano
e recuperano dati di sessione, e la configurazione del comportamento della sessione.

Gestori del salvataggio
~~~~~~~~~~~~~~~~~~~~~~~

Il flusso della sessione di PHP ha sei possibili operazioni da eseguire. La sessione normale
segue ``open``, ``read``, ``write`` e ``close``, con la possibilità di
``destroy`` e ``gc`` (garbage collection, che fa scadere le vecchie sessioni:
``gc`` viene richiamato in modo casuale, secondo la configurazione di PHP, e, se richiamato, è
invocato dopo l'operazione ``open``). Si può approfondire l'argomento su
`php.net/session.customhandler`_

Gestori del salvataggio nativi di PHP
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

I gestori cosiddetti 'nativi' sono gestori di sessione che sono o compilati in
PHP o forniti da estensioni di PHP, come PHP-Sqlite, PHP-Memcached e così via.

Tutti i gestori di salvataggio nativi sono interni a PHP e quindi non hanno API pubbliche.
Vanno configurati tramite direttive ini, solitamente ``session.save_path``, e
potenzialmente altre direttive specifiche. Si possono trovare altri dettagli nei docblock
dei metodi ``setOptions()`` di ciascuna classe. Per esempio, quello
fornito dall'estensione Memcached si può trovare in `php.net/memcached.setoption`_.

Sebbene i gestori di salvataggio nativi possano essere attivati direttamente, usando
``ini_set('session.save_handler', $nome);``, Symfony fornisce un modo conveniente
per attivarli nello stesso modo dei gestori personalizzati.

Symfony fornisce driver per i gestori nativi, come per esempio:

  * :class:`Symfony\\Component\\HttpFoundation\\Session\\Storage\\Handler\\NativeFileSessionHandler`

Esempio di utilzzo::

    use Symfony\Component\HttpFoundation\Session\Session;
    use Symfony\Component\HttpFoundation\Session\Storage\NativeSessionStorage;
    use Symfony\Component\HttpFoundation\Session\Storage\Handler\NativeFileSessionHandler;

    $storage = new NativeSessionStorage(array(), new NativeFileSessionHandler());
    $session = new Session($storage);

.. note::

    Con l'eccezione del gestore ``files``, nativo di PHP e sempre disponibile,
    la disponibilità di altri gestori dipende da quali estensioni PHP
    sono attive a runtime.

.. note::

    I gestori di salvataggio nativi forniscono una soluzione rapida alla memorizzazione di sessioni, tuttavia
    in sistemi complessi, in cui occorre maggior controllo, i gestori di salvataggio personalizzati possono
    fornire più libertà e flessibilità. Symfony fornisce varie implementazioni,
    personalizzabili a piacimento.

Gestori di salvataggio personalizzati
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

I gestori personalizzati sono quelli che sostituiscono completamente i gestori del salvataggio
nativi di PHP, fornendo sei funzioni di callback, richiamate internamente da PHP in vari
punti del flusso della sessione.

HttpFoundation di Symfony ne fornisce alcuni predefiniti, che possono facilmente servire
da esempi, se se ne vuole scrivere uno.

* :class:`Symfony\\Component\\HttpFoundation\\Session\\Storage\\Handler\\PdoSessionHandler`
* :class:`Symfony\\Component\\HttpFoundation\\Session\\Storage\\Handler\\MemcacheSessionHandler`
* :class:`Symfony\\Component\\HttpFoundation\\Session\\Storage\\Handler\\MemcachedSessionHandler`
* :class:`Symfony\\Component\\HttpFoundation\\Session\\Storage\\Handler\\MongoDbSessionHandler`
* :class:`Symfony\\Component\\HttpFoundation\\Session\\Storage\\Handler\\NullSessionHandler`

Esempio::

    use Symfony\Component\HttpFoundation\Session\Session;
    use Symfony\Component\HttpFoundation\Session\Storage\NativeSessionStorage;
    use Symfony\Component\HttpFoundation\Session\Storage\Handler\PdoSessionHandler;

    $pdo = new \PDO(...);
    $storage = new NativeSessionStorage(array(), new PdoSessionHandler($pdo));
    $session = new Session($storage);

Configurare le sessioni di PHP
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

:class:`Symfony\\Component\\HttpFoundation\\Session\\Storage\\NativeSessionStorage` può
configurare la maggior parte delle direttive di php.ini documentate su
`php.net/session.configuration`_.

Per configurare tali impostazioni, passare le chiavi (omettendo la parte ``session.`` iniziale
della chiave) come array chiave-valore al parametro ``$options`` del costruttore.
Oppure impostarle tramite il metodo
:method:`Symfony\\Component\\HttpFoundation\\Session\\Storage\\NativeSessionStorage::setOptions`
.

Per ulteriore chiarezza, alcune chiavi di opzioni sono spiegate in questa documentazione.

Scadenza del cookie di sessione
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Per sicurezza, generalmente si raccomanda di inviare i token di sessione come cookie.
SI può configurare la scadenza dei cookie di sessione, specificando il tempo
(in secondi) usando la chiave ``cookie_lifetime`` nel parametro ``$options`` del
costruttore di :class:`Symfony\\Component\\HttpFoundation\\Session\\Storage\\NativeSessionStorage`.

Impostare ``cookie_lifetime`` a ``0`` farà sì che il cookie durerà solo finché il
browser non resta aperto. Generalmente, ``cookie_lifetime`` andrebbe impostato a
un numero relativamente grande di giorni, settimane o mesi. Non è raro impostare i
cookie per un anno o più, a seconda dell'applicazione.

Essendo i cookie di sessione dei token esclusivamente lato client, sono meno importanti
nel controllo dei dettagli rispetto alle impostazioni di sicurezza, che alla fine possono
essere controllate con tranquillità solamente lato server.

.. note::

    L'impostazione ``cookie_lifetime`` è il numero di secondi per cui il cookie sarà
    valido, non è un timestamp Unix. Il cookie di sessione risultante sarà emesso con
    un tempo di scadenza di ``time()`` + ``cookie_lifetime``, con riferimento alla
    data del server.

Configurare il garbage collector
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Quando si apre una sessione, PHP richiamerà ``gc`` in modo casuale, in base alla
probabilità impostata da ``session.gc_probability`` / ``session.gc_divisor``. Per
esempio, se impostati rispettivamente a ``5/100``, risulterebbe in una probabilità
del 5%. In modo simile, ``3/4`` vorrebbe dire 3 possibilità su 4 di essere richiamato, quindi il 75%.

Se il garbage collector viene invocato, PHP passerà il valore memorizzato nella
direttiva php.ini ``session.gc_maxlifetime``. Il significato in questo contesto è
che ogni sessione memorizzata prima di ``maxlifetime`` secondi fa andrebbe
cancellata. Questo consente di far scadere le sessioni in base al tempo di inattività.

Si possono impostare queste impostazioni passando ``gc_probability``, ``gc_divisor``
e ``gc_maxlifetime`` in un array al costruttore di
:class:`Symfony\\Component\\HttpFoundation\\Session\\Storage\\NativeSessionStorage`
o al metodo :method:`Symfony\\Component\\HttpFoundation\\Session\\Storage\\NativeSessionStorage::setOptions`.


Scadenza della sessione
~~~~~~~~~~~~~~~~~~~~~~~

Quando viene creata una nuova sessione, quindi quando Symfony invia un nuovo cookie di
sessione al client, il cookie sarà emesso con un tempo di scadenza. Questo tempo è
calcolato aggiungendo il valore di configurazione di PHP in
``session.cookie_lifetime`` al tempo attuale del server.

.. note::

    PHP invierà un cookie una sola volta. Ci si aspetta che il client memorizzi tale
    cookie per l'intero periodo. Sarà inviato un nuovo cookie solo quando la sessione
    viene distrutta, il cookie viene cancellato dal browser oppure l'id della sessione
    viene rigenerato, usando i metodi ``migrate()`` o ``invalidate()`` della classe ``Session``.

    Il tempo di scadenza iniziale del cookie può essere impostato configurando ``NativeSessionStorage``,
    usando il metodo ``setOptions(array('cookie_lifetime' => 1234))``.

.. note::

    Un tempo di scadenza del cookie di ``0`` vuol dire che il cookie scadrà alla chiusura del browser.

Tempo di inattività della sessione
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Spesso, ci sono circostanze in cui si vuole proteggere l'uso della sessione oppure
minimizzarne l'uso non autorizzato, quando un utente si allontana dalla sua postazione
mentre è loggato, distruggendo la sessione dopo un certo periodo di inattività. Per
esempio, solitamente le applicazioni delle banche buttano fuori l'utente dopo appena 5
o 10 minuti di inattività. L'impostazione della scadenza del cookie, in questo caso, non
è appropriata, perché potrebbe essere manipolata dal client, quindi occorre farlo
scadere lato server. Il modo più facile di farlo è tramite il garbage collector, che viene
eseguito con una frequenza ragionevole. Il ``lifetime`` del cookie andrebbe impostato a
un valore relativamente alto e il ``maxlifetime`` del garbage collector andrebbe impostato
per distruggere le sessioni al periodo di inattività desiderato.

L'altra opzione è verificare specificatamente se una sessione sia scaduta dopo che
la sessione parte. La sessione può essere distrutta, come richiesto. Questo metodo può
consentire di integrare la scadenza delle sessioni nell'esperienza utente, per esempio,
mostrando un messaggio.

Symfony registra alcuni meta-dati di base su ogni sessione, per dare completa libertà
in quest'area.

Limitare la cache della sessione
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Per evitare che gli utenti vedano dati vecchi, di solito si inviano risorse in cui compare la sessione
con header che disabilitano la cache. Per questo scopo, le sessioni di PHP hanno un'opzione
``sessions.cache_limiter``, che determina quali header eventualmente inviare
nella risposta, quando inizia una sessione.

Dopo la costruzione,
:class:`Symfony\\Component\\HttpFoundation\\Session\\Storage\\NativeSessionStorage`
imposta questa opzione globale a ``""`` (non inviare header), in caso lo sviluppatore voglia
usare un oggetto :class:`Symfony\\Component\\HttpFoundation\\Response` per gestire gli
header della risposta.

.. caution::

    Se ci si basa sulle sessioni di PHP per gestire la cache HTTP, si *deve* impostare manualmente l'opzione
    ``cache_limiter`` in
    :class:`Symfony\\Component\\HttpFoundation\\Session\\Storage\\NativeSessionStorage`
    a un valore non vuoto.

    Per esempio, la si potrebbe impostare al valore predefinito di PHP durante la costruzione:

    Esempio di utilizzo::

        use Symfony\Component\HttpFoundation\Session\Storage\NativeSessionStorage;

        $options['cache_limiter'] = session_cache_limiter();
        $storage = new NativeSessionStorage($options);

Meta-dati di sessione
~~~~~~~~~~~~~~~~~~~~~

Le sessioni vengono decorate da alcuni meta-dati di base, per consentire maggior controllo
sulle impostazioni di sicurezza. L'oggetto sessione ha un getter per i meta-dati,
:method:`Symfony\\Component\\HttpFoundation\\Session\\Session::getMetadataBag`, che
espone un'istanza di :class:`Symfony\\Component\\HttpFoundation\\Session\\Storage\\MetadataBag`::

    $session->getMetadataBag()->getCreated();
    $session->getMetadataBag()->getLastUsed();

Entrambi i metodi restituiscono un timestamp Unix (relativo al server).

Questi meta-dati possono essere usati per far scadere in modo espliciti una sessione all'accesso, p.e.::

    $session->start();
    if (time() - $session->getMetadataBag()->getLastUsed() > $maxIdleTime) {
        $session->invalidate();
        throw new SessionExpired(); // rinvia alla pagina di sessione scaduta
    }

Si può anche specificare a cosa è stato impostato ``cookie_lifetime`` per un determinato
cookie, usando il metodo ``getLifetime()``::

    $session->getMetadataBag()->getLifetime();

Il tempo di scadenza del cookie può essere determinato aggiungendo il timestamp creato
e il lifetime.

Compatibilità con PHP 5.4
~~~~~~~~~~~~~~~~~~~~~~~~~

A partire da PHP 5.4.0, sono disponibili :phpclass:`SessionHandler` e :phpclass:`SessionHandlerInterface`.
Symfony 2.1 fornisce compatibilità in avanti per :phpclass:`SessionHandlerInterface`,
in modo che possa essere usata con PHP 5.3. Questo aumenta molto l'interoperabilità con altre
librerie.

:phpclass:`SessionHandler` è una classe interna speciale di PHP, che espone i gestori del
salvataggio nativi nello user space di PHP.

Per poter fornire una soluzione a chi usa PHP 5.4, Symfony ha una classe speciale,
chiamata :class:`Symfony\\Component\\HttpFoundation\\Session\\Storage\\Handler\\NativeSessionHandler`,
che sotto PHP 5.4 estende da `\SessionHandler` e sotto PHP 5.3 è solo una classe
di base vuota. Questo dà alcune interessanti opportunità, per sfruttare le
funzionalità di PHP 5.4, se disponibile.

Proxy per il gestore del salvataggio
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Un proxy per il gestore del salvataggio di base avvolge un gestore di salvataggio, che è
stato introdotto per supportare senza problemi la migrazione da PHP 5.3 a PHP 5.4+.
Inoltre, crea un punto di estensione da cui si può aggiungere una logica personalizzata, che
funzioni indipendentemente da quale gestore sia all'interno.

Ci sono due tipi di classi di proxy per il gestore del salvataggio, che ereditano da
:class:`Symfony\\Component\\HttpFoundation\\Session\\Storage\\Handler\\AbstractProxy`:
sono :class:`Symfony\\Component\\HttpFoundation\\Session\\Storage\\Handler\\NativeProxy`
e :class:`Symfony\\Component\\HttpFoundation\\Session\\Storage\\Handler\\SessionHandlerProxy`.

:class:`Symfony\\Component\\HttpFoundation\\Session\\Storage\\NativeSessionStorage`
inietta automaticamente i gestori della memorizzazione in un proxy per il gestore del
salvataggio, a meno che non ce ne sia già uno che lo avvolge.

:class:`Symfony\\Component\\HttpFoundation\\Session\\Storage\\Handler\\NativeProxy`
è usato automaticamente sotto PHP 5.3, quando i gestori del salvataggio interni di PHP
vengono specificati tramite le classi `Native*SessionHandler` classes, mentre
:class:`Symfony\\Component\\HttpFoundation\\Session\\Storage\\Handler\\SessionHandlerProxy`
sarà usato per avvolgere qualsiasi gestore del salvataggio personalizzato, che implementi :phpclass:`SessionHandlerInterface`.

A partire da PHP 5.4, tutti i gestori di sessione implementano :phpclass:`SessionHandlerInterface`,
incluse le classi `Native*SessionHandler` che ereditano da :phpclass:`SessionHandler`.

Il meccanismo del proxy consente di essere coinvolto in modo più approfondito nelle classi
dei gestori del salvataggio. Un proxy, per esempio, può essere usato per criptare ogni
transazione di una sessione, senza sapere nulla del gestore del salvataggio specifico.

.. note::

    Prima di PHP 5.4, si possono usare proxy di gestori di salvataggio personalizzati,
    non quelli nativi di PHP.

.. _`php.net/session.customhandler`: http://php.net/session.customhandler
.. _`php.net/session.configuration`: http://php.net/session.configuration
.. _`php.net/memcached.setoption`: http://php.net/memcached.setoption
