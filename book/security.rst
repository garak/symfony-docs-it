.. index::
   single: Sicurezza

Sicurezza
=========

La sicurezza è una procedura che avviene in due fasi, il cui obiettivo è quello
di impedire a un utente di accedere a una risorsa a cui non dovrebbe avere accesso.

Nella prima fase del processo, il sistema di sicurezza identifica chi è
l'utente, chiedendogli di presentare una sorta di identificazione.
Quest'ultima è chiamata **autenticazione** e significa che il sistema
sta cercando di scoprire chi sei.

Una volta che il sistema sa chi sei, il passo successivo è quello di determinare
se dovresti avere accesso a una determinata risorsa. Questa parte del
processo è chiamato **autorizzazione** e significa che il sistema
verifica se disponi dei privilegi per eseguire una certa azione.

.. image:: /images/book/security_authentication_authorization.png
   :align: center

Il modo migliore per imparare è quello di vedere un esempio, vediamolo subito.

.. note::

    Il `componente della sicurezza`_ di Symfony è disponibile come libreria PHP a sé stante,
    per l'utilizzo all'interno di qualsiasi progetto PHP.

Esempio di base: l'autenticazione HTTP
--------------------------------------

Il componente della sicurezza può essere configurato attraverso la configurazione dell'applicazione.
In realtà, per molte configurazioni standard di sicurezza basta solo usare la giusta
configurazione. La seguente configurazione dice a Symfony di proteggere qualunque URL
corrispondente a ``/admin/*`` e chiedere le credenziali all'utente  utilizzando l'autenticazione
base HTTP (cioè il classico vecchio box nome utente/password):

.. configuration-block::

    .. code-block:: yaml

        # app/config/security.yml
        security:
            firewalls:
                secured_area:
                    pattern:    ^/
                    anonymous: ~
                    http_basic:
                        realm: "Area demo protetta"

            access_control:
                - { path: ^/admin, roles: ROLE_ADMIN }

            providers:
                in_memory:
                    users:
                        ryan:  { password: ryanpass, roles: 'ROLE_USER' }
                        admin: { password: kitten, roles: 'ROLE_ADMIN' }

            encoders:
                Symfony\Component\Security\Core\User\User: plaintext

    .. code-block:: xml

        <!-- app/config/security.xml -->
        <srv:container xmlns="http://symfony.com/schema/dic/security"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xmlns:srv="http://symfony.com/schema/dic/services"
            xsi:schemaLocation="http://symfony.com/schema/dic/services http://symfony.com/schema/dic/services/services-1.0.xsd">

            <config>
                <firewall name="secured_area" pattern="^/">
                    <anonymous />
                    <http-basic realm="Area demo protetta" />
                </firewall>

                <access-control>
                    <rule path="^/admin" role="ROLE_ADMIN" />
                </access-control>

                <provider name="in_memory">
                    <user name="ryan" password="ryanpass" roles="ROLE_USER" />
                    <user name="admin" password="kitten" roles="ROLE_ADMIN" />
                </provider>

                <encoder class="Symfony\Component\Security\Core\User\User" algorithm="plaintext" />
            </config>
        </srv:container>

    .. code-block:: php

        // app/config/security.php
        $container->loadFromExtension('security', array(
            'firewalls' => array(
                'secured_area' => array(
                    'pattern' => '^/',
                    'anonymous' => array(),
                    'http_basic' => array(
                        'realm' => 'Area demo protetta',
                    ),
                ),
            ),
            'access_control' => array(
                array('path' => '^/admin', 'role' => 'ROLE_ADMIN'),
            ),
            'providers' => array(
                'in_memory' => array(
                    'users' => array(
                        'ryan' => array('password' => 'ryanpass', 'roles' => 'ROLE_USER'),
                        'admin' => array('password' => 'kitten', 'roles' => 'ROLE_ADMIN'),
                    ),
                ),
            ),
            'encoders' => array(
                'Symfony\Component\Security\Core\User\User' => 'plaintext',
            ),
        ));

.. tip::

    Una distribuzione standard di Symfony pone la configurazione di sicurezza
    in un file separato (ad esempio ``app/config/security.yml``). Se non si ha
    un file di sicurezza separato, è possibile inserire la configurazione direttamente
    nel file di configurazione principale (ad esempio ``app/config/config.yml``).

Il risultato finale di questa configurazione è un sistema di sicurezza pienamente funzionale,
simile al seguente:

* Ci sono due utenti nel sistema (``ryan`` e ``admin``);
* Gli utenti si autenticano tramite autenticazione HTTP;
* Qualsiasi URL corrispondente a ``/admin/*`` è protetto e solo l'utente ``admin``
  può accedervi;
* Tutti gli URL che *non* corrispondono ad ``/admin/*`` sono accessibili da tutti gli utenti (e
  all'utente non viene chiesto il login).

Di seguito si vedrà brevemente come funziona la sicurezza e come ogni parte della configurazione
entra in gioco.

Come funziona la sicurezza: autenticazione e autorizzazione
-----------------------------------------------------------

Il sistema di sicurezza di Symfony funziona determinando l'identità di un utente (autenticazione)
e poi controllando se l'utente deve avere accesso a una risorsa specifica
o URL.

Firewall (autenticazione)
~~~~~~~~~~~~~~~~~~~~~~~~~

Quando un utente effettua una richiesta a un URL che è protetta da un firewall, viene attivato
il sistema di sicurezza. Il compito del firewall è quello di determinare se
l'utente deve o non deve essere autenticato e se deve autenticarsi, rimandare una risposta
all'utente, avviando il processo di autenticazione.

Un firewall viene attivato quando l'URL di una richiesta in arrivo corrisponde
al valore ``pattern`` dell'espressione regolare del firewall configurato. In questo esempio, 
``pattern`` (``^/``) corrisponderà a *ogni* richiesta in arrivo. Il fatto che il
firewall venga attivato *non* significa tuttavia che venga visualizzato
il box di autenticazione con nome utente e password per ogni URL. Per esempio, qualunque utente
può accedere a ``/foo`` senza che venga richiesto di autenticarsi.

.. image:: /images/book/security_anonymous_user_access.png
   :align: center

Questo funziona in primo luogo perché il firewall consente *utenti anonimi*, attraverso
il parametro di configurazione ``anonymous``. In altre parole, il firewall non richiede
all'utente di fare immediatamente un'autenticazione. E poiché non è
necessario nessun ``ruolo`` speciale per accedere a ``/foo`` (sotto la sezione ``access_control``), la richiesta
può essere soddisfatta senza mai chiedere all'utente di autenticarsi.

Se si rimuove la chiave ``anonymous``, il firewall chiederà *sempre* 
l'autenticazione all'utente.

Controlli sull'accesso (autorizzazione)
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Se un utente richiede ``/admin/foo``, il processo ha un diverso comportamento.
Questo perché la sezione di configurazione ``access_control`` dice
che qualsiasi URL che corrispondono allo schema dell'espressione regolare ``^/admin`` (cioè ``/admin``
o qualunque URL del tipo ``/admin/*``) richiede il ruolo ``ROLE_ADMIN``. I ruoli
sono la base per la maggior parte delle autorizzazioni: un utente può accedere ``/admin/foo`` solo
se ha il ruolo ``ROLE_ADMIN``.

.. image:: /images/book/security_anonymous_user_denied_authorization.png
   :align: center

Come prima, quando l'utente effettua inizialmente la richiesta, il firewall non
chiede nessuna identificazione. Tuttavia, non appena il livello di controllo di accesso
nega l'accesso all'utente (perché l'utente anonimo non ha il ruolo
``ROLE_ADMIN``), il firewall entra in azione e avvia il processo di autenticazione.
Il processo di autenticazione dipende dal meccanismo di autenticazione in uso.
Per esempio, se si sta utilizzando il metodo di autenticazione tramite form di login,
l'utente verrà rinviato alla pagina di login. Se si utilizza l'autenticazione HTTP,
all'utente sarà inviata una risposta HTTP 401 e verrà visualizzato una finestra del browser
con nome utente e password.

Ora l'utente ha la possibilità di inviare le credenziali all'applicazione.
Se le credenziali sono valide, può essere riprovata la richiesta originale.

.. image:: /images/book/security_ryan_no_role_admin_access.png
   :align: center

In questo esempio, l'utente ``ryan`` viene autenticato con successo con il firewall.
Ma poiché ``ryan`` non ha il ruolo ``ROLE_ADMIN``, viene ancora negato
l'accesso a ``/admin/foo``. In definitiva, questo significa che l'utente vedrà un
qualche messaggio che indica che l'accesso è stato negato.

.. tip::

    Quando Symfony nega l'accesso all'utente, l'utente vedrà una schermata di errore e
    riceverà un codice di stato HTTP 403 (``Forbidden``). È possibile personalizzare la
    schermata di errore di accesso negato seguendo le istruzioni sulle
    :ref:`pagine di errore<cookbook-error-pages-by-status-code>` presenti nel ricettario
    per personalizzare la pagina di errore 403.

Infine, se l'utente ``admin`` richiede ``/admin/foo``, avviene un processo
simile, solo che adesso, dopo essere stato autenticato, il livello di controllo di accesso
lascerà passare la richiesta:

.. image:: /images/book/security_admin_role_access.png
   :align: center

Il flusso di richiesta quando un utente richiede una risorsa protetta è semplice,
ma incredibilmente flessibile. Come si vedrà in seguito, l'autenticazione può essere gestita
in molti modi, come un form di login, un certificato X.509, o da
un'autenticazione dell'utente tramite Twitter. Indipendentemente dal metodo di autenticazione,
il flusso di richiesta è sempre lo stesso:

#. Un utente accede a una risorsa protetta;
#. L'applicazione rinvia l'utente al form di login;
#. L'utente invia le proprie credenziali (ad esempio nome utente / password);
#. Il firewall autentica l'utente;
#. L'utente autenticato riprova la richiesta originale.

.. note::

    L'*esatto* processo in realtà dipende un po' da quale meccanismo di
    autenticazione si sta usando. Per esempio, quando si utilizza il form di login, l'utente
    invia le sue credenziali a un URL che elabora il form (ad esempio ``/login_check``)
    e poi viene rinviato all'URL originariamente richiesto (ad esempio ``/admin/foo``).
    Ma con l'autenticazione HTTP, l'utente invia le proprie credenziali direttamente
    all'URL originale (ad esempio ``/admin/foo``) e poi la pagina viene restituita
    all'utente nella stessa richiesta (cioè senza rinvio).
    
    Questo tipo di idiosincrasie non dovrebbe causare alcun problema, ma è
    bene tenerle a mente.

.. tip::

    Più avanti si imparerà che in Symfony2 *qualunque cosa* può essere protetto, tra cui
    controllori specifici, oggetti, o anche metodi PHP.

.. _book-security-form-login:

Utilizzo di un form di login tradizionale
-----------------------------------------

Finora, si è visto come proteggere l'applicazione con un firewall e
poi proteggere l'accesso a determinate aree tramite i ruoli. Utilizzando l'autenticazione HTTP,
si può sfruttare senza fatica il box nativo nome utente/password offerti da
tutti i browser. Tuttavia, Symfony supporta nativamente molti meccanismi di autenticazione.
Per i dettagli su ciascuno di essi, vedere il
:doc:`Riferimento sulla configurazione di sicurezza</reference/configuration/security>`.

In questa sezione, si potrà proseguire l'apprendimento, consentendo all'utente di autenticarsi
attraverso un tradizionale form di login HTML.

In primo luogo, abilitare il form di login sotto il firewall:

.. configuration-block::

    .. code-block:: yaml

        # app/config/security.yml
        security:
            firewalls:
                secured_area:
                    pattern:    ^/
                    anonymous: ~
                    form_login:
                        login_path:  /login
                        check_path:  /login_check

    .. code-block:: xml

        <!-- app/config/security.xml -->
        <srv:container xmlns="http://symfony.com/schema/dic/security"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xmlns:srv="http://symfony.com/schema/dic/services"
            xsi:schemaLocation="http://symfony.com/schema/dic/services http://symfony.com/schema/dic/services/services-1.0.xsd">

            <config>
                <firewall name="secured_area" pattern="^/">
                    <anonymous />
                    <form-login login_path="/login" check_path="/login_check" />
                </firewall>
            </config>
        </srv:container>

    .. code-block:: php

        // app/config/security.php
        $container->loadFromExtension('security', array(
            'firewalls' => array(
                'secured_area' => array(
                    'pattern' => '^/',
                    'anonymous' => array(),
                    'form_login' => array(
                        'login_path' => '/login',
                        'check_path' => '/login_check',
                    ),
                ),
            ),
        ));

.. tip::

    Se non è necessario personalizzare i valori ``login_path`` o ``check_path``
    (i valori usati qui sono i valori predefiniti), è possibile accorciare
    la configurazione:

    .. configuration-block::

        .. code-block:: yaml

            form_login: ~

        .. code-block:: xml

            <form-login />

        .. code-block:: php

            'form_login' => array(),

Ora, quando il sistema di sicurezza inizia il processo di autenticazione,
rinvierà l'utente al form di login (``/login`` per impostazione predefinita). Implementare
visivamente il form di login è compito dello sviluppatore. In primo luogo, bisogna creare due rotte: una che
visualizzerà il form di login (cioè ``/login``) e un'altra che gestirà
l'invio del form di login (ad esempio ``/login_check``):

.. configuration-block::

    .. code-block:: yaml

        # app/config/routing.yml
        login:
            pattern:   /login
            defaults:  { _controller: AcmeSecurityBundle:Security:login }
        login_check:
            pattern:   /login_check

    .. code-block:: xml

        <!-- app/config/routing.xml -->
        <?xml version="1.0" encoding="UTF-8" ?>

        <routes xmlns="http://symfony.com/schema/routing"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://symfony.com/schema/routing http://symfony.com/schema/routing/routing-1.0.xsd">

            <route id="login" pattern="/login">
                <default key="_controller">AcmeSecurityBundle:Security:login</default>
            </route>
            <route id="login_check" pattern="/login_check" />

        </routes>

    ..  code-block:: php

        // app/config/routing.php
        use Symfony\Component\Routing\RouteCollection;
        use Symfony\Component\Routing\Route;

        $collection = new RouteCollection();
        $collection->add('login', new Route('/login', array(
            '_controller' => 'AcmeDemoBundle:Security:login',
        )));
        $collection->add('login_check', new Route('/login_check', array()));

        return $collection;

.. note::

    *Non* è necessario implementare un controllore per l'URL ``/login_check``
    perché il firewall catturerà ed elaborerà qualunque form inviato
    a questo URL. È facoltativo, ma utile, creare una rotta, in modo che la si possa
    usare per generare l'URL di invio del form nel template del login.

Notare che il nome della rotta ``login`` non è importante. Quello che è importante
è che l'URL della rotta (``/login``) corrisponda al valore di configurazione ``login_path``,
in quanto è lì che il sistema di sicurezza rinvierà gli utenti che necessitano di
effettuare il login.

Successivamente, creare il controllore che visualizzerà il form di login:

.. code-block:: php

    // src/Acme/SecurityBundle/Controller/Main;
    namespace Acme\SecurityBundle\Controller;

    use Symfony\Bundle\FrameworkBundle\Controller\Controller;
    use Symfony\Component\Security\Core\SecurityContext;

    class SecurityController extends Controller
    {
        public function loginAction()
        {
            $request = $this->getRequest();
            $session = $request->getSession();

            // verifica di eventuali errori
            if ($request->attributes->has(SecurityContext::AUTHENTICATION_ERROR)) {
                $error = $request->attributes->get(SecurityContext::AUTHENTICATION_ERROR);
            } else {
                $error = $session->get(SecurityContext::AUTHENTICATION_ERROR);
                $session->remove(SecurityContext::AUTHENTICATION_ERROR);
            }

            return $this->render('AcmeSecurityBundle:Security:login.html.twig', array(
                // ultimo nome utente inserito
                'last_username' => $session->get(SecurityContext::LAST_USERNAME),
                'error'         => $error,
            ));
        }
    }

Non bisogna farsi confondere da questo controllore. Come si vedrà a momenti, quando
l'utente compila il form, il sistema di sicurezza lo gestisce automaticamente.
Se l'utente ha inviato un nome utente o una password non validi,
questo controllore legge l'errore di invio del form dal sistema di sicurezza, in modo che
possano essere visualizzati all'utente.

In altre parole, il vostro compito è quello di visualizzare il form di login e gli eventuali errori di login
che potrebbero essersi verificati, ma è il sistema di sicurezza stesso che si prende cura di verificare
il nome utente e la password inviati e di autenticare l'utente.

Infine, creare il template corrispondente:

.. configuration-block::

    .. code-block:: html+jinja

        {# src/Acme/SecurityBundle/Resources/views/Security/login.html.twig #}
        {% if error %}
            <div>{{ error.message }}</div>
        {% endif %}

        <form action="{{ path('login_check') }}" method="post">
            <label for="username">Username:</label>
            <input type="text" id="username" name="_username" value="{{ last_username }}" />

            <label for="password">Password:</label>
            <input type="password" id="password" name="_password" />

            {#
                Se si desidera controllare l'URL a cui l'utente viene rinviato in caso di successo (maggiori dettagli qui sotto)
                <input type="hidden" name="_target_path" value="/account" />
            #}

            <input type="submit" name="login" />
        </form>

    .. code-block:: html+php

        <?php // src/Acme/SecurityBundle/Resources/views/Security/login.html.php ?>
        <?php if ($error): ?>
            <div><?php echo $error->getMessage() ?></div>
        <?php endif; ?>

        <form action="<?php echo $view['router']->generate('login_check') ?>" method="post">
            <label for="username">Username:</label>
            <input type="text" id="username" name="_username" value="<?php echo $last_username ?>" />

            <label for="password">Password:</label>
            <input type="password" id="password" name="_password" />

            <!--
                Se si desidera controllare l'URL a cui l'utente viene rinviato in caso di successo (maggiori dettagli qui sotto)
                <input type="hidden" name="_target_path" value="/account" />
            -->

            <input type="submit" name="login" />
        </form>

.. tip::

    La variabile ``error`` passata nel template è un'istanza di
    :class:`Symfony\\Component\\Security\\Core\\Exception\\AuthenticationException`.
    Potrebbe contenere informazioni, anche sensibili, sull'errore
    di autenticazione: va quindi usata con cautela.

Il form ha pochi requisiti. In primo luogo, inviando il form a ``/login_check``
(tramite la rotta ``login_check``), il sistema di sicurezza intercetterà l'invio
del form e lo processerà automaticamente. In secondo luogo, il sistema
di sicurezza si aspetta che i campi inviati siano chiamati ``_username`` e ``_password``
(questi nomi di campi possono essere :ref:`configurati<reference-security-firewall-form-login>`).

E questo è tutto! Quando si invia il form, il sistema di sicurezza controllerà
automaticamente le credenziali dell'utente e autenticherà l'utente o
rimanderà l'utente al form di login, dove sono visualizzati gli errori.

Rivediamo l'intero processo:

#. L'utente prova ad accedere a una risorsa protetta;
#. Il firewall avvia il processo di autenticazione rinviando
   l'utente al form di login (``/login``);
#. La pagina ``/login`` rende il form di login, attraverso la rotta e il controllore
   creato in questo esempio;
#. L'utente invia il form di login ``/login_check``;
#. Il sistema di sicurezza intercetta la richiesta, verifica le credenziali inviate
   dall'utente, autentica l'utente se sono corrette e, se non lo sono,
   lo rinvia al form di login.

Per impostazione predefinita, se le credenziali inviate sono corrette, l'utente verrà rinviato
alla pagina originale che è stata richiesta  (ad esempio ``/admin/foo``). Se l'utente
originariamente è andato direttamente alla pagina di login, sarà rinviato alla pagina iniziale.
Questo comportamento può essere personalizzato, consentendo, ad esempio, di rinviare
l'utente a un URL specifico.

Per maggiori dettagli su questo e su come personalizzare in generale il processo di login con il form,
vedere :doc:`/cookbook/security/form_login`.

.. _book-security-common-pitfalls:

.. sidebar:: Come evitare gli errori più comuni

    Quando si imposta il proprio form di login, bisogna fare attenzione a non incorrere in alcuni errori comuni.

    **1. Creare le rotte giuste**

    In primo luogo, essere sicuri di aver definito correttamente le rotte 
    ``/login`` e ``/login_check`` e che corrispondano ai valori di configurazione
    ``login_path`` e ``check_path``. Un errore di configurazione qui può significare che si viene
    rinviati a una pagina 404 invece che nella pagina di login, o che inviando
    il form di login non succede nulla (continuando a vedere sempre il form
    di login).

    **2. Assicurarsi che la pagina di login non sia protetta**

    Inoltre, bisogna assicurarsi che la pagina di login *non* richieda nessun ruolo per essere
    visualizzata. Per esempio, la seguente configurazione, che richiede il
    ruolo ``ROLE_ADMIN`` per tutti gli URL (includendo l'URL ``/login``),
    causerà un loop di redirect:

    .. configuration-block::

        .. code-block:: yaml

            access_control:
                - { path: ^/, roles: ROLE_ADMIN }

        .. code-block:: xml

            <access-control>
                <rule path="^/" role="ROLE_ADMIN" />
            </access-control>

        .. code-block:: php

            'access_control' => array(
                array('path' => '^/', 'role' => 'ROLE_ADMIN'),
            ),

    Rimuovendo il controllo degli accessi sull'URL ``/login`` il problema si risolve:

    .. configuration-block::

        .. code-block:: yaml

            access_control:
                - { path: ^/login, roles: IS_AUTHENTICATED_ANONYMOUSLY }
                - { path: ^/, roles: ROLE_ADMIN }

        .. code-block:: xml

            <access-control>
                <rule path="^/login" role="IS_AUTHENTICATED_ANONYMOUSLY" />
                <rule path="^/" role="ROLE_ADMIN" />
            </access-control>

        .. code-block:: php

            'access_control' => array(
                array('path' => '^/login', 'role' => 'IS_AUTHENTICATED_ANONYMOUSLY'),
                array('path' => '^/', 'role' => 'ROLE_ADMIN'),
            ),

    Inoltre, se il firewall *non* consente utenti anonimi, sarà
    necessario creare un firewall speciale, che consenta agli utenti anonimi la pagina
    di login:

    .. configuration-block::

        .. code-block:: yaml

            firewalls:
                login_firewall:
                    pattern:    ^/login$
                    anonymous:  ~
                secured_area:
                    pattern:    ^/
                    form_login: ~

        .. code-block:: xml

            <firewall name="login_firewall" pattern="^/login$">
                <anonymous />
            </firewall>
            <firewall name="secured_area" pattern="^/">
                <form_login />
            </firewall>

        .. code-block:: php

            'firewalls' => array(
                'login_firewall' => array(
                    'pattern' => '^/login$',
                    'anonymous' => array(),
                ),
                'secured_area' => array(
                    'pattern' => '^/',
                    'form_login' => array(),
                ),
            ),

    **3. Assicurarsi che ``/login_check`` sia dietro al firewall**

    Quindi, assicurarsi che l'URL ``check_path`` (ad esempio ``/login_check``)
    sia dietro al firewall che si sta usando per il form di login (in questo esempio,
    l'unico firewall fa passare *tutti* gli URL, includendo ``/login_check``). Se
    ``/login_check`` non corrisponde a nessun firewall, si riceverà un ``Impossibile
    trovare il controllore per il percorso "/login_check"`` dell'eccezione.

    **4. Più firewall non condividono il contesto di sicurezza**

    Se si utilizzano più firewall e ci si autentica su un firewall,
    *non* si verrà autenticati automaticamente su qualsiasi altro firewall.
    Firewall diversi sono come diversi sistemi di sicurezza. Ecco perché,
    per la maggior parte delle applicazioni, avere un solo firewall è sufficiente.

Autorizzazione
--------------

Il primo passo per la sicurezza è sempre l'autenticazione: il processo di verificare
l'identità dell'utente. Con Symfony, l'autenticazione può essere fatta in qualunque modo, attraverso
un form di login, autenticazione HTTP o anche tramite Facebook.

Una volta che l'utente è stato autenticato, l'autorizzazione ha inizio. L'autorizzazione
fornisce un metodo standard e potente per decidere se un utente può accedere a una qualche risorsa
(un URL, un oggetto del modello, una chiamata a metodo, ...). Questo funziona tramite l'assegnazione
di specifici ruoli a ciascun utente e quindi richiedendo ruoli diversi per differenti risorse.

Il processo di autorizzazione ha due diversi lati:

#. L'utente ha un insieme specifico di ruoli;
#. Una risorsa richiede un ruolo specifico per poter accedervi.

In questa sezione, ci si concentrerà su come proteggere risorse diverse (ad esempio gli URL,
le chiamate a metodi, ecc) con ruoli diversi. Più avanti, si imparerà di più su come
i ruoli sono creati e assegnati agli utenti.

Protezione di specifici schemi di URL
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Il modo più semplice per proteggere parte dell'applicazione è quello di proteggere un intero
schema di URL. Si è già visto questo nel primo esempio di questo capitolo,
dove tutto ciò a cui corrisponde lo schema di espressione regolare  ``^/admin`` richiede
il ruolo ``ROLE_ADMIN``.

È possibile definire tanti schemi di URL quanti ne occorrono, ciascuno è un'espressione regolare.

.. configuration-block::

    .. code-block:: yaml

        # app/config/security.yml
        security:
            # ...
            access_control:
                - { path: ^/admin/users, roles: ROLE_SUPER_ADMIN }
                - { path: ^/admin, roles: ROLE_ADMIN }

    .. code-block:: xml

        <!-- app/config/security.xml -->
        <config>
            <!-- ... -->
            <rule path="^/admin/users" role="ROLE_SUPER_ADMIN" />
            <rule path="^/admin" role="ROLE_ADMIN" />
        </config>

    .. code-block:: php

        // app/config/security.php
        $container->loadFromExtension('security', array(
            // ...
            'access_control' => array(
                array('path' => '^/admin/users', 'role' => 'ROLE_SUPER_ADMIN'),
                array('path' => '^/admin', 'role' => 'ROLE_ADMIN'),
            ),
        ));

.. tip::

    Anteporre il percorso con il simbolo ``^`` assicura che corrispondano solo gli URL che *iniziano* con
    lo schema. Per esempio, un semplice percorso ``/admin`` (senza
    simbolo ``^``) corrisponderebbe correttamente a ``/admin/foo``, ma corrisponderebbe anche a URL
    come ``/foo/admin``.

Per ogni richiesta in arrivo, Symfony2 cerca di trovare una regola per il controllo dell'accesso
che corrisponde (la prima vince). Se l'utente non è ancora autenticato, viene avviato
il processo di autenticazione (cioè viene data all'utente la possibilità di fare login). Tuttavia,
*se* l'utente è autenticato ma non ha il ruolo richiesto, viene lanciata
un'eccezione :class:`Symfony\\Component\\Security\\Core\\Exception\\AccessDeniedException`,
che è possibile gestire e trasformare in una simpatica pagina di errore "accesso negato"
per l'utente. Vedere :doc:`/cookbook/controller/error_pages` per
maggiori informazioni.

Poiché Symfony utilizza la prima regola di controllo accesso trovata, un URL del tipo ``/admin/users/new``
corrisponderà alla prima regola e richiederà solo il ruolo ``ROLE_SUPER_ADMIN``.
Qualunque URL tipo ``/admin/blog`` corrisponderà alla seconda regola e richiederà ``ROLE_ADMIN``.

.. _book-security-securing-ip:

Protezione tramite IP
~~~~~~~~~~~~~~~~~~~~~

In certe situazioni può succedere di limitare l'accesso a una data
rotta basata su IP. Questo è particolarmente rilevante nel caso di :ref:`Edge Side Includes<edge-side-includes>`
(ESI), per esempio, che utilizzano una rotta chiamata "_internal". Quando
viene utilizzato ESI, è richiesta la rotta interna dal gateway della cache per abilitare
diverse opzioni di cache per le sottosezioni all'interno di una determinata pagina. Queste rotte
fornite con il prefisso ^/_internal per impostazione predefinita nell'edizione standard di Symfony (assumendo
di aver scommentato queste linee dal file delle rotte).

Ecco un esempio di come si possa garantire questa rotta da intrusioni esterne:

.. configuration-block::

    .. code-block:: yaml

        # app/config/security.yml
        security:
            # ...
            access_control:
                - { path: ^/_internal, roles: IS_AUTHENTICATED_ANONYMOUSLY, ip: 127.0.0.1 }

    .. code-block:: xml

            <access-control>
                <rule path="^/_internal" role="IS_AUTHENTICATED_ANONYMOUSLY" ip="127.0.0.1" />
            </access-control>

    .. code-block:: php

            'access_control' => array(
                array('path' => '^/_internal', 'role' => 'IS_AUTHENTICATED_ANONYMOUSLY', 'ip' => '127.0.0.1'),
            ),

.. _book-security-securing-channel:

Protezione tramite canale
~~~~~~~~~~~~~~~~~~~~~~~~~

Molto simile alla sicurezza basata su IP, richiedere l'uso di SSL è semplice, basta
aggiungere la voce ``access_control``:

.. configuration-block::

    .. code-block:: yaml

        # app/config/security.yml
        security:
            # ...
            access_control:
                - { path: ^/cart/checkout, roles: IS_AUTHENTICATED_ANONYMOUSLY, requires_channel: https }

    .. code-block:: xml

            <access-control>
                <rule path="^/cart/checkout" role="IS_AUTHENTICATED_ANONYMOUSLY" requires_channel="https" />
            </access-control>

    .. code-block:: php

            'access_control' => array(
                array('path' => '^/cart/checkout', 'role' => 'IS_AUTHENTICATED_ANONYMOUSLY', 'requires_channel' => 'https'),
            ),
          
.. _book-security-securing-controller:

Proteggere un controllore
~~~~~~~~~~~~~~~~~~~~~~~~~

Proteggere l'applicazione basandosi su schemi di URL è semplice, ma in
alcuni casi può non essere abbastanza granulare. Quando necessario, si può facilmente forzare
l'autorizzazione dall'interno di un controllore:

.. code-block:: php

    use Symfony\Component\Security\Core\Exception\AccessDeniedException;
    // ...

    public function helloAction($name)
    {
        if (false === $this->get('security.context')->isGranted('ROLE_ADMIN')) {
            throw new AccessDeniedException();
        }

        // ...
    }

.. _book-security-securing-controller-annotations:

È anche possibile scegliere di installare e utilizzare l'opzionale ``JMSSecurityExtraBundle``,
che può proteggere il controllore utilizzando le annotazioni:

.. code-block:: php

    use JMS\SecurityExtraBundle\Annotation\Secure;

    /**
     * @Secure(roles="ROLE_ADMIN")
     */
    public function helloAction($name)
    {
        // ...
    }

