.. index::
    single: Security; Autenticatore di richieste personalizzato

Autenticare utenti con chiavi API
=================================

Oggigiorno, non è insolito autenticare un utente tramite una chiave API (per esempio,
sviluppando un servizio web). La chiave API va fornita per ciascuna richiesta ed è
passata come parametro di query string oppure tramite un header HTTP.

L'autenticatore di chiave API
-----------------------------

.. versionadded:: 2.4
    L'interfaccia ``SimplePreAuthenticatorInterface`` è stata introdotta in Symfony 2.4.

Si dovrebbe autenticare un utente in base alle informazioni della richiesta tramite un
meccanismo di pre-autenticazione. :class:`Symfony\\Component\\Security\\Core\\Authentication\\SimplePreAuthenticatorInterface`
consente di implementare tale schema in modo molto facile.

La situazione esatta potrebbe essere diversa, ma in questo esempio viene letto un token
da un parametro ``apikey``, il nome utente viene caricato in base a quel valore
e quindi viene creato un oggetto utente::

    // src/Acme/HelloBundle/Security/ApiKeyAuthenticator.php
    namespace Acme\HelloBundle\Security;

    use Symfony\Component\Security\Core\Authentication\SimplePreAuthenticatorInterface;
    use Symfony\Component\Security\Core\Authentication\Token\TokenInterface;
    use Symfony\Component\Security\Core\Exception\AuthenticationException;
    use Symfony\Component\Security\Core\Authentication\Token\PreAuthenticatedToken;
    use Symfony\Component\HttpFoundation\Request;
    use Symfony\Component\Security\Core\User\UserProviderInterface;
    use Symfony\Component\Security\Core\Exception\UsernameNotFoundException;
    use Symfony\Component\Security\Core\Exception\BadCredentialsException;

    class ApiKeyAuthenticator implements SimplePreAuthenticatorInterface
    {
        protected $userProvider;

        public function __construct(ApiKeyUserProvider $userProvider)
        {
            $this->userProvider = $userProvider;
        }

        public function createToken(Request $request, $providerKey)
        {
            // cerca il parametro apikey
            $apiKey = $request->query->get('apikey');

            // oppure, cerca un header "apikey"
            // $apiKey = $request->headers->get('apikey');

            if (!$apiKey) {
                throw new BadCredentialsException('Chiave API non trovata');
            }

            return new PreAuthenticatedToken(
                'anon.',
                $apiKey,
                $providerKey
            );
        }

        public function authenticateToken(TokenInterface $token, UserProviderInterface $userProvider, $providerKey)
        {
            $apiKey = $token->getCredentials();
            $username = $this->userProvider->getUsernameForApiKey($apiKey);

            if (!$username) {
                throw new AuthenticationException(
                    sprintf('La chiave API "%s" non esiste.', $apiKey)
                );
            }

            $user = $this->userProvider->loadUserByUsername($username);

            return new PreAuthenticatedToken(
                $user,
                $apiKey,
                $providerKey,
                $user->getRoles()
            );
        }

        public function supportsToken(TokenInterface $token, $providerKey)
        {
            return $token instanceof PreAuthenticatedToken && $token->getProviderKey() === $providerKey;
        }
    }

Dopo aver :ref:`configurato <cookbook-security-api-key-config>` ogni cosa,
si potrà autenticare aggiungendo un parametro ``apikey`` alla query
string, come ``http://example.com/admin/pippo?apikey=37b51d194a7513e45b56f6524f2d51f2``.

Il processo di autenticazione ha molti passi e la vera impolementazione probabilmente
sarà diversa:

1. createToken
~~~~~~~~~~~~~~

All'inizio del ciclo di richiesta, Symfony richiama ``createToken()``. Qui si deve
creare un oggetto token, che contenga tutte le informazioni della
richiesta necessarie per autenticare l'utente (p.e. il parametro ``apikey``).
Se tali informazioni mancano, lanciare una
:class:`Symfony\\Component\\Security\\Core\\Exception\\BadCredentialsException`
causerà il fallimento dell'autenticazione.

2. supportsToken
~~~~~~~~~~~~~~~~

.. include:: _supportsToken.rst.inc

3. authenticateToken
~~~~~~~~~~~~~~~~~~~~

Se ``supportsToken()`` restituisce ``true``, Symfony richiamerà ``authenticateToken()``.
Una parte importante è ``$userProvider``, che è una classe esterna che aiuta a
caricare informazioni sull'utente. Verrà approfondita successivamente.

