.. index::
   single: DependencyInjection; Configurazione avanzata

Configurazione avanzata del contenitore
=======================================

Segnare i servizi come pubblici / privati
-----------------------------------------

Quando si definisce un servizio, di solito si vuole potervi accedere dall'interno
di un'applicazione. Tali servizi sono chiamati "pubblici". Per esempio, il servizio
``doctrine`` registrato con il contenitore durante l'uso di DoctrineBundle
è un servizio pubblico, accessibile tramite::

   $doctrine = $container->get('doctrine');

Ci sono tuttavia dei casi in cui non si desidera che un servizio sia pubblico.
Di solito avviene quando un servizio è definito solo per essere usato come parametro
da un altro servizio.

.. _inlined-private-services:

.. note::

    Se si usa un servizio privato come parametro di più di un altro servizio,
    ciò provocherà un'istanza in linea (p.e. ``new PippoPlutoPrivato()``) all'interno
    di quest'altro servizio, rendendola non disponibile pubblicamente a runtime.

In parole povere: un servizio sarà privato quanto non si vuole che sia accessibile
direttamente dal codice.

Ecco un esempio:

.. configuration-block::

    .. code-block:: yaml

        services:
           pippo:
             class: Esempio\Pippo
             public: false

    .. code-block:: xml

        <?xml version="1.0" encoding="UTF-8" ?>
        <container xmlns="http://symfony.com/schema/dic/services"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://symfony.com/schema/dic/services http://symfony.com/schema/dic/services/services-1.0.xsd">

            <services>
                <service id="pippo" class="Esempio\Pippo" public="false" />
            </services>
        </container>

    .. code-block:: php

        use Symfony\Component\DependencyInjection\Definition;

        $definition = new Definition('Esempio\Pippo');
        $definition->setPublic(false);
        $container->setDefinition('pippo', $definition);

Essendo il servizio privato, *non* si può richiamare::

    $container->get('pippo');

Tuttavia, se un servizio è stato segnato come privato, gli si può comunque assegnare un
alias (vedere sotto) per accedervi (tramite alias).

.. note::

   I servizi sono predefiniti come pubblici.

Servizi sintetici
-----------------

I servizi sintetici sono servizi che vengono iniettati nel contenitore, invece
di essere creati dal contenitore stesso.

Per esempio, se si usa il componente :doc:`HttpKernel </components/http_kernel/introduction>`
con il componente DependencyInjection, il servizio ``request``
è iniettato nel metodo
:method:`ContainerAwareHttpKernel::handle() <Symfony\\Component\\HttpKernel\\DependencyInjection\\ContainerAwareHttpKernel::handle>`,
quando entra nello :doc:`scope </cookbook/service_container/scopes>` della richiesta.
Se non c'è una richiesta, la classe non esiste, quindi non può essere inclusa nella
configurazione del contenitore. Inoltre, il servizio deve essere diverso per ogni
sotto-richiesta nell'applicazione.

Per creare un servizio sintetico, impostare ``synthetic`` a ``true``:

.. configuration-block::

    .. code-block:: yaml

        services:
            request:
                synthetic: true

    .. code-block:: xml

        <?xml version="1.0" encoding="UTF-8" ?>
        <container xmlns="http://symfony.com/schema/dic/services"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://symfony.com/schema/dic/services http://symfony.com/schema/dic/services/services-1.0.xsd">

            <services>
                <service id="request" synthetic="true" />
            </services>
        </container>

    .. code-block:: php

        use Symfony\Component\DependencyInjection\Definition;

        $container
            ->setDefinition('request', new Definition())
            ->setSynthetic(true);

Come si può vedere, viene impostata solo l'opzione ``synthetic``. Tutte le altre opzioni vengono solo usate
per configurare il modo in cui un servizio viene creato dal contenitore. Non essendo il servizio
creato dal contenitore, tali opzioni sono omesse.

Si può ora iniettare la classe, usando
:method:`Container::set <Symfony\\Component\\DependencyInjection\\Container::set>`::

    // ...
    $container->set('request', new MyRequest(...));

Alias
-----

A volte si ha bisogno di usare scorciatoie per accedere ad alcuni servizi. Si possono
impostare degli alias e si può anche impostare un alias su un servizio non
pubblico.

.. configuration-block::

    .. code-block:: yaml

        services:
           pippo:
             class: Esempio\Pippo
           pluto:
             alias: pippo

    .. code-block:: xml

        <?xml version="1.0" encoding="UTF-8" ?>
        <container xmlns="http://symfony.com/schema/dic/services"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://symfony.com/schema/dic/services http://symfony.com/schema/dic/services/services-1.0.xsd">

            <services>
                <service id="pippo" class="Esempio\Pippo" />

                <service id="pluto" alias="pippo" />
            </services>
        </container>

    .. code-block:: php

        use Symfony\Component\DependencyInjection\Definition;

        $container->setDefinition('pippo', new Definition('Esempio\Pippo'));

        $containerBuilder->setAlias('pluto', 'pippo');

Ciò vuol dire che, quando si usa direttamente il contenitore, si può accedere al servizio
``pippo`` richiedendo il servizio ``pluto``, in questo modo::

    $container->get('pluto'); // restituisce il servizio pippo

.. tip::

    In YAML, si può anche usare una scorciatoia come alias di un servizio:

    .. code-block:: yaml

        services:
           pippo:
             class: Esempio\Pippo
           pluto: "@pippo"


Richiesta di file
-----------------

Possono esserci dei casi in cui occorra includere altri file subito prima che il
servizio stesso sia caricato. Per poterlo fare, si può usare la direttiva ``file``.

.. configuration-block::

    .. code-block:: yaml

        services:
           foo:
             class: Esempio\Pippo\Pluto
             file: "%kernel.root_dir%/src/percorso/del/file/pippo.php"

    .. code-block:: xml

        <?xml version="1.0" encoding="UTF-8" ?>
        <container xmlns="http://symfony.com/schema/dic/services"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://symfony.com/schema/dic/services http://symfony.com/schema/dic/services/services-1.0.xsd">

            <services>
                <service id="foo" class="Esempio\Pippo\Pluto">
                    <file>%kernel.root_dir%/src/percorso/del/file/pippo.php</file>
                </service>
            </services>
        </container>

    .. code-block:: php

        use Symfony\Component\DependencyInjection\Definition;

        $definition = new Definition('Esempio\Pippo\Pluto');
        $definition->setFile('%kernel.root_dir%/src/percorso/del/file/pippo.php');
        $container->setDefinition('pippo', $definition);

Si noti che Symfony richiamerà internamente la funzione ``require_once`` di PHP,
il che vuol dire che il file sarà incluso una sola volta per richiesta. 