Per maggiori informazioni, vedere la documentazione di `JMSSecurityExtraBundle`_. Se si sta
utilizzando la distribuzione standard di Symfony, questo bundle è disponibile per impostazione predefinita.
In caso contrario, si può facilmente scaricare e installare.

Protezione degli altri servizi
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

In realtà, con Symfony si può proteggere qualunque cosa, utilizzando una strategia simile a
quella vista nella sezione precedente. Per esempio, si supponga di avere un servizio
(ovvero una classe PHP) il cui compito è quello di inviare email da un utente all'altro.
È possibile limitare l'uso di questa classe, non importa dove è stata utilizzata,
per gli utenti che hanno un ruolo specifico.

Per ulteriori informazioni su come utilizzare il componente della sicurezza per proteggere
servizi e metodi diversi nell'applicazione, vedere :doc:`/cookbook/security/securing_services`.

Access Control List (ACL): protezione dei singoli oggetti della base dati
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Si immagini di progettare un sistema di blog, in cui gli utenti possono commentare i
messaggi. Si vuole che un utente possa modificare i propri commenti, ma non
quelli degli altri. Inoltre, come utente admin, si vuole essere in grado
di modificare *tutti* i commenti.

Il componente della sicurezza viene fornito con un sistema opzionale di access control list (ACL), 
che è possibile utilizzare quando è necessario controllare l'accesso alle singole istanze
di un oggetto nel sistema. *Senza* ACL, è possibile proteggere il sistema in modo che
solo certi utenti possono modificare i commenti sui blog. Ma *con* ACL,
si può limitare o consentire l'accesso commento per commento.

