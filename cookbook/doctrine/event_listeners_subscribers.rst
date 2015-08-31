.. index::
   single: Doctrine; Ascoltatori e sottoscrittori di eventi

.. _doctrine-event-config:

Registrare ascoltatori e sottoscrittori di eventi
=================================================

Doctrine include un ricco sistema di eventi, lanciati quasi ogni volta che accade
qualcosa nel sistema. Per lo sviluppatore, significa la possibilità di creare
:doc:`servizi </book/service_container>` arbitrari e dire a Doctrine di notificare
questi oggetti ogni volta che accade una certa azione (p.e. ``prePersist``).
Questo può essere utile, per esempio, per creare un indice di ricerca indipendente
ogni volta che un oggetto viene salvato nella base dati.

Doctrine definisce due tipi di oggetti che possono ascoltare eventi:
ascoltatori e sottoscrittori. Sono simili tra loro, ma gli ascoltatori sono leggermente
più semplificati. Per approfondimenti, vedere `The Event System`_ sul sito di Doctrine.

Il sito di Doctrine spiega anche tutti gli eventi esistenti che possono essere ascoltati.

Configurare ascoltatori e sottoscrittori
----------------------------------------

Per registrare un servizio come ascoltatore o sottoscrittore di eventi, basta assegnarli
il :ref:`tag <book-service-container-tags>` appropriato. A seconda del caso,
si può agganciare un ascoltatore a ogni connessione DBAL o gestore di entità
dell'ORM, oppure solo a una specifica connessione DBAL e a tutti i gestori di entità
che usano tale connessione.

.. configuration-block::

    .. code-block:: yaml

        doctrine:
            dbal:
                default_connection: default
                connections:
                    default:
                        driver: pdo_sqlite
                        memory: true

        services:
            my.listener:
                class: Acme\SearchBundle\EventListener\SearchIndexer
                tags:
                    - { name: doctrine.event_listener, event: postPersist }
            my.listener2:
                class: Acme\SearchBundle\EventListener\SearchIndexer2
                tags:
                    - { name: doctrine.event_listener, event: postPersist, connection: default }
            my.subscriber:
                class: Acme\SearchBundle\EventListener\SearchIndexerSubscriber
                tags:
                    - { name: doctrine.event_subscriber, connection: default }

    .. code-block:: xml

        <?xml version="1.0" ?>
        <container xmlns="http://symfony.com/schema/dic/services"
            xmlns:doctrine="http://symfony.com/schema/dic/doctrine">

            <doctrine:config>
                <doctrine:dbal default-connection="default">
                    <doctrine:connection driver="pdo_sqlite" memory="true" />
                </doctrine:dbal>
            </doctrine:config>

            <services>
                <service id="my.listener" class="Acme\SearchBundle\EventListener\SearchIndexer">
                    <tag name="doctrine.event_listener" event="postPersist" />
                </service>
                <service id="my.listener2" class="Acme\SearchBundle\EventListener\SearchIndexer2">
                    <tag name="doctrine.event_listener" event="postPersist" connection="default" />
                </service>
                <service id="my.subscriber" class="Acme\SearchBundle\EventListener\SearchIndexerSubscriber">
                    <tag name="doctrine.event_subscriber" connection="default" />
                </service>
            </services>
        </container>

    .. code-block:: php

        use Symfony\Component\DependencyInjection\Definition;

        $container->loadFromExtension('doctrine', array(
            'dbal' => array(
                'default_connection' => 'default',
                'connections' => array(
                    'default' => array(
                        'driver' => 'pdo_sqlite',
                        'memory' => true,
                    ),
                ),
            ),
        ));

        $container
            ->setDefinition(
                'my.listener',
                new Definition('Acme\SearchBundle\EventListener\SearchIndexer')
            )
            ->addTag('doctrine.event_listener', array('event' => 'postPersist'))
        ;
        $container
            ->setDefinition(
                'my.listener2',
                new Definition('Acme\SearchBundle\EventListener\SearchIndexer2')
            )
            ->addTag('doctrine.event_listener', array('event' => 'postPersist', 'connection' => 'default'))
        ;
        $container
            ->setDefinition(
                'my.subscriber',
                new Definition('Acme\SearchBundle\EventListener\SearchIndexerSubscriber')
            )
            ->addTag('doctrine.event_subscriber', array('connection' => 'default'))
        ;

