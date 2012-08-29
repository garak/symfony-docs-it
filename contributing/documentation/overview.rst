Contribuire alla documentazione
===============================

La documentazione è importante tanto quanto il codice. E segue esattamente gli stessi principi:
DRY, test, facilità di manutenzione, estensibilità, ottimizzazione e refactoring,
solo per nominarne alcuni. E certamente la documentazione ha bug, errori di battitura, difficoltà di lettura dei tutorial
e molto altro.

Contribuire
-----------

Prima di contribuire, è necessario famigliarizzare con il :doc:`linguaggio di markup<format>` 
usato per la documentazione.

La documentazione di Symfony 2 è ospitata da GitHub:

.. code-block:: text

    https://github.com/symfony/symfony-docs

Se si vuole inviare una patch, fare un `fork`_ del repository ufficiale su GitHub
e quindi un clone del proprio fork:

.. code-block:: bash

    $ git clone git://github.com/TUONOME/symfony-docs.git

A meno di non documentare una caratteristica aggiunta in Symfony 2.1, le modifiche
vanno basate sul ramo 2.0, non sul ramo master. Per poterlo fare,
eseguire un checkout del ramo 2.0 prima del prossimo passo:

.. code-block:: bash

    $ git checkout 2.0

Quindi, creare un ramo dedicato per le proprie modifiche (per questioni organizzative):

.. code-block:: bash

    $ git checkout -b miglioramenti_di_pippo_e_pluto

Si possono ora eseguire le proprie modifiche in tale ramo. Quando si ha finito,
fare il push di quest ramo nel *proprio* fork su GitHub e richiedere un pull.
La richiesta di pull sarà tra il proprio ramo ``miglioramenti_di_pippo_e_pluto`` e
il ramo ``master`` di ``symfony-docs``.

.. image:: /images/docs-pull-request.png
   :align: center

Se le proprie modifiche sono basate sul ramo 2.0, occorre seguire il collegamento di modifica del commit
e cambiare il ramo base in @2.0:

.. image:: /images/docs-pull-request-change-base.png
   :align: center

GitHub spiega l'argomento in modo dettagliato, su `richieste di pull`_.

.. note::

  La documentazione di Symfony2 è rilasciata sotto :doc:`licenza <license>`
  Creative Commons Attribuzione - Condividi allo stesso modo 3.0 Unported.

.. tip::

    Le modifiche appaiono sul sito symfony.com website non oltre 15 minuti
    dopo il merge della richiesta di pull nella documentazione. Si può verificare
    se le proprie modifiche non abbiano introdotto problemi di markup, guardando la
    pagina `Errori di build della documentazione`_ (aggiornata ogni notte alle 3,
    quando il server ricostruisce la documentazione).

Standard
--------

Per aiutare il più possibile il lettore e per creare esempi di codice che sembrino
familiari, seguire queste regole:

* Il code segue gli :doc:`standard di codice di Symfony</contributing/code/standards>`
  e gli `standard di codice di Twig`_;
* Ogni riga dovrebbe interrompersi dopo che la prima parola attraversa la
  72esima colonna (quindi con la maggior parte delle righe tra 72 e 78 caratteri);
* Quando si omettono righe di codice, porre ``...`` in un commento nel punto
  di omissione. I commenti sono: ``// ...`` (php), ``# ...`` (yaml/bash), ``{# ... #}``
  (twig), ``<!-- ... -->`` (xml/html), ``; ...`` (ini), ``...`` (testo);
* Quando si omette una parte di riga, p.e. il valore di una variabile, porre ``...`` (senza commenti)
  nel punto di omissione;
* Descrizione del codice omesso (facoltativa):
  se si omettono molte righe: la descrizione dell'omissione può essere posta dopo ``...``
  se si omette parte di una riga: la descrizione può essere posta prima della riga;
* Se utile, un ``codeblock`` dovrebbe iniziare con un commento contenente il nome del
  file nel blocco di codce. Inserire una riga vuota dopo il commento, a meno che la riga
  successiva non sia anch'essa un commento;
* Inserire il simbolo ``$`` all'inizio di ogni riga di bash;
* Preferire la scorciatoia ``::`` a ``.. code-block:: php`` per iniziare un codice di
  blocco PHP.

Un esempio::

    // src/Foo/Bar.php

    // ...
    class Bar
    {
        // ...

        public function foo($bar)
        {
            // imposta foo al valore di bar
            $foo = ...;

            // ... verifica se $bar ha il valore corretto

            return $foo->baz($bar, ...);
        }
    }

.. note::

    * In Yaml, mettere uno spazio dopo ``{`` e prima di ``}`` (p.e. ``{ _controller: ... }``),
      tranne che in Twig (p.e. ``{'ciao' : 'valore'}``).
    * Un array è parte di una riga, non una riga completa. Quindi non usare
      ``// ...`` ma ``...,`` (la virgola fa parte degli standard di codice)::

        array(
            'un valore',
            ...,
        )

Segnalare una problematica
--------------------------

Il modo più semplice di contribuire è segnalando una problematica: un errore di battitura,
un errore grammaticale, un bug nel codice di esempio, e così via

Passi:

* Segnalare un bug attraverso il bug Tracker;

* *(opzionale)* Inviare una patch.

Traduzione
----------

Leggere la :doc:`documentazione <translations>`.

.. _`fork`: http://help.github.com/fork-a-repo/
.. _`richieste di pull`: http://help.github.com/pull-requests/
.. _`Errori di build della documentazione`: http://symfony.com/doc/build_errors
.. _`standard di codice di Twig`: http://twig.sensiolabs.org/doc/coding_standards.html
