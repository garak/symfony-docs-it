.. index::
   single: Sicurezza; Personalizzazione del form di login

Come personalizzare il form di login
====================================

L'uso di un :ref:`form di login<book-security-form-login>` per l'autenticazione è un
metodo comune e flessibile per gestire l'autenticazione in Symfony2. Quasi ogni aspetto
del form è personalizzabile. La configurazione predefinita e completa è mostrata
nella prossima sezione.

Riferimento di configurazione del form di login
-----------------------------------------------

.. configuration-block::

    .. code-block:: yaml

        # app/config/security.yml
        security:
            firewalls:
                main:
                    form_login:
                        # the user is redirected here when he/she needs to login
                        login_path:                     /login

                        # if true, forward the user to the login form instead of redirecting
                        use_forward:                    false

                        # submit the login form here
                        check_path:                     /login_check

                        # by default, the login form *must* be a POST, not a GET
                        post_only:                      true

                        # login success redirecting options (read further below)
                        always_use_default_target_path: false
                        default_target_path:            /
                        target_path_parameter:          _target_path
                        use_referer:                    false

                        # login failure redirecting options (read further below)
                        failure_path:                   null
                        failure_forward:                false

                        # field names for the username and password fields
                        username_parameter:             _username
                        password_parameter:             _password

                        # csrf token options
                        csrf_parameter:                 _csrf_token
                        intention:                      authenticate

    .. code-block:: xml

        <!-- app/config/security.xml -->
        <config>
            <firewall>
                <form-login
                    check_path="/login_check"
                    login_path="/login"
                    use_forward="false"
                    always_use_default_target_path="false"
                    default_target_path="/"
                    target_path_parameter="_target_path"
                    use_referer="false"
                    failure_path="null"
                    failure_forward="false"
                    username_parameter="_username"
                    password_parameter="_password"
                    csrf_parameter="_csrf_token"
                    intention="authenticate"
                    post_only="true"
                />
            </firewall>
        </config>

    .. code-block:: php

        // app/config/security.php
        $container->loadFromExtension('security', array(
            'firewalls' => array(
                'main' => array('form_login' => array(
                    'check_path'                     => '/login_check',
                    'login_path'                     => '/login',
                    'user_forward'                   => false,
                    'always_use_default_target_path' => false,
                    'default_target_path'            => '/',
                    'target_path_parameter'          => _target_path,
                    'use_referer'                    => false,
                    'failure_path'                   => null,
                    'failure_forward'                => false,
                    'username_parameter'             => '_username',
                    'password_parameter'             => '_password',
                    'csrf_parameter'                 => '_csrf_token',
                    'intention'                      => 'authenticate',
                    'post_only'                      => true,
                )),
            ),
        ));

Rinvio dopo il successo
-----------------------

Si può modificare il posto in cui il form di login rinvia dopo un login eseguito con
successo, usando le varie opzioni di configurazione. Per impostazione predefinita, il
form rinvierà all'URL richiesto dall'utente (cioè l'URL che ha portato al form di login).
Per esempio, se l'utente ha richiesto ``http://www.example.com/admin/post/18/edit``,
sarà successivamente rimandato indietro a ``http://www.example.com/admin/post/18/edit``,
dopo il login. Questo grazie alla memorizzazione in sessione dell'URL richiesto. Se non
c'è alcun URL in sessione (forse l'utente ha richiesto direttamente la pagina di login),
l'utente è rinviato alla pagina predefinita, che è ``/`` (ovvero la homepage). Si può
modificare questo comportamento in diversi
modi.

.. note::

    Come accennato, l'utente viene rinviato alla pagina che ha precedentemente
    richiesto. A volte questo può causare problemi, per esempio se una richiesta AJAX
    eseguita in background appare come ultimo URL visitato, rinviando quindi l'utente
    in quell'URL. Per informazioni su come controllare questo comportamento, vedere
    :doc:`/cookbook/security/target_path`.

Cambiare la pagina predefinita
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Prima di tutto, la pagina predefinita (la pagina a cui l'utente viene rinviato, se
non ci sono pagine precedenti in sessione) può essere impostata. Per impostarla a
``/admin``, usare la seguente configurazione:

.. configuration-block::

    .. code-block:: yaml

        # app/config/security.yml
        security:
            firewalls:
                main:
                    form_login:
                        # ...
                        default_target_path: /admin

    .. code-block:: xml

        <!-- app/config/security.xml -->
        <config>
            <firewall>
                <form-login
                    default_target_path="/admin"                    
                />
            </firewall>
        </config>

    .. code-block:: php

        // app/config/security.php
        $container->loadFromExtension('security', array(
            'firewalls' => array(
                'main' => array('form_login' => array(
                    // ...
                    'default_target_path' => '/admin',
                )),
            ),
        ));

Ora, se non ci sono URL in sessione, gli utenti saranno mandati su ``/admin``.

