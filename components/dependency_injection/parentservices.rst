.. index::
   single: Dependency Injection; Servizi genitori

Come gestire le dipendenze comuni con servizi genitori
======================================================

Quando si aggiungono molte funzionalità a un'applicazione, si potrebbe voler condividere
delle dipendenze comuni tra classi correlate. Per esempio, si potrebbe avere un gestore di
newsletter che usa un setter per impostare le sue dipendenze::

    class NewsletterManager
    {
        protected $mailer;
        protected $emailFormatter;

        public function setMailer(Mailer $mailer)
        {
            $this->mailer = $mailer;
        }

        public function setEmailFormatter(EmailFormatter $emailFormatter)
        {
            $this->emailFormatter = $emailFormatter;
        }

        // ...
    }

e poi dei biglietti di auguri, con una classe che condivide alcune dipendenze::

    class GreetingCardManager
    {
        protected $mailer;
        protected $emailFormatter;

        public function setMailer(Mailer $mailer)
        {
            $this->mailer = $mailer;
        }

        public function setEmailFormatter(EmailFormatter $emailFormatter)
        {
            $this->emailFormatter = $emailFormatter;
        }

        // ...
    }

La configurazione dei servizi di queste classe potrebbe essere qualcosa del genere:

.. configuration-block::

    .. code-block:: yaml

        parameters:
            # ...
            newsletter_manager.class: NewsletterManager
            greeting_card_manager.class: GreetingCardManager
        services:
            my_mailer:
                # ...
            my_email_formatter:
                # ...
            newsletter_manager:
                class:     "%newsletter_manager.class%"
                calls:
                    - [setMailer, ["@my_mailer"]]
                    - [setEmailFormatter, ["@my_email_formatter"]]

            greeting_card_manager:
                class:     "%greeting_card_manager.class%"
                calls:
                    - [setMailer, ["@my_mailer"]]
                    - [setEmailFormatter, ["@my_email_formatter"]]

    .. code-block:: xml

        <parameters>
            <!-- ... -->
            <parameter key="newsletter_manager.class">NewsletterManager</parameter>
            <parameter key="greeting_card_manager.class">GreetingCardManager</parameter>
        </parameters>

        <services>
            <service id="my_mailer" ... >
              <!-- ... -->
            </service>
            <service id="my_email_formatter" ... >
              <!-- ... -->
            </service>
            <service id="newsletter_manager" class="%newsletter_manager.class%">
                <call method="setMailer">
                     <argument type="service" id="my_mailer" />
                </call>
                <call method="setEmailFormatter">
                     <argument type="service" id="my_email_formatter" />
                </call>
            </service>
            <service id="greeting_card_manager" class="%greeting_card_manager.class%">
                <call method="setMailer">
                     <argument type="service" id="my_mailer" />
                </call>
                <call method="setEmailFormatter">
                     <argument type="service" id="my_email_formatter" />
                </call>
            </service>
        </services>

    .. code-block:: php

        use Symfony\Component\DependencyInjection\Definition;
        use Symfony\Component\DependencyInjection\Reference;

        // ...
        $container->setParameter('newsletter_manager.class', 'NewsletterManager');
        $container->setParameter('greeting_card_manager.class', 'GreetingCardManager');

        $container->setDefinition('my_mailer', ... );
        $container->setDefinition('my_email_formatter', ... );
        $container->setDefinition('newsletter_manager', new Definition(
            '%newsletter_manager.class%'
        ))->addMethodCall('setMailer', array(
            new Reference('my_mailer')
        ))->addMethodCall('setEmailFormatter', array(
            new Reference('my_email_formatter')
        ));
        $container->setDefinition('greeting_card_manager', new Definition(
            '%greeting_card_manager.class%'
        ))->addMethodCall('setMailer', array(
            new Reference('my_mailer')
        ))->addMethodCall('setEmailFormatter', array(
            new Reference('my_email_formatter')
        ));

