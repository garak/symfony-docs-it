Contribuire alla documentazione
===============================

Uno dei princìpi essenziali del progetto Symony è che **la documentazione è
importante quanto il codice**. Ecco perché viene dedicata una grande quantità di risorse
alla documentazione di nuove caratteristiche e all'aggiornamento del resto della documentazione.

Più di 700 sviluppatori, sparsi in tutto il mondo, hanno contribuito alla documentazione di Symfony
ed è con piacere che viene accolta ogni volontà di unirsi a questa grande famiglia.
Questa guida spiegherà tutto ciò che serve per contribuire alla documentazione
di Symfony.

Prima di iniziare a contribuire
-------------------------------

**Prima di contribuire**, si dovrebbero considerare i seguenti punti:

* La documentazione di Symfony è scritta usando il linguaggio reStructuredText.
  Se non si ha familiarità con tale formato, leggere :doc:`questa pagina </contributing/documentation/format>`
  per una veloce panoramica sulle sue caratteristiche di base.
* La documentazione di Symfony è ospitata su GitHub_. Si deve essere iscritti a GitHub per
  poter contribuire.
* La documentazione di Symfony è pubblicata sotto
  :doc:`licenza Creative Commons BY-SA 3.0 </contributing/documentation/license>`
  e qualsiasi contributo aderirà implicitamente a tale licenza.

Il primo contributo alla documentazione
---------------------------------------

In questa sezione, si vedrà come contribuire alla documentazione di Symfony per la
prima volta. La sezione successiva spiegherà il processo, più breve, da
seguire in futuro per ogni contributo successivo al primo.

Si immagini di voler migliorare il capitolo dell'installazione del libro di Symfony.
Per poter apportare modifiche, seguire questi passi:

**Passo 1.** Andare sul repository ufficiale della documentazione di Symfony, che si trova su
`github.com/symfony/symfony-docs`_ ed eseguire un `fork`_ verso il proprio utente
personale. Questa procedura è necessaria solo la prima volta.

**Passo 2.** **Clonare** il repository forkato sulla macchina locale (questo esempio
usa la cartella ``progetti/symfony-docs/`` per memorizzare la documentazione,
cambiare a piacimento):

.. code-block:: bash

    $ cd progetti/
    $ git clone git://github.com/NOMEUTENTE/symfony-docs.git

**Passo 3.** Passare al **più vecchio ramo ancora mantenuto**, prima di fare qualsiasi modifica.
Attualmente, è il ramo ``2.3``:

.. code-block:: bash

    $ cd symfony-docs/
    $ git checkout 2.3

Se si vuole documentare una nuova caratteristica, usare la prima versione di  Symfony
in cui sia stata inclusa: ``2.5``, ``2.6``, ecc.

**Passo 4.** Creare un **nuovo ramo**, dedicato alle modifiche. Questo semplifica di molto
il lavoro di revisione delle modifiche. Usare un nome breve e comprensibile
per questo nuovo ramo:

.. code-block:: bash

    $ git checkout -b miglioramento_capitolo_installazione

**Passo 5.** Eseguire le modifiche nella documentazione. Aggiungere, migliorare, riorganizzare
o anche rimuovere contenuti, purché ci si assicuri di aderire agli
doc:`/contributing/documentation/standards`.

**Passo 6.** Eseguire un **push** verso il repository forkato:

.. code-block:: bash

    $ git commit book/installation.rst
    $ git push origin miglioramento_capitolo_installazione

**Passo 7.** Ora è tutto pronto per iniziare una **richiesta di pull**. Andare sul
repository forkato, su ``https//github.com/<NOME_SU_GITHUB>/symfony-docs``
e cliccare sul collegamento ``Pull Requests``, nella barra laterale.

Quindi, cliccare sul bottone ``New pull request``. Poiché GitHub non può sapere esattamente
le modifiche che si vogliono proporre, scegliere il ramo appropriato, in cui
applicare le modifiche:

.. image:: /images/contributing/docs-pull-request-change-base.png
   :align: center

In questo esempio, il **repository di base** dovrebbe essere ``symfony/symfony-docs`` e
il **ramo di base** dovrebbe essere ``2.3``, che è il ramo su sui si è scelto di
basare le modifiche. Il **repository di confronto** dovrebbe essere la copia forkata
di ``symfony-docs`` e il **ramo di confronto** dovrebbe essere ``miglioramento_capitolo_installazione``,
che è il nome del ramo creato e su cui sono state eseguite le modifiche.

.. _pull-request-format:

**Passo 8.** Il passo successivo è preparare la **descrizione** della richiesta di pull.
Per consentire una revisione rapida, si prega di aggiungere la seguente tabella
in cima alla descrizione della richiesta di pull:

