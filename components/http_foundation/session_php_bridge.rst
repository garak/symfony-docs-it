.. index::
   single: HTTP
   single: HttpFoundation, Sessioni

Integrazione con sessioni legacy
================================

Alle volte potrebbe essere necessario integrare Symfony all'interno di applicazioni 
preesistenti sulle quali non si ha un sufficiente livello di controllo.

Come già visto in altre parti, in Symfony le sessioni sono disegnate in modo da eliminare
sia l'utilizzo delle funzioni native PHP ``session_*()`` che l'utilizzo dell'array superglobale
``$_SESSION``. Per Symfony, inoltre, far partire la sessione è obbligatorio.

Tuttavia, quando ci sono circostanze in cui ciò non sia possibile, si potrà
comunque utilizzare lo speciale bridge per la memorizzazione
:class:`Symfony\\Component\\HttpFoundation\\Session\\Storage\\PhpBridgeSessionStorage`
che è disegnato con l'intento di permettere a Symfony di lavorare con una sessione iniziata all'esterno
dell'ambiente delle sessioni di Symfony. Bisogna comunque fare attenzione perché alcuni fattori potrebbero
inibirne il funzionamento, come nel caso in cui l'applicazione preesistente
dovesse cancellare ``$_SESSION``.

Il seguente esempio ne mostra il tipico utilizzo::

    use Symfony\Component\HttpFoundation\Session\Session;
    use Symfony\Component\HttpFoundation\Session\Storage\PhpBridgeSessionStorage;

    // l'applicazione preesistente configura la sessione
    ini_set('session.save_handler', 'files');
    ini_set('session.save_path', '/tmp');
    session_start();

    // si fa in modo che Symfony si colleghi alla sessione esistente
    $session = new Session(new PhpBridgeSessionStorage());

    // da ora in poi symfony si interfaccia con la sessione PHP esistente
    $session->start();

Tutto ciò consente l'utilizzo delle API per la gestione della sessione di Symfony e permette di
migrare la propria applicazione per utilizzare le sessioni di Symfony.

.. note::

    Le sessioni di Symfony salvano i dati come attributi in una speciale 'sacca' che utilizza
    una chiave nell'array superglobale ``$_SESSION``. Questo vuol dire che la sessione di Symfony
    non puo accedere a chiavi arbitrarie di ``$_SESSION``, le quali potrebbero appartenere all'applicazione
    preesistente, anche se tutto il contenuto di ``$_SESSION`` verrà salvato quando la sessione
    viene salvata.