In questo specifico esempio, accadono le seguenti cose in ``authenticateToken()``:

#. Primo, si usa ``$userProvider`` per cercare in qualche modo lo ``$username`` che
   corrisponda ad ``$apiKey``;
#. Secondo, si usa ancora ``$userProvider`` per caricare o creare un oggetto ``User``
   per ``$username``;
#. Infine, si crea un *token di autenticazione* (cioè un token con almeno un
   ruolo), che ha i ruoli giusti e l'oggetto utente allegato.

Lo scopo finale è usare ``$apiKey`` per trovare o creare un oggetto ``User``.
*Come* farlo (p.e. cercare in una base dati) e la classe esatta per
l'oggetto ``User`` possono variare. Tali differenze saranno più ovvie nel
fornitore di utenti.

Il fornitore di utenti
~~~~~~~~~~~~~~~~~~~~~~

``$userProvider`` può essere un qualsiasi fornitore di utenti (vedere :doc:`/cookbook/security/custom_provider`).
In questo esempio, si usa ``$apiKey`` per trovare in qualche modo il nome utente.
Questo lavoro è svolto in un metodo ``getUsernameForApiKey()``, che è
creato in modo del tutto personalizzato per questo caso d'uso (cioè non un metodo usato
dal sistema per fornire gli utenti dei nucleo di Symfony).

``$userProvider`` potrebbe assomigliare a questo::

    // src/Acme/HelloBundle/Security/ApiKeyUserProvider.php
    namespace Acme\HelloBundle\Security;

    use Symfony\Component\Security\Core\User\UserProviderInterface;
    use Symfony\Component\Security\Core\User\User;
    use Symfony\Component\Security\Core\User\UserInterface;
    use Symfony\Component\Security\Core\Exception\UnsupportedUserException;

    class ApiKeyUserProvider implements UserProviderInterface
    {
        public function getUsernameForApiKey($apiKey)
        {
            // Cerca il nome utente in base al token nella base dati, tramite
            // una chiamata ad API oppure con qualcosa di completamente diverso
            $username = ...;

            return $username;
        }

        public function loadUserByUsername($username)
        {
            return new User(
                $username,
                null,
                // i ruoli dell'utente. Potrebbero essere determinati
                // dinamicamente in qualche modo
                array('ROLE_USER')
            );
        }

        public function refreshUser(UserInterface $user)
        {
            // usato per memorizzare l'autenticazione nella sessione,
            // ma in questo esempio il token è inviato a ogni richiesta,
            // per cui l'autenticazione può essere senza stato. Lanciare questa eccezione
            // è il modo per rendere le cose senza stato
            throw new UnsupportedUserException();
        }

        public function supportsClass($class)
        {
            return 'Symfony\Component\Security\Core\User\User' === $class;
        }
    }

.. note::

    Leggere la ricetta dedicata a come
    :doc:`creare un fornitore di utenti personalizzato </cookbook/security/custom_provider>`.

La logica in ``getUsernameForApiKey()`` è a carico dello sviluppatore. Si potrebbe trasformare in qualche modo
la chiave API (p.e. ``37b51d``) in un nome utente (p.e. ``tizio``) cercando
delle informazioni in una tabella "token" di una base dati.

Lo stesso per ``loadUserByUsername()``. In questo esempio, viene semplicemente creata la classe
:class:`Symfony\\Component\\Security\\Core\\User\\User` del nucleo di Symfony.
Questo ha senso se non serve memorizzare ulteriori informazioni sull'oggetto
utente (come ``firstName``). Se invece serve, si potrebbe invece avere una classe utente
personalizzata, da creare e in cui si popola l'utente recuperandolo da una base dati. Questo
consentirebbe di avere dati personalizzati nell'oggetto ``User``.

Infine, assicurarsi che ``supportsClass()`` restituisca ``true`` per oggetti utente
della stessa classe restituita da ``loadUserByUsername()``.
Se l'autenticazione è senza stato, come in questo esempio (cioè ci si aspetta
che l'utente invii la chiave API a ogni richiesta e quindi non si salva il
login in session), si può semplicemente lanciare una ``UnsupportedUserException``
in ``refreshUser()``.

