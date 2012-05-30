.. index::
   single: Richiesta; Aggiungere un formato di richiesta e un tipo mime

Registrare un nuovo formato di richiesta e un nuovo tipo mime
=============================================================

Ogni ``Richiesta`` ha a un "formato" (come ``html``, ``json``), che viene usato
per determinare il tipo di contenuto che dovrà essere restituito nell ``Risposta``.
Il formato della richiesta, accessibile tramite
:method:`Symfony\\Component\\HttpFoundation\\Request::getRequestFormat`,
viene infatti utilizzato per definire il tipo MIME dell'intestazione ``Content-Type`` 
dell'oggetto ``Risposta``. Symfony contiene una mappa dei formati più comuni (come 
``html``, ``json``) e del corrispettivo tipo MIME (come ``text/html``,
``application/json``). È comunque possibile aggiungere nuovi formati-MIME.
In questo documento si vedrà come aggiungere un nuovo formato ``jsonp``
e il corrispondente tipo MIME.

Creazione di un ascoltatore per il ``kernel.request``
-----------------------------------------------------

La chiave per definire un nuovo tipo MIME è creare una classe che rimarrà in ``ascolto``
dell'evento ``kernel.request`` emesso dal kernel di Symfony. L'evento ``kernel.request``
è emesso da Symfony nelle primissime fasi della gestione della richiesta
e permette di modificare l' oggetto richiesta.

Si crea una classe simile alla seguente, sostituendo i percorsi in modo che
puntino a un bundle del proprio progetto::

    // src/Acme/DemoBundle/RequestListener.php
    namespace Acme\DemoBundle;

    use Symfony\Component\HttpKernel\HttpKernelInterface;
    use Symfony\Component\HttpKernel\Event\GetResponseEvent;

    class RequestListener
    {
        public function onKernelRequest(GetResponseEvent $event)
        {
            $event->getRequest()->setFormat('jsonp', 'application/javascript');
        }
    }

Registrazione dell'ascoltatore
------------------------------

Come per ogni ascoltatore, è necessario aggiungere anche questo nel file di
configurazione e registrarlo come tale aggiungendogli il tag ``kernel.event_listener``:

.. configuration-block::

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <?xml version="1.0" ?>

        <container xmlns="http://symfony.com/schema/dic/services"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://symfony.com/schema/dic/services http://symfony.com/schema/dic/services/services-1.0.xsd">

            <service id="acme.demobundle.listener.request" class="Acme\DemoBundle\RequestListener">
                <tag name="kernel.event_listener" event="kernel.request" method="onKernelRequest" />
            </service>

        </container>

    .. code-block:: yaml

        # app/config/config.yml
        services:
            acme.demobundle.listener.request:
                class: Acme\DemoBundle\RequestListener
                tags:
                    - { name: kernel.event_listener, event: kernel.request, method: onKernelRequest }

    .. code-block:: php

        # app/config/config.php
        $definition = new Definition('Acme\DemoBundle\RequestListener');
        $definition->addTag('kernel.event_listener', array('event' => 'kernel.request', 'method' => 'onKernelRequest'));
        $container->setDefinition('acme.demobundle.listener.request', $definition);

A questo punto, il servizio ``acme.demobundle.listener.request`` è stato configurato
e verrà notificato dell'avvenuta emissione, da parte del kernel Symfony,
dell'evento ``kernel.request``.

.. tip::

    È possibile registrare l'ascoltatore anche in una classe di estensione della configurazione (si veda
    :ref:`service-container-extension-configuration` per ulteriori informazioni).
