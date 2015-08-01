Il processo di rilascio
=======================

Questo documento spiega il processo di rilascio di Symfony (cioè del codice
ospitato sul `repository Git`_ symfony/symfony).

Symfony gestisce i suoi rilasci con un *modello temporale*; un nuovo rilascio di Symfony
esce ogni *sei mesi*: uno in *maggio* e uno in *novembre*.

.. tip::

    Il significato di "minore" proviene dalla strategia di `versionamento semantico`_.

Ciascuna versione minore si attiene al medesimo processo, che inizia con un
periodo di sviluppo, seguito da un periodo di manutenzione.

.. note::

    Tale processo di rilascio è stato adottato a partire da Symfony 2.2 e ogni
    "regola" spiegata in questo documento deve essere rigorosamente seguita a partire da Symfony
    2.4.

.. _contributing-release-development:

Sviluppo
--------

Il periodo semestrale è suddiviso in due fasi:

* *Sviluppo*: *quattro mesi* per aggiungere nuove caratteristiche e per migliorare
  quelle esistenti;

* *Stabilizzazione*: *due mesi* per chiudere bug, preparare il rilascio e attendere che
  l'intero ecosistema di Symfony (librerie di terze parti, bundle, progetti che usano
  Symfony) sia pronto.

Durante la fase di sviluppo, ogni nuova caratteristica può essere annullata, in caso non
sia pronta per tempo o se non fosse abbastanza stabile da essere inclusa nell'attuale
rilascio finale.

.. _contributing-release-maintenance:

Manutenzione
------------

Ogni versione di Symfony è mantenuta per un periodo fissato di tempo, a seconda del tipo
di rilascio. Ci sono due periodi di manutenzione:

* *Fix di bug e fix di sicurezza*: durante questo periodo, possono essere risolti tutti i tipi di problemi.
  La fine di questo periodo è indicata come *fine manutenzione* di un
  rilascio.

* *Solo fix di sicurezza*: durante questo periodo, possono essere risolti solamente problemi relativi
  alla sicurezza. La fine di questo periodo è indicata come *fine
  vita* di un rilascio.

Rilasci standard
~~~~~~~~~~~~~~~~

Un rilascio standard è mantenuto per un periodo di *otto mesi* per i bug
e per un periodo di *quattordici mesi* per i problemi di sicurezza.

.. _releases-lts:

Rilasci a supporto prolungato
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Ogni due anni, viene pubblicato un rilascio a supporto prolungato (detto anche LTS, Long
Term Support). Ogni rilascio LTS è supportato per un periodo di *tre anni* per i bug
e per un periodo di *quattro anni* per i problemi di sicurezza.

.. note::

    Un supporto a pagamento, successivo ai tre anni di supporto della comunità, potrà
    essere acquistato da `SensioLabs`_.

Piano
-----

Di seguito il piano per le prime due versioni che useranno questo modello di rilascio:

.. image:: /images/contributing/release-process.jpg
   :align: center

* **Giallo** rappresenta la fase di sviluppo
* **Blu** rappresenta la fase di  stabilizzazione
* **Verde** rappresenta il periodo di supporto

Tutto ciò produrrà date e periodi di manutenzione molto attendibili:

========  =======  ========  ======================  =========
Versione  Freeze   Rilascio  Fine manutenzione       Fine vita
========  =======  ========  ======================  =========
2.0       05/2011  07/2011   03/2013 (20 mesi)       09/2013
2.1       07/2012  09/2012   05/2013 (9 mesi)        11/2013
2.2       01/2013  03/2013   11/2013 (8 mesi)        05/2014
**2.3**   03/2013  05/2013   05/2016 (36 mesi)       05/2017
2.4       09/2013  11/2013   09/2014 (10 mesi [1]_)  01/2015
2.5       02/2014  05/2014   01/2015 (8 mesi)        07/2015
2.6       09/2014  11/2014   07/2015 (8 mesi)        01/2016
**2.7**   02/2015  05/2015   05/2018 (36 mesi)       05/2019
**2.8**   09/2015  11/2015   11/2018 (36 mesi [2]_)  11/2019
3.0       09/2015  11/2015   07/2016 (8 mesi)        01/2017
3.1       02/2016  05/2016   01/2017 (8 mesi)        07/2017
3.2       09/2016  11/2016   07/2017 (8 mesi)        01/2018
**3.3**   02/2017  05/2017   05/2020 (36 mesi)       05/2021
...       ...      ...       ...                     ...
========  =======  ========  ======================  =========

