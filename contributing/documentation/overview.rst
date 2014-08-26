Contribuire alla documentazione
===============================

La documentazione è importante tanto quanto il codice. E segue esattamente gli stessi principi:
DRY, test, facilità di manutenzione, estensibilità, ottimizzazione e refactoring,
solo per nominarne alcuni. E certamente la documentazione ha bug, errori di battitura, difficoltà di lettura dei tutorial
e molto altro.

Contribuire
-----------

Prima di contribuire, è necessario familiarizzare con il :doc:`linguaggio di markup<format>` 
usato per la documentazione.

La documentazione di Symfony2 è ospitata da GitHub:

.. code-block:: text

    https://github.com/symfony/symfony-docs

Se si vuole inviare una patch, fare un `fork`_ del repository ufficiale su GitHub
e quindi un clone del proprio fork:

.. code-block:: bash

    $ git clone git://github.com/NOMEUTENTE/symfony-docs.git

Coerentemente con il codice sorgente di Symfony, il repository della documentazione è suddiviso in
vari rami, corrispondenti alle diverse versioni di Symfony stesso.
Il ramo ``master`` contiene la documentazione per il ramo in sviluppo del codice.

A meno di non documentare una caratteristica aggiunta *dopo* Symfony 2.3
(p.e. in Symfony 2.4), le modifiche vanno sempre basate sul ramo 2.3.
Per poterlo fare, eseguire un checkout del ramo 2.3 prima del prossimo passo:

.. code-block:: bash

    $ git checkout 2.3

.. tip::

    Il ramo base (p.e. 2.3) diventerà "Applies to" nel :ref:`doc-contributing-pr-format`
    usato successivamente.

Quindi, creare un ramo dedicato per le proprie modifiche (per questioni organizzative):

.. code-block:: bash

    $ git checkout -b miglioramenti_di_pippo_e_pluto

Si possono ora eseguire le proprie modifiche in tale ramo. Quando si ha finito,
fare il push di quest ramo nel *proprio* fork su GitHub e richiedere un pull.

Richiedere un pull
~~~~~~~~~~~~~~~~~~

Seguendo l'esempio, la richiesta di pull sarà tra il proprio ramo
``miglioramenti_di_pippo_e_pluto`` e il ramo ``master`` di ``symfony-docs``.

Se le proprie modifiche sono basate sul ramo 2.3, occorre cambiare il
ramo base in 2.3 sulla pagina di anteprima, cliccando sul pulsante ``edit``
in alto a sinistra:

.. image:: /images/docs-pull-request-change-base.png
   :align: center

.. note::

  Tutte le modifiche fatte nel ramo 2.3 subiranno un merge nei rami più nuovi
  (cioè 2.4, master, ecc.) per il prossimo rilascio, su base settimanale.

GitHub spiega l'argomento in modo dettagliato, su `richieste di pull`_.

.. note::

  La documentazione di Symfony2 è rilasciata sotto :doc:`licenza <license>`
  Creative Commons Attribuzione - Condividi allo stesso modo 3.0 Unported.

Si può anche aggiungere un prefisso al titolo della richiesta di pull, in questi casi:

* ``[WIP]`` (Work in Progress) è usato quando non si ha ancora finito con la propria
  richiesta di pull, ma si vorrebbe che fosse rivista. La richiesta di pull non
  subirà merge finché non si dichiara che è pronta.

* ``[WCM]`` (Waiting Code Merge) è usato quando si sta documentando una nuova caratteristica
  o modifica che non è ancora stata accettata. La richiesta di pull non subirà
  merge fino al merge del codice (oppure sarà chiusa, se la modifica
  verrà respinta).

.. _doc-contributing-pr-format:

Formato della richiesta di pull
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

A meno di non risolvere errori di battitura, la descrizione della richiesta di pull deve
includere la seguente lista, per assicurare che il contributo possa essere rivisto
senza cicli inutili di feedback e che il proprio contributo possa essere incluso
nella documentazione il prima possibile:

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

.. tip::

    Serve un po' di pazienza. Le modifiche appaiono sul sito symfony.com tra i 15 minuti e alcuni giorni
    dopo il merge della richiesta di pull nella documentazione. Si può verificare
    se le proprie modifiche non abbiano introdotto problemi di markup, guardando la
    pagina `Errori di build della documentazione`_ (aggiornata ogni notte alle 3,
    quando il server ricostruisce la documentazione).

