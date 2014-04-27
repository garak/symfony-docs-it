.. index::
   single: Doctrine; Comandi console ORM
   single: CLI; Doctrine ORM

Comandi console
---------------

L'integrazione con l'ORM di Doctrine2 offre vari comandi di console, sotto lo spazio dei nomi
``doctrine``. Per vedere la lista dei comandi, si può usare il
comando ``list``:

.. code-block:: bash

    $ php app/console list doctrine

Sarà mostrata una lista di comandi disponibili. Si possono trovare maggiori informazioni
su tali comandi (o su qualsiasi comando di Symfony) tramite il comando ``help``.
Per esempio, per avere dettagli su ``doctrine:database:create``,
eseguire:

.. code-block:: bash

    $ php app/console help doctrine:database:create

Alcuni comandi interessanti includono:

* ``doctrine:ensure-production-settings`` - verifica che il sistema
  sia configurato in modo efficiente per la produzione. Andrebbe eseguito sempre
  in ambiente ``prod``:

  .. code-block:: bash

      $ php app/console doctrine:ensure-production-settings --env=prod

* ``doctrine:mapping:import`` - consente a Doctrine un'introspezione di una base dati
  esistente e la creazione di informazioni di mappatura. Per maggiori informazioni, vedere
  :doc:`/cookbook/doctrine/reverse_engineering`.

* ``doctrine:mapping:info`` - elenca tutte le entità di cui Doctrine
  è a conoscenza e se ci siano o meno errori di base con la mappatura.

* ``doctrine:query:dql`` e ``doctrine:query:sql`` - consentono di eseguire query
  DQL o SQL direttamente da linea di comando.
