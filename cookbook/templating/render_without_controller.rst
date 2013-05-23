.. index::
   single: Template; Rendere un template senza controllore

Rendere un template senza un controllore
========================================

Di solito, quando occorre creare una pagina, si deve creare un controllore
e al suo interno rendere un template . Ma, se si sta rendendo un
semplice template, che non ha bisogno di ricevere dati, si può evitare del
tutto di creare un controllore, usando il controllore predefinito
``FrameworkBundle:Template:template``.

Per esempio, si supponga di voler rendere un template ``AcmeBundle:Static:privacy.html.twig``,
che non richiede il passaggio di alcuna variabile. Lo si può fare
senza creare un controllore:

.. configuration-block::

    .. code-block:: yaml

        acme_privacy:
            path: /privacy
            defaults:
                _controller: FrameworkBundle:Template:template
                template: 'AcmeBundle:Static:privacy.html.twig'

    .. code-block:: xml

        <?xml version="1.0" encoding="UTF-8" ?>

        <routes xmlns="http://symfony.com/schema/routing"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://symfony.com/schema/routing http://symfony.com/schema/routing/routing-1.0.xsd">

            <route id="acme_privacy" path="/privacy">
                <default key="_controller">FrameworkBundle:Template:template</default>
                <default key="template">AcmeBundle:Static:privacy.html.twig</default>
            </route>
        </routes>

    .. code-block:: php

        use Symfony\Component\Routing\RouteCollection;
        use Symfony\Component\Routing\Route;

        $collection = new RouteCollection();
        $collection->add('acme_privacy', new Route('/privacy', array(
            '_controller'  => 'FrameworkBundle:Template:template',
            'template'     => 'AcmeBundle:Static:privacy.html.twig',
        )));

        return $collection;

Il controllore ``FrameworkBundle:Template:template`` renderà semplicemente
qualsiasi template passato nel valore di ``template``.

Si può usare questo trucco anche quando si rendono controllori inclusi in
un template. Ma poiché lo scopo di rendere un controllore da dentro un
template è solitamente quello di preparare dei dati in un controllore,
probabilmente non sarà molto utile, tranne per mettere facilmente in cache dei
frammenti statici, una caratteristica che sarà disponibile in Symfony 2.2.

.. configuration-block::

    .. code-block:: html+jinja

        {{ render(url('acme_privacy')) }}

    .. code-block:: html+php

        <?php echo $view['actions']->render(
            $view['router']->generate('acme_privacy', array(), true)
        ) ?>

.. _cookbook-templating-no-controller-caching:

Template statici in cache
-------------------------

.. versionadded:: 2.2
    La possibilità di mettere in cache i template resi tramite ``FrameworkBundle:Template:template``
    è nuova in Symfony 2.2.

Poiché i template resi in questo modo sono tipicamente statici, può aver
senso metterli in cache. Per fortuna, questo è facile! Configurando alcune
variabili in più nella rotta, si può controllare esattamente il modo in cui la pagina è messa in cache:

.. configuration-block::

    .. code-block:: yaml

        acme_privacy:
            path: /privacy
            defaults:
                _controller: FrameworkBundle:Template:template
                template: 'AcmeBundle:Static:privacy.html.twig'
                maxAge: 86400
                sharedMaxAge: 86400

    .. code-block:: xml

        <?xml version="1.0" encoding="UTF-8" ?>

        <routes xmlns="http://symfony.com/schema/routing"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://symfony.com/schema/routing http://symfony.com/schema/routing/routing-1.0.xsd">

            <route id="acme_privacy" path="/privacy">
                <default key="_controller">FrameworkBundle:Template:template</default>
                <default key="template">AcmeBundle:Static:privacy.html.twig</default>
                <default key="maxAge">86400</default>
                <default key="sharedMaxAge">86400</default>
            </route>
        </routes>

    .. code-block:: php

        use Symfony\Component\Routing\RouteCollection;
        use Symfony\Component\Routing\Route;

        $collection = new RouteCollection();
        $collection->add('acme_privacy', new Route('/privacy', array(
            '_controller'  => 'FrameworkBundle:Template:template',
            'template'     => 'AcmeBundle:Static:privacy.html.twig',
            'maxAge'       => 86400,
            'sharedMaxAge' => 86400,
        )));

        return $collection;

I valori ``maxAge`` e ``sharedMaxAge`` sono usati per modificare l'oggetto della risposta
creato dal controllore. Per maggiori informazioni sulla cache, vedere
:doc:`/book/http_cache`.

C'è anche una variabile ``private`` (non mostrata qui). Per impostazione preferinita, la risposta
sarà pubblica, purché vengano passati ``maxAge`` o ``sharedMaxAge``.
Se impostata a ``true``, la risposta sarà privata.