Documentare nuove caratteristiche o modifiche di comportamenti
--------------------------------------------------------------

Se si sta documentando una nuova caratteristica o una modifica fatta in
Symfony2, si deve precedere la descrizione con un tag ``.. versionadded:: 2.X``
e una brave descrizione:

.. code-block:: text

    .. versionadded:: 2.3
        Il metodo ``askHiddenResponse`` è stato aggiunto in Symfony 2.3.

    Si può anche fare una domanda e nascondere la risposta. Questo è particolarmente...

Se si sta documentando una modifica di comportamento, potrebbe essere di aiuto descrivere *brevemente*
il modo in cui il comportamento è cambiato.

.. code-block:: text

    .. versionadded:: 2.3
        La funzione ``include()`` è una nuova caratteristica di Twig, disponibile in
        Symfony 2.3. In precedenza, veniva usato il tag ``{% include %}``.

Ogni volta che viene rilasciata una nuova versione minore di Symfony2 (p.e. 2.4, 2.5, ecc.),
viene creato un nuovo ramo della documentazione, partendo dal ramo ``master``.
A questo punto, tutti i tag ``versionadded`` per versioni di Symfony2 che hanno raggiunto il
fine vita saranno rimossi. Per esempio, se Symfony 2.5 fosse rilasciato
oggi e se il 2.2 avesse raggiunto il suo fine vita, il tag ``versionadded`` 2.2
sarebbe rimosso dal nuovo ramo 2.5.

Standard
--------

Tutta la documentazione di Symfony deve seguire gli
:doc:`standard di documentazione <standards>`.

Segnalare una problematica
--------------------------

Il modo più semplice di contribuire è segnalando una problematica: un errore di battitura,
un errore grammaticale, un bug nel codice di esempio, e così via

Passi:

* Segnalare un bug attraverso il bug tracker;

* *(opzionale)* Inviare una patch.

Traduzione
----------

Leggere la :doc:`documentazione <translations>`.

Gestione dei rilasci
--------------------

Symfony ha un processo di rilasci molto standardizzato, che si può approfondire
nella sezione :doc:`/contributing/community/releases`.

Per mantenere il processo dei rilasci, la squadra della documentazione esegue molte
modifiche alla documentazione nelle varie parti del ciclo di vita.

Quando un rilascio raggiunge la "fine manutenzione"
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Ogni rilascio prima o poi raggiunge la sua "fine manutenzione". Per maggiori dettagli,
vedere :ref:`contributing-release-maintenance`.

Quando finisce la manutenzione di un rilascio, si eseguono le seguenti azioni.
Per questo esempio, supponiamo che la versione 2.1 abbia appena raggiunta la sua fine manutenzione:

* Non si esegue più il merge di modifiche e richieste di pull nel ramo (2.1),
  tranne per aggiornamenti di sicurezza, fino a che il rilascio non raggiunge
  la suo "fine vita".

* Tutti i rami ancora mantenuti (p.e. 2.2 e superiori) vengono aggiornati
  per riflettere che le richieste di pull vanno iniziate dalla versione mantenuta più
  vecchia (p.e. 2.2).

* Si rimuovono tutte le direttive ``versionadded`` e ogni altra nota relative a caratteristiche
  modificate o aggiunte, per la versione nuova (p.e. 2.1) nel ramo master.
  Come risultato, il prossimo rilascio (che è il primo ad arrivare
  *dopo*  la fine manutenzione di questo ramo), non avrà menzioni della
  vecchia versione (p.e. 2.1).

Quando si crea un nuovo ramo per un rilascio
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Durante la :ref:`fase di stabilizzazione<contributing-release-development>`, viene
creato un nuovo ramo della documentazione. Per esempio, se la versione 2.3 è
stata stabilizzata, viene creato un ramo 2.3 per essa. Quando questo
accade, vengono eseguite le seguenti azioni:

* Si cambiano tutti riferimenti a versione e master alla versione correttta (p.e. 2.3).
  Per esempio, nei capitoli sull'installazione, si fa riferimento alla versione da usare
  per un'installazione. Come esempio, si vedano le modifiche eseguite nella `PR #2688`_.

.. _`PR #2688`: https://github.com/symfony/symfony-docs/pull/2688
.. _`fork`: https://help.github.com/articles/fork-a-repo
.. _`richieste di pull`: https://help.github.com/articles/using-pull-requests
.. _`Errori di build della documentazione`: http://symfony.com/doc/build_errors
