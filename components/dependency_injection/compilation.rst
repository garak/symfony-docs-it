Compilazione del contenitore
============================

Ci sono diverse ragioni per compilare il contenitore di servizi. Tra queste, poter
verificare potenziali problemi, come i riferimenti circolari, e rendere il contenitore più
efficiente, risolvendo i parametri e rimuovendo i servizi
inutilizzati.

La compilazione avviene eseguendo::

    $container->compile();

Il metodo ``compile`` usa dei **passi di compilatore** per la compilazione. Il componente
*Dependency Injection* dispone di diversi passi, registrati automaticamente per la
compilazione. Per esempio, :class:`Symfony\\Component\\DependencyInjection\\Compiler\\CheckDefinitionValidityPass`
verifica diversi problemi potenziali con le definizioni impostate nel
contenitore. Dopo questo e molti altri passi, che verificano la validità del
contenitore, ulteriori passi sono usati per ottimizzare la configurazione, prima che sia
messa in cache. Per esempio, vengono rimossi i servizi privati e i servizi astratti, e
vengono risolti gli alias.

Creare un passo di compilatore
------------------------------

Si possono anche creare e registrare i propri passi di compilatore con il contenitore.
Per creare un passo di compilatore, si deve implementare
:class:`Symfony\\Component\\DependencyInjection\\Compiler\\CompilerPassInterface`. Il
compilatore offre la possibilità di manipolare le definizioni del servizio che sono state
compilate. Questo può essere molto potente, ma non necessario nell'uso quotidiano.

Il passo di compilatore deve avere il metodo ``process``, che viene passato al contenitore
che si sta compilando::

    class CustomCompilerPass
    {
        public function process(ContainerBuilder $container)
        {
           //--
        }
    }

Si possono manipolare parametri e definizioni del contenitore, usando i metodi descritti
in :doc:`/components/dependency_injection/definitions`. Un cosa che si fa solitamente in
un passo di compilatore è la ricerca di tutti i servizi con determinato tag, in modo
da poterli processare in quealche modo o collegarli dinamicamente in qualche
altro servizio.

Registrare un passo di compilatore
----------------------------------

Occorre registrare il proprio passo di compilatore con il contenitore. Il suo metodo ``process``
sarà richiamato quando il contenitore viene compilato::

    use Symfony\Component\DependencyInjection\ContainerBuilder;

    $container = new ContainerBuilder();
    $container->addCompilerPass(new CustomCompilerPass);

Controllare l'ordine dei passi
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

I passi di compilatore predefiniti sono raggruppati in passi di ottimizzazione e passi di
rimozione. I passi di ottimizzazione girano prima e includono compiti come la risoluzione
di riferimenti dentro le definizioni. I passi di rimozione eseguono compiti come la
rimozione di alias privati e di servizi inutilizzati. Si può scegliere in quale ordine
sia eseguito ogni passo aggiuntivo. Per impostazione predefinita, sono eseguiti prima dei passi di ottimizzazione.

Si possono usare le seguenti costanti come secondo parametro quando si registra un
passo con il contenitore, per controllare in quale posizione vada il passo:

* ``PassConfig::TYPE_BEFORE_OPTIMIZATION``
* ``PassConfig::TYPE_OPTIMIZE``
* ``PassConfig::TYPE_BEFORE_REMOVING``
* ``PassConfig::TYPE_REMOVE``
* ``PassConfig::TYPE_AFTER_REMOVING``

Per esempio, per eseguire il proprio passo dopo i passi di rimozione predefiniti::

    use Symfony\Component\DependencyInjection\ContainerBuilder;

    $container = new ContainerBuilder();
    $container->addCompilerPass(new CustomCompilerPass, PassConfig::TYPE_AFTER_REMOVING);


Gestire la configurazione con le estensioni
-------------------------------------------

