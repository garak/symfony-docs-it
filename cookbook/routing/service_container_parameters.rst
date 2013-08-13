.. index::
   single: Rotte; Parametri del contenitore di servizi

Come usare i parametri del contenitore di servizi nelle rotte
=============================================================

.. versionadded:: 2.1
    La possibilità di usare parametri nelle rotte è stata aggiunta in Symfony 2.1.

A volte potrebbe essere utile rendere alcune parti delle rotte
configurabili globalmente. Per esempio, se si costruisce un sito internazionalizzato,
probabilmente si inizierà con una o due lingue. Sicuramente, si aggiungerà un
requisito alle rotte, per prevenire la corrispondenza con lingue che non sono
ancora supportate.

Si *potrebbe* inserire a mano il requisito ``_locale`` in tutte le rotte. Ma una
soluzione migliore consiste nell'usare i parametri configurabili del contenitore di
servizi, dentro alla configurazione delle rotte:

.. configuration-block::

    .. code-block:: yaml

        contact:
            path:     /{_locale}/contact
            defaults: { _controller: AcmeDemoBundle:Main:contact }
            requirements:
                _locale: %acme_demo.locales%

    .. code-block:: xml

        <?xml version="1.0" encoding="UTF-8" ?>

        <routes xmlns="http://symfony.com/schema/routing"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://symfony.com/schema/routing http://symfony.com/schema/routing/routing-1.0.xsd">

            <route id="contact" path="/{_locale}/contact">
                <default key="_controller">AcmeDemoBundle:Main:contact</default>
                <requirement key="_locale">%acme_demo.locales%</requirement>
            </route>
        </routes>

    .. code-block:: php

        use Symfony\Component\Routing\RouteCollection;
        use Symfony\Component\Routing\Route;

        $collection = new RouteCollection();
        $collection->add('contact', new Route('/{_locale}/contact', array(
            '_controller' => 'AcmeDemoBundle:Main:contact',
        ), array(
            '_locale' => '%acme_demo.locales%',
        )));

        return $collection;

Ora possiamo controllare e impostare il parametro ``acme_demo.locales`` da qualche
parte nel contenitore:

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        parameters:
            acme_demo.locales: en|es

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <parameters>
            <parameter key="acme_demo.locales">en|es</parameter>
        </parameters>

    .. code-block:: php

        // app/config/config.php
        $container->setParameter('acme_demo.locales', 'en|es');

Si può anche usare un parametro per definire lo schema (o parte dello
schema) della rotta:

.. configuration-block::

    .. code-block:: yaml

        some_route:
            path:     /%acme_demo.route_prefix%/contact
            defaults: { _controller: AcmeDemoBundle:Main:contact }

    .. code-block:: xml

        <?xml version="1.0" encoding="UTF-8" ?>

        <routes xmlns="http://symfony.com/schema/routing"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://symfony.com/schema/routing http://symfony.com/schema/routing/routing-1.0.xsd">

            <route id="some_route" path="/%acme_demo.route_prefix%/contact">
                <default key="_controller">AcmeDemoBundle:Main:contact</default>
            </route>
        </routes>

    .. code-block:: php

        use Symfony\Component\Routing\RouteCollection;
        use Symfony\Component\Routing\Route;

        $collection = new RouteCollection();
        $collection->add('some_route', new Route('/%acme_demo.route_prefix%/contact', array(
            '_controller' => 'AcmeDemoBundle:Main:contact',
        )));

        return $collection;

.. note::

    Proprio come nei normali file di configurazione del contenitore di servizi, se
    occorre un ``%`` in una rotta, si può fare escape del simbolo percentuale raddoppiandolo,
    p.e. ``/score-50%%``, che sarà risolto in ``/score-50%``.

    Tuttavia, poiché il carattere ``%`` incluso in un URL viene automaticamente codificato,
    l'URL risultante in questo esempio sarà ``/score-50%25`` (``%25`` è il
    risultato della codifica del carattere ``%``).
