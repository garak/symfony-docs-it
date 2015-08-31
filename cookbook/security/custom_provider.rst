.. index::
   single: Sicurezza; Fornitore utenti

Creare un fornitore utenti personalizzato
=========================================

Parte del processo standard di autenticazione di Symfony dipende dai "fornitori utenti".
Quando un utente invia nome e password, il livello di autenticazione chiede al fornitore
utenti configurato di restituire un oggetto utente per un dato nome utente.
Symfony quindi verifica che la password di tale utente sia corretta e genera
un token di sicurezza, in modo che l'utente resti autenticato per la sessione corrente.
Symfony dispone di due fornitori utenti predefiniti, "in_memory" e "entity".
In questa ricetta, vedremo come poter creare il proprio fornitore utenti, che potrebbe
essere utile se gli utenti accedono tramite una base dati personalizzata, un file, oppure
(come mostrato in questo esempio) tramite un servizio web.

Creare una classe utente
------------------------

Prima di tutto, indipendentemente dalla *provenienza* dei dati utente, occorre creare
una classe ``User``, che rappresenti tali dati. La classe ``User``, comunque, può
essere fatta a piacere e contenere qualsiasi dato si desideri. L'unico requisito è che
implementi :class:`Symfony\\Component\\Security\\Core\\User\\UserInterface`.
I metodi in tale interfaccia vanno quindi definiti nella classe utente personalizzata:
:method:`Symfony\\Component\\Security\\Core\\User\\UserInterface::getRoles`,
:method:`Symfony\\Component\\Security\\Core\\User\\UserInterface::getPassword`,
:method:`Symfony\\Component\\Security\\Core\\User\\UserInterface::getSalt`,
:method:`Symfony\\Component\\Security\\Core\\User\\UserInterface::getUsername`,
:method:`Symfony\\Component\\Security\\Core\\User\\UserInterface::eraseCredentials`.
Potrebbe essere utile anche implementare l'interfaccia
:class:`Symfony\\Component\\Security\\Core\\User\\EquatableInterface`,
che definisce un metodo per verificare se l'utente corrisponde all'utente corrente. Tale
interfaccia richiede un metodo
:method:`Symfony\\Component\\Security\\Core\\User\\EquatableInterface::isEqualTo`.

Ecco la class ``WebserviceUser`` in azione::

    // src/Acme/WebserviceUserBundle/Security/User/WebserviceUser.php
    namespace Acme\WebserviceUserBundle\Security\User;

    use Symfony\Component\Security\Core\User\UserInterface;
    use Symfony\Component\Security\Core\User\EquatableInterface;

    class WebserviceUser implements UserInterface, EquatableInterface
    {
        private $username;
        private $password;
        private $salt;
        private $roles;

        public function __construct($username, $password, $salt, array $roles)
        {
            $this->username = $username;
            $this->password = $password;
            $this->salt = $salt;
            $this->roles = $roles;
        }

        public function getRoles()
        {
            return $this->roles;
        }

        public function getPassword()
        {
            return $this->password;
        }

        public function getSalt()
        {
            return $this->salt;
        }

        public function getUsername()
        {
            return $this->username;
        }

        public function eraseCredentials()
        {
        }

        public function isEqualTo(UserInterface $user)
        {
            if (!$user instanceof WebserviceUser) {
                return false;
            }

            if ($this->password !== $user->getPassword()) {
                return false;
            }

            if ($this->salt !== $user->getSalt()) {
                return false;
            }

            if ($this->username !== $user->getUsername()) {
                return false;
            }

            return true;
        }
    }

Se si hanno maggiori informazioni sui propri utenti, come il nome di battesimo, si
possono aggiungere campi per memorizzare tali dati.

Creare un fornitore utenti
--------------------------

Ora che abbiamo una classe ``User``, creeremo un fornitore di utenti, che estrarrà
informazioni da un servizio web, creerà un oggetto ``WebserviceUser`` e lo
popolerà con i dati.

Il fornitore utenti è semplicemente una classe PHP che deve implementare
:class:`Symfony\\Component\\Security\\Core\\User\\UserProviderInterface`, 
la quale richiede la definizione di tre metodi: ``loadUserByUsername($username)``,
``refreshUser(UserInterface $user)`` e ``supportsClass($class)``. Per maggiori
dettagli, vedere :class:`Symfony\\Component\\Security\\Core\\User\\UserProviderInterface`.

Ecco un esempio di come potrebbe essere::

    // src/Acme/WebserviceUserBundle/Security/User/WebserviceUserProvider.php
    namespace Acme\WebserviceUserBundle\Security\User;

    use Symfony\Component\Security\Core\User\UserProviderInterface;
    use Symfony\Component\Security\Core\User\UserInterface;
    use Symfony\Component\Security\Core\Exception\UsernameNotFoundException;
    use Symfony\Component\Security\Core\Exception\UnsupportedUserException;

    class WebserviceUserProvider implements UserProviderInterface
    {
        public function loadUserByUsername($username)
        {
            // fare qui una chiamata al servizio web
            $userData = ...
            // supponiamo che restituisca un array, oppure false se non trova utenti

            if ($userData) {
                $password = '...';

                // ...

                return new WebserviceUser($username, $password, $salt, $roles);
            }

            throw new UsernameNotFoundException(
                sprintf('Nome utente "%s" non trovato.', $username)
            );
        }

        public function refreshUser(UserInterface $user)
        {
            if (!$user instanceof WebserviceUser) {
                throw new UnsupportedUserException(
                    sprintf('Istanza di "%s" non supportata.', get_class($user))
                );
            }

            return $this->loadUserByUsername($user->getUsername());
        }

        public function supportsClass($class)
        {
            return $class === 'Acme\WebserviceUserBundle\Security\User\WebserviceUser';
        }
    }

