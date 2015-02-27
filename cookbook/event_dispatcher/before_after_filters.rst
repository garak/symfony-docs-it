.. index::
   single: Event Dispatcher

Impostare filtri prima e dopo
=============================

È molto comune, durante lo sviluppo di un'applicazione web, aver bisogno di eseguire un
po' di logica subito prima o subito dopo che l'azione di un controllore abbia agito da
filtro o da hook.

In Symfony1 lo si poteva fare con i metodi preExecute e postExecute e ci sono metodi
simili in molti grossi framework, ma non in Symfony.
La buona notizia è che c'è un modo molto migliore per intervenire nel processo
richiesta/risposta, usando il componente EventDispatcher.

Esempio di validazione di un token
----------------------------------

Si immagini di dover sviluppare un'API in cui alcuni controllori sono pubblici e altri
sono riservati a uno o più client. Per queste caratteristiche private, si potrebbe
voler fornire un token ai clienti, in modo che si possano autenticare.

Quindi, prima di eseguire l'azione del controllore, occorre verificare se l'azione
sia riservata o meno. Se lo è, occorre validare il token
fornito.

.. note::

    Si noti che per semplicità, in questa ricetta i token saranno definiti nella
    configurazione. Non saranno usate basi di dati né un fornitore di autenticazione
    tramite il componente della sicurezza.

Pre-filtri con l'evento controller.request
------------------------------------------

Innanzitutto, impostare una configurazione di base per il token, usando ``config.yml`` e i
parametri:

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        parameters:
            tokens:
                client1: pass1
                client2: pass2

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <parameters>
            <parameter key="tokens" type="collection">
                <parameter key="client1">pass1</parameter>
                <parameter key="client2">pass2</parameter>
            </parameter>
        </parameters>

    .. code-block:: php

        // app/config/config.php
        $container->setParameter('tokens', array(
            'client1' => 'pass1',
            'client2' => 'pass2',
        ));

Controllori da verificare
-------------------------

Un ascoltatore ``kernel.controller`` riceve una notifica a ogni richiesta, appena prima
dell'esecuzione del controllore. Occorre innanzitutto un qualche modo per verificare se
il controllore corrispondente alla richiesta abbia bisogno di validare il token.

Un modo semplice e pulito è quello di creare un'interfaccia vuota e farla implementare
ai controllori::

    namespace Acme\DemoBundle\Controller;

    interface TokenAuthenticatedController
    {
        // ...
    }

Un controllore che implementa tale interfaccia assomiglia a questo::

    namespace Acme\DemoBundle\Controller;

    use Acme\DemoBundle\Controller\TokenAuthenticatedController;
    use Symfony\Bundle\FrameworkBundle\Controller\Controller;

    class FooController extends Controller implements TokenAuthenticatedController
    {
        // Le azioni che necessitano di autenticazione
        public function barAction()
        {
            // ...
        }
    }

Creare un ascoltatore di eventi
-------------------------------

Occorre quindi creare un ascoltatore di eventi, che conterrà la logica che si vuole
eseguire prima dei controllori. Se non si ha familiarità con gli ascoltatori di
eventi, si possono ottenere maggiori informazioni su :doc:`/cookbook/service_container/event_listener`::

    // src/Acme/DemoBundle/EventListener/TokenListener.php
    namespace Acme\DemoBundle\EventListener;

    use Acme\DemoBundle\Controller\TokenAuthenticatedController;
    use Symfony\Component\HttpKernel\Exception\AccessDeniedHttpException;
    use Symfony\Component\HttpKernel\Event\FilterControllerEvent;

    class TokenListener
    {
        private $tokens;

        public function __construct($tokens)
        {
            $this->tokens = $tokens;
        }

        public function onKernelController(FilterControllerEvent $event)
        {
            $controller = $event->getController();

            /*
             * $controller passato può essere una classe o una Closure. Non è frequente in Symfony ma può accadere.
             * Se è una classe, è in formato array
             */
            if (!is_array($controller)) {
                return;
            }

            if($controller[0] instanceof TokenAuthenticatedController) {
                $token = $event->getRequest()->query->get('token');
                if (!in_array($token, $this->tokens)) {
                    throw new AccessDeniedHttpException('Questa azione ha bisogno di un token valido!');
                }
            }
        }
    }

Registrare l'ascoltatore
------------------------

Infine, registrare l'ascoltatore come servizio e assegnargli il tag di ascoltatore di eventi.
Ascoltando ``kernel.controller``, si sta dicendo a  Symfony che si vuole che l'ascoltatore
sia richiamato appena prima l'esecuzione di ogni controllore:

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml (oppure dentro services.yml)
        services:
            demo.tokens.action_listener:
                class: Acme\DemoBundle\EventListener\TokenListener
                arguments: [ %tokens% ]
                tags:
                    - { name: kernel.event_listener, event: kernel.controller, method: onKernelController }

    .. code-block:: xml

        <!-- app/config/config.xml (oppure dentro services.xml) -->
        <service id="demo.tokens.action_listener" class="Acme\DemoBundle\EventListener\TokenListener">
            <argument>%tokens%</argument>
            <tag name="kernel.event_listener" event="kernel.controller" method="onKernelController" />
        </service>

    .. code-block:: php

        // app/config/config.php (oppure dentro services.php)
        use Symfony\Component\DependencyInjection\Definition;

        $listener = new Definition('Acme\DemoBundle\EventListener\TokenListener', array('%tokens%'));
        $listener->addTag('kernel.event_listener', array(
            'event'  => 'kernel.controller',
            'method' => 'onKernelController'
        ));
        $container->setDefinition('demo.tokens.action_listener', $listener);

