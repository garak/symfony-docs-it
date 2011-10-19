.. index::
   single: Finder

Come trovare i file
===================

Il componente :namespace:`Symfony\\Component\\Finder` aiuta a trovare file e cartelle
rapidamente e in modo facile.

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

La variabile ``$file`` è un'istanza di :phpclass:`SplFileInfo`.

Il codice sopra mostra i nomi di tutti i file nella cartella corrente, ricorsivamente.
La classe ``Finder`` usa un'interfaccia fluida, per cui tutti i metodi restituiscono
un'istanza di ``Finder`` stesso.

.. tip::

    Un'istanza di ``Finder`` è un `iteratore`_ di PHP. Quindi, invece di iterare sul
    ``Finder`` con ``foreach``, lo si può anche convertire in un array con il metodo
    :phpfunction:`iterator_to_array`, oppure ottenere il numero di elementi con
    :phpfunction:`iterator_count`.

Criteri
-------

Posto
~~~~~

Il posto è l'unico criterio obbligatorio. Dice al finder quale cartella usare
per la ricerca::

    $finder->in(__DIR__);

Si può cercare in più posti, concatendando le chiamate a
:method:`Symfony\\Component\\Finder\\Finder::in`::

    $finder->files()->in(__DIR__)->in('/elsewhere');

Si possono escludere delle cartelle dai risultati con il metodo
:method:`Symfony\\Component\\Finder\\Finder::exclude`::

    $finder->in(__DIR__)->exclude('ruby');

Poiché il finder usa gli iteratori di PHP, si può passare qualsiasi URL con un
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

    Leggere la documentazione sugli `stream`_ per sapere come creare i propri stream.

File o cartelle
~~~~~~~~~~~~~~~

Per impostazione predefinita, il finder restituisce file e cartelle. I metodi
:method:`Symfony\\Component\\Finder\\Finder::files` e
:method:`Symfony\\Component\\Finder\\Finder::directories` forniscono un controllo::

    $finder->files();

    $finder->directories();

Se si vogliono seguire i link, usare il metodo ``followLinks()``::

    $finder->files()->followLinks();

Per impostazione predefinita, l'iteratore ignora i file dei sistemi di versionamento più
popolari. Lo si può cambiare con il metodo ``ignoreVCS()``::

    $finder->ignoreVCS(false);

Ordinamento
~~~~~~~~~~~

Ordinare i risultati per nome o per tipo (prima le cartelle, poi i file)::

    $finder->sortByName();

    $finder->sortByType();

.. note::

    Si noti che i metodi ``sort*`` hanno bisogno di estrarre tutti gli elementi
    corrispondenti per funzionare. Per iteratori molto grandi, può risultare lento.

Si possono anche definire propri algoritmi di ordinamento, con il metodo ``sort()``::

    $sort = function (\SplFileInfo $a, \SplFileInfo $b)
    {
        return strcmp($a->getRealpath(), $b->getRealpath());
    };

    $finder->sort($sort);

Nomi di file
~~~~~~~~~~~~

Restringere i file per nome, col metodo
:method:`Symfony\\Component\\Finder\\Finder::name`::

    $finder->files()->name('*.php');

Il metodo ``name()`` accetta glob, stringhe o espressioni regolari::

    $finder->files()->name('/\.php$/');

Il metodo ``notNames()`` esclude i file corrispondenti a uno schema::

    $finder->files()->notName('*.rb');

Dimensione dei file
~~~~~~~~~~~~~~~~~~~

Restringere i file per dimensione, col metodo
:method:`Symfony\\Component\\Finder\\Finder::size`::

    $finder->files()->size('< 1.5K');

Restringere per un intorno di dimensioni, concatenando le chiamate::

    $finder->files()->size('>= 1K')->size('<= 2K');

L'operatore di confronto può essere uno di questi: ``>``, ``>=``, ``<``, '<=',
'=='.

Il valore indicato puù usare multipli di kilobyte, (``k``, ``ki``), megabyte
(``m``, ``mi``) o gigabyte (``g``, ``gi``). Quelli con suffisso ``i`` usano
la versione appropriata ``2**n``, in accordo con lo `standard IEC`_.

Date di file
~~~~~~~~~~~~

Restringere i file per data di ultima modifica, col metodo
:method:`Symfony\\Component\\Finder\\Finder::date`::

    $finder->date('since yesterday');

L'operatore di confronto può essere uno di questi: ``>``, ``>=``, ``<``, '<=',
'=='. Si può anche usare ``since`` o ``after`` come alias di ``>``, e
``until`` o ``before`` come alias di ``<``.

Il valore indicato può essere qualsiasi data supportata dalla funzione `strtotime`_.

Profondità delle cartelle
~~~~~~~~~~~~~~~~~~~~~~~~~

Per impostazione predefinita, il finder attraversa ricorsivamente le cartelle. Restringere
la profondità con il metodo :method:`Symfony\\Component\\Finder\\Finder::depth`::

    $finder->depth('== 0');
    $finder->depth('< 3');

Filtri personalizzati
~~~~~~~~~~~~~~~~~~~~~

Per restringere i file corrispondenti tramite una propria strategia, usare
:method:`Symfony\\Component\\Finder\\Finder::filter`::

    $filter = function (\SplFileInfo $file)
    {
        if (strlen($file) > 10) {
            return false;
        }
    };

    $finder->files()->filter($filter);

Il metodo ``filter()`` accetta una closure come parametro. Per ogni file corrispondente,
la closure è richiamata con il file come istanza di :phpclass:`SplFileInfo`. Il file è
escluso dai risultati se la closure restituisce ``false``.

.. _strtotime:    http://www.php.net/manual/en/datetime.formats.php
.. _iteratore:    http://www.php.net/manual/en/spl.iterators.php
.. _protocollo:   http://www.php.net/manual/en/wrappers.php
.. _stream:       http://www.php.net/streams
.. _standard IEC: http://physics.nist.gov/cuu/Units/binary.html
