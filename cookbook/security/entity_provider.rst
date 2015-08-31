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
basic authentication. Symfony cercherà un'entità ``Utente`` corrispondente
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

Creare il primo utente
~~~~~~~~~~~~~~~~~~~~~~

Per aggiungere utenti, si può implementare un :doc:`form di registrazione </cookbook/doctrine/registration_form>`
o aggiungere delle `fixture`_. È solo una normale entità, quindi non c'è niente di
speciale, *tranne* il fatto che si devono codificare le password. Ma niente
paura, Symfony dispone di un servizio che se ne occuperà. Vedere :ref:`security-encoding-password`
per i dettagli.

Di seguito c'è un'esportazione della tabella ``utente`` di MySQL con utente ``admin``
e password ``admin`` (codificata).

.. code-block:: bash

    $ mysql> SELECT * FROM utente;
    +----+----------+--------------------------------------------------------------+--------------------+-----------+
    | id | username | password                                                     | email              | is_active |
    +----+----------+--------------------------------------------------------------+--------------------+-----------+
    |  1 | admin    | $2a$08$jHZj/wJfcVKlIwr5AvR78euJxYK7Ku5kURNhNx.7.CSIJ3Pq6LEPC | admin@example.com  |         1 |
    +----+----------+--------------------------------------------------------------+--------------------+-----------+

.. sidebar:: Si ha bisogno di un sale?

    Se si usa ``bcrypt``, no. Altrimenti, sì. Tutte le password necessitano
    di un sale, ma ``bcrypt`` se ne occupa internamente. Siccome questa ricetta usa
    effettivamente ``bcrypt``, il metodo ``getSalt()`` di ``Utente`` può
    restituire ``null`` (non viene usato). Se si usa un algoritmo diverso, si devono
    scommentare le righe con ``salt`` nell'entità ``Utente`` e aggiungere un
    proprietà ``salt``.

.. _security-advanced-user-interface:

Inibire gli utenti inattivi (AdvancedUserInterface)
---------------------------------------------------

Se la proprietà ``isActive`` di ``Utente`` è ``false`` (cioè se ``is_active``
è 0 nella base dati), l'utente potrà ancora eseguire l'accesso al sito.
Per prevenire l'accesso di utenti "inattivi", occorre un po' di lavoro in più.

Il modo più facile per escludere gli utenti inattivi è implementare l'interfaccia
:class:`Symfony\\Component\\Security\\Core\\User\\AdvancedUserInterface`.
L'interfaccia estende :class:`Symfony\\Component\\Security\\Core\\User\\UserInterface`,
quindi occorre solo modificare l'interfaccia::

    // src/AppBundle/Entity/Utente.php

    use Symfony\Component\Security\Core\User\AdvancedUserInterface;
    // ...

    class Utente implements AdvancedUserInterface, \Serializable
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

        // bisogna modificare serialize e unserialize (vedere più avanti)
        public function serialize()
        {
            return serialize(array(
                // ...
                $this->isActive
            ));
        }
        public function unserialize($serialized)
        {
            list (
                // ...
                $this->isActive
            ) = unserialize($serialized);
        }
    }

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

Se *uno* di questi metodi restituisce ``false``, all'utente sarà negato l'accesso. Si può
scegliere di avere proprietà persistite per tutti o solo per alcuni di questi
(in questo esempio, solo ``isActive`` è su base dati).

Qual è la differenza tra questi metodi? Ognuno restituisce un messaggio di errore
diverso (traducibile quando si rende il template di login, se
occorre ulteriore personalizzazione).

.. note::

    Quando si usa ``AdvancedUserInterface``, si deve aggiungere anche una delle
    proprietà usate da tali metodi (come ``isActive()``) al metodo ``serialize()``.
    Se *non* lo si fa, l'oggetto utente potrebbe non essere deserializzato correttamente
    dalla sessione a ogni richiesta.

Ottimo! Il sistema di sicurezza su base dati è pronto! Successivamente, aggiungere un
vero :doc:`form di login </cookbook/security/form_login>` invece di HTTP Basic
oppure continuare a leggere i prossimi argomenti.

