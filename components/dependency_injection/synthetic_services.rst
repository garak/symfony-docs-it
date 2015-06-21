.. index::
    single: DependencyInjection; Servizi sintetici

Iniettare istanze dento al contenitore
--------------------------------------

Usando il contenitore in un'applicazione, a volte si ha bisogno di iniettare
un'istanza, invece di configurare il contenitore per creare una nuova istanza.

Per esempi, se si usa il componente :doc:`HttpKernel </components/http_kernel/introduction>`
con il componente DependencyInjection, il servizio ``kernel``
è iniettato nel contenitore dalla classe ``Kernel``::

    // ...
    abstract class Kernel implements KernelInterface, TerminableInterface
    {
        // ...
        protected function initializeContainer()
        {
            // ...
            $this->container->set('kernel', $this);

            // ...
        }
    }

Il servizio ``kernel`` è chiamato servizio sintetico. Questo servizio va
configurato nel contenitore, in modo che il contenitore sappia che il servizio esiste
durante la compilazione (altrimenti, servizi che dipendono dal servizio ``kernel``
otterrebbero un errore "service does not exists").

Per poterlo fare, si deve usare
:method:`Definition::setSynthetic() <Symfony\\Component\\DependencyInjection\\Definition::setSynthetic>`::

    use Symfony\Component\DependencyInjectino\Definition;

    // i servizi sintetici non specificano una classe
    $kernelDefinition = new Definition();
    $kernelDefinition->setSynthetic(true);

    $container->setDefinition('un_servizio', $kernelDefinition);

Ora, si può iniettare l'istanza nel contenitore, usando
:method:`Container::set() <Symfony\\Component\\DependencyInjection\\Container::set>`::

    $yourService = new YourObject();
    $container->set('un_servizio', $unServizio);

``$container->get('un_servizio')`` ora restituirà la stessa istanza di
``$unServizio``.