.. note::

    Se invece si vuole memorizzare dati di autenticazione in sessione, in moodo che
    la chiave non debba essere inviare a ogni richiesta, vedere :ref:`cookbook-security-api-key-session`.

Gestire il fallimento dell'autenticazione
-----------------------------------------

Per fare in modo che ``ApiKeyAuthentication`` mostri correttamente un codice di stato HTTP
403 quando l'autenticazione fallisce, si dovrà
implementare :class:`Symfony\\Component\\Security\\Http\\Authentication\\AuthenticationFailureHandlerInterface`
nell'autenticatore. Questa fornirà un metodo ``onAuthenticationFailure``, che 
si può usare per creare una risposta di errore.

.. code-block:: php

    // src/Acme/HelloBundle/Security/ApiKeyAuthenticator.php
    namespace Acme\HelloBundle\Security;

    use Symfony\Component\Security\Core\Authentication\SimplePreAuthenticatorInterface;
    use Symfony\Component\Security\Core\Exception\AuthenticationException;
    use Symfony\Component\Security\Http\Authentication\AuthenticationFailureHandlerInterface;
    use Symfony\Component\HttpFoundation\Response;
    use Symfony\Component\HttpFoundation\Request;

    class ApiKeyAuthenticator implements SimplePreAuthenticatorInterface, AuthenticationFailureHandlerInterface
    {
        // ...

        public function onAuthenticationFailure(Request $request, AuthenticationException $exception)
        {
            return new Response("Autenticazione fallita.", 403);
        }
    }

.. _cookbook-security-api-key-config:

Configurazione
--------------

Una volta preparata ``ApiKeyAuthentication``, la si deve registrare
come servizio e sarla nella configurazione della sicurezza (``security.yml``).
Primo, registrarla come servizio. Questo presume di aver già preparato
il fornitore di utenti personalizzato come servizio, chiamato ``fornitore_utenti_apikey``
(vedere :doc:`/cookbook/security/custom_provider`).

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        services:
            # ...

            autenticatore_apikey:
                class:     Acme\HelloBundle\Security\ApiKeyAuthenticator
                arguments: ["@fornitore_utenti_apikey"]

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <?xml version="1.0" ?>
        <container xmlns="http://symfony.com/schema/dic/services"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://symfony.com/schema/dic/services
                http://symfony.com/schema/dic/services/services-1.0.xsd">
            <services>
                <!-- ... -->

                <service id="autenticatore_apikey"
                    class="Acme\HelloBundle\Security\ApiKeyAuthenticator"
                >
                    <argument type="service" id="fornitore_utenti_apikey" />
                </service>
            </services>
        </container>

    .. code-block:: php

        // app/config/config.php
        use Symfony\Component\DependencyInjection\Definition;
        use Symfony\Component\DependencyInjection\Reference;

        // ...

        $container->setDefinition('autenticatore_apikey', new Definition(
            'Acme\HelloBundle\Security\ApiKeyAuthenticator',
            array(new Reference('fornitore_utenti_apikey'))
        ));

Ora, attivarla nella sezione ``firewalls`` nella configurazione di sicurezza,
usando la voce ``simple_preauth``:

.. configuration-block::

    .. code-block:: yaml

        # app/config/security.yml
        security:
            # ...

            firewalls:
                secured_area:
                    pattern: ^/admin
                    stateless: true
                    simple_preauth:
                        authenticator: autenticatore_apikey

    .. code-block:: xml

        <!-- app/config/security.xml -->
        <?xml version="1.0" encoding="UTF-8"?>
        <srv:container xmlns="http://symfony.com/schema/dic/security"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xmlns:srv="http://symfony.com/schema/dic/services"
            xsi:schemaLocation="http://symfony.com/schema/dic/services
                http://symfony.com/schema/dic/services/services-1.0.xsd">
            <config>
                <!-- ... -->

                <firewall name="secured_area"
                    pattern="^/admin"
                    stateless="true"
                >
                    <simple-preauth authenticator="autenticatore_apikey" />
                </firewall>
            </config>
        </srv:container>

    .. code-block:: php

        // app/config/security.php

        // ..

        $container->loadFromExtension('security', array(
            'firewalls' => array(
                'secured_area'       => array(
                    'pattern'        => '^/admin',
                    'stateless'      => true,
                    'simple_preauth' => array(
                        'authenticator'  => 'autenticatore_apikey',
                    ),
                ),
            ),
        ));

