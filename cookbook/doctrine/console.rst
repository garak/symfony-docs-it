.. index::
   single: Doctrine; Comandi di console per ORM
   single: CLI; Doctrine ORM

Comandi di console
------------------

L'integrazione con ORM di Doctrine2 offre vari comandi di console, sotto lo spazio dei nomi
``doctrine``. Per vedere una lista di comandi, si può usare il comando
``list``:

.. code-block:: bash

    $ php app/console list doctrine

Sarà mostrata una lista di comandi disponibili. Si possono trovare maggiori informazioni
su questi comandi (o su qualsiasi altro comando di Symfony) eseguendo il comado ``help``.
Per esempio, per avere dettagli su ``doctrine:database:create``,
eseguire:

.. code-block:: bash

    $ php app/console help doctrine:database:create

Alcuni comandi interessanti:

* ``doctrine:ensure-production-settings`` - verifica se l'ambiente attuale
  sia configurato in modo efficiente per la produzione. Andrebbe sempre
  eseguito in ambiente ``prod``:

  .. code-block:: bash

      $ php app/console doctrine:ensure-production-settings --env=prod

* ``doctrine:mapping:import`` - consente a Doctrine un'introspezione di una base dati
  esistente e la creazione di informazioni di mappature. Per maggiori informazioni, vedere
  :doc:`/cookbook/doctrine/reverse_engineering`.

* ``doctrine:mapping:info`` - mostra tutte le entità di cui Doctrine è a
  conoscenza e se ci siano o meno errori di base nella mappatura.

* ``doctrine:query:dql`` e ``doctrine:query:sql`` - consentono di eseguire query
  DQL o SQL direttamente dalla linea di comando.