Oltre che caricare la configurazione direttamente nel contenitore, come mostrato in
:doc:`/components/dependency_injection/introduction`, la si può gestire registrando
estensioni con il contenitore. Le estensioni devono implementare :class:`Symfony\\Component\\DependencyInjection\\Extension\\ExtensionInterface`
e possono essere registrare con il contenitore usando::

    $container->registerExtension($extension);

Il lavoro principale dell'estensione viene fatto nel metodo ``load``. In tale metodo, si
può caricare la configurazione da uno o più file di configurazione, oltre che manipolare
le definizioni del contenitore, usando i metodi mostrati in :doc:`/components/dependency_injection/definitions`. 

Il metodo ``load`` viene passato a un nuovo contenitore da configurare, poi fuso nel
contenitore con cui è registrato. Ciò consente di avere molte estensioni che gestiscono
contemporaneamente le definizioni del contenitore in modo indipendente.
Le estensioni sono si aggiungono alla configurazione dei contenitore al momento
dell'aggiunta, ma sono processati al richiamo del metodo ``compile`` del contenitore.

.. note::

    Se si ha bisogno di manipolare la configurazione caricata da un'estensione, non lo si
    può fare da un'altra estensione, perché usa un nuovo contenitore.
    Si deve invece usare un passo di compilatore che funziona con l'intero contenitore,
    dopo che le estensioni sono state processate.

Esportare la configurazione per le prestazioni
----------------------------------------------

L'uso di file di configurazione per gestire il contenitore di servizi può essere molto più
facile da capire rispetto all'uso di PHP, appena ci sono molti servizi. Questa facilità
ha un prezzo, quando si considerano le prestazioni, perché i file di configurazione
necessitano di essere analizzati, in modo da costruire la configurazione in PHP. Si
possono prendere due piccioni con una fava, usando i file di configurazione e poi
esportando e mettendo in cache la configurazione risultante. ``PhpDumper`` rende
facile l'esportazione del contenitore compilato::

    use Symfony\Component\DependencyInjection\ContainerBuilder;
    use Symfony\Component\Config\FileLocator;
    use Symfony\Component\DependencyInjection\Loader\XmlFileLoader;
    use Symfony\Component\DependencyInjection\Dumper\PhpDumper

    $container = new ContainerBuilder();
    $loader = new XmlFileLoader($container, new FileLocator(__DIR__));
    $loader->load('services.xml');

    $file = __DIR__ .'/cache/container.php';

    if (file_exists($file)) {
        require_once $file;
        $container = new ProjectServiceContiner();
    } else {
        $container = new ContainerBuilder();
        //--
        $container->compile();

        $dumper = new PhpDumper($container);
        file_put_contents($file, $dumper->dump());
    }

``ProjectServiceContiner`` è il nome predefinito dato alla classe del contenitore
esportata: lo si può cambiare tramite l'opzione ``class``, al momento
dell'esportazione::

    // ...
    $file = __DIR__ .'/cache/container.php';

    if (file_exists($file)) {
        require_once $file;
        $container = new MyCachedContainer();
    } else {
        $container = new ContainerBuilder();
        //--
        $container->compile();

        $dumper = new PhpDumper($container);
        file_put_contents($file, $dumper->dump(array('class' => 'MyCachedContainer')));
    }

Si otterrà la velocità del contenitore compilato in PHP con la facilità di usare file di
configurazione. Nell'esempio precedente, occorrerà pulire il contenitore in cache ogni
volta che si fa una modifica. L'aggiunta di una variabile che determini se si è in
modalità di debug consente di mantenere la velocità del contenitore in cache
in produzione, mantenendo una configurazione aggiornata durante lo sviluppo
dell'applicazione::

    // ...

    // impostare $isDebug in base a una logica del progetto

    $file = __DIR__ .'/cache/container.php';

    if (!$isDebug && file_exists($file)) {
        require_once $file;
        $container = new MyCachedContainer();
    } else {
        $container = new ContainerBuilder();
        //--
        $container->compile();

        if(!$isDebug) 
            $dumper = new PhpDumper($container);
            file_put_contents($file, $dumper->dump(array('class' => 'MyCachedContainer')));
        }
    }

