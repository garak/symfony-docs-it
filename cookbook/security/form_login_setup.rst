Costruire un form di login tradizionale
=======================================

.. tip::

    Se si ha bisogno di un form di login e si stanno memorizzando gli utenti in una base dati,
    si dovrebbe considerare l'uso di `FOSUserBundle`_, che aiuta a costruire
    un oggetto ``User`` e fornisce molte rotte e controllori per compiti comuni,
    come login, registrazione e recupero della password.

In questa ricetta, si costruirà un form di login tradizionale. Ovviamente, quando l'utente
entra, si possono caricare utenti da dovunque, anche da una base dati.
Vedere :ref:`security-user-providers` per i dettagli.

Diamo per scontata la lettura dell'inizio del
:doc:`capito sulla sicurezza </book/security>` e quindi l'autenticazione ``http_basic``
correttamente funzionante.

First, enable form login under your firewall:

.. configuration-block::

    .. code-block:: yaml

        # app/config/security.yml
        security:
            # ...

            firewalls:
                default:
                    anonymous: ~
                    http_basic: ~
                    form_login:
                        login_path: /login
                        check_path: /login_check

    .. code-block:: xml

        <!-- app/config/security.xml -->
        <?xml version="1.0" encoding="UTF-8"?>
        <srv:container xmlns="http://symfony.com/schema/dic/security"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xmlns:srv="http://symfony.com/schema/dic/services"
            xsi:schemaLocation="http://symfony.com/schema/dic/services
                http://symfony.com/schema/dic/services/services-1.0.xsd">

            <config>
                <firewall name="main">
                    <anonymous />
                    <form-login login-path="/login" check-path="/login_check" />
                </firewall>
            </config>
        </srv:container>

    .. code-block:: php

        // app/config/security.php
        $container->loadFromExtension('security', array(
            'firewalls' => array(
                'main' => array(
                    'anonymous'  => array(),
                    'form_login' => array(
                        'login_path' => '/login',
                        'check_path' => '/login_check',
                    ),
                ),
            ),
        ));

.. tip::

    Le voci ``login_path`` e ``check_path``  possono anche contenere nomi di rotte (purché
    senza segnaposto obbligatori, come ``/login/{pippo}`` dove ``pippo`` non ha un
    valore predefinito).

Ora, quando il sistema di sicurezza inizia il processo di autenticazione, rinvierà
l'utente al form di login su ``/login``. L'implementazione visuale di tale form è compito
dello sviluppatore. Primo, creare un nuovo ``SecurityController`` in un
bundle, con una ``loginAction``::

    // src/AppBundle/Controller/SecurityController.php
    namespace AppBundle\Controller;

    use Symfony\Bundle\FrameworkBundle\Controller\Controller;
    use Symfony\Component\HttpFoundation\Request;

    class SecurityController extends Controller
    {
    }

Quindi, creare due rotte: una per ciascuno dei percorsi configurati in precedenza
sotto ``form_login`` (``/login`` e ``/login_check``):

