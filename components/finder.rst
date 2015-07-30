.. index::
   single: Finder
   single: Componenti; Finder

Il componente Finder
====================

   Il componente Finder cerca file e cartelle tramite un'interfaccia intuitiva 
   e "fluida".

Installazione
-------------

È possibile installare il componente in due modi:

* Installandolo :doc:`tramite Composer </components/using_components>` (``symfony/finder`` su `Packagist`_);
* Usando il repository ufficiale su Git (https://github.com/symfony/Finder).

.. include:: /components/require_autoload.rst.inc

Uso
---

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

    Un Finder è un'istanza di un :phpclass:`Iterator` PHP. Perciò, invece di dover iterare attraverso
    Finder con un ciclo ``foreach``, è possibile convertirlo in un array, tramite il metodo
    :phpfunction:`iterator_to_array`, oppure ottenere il numero di oggetti in esso contenuti, con
    :phpfunction:`iterator_count`.

.. caution::

    Quando si cerca in posizioni diverse, passate al metodo
    :method:`Symfony\\Component\\Finder\\Finder::in`, internamente viene creato un
    iteratore separato per ogni posizione. Questo vuol dire che si ottengono vari insiemi di
    risultati, aggregati in uno solo.
    Poiché :phpfunction:`iterator_to_array` usa le chiavi degli insiemi di risultati,
    durante la conversione in array, alcune chiavi potrebbero essere duplicate e i loro
    valori sovrascritti. Lo si può evitare, passando ``false`` come secondo parametro
    di :phpfunction:`iterator_to_array`.

Criteri
-------

Ci sono molti modi per filtrare e ordinare i risultati.

Posizione
~~~~~~~~~

La posizione è l'unico parametro obbligatorio. Indica al finder la cartella da
utilizzare come base per la ricerca::

    $finder->in(__DIR__);

Per cercare in diverse posizioni, è possibile concatenare diverse chiamate a
:method:`Symfony\\Component\\Finder\\Finder::in`::

    $finder->files()->in(__DIR__)->in('/daunaltraparte');

.. versionadded:: 2.2
   Il supporto per i caratteri jolly è stato aggiunto nella  versione 2.2.

Si possono usare caratteri jolly nelle cartelle, per cercare uno schema::

    $finder->in('src/Symfony/*/*/Resources');

Ogni schema deve risolvere almeno un percorso di cartella.

È possibile escludere cartelle dalla ricerca tramite il metodo
:method:`Symfony\\Component\\Finder\\Finder::exclude`::

    $finder->in(__DIR__)->exclude('ruby');

.. versionadded:: 2.3
   Il metodo :method:`Symfony\\Component\\Finder\\Finder::ignoreUnreadableDirs`
   è stato aggiunto in Symfony 2.3.

È anche possibile ignorare le cartelle che non si ha il permesso di leggere::

    $finder->ignoreUnreadableDirs()->in(__DIR__);

Visto che Finder utilizza gli iteratori di PHP, è possibile passargli qualsiasi
URL con `protocollo`_ supportato::

    $finder->in('ftp://example.com/pub/');

Funziona anche con flussi definiti dall'utente::

    use Symfony\Component\Finder\Finder;

    $s3 = new \Zend_Service_Amazon_S3($chiave, $segreto);
    $s3->registerStreamWrapper("s3");

    $finder = new Finder();
    $finder->name('photos*')->size('< 100K')->date('since 1 hour ago');
    foreach ($finder->in('s3://bucket-name') as $file) {
        // ... fare qualcosa

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

Per seguire i collegamenti, è possibile utilizzare il metodo ``followLinks()``::

    $finder->files()->followLinks();

Normalmente l'iteratore ignorerà i file dei VCS più diffusi. È possibile modificare questo
comportamento, grazie al metodo ``ignoreVCS()``::

    $finder->ignoreVCS(false);

Ordinamento
~~~~~~~~~~~

È possibile ordinare i risultati per nome o per tipo (prima le cartelle e poi i file)::

    $finder->sortByName();

    $finder->sortByType();

.. note::

    Si noti che i metodi ``sort*``, per poter funzionare, richiedono tutti gli elementi
    ricercati. In caso di iteratori molto grandi, l'ordinamento potrebbe risultare lento.

È anche possibile definire algoritmi di ordinamento personalizzati, grazie al metodo ``sort()``::

    $sort = function (\SplFileInfo $a, \SplFileInfo $b)
    {
        return strcmp($a->getRealpath(), $b->getRealpath());
    };

    $finder->sort($sort);

Nomi dei file
~~~~~~~~~~~~~

È possibile eseguire filtri sui nomi dei file, utilizzando il metodo
:method:`Symfony\\Component\\Finder\\Finder::name`::

    $finder->files()->name('*.php');

Il metodo ``name()`` accetta, come parametri, glob, stringhe o espressioni regolari::

    $finder->files()->name('/\.php$/');

Il metodo ``notNames()`` viene invece usato per escludere i file che corrispondono allo schema::

    $finder->files()->notName('*.rb');

Contenuti dei file
~~~~~~~~~~~~~~~~~~

Si possono filtrare file per contenuto, con il metodo
:method:`Symfony\\Component\\Finder\\Finder::contains`::

    $finder->files()->contains('lorem ipsum');

Il metodo ``contains()`` accetta stringhe o espressioni regolari::

    $finder->files()->contains('/lorem\s+ipsum$/i');

Il metodo ``notContains()`` esclude file che contengono lo schema dato::

    $finder->files()->notContains('dolor sit amet');

Percorso
~~~~~~~~

.. versionadded:: 2.2
   I metodi ``path()`` e ``notPath()`` sono stati aggiunti nella versione 2.2.

Si possono filtrare file e cartelle per percorso, con il
metodo :method:`Symfony\\Component\\Finder\\Finder::path`::

    $finder->path('una/cartella/particolare');

Su tutte le piattaforme, bisogna usare la barra (cioè ``/``) come separatore di cartelle.

Il metodo ``path()`` accetta stringhe o espressioni regolari::

    $finder->path('pippo/pluto');
    $finder->path('/^pippo\/pluto/');

Internamente, le stringhe sono convertite in espressioni regolari, tramite escape delle barre
e aggiunta di delimitatori:

.. code-block:: text

    nomecartella ===>    /nomecartella/
    a/b/c        ===>    /a\/b\/c/

Il metodo :method:`Symfony\\Component\\Finder\\Finder::notPath` esclude i file per percorso::

    $finder->notPath('altra/cartella');

Dimensione dei file
~~~~~~~~~~~~~~~~~~~

Per filtrare i file in base alla dimensione, si usa il metodo
:method:`Symfony\\Component\\Finder\\Finder::size`::

    $finder->files()->size('< 1.5K');

Si possono filtrare i file di dimensione compresa tra due valori, concatenando le chiamate::

    $finder->files()->size('>= 1K')->size('<= 2K');

È possibile utilizzare uno qualsiasi dei seguenti operatori di confronto: ``>``, ``>=``, ``<``, ``<=``,
``==``, ``!=``.

La dimensione può essere indicata usando l'indicazione in kilobyte (``k``, ``ki``),
megabyte (``m``, ``mi``) o in gigabyte (``g``, ``gi``). Gli indicatori che terminano
con ``i`` utilizzano l'appropriata versione ``2**n``, in accordo allo `standard IEC`_

Data dei file
~~~~~~~~~~~~~

È possibile filtrare i file in base alla data dell'ultima modifica, con il metodo
:method:`Symfony\\Component\\Finder\\Finder::date`::

    $finder->date('since yesterday');

È possibile utilizzare uno qualsiasi dei seguenti operatori di confronto: ``>``, ``>=``, ``<``, '<=',
'=='. È anche possibile usare i sostantivi ``since`` o ``after`` come degli alias di ``>`` e
``until`` o ``before`` come alias di ``<``.

Il valore usato può essere una data qualsiasi tra quelle supportate dalla funzione `strtotime`_.

Profondità della ricerca
~~~~~~~~~~~~~~~~~~~~~~~~

Normalmente, Finder attraversa ricorsivamente tutte le cartelle. Per restringere la profondità
dell'attraversamento, si usa il metodo :method:`Symfony\\Component\\Finder\\Finder::depth`::

    $finder->depth('== 0');
    $finder->depth('< 3');

Filtri personalizzati
~~~~~~~~~~~~~~~~~~~~~

È possibile definire filtri personalizzati, grazie al metodo
:method:`Symfony\\Component\\Finder\\Finder::filter`::

    $filtro = function (\SplFileInfo $file)
    {
        if (strlen($file) > 10) {
            return false;
        }
    };

    $finder->files()->filter($filtro);

Il metodo ``filter()`` prende una Closure come argomento. Per ogni file che corrisponde ai criteri,
la Closure viene chiamata passandogli il file come un'istanza di :class:`Symfony\\Component\\Finder\\SplFileInfo`.
Il file sarà escluso dal risultato della ricerca nel caso in cui la Closure restituisca
``false``.

Leggere il contenuto dei file restituiti
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Il contenuto dei file restituiti può essere letto con
:method:`Symfony\\Component\\Finder\\SplFileInfo::getContents`::

    use Symfony\Component\Finder\Finder;

    $finder = new Finder();
    $finder->files()->in(__DIR__);

    foreach ($finder as $file) {
        $contents = $file->getContents();
        
        // ...
    }

.. _strtotime:    http://php.net/manual/it/datetime.formats.php
.. _protocollo:   http://php.net/manual/it/wrappers.php
.. _stream:       http://php.net/streams
.. _standard IEC: http://physics.nist.gov/cuu/Units/binary.html
.. _Packagist:    https://packagist.org/packages/symfony/finder