Fatto! Ora ``ApiKeyAuthentication`` sarà richiamata all'inizio di
ogni richiesta e avrà luogo il processo di autenticazione.

Il parametro di configurazione ``stateless`` fa in modo che Symfony non provi a
salvare le informazioni di autenticazione in sessione, cosa non è necessaria,
perché il client manderà ``apikey`` a ogni richiesta. Se invece si ha bisogno di
salvare l'autenticazione in sessione, continuare a leggere.

.. _cookbook-security-api-key-session:

Salvare l'autenticazione in sessione
------------------------------------

Finora, questa ricetta ha descritto una situazione in cui una sorta di token di autenticazione
viene inviato a ogni richiesta. Tuttavia, in alcune situazioni (come nel flusso di OAuth),
il token potrebbe essere inviato in *una sola* richiesta. In tal caso, si vorrà
autenticare l'utente e salvare questa autenticazione in sessione, in modo che
l'utente sia autenticato per ogni richiesta successiva.

Perché funzioni, rimuovere la voce ``stateless`` dalla configurazione del firewall
oppure impostarla a ``false``:

.. configuration-block::

    .. code-block:: yaml

        # app/config/security.yml
        security:
            # ...

            firewalls:
                secured_area:
                    pattern: ^/admin
                    stateless: false
                    simple_preauth:
                        authenticator: autenticatore_apikey

    .. code-block:: xml

        <!-- app/config/security.xml -->
        <?xml version="1.0" encoding="UTF-8"?>
        <srv:container xmlns="http://symfony.com/schema/dic/security"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xmlns:srv="http://symfony.com/schema/dic/services"
            xsi:schemaLocation="http://symfony.com/schema/dic/services
                http://symfony.com/schema/dic/services/services-1.0.xsd">
            <config>
                <!-- ... -->

                <firewall name="secured_area"
                    pattern="^/admin"
                    stateless="false"
                >
                    <simple-preauth authenticator="autenticatore_apikey" />
                </firewall>
            </config>
        </srv:container>

    .. code-block:: php

        // app/config/security.php

        // ..
        $container->loadFromExtension('security', array(
            'firewalls' => array(
                'secured_area'       => array(
                    'pattern'        => '^/admin',
                    'stateless'      => false,
                    'simple_preauth' => array(
                        'authenticator'  => 'autenticatore_apikey',
                    ),
                ),
            ),
        ));

Anche se il token verrà salvato in sessione, le credenziali, in questo caso
la chiave API (cioè ``$token->getCredentials()``), sono sono salvate in sessione,
per ragioni di sicurezza. Per sfruttare la sessione, aggiornare ``ApiKeyAuthenticator``
per verificare se il token salvato abbia un oggetto utente valido e possa essere usato::

    // src/Acme/HelloBundle/Security/ApiKeyAuthenticator.php
    // ...

    class ApiKeyAuthenticator implements SimplePreAuthenticatorInterface
    {
        // ...
        public function authenticateToken(TokenInterface $token, UserProviderInterface $userProvider, $providerKey)
        {
            $apiKey = $token->getCredentials();
            $username = $this->userProvider->getUsernameForApiKey($apiKey);

            // User è l'entità che rappresenta l'utente
            $user = $token->getUser();
            if ($user instanceof User) {
                return new PreAuthenticatedToken(
                    $user,
                    $apiKey,
                    $providerKey,
                    $user->getRoles()
                );
            }

            if (!$username) {
                throw new AuthenticationException(
                    sprintf('La chiave API "%s" non esiste.', $apiKey)
                );
            }

            $user = $this->userProvider->loadUserByUsername($username);

            return new PreAuthenticatedToken(
                $user,
                $apiKey,
                $providerKey,
                $user->getRoles()
            );
        }
        // ...
    }

Il salvataggio delle informazioni di autenticazione funziona in questo modo:

#. Alla fine di ogni richiesta, Symfony serializza l'oggetto token (restituito
   da ``authenticateToken()``), che serializza anche l'oggetto ``User``
   (essendo impostato su una proprietà del token);
#. Alla richiesta successiva, il token è deserializzato e l'oggetto ``User``
   deserializzato viene passato al metodo ``refreshUser()`` del fornitore di utenti.

