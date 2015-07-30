.. index::
   single: DependencyInjection; Servizi genitori

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

        services:
            my_mailer:
                # ...

            my_email_formatter:
                # ...

            newsletter_manager:
                class: NewsletterManager
                calls:
                    - [setMailer, ["@my_mailer"]]
                    - [setEmailFormatter, ["@my_email_formatter"]]

            greeting_card_manager:
                class: "GreetingCardManager"
                calls:
                    - [setMailer, ["@my_mailer"]]
                    - [setEmailFormatter, ["@my_email_formatter"]]

    .. code-block:: xml

        <?xml version="1.0" encoding="UTF-8" ?>
        <container xmlns="http://symfony.com/schema/dic/services"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://symfony.com/schema/dic/services http://symfony.com/schema/dic/services/services-1.0.xsd">

            <services>
                <service id="my_mailer">
                    <!-- ... -->
                </service>

                <service id="my_email_formatter">
                    <!-- ... -->
                </service>

                <service id="newsletter_manager" class="NewsletterManager">
                    <call method="setMailer">
                        <argument type="service" id="my_mailer" />
                    </call>
                    <call method="setEmailFormatter">
                        <argument type="service" id="my_email_formatter" />
                    </call>
                </service>

                <service id="greeting_card_manager" class="GreetingCardManager">
                    <call method="setMailer">
                        <argument type="service" id="my_mailer" />
                    </call>

                    <call method="setEmailFormatter">
                        <argument type="service" id="my_email_formatter" />
                    </call>
                </service>
            </services>
        </container>

    .. code-block:: php

        use Symfony\Component\DependencyInjection\Reference;

        // ...
        $container->register('my_mailer', ...);
        $container->register('my_email_formatter', ...);

        $container
            ->register('newsletter_manager', 'NewsletterManager')
            ->addMethodCall('setMailer', array(
                new Reference('my_mailer'),
            ))
            ->addMethodCall('setEmailFormatter', array(
                new Reference('my_email_formatter'),
            ))
        ;

        $container
            ->register('greeting_card_manager', 'GreetingCardManager')
            ->addMethodCall('setMailer', array(
                new Reference('my_mailer'),
            ))
            ->addMethodCall('setEmailFormatter', array(
                new Reference('my_email_formatter'),
            ))
        ;

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

In modo simile, il contenitore di servizi di Symfony supporta anche l'estensione di
servizi nella configurazione, in modo da poter ridurre le ripetizioni, specificando un
genitore per un servizio.

.. configuration-block::

    .. code-block:: yaml

        # ...
        services:
            # ...
            mail_manager:
                abstract:  true
                calls:
                    - [setMailer, ["@my_mailer"]]
                    - [setEmailFormatter, ["@my_email_formatter"]]

            newsletter_manager:
                class:  "NewsletterManager"
                parent: mail_manager

            greeting_card_manager:
                class:  "GreetingCardManager"
                parent: mail_manager

    .. code-block:: xml

        <?xml version="1.0" encoding="UTF-8" ?>
        <container xmlns="http://symfony.com/schema/dic/services"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://symfony.com/schema/dic/services http://symfony.com/schema/dic/services/services-1.0.xsd">

            <!-- ... -->
            <services>
                <!-- ... -->
                <service id="mail_manager" abstract="true">
                    <call method="setMailer">
                        <argument type="service" id="my_mailer" />
                    </call>

                    <call method="setEmailFormatter">
                        <argument type="service" id="my_email_formatter" />
                    </call>
                </service>

                <service
                    id="newsletter_manager"
                    class="NewsletterManager"
                    parent="mail_manager" />

                <service
                    id="greeting_card_manager"
                    class="GreetingCardManager"
                    parent="mail_manager" />
            </services>
        </container>

    .. code-block:: php

        use Symfony\Component\DependencyInjection\Definition;
        use Symfony\Component\DependencyInjection\DefinitionDecorator;
        use Symfony\Component\DependencyInjection\Reference;

        // ...
        $mailManager = new Definition();
        $mailManager
            ->setAbstract(true);
            ->addMethodCall('setMailer', array(
                new Reference('my_mailer'),
            ))
            ->addMethodCall('setEmailFormatter', array(
                new Reference('my_email_formatter'),
            ))
        ;
        $container->setDefinition('mail_manager', $mailManager);

        $newsletterManager = new DefinitionDecorator('mail_manager');
        $newsletterManager->setClass('NewsletterManager');
        $container->setDefinition('newsletter_manager', $newsletterManager);

        $greetingCardManager = new DefinitionDecorator('mail_manager');
        $greetingCardManager->setClass('GreetingCardManager');
        $container->setDefinition('greeting_card_manager', $greetingCardManager);

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