Creare un servizio per il fornitore utenti
------------------------------------------

Ora renderemo il fornitore utenti disponibile come servizio.

.. configuration-block::

    .. code-block:: yaml

        # src/Acme/WebserviceUserBundle/Resources/config/services.yml
        services:
            webservice_user_provider:
                class: Acme\WebserviceUserBundle\Security\User\WebserviceUserProvider

    .. code-block:: xml

        <!-- src/Acme/WebserviceUserBundle/Resources/config/services.xml -->
        <services>
            <service id="webservice_user_provider" class="Acme\WebserviceUserBundle\Security\User\WebserviceUserProvider" />
        </services>

    .. code-block:: php

        // src/Acme/WebserviceUserBundle/Resources/config/services.php
        use Symfony\Component\DependencyInjection\Definition;

        $container->setDefinition(
            'webservice_user_provider',
            new Definition('Acme\WebserviceUserBundle\Security\User\WebserviceUserProvider')
        );

.. tip::

    La vera implementazione del fornitore utenti avrà probabilmente alcune
    dipendenze da opzioni di configurazione o altri servizi. Aggiungerli come
    parametri nella definizione del servizio.

.. note::

    Assicurarsi che il file dei servizi sia importato. Vedere :ref:`service-container-imports-directive`
    per maggiori dettagli.

Modificare ``security.yml``
---------------------------

È tutto in ``/app/config/security.yml``. Aggiungere il fornitore di utenti alla
lista di fornitori nella sezione "security". Scegliere un nome per il fornitore di utenti
(p.e. "webservice") e menzionare l'id del servizio appena definito.

.. configuration-block::

    .. code-block:: yaml

        # app/config/security.yml
        security:
            providers:
                webservice:
                    id: webservice_user_provider

    .. code-block:: xml

        <!-- app/config/security.xml -->
        <config>
            <provider name="webservice" id="webservice_user_provider" />
        </config>

    .. code-block:: php

        // app/config/security.php
        $container->loadFromExtension('security', array(
            'providers' => array(
                'webservice' => array(
                    'id' => 'webservice_user_provider',
                ),
            ),
        ));

Symfony deve anche sapere come codificare le password fornite dagli utenti, per esempio
quando compilano il form di login. Lo si può fare aggiungendo una riga alla sezione
"encoders", in ``/app/config/security.yml``. 

.. configuration-block::

    .. code-block:: yaml

        # app/config/security.yml
        security:
            encoders:
                Acme\WebserviceUserBundle\Security\User\WebserviceUser: sha512

    .. code-block:: xml

        <!-- app/config/security.xml -->
        <config>
            <encoder class="Acme\WebserviceUserBundle\Security\User\WebserviceUser">sha512</encoder>
        </config>

    .. code-block:: php

        // app/config/security.php
        $container->loadFromExtension('security', array(
            'encoders' => array(
                'Acme\WebserviceUserBundle\Security\User\WebserviceUser' => 'sha512',
            ),
        ));

Il valore inserito deve corrispondere al modo in cui le password sono state codificate
originariamente, alla creazione degli utenti (in qualsiasi modo siano stati creati).
Quando un utente inserisce la sua password, la password viene concatenata con il valore
del sale e quindi codificata con questo algoritmo, prima di confrontarla con la password
restituita dal proprio metodo ``getPassword()``. Inoltre, a seconda delle proprie opzioni,
la password può essere codificata più volte e poi codificata in base64.

.. sidebar:: Specifiche sulle codifiche delle password

    Symfony usa un metodo specifico per concatenare il sale e codificare la password,
    prima di confrontarla con la password memorizzata. Se ``getSalt()`` non restituisce
    nulla, la password inserita è semplicemente codificata con l'algoritmo specificato
    in ``security.yml``. Se invece il sale *è* fornito, il seguente valore viene creato e
    *poi* codificato tramite l'algoritmo:

        ``$password.'{'.$salt.'}';``

    Se gli utenti esterni hanno password con sali diversi, occorre un po' di lavoro in
    più per far sì che Symfony possa codificare correttamente la password.
    Questo va oltre lo scopo di questa ricetta, possiamo accennare che includerebbe la
    creazione di una sotto-classe di ``MessageDigestPasswordEncoder`` e la sovrascrittura
    del metodo ``mergePasswordAndSalt``.

    Inoltre, per impostazione predefinita, l'hash è codificato più volte e poi codificato 
    in base64. Per i dettagli, si veda `MessageDigestPasswordEncoder`_.
    Se lo si vuole evitare, impostarlo nel file di configurazione:

    .. configuration-block::

        .. code-block:: yaml

            # app/config/security.yml
            security:
                encoders:
                    Acme\WebserviceUserBundle\Security\User\WebserviceUser:
                        algorithm: sha512
                        encode_as_base64: false
                        iterations: 1

        .. code-block:: xml

            <!-- app/config/security.xml -->
            <config>
                <encoder class="Acme\WebserviceUserBundle\Security\User\WebserviceUser"
                    algorithm="sha512"
                    encode-as-base64="false"
                    iterations="1"
                />
            </config>

        .. code-block:: php

            // app/config/security.php
            $container->loadFromExtension('security', array(
                'encoders' => array(
                    'Acme\WebserviceUserBundle\Security\User\WebserviceUser' => array(
                        'algorithm'         => 'sha512',
                        'encode_as_base64'  => false,
                        'iterations'        => 1,
                    ),
                ),
            ));

.. _MessageDigestPasswordEncoder: https://github.com/symfony/symfony/blob/master/src/Symfony/Component/Security/Core/Encoder/MessageDigestPasswordEncoder.php
