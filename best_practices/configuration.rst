Configurazione
==============

La configurazione di un'applicazione, normalmente, coinvolge diverse parti (ad es. infrastruttura
tecnologica e sicurezza) e diversi ambienti (sviluppo, produzione).
Proprio per questo, Symfony raccomanda di suddividere la configurazione in
tre parti.

.. _config-parameters.yml:

Configurazione relativa all'infrastruttura
------------------------------------------

.. best-practice::

    Definire le opzioni della configurazione relativa all'infrastruttura tecnologica nel file
    ``app/config/parameters.yml``.

Il file ``parameters.yml`` segue questa regola e definisce le opzioni relative alla
base dati e al server di posta:

.. code-block:: yaml

    # app/config/parameters.yml
    parameters:
        database_driver:   pdo_mysql
        database_host:     127.0.0.1
        database_port:     ~
        database_name:     symfony
        database_user:     root
        database_password: ~

        mailer_transport:  smtp
        mailer_host:       127.0.0.1
        mailer_user:       ~
        mailer_password:   ~

        # ...

Queste opzioni non sono definite nel file ``app/config/config.yml``, perché non hanno niente a che fare
con il comportamento dell'applicazione. In altre parole, l'applicazione non si deve preoccupare di
sapere dov'è posizionata la base dati o come vi accede fintanto che la base dati è configurata correttamente.

.. _best-practices-canonical-parameters:

Parametri canonici
------------------

.. best-practice::

    Definire tutti i parametri dell'applicazione nel file
    ` app/config/parameters.dist.yml``.

Dalla versione 2.3, Symfony include un file di configurazione chiamato ``parameters.dist.yml``,
che contiene tutti i parametri di configurazione che devono essere definiti affinché 
l'applicazione funzioni correttamente.

Ogni volta che viene definito un nuovo parametro di configurazione per l'applicazione,
esso dovrebbe essere aggiunto a questo file e tale modifica dovrebbe essere registrata anche
sul sistema di controllo di versione. Quando lo sviluppatore aggiorna il progetto o esegue un deploy,
Symfony controllerà eventuali differenze tra il file canonico 
`parameters.dist.yml`` e il file locale ``parameters.yml``. In presenza
di differenze Symfony chiederà di fornire un valore per il nuovo parametro e lo aggiungerà
al file locale ``parameters.yml``.

Configurazione relativa all'applicazione
----------------------------------------

.. best-practice::

    Definire le opzioni di configurazione relative all'applicazione nel file
    ``app/config/config.yml``.

Il file ``config.yml`` contiene le opzioni usate dall'applicazione per modificare
il suo comportamento, come ad esempio il mittente delle email o l'abilitazione di
`feature toggle`_. È possibile definire questi valori anche in ``parameters.yml``,
ma questo aggiungerebbe un livello di configurazione extra non necessario, perché
solitamente non si ha bisogno o non si vuole che questi valori cambino su ogni server.

Le opzioni di configurazione definite nel file ``config.yml`` solitamente variano da
un :doc:`ambiente </cookbook/configuration/environments>` all'altro. È per questo
che Symfony include già i file ``app/config/config_dev.yml`` e ``app/config/config_prod.yml``,
in modo che si possano ridefinire valori specifici per ciascun ambiente.

Costanti od opzioni di Configurazione
-------------------------------------

Uno degli errori più comuni nel definire la configurazione dell'applicazione è creare nuove
opzioni per valori che non cambieranno mai, come ad esempio il numero degli elementi mostrati
in una paginazione.

.. best-practice::

    Usare costanti per definire opzioni di configurazione che cambieranno raramente.

L'approccio tradizionale della definizione delle opzioni di configurazione ha costretto molte applicazioni
Symfony a includere opzioni come la seguente, che controlla il numero di post da mostrare
nell'homepage del blog:

.. code-block:: yaml

    # app/config/config.yml
    parameters:
        homepage.num_items: 10

Se avete fatto qualcosa del genere in passato, è probabile che in effetti non abbiate **mai** avuto
realmente bisogno di cambiare quel valore. Creare un'opzione di configurazione
per un valore che non si andrà mai a modificare è inutile. La nostra raccomandazione è di
definire questi valori come costanti nell'applicazione. Si potrebbe, ad esempio, definire
una costante ``NUM_ITEMS`` nell'entità ``Post``:

.. code-block:: php

    // src/AppBundle/Entity/Post.php
    namespace AppBundle\Entity;

    class Post
    {
        const NUM_ITEMS = 10;

        // ...
    }

Il vantaggio più importante nella definizione di costanti è che si possono utilizzare dappertutto
nell'applicazione. Quando si usano i parametri, essi sono disponibili solamente tramite l'accesso al
contenitore di Symfony.

Le costanti possono essere usate, per esempio, nei template di Twig, grazie alla 
`funzione constant()`_:

.. code-block:: html+twig

    <p>
        Visualizzo i {{ constant('NUM_ITEMS', post) }} risultati più recenti.
    </p>

Così facendo, sia le entità di Doctrine che i repository possono accedere facilmente a questi
valori, mentre le stesse classi non posso accedere ai parametri del contenitore:

.. code-block:: php

    namespace AppBundle\Repository;

    use Doctrine\ORM\EntityRepository;
    use AppBundle\Entity\Post;

    class PostRepository extends EntityRepository
    {
        public function findLatest($limit = Post::NUM_ITEMS)
        {
            // ...
        }
    }

L'unico svantaggio da considerare nell'utilizzo delle costanti come opzioni di configurazione, è
che non possono essere ridefinite facilmente nei test.

Configurazione semantica: non usarla
------------------------------------

.. best-practice::

    Non definire nei bundle una configurazione semantica per il contenitore.

Come spiegato nella ricetta :doc:`/cookbook/bundles/extension`, i bundle di Symfony
possono gestire le opzioni di configurazione in due modi: la configurazione normale del servizio,
attraverso il file ``services.yml``, e la configurazione semantica, attraverso una classe speciale
di tipo ``*Extension``.

Sebbene la configurazione semantica è molto più potente e fornisce interessanti caratteristiche,
come la validazione delle opzioni di configurazione, la quantità di lavoro necessaria per la
definizione del bundle è notevole e non vale la pena cimentarsi per bundle non rivolti
a terzi.

Spostare le opzioni di configurazione sensibili al di fuori di Symfony
----------------------------------------------------------------------

Quando si lavora con opzioni sensibili, come le credenziali di accesso alla base dati, si raccomanda
di spostarle al di fuori dell'applicazione Symfony e di renderle disponibili tramite variabili
d'ambiente. Si può imparare come farlo nella seguente ricetta:
:doc:`/cookbook/configuration/external_parameters`

.. _`feature toggle`: http://en.wikipedia.org/wiki/Feature_toggle
.. _`funzione constant()`: http://twig.sensiolabs.org/doc/functions/constant.html