.. _authenticating-someone-with-a-custom-entity-provider:

Usare una query personalizzata per caricare gli utenti
------------------------------------------------------

Sarebbe bello se un utente potesse autenticarsi con il suo nome *o* con il suo indirizzo email, che
sono entrambi unici nella base dati. Sfortunatamente, il fornitore di entità nativo è in grado di
gestire una sola proprietà per recuperare l'utente dalla base dati.

Per poterlo fare, fare in modo che ``UtenteRepository`` implementi l'interfaccia
:class:`Symfony\\Component\\Security\\Core\\User\\UserProviderInterface`. Questa
interfaccia ha tre metodi da implementare: ``loadUserByUsername($username)``,
``refreshUser(UserInterface $user)`` e ``supportsClass($class)``::

    // src/AppBundle/Entity/UserRepository.php
    namespace AppBundle\Entity;

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
                ->getOneOrNullResult();

            if (null === $user) {
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

Per maggiori dettagli su questi metodi, vedere :class:`Symfony\\Component\\Security\\Core\\User\\UserProviderInterface`.

.. tip::

    Non dimenticare di aggiungere la classe repository alla
    :ref:`definizione di mappatura dell'entità <book-doctrine-custom-repository-classes>`.

Per concludere l'implementazione, basta rimuovere il campo ``property``
dal file ``security.yml``.

.. configuration-block::

    .. code-block:: yaml

        # app/config/security.yml
        security:
            # ...
            providers:
                fornitore:
                    entity:
                        class: AppBundle:Utente
            # ...

    .. code-block:: xml

        <!-- app/config/security.xml -->
        <config>
            <!-- ... -->

            <provider name="fornitore">
                <entity class="AppBundle:Utente" />
            </provider>

            <!-- ... -->
        </config>

    .. code-block:: php

        // app/config/security.php
        $container->loadFromExtension('security', array(
            ...,
            'providers' => array(
                'fornitore' => array(
                    'entity' => array(
                        'class' => 'AppBundle:Utente',
                    ),
                ),
            ),
            ...,
        ));

In questo modo, il livello della sicurezza userà un'istanza di ``UserRepository`` e
richiamerà il suo metodo ``loadUserByUsername()`` per recuperare un utente dalla base dati,
sia che abbia inserito il suo nome utente che abbia inserito la sua email.

.. _`cookbook-security-serialize-equatable`:

Capire la serializzazione e come un utente è salvato in sessione
----------------------------------------------------------------

Questa sezione è per chi fosse curioso riguardo all'importanza del metodo ``serialize()``
della classe ``User`` o su come l'oggetto utente sia serializzato e
deserializzato.

Una volta che l'utente ha eseguito l'accesso l'intero oggetto ``Utente`` è serializzato
in sessione. Alla richiesta successiva, l'oggetto ``Utente`` è deserializzato. Quindi,
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
sopra). Questo assicura che tutti i dati dell'utente siano aggiornati.

Symfony usa anche ``username``, ``salt`` e ``password`` per verificare che
l'utente non sia cambiato tra una richiesta e l'altra (richiama anche i metodi di ``AdvancedUsetInterface``,
se sono stati implementati). Non serializzare queste informazioni
potrebbe causare il logout dell'utente. Se ``User`` implementa
:class:`Symfony\\Component\\Security\\Core\\User\\EquatableInterface`,
invece di confrontare queste proprietà, viene semplicemente richiamato il metodo ``isEqualTo``
e si possono verificare tutte le proprietà desiderate. Se questo aspetto non è chiaro,
probabilmente è meglio non implementare questa interfaccia.


.. versionadded:: 2.1
    In Symfony 2.1, è stato rimosso il metodo ``equals`` da ``UserInterface``
    ed è stata introdotta l'interfaccia ``EquatableInterface`` al suo posto.

.. _fixtures: http://symfony.com/doc/master/bundles/DoctrineFixturesBundle/index.html
.. _FOSUserBundle: https://github.com/FriendsOfSymfony/FOSUserBundle
