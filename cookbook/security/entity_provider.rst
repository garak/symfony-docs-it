.. index::
   single: Sicurezza; Fornitore di utenti
   single: Sicurezza; Fornitore di entità

Caricare gli utenti dalla base dati (il fornitore di entità)
============================================================

Il sistema di sicurezza di Symfony può caricare utente da qualsiasi fonte, come
basi di dati, Active Directory o server OAuth. Questa ricetta mostrerà
come caricare utenti dalla base dati, tramite un'entità Doctrine.

Introduzione
------------

.. tip::

    Prima di iniziare, dare un'occhiata a `FOSUserBundle`_. Questo
    bundle consente di caricare utenti dalla base dati (come si vedrà più avanti)
    *e* fornisce rotte e controllori per questioni come login,
    registrazione e recupero della password. Se invece occorre personalizzare
    il proprio sistema di utenti *oppure* si vuole capire come funzionano le cose, questa
    ricetta è anche meglio.

Il caricamento di utenti tramite entità Doctrine ha essenzialmente due passi:

#. :ref:`Creare un'entità Utente <security-crete-user-entity>`
#. :ref:`Configurare security.yml per caricare dall'entità <security-config-entity-provider>`

Successivamente, si può approfondire su :ref:`bloccare utenti inattivi <security-advanced-user-interface>`,
:ref:`usare query personalizzate <authenticating-someone-with-a-custom-entity-provider>`
e :ref:`serializzare l'utente in sessione <cookbook-security-serialize-equatable>`

.. _security-crete-user-entity:
.. _the-data-model:

Il modello dei dati
-------------------

1) Creare l'entità Utente
-------------------------

Per questa ricetta, si suppone di avere già un'entità ``Utente`` in un bundle
``AppBundle``, con i campi seguenti: ``id``, ``username``, ``password``,
``email`` e ``isActive``:

.. code-block:: php

    // src/AppBundle/Entity/Utente.php
    namespace AppBundle\Entity;

    use Doctrine\ORM\Mapping as ORM;
    use Symfony\Component\Security\Core\User\UserInterface;

    /**
     * @ORM\Table()
     * @ORM\Entity(repositoryClass="AppBundle\Entity\UserRepository")
     */
    class Utente implements UserInterface, \Serializable
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
         * @ORM\Column(type="string", length=64)
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
            // potrebbe non essere necessario, vedere la sezione sul sale più avanti
            // $this->salt = md5(uniqid(null, true));
        }

        public function getUsername()
        {
            return $this->username;
        }

        public function getSalt()
        {
            // *potrebbe* non essere necessario un vero sale, a seconda del codificatore
            // vedere la sezione sul sale più avanti
            return null;
        }

        public function getPassword()
        {
            return $this->password;
        }

        public function getRoles()
        {
            return array('ROLE_USER');
        }

        public function eraseCredentials()
        {
        }

        /** @see \Serializable::serialize() */
        public function serialize()
        {
            return serialize(array(
                $this->id,
                $this->username,
                $this->password,
                // vedere la sezione sul sale più avanti
                // $this->salt,
            ));
        }

        /** @see \Serializable::unserialize() */
        public function unserialize($serialized)
        {
            list (
                $this->id,
                $this->username,
                $this->password,
                // vedere la sezione sul sale più avanti
                // $this->salt
            ) = unserialize($serialized);
        }
    }

Per sintetizzare, alcuni getter e setter non sono mostrati in questo esempio.
Possono comunque essere :ref:`generati <book-doctrine-generating-getters-and-setters>` con
il comando:

.. code-block:: bash

    $ php app/console doctrine:generate:entities AppBundle/Entity/User

Occorre quindi :ref:`creare la tabella nella base dati <book-doctrine-creating-the-database-tables-schema>`:

.. code-block:: bash

    $ php app/console doctrine:schema:update --force

Che cos'è UserInterface?
~~~~~~~~~~~~~~~~~~~~~~~~

Finora, questa è solo una normale entità. Ma per poter usare questa classe nel sistema di
sicurezza, deve implementare
:class:`Symfony\\Component\\Security\\Core\\User\\UserInterface`. Questo obbliga
la classe ad avere questi cinque metodi:

* :method:`Symfony\\Component\\Security\\Core\\User\\UserInterface::getRoles`
* :method:`Symfony\\Component\\Security\\Core\\User\\UserInterface::getPassword`
* :method:`Symfony\\Component\\Security\\Core\\User\\UserInterface::getSalt`
* :method:`Symfony\\Component\\Security\\Core\\User\\UserInterface::getUsername`
* :method:`Symfony\\Component\\Security\\Core\\User\\UserInterface::eraseCredentials`

Per maggiori dettagli su tali metodi, vedere :class:`Symfony\\Component\\Security\\Core\\User\\UserInterface`.

Cosa fanno i metodi serialize e unserialize?
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Alla fine di ogni richiesta, l'oggetto Utente è serializzato in sessione.
Alla richiesta successiva, è deserializzato. Per aiutare PHP in questo processo, si
deve implementare ``Serializable``. Ma non occorre serializzare tutto:
bastano solo pochi campi (quelli mostrati sopra, più alcuni altri se si decide
di implementare :ref:`AdvancedUserInterface <security-advanced-user-interface>`).
A ogni richiesta, si usa ``id`` per cercare di nuovo l'oggetto ``Utente``
nella base dati.

Per approfondire, vedere :ref:`cookbook-security-serialize-equatable`.

.. _authenticating-someone-against-a-database:
.. _security-config-entity-provider:

2) Configurazione di sicurezza per caricare dall'entità
-------------------------------------------------------

Ora che si ha un'entità ``Utente``, che implementa ``UserInterface``, basta
dirlo al sistema di sicurezza di Symfony, all'interno di ``security.yml``.

In questo esempio, l'utente inserirà username e password tramite HTTP
basic authentication. Symfony cercherà un'entità ``Utente`` corrsipondente
allo username e ne verificherà la password:

.. configuration-block::

    .. code-block:: yaml

        # app/config/security.yml
        security:
            encoders:
                AppBundle\Entity\Utente:
                    algorithm: bcrypt

            # ...

            providers:
                fornitore:
                    entity:
                        class: AppBundle:Utente
                        property: username
                        # se si usano più gestori di entità
                        # manager_name: personalizzato

            firewalls:
                default:
                    pattern:    ^/
                    http_basic: ~
                    provider: fornitore

            # ...

    .. code-block:: xml

        <!-- app/config/security.xml -->
        <config>
            <encoder class="AppBundle\Entity\Utente"
                algorithm="bcrypt"
            />

            <!-- ... -->

            <provider name="fornitore">
                <entity class="AppBundle:Utente" property="username" />
            </provider>

            <firewall name="default" pattern="^/" provider="fornitore">
                <http-basic />
            </firewall>
            
            <!-- ... -->
        </config>

    .. code-block:: php

        // app/config/security.php
        $container->loadFromExtension('security', array(
            'encoders' => array(
                'AppBundle\Entity\Utente' => array(
                    'algorithm' => 'bcrypt',
                ),
            ),
            // ...
            'providers' => array(
                'fornitore' => array(
                    'entity' => array(
                        'class'    => 'AppBundle:Utente',
                        'property' => 'username',
                    ),
                ),
            ),
            'firewalls' => array(
                'default' => array(
                    'pattern' => '^/',
                    'http_basic' => null,
                    'provider' => 'fornitore',
                ),
            ),
            // ...
        ));

La sezione ``encoders`` dice a Symfony che le password nella
base dati saranno codificate con ``bcrypt``. La sezione ``providers``
crea un "fornitore di utenti" chiamato ``fornitore``, che sa che deve
cercare nell'entità ``AppBundle:Utente`` la proprietà ``username``. Il
nome ``fornitore`` non è importante: basta che corrisponda al valore della
voce ``provider`` del firewall. Se invece non si imposta la voce ``provider``
nel firewall, verrà usato il primo "fornitore di utenti".

.. include:: /cookbook/security/_ircmaxwell_password-compat.rst.inc

La sezione ``providers`` definsice un fornitore di utenti ``administrators``. Un
fornitore di utenti è una "sorgente" da cui gli utenti vengono caricati durante
l'autenticazione. In questo caso, la chiave ``entity`` vuol dire che Symfony userà
il fornitore di entità di Doctrine per caricare gli oggetti ``User`` dalla base dati,
usando il campo univoco ``username``. In altre parole, dice a Symfony come recuperare
gli utenti dalla base dati, prima di verificare la validità della password.