Per maggiori informazioni, vedere l'articolo del ricettario: :doc:`/cookbook/security/acl`.

Utenti
------

Nelle sezioni precedenti, si è appreso come sia possibile proteggere diverse risorse,
richiedendo una serie di *ruoli* per una risorsa. In questa sezione, esploreremo
l'altro lato delle autorizzazioni: gli utenti.

Da dove provengono gli utenti? (*User Provider*)
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Durante l'autenticazione, l'utente invia un insieme di credenziali (di solito un nome utente
e una password). Il compito del sistema di autenticazione è quello di soddisfare queste credenziali 
con l'insieme degli utenti. Quindi da dove proviene questa lista di utenti?

In Symfony2, gli utenti possono arrivare da qualsiasi parte: un file di configurazione, una tabella
di una base dati, un servizio web o qualsiasi altra cosa si può pensare. Qualsiasi cosa che prevede
uno o più utenti nel sistema di autenticazione è noto come "fornitore di utenti".
Symfony2 viene fornito con i due fornitori utenti più diffusi; uno che
carica gli utenti da un file di configurazione e uno che carica gli utenti da una tabella
di una base dati.

Definizione degli utenti in un file di configurazione
.....................................................

Il modo più semplice per specificare gli utenti è direttamente in un file di configurazione.
In effetti, questo si è già aver visto nell'esempio di questo capitolo.

