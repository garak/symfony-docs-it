.. index::
   single: Security, Autenticazione

Autenticazione
==============

Quando una richiesta punta a un'area protetta e se uno degli ascoltatori della mappa
dei firewall è in grado di estrarre le credenziali dell'utente dall'oggetto
:class:`Symfony\\Component\\HttpFoundation\\Request` corrente, dovrebbe creare
un token, che contiene tali credenziali. La cosa successiva che l'ascoltatore dovrebbe
fare è chiedere al gestore di autenticazione di validare il token fornito e restituire
un token *autenticato*, se le credenziali fornite sono state riconosciute come valide.
L'ascoltatore quindi dovrebbe memorizzare il token autenticato nel contesto di sicurezza::

    use Symfony\Component\Security\Http\Firewall\ListenerInterface;
    use Symfony\Component\Security\Core\SecurityContextInterface;
    use Symfony\Component\Security\Core\Authentication\AuthenticationManagerInterface;
    use Symfony\Component\HttpKernel\Event\GetResponseEvent;
    use Symfony\Component\Security\Core\Authentication\Token\UsernamePasswordToken;

    class SomeAuthenticationListener implements ListenerInterface
    {
        /**
         * @var SecurityContextInterface
         */
        private $securityContext;

        /**
         * @var AuthenticationManagerInterface
         */
        private $authenticationManager;

        /**
         * @var string Identifica univocamente l'area protetta
         */
        private $providerKey;

        // ...

        public function handle(GetResponseEvent $event)
        {
            $request = $event->getRequest();

            $username = ...;
            $password = ...;

            $unauthenticatedToken = new UsernamePasswordToken(
                $username,
                $password,
                $this->providerKey
            );

            $authenticatedToken = $this
                ->authenticationManager
                ->authenticate($unauthenticatedToken);

            $this->securityContext->setToken($authenticatedToken);
        }
    }

.. note::

    Un token può essere di qualsiasi classe, a patto che implementi
    :class:`Symfony\\Component\\Security\\Core\\Authentication\\Token\\TokenInterface`.

Il gestore di autenticazione
----------------------------

Il gestore di autenticazione predefinito è un'istanza di
:class:`Symfony\\Component\\Security\\Core\\Authentication\\AuthenticationProviderManager`::

    use Symfony\Component\Security\Core\Authentication\AuthenticationProviderManager;

    // instanze di Symfony\Component\Security\Core\Authentication\AuthenticationProviderInterface
    $providers = array(...);

    $authenticationManager = new AuthenticationProviderManager($providers);

    try {
        $authenticatedToken = $authenticationManager
            ->authenticate($unauthenticatedToken);
    } catch (AuthenticationException $failed) {
        // autenticazione fallita
    }

``AuthenticationProviderManager``, quando istanziata, riceve vari fornitori
di autenticazione, ciascuno che supporta un diverso tipo di token.

.. note::

    Ovviamente, si può scrivere un proprio gestore di autenticazione, basta che
    implementi :class:`Symfony\\Component\\Security\\Core\\Authentication\\AuthenticationManagerInterface`.

.. _authentication_providers:

Fornitori di autenticazione
---------------------------

