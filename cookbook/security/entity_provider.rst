.. index::
   single: Sicurezza; Fornitore di utenti
   single: Sicurezza; Fornitore di entità

Come caricare gli utenti dal database (il fornitore di entità)
==============================================================

Il livello della sicurezza è uno degli strumenti migliori di Symfony. Gestisce due
aspetti: il processo di autenticazione e quello di autorizzazione. Sebbene possa
sembrare difficile capirne il funzionamento interno, il sistema di sicurezza è
molto flessibile e consente di integrare la propria applicazione con qualsiasi
backend di autenticazione, come Active Directory, OAuth o un database.

Introduzione
------------

Questo articolo mostra come autenticare gli utenti con una tabella di database,
gestita da una classe entità di Doctrine. Il contenuto di questa ricetta è suddiviso
in tre parti. La prima parte riguarda la progettazione di una classe entità ``User``
e il renderla usabile nel livello della sicurezza di Symfony. La seconda parte
descrive come autenticare facilmente un utente con l'oggetto
:class:`Symfony\\Bridge\\Doctrine\\Security\\User\\EntityUserProvider` distribuito
con il framework, oltre che con un po' di configurazione.
Infine, la guida dimostrerà come creare una classe
:class:`Symfony\\Bridge\\Doctrine\\Security\\User\\EntityUserProvider` personalizzata,
per recuperare utenti dal database con condizioni particolari.

Questa guida presume che ci sia un bundle ``Acme\UserBundle`` già pronto
nell'applicazione.

Il modello dei dati
-------------------

Ai fini di questa ricetta, il bundle ``AcmeUserBundle`` contiene una classe
entità ``User``, con i seguenti campi: ``id``, ``username``, ``salt``,
``password``, ``email`` e ``isActive``. Il campo ``isActive`` indica se l'utente
è attivo o meno.

Per sintetizzare, i metodi setter e getter per ogni campo sono stati rimossi, in
modo da focalizzarsi sui metodi più importanti, provenienti da
:class:`Symfony\\Component\\Security\\Core\\User\\UserInterface`.

.. code-block:: php

    // src/Acme/UserBundle/Entity/User.php

    namespace Acme\UserBundle\Entity;

    use Doctrine\ORM\Mapping as ORM;
    use Symfony\Component\Security\Core\User\UserInterface;

    /**
     * Acme\UserBundle\Entity\User
     *
     * @ORM\Table(name="acme_users")
     * @ORM\Entity(repositoryClass="Acme\UserBundle\Entity\UserRepository")
     */
    class User implements UserInterface
    {
        /**
         * @ORM\Column(type="integer")
         * @ORM\Id
         * @ORM\GeneratedValue(strategy="AUTO")
         */
        private $id;

        /**
         * @ORM\Column(type="string", length=25, unique=true)
         */
        private $username;

        /**
         * @ORM\Column(type="string", length=32)
         */
        private $salt;

        /**
         * @ORM\Column(type="string", length=40)
         */
        private $password;

        /**
         * @ORM\Column(type="string", length=60, unique=true)
         */
        private $email;

        /**
         * @ORM\Column(name="is_active", type="boolean")
         */
        private $isActive;

        public function __construct()
        {
            $this->isActive = true;
            $this->salt = md5(uniqid(null, true));
        }

        /**
         * @inheritDoc
         */
        public function getUsername()
        {
            return $this->username;
        }

        /**
         * @inheritDoc
         */
        public function getSalt()
        {
            return $this->salt;
        }

        /**
         * @inheritDoc
         */
        public function getPassword()
        {
            return $this->password;
        }

        /**
         * @inheritDoc
         */
        public function getRoles()
        {
            return array('ROLE_USER');
        }

        /**
         * @inheritDoc
         */
        public function eraseCredentials()
        {
        }
    }

