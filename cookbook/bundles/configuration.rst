.. index::
   single: Configurazione; Semantica
   single: Bundle; Configurazione di Extension

Creare una configurazione amichevole per un bundle
==================================================

Se si apre il file di configurazione di un'applicazione (solitamente ``app/config/config.yml``),
si vedranno vari "spazi di nomi" di configurazione, come ``framework``,
``twig`` e ``doctrine``. Ciascuno di questi configura uno specifico bundle, consentendo
di configurare le cose ad alto livello e quindi lasciando al bundle tutte le modifiche
complesse e a basso livello, in base alle impostazioni.

Per esempio, il codice seguente dice a FrameworkBundle di abilitare l'integrazione con
i form, che implica la definizione di alcuni servizi, come anche
l'integrazione con alcuni componenti correlati:

.. configuration-block::

    .. code-block:: yaml

        framework:
            form: true

    .. code-block:: xml

        <?xml version="1.0" encoding="UTF-8" ?>
        <container xmlns="http://symfony.com/schema/dic/services"
            xmlns:framework="http://symfony.com/schema/dic/symfony"
            xsi:schemaLocation="http://symfony.com/schema/dic/services
                http://symfony.com/schema/dic/services/services-1.0.xsd
                http://symfony.com/schema/dic/symfony
                http://symfony.com/schema/dic/symfony/symfony-1.0.xsd">

            <framework:config>
                <framework:form />
            </framework:config>
        </container>

    .. code-block:: php

        $container->loadFromExtension('framework', array(
            'form' => true,
        ));

.. sidebar:: Uso di parametri per configurare un bundle

    Se non si pensa di condividere un bundle tra progetti diversi, non ha molto
    senso usare questa modalità avanzata di configurazione. Usando un bundle
    in un unico progetto, si può semplicemente cambiare la configurazione
    del servizio ogni volta.

    Ma se invece si vuole essere in grado di configurare qualcosa dall'interno di
    ``config.yml``, si può sempre creare un parametro e usarlo
    da qualche altra parte.

Usare l'estensione del bundle
-----------------------------

L'idea di base è che, invece di costringere l'utente a sovrascrivere ciascun
parametro, gli si dia la possibilità di configurare alcune opzioni, create
appositamente. Come sviluppatore del bundle, si deve quindi analizzare tale configurazione e
caricare i servizi e i parametri corretti, dentro a una classe "Extension".

Come esempio, si immagini di star creando un bundle sociale, che fornisce
integrazione con Twitter e simili. Per poter riusare questo bundle, occorre
rendere le variabili ``client_id`` e ``client_secret`` configurabili. La
configurazione del bundle sarebbe simile a questa:

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        acme_social:
            twitter:
                client_id: 123
                client_secret: $ecret

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <?xml version="1.0" ?>

        <container xmlns="http://symfony.com/schema/dic/services"
            xmlns:acme-social="http://example.org/dic/schema/acme_social"
            xsi:schemaLocation="http://symfony.com/schema/dic/services
                http://symfony.com/schema/dic/services/services-1.0.xsd">

           <acme-social:config>
               <twitter client-id="123" client-secret="$ecret" />
           </acme-social:config>

           <!-- ... -->
        </container>

    .. code-block:: php

        // app/config/config.php
        $container->loadFromExtension('acme_social', array(
            'client_id'     => 123,
            'client_secret' => '$ecret',
        ));

.. seealso::

    Si può approfondire sull'estensione in :doc:`/cookbook/bundles/extension`.

.. tip::

    Se un bundle fornisce una classe Extension, *non* si dovrebbe  in generale
    sovrascrivere alcun parametro del contenitore di servizi per quel bundle. L'idea
    è che, se è presente una classe Extension, ogni impostazione che si possa
    configurare dovrebbe essere presente nella configurazione messa a disposizione da
    tale classe. In altre parole, la classe Extension definisce tutte le impostazioni
    pubbliche per cui sara mantenuta una retrocompatibilità.

.. seealso::

    Per gestire parametri dentro una classe Dependency Injection, vedere
    :doc:`/cookbook/configuration/using_parameters_in_dic`.


Processare l'array ``$configs``
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Per prima cosa, occorre creare una classe Extension, come spiegato in
:doc:`extension`.

Ogni volta che un utente include la voce ``acme_social`` (che è l'alias DI) in un
file di configurazione, la configurazione ivi presente viene aggiunta a un array di
configurazioni e passata al metodo ``load()`` dell'estensione (Symfony
converte automaticamente XML e YAML in array).

Per l'esempio di configurazione nella sezione precedente, l'array passato al metodo
``load()`` sarebbe simile a questo::

    array(
        array(
            'twitter' => array(
                'client_id' => 123,
                'client_secret' => '$ecret',
            ),
        ),
    )