.. configuration-block::

    .. code-block:: php-annotations

        // src/AppBundle/Controller/SecurityController.php

        // ...
        use Symfony\Component\HttpFoundation\Request;
        use Sensio\Bundle\FrameworkExtraBundle\Configuration\Route;

        class SecurityController extends Controller
        {
            /**
             * @Route("/login", name="login_route")
             */
            public function loginAction(Request $request)
            {
            }

            /**
             * @Route("/login_check", name="login_check")
             */
            public function loginCheckAction()
            {
                // questa azione non verrà eseguita,
                // perché la rotta è gestita dal sistema di sicurezza
            }
        }

    .. code-block:: yaml

        # app/config/routing.yml
        login_route:
            path:     /login
            defaults: { _controller: AppBundle:Security:login }
        login_check:
            path: /login_check
            # questa azione non verrà eseguita,
            # perché la rotta è gestita dal sistema di sicurezza

    .. code-block:: xml

        <!-- app/config/routing.xml -->
        <?xml version="1.0" encoding="UTF-8" ?>
        <routes xmlns="http://symfony.com/schema/routing"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://symfony.com/schema/routing
                http://symfony.com/schema/routing/routing-1.0.xsd">

            <route id="login_route" path="/login">
                <default key="_controller">AppBundle:Security:login</default>
            </route>

            <route id="login_check" path="/login_check" />
            <!-- questa azione non verrà eseguita,
                 perché la rotta è gestita dal sistema di sicurezza -->
        </routes>

    ..  code-block:: php

        // app/config/routing.php
        use Symfony\Component\Routing\RouteCollection;
        use Symfony\Component\Routing\Route;

        $collection = new RouteCollection();
        $collection->add('login_route', new Route('/login', array(
            '_controller' => 'AppBundle:Security:login',
        )));
        $collection->add('login_check', new Route('/login_check', array()));
        // nessuna azione è abbinata a questa rotta,
        // essendo gestita dal sistema di sicurezza

        return $collection;

Ottimo! Ora, aggiungere a ``loginAction`` la logica che mostrerà il form
di login::

    // src/AppBundle/Controller/SecurityController.php

    // ...
    use Symfony\Component\Security\Core\SecurityContextInterface;

    // ...
    public function loginAction(Request $request)
    {
        $session = $request->getSession();

        // recupera l'errore, se ce n'è uno
        if ($request->attributes->has(SecurityContextInterface::AUTHENTICATION_ERROR)) {
            $error = $request->attributes->get(SecurityContextInterface::AUTHENTICATION_ERROR);
        } elseif (null !== $session && $session->has(SecurityContextInterface::AUTHENTICATION_ERROR)) {
            $error = $session->get(SecurityContextInterface::AUTHENTICATION_ERROR);
            $session->remove(SecurityContextInterface::AUTHENTICATION_ERROR);
        } else {
            $error = null;
        }

        // ultimo nome utente inserito
        $lastUsername = (null === $session) ? '' : $session->get(SecurityContextInterface::LAST_USERNAME);

        return $this->render(
            'security/login.html.twig',
            array(
                // last username entered by the user
                'last_username' => $lastUsername,
                'error'         => $error,
            )
        );
    }

Non ci si lasci confondere da questo controllore. Come vedremo tra poco, quando
l'utente compila il form, il sistema di sicurezza lo gestisce automaticamente.
Se l'utente ha inserito un nome o una password non validi,
questo controllore legge l'errore dal sistema di sicurezza, in modo
che possa essere mostrato all'utente.

In altre parole, il compito dello sviluppatore è quello di *mostrare* il form di login,
con eventuali errori, ma è il sistema di sicurezza che si prende cura di verificare
il nome utente e la password ed eventualmente autenticare l'utente.

Finally, create the template:

