.. index::
   single: DependencyInjection; Tag

Usare i tag nei servizi
=======================

I tag sono generiche stinghe (con alcune opzioni) che si possono applicare a un
servizio. Di per sé, i tag non alterano la funzionalità di un servizio in
alcun modo. Ma, se lo si desidera, si può chiedere a un costruttore di contenitori
una lista di tutti i servizi che hanno uno specifico tag. Questo può tornare utile
nei passi di compilatore, in cui si possono trovare tali servizi e usarli per
modificarli in qualche modo.

Per esempio, se si usa SwiftMailer, si può immaginare di voler implementare
una "catena di trasporto", che è un insieme di classi che implementano
``\Swift_Transport``. Usando una catena, si vogliono offrire a SwiftMailer diversi
modi di trasportare un messsaggio, finché uno non ha successo.

Per iniziare, definire la classe ``TransportChain``::

    class TransportChain
    {
        private $transports;

        public function __construct()
        {
            $this->transports = array();
        }

        public function addTransport(\Swift_Transport $transport)
        {
            $this->transports[] = $transport;
        }
    }

Quindi, definire la catena come servizio:

.. configuration-block::

    .. code-block:: yaml

        services:
            acme_mailer.transport_chain:
                class: TransportChain

    .. code-block:: xml

        <?xml version="1.0" encoding="UTF-8" ?>
        <container xmlns="http://symfony.com/schema/dic/services"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://symfony.com/schema/dic/services http://symfony.com/schema/dic/services/services-1.0.xsd">

            <services>
                <service id="acme_mailer.transport_chain" class="TransportChain" />
            </services>
        </container>

    .. code-block:: php

        use Symfony\Component\DependencyInjection\Definition;

        $container->setDefinition('acme_mailer.transport_chain', new Definition('TransportChain'));

Definire servizi con un tag personalizzato
------------------------------------------

Ora vogliamo che diverse classi ``\Swift_Transport`` siano istanziate e aggiunte
alla catena automaticamente, usando il metodo ``addTransport()``.
Come esempio, aggiungere i seguenti trasporti come servizi:

.. configuration-block::

    .. code-block:: yaml

        services:
            acme_mailer.transport.smtp:
                class: \Swift_SmtpTransport
                arguments:
                    - "%mailer_host%"
                tags:
                    -  { name: acme_mailer.transport }
            acme_mailer.transport.sendmail:
                class: \Swift_SendmailTransport
                tags:
                    -  { name: acme_mailer.transport }

    .. code-block:: xml

        <?xml version="1.0" encoding="UTF-8" ?>
        <container xmlns="http://symfony.com/schema/dic/services"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://symfony.com/schema/dic/services http://symfony.com/schema/dic/services/services-1.0.xsd">

            <services>
                <service id="acme_mailer.transport.smtp" class="\Swift_SmtpTransport">
                    <argument>%mailer_host%</argument>
                    <tag name="acme_mailer.transport" />
                </service>

                <service id="acme_mailer.transport.sendmail" class="\Swift_SendmailTransport">
                    <tag name="acme_mailer.transport" />
                </service>
            </services>
        </container>

    .. code-block:: php

        use Symfony\Component\DependencyInjection\Definition;

        $definitionSmtp = new Definition('\Swift_SmtpTransport', array('%mailer_host%'));
        $definitionSmtp->addTag('acme_mailer.transport');
        $container->setDefinition('acme_mailer.transport.smtp', $definitionSmtp);

        $definitionSendmail = new Definition('\Swift_SendmailTransport');
        $definitionSendmail->addTag('acme_mailer.transport');
        $container->setDefinition('acme_mailer.transport.sendmail', $definitionSendmail);

Si noti che a ognuno è stato assegnato il tag ``acme_mailer.transport``. Questo è il tag
personalizzato che useremo nel passo di compilatore. Il passo di compilatore è ciò
che dà un significato a questo tag.

Creare un ``CompilerPass``
--------------------------

Il passo di compilatore ora chiede al contenitore ogni servizio che abbia il
tag personalizzato::

    use Symfony\Component\DependencyInjection\ContainerBuilder;
    use Symfony\Component\DependencyInjection\Compiler\CompilerPassInterface;
    use Symfony\Component\DependencyInjection\Reference;

    class TransportCompilerPass implements CompilerPassInterface
    {
        public function process(ContainerBuilder $container)
        {
            if (!$container->hasDefinition('acme_mailer.transport_chain')) {
                return;
            }

            $definition = $container->getDefinition(
                'acme_mailer.transport_chain'
            );

            $taggedServices = $container->findTaggedServiceIds(
                'acme_mailer.transport'
            );
            foreach ($taggedServices as $id => $tags) {
                $definition->addMethodCall(
                    'addTransport',
                    array(new Reference($id))
                );
            }
        }
    }

