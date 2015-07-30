.. index::
   single: DependencyInjection; Factory

Usare un factory per creare servizi
===================================

Il contenitore di servizi di Symfony fornisce un modo potente per controllare la
creazione di oggetti, consentendo di specificare parametri da passare al costruttore,
così come chiamate a metodi e impostazioni di parametri. A volte, tuttavia, non fornisce
tutto ciò che è necessario per costruire gli oggetti.
Per tali situazioni, si può usare un factory per creare oggetti e dire al contenitore di
servizi di richiamare un metodo del factory, invece che istanziare direttamente
l'oggetto.

Si supponga di avere un factory che configura e restituisce un nuovo oggetto
``NewsletterManager``::

    class NewsletterManagerFactory
    {
        public static function createNewsletterManager()
        {
            $newsletterManager = new NewsletterManager();

            // ...

            return $newsletterManager;
        }
    }

Per rendere l'oggetto ``NewsletterManager`` disponibile come servizio, si può configurare 
il contenitore di servizi per usare la classe factory
``NewsletterManagerFactory``:

.. configuration-block::

    .. code-block:: yaml

        services:
            newsletter_manager:
                class:          NewsletterManager
                factory_class:  NewsletterManagerFactory
                factory_method: createNewsletterManager

    .. code-block:: xml

        <?xml version="1.0" encoding="UTF-8" ?>
        <container xmlns="http://symfony.com/schema/dic/services"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://symfony.com/schema/dic/services http://symfony.com/schema/dic/services/services-1.0.xsd">

            <services>
                <service
                    id="newsletter_manager"
                    class="NewsletterManager"
                    factory-class="NewsletterManagerFactory"
                    factory-method="createNewsletterManager" />
            </services>
        </services>

    .. code-block:: php

        use Symfony\Component\DependencyInjection\Definition;

        // ...
        $definition = new Definition('NewsletterManager');
        $definition->setFactoryClass('NewsletterManagerFactory');
        $definition->setFactoryMethod('createNewsletterManager');

        $container->setDefinition('newsletter_manager', $definition);

.. note::

    Quando si usa un factory per creare servizi, il valore scelto per l'opzione ``class``
    non ha effeetto sul servizio risultante. Il nome effettivo della classe dipende solo
    dall'oggetto restituito dal factory. Tuttavia, il nome configurato della classe
    può essere usato dai passi di compilatore e quindi dovrebbe essere impostato con
    un valore adeguato.

Quando si specificare la classe da usare come factory (tramite ``factory_class``),
il metodo sarà richiamato staticamente. Se il factory stesso va istanziato e il
metodo dell'oggetto risultante richiamato, configurare il factory stesso come servizio:
In questo caso, il metodo (p.e. ``createNewsletterManager``) va cambiato per non
essere statico:

.. configuration-block::

    .. code-block:: yaml

        services:
            newsletter_manager_factory:
                class:            NewsletterManagerFactory
            newsletter_manager:
                class:            NewsletterManager
                factory_service:  newsletter_manager_factory
                factory_method:   createNewsletterManager

    .. code-block:: xml

        <?xml version="1.0" encoding="UTF-8" ?>
        <container xmlns="http://symfony.com/schema/dic/services"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://symfony.com/schema/dic/services http://symfony.com/schema/dic/services/services-1.0.xsd">

            <services>
                <service id="newsletter_manager_factory" class="NewsletterManagerFactory" />

                <service
                    id="newsletter_manager"
                    class="NewsletterManager"
                    factory-service="newsletter_manager_factory"
                    factory-method="createNewsletterManager" />
            </services>
        </container>

    .. code-block:: php

        use Symfony\Component\DependencyInjection\Definition;

        $container->setDefinition('newsletter_manager_factory', new Definition(
            'NewsletterManager'
        ));
        $container->setDefinition('newsletter_manager', new Definition(
            'NewsletterManagerFactory'
        ))->setFactoryService(
            'newsletter_manager_factory'
        )->setFactoryMethod(
            'createNewsletterManager'
        );

.. note::

   Il servizio factory è specificato dal suo nome id e non da un riferimento al servizio
   stesso. Non occorre quindi usare la sintassi con la chiocciola nelle configurazioni
   YAML.

Passare parametri al metodo del factory
---------------------------------------

Se occorre passare parametri al metodo del factory, si può usare l'opzione ``arguments``
dentro al contenitore di servizi. Per esempio, si supponga che il metodo ``get``
dell'esempio precedente accetti un servizio ``templating`` come parametro:

.. configuration-block::

    .. code-block:: yaml

        services:
            newsletter_manager_factory:
                class:            NewsletterManagerFactory
            newsletter_manager:
                class:            NewsletterManager
                factory_service:  newsletter_manager_factory
                factory_method:   createNewsletterManager
                arguments:
                    - "@templating"

    .. code-block:: xml

        <?xml version="1.0" encoding="UTF-8" ?>
        <container xmlns="http://symfony.com/schema/dic/services"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://symfony.com/schema/dic/services http://symfony.com/schema/dic/services/services-1.0.xsd">

            <services>
                <service id="newsletter_manager_factory" class="NewsletterManagerFactory" />

                <service
                    id="newsletter_manager"
                    class="NewsletterManager"
                    factory-service="newsletter_manager_factory"
                    factory-method="createNewsletterManager">

                    <argument type="service" id="templating" />
                </service>
            </services>
        </container>

    .. code-block:: php

        use Symfony\Component\DependencyInjection\Definition;

        // ...
        $container->setDefinition('newsletter_manager_factory', new Definition(
            'NewsletterManagerFactory'
        ));
        $container->setDefinition('newsletter_manager', new Definition(
            'NewsletterManager',
            array(new Reference('templating'))
        ))->setFactoryService(
            'newsletter_manager_factory'
        )->setFactoryMethod(
            'createNewsletterManager'
        );