.. configuration-block::

    .. code-block:: yaml

        # app/config/security.yml
        security:
            # ...
            providers:
                default_provider:
                    users:
                        ryan:  { password: ryanpass, roles: 'ROLE_USER' }
                        admin: { password: kitten, roles: 'ROLE_ADMIN' }

    .. code-block:: xml

        <!-- app/config/security.xml -->
        <config>
            <!-- ... -->
            <provider name="default_provider">
                <user name="ryan" password="ryanpass" roles="ROLE_USER" />
                <user name="admin" password="kitten" roles="ROLE_ADMIN" />
            </provider>
        </config>

    .. code-block:: php

        // app/config/security.php
        $container->loadFromExtension('security', array(
            // ...
            'providers' => array(
                'default_provider' => array(
                    'users' => array(
                        'ryan' => array('password' => 'ryanpass', 'roles' => 'ROLE_USER'),
                        'admin' => array('password' => 'kitten', 'roles' => 'ROLE_ADMIN'),
                    ),
                ),
            ),
        ));

Questo fornitore utenti è chiamato "in-memory" , dal momento che gli utenti
non sono memorizzati in una base dati. L'oggetto utente effettivo è fornito
da Symfony (:class:`Symfony\\Component\\Security\\Core\\User\\User`).

.. tip::
    Qualsiasi fornitore utenti può caricare gli utenti direttamente dalla configurazione, specificando    
    il parametro di configurazione ``users`` ed elencando gli utenti sotto di esso.

