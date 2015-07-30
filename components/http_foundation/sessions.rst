.. index::
   single: HTTP
   single: HttpFoundation, Sessioni

Gestione della sessione
=======================

Il componente HttpFoundation di Symfony ha un sotto-sistema per le sessioni molto potente
e flessibile, progettato per fornire una gestione delle sessioni tramite una semplice
interfaccia orientata agli oggetti, usando una varietà di driver per memorizzare la sessione.

Le sessioni sono usate tramite la semplice implementazione :class:`Symfony\\Component\\HttpFoundation\\Session\\Session`
dell'interfaccia :class:`Symfony\\Component\\HttpFoundation\\Session\\SessionInterface`.

.. caution::

    Assicurarsi che la sessione di PHP non sia già partita, prima di usare la classe Session.
    Se si ha un vecchio sistema che fa partire una sessione, vedere
    http://symfony.com/doc/current/components/http_foundation/session_php_bridge.html

Un rapido esempio::

    use Symfony\Component\HttpFoundation\Session\Session;

    $session = new Session();
    $session->start();

    // imposta e ottiene attributi di sessione
    $session->set('name', 'Drak');
    $session->get('name');

    // imposta messaggi flash
    $session->getFlashBag()->add('notice', 'Profilo aggiornato');

    // recupera i messaggi
    foreach ($session->getFlashBag()->get('notice', array()) as $message) {
        echo "<div class='flash-notice'>$message</div>";
    }

.. note::

    Le sessioni di Symfony sono pensate per sostituire diverse funzioni native di PHP.
    Le applicazioni devono evitare l'uso di ``session_start()``, ``session_regenerate_id()``,
    ``session_id()``, ``session_name()`` e ``session_destroy()``, usando invece
    le API della sezione seguente.

.. note::

    Pur essendo raccomandato di far partire esplicitamente una sessione, alcune sessioni
    in effetti partiranno su richiesta, nel caso in cui venga eseguita una richiesta di
    leggere o scrivere dati di sessione.

.. warning::

    Le sessioni di Symfony sono incompatibili con la direttiva ini di PHP ``session.auto_start = 1``.
    Tale direttiva andrebbe disattivata in ``php.ini``, nelle direttive del server web
    o in ``.htaccess``.

API delle sessioni
~~~~~~~~~~~~~~~~~~

La classe :class:`Symfony\\Component\\HttpFoundation\\Session\\Session` implementa
:class:`Symfony\\Component\\HttpFoundation\\Session\\SessionInterface`.

La classe :class:`Symfony\\Component\\HttpFoundation\\Session\\Session` ha una semplice API,
suddivisa in un paio di gruppi.

Flusso della sessione
.....................

:method:`Symfony\\Component\\HttpFoundation\\Session\\Session::start`
    Fa partire la sessione. Non usare ``session_start()``.

:method:`Symfony\\Component\\HttpFoundation\\Session\\Session::migrate`
    Rigenera l'id di sessione. Non usare ``session_regenerate_id()``.
    Questo metodo, facoltativamente, può cambiare la scadenza del nuovo cookie, che sarà
    emesso alla chiamata di questo metodo.

:method:`Symfony\\Component\\HttpFoundation\\Session\\Session::invalidate`
    Pulisce i dati della sessione e rigenera la sessione. Non usare ``session_destroy()``.

:method:`Symfony\\Component\\HttpFoundation\\Session\\Session::getId`
    Restituisce l'id della sessione. Non usare ``session_id()``.

:method:`Symfony\\Component\\HttpFoundation\\Session\\Session::setId`
    Imposta l'id della sessione. Non usare ``session_id()``.

:method:`Symfony\\Component\\HttpFoundation\\Session\\Session::getName`
    Restituisce il nome della sessione. Non usare ``session_name()``.

:method:`Symfony\\Component\\HttpFoundation\\Session\\Session::setName`
    Imposta il nome della sessione. Non usare ``session_name()``.

Attributi della sessione
........................

:method:`Symfony\\Component\\HttpFoundation\\Session\\Session::set`
    Imposta un attributo per chiave.