Per poter usare un'istanza della classe ``AcmeUserBundle:User`` nel livello della sicurezza
di Symfony, la classe entità deve implementare
:class:`Symfony\\Component\\Security\\Core\\User\\UserInterface`. Questa
interfaccia costringe la classe a implementare i seguenti cinque metodi:

* ``getRoles()``,
* ``getPassword()``,
* ``getSalt()``,
* ``getUsername()``,
* ``eraseCredentials()``

Per maggiori dettagli su tali metodi, vedere :class:`Symfony\\Component\\Security\\Core\\User\\UserInterface`.

.. versionadded:: 2.1

    In Symfony 2.1, il metodo ``equals`` è stato rimosso da ``UserInterface``.
    Se occorre sovrascrivere l'implementazione predefinita della logica di confronto,
    implementare la nuova interfaccia :class:`Symfony\\Component\\Security\\Core\\User\\EquatableInterface`
    e implementare il metodo ``isEqualTo``.

.. code-block:: php

    // src/Acme/UserBundle/Entity/User.php

    namespace Acme\UserBundle\Entity;

    use Symfony\Component\Security\Core\User\EquatableInterface;

    // ...

    public function isEqualTo(UserInterface $user)
    {
        return $this->username === $user->getUsername();
    }

Di seguito è mostrata un'esportazione della tabella ``User`` in MySQL. Per dettagli sulla
creazione delle righe degli utenti e sulla codifica delle password, vedere :ref:`book-security-encoding-user-password`.

.. code-block:: text

    mysql> select * from user;
    +----+----------+----------------------------------+------------------------------------------+--------------------+-----------+
    | id | username | salt                             | password                                 | email              | is_active |
    +----+----------+----------------------------------+------------------------------------------+--------------------+-----------+
    |  1 | hhamon   | 7308e59b97f6957fb42d66f894793079 | 09610f61637408828a35d7debee5b38a8350eebe | hhamon@example.com |         1 |
    |  2 | jsmith   | ce617a6cca9126bf4036ca0c02e82dee | 8390105917f3a3d533815250ed7c64b4594d7ebf | jsmith@example.com |         1 |
    |  3 | maxime   | cd01749bb995dc658fa56ed45458d807 | 9764731e5f7fb944de5fd8efad4949b995b72a3c | maxime@example.com |         0 |
    |  4 | donald   | 6683c2bfd90c0426088402930cadd0f8 | 5c3bcec385f59edcc04490d1db95fdb8673bf612 | donald@example.com |         1 |
    +----+----------+----------------------------------+------------------------------------------+--------------------+-----------+
    4 rows in set (0.00 sec)

Il database ora contiene quattro utenti, con differenti nomi, email e status. Nella
prossima parte, vedremo come autenticare uno di questi utenti,
grazie al fornitore di entità di Doctrine e a un paio di righe di
configurazione.

Autenticazione con utenti sul database
--------------------------------------

L'autenticazione di un utente tramite database, usando il livello della sicurezza di
Symfony, è un gioco da ragazzi. Sta tutto nella configurazione
:doc:`SecurityBundle</reference/configuration/security>`, memorizzata nel file
``app/config/security.yml``.

Di seguito è mostrato un esempio di configurazione, in cui l'utente inserirà
il suo nome e la sua password, tramite autenticazione HTTP. Queste informazioni
saranno poi verificate sulla nostra entità ``User``, nel database:

.. configuration-block::

    .. code-block:: yaml

        # app/config/security.yml

        security:
            encoders:
                Acme\UserBundle\Entity\User:
                    algorithm:        sha1
                    encode_as_base64: false
                    iterations:       1

            role_hierarchy:
                ROLE_ADMIN:       ROLE_USER
                ROLE_SUPER_ADMIN: [ ROLE_USER, ROLE_ADMIN, ROLE_ALLOWED_TO_SWITCH ]

            providers:
                administrators:
                    entity: { class: AcmeUserBundle:User, property: username }

            firewalls:
                admin_area:
                    pattern:    ^/admin
                    http_basic: ~

            access_control:
                - { path: ^/admin, roles: ROLE_ADMIN }

