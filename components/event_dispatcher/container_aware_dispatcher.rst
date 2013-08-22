.. index::
   single: Distributore di eventi; Consapevolezza del contenitore; Dependency Injection; DIC

Il distributore consapevole del contenitore
===========================================

Introduzione
------------

La classe :class:`Symfony\\Component\\EventDispatcher\\ContainerAwareEventDispatcher` è
una speciale implementazione di distributore di eventi, accoppiata con il contenitore di servizi,
che fa parte del :doc:`the Dependency Injection component</components/dependency_injection/introduction>`.
Questo consente di specificare i servizi come ascoltatori di eventi, rendendo il distributore
di eventi molto potente.

Si servizi sono caricati in modo pigro, il che vuol dire che i servizi allegati come ascoltatori
saranno creato solo se viene distribuito un evento che richieda tali ascoltatori.

Preparazione
------------

La preparazione è molto semplice, basta iniettare un :class:`Symfony\\Component\\DependencyInjection\\ContainerInterface`
in :class:`Symfony\\Component\\EventDispatcher\\ContainerAwareEventDispatcher`::

    use Symfony\Component\DependencyInjection\ContainerBuilder;
    use Symfony\Component\EventDispatcher\ContainerAwareEventDispatcher;

    $container = new ContainerBuilder();
    $dispatcher = new ContainerAwareEventDispatcher($container);

Aggiungere ascoltatori
----------------------

Il *distributore di eventi consapevole del contenitore* può caricare direttamente servizi
specifici, oppure servizi che implementino :class:`Symfony\\Component\\EventDispatcher\\EventSubscriberInterface`.

Gli esempi seguenti presumo che il DIC sia stato caricato con i servizi che
vengono menzionati.

.. note::

    I servizi devono essere segnati come pubblici nel DIC.

Aggiungere servizi
~~~~~~~~~~~~~~~~~~

Per collegare definizioni di servizi esistenti, usare il metodo
:method:`Symfony\\Component\\EventDispatcher\\ContainerAwareEventDispatcher::addListenerService`,
dove ``$callback`` è un array ``array($idServizio, $nomeMetodo)``::

    $dispatcher->addListenerService($eventName, array('pippo', 'logListener'));

Aggiungere servizi sottoscrittori
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Si possono aggiungere degli ``EventSubscribers``, usando il metodo
:method:`Symfony\\Component\\EventDispatcher\\ContainerAwareEventDispatcher::addSubscriberService`,
dove il primo parametro è l'ID del servizio sottoscrittore e il secondo
parametro è il nome della classe del servizio (che deve implementare
:class:`Symfony\\Component\\EventDispatcher\\EventSubscriberInterface`), come segue::

    $dispatcher->addSubscriberService(
        'kernel.store_subscriber',
        'StoreSubscriber'
    );

``EventSubscriberInterface`` sarà esattamente come ci si può aspettare::

    use Symfony\Component\EventDispatcher\EventSubscriberInterface;
    // ...

    class StoreSubscriber implements EventSubscriberInterface
    {
        public static function getSubscribedEvents()
        {
            return array(
                'kernel.response' => array(
                    array('onKernelResponsePre', 10),
                    array('onKernelResponsePost', 0),
                ),
                'store.order'     => array('onStoreOrder', 0),
            );
        }

        public function onKernelResponsePre(FilterResponseEvent $event)
        {
            // ...
        }

        public function onKernelResponsePost(FilterResponseEvent $event)
        {
            // ...
        }

        public function onStoreOrder(FilterOrderEvent $event)
        {
            // ...
        }
    }