.. caution::

    Se il nome utente è completamente numerico (ad esempio ``77``) o contiene un trattino
    (ad esempio ``user-name``), è consigliabile utilizzare la seguente sintassi alternativa quando si specificano
    utenti in YAML:

    .. code-block:: yaml

        users:
            - { name: 77, password: pass, roles: 'ROLE_USER' }
            - { name: user-name, password: pass, roles: 'ROLE_USER' }

Per i siti più piccoli, questo metodo è semplice e veloce da configurare. Per sistemi più
complessi, si consiglia di caricare gli utenti dalla base dati.

.. _book-security-user-entity:

Caricare gli utenti da una base dati 
....................................

Se si vuole caricare gli utenti tramite l'ORM Doctrine, si può farlo facilmente
attraverso la creazione di una classe ``User`` e configurando il fornitore ``entity``.

.. tip:

    È disponibile un bundle open source di alta qualità che consente agli utenti
    di essere memorizzati tramite l'ORM o l'ODM Doctrine. Si trovano maggiori informazioni in `FOSUserBundle`_
    su GitHub.

Con questo approccio, bisogna prima creare la propria classe ``User``, che
sarà memorizzata nella base dati.

.. code-block:: php

    // src/Acme/UserBundle/Entity/User.php
    namespace Acme\UserBundle\Entity;

    use Symfony\Component\Security\Core\User\UserInterface;
    use Doctrine\ORM\Mapping as ORM;

    /**
     * @ORM\Entity
     */
    class User implements UserInterface
    {
        /**
         * @ORM\Column(type="string", length="255")
         */
        protected $username;

        // ...
    }

Per come è stato pensato il sistema di sicurezza, l'unico requisito per
la classe utente personalizzata è che implementi l'interfaccia :class:`Symfony\\Component\\Security\\Core\\User\\UserInterface`.
Questo significa che il concetto di "utente" può essere qualsiasi cosa, purché
implementi questa interfaccia.

.. note::

     L'oggetto utente verrà serializzato e salvato nella sessione durante le richieste,
     quindi si consiglia di `implementare l'interfaccia \Serializable`_
     nel proprio oggetto utente. Ciò è particolarmente importante se la classe ``User``
     ha una classe genitore con proprietà private.

Quindi, configurare un fornitore utenti ``entity`` e farlo puntare alla classe
``User``:

.. configuration-block::

    .. code-block:: yaml

        # app/config/security.yml
        security:
            providers:
                main:
                    entity: { class: Acme\UserBundle\Entity\User, property: username }

    .. code-block:: xml

        <!-- app/config/security.xml -->
        <config>
            <provider name="main">
                <entity class="Acme\UserBundle\Entity\User" property="username" />
            </provider>
        </config>

    .. code-block:: php

        // app/config/security.php
        $container->loadFromExtension('security', array(
            'providers' => array(
                'main' => array(
                    'entity' => array('class' => 'Acme\UserBundle\Entity\User', 'property' => 'username'),
                ),
            ),
        ));

Con l'introduzione di questo nuovo fornitore, il sistema di autenticazione
tenterà di caricare un oggetto ``User`` dalla base dati, utilizzando il campo
``username`` di questa classe.

