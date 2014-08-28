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

Decorare i servizi
------------------

.. versionadded:: 2.5
    I servizi decorati sono stati introdotti in Symfony 2.5.

Quando si sovrascrive una definizione esistente, il vecchio servizio va perduto:

.. code-block:: php

    $container->register('pippo', 'ServizioPippo');

    // questo rimpiazzerà la vecchia definizione con quella nuova
    // la vecchia definizione va perduta
    $container->register('pippo', 'NuovoServizioPippo');

La maggior parte delle volte questo è esattamente quello che si desidera. A volte, però,
si potrebbe invece voler decorare il vecchio servizio. In questo caso, il
vecchio servizio viene mantenuto, per potervi fare riferimento all'interno
del nuovo. Questa configurazione sostituisce ``pippo`` con un nuovo servizio, ma mantiene
un riferimento al vecchio, come ``pluto.inner``:

.. configuration-block::

    .. code-block:: yaml

       bar:
         public: false
         class: stdClass
         decorates: pippo
         arguments: ["@pluto.inner"]

    .. code-block:: xml

        <service id="bar" class="stdClass" decorates="pippo" public="false">
            <argument type="service" id="pluto.inner" />
        </service>

    .. code-block:: php

        use Symfony\Component\DependencyInjection\Reference;

        $container->register('bar', 'stdClass')
            ->addArgument(new Reference('pluto.inner'))
            ->setPublic(false)
            ->setDecoratedService('pippo');

Ecco quello che succede: il metodo ``setDecoratedService()` dice
al contenitore che il servizio ``pluto`` sostituisce il servizio ``pippo``,
rinominando ``pippo`` in ``pluto.inner``.
Per convenzione, il vecchio servizio ``pippo`` è rinominato ``pluto.inner``,
in modo da poterlo iniettare nel nuovo servizio.

.. note::
    L'identificativo interno generato è basato sull'id del servizio generato
    (``pluto``, in questo caso), non su quello del servizio decorato (``pippo``, in questo caso). 
    Questo è necessario, per consentire più decoratori sullo stesso servizio (devono avere
    id generati diversi).

    La maggior parte delle volte, il decoratore deve essere dichiarato privato, perché non ci sarà
    bisogno di recuperarlo come ``pluto`` dal contenitore. La visibilità del
    servizio edcorato ``pippo`` (che è un alias per ``pluto``) resterà quella
    originale di ``pippo``.

Si può cambiare il nome del servizio interno, se lo si desidera:

.. configuration-block::

    .. code-block:: yaml

       bar:
         class: stdClass
         public: false
         decorates: pippo
         decoration_inner_name: pluto.wooz
         arguments: ["@pluto.wooz"]

    .. code-block:: xml

        <service id="bar" class="stdClass" decorates="pippo" decoration-inner-name="pluto.wooz" public="false">
            <argument type="service" id="pluto.wooz" />
        </service>

    .. code-block:: php

        use Symfony\Component\DependencyInjection\Reference;

        $container->register('bar', 'stdClass')
            ->addArgument(new Reference('pluto.wooz'))
            ->setPublic(false)
            ->setDecoratedService('pippo', 'pluto.wooz');
