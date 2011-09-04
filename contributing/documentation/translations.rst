Traduzioni
============

La documentazione di Symfony2 è scritta in Inglese e molte persone sono coinvolte nel processo di traduzione.

Contribuire
------------

Prima di tutto, bisogna diventare famigliari con :doc:`markup language <format>` utilizzato dalla documentazione

Successivamente, inscriversi alla `Symfony docs mailing-list` come collaboratori

In fine, trovare il repository *master* per il linguaggio per il quale si vuole contribuire.
Di seguito la lista dei principali repository:

* *English*:  http://github.com/symfony/symfony-docs
* *Russian*:  http://github.com/avalanche123/symfony-docs-ru
* *Romanian*: http://github.com/sebastian-ionescu/symfony-docs-ro

.. note::
   Se si vuole contribuire nella truduzione di un nuovo linguaggio, leggere la
   :ref:`sezione dedicata <translations-adding-a-new-language>`.

Far parte del team di traduzione:
----------------------------

Se si vuole aiutare nella traduzione di alcuni documenti nella propria lingua o fissare qualche bug, questo è il semplice
processo da seguire per far parte del team: 

* Presentarti su `Symfony docs mailing-list`_;
* *(opzionale)* Domandare su quali documenti si puo lavorare;
* Fork il repository *master* della tua lingua (cliccare  il bottone "Fork" nella pagina di Github);
* Tradurre qualche documento;
* Fare una richiesta di PUll  (cliccare sul bottone "Pull Request" della tua pagina di Github);
* Il team manager accetta le modifiche e le mergia nel repositori principale;
* La documentazione sul sito è aggiornata ogni notte dal repository ufficiale.

.. _translations-adding-a-new-language:

Aggiungere una nuova lingua
---------------------

Questa sezione fornisce alcune guide per iniziare la traduzione di Symfony2 per una nuova lingua.

Iniziare la trduzione in una nuova lingua comporta molto lavoro, è necessario parlarne sulla
`Symfony docs mailing-list`_ e trovare altre persone che diano supporto.

Quando il team è pronto, nominare un manager; Quest'ultimo sarà il responsabile del repository *master* 

Creare il repository e copiarci i documenti in lingua inglese.

Il team a questo punto può iniziare il processo di traduzione.

Quando il team pensa che il repository è in uno stato consistente e stabile ( è tutto tradotto, oppure i documenti non tradotti
saranno rimossi) il tema manager può fare richiesta che il repository sia aggiunto alla lista di quelli *master* ufficiali inviando
una mail a Fabien (fabien.potencier at symfony-project.org).

Manutenzione
-----------

Translation does not end when everything is translated. The documentation is a
moving target (new documents are added, bugs are fixed, paragraphs are
reorganized, ...). The translation team need to closely follow the English
repository and apply changes to the translated documents as soon as possible.

La traduzione non finisce quanto tutto è stato tradotto. La documentazione
evolve continuamente ( aggiunta di nuovi documenti, fissati bug, paragrafi riorganizzati)
. Il team deve seguire continuamente la documentazione in
inglese e applicare i cambiamenti alla propria versione quanto prima.

.. Attenzione::
   I linguaggi non correttamente manutenuti sono rimossi dalla lista di quelli 
   ufficiali poichè la documentazione non aggiornata è pericolosa

.. _Symfony docs mailing-list: http://groups.google.com/group/symfony-docs