Rinviare sempre alla pagina predefinita
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Si può fare in modo che gli utenti siano sempre rinviati alla pagina predefinita,
senza considerare l'URL richiesta prima del login, impostando l'opzione
``always_use_default_target_path`` a ``true``:

.. configuration-block::

    .. code-block:: yaml

        # app/config/security.yml
        security:
            firewalls:
                main:
                    form_login:
                        # ...
                        always_use_default_target_path: true
                        
    .. code-block:: xml

        <!-- app/config/security.xml -->
        <config>
            <firewall>
                <form-login
                    always_use_default_target_path="true"
                />
            </firewall>
        </config>

    .. code-block:: php

        // app/config/security.php
        $container->loadFromExtension('security', array(
            'firewalls' => array(
                'main' => array('form_login' => array(
                    // ...
                    'always_use_default_target_path' => true,
                )),
            ),
        ));

Usare l'URL del referer
~~~~~~~~~~~~~~~~~~~~~~~

Se nessun URL è stato memorizzato in sessione, si potrebbe voler provare a usare
``HTTP_REFERER``, che spesso coincide. Lo si può fare impostando
``use_referer`` a ``true`` (il valore predefinito è ``false``): 

.. configuration-block::

    .. code-block:: yaml

        # app/config/security.yml
        security:
            firewalls:
                main:
                    form_login:
                        # ...
                        use_referer:        true

    .. code-block:: xml

        <!-- app/config/security.xml -->
        <config>
            <firewall>
                <form-login
                    use_referer="true"
                />
            </firewall>
        </config>

    .. code-block:: php

        // app/config/security.php
        $container->loadFromExtension('security', array(
            'firewalls' => array(
                'main' => array('form_login' => array(
                    // ...
                    'use_referer' => true,
                )),
            ),
        ));

.. versionadded:: 2.1
    Dalla 2.1, se il referer è uguale all'opzione ``login_path``, l'utente
    sarà rinviato a ``default_target_path``.

Controllare l'URL di rinvio da dentro un form
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Si può anche forzare la pagina di rinvio dell'utente nel form stesso, includendo un
campo nascosto dal nome ``_target_path``. Per esempio, per rinviare all'URL
definito in una rotta ``account``, fare come segue:

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

            <input type="hidden" name="_target_path" value="account" />

            <input type="submit" name="login" />
        </form>

    .. code-block:: html+php

        <?php // src/Acme/SecurityBundle/Resources/views/Security/login.html.php ?>
        <?php if ($error): ?>
            <div><?php echo $error->getMessage() ?></div>
        <?php endif; ?>

        <form action="<?php echo $view['router']->generate('login_check') ?>" method="post">
            <label for="username">Nome utente:</label>
            <input type="text" id="username" name="_username" value="<?php echo $last_username ?>" />

            <label for="password">Password:</label>
            <input type="password" id="password" name="_password" />

            <input type="hidden" name="_target_path" value="account" />
            
            <input type="submit" name="login" />
        </form>

L'utente sarà ora rinviato al valore del campo nascosto. Il valore può essere
un percorso relativo, un URL assoluto o un nome di rotta. Si può anche modificare il
nome del campo nascosto, cambiando l'opzione ``target_path_parameter`` con
il valore desiderato.

.. configuration-block::

    .. code-block:: yaml

        # app/config/security.yml
        security:
            firewalls:
                main:
                    form_login:
                        target_path_parameter: redirect_url

    .. code-block:: xml

        <!-- app/config/security.xml -->
        <config>
            <firewall>
                <form-login
                    target_path_parameter="redirect_url"
                />
            </firewall>
        </config>

    .. code-block:: php

        // app/config/security.php
        $container->loadFromExtension('security', array(
            'firewalls' => array(
                'main' => array('form_login' => array(
                    'target_path_parameter' => redirect_url,
                )),
            ),
        ));

Rinvio al fallimento del login
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Oltre a rinviare l'utente dopo un login eseguito con successo, si può anche impostare
l'URL a cui l'utente va rinviato dopo un login fallito (p.e. perché è stato inserito
un nome utente o una password non validi). Per impostazione predefinita, l'utente viene
rinviato al medesimo form di login. Si può impostare un URL diverso, usando la
configurazione seguente:

.. configuration-block::

    .. code-block:: yaml

        # app/config/security.yml
        security:
            firewalls:
                main:
                    form_login:
                        # ...
                        failure_path: /login_failure
                        
    .. code-block:: xml

        <!-- app/config/security.xml -->
        <config>
            <firewall>
                <form-login
                    failure_path="login_failure"
                />
            </firewall>
        </config>

    .. code-block:: php

        // app/config/security.php
        $container->loadFromExtension('security', array(
            'firewalls' => array(
                'main' => array('form_login' => array(
                    // ...
                    'failure_path' => login_failure,
                )),
            ),
        ));
