API Stabile di Symfony2
=======================

L'API stabile di Symfony2 è un sottoinsieme di di tutti i metodi pubblici 
pubblicati di Symfony2 (componenti e bundle del core) che condividono le 
seguenti proprietà:

* Il namespace ed il nome della classe non cambieranno;
* Il nome del metodo non cambierà;
* La firma del metodo (argomenti e tipo del valore restituito) non cambierà;
* La semantica di quanto il metodo esegue non cambierà.

L'implementazione tuttavia può cambiare. L'unico caso valido per un cambiamento
nell'API stabile è quello di una modifica per un problema di sicurezza.

.. note::

    L'API stabile si basa su una whitelist. Quindi tutto quanto non elencato
    esplicitamente in questo documento non fa parte dell'API stabile.

.. note::

    Essendo in via di sviluppo l'elenco definitivo verrà pubblicato quando la
    versione finale di Symfony2 verrà rilasciata. Nel frattempo, se pensate che
    qualche metodo dovesse apparire in questa lista, iniziare una discussione
    sulla mailing-list degli sviluppatori di Symfony.

.. tip::

    Ogni metodo facente parte dell'API stabile è segnalato come tale sul sito
    dell'API di Symfony2 (con l'annotazione ``@stable``).

.. tip::

    Ogni bundle di terze parti dovrebbe pubblicare la sua API stabile.

HttpKernel Component
--------------------

* HttpKernelInterface:::method:`Symfony\\Components\\HttpKernel\\HttpKernelInterface::handle`