La sezione ``encoders`` associa il codificatore ``sha1`` alla classe entità.
Ciò vuol dire che Symfony si aspetta che le password siano codificate nel
database, tramite tale algoritmo. Per maggiori dettagli su come creare un nuovo
oggetto utente, vedere la sezione
:ref:`book-security-encoding-user-password` del capitolo sulla sicurezza.

La sezione ``providers`` definsice un fornitore di utenti ``administrators``. Un
fornitore di utenti è una "sorgente" da cui gli utenti vengono caricati durante
l'autenticazione. In questo caso, la chiave ``entity`` vuol dire che Symfony userà
il fornitore di entità di Doctrine per caricare gli oggetti ``User`` dal database,
usando il campo univoco ``username``. In altre parole, dice a Symfony come recuperare
gli utenti dal database, prima di verificare la validità della password.

Questo codice e questa configurazione funzionano, ma non bastano per proteggere
l'applicazione per gli utenti **attivi**. Finora, possiamo ancora autenticarci
con ``maxime``. Nella prossima sezione, vedremo come inibire gli utenti non attivi.

Inibire gli utenti inattivi
---------------------------

Il modo più facile per escludere gli utenti inattivi è implementare l'interfaccia
:class:`Symfony\\Component\\Security\\Core\\User\\AdvancedUserInterface`,
che si occupa di verificare lo stato degli utenti.
L'interfaccia :class:`Symfony\\Component\\Security\\Core\\User\\AdvancedUserInterface`
estende :class:`Symfony\\Component\\Security\\Core\\User\\UserInterface`,
quindi occorre solo modificare l'interfaccia nella classe ``AcmeUserBundle:User``,
per poter beneficiare di comportamenti semplici e avanzati di autenticazione.

L'interfaccia :class:`Symfony\\Component\\Security\\Core\\User\\AdvancedUserInterface`
aggiunge altri quattro metodi, per validare lo stato degli utenti:

* ``isAccountNonExpired()`` verifica se l'utente è scaduto,
* ``isAccountNonLocked()`` verifica se l'utente è bloccato,
* ``isCredentialsNonExpired()`` verifica se la password dell'utente è
  scaduta,
* ``isEnabled()`` verifica se l'utente è abilitato

Per questo esempio, i primi tre metodi restituiranno ``true``, mentre il metodo
``isEnabled()`` restituire il valore booleano del campo  ``isActive``.

.. code-block:: php

    // src/Acme/UserBundle/Entity/User.php

    namespace Acme\Bundle\UserBundle\Entity;

    // ...
    use Symfony\Component\Security\Core\User\AdvancedUserInterface;

    // ...
    class User implements AdvancedUserInterface
    {
        // ...
        public function isAccountNonExpired()
        {
            return true;
        }

        public function isAccountNonLocked()
        {
            return true;
        }

        public function isCredentialsNonExpired()
        {
            return true;
        }

        public function isEnabled()
        {
            return $this->isActive;
        }
    }

Se proviamo ora ad autenticare ``maxime``, l'accesso sarà negato, perché questo
utente non è stato abilitato. La prossima parte analizzerà il modo
in cui scrivere fornitori di utenti personalizzati, per autenticare un utente
con il suo nome oppure con la sua email.

Autenticazione con un fornitore entità personalizzato
-----------------------------------------------------

Il passo successivo consisten nel consentire a un utente di autenticarsi con il suo nome
o con il suo indirizzo email, che sono entrambi unici nel database. Sfortunatamente, il
fornitore di entità nativo è in grado di gestire una sola proprietà per recuperare
l'utente dal database.