.. tip::

    Nell'esempio mostrato, le classi che condividono la stessa configurazione estendono
    anche la stessa classe in PHP. Questo non è assolutamente necessario.
    Si possono semplicemente estrarre le parti comuni di definizioni simili di servizi in
    un servizio genitori, senza dover necessariamente estendere una classe in PHP.

Sovrascrivere le dipendenze del genitore
----------------------------------------

A volte si potrebbe voler sovrascrivere la classe passata come dipendenza solo
per un servizio figlio. Fortunatamente, aggiungendo la configurazione della chiamata al
metodo per il servizio figlio, le dipendenze impostate dalla classe genitore saranno
sovrascritte. Se quindi si ha bisogno di passare una dipendenza diversa, solo alla classe
``NewsletterManager``, la configurazione potrebbe essere come la seguente:

.. configuration-block::

    .. code-block:: yaml

        # ...
        services:
            # ...
            my_alternative_mailer:
                # ...

            mail_manager:
                abstract: true
                calls:
                    - [setMailer, ["@my_mailer"]]
                    - [setEmailFormatter, ["@my_email_formatter"]]

            newsletter_manager:
                class:  "NewsletterManager"
                parent: mail_manager
                calls:
                    - [setMailer, ["@my_alternative_mailer"]]

            greeting_card_manager:
                class:  "GreetingCardManager"
                parent: mail_manager

    .. code-block:: xml

        <?xml version="1.0" encoding="UTF-8" ?>
        <container xmlns="http://symfony.com/schema/dic/services"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://symfony.com/schema/dic/services http://symfony.com/schema/dic/services/services-1.0.xsd">

            <!-- ... -->
            <services>
                <!-- ... -->
                <service id="my_alternative_mailer">
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

                <service
                    id="newsletter_manager"
                    class="NewsletterManager"
                    parent="mail_manager">

                    <call method="setMailer">
                        <argument type="service" id="my_alternative_mailer" />
                    </call>
                </service>

                <service
                    id="greeting_card_manager"
                    class="GreetingCardManager"
                    parent="mail_manager" />
            </services>
        </container>

    .. code-block:: php

        use Symfony\Component\DependencyInjection\Definition;
        use Symfony\Component\DependencyInjection\DefinitionDecorator;
        use Symfony\Component\DependencyInjection\Reference;

        // ...
        $container->setDefinition('my_alternative_mailer', ...);

        $mailManager = new Definition();
        $mailManager
            ->setAbstract(true);
            ->addMethodCall('setMailer', array(
                new Reference('my_mailer'),
            ))
            ->addMethodCall('setEmailFormatter', array(
                new Reference('my_email_formatter'),
            ))
        ;
        $container->setDefinition('mail_manager', $mailManager);

        $newsletterManager = new DefinitionDecorator('mail_manager');
        $newsletterManager->setClass('NewsletterManager');
            ->addMethodCall('setMailer', array(
                new Reference('my_alternative_mailer'),
            ))
        ;
        $container->setDefinition('newsletter_manager', $newsletterManager);

        $greetingCardManager = new DefinitionDecorator('mail_manager');
        $greetingCardManager->setClass('GreetingCardManager');
        $container->setDefinition('greeting_card_manager', $greetingCardManager);

La classe ``GreetingCardManager`` riceverà le stesse dipendenze di prima,
ma a ``NewsletterManager`` sarà passato il servizio ``my_alternative_mailer``, invece
di ``my_mailer``.

.. caution::

    Non si possono sovrascrivere le chiamate a metodi. Quando si definiscono nuove chiamate a metodi nel
    servizio figlio, saranno aggiunte all'insieme corrente di chiamate a metodi configurate. Questo vuol dire
    che funziona ugualmente quanto il setter sovrascrive la proprietà corrente, ma non funziona
    come ci si potrebbe aspettare quanto il setter lo aggiunge ai dati esistenti (p.e. un metodo
    ``addFilters()``).
    In questi casi, l'unica soluzione è di *non* estendere il servizio genitore e configurare
    il servizio proprio come si sarebbe fatto in precedenza.
