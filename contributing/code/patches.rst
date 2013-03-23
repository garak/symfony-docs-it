Inviare una patch
=================

Una patch è il modo migliore per rimediare a un bug e per proporre dei miglioramenti
a Symfony2

Passo 1: preparare l'ambiente
-----------------------------

Installare il software
----------------------

Prima di lavorare con Symfony2, preparare l'ambiente con il seguente
software:

* Git;
* PHP versione 5.3.3 o successive;
* PHPUnit 3.6.4 o successivi.

Configurare Git
~~~~~~~~~~~~~~~

Impostare le informazioni utente con il proprio nome reale e il proprio indirizzo di posta elettronica:

.. code-block:: bash

    $ git config --global user.name "Il proprio nome"
    $ git config --global user.email la_propria_email@example.com

.. tip::

    Si raccomanda caldamente a chi fosse nuovo la lettura del libro `ProGit`_,
    eccellente e libero.

.. tip::

    Se il proprio IDE crea dei file di configurazione dentro la cartella del progetto,
    si può usare il file globale ``.gitignore`` (per tutti i progetti) o il file
    ``.git/info/exclude`` (per progetto) per ignorarli. Vedere la
    `documentazione di Github`_.

.. tip::

    Utenti di Windows: quando si installa Git, l'installazione chiederà cosa fare con
    i fine riga, suggerendo di sostituire Lf con CRLF. Questa impostazine è sbagliata,
    se si vuole contribuire a Symfony! Impostare il metodo "as-is" come scelta
    migliore, così git convertirà i fine riga con quelli nel
    repository. Se git è già stato installato, si può verificare questa impostazione
    con:

    .. code-block:: bash

        $ git config core.autocrlf

    Restituirà "false", "input" o "true", dove "true" e "false" sono i
    valori sbagliati. Impostare nuovamente con:

    .. code-block:: bash

        $ git config --global core.autocrlf input

    Sostituire --global con --local se si vuole impostare solo per il repository
    attivo.

Ottenere il codice sorgente di Symfony
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Ottenere il codice sorgente di Symfony:

* Creare un account su `GitHub`_ ed entrare;

* Forkare il `repository di Symfony2`_ (cliccando sul bottone "Fork");

* Dopo che l'azione "hardcore forking" è stata completata, clonare il proprio fork in locale
  (creerà una cartella `symfony`):

.. code-block:: bash

      $ git clone git@github.com:USERNAME/symfony.git

* Aggiungere il repository upstream come ``remote``:

.. code-block:: bash

      $ cd symfony
      $ git remote add upstream git://github.com/symfony/symfony.git

Verificare che i test passino
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Ora che Symfony2 è installato, verifcare che tutti i test unitari passino nel proprio
ambiente, come spiegato nel :doc:`documento <tests>` dedicato.

Passo 2: lavorare su una patch
------------------------------

La licenza
~~~~~~~~~~

Prima di iniziare, occorre sapere che tutte le patch da inviare devono essere rilasciate
sotto *licenza MIT*, a meno che non sia specificato diversamente nel proprio
codice.

Scegliere il ramo giusto
~~~~~~~~~~~~~~~~~~~~~~~~

Prima di lavorare su una patch, è necessario individuare il ramo giusto. Il ramo deve
essere basato sul ramo `master` se si vuole aggiungere una nuova
funzionalità. Ma se si vuole risolvere un bug, utilizzare le versioni vecchie (ma ancora
mantenute) di Symfony nelle quali appare il bug (come `2.1`).

.. tip::

    Tutti i bug risolti in rami di manutenzione sono inseriti anche i rami più recenti,
    su base regolare. Per esempio, se si invia una patch
    per il ramo `2.1`, la patch sarà applicata anche sul ramo
    `master`.

Creare un ramo
~~~~~~~~~~~~~~

Ogni volta che si vuole lavorare su una patch per un bug o un miglioramento, creare
un ramo:

.. code-block:: bash

    $ git checkout -b NOME_RAMO master

Oppure, se si vuole risolvere un bug per il ramo 2.1, tracciare il ramo `2.1` remoto
in locale:

.. code-block:: bash

    $ git checkout -t origin/2.1

Quindi creare un nuovo ramo dal ramo `2.1`:

.. code-block:: bash

    $ git checkout -b NOME_RAMO 2.1

.. tip::

    Usare un nome descrittivo per il proprio ramo (`ticket_XXX` dove `XXX` è il
    numero di ticket è una buona convenzione per i bug).

I comandi precedenti porteranno automaticamente sul ramo appena creato
(verificare il ramo su cui si sta lavorando con `git branch`).

Lavorare sulla propria patch
~~~~~~~~~~~~~~~~~~~~~~~~~~~~

È possibile lavorare sul codice quanto si vuole e committare tanto quanto si vuole; ma bisogna tenere a mente le
seguenti indicazioni:

* Seguire le :doc:`convenzioni <conventions>` di Symfony e gli
  :doc:`standard <standards>` del codice (utilizzare `git diff --check` per
  controllare i spazi alla fine);

* Aggiungere test unitari per provare che il bug è stato fissato per mostrare che
  la funzionalità è effettivamente funzionante;

* Sforzarsi di non infrangere la retro-compatibilità (se lo si deve fare, provare a fornire
  un livello di compatibilità che supporti il vecchio modo), le patch che infrangono la
  retro-compatbilità hanno meno probabilità di essere accettate;

* Fare commit separati e atomici (utilizzare le funzionalità di `git rebase` 
  per ottenere uno storico chiaro e pulito);

* Comprimere i commit irrilevanti, che sistemano solamente gli standard di codice o gli errori
  di battitura;

* Non sistemare mai gli standard nel codice esistente, perché rende più difficoltosa la
  revisione del codice;

* Scrivere buoni messaggi di commit.

.. tip::

    Si possono verificare gli standard del codice eseguente il seguente
    `script` <http://cs.sensiolabs.org/get/php-cs-fixer.phar>`_
    (`sorgente <https://github.com/fabpot/PHP-CS-Fixer>`_):

    .. code-block:: bash

        $ cd /path/to/symfony/src
        $ php symfony-cs-fixer.phar fix . Symfony20Finder

.. tip::

    Un buon messaggio di commit è composto dal riepilogo nella (prima linea),
    opzionalmente seguito da una linea vuota e da una descrizione dettagliata.
    Il riepilogo dovrebbe cominciare con il componente sul quale si sta lavorando
    posto fra parentesi quadre (``[DependencyInjection]``, ``[FrameworkBundle]``, ...) .
    Utilizzare un verbo (``fixed ...``, ``added ...``, ...) per iniziare e non
    utilizzare il punto finale.

Preparare la propria patch
--------------------------

Quando la proprià patch non riguarda la sistemazione di un bug (quando si aggiunge una nuova
caratteristica o se ne cambia una, per esempio), occorre includere quello che segue:

* Una spiegazione delle modifiche nel file (o nei file) CHANGELOG rilevante (usare il prefisso
  ``[BC BREAK]`` o ``[DEPRECATION]``, se rilevanti);

* Una spiegazione di come aggiornare un'applicazione esistente, nel file (o nei file)
  UPGRADE rilevante, se le modifiche infrangono la retro-compatibilità o se si sta
  deprecando qualcosa che alla fine infrangerà la retro-compatibilità.

Passo 3: inviare la propria patch
---------------------------------

Quando si ritiene la propria patch pronta per l'invio, seguire i passi
seguenti.

Fare un rebase
~~~~~~~~~~~~~~

Prima di inviare una patch, aggiornare il proprio ramo (necessario se passa del 
tempo tra il checkout e il commit delle nuove funzionalità)

.. code-block:: bash

    $ git checkout master
    $ git fetch upstream
    $ git merge upstream/master
    $ git checkout NOME_RAMO
    $ git rebase master

.. tip::

    Sostituire `master` con `2.1` se si sta lavorando sulla risoluzione di un bug

Quando si esegue il comando ``rebase``, potrebbe essere necessario risolvere
conflitti. Il comando ``git status`` metterà in mostra
i file non ancora uniti (*unmerged* ). Risolvere tutti i conflitti e continuare con il rebase:

.. code-block:: bash

    $ git add ... # aggiunge file risolti
    $ git rebase --continue

Verificare che tutti i test stiano ancora passando e inviare gli sviluppi nel ramo remoto.

.. code-block:: bash

    $ git push origin NOME_RAMO

Richiedere un pull
~~~~~~~~~~~~~~~~~~

Si può ora eseguire una richiesta di pull sul repository ``symfony/symfony`` su Github.

.. tip::

    Si faccia attenzione a puntare la richiesta di pull verso ``symfony:2.1``, se si vuole
    che la risoluzione del bug riceva un pull basato sul ramo 2.1.

Per facilitare il lavoro, includere sempre i componenti modificati nel messaggio di
richiesta di pull, come in:

.. code-block:: text

    [Yaml] sistemato qualcosa
    [Form] [Validator] [FrameworkBundle] aggiunto qualcosa

La descrizione della richiesta di pull deve includere la seguente lista in cima, per assicurare
che i contributi siano rivisti senza continui giri di feedback e che quindi possano
essere inclusi in Symfony2 il prima
possibile:

.. code-block:: text

    | Q             | A
    | ------------- | ---
    | Bug fix?      | [yes|no]
    | New feature?  | [yes|no]
    | BC breaks?    | [yes|no]
    | Deprecations? | [yes|no]
    | Tests pass?   | [yes|no]
    | Fixed tickets | [lista separata da virgole di ticket risolti nella PR]
    | License       | MIT
    | Doc PR        | [Riferimento alla PR di documentazione, se presente]

