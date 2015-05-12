.. index::
    single: Uso di parametri in una classe della Dependency Injection

Uso di parametri in una classe della Dependency Injection
---------------------------------------------------------

Abbiamo già visto come usare parametri della configurazione nei
:ref:`contenitori di servizi di Symfony <book-service-container-parameters>`.
Ci sono casi speciali in cui si vuole, per esempio, usare il parametro
``%kernel.debug%`` per far modo che un servizio entri in
modalità di debug. Per questi casi, occorre un po' di lavoro in più per
far capire il valore del parametro al sistema. Per impostazione predefinita,
il parametro ``%kernel.debug%`` sarà trattato come una
semplice stringa. Si consideri questo esempio con AcmeDemoBundle::

    // Dentro una classe Configuration
    $rootNode
        ->children()
            ->booleanNode('logging')->defaultValue('%kernel.debug%')->end()
            // ...
        ->end()
    ;

    // Dentro una classe Extension
    $config = $this->processConfiguration($configuration, $configs);
    var_dump($config['logging']);

Vediamo ora i risultati:

.. configuration-block::

    .. code-block:: yaml

        un_bundle:
            logging: true
            # true, come ci si aspettava

        un_bundle:
            logging: %kernel.debug%
            # true/false (a seconda del secondo parametro di AppKernel),
            # come ci si aspettava, perché %kernel.debug% dentro una configurazione
            # è valutato prima di essere passato all'estensione

        un_bundle: ~
        # passa la stringa "%kernel.debug%".
        # Che è sempre considerata true.
        # Il configuratore non sa che
        # "%kernel.debug%" va preso come un parametro.

    .. code-block:: xml

        <?xml version="1.0" encoding="UTF-8" ?>
        <container xmlns="http://symfony.com/schema/dic/services"
            xmlns:my-bundle="http://example.org/schema/dic/my_bundle">

            <un-bundle:config logging="true" />
            <!-- true, come ci si aspettava -->

             <my-bundle:config logging="%kernel.debug%" />
             <!-- true/false (a seconda del secondo parametro di AppKernel),
                  come ci si aspettava, perché %kernel.debug% dentro una configurazione
                  è valutato prima di essere passato all'estensione -->

            <un-bundle:config />
            <!-- passa la stringa "%kernel.debug%".
                 Che è sempre considerata true.
                 Il configuratore non sa che
                 "%kernel.debug%" va preso come un parametro. -->
        </container>

    .. code-block:: php

        $container->loadFromExtension('un_bundle', array(
                'logging' => true,
                // true, come ci si aspettava
            )
        );

        $container->loadFromExtension('un_bundle', array(
                'logging' => "%kernel.debug%",
                // true/false (a seconda del secondo parametro di AppKernel),
                // come ci si aspettava, perché %kernel.debug% dentro una configurazione
                // è valutato prima di essere passato all'estensione
            )
        );

        $container->loadFromExtension('my_bundle');
        // passa la stringa "%kernel.debug%".
        // Che è sempre considerata true.
        // Il configuratore non sa che
        // "%kernel.debug%" va preso come un parametro.

Per supportare anche questo caso, alla classe ``Configuration`` va
iniettato questo parametro, tramite l'estensione, come segue::

    namespace AppBundle\DependencyInjection;

    use Symfony\Component\Config\Definition\Builder\TreeBuilder;
    use Symfony\Component\Config\Definition\ConfigurationInterface;

    class Configuration implements ConfigurationInterface
    {
        private $debug;

        public function  __construct($debug)
        {
            $this->debug = (Boolean) $debug;
        }

        public function getConfigTreeBuilder()
        {
            $treeBuilder = new TreeBuilder();
            $rootNode = $treeBuilder->root('un_bundle');

            $rootNode
                ->children()
                    // ...
                    ->booleanNode('logging')->defaultValue($this->debug)->end()
                    // ...
                ->end()
            ;

            return $treeBuilder;
        }
    }

E poi impostato nel costruttore di ``Configuration`` tramite la classe ``Extension``::

    namespace AppBundle\DependencyInjection;

    use Symfony\Component\DependencyInjection\ContainerBuilder;
    use Symfony\Component\HttpKernel\DependencyInjection\Extension;

    class AppExtension extends Extension
    {
        // ...

        public function getConfiguration(array $config, ContainerBuilder $container)
        {
            return new Configuration($container->getParameter('kernel.debug'));
        }
    }

.. sidebar:: Impostare il valore predefinito nell'estensione

    Ci sono alcuni esempi di uso di ``%kernel.debug%`` dentro a una classe ``Configurator``,
    in TwigBundle e in AsseticBundle, tuttavia questo dipende dal fatto che il parametro
    predefinito è impostato dalla classe ``Extension``. Per esempio, in AsseticBundle
    si può trovare::

        $container->setParameter('assetic.debug', $config['debug']);

    La stringa ``%kernel.debug%``, passata qui come parametro, si occupa
    dell'interpretazione per il contenitore, che a sua volta fa la valutazione.
    Entrambi i modi hanno scopi simili. AsseticBundle non userà
    ``%kernel.debug%``, ma invece il nuovo parametro ``%assetic.debug%``.