Creare la classe dell'ascoltatore
---------------------------------

Nell'esempio precedente, è stato configurato un servizio ``my.listener`` come ascoltatore
dell'evento ``postPersist``. La classe dietro al servizio deve avere un metodo
``postPersist``, che sarà richiamato al lancio dell'evento::

    // src/Acme/SearchBundle/EventListener/SearchIndexer.php
    namespace Acme\SearchBundle\EventListener;

    use Doctrine\ORM\Event\LifecycleEventArgs;
    use Acme\StoreBundle\Entity\Product;

    class SearchIndexer
    {
        public function postPersist(LifecycleEventArgs $args)
        {
            $entity = $args->getEntity();
            $entityManager = $args->getEntityManager();

            // si potrebbe voler fare qualcosa su un'entità Product
            if ($entity instanceof Product) {
                // ... fare qualcosa con l'oggetto Product
            }
        }
    }

In ciascun evento, si ha accesso all'oggetto ``LifecycleEventArgs``, che rende
disponibili sia l'oggetto entità dell'evento che lo stesso gestore di
entità.

Una cosa importante da notare è che un ascoltatore ascolterà *tutte* le
entità dell'applicazione. Quindi, se si vuole gestire solo un tipo
specifico di entità (p.e. un'entità ``Product``, ma non un'entità ``BlogPost``),
si dovrebbe verificare il nome della classe dell'entità nel metodo
(come precedentemente mostrato).

Creare la classe Subscriber
---------------------------

Un sottoscrittore di eventi Doctrine deve implementare l'interfaccia ``Doctrine\Common\EventSubscriber``
e avere un metodo per ogni evento sottoscritto::

    // src/Acme/SearchBundle/EventListener/SearchIndexerSubscriber.php
    namespace Acme\SearchBundle\EventListener;

    use Doctrine\Common\EventSubscriber;
    use Doctrine\ORM\Event\LifecycleEventArgs;
    // per Doctrine 2.4: Doctrine\Common\Persistence\Event\LifecycleEventArgs;
    use Acme\StoreBundle\Entity\Product;

    class SearchIndexerSubscriber implements EventSubscriber
    {
        public function getSubscribedEvents()
        {
            return array(
                'postPersist',
                'postUpdate',
            );
        }

        public function postUpdate(LifecycleEventArgs $args)
        {
            $this->index($args);
        }

        public function postPersist(LifecycleEventArgs $args)
        {
            $this->index($args);
        }

        public function index(LifecycleEventArgs $args)
        {
            $entity = $args->getEntity();
            $entityManager = $args->getEntityManager();

            // forse si vuole agire solo su un'entità "Product"
            if ($entity instanceof Product) {
                // ... fare qualcosa con Product
            }
        }
    }

.. tip::

    I sottoscrittori di eventi Doctrine non possono restituire un array flessibile di metodi da
    richiamare per gli eventi, come invece possono fare i :ref:`sottoscrittore di eventi Symfony <event_dispatcher-using-event-subscribers>`.
    I sottoscrittori di eventi Doctrine devono restituire un semplice array di nomi di eventi
    sottoscritti. Doctrine quindi si aspetterà metodi del sottoscrittore
    con lo stesso nome di ciascun evento sottoscritto, proprio come quando si usa un ascoltatore di eventi.

Per ulteriori riferimenti, si veda il capitolo `The Event System`_ nella documentazione di Doctrine.

.. _`The Event System`: http://docs.doctrine-project.org/projects/doctrine-orm/en/latest/reference/events.html
