.. index::
   single: Stopwatch
   single: Componenti; Stopwatch

Il componente Stopwatch
=======================

Il componente Stopwatch fornisce uno strumento per profilare il codice.

.. versionadded:: 2.2
    Il componente Stopwatch è stato introdotto in Symfony 2.2. Precedentemente, la classe
    ``Stopwatch`` si trovava nel componente HttpKernel (ed è stata introdotta
    in Symfony 2.1).

Installazione
-------------

Si può installare il componente in due modi:

* :doc:`Installarlo tramite Composer</components/using_components>` (``symfony/stopwatch`` su `Packagist`_);
* Usare il repository Git ufficiale (https://github.com/symfony/Stopwatch).

.. include:: /components/require_autoload.rst.inc

Uso
---

Il componente Stopwatch fornisce un modo facile e coerente di misurare il tempo di esecuzione
di alcune parti di codice, in modo da non dover continuamente analizzare i
microtime da soli. Basta usare la semplice classe
:class:`Symfony\\Component\\Stopwatch\\Stopwatch`::

    use Symfony\Component\Stopwatch\Stopwatch;

    $stopwatch = new Stopwatch();
    // Inizia l'evento chiamato 'nomeEvento'
    $stopwatch->start('nomeEvento');
    // ... un po' di codice
    $event = $stopwatch->stop('nomeEvento');

Si può recuperare l'oggetto :class:`Symfony\\Component\\Stopwatch\StopwatchEvent`
dai metodi :method:`Symfony\\Component\\Stopwatch\\Stopwatch::start`, 
:method:`Symfony\\Component\\Stopwatch\\Stopwatch::stop` e
:method:`Symfony\\Component\\Stopwatch\\Stopwatch::lap`.

Si può anche fornire un nome di categoria per un evento::

    $stopwatch->start('nomeEvento', 'nomeCategoria');

Si può pensare alle categorie come tag per gli eventi. Per esempio, il
profilatore di Symfony usa le categorie per colorare diversamente il codice di vari eventi.

Periodi
-------

Come sappiamo dal mondo reale, tutti i cronometri hanno due bottoni:
uno per far partire e fermare il cronometro, l'altro per per le frazioni di tempo.
Questo è esattamente ciò che il metodo :method:`Symfony\\Component\\Stopwatch\\Stopwatch::lap`
fa::

    $stopwatch = new Stopwatch();
    // Inizia un evento di nome 'pippo'
    $stopwatch->start('pippo');
    // ... un po' di codice
    $stopwatch->lap('pippo');
    // ... un po' di codice
    $stopwatch->lap('pippo');
    // ... un altro po' di codice
    $event = $stopwatch->stop('pippo');

Le informazioni sulle frazioni sono memorizzate come "periodi" dell'evento. Per ottenere informazioni
sulle frazioni, richiamare::

    $event->getPeriods();

Oltre ai periodi, si possono ottenere informazioni utili dall'oggetto evento.
Per esempio::

    $event->getCategory();   // Restituisce la categoria dell'evento
    $event->getOrigin();     // Restituisce il tempo di inizio dell'evento, in millisecondi
    $event->ensureStopped(); // Ferma tutti i periodi ancora in corso
    $event->getStartTime();  // Restituisce il tempo di inizio del primo periodo
    $event->getEndTime();    // Restituisce il tempo di inizio dell'ultimo periodo
    $event->getDuration();   // Restituisce la durata dell'evento, inclusi tutti i periodi
    $event->getMemory();     // Restituisce l'utilizzo massimo di memoria di tutti i periodi

Sezioni
-------

Le sezioni sono un modo per suddividere logicamente la linea temporale in gruppi. Si possono
vedere come Symfony usa le sezioni per visualizzare il ciclo di vita del framework
nel profilatore. Ecco un esempio di uso di base delle sezioni::

    $stopwatch = new Stopwatch();

    $stopwatch->openSection();
    $stopwatch->start('parsing_config_file', 'filesystem_operations');
    $stopwatch->stopSection('routing');

    $events = $stopwatch->getSectionEvents('routing');

Si può riaprire una sezione chiusa, richiamando il metodo :method:`Symfony\\Component\\Stopwatch\\Stopwatch::openSection`
e specificando l'id della sezione da riaprire::

    $stopwatch->openSection('routing');
    $stopwatch->start('building_config_tree');
    $stopwatch->stopSection('routing');

.. _Packagist: https://packagist.org/packages/symfony/stopwatch