.. note::

    Per impostazione predefinita, il fornitore di entità usa il gestore di entità predefinito
    per recuperare dalla base dati le informazoni sugli utenti. Se si usano
    :doc:`gestori di entità multipli </cookbook/doctrine/multiple_entity_managers>`,
    si può specificare quale gestore usare, con l'opzione ``manager_name``:

    .. configuration-block::

        .. code-block:: yaml

            # app/config/config.yml
            security:
                # ...

                providers:
                    administrators:
                        entity:
                            class: AppBundle:User
                            property: username
                            manager_name: customer

                # ...

        .. code-block:: xml

            <!-- app/config/config.xml -->
            <?xml version="1.0" encoding="UTF-8"?>
            <srv:container xmlns="http://symfony.com/schema/dic/security"
                xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
                xmlns:srv="http://symfony.com/schema/dic/services"
                xsi:schemaLocation="http://symfony.com/schema/dic/services
                    http://symfony.com/schema/dic/services/services-1.0.xsd">
                <config>
                    <!-- ... -->

                    <provider name="administrators">
                        <entity class="AcmeUserBundle:User"
                            property="username"
                            manager-name="customer" />
                    </provider>

                    <!-- ... -->
                </config>
            </srv:container>

        .. code-block:: php

            // app/config/config.php
            $container->loadFromExtension('security', array(
                // ...
                'providers' => array(
                    'administrator' => array(
                        'entity' => array(
                            'class' => 'AcmeUserBundle:User',
                            'property' => 'username',
                            'manager_name' => 'customer',
                        ),
                    ),
                ),
                // ...
            ));

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

* :method:`Symfony\\Component\\Security\\Core\\User\\AdvancedUserInterface::isAccountNonExpired`
   verifica se l'utente è scaduto,
* :method:`Symfony\\Component\\Security\\Core\\User\\AdvancedUserInterface::isAccountNonLocked`
  verifica se l'utente è bloccato,
* :method:`Symfony\\Component\\Security\\Core\\User\\AdvancedUserInterface::isCredentialsNonExpired`
  verifica se le credenziali (la password) dell'utente siano scadute,
* :method:`Symfony\\Component\\Security\\Core\\User\\AdvancedUserInterface::isEnabled`
  verifica se l'utente è abilitato.

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

