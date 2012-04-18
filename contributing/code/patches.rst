Inviare una patch
=================

Una patch è il modo migliore per rimediare a un bug e per proporre dei miglioramenti
a Symfony2

Lista di controllo
------------------

Lo scopo della lista di controllo è assicurare che i contributi possano essere valutati
senza il bisogno di domande e risposte continue e assicurare che i contributi possano
essere inclusi in Symfony2 il più rapidamente possibile.

Le richieste di pull vanno prefissate con il nome del componente o del bundle a cui
si riferiscono.

.. code-block:: text

    [Componente] breve titolo descrittivo.

Un esempio potrebbe essere simile a questo:

.. code-block:: text

    [Form] Aggiunta del tipo di campo selectbox.

.. tip::

    Si prega di aggiungere "[WIP]" al titolo, se la proposta non è ancora completa
    oppure se i test sono incompleti o non passano.

Tutte le richieste di pull devono includere il seguente template nella descrizione
della richiesta:

.. code-block:: text

    Bug fix: [yes|no]
    Feature addition: [yes|no]
    Backwards compatibility break: [yes|no]
    Symfony2 tests pass: [yes|no]
    Fixes the following tickets: [lista separata da virgole di ticket risolti]
    Todo: [lista di todo in corso]

Un esempio di proposta potrebbe essere il seguente:

.. code-block:: text

    Bug fix: no
    Feature addition: yes
    Backwards compatibility break: no
    Symfony2 tests pass: yes
    Fixes the following tickets: -
    Todo: -

Grazie per includere il template completo nelle vostre proposte!

.. tip::

    Tutte le aggiunte di caratteristiche vanno inviate al ramo "master", mentre
    tutti i bug risolti vanno inviati al più vecchio ramo ancora attivo. Inoltre,
    le proposte non devono, di norma, infrangere la retro-compatibilità.

.. tip::

    Per fare in modo che il proprio ramo sia automaticamente testato, si può aggiungere
    il proprio fork a `travis-ci.org`_. Basta entrare con l'account usato su github.com e
    e abilitare un singolo switch, per i test automatici. Nella propria richiesta di pull,
    invece di specificare "*Symfony2 tests pass: [yes|no]*", si può collegare
    l'`icona di stato di travis-ci.org`_. Per maggiori dettagli, vedere
    `travis-ci.org Getting Started Guide`_.

Configurazione iniziale
-----------------------

Prima di lavorare su Symfony2, configurare il proprio ambiente con 
il seguente software:

* Git;

* PHP versione 5.3.2 o superiore;

* PHPUnit 3.5.11 o superiore.

Impostare il informazioni utente con il nome e l'indirizzo email:

.. code-block:: bash

    $ git config --global user.name "Il mio nome"
    $ git config --global user.email mia_emil@example.com

.. tip::

    Raccomandiamo caldamente a chi fosse nuovo di Git la lettura dell'eccellente
    libro gratuito `ProGit`_

.. tip::

    Utenti Windows: installando Git, l'installazione chiederà cosa fare con i fine
    riga e suggerità di sostituire Lf con CRLF. Questa impostazione è sbagliata,
    se si vuole contribuire a Symfony! La scelta migliore è il metodo "as-is",
    perché git converitrà i fine riga in quelli del repository.
    Se Git è già stato installato, si può verificare il valore dell'impostazione,
    scrivendo:

    .. code-block:: bash

        $ git config core.autocrlf

    Questo comando resituirà "false", "input" o "true", dove "true" e "false" sono
    i valori sbagliati. Si può modificare il valore, scrivendo:

    .. code-block:: bash

        $ git config --global core.autocrlf input

    Sostituire --global con --local se si vuole impostare solo per il repository
    attivo

Recuperare il codice sorgente di Symfony2:

* Creare un account su `GitHub`_ ed eseguire l'autenticazione;

* Forkare il `repository di Symfony2`_: cliccare sul bottone "fork";

* Dopo aver completato il fork, clonare il proprio fork localmente
  (questo creerà una cartella `symfony`)

.. code-block:: bash

      $ git clone git@github.com:USERNAME/symfony.git

* Aggiungere il repository di upstream come ``remote``:

.. code-block:: bash

      $ cd symfony
      $ git remote add upstream git://github.com/symfony/symfony.git

Ora che Symfony2 è installato, verificate che tutti i test unitari passino
per il vostro ambiente come spiegato nel capitolo :doc:`document <tests>`.

Lavorare su una patch
---------------------

Ogni volta che si desidera lavorare su una patch per un bug o per un
miglioramento, è necessario creare uno specifico ramo.

Il ramo deve essere basato sul ramo `master` se si vuole aggiungere una nuova
funzionalità. Ma se si vuole fissare un bug, utilizzare le vecchie versioni ma
mantenute versioni Symfony nelle quali appare il bug (come `2.0`).

Creare il ramo dell'argomento con il seguente comando:

.. code-block:: bash

    $ git checkout -b NOME_RAMO master

Oppure, se si vuole fornire il fix di un bug per il ramo 2.0, occorre prima tracciare
localmente il ramo remoto `2.0`:

.. code-block:: bash

    $ git checkout -t origin/2.0

Si può quindi creare un nuovo ramo dal 2.0, per lavorare sul fix del bug:

.. code-block:: bash

    $ git checkout -b NOME_RAMO 2.0

.. tip::

    Usare un nome descrittivo per il ramo (`ticket_XXX` dove `XXX` è il 
    numero del ticket è una buona convezione per il fix del bug)

