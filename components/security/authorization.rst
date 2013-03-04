.. index::
   single: Security, Autorizzazione

Autorizzazione
==============

Quando uno dei fornitori di autenticazione (vedere :ref:`authentication_providers`)
ha verificato il token non ancora autenticato, sarà restituito un token autenticato.
L'ascoltatore di autenticazione dovrebbe impostare questo token direttamente
in :class:`Symfony\\Component\\Security\\Core\\SecurityContextInterface`
usando il suo metodo :method:`Symfony\\Component\\Security\\Core\\SecurityContextInterface::setToken`.


Da quel momento, l'utente è autenticato, cioè identificato. Ora, altre parti
dell'applicazione possono usare il token per decidere se l'utente possa o meno
richiedere un certo URI o modificare un certo oggetto. Questa decisione sarà presa
da un'istanza di :class:`Symfony\\Component\\Security\\Core\\Authorization\\AccessDecisionManagerInterface`.

Una decisione di autorizzazione sarà sempre basata su alcuni aspetti:

* Il token corrente
    Per esempio il metodo :method:`Symfony\\Component\\Security\\Core\\Authentication\\Token\\TokenInterface::getRoles`
    del token può essere usato per recuperare i ruoli dell'utente attuale (p.e.
    ``ROLE_SUPER_ADMIN``) o una decisione potrebbe essere basata sulla classe del token.
* Un insieme di attributi
    Ogni attributo sta per un certo diritto che l'utente dovrebbe avere, p.e.
    ``ROLE_ADMIN`` per assicurarsi che l'utente sia un amministratore.
* Un oggetto (facoltativo)
    Un oggetto per cui il controllo di accesso necessiti di essere verificato, come
    un oggetto articolo o un oggetto commento.

Gestore delle decisioni di accesso
----------------------------------

Poiché decidere se un utente sia o meno autorizzato a eseguire una certa
azione può essere un processo complicato, :class:`Symfony\\Component\\Security\\Core\\Authorization\\AccessDecisionManager`
dipende da molti votanti ed emette un verdetto finale in base a tutti
i voti (siano essi positivi, negativi o neutrali) ricevuti. Riconosce
diverse strategie:

* ``affirmative`` (predefinita)
    garantisce l'accesso se uno dei votanti restituisce una risposta affermativa;

* ``consensus``
    garantisce l'accesso se ci sono più votanti affermativi che negativi;

* ``unanimous``
    garantisce l'accesso se nessuno dei votanti è negativo;

.. code-block:: php

    use Symfony\Component\Security\Core\Authorization\AccessDecisionManager;

    // instanze di Symfony\Component\Security\Core\Authorization\Voter\VoterInterface
    $voters = array(...);

    // uno tra "affirmative", "consensus", "unanimous"
    $strategy = ...;

    // se garantire o meno l'accesso quando tutti i votanti si astengono
    $allowIfAllAbstainDecisions = ...;

    // se garantire o meno l'accesso quando non c'è maggioranza (si applica solo alla strategia "consensus")
    $allowIfEqualGrantedDeniedDecisions = ...;

    $accessDecisionManager = new AccessDecisionManager(
        $voters,
        $strategy,
        $allowIfAllAbstainDecisions,
        $allowIfEqualGrantedDeniedDecisions
    );

Votanti
-------

I votanti sono istanze di
:class:`Symfony\\Component\\Security\\Core\\Authorization\\Voter\\VoterInterface`,
il che vuol dire che devono implementare alcuni metodi, che consentono al gestore di
decisioni di usarli:

* ``supportsAttribute($attributo)``
    usato per verificare se il votante sa come gestire il dato attributo;

* ``supportsClass($classe)``
    usato per verificare se il votante può garantire o negare accesso per
    un oggetto di una data classe;

* ``vote(TokenInterface $token, $object, array $attributi)``
    questo metodo eseguira l'effettiva votazione e restituirà un valore pari a una
    delle costanti di classe di :class:`Symfony\\Component\\Security\\Core\\Authorization\\Voter\\VoterInterface`,
    cioè ``VoterInterface::ACCESS_GRANTED``, ``VoterInterface::ACCESS_DENIED``
    o ``VoterInterface::ACCESS_ABSTAIN``;

Il componente Security contiene alcuni votanti standard, che coprono diversi casi
d'uso:

AuthenticatedVoter
~~~~~~~~~~~~~~~~~~

Il votante :class:`Symfony\\Component\\Security\\Core\\Authorization\\Voter\\AuthenticatedVoter`
supporta gli attriubti ``IS_AUTHENTICATED_FULLY``, ``IS_AUTHENTICATED_REMEMBERED``
e ``IS_AUTHENTICATED_ANONYMOUSLY`` e garantisce accesso in base all'attuale livello
di autenticazione, cioè se l'utente è pienamente autenticato o solo in base a
un cookie "ricordami", o ancora se è autenticato anonimamente.