Ci sono diverse ripetizioni, sia nelle classi che nella configurazione. Questo vuol dire
che, se per esempio si cambiano le classi ``Mailer`` o ``EmailFormatter`` per essere
iniettate tramite costruttore, si avrà bisogno di aggiornare la configurazione in due
punti. In modo simile, se fosse necessario cambiare i metodi setter, si avrebbe bisogno
di farlo in entrambe le classi. Il tipico modo di trattare i metodi comuni di queste
classi correlate sarebbe estrarli in una classe superiore::

    abstract class MailManager
    {
        protected $mailer;
        protected $emailFormatter;

        public function setMailer(Mailer $mailer)
        {
            $this->mailer = $mailer;
        }

        public function setEmailFormatter(EmailFormatter $emailFormatter)
        {
            $this->emailFormatter = $emailFormatter;
        }

        // ...
    }

Quindi ``NewsletterManager`` e ``GreetingCardManager`` possono estendere tale
classe::

    class NewsletterManager extends MailManager
    {
        // ...
    }

e::

    class GreetingCardManager extends MailManager
    {
        // ...
    }

In modo simile, il contenitore di servizi di Symfony2 supporta anche l'estensione di
servizi nella configurazione, in modo da poter ridurre le ripetizioni, specificando un
genitore per un servizio.

.. configuration-block::

    .. code-block:: yaml

        parameters:
            # ...
            newsletter_manager.class: NewsletterManager
            greeting_card_manager.class: GreetingCardManager
        services:
            my_mailer:
                # ...
            my_email_formatter:
                # ...
            mail_manager:
                abstract:  true
                calls:
                    - [setMailer, ["@my_mailer"]]
                    - [setEmailFormatter, ["@my_email_formatter"]]
            
            newsletter_manager:
                class:     "%newsletter_manager.class%"
                parent: mail_manager
            
            greeting_card_manager:
                class:     "%greeting_card_manager.class%"
                parent: mail_manager
            
    .. code-block:: xml

        <parameters>
            <!-- ... -->
            <parameter key="newsletter_manager.class">NewsletterManager</parameter>
            <parameter key="greeting_card_manager.class">GreetingCardManager</parameter>
        </parameters>

        <services>
            <service id="my_mailer" ... >
              <!-- ... -->
            </service>
            <service id="my_email_formatter" ... >
              <!-- ... -->
            </service>
            <service id="mail_manager" abstract="true">
                <call method="setMailer">
                     <argument type="service" id="my_mailer" />
                </call>
                <call method="setEmailFormatter">
                     <argument type="service" id="my_email_formatter" />
                </call>
            </service>
            <service id="newsletter_manager" class="%newsletter_manager.class%" parent="mail_manager"/>
            <service id="greeting_card_manager" class="%greeting_card_manager.class%" parent="mail_manager"/>
        </services>

    .. code-block:: php

        use Symfony\Component\DependencyInjection\Definition;
        use Symfony\Component\DependencyInjection\DefinitionDecorator;
        use Symfony\Component\DependencyInjection\Reference;

        // ...
        $container->setParameter('newsletter_manager.class', 'NewsletterManager');
        $container->setParameter('greeting_card_manager.class', 'GreetingCardManager');

        $container->setDefinition('my_mailer', ... );
        $container->setDefinition('my_email_formatter', ... );
        $container->setDefinition('mail_manager', new Definition(
        ))->setAbstract(
            true
        )->addMethodCall('setMailer', array(
            new Reference('my_mailer')
        ))->addMethodCall('setEmailFormatter', array(
            new Reference('my_email_formatter')
        ));
        $container->setDefinition('newsletter_manager', new DefinitionDecorator(
            'mail_manager'
        ))->setClass(
            '%newsletter_manager.class%'
        );
        $container->setDefinition('greeting_card_manager', new DefinitionDecorator(
            'mail_manager'
        ))->setClass(
            '%greeting_card_manager.class%'
        );

In questo contesto, avere un servizio ``parent`` implica che i parametri e le chiamate a
metodi del servizio genitore andrebbero usati per i servizi figli. Specificatamente,
i metodi setter definiti per il servizio genitore saranno richiamati all'istanza dei
servizi figli.

.. note::

   Se si rimuove la voce di configurazione ``parent``, i servizi saranno ancora istanziati
   e estenderanno ancora la classe ``MailManager``. La differenza è che, omettendo la 
   voce di configurazione ``parent``, si farà in modo che ``calls``, definito nel servizio
   ``mail_manager``, non sarà eseguito quando i servizi figli saranno
   istanziati.

