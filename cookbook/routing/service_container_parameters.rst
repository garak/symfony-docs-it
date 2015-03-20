.. index::
   single: Rotte; Parametri del contenitore di servizi

Usare i parametri del contenitore di servizi nelle rotte
========================================================

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

        # app/config/routing.yml
        contact:
            path:     /{_locale}/contact
            defaults: { _controller: AppBundle:Main:contact }
            requirements:
                _locale: "%app.locales%"

    .. code-block:: xml

        <!-- app/config/routing.xml -->
        <?xml version="1.0" encoding="UTF-8" ?>
        <routes xmlns="http://symfony.com/schema/routing"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://symfony.com/schema/routing http://symfony.com/schema/routing/routing-1.0.xsd">

            <route id="contact" path="/{_locale}/contact">
                <default key="_controller">AppBundle:Main:contact</default>
                <requirement key="_locale">%app.locales%</requirement>
            </route>
        </routes>

    .. code-block:: php

        // app/config/routing.php
        use Symfony\Component\Routing\RouteCollection;
        use Symfony\Component\Routing\Route;

        $collection = new RouteCollection();
        $collection->add('contact', new Route('/{_locale}/contact', array(
            '_controller' => 'AppBundle:Main:contact',
        ), array(
            '_locale' => '%app.locales%',
        )));

        return $collection;

Ora possiamo controllare e impostare il parametro ``acme_demo.locales`` da qualche
parte nel contenitore:

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        parameters:
            app.locales: en|es

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <parameters>
            <parameter key="app.locales">en|es</parameter>
        </parameters>

    .. code-block:: php

        // app/config/config.php
        $container->setParameter('app.locales', 'en|es');

Si può anche usare un parametro per definire lo schema (o parte dello
schema) della rotta:

.. configuration-block::

    .. code-block:: yaml

        # app/config/routing.yml
        some_route:
            path:     /%app.route_prefix%/contact
            defaults: { _controller: AppBundle:Main:contact }

    .. code-block:: xml

        <!-- app/config/routing.xml -->
        <?xml version="1.0" encoding="UTF-8" ?>
        <routes xmlns="http://symfony.com/schema/routing"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://symfony.com/schema/routing http://symfony.com/schema/routing/routing-1.0.xsd">

            <route id="some_route" path="/%app.route_prefix%/contact">
                <default key="_controller">AppBundle:Main:contact</default>
            </route>
        </routes>

    .. code-block:: php

        // app/config/routing.php
        use Symfony\Component\Routing\RouteCollection;
        use Symfony\Component\Routing\Route;

        $collection = new RouteCollection();
        $collection->add('some_route', new Route('/%app.route_prefix%/contact', array(
            '_controller' => 'AppBundle:Main:contact',
        )));

        return $collection;

.. note::

    Proprio come nei normali file di configurazione del contenitore di servizi, se
    occorre un ``%`` in una rotta, si può fare escape del simbolo percentuale raddoppiandolo,
    p.e. ``/score-50%%``, che sarà risolto in ``/score-50%``.

    Tuttavia, poiché i caratteri ``%`` inclusi un un URL sono automaticamente codificati,
    l'URL risultante da questo esempio sarebbe ``/score-50%25`` (``%25`` è il
    risultati della codifica del carattere ``%``).

.. seealso::

    Per gestire parametri in una classe della Dependency Injection, vedere
    :doc:`/cookbook/configuration/using_parameters_in_dic`.
