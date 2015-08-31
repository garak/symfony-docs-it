.. index::
   single: Configurazione; Semantica
   single: Bundle; Configurazione delle estensioni

Semplificare la configurazione di più bundle
============================================

Costruendo applicazioni riusabili ed estendibili, gli sviluppatori spesso
affrontano un dilemma: creare un singolo grosso bundle oppure molti bundle
più piccoli. La creazione di un singolo bundle ha il difetto che è impossibile
per gli utenti rimuovere funzionalità che non usano. La creazioni di molti
bundle ha il difetto che la configurazione diventa più noiosa e spesso occorre
ripetere le impostazioni per i vari bundle.

Usando l'approccio seguente, è possibile rimuovere lo svantaggio dell'approccio
dei molti bundle, abilitando una singola estensione a prependere le
impostazioni di ciascun bundle. Si possono usare le impostazioni definite in ``app/config/config.yml``
per le impostazioni prepese, come se fossero state scritte esplicitamente
dall'utente nella configurazione dell'applicazione.

Per esempio, lo si potrebbe fare per configurare il nome del gestore di entità da usare
in più bundle. Oppure si potrebbe usare per abilitare una caratteristica opzionale, che dipende
da un altro bundle, caricato insieme.

Per dare tale possibilità a un'estensione, questa  deve implementare
:class:`Symfony\\Component\\DependencyInjection\\Extension\\PrependExtensionInterface`::

    // src/Acme/HelloBundle/DependencyInjection/AcmeHelloExtension.php
    namespace Acme\HelloBundle\DependencyInjection;

    use Symfony\Component\HttpKernel\DependencyInjection\Extension;
    use Symfony\Component\DependencyInjection\Extension\PrependExtensionInterface;
    use Symfony\Component\DependencyInjection\ContainerBuilder;

    class AcmeHelloExtension extends Extension implements PrependExtensionInterface
    {
        // ...

        public function prepend(ContainerBuilder $container)
        {
            // ...
        }
    }

Dentro al metodo :method:`Symfony\\Component\\DependencyInjection\\Extension\\PrependExtensionInterface::prepend`,
gli sviluppatori hanno pieno accesso all'istanza di :class:`Symfony\\Component\\DependencyInjection\\ContainerBuilder`
subito prima che il metodo :method:`Symfony\\Component\\DependencyInjection\\Extension\\ExtensionInterface::load`
sia richiamato su ogni estensione dei bundle registrati. Per poter prependere
le impostazioni dell'estensione di un bundle, gli sviluppatori possono usare il metodo
:method:`Symfony\\Component\\DependencyInjection\\ContainerBuilder::prependExtensionConfig`
sull'istanza di :class:`Symfony\\Component\\DependencyInjection\\ContainerBuilder`.
Poiché tale metodo aggiunge solo prefissi, ogni altra impostazione esplicita dentro
al file ``app/config/config.yml`` sovrascriverà tali impostazioni prepese.

L'esempio seguente illustra come prependere
un'impostazione di configurazione in più bundle, nonché come disabilitare un flag in più bundle,
nel caso in cui un altro specifico bundle non sia registrato::

    public function prepend(ContainerBuilder $container)
    {
        // restituisce tutti i bundle
        $bundles = $container->getParameter('kernel.bundles');
        // determina se AcmeGoodbyeBundle sia registrato o meno
        if (!isset($bundles['AcmeGoodbyeBundle'])) {
            // disabilita AcmeGoodbyeBundle
            $config = array('use_acme_goodbye' => false);
            foreach ($container->getExtensions() as $name => $extension) {
                switch ($name) {
                    case 'acme_something':
                    case 'acme_other':
                        // imposta use_acme_goodbye a false nella configurazione di
                        // acme_something e acme_other. Notare che se l'utente configura a mano
                        // use_acme_goodbye a true in app/config/config.yml,
                        // l'impostazione finale sarà true e non false
                        $container->prependExtensionConfig($name, $config);
                        break;
                }
            }
        }

        // processa la configurazione di AcmeHelloExtension
        $configs = $container->getExtensionConfig($this->getAlias());
        // usa la classe Configuration per generare un array di configurazione con
        // le impostazioni "acme_hello"
        $config = $this->processConfiguration(new Configuration(), $configs);

        // verifica se entity_manager_name sia impostato nella configurazione "acme_hello"
        if (isset($config['entity_manager_name'])) {
            // prepende le impostazioni acme_something con entity_manager_name
            $config = array('entity_manager_name' => $config['entity_manager_name']);
            $container->prependExtensionConfig('acme_something', $config);
        }
    }

Quanto visto sarebbe equivalente a scrivere quanto segue in ``app/config/config.yml``,
nel caso in cui ``AcmeGoodbyeBundle`` non sia registrato e l'impostazione ``entity_manager_name``
per ``acme_hello`` sia impostata a ``non_default``:

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        acme_something:
            # ...
            use_acme_goodbye: false
            entity_manager_name: non_default

        acme_other:
            # ...
            use_acme_goodbye: false

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <acme-something:config use-acme-goodbye="false">
            <acme-something:entity-manager-name>non_default</acme-something:entity-manager-name>
        </acme-something:config>

        <acme-other:config use-acme-goodbye="false" />

    .. code-block:: php

        // app/config/config.php
        $container->loadFromExtension('acme_something', array(
            // ...
            'use_acme_goodbye' => false,
            'entity_manager_name' => 'non_default',
        ));
        $container->loadFromExtension('acme_other', array(
            // ...
            'use_acme_goodbye' => false,
        ));
