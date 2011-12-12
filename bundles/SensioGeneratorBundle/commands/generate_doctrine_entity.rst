Generare un nuovo stub per un'entità Doctrine
=============================================

Utilizzo
--------

Il comando ``generate:doctrine:entity`` genera un nuovo stub per un'entità Doctrine,
inclusa la definizione della mappatura, le proprietà delle classi, i getter e i setter.

Per impostazione predefinita, il comando gira in modalità interattiva e pone domande per
determinare il nome del bundle, la posizione, il formato di configurazione e la
struttura:

.. code-block:: bash

    php app/console generate:doctrine:entity

Il comando può essere eseguito in modalità non interattiva, usando l'opzione
`--no-interaction`, senza dimenticare di passare tutte le opzioni necessarie:

.. code-block:: bash

    php app/console generate:doctrine:entity --non-interaction --entity=AcmeBlogBundle:Post --fields="title:string(100) body:text" --format=xml

Opzioni disponibili
-------------------

* ``--entity``: Il nome dell'entità, fornito nella notazione breve, contenente il nome
  del bundle che contiene l'entità e il nome dell'entità stessa. Per esempio:
  ``AcmeBlogBundle:Post``:

    .. code-block:: bash

        php app/console generate:doctrine:entity --entity=AcmeBlogBundle:Post

* ``--fields``: L'elenco dei campi da inserire nell'entità:

    .. code-block:: bash

        php app/console generate:doctrine:entity --fields="title:string(100) body:text"

* ``--format``: (**annotation**) [valori: yml, xml, php o annotation]
  Determina il formato da usare per i file di configurazione, come quelli delle
  rotte. Il valore predefinito è ``annotation``:

    .. code-block:: bash

        php app/console generate:doctrine:entity --format=annotation

* ``--with-repository``: Questa opzione dice se generare o meno la classe
  `EntityRepository` relativa:

    .. code-block:: bash

        php app/console generate:doctrine:entity --with-repository

