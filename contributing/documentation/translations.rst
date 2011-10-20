Traduzioni
==========

La documentazione di Symfony2 è scritta in inglese e molte persone sono coinvolte
nel processo di traduzione.

Contribuire
-----------

Prima di tutto, bisogna prendere familiarità con il :doc:`linguaggio <format>`
utilizzato dalla documentazione.

Successivamente, inscriversi alla `lista di Symfony docs`, come
collaboratori.

Infine, trovare il repository *master* per il linguaggio per il quale si vuole contribuire.
Di seguito la lista dei principali repository:

* *Inglese*:    http://github.com/symfony/symfony-docs
* *Francese*:   https://github.com/gscorpio/symfony-docs-fr
* *Italiano*:   https://github.com/garak/symfony-docs-it
* *Giapponese*: https://github.com/symfony-japan/symfony-docs-ja
* *Polacco*:    http://github.com/ampluso/symfony-docs-pl
* *Rumeno*:     http://github.com/sebio/symfony-docs-ro
* *Russo*:      http://github.com/avalanche123/symfony-docs-ru
* *Spagnolo*:   https://github.com/gitnacho/symfony-docs-es

.. note::

   Se si vuole contribuire nella traduzione di un nuovo linguaggio, leggere la
   :ref:`sezione dedicata <translations-adding-a-new-language>`.

Far parte del team di traduzione
--------------------------------

Se si vuole aiutare nella traduzione di alcuni documenti nella propria lingua o risolvere
qualche bug, questo è il semplice processo da seguire per far parte del team: 

* Presentarti sulla `lista di Symfony docs`_;
* *(facoltativo)* Domandare su quali documenti si può lavorare;
* Forkare il repository *master* della propria lingua (cliccare  il bottone "Fork" nella
  pagina di Github);
* Tradurre qualche documento;
* Fare una pull request (cliccare sul bottone "Pull Request" della propria pagina
  di Github);
* Il responsabile della squadra di traduzione accetta le modifiche e ne fa il merge nel
  repository principale;
* La documentazione sul sito è aggiornata ogni notte dal repository
  ufficiale.

.. _translations-adding-a-new-language:

Aggiungere una nuova lingua
---------------------------

Questa sezione fornisce alcune guide per iniziare la traduzione di Symfony2 per una
nuova lingua.

Iniziare la traduzione in una nuova lingua comporta molto lavoro, è necessario parlarne
sulla `lista di Symfony docs`_ e trovare altre persone motivate, che possano dare supporto.

Quando la squadra è pronta, nominare un responsabile: quest'ultimo si occuperà del
repository *master*.

Creare il repository e copiarci i documenti in lingua *inglese*.

La squadra a questo punto può iniziare il processo di traduzione.

Quando la squadra pensa che il repository è in uno stato coerente e stabile (è
tutto tradotto, oppure i documenti non tradotti sono stati rimossi dagli indici, che sono
i file ``index.rst`` e ``map.rst.inc``), il responsabile può fare richiesta che il
repository sia aggiunto alla lista di quelli *master* ufficiali, inviando
un'email a Fabien (fabien.potencier at symfony-project.org).

Manutenzione
------------

La traduzione non finisce quanto tutto è stato tradotto. La documentazione
evolve continuamente (aggiunta di nuovi documenti, bug risolti, paragrafi riorganizzati).
La squadra deve seguire continuamente la documentazione in inglese e
applicare i cambiamenti alla propria versione quanto prima.

.. caution::

    I linguaggi non correttamente manutenuti sono rimossi dalla lista di quelli 
    ufficiali, poiché la documentazione non aggiornata è pericolosa.

.. _Symfony docs mailing-list: http://groups.google.com/group/symfony-docs