.. note::
    Questo esempio ha come unico scopo quello di mostrare l'idea di base dietro al fornitore
    ``entity``. Per un esempio completamente funzionante, vedere :doc:`/cookbook/security/entity_provider`.

Per ulteriori informazioni sulla creazione di un proprio fornitore personalizzato (ad esempio se è necessario
caricare gli utenti tramite un servizio web), vedere :doc:`/cookbook/security/custom_provider`.

.. _book-security-encoding-user-password:

Codificare la password dell'utente
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Finora, per semplicità, tutti gli esempi hanno memorizzato le password dell'utente
in formato testo (se tali utenti sono memorizzati in un file di configurazione o in
una qualche base dati). Naturalmente, in un'applicazione reale, si consiglia per ragioni
di sicurezza di codificare le password degli utenti. Questo è facilmente realizzabile,
mappando la classe User in uno dei numerosi "encoder" disponibili. Per esempio,
per memorizzare gli utenti in memoria, ma oscurare le loro password tramite ``sha1``,
fare come segue:

.. configuration-block::

    .. code-block:: yaml

        # app/config/security.yml
        security:
            # ...
            providers:
                in_memory:
                    users:
                        ryan:  { password: bb87a29949f3a1ee0559f8a57357487151281386, roles: 'ROLE_USER' }
                        admin: { password: 74913f5cd5f61ec0bcfdb775414c2fb3d161b620, roles: 'ROLE_ADMIN' }

            encoders:
                Symfony\Component\Security\Core\User\User:
                    algorithm:   sha1
                    iterations: 1
                    encode_as_base64: false

    .. code-block:: xml

        <!-- app/config/security.xml -->
        <config>
            <!-- ... -->
            <provider name="in_memory">
                <user name="ryan" password="bb87a29949f3a1ee0559f8a57357487151281386" roles="ROLE_USER" />
                <user name="admin" password="74913f5cd5f61ec0bcfdb775414c2fb3d161b620" roles="ROLE_ADMIN" />
            </provider>

            <encoder class="Symfony\Component\Security\Core\User\User" algorithm="sha1" iterations="1" encode_as_base64="false" />
        </config>

    .. code-block:: php

        // app/config/security.php
        $container->loadFromExtension('security', array(
            // ...
            'providers' => array(
                'in_memory' => array(
                    'users' => array(
                        'ryan' => array('password' => 'bb87a29949f3a1ee0559f8a57357487151281386', 'roles' => 'ROLE_USER'),
                        'admin' => array('password' => '74913f5cd5f61ec0bcfdb775414c2fb3d161b620', 'roles' => 'ROLE_ADMIN'),
                    ),
                ),
            ),
            'encoders' => array(
                'Symfony\Component\Security\Core\User\User' => array(
                    'algorithm'         => 'sha1',
                    'iterations'        => 1,
                    'encode_as_base64'  => false,
                ),
            ),
        ));

Impostando ``iterations`` a ``1`` ed ``encode_as_base64`` a ``false``,
è sufficiente eseguire una sola volta l'algoritmo ``sha1`` sulla password e senza
alcuna codifica supplementare. È ora possibile calcolare l'hash della password a livello di codice
(ad esempio ``hash('sha1', 'ryanpass')``) o tramite qualche strumento online come `functions-online.com`_

Se si creano gli utenti in modo dinamico (e si memorizzano in una base dati),
è possibile utilizzare algoritmi di hash ancora più complessi e poi contare su un oggetto
encoder, per codificare le password. Per esempio, supponiamo che l'oggetto
User sia ``Acme\UserBundle\Entity\User`` (come nell'esempio precedente). In primo luogo,
configurare l'encoder per User:

.. configuration-block::

    .. code-block:: yaml

        # app/config/security.yml
        security:
            # ...

            encoders:
                Acme\UserBundle\Entity\User: sha512

    .. code-block:: xml

        <!-- app/config/security.xml -->
        <config>
            <!-- ... -->

            <encoder class="Acme\UserBundle\Entity\User" algorithm="sha512" />
        </config>

    .. code-block:: php

        // app/config/security.php
        $container->loadFromExtension('security', array(
            // ...

            'encoders' => array(
                'Acme\UserBundle\Entity\User' => 'sha512',
            ),
        ));

In questo caso, si utilizza il più forte algoritmo ``sha512``. Inoltre, poiché
si è semplicemente specificato l'algoritmo (``sha512``) come stringa, il sistema
per impostazione predefinita farà l'hash 5000 volte in un riga e poi la codificherà
in base64. In altre parole, la password è stata notevolmente offuscata, in modo
che l'hash della password non possa essere decodificato (cioè non è possibile determinare la password
partendo dal suo hash).

Se si ha un form di registrazione per gli utenti, è necessario essere in grado
di determinare la password con hash, in modo che sia possibile impostarla per l'utente.
Indipendentemente dall'algoritmo configurato per l'oggetto User, la password con hash
può essere determinata nel seguente modo, da dentro un controllore:

.. code-block:: php

    $factory = $this->get('security.encoder_factory');
    $user = new Acme\UserBundle\Entity\User();

    $encoder = $factory->getEncoder($user);
    $password = $encoder->encodePassword('ryanpass', $user->getSalt());
    $user->setPassword($password);

Recuperare l'oggetto User
~~~~~~~~~~~~~~~~~~~~~~~~~

Dopo l'autenticazione, si può accedere all'oggetto ``User`` per l 'utente corrente
tramite il servizio ``security.context``. Da dentro un controllore, assomiglierà
a questo:

.. code-block:: php

    public function indexAction()
    {
        $user = $this->get('security.context')->getToken()->getUser();
    }

.. note::

    Gli utenti anonimi sono tecnicamente autenticati, nel senso che il metodo
    ``isAuthenticated()`` dell'oggetto di un utente anonimo restituirà ``true``. Per controllare se 
    l'utente sia effettivamente autenticato, verificare il ruolo 
    ``IS_AUTHENTICATED_FULLY``.

In un template Twig, si può accedere a questo oggetto tramite la chiave ``app.user``,
che richiama il metodo
:method:`GlobalVariables::getUser()<Symfony\\Bundle\\FrameworkBundle\\Templating\\GlobalVariables::getUser>`:

.. configuration-block::

    .. code-block:: html+jinja

        <p>Nome utente: {{ app.user.username }}</p>