.. caution::

   Gli attributi ``scope``, ``abstract`` e ``tags`` sono sempre presi dal
   servizio figlio.

La classe genitore è astratta, perché non andrebbe istanziata direttamente dal
contenitore o passata in un altro servizio. Esiste puramente come "template" per
altri servizi. Per questo può non avere ``class`` configurata, che
provocherebbe un'eccezione per un servizio non astratto.

.. note::

   Per poter risolvere dipendenze dei genitori, ``ContainerBuilder`` deve essere
   precedentemente compilato. Si veda :doc:`/components/dependency_injection/compilation` 
   per maggiori dettagli.

Sovrascrivere le dipendenze del genitore
----------------------------------------

A volte si potrebbe voler sovrascrivere la classe passata come dipendenza solo
per un servizio figlio. Fortunatamente, aggiungendo la configurazione della chiamata al
metodo per il servizio figlio, le dipendenze impostate dalla classe genitore saranno
sovrascritte. Se quindi si ha bisogno di passare una dipendenza diversa, solo alla classe
``NewsletterManager``, la configurazione potrebbe essere come la seguente:

.. configuration-block::

    .. code-block:: yaml

        parameters:
            # ...
            newsletter_manager.class: NewsletterManager
            greeting_card_manager.class: GreetingCardManager
        services:
            my_mailer:
                # ...
            my_alternative_mailer:
                # ...
            my_email_formatter:
                # ...
            mail_manager:
                abstract:  true
                calls:
                    - [setMailer, ["@my_mailer"]]
                    - [setEmailFormatter, ["@my_email_formatter"]]
            
            newsletter_manager:
                class:     "%newsletter_manager.class%"
                parent: mail_manager
                calls:
                    - [setMailer, ["@my_alternative_mailer"]]
            
            greeting_card_manager:
                class:     "%greeting_card_manager.class%"
                parent: mail_manager
            
    .. code-block:: xml

        <parameters>
            <!-- ... -->
            <parameter key="newsletter_manager.class">NewsletterManager</parameter>
            <parameter key="greeting_card_manager.class">GreetingCardManager</parameter>
        </parameters>

        <services>
            <service id="my_mailer" ... >
              <!-- ... -->
            </service>
            <service id="my_alternative_mailer" ... >
              <!-- ... -->
            </service>
            <service id="my_email_formatter" ... >
              <!-- ... -->
            </service>
            <service id="mail_manager" abstract="true">
                <call method="setMailer">
                     <argument type="service" id="my_mailer" />
                </call>
                <call method="setEmailFormatter">
                     <argument type="service" id="my_email_formatter" />
                </call>
            </service>
            <service id="newsletter_manager" class="%newsletter_manager.class%" parent="mail_manager">
                 <call method="setMailer">
                     <argument type="service" id="my_alternative_mailer" />
                </call>
            </service>
            <service id="greeting_card_manager" class="%greeting_card_manager.class%" parent="mail_manager"/>
        </services>

    .. code-block:: php

        use Symfony\Component\DependencyInjection\Definition;
        use Symfony\Component\DependencyInjection\DefinitionDecorator;
        use Symfony\Component\DependencyInjection\Reference;

        // ...
        $container->setParameter('newsletter_manager.class', 'NewsletterManager');
        $container->setParameter('greeting_card_manager.class', 'GreetingCardManager');

        $container->setDefinition('my_mailer', ... );
        $container->setDefinition('my_alternative_mailer', ... );
        $container->setDefinition('my_email_formatter', ... );
        $container->setDefinition('mail_manager', new Definition(
        ))->setAbstract(
            true
        )->addMethodCall('setMailer', array(
            new Reference('my_mailer')
        ))->addMethodCall('setEmailFormatter', array(
            new Reference('my_email_formatter')
        ));
        $container->setDefinition('newsletter_manager', new DefinitionDecorator(
            'mail_manager'
        ))->setClass(
            '%newsletter_manager.class%'
        )->addMethodCall('setMailer', array(
            new Reference('my_alternative_mailer')
        ));
        $container->setDefinition('greeting_card_manager', new DefinitionDecorator(
            'mail_manager'
        ))->setClass(
            '%greeting_card_manager.class%'
        );

