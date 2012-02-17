Usare il factory per creare servizi
===================================

Il contenitore di servizi di Symfony2 mette a disposizione potenti strumenti
per la creazione di oggetti, permettendo di specificare sia gli argomenti da passare
al costruttore sia i metodi di chiamata che i parametri di configurazione. Alle volte, però,
questo non è sufficiente a soddisfare tutti i requisiti per la creazione dei propri oggetti.
In questi casi, è possibile usare un factory per la creazione di oggetti e fare in modo che
il contenitore di servizi chiami uno specifico metodo, nel factory, invece che 
inizializzare direttamente l'oggetto.

Supponiamo di avere un factory che configura e restituisce un oggetto
GestoreNewsletter::

    namespace Acme\HelloBundle\Newsletter;

    class NewsletterFactory
    {
        public function get()
        {
            $gestoreNewsletter = new GestoreNewsletter();
            
            // ...
            
            return $gestoreNewsletter;
        }
    }

Per rendere disponibile, in forma di servizio, l'oggetto ``GestoreNewsletter``, 
è possibile configurare un contenitore di servizi in modo che usi la classe factory 
``NewsletterFactory``:

.. configuration-block::

    .. code-block:: yaml

        # src/Acme/HelloBundle/Resources/config/services.yml
        parameters:
            # ...
            gestore_newsletter.class: Acme\HelloBundle\Newsletter\GestoreNewsletter
            newsletter_factory.class: Acme\HelloBundle\Newsletter\NewsletterFactory
        services:
            gestore_newsletter:
                class:          %gestore_newsletter.class%
                factory_class:  %newsletter_factory.class%
                factory_method: get 

    .. code-block:: xml

        <!-- src/Acme/HelloBundle/Resources/config/services.xml -->
        <parameters>
            <!-- ... -->
            <parameter key="gestore_newsletter.class">Acme\HelloBundle\Newsletter\GestoreNewsletter</parameter>
            <parameter key="newsletter_factory.class">Acme\HelloBundle\Newsletter\NewsletterFactory</parameter>
        </parameters>

        <services>
            <service id="gestore_newsletter" 
                     class="%gestore_newsletter.class%"
                     factory-class="%newsletter_factory.class%"
                     factory-method="get"
            />
        </services>

    .. code-block:: php

        // src/Acme/HelloBundle/Resources/config/services.php
        use Symfony\Component\DependencyInjection\Definition;

        // ...
        $container->setParameter('gestore_newsletter.class', 'Acme\HelloBundle\Newsletter\GestoreNewsletter');
        $container->setParameter('newsletter_factory.class', 'Acme\HelloBundle\Newsletter\NewsletterFactory');

        $container->setDefinition('gestore_newsletter', new Definition(
            '%gestore_newsletter.class%'
        ))->setFactoryClass(
            '%newsletter_factory.class%'
        )->setFactoryMethod(
            'get'
        );

Quando si specifica la classe da utilizzare come factory (tramite ``factory_class``)
il metodo verrà chiamato staticamente. Se il factory stesso dovesse essere istanziato
e il relativo metodo dell'oggetto sia chiamato (come nell'esempio), si
dovrà configurare il factory come servizio:

.. configuration-block::

    .. code-block:: yaml

        # src/Acme/HelloBundle/Resources/config/services.yml
        parameters:
            # ...
            gestore_newsletter.class: Acme\HelloBundle\Newsletter\GestoreNewsletter
            newsletter_factory.class: Acme\HelloBundle\Newsletter\NewsletterFactory
        services:
            newsletter_factory:
                class:            %newsletter_factory.class%
            gestore_newsletter:
                class:            %gestore_newsletter.class%
                factory_service:  newsletter_factory
                factory_method:   get 

    .. code-block:: xml

        <!-- src/Acme/HelloBundle/Resources/config/services.xml -->
        <parameters>
            <!-- ... -->
            <parameter key="gestore_newsletter.class">Acme\HelloBundle\Newsletter\GestoreNewsletter</parameter>
            <parameter key="newsletter_factory.class">Acme\HelloBundle\Newsletter\NewsletterFactory</parameter>
        </parameters>

        <services>
            <service id="newsletter_factory" class="%newsletter_factory.class%"/>
            <service id="gestore_newsletter" 
                     class="%gestore_newsletter.class%"
                     factory-service="newsletter_factory"
                     factory-method="get"
            />
        </services>

    .. code-block:: php

        // src/Acme/HelloBundle/Resources/config/services.php
        use Symfony\Component\DependencyInjection\Definition;

        // ...
        $container->setParameter('gestore_newsletter.class', 'Acme\HelloBundle\Newsletter\GestoreNewsletter');
        $container->setParameter('newsletter_factory.class', 'Acme\HelloBundle\Newsletter\NewsletterFactory');

        $container->setDefinition('newsletter_factory', new Definition(
            '%newsletter_factory.class%'
        ))
        $container->setDefinition('gestore_newsletter', new Definition(
            '%gestore_newsletter.class%'
        ))->setFactoryService(
            'newsletter_factory'
        )->setFactoryMethod(
            'get'
        );

.. note::

   Il servizio del factory viene specificato tramite il suo nome id e non
   come un riferimento al servizio stesso. Perciò non è necessario usare la sintassi con @.

Passaggio di argomenti al metodo del factory
--------------------------------------------

Per poter passare argomenti al metodo del factory, si può utilizzare l'opzione
``arguments`` all'interno del contenitore di servizi. Si supponga, ad esempio, che
il metodo ``get``, del precedente esempio, accetti il servizio ``templating`` come argomento:

.. configuration-block::

    .. code-block:: yaml

        # src/Acme/HelloBundle/Resources/config/services.yml
        parameters:
            # ...
            gestore_newsletter.class: Acme\HelloBundle\Newsletter\GestoreNewsletter
            newsletter_factory.class: Acme\HelloBundle\Newsletter\NewsletterFactory
        services:
            newsletter_factory:
                class:            %newsletter_factory.class%
            gestore_newsletter:
                class:            %gestore_newsletter.class%
                factory_service:  newsletter_factory
                factory_method:   get
                arguments:
                    -             @templating

    .. code-block:: xml

        <!-- src/Acme/HelloBundle/Resources/config/services.xml -->
        <parameters>
            <!-- ... -->
            <parameter key="gestore_newsletter.class">Acme\HelloBundle\Newsletter\GestoreNewsletter</parameter>
            <parameter key="newsletter_factory.class">Acme\HelloBundle\Newsletter\NewsletterFactory</parameter>
        </parameters>

        <services>
            <service id="newsletter_factory" class="%newsletter_factory.class%"/>
            <service id="gestore_newsletter" 
                     class="%gestore_newsletter.class%"
                     factory-service="newsletter_factory"
                     factory-method="get"
            >
                <argument type="service" id="templating" />
            </service>
        </services>

    .. code-block:: php

        // src/Acme/HelloBundle/Resources/config/services.php
        use Symfony\Component\DependencyInjection\Definition;

        // ...
        $container->setParameter('gestore_newsletter.class', 'Acme\HelloBundle\Newsletter\GestoreNewsletter');
        $container->setParameter('newsletter_factory.class', 'Acme\HelloBundle\Newsletter\NewsletterFactory');

        $container->setDefinition('newsletter_factory', new Definition(
            '%newsletter_factory.class%'
        ))
        $container->setDefinition('gestore_newsletter', new Definition(
            '%gestore_newsletter.class%',
            array(new Reference('templating'))
        ))->setFactoryService(
            'newsletter_factory'
        )->setFactoryMethod(
            'get'
        );
