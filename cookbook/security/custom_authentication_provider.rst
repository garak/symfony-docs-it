.. index::
   single: Sicurezza; Fornitore di autenticazione personalizzato

Creare un fornitore di autenticazione personalizzato
====================================================

Chi ha letto il capitolo sulla :doc:`/book/security` può capire
la distinzione che fa Symfony tra autenticazione e autorizzazione,
nell'implementazione della sicurezza. Questo capitolo discute le classi
di base coinvolte nel processo di autenticazione e come implementare un
fornitore di autenticazione personalizzato. Poiché autenticazione e autorizzazione
sono concetti separati, questa estensione sarà agnostica rispetto al fornitore
di utenti e funzionerà con il fornitore di utenti dell'applicazione, sia
esso basato sulla memoria, su una base dati o su qualsiasi altro supporto scelto.

WSSE
----

Il seguente capitolo mostra come creare un fornitore di autenticazione
personalizzato per l'autenticazione WSSE. Questo protocollo di sicurezza per
WSSE fornisce diversi benefici:

#. Criptazione di nome utente e password
#. Protezione dagli attacchi di replay
#. Nessuna configurazione del server web necessaria

WSSE è molto utile per proteggere i servizi web, siano essi SOAP o
REST.

C'è molta buona documentazione su `WSSE`_, ma questo articolo non approfondirà
il protocollo di sicurezza, quanto il modo in cui un protocollo personalizzato
possa essere aggiunto a un'applicazione Symfony. La base di WSSE è la
verifica degli header di richiesta tramite credenziali criptate, con l'uso di
un timestamp e di `nonce`_, e la verifica dell'utente richiesto tramite un digest
di password.

.. note::

    WSSE supporta anche la validazione di chiavi dell'applicazione, che è utile per
    i servizi web, ma va oltre lo scopo di questo capitolo.

Il token
--------

Il ruolo del token nel contesto della sicurezza di Symfony è importante.
Un token rappresenta i dati di autenticazione dell'utente presenti nella richiesta.
Una volta autenticata la richiesta, il token mantiene i dati dell'utente e fornisce
tali dati attraverso il contesto della sicurezza. Prima di tutto, va creata una
classe per il token. Questo consentirà il passaggio di tutte le informazioni rilevanti
al fornitore di autenticazione.

.. code-block:: php

    // src/AppBundle/Security/Authentication/Token/WsseUserToken.php
    namespace AppBundle\Security\Authentication\Token;

    use Symfony\Component\Security\Core\Authentication\Token\AbstractToken;

    class WsseUserToken extends AbstractToken
    {
        public $created;
        public $digest;
        public $nonce;

        public function __construct(array $roles = array())
        {
            parent::__construct($roles);

            // Se l'utente ha dei ruoli, considerarlo autenticato
            $this->setAuthenticated(count($roles) > 0);
        }

        public function getCredentials()
        {
            return '';
        }
    }

.. note::

    La classe ``WsseUserToken`` estende la classe
    :class:`Symfony\\Component\\Security\\Core\\Authentication\\Token\\AbstractToken`
    del componente della sicurezza, la quale fornisce funzionalità di base per il token. Si può implementare
    :class:`Symfony\\Component\\Security\\Core\\Authentication\\Token\\TokenInterface` su
    una qualsiasi classe da usare come token.

L'ascoltatore
-------------

Ora occorre un ascoltatore, che ascolti nel contesto della sicurezza. L'ascoltatore è
responsabile delle richieste al firewall e di richiamare il fornitore di
autenticazione. Un ascoltatore deve essere un'istanza di
:class:`Symfony\\Component\\Security\\Http\\Firewall\\ListenerInterface`.
Un ascoltatore di sicurezza dovrebbe gestire l'evento
:class:`Symfony\\Component\\HttpKernel\\Event\\GetResponseEvent` e impostare un
token di autenticazione nel contesto della sicurezza, in caso positivo.