Ogni fornitore (poiché implementa
:class:`Symfony\\Component\\Security\\Core\\Authentication\\Provider\\AuthenticationProviderInterface`)
ha un metodo :method:`Symfony\\Component\\Security\\Core\\Authentication\\Provider\\AuthenticationProviderInterface::supports`
da cui ``AuthenticationProviderManager``
può determinare se supporti il dato token. Se questo è il caso, il gestore
richiama il metodo :class:`Symfony\\Component\\Security\\Core\\Authentication\\Provider\\AuthenticationProviderInterface::authenticate` del fornitore.
Tale metodo dovrebbe restituire un token autenticato o lanciare una
:class:`Symfony\\Component\\Security\\Core\\Exception\\AuthenticationException`
(o un'eccezione che la estenda).

Autenticare utenti con nome e password
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Un fornitore di autenticazione proverà ad autenticare un utente in base alle
credenziali fornite. Solitamente queste sono un nome utente e una password.
La maggior parte delle applicazioni web memorizzano i nomi utente e un hash delle
password combinate con un sale generato casualmente. Ciò vuol dire che
l'autenticazione media consiste nel recuperare il sale e l'hash della password dal
sistema di memorizzazione dei dati degli utenti, trasformare in hash la password appena
fornita dall'utente (p.e. in un form di login) con il sale e confrontare entrambi, per determinare
se la password fornita sia valida.

Tale funzionalità è offerta da :class:`Symfony\\Component\\Security\\Core\\Authentication\\Provider\\DaoAuthenticationProvider`.
Questa classe recupera i dati dell'utente da un :class:`Symfony\\Component\\Security\\Core\\User\\UserProviderInterface``,
usa un :class:`Symfony\\Component\\Security\\Core\\Encoder\\PasswordEncoderInterface`
per creare un hash della password e restituisce un token autenticato, se la
password è valida::

    use Symfony\Component\Security\Core\Authentication\Provider\DaoAuthenticationProvider;
    use Symfony\Component\Security\Core\User\UserChecker;
    use Symfony\Component\Security\Core\User\InMemoryUserProvider;
    use Symfony\Component\Security\Core\Encoder\EncoderFactory;

    $userProvider = new InMemoryUserProvider(
        array(
            'admin' => array(
                // la password è "foo"
                'password' => '5FZ2Z8QIkA7UTZ4BYkoC+GsReLf569mSKDsfods6LYQ8t+a8EW9oaircfMpmaLbPBh4FOBiiFyLfuZmTSUwzZg==',
                'roles'    => array('ROLE_ADMIN'),
            ),
        )
    );

    // per alcuni controlli ulteriori: account abilitato, bloccato, scaduto, ecc.?
    $userChecker = new UserChecker();

    // un array di codificatori di password (vedere più avanti)
    $encoderFactory = new EncoderFactory(...);

    $provider = new DaoAuthenticationProvider(
        $userProvider,
        $userChecker,
        'secured_area',
        $encoderFactory
    );

    $provider->authenticate($unauthenticatedToken);

.. note::

    L'esempio sopra dimostra l'uso di un fornitore "in-memory" (in memoria),
    ma si può usare qualsiasi fornitore di utente, purché implementi
    :class:`Symfony\\Component\\Security\\Core\\User\\UserProviderInterface`.
    È anche possibile far cercare i dati dell'utente a più di un fornitore di utenti,
    usando :class:`Symfony\\Component\\Security\\Core\\User\\ChainUserProvider`.

Il factory codificatore di password
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

:class:`Symfony\\Component\\Security\\Core\\Authentication\\Provider\\DaoAuthenticationProvider`
usa un factory codificatore per creare un codificatore di password per un dato tipo di
utente. Questo consente di usare diverse strategie di codifica per diversi
tipi di utenti. La classe predefinita :class:`Symfony\\Component\\Security\\Core\\Encoder\\EncoderFactory`
riceve un array di codificatori::

    use Symfony\Component\Security\Core\Encoder\EncoderFactory;
    use Symfony\Component\Security\Core\Encoder\MessageDigestPasswordEncoder;

    $defaultEncoder = new MessageDigestPasswordEncoder('sha512', true, 5000);
    $weakEncoder = new MessageDigestPasswordEncoder('md5', true, 1);

    $encoders = array(
        'Symfony\\Component\\Security\\Core\\User\\User' => $defaultEncoder,
        'Acme\\Entity\\LegacyUser'                       => $weakEncoder,

        // ...
    );

    $encoderFactory = new EncoderFactory($encoders);

Ogni codificatore deve implementare :class:`Symfony\\Component\\Security\\Core\\Encoder\\PasswordEncoderInterface`
o essere un array con chiavi ``class`` e ``arguments``, che consente al
factory codificatore di costruire il codificatore solo quando necessario.

Creare un codificatore di password
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Ci sono molti codificatori di password predefiniti. Se si ha l'esigenza di crearne
uno nuovo, basta seguire le seguenti tre regole:

#. La classe deve implementare :class:`Symfony\\Component\\Security\\Core\\Encoder\\PasswordEncoderInterface`;

#. La prima riga in ``encodePassword`` e ``isPasswordValid`` deve
   assicurarsi che la password non sia troppo lunga (p.e. 4096). Questo per ragioni di sicurezza
   (vedere `CVE-2013-5750`_). Si può copiare l'implementazione di `BasePasswordEncoder::checkPasswordLength`_
   da Symfony 2.4.

Usare codificatori di password
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Quando il metodo :method:`Symfony\\Component\\Security\\Core\\Encoder\\EncoderFactory::getEncoder`
del factory codificatore di password viene richiamato con l'oggetto utente come
primo parametro, restituirà un codificatore di tipo :class:`Symfony\\Component\\Security\\Core\\Encoder\\PasswordEncoderInterface`,
che va usato per codificare la password dell'utente::

    // recupera un utente di tipo Acme\Entity\LegacyUser
    $user = ...

    $encoder = $encoderFactory->getEncoder($user);

    // restituirà $weakEncoder (vedere sopra)

    $encodedPassword = $encoder->encodePassword($password, $user->getSalt());

    // verifica se la password è valida:

    $validPassword = $encoder->isPasswordValid(
        $user->getPassword(),
        $password,
        $user->getSalt());

.. _`CVE-2013-5750`: http://symfony.com/blog/cve-2013-5750-security-issue-in-fosuserbundle-login-form
.. _`BasePasswordEncoder::checkPasswordLength`: https://github.com/symfony/symfony/blob/master/src/Symfony/Component/Security/Core/Encoder/BasePasswordEncoder.php
