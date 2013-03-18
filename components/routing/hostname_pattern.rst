.. index::
   single: Rotte; Corrispondenza per host

Corrispondere una rotta in base all'host
========================================

.. versionadded:: 2.2
    Il supporto per la corrispondenza degli host è stati aggiunto in Symfony 2.2

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

Segnaposto e requisiti negli schemi degli host
----------------------------------------------

Se si usa il :doc:`componente DependencyInjection</components/dependency_injection/index>`
(o l'intero framework Symfony2), si possono usare i
:ref:`parametri del contenitore di servizi<book-service-container-parameters>` come
variabili nelle rotte.

Si può evitare di scrivere a mano un nome di dominio, usando un segnaposto e un requisito.
La stringa ``%domain%`` nei requisiti è rimpiazzata dal valore del parametro ``domain``
del contenitore di servizi.

.. configuration-block::

    .. code-block:: yaml

        mobile_homepage:
            path:     /
            host:     m.{domain}
            defaults: { _controller: AcmeDemoBundle:Main:mobileHomepage }
            requirements:
                domain: %domain%

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
        ), array(
            'domain' => '%domain%',
        ), array(), 'm.{domain}'));

        $collection->add('homepage', new Route('/', array(
            '_controller' => 'AcmeDemoBundle:Main:homepage',
        )));

        return $collection;

.. _component-routing-host-imported:

Aggiungere un'espressione regolare
----------------------------------

Si può impostare un'espressione regolare per un host sulle rotte importate:

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
