.. index::
    single: Security; Autenticatore di password personalizzato

Creare un autenticatore di password personalizzato
==================================================

Si immagini di voler consentire l'accesso a un sito solo tra le 2 e le 4 del
pomeriggio. Prima di Symfony 2.4, occorreva creare token, factory, ascoltatore e
fornitore personalizzati. In questa ricetta, si vedrà come poterlo fare per un form di login
(cioè il posto in cui l'utente inserisce nome utente e password).

L'autenticatore di password
---------------------------

.. versionadded:: 2.4
    L'interfaccia ``SimpleFormAuthenticatorInterface`` è stata introdotta in Symfony 2.4.

Prima di tutto, creare una classe che implementi
:class:`Symfony\\Component\\Security\\Core\\Authentication\\SimpleFormAuthenticatorInterface`.
Successivamente, questa consentirà di creare logica personalizzata per autenticare
l'utente::

    // src/Acme/HelloBundle/Security/TimeAuthenticator.php
    namespace Acme\HelloBundle\Security;

    use Symfony\Component\HttpFoundation\Request;
    use Symfony\Component\Security\Core\Authentication\SimpleFormAuthenticatorInterface;
    use Symfony\Component\Security\Core\Authentication\Token\TokenInterface;
    use Symfony\Component\Security\Core\Authentication\Token\UsernamePasswordToken;
    use Symfony\Component\Security\Core\Encoder\EncoderFactoryInterface;
    use Symfony\Component\Security\Core\Exception\AuthenticationException;
    use Symfony\Component\Security\Core\Exception\UsernameNotFoundException;
    use Symfony\Component\Security\Core\User\UserProviderInterface;

    class TimeAuthenticator implements SimpleFormAuthenticatorInterface
    {
        private $encoderFactory;

        public function __construct(EncoderFactoryInterface $encoderFactory)
        {
            $this->encoderFactory = $encoderFactory;
        }

        public function authenticateToken(TokenInterface $token, UserProviderInterface $userProvider, $providerKey)
        {
            try {
                $user = $userProvider->loadUserByUsername($token->getUsername());
            } catch (UsernameNotFoundException $e) {
                throw new AuthenticationException('Nome utente o password non validi');
            }

            $encoder = $this->encoderFactory->getEncoder($user);
            $passwordValid = $encoder->isPasswordValid(
                $user->getPassword(),
                $token->getCredentials(),
                $user->getSalt()
            );

            if ($passwordValid) {
                $currentHour = date('G');
                if ($currentHour < 14 || $currentHour > 16) {
                    throw new AuthenticationException(
                        'Si può accedere solo tra le 14.00 e le 16.00!',
                        100
                    );
                }

                return new UsernamePasswordToken(
                    $user,
                    $user->getPassword(),
                    $providerKey,
                    $user->getRoles()
                );
            }

            throw new AuthenticationException('Nome utente o password non validi');
        }

        public function supportsToken(TokenInterface $token, $providerKey)
        {
            return $token instanceof UsernamePasswordToken
                && $token->getProviderKey() === $providerKey;
        }

        public function createToken(Request $request, $username, $password, $providerKey)
        {
            return new UsernamePasswordToken($username, $password, $providerKey);
        }
    }

Come funziona
-------------

Bene! Ora bisogna solo preparare la :ref:`cookbook-security-password-authenticator-config`.
Ma, prima, vediamo cosa fa ogni metodo di questa classe.

1) createToken
~~~~~~~~~~~~~~

Quando Symfony inizia a gestire una richiesta, viene richiamato ``createToken()``, dove
si crea un oggetto :class:`Symfony\\Component\\Security\\Core\\Authentication\\Token\\TokenInterface`,
che contiene qualsiasi informazione necessaria in ``authenticateToken()``
per autenticare l'utente (p.e. nome utente e password).

Qualsiasi oggetto token creato qui sarà passato più avanti ad ``authenticateToken()``.