.. code-block:: php

    // src/AppBundle/Security/Firewall/WsseListener.php
    namespace AppBundle\Security\Firewall;

    use Symfony\Component\HttpFoundation\Response;
    use Symfony\Component\HttpKernel\Event\GetResponseEvent;
    use Symfony\Component\Security\Http\Firewall\ListenerInterface;
    use Symfony\Component\Security\Core\Exception\AuthenticationException;
    use Symfony\Component\Security\Core\SecurityContextInterface;
    use Symfony\Component\Security\Core\Authentication\AuthenticationManagerInterface;
    use AppBundle\Security\Authentication\Token\WsseUserToken;

    class WsseListener implements ListenerInterface
    {
        protected $securityContext;
        protected $authenticationManager;

        public function __construct(SecurityContextInterface $securityContext, AuthenticationManagerInterface $authenticationManager)
        {
            $this->securityContext = $securityContext;
            $this->authenticationManager = $authenticationManager;
        }

        public function handle(GetResponseEvent $event)
        {
            $request = $event->getRequest();

            $wsseRegex = '/UsernameToken Username="([^"]+)", PasswordDigest="([^"]+)", Nonce="([^"]+)", Created="([^"]+)"/';
            if (!$request->headers->has('x-wsse') || 1 !== preg_match($wsseRegex, $request->headers->get('x-wsse'), $matches)) {
                return;
            }

            $token = new WsseUserToken();
            $token->setUser($matches[1]);

            $token->digest   = $matches[2];
            $token->nonce    = $matches[3];
            $token->created  = $matches[4];

            try {
                $authToken = $this->authenticationManager->authenticate($token);
                $this->securityContext->setToken($authToken);

                return;
            } catch (AuthenticationException $failed) {
                // ... si potrebbe loggare qualcosa in questo punto

                // Per negare l'autenticazione, pulire il token. L'utente sarà rinviato alla pagina di login.
                // Assicurarsi di pulire solo il proprio token, non quello di altri ascoltatori di autenticazione.
                // $token = $this->securityContext->getToken();
                // if ($token instanceof WsseUserToken && $this->providerKey === $token->getProviderKey()) {
                //     $this->securityContext->setToken(null);
                // }
                // return;
            }

            // Negare autenticazione per impostazione predefinita
            $response = new Response();
            $response->setStatusCode(403);
            $event->setResponse($response);
        }
    }

Questo ascoltatore verifica che la richiesta contenga l'header `X-WSSE`, confronta il
valore restituito con l'informazione WSSE attesa, crea un token usando tale informazione
e passa il token al gestore di autenticazione. Se non viene fornita un'informazione
adeguata oppure se il gestore di autenticazione lancia una
:class:`Symfony\\Component\\Security\\Core\\Exception\\AuthenticationException`,
viene restituita una risposta 403.

.. note::

    Una classe non usata precedentemente, la classe
    :class:`Symfony\\Component\\Security\\Http\\Firewall\\AbstractAuthenticationListener`,
    è una classe base molto utile, che fornisce le funzionalità solitamente necessarie
    per le estensioni della sicurezza. Ciò include il mantenimento del token in sessione,
    fornire gestori di successo/fallimento, login da URL, eccetera. Poiché WSSE
    non richiede di mantenere sessioni di autenticazione né form di login, non sarà
    usata per questo esempio.

.. note::

    Il return prematuro nell'ascoltatore è rilevante solo se vogliono concatenare più
    fornitori di autenticazione (per esempio per consentire utenti anonimi). Se si vuole
    impedire l'accesso a utenti anonimi e avere un errore 403, si dovrebbe impostare
    il codice di stato della risposta prima del return.

Il fornitore di autenticazione
------------------------------

Il fornitore di autenticazione verificherà il token ``WsseUserToken``. Questo
vuol dire che il fornitore verificherà che il valore dell'header ``Created`` sia
valido entro cinque minuti, che il valore dell'header ``Nonce`` sia unico nei cinque
minuti e che il valore dell'header ``PasswordDigest`` corrisponda alla password dell'utente.

