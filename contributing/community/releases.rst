Il processo di rilascio
=======================

Questo documento spiega il processo di rilascio di Symfony (dove Symfony è il codice
ospitato sul `repository Git`_ symfony/symfony.

Symfony gestisce i suoi rilasci con un *modello temporale*; un nuovo rilascio di Symfony
esce ogni *sei mesi*: uno in *maggio* e uno in *novembre*.

.. note::

    Tale processo di rilascio è stato adottato a partire da Symfony 2.2 e ogni
    "regola" spiegata in questo documento deve essere rigorosamente eseguita a partire da Symfony
    2.4.

.. _contributing-release-development:

Sviluppo
--------

Il periodo semestrale è suddiviso in due fasi:

* *Sviluppo*: *quattro mesi* per aggiungere nuove caratteristiche e per migliorare
  quelle esistenti;

* *Stabilizazzione*: *due mesi* per chiudere bug, preparare il rilascio e attendere che
  l'intero ecosistema di Symfony (librerie di terze parti, bundle, progetti che usano
  Symfony) sia pronto.

Durante la fase di sviluppo, ogni nuova caratteristica può essere annullata, in caso non
sia pronta per tempo o se non fosse abbastanza stabile da essere inclusa nell'attuale
rilascio finale.

.. _contributing-release-maintenance:

Manutenzione
------------

Ogni versione di Symfony è mantenuta per un periodo fissato di tempo, a seconda del tipo
di rilascio. CI sono due periodi di manutenzione:

* *Fix di bug e fix di sicurezza*: durante questo periodo, possono essere risolti tutti i problemi.
  La fine di questo periodo è indicata come *fine manutenzione* di un
  rilascio.

* *Solo fix di sicurezza*: durante questo periodo, possono essere risolti solamente propblemi relativi
  alla sicurezza. La fine di questo periodo è indicata come *fine
  vita* di un rilascio.

Rilasci standard
~~~~~~~~~~~~~~~~

Un rilascio standard è mantenuto per un periodo di *otto mesi* per i bug
e per un periodo di *quattordici mesi* per i problemi di sicurezza.

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

.. image:: /images/release-process.jpg
   :align: center

* **Giallo** rappresenta la fase di sviluppo
* **Blu** rappresenta la fase di  stabilizzazione
* **Verde** rappresenta il periodo di supporto

Tutto ciò produrrà date e periodi di manutenzione molto attendibili:

+----------+----------+--------------------+-----------+
| Versione | Rilascio | Fine manutenzione  | Fine vita |
+==========+==========+====================+===========+
| 2.0      | 07/2011  | 03/2013 (20 mesi)  | 09/2013   |
+----------+----------+--------------------+-----------+
| 2.1      | 09/2012  | 05/2013 (9 mesi)   | 11/2013   |
+----------+----------+--------------------+-----------+
| 2.2      | 03/2013  | 11/2013 (8 mesi)   | 05/2014   |
+----------+----------+--------------------+-----------+
| **2.3**  | 05/2013  | 05/2016 (36 mesi)  | 05/2017   |
+----------+----------+--------------------+-----------+
| 2.4      | 11/2013  | 07/2014 (8 mesi)   | 01/2015   |
+----------+----------+--------------------+-----------+
| 2.5      | 05/2014  | 01/2015 (8 mesi)   | 07/2016   |
+----------+----------+--------------------+-----------+
| 2.6      | 11/2014  | 07/2015 (8 mesi)   | 01/2016   |
+----------+----------+--------------------+-----------+
| **2.7**  | 05/2015  | 05/2018 (36 mesi)  | 05/2019   |
+----------+----------+--------------------+-----------+
| 2.8      | 11/2015  | 07/2016 (8 mesi)   | 01/2017   |
+----------+----------+--------------------+-----------+
| ...      | ...      | ...                | ...       |
+----------+----------+--------------------+-----------+

.. tip::

    Se si vuole approfondire la linea temporale di una data versione di Symfony,
    si può usare il `calcolatore di linea temporale`_. Si possono anche ottenere tutti i dati come JSON
    via URL, per esempio `http://symfony.com/roadmap.json?version=2.x`.

Retrocompatibilità
------------------

Dopo il rilascio di Symfony 2.3, la retrocompatibilità sarà mantenuta a ogni
costo. Se non fosse possibile, la caratteristica, il miglioramento o la chiusura del bug
saranno programmati per la versione maggiore successiva: Symfony 3.0.

.. note::

    Il lavoro su Symfony 3.0 inizierà nel momento in cui ci saranno abbastanza
    caratteristiche non retrocompatibili in attesa sulla lista delle cose da fare.

Deprecati
---------

Quando non è possibile migliorare l'implementazione di una caratteristica senza
infrangere la retrocompatibilità, resta la possibilità di deprecare
la vecchia implementazione e aggiungerne una nuova. Leggere il documento sulle
:ref:`convenzioni<contributing-code-conventions-deprecations>` per saperne
di più suglia gestione dei depracati in Symfony.

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
una nuova versione ogni due anni e un anno di tempo per
aggiornare.

.. _repository Git: https://github.com/symfony/symfony
.. _SensioLabs:     http://sensiolabs.com/
.. _roadmap:        http://symfony.com/roadmap
.. _calcolatore di linea temporale: http://symfony.com/roadmap