Per poterlo fare, creare un fornitore di entità personalizzato, che cerchi un utente il
cui nome *o* la cui email corrisponda al nome utente inserito. La buona notizia
è che un oggetto repository di Doctrine può agire da fornitore di entità, se 
implementa l'interfaccia
:class:`Symfony\\Component\\Security\\Core\\User\\UserProviderInterface`. Questa
interfaccia ha tre metodi da implementare: ``loadUserByUsername($username)``,
``refreshUser(UserInterface $user)`` e ``supportsClass($class)``. Per maggiori
dettagli, si veda :class:`Symfony\\Component\\Security\\Core\\User\\UserProviderInterface`.

Il codice successivo mostra l'implementazione di
:class:`Symfony\\Component\\Security\\Core\\User\\UserProviderInterface` nella classe
``UserRepository``::

    // src/Acme/UserBundle/Entity/UserRepository.php

    namespace Acme\UserBundle\Entity;

    use Symfony\Component\Security\Core\User\UserInterface;
    use Symfony\Component\Security\Core\User\UserProviderInterface;
    use Symfony\Component\Security\Core\Exception\UsernameNotFoundException;
    use Symfony\Component\Security\Core\Exception\UnsupportedUserException;
    use Doctrine\ORM\EntityRepository;
    use Doctrine\ORM\NoResultException;

    class UserRepository extends EntityRepository implements UserProviderInterface
    {
        public function loadUserByUsername($username)
        {
            $q = $this
                ->createQueryBuilder('u')
                ->where('u.username = :username OR u.email = :email')
                ->setParameter('username', $username)
                ->setParameter('email', $username)
                ->getQuery()
            ;

            try {
                // The Query::getSingleResult() method throws an exception
                // if there is no record matching the criteria.
                $user = $q->getSingleResult();
            } catch (NoResultException $e) {
                throw new UsernameNotFoundException(sprintf('Impossibile trovare un oggetto AcmeUserBundle:User identificato da "%s".', $username), null, 0, $e);
            }

            return $user;
        }

        public function refreshUser(UserInterface $user)
        {
            $class = get_class($user);
            if (!$this->supportsClass($class)) {
                throw new UnsupportedUserException(sprintf('Istanze di "%s" non supportate.', $class));
            }

            return $this->loadUserByUsername($user->getUsername());
        }

        public function supportsClass($class)
        {
            return $this->getEntityName() === $class || is_subclass_of($class, $this->getEntityName());
        }
    }

Per concludere l'implementazione, occorre modificare la configurazione del livello della
sicurezza, per dire a Symfony di usare il nuovo fornitore di entità personalizzato, al
posto del fornitore di entità generico di Doctrine. Lo si può fare facilmente, rimuovendo
il campo ``property`` nella sezione ``security.providers.administrators.entity``
del file ``security.yml``.

.. configuration-block::

    .. code-block:: yaml

        # app/config/security.yml
        security:
            # ...
            providers:
                administrators:
                    entity: { class: AcmeUserBundle:User }
            # ...

In questo modo, il livello della sicurezza userà un'istanza di ``UserRepository`` e
richiamerà il suo metodo ``loadUserByUsername()`` per recuperare un utente dal database,
sia che abbia inserito il suo nome utente che abbia inserito la sua email.

Gestire i ruoli nel database
----------------------------

L'ultima parte della guida spiega come memorizzare e recuperare una lista di ruoli
dal database. Come già accennato, quando l'utente viene caricato, il metodo
``getRoles()`` restituisce un array di ruoli di sicurezza, che gli andrebbero assegnati.
Si possono caricare tali dati da qualsiasi posto, una lista predefinita usata per
ogni utente (p.e. ``array('ROLE_USER')``), un array di Doctrine chiamato
``roles``, oppure tramite una relazione di Doctrine, come vedremo in
questa sezione.