.. code-block:: php

    // src/AppBundle/Security/Authentication/Provider/WsseProvider.php
    namespace AppBundle\Security\Authentication\Provider;

    use Symfony\Component\Security\Core\Authentication\Provider\AuthenticationProviderInterface;
    use Symfony\Component\Security\Core\User\UserProviderInterface;
    use Symfony\Component\Security\Core\Exception\AuthenticationException;
    use Symfony\Component\Security\Core\Exception\NonceExpiredException;
    use Symfony\Component\Security\Core\Authentication\Token\TokenInterface;
    use AppBundle\Security\Authentication\Token\WsseUserToken;
    use Symfony\Component\Security\Core\Util\StringUtils;

    class WsseProvider implements AuthenticationProviderInterface
    {
        private $userProvider;
        private $cacheDir;

        public function __construct(UserProviderInterface $userProvider, $cacheDir)
        {
            $this->userProvider = $userProvider;
            $this->cacheDir     = $cacheDir;
        }

        public function authenticate(TokenInterface $token)
        {
            $user = $this->userProvider->loadUserByUsername($token->getUsername());

            if ($user && $this->validateDigest($token->digest, $token->nonce, $token->created, $user->getPassword())) {
                $authenticatedToken = new WsseUserToken($user->getRoles());
                $authenticatedToken->setUser($user);

                return $authenticatedToken;
            }

            throw new AuthenticationException('The WSSE authentication failed.');
        }

        /**
         * Questa funzione è specifica dell'autenticazione Wsse ed è usata solo per aiutare in questo esempio
         *
         * Per approfondire questa logica, vedere
         * https://github.com/symfony/symfony-docs/pull/3134#issuecomment-27699129
         */
        protected function validateDigest($digest, $nonce, $created, $secret)
        {
            // Verifica che il tempo di creazione non sia nel futuro
            if (strtotime($created) > time()) {
                return false;
            }

            // Scade dopo 5 minuti
            if (time() - strtotime($created) > 300) {
                return false;
            }

            // Valida che nonce *non* sia stato usato negli ultimi 5 minuti
            // se lo è stato, potrebbe trattarsi di un attacco
            if (file_exists($this->cacheDir.'/'.$nonce) && file_get_contents($this->cacheDir.'/'.$nonce) + 300 > time()) {
                throw new NonceExpiredException('Previously used nonce detected');
            }
            // Se la cartella della cache non esiste, va creata
            if (!is_dir($this->cacheDir)) {
                mkdir($this->cacheDir, 0777, true);
            }
            file_put_contents($this->cacheDir.'/'.$nonce, time());

            // Valida la parola segreta
            $expected = base64_encode(sha1(base64_decode($nonce).$created.$secret, true));

            return StringUtils::equals($expected, $digest);
        }

        public function supports(TokenInterface $token)
        {
            return $token instanceof WsseUserToken;
        }
    }

.. note::

    L'interfaccia :class:`Symfony\\Component\\Security\\Core\\Authentication\\Provider\\AuthenticationProviderInterface`
    richiede un metodo ``authenticate`` sul token dell'utente e un metodo ``supports``,
    che dice al gestore di autenticazione se usare o meno questo fornitore per il token
    dato. In caso di più fornitori, il gestore di autenticazione passerà al fornitore
    successivo della lista.

.. note::

    Il confronto tra digest atteso e fornito è eseguito a tempo costante,
    grazie al metodo
    :method:`Symfony\\Component\\Security\\Core\\Util\\StringUtils::equals`
    della classe ``StringUtils``. Questo serve a mitigare possibili attacchi di tipo
    `timing attack`_.

Il factory
----------

Abbiamo creato un token personalizzato, un ascoltatore personalizzato e un fornitore
personalizzato. Ora dobbiamo legarli insieme. Come rendere disponibile il fornitore
alla configurazione della sicurezza? La risposta è: usando un ``factory``. Un factory
è quando ci si aggancia al componente Security, dicendogli il nome del 
fornitore e qualsiasi opzione di configurazione disponibile per esso. Prima di tutto,
occorre creare una classe che implementi
:class:`Symfony\\Bundle\\SecurityBundle\\DependencyInjection\\Security\\Factory\\SecurityFactoryInterface`.

.. code-block:: php

    // src/AppBundle/DependencyInjection/Security/Factory/WsseFactory.php
    namespace AppBundle\DependencyInjection\Security\Factory;

    use Symfony\Component\DependencyInjection\ContainerBuilder;
    use Symfony\Component\DependencyInjection\Reference;
    use Symfony\Component\DependencyInjection\DefinitionDecorator;
    use Symfony\Component\Config\Definition\Builder\NodeDefinition;
    use Symfony\Bundle\SecurityBundle\DependencyInjection\Security\Factory\SecurityFactoryInterface;

    class WsseFactory implements SecurityFactoryInterface
    {
        public function create(ContainerBuilder $container, $id, $config, $userProvider, $defaultEntryPoint)
        {
            $providerId = 'security.authentication.provider.wsse.'.$id;
            $container
                ->setDefinition($providerId, new DefinitionDecorator('wsse.security.authentication.provider'))
                ->replaceArgument(0, new Reference($userProvider))
            ;

            $listenerId = 'security.authentication.listener.wsse.'.$id;
            $listener = $container->setDefinition($listenerId, new DefinitionDecorator('wsse.security.authentication.listener'));

            return array($providerId, $listenerId, $defaultEntryPoint);
        }

        public function getPosition()
        {
            return 'pre_auth';
        }

        public function getKey()
        {
            return 'wsse';
        }

        public function addConfiguration(NodeDefinition $node)
        {
        }
    }

