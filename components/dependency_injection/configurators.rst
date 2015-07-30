.. index::
   single: DependencyInjection; Configuratori di servizi

Configurare servizi con un configuratore di servizi
===================================================

Il configuratore di servizi è una caratteristica del contenitore, che consente
di usare un callable per configurare un servizio appena istanziato.

Si può specificare un metodo di un altro servizio, una funzione PHP o un metodo statico
in una classe. L'istanza del servizio viene passata al callable, consentendo al
configuratore di fare tutto ciò di cui ha bisogno per configurare il servizio
creato.

Per esempio, si può usare un configuratore di servizi quando si ha un servizio che
richiede una preparazione complessa, in base a impostazioni di configurazione provenienti
da diversi sorgenti/servizi. Usando un configuratore esterno, si può mantenere l'implementazione
del servizio pulita e disaccoppiata da altri oggetti, che forniscono la
configurazione necessaria.

Un altro caso d'uso interessante è quando si hanno molti oggetti che condividono
una configurazione comune o che vanno configurati in modo simile a runtime.

Per esempio, si supponga di avere un'applicazione in cui si inviano diversi tipi di
email agli utenti. Le email passano attraverso diversi formattatori, che possono essere
abilitati o meno, a seconda di alcune impostazioni dinamiche dell'applicazione. Si inizia
definendo una classe ``NewsletterManager``, come questa::

    class NewsletterManager implements EmailFormatterAwareInterface
    {
        protected $mailer;
        protected $enabledFormatters;

        public function setMailer(Mailer $mailer)
        {
            $this->mailer = $mailer;
        }

        public function setEnabledFormatters(array $enabledFormatters)
        {
            $this->enabledFormatters = $enabledFormatters;
        }

        // ...
    }

e una classe ``GreetingCardManager``::

    class GreetingCardManager implements EmailFormatterAwareInterface
    {
        protected $mailer;
        protected $enabledFormatters;

        public function setMailer(Mailer $mailer)
        {
            $this->mailer = $mailer;
        }

        public function setEnabledFormatters(array $enabledFormatters)
        {
            $this->enabledFormatters = $enabledFormatters;
        }

        // ...
    }

Come menzionato in precedenza, lo scopo è quello di impostare i formattatori a runtime, a
seconda delle configurazioni dell'applicazione. Per farlo, definiamo anche una classe ``EmailFormatterManager``,
che si occupi di caricare e validatore i formattatori abilitati
nell'applicazione::

    class EmailFormatterManager
    {
        protected $enabledFormatters;

        public function loadFormatters()
        {
            // codice per configurare quali formattatori usare
            $enabledFormatters = array(...);
            // ...

            $this->enabledFormatters = $enabledFormatters;
        }

        public function getEnabledFormatters()
        {
            return $this->enabledFormatters;
        }

        // ...
    }

Se lo scopo è quello di evitare accoppiamenti tra ``NewsletterManager`` e
``GreetingCardManager`` con ``EmailFormatterManager``, si potrebbe voler
creare una classe configuratore, per configurare tali istanze::

    class EmailConfigurator
    {
        private $formatterManager;

        public function __construct(EmailFormatterManager $formatterManager)
        {
            $this->formatterManager = $formatterManager;
        }

        public function configure(EmailFormatterAwareInterface $emailManager)
        {
            $emailManager->setEnabledFormatters(
                $this->formatterManager->getEnabledFormatters()
            );
        }

        // ...
    }

Il compito di ``EmailConfigurator`` è iniettare i filtri abilitati in ``NewsletterManager``
e ``GreetingCardManager``, perché non sono consapevoli di dove i filtri abilitati
arrivino. D'altro canto, ``EmailFormatterManager`` sa dei
formattatori abilitati e come caricarli, mantenendo il principio della
singola responsabilità.

Configurazione del configuratore di servizi
-------------------------------------------

La configurazione del servizio per le classi viste sopra assomiglia a questa:

.. configuration-block::

    .. code-block:: yaml

        services:
            my_mailer:
                # ...

            email_formatter_manager:
                class:     EmailFormatterManager
                # ...

            email_configurator:
                class:     EmailConfigurator
                arguments: ["@email_formatter_manager"]
                # ...

            newsletter_manager:
                class:     NewsletterManager
                calls:
                    - [setMailer, ["@my_mailer"]]
                configurator: ["@email_configurator", configure]

            greeting_card_manager:
                class:     GreetingCardManager
                calls:
                    - [setMailer, ["@my_mailer"]]
                configurator: ["@email_configurator", configure]

    .. code-block:: xml

        <?xml version="1.0" encoding="UTF-8" ?>
        <container xmlns="http://symfony.com/schema/dic/services"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://symfony.com/schema/dic/services http://symfony.com/schema/dic/services/services-1.0.xsd">

            <services>
                <service id="my_mailer">
                    <!-- ... -->
                </service>

                <service id="email_formatter_manager" class="EmailFormatterManager">
                    <!-- ... -->
                </service>

                <service id="email_configurator" class="EmailConfigurator">
                    <argument type="service" id="email_formatter_manager" />
                    <!-- ... -->
                </service>

                <service id="newsletter_manager" class="NewsletterManager">
                    <call method="setMailer">
                        <argument type="service" id="my_mailer" />
                    </call>
                    <configurator service="email_configurator" method="configure" />
                </service>

                <service id="greeting_card_manager" class="GreetingCardManager">
                    <call method="setMailer">
                        <argument type="service" id="my_mailer" />
                    </call>
                    <configurator service="email_configurator" method="configure" />
                </service>
            </services>
        </container>

    .. code-block:: php

        use Symfony\Component\DependencyInjection\Definition;
        use Symfony\Component\DependencyInjection\Reference;

        // ...
        $container->setDefinition('my_mailer', ...);
        $container->setDefinition('email_formatter_manager', new Definition(
            'EmailFormatterManager'
        ));
        $container->setDefinition('email_configurator', new Definition(
            'EmailConfigurator'
        ));
        $container->setDefinition('newsletter_manager', new Definition(
            'NewsletterManager'
        ))->addMethodCall('setMailer', array(
            new Reference('my_mailer'),
        ))->setConfigurator(array(
            new Reference('email_configurator'),
            'configure',
        )));
        $container->setDefinition('greeting_card_manager', new Definition(
            'GreetingCardManager'
        ))->addMethodCall('setMailer', array(
            new Reference('my_mailer'),
        ))->setConfigurator(array(
            new Reference('email_configurator'),
            'configure',
        )));