.. code-block:: php

    use Symfony\Component\Security\Core\Authentication\AuthenticationTrustResolver;

    $anonymousClass = 'Symfony\Component\Security\Core\Authentication\Token\AnonymousToken';
    $rememberMeClass = 'Symfony\Component\Security\Core\Authentication\Token\RememberMeToken';

    $trustResolver = new AuthenticationTrustResolver($anonymousClass, $rememberMeClass);

    $authenticatedVoter = new AuthenticatedVoter($trustResolver);

    // istanza di Symfony\Component\Security\Core\Authentication\Token\TokenInterface
    $token = ...;

    // un qualsiasi oggetto
    $object = ...;

    $vote = $authenticatedVoter->vote($token, $object, array('IS_AUTHENTICATED_FULLY');

RoleVoter
~~~~~~~~~

Il votante :class:`Symfony\\Component\\Security\\Core\\Authorization\\Voter\\RoleVoter`
supporta attributi che iniziano con ``ROLE_`` e garantisce accesso all'utente
quando gli attributi ``ROLE_*`` richiesti possono essere trovati nell'array dei ruoli
restituiti dal metodo :method:`Symfony\\Component\\Security\\Core\\Authentication\\Token\\TokenInterface::getRoles`
del token::

    use Symfony\Component\Security\Core\Authorization\Voter\RoleVoter;

    $roleVoter = new RoleVoter('ROLE_');

    $roleVoter->vote($token, $object, 'ROLE_ADMIN');

RoleHierarchyVoter
~~~~~~~~~~~~~~~~~~

Il votante :class:`Symfony\\Component\\Security\\Core\\Authorization\\Voter\\RoleHierarchyVoter`
estende :class:`Symfony\\Component\\Security\\Core\\Authorization\\Voter\\RoleVoter`
e fornisce alcune funzionalità aggiuntive: sa come gestire una
gerarchia di ruoli. Per esempio un ruolo ``ROLE_SUPER_ADMIN`` potrebbe avere dei
sotto-ruoli ``ROLE_ADMIN`` e ``ROLE_USER``, in modo che se un certo oggetto richiede
all'utente di avere il ruolo ``ROLE_ADMIN``, sia garantito accesso agli utenti che in
effetti hanno il ruolo ``ROLE_ADMIN``, ma anche a quelli che hanno il ruolo
``ROLE_SUPER_ADMIN``::

    use Symfony\Component\Security\Core\Authorization\Voter\RoleHierarchyVoter;
    use Symfony\Component\Security\Core\Role\RoleHierarchy;

    $hierarchy = array(
        'ROLE_SUPER_ADMIN' => array('ROLE_ADMIN', 'ROLE_USER'),
    );

    $roleHierarchy = new RoleHierarchy($hierarchy);

    $roleHierarchyVoter = new RoleHierarchyVoter($roleHierarchy);

.. note::

    Quando si crea il proprio votante, ovviamente si può usare il suo costruttore
    per iniettare una dipendenza eventualmente necessaria per prendere una decisione.

Ruoli
-----

I ruoli sono oggetti che danno espressioni ad alcuni diritti posseduti dall'utente.
Il solo requisito è che implementino :class:`Symfony\\Component\\Security\\Core\\Role\\RoleInterface`,
il che vuol dire che devono avere un metodo :method:`Symfony\\Component\\Security\\Core\\Role\\Role\\RoleInterface::getRole`
che restituisca una stringa, rappresentazione del ruolo stesso. La classe predefinita
:class:`Symfony\\Component\\Security\\Core\\Role\\Role` restituisce semplicemente
il primo parametro del suo costruttore::

    use Symfony\Component\Security\Core\Role\Role;

    $role = new Role('ROLE_ADMIN');

    // mostra 'ROLE_ADMIN'
    echo $role->getRole();

.. note::

    La maggior parte dei token di autenticazione estendono :class:`Symfony\\Component\\Security\\Core\\Authentication\\Token\\AbstractToken`,
    che vuol dire che i ruoli forniti al suo costruttore saranno
    automaticamente convertiti da stringe a semplici oggetti ``Role``.

Usare il gestore di decisioni
-----------------------------

L'ascoltatore degli accessi
~~~~~~~~~~~~~~~~~~~~~~~~~~~

Il gestore di decisioni degli accessi può essere usato in qualsiasi punto di una richiesta
per decidere se l'utente sia o meno titolato per accedere a una data risorsa. Un metodo
facoltativo, ma utile, per restringere l'accesso in base a uno schema di URL è
:class:`Symfony\\Component\\Security\\Http\\Firewall\\AccessListener`,
che è uno degli ascoltatori del firewall (vedere :ref:`firewall_listeners`) che
è attivato per ogni richiesta corrispondente alla mappa dei firewall (vedere :ref:`firewall`).

Usa una mappa di accesso (che deve essere un'istanza di :class:`Symfony\\Component\\Security\\Http\\AccessMapInterface`),
la quale contiene gli schemi della richiesta e il corrispettivo insieme di attributi
richiesti all'utente per aver accesso all'applicazione::

    use Symfony\Component\Security\Http\AccessMap;
    use Symfony\Component\HttpFoundation\RequestMatcher;
    use Symfony\Component\Security\Http\Firewall\AccessListener;

    $accessMap = new AccessMap();
    $requestMatcher = new RequestMatcher('^/admin');
    $accessMap->add($requestMatcher, array('ROLE_ADMIN'));

    $accessListener = new AccessListener(
        $securityContext,
        $accessDecisionManager,
        $accessMap,
        $authenticationManager
    );

Contesto di sicurezza
~~~~~~~~~~~~~~~~~~~~~

Il gestore di decisioni degli accessi è disponibile anche in altre parti dell'applicazione,
tramite il metodo :method:`Symfony\\Component\\Security\\Core\\SecurityContext::isGranted`
di :class:`Symfony\\Component\\Security\\Core\\SecurityContext`.
Una chiamata a questo metodo delegherà la questione al gestore di decisioni degli
accessi::

    use Symfony\Component\Security\SecurityContext;
    use Symfony\Component\Security\Core\Exception\AccessDeniedException;

    $securityContext = new SecurityContext(
        $authenticationManager,
        $accessDecisionManager
    );

    if (!$securityContext->isGranted('ROLE_ADMIN')) {
        throw new AccessDeniedException();
    }