Il metodo ``process()`` verifica l'esistenza del servizio ``acme_mailer.transport_chain``,
quindi cerca tutti i servizi con tag ``acme_mailer.transport``. Aggiunge all
definizione del servizio ``acme_mailer.transport_chain`` una chiamata a
``addTransport()`` per ogni servizio "acme_mailer.transport" trovato.
Il primo parametro di ognuna di queste chiamate sarà il servizio di trasporto
stesso.

Registrare il passo con il contenitore
--------------------------------------

Occorrerà anche registrare il passo con il contenitore, sarà poi eseguito quando
il contenitore viene compilato::

    use Symfony\Component\DependencyInjection\ContainerBuilder;

    $container = new ContainerBuilder();
    $container->addCompilerPass(new TransportCompilerPass());

.. note::

    I passi di compilatore sono registrati in modo diverso, se si usa il framework
    completo. Vedere :doc:`/cookbook/service_container/compiler_passes`
    per maggiori dettagli.

Aggiungere altri attributi ai tag
---------------------------------

A volte occorrono informazioni aggiuntive su ogni servizio che ha un certo tag.
Per esempio, si potrebbe voler aggiungere un alias a ogni TransportChain.

Per iniziare, cambiare la classe ``TransportChain``::

    class TransportChain
    {
        private $transports;

        public function __construct()
        {
            $this->transports = array();
        }

        public function addTransport(\Swift_Transport $transport, $alias)
        {
            $this->transports[$alias] = $transport;
        }

        public function getTransport($alias)
        {
            if (array_key_exists($alias, $this->transports)) {
                return $this->transports[$alias];
            }
        }
    }

Come si può vedere, al richiamo di ``addTransport``, non prende solo un oggetto
``Swift_Transport``, ma anche una stringa alias per il trasporto. Quindi, come si può
fare in modo che ogni servizio di trasporto fornisca anche un alias?

Per rispondere, cambiare la dichiarazione del servizio:

.. configuration-block::

    .. code-block:: yaml

        services:
            acme_mailer.transport.smtp:
                class: \Swift_SmtpTransport
                arguments:
                    - "%mailer_host%"
                tags:
                    -  { name: acme_mailer.transport, alias: pippo }
            acme_mailer.transport.sendmail:
                class: \Swift_SendmailTransport
                tags:
                    -  { name: acme_mailer.transport, alias: pluto }

    .. code-block:: xml

        <?xml version="1.0" encoding="UTF-8" ?>
        <container xmlns="http://symfony.com/schema/dic/services"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://symfony.com/schema/dic/services http://symfony.com/schema/dic/services/services-1.0.xsd">

            <services>
                <service id="acme_mailer.transport.smtp" class="\Swift_SmtpTransport">
                    <argument>%mailer_host%</argument>
                    <tag name="acme_mailer.transport" alias="pippo" />
                </service>

                <service id="acme_mailer.transport.sendmail" class="\Swift_SendmailTransport">
                    <tag name="acme_mailer.transport" alias="pluto" />
                </service>
            </services>
        </container>

    .. code-block:: php

        use Symfony\Component\DependencyInjection\Definition;

        $definitionSmtp = new Definition('\Swift_SmtpTransport', array('%mailer_host%'));
        $definitionSmtp->addTag('acme_mailer.transport', array('alias' => 'pippo'));
        $container->setDefinition('acme_mailer.transport.smtp', $definitionSmtp);

        $definitionSendmail = new Definition('\Swift_SendmailTransport');
        $definitionSendmail->addTag('acme_mailer.transport', array('alias' => 'pluto'));
        $container->setDefinition('acme_mailer.transport.sendmail', $definitionSendmail);

Si noti che è stata aggiunta una chiave generica ``alias`` al tag. Per usarla
effettivamente, aggiornare il compilatore::

    use Symfony\Component\DependencyInjection\ContainerBuilder;
    use Symfony\Component\DependencyInjection\Compiler\CompilerPassInterface;
    use Symfony\Component\DependencyInjection\Reference;

    class TransportCompilerPass implements CompilerPassInterface
    {
        public function process(ContainerBuilder $container)
        {
            if (!$container->hasDefinition('acme_mailer.transport_chain')) {
                return;
            }

            $definition = $container->getDefinition(
                'acme_mailer.transport_chain'
            );

            $taggedServices = $container->findTaggedServiceIds(
                'acme_mailer.transport'
            );
            foreach ($taggedServices as $id => $tags) {
                foreach ($tags as $attributes) {
                    $definition->addMethodCall(
                        'addTransport',
                        array(new Reference($id), $attributes["alias"])
                    );
                }
            }
        }
    }

La parte più strana è la variabile ``$attributes``. Poiché si può usare lo stesso tag
più volte sullo stesso servizio (p.e. in teoria si potrebbe assegnare il
tag ``acme_mailer.transport`` allo stesso servizio cinque volte, ``$attributes``
è un array di informazioni sul tag per ciascun tag su tale servizio.
