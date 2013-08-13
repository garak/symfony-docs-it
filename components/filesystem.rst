.. index::
   single: Filesystem

Il componente Filesystem
========================

    Il componente Filesystem fornisce utilità di base per il filesystem.

.. versionadded:: 2.1
    Il componente Filesystem è stato aggiunto in Symfony 2.1. Precedentemente, la classe
    ``Filesystem`` si trovava nel componente ``HttpKernel``.

Installazione
-------------

Si può installare il componente in molti modi diversi:

* Usare il repository ufficiale su Git (https://github.com/symfony/Filesystem);
* Installarlo via Composer (``symfony/filesystem`` su `Packagist`_).

Utilizzo
--------

La classe :class:`Symfony\\Component\\Filesystem\\Filesystem` è l'unico
punto finale per le operazioni su filesystem::

    use Symfony\Component\Filesystem\Filesystem;
    use Symfony\Component\Filesystem\Exception\IOException;

    $fs = new Filesystem();

    try {
        $fs->mkdir('/tmp/random/dir/' . mt_rand());
    } catch (IOException $e) {
        echo "Errore durante la creazione della cartella";
    }

.. note::

    I metodi :method:`Symfony\\Component\\Filesystem\\Filesystem::mkdir`,
    :method:`Symfony\\Component\\Filesystem\\Filesystem::exists`,
    :method:`Symfony\\Component\\Filesystem\\Filesystem::touch`,
    :method:`Symfony\\Component\\Filesystem\\Filesystem::remove`,
    :method:`Symfony\\Component\\Filesystem\\Filesystem::chmod`,
    :method:`Symfony\\Component\\Filesystem\\Filesystem::chown` e
    :method:`Symfony\\Component\\Filesystem\\Filesystem::chgrp` possono ricevere
    come parametro una stringa, un array o qualsiasi oggetto che implementi
    :phpclass:`Traversable`.

Mkdir
~~~~~

:method:`Symfony\\Component\\Filesystem\\Filesystem::mkdir` crea una cartella.
Su filesystem di tipo posix, le cartelle sono create in modalità predefinita
`0777`. Si può usare il secondo parametro per impostare la modalità::

    $fs->mkdir('/tmp/photos', 0700);

.. note::

    Si può passare un array o un oggetto :phpclass:`Traversable` come primo
    parametro.

Exists
~~~~~~

Exists verifica la presenza di tutti i file o cartelle e restituisce `false` se un file
manca::

    // questa cartella esiste, restituisce true
    $fs->exists('/tmp/photos');

    // rabbit.jpg esiste, bottle.png non esiste, restituisce false
    $fs->exists(array('rabbit.jpg', 'bottle.png'));

.. note::

    Si può passare un array o un oggetto :phpclass:`Traversable` come primo
    parametro.

Copy
~~~~

:method:`Symfony\\Component\\Filesystem\\Filesystem::copy` copia file.
Se la destinazione esiste già, file file è copiato solo se la data di
modifica del sorgente è precedente a quella della destinazione. Questo
comportamento è modificabile tramite un terzo parametro booleano::

    // funziona solo se image-ICC è stato modificato dopo image.jpg
    $fs->copy('image-ICC.jpg', 'image.jpg');

    // image.jpg sarà sovrascritto
    $fs->copy('image-ICC.jpg', 'image.jpg', true);

Touch
~~~~~

Touch imposta l'ora di accesso e modifica di un file. Per impostazione predefinita, usa
l'ora attuale. Si può impostare un'ora diversa con il secondo parametro. Il terzo parametro
è l'ora di accesso::

    // imposta l'ora di accesso al timestamp attuale
    $fs->touch('file.txt');
    // imposta l'ora di modifica a 10 secondi nel futuro
    $fs->touch('file.txt', time() + 10);
    // imposta l'ora di accessoa 10 secondi nel passato
    $fs->touch('file.txt', time(), time() - 10);

.. note::

    Si può passare un array o un oggetto :phpclass:`Traversable` come primo
    parametro.

Chown
~~~~~

Chown è usato per cambiare il proprietario di un file. Il terzo parametro è un booleano per
un'opzione ricorsiva::

    // imposta il proprietario del video lolcat a www-data
    $fs->chown('lolcat.mp4', 'www-data');
    // cambia il proprietario della cartella video ricorsivamente
    $fs->chown('/video', 'www-data', true);

.. note::

    Si può passare un array o un oggetto :phpclass:`Traversable` come primo
    parametro.

Chgrp
~~~~~

Chgrp è usato per cambiare il gruppo di un file. Il terzo parametro è un booleano per
un'opzione ricorsiva::

    // imposta il gruppo del video lolcat a nginx
    $fs->chgrp('lolcat.mp4', 'nginx');
    // cambia il gruppo della cartella video ricorsivamente
    $fs->chgrp('/video', 'nginx', true);

.. note::

    Si può passare un array o un oggetto :phpclass:`Traversable` come primo
    parametro.

Chmod
~~~~~

Chmod modifica la modalità di un file. Il terzo parametro è un booleano con un'opzione
ricorsiva::

    // imposta la modalità di video.ogg a 0600
    $fs->chmod('video.ogg', 0600);
    // imposta ricorsivamente la modalità della cartella src
    $fs->chmod('src', 0700, true);

.. note::

    Si può passare un array o un oggetto :phpclass:`Traversable` come primo
    parametro.

Remove
~~~~~~

:method:`Symfony\\Component\\Filesystem\\Filesystem::remove` rimuove file,
collegamenti simbolici, cartelle::

    $fs->remove(array('symlink', '/path/to/directory', 'activity.log'));

.. note::

    Si può passare un array o un oggetto :phpclass:`Traversable` come primo
    parametro.

Rename
~~~~~~

:method:`Symfony\\Component\\Filesystem\\Filesystem::rename` rinomina file
e cartelle::

    // rinomina un file
    $fs->rename('/tmp/processed_video.ogg', '/path/to/store/video_647.ogg');
    // rinomina una cartella
    $fs->rename('/tmp/files', '/path/to/store/files');

symlink
~~~~~~~

:method:`Symfony\\Component\\Filesystem\\Filesystem::symlink` crea un
collegamento simbolico dal sorgente alla destinazione. Se il filesystem
non supporta i collegamenti simbolici, c'è un terzo parametro booleano::

    // crea un collegamento simbolico
    $fs->symlink('/percorso/della/sorgente', '/percorso/della/destinazione');
    // duplica la cartella sorgente, se il filesystem
    // non supporta i collegamenti simbolici
    $fs->symlink('/percorso/della/sorgente', '/percorso/della/destinazione', true);

makePathRelative
~~~~~~~~~~~~~~~~

:method:`Symfony\\Component\\Filesystem\\Filesystem::makePathRelative` restituisce
il percorso relativo di una cartella, data un'altra::

    // restituisce '../'
    $fs->makePathRelative(
        '/var/lib/symfony/src/Symfony/',
        '/var/lib/symfony/src/Symfony/Component'
    );
    // restituisce 'videos'
    $fs->makePathRelative('/tmp', '/tmp/videos');

mirror
~~~~~~

:method:`Symfony\\Component\\Filesystem\\Filesystem::mirror` esegute il mirror
di una cartella::

    $fs->mirror('/percorso/della/sorgente', '/percorso/della/destinazione');

isAbsolutePath
~~~~~~~~~~~~~~

:method:`Symfony\\Component\\Filesystem\\Filesystem::isAbsolutePath` restiuisce
`true` se il percorso dato è assoluto, `false` altrimenti::

    // restituisce true
    $fs->isAbsolutePath('/tmp');
    // restituisce true
    $fs->isAbsolutePath('c:\\Windows');
    // restituisce false
    $fs->isAbsolutePath('tmp');
    // restituisce false
    $fs->isAbsolutePath('../dir');

Error Handling
--------------

Quando si verifica un problema, viene sollevata un'eccezione che
implementa la classe
:class:`Symfony\\Component\\Filesystem\\Exception\\ExceptionInterface`.

.. note::

    Prima della versione 2.1, :method:`Symfony\\Component\\Filesystem\\Filesystem::mkdir`
    restituiva un booleano e non lanciava eccezioni. Dalla 2.1, viene sollevata una
    :class:`Symfony\\Component\\Filesystem\\Exception\\IOException` se
    la creazione della cartella fallisce.

.. _`Packagist`: https://packagist.org/packages/symfony/filesystem
