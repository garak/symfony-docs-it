Organizzare la logica di business
=================================

In informatica, La logica di business è "la parte del programma che codifica
le regole di business della vita reale, che determinano il modo in cui i dati possono essere
creati, memorizzati e modificati" (vedere la `definizione completa`_).

Nelle applicazioni Symfony, la logica di business comprende tutto il codice implementato per
l'applicazione non relativo al framework. Le classi di dominio, le entità Doctrine
e classiche classi PHP utilizzate come servizi rappresentano buoni esempi di logica
di business.

Nella maggior parte dei progetti, la logica di business dovrebbe essere inserita dentro AppBundle.
All'interno di tale bundle è possibile creare qualsiasi gerarchia di cartelle, come struttura organizzativa.

.. code-block:: text

    progetto-symfony/
    ├─ app/
    ├─ src/
    │  └─ AppBundle/
    │     └─ Utils/
    │        └─ MiaClasse.php
    ├─ vendor/
    └─ web/

Mettere classi fuori dal bundle?
--------------------------------

Non vi è alcuna limitazione tecnica che ci impedisca di mettere la logica di business
fuori dal bundle. Se si vuole si può creare il proprio namespace
dentro **src/** e mettere tutto là dentro:

.. code-block:: text

    progetto-symfony/
    ├─ app/
    ├─ src/
    │  ├─ Acme/
    │  │   └─ Utils/
    │        └─ MiaClasse.php
    │  └─ AppBundle/
    ├─ vendor/
    └─ web/

.. tip::

    La raccomandazione di usare il bundle `AppBundle` è giustificata dal voler rendere
    tutto più facile da gestire. Se sei così esperto da sapere cosa è necessario mettere
    dentro un bundle e cosa mettere invece fuori, sentiti libero di farlo.

Servizi: nomi e formati
-----------------------

La nostra applicazione blog ha bisogno di una utility in grado di trasformare il titolo di ogni post
(ad es. "Ciao mondo") nel suo relativo slug (ad es. "ciao-mondo").
Lo slug verrà quindi usato come parte dell'URL del post.

Creiamo una classe ``Slugger`` dentro ``src/AppBundle/Utils/`` e aggiungiamo il metodo
``slugify()`:

.. code-block:: php

    // src/AppBundle/Utils/Slugger.php
    namespace AppBundle\Utils;

    class Slugger
    {
        public function slugify($string)
        {
            return preg_replace(
                '/[^a-z0-9]/', '-', strtolower(trim(strip_tags($string)))
            );
        }
    }

Definiamo quindi un nuovo servizio per tale classe.

.. code-block:: yaml

    # app/config/services.yml
    services:
        # keep your service names short
        app.slugger:
            class: AppBundle\Utils\Slugger

Per la definizione dei nomi dei servizi solitamente si sceglie
di utilizzare il nome e la posizione della classe per evitare collisioni di nomi.
Pertanto il servizio dovrebbe chiamarsi **app.utils.slugger**.
Tuttavia se si usano nomi dei servizi brevi il codice risulterà più facile da leggere e da usare.

.. best-practice::

    Il nome dei servizi dovrebbe essere il più breve possibile,
    ma abbastanza univoco da poter cercare il servizio nel progetto, se
    necessario.

Adesso è possibile usare lo slugger da ogni controller, come 
``AdminController`:

.. code-block:: php

    public function createAction(Request $request)
    {
        // ...

        if ($form->isSubmitted() && $form->isValid()) {
            $slug = $this->get('app.slugger')->slugify($post->getTitle());
            $post->setSlug($slug);

            // ...
        }
    }

Formato dei servizi: YAML
-------------------------

Per la definizione del servizio, nella sezione precedente, è stato usato il formato YAML.

.. best-practice::

    Per la definizione dei propri servizi usare il formato YAML.

Si sa che questa raccomandazione è molto controversa. È noto che sia il formato YAML sia il formato XML
sono ugualmente utilizzati tra gli sviluppatori, con una leggere preferenza verso YAML.
Entrambi i formati hanno le stesse prestazioni, quindi la scelta di quale utilizzare
è una questione di gusti personali.

Si raccomanda di usare YAML, perché risulta più semplice da gestire dai nuovi
programmatori e perché più conciso. Ovviamente, si può usare il formato che si preferisce.

Servizi: niente parametri di classe
-----------------------------------

Qualcuno potrebbe aver notato che nella definizione del servizio precedente non è stato creato
un parametro di configurazione per definire la classe di servizio:

.. code-block:: yaml

    # app/config/services.yml

    # definzione di servizio con classe come parametro
    parameters:
        slugger.class: AppBundle\Utils\Slugger

    services:
        app.slugger:
            class: "%slugger.class%"

Questa pratica risulta scomoda e assolutamente non necessaria per i propri servizi:

.. best-practice::

    Non definire parametri di configurazione per le classi dei servizi.

Questa abitudine trae la sua origine da un'erronea imitazione dei bundle di terze parti.
Se si sviluppa un bundle da condividere, è possibile allora definire parametri di configurazione
per le classi. Ma se si sviluppa un servizio per la propria applicazione, non c'è bisogno
che le sue classi siano 
configurabili.

Usare uno strato di persistenza
-------------------------------

Symfony è un framework HTTP, che si preoccupa solo di generare una risposta HTTP
per ogni richiesta HTTP. Questo è il motivo per cui Symfony non prevede una
sua modalità per comunicare con uno strato di persistenza (come una base dati o API esterne)
È possibile quindi scegliere la libreria o la strategia preferita.

In pratica, molte applicazioni Symfony si appoggiano al
`progetto Doctrine`_ per definire il loro modello tramite entità e repository.
Così come per la logica di business, si raccomanda di creare le entità di Doctrine in
AppBundle.

Le tre entità definite dall'applicazione blog sono un buon esempio di come rappresentare le classi:

.. code-block:: text

    progetto-symfony/
    ├─ ...
    └─ src/
       └─ AppBundle/
          └─ Entity/
             ├─ Comment.php
             ├─ Post.php
             └─ User.php

.. tip::

    Per gli sviluppatori esperti, si possono creare classi in uno 
    spazio dei nomi in ``src/``.

Informazioni di mappatura di Doctrine
-------------------------------------

Le entità doctrine sono semplici classi PHP le cui informazioni vengono memorizzate in qualche "database".
Le uniche informazioni conosciute da Doctrine su queste entità sono informazioni di
mapping di metadati sul modello.
Doctrine supporta quattro formati per definire queste informazioni: YAML, XML, PHP e annotazioni.

.. best-practice::

    Usare le annotazioni per definire la mappatura delle entità Doctrine.

Le annotazioni sono di gran lunga il modo più conveniente e agile per definire e cercare
le informazioni di mappatura:

.. code-block:: php

    namespace AppBundle\Entity;

    use Doctrine\ORM\Mapping as ORM;
    use Doctrine\Common\Collections\ArrayCollection;

    /**
     * @ORM\Entity
     */
    class Post
    {
        const NUM_ITEMS = 10;

        /**
         * @ORM\Id
         * @ORM\GeneratedValue
         * @ORM\Column(type="integer")
         */
        private $id;

        /**
         * @ORM\Column(type="string")
         */
        private $title;

        /**
         * @ORM\Column(type="string")
         */
        private $slug;

        /**
         * @ORM\Column(type="text")
         */
        private $content;

        /**
         * @ORM\Column(type="string")
         */
        private $authorEmail;

        /**
         * @ORM\Column(type="datetime")
         */
        private $publishedAt;

        /**
         * @ORM\OneToMany(
         *      targetEntity="Comment",
         *      mappedBy="post",
         *      orphanRemoval=true
         * )
         * @ORM\OrderBy({"publishedAt" = "ASC"})
         */
        private $comments;

        public function __construct()
        {
            $this->publishedAt = new \DateTime();
            $this->comments = new ArrayCollection();
        }

        // getter e setter ...
    }

