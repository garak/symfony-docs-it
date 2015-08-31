.. index::
    single: Sessioni, salvare il locale

Far persistere il locale durante la sessione utente
===================================================

Prima di Symfony 2.1, il locale veniva memorizzato in una variabile di sessione chiamata ``_locale``.
Dalla versione 2.1, viene memorizzato nella richiesta, il che vuol dire che non "persiste"
tra una richiesta e l'altra. In questa ricetta, vedremo come fare in modo che il locale
"persista", in modo che, una volta impostato, sia usato per ogni richiesta
successiva.

Creazione di LocaleListener
---------------------------

Per simulare che il locale sia memorizzato in sessione, occorre creare e
registrare un :doc:`ascoltatore di eventi </cookbook/service_container/event_listener>`.
L'ascoltatore sarà simile al seguente. Tipicamente, ``_locale`` è usato
come parametro di rotta per indicare il locale, sebbene non sia veramente importante
il modo in cui si ricavi il locale dalla richiesta::

    // src/AppBundle/EventListener/LocaleListener.php
    namespace AppBundle\EventListener;

    use Symfony\Component\HttpKernel\Event\GetResponseEvent;
    use Symfony\Component\HttpKernel\KernelEvents;
    use Symfony\Component\EventDispatcher\EventSubscriberInterface;

    class LocaleListener implements EventSubscriberInterface
    {
        private $defaultLocale;

        public function __construct($defaultLocale = 'en')
        {
            $this->defaultLocale = $defaultLocale;
        }

        public function onKernelRequest(GetResponseEvent $event)
        {
            $request = $event->getRequest();
            if (!$request->hasPreviousSession()) {
                return;
            }

            // prova a vedere se il locale sia stato impostato come parametro _locale di una rotta
            if ($locale = $request->attributes->get('_locale')) {
                $request->getSession()->set('_locale', $locale);
            } else {
                // se non trova un locale esplicito in questa richiesta, usa quello della sessione
                $request->setLocale($request->getSession()->get('_locale', $this->defaultLocale));
            }
        }

        public static function getSubscribedEvents()
        {
            return array(
                // deve essere registrato prima dell'ascoltatore predefinito di locale
                KernelEvents::REQUEST => array(array('onKernelRequest', 17)),
            );
        }
    }

Registrare l'ascoltatore:

.. configuration-block::

    .. code-block:: yaml

        services:
            app.locale_listener:
                class: AppBundle\EventListener\LocaleListener
                arguments: ["%kernel.default_locale%"]
                tags:
                    - { name: kernel.event_subscriber }

    .. code-block:: xml

        <service id="app.locale_listener"
            class="AppBundle\EventListener\LocaleListener">
            <argument>%kernel.default_locale%</argument>

            <tag name="kernel.event_subscriber" />
        </service>

    .. code-block:: php

        use Symfony\Component\DependencyInjection\Definition;

        $container
            ->setDefinition('app.locale_listener', new Definition(
                'AppBundle\EventListener\LocaleListener',
                array('%kernel.default_locale%')
            ))
            ->addTag('kernel.event_subscriber')
        ;

Ecco fatto! Si può ora provare a modificare il locale dell'utente e vedere
che resta invariato tra le richieste. Ricordarsi di usare sempre il metodo
use the :method:`Request::getLocale<Symfony\\Component\\HttpFoundation\\Request::getLocale>`
per ottenere il locale dell'utente::

    // da un controllore...
    use Symfony\Component\HttpFoundation\Request;

    public function indexAction(Request $request)
    {
        $locale = $request->getLocale();
    }

Impostare il locale in base alle preferenze dell'utente
-------------------------------------------------------

Si potrebbe voler migliorare questa tecnica ulteriormente, definendo il locale in base all'entità
dell'utente o all'utente connesso. Tuttavia, poiché ``LocaleListener`` è richiamato
prima di ``FirewallListener``, che si occupa di gestira l'autenticazione e
impostare il token dell'utente su ``TokenStorage``, non si può accedere
all'utente connesso.

Si supponga di aver definito una proprietà ``locale`` nella propria entità ``User`` e di
volerla usare come locale per l'utente dato. Per arrivare allo scopo,
ci si può agganciare al processo di login e aggiornare la sessione dell'utente con questo
valore di locale, prima che sia rinviato alla prima pagina.

Per poterlo fare, occorre un ascoltare di eventi per l'evento ``security.interactive_login``:


.. code-block:: php

    // src/AppBundle/EventListener/UserLocaleListener.php
    namespace AppBundle\EventListener;

    use Symfony\Component\HttpFoundation\Session\Session;
    use Symfony\Component\Security\Http\Event\InteractiveLoginEvent;

    /**
     * Memorizza il locale dell'utente in sessione, dopo il
     * login. Potrà quindi essere usato da LocaleListener.
     */
    class UserLocaleListener
    {
        /**
         * @var Session
         */
        private $session;

        public function __construct(Session $session)
        {
            $this->session = $session;
        }

        /**
         * @param InteractiveLoginEvent $event
         */
        public function onInteractiveLogin(InteractiveLoginEvent $event)
        {
            $user = $event->getAuthenticationToken()->getUser();

            if (null !== $user->getLocale()) {
                $this->session->set('_locale', $user->getLocale());
            }
        }
    }

Registrare quindi l'ascoltatore:

.. configuration-block::

    .. code-block:: yaml

        # app/config/services.yml
        services:
            app.user_locale_listener:
                class: AppBundle\EventListener\UserLocaleListener
                arguments: [@session]
                tags:
                    - { name: kernel.event_listener, event: security.interactive_login, method: onInteractiveLogin }

    .. code-block:: xml

        <!-- app/config/services.xml -->
        <?xml version="1.0" encoding="UTF-8" ?>
        <container xmlns="http://symfony.com/schema/dic/services"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://symfony.com/schema/dic/services
                http://symfony.com/schema/dic/services/services-1.0.xsd">

            <services>
                <service id="app.user_locale_listener"
                    class="AppBundle\EventListener\UserLocaleListener">

                    <argument type="service" id="session"/>

                    <tag name="kernel.event_listener"
                        event="security.interactive_login"
                        method="onInteractiveLogin" />
                </service>
            </services>
        </container>

    .. code-block:: php

        // app/config/services.php
        $container
            ->register('app.user_locale_listener', 'AppBundle\EventListener\UserLocaleListener')
            ->addArgument('session')
            ->addTag(
                'kernel.event_listener',
                array('event' => 'security.interactive_login', 'method' => 'onInteractiveLogin'
            );

.. caution::

    Per poter aggiornare la lingua immediatamente dopo che un utente ha modificato
    le sue preferenze, si deve aggiornare la sessione dopo un aggiornamento
    dell'entità  ``User``.
