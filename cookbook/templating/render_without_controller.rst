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
            pattern: /privacy
            defaults:
                _controller: FrameworkBundle:Template:template
                template: 'AcmeBundle:Static:privacy.html.twig'

    .. code-block:: xml

        <?xml version="1.0" encoding="UTF-8" ?>

        <routes xmlns="http://symfony.com/schema/routing"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://symfony.com/schema/routing http://symfony.com/schema/routing/routing-1.0.xsd">

            <route id="acme_privacy" pattern="/privacy">
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

        {% render url('acme_privacy') %}

    .. code-block:: html+php

        <?php echo $view['actions']->render(
            $view['router']->generate('acme_privacy', array(), true)
        ) ?>
