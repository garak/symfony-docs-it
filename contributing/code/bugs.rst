Segnalare un bug
================

A chi dovesse incontrare un bug in Symfony, chiediamo di segnalarlo. Questo aiuta
a rendere migliore Symfony.

.. caution::

    Chi ritenga di aver trovato un problema di sicurezza, per favore, segua
    invece l'apposita :doc:`procedura <security>`.

Prima di inviare un bug:

* Ricontrollare la :doc:`documentazione </index>` ufficiale, per verificare che non si stia facendo 
  un uso scorretto del framework;

* Chiedere assistenza alla `lista degli utenti`_ , al `forum`_ o al
  `canale IRC`_ #symfony, se non si è sicuri che sia effettivamente un bug.

Se il problema è effettivamente un bug, segnalarlo utilizzando
il `tracker`_ ufficiale e seguendo alcune regole:

* Utilizzare il campo titolo per descrivere chiaramente la questione;

* Descrivere i passi necessari per riprodurre il bug, con brevi esempi di codice
  (la cosa migliore è fornire un test unitario per replicare il bug)

* Se il bug riscontrato affligge più livelli, fornire un semplice test unitario
  che fallisca potrebbe non essere sufficiente. In questo caso, eseguire un fork di
  `Symfony Standard Edition`_ e riprodurre il problema su un nuovo ramo;

* Fornire il maggior numero di dettagli possibile sul proprio ambiente (sistema operativo, versione di PHP,
  versione di Symfony, estensioni abilitate, ...)

* *(facoltativo)* Allegare una :doc:`patch <patches>`.

.. _lista degli utenti: http://groups.google.com/group/symfony2
.. _forum: http://forum.symfony-project.org/
.. _canale IRC: irc://irc.freenode.net/symfony
.. _tracker: https://github.com/symfony/symfony/issues
.. _Symfony Standard Edition: https://github.com/symfony/symfony-standard/
