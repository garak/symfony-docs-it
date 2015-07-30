.. index::
   single: Symfony Framework Best Practice

Best Practice Symfony Framework 
===============================

Symfony è conosciuto per essere un framework *molto* flessibile ed è infatti utilizzato
in diversi ambiti come ad esempio piccoli siti web, applicazioni enterprise con miliardi di richieste
e anche come base per altri framework. Da quando è stato pubblicato nel luglio 2011,
la comunità ha imparato molto su ciò che Symfony è in grado di fare e su quale sia il modo *migliore* per farlo.

Le diverse risorse create dalla comunità, da articoli di blog a presentazioni a conferenze, hanno creato
una serie di raccomandazioni e pratiche ufficiose per lo sviluppo di applicazioni Symfony.
Purtroppo molte di queste raccomandazioni sono sbagliate e
oltre a complicare lo sviluppo delle stesse applicazioni non sono allineate con la filosofia
originale e pragmatica dei creatori di Symfony.

Di che parla questa guida?
--------------------------

Lo scopo di questa guida è quello di risolvere il problema sopra menzionato stabilendo una serie di
**buone pratiche ufficiali per lo sviluppo di applicazioni web con il framework Symfony**.
Queste pratiche sono quelle che meglio si adattano alla filosofia immaginata dal creatore originale del framework
`Fabien Potencier`_.

.. note::

    **Best practice** è un termine che vuol dire *"una procedura ben definita, nota per
    produrre risultati quasi ottimali"*. Ciò è esattamente quello che questa guida di propone
    di fornire. Anche se non tutti saranno d'accordo con ogni raccomandazione, c'è una ferma
    convinzione che saranno tutte d'aiuto per costruire grandi applicazioni con minore complessità.

Questa guida è **particolarmente indicata** per:

* Siti web e applicazioni web sviluppato con il framework Symfony.

Per altre situazioni, questa guida potrebbe essere un buon **punto di partenza**, che si
potrà **estendere per soddisfare le proprie esigenze specifiche**:

* Bundle condivisi pubblicamente con la comunità di Symfony;
* Sviluppatori esperti o squadre che hanno creato i propri standard;
* Alcune applicazioni complesse, con requisiti altamente personalizzati;
* Bundle che potrebbero essere condivisi internamente in un'azienda.

Si sa che le vecchie abitudini sono dure a morire e qualcuno potrebbe meravigliarsi o non essere
d'accordo con alcune di queste regole. Nonostante ciò, crediamo che seguendo questi consigli potrete
sviluppare le applicazioni più velocemente, con una minore complessità e con la stessa o perfino una
migliore qualità. Inoltre le raccomandazioni saranno continuamente aggiornate e migliorate.

In ogni caso, considerare che le raccomandazioni sono ***opzionali** e che ogni sviluppatore
può decidere se adottarle oppure no. Se si preferisce continuare a sviluppare utilizzando
altre pratiche e metodologie, si può continuare a farlo.
Symfony è abbastanza flessibile da adattarsi a ogni necessità e questo non
cambierà mai.

A chi è rivolto il libro? (Suggerimento: non è un tutorial)
-----------------------------------------------------------

Questa guida è pensata per qualsiasi sviluppatore Symfony, sia esperto che principiante.
Non essendo un tutorial passo passo, però, è necessaria una conoscenza basilare di
Symfony, per seguire la guida al meglio. Per chi è proprio agli inizi, benvenuto nella comunità!
Meglio iniziare con il tutorial :doc:`la guida rapida </quick_tour/the_big_picture>`. 

Inoltre è stato deciso di mantenere questa guida più piccola possibile, per essere molto facile da leggere.
Non ci si soffermerà su spiegazioni che si possono trovare nella documentazione ufficiale di Symfony,
come discussioni sulla dependency injection o sui front controller. Ci si soffermerà unicamente
a spiegare come fare ciò che già si conosce.

L'applicazione
--------------

Insieme a questa guida ci sarà un'applicazione di esempio, sviluppata seguendo le
best practice. Questo progetto, chiamato Symfony Demo, può essere installato
tramite l'installatore di Symfony. `Scaricare e installare`_ l'installatore
e quindi eseguire il comando seguente, per scaricare l'applicazione:

.. code-block:: bash

    # Linux and Mac OS X
    $ symfony demo

    # Windows
    c:\> php symfony demo

**L'applicazione è un semplice blog**, che consentirà di concentrarsi sui
concetti di Symfony, senza sconfinare in particolarità proprie dell'applicazione,
più complesse. Invece di sviluppare l'applicazione passo passo in 
questa guida, ci saranno pezzi di codice all'interno dei singoli capitoli.

Non aggiornare un'applicazione esistente
----------------------------------------

Dopo aver letto questo manuale, qualcuno potrebbe pensare di rifattorizzare la
sua applicazione Symfon. La raccomandazione è chiara e netta: **meglio
non rifattorizzare applicazioni esistenti per aderire a queste best
practice**. Le ragioni per non farlo sono diverse:

* Le applicazioni esistenti non sono sbagliate, seguono semplicemente un insieme diverso
  di linee guida;
* Una rifattorizzazione totale espone a possibili errori
  nell'applicazione;
* L'ammontare di lavoro speso potrebbe essere dedicato con più profitto a migliorare
  i test o ad aggiungere funzionalità che forniscano valore reale agli utenti finali.

.. _`Fabien Potencier`: https://connect.sensiolabs.com/profile/fabpot
.. _`Scaricare e installare`: http://symfony.com/download