Con questa configurazione, il metodo ``onKernelController`` di ``TokenListener`` 
sarà eseguito a ogni richiesta. Se il controllore che sta per essere eseguito
implementa ``TokenAuthenticatedController``, si applica l'autenticazione con
token. Questo consente di avere un pre-filtro su ogni controllore
desiderato.

Post-filtri con l'evento ``kernel.response``
--------------------------------------------

Oltre ad avere un "aggancio" eseguito prima del controllore, si può anche
aggiungere un aggancio da eseguire *dopo* il controllore. Per questo esempio,
immaginiamo di voler aggiungere un hash sha1 (con un sale che usi quel token) a
tutte le rispose che hanno passato questa autenticazione con token.

C'è un altro evento del nucleo di Symfony, chiamato ``kernel.response``, che viene
notificato a ogni richiesta, ma dopo che il controllore ha restituito un oggetto Response.
Creare un post-filtro è facile, basta creare una classe ascoltatore e registrarla come
servizio su tale evento.

Per esempio, si prenda ``TokenListener`` dell'esempio precedente e si registri prima
il token di autenticazione negli attributi della richiesta. Questo servirà come
indicatore di base che tale richiesta ha subito un'autenticazione con token::

    public function onKernelController(FilterControllerEvent $event)
    {
        // ...

        if ($controller[0] instanceof TokenAuthenticatedController) {
            $token = $event->getRequest()->query->get('token');
            if (!in_array($token, $this->tokens)) {
                throw new AccessDeniedHttpException('Questa azione necessita di un token valido!');
            }

            // segna che la richiesta ha passato l'autenticazione con token
            $event->getRequest()->attributes->set('auth_token', $token);
        }
    }

Ora, aggiungere un altro metodo alla classe, ``onKernelResponse``, che cerca l'indicatore
nell'oggetto richiesta e imposta un header personalizzato nella risposta, se lo
trova::

    // aggiungere la nuova istruzione "use" in cima al file
    use Symfony\Component\HttpKernel\Event\FilterResponseEvent;

    public function onKernelResponse(FilterResponseEvent $event)
    {
        // verifica se onKernelController ha segnato la richiesta come autenticata
        if (!$token = $event->getRequest()->attributes->get('auth_token')) {
            return;
        }

        $response = $event->getResponse();

        // crea un hash e lo imposta come header della risposta
        $hash = sha1($response->getContent().$token);
        $response->headers->set('X-CONTENT-HASH', $hash);
    }

Infine, occorre un secondo tag nella definizione del servizio, per dire a Symfony
di notificare l'evento ``onKernelResponse`` per l'evento
``kernel.response``:

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml (oppure dentro services.yml)
        services:
            demo.tokens.action_listener:
                class: Acme\DemoBundle\EventListener\TokenListener
                arguments: [ %tokens% ]
                tags:
                    - { name: kernel.event_listener, event: kernel.controller, method: onKernelController }
                    - { name: kernel.event_listener, event: kernel.response, method: onKernelResponse }

    .. code-block:: xml

        <!-- app/config/config.xml (oppure dentro services.xml) -->
        <service id="demo.tokens.action_listener" class="Acme\DemoBundle\EventListenerTokenListener">
            <argument>%tokens%</argument>
            <tag name="kernel.event_listener" event="kernel.controller" method="onKernelController" />
            <tag name="kernel.event_listener" event="kernel.response" method="onKernelResponse" />
        </service>

    .. code-block:: php

        // app/config/config.php (oppure dentro services.php)
        use Symfony\Component\DependencyInjection\Definition;

        $listener = new Definition('Acme\DemoBundle\EventListener\TokenListener', array('%tokens%'));
        $listener->addTag('kernel.event_listener', array(
            'event'  => 'kernel.controller',
            'method' => 'onKernelController'
        ));
        $listener->addTag('kernel.event_listener', array(
            'event'  => 'kernel.response',
            'method' => 'onKernelResponse'
        ));
        $container->setDefinition('demo.tokens.action_listener', $listener);

Ecco fatto! Ora ``TokenListener`` sarà notificato prima di ogni esecuzione di un
controllore (``onKernelController``) e dopo che ogni controllore ha restituito una risposta
(``onKernelResponse``). Facendo implementare ai controllori l'interfaccia ``TokenAuthenticatedController``,
i nostri ascoltatori sanno quale controllore deve occuparsene.
Inoltre, memorizzando un valore tra gli attributi della richiesta, il metodo ``onKernelResponse``
sa che deve aggiungere un header in più. Buon divertimento!
