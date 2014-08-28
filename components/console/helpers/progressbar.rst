.. index::
    single: Aiutanti di console; Barra di progressione

Barra di progressione
=====================

.. versionadded:: 2.5
    La barra di progressione è stata introdotta in Symfony 2.5, come sostituto
    dell':doc:`aiutante Progress </components/console/helpers/progresshelper>`.

Durante l'esecuzione di comandi molto lunghi, può essere d'aiuto mostrare informazioni
sull'avanzamento, man mano che il comando gira:

.. image:: /images/components/console/progressbar.gif

Per mostrare dettagli sull'avanzamento, usare
:class:`Symfony\\Component\\Console\\Helper\\ProgressBar`, passandogli un
numero totale di unità, e avanzando la progressione durante i passi::

    use Symfony\Component\Console\Helper\ProgressBar;

    // crea una nuova barra di progressione (50 unità)
    $progress = new ProgressBar($output, 50);

    // inizia la barra
    $progress->start();

    $i = 0;
    while ($i++ < 50) {
        // ... fare qualcosa

        // avanzare la progressione di una unità
        $progress->advance();

        // si può anche avanzare la progressione di più unità
        // $progress->advance(3);
    }

    // assicura che la barra di progressione sia al 100%
    $progress->finish();

Invece di far avanzare la barra di un numero di passi (con il metodo
:method:`Symfony\\Component\\Console\\Helper\\ProgressBar::advance`),
si può anche impostare la progressione corrente, richiamando il metodo
:method:`Symfony\\Component\\Console\\Helper\\ProgressBar::setCurrent`.

.. caution::

    La barra di progressione funziona solo su piattaforme con supporto per i codici ANSI. Su altre
    piattaforme non verrà generato alcun output.

Se non si conosce in anticipo il numero di passi, si può omettere il relativo parametro
durante la creazione dell'istanza di
:class:`Symfony\\Component\\Console\\Helper\\ProgressBar`::

    $progress = new ProgressBar($output);

La progressione sarà quindi mostrata come un throbber:

.. code-block:: text

    # passi non definiti (mostra un throbber)
        0 [>---------------------------]
        5 [----->----------------------]
        5 [============================]

    # passi definiti
     0/3 [>---------------------------]   0%
     1/3 [=========>------------------]  33%
     3/3 [============================] 100%

Non appena il comando finisce, non dimenticare di richiamare
:method:`Symfony\\Component\\Console\\Helper\\ProgressBar::finish`, per assicurarsi
che la barra di progressione venga aggiornata con il completamento al 100%.

.. note::

    Se si vuole mostrare qualcosa durante l'esecuzione della barra di progressione,
    richiamare prima :method:`Symfony\\Component\\Console\\Helper\\ProgressBar::clear`.
    Successivamente, richiamare
    :method:`Symfony\\Component\\Console\\Helper\\ProgressBar::display`
    per mostrare nuovamente la barra di progressione.

Personalizzazione della barra di progressione
---------------------------------------------

Formati predefiniti
~~~~~~~~~~~~~~~~~~~

Le informazioni predefinite della barra di progressione dipendono dal livello
di verbosità dell'instanza di ``OutputInterface``:

.. code-block:: text

    # OutputInterface::VERBOSITY_NORMAL (CLI senza opzioni di verbosità)
     0/3 [>---------------------------]   0%
     1/3 [=========>------------------]  33%
     3/3 [============================] 100%

    # OutputInterface::VERBOSITY_VERBOSE (-v)
     0/3 [>---------------------------]   0%  1 sec
     1/3 [=========>------------------]  33%  1 sec
     3/3 [============================] 100%  1 sec

    # OutputInterface::VERBOSITY_VERY_VERBOSE (-vv)
     0/3 [>---------------------------]   0%  1 sec
     1/3 [=========>------------------]  33%  1 sec
     3/3 [============================] 100%  1 sec

    # OutputInterface::VERBOSITY_DEBUG (-vvv)
     0/3 [>---------------------------]   0%  1 sec/1 sec  1.0 MB
     1/3 [=========>------------------]  33%  1 sec/1 sec  1.0 MB
     3/3 [============================] 100%  1 sec/1 sec  1.0 MB

.. note::

    Se si richiama un comando con l'opzione ``-q``, la barra di progressione
    non sarà mostrata.

Invece di appoggioarsi al livello di verbosità del comando, si può anche
forzare il formato tramite ``setFormat()``::

    $bar->setFormat('verbose');

I formati predefiniti sono i seguenti:

* ``normal``
* ``verbose``
* ``very_verbose``
* ``debug``

Se non si imposta il numero di passi della barra di progressione, usare le varianti
``_nomax``:

* ``normal_nomax``
* ``verbose_nomax``
* ``very_verbose_nomax``
* ``debug_nomax``

Formati personalizzati
~~~~~~~~~~~~~~~~~~~~~~

Invece di usare i formati predefiniti, se ne possono definire di propri::

    $bar->setFormat('%bar%');

In questo modo si imposta il formato per mostrare solo la barra stessa:

.. code-block:: text

    >---------------------------
    =========>------------------
    ============================

Un formato per la barra di progressione è una stringa con determinati segnaposto (un nome
racchiuso da simboli ``%``). I segnaposto vengono sostituiti in base alla
progressione corrente della barra. Ecco una lista di segnaposto predefiniti:

* ``current``: il passo corrente;
* ``max``: il numero massimo di passi (o 0, se non definito);
* ``bar``: la barra stessa;
* ``percent``: la percentuale di completamento (non disponibile se ``max`` non è definito);
* ``elapsed``: il tempo trascorso dall'inizio della barra di progressione;
* ``remaining``: il tempo restante al completamento (non disponibile se ``max`` non è definito);;
* ``estimated``: il tempo stimato per il completamento (non disponibile se ``max`` non è definito);;
* ``memory``: l'uso della memoria;
* ``message``: il messaggio allegato alla barra di progressione.

Per esempi, ecco come si può impostare il formato per essere uguale a quello
``debug``::

    $bar->setFormat(' %current%/%max% [%bar%] %percent:3s%% %elapsed:6s%/%estimated:-6s% %memory:6s%');

Notare la parte ``:6s`` aggiunta ad alcuni segnaposto. Questo è il modo in cui si può alterare
il modo in cui appare la barra (formattazione e allineamento). La parte dopo il simbolo
``:`` è usata per impostare il formato ``sprintf`` della stringa.

Il segnaposto ``message`` è un po' speciale, perché lo si deve impostare
manualmente::

    $bar->setMessage('Inizio');
    $bar->start();

    $bar->setMessage('in corso...');
    $bar->advance();

    // ...

    $bar->setMessage('Finito');
    $bar->finish();

Invece di impostare il formato per una data istanza della barra di progressione, si possono
anche definire formati globali::

    ProgressBar::setFormatDefinition('minimal', 'Progress: %percent%%');

    $bar = new ProgressBar($output, 3);
    $bar->setFormat('minimal');

Questo codice definisce un nuovo formato, chiamato ``minimal``, che si può usare per
le barre di progressione:

.. code-block:: text

    Progress: 0%
    Progress: 33%
    Progress: 100%

.. tip::

    Spesso è meglio ridefinire i formati predefiniti, invece di crearn
    di nuovi, perché questo consente la variazione automatica in base
    alla verbosità del comando.

Quando si definisce un nuovo stile, che contiene segnaposto disponibili solo
quando il numero massimo di passi sia noto, si dovrebbe creare una variante
``_nomax``::

    ProgressBar::setFormatDefinition('minimal', '%percent%% %remaining%');
    ProgressBar::setFormatDefinition('minimal_nomax', '%percent%%');

    $bar = new ProgressBar($output);
    $bar->setFormat('minimal');

Quando si mostra la barra di progressione, il formato sarà impostato automaticamente a
``minimal_nomax`` se la barra non dispone del numero massimo di passi, come
nell'esempio precedente.

.. tip::

    Un formato può contenere qualsiasi codice ANSI valido e può anche usare i modi specifici di
    Symfony per impostare i colori::

        ProgressBar::setFormatDefinition(
            'minimal',
            '<info>%percent%</info>\033[32m%\033[0m <fg=white;bg=blue>%remaining%</>'
        );

.. note::

    Un formato può estendersi per più righe. Questo torna molto utile qando si vogliono
    mostrare più informazioni contestuali, insieme alla barra di progressione (vedere
    l'esempio all'inizio di questo articolo).

Impostazioni della barra
~~~~~~~~~~~~~~~~~~~~~~~~

Tra i segnaposto, ``bar`` è un po' speciale, perché tutti i caratteri usati per
mostrarlo possono essere personalizzati::

    // la parte finita della barra
    $progress->setBarCharacter('<comment>=</comment>');

    // la parte non finita della barra
    $progress->setEmptyBarCharacter(' ');

    // il carattere di progressione
    $progress->setProgressCharacter('|');

    // la larghezza della barra
    $progress->setBarWidth(50);

.. caution::

    Per questioni prestazionali, fare attenzione a non impostare un numero totale di passi
    troppo elevato. Per esempio, se si itera un gran numero
    di elementi, considerare l'impostazione di una frequenza maggiore, richiamando
    :method:`Symfony\\Component\\Console\\Helper\\ProgressHelper::setRedrawFrequency`,
    in modo da aggiornare solamente ogni tot iterazioni::

        $progress->start($output, 50000);

        // aggiorna ogni 100 iterazioni
        $progress->setRedrawFrequency(100);

        $i = 0;
        while ($i++ < 50000) {
            // ... fare qualcosa

            $progress->advance();
        }

Segnaposto personalizzati
~~~~~~~~~~~~~~~~~~~~~~~~~

Se si vogliono mostrare alcune informazioni che dipendono dalla barra di progressione
e non sono diponibili tra i segnaposto predefiniti, se ne possono creare
di propri. Vediamo come creare un segnaposto ``remaining_steps``, che
mostri il numero di passi rimanenti::

    ProgressBar::setPlaceholderFormatter(
        '%remaining_steps%',
        function (ProgressBar $bar, OutputInterface $output) {
            return $bar->getMaxSteps() - $bar->getStep();
        }
    );

Messaggi personalizzati
~~~~~~~~~~~~~~~~~~~~~~~

Il segnaposto ``%message%`` consente di specificare un messaggio perrsonalizzato, da
mostrare insieme alla barra di progressione. Se però ne servono più di uno, basta
definire il proprio::

    $bar->setMessage('Inizio');
    $bar->setMessage('', 'filename');
    $bar->start();

    $bar->setMessage('in corso...');
    while ($file = array_pop($files)) {
        $bar->setMessage($filename, 'filename');
        $bar->advance();
    }

    $bar->setMessage('Finito');
    $bar->setMessage('', 'filename');
    $bar->finish();

Per la parte ``filename`` della barra di progressione, basta aggiungere il segnaposto
``%filename%`` al proprio formato::

    $bar->setFormat(" %message%\n %step%/%max%\n Working on %filename%");
