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

Per simulare che il locale sia memorizzato in sessione, occorre creare r
registrare un :doc:`ascoltatore di eventi</cookbook/service_container/event_listener>`.
L'ascoltatore sarà simile al seguente. Tipicamente, ``_locale`` è usato
come parametro di rotta per indicare il locale, sebbene non sia veramente importante
il modo in cui si ricavi il locale dalla richiesta::

    // src/Acme/LocaleBundle/EventListener/LocaleListener.php
    namespace Acme\LocaleBundle\EventListener;

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
            acme_locale.locale_listener:
                class: Acme\LocaleBundle\EventListener\LocaleListener
                arguments: ["%kernel.default_locale%"]
                tags:
                    - { name: kernel.event_subscriber }

    .. code-block:: xml

        <service id="acme_locale.locale_listener"
            class="Acme\LocaleBundle\EventListener\LocaleListener">
            <argument>%kernel.default_locale%</argument>

            <tag name="kernel.event_subscriber" />
        </service>

    .. code-block:: php

        use Symfony\Component\DependencyInjection\Definition;

        $container
            ->setDefinition('acme_locale.locale_listener', new Definition(
                'Acme\LocaleBundle\EventListener\LocaleListener',
                array('%kernel.default_locale%')
            ))
            ->addTag('kernel.event_subscriber')
        ;

Ecco fatto! Si può ora provare a modificare il locale dell'utente e vedere
che resta invaariato tra le richieste. Ricordarsi di usare sempre il metodo
use the :method:`Request::getLocale<Symfony\\Component\\HttpFoundation\\Request::getLocale>`
per ottenere il locale dell'utente::

    // da un controllore...
    $locale = $this->getRequest()->getLocale();