L'interfaccia :class:`Symfony\\Bundle\\SecurityBundle\\DependencyInjection\\Security\\Factory\\SecurityFactoryInterface`
richiede i seguenti metodi:

``create``
    Metodo che aggiunge l'ascoltatore e il fornitore di autenticazione provider
    al contenitore di dipendenze per il contesto della sicurezza appropriato;

``getPosition``
    Metodo che deve essere del tipo ``pre_auth``, ``form``, ``http``
    o ``remember_me`` e definisce la posizione in cui il fornitore viene chiamato;

``getKey``
    Metodo che definisce la chiave di configurazione usata per fare riferimento
    al fornitore;

``addConfiguration``
    Metodo usato per definire le opzioni di configurazione
    sotto la chiave ``configuration`` della configurazione della sicurezza.
    Le opzioni di configurazione sono spiegate più avanti in questo capitolo.

.. note::

    Una classe non usata in questo esempio,
    :class:`Symfony\\Bundle\\SecurityBundle\\DependencyInjection\\Security\\Factory\\AbstractFactory`,
    è una classe base molto utile, che fornisce funzionalità solitamente necessaria per
    i factory della sicurezza. Può tornare utile quando si definisce un fornitore di
    autenticazione di tipo diverso.

Una volta creata la classe factory, la chiave ``wsse`` può essere usata con
firewall nella configurazione della sicurezza.

.. note::

    Ci si potrebbe chiedere il motivo per cui sia necessaria una speciale classe factory
    per aggiungere ascoltatori e fornitori al contenitore di dipendenze. È una buona
    domanda. La ragione è che si può usare un firewall più volte,
    per proteggere diverse parti di un'applicazione. Per questo, ogni volta che
    si usa un firewall, il contenitore di dipendenze crea un nuovo servizio.
    Il factory serve a creare questi nuovi servizi.

Configurazione
--------------

È tempo di vedere in azione il nuovo fornitore di autenticazione. Servono ancora alcune
cose per farlo funzionare. La prima cosa è aggiungere i servizi di cui sopra al
contenitore di servizi. La classe factory vista prima fa riferimento a degli id di
servizi che non esistono ancora: ``wsse.security.authentication.provider`` e
``wsse.security.authentication.listener``. È tempo di definire questi servizi.

.. configuration-block::

    .. code-block:: yaml

        # src/AppBundle/Resources/config/services.yml
        services:
            wsse.security.authentication.provider:
                class: AppBundle\Security\Authentication\Provider\WsseProvider
                arguments: ["", "%kernel.cache_dir%/security/nonces"]

            wsse.security.authentication.listener:
                class: AppBundle\Security\Firewall\WsseListener
                arguments: ["@security.context", "@security.authentication.manager"]

    .. code-block:: xml

        <!-- src/AppBundle/Resources/config/services.xml -->
        <container xmlns="http://symfony.com/schema/dic/services"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://symfony.com/schema/dic/services http://symfony.com/schema/dic/services/services-1.0.xsd">

            <services>
                <service id="wsse.security.authentication.provider"
                    class="AppBundle\Security\Authentication\Provider\WsseProvider" public="false">
                    <argument /> <!-- Fornitore di utenti -->
                    <argument>%kernel.cache_dir%/security/nonces</argument>
                </service>

                <service id="wsse.security.authentication.listener"
                    class="AppBundle\Security\Firewall\WsseListener" public="false">
                    <argument type="service" id="security.context"/>
                    <argument type="service" id="security.authentication.manager" />
                </service>
            </services>
        </container>

    .. code-block:: php

        // src/AppBundle/Resources/config/services.php
        use Symfony\Component\DependencyInjection\Definition;
        use Symfony\Component\DependencyInjection\Reference;

        $container->setDefinition('wsse.security.authentication.provider',
            new Definition(
                'AppBundle\Security\Authentication\Provider\WsseProvider', array(
                    '',
                    '%kernel.cache_dir%/security/nonces',
                )
            )
        );

        $container->setDefinition('wsse.security.authentication.listener',
            new Definition(
                'AppBundle\Security\Firewall\WsseListener', array(
                    new Reference('security.context'),
                    new Reference('security.authentication.manager'),
                )
            )
        );

Ora che i servizi sono stati definiti, diciamo al contesto della sicurezza del
factory. 

.. code-block:: php

    // src/AppBundle/AppBundle.php
    namespace AppBundle;

    use AppBundle\DependencyInjection\Security\Factory\WsseFactory;
    use Symfony\Component\HttpKernel\Bundle\Bundle;
    use Symfony\Component\DependencyInjection\ContainerBuilder;

    class AppBundle extends Bundle
    {
        public function build(ContainerBuilder $container)
        {
            parent::build($container);

            $extension = $container->getExtension('security');
            $extension->addSecurityListenerFactory(new WsseFactory());
        }
    }

