.. index::
   single: Sicurezza; Fornitore di utenti
   single: Sicurezza; Fornitore di entità

Caricare gli utenti dalla base dati (il fornitore di entità)
============================================================

Il livello della sicurezza è uno degli strumenti migliori di Symfony. Gestisce due
aspetti: il processo di autenticazione e quello di autorizzazione. Sebbene possa
sembrare difficile capirne il funzionamento interno, il sistema di sicurezza è
molto flessibile e consente di integrare un'applicazione con qualsiasi
backend di autenticazione, come Active Directory, OAuth o una base dati.

Introduzione
------------

Questo articolo mostra come autenticare gli utenti con una tabella della base dati,
gestita da una classe entità di Doctrine. Il contenuto di questa ricetta è suddiviso
in tre parti. La prima parte riguarda la progettazione di una classe entità ``User``
e il renderla usabile nel livello della sicurezza di Symfony. La seconda parte
descrive come autenticare facilmente un utente con l'oggetto
:class:`Symfony\\Bridge\\Doctrine\\Security\\User\\EntityUserProvider` distribuito
con il framework, oltre che con un po' di configurazione.
Infine, la guida dimostrerà come creare una classe
:class:`Symfony\\Bridge\\Doctrine\\Security\\User\\EntityUserProvider` personalizzata,
per recuperare utenti dalla base dati con condizioni particolari.

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

.. tip::

    Si possono :ref:`generare getter e setter mancanti <book-doctrine-generating-getters-and-setters>`
    eseguendo:

    .. code-block:: bash

        $ php app/console doctrine:generate:entities Acme/UserBundle/Entity/User

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
    class User implements UserInterface, \Serializable
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

        /**
         * @see \Serializable::serialize()
         */
        public function serialize()
        {
            return serialize(array(
                $this->id,
            ));
        }

        /**
         * @see \Serializable::unserialize()
         */
        public function unserialize($serialized)
        {
            list (
                $this->id,
            ) = unserialize($serialized);
        }
    }

.. tip::

    :ref:`Generare la tabella della base dati <book-doctrine-creating-the-database-tables-schema>`
    per l'entità ``User`` eseguendo:

    .. code-block:: bash

        $ php app/console doctrine:schema:update --force

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

.. note::

    L'interfaccia :phpclass:`Serializable` e i suoi metodi ``serialize`` e ``unserialize``
    sono stati aggiunti per consentire alla classe ``User`` di essere serializzata
    nella sessione. Questo potrebbe essere necessario o meno, a seconda della configurazione,
    ma probabilmente è una buona idea. Solo ``id`` ha bisogno di essere serializzato,
    perché il metodo :method:`Symfony\\Bridge\\Doctrine\\Security\\User\\EntityUserProvider::refreshUser`
    ricarica l'utente a ogni richiesta, usando ``id``.

Di seguito è mostrata un'esportazione della tabella ``User`` in MySQL, con utente ``admin`` e
password (codificata) ``admin``. Per dettagli sulla creazione
delle righe degli utenti e sulla codifica delle password, vedere :ref:`book-security-encoding-user-password`.

.. code-block:: bash

    $ mysql> select * from acme_users;
    +----+----------+------+------------------------------------------+--------------------+-----------+
    | id | username | salt | password                                 | email              | is_active |
    +----+----------+------+------------------------------------------+--------------------+-----------+
    |  1 | admin    |      | d033e22ae348aeb5660fc2140aec35850c4da997 | admin@example.com  |         1 |
    +----+----------+------+------------------------------------------+--------------------+-----------+

Nella prossima parte, vedremo come autenticare uno di questi utenti,
grazie al fornitore di entità di Doctrine e a un paio di righe di
configurazione.

Autenticazione con utenti sulla base dati
-----------------------------------------

L'autenticazione di un utente tramite base dati, usando il livello della sicurezza di
Symfony, è un gioco da ragazzi. Sta tutto nella configurazione
:doc:`SecurityBundle</reference/configuration/security>`, memorizzata nel file
``app/config/security.yml``.

Di seguito è mostrato un esempio di configurazione, in cui l'utente inserirà
il suo nome e la sua password, tramite autenticazione HTTP. Queste informazioni
saranno poi verificate sulla nostra entità ``User``, nella base dati:

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

    .. code-block:: xml

        <!-- app/config/security.xml -->
        <config>
            <encoder class="Acme\UserBundle\Entity\User"
                algorithm="sha1"
                encode-as-base64="false"
                iterations="1"
            />

            <role id="ROLE_ADMIN">ROLE_USER</role>
            <role id="ROLE_SUPER_ADMIN">ROLE_USER, ROLE_ADMIN, ROLE_ALLOWED_TO_SWITCH</role>

            <provider name="administrators">
                <entity class="AcmeUserBundle:User" property="username" />
            </provider>

            <firewall name="admin_area" pattern="^/admin">
                <http-basic />
            </firewall>

            <rule path="^/admin" role="ROLE_ADMIN" />
        </config>

    .. code-block:: php

        // app/config/security.php
        $container->loadFromExtension('security', array(
            'encoders' => array(
                'Acme\UserBundle\Entity\User' => array(
                    'algorithm'         => 'sha1',
                    'encode_as_base64'  => false,
                    'iterations'        => 1,
                ),
            ),
            'role_hierarchy' => array(
                'ROLE_ADMIN'       => 'ROLE_USER',
                'ROLE_SUPER_ADMIN' => array('ROLE_USER', 'ROLE_ADMIN', 'ROLE_ALLOWED_TO_SWITCH'),
            ),
            'providers' => array(
                'administrator' => array(
                    'entity' => array(
                        'class'    => 'AcmeUserBundle:User',
                        'property' => 'username',
                    ),
                ),
            ),
            'firewalls' => array(
                'admin_area' => array(
                    'pattern' => '^/admin',
                    'http_basic' => null,
                ),
            ),
            'access_control' => array(
                array('path' => '^/admin', 'role' => 'ROLE_ADMIN'),
            ),
        ));

