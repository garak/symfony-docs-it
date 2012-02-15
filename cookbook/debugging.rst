.. index::
   single: Debug

Come ottimizzare l'ambiente di sviluppo per il debug
====================================================

Lavorando a un progetto Symfony sulla propria macchina locale, si dovrebbe 
utilizzare l'ambiente ``dev`` (il front controller ``app_dev.php``). La configurazione 
di questo ambiente è ottimizzata per due scopi principali:

 * Dare accurate informazioni al programmatore quando qualcosa non funziona (barra
   web di debug, chiare pagine delle eccezzioni, misurazione delle prestazioni, ...);

 * Essere più simile possibile all'ambiente di produzione per evitare spiacevoli 
   sorprese nel momento del rilascio del progetto.

.. _cookbook-debugging-disable-bootstrap:

Disabilitare il file di avvio e la cache delle classi
-----------------------------------------------------

Per rendere l'ambiente di produzione il più veloce possibile, Symfony crea 
un unico file PHP, all'interno della cache, che raccoglie tutte le classi PHP 
di cui ha bisogno il progetto. Un comportamento che potrebbe però confondere 
l'IDE o il debugger. Questa ricetta mostrerà come modificare il meccanismo di
gestione della cache per rendere più agevole il debug del codice relativo 
alle classi di Symfony.

Il front controller ``app_dev.php`` contiene, nella sua versione predefinita, il seguente codice::

    // ...

    require_once __DIR__.'/../app/bootstrap.php.cache';
    require_once __DIR__.'/../app/AppKernel.php';

    use Symfony\Component\HttpFoundation\Request;

    $kernel = new AppKernel('dev', true);
    $kernel->loadClassCache();
    $kernel->handle(Request::createFromGlobals())->send();

Per facilitare il lavoro del debugger, è possibile disabilitare la cache di
tutte le classi PHP rimuovendo la chiamata a ``loadClassCache()`` e sostituendo 
la dichirazione del require, nel seguente modo::

    // ...

    // require_once __DIR__.'/../app/bootstrap.php.cache';
    require_once __DIR__.'/../vendor/symfony/src/Symfony/Component/ClassLoader/UniversalClassLoader.php';
    require_once __DIR__.'/../app/autoload.php';
    require_once __DIR__.'/../app/AppKernel.php';

    use Symfony\Component\HttpFoundation\Request;

    $kernel = new AppKernel('dev', true);
    // $kernel->loadClassCache();
    $kernel->handle(Request::createFromGlobals())->send();

.. tip::

    Una volta disabilitata la cache delle classi PHP, non bisogna dimenticare di riabilitarla 
    alla fine della sessione di debug.

Alcuni IDE non gradiscono il fatto che certe classi siano salvate in posti differenti. 
Per evitare problemi, è possibile o configurare l'IDE per ignorare i file PHP della cache 
oppure modificare l'estensione che Symfony assegna a questi file::

    $kernel->loadClassCache('classes', '.php.cache');
