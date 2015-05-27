.. index::
   single: Sicurezza; Access Control List (ACL)

Access Control List (ACL)
=========================

In applicazioni complesse, ci si trova spesso con il problema di decisioni di accesso
che non possono essere basate solo sulla persona che lo richiede (il cosiddetto ``Token``),
ma che coinvolgono anche un oggetto del dominio per cui l'accesso viene richiesto. Qui
entrano in gioco le ACL.

.. sidebar:: Alternative ad ACL

    L'uso di ACL non è banale e, per casi d'uso semplice, potrebbe essere eccessivo.
    Se la logica dei permessi può essere descritta solo scrivendo un po' di codice (p.e.
    per verificare se l'utente corrente è proprietario di un Blog), si prenda in considerazione l'uso dei
    :doc:`votanti </cookbook/security/voters>`. A un votante viene passato l'oggetto
    su cui si vota, quindi su può usare per prendere decisioni complesse e in effetti
    implementare un proprio ACL. Usando l'autorizzazione (p.e. la parte ``isGranted``),
    sembrerà simile a quello che si vedrà in questa ricetta, ma la classe votante
    gestirà la logica dietro le quinte, al posto del sistema ACL.

Si immagini di progettare un sistema di blog, in cui gli utenti possano commentare i post.
Ora, si vuole che un utente sia in grado di modificare i propri commenti, ma non quelli
degli altri utenti; inoltre, si vuole che l'amministratore possa modificare tutti i commenti.
In questo scenario, ``Comment`` sarebbe l'oggetto del dominio a cui si vuole restringere
l'accesso. Si possono usare diversi approcci per ottenere questo scopo in
Symfony, due approcci di base (non esaustivi) sono:

- *Gestire la sicurezza nei metodi*: Di base, questo vuol dire mantenere un riferimento
  in ogni oggetto ``Comment`` a tutti gli utenti che vi hanno accesso, e quindi confrontare
  tali utenti con quello del ``Token``.
- *Gestire la sicurezza con i ruoli*: In questo approccio, si aggiunge un ruolo per ogni
  oggetto ``Comment``, p.e. ``ROLE_COMMENT_1``, ``ROLE_COMMENT_2``, ecc.

Entrambi gli approcci sono perfettamente validi. Tuttavia, accoppiano la logica di
autorizzazione al codice, il che li rende meno riutilizzabili altrove, e inoltre
incrementano le difficoltà nei test unitari. D'altra parte, si potrebbero avere problemi
di prestazioni, se molti utenti avessero accesso a un singolo oggetto del dominio.

Per fortuna, c'è un modo migliore, di cui ora parleremo.

Preparazione
------------

Prima di entrare in azione, ci occorre una breve preparazione.
Innanzitutto, occorre configurare la connessione al sistema ACL da usare:

.. configuration-block::

    .. code-block:: yaml

        # app/config/security.yml
        security:
            acl:
                connection: default

    .. code-block:: xml

        <!-- app/config/security.xml -->
        <acl>
            <connection>default</connection>
        </acl>

    .. code-block:: php

        // app/config/security.php
        $container->loadFromExtension('security', 'acl', array(
            'connection' => 'default',
        ));

.. note::

    Il sistema ACL richiede almeno una connessione di Doctrine configurata o nel DBAL (usabile
    senza interventi) o con MongoDB (usabile con `MongoDBAclBundle`_). Tuttavia, questo non
    significa che si debba usare Doctrine per mappare i propri oggetti del dominio. Si può usare
    qualsiasi mapper si desideri per i propri oggetti, sia esso l'ORM Doctrine, l'ODM Mongo, Propel o anche
    SQL puro, la scelta è lasciata allo sviluppatore.

Dopo aver configurato la connessione, occorre importare la struttura della base dati.
Fortunatamente, c'è un task per farlo. Basta eseguire il comando seguente:

.. code-block:: bash

    $ php app/console init:acl

Iniziare
--------

Tornando al piccolo esempio iniziale, si può ora implementare una
ACL.

Una volta creata l'ACL, si può consentire l'accesso a oggetti creando una
Access Control Entity (ACE), per solidificare la relazione tra entità
e utente.

Creare una ACL e aggiungere un ACE
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