Un esempio di proposta potrebbe essere il seguente:

.. code-block:: text

    | Q             | A
    | ------------- | ---
    | Bug fix?      | no
    | New feature?  | no
    | BC breaks?    | no
    | Deprecations? | no
    | Tests pass?   | yes
    | Fixed tickets | #12, #43
    | License       | MIT
    | Doc PR        | symfony/symfony-docs#123

L'intera tabella va inclusa (**non** rimuovere le righe che si ritengono
non rilevanti). Per sempici errori di battitura, modifiche minori in PHPDoc o modifiche
nei file di traduzione, usare la versione breve della lista:

.. code-block:: text

    | Q             | A
    | ------------- | ---
    | Fixed tickets | [lista separata da virgole di ticket risolti nella PR]
    | License       | MIT

Alcune risposte alle domande richiedono ulteriori requisiti:

 * Se si risponde affermativamente a "Bug fix?", verificare se il bug sia già elencato
   tra le issue di Symfony e referenziarlo in "Fixed tickets";

 * Se si risponde affermativamente a "New feature?", si deve inviare una richiesta di pull alla
   documentazione e referenziarla sotto la sezione "Doc PR";

 * Se si risponde affermativamente a "BC breaks?", la patch deve contenere aggiornamenti ai file
   CHANGELOG e UPGRADE rilevanti;

 * Se si risponde affermativamente a "Deprecations?", la patch deve contenere aggiornamenti ai file
   CHANGELOG e UPGRADE rilevanti;

 * Se si risponde negativamente a "Tests pass", si deve aggiungere un elemento a una lista di todo con
   la zioni da eseguire per sistemare i test;

 * Se "license" non è MIT, non inviare la richiesta di pull, perché non
   sarà comunque accettata.

Se alcuni dei precedenti requisiti non sono soddisfatti, creare una lista di todo e
aggiungere gli elementi rilevanti:

.. code-block:: text

    - [ ] fix the tests as they have not been updated yet
    - [ ] submit changes to the documentation
    - [ ] document the BC breaks

Se il codice non è finito perché non si ha il tempo di finirlo o
perché si desidera prima un feedback, aggiungere un elemento alla lista di todo:

.. code-block:: text

    - [ ] finish the code
    - [ ] gather feedback my changes

Finché si hanno elementi nella lista di todo, si prega di aggiungere alla richiesta di pull
il prefisso "[WIP]".

Nella descrizione della richiesta di pull, dare quanti più dettagli possibile sulle
proprie modifiche (non esitare a fornire esempi di codice per illustrare il punto). Se
la richiesta di pull aggiunge nuove caratteristiche o ne modifica di esistenti,
spiegare le ragioni delle modifiche. La descrizione della richiesta di pull aiuta la
revisione del codice e serve da riferimento nel momento del merge (la descrizione della
richiesta di pull e tutti i commenti associati sono parte del messaggio di commit del
merge).

Oltre alla richiesta di pull sul codice, si deve inviare anche una richiesta di pull
al `repository della documentazione`_, per aggiornare la documentazione relativa.

Rielaborare una patch
~~~~~~~~~~~~~~~~~~~~~

Dipendentemente dal riscontro della lista o attraverso la richiesta di pull su 
Github, potrebbe essere necessario rielaborare la patch. Prima di re-inserire la patch,
eseguire il rebase con il ramo master, ma non unire attraverso il merge; e forzare il push nell'origin:

.. code-block:: bash

    $ git rebase -f upstream/master
    $ git push -f origin NOME_RAMO

.. note::

    Quando si fa un ``push --force``, specificare sempre il nome del ramo in modo esplicito,
    per evitare complicazioni con altri rami del repository (``--force`` dice a git che si
    vogliono veramente complicare le cose, quindi va usato con cautela).

Spesso, i moderatori richiederanno una compressione dei commit. Questo vuol dire che si
convertiranno molti commit in uno solo. Per farlo, usare il comando ``rebase``:

.. code-block:: bash

    $ git rebase -i HEAD~3
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

.. _ProGit:                                http://git-scm.com/book
.. _GitHub:                                https://github.com/signup/free
.. _`documentazione di Github`:            https://help.github.com/articles/ignoring-files
.. _repository di Symfony2:                https://github.com/symfony/symfony
.. _lista dev:                             http://groups.google.com/group/symfony-devs
.. _travis-ci.org:                         https://travis-ci.org/
.. _`icona di stato di travis-ci.org`:     http://about.travis-ci.org/docs/user/status-images/
.. _`travis-ci.org Getting Started Guide`: http://about.travis-ci.org/docs/user/getting-started/
.. _`repository della documentazione`:     https://github.com/symfony/symfony-docs
