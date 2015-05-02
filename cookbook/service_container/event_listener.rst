.. index::
   single: Eventi; Creare un ascoltatore

Creare un ascoltatore di eventi
===============================

Symfony dispone di vari eventi e agganci, che si possono usare per far scattare comportamenti
personalizzati in un'applicazione. Questi eventi sono lanciati dal componente HttpKernel 
e possono essere visualizzati nella classe :class:`Symfony\\Component\\HttpKernel\\KernelEvents`. 

Per agganciarsi a un evento e aggiungere la propria logica, occorre creare un servizio
che agisca da ascoltatore su tale evento. In questa ricetta, creeremo un servizio
che agirà da ascoltatore di eccezioni, consentendo di modificare il modo in cui sono
mostrare le eccezioni nella nostra applicazione. L'evento ``KernelEvents::EXCEPTION``
è solo uno degli eventi del nucleo::

    // src/AppBundle/EventListener/AcmeExceptionListener.php
    namespace AppBundle\EventListener;

    use Symfony\Component\HttpKernel\Event\GetResponseForExceptionEvent;
    use Symfony\Component\HttpFoundation\Response;
    use Symfony\Component\HttpKernel\Exception\HttpExceptionInterface;

    class AcmeExceptionListener
    {
        public function onKernelException(GetResponseForExceptionEvent $event)
        {
            // Prende l'oggetto eccezione dall'evento ricevuto
            $exception = $event->getException();
            $message = sprintf(
                'Il mio errore dice: %s con codice: %s',
                $exception->getMessage(),
                $exception->getCode()
            );

            // Personalizza l'oggetto risposta per mostrare i dettagli sull'eccezione
            $response = new Response();
            $response->setContent($message);

            // HttpExceptionInterface è un tipo speciale di eccezione, che
            // contiene il codice di stato e altri dettagli sugli header
            if ($exception instanceof HttpExceptionInterface) {
                $response->setStatusCode($exception->getStatusCode());
                $response->headers->replace($exception->getHeaders());
            } else {
                $response->setStatusCode(500);
            }

            // Invia la risposta modificata all'evento
            $event->setResponse($response);
        }
    }

.. tip::

    Ciascun ascoltatore riceve un tipo leggermente diverso di oggetto ``$event``. Per esempio,
    l'evento ``kernel.exception`` è un :class:`Symfony\\Component\\HttpKernel\\Event\\GetResponseForExceptionEvent`.
    Per vedere il tipo di oggetto ricevuto da un ascoltatore, si veda :class:`Symfony\\Component\\HttpKernel\\KernelEvents`.

.. note::

    Quando si imposta una risposta per gli eventi ``kernel.request``, ``kernel.view`` o
    ``kernel.exception``, la propagazione si ferma, quindi gli ascoltatori dello stesso
    evento con priorità inferiore non saranno richiamati.

Dopo aver creato la classe, basta registrarla come servizio e notificare a Symfony
che è un ascoltatore dell'evento ``kernel.exception``, usando un particolare
tag:

.. configuration-block::

    .. code-block:: yaml

        # app/config/services.yml
        services:
            kernel.listener.nome_ascoltatore:
                class: AppBundle\EventListener\AcmeExceptionListener
                tags:
                    - { name: kernel.event_listener, event: kernel.exception, method: onKernelException }

    .. code-block:: xml

        <!-- app/config/services.xml -->
        <service id="kernel.listener.nome_ascoltatore" class="AppBundle\EventListener\AcmeExceptionListener">
            <tag name="kernel.event_listener" event="kernel.exception" method="onKernelException" />
        </service>

    .. code-block:: php

        // app/config/services.php
        $container
            ->register('kernel.listener.nome_ascoltatore', 'AppBundle\EventListener\AcmeExceptionListener')
            ->addTag('kernel.event_listener', array('event' => 'kernel.exception', 'method' => 'onKernelException'))
        ;

.. note::

    C'è un'ulteriore opzione del tag, ``priority``, facoltativa e con valore predefinito 0. Gli
    ascoltatori saranno eseguiti con un ordine basato sulla loro priorità (dalla più alta alla più bassa).
    Questo è utile quando occorre assicurarsi che un ascoltatore sia eseguito prima di un altro.

Eventi richiesta, verifica dei tipi
-----------------------------------

Una singola pagina può eseguire diverse richieste (una principale e poi diverse
sotto-richieste); per questo, quando si ha a che fare con l'evento ``KernelEvents::REQUEST``,
si potrebbe voler verificare il tipo di richiesta. Lo si può fare facilmente,
come segue::

    // src/AppBundle/EventListener/AcmeRequestListener.php
    namespace AppBundle\EventListener;

    use Symfony\Component\HttpKernel\Event\GetResponseEvent;
    use Symfony\Component\HttpKernel\HttpKernel;

    class AcmeRequestListener
    {
        public function onKernelRequest(GetResponseEvent $event)
        {
            if (HttpKernel::MASTER_REQUEST != $event->getRequestType()) {
                // non fare niente se non si è nella richiesta principale
                return;
            }

            // ...
        }
    }

.. tip::

    Sono disponibili due tipi di richiesta nell'interfaccia :class:`Symfony\\Component\\HttpKernel\\HttpKernelInterface`:
    ``HttpKernelInterface::MASTER_REQUEST`` e
    ``HttpKernelInterface::SUB_REQUEST``.