Utilizzare fornitori utenti multipli
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Ogni meccanismo di autenticazione (ad esempio l'autenticazione HTTP, il form di login, ecc.)
utilizza esattamente un fornitore utenti e, per impostazione predefinita, userà il primo fornitore
dichiarato. Ma cosa succede se si desidera specificare alcuni utenti tramite configurazione
e il resto degli utenti nella base dati? Questo è possibile attraverso la creazione di
un nuovo fornitore, che li unisca:

.. configuration-block::

    .. code-block:: yaml

        # app/config/security.yml
        security:
            providers:
                chain_provider:
                    providers: [in_memory, user_db]
                in_memory:
                    users:
                        foo: { password: test }
                user_db:
                    entity: { class: Acme\UserBundle\Entity\User, property: username }

    .. code-block:: xml

        <!-- app/config/security.xml -->
        <config>
            <provider name="chain_provider">
                <provider>in_memory</provider>
                <provider>user_db</provider>
            </provider>
            <provider name="in_memory">
                <user name="foo" password="test" />
            </provider>
            <provider name="user_db">
                <entity class="Acme\UserBundle\Entity\User" property="username" />
            </provider>
        </config>

    .. code-block:: php

        // app/config/security.php
        $container->loadFromExtension('security', array(
            'providers' => array(
                'chain_provider' => array(
                    'providers' => array('in_memory', 'user_db'),
                ),
                'in_memory' => array(
                    'users' => array(
                        'foo' => array('password' => 'test'),
                    ),
                ),
                'user_db' => array(
                    'entity' => array('class' => 'Acme\UserBundle\Entity\User', 'property' => 'username'),
                ),
            ),
        ));

Ora, tutti i meccanismi di autenticazione utilizzeranno il ``chain_provider``, dal momento che
è il primo specificato. Il ``chain_provider``, a sua volta, tenta di caricare
l'utente da entrambi i fornitori ``in_memory`` e ``user_db``.

.. tip::

    Se non ci sono ragioni per separare gli utenti ``in_memory`` dagli
    utenti ``user_db``, è possibile ottenere ancora più facilmente questo risultato, combinando
    le due sorgenti in un unico fornitore:

    .. configuration-block::

        .. code-block:: yaml

            # app/config/security.yml
            security:
                providers:
                    main_provider:
                        users:
                            foo: { password: test }
                        entity: { class: Acme\UserBundle\Entity\User, property: username }

        .. code-block:: xml

            <!-- app/config/security.xml -->
            <config>
                <provider name=="main_provider">
                    <user name="foo" password="test" />
                    <entity class="Acme\UserBundle\Entity\User" property="username" />
                </provider>
            </config>

        .. code-block:: php

            // app/config/security.php
            $container->loadFromExtension('security', array(
                'providers' => array(
                    'main_provider' => array(
                        'users' => array(
                            'foo' => array('password' => 'test'),
                        ),
                        'entity' => array('class' => 'Acme\UserBundle\Entity\User', 'property' => 'username'),
                    ),
                ),
            ));

È anche possibile configurare il firewall o meccanismi di autenticazione individuali
per utilizzare un provider specifico. Ancora una volta, a meno che un provider sia specificato esplicitamente,
viene sempre utilizzato il primo fornitore:

.. configuration-block::

    .. code-block:: yaml

        # app/config/security.yml
        security:
            firewalls:
                secured_area:
                    # ...
                    provider: user_db
                    http_basic:
                        realm: "Demo area sicura"
                        provider: in_memory
                    form_login: ~

    .. code-block:: xml

        <!-- app/config/security.xml -->
        <config>
            <firewall name="secured_area" pattern="^/" provider="user_db">
                <!-- ... -->
                <http-basic realm="Demo area sicura" provider="in_memory" />
                <form-login />
            </firewall>
        </config>

    .. code-block:: php

        // app/config/security.php
        $container->loadFromExtension('security', array(
            'firewalls' => array(
                'secured_area' => array(
                    // ...
                    'provider' => 'user_db',
                    'http_basic' => array(
                        // ...
                        'provider' => 'in_memory',
                    ),
                    'form_login' => array(),
                ),
            ),
        ));

In questo esempio, se un utente cerca di accedere tramite autenticazione HTTP, il sistema di
autenticazione utilizzerà il fornitore utenti ``in_memory``. Ma se l'utente tenta di
accedere tramite il form di login, sarà usato il fornitore ``user_db`` (in quanto
è l'impostazione predefinita per il firewall).

Per ulteriori informazioni su fornitori utenti e configurazione del firewall, vedere
il :doc:`/reference/configuration/security`.

Ruoli
-----

L'idea di un "ruolo" è la chiave per il processo di autorizzazione. A ogni utente viene assegnato
un insieme di ruoli e quindi ogni risorsa richiede uno o più ruoli. Se l'utente
ha i ruoli richiesti, l'accesso è concesso. In caso contrario, l'accesso è negato.

I ruoli sono abbastanza semplici e sono fondamentalmente stringhe che si possono inventare e
utilizzare secondo necessità (anche se i ruoli internamente sono oggetti). Per esempio, se
è necessario limitare l'accesso alla sezione admin del sito web del blog ,
si potrebbe proteggere quella parte con un ruolo ``ROLE_BLOG_ADMIN``. Questo ruolo
non ha bisogno di essere definito ovunque, è sufficiente iniziare a usarlo.

.. note::

    Tutti i ruoli **devono** iniziare con il prefisso ``ROLE_`` per poter essere gestiti da
    Symfony2. Se si definiscono i propri ruoli con una classe ``Role`` dedicata
    (caratteristica avanzata), non bisogna usare il prefisso ``ROLE_``.

I ruoli gerarchici
~~~~~~~~~~~~~~~~~~

Invece di associare molti ruoli agli utenti, è possibile definire regole di ereditarietà
dei ruoli creando una gerarchia di ruoli:

.. configuration-block::

    .. code-block:: yaml

        # app/config/security.yml
        security:
            role_hierarchy:
                ROLE_ADMIN:       ROLE_USER
                ROLE_SUPER_ADMIN: [ROLE_ADMIN, ROLE_ALLOWED_TO_SWITCH]

    .. code-block:: xml

        <!-- app/config/security.xml -->
        <config>
            <role id="ROLE_ADMIN">ROLE_USER</role>
            <role id="ROLE_SUPER_ADMIN">ROLE_ADMIN, ROLE_ALLOWED_TO_SWITCH</role>
        </config>

    .. code-block:: php

        // app/config/security.php
        $container->loadFromExtension('security', array(
            'role_hierarchy' => array(
                'ROLE_ADMIN'       => 'ROLE_USER',
                'ROLE_SUPER_ADMIN' => array('ROLE_ADMIN', 'ROLE_ALLOWED_TO_SWITCH'),
            ),
        ));

Nella configurazione sopra, gli utenti con ruolo ``ROLE_ADMIN`` avranno anche il
ruolo ``ROLE_USER``. Il ruolo ``ROLE_SUPER_ADMIN`` ha ``ROLE_ADMIN``, ``ROLE_ALLOWED_TO_SWITCH``
e ``ROLE_USER`` (ereditati da ``ROLE_ADMIN``).

Logout
------

Generalmente, si vuole che gli utenti possano disconnettersi tramite logout. Fortunatamente,
il firewall può gestire automaticamente questo caso quando si attiva il
parametro di configurazione ``logout``:

.. configuration-block::

    .. code-block:: yaml

        # app/config/security.yml
        security:
            firewalls:
                secured_area:
                    # ...
                    logout:
                        path:   /logout
                        target: /
            # ...

    .. code-block:: xml

        <!-- app/config/security.xml -->
        <config>
            <firewall name="secured_area" pattern="^/">
                <!-- ... -->
                <logout path="/logout" target="/" />
            </firewall>
            <!-- ... -->
        </config>

    .. code-block:: php

        // app/config/security.php
        $container->loadFromExtension('security', array(
            'firewalls' => array(
                'secured_area' => array(
                    // ...
                    'logout' => array('path' => 'logout', 'target' => '/'),
                ),
            ),
            // ...
        ));

Una volta che questo viene configurato sotto il firewall, l'invio di un utente in ``/logout``
(o qualunque debba essere il percorso) farà disconnettere
l'utente corrente. L'utente sarà quindi inviato alla pagina iniziale (il valore definito
dal parametro  ``target``). Entrambi i parametri di configurazione ``path`` e ``target``
assumono come impostazione predefinita ciò che è specificato qui. In altre parole, se non è necessario personalizzarli,
è possibile ometterli completamente e accorciare la configurazione:

.. configuration-block::

    .. code-block:: yaml

        logout: ~

    .. code-block:: xml

        <logout />

    .. code-block:: php

        'logout' => array(),

Si noti che *non* è necessario implementare un controllore per l'URL ``/logout``,
perché il firewall si occupa di tutto. Si può, tuttavia, creare
una rotta da poter utilizzare per generare l'URL:

.. configuration-block::

    .. code-block:: yaml

        # app/config/routing.yml
        logout:
            pattern:   /logout

    .. code-block:: xml

        <!-- app/config/routing.xml -->
        <?xml version="1.0" encoding="UTF-8" ?>

        <routes xmlns="http://symfony.com/schema/routing"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://symfony.com/schema/routing http://symfony.com/schema/routing/routing-1.0.xsd">

            <route id="logout" pattern="/logout" />

        </routes>

    ..  code-block:: php

        // app/config/routing.php
        use Symfony\Component\Routing\RouteCollection;
        use Symfony\Component\Routing\Route;

        $collection = new RouteCollection();
        $collection->add('logout', new Route('/logout', array()));

        return $collection;

Una volta che l'utente è stato disconnesso, viene rinviato al percorso
definito dal parametro ``target`` sopra (ad esempio, la ``homepage``). Per
ulteriori informazioni sulla configurazione di logout, vedere il
:doc:`Riferimento della configurazione di sicurezza</reference/configuration/security>`.

Controllare l'accesso nei template
----------------------------------

Nel caso si voglia controllare all'interno di un template se l'utente corrente ha un ruolo, usare
la funzione helper:

.. configuration-block::

    .. code-block:: html+jinja

        {% if is_granted('ROLE_ADMIN') %}
            <a href="...">Delete</a>
        {% endif %}

    .. code-block:: html+php

        <?php if ($view['security']->isGranted('ROLE_ADMIN')): ?>
            <a href="...">Delete</a>
        <?php endif; ?>

.. note::

    Se si utilizza questa funzione e *non* si è in un URL dove c'è un firewall
    attivo, viene lanciata un'eccezione. Anche in questo caso, è quasi sempre una buona
    idea avere un firewall principale che copra tutti gli URL (come si è visto
    in questo capitolo).

Verifica dell'accesso nei controllori
-------------------------------------

Quando si vuole verificare se l'utente corrente abbia un ruolo nel controllore, usare
il metodo ``isGranted`` del contesto di sicurezza:

.. code-block:: php

    public function indexAction()
    {
        // mostrare contenuti diversi agli utenti admin
        if($this->get('security.context')->isGranted('ADMIN')) {
            // caricare qui contenuti di amministrazione
        }
        // caricare qui altri contenuti normali 
    }

.. note::

    Un firewall deve essere attivo o verrà lanciata un'eccezione quando viene
    chiamato il metodo ``isGranted``. Vedere la nota precedente sui template per maggiori dettagli.

Impersonare un utente
---------------------

A volte, è utile essere in grado di passare da un utente all'altro senza
dover uscire e rientrare tutte le volte (per esempio quando si esegue il debug o si cerca
di capire un bug che un utente vede ma che non si riesce a riprodurre). Lo si può fare
facilmente, attivando l'ascoltatore ``switch_user`` del firewall:

.. configuration-block::

    .. code-block:: yaml

        # app/config/security.yml
        security:
            firewalls:
                main:
                    # ...
                    switch_user: true

    .. code-block:: xml

        <!-- app/config/security.xml -->
        <config>
            <firewall>
                <!-- ... -->
                <switch-user />
            </firewall>
        </config>

    .. code-block:: php

        // app/config/security.php
        $container->loadFromExtension('security', array(
            'firewalls' => array(
                'main'=> array(
                    // ...
                    'switch_user' => true
                ),
            ),
        ));

Per passare a un altro utente, basta aggiungere una stringa query all'URL corrente,
con il parametro ``_switch_user`` e il nome utente come valore :

    http://example.com/indirizzo?_switch_user=thomas

Per tornare indietro all'utente originale, usare il nome utente speciale ``_exit``:

    http://example.com/indirizzo?_switch_user=_exit

Naturalmente, questa funzionalità deve essere messa a disposizione di un piccolo gruppo di utenti.
Per impostazione predefinita, l'accesso è limitato agli utenti che hanno il ruolo ``ROLE_ALLOWED_TO_SWITCH``.
Il nome di questo ruolo può essere modificato tramite l'impostazione ``role``. Per
maggiore sicurezza, è anche possibile modificare il nome del parametro della query tramite l'impostazione
``parameter``:

.. configuration-block::

    .. code-block:: yaml

        # app/config/security.yml
        security:
            firewalls:
                main:
                    // ...
                    switch_user: { role: ROLE_ADMIN, parameter: _want_to_be_this_user }

    .. code-block:: xml

        <!-- app/config/security.xml -->
        <config>
            <firewall>
                <!-- ... -->
                <switch-user role="ROLE_ADMIN" parameter="_want_to_be_this_user" />
            </firewall>
        </config>

    .. code-block:: php

        // app/config/security.php
        $container->loadFromExtension('security', array(
            'firewalls' => array(
                'main'=> array(
                    // ...
                    'switch_user' => array('role' => 'ROLE_ADMIN', 'parameter' => '_want_to_be_this_user'),
                ),
            ),
        ));

Autenticazione senza stato
--------------------------

Per impostazione predefinita, Symfony2 si basa su un cookie (Session) per persistere il contesto
di sicurezza dell'utente. Ma se si utilizzano certificati o l'autenticazione HTTP, per
esempio, la persistenza non è necessaria, in quanto le credenziali sono disponibili a ogni
richiesta. In questo caso e se non è necessario memorizzare nient'altro tra le
richieste, è possibile attivare l'autenticazione senza stato (il che significa Symfony non creerà
alcun cookie):

.. configuration-block::

    .. code-block:: yaml

        # app/config/security.yml
        security:
            firewalls:
                main:
                    http_basic: ~
                    stateless:  true

    .. code-block:: xml

        <!-- app/config/security.xml -->
        <config>
            <firewall stateless="true">
                <http-basic />
            </firewall>
        </config>

    .. code-block:: php

        // app/config/security.php
        $container->loadFromExtension('security', array(
            'firewalls' => array(
                'main' => array('http_basic' => array(), 'stateless' => true),
            ),
        ));

.. note::

    Se si usa un form di login, Symfony2 creerà un cookie anche se si imposta
    ``stateless`` a ``true``.

Considerazioni finali
---------------------

La sicurezza può essere un problema profondo e complesso nell'applicazione da risolvere in modo corretto.
Per fortuna, il componente della sicurezza di Symfony segue un ben collaudato modello di
sicurezza basato su *autenticazione* e *autorizzazione*. L'autenticazione,
che avviene sempre per prima, è gestita da un firewall il cui compito è quello di determinare
l'identità degli utenti attraverso diversi metodi (ad esempio l'autenticazione HTTP,
il form di login, ecc.). Nel ricettario, si trovano esempi di altri metodi 
per la gestione dell'autenticazione, includendo quello che tratta l'implementazione della funzionalità cookie 
"Ricorda i dati".

