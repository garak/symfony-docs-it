.. index::
   single: Event Dispatcher

Come impostare filtri prima e dopo
==================================

È molto comune, durante lo sviluppo di un'applicazione web, aver bisogno di eseguire un
po' di logica subito prima o subito dopo che l'azione di un controllore abbia agito da
filtro o da hook.

In Symfony1 lo si poteva fare con i metodi preExecute e postExecute e ci sono metodi
simili in molti grossi framework, ma non in Symfony2.
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

Creare un pre-filtro con un evento controller.request
-----------------------------------------------------

Impostazioni di base
~~~~~~~~~~~~~~~~~~~~

Si puà aggiungere una configurazione di base per il token, usando ``config.yml`` e i parametri:

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
            'client2' => 'pass2'
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
        // Niente
    }

Un controllore che implementa tale interfaccia assomiglia a questo::

    class FooController implements TokenAuthenticatedController
    {
        // Le azioni che necessitano di autenticazione
    }

Creare un ascoltatore di eventi
-------------------------------

Occorre quindi creare un ascoltatore di eventi, che conterrà la logica che si vuole
eseguire prima dei controllori. Se non si ha familiarità con gli ascoltatori di
eventi, si possono ottenere maggiori informazioni su :doc:`/cookbook/service_container/event_listener`::

    namespace Acme\DemoBundle\EventListener;

    use Acme\DemoBundle\Controller\TokenAuthenticatedController;
    use Symfony\Component\HttpKernel\Exception\AccessDeniedHttpException;
    use Symfony\Component\HttpKernel\Event\FilterControllerEvent;

    class BeforeListener
    {
        private $tokens;

        public function __contruct($tokens)
        {
            $this->tokens = $tokens;
        }

        public function onKernelController(FilterControllerEvent $event)
        {
            $controller = $event->getController();

            /*
             * $controller passato può essere una classe o una Closure. Non è frequente in Symfony2 ma può accadere.
             * Se è una classe, è in formato array
             */
            if (!is_array($controller)) {
                return;
            }

            if($controller[0] instanceof TokenAuthenticatedController) {
                $token = $event->getRequest()->get('token');
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
              class: Acme\DemoBundle\EventListener\BeforeListener
              arguments: [ %tokens% ]
              tags:
                    - { name: kernel.event_listener, event: kernel.controller, method: onKernelController }

    .. code-block:: xml

        <service id="demo.tokens.action_listener" class="Acme\DemoBundle\EventListener\BeforeListener">
            <argument>%tokens%</argument>
            <tag name="kernel.event_listener" event="kernel.controller" method="onKernelController" />
        </service>

    .. code-block:: php

        use Symfony\Component\DependencyInjection\Definition;

        $listener = new Definition('Acme\DemoBundle\EventListener\BeforeListener', array('%tokens%'));
        $listener->addTag('kernel.event_listener', array('event' => 'kernel.controller', 'method' => 'onKernelController'));
        $container->setDefinition('demo.tokens.action_listener', $listener);