.. configuration-block::

    .. code-block:: html+jinja

        {# app/Resources/views/security/login.html.twig #}
        {# ... probabilmente si vorrà estendere il proprio template di base, p.e. base.html.twig #}

        {% if error %}
            <div>{{ error.messageKey|trans(error.messageData) }}</div>
        {% endif %}

        <form action="{{ path('login_check') }}" method="post">
            <label for="username">Username:</label>
            <input type="text" id="username" name="_username" value="{{ last_username }}" />

            <label for="password">Password:</label>
            <input type="password" id="password" name="_password" />

            {#
                Se si vuole controllare l'URL a cui l'utente viene
                rinviato in caso di successo (approfondito più avanti)
                <input type="hidden" name="_target_path" value="/account" />
            #}

            <button type="submit">login</button>
        </form>

    .. code-block:: html+php

        <!-- src/Acme/SecurityBundle/Resources/views/Security/login.html.php -->
        <?php if ($error): ?>
            <div><?php echo $error->getMessage() ?></div>
        <?php endif ?>

        <form action="<?php echo $view['router']->generate('login_check') ?>" method="post">
            <label for="username">Username:</label>
            <input type="text" id="username" name="_username" value="<?php echo $last_username ?>" />

            <label for="password">Password:</label>
            <input type="password" id="password" name="_password" />

            <!--
                Se si vuole controllare l'URL a cui l'utente viene
                rinviato in caso di successo (approfondito più avanti)
                <input type="hidden" name="_target_path" value="/account" />
            -->

            <button type="submit">login</button>
        </form>


.. tip::

    La variabile ``error`` passata nel template è un'istanza di
    :class:`Symfony\\Component\\Security\\Core\\Exception\\AuthenticationException`.
    Potrebbe contenere informazioni, alcune delle quali sensibili, sul motivo
    del fallimento, per cui va usata con cautela.

Il form può avere qualsiasi aspetto, ma ha alcuni requisiti:

* Deve eseguire un POST su ``/login_check``, cioè il valore configurato
  sotto la voce ``form_login`` in ``security.yml``.

* Il campo del nome utente deve chiamarsi ``_username`` e quello della password deve
  chiamarsi ``_password``.

.. tip::

    In realtà, questi valori possono essere cambiati in ``form_login``. Vedere
    :ref:`reference-security-firewall-form-login` per maggiori dettagli.

.. caution::

    Questo form di login non è al momento coperto contro attacchi di tipo CSRF- Leggere
    :doc:`/cookbook/security/csrf_in_login_form` per sapere come proteggere il
    form.

Ecco fatto! Compilando il form, il sistema di sicurezza verifica automaticamente
le credenziali dell'utente e lo autentica oppure lo rimanda
al form stesso, dove gli errori possono essere mostrati.

Per rivedere l'intero processo:

#. L'utente prova ad accedere a una risorsa protetta;
#. Il firewall inizia il processo di autenticazione, rinviando l'utente
   al form di login (``/login``);
#. La pagina ``/login`` rende il form di login, tramite rotta e controllore creati
   in questo esempio;
#. L'utente compila il form e lo invia su ``/login_check``;
#. Il sistema di sicurezza intercetta la richiesta, verifica le credenziali inserite,
   autentica l'utente, se sono corrette, o lo rimanda indietro
   al form di login, se non lo sono.

Rinvio dopo il successo
-----------------------

Se le credenziali inserite sono corrette, l'utente sarà rinviat alla
pagina che era stata originariamente richiesta (p.e. ``/admin/pippo``). Se l'utente aveva
richiamato direttamente la pagina di login, sarà rinviato alla homepage. Tutto questo
è personalizzabile, consentendo, per esempio, di rinviare l'utente a
uno specifico URL.

Per ulteriori dettagli su come si possa personalizzare il form di login e in generale tutto il processo,
vedere :doc:`/cookbook/security/form_login`.

.. _book-security-common-pitfalls:

Evitare errori comuni
---------------------

Configurando un form di login, fare attenzioni ad alcuni errori comuni.

**1. Creare le rotte giuste**

Innanzitutto, assicurarsi di aver definito correttamente le rotte ``/login`` e ``/login_check``
e che corrispondano ai valori configurati di ``login_path`` e
``check_path``. Un errore in questo punto vuol dire che si viene rinviati
a una pagina 404 invece che a quella di login oppure che all'invio del
form di login non succede nulla (si vede solo il form di login
in continuazione).

**2. Assicurarsi che la pagina di login non sia protetta (andrebbe in loop!)**

Assicurarsi anche che la pagina di login sia accessibile a un utente anonimo. Per esempio,
la configurazione seguente, che richiede il ruolo ``ROLE_ADMIN`` per tutti
gli URL (incluso l'URL ``/login``), causerà un loop di rinvii:

.. configuration-block::

    .. code-block:: yaml

        # app/config/security.yml

        # ...
        access_control:
            - { path: ^/, roles: ROLE_ADMIN }

    .. code-block:: xml

        <!-- app/config/security.xml -->

        <!-- ... -->
        <access-control>
            <rule path="^/" role="ROLE_ADMIN" />
        </access-control>

    .. code-block:: php

        // app/config/security.php

        // ...
        'access_control' => array(
            array('path' => '^/', 'role' => 'ROLE_ADMIN'),
        ),

Aggiungendo una riga che corrisponde a ``/login/*`` e *non* richiede autenticazione,
il problema si risolve:

.. configuration-block::

    .. code-block:: yaml

        # app/config/security.yml

        # ...
        access_control:
            - { path: ^/login, roles: IS_AUTHENTICATED_ANONYMOUSLY }
            - { path: ^/, roles: ROLE_ADMIN }

    .. code-block:: xml

        <!-- app/config/security.xml -->

        <!-- ... -->
        <access-control>
            <rule path="^/login" role="IS_AUTHENTICATED_ANONYMOUSLY" />
            <rule path="^/" role="ROLE_ADMIN" />
        </access-control>

    .. code-block:: php

        // app/config/security.php

        // ...
        'access_control' => array(
            array('path' => '^/login', 'role' => 'IS_AUTHENTICATED_ANONYMOUSLY'),
            array('path' => '^/', 'role' => 'ROLE_ADMIN'),
        ),

Inoltre, se il firewall *non* consente utentei anonimi (manca la voce ``anonymous``),
si dovrà creare un altro firewall, che consenta utenti anonimi per
la pagina di login:

.. configuration-block::

    .. code-block:: yaml

        # app/config/security.yml

        # ...
        firewalls:
            # l'ordine conta! Questo va prima del firewall ^/
            login_firewall:
                pattern:   ^/login$
                anonymous: ~
            secured_area:
                pattern:    ^/
                form_login: ~

    .. code-block:: xml

        <!-- app/config/security.xml -->

        <!-- ... -->
        <firewall name="login_firewall" pattern="^/login$">
            <anonymous />
        </firewall>
        <firewall name="secured_area" pattern="^/">
            <form-login />
        </firewall>

    .. code-block:: php

        // app/config/security.php

        // ...
        'firewalls' => array(
            'login_firewall' => array(
                'pattern'   => '^/login$',
                'anonymous' => array(),
            ),
            'secured_area' => array(
                'pattern'    => '^/',
                'form_login' => array(),
            ),
        ),

**3. Assicurarsi che /login_check sia dietro a un firewall**

Quindi, assicurarsi che l'URL ``check_path`` URL (p.e. ``/login_check``) sia dietro
al firewall usato per il form di login (in questo esempio, l'unico
firewall corrisponde a *tutti* gli URL, incluso ``/login_check``). Se ``/login_check``
non corrisponde ad alcun firewall, si avrà un'eccezione ``Unable to find the controller
for path "/login_check"``.

**4. Firewall diversi non condividono il contesto di sicurezza**

Se si usano più firewall e si autentica su un solo firewall,
*non* ci si troverà autenticati automaticamente sugli altri.
Firewall diversi sono come sistemi di sicurezza diversi. Per poterlo fare, occorre
esplicitare lo stesso :ref:`reference-security-firewall-context`
per firewall diversi. Solitamente, per la maggior parte delle applicazion, basta comunque
un singolo firewall.

**5. Le pagine di errore non sono coperte da alcun firewall**

Poiché le rotte sono attivate *prima* della sicurezza, le pagine di errore 404 non sono coperte
da alcun firewall. Questo vuol dire non si possono fare verifiche di sicurezza, né accedere
all'oggetto utente in tali pagine. Vedere :doc:`/cookbook/controller/error_pages`
per ulteriori dettagli.

.. _`FOSUserBundle`: https://github.com/FriendsOfSymfony/FOSUserBundle