.. code-block:: php

    // src/Acme/DemoBundle/Controller/BlogController.php
    namespace Acme\DemoBundle\Controller;

    use Symfony\Bundle\FrameworkBundle\Controller\Controller;
    use Symfony\Component\Security\Core\Exception\AccessDeniedException;
    use Symfony\Component\Security\Acl\Domain\ObjectIdentity;
    use Symfony\Component\Security\Acl\Domain\UserSecurityIdentity;
    use Symfony\Component\Security\Acl\Permission\MaskBuilder;

    class BlogController
    {
        // ...

        public function addCommentAction(Post $post)
        {
            $comment = new Comment();

            // ... preparazione di $form e collegamento dei dati

            if ($form->isValid()) {
                $entityManager = $this->get('doctrine.orm.default_entity_manager');
                $entityManager->persist($comment);
                $entityManager->flush();

                // creazione dell'ACL
                $aclProvider = $this->get('security.acl.provider');
                $objectIdentity = ObjectIdentity::fromDomainObject($comment);
                $acl = $aclProvider->createAcl($objectIdentity);

                // recupero dell'identità di sicurezza dell'utente attuale
                $securityContext = $this->get('security.context');
                $user = $securityContext->getToken()->getUser();
                $securityIdentity = UserSecurityIdentity::fromAccount($user);

                // l'utente può accedere
                $acl->insertObjectAce($securityIdentity, MaskBuilder::MASK_OWNER);
                $aclProvider->updateAcl($acl);
            }
        }
    }

In questo pezzo di codice ci sono alcune importanti decisioni implementative.
Per ora, ne mettiamo in evidenza solo due:

Prima di tutto, il metodo ``->createAcl()`` non accetta direttamente oggetti del
dominio, ma solo implementazioni di ``ObjectIdentityInterface``.
Questo passo aggiuntivo consente di lavorare con le ACL, anche se non si hanno veri
oggetti del dominio a portata di mano. Questo può essere molto utile quando si vogliono
verificare i permessi di un gran numero di oggetti, senza dover idratare gli oggetti
stessi.

L'altra parte interessante è la chiamata a ``->insertObjectAce()``. Nel nostro esempio,
stiamo consentendo l'accesso come proprietario del commento all'utente corrente.
La costante ``MaskBuilder::MASK_OWNER`` è un intero predefinito; non ci si deve
preoccupare, perché il costruttore di maschere astrae la maggior parte dei dettagli tecnici,
ma usando questa tecnica si possono memorizzare molti permessi diversi in una singola riga
di base dati, che fornisce un considerevole vantaggio in termini di prestazioni.

.. tip::

    L'ordine in cui gli ACE sono verificati è significativo. Come regola generale, si
    dovrebbero mettere le voci più specifiche all'inizio.

Verifica dell'accesso
~~~~~~~~~~~~~~~~~~~~~

.. code-block:: php

    // src/Acme/DemoBundle/Controller/BlogController.php

    // ...

    class BlogController
    {
        // ...

        public function editCommentAction(Comment $comment)
        {
            $securityContext = $this->get('security.context');

            // verifica per l'accesso in modifica
            if (false === $securityContext->isGranted('EDIT', $comment)) {
                throw new AccessDeniedException();
            }

            // ... recuperare l'oggetto commento e fare le modifiche
        }
    }

In questo esempio, verifichiamo se l'utente abbia il permesso ``EDIT``.
Internamente, Symfony mappa i permessi a diversi interi e verifica se l'utente possieda
uno di essi.

.. note::

    Si possono definire fino a 32 permessi base (a seconda del sistema operativo,
    può variare tra 30 e 32). Inoltre, si possono anche definire dei permessi
    cumulativi.

Permessi cumulativi
-------------------

Nel nostro primo esempio, abbiamo assegnato al'utente solo il permesso di base ``OWNER``.
Sebbene questo consenta effettivamente all'utente di eseguire qualsiasi operazione
sull'oggetto del dominio, come vedere, modificare, ecc., ci sono dei casi in cui si  vuole
assegnare tali permessi in modo esplicito.

``MaskBuilder`` può essere usato per creare facilmente delle maschere, combinando diversi
permessi di base:

.. code-block:: php

    $builder = new MaskBuilder();
    $builder
        ->add('view')
        ->add('edit')
        ->add('delete')
        ->add('undelete')
    ;
    $mask = $builder->get(); // int(29)

Questa maschera può quindi essere usata per assegnare all'utente i permessi di base
aggiunti in precedenza:

.. code-block:: php

    $identity = new UserSecurityIdentity('johannes', 'Acme\UserBundle\Entity\User');
    $acl->insertObjectAce($identity, $mask);

Ora l'utente ha il permesso di vedere, modificare, cancellare e ripristinare gli oggetti.

.. _`MongoDBAclBundle`: https://github.com/IamPersistent/MongoDBAclBundle