2) supportsToken
~~~~~~~~~~~~~~~~

.. include:: _supportsToken.rst.inc

3) authenticateToken
~~~~~~~~~~~~~~~~~~~~

Se ``supportsToken`` restituisce ``true``, Symfony ora richiamerà ``authenticateToken()``.
Qui occorre verificare che il token possa accedere, prima recuperando
l'oggetto ``User`` dal fornitore di utenti e poi verificando la password
e l'ora.

.. note::

    Il "flusso" su come si ottiene l'oggetto ``User`` e come si determina se
    il token sia valido (p.e. verificandone la password) può variare in base ai
    requisiti.

Infine, si deve restituire un *nuovo* oggetto token, che è "autenticato"
(cioè ha almeno un ruolo associato) e che ha al suo interno l'oggetto
``User``.

In questo metodo, occorre un codificatore che verifichi la validità della password::

        $encoder = $this->encoderFactory->getEncoder($user);
        $passwordValid = $encoder->isPasswordValid(
            $user->getPassword(),
            $token->getCredentials(),
            $user->getSalt()
        );

Questo è un servizio già disponibile in Symfony, in cui algoritmo per la password
è configurato nella configurazione di sicurezza (``security.yml``), sotto
la voce ``encoders``. Più avanti si vedrà come iniettarlo in ``TimeAuthenticator``.

.. _cookbook-security-password-authenticator-config:

Configuratione
--------------

Ora, configurare ``TimeAuthenticator`` come servizio:

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        services:
            # ...

            time_authenticator:
                class:     Acme\HelloBundle\Security\TimeAuthenticator
                arguments: ["@security.encoder_factory"]

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <?xml version="1.0" ?>
        <container xmlns="http://symfony.com/schema/dic/services"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://symfony.com/schema/dic/services
                http://symfony.com/schema/dic/services/services-1.0.xsd">
            <services>
                <!-- ... -->

                <service id="time_authenticator"
                    class="Acme\HelloBundle\Security\TimeAuthenticator"
                >
                    <argument type="service" id="security.encoder_factory" />
                </service>
            </services>
        </container>

    .. code-block:: php

        // app/config/config.php
        use Symfony\Component\DependencyInjection\Definition;
        use Symfony\Component\DependencyInjection\Reference;
        
        // ...

        $container->setDefinition('time_authenticator', new Definition(
            'Acme\HelloBundle\Security\TimeAuthenticator',
            array(new Reference('security.encoder_factory'))
        ));

Quindi, attivarlo nella sezione ``firewalls`` della configurazione di sicurezza,
usando la voce ``simple_form``:

.. configuration-block::

    .. code-block:: yaml

        # app/config/security.yml
        security:
            # ...

            firewalls:
                secured_area:
                    pattern: ^/admin
                    # ...
                    simple_form:
                        authenticator: time_authenticator
                        check_path:    login_check
                        login_path:    login

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
                    >
                    <simple-form authenticator="time_authenticator"
                        check-path="login_check"
                        login-path="login"
                    />
                </firewall>
            </config>
        </srv:container>

    .. code-block:: php

        // app/config/security.php

        // ..

        $container->loadFromExtension('security', array(
            'firewalls' => array(
                'secured_area'    => array(
                    'pattern'     => '^/admin',
                    'simple_form' => array(
                        'provider'      => ...,
                        'authenticator' => 'time_authenticator',
                        'check_path'    => 'login_check',
                        'login_path'    => 'login',
                    ),
                ),
            ),
        ));

La voce ``simple_form`` ha le stesse opzioni della normale opzione ``form_login``,
ma con la voce aggiuntiva ``authenticator``, che punta al
nuovo servizio. Per dettagli, vedere :ref:`reference-security-firewall-form-login`.

Chi non avesse familiarità con i form di login in generale o non capisse le
opzioni ``check_path`` o ``login_path`` può vedere :doc:`/cookbook/security/form_login`.