Si noti che questo è un *array di array*, non un semplice array di valori di
configurazione. Questo è intenzionale, perché consente a Symfony di analizzare
varie risorse di configurazione. Per esempio, se ``acme_social`` appare in un
altro file di configurazione, per esempio in ``config_dev.yml``, con valori
differenti, l'array sarebbe simile a questo::

    array(
        // values from config.yml
        array(
            'twitter' => array(
                'client_id' => 123,
                'client_secret' => '$secret',
            ),
        ),
        // values from config_dev.yml
        array(
            'twitter' => array(
                'client_id' => 456,
            ),
        ),
    )

L'ordine dei due array dipende da quale è stato creato prima.

Niente paura! Il componente Config di Symfony fornirà un aiuto per la fusione di questi valori,
fornendo dei valori predefiniti e restituendo errori di validazione in caso di configurazioni errate.
Ecco come funziona. Creare una classe ``Configuration`` nella cartella
``DependencyInjection`` e costruire un albero che definisca la struttura
della configurazione del bundle.

La classe ``Configuration`` per gestire la configurazione di esempio è simile a questa::

    // src/Acme/SocialBundle/DependencyInjection/Configuration.php
    namespace Acme\SocialBundle\DependencyInjection;

    use Symfony\Component\Config\Definition\Builder\TreeBuilder;
    use Symfony\Component\Config\Definition\ConfigurationInterface;

    class Configuration implements ConfigurationInterface
    {
        public function getConfigTreeBuilder()
        {
            $treeBuilder = new TreeBuilder();
            $rootNode = $treeBuilder->root('acme_social');

            $rootNode
                ->children()
                    ->arrayNode('twitter')
                        ->children()
                            ->integerNode('client_id')->end()
                            ->scalarNode('client_secret')->end()
                        ->end()
                    ->end() // twitter
                ->end()
            ;

            return $treeBuilder;
        }
    }

.. seealso::

    La classe ``Configuration`` può essere molto più complicata di quanto mostrato,
    con supporto per nodi "prototipo", validazione avanzata, normalizzazione di XML
    e fusione avanzata. Si possono approfondire questi aspetti nella
    :doc:`documentazione del componente Config </components/config/definition>`. Si
    può anche vederla in azione, dando un'occhiata alcune classi Configuration,
    come quelle della `configurazione di FrameworkBundle`_ o della
    `configurazione di TwigBundle`_.