La sezione ``encoders`` associa il codificatore ``sha1`` alla classe entità.
Ciò vuol dire che Symfony si aspetta che le password siano codificate nella
base dati, tramite tale algoritmo. Per maggiori dettagli su come creare un nuovo
oggetto utente, vedere la sezione
:ref:`book-security-encoding-user-password` del capitolo sulla sicurezza.

La sezione ``providers`` definsice un fornitore di utenti ``administrators``. Un
fornitore di utenti è una "sorgente" da cui gli utenti vengono caricati durante
l'autenticazione. In questo caso, la chiave ``entity`` vuol dire che Symfony userà
il fornitore di entità di Doctrine per caricare gli oggetti ``User`` dalla base dati,
usando il campo univoco ``username``. In altre parole, dice a Symfony come recuperare
gli utenti dalla base dati, prima di verificare la validità della password.

Inibire gli utenti inattivi
---------------------------

Se la proprietà ``isActive`` di User è ``false`` (cioè se ``is_active``
è 0 nella base dati), l'utente potrà ancora eseguire l'accesso al sito.
Per prevenire l'accesso di utenti "inattivi", occorre un po' di lavoro
in più.

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
    namespace Acme\UserBundle\Entity;

    use Doctrine\ORM\Mapping as ORM;
    use Symfony\Component\Security\Core\User\AdvancedUserInterface;

    class User implements AdvancedUserInterface, \Serializable 
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

Se proviamo ora ad autenticare  un untente inattivo, l'accesso sarà
negato.

La prossima parte analizzerà il modo in cui scrivere fornitori di utenti personalizzati,
per autenticare un utente con il suo nome oppure con la sua email.

Autenticazione con un fornitore entità personalizzato
-----------------------------------------------------

Il passo successivo consiste nel consentire a un utente di autenticarsi con il suo nome
o con il suo indirizzo email, che sono entrambi unici nella base dati. Sfortunatamente, il
fornitore di entità nativo è in grado di gestire una sola proprietà per recuperare
l'utente dalla base dati.

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
                ->getQuery();

            try {
                // Il metodo Query::getSingleResult() lancia un'eccezione
                // se nessuna riga corrisponde ai criteri
                $user = $q->getSingleResult();
            } catch (NoResultException $e) {
                $message = sprintf(
                    'Impossibile trovare un oggetto AcmeUserBundle:User identificato da  "%s".',
                    $username
                );
                throw new UsernameNotFoundException($message, 0, $e);
            }

            return $user;
        }

        public function refreshUser(UserInterface $user)
        {
            $class = get_class($user);
            if (!$this->supportsClass($class)) {
                throw new UnsupportedUserException(
                    sprintf(
                        'Istanze di "%s" non supportate.',
                        $class
                    )
                );
            }

            return $this->find($user->getId());
        }

        public function supportsClass($class)
        {
            return $this->getEntityName() === $class
                || is_subclass_of($class, $this->getEntityName());
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

    .. code-block:: xml

        <!-- app/config/security.xml -->
        <config>
            <!-- ... -->

            <provider name="administrator">
                <entity class="AcmeUserBundle:User" />
            </provider>

            <!-- ... -->
        </config>

    .. code-block:: php

        // app/config/security.php
        $container->loadFromExtension('security', array(
            ...,
            'providers' => array(
                'administrator' => array(
                    'entity' => array(
                        'class' => 'AcmeUserBundle:User',
                    ),
                ),
            ),
            ...,
        ));

In questo modo, il livello della sicurezza userà un'istanza di ``UserRepository`` e
richiamerà il suo metodo ``loadUserByUsername()`` per recuperare un utente dalla base dati,
sia che abbia inserito il suo nome utente che abbia inserito la sua email.

Gestire i ruoli nella base dati 
-------------------------------

L'ultima parte della guida spiega come memorizzare e recuperare una lista di ruoli
dalla base dati. Come già accennato, quando l'utente viene caricato, il metodo
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
molti-a-molti con la classe entità ``AcmeUserBundle:Role``. Un utente può essere
in relazione con molti ruoli e un ruolo può essere composto da uno o più
utenti. Il precedente metodo ``getRoles()`` ora restituisce
l'elenco dei ruoli correlati. Notare che i metodi ``__construct()`` e ``getRoles()``
sono cambiati::

    // src/Acme/UserBundle/Entity/User.php
    namespace Acme\UserBundle\Entity;

    use Doctrine\Common\Collections\ArrayCollection;
    // ...

    class User implements AdvancedUserInterface, \Serializable
    {
        // ...
        
        /**
         * @ORM\ManyToMany(targetEntity="Role", inversedBy="users")
         *
         */
        private $roles;

        public function __construct()
        {
            $this->roles = new ArrayCollection();
        }

        public function getRoles()
        {
            return $this->roles->toArray();
        }
        
        // ...

    }

La classe entità ``AcmeUserBundle:Role`` definisce tre campi (``id``,
``name`` e ``role``). Il campo univoco ``role`` contiene i nomi dei ruoli
(p.e. ``ROLE_ADMIN``) usati dal livello della sicurezza di Symfony per proteggere parti
dell'applicazione::

    // src/Acme/Bundle/UserBundle/Entity/Role.php
    namespace Acme\UserBundle\Entity;

    use Symfony\Component\Security\Core\Role\RoleInterface;
    use Doctrine\Common\Collections\ArrayCollection;
    use Doctrine\ORM\Mapping as ORM;

    /**
     * @ORM\Table(name="acme_roles")
     * @ORM\Entity()
     */
    class Role implements RoleInterface
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
         * @ORM\ManyToMany(targetEntity="User", mappedBy="roles")
         */
        private $users;

        public function __construct()
        {
            $this->users = new ArrayCollection();
        }

        /**
         * @see RoleInterface
         */
        public function getRole()
        {
            return $this->role;
        }

        // ... getter e setter per ogni proprietà
    }