:method:`Symfony\\Component\\HttpFoundation\\Session\\Session::get`
    Restituisce un attributo per chiave.

:method:`Symfony\\Component\\HttpFoundation\\Session\\Session::all`
    Restituisce tutti gli attributi, come array chiave => valore.

:method:`Symfony\\Component\\HttpFoundation\\Session\\Session::has`
    Restituisce ``true`` se l'attributo esiste.

:method:`Symfony\\Component\\HttpFoundation\\Session\\Session::replace`
    Imposta molti attributi contemporaneamente: accetta un array e imposta ogni coppia chiave => valore.

:method:`Symfony\\Component\\HttpFoundation\\Session\\Session::remove`
    Cancella un attributo per chiave.

:method:`Symfony\\Component\\HttpFoundation\\Session\\Session::clear`
    Pulisce tutti gli attributi.

Gli attributi sono memorizzati internamente in un "Bag", un oggetto PHP che agisce come
un array. Ci sono alcuni metodi per la gestione del "Bag":

:method:`Symfony\\Component\\HttpFoundation\\Session\\Session::registerBag`
    Registra una :class:`Symfony\\Component\\HttpFoundation\\Session\\SessionBagInterface`.

:method:`Symfony\\Component\\HttpFoundation\\Session\\Session::getBag`
    Restituisce una :class:`Symfony\\Component\\HttpFoundation\\Session\\SessionBagInterface` per
    nome del bag.

:method:`Symfony\\Component\\HttpFoundation\\Session\\Session::getFlashBag`
    Restituisce la :class:`Symfony\\Component\\HttpFoundation\\Session\\Flash\\FlashBagInterface`.
    Questa è solo una scorciatoia.

Meta-dati della sessione
........................

:method:`Symfony\\Component\\HttpFoundation\\Session\\Session::getMetadataBag`
    Restituisce la :class:`Symfony\\Component\\HttpFoundation\\Session\\Storage\MetadataBag`,
    che contiene informazioni sulla sessione.

Gestori del salvataggio
~~~~~~~~~~~~~~~~~~~~~~~

La gestione delle sessioni di PHP richiede l'uso della variabile ``$_SESSION``,
tuttavia questo interferisce in qualche modo con la testabilità e l'incapsulamento del codice
in un paradigma OOP. Per superare questo problema, Symfony usa delle "bag" di sessione, collegate
alla sessione, che incapsulano dati specifici di "attributi" o "messaggi flash".

Questo approccio mitiga anche l'inquinamento dello spazio dei nomi all'interno di `$_SESSION`,
perché ogni bas memorizza i suoi dati sotto uno spazio dei nomi univoco.
Questo consente a Symfony di coesistere in modo pacifico con altre applicazioni o librerie
che potrebbero usare `$_SESSION`, mantenendo tutti i dati completamente compatibili
con la gestione delle sessioni di Symfony.

Symfony fornisce due tipi di bag, con due implementazioni separate.
Ogni cosa è scritta su interfacce, quindi si può estendere o creare i propri tipi di
bag, se necessario.

:class:`Symfony\\Component\\HttpFoundation\\Session\\SessionBagInterface` ha la
seguente API, intesa principalmente per scopi interni:

:method:`Symfony\\Component\\HttpFoundation\\Session\\SessionBagInterface::getStorageKey`
    restituisce la chiave che il bag memorizzerà nell'array sotto `$_SESSION`.
    In generale questo valore può essere lasciato al suo predefinito ed è per uso interno.

:method:`Symfony\\Component\\HttpFoundation\\Session\\SessionBagInterface::initialize`
    richiamato internamente dalle classi memorizzazione della sessione di Symfony per collegare
    i dati del bag alla sessione.

:method:`Symfony\\Component\\HttpFoundation\\Session\\SessionBagInterface::getName`
    Restituisce il nome del bag della sessione.

Attributi
~~~~~~~~~

Lo scopo dei bag che implementano :class:`Symfony\\Component\\HttpFoundation\\Session\\Attribute\\AttributeBagInterface`
è gestire la memorizzazione degli attributi di sessione. Questo potrebbe includere cose come l'id utente,
le impostazioni "ricordami" o altre informazioni basate sullo stato dell'utente.

