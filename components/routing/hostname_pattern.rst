.. index::
   single: Rotte; Corrispondenza per host

Corrispondere una rotta in base all'host
========================================

Si può anche far corrispondere l'*host* HTTP della richiesta in entrata.

.. configuration-block::

    .. code-block:: yaml

        mobile_homepage:
            path:     /
            host:     m.example.com
            defaults: { _controller: AcmeDemoBundle:Main:mobileHomepage }

        homepage:
            path:     /
            defaults: { _controller: AcmeDemoBundle:Main:homepage }

    .. code-block:: xml

        <?xml version="1.0" encoding="UTF-8" ?>

        <routes xmlns="http://symfony.com/schema/routing"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://symfony.com/schema/routing
                http://symfony.com/schema/routing/routing-1.0.xsd"
        >

            <route id="mobile_homepage" path="/" host="m.example.com">
                <default key="_controller">AcmeDemoBundle:Main:mobileHomepage</default>
            </route>

            <route id="homepage" path="/">
                <default key="_controller">AcmeDemoBundle:Main:homepage</default>
            </route>
        </routes>

    .. code-block:: php

        use Symfony\Component\Routing\RouteCollection;
        use Symfony\Component\Routing\Route;

        $collection = new RouteCollection();
        $collection->add('mobile_homepage', new Route('/', array(
            '_controller' => 'AcmeDemoBundle:Main:mobileHomepage',
        ), array(), array(), 'm.example.com'));

        $collection->add('homepage', new Route('/', array(
            '_controller' => 'AcmeDemoBundle:Main:homepage',
        )));

        return $collection;

Entrambe le rotte corrispondono allo stesso percorso ``/``, ma la prima corrisponderà
solo se l'host è ``m.example.com``.

Usare i segnaposto
------------------

L'opzione ``host`` usa la stessa sintassi del sistema di corrispondenza dei percorsi. Questo vuol
dire che si possono usare segnaposto nel nome dell'host:

.. configuration-block::

    .. code-block:: yaml

        projects_homepage:
            path:     /
            host:     "{nome progetto}.example.com"
            defaults: { _controller: AcmeDemoBundle:Main:mobileHomepage }

        homepage:
            path:     /
            defaults: { _controller: AcmeDemoBundle:Main:homepage }

    .. code-block:: xml

        <?xml version="1.0" encoding="UTF-8" ?>

        <routes xmlns="http://symfony.com/schema/routing"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://symfony.com/schema/routing
                http://symfony.com/schema/routing/routing-1.0.xsd"
        >

            <route id="projects_homepage" path="/" host="{nome progetto}.example.com">
                <default key="_controller">AcmeDemoBundle:Main:mobileHomepage</default>
            </route>

            <route id="homepage" path="/">
                <default key="_controller">AcmeDemoBundle:Main:homepage</default>
            </route>
        </routes>

    .. code-block:: php

        use Symfony\Component\Routing\RouteCollection;
        use Symfony\Component\Routing\Route;

        $collection = new RouteCollection();
        $collection->add('project_homepage', new Route('/', array(
            '_controller' => 'AcmeDemoBundle:Main:mobileHomepage',
        ), array(), array(), '{nome progetto}.example.com'));

        $collection->add('homepage', new Route('/', array(
            '_controller' => 'AcmeDemoBundle:Main:homepage',
        )));

        return $collection;

Si possono anche impostare requisiti e opzioni predefinite per i segnaposto. Per
esempio, se si vuole che ``m.example.com`` e
``mobile.example.com`` corrispondano, si può usare:

.. configuration-block::

    .. code-block:: yaml

        mobile_homepage:
            path:     /
            host:     "{subdomain}.example.com"
            defaults:
                _controller: AcmeDemoBundle:Main:mobileHomepage
                subdomain: m
            requirements:
                subdomain: m|mobile

        homepage:
            path:     /
            defaults: { _controller: AcmeDemoBundle:Main:homepage }

    .. code-block:: xml

        <?xml version="1.0" encoding="UTF-8" ?>

        <routes xmlns="http://symfony.com/schema/routing"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://symfony.com/schema/routing
                http://symfony.com/schema/routing/routing-1.0.xsd"
        >

            <route id="mobile_homepage" path="/" host="{subdomain}.example.com">
                <default key="_controller">AcmeDemoBundle:Main:mobileHomepage</default>
                <default key="subdomain">m</default>

                <requirement key="subdomain">m|mobile</requirement>
            </route>

            <route id="homepage" path="/">
                <default key="_controller">AcmeDemoBundle:Main:homepage</default>
            </route>
        </routes>

    .. code-block:: php

        use Symfony\Component\Routing\RouteCollection;
        use Symfony\Component\Routing\Route;

        $collection = new RouteCollection();
        $collection->add('mobile_homepage', new Route('/', array(
            '_controller' => 'AcmeDemoBundle:Main:mobileHomepage',
            'subdomain'   => 'm',
        ), array(
            'subdomain' => 'm|mobile',
        ), array(), '{subdomain}.example.com'));

        $collection->add('homepage', new Route('/', array(
            '_controller' => 'AcmeDemoBundle:Main:homepage',
        )));

        return $collection;

