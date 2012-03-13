Gestire le librerie dei venditori con bin/vendors e deps
--------------------------------------------------------

### Come funziona?

Ogni progetto Symfony usa un gruppo di librerie di "venditori". In un modo o
nell'altro, lo scopo è scaricare tali file nella propria cartella ``vendor/``
e, idealmente, avere un modo tranquillo per gestire l'esatta versione necessaria
per ciascuno.

Per impostazione predefinita, tali librerie sono scaricate eseguendo uno script
"scaricatore" ``php bin/vendors install``. Questo script legge dal file ``deps``
nella radice del proprio progetto. Questo è uno script in formato ini, che contiene
una lista di ogni libreria necessaria, la cartella in cui ognuna va scaricata e
(opzionalmente) la versione da scaricare. Lo script ``bin/vendors`` usa
``git`` per scaricare, solamente perché queste librerie esterne solitamente
sono memorizzate tramite git. Lo script ``bin/vendors`` legge anche il file
``deps.lock``, che consente di bloccare ogni libreria a un preciso hash di
commit.

È importante capire che queste librerie di venditori *non* sono in realtà parte del
*proprio* repository. Sono invece dei semplici file non tracciati, che sono scaricati
dallo script ``bin/vendors`` nella cartella ``vendor/``. Ma, poiché ogni informazione
necessaria a scaricare tali file è nei file ``deps`` e ``deps.lock``
(che *sono* memorizzati nel proprio repository), ogni altro sviluppatore può usare il
progetto, eseguendo ``php bin/vendors install`` e scaricando lo stesso preciso insieme
di librerie di venditori. Questo vuol dire che si può controllare con precisione ogni
libreria di venditore, senza dover in realtà inserirle nel *proprio*
repository.

Quindi, ogni volta che uno sviluppatore usa il progetto, deve eseguire lo script
``php bin/vendors install``, per assicurarsi di avere tutt le librerie necessarie.

.. sidebar:: Aggiornare Symfony

    Poiché Symfony non è altro che un gruppo di librerie di terze parti e le librerie
    di terze parti sono interamente controllate tramite ``deps`` e ``deps.lock``,
    aggiornare Symfony vuol dire semplicemente aggiornare questi due file, per far
    corrispondere il loro stato a quello dell'ultima Standard Edition di Symfony.

    Ovviamente, se sono state aggiunte nuove voci a ``deps`` o ``deps.lock``, assicurarsi
    di sostituire solo le parti originali (cioè assicurarsi di non cancellare alcuna
    delle proprie voci).

.. caution::

    C'è anche un comando ``php bin/vendors update``, ma non ha niente a che fare con
    l'aggiornamento del progetto e solitamente non sarà necessario usarlo.
    Questo comando è usato per congelare le versioni di tutte le librerie dei venditori,
    aggiornandole alle versioni specificate in ``deps`` e registrandole nel
    file ``deps.lock``.

### Modificare le librerie dei venditori

A volte, si ha bisogno di un ramo specifico, oppure di un tag o di un commit di una
libreria. Lo si può impostare direttamente nel file ``deps``:

.. code-block:: text

    [AcmeAwesomeBundle]
        git=http://github.com/johndoe/Acme/AwesomeBundle.git
        target=/bundles/Acme/AwesomeBundle
        version=una-versione-aggiornata

* L'opzione``git`` imposta l'URL della libreria. Può essere anche un altro protocollo,
  come ``http://`` o anche ``git://``. 

* L'opzione ``target`` specifica dove starà il repository: i semplici bundle di Symfony
  andrebbero sotto la cartella ``vendor/bundles/Acme``, altre librerie di terze parti
  solitamente vanno in ``vendor/la-mia-libreria``. La cartella di destinazione è
  predefinita su quest'ultima opzione, se non specificata.

* L'opzione ``version`` consente di imposare una versione specifica. Si può usare un tag
    (``version=origin/0.42``) o un ramo (``refs/remotes/origin/awesome-branch``).
    Il valore predefinito è ``origin/HEAD``.

### Flusso di aggiornamento

Quando si esegue ``php bin/vendors install``, per ogni libreria, lo script verifica prima
se esiste la cartella di installazione. 

Se non esiste (e SOLO se non esiste), esegue un ``git clone``. 

Quindi, esegue un ``git fetch origin`` e un ``git reset --hard la-mia-versione``. 

Questo vuole dire che il repository sarà clonato una sola volta. Se si vuole eseguire una
modifica sui remote di git, si DEVE cancellare l'intera cartella di destionazione, non solo il suo contenuto.