La classe ``GreetingCardManager`` riceverà le stesse dipendenze di prima,
ma a ``NewsletterManager`` sarà passato il servizio ``my_alternative_mailer``, invece
di ``my_mailer``.

Insiemi di dipendenze
---------------------

Va notato che il metodo setter sovrascritto nell'esempio precedente è in effetti
richiamato due volte: una per la definizione del genitore e una per la definizione
del figlio. Nell'esempio precedente, questo andava bene, perché la seconda chiamata a
``setMailer`` sostituiva l'oggetto mailer impostato dalla prima chiamata.

In alcuni casi, tuttavia, questo potrebbe rappresentare un problema. Per esempio, se il
metodo sovrascritto coinvolge l'aggiunta di qualcosa a un insieme, i due oggetti saranno
aggiunti all'insieme. Di seguito viene mostrato un caso simile, in cui la classe
genitore è::

    abstract class MailManager
    {
        protected $filters;

        public function setFilter($filter)
        {
            $this->filters[] = $filter;
        }

        // ...
    }

Se si ha la seguente configurazione:

.. configuration-block::

    .. code-block:: yaml

        parameters:
            # ...
            newsletter_manager.class: NewsletterManager
        services:
            my_filter:
                # ...
            another_filter:
                # ...
            mail_manager:
                abstract:  true
                calls:
                    - [setFilter, ["@my_filter"]]
                    
            newsletter_manager:
                class:     "%newsletter_manager.class%"
                parent: mail_manager
                calls:
                    - [setFilter, ["@another_filter"]]
            
    .. code-block:: xml

        <parameters>
            <!-- ... -->
            <parameter key="newsletter_manager.class">NewsletterManager</parameter>
        </parameters>

        <services>
            <service id="my_filter" ... >
              <!-- ... -->
            </service>
            <service id="another_filter" ... >
              <!-- ... -->
            </service>
            <service id="mail_manager" abstract="true">
                <call method="setFilter">
                     <argument type="service" id="my_filter" />
                </call>
            </service>
            <service id="newsletter_manager" class="%newsletter_manager.class%" parent="mail_manager">
                 <call method="setFilter">
                     <argument type="service" id="another_filter" />
                </call>
            </service>
        </services>

    .. code-block:: php

        use Symfony\Component\DependencyInjection\Definition;
        use Symfony\Component\DependencyInjection\DefinitionDecorator;
        use Symfony\Component\DependencyInjection\Reference;

        // ...
        $container->setParameter('newsletter_manager.class', 'NewsletterManager');
        $container->setParameter('mail_manager.class', 'MailManager');

        $container->setDefinition('my_filter', ... );
        $container->setDefinition('another_filter', ... );
        $container->setDefinition('mail_manager', new Definition(
        ))->setAbstract(
            true
        )->addMethodCall('setFilter', array(
            new Reference('my_filter')
        ));
        $container->setDefinition('newsletter_manager', new DefinitionDecorator(
            'mail_manager'
        ))->setClass(
            '%newsletter_manager.class%'
        )->addMethodCall('setFilter', array(
            new Reference('another_filter')
        ));

In questo esempi, il metodo ``setFilter`` del servizio ``newsletter_manager`` sarà
richiamato due volte, risultando in un array ``$filters`` che conterrà sia l'oggetto
``my_filter`` che l'oggetto ``another_filter``. Questa cosa va bene se si vuole
effettivamente aggiungere filtri alla sotto-classe. Se invece si vuole sostituire il
filtro passato alla sotto-classe, la rimozione dell'impostazione del genitore dalla
configurazione impedisce la chiamata a ``setFilter`` dalla classe base.

.. tip::

    Negli esempi mostrati c'è una relazione simile tra servizi padre e figlio
    e classi padre e figlio sottostanti. Sebbene non sia detto che questo debba
    sempre essere il caso, si possono estrarre le parti comuni di definizioni
    simili di servizi in un servizio padre, senza ereditare anche una classe
    padre.
