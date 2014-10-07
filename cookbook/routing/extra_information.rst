.. index::
   single: Rotte; Informazioni aggiuntive

Passare informazioni aggiuntive da una rotta a un controllore
=============================================================

I parametri all'interno di ``defaults`` non devono necessariamente corrispondere
a un segnaposto nel ``path`` della rotta. In effetti, si può usare
``defaults`` per specificare parametri aggiuntivi, che saranno accessibili come
parametri del controllore:

.. configuration-block::

    .. code-block:: yaml

        # app/config/routing.yml
        blog:
            path:      /blog/{page}
            defaults:
                _controller: AcmeBlogBundle:Blog:index
                page:        1
                title:       "Ciao mondo!"

    .. code-block:: xml

        <!-- app/config/routing.xml -->
        <?xml version="1.0" encoding="UTF-8" ?>
        <routes xmlns="http://symfony.com/schema/routing"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://symfony.com/schema/routing
                http://symfony.com/schema/routing/routing-1.0.xsd">

            <route id="blog" path="/blog/{page}">
                <default key="_controller">AcmeBlogBundle:Blog:index</default>
                <default key="page">1</default>
                <default key="title">Ciao mondo!</default>
            </route>
        </routes>

    .. code-block:: php

        // app/config/routing.php
        use Symfony\Component\Routing\RouteCollection;
        use Symfony\Component\Routing\Route;

        $collection = new RouteCollection();
        $collection->add('blog', new Route('/blog/{page}', array(
            '_controller' => 'AcmeBlogBundle:Blog:index',
            'page'        => 1,
            'title'       => 'Ciao mondo!',
        )));

        return $collection;

Si può quindi accedere a tali parametri aggiuntivi in un controllore::

    public function indexAction($page, $title)
    {
        // ...
    }

Come si può vedere, la variabile ``$title`` non è mai stata definita nel percorso della rotta,
ma vi si può comunque accedere dall'interno del controllore.