Una volta che un utente è autenticato, lo strato di autorizzazione può stabilire se
l'utente debba o meno avere accesso a una specifica risorsa. Più frequentemente,
i *ruoli* sono applicati a URL, classi o metodi e se l'utente corrente
non ha quel ruolo, l'accesso è negato. Lo strato di autorizzazione, però,
è molto più profondo e segue un sistema di "voto", in modo che tutte le parti
possono determinare se l'utente corrente dovrebbe avere accesso a una data risorsa.
Ulteriori informazioni su questo e altri argomenti nel ricettario.

Saperne di più con il ricettario
--------------------------------

* :doc:`Forzare HTTP/HTTPS </cookbook/security/force_https>`
* :doc:`Blacklist di utenti per indirizzo IP </cookbook/security/voters>`
* :doc:`Access Control List (ACL) </cookbook/security/acl>`
* :doc:`/cookbook/security/remember_me`

.. _`componente della sicurezza`: https://github.com/symfony/Security
.. _`JMSSecurityExtraBundle`: https://github.com/schmittjoh/JMSSecurityExtraBundle
.. _`FOSUserBundle`: https://github.com/FriendsOfSymfony/FOSUserBundle
.. _`implementare l'interfaccia \Serializable`: http://php.net/manual/en/class.serializable.php
.. _`functions-online.com`: http://www.functions-online.com/sha1.html