Il comando sopra scambia automaticamente il codice con il ramo appena creato
(per verificare in quale ramo ci si trovi eseguire il comando `git branch`)

È possibile lavorare sul codice quanto si vuole e committare tanto quanto si vuole; ma bisogna tenere a mente le seguenti indicazioni:

* Seguire gli :doc:`standards <standards>` del codice (utilizzare `git diff --check` per
  controllare i spazi alla fine);

* Aggiungere test unitari per provare che il bug è stato fissato per mostrare che
  la funzionalità è effettivamente funzionante;

* Fare commit separati e atomici (utilizzare le funzionalità di `git rebase` 
  per ottenere uno storico chiaro e pulito);

* Scrivere buoni messaggi di commit.

.. tip::

    Un buon messaggio di commit è composto dal riepilogo nella (prima linea),
    opzionalmente seguito da una linea vuota e da una descrizione dettagliata.
    Il riepilogo dovrebbe cominciare con il componente sul quale si sta lavorando
    posto fra parentesi quadre (``[DependencyInjection]``, ``[FrameworkBundle]``, ...) .
    Utilizzare un verbo (``fixed ...``, ``added ...``, ...) per iniziare e non
    utilizzare il punto finale.

Inviare una patch
------------------

Prima di inviare una patch, aggiornare il proprio ramo (necessario se passa del 
tempo tra il checkout e il commit delle nuove funzionalità)

.. code-block:: bash

    $ git checkout master
    $ git fetch upstream
    $ git merge upstream/master
    $ git checkout NOME_RAMO
    $ git rebase master

.. tip::

    Sostituire `master` con `2.0` se si sta lavorando sul fix di un bug

Quando si esegue il comando ``rebase``, potrebbe essere necessario risolvere
conflitti dovuti all'unione del codice. Il comando ``git status`` metterà in mostra
i file non ancora uniti (*unmerged* ). Risolvere tutti i conflitti e continuare con 
il rebase

.. code-block:: bash

    $ git add ... # aggiunge file risolti
    $ git rebase --continue

Verificare che tutti i test stiano ancora passando e inviare gli sviluppi nel ramo remoto.

.. code-block:: bash

    $ git push origin NOME_RAMO

A questo punto è possibile discutere della patch nella `lista dev`_ o effettuare
direttamente una richiesta di pull (deve essere eseguita nel repository ``symfony/symfony``).
Per facilitare il lavoro del team di sviluppo principale, includere sempre nella richiesta di pull
un messaggio con i componenti modificati, come di seguito:

.. code-block:: text

    [Yaml] pippo pluto
    [Form] [Validator] [FrameworkBundle] pippo pluto

.. tip::

    Si faccia attenzione a puntare la richiesta di pull a ``symfony:2.0``, se si vuole
    che il team faccia il pull del fix di un bug sul ramo 2.0.

Se si decide di inviare un'email alla lista, non dimenticare di 
inserire l'URL del ramo (``https://github.com/USERNAME/symfony.git
NOME_RAMO``) oppure l'URL della richiesta di pull.

Dipendentemente dal riscontro della lista o attraverso la richiesta di pull su 
Github, potrebbe essere necessario rielaborare la patch. Prima di re-inserire la path,
eseguire il rebase con il ramo master, ma non unire attraverso il merge; e forzare il push
nell'origin:

.. code-block:: bash

    $ git rebase -f upstream/master
    $ git push -f origin NOME_RAMO

.. note::

    Tutte le patch che si rilasciano devono essere sotto licenza MIT a meno che
    non sia esplicitato diversamente nel codice.

Tutti i bug risolti uniti nei rami di manutenzione sono anche uniti nei più
recenti rami. Per esempio se si invia una patch per il ramo `2.0`, la patch sarà
applicata dal team di sviluppo principale nel ramo master.

.. code-block:: bash

    $ git rebase -i head~3
    $ git push -f origin NOME_RAMO

Il numero 3 deve essere uguale al numero di commit nel proprio ramo. Dopo aver scritto
questo comando, si aprirà un programma di modifica, con una lista di commit:

.. code-block:: text

    pick 1a31be6 primo commit
    pick 7fc64b4 secondo commit
    pick 7d33018 terzo commit

Per unificare tutti i commit nel primo, rimuovere la parola "pick" prima del secondo
e dell'ultimo commit e sostituirla con la parola "squash", o anche solo "s".
Quando si salva, git inizierà il rebase e, in caso di successo, chiederà di modificare
il messaggio di commit, che come predefinito è una lista di messaggi di commit di tutti
i commit. Dopo aver finito, eseguire il push.

.. note::

    Tutte le patch da inviare devono essere rilasciate sotto licenza MIT,
    a meno che non sia specificato diversamente nel codice.

Tutti i merge di fix di bug nei rami di manutenzione subiscono merge anche nei rami
più recente, regolarmente. Per esempio, se si propone una patch per il ramo `2.0`,
la patch sarà applicata dal team anche al ramo
`master`.

.. _ProGit:              http://progit.org/
.. _GitHub:              https://github.com/signup/free
.. _repository di Symfony2: https://github.com/symfony/symfony
.. _lista dev:           http://groups.google.com/group/symfony-devs
.. _travis-ci.org:       http://travis-ci.org
.. _`icona di stato di travis-ci.org`: http://about.travis-ci.org/docs/user/status-images/
.. _`travis-ci.org Getting Started Guide`: http://about.travis-ci.org/docs/user/getting-started/
