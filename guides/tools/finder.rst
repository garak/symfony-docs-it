.. index::
   single: Finder

Il Finder
=========

Il componente :namespace:`Symfony\\Component\\Finder` aiuta a cercare file
e cartelle facilmente e velocemente.

Utilizzo
--------

La classe :class:`Symfony\\Component\\Finder\\Finder` trova file e/o
cartelle::

    use Symfony\Component\Finder\Finder;

    $finder = new Finder();
    $finder->files()->in(__DIR__);

    foreach ($finder as $file) {
        print $file->getRealpath()."\n";
    }

``$file`` è una istanza di :phpclass:`SplFileInfo`.

Il codice sopra visualizza i nomi di tutti i file nella cartella corrente
ricorsivamente. La classe Finder utilizza una interfaccia fluente, quindi tutti
i metodi restituiscono l'istanza di Finder.

.. tip::
   Una istanza di Finder è un `Iterator`_ PHP. Quindi, invece di iterare su
   Finder con ``foreach``, si può anche convertirla in un array con il
   metodo :phpfunction:`iterator_to_array`, od ottenere il numero di elementi con
   :phpfunction:`iterator_count`.

Criteria
--------

Posizione
~~~~~~~~

La posizione è l'unico criterio obbligatorio. Dice alI finder quale
cartella utilizzare per la ricerca::

    $finder->in(__DIR__);

Si può cercare in diverse cartelle concatenando chiamate
:method:`Symfony\\Component\\Finder\\Finder::in`::

    $finder->files()->in(__DIR__)->in('/elsewhere');

Si possono escludere cartelle dalla ricerca con il metodo
:method:`Symfony\\Component\\Finder\\Finder::exclude`::

    $finder->in(__DIR__)->exclude('ruby');

Essendo che il Finder utilizza gli iteratori PHP, si può passare qualunque URL che abbia un
`protocollo`_ supportato::

    $finder->in('ftp://example.com/pub/');

Funziona anche con stream definiti dall'utente::

    use Symfony\Component\Finder\Finder;

    $s3 = new \Zend_Service_Amazon_S3($key, $secret);
    $s3->registerStreamWrapper("s3");

    $finder = new Finder();
    $finder->name('photos*')->size('< 100K')->date('since 1 hour ago');
    foreach ($finder->in('s3://bucket-name') as $file) {
        // do something

        print $file->getFilename()."\n";
    }

.. note::
   Leggere la documentazione degli `Stream`_ per capire come creare un proprio stream.

File o cartelle
~~~~~~~~~~~~~~~

Per impostazione predefinita, il Finder restituisce file e cartelle; mai metodi
:method:`Symfony\\Component\\Finder\\Finder::files` e
:method:`Symfony\\Component\\Finder\\Finder::directories` controllano che::

    $finder->files();

    $finder->directories();

Se si vogliono seguire i link, utilizzare il metodo ``followLinks()``::

    $finder->files()->followLinks();

Per impostazione predefinita, l'iteratore ignora i più conosciuti file VCS. Questo comportamento può essere cambiato con
il metodo ``ignoreVCS()`` method::

    $finder->ignoreVCS(false);

Ordinamento
~~~~~~~~~~~

Ordinare il risultato per nome o per tipo (prima le cartelle, poi i file)::

    $finder->sortByName();

    $finder->sortByType();

.. note::
   Notare che i metodi ``sort*`` hanno bisogno di avere tutti gli elementi per eseguire
   il loro lavoro. Per iteratori di grandi dimensioni, il processo è lento.

Si può anche definire un algoritmo di ordinamento personalizzato con il metodo ``sort()``::

    $sort = function (\SplFileInfo $a, \SplFileInfo $b)
    {
        return strcmp($a->getRealpath(), $b->getRealpath());
    };

    $finder->sort($sort);

Nome del file
~~~~~~~~~~~~~

Limitare i file  in base al nome con il metodo
:method:`Symfony\\Component\\Finder\\Finder::name`::

    $finder->files()->name('*.php');

Il metodo ``name()`` accetta glob, stringhe, o espressioni regolari::

    $finder->files()->name('/\.php$/');

Il metodo ``notNames()`` esclude i file che corrispondono ad un certo schema::

    $finder->files()->notName('*.rb');

Dimensione del file
~~~~~~~~~~~~~~~~~~~

Limitare i file in base alla dimensione con il metodo
:method:`Symfony\\Component\\Finder\\Finder::size`::

    $finder->files()->size('< 1.5K');

Limitare in base ad un intervallo di dimensioni concatenando le chiamate::

    $finder->files()->size('>= 1K')->size('<= 2K');

L'operatore di confronto può essere uno qualsiasi dei seguenti: ``>``, ``>=``, ``<``, '<=',
'=='.

Il valore può utilizzare grandezze in kilobyte (``k``, ``ki``), megabyte
(``m``, ``mi``), o gigabyte (``g``, ``gi``). Questi suffissi con una ``i`` utilizzano
l'appropriata versione ``2**n`` in accordo con gli `standard IEC`_.

Data del file
~~~~~~~~~~~~~

Limitare i file in base alla data di ultima modifica con il metodo
:method:`Symfony\\Component\\Finder\\Finder::date`::

    $finder->date('since yesterday');

L'operatore di confronto può essere uno qualsiasi dei seguenti: ``>``, ``>=``, ``<``, '<=',
'=='. Si può anche utilizzare ``since`` o ``after`` come alias per ``>``, e
``until`` o ``before`` come alias per ``<``.

Il valore può utilizzare un qualsiasi formato di data supportato dalla funzione `strtotime`_.

Profondità delle cartelle
~~~~~~~~~~~~~~~~~~~~~~~~~

Per impostazione predefinita, il Finder percorre ricorsivamente le cartelle. Si può restringere la profondità di tale
percorso con il metodo :method:`Symfony\\Component\\Finder\\Finder::depth`::

    $finder->depth('== 0');
    $finder->depth('< 3');

Personalizzare i filtri
~~~~~~~~~~~~~~~~~~~~~~~

Per limitare i file da cercare secondo una propria strategia, bisogna
utilizzare :method:`Symfony\\Component\\Finder\\Finder::filter`::

    $filter = function (\SplFileInfo $file)
    {
        if (strlen($file) > 10) {
            return false;
        }
    };

    $finder->files()->filter($filter);

Il metodo ``filter()`` accetta come argomento una Closure. Per ogni file cercato,
viene chiamata con il file come istanza di :phpclass:`SplFileInfo`. Il file viene
escluso dal risultato se la Closure restituisce ``false``.

.. _strtotime:   http://www.php.net/manual/en/datetime.formats.php
.. _Iterator:     http://www.php.net/manual/en/spl.iterators.php
.. _protocol:     http://www.php.net/manual/en/wrappers.php
.. _Streams:      http://www.php.net/streams
.. _IEC standard: http://physics.nist.gov/cuu/Units/binary.html