Si può ora usare questa classe nel metodo ``load()``, per fondere le configurazioni e
forzare la validazione (p.e. se è stata passata un'opzione in più, viene lanciata
un'eccezione)::

    public function load(array $configs, ContainerBuilder $container)
    {
        $configuration = new Configuration();

        $config = $this->processConfiguration($configuration, $configs);
        // ...
    }

Il metodo ``processConfiguration()`` usa l'albero di configurazione definito
nella classe ``Configuration`` per validare, normalizzare e fondere tutti gli
array di configurazione insieme.

.. tip::

    Invece di richiamare ``processConfiguration()`` nell'estensione  ogni volta che si
    forniscono opzioni di configurazione, si potrebbe voler usare
    :class:`Symfony\\Component\\HttpKernel\\DependencyInjection\\ConfigurableExtension`,
    che lo fa in modo automatico::

        // src/Acme/HelloBundle/DependencyInjection/AcmeHelloExtension.php
        namespace Acme\HelloBundle\DependencyInjection;

        use Symfony\Component\DependencyInjection\ContainerBuilder;
        use Symfony\Component\HttpKernel\DependencyInjection\ConfigurableExtension;

        class AcmeHelloExtension extends ConfigurableExtension
        {
            // notare che questo metodo si chiama "loadInternal" e non "load"
            protected function loadInternal(array $mergedConfig, ContainerBuilder $container)
            {
                // ...
            }
        }

    Questa classe usa il metodo ``getConfiguration()`` per ottenere l'istanza di Configuration,
    va sovrascritto se la classe di configurazione non si chiama
    ``Configuration`` o se non si trova nello stesso spazio di nomi
    dell'estensione.

.. sidebar:: Processare la configurazione in autonomia

    L'uso del componente Config è assolutamente facoltativo. Il metodo ``load()`` ottiene un
    array di valori di configurazione. Si possono semplicemente analizzare gli array
    (p.e. sovrascrivendo le configurazione e usando :phpfunction:`isset` per verificare
    l'esistenza di un valore). Si faccia attenzione, perché è molto difficile supportare XML.

    .. code-block:: php

        public function load(array $configs, ContainerBuilder $container)
        {
            $config = array();
            // fa in modo che le risorse sovrascrivano il valore precedente
            foreach ($configs as $subConfig) {
                $config = array_merge($config, $subConfig);
            }

            // ... usare ora l'array $config
        }

Modificare la configurazione di un altro bundle
-----------------------------------------------

Se si hanno più bundle che dipendono l'uno dall'altro, potrebbe essere utile
consentire a una classe ``Extension`` di modificare la configurazione passata alla
classe ``Extension`` di un altro bundle, come se lo sviluppatore finale avesse effettivamente messo tale
configurazione nel suo file ``app/config/config.yml``. Lo si può fare,
usando una ``PrependExtension``. Per maggiori dettagli, vedere
:doc:`/cookbook/bundles/prepend_extension`.

Esportare la configurazione
---------------------------

Il comando ``config:dump-reference`` esporta la configurazione predefinita di un
bundle in console, usando il formato YAML.

Se la configurazione di un bundle si trova nella posizione standard
(``MioBundle\DependencyInjection\Configuration``) e non ha bisogno
di parametri da passare al costruttore, funzionerà automaticamente. Se
qualcosa cambia, la classe ``Extension`` deve sovrascrivere il metodo
:method:`Extension::getConfiguration() <Symfony\\Component\\HttpKernel\\DependencyInjection\\Extension::getConfiguration>`
e restituire un'istanza di ``Configuration``.

Supportare XML
--------------

Symfony consente agli sviluppatori di fornire configurazioni in tre formati:
YAML, XML e PHP. Sia YAML sia PHP usano la stessa sintassi e sono supportati
nativamente dal componente Config. Il supporto per XML richiede un po' di lavoro
aggiuntivo. Ma, quando si condivide un bundle con altri, si raccomanda di
seguire questi passi.

Preparare l'albero di configurazione per XML
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Il componente Config fornisce alcuni metodi per consentire di processare correttamente
configurazioni XML. Vedere ":ref:`component-config-normalization`" della
documentazione del componente. Tuttavia, si possono anche fare alcune cose facoltative,
per aumentare l'esperienza nell'uso della configurazione XML:

Scegliere uno spazio di nomi XML
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

In XML, lo `spazio dei nomi XML`_ viene usato per determinare quali elementi appartengono
alla configurazione di un determinato bundle. Lo spazio dei nomi è restituito dal metodo
:method:`Extension::getNamespace() <Symfony\\Component\\DependencyInjection\\Extension\\Extension::getNamespace>`.
Per convenzione, lo spazio dei nomi è un URL (non deve essere un
URL valido, né deve necessariamente esistere). Per impostazione predefinita, lo spazio dei nomi è
``http://example.org/dic/schema/ALIAS_DI``, dove ``ALIAS_DI`` e l'aliasi DI
dell'estensione. La si può cambiare in un URL più professionale::

    // src/Acme/HelloBundle/DependencyInjection/AcmeHelloExtension.php

    // ...
    class AcmeHelloExtension extends Extension
    {
        // ...

        public function getNamespace()
        {
            return 'http://acme_company.com/schema/dic/hello';
        }
    }

Fornire uno schema XML
~~~~~~~~~~~~~~~~~~~~~~

XML ha una caratteristica molto utile, chiamata `schema XML`_. Consente di
descrivere tutti i possibili elementi e attributi e i valori in una definizione di schema
XML (un file xsd). Questo file XSD è usato dagli IDE per l'autocompletamento ed
è usato dal componente Config per validare gli elementi.

Per usare lo schema, il file di configurazione XML deve fornire un attributo
``xsi:schemaLocation``, che punti al file XSD per un determinato spazio di nomi XML.
Questa posizione inizia sempre con lo spazio di nomi XML. Questo spazio di nomi XML
viene quindi sostituito dal percorso base di validazione XSD, restituito dal metodo
:method:`Extension::getXsdValidationBasePath() <Symfony\\Component\\DependencyInjection\\ExtensionInterface::getXsdValidationBasePath>`.
Questo spazio di nomi è quindi seguito dal resto del percorso dal percorso base
fino al file stesso.

Per convenzione, il file XSD si trova in ``Resources/config/schema``, ma lo
si può mettere in altre posizioni. Si deve restituire questo percorso come percorso base::

    // src/Acme/HelloBundle/DependencyInjection/AcmeHelloExtension.php

    // ...
    class AcmeHelloExtension extends Extension
    {
        // ...

        public function getXsdValidationBasePath()
        {
            return __DIR__.'/../Resources/config/schema';
        }
    }

Ipotizzando che il file XSD si chiami ``hello-1.0.xsd``, la posizione dello schema sarebbe
``http://acme_company.com/schema/dic/hello/hello-1.0.xsd``:

.. code-block:: xml

    <!-- app/config/config.xml -->
    <?xml version="1.0" ?>

    <container xmlns="http://symfony.com/schema/dic/services"
        xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
        xmlns:acme-hello="http://acme_company.com/schema/dic/hello"
        xsi:schemaLocation="http://acme_company.com/schema/dic/hello
            http://acme_company.com/schema/dic/hello/hello-1.0.xsd">

        <acme-hello:config>
            <!-- ... -->
        </acme-hello:config>

        <!-- ... -->
    </container>

.. _`configurazione di FrameworkBundle`: https://github.com/symfony/symfony/blob/master/src/Symfony/Bundle/FrameworkBundle/DependencyInjection/Configuration.php
.. _`configurazione di TwigBundle`: https://github.com/symfony/symfony/blob/master/src/Symfony/Bundle/TwigBundle/DependencyInjection/Configuration.php
.. _`spazio dei nomi XML`: http://en.wikipedia.org/wiki/XML_namespace
.. _`schema XML`: http://en.wikipedia.org/wiki/XML_schema