:class:`Symfony\\Component\\HttpFoundation\\Session\\Attribute\\AttributeBag`
    è l'implementazione standard predefinita.

:class:`Symfony\\Component\\HttpFoundation\\Session\\Attribute\\NamespacedAttributeBag`
    consente agli attributi di essere memorizzati in uno spazio dei nomi strutturato.

Qualsiasi sistema di memorizzazione `chiave => valore` è limitato riguardo alla complessità
dei dati che possono essere memorizzati, perché ogni chiave deve essere univoca. Si può ottenere
una sorta di spazio di nomi, introducendo una convenzione di nomi nelle chiavi, in modo che
le varie parti dell'applicazioni possano operare senza interferenze. Per esempio, `modulo1.pippo`
e `modulo2.pippo`. Tuttavia, a volte questo non è molto pratico quando gli attributi sono
array, per esempio un insieme di token. In questo caso, gestire l'array diventa pesante,
perché di deve recuperare l'array e poi processarlo e memorizzarlo di
nuovo::

    $tokens = array(
        'tokens' => array(
            'a' => 'a6c1e0b6',
            'b' => 'f4a7b1f3',
        ),
    );

Quindi ogni processamento può rapidamente diventare brutto, persino la semplice aggiunta
di un token all'array::

    $tokens = $session->get('tokens');
    $tokens['c'] = $value;
    $session->set('tokens', $tokens);

Con uno spazio di nomi strutturato, la chiave può essere tradotta nella struttura
dell'array, usando un carattere che crei lo spazio dei nomi (predefinito a `/`)::

    $session->set('tokens/c', $value);

In questo modo si può accedere facilmente a una chiave nell'array direttamente e facilmente.

:class:`Symfony\\Component\\HttpFoundation\\Session\\Attribute\\AttributeBagInterface`
ha una semplice API

:method:`Symfony\\Component\\HttpFoundation\\Session\\Attribute\\AttributeBagInterface::set`
    Imposta un attributo per chiave.

:method:`Symfony\\Component\\HttpFoundation\\Session\\Attribute\\AttributeBagInterface::get`
    Restituisce un attributo per chiave.

:method:`Symfony\\Component\\HttpFoundation\\Session\\Attribute\\AttributeBagInterface::all`
    Restituisce tutti gli attributi come array chiave => valore.

:method:`Symfony\\Component\\HttpFoundation\\Session\\Attribute\\AttributeBagInterface::has`
    Restituisce ``true`` se l'attributo esiste.

:method:`Symfony\\Component\\HttpFoundation\\Session\\Attribute\\AttributeBagInterface::keys`
    Restituisce un array di chiavi di attributi.

:method:`Symfony\\Component\\HttpFoundation\\Session\\Attribute\\AttributeBagInterface::replace`
    Imposta molti attributi contemporaneamente: accetta un array e imposta ogni coppia chiave => valore.

:method:`Symfony\\Component\\HttpFoundation\\Session\\Attribute\\AttributeBagInterface::remove`
    Cancella un attributo per chiave.

:method:`Symfony\\Component\\HttpFoundation\\Session\\Attribute\\AttributeBagInterface::clear`
    Pulisce il bag.

Messaggi flash
~~~~~~~~~~~~~~

Lo scopo di :class:`Symfony\\Component\\HttpFoundation\\Session\\Flash\\FlashBagInterface`
è fornire un modo di impostare e recuperare messaggi basati sulla sessione.
Il flusso dei messaggi flash di solito è impostarli in una richiesta e mostrarli dopo
il rinvio di una pagina. Per esempio, un utente invia un form che esegue un controllore
che aggiorna un dato e dopo il processo il controllore rinvia o alla pagina di
aggiornamento o a quella di errore. I messaggi flash impostati nella pagina precedente
sarebbero mostrati immediatamente nella pagina successiva.
Tuttavia questa è solo una possibile applicazione per i messaggi flash.