Per brevità, i metodi getter e setter non sono mostrati, ma si possono
:ref:`generare <book-doctrine-generating-getters-and-setters>`:

.. code-block:: bash

    $ php app/console doctrine:generate:entities Acme/UserBundle/Entity/User

Non dimenticare di aggiornare anche lo schema della base dati:

.. code-block:: bash

    $ php app/console doctrine:schema:update --force

Saranno create le tabelle ``acme_role`` e ``user_role``, che conterranno
le relazioni molti-a-molti tra ``acme_user`` e ``acme_role``. Se si
ha un utente collegato a un ruolo, la base dati potrebbe essere simile
a questa:

.. code-block:: text

    $ mysql> select * from acme_role;
    +----+-------+------------+
    | id | name  | role       |
    +----+-------+------------+
    |  1 | admin | ROLE_ADMIN |
    +----+-------+------------+

    mysql> select * from user_role;
    +---------+---------+
    | user_id | role_id |
    +---------+---------+
    |       1 |       1 |
    +---------+---------+

Ecco fatto! Quando l'utente accede, il sistema della sicurezza di Symfony richiamerà
il metodo ``User::getRoles``, che restituirà un array di oggetti ``Role``,
usati da Symfony per determinare se l'utente può accedere o meno ad alcune
parti del sistema.

.. sidebar:: Che scopo ha RoleInterface?

    Si noti che la classe ``Role`` implementa
    :class:`Symfony\\Component\\Security\\Core\\Role\\RoleInterface`. Questo
    perché il sistema della sicurezza di Symfony richiede che il metodo ``User::getRoles``
    restituisca un array di stringhe ruoli o di oggetti ruoli che implementino tale interfaccia.
    Se ``Role`` non implementasse tale interfaccia, ``User::getRoles`` avrebbe
    bisogno di iterare su tutti gli oggetti ``Role``, richiamare ``getRole``
    su ciascuno e creare un array di stringhe da restiturie. Entrambi gli approcci sono
    validi ed equivalenti.

.. _cookbook-doctrine-entity-provider-role-db-schema:

Migliorare le prestazioni con una join
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Per migliorare le prestazioni ed evitare il caricamento pigro dei gruppi al momento
del recupero dell'utente dal fornitore di utenti personalizzato, la soluzione migliore è
fare un join dei gruppi correlati nel metodo ``UserRepository::loadUserByUsername()``.
In tal modo, sarà recuperato l'utente e i suoi gruppi/ruoli associati, con una sola query::

    // src/Acme/UserBundle/Entity/UserRepository.php
    namespace Acme\UserBundle\Entity;

    // ...

    class UserRepository extends EntityRepository implements UserProviderInterface
    {
        public function loadUserByUsername($username)
        {
            $q = $this
                ->createQueryBuilder('u')
                ->select('u, r')
                ->leftJoin('u.roles', 'r')
                ->where('u.username = :username OR u.email = :email')
                ->setParameter('username', $username)
                ->setParameter('email', $username)
                ->getQuery();

            // ...
        }

        // ...
    }

Il metodo ``QueryBuilder::leftJoin()`` recupera con un join i ruoli correlati dalla
classe del modello ``AcmeUserBundle:User``, quando un utente viene recuperato con la sua
email o con il suo nome.
