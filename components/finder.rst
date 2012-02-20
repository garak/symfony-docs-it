.. index::
   single: Finder

Il componente Finder
====================

   Il componente Finder cerca file e cartelle tramite un'interfaccia intuitiva 
   e "fluida".

Installazione
-------------

È possibile installare il componente in diversi modi:

* Utilizzando il repository ufficiale su Git (https://github.com/symfony/Finder);
* Installandolo via PEAR ( `pear.symfony.com/Finder`);
* Installandolo tramite Composer (`symfony/finder` su Packagist).

Utilizzo
--------

La classe :class:`Symfony\\Component\\Finder\\Finder` trova i file e/o le
cartelle::

    use Symfony\Component\Finder\Finder;

    $finder = new Finder();
    $finder->files()->in(__DIR__);

    foreach ($finder as $file) {
        // Stampa il percorso assoluto
        print $file->getRealpath()."\n";
        // Stampa il percorso relativo del file, omettendo il nome del file stesso
        print $file->getRelativePath()."\n";
        // Stampa il percorso relativo del file
        print $file->getRelativePathname()."\n";
    }

``$file`` è un'istanza di :class:`Symfony\\Component\\Finder\\SplFileInfo`
la quale estende :phpclass:`SplFileInfo` che mette a disposizione i metodi per 
poter lavorare con i percorsi relativi.

Il precedente codice stampa, ricorsivamente, i nomi di tutti i file della
cartella corrente. La classe Finder implementa il concetto di interfaccia fluida, perciò tutti
i metodi restituiscono un'istanza di Finder.

.. tip::

    Un Finder è un'istanza di un `Iterator`_ PHP. Perciò, invece di dover iterare attraverso
    Finder con un ciclo ``foreach``, è possibile convertirlo in un array, tramite il metodo
    :phpfunction:`iterator_to_array`, o ottenere il numero di oggetto in esso contenuti con
    :phpfunction:`iterator_count`.

Criteri
-------

Posizione
~~~~~~~~~

La posizione è l'unico parametro obbligatorio. Indica al finder la cartella da
utilizzare come base per la ricerca::

    $finder->in(__DIR__);

Per cercare in diverse posizioni, è possibile concatenare diverse chiamate a
:method:`Symfony\\Component\\Finder\\Finder::in`::

    $finder->files()->in(__DIR__)->in('/altraparte');

È possibile escludere cartelle dalla ricerca tramite il metodo
:method:`Symfony\\Component\\Finder\\Finder::exclude`::

    $finder->in(__DIR__)->exclude('ruby');

Visto che Finder utilizza gli iteratori di PHP, è possibile passargli qualsiasi
URL che sia supportata dal `protocollo`_::

    $finder->in('ftp://example.com/pub/');

Funziona anche con flussi definiti dall'utente::

    use Symfony\Component\Finder\Finder;

    $s3 = new \Zend_Service_Amazon_S3($chiave, $segreto);
    $s3->registerStreamWrapper("s3");

    $finder = new Finder();
    $finder->name('photos*')->size('< 100K')->date('since 1 hour ago');
    foreach ($finder->in('s3://bucket-name') as $file) {
        // fare qualcosa

        print $file->getFilename()."\n";
    }

.. note::

    Per approfondire l'argomento su come creare flussi personalizzati, si legga la documentazione degli `stream`_.

File o cartelle
~~~~~~~~~~~~~~~

Il comportamento predefinito di Finder è quello di restituire file e cartelle, ma
grazie ai metodi :method:`Symfony\\Component\\Finder\\Finder::files` e
:method:`Symfony\\Component\\Finder\\Finder::directories`, è possibile raffinare i risultati::

    $finder->files();

    $finder->directories();

Per seguire i collegamenti è possibile utilizzare il metodo ``followLinks()``::

    $finder->files()->followLinks();

Normalmente l'iteratore ignorerà i popolari file VCS. È possibile modificare questo
comportamento grazie al metodo ``ignoreVCS()``::

    $finder->ignoreVCS(false);

Ordinamento
~~~~~~~~~~~

È possibile ordinare i risultati per nome o per tipo (prima le cartelle e poi i file)::

    $finder->sortByName();

    $finder->sortByType();

.. note::

    Si noti che i metodi ``sort*``, per poter funzionare, richiedono tutti gli 
    elementi ricercati. In caso di iteratori molto grandi, l'ordinamento potrebbe risultare lento.

È anche possibile definire algoritmi di ordinamento personalizzati grazie al metodo ``sort()``::

    $sort = function (\SplFileInfo $a, \SplFileInfo $b)
    {
        return strcmp($a->getRealpath(), $b->getRealpath());
    };

    $finder->sort($sort);

Nomi dei file
~~~~~~~~~~~~~

È eseguire filtri sui nomi dei file utilizzando il metodo
:method:`Symfony\\Component\\Finder\\Finder::name`::

    $finder->files()->name('*.php');

Il metodo ``name()`` accetta, come parametri, glob, stringhe, o espressioni regolari::

    $finder->files()->name('/\.php$/');

Il metodo ``notNames()`` viene invece usato per escludere i file che corrispondono allo schema::

    $finder->files()->notName('*.rb');

Dimensione del file
~~~~~~~~~~~~~~~~~~~

Per filtrare i file in base alla dimensione, si usa il metodo
:method:`Symfony\\Component\\Finder\\Finder::size`::

    $finder->files()->size('< 1.5K');

Si possono filtrare i file di dimensione compresa tra due valori incatenando le chiamate al metodo::

    $finder->files()->size('>= 1K')->size('<= 2K');

È possibile utilizzare uno qualsiasi dei seguenti operatori di confronto: ``>``, ``>=``, ``<``, '<=',
'=='.

La dimensione può essere indicata usando l'indicazione in kilobyte (``k``, ``ki``),
megabyte (``m``, ``mi``) o in gigabyte (``g``, ``gi``). Gli indicatori che terminano
con ``i`` utilizzano l'appropriata versione ``2**n`` in accordo allo `standard IEC`_

Data del file
~~~~~~~~~~~~~

È possibile filtrare i file in base alla data dell'ultima modifica con il metodo
:method:`Symfony\\Component\\Finder\\Finder::date`::

    $finder->date('since yesterday');

È possibile utilizzare uno qualsiasi dei seguenti operatori di confronto: ``>``, ``>=``, ``<``, '<=',
'=='. È anche possibile usare i sostantivi ``since`` o ``after`` come degli alias di ``>``, e
``until`` o ``before`` come alias di ``<``.

Il valore usato può essere una data qualsiasi tra quelle supportate dalla funzione `strtotime`_.

Profondità della ricerca
~~~~~~~~~~~~~~~~~~~~~~~~

Normalmente, Finder attraversa ricorsivamente tutte le cartelle. Per restringere la profondità
dell'attraversamento si usa il metodo :method:`Symfony\\Component\\Finder\\Finder::depth`::

    $finder->depth('== 0');
    $finder->depth('< 3');

Filtri personalizzati
~~~~~~~~~~~~~~~~~~~~~

È possibile definire filtri personalizzati grazie al metodo
:method:`Symfony\\Component\\Finder\\Finder::filter`::

    $filtro_personalizzato = function (\SplFileInfo $file)
    {
        if (strlen($file) > 10) {
            return false;
        }
    };

    $finder->files()->filter($filtro_personalizzato);

Il metodo ``filter()`` prende una Closure come argomento. Per ogni file che corrisponde ai criteri,
la Closure viene chiamata passandogli il file come un'istanza di :class:`Symfony\\Component\\Finder\\SplFileInfo`.
Il file sarà escluso dal risultato della ricerca nel caso in cui la Closure restituisca
``false``.

.. _strtotime:   http://www.php.net/manual/en/datetime.formats.php
.. _Iterator:     http://www.php.net/manual/en/spl.iterators.php
.. _protocollo:   http://www.php.net/manual/en/wrappers.php
.. _stream:       http://www.php.net/streams
.. _standard IEC: http://physics.nist.gov/cuu/Units/binary.html