:class:`Symfony\\Component\\HttpFoundation\\Session\\Flash\\AutoExpireFlashBag`
    con questa implementazione, i messaggi impostati in una pagina saranno disponibili
    per essere mostrati sono al caricamento della pagina successiva. Tali messaggi
    scadranno automaticamente, che siano stati recuperati o meno.

:class:`Symfony\\Component\\HttpFoundation\\Session\\Flash\\FlashBag`
    con questa implementazione, i messaggi rimarranno i sessione finché non saranno
    esplicitamente recuperati o rimossi. Questo rende possibile l'utilizzo della
    cache ESI.

:class:`Symfony\\Component\\HttpFoundation\\Session\\Flash\\FlashBagInterface`
ha una semplice API

:method:`Symfony\\Component\\HttpFoundation\\Session\\Flash\\FlashBagInterface::add`
    aggiunge un messaggio flash alla pila del tipo specificato.

:method:`Symfony\\Component\\HttpFoundation\\Session\\Flash\\FlashBagInterface::set`
    imposta i flash per tipo. Questo metodo accetta sia messaggi singoli come stringa,
    che messaggi multipli come array.

:method:`Symfony\\Component\\HttpFoundation\\Session\\Flash\\FlashBagInterface::get`
    restituisce i flash per tipo e cancella tali flash dal bag.

:method:`Symfony\\Component\\HttpFoundation\\Session\\Flash\\FlashBagInterface::setAll`
    imposta tutti i flash, accetta un array di array con chiavi ``tipo => array(messaggi)``.

:method:`Symfony\\Component\\HttpFoundation\\Session\\Flash\\FlashBagInterface::all`
    restituisce tutti i flash (come array di array con chiavi) e cancella i flash dal bag.

:method:`Symfony\\Component\\HttpFoundation\\Session\\Flash\\FlashBagInterface::peek`
    restituisce i flash per tipo (sola lettura).

:method:`Symfony\\Component\\HttpFoundation\\Session\\Flash\\FlashBagInterface::peekAll`
    restituisce tutti i flash (sola lettura) come array di array con chiavi.

:method:`Symfony\\Component\\HttpFoundation\\Session\\Flash\\FlashBagInterface::has`
    restituisce ``true`` se il tipo esiste, ``false`` altrimenti.

:method:`Symfony\\Component\\HttpFoundation\\Session\\Flash\\FlashBagInterface::keys`
    restituisce un array di tipi di flash memorizzati.

:method:`Symfony\\Component\\HttpFoundation\\Session\\Flash\\FlashBagInterface::clear`
    pulisce il bag.

Solitamente, per applicazioni semplici basta avere un solo messaggio flash per
tipo, per esempio una nota di conferma dopo l'invio di un form. Tuttavia,
i messaggi flash sono memorizzati in un array per ``$type``, il che vuol dire che
l'applicazione può inviare più messaggi di un dato tipo. Questo consente l'uso dell'API
per messaggi più complessi.

Esempi di impostazioni di flash multipli::

    use Symfony\Component\HttpFoundation\Session\Session;

    $session = new Session();
    $session->start();

    // aggiunge i messaggi flash
    $session->getFlashBag()->add(
        'warning',
        'Il file di config è scrivibile, dovrebbe essere in sola lettura'
    );
    $session->getFlashBag()->add('error', 'Aggiornamento del nome fallito');
    $session->getFlashBag()->add('error', 'Un altro errore');

Si potrebbero mostrare i messaggi in questo modo:

Semplice, mostra un tipo di messaggio::

    // mostra avvertimenti
    foreach ($session->getFlashBag()->get('warning', array()) as $message) {
        echo '<div class="flash-warning">'.$message.'</div>';
    }

    // mostra errori
    foreach ($session->getFlashBag()->get('error', array()) as $message) {
        echo '<div class="flash-error">'.$message.'</div>';
    }

Metodo compatto per processare la visualizzazione di tutti i flash in un colpo solo::

    foreach ($session->getFlashBag()->all() as $type => $messages) {
        foreach ($messages as $message) {
            echo '<div class="flash-'.$type.'">'.$message.'</div>';
        }
    }
