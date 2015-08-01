.. index::
   single: Test

Prestazioni
===========

Symfony è veloce, senza alcuna modifica. Ovviamente, se occorre maggiore velocità,
ci sono molti modi per rendere Symfony ancora più veloce. In questo capitolo
saranno esplorati molti dei modi più comuni e potenti per rendere
un'applicazione Symfony più veloce.

.. index::
   single: Prestazioni; Cache bytecode

Usare una cache bytecode (p.e. APC)
-----------------------------------

Una delle cose migliori (e più facili) che si possono fare per migliorare le prestazioni
è quella di usare una cache bytecode. L'idea di una cache bytecode è di rimuove
l'esigenza di dover ricompilare ogni volta il codice sorgente PHP. Ci sono numerose
`cache bytecode`_  disponibili, alcune delle quali open source. Dalla versione 5.5,
PHP include `OPcache`_. Per versioni precedenti, la cache più usata
è probabilmente `APC`_.

Usare una cache bytecode non ha alcun effetto negativo, e Symfony è stato progettato
per avere prestazioni veramente buone in questo tipo di ambiente.

Ulteriori ottimizzazioni
~~~~~~~~~~~~~~~~~~~~~~~~

Le cache bytecode solitamente monitorano i cambiamenti dei file sorgente. Questo assicura
che, se la sorgente del file cambia, il bytecode sia ricompilato automaticamente.
Questo è molto conveniente, ma ovviamente ha un costo.

Per questa ragione, alcune cache bytecode offrono un'opzione per disabilitare questi
controlli. Ovviamente, quando si disabilitano i controlli, sarà compito dell'amministratore
del server assicurarsi che la cache sia svuotata a ogni modifica dei file sorgente. Altrimenti,
gli aggiornamenti eseguiti non saranno mostrati.

Per esempio, per disabilitare questi controlli in APC, aggiungere semplicemente ``apc.stat=0``
al file di configurazione php.ini.

.. index::
   single: Prestazioni; Autoloader

Usare un autoloader con cache (p.e. ``ApcUniversalClassLoader``)
----------------------------------------------------------------

Per impostazione predefinita, Symfony standard edition usa ``UniversalClassLoader``
nel file `autoloader.php`_. Questo autoloader è facile da usare, perché troverà
automaticamente ogni nuova classe inserita nelle cartelle
registrate.

Sfortunatamente, questo ha un costo, perché il caricatore itera tutti gli spazi dei nomi
configurati per trovare un particolare file, richiamando ``file_exists`` finché
non trova il file cercato.

La soluzione più semplice è dire a Composer di costruire una "mappa di classi" (cioè un
grosso array con le posizioni di tutte le classi). Lo si può fare da
linea di comando e potrebbe diventare parte del processo di deploy:

.. code-block:: bash

    $ composer dump-autoload --optimize

Internamente, costruisce un grosso array di mappature delle classi in ``vendor/composer/autoload_classmap.php``.

Cache dell'autoloader con APC
-----------------------------

Un'altra soluzione è mettere in cache la posizione di ogni classe, dopo che è stata trovata
per la prima volta. Symfony dispone di una classe, :class:`Symfony\\Component\\ClassLoader\\ApcClassLoader`,
che si occupa proprio di questo. Per usarla, basta adattare il file del front controller.
Se si usa la Standard Edition, il codice è già disponibile nel file, ma
commentato::

    // app.php
    // ...

    $loader = require_once __DIR__.'/../app/bootstrap.php.cache';

    // Usa APC per aumentare le prestazioni dell'auto-caricamento
    // Cambiare 'sf2' con il prefisso desiderato
    // per prevenire conflitti di chiavi con altre applicazioni
    /*
    $loader = new ApcClassLoader('sf2', $loader);
    $loader->register(true);
    */

    // ...

Per maggiori dettagli, vedere :doc:`/components/class_loader/cache_class_loader`.

.. note::

    Quando si usa l'autoloader APC, se si aggiungono nuove classi, saranno trovate
    automaticamente e tutto funzionerà come prima (cioè senza motivi per "pulire"
    la cache). Tuttavia, se si cambia la posizione di un particolare spazio dei nomi o
    prefisso, occorrerà pulire la cache di APC. Altrimenti, l'autoloader cercherà
    ancora la classe nella vecchia posizione per tutte le classi in quello
    spazio dei nomi.

.. index::
   single: Prestazioni; File di avvio

Usare i file di avvio
---------------------

Per assicurare massima flessibilità e riutilizzo del codice, le applicazioni Symfony
sfruttano una varietà di classi e componenti di terze parti. Ma il caricamento di tutte
queste classi da diversi file a ogni richiesta può risultate in un overhead. Per ridurre
tale overhead, Symfony Standard Edition fornisce uno script per generare i cosiddetti
`file di avvio`_, che consistono in definizioni di molte classi in un singolo file.
Includendo questo file (che contiene una copia di molte classi del nucleo), Symfony
non avrà più bisogno di includere alcuno dei file sorgente contenuti nelle classi stesse.
Questo riduce un po' la lettura/scrittura su disco.

Se si usa Symfony Standard Edition, probabilmente si usa già un file di avvio.
Per assicurarsene, aprire il front controller (solitamente
``app.php``) e verificare che sia presente la seguente riga::

    require_once __DIR__.'/../app/bootstrap.php.cache';

Si noti che ci sono due svantaggi nell'uso di un file di avvio:

* il file deve essere rigenerato ogni volta che cambia una delle sorgenti originali
  (p.e. quando si aggiorna il sorgente di Symfony o le librerie dei venditori);

* durante il debug, occorre inserire i breakpoint nel file di avvio.

Se si usa Symfony Standard Edition, il file di avvio è ricostruito automaticamente
dopo l'aggiornamento delle librerie dei venditori, tramite il comando ``composer install``.

File di avvio e cache bytecode
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Anche usando una cache bytecode, le prestazioni aumenteranno con l'uso di un file di
avvio, perché ci saranno meno file da monitorare per i cambiamenti. Certamente, se
questa caratteristica è disabilitata nella cache bytecode (p.e. con ``apc.stat=0`` in APC),
non c'è più ragione di usare un file di avvio.

.. _`cache bytecode`: http://en.wikipedia.org/wiki/List_of_PHP_accelerators
.. _`OPcache`: http://php.net/manual/it/book.opcache.php
.. _`APC`: http://php.net/manual/it/book.apc.php
.. _`autoloader.php`: https://github.com/symfony/symfony-standard/blob/master/app/autoload.php
.. _`file di avvio`: https://github.com/sensio/SensioDistributionBundle/blob/master/Composer/ScriptHandler.php