Il secondo passo è quello importante: Symfony richiama ``refreshUser()`` e passa
l'oggetto utente che era stato serializzato in sessione. Se gli utenti sono
salvati nella base dati, si potrebbe voler fare una nuova query, cercando una versione aggiornata
dell'utente, per assicurarsi che non sia cambiata. Indipentemente dai requisiti,
``refreshUser()`` dovrebbe ora restituire l'oggetto utente::

    // src/Acme/HelloBundle/Security/ApiKeyUserProvider.php

    // ...
    class ApiKeyUserProvider implements UserProviderInterface
    {
        // ...

        public function refreshUser(UserInterface $user)
        {
            // $user è l'utente che era stato impostato nel token in authenticateToken()
            // dopo che è stato deserializzato dalla sessione

            // si potrebbe usare $user per cercare su base dati un utente aggiornato
            // $id = $user->getId();
            // usare $id per fare la query

            // se *non* si legge da una base dati e si sta solo creando un
            // oggetto utente (come in questo esempio), basta restituirlo
            return $user;
        }
    }

.. note::

    Ci si potrebbe anche voler assicurare che l'oggetto ``User`` sia stato serializzato
    correttamente. Se l'oggetto ``User`` ha delle proprietà private, PHP non le può
    serializzare. In tal caso, si potrebbe riavere indietro un oggetto utente con dei ``null``
    su alcune proprietà. Per un esempio, vedere :doc:`/cookbook/security/entity_provider`.

Autenticare solo per alcuni URL
-------------------------------

Questa ricetta ha ipotizzato che si volesse cercare ``apikey``
in *ogni* richiesta. Tuttavia, in alcune situazioni (come nel flusso di OAuth),
si ha bisogno di cercare informazioni di autenticazioni solo una volta che l'utente ha raggiunto
un certo URL (p.e. l'URL di rinvio in OAuth).

Fortunatamente, la gestione di questa situazione è facile: basta verificare
l'URL corrente prima di creare il token in ``createToken()``::

    // src/Acme/HelloBundle/Security/ApiKeyAuthenticator.php

    // ...
    use Symfony\Component\Security\Http\HttpUtils;
    use Symfony\Component\HttpFoundation\Request;

    class ApiKeyAuthenticator implements SimplePreAuthenticatorInterface
    {
        protected $userProvider;

        protected $httpUtils;

        public function __construct(ApiKeyUserProviderInterface $userProvider, HttpUtils $httpUtils)
        {
            $this->userProvider = $userProvider;
            $this->httpUtils = $httpUtils;
        }

        public function createToken(Request $request, $providerKey)
        {
            // imposta l'unico URL in cui cercare informazioni di autenticazione
            // e restituisce il token solo se l'URL corrisponde
            $targetUrl = '/login/check';
            if (!$this->httpUtils->checkRequestPath($request, $targetUrl)) {
                return;
            }

            // ...
        }
    }

Viene usata la classe :class:`Symfony\\Component\\Security\\Http\\HttpUtils`
per verificare se l'URL corrente corrisponde all'URL cercato. In questo
caso, l'URL (``/login/check``) è stato scritto a mano nella classe, ma lo si può
anche iniettare come terzo parametro del costruttore.

Successivamente, basta aggiornare la configurazione del servizio, per iniettare il
servizio ``security.http_utils``:

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        services:
            # ...

            autenteicatore_apikey:
                class:     Acme\HelloBundle\Security\ApiKeyAuthenticator
                arguments: ["@fornitore_utenti_apikey", "@security.http_utils"]

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <?xml version="1.0" ?>
        <container xmlns="http://symfony.com/schema/dic/services"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://symfony.com/schema/dic/services
                http://symfony.com/schema/dic/services/services-1.0.xsd">
            <services>
                <!-- ... -->

                <service id="autenteicatore_apikey"
                    class="Acme\HelloBundle\Security\ApiKeyAuthenticator"
                >
                    <argument type="service" id="fornitore_utenti_apikey" />
                    <argument type="service" id="security.http_utils" />
                </service>
            </services>
        </container>

    .. code-block:: php

        // app/config/config.php
        use Symfony\Component\DependencyInjection\Definition;
        use Symfony\Component\DependencyInjection\Reference;

        // ...

        $container->setDefinition('autenteicatore_apikey', new Definition(
            'Acme\HelloBundle\Security\ApiKeyAuthenticator',
            array(
                new Reference('fornitore_utenti_apikey'),
                new Reference('security.http_utils')
            )
        ));

Ecco fatto! Buon divertimento!