.. [1] La manutenzione di Symfony 2.4 è stata `estesa a settembre 2014`_.
.. [2] Symfony 2.8 è l'ultima versione del ramo 2.x di Symfony.

.. tip::

    Se si vuole approfondire la linea temporale di una data versione di Symfony,
    si può usare il `calcolatore di linea temporale`_. Si possono anche ottenere tutti i dati come JSON
    via URL, per esempio `http://symfony.com/roadmap.json?version=2.x`.

.. tip::

    Ogni volta che accade un evento importante legato alle versioni di Symfony (una versione
    raggiunge la fine della manutenzione o una nuova versione patch viene rilasciata, per
    esempio), si può ricevere una notifica automatica via email, se ci iscrive
    alla pagina `roadmap notification`_.

Retrocompatibilità
------------------

Esiste una  :doc:`promessa di retrocompatibilità </contributing/code/bc>` molto
stretta, che consente agli sviluppatori di aggiornare con fiducia da una versione minore
di Symfony a quella successiva.

Ogni volta che la retrocompatibilità non sarà possibile, la caratteristica,
il miglioramento o la sistemazione del bug saranno programmate per la versione maggiore successiva.

.. note::

    Il lavoro su una nuova versione maggiore di Symfony inizierà nel momento in cui ci saranno abbastanza
    caratteristiche non retrocompatibili in attesa sulla lista delle cose da fare.

Deprecati
---------

Quando non è possibile migliorare l'implementazione di una caratteristica senza
infrangere la retrocompatibilità, resta la possibilità di deprecare
la vecchia implementazione e aggiungerne una nuova. Leggere il documento sulle
:ref:`convenzioni <contributing-code-conventions-deprecations>` per saperne
di più sulla gestione dei deprecati in Symfony.

Motivazioni
-----------

Questo processo di rilascio è stato adottato per fornire maggiore *prevedibilità* e
*trasparenza*. È stato discusso sulla base dei seguenti obiettivi:

* Abbreviare il ciclo di rilascio (consentendo agli sviluppatori di beneficiare più
  velocemente delle nuove caratteristiche);
* Dare più visibilità agli sviluppatori che usando il framework e ai progetti open source
  che usano Symfony;
* Migliorare l'esperienza dei contributori del nucleo di Symfony: ognuno sa quando una
  caratteristica sarà disponibile in Symfony;
* Coordinare la linea temporale di Symfony con progetti PHP popolari che lavorano
  con Symfony e con progetti che usano Symfony;
* Dare tempo all'ecosistema Symfony di stare al passo con le nuove versioni
  (autori di bundle, scrittori di documentazione, traduttori, ecc.).

Il periodo semestrale è stato scelto perché un anno conterrà due rilasci. Inoltre consente
di avere molto tempo per lavorare su una nuova caratteristica e consente alle
caratteristiche non ancora pronte di essere rimandate alla versione successiva, senza
dover aspettare troppo a lungo per il prossimo ciclo.

La doppia modalità di manutenzione è stata adottata per far felice ogni utente di Symfony.
Chi preferisce rilasci veloci e vuole usare le ultime versioni potrà usare i rilasci
standard: una nuova versione ogni sei mesi e due mesi di tempo per
aggiornare. Le aziende che desiderano maggiore stabilità possono usare i rilasci LTS:
una nuova versione ogni due anni e un anno di tempo per aggiornare.

.. _versionamento semantico: http://semver.org/
.. _repository Git: https://github.com/symfony/symfony
.. _SensioLabs:     http://sensiolabs.com/
.. _roadmap notification: http://symfony.com/roadmap
.. _estesa a settembre 2014: http://symfony.com/blog/extended-maintenance-for-symfony-2-4
.. _calcolatore di linea temporale: http://symfony.com/roadmap