Abbiamo finito! Ora si possono definire le parti dell'applicazione sotto protezione WSSE.

.. configuration-block::

    .. code-block:: yaml

        security:
            firewalls:
                wsse_secured:
                    pattern:   /api/.*
                    stateless: true
                    wsse:      true

    .. code-block:: xml

        <config>
            <firewall name="wsse_secured" pattern="/api/.*">
                <stateless />
                <wsse />
            </firewall>
        </config>

    .. code-block:: php

        $container->loadFromExtension('security', array(
            'firewalls' => array(
                'wsse_secured' => array(
                    'pattern' => '/api/.*',
                    'stateless'    => true,
                    'wsse'    => true,
                ),
            ),
        ));

Con questo abbiamo concluso la scrittura di un fornitore di autenticazione
personalizzato.

Un piccolo extra
----------------

E se si volesse rendere il fornitore di autenticazione WSSE un po' più eccitante?
Le possibilità sono infinite. Possiamo iniziare a renderlo ancora più
brillante.

Configurazione
~~~~~~~~~~~~~~

Si possono aggiungere opzioni personalizzate sotto la voce ``wsse`` nella configurazione
della sicurezza. Per esempio, il tempo consentito predefinito prima della scadenza
dell'header di creazione è di 5 minuti. Lo si può rendere configurabile, in modo che
firewall diversi possano avere lunghezze di scadenza diverse.

Occorre innanzitutto modificare ``WsseFactory`` e definire la nuova opzione nel metodo
``addConfiguration``.

.. code-block:: php

    class WsseFactory implements SecurityFactoryInterface
    {
        // ...

        public function addConfiguration(NodeDefinition $node)
        {
          $node
            ->children()
            ->scalarNode('lifetime')->defaultValue(300)
            ->end();
        }
    }

Ora, nel metodo ``create`` del factory, il parametro ``$config`` conterrà
una chiave 'lifetime', impostata a 5 minuti (300 secondi), a meno che non sia specificato
diversamente nella configurazione. Per usarlo, occorre passarlo come parametro al
fornitore di autenticazione.

.. code-block:: php

    class WsseFactory implements SecurityFactoryInterface
    {
        public function create(ContainerBuilder $container, $id, $config, $userProvider, $defaultEntryPoint)
        {
            $providerId = 'security.authentication.provider.wsse.'.$id;
            $container
                ->setDefinition($providerId,
                  new DefinitionDecorator('wsse.security.authentication.provider'))
                ->replaceArgument(0, new Reference($userProvider))
                ->replaceArgument(2, $config['lifetime']);
            // ...
        }

        // ...
    }

.. note::

    Occorre aggiungere anche un terzo parametro alla configurazione del servizio
    ``wsse.security.authentication.provider``, che potrebbe essere vuoto, oppure
    contenente il tempo di scadenza nel factory. La classe ``WsseProvider`` dovrà
    anche accettare un terzo parametro nel costruttore, il tempo, che dovrebbe usare
    al posto dei 300 secondi precedentemente fissati. Questi due passi non sono
    mostrati.

Il  tempo di scadenza di ogni richiesta WSSE è ora configurabile e può essere impostato
con qualsiasi valore desiderato per ogni firewall.

.. configuration-block::

    .. code-block:: yaml

        security:
            firewalls:
                wsse_secured:
                    pattern:   /api/.*
                    stateless: true
                    wsse:      { lifetime: 30 }

    .. code-block:: xml

        <config>
            <firewall name="wsse_secured"
                pattern="/api/.*"
            >
                <stateless />
                <wsse lifetime="30" />
            </firewall>
        </config>

    .. code-block:: php

        $container->loadFromExtension('security', array(
            'firewalls' => array(
                'wsse_secured' => array(
                    'pattern' => '/api/.*',
                    'stateless' => true,
                    'wsse'    => array(
                        'lifetime' => 30,
                    ),
                ),
            ),
        ));

Qualsiasi altra configurazione rilevante può essere definita nel factory e
utilizzata o passata a altre classi nel contenitore.

.. _`WSSE`: http://www.xml.com/pub/a/2003/12/17/dive.html
.. _`nonce`: http://it.wikipedia.org/wiki/Nonce
.. _`timing attack`: http://en.wikipedia.org/wiki/Timing_attack