.. note::

    Quando si usa ``AdvancedUserInterface``, si deve aggiungere anche una delle
    proprietà usate da tali metodi (come ``isActive()``) al metodo ``serialize()``.
    Se *non* lo si fa, l'oggetto utente potrebbe non essere deserializzato correttamente
    dalla sessione a ogni richiesta.

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

    class UserRepository extends EntityRepository implements UserProviderInterface
    {
        public function loadUserByUsername($username)
        {
            $user = $this->createQueryBuilder('u')
                ->where('u.username = :username OR u.email = :email')
                ->setParameter('username', $username)
                ->setParameter('email', $username)
                ->getQuery()
                ->getOneOrNullResult()

            if ($user) {
                $message = sprintf(
                    'Impossibile trovare un oggetto AcmeUserBundle:User identificato da  "%s".',
                    $username
                );
                throw new UsernameNotFoundException($message);
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

.. tip::

    Non dimenticare di aggiungere la classe repository alla
    :ref:`definizione di mappatura dell'entità <book-doctrine-custom-repository-classes>`.

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
                    entity: { class: AppBundle:User }
            # ...

    .. code-block:: xml

        <!-- app/config/security.xml -->
        <config>
            <!-- ... -->

            <provider name="administrator">
                <entity class="AppBundle:User" />
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
                        'class' => 'AppBundle:User',
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
    metodo ``getRoles()``. Per convenzione, solitamente si restituisce un ruolo chiamato
    ``ROLE_USER``. Se non si restituisce alcun ruolo, l'utente potrebbe apparire come
    non autenticato.

.. caution::

    Per funzionare con gli esempi della configurazione di sicurezza di questa ricetta,
    tutti i ruoli devono avere il prefisso ``ROLE_`` (vedere
    la :ref:`sezione sui ruoli <book-security-roles>` nel libro). Per
    example, your roles will be ``ROLE_ADMIN`` or ``ROLE_USER`` instead of
    ``ADMIN`` or ``USER``.

In questo esempio, la classe entità ``AcmeUserBundle:User`` definisce una relazione
molti-a-molti con la classe entità ``AcmeUserBundle:Role``. Un utente può essere
in relazione con molti ruoli e un ruolo può essere composto da uno o più
utenti. Il precedente metodo ``getRoles()`` ora restituisce
l'elenco dei ruoli correlati. Notare che i metodi ``__construct()`` e ``getRoles()``
sono cambiati::

    // src/AppBundle/Entity/User.php
    namespace AppBundle\Entity;

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

    // src/AppBundle/Entity/Role.php
    namespace AppBundle\Entity;

    use Symfony\Component\Security\Core\Role\RoleInterface;
    use Doctrine\Common\Collections\ArrayCollection;
    use Doctrine\ORM\Mapping as ORM;

    /**
     * @ORM\Table(name="app_role")
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

.. code-block:: bash

    $ mysql> SELECT * FROM app_role;
    +----+-------+------------+
    | id | name  | role       |
    +----+-------+------------+
    |  1 | admin | ROLE_ADMIN |
    +----+-------+------------+

    $ mysql> SELECT * FROM user_role;
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

    // src/AppBundle/Entity/UserRepository.php
    namespace AppBundle\Entity;

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
classe del modello ``AppBundle:User``, quando un utente viene recuperato con la sua
email o con il suo nome.

.. _`cookbook-security-serialize-equatable`:

Capire la serializzazione e come un utente è salvato in sessione
----------------------------------------------------------------

Questa sezione è per chi fosse curioso riguardo all'importanza del metodo ``serialize()``
della classe ``User`` o su come l'oggetto utente sia serializzato e
deserializzato.

Una volta che l'utente ha eseguito l'accesso l'intero oggetto ``User`` è serializzato
in sessione. Alla richiesta successiva, l'oggetto ``User`` è deserializzato. Quindi,
viene usato il valore della proprietà ``id`` per cercare nuovamente l'oggetto ``User``
nella base dati. Infine, il nuovo oggetto ``User`` viene confrontato in qualche modo
all'oggetto deserializzato, per assicurarsi che rappresenti lo stesso utente. Per esempio, se
per qualche motivo la proprietà ``username`` non corrisponde, l'utente
sarà buttato fuori, per questioni di sicurezza.

Anche se tutto ciò avviene in modo automatico, ci sono alcuni importanti effetti collaterali.

Primo, l'interfaccia :phpclass:`Serializable` e i suoi metodi ``serialize`` e ``unserialize``
sono stati aggiunti, per consentire alla classe ``User`` di essere serializzata
in sessione. Questo potrebbe essere necessario o meno, a seconda della propria configurazione,
ma è probabilmente una buona idea. In teoria, basterebbe serializzare solo ``id``,
perché il metodo :method:`Symfony\\Bridge\\Doctrine\\Security\\User\\EntityUserProvider::refreshUser`
aggiorna l'utente a ciascuna richiesta, usando ``id`` (come spiegato
sopra). In pratica, tuttavia, questo vuol dire che l'oggetto ``User`` viene ricaricato
dalla base dati a ogni richiesta, usando ``id`` dall'oggetto serializzato.
Questo assicura che tutti i dati dell'utente siano aggiornati.

Symfony usa anche ``username``, ``salt`` e ``password`` per verificare che
l'utente non sia cambiato tra una richiesta e l'altra. Non serializzare queste informazioni
potrebbe causare il logout dell'utente. Se ``User`` implementa
:class:`Symfony\\Component\\Security\\Core\\User\\EquatableInterface`,
invece di confrontare queste proprietà, viene semplicemente richiamato il metodo ``isEqualTo``
e si possono verificare tutte le proprietà desiderate. Se questo aspetto non è chiaro,
probabilmente è meglio non implementare questa interfaccia.


.. versionadded:: 2.1
    In Symfony 2.1, è stato rimosso il metodo ``equals`` da ``UserInterface``
    ed è stata introdotta l'interfaccia ``EquatableInterface`` al suo posto.

.. _FOSUserBundle: https://github.com/FriendsOfSymfony/FOSUserBundle
