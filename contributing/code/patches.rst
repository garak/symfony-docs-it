Inviare una Patch
==================

Una Patche è il modo migliore per rimediare ad un bug e per proporre dei miglioramenti
a Symfony2

Configurazione Iniziale
-------------

Prima di lavorare su Symfony2, configurare il proprio ambiente con 
il seguente software software:

* Git;

* PHP version 5.3.2 o superiore;

* PHPUnit 3.5.11 o superiore.

Impostare il informazioni utente con il nome e l'indirizzo email:

.. code-block:: bash

    $ git config --global user.name "Your Name"
    $ git config --global user.email you@example.com

.. suggerimento::

    Se siete nuovi a Git, vi raccomandiamo caldamente di leggere l'eccellente
    libro gratuito `ProGit`_

Recuperare il codice sorgente di Symfony2:

* Creare un account su `GitHub`_ ed eseguire l'autenticazione;

* Forkare il `Symfony2 repository`_ cliccare sul bottone "fork";

* Dopo aver completato il Forking, clonare il proprio fork localmente
  (Questo creerà una cartella `symfony`)

.. code-block:: bash

      $ git clone git@github.com:USERNAME/symfony.git

* Aggiungere il repository di upstream come ``remote``:

.. code-block:: bash

      $ cd symfony
      $ git remote add upstream git://github.com/symfony/symfony.git

Ora che Symfony2 è installato, verificate che tutti i test unitari passino
per il vostro ambiente come spiegato nel capitolo :doc:`document <tests>`.

Lavorare su una Patch
------------------

Ogni volta che si desidera lavorare su una patch per un bug o per un
miglioramento, è necessario creare uno specifico ramo.

Il ramo deve essere basato sul ramo `master` se si vuole aggiungere una nuova
funzionalità. Ma se si vuole fissare un bug, utilizzare le vecchie versioni ma
mantenute versioni Symfony nelle quali appare il bug (come `2.0`).


Creare il ramo dell'argomento con il seguente comando:

.. code-block:: bash

    $ git checkout -b BRANCH_NAME master

.. suggerimento::

    Usare un nome descrittivo per il ramo (`ticket_XXX` dove `XXX` è il 
    numero del ticket è una buona convezione per il bug fixing)

Il comando sopra scambia automaticamente il codice con il ramo appena creato
(per verificare in quale ramo ci si trovi eseguire il comando `git branch`)

E possbile lavorare sul codice quanto si vuole e committare tanto quanto si vuole;
ma bisogna tenere a mente le seguenti indicazioni:

* Seguire i coding :doc:`standards <standards>` (utilizzare `git diff --check` per
  controllare i spazi alla fine);

* Aggiungere test unitari per provare che il bug è stato fissato per mostrare che
  la funzionalità è effettivamente funzionante;

* Fare commit separati e atomici (utilizzare le funzionalità di `git rebase` 
  per ottenere uno storico chiaro e pulito);

* Write good commit messages.

.. suggerimento::

    Un buon messaggio di commit è composto dal riepilogo nella (prima linea),
    opzionalmente seguito da una linea vuota e da una descrizione dettagliata.
    Il riepilogo dovrebbe cominciare con il componente sul quale si sta lavorando
    posto fra parentesi quadre (``[DependencyInjection]``, ``[FrameworkBundle]``, ...) .
    Utilizzare un verbo (``fixed ...``, ``added ...``, ...) per iniziare e non
    utilizzare il punto finale.

Inviare una patch
------------------

Before submitting your patch, update your branch (needed if it takes you a
while to finish your changes):

Prima di inviare una patch, aggiornare il proprio ramo (necessario se passa del 
tempo tra il checkout e il commit delle nuove funzionalità)

.. code-block:: bash

    $ git checkout master
    $ git fetch upstream
    $ git merge upstream/master
    $ git checkout BRANCH_NAME
    $ git rebase master

Quando si esegue il comando ``rebase``, potrebbe essere necessario risolvere
conflitti dovuti all'unione del codice. Il comando ``git status`` metterà in mostra
i file non ancora uniti (*unmerged* ). Risolvere tutti i conflitti e continuare con 
il rebase

.. code-block:: bash

    $ git add ... # add resolved files
    $ git rebase --continue

Verificare che tutti i test stiano ancora passando e inviare gli sviluppi 
nel ramo remoto.

.. code-block:: bash

    $ git push origin BRANCH_NAME

A questo punto è possibile discutere della patch nella `dev mailing-list`_ o effettuare
direttamente un pull request ( deve essere eseguita nel repository ``symfony/symfony``).
Per facilitare il lavoro del team di sviluppo principlae includere sempre nella pull request
un messaggio con i componenti modificati come di seguito:

.. code-block:: text

    [Yaml] foo bar
    [Form] [Validator] [FrameworkBundle] foo bar

Se si decide di inviare una mail alla mailing-list, non dimenticare di 
inserire l'URL del ramo (``https://github.com/USERNAME/symfony.git
BRANCH_NAME``) oppure l'URL della pull request

Dipendentemente dal riscontro della mailing-list o attraverso la pull request su 
Github, potrebbe essere necessario rielaborare la patch. Prima di re-inserire la path,
eseguire il rebase con il ramo master, ma non unire attraverso il merge; e forzare il push
nell'origin:

.. code-block:: bash

    $ git rebase -f upstream/master
    $ git push -f origin BRANCH_NAME

.. note::

    Tutte le patch che si rilasciano devono essere sotto licenza MIT a meno che
    non sia esplicitato diversamente nel codice.

Tutti i bug risolti uniti nei rami di manutenzione sono anche uniti nei piu
recenti rami. Per esempio se si invia una patch per il ramo `2.0`, la patch sarà
applicata dal team di sviluppo principale nel ramo master.



.. _ProGit:              http://progit.org/
.. _GitHub:              https://github.com/signup/free
.. _Symfony2 repository: https://github.com/symfony/symfony
.. _dev mailing-list:    http://groups.google.com/group/symfony-devs