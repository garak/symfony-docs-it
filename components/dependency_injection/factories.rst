.. index::
   single: DependencyInjection; Factory

Usare un factory per creare servizi
===================================

.. versionadded:: 2.6
    Il metodo :method:`Symfony\\Component\\DependencyInjection\\Definition::setFactory`
    è stato introdotto in Symfony 2.6. Fare riferimento alle versioni precedenti per
    la sintassi per i factory prima di 2.6.

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
il contenitore di servizi per usare il metodo factory
``NewsletterFactory::createNewsletterManager()``:

.. configuration-block::

    .. code-block:: yaml

        services:
            newsletter_manager:
                class:   NewsletterManager
                factory: [NewsletterManagerFactory, createNewsletterManager]

    .. code-block:: xml

        <?xml version="1.0" encoding="UTF-8" ?>
        <container xmlns="http://symfony.com/schema/dic/services"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://symfony.com/schema/dic/services http://symfony.com/schema/dic/services/services-1.0.xsd">

            <services>
                <service id="newsletter_manager" class="NewsletterManager">
                    <factory class="NewsletterManagerFactory" method="createNewsletterManager" />
                </service>
            </services>
        </services>

    .. code-block:: php

        use Symfony\Component\DependencyInjection\Definition;

        // ...
        $definition = new Definition('NewsletterManager');
        $definition->setFactory(array('NewsletterManagerFactory', 'createNewsletterManager'));

        $container->setDefinition('newsletter_manager', $definition);

Ora il metodo sarà richiamato staticamente. Se il factory stesso va istanziato e il
metodo dell'oggetto risultante richiamato, configurare il factory stesso come servizio:
In questo caso, il metodo (p.e. ``get``) va cambiato per non
essere statico:

.. configuration-block::

    .. code-block:: yaml

        services:
            newsletter_manager.factory:
                class: NewsletterManagerFactory
            newsletter_manager:
                class:   NewsletterManager
                factory: ["@newsletter_manager.factory", createNewsletterManager]

    .. code-block:: xml

        <?xml version="1.0" encoding="UTF-8" ?>
        <container xmlns="http://symfony.com/schema/dic/services"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://symfony.com/schema/dic/services http://symfony.com/schema/dic/services/services-1.0.xsd">

            <services>
                <service id="newsletter_manager.factory" class="NewsletterManagerFactory" />

                <service id="newsletter_manager" class="NewsletterManager">
                    <factory service="newsletter_manager.factory" method="createNewsletterManager" />
                </service>
            </services>
        </container>

    .. code-block:: php

        use Symfony\Component\DependencyInjection\Reference;
        use Symfony\Component\DependencyInjection\Definition;

        // ...
        $container->register('newsletter_manager.factory', 'NewsletterManagerFactory');

        $newsletterManager = new Definition();
        $newsletterManager->setFactory(array(
            new Reference('newsletter_manager.factory'),
            'createNewsletterManager'
        ));
        $container->setDefinition('newsletter_manager', $newsletterManager);

Passare parametri al metodo del factory
---------------------------------------

Se occorre passare parametri al metodo del factory, si può usare l'opzione ``arguments``
dentro al contenitore di servizi. Per esempio, si supponga che il metodo ``get``
dell'esempio precedente accetti un servizio ``templating`` come parametro:

.. configuration-block::

    .. code-block:: yaml

        services:
            newsletter_manager.factory:
                class: NewsletterManagerFactory

            newsletter_manager:
                class:   NewsletterManager
                factory: ["@newsletter_manager.factory", createNewsletterManager]
                arguments:
                    - "@templating"

    .. code-block:: xml

        <?xml version="1.0" encoding="UTF-8" ?>
        <container xmlns="http://symfony.com/schema/dic/services"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://symfony.com/schema/dic/services http://symfony.com/schema/dic/services/services-1.0.xsd">

            <services>
                <service id="newsletter_manager.factory" class="NewsletterManagerFactory"/>

                <service id="newsletter_manager" class="NewsletterManager">
                    <factory service="newsletter_manager.factory" method="createNewsletterManager"/>
                    <argument type="service" id="templating"/>
                </service>
            </services>
        </container>

    .. code-block:: php

        use Symfony\Component\DependencyInjection\Reference;
        use Symfony\Component\DependencyInjection\Definition;

        // ...
        $container->register('newsletter_manager.factory', 'NewsletterManagerFactory');

        $newsletterManager = new Definition(
            'NewsletterManager',
            array(new Reference('templating'))
        );
        $newsletterManager->setFactory(array(
            new Reference('newsletter_manager.factory'),
            'createNewsletterManager'
        ));
        $container->setDefinition('newsletter_manager', $newsletterManager);