.. caution::

    In una configurazione tipica, si dovrebbe sempre restituire almeno un ruolo nel
    metodo``getRoles()``. Per convenzione, solitamente si restituisce un ruolo chiamato
    ``ROLE_USER``. Se non si restituisce alcun ruolo, l'utente potrebbe apparire come
    non autenticato.

In questo esempio, la classe entità ``AcmeUserBundle:User`` definisce una relazione
molti-a-molti con la classe entità ``AcmeUserBundle:Group``. Un utente può essere in
relazione con molti gruppi e un gruppo può essere composto da uno o più utenti.
Poiché un gruppo è anche un ruolo, il precedente metodo ``getRoles()`` ora restituisce
l'elenco dei gruppi correlati::

    // src/Acme/UserBundle/Entity/User.php

    namespace Acme\Bundle\UserBundle\Entity;

    use Doctrine\Common\Collections\ArrayCollection;

    // ...
    class User implements AdvancedUserInterface
    {
        /**
         * @ORM\ManyToMany(targetEntity="Group", inversedBy="users")
         *
         */
        private $groups;

        public function __construct()
        {
            $this->groups = new ArrayCollection();
        }

        // ...

        public function getRoles()
        {
            return $this->groups->toArray();
        }
    }

La classe entità ``AcmeUserBundle:Group`` definisce tre campi di tabella (``id``,
``name`` e ``role``). Il campo univoco ``role`` contiene i nomi dei ruoli usati dal livello
della sicurezza di Symfony per proteggere parti dell'applicazione. La cosa più
importante da notare è che la classe entità ``AcmeUserBundle:Group`` implementa
:class:`Symfony\\Component\\Security\\Core\\Role\\RoleInterface`, che la obbliga ad avere
un metodo ``getRole()``::

    namespace Acme\Bundle\UserBundle\Entity;

    use Symfony\Component\Security\Core\Role\RoleInterface;
    use Doctrine\Common\Collections\ArrayCollection;
    use Doctrine\ORM\Mapping as ORM;

    /**
     * @ORM\Table(name="acme_groups")
     * @ORM\Entity()
     */
    class Group implements RoleInterface
    {
        /**
         * @ORM\Column(name="id", type="integer")
         * @ORM\Id()
         * @ORM\GeneratedValue(strategy="AUTO")
         */
        private $id;

        /**
         * @ORM\Column(name="name", type="string", length=30)
         */
        private $name;

        /**
         * @ORM\Column(name="role", type="string", length=20, unique=true)
         */
        private $role;

        /**
         * @ORM\ManyToMany(targetEntity="User", mappedBy="groups")
         */
        private $users;

        public function __construct()
        {
            $this->users = new ArrayCollection();
        }

        // ... getter e setter per ogni proprietà

        /**
         * @see RoleInterface
         */
        public function getRole()
        {
            return $this->role;
        }
    }

Per migliorare le prestazioni ed evitare il caricamento pigro dei gruppi al momento
del recupero dell'utente dal fornitore di utenti personalizzato, la soluzione migliore è
fare un join dei gruppi correlati nel metodo ``UserRepository::loadUserByUsername()``.
In tal modo, sarà recuperato l'utente e i suoi gruppi/ruoli associati, con una sola query::

    // src/Acme/UserBundle/Entity/UserRepository.php

    namespace Acme\Bundle\UserBundle\Entity;

    // ...

    class UserRepository extends EntityRepository implements UserProviderInterface
    {
        public function loadUserByUsername($username)
        {
            $q = $this
                ->createQueryBuilder('u')
                ->select('u, g')
                ->leftJoin('u.groups', 'g')
                ->where('u.username = :username OR u.email = :email')
                ->setParameter('username', $username)
                ->setParameter('email', $username)
                ->getQuery()
            ;

            // ...
        }

        // ...
    }

Il metodo ``QueryBuilder::leftJoin()`` recupera con un join i gruppi correlati dalla
classe del modello ``AcmeUserBundle:User``, quando un utente viene recuperato con la sua
email o con il suo nome.