Tutti i formati hanno le stesse prestazioni, quindi la scelta su quale formato
usare dipende, ancora una volta, dai gusti personali.

Fixture dei dati
----------------

Symfony non ha un supporto predefinito per le fixture, è necessario installare
il bundle di gestione delle fixture in Doctrine, eseguendo il seguente comando:

.. code-block:: bash

    $ composer require "doctrine/doctrine-fixtures-bundle"

Quindi è necessario abilitare il bundle in ``AppKernel.php``, ma solo per gli ambienti ``dev` e
``test``:

.. code-block:: php

    use Symfony\Component\HttpKernel\Kernel;

    class AppKernel extends Kernel
    {
        public function registerBundles()
        {
            $bundles = array(
                // ...
            );

            if (in_array($this->getEnvironment(), array('dev', 'test'))) {
                // ...
                $bundles[] = new Doctrine\Bundle\FixturesBundle\DoctrineFixturesBundle();
            }

            return $bundles;
        }

        // ...
    }

Per semplicità, si raccomanda di creare solamente *una* `classe fixture`_, anche
se è consentito averne di più, se questa classe diventa troppo grande.

Ipotizzando di avere almeno una classe fixture e che l'accesso alla base dati sia configurato
correttamente, è possibile caricare il tutto eseguendo il seguente
comando:

.. code-block:: bash

    $ php app/console doctrine:fixtures:load

    Careful, database will be purged. Do you want to continue Y/N ? Y
      > purging database
      > loading AppBundle\DataFixtures\ORM\LoadFixtures

Standard di codice
------------------

Il codice sorgente di Symfony rispetta gli standard `PSR-1`_ e `PSR-2`_,
definiti dalla comunità PHP. Per saperne di più, vedere
:doc:`gli standard di codice di Symfony </contributing/code/standards>`. Inoltre,
usare `PHP-CS-Fixer`_, una utility a riga di comando in grado di
riformattare tutto il codice sorgente dell'applicazione in pochi secondi.

.. _`definizione completa`: http://en.wikipedia.org/wiki/Business_logic
.. _`progetto Doctrine`: http://www.doctrine-project.org/
.. _`classe fixture`: http://symfony.com/doc/current/bundles/DoctrineFixturesBundle/index.html#writing-simple-fixtures
.. _`PSR-1`: http://www.php-fig.org/psr/psr-1/
.. _`PSR-2`: http://www.php-fig.org/psr/psr-2/
.. _`PHP-CS-Fixer`: https://github.com/FriendsOfPHP/PHP-CS-Fixer
