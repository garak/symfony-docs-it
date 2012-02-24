.. index::
   single: Sicurezza; Riferimento configurazione

Riferimento configurazione sicurezza
====================================

Il sistema di sicurezza è una delle parti più potenti di Symfony2 e può
essere controllato in gran parte tramite la sua configurazione.

Configurazione predefinita completa
-----------------------------------

La seguente è la configurazione predefinita completa del sistema di sicurezza.
Ogni parte sarà spiegata nella prossima sezione.

.. configuration-block::

    .. code-block:: yaml

        # app/config/security.yml
        security:
            access_denied_url: /foo/error403

            always_authenticate_before_granting: false

            # la strategia può essere: none, migrate, invalidate
            session_fixation_strategy: migrate

            access_decision_manager:
                strategy: affirmative
                allow_if_all_abstain: false
                allow_if_equal_granted_denied: true

            acl:
                connection: default # qualsiasi nome configurato nella sezione doctrine.dbal
                tables:
                    class: acl_classes
                    entry: acl_entries
                    object_identity: acl_object_identities
                    object_identity_ancestors: acl_object_identity_ancestors
                    security_identity: acl_security_identities
                cache:
                    id: service_id
                    prefix: sf2_acl_
                voter:
                    allow_if_object_identity_unavailable: true

            encoders:
                somename:
                    class: Acme\DemoBundle\Entity\User
                Acme\DemoBundle\Entity\User: sha512
                Acme\DemoBundle\Entity\User: plaintext
                Acme\DemoBundle\Entity\User:
                    algorithm: sha512
                    encode_as_base64: true
                    iterations: 5000
                Acme\DemoBundle\Entity\User:
                    id: my.custom.encoder.service.id

            providers:
                memory:
                    name: memory
                    users:
                        foo: { password: foo, roles: ROLE_USER }
                        bar: { password: bar, roles: [ROLE_USER, ROLE_ADMIN] }
                entity:
                    entity: { class: SecurityBundle:User, property: username }

            factories:
                MyFactory: %kernel.root_dir%/../src/Acme/DemoBundle/Resources/config/security_factories.xml

            firewalls:
                somename:
                    pattern: .*
                    request_matcher: some.service.id
                    access_denied_url: /foo/error403
                    access_denied_handler: some.service.id
                    entry_point: some.service.id
                    provider: nome_di_un_provider_di_cui_sopra
                    context: name
                    stateless: false
                    x509:
                        provider: nome_di_un_provider_di_cui_sopra
                    http_basic:
                        provider: nome_di_un_provider_di_cui_sopra
                    http_digest:
                        provider: nome_di_un_provider_di_cui_sopra
                    form_login:
                        check_path: /login_check
                        login_path: /login
                        use_forward: false
                        always_use_default_target_path: false
                        default_target_path: /
                        target_path_parameter: _target_path
                        use_referer: false
                        failure_path: /foo
                        failure_forward: false
                        failure_handler: some.service.id
                        success_handler: some.service.id
                        username_parameter: _username
                        password_parameter: _password
                        csrf_parameter: _csrf_token
                        intention: authenticate
                        csrf_provider: my.csrf_provider.id
                        post_only: true
                        remember_me: false
                    remember_me:
                        token_provider: name
                        key: someS3cretKey
                        name: NameOfTheCookie
                        lifetime: 3600 # in seconds
                        path: /foo
                        domain: somedomain.foo
                        secure: true
                        httponly: true
                        always_remember_me: false
                        remember_me_parameter: _remember_me
                    logout:
                        path:   /logout
                        target: /
                        invalidate_session: false
                        delete_cookies:
                            a: { path: null, domain: null }
                            b: { path: null, domain: null }
                        handlers: [some.service.id, another.service.id]
                        success_handler: some.service.id
                    anonymous: ~

            access_control:
                -
                    path: ^/foo
                    host: mydomain.foo
                    ip: 192.0.0.0/8
                    roles: [ROLE_A, ROLE_B]
                    requires_channel: https

            role_hierarchy:
                ROLE_SUPERADMIN: ROLE_ADMIN
                ROLE_SUPERADMIN: 'ROLE_ADMIN, ROLE_USER'
                ROLE_SUPERADMIN: [ROLE_ADMIN, ROLE_USER]
                anything: { id: ROLE_SUPERADMIN, value: 'ROLE_USER, ROLE_ADMIN' }
                anything: { id: ROLE_SUPERADMIN, value: [ROLE_USER, ROLE_ADMIN] }

.. _reference-security-firewall-form-login:

Configurazione del form di login
--------------------------------

Quando si usa l'ascoltatore di autenticazione ``form_login`` dietro un firewall,
ci sono diverse opzioni comuni per configurare l'esoerienza del form di login:

Il form e il processo di login
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

*   ``login_path`` (tipo: ``stringa``, predefinito: ``/login``)
    È l'URL a cui l'utente sarà rinviato (a meno che ``use_forward`` non sia
    ``true``) quando prova ad accedere a una risorsa protetta,
    ma non è autenticato.

    Questo URL **deve** essere accessibile da un utente normale e non autenticato,
    altrimenti si creerebbe un loop infinito. Per dettagli, vedere
    ":ref:`Evitare problemi comuni<book-security-common-pitfalls>`".

*   ``check_path`` (tipo: ``stringa``, predefinito: ``/login_check``)
    È l'URL a cui il form di login viene inviato. Il firewall intercetterà
    ogni richiesta (solo quelle ``POST``, per impostazione predefinita) a questo URL
    e processerà le credenziali di login inviate.

    Assicurarsi che questo URL sia coperto dal proprio firewall principale (cioè non
    creare un firewall separato solo per l'URL ``check_path``).

*   ``use_forward`` (tipo: ``booleano``, predefinito: ``false``)
    Se si vuole che l'utente sia rimandato al form di login invece di essere 
    rinviato, impostare questa opzione a ``true``.

*   ``username_parameter`` (tipo: ``stringa``, predefinito: ``_username``)
    Questo il nome del campo che si dovrebbe dare al campo username del proprio
    form di login. Quando si invia il form a ``check_path``, il sistema di
    sicurezza cercherà un parametro POST con questo nome.

*   ``password_parameter`` (tipo: ``stringa``, predefinito: ``_password``)
    Questo il nome del campo che si dovrebbe dare al campo password del proprio
    form di login. Quando si invia il form a ``check_path``, il sistema di
    sicurezza cercherà un parametro POST con questo nome.

*   ``post_only`` (tipo: ``booleano``, predefinito: ``true``)
    Per impostazione predefinita, occorre inviare il proprio form di login
    all'URL ``check_path`` usando una richiesta POST. Impostando questa opzione
    a ``true``, si può inviare una richiesta GET all'URL ``check_path``.

Rinvio dopo il login
~~~~~~~~~~~~~~~~~~~~

* ``always_use_default_target_path`` (tipo: ``booleano``, predefinito: ``false``)
* ``default_target_path`` (tipo: ``stringa``, predefinito: ``/``)
* ``target_path_parameter`` (tipo: ``stringa``, predefinito: ``_target_path``)
* ``use_referer`` (tipo: ``booleano``, predefinito: ``false``)
