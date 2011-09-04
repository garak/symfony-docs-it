.. index::
   single: Test

Prestazioni
===========

Symfony2 è veloce, senza alcuna modifica. Ovviamente, se occorre maggiore velocità,
ci sono molti modi per rendere Symfony2 ancora più veloce. In questo capitolo,
saranno esplorati molti dei modi più comuni e potenti per rendere la propria
applicazione Symfony più veloce.

.. index::
   single: Prestazioni; Cache bytecode

Usare una cache bytecode (p.e. APC)
-----------------------------------

Una delle cose migliori (e più facili) che si possono fare per migliorare le prestazioni
è quella di usare una cache bytecode. L'idea di una cache bytecode è di rimuove
l'esigenza di dover ricompilare ogni volta il codice sorgente PHP. Ci sono numerose
`cache bytecode`_  disponibili, alcune delle quali open source. La più usata
è probabilmente `APC`_.

Usare una cache bytecode non ha alcun effetto negativo, e Symfony2 è stato progettato
per avere prestazioni veramente buone in questo tipo di ambiente.

Ulteriori ottimizzazioni
~~~~~~~~~~~~~~~~~~~~~~~~

Le cache bytecode solitamente monitorano i cambiamenti dei file sorgente. Questo assicura
che, se la sorgente del file cambia, il bytecode sia ricompilato automaticamente.
Questo è molto conveniente, ma ovviamente aggiunge un overhead.

Per questa ragione, alcune cache bytecode offrono un'opzione per disabilitare questi
controlli. Ovviamente, quando si disabilitano i controlli, sarà compito dell'amministratore
del server assicurarsi che la cache sia svuotata a ogni modifica dei file sorgente. Altrimenti,
gli aggiornamenti eseguiti non saranno mostrati.

Per esempio, per disabilitare questi controlli in APC, aggiungere semplicemente ``apc.stat=0``
al proprio file di configurazione php.ini.

.. index::
   single: Prestazioni; Autoloader

Usare un autoloader con caches (p.e. ``ApcUniversalClassLoader``)
-----------------------------------------------------------------

Per impostazione predefinita, Symfony2 standard edition usa ``UniversalClassLoader``
nel file `autoloader.php`_. Questo autoloader è facile da usare, perché troverà
automaticamente ogni nuova classe inserita nelle cartella registrate.

Sfortunatamente, questo ha un costo, perché il caricatore itera tutti i namespace
configurati per trovare un particolare file, richiamando ``file_exists`` finché
non trova il file cercato.


La soluzione più semplice è mettere in cache la posizione di ogni classe, dopo che
è stata trovata per la prima volta. Symfony dispone di una classe di caricamento,
``ApcUniversalClassLoader``, che estende ``UniversalClassLoader`` e memorizza le
posizioni delle classi in APC.

Per usare questo caricatore, basta adattare il file ``autoloader.php`` come segue:

.. code-block:: php

    // app/autoload.php
    require __DIR__.'/../vendor/symfony/src/Symfony/Component/ClassLoader/ApcUniversalClassLoader.php';

    use Symfony\Component\ClassLoader\ApcUniversalClassLoader;

    $loader = new ApcUniversalClassLoader('some caching unique prefix');
    // ...

.. note::

    Quando si usa l'autoloader APC, se si aggiungono nuove classi, saranno trovate
    automaticamente e tutto funzionerà come prima (cioè senza motivi per "pulire"
    la cache). Tuttavia, se si cambia la posizione di un particolare namespace o
    prefisso, occorrerà pulire la cache di APC. Altrimenti, l'autoloader cercherà
    ancora la classe nella vecchia posizione per tutte le classi in quel
    namespace.

.. index::
   single: Prestazioni; file di avvio

Usare i file di avvio
---------------------

Per assicurare massima flessibilità e riutilizzo del codice, le applicazioni Symfony2
sfruttano una varietà di classi e componenti di terze parti. Ma il caricamento di tutte
queste classi da diversi file a ogni richiesta può risultate in un overhead. Per ridurre
tale overhead, Symfony2 Standard Edition fornisce uno script per generare i cosiddetti
`file di avvio`_, che consistono in definizioni di molte classi in un singolo file.
Includendo questo file (che contiene una copia di molte classi del nucleo), Symfony
non avrà più bisogno di includere alcuno dei file sorgente contenuti nelle classi stesse.
Questo riduce un po' la lettura/scrittura su disco.

Se si usa Symfony2 Standard Edition, probabilmente si usa già un file di avvio.
Per assicurarsene, aprire il proprio front controller (solitamente
``app.php``) e verificare che sia presente la seguente riga::

    require_once __DIR__.'/../app/bootstrap.php.cache';

Si noti che ci sono due svantaggi nell'uso di un file di avvio:

* il file deve essere rigenerato ogni volta che cambia una delle sorgenti originali
  (p.e. quando si aggiorna il sorgente di Symfony2 o le librerie dei venditori);

* durante il debug, occorre inserire i breakpoint nel file di avvio.

Se si usa Symfony2 Standard Edition, il file di avvio è ricostruito automaticamente
dopo l'aggiornamento delle librerie dei venditori, tramite il comando
``php bin/vendors install``.

File di avvio e cache bytecode
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Anche usando una cache bytecode, le prestazioni aumenteranno con l'uso di un file di
avvio, perché ci saranno meno file da monitorare per i cambiamenti. Certamente, se
questa caratteristica è disabilitata nella cache bytecode (p.e. con ``apc.stat=0`` in APC),
non c'è più ragione di usare un file di avvio.

.. _`cache bytecode`: http://en.wikipedia.org/wiki/List_of_PHP_accelerators
.. _`APC`: http://php.net/manual/en/book.apc.php
.. _`autoloader.php`: https://github.com/symfony/symfony-standard/blob/master/app/autoload.php
.. _`file di avvio`: https://github.com/sensio/SensioDistributionBundle/blob/master/Resources/bin/build_bootstrap.php
