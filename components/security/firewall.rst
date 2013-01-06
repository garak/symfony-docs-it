.. index::
   single: Security, Firewall

Il Firewall e il contesto di sicurezza
======================================

Il contesto di sicurezza è un concetto centrale nel componente Security: è un'istanza
di :class:`Symfony\\Component\\Security\\Core\\SecurityContextInterface`. Quando tutti i
passi nel processo di autenticazione dell'utente sono stati eseguiti con successo,
si può chiedere al contesto di sicurezza se l'utente autenticato ha accesso a una
determinata azione o risorsa dell'applicazione::

    use Symfony\Component\Security\SecurityContext;
    use Symfony\Component\Security\Core\Exception\AccessDeniedException;

    $securityContext = new SecurityContext();

    // ... autenticare l'utente

    if (!$securityContext->isGranted('ROLE_ADMIN')) {
        throw new AccessDeniedException();
    }

.. _firewall:

Un firewall per le richieste HTTP
---------------------------------

L'autenticazione dell'utente è eseguita dal firewall. Un'applicazione può avere
più aree protette, quindi il firewall viene configurato usando una mappa di tali
aree. Per ciascuna area, la mappa contiene l'indicazione di una richiesta e un
insieme di ascoltatori. L'indicazione della richiesta dà al firewall la possibilità
di associare la richiesta corrente a un data area protetta.
Quindi viene chiesto agli ascoltatori se la richiesta corrente possa essere usata per
autenticare l'utente::

    use Symfony\Component\Security\Http\FirewallMap;
    use Symfony\Component\HttpFoundation\RequestMatcher;
    use Symfony\Component\Security\Http\Firewall\ExceptionListener;

    $map = new FirewallMap();

    $requestMatcher = new RequestMatcher('^/secured-area/');

    // istanze di Symfony\Component\Security\Http\Firewall\ListenerInterface
    $listeners = array(...);

    $exceptionListener = new ExceptionListener(...);

    $map->add($requestMatcher, $listeners, $exceptionListener);

La mappa di firewall sarà fornita al firewall come primo parametro, insieme con il
distributore di eventi, che è usato da :class:`Symfony\\Component\\HttpKernel\\HttpKernel`::

    use Symfony\Component\Security\Http\Firewall;
    use Symfony\Component\HttpKernel\KernelEvents;

    // l'EventDispatcher usato da HttpKernel
    $dispatcher = ...;

    $firewall = new Firewall($map, $dispatcher);

    $dispatcher->addListener(KernelEvents::REQUEST, array($firewall, 'onKernelRequest');

Il firewall viene registrato per ascoltare l'evento ``kernel.request``, che sarà
distribuito da ``HttpKernel`` all'inizio di ogni richiesta che
esso processa. In questo modo, il firewall può impedire all'utente di proseguire
oltre quanto consentito.

.. _firewall_listeners:

Ascoltatori dei firewall
~~~~~~~~~~~~~~~~~~~~~~~~

Quando al firewall viene notificato l'evento ``kernel.request``, esso richiede
la mappa dei firewall, se la richiesta corrisponde a una delle aree protette. La prima
area protetta corrispondente alla richiesta restituirà un insieme di ascoltatori di
firewall corrispondenti (ciascuno dei quali implementa :class:`Symfony\\Component\\Security\\Http\\Firewall\\ListenerInterface`).
A questi ascoltatori sarà chiesto di gestire la richiesta corrente. Questo, di base, vuol
dire: trovare se la richiesta corrente contiene informazioni su come l'utente
possa essere autenticato (per esempio il listenere basic HTTP authentication
verifica se la richiesta ha un header chiamato ``PHP_AUTH_USER``).

Ascoltatore delle eccezioni
~~~~~~~~~~~~~~~~~~~~~~~~~~~

Se uno degli ascoltatori lancia una :class:`Symfony\\Component\\Security\\Core\\Exception\\AuthenticationException`,
l'ascoltatore delle eccezioni fornito durante l'aggiunta di aree protette alla
mappa dei firewall interverrà.

L'ascoltatore di eccezioni determina cosa accadrà successivamente, in base ai parametri
che riceve durante la sua creazione. Può iniziare la procedura di autenticazione,
magari chiedere all'utente di fornire nuovamente le sue credenziali (quando si è
autenticato tramite un cookie "ricordami") oppure trasformare l'eccezione 
in :class:`Symfony\\Component\\HttpKernel\\Exception\\AccessDeniedHttpException`,
che successivamente risulterà in una risposta "HTTP/1.1 403: Access Denied".

Punti di ingresso
~~~~~~~~~~~~~~~~~

Quando l'utente non è autenticato (p.e. quando il contesto di sicurezza non ha
ancora alcun token), il punto di ingresso del firewall sarà richiamato, per "iniziare"
il processo di autenticazione. Un punto di ingresso dovrebbe implementare
:class:`Symfony\\Component\\Security\\Http\\EntryPoint\\AuthenticationEntryPointInterface`,
che un unico metodo: :method:`Symfony\\Component\\Security\\Http\\EntryPoint\\AuthenticationEntryPointInterface::start`.
Questo metodo riceve l'oggetto :class:`Symfony\\Component\\HttpFoundation\\Request`
corrente e l'ascoltatore di eccezioni che è stato attivato.
Il metodo dovrebbe restituire un oggetto :class:`Symfony\\Component\\HttpFoundation\\Response`.
Potrebbe essere, per esempio, la pagina che contiene il form di login oppure, nel
caso di basic HTTP authentication, una risposta con un header ``WWW-Authenticate``,
che chiederà all'utente di fornire nome e password.

Flusso: firewall, autenticazione, autorizzazione
------------------------------------------------

Forse ora si può capire meglio come funziona il flusso del contesto di
sicurezza:

#. il firewall viene registrato come acoltatore sulla richiesta;
#. all'inizio della richiesta, il firewall controlla la mappa dei firewall
   per vedere se ce n'è uno attivo sull'URL;
#. se nella mappa viene trovato un firewall corrispondente all'URL, i suoi ascoltatori vengono notificati
#. ciascun ascoltatore verifica se la richiesta corrente contiene informazioni di autenticazione.
   Un ascoltatore può (a) autenticare un utente, (b) lanciare una
   ``AuthenticationException``, o (c) non far nulla (perché non ci sono
   informazioni di autenticazione nella richiesta);
#. una volta che l'utente è autenticato, si userà :doc:`/components/security/authorization`
   per negare l'accesso a determinate risorse.

Leggere la prossima sezione per saperne di più su :doc:`/components/security/authentication`
e :doc:`/components/security/authorization`.
