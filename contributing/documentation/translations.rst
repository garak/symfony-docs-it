Traduzioni
==========

La documentazione di Symfony è scritta originariamente in inglese e molte persone
sono coinvolte nel processo di traduzione.

.. note::

    Il progetto Symfony sconsiglia ufficialmente l'inizio di nuove traduzioni della
    documentazione. In effetti, c'è `una discussione in corso`_, all'interno
    della comunità, su benefici e problemi di traduzioni mantenute dalla comunità.

Contribuire
-----------

Prima di tutto, bisogna diventare familiari con il :doc:`linguaggio di markup<format>` usato dalla
documentazione.

Successivamente, iscriversi alla `lista Symfony docs`, per poter 
collaborare.

Infine, trovare il repository *master* per il linguaggio per il quale si vuole contribuire.
Di seguito la lista dei repository *master* ufficiali:

* *Inglese*:  https://github.com/symfony/symfony-docs
* *Francese*:   https://github.com/symfony-fr/symfony-docs-fr
* *Italiano*:  https://github.com/garak/symfony-docs-it
* *Giapponese*: https://github.com/symfony-japan/symfony-docs-ja
* *Portoghese (Brasile)*:  https://github.com/andreia/symfony-docs-pt-BR

.. note::

   Se si vuole contribuire nella truduzione di un nuovo linguaggio, leggere la
   :ref:`sezione dedicata <translations-adding-a-new-language>`.

Far parte del team di traduzione
--------------------------------

Se si vuole aiutare nella traduzione di alcuni documenti nella propria lingua o risolvere dei bug, questo è il semplice
processo da seguire per far parte del team: 

* Presentarsi sulla `lista Symfony docs`_;
* *(opzionale)* Chiedere su quali documenti si puo lavorare;
* Forkare il repository *master* della propria lingua (cliccare  il bottone
  "Fork" nella pagina di GitHub);
* Tradurre qualche documento;
* Fare una richiesta di pull (cliccare sul bottone "Pull Request" della propria pagina di
  GitHub);
* Il team manager accetta le modifiche e ne fa il merge nel repository
  principale;
* La documentazione sul sito è aggiornata ogni notte dal repository
  ufficiale.

.. _translations-adding-a-new-language:

Aggiungere una nuova lingua
---------------------------

Questa sezione fornisce alcune guide per iniziare la traduzione di Symfony per una nuova
lingua.

Iniziare la traduzione in una nuova lingua comporta molto lavoro, è necessario parlarne sulla
`lista Symfony docs`_ e trovare altre persone che diano supporto.

Quando il team è pronto, nominare un manager; Quest'ultimo sarà il responsabile del repository
*master*.

Creare il repository e copiarci i documenti *in lingua inglese*.

Il team a questo punto può iniziare il processo di traduzione.

Quando il team pensa che il repository sia in uno stato coerente e stabile (è tutto
tradotto, oppure i documenti non tradotti sono stati rimossi dai toctree, che sono i
file index.rst e map.rst.inc), il team manager può fare richiesta che il repository
sia aggiunto alla lista di quelli *master* ufficiali, inviando un'email a Fabien
(fabien.potencier at symfony.com).

Manutenzione
------------

La traduzione non finisce quanto tutto è stato tradotto. La documentazione
evolve continuamente (aggiunta di nuovi documenti, bug risolti, paragrafi riorganizzati).
Il team deve seguire continuamente la documentazione in
inglese e applicare i cambiamenti alla propria versione quanto prima.

.. caution::

   I linguaggi non correttamente manutenuti sono rimossi dalla lista di quelli 
   ufficiali, poiché la documentazione non aggiornata è pericolosa

.. _`una discussione in corso`: https://github.com/symfony/symfony-docs/issues/4078
.. _lista Symfony docs: http://groups.google.com/group/symfony-docs
