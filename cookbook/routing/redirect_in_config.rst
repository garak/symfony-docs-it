.. index::
   single: Rotte; Rinvio con Framework:RedirectController

Configurare un rinvio senza controllori ad hoc
==============================================

A volte, occorre rinviare un URL a un altro URL. Lo si può fare creando
una nuova azione, in cui l'unico compito è il rinvio, ma l'uso di
:class:`Symfony\\Bundle\\FrameworkBundle\\Controller\\RedirectController`
di FrameworkBundle è ancora più semplice.

You can redirect to a specific path (e.g. ``/about``) or to a specific route
using its name (e.g. ``homepage``).

Rinvio usando un percorso
-------------------------

Si ipotizzi di non avere un controllore impostato per il percorso ``/`` dell'applicazione
e di voler rinviare tali richieste ad ``/app``. Si dovrà usare l'azione
:method:`Symfony\\Bundle\\FrameworkBundle\\Controller\\RedirectController::urlRedirect`
per rinviare al nuovo URL:

.. configuration-block::

    .. code-block:: yaml

        # app/config/routing.yml

        # caricare alcune rotte, bisogna avere il percorso "/app"
        AppBundle:
            resource: "@AcmeAppBundle/Controller/"
            type:     annotation
            prefix:   /app

        # rinvio della radice
        root:
            path: /
            defaults:
                _controller: FrameworkBundle:Redirect:urlRedirect
                path: /app
                permanent: true

    .. code-block:: xml

        <!-- app/config/routing.xml -->
        <?xml version="1.0" encoding="UTF-8" ?>
        <routes xmlns="http://symfony.com/schema/routing"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://symfony.com/schema/routing
                http://symfony.com/schema/routing/routing-1.0.xsd">

            <!-- caricare alcune rotte, bisogna avere il percorso "/app" -->
            <import resource="@AcmeAppBundle/Controller/"
                type="annotation"
                prefix="/app"
            />

            <!-- rinvio della radice -->
            <route id="root" path="/">
                <default key="_controller">FrameworkBundle:Redirect:urlRedirect</default>
                <default key="path">/app</default>
                <default key="permanent">true</default>
            </route>
        </routes>

    .. code-block:: php

        // app/config/routing.php
        use Symfony\Component\Routing\RouteCollection;
        use Symfony\Component\Routing\Route;

        $collection = new RouteCollection();

        // caricare alcune rotte, bisogna avere il percorso "/app"
        $acmeApp = $loader->import(
            "@AcmeAppBundle/Controller/",
            "annotation"
        );
        $acmeApp->setPrefix('/app');

        $collection->addCollection($acmeApp);

        // rinvio della radice
        $collection->add('root', new Route('/', array(
            '_controller' => 'FrameworkBundle:Redirect:urlRedirect',
            'path'        => '/app',
            'permanent'   => true,
        )));

        return $collection;

In questo esempio, configuriamo una rotta per il percorso ``/`` e facciamo in modo che
``RedirectController`` lo rinvii a ``/app``. L'opzione ``permanent``
dice all'azione di inviare un codice di stato HTTP ``301``, invece del codice predefinito
``302``.

Rinvio usando una rotta
-----------------------

Si ipotizzi di migrare un sito da WordPress a Symfony, si vuole
rinviare ``/wp-admin`` alla rotta ``sonata_admin_dashboard``. Non si conosce
il percorso, ma solo il nome della rotta. Lo si può fare con l'azione
:method:`Symfony\\Bundle\\FrameworkBundle\\Controller\\RedirectController::redirect`:


.. configuration-block::

    .. code-block:: yaml

        # app/config/routing.yml

        # ...

        # rinvio dell'admin
        root:
            path: /wp-admin
            defaults:
                _controller: FrameworkBundle:Redirect:redirect
                route: sonata_admin_dashboard
                permanent: true

    .. code-block:: xml

        <!-- app/config/routing.xml -->
        <?xml version="1.0" encoding="UTF-8" ?>
        <routes xmlns="http://symfony.com/schema/routing"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://symfony.com/schema/routing
                http://symfony.com/schema/routing/routing-1.0.xsd">

            <!-- ... -->

            <!-- rinvio dell'admin -->
            <route id="root" path="/wp-admin">
                <default key="_controller">FrameworkBundle:Redirect:redirect</default>
                <default key="route">sonata_admin_dashboard</default>
                <default key="permanent">true</default>
            </route>
        </routes>

    .. code-block:: php

        // app/config/routing.php
        use Symfony\Component\Routing\RouteCollection;
        use Symfony\Component\Routing\Route;

        $collection = new RouteCollection();
        // ...

        // rinvio dell'admin
        $collection->add('root', new Route('/wp-admin', array(
            '_controller' => 'FrameworkBundle:Redirect:redirect',
            'route'       => 'sonata_admin_dashboard',
            'permanent'   => true,
        )));

        return $collection;

.. caution::

    Poiché si sta rinviando a una rotta e non a un percorso, l'opione richiesta
    è ``route`` nell'azione ``redirect``, invece di ``path``
    nell'azione ``urlRedirect``.