.. sidebar:: Uso dei parametri dei servizi

    Si possono anche usare i parametri dei servizi, se non si vuole scrivere il
    nome dell'host direttamente:

    .. configuration-block::

        .. code-block:: yaml

            mobile_homepage:
                path:     /
                host:     "m.{domain}"
                defaults:
                    _controller: AcmeDemoBundle:Main:mobileHomepage
                    domain: "%domain%"
                requirements:
                    domain: "%domain%"

            homepage:
                path:  /
                defaults: { _controller: AcmeDemoBundle:Main:homepage }

        .. code-block:: xml

            <?xml version="1.0" encoding="UTF-8" ?>

            <routes xmlns="http://symfony.com/schema/routing"
                xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
                xsi:schemaLocation="http://symfony.com/schema/routing http://symfony.com/schema/routing/routing-1.0.xsd">

                <route id="mobile_homepage" path="/" host="m.example.com">
                    <default key="_controller">AcmeDemoBundle:Main:mobileHomepage</default>
                    <default key="domain">%domain%</requirement>
                    <requirement key="domain">%domain%</requirement>
                </route>

                <route id="homepage" path="/">
                    <default key="_controller">AcmeDemoBundle:Main:homepage</default>
                </route>
            </routes>

        .. code-block:: php

            use Symfony\Component\Routing\RouteCollection;
            use Symfony\Component\Routing\Route;

            $collection = new RouteCollection();
            $collection->add('mobile_homepage', new Route('/', array(
                '_controller' => 'AcmeDemoBundle:Main:mobileHomepage',
                'domain' => '%domain%',
            ), array(
                'domain' => '%domain%',
            ), array(), 'm.{domain}'));

            $collection->add('homepage', new Route('/', array(
                '_controller' => 'AcmeDemoBundle:Main:homepage',
            )));

            return $collection;

    .. tip::

       Assicurarsi di includere anche un'opzione per il segnaposto ``subdomain``,
       atrlimenti occorrerà includere i valori dei sottodomini ogni volta
       che si genera la rotta.

.. _component-routing-host-imported:

Corrispondenza dell'host su rotte importate
-------------------------------------------

Si può impostare l'opzione ``host`` sulle rotte importate:

.. configuration-block::

    .. code-block:: yaml

        # app/config/routing.yml
        acme_hello:
            resource: "@AcmeHelloBundle/Resources/config/routing.yml"
            host:     "hello.example.com"

    .. code-block:: xml

        <!-- app/config/routing.xml -->
        <?xml version="1.0" encoding="UTF-8" ?>

        <routes xmlns="http://symfony.com/schema/routing"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://symfony.com/schema/routing http://symfony.com/schema/routing/routing-1.0.xsd">

            <import resource="@AcmeHelloBundle/Resources/config/routing.xml" host="hello.example.com" />
        </routes>

    .. code-block:: php

        // app/config/routing.php
        use Symfony\Component\Routing\RouteCollection;

        $collection = new RouteCollection();
        $collection->addCollection($loader->import("@AcmeHelloBundle/Resources/config/routing.php"), '', array(), array(), array(), 'hello.example.com');

        return $collection;

L'host ``hello.example.com`` sarà impostato su ciascuna rotta caricata dalla nuova
risorsa delle rotte.

Testare i controllori
---------------------

Se si vuole far funzionare la corrispondenza degli URL nei test funzionali, occorre
impostare l'header ``HTTP_HOST`` negli oggetti richiesta.

.. code-block:: php

    $crawler = $client->request(
        'GET',
        '/homepage',
        array(),
        array(),
        array('HTTP_HOST' => 'm.' . $client->getContainer()->getParameter('domain'))
    );