.. code-block:: text

    | Q             | A
    | ------------- | ---
    | Doc fix?      | [yes|no]
    | New docs?     | [yes|no] (PR # su symfony/symfony, se applicabile)
    | Applies to    | [numero di versione di Symfony a cui si applica]
    | Fixed tickets | [lista separata da virgole di ticket risolti dalla PR]

Un esempio di invio potrebbe essere come il seguente:

.. code-block:: text

    | Q             | A
    | ------------- | ---
    | Doc fix?      | yes
    | New docs?     | yes (symfony/symfony#2500)
    | Applies to    | all (or 2.4+)
    | Fixed tickets | #1075

**Passo 9.** Ora che il primo contributo è stato inviato con successo, si può
festeggiare! I gestori della documentazione revisioneranno con attenzione
questo lavoro e, in breve tempo, renderanno noti eventuali cambiamenti
necessari.

In caso si renda necessario aggiungere o modificare qualcosa, non occorre creare una nuova
richiesta di pull. Basta assicurarsi di essere sul ramo giusto, fare le modifiche
e inviarlo con un push:

.. code-block:: bash

    $ cd progetti/symfony-docs/
    $ git checkout miglioramento_capitolo_installazione

    # ... fare qualche modifica

    $ git push

**Passo 10.** Dopo che la richiesta di pull sarà stata accettata e inserita nella
documentazione di Symfony, si verrà inclusi tra i `contributori della documentazione di Symfony`_.
Inoltre, se si dispone di un profilo su SensioLabsConnect_, si otterrà un
bellissimo `distintivo della documentazione di Symfony`_.

Il secondo contributo alla documentazione
-----------------------------------------

Il primo contributo ha richiesto del tempo, perché si doveva eseguire il fork del repository,
capire come scrivere documentazione, adeguarsi agli standard delle richieste di pull, ecc.
Il secondo contributo sarà molto più facile, tranne per un dettaglio: data
l'altissima attività di aggiornamenti sul repository della documentazione di Symfony, è probabile
che il proprio fork sia ora rimasto indietro rispetto al repository ufficiale.

Per risolvere la questione, si deve `sincronizzare il proprio fork` con il repository ufficiale.
Per farlo, eseguire una tantum il seguente comando, che dice a git dove si trova:

.. code-block:: bash

    $ cd progetti/symfony-docs/
    $ git remote add upstream https://github.com/symfony/symfony-docs.git

Ora si può **sincronizzare il proprio fork**, tramite il comando seguente:

.. code-block:: bash

    $ cd progetti/symfony-docs/
    $ git fetch upstream
    $ git checkout 2.3
    $ git merge upstream/2.3

Questo comando aggiornerà il ramo ``2.3``, che è quello usato per creare
il nuovo ramo per le modifiche. Se si è usato un ramo diverso,
come ``master``, sostituire ``2.3`` con il relativo nome.

Ottimo! Ora si può procedere, seguendo gli stessi passi spiegati nella sezione
precedente:

.. code-block:: bash

    # creare un nuovo ramo per memorizzare le modifiche, basato sul ramo 2.3
    $ cd progetti/symfony-docs/
    $ git checkout 2.3
    $ git checkout -b modifiche

    # ... fare qualche modifica

    # inviare le modifiche al proprio fork del repository
    $ git add xxx.rst     # (opzionale) solo se è un nuovo contenuto
    $ git commit xxx.rst
    $ git push

    # andare su GitHub e creare una richiesta di pull
    #
    # Includere questa tabella nella descrizione:
    # | Q             | A
    # | ------------- | ---
    # | Doc fix?      | [yes|no]
    # | New docs?     | [yes|no] (PR # su symfony/symfony, se applicabile)
    # | Applies to    | [numero di versione di Symfony a cui si applica]
    # | Fixed tickets | [lista separata da virgole di ticket risolti dalla PR]

Il secondo contributo è ora completo, quindi si può festeggiare di nuovo!
Si può anche vedere come la propria posizione salga nella lista dei
`contributori della documentazione di Symfony`_.

Successivi contributi alla documentazione
-----------------------------------------

Dopo due contributi alla documentazione di Symfony, probabilmente si è
più a proprio agio con tutta la magia di Git che il processo implica. Ecco
perché il prossimo contributo sarà molto più veloce. Ecco la lista completa
dei passi per contribuire alla documentazione di Symfony, che si può usare come
**elenco**:

.. code-block:: bash

    # sincronizzare il proprio fork con il repository ufficiale di Symfony
    $ cd progetti/symfony-docs/
    $ git fetch upstream
    $ git checkout 2.3
    $ git merge upstream/2.3

    # creare un nuovo ramo, dalla versione più vecchia ancora mantenuta
    $ git checkout 2.3
    $ git checkout -b modifiche

    # ... fare qualche modifica

    # inviare le modifiche
    $ git add xxx.rst     # (opzionale) solo se è un nuovo contenuto
    $ git commit xxx.rst
    $ git push

    # andare su GitHub e creare una richiesta di pull
    #
    # Includere questa tabella nella descrizione:
    # | Q             | A
    # | ------------- | ---
    # | Doc fix?      | [yes|no]
    # | New docs?     | [yes|no] (PR # su symfony/symfony, se applicabile)
    # | Applies to    | [numero di versione di Symfony a cui si applica]
    # | Fixed tickets | [lista separata da virgole di ticket risolti dalla PR]

    # (opzionale) apportare eventuali modifiche richieste dai revisori e inviarle
    $ git commit xxx.rst
    $ git push

E dopo tutto questo duro lavoro, è ora di festeggiare di nuovo!

Modifiche minori (p.e. errori di battitura)
-------------------------------------------

Si potrebbe voler correggere un semplice errore di battitura. Grazie all'interfaccia di GitHub,
è molto semplice creare una richiesta di pull direttamente nel
browser, mentre si leggono i documenti su symfony.com. Per poterlo fare, basta cliccare
sul link **edit this page** nell'angolo in alto a destra. In precedenza,
si prega di scegliere il ramo giusto, come già menzionato. Ora si può
modificare il contenuto e descrivere le modifiche all'interno di GitHub.
A lavoro concluso, cliccare su **Propose file change** per
creare un commit e, se questo è il primo contributo, anche un
fork. Viene creato automaticamente un nuovo ramo, per poter fornire una base
per la richiesta di pull. Compilare quindi il form e creare la richiesta di pull,
come già descritto.

Domande frequenti
-----------------

Perché ci vuole così tanto per la revisione delle modifiche?
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Occorre pazienza. Possono volerci vari giorni prima della una revisione di una
richiesta di pull. Dopo il merge delle modifiche, potrebbero volerci ancora varie ore
prima che le modifiche compaiano sul sito symfony.com.

Cosa fare per tradurre la documentazione in un'altra lingua?
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Leggere l'apposito :doc:`documento </contributing/documentation/translations>`.

Perché si deve usare il più vecchio ramo in manutenzione e non master?
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Coerentemente con il codice di Symfony, il repository è suddiviso in
vari rami, che corrispondono alle diverse versioni di Symfony stesso.
Il ramo ``master`` contiene la documentazione per il ramo in sviluppo
del codice.

A meno di documentare una caratteristica che è stata introdotto dopo Symfony 2.3,
le modifiche vanno sempre basate sul ramo ``2.3``. I gestori della documentazione
useranno la necessaria magia di Git per applicare le modifiche a tutti i rami
attivi della documentazione.

Se si volesse inviare un lavoro prima di averlo completato?
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Lo si può fare. Si prega in questo caso di usare uno di questi due prefissi, per rendere
noto ai revisori lo stato del lavoro:

* ``[WIP]`` (Work in Progress) si usa quando la richiesta di pull non è ancora
  finita, ma si desidera una revisione. La richiesta di pull non riceverà
  un merge, finché non sarà segnalata come pronta.

* ``[WCM]`` (Waiting Code Merge) si usa quando si sta documentando una nuova caratteristica
  o una modifica non ancora accetta nel codice del nucleo. La richiesta di pull
  non riceverà un merge prima del merge nel codice del nucleo (oppure sarà chiusa, se la
  modifica sarà rigettata).

Una richiesta di pull enorme e con un sacco di modiche sarà accettata?
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Primo, assicurarsi che le modifiche siano correlate tra loro. Altrimenti, si prega di creare
richieste di pull separate. A ogni modo, prima di proporre una modifica enorme, potrebbe essere una
buona idea aprire una issue nel repository della documentazione di Symfony, chiedendo ai
gestori se concordano con i cambiamenti proposti. Altrimenti, potrebbero rifiutare
la proposta, dopo un lungo e duro lavoro. Sarebbe sicuramente meglio evitare
di sprecare il proprio tempo.

.. _`github.com/symfony/symfony-docs`: https://github.com/symfony/symfony-docs
.. _reStructuredText: http://docutils.sourceforge.net/rst.html
.. _GitHub: https://github.com/
.. _`fork`: https://help.github.com/articles/fork-a-repo
.. _`contributori della documentazione di Symfony`: http://symfony.com/contributors/doc
.. _SensioLabsConnect: https://connect.sensiolabs.com/
.. _`distintivo della documentazione di Symfony`: https://connect.sensiolabs.com/badge/36/symfony-documentation-contributor
.. _`sincronizzare il proprio fork`: https://help.github.com/articles/syncing-a-fork
