.. index::
   single: Rotte; metodi

Usare i metodi HTTP oltre a GET e POST nelle rotte
==================================================

Il metodo HTTP di una richiesta è uno dei requisiti verificabili per la
corrispondenza a una rotta. L'argomento è trattato nella capitolo sulle rotte
del libro, ":doc:`/book/routing`", con esempi che usano GET e POST. Ma si possono
usare anche altri metodi HTTP in questo modo. Per esempio, se si ha una rotta relativa
a un post di un blog, si può usare lo stesso schema di URL per mostrarlo, aggiornarlo
e cancellarlo, cercando corrispondenza con GET, PUT e DELETE.

.. configuration-block::

    .. code-block:: yaml

        blog_show:
            path:     /blog/{slug}
            defaults: { _controller: AcmeDemoBundle:Blog:show }
            methods:   [GET]

        blog_update:
            path:     /blog/{slug}
            defaults: { _controller: AcmeDemoBundle:Blog:update }
            methods:   [PUT]

        blog_delete:
            path:     /blog/{slug}
            defaults: { _controller: AcmeDemoBundle:Blog:delete }
            methods:   [DELETE]

    .. code-block:: xml

        <?xml version="1.0" encoding="UTF-8" ?>

        <routes xmlns="http://symfony.com/schema/routing"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://symfony.com/schema/routing http://symfony.com/schema/routing/routing-1.0.xsd">

            <route id="blog_show" path="/blog/{slug}" methods="GET">
                <default key="_controller">AcmeDemoBundle:Blog:show</default>
            </route>

            <route id="blog_update" path="/blog/{slug}" methods="PUT">
                <default key="_controller">AcmeDemoBundle:Blog:update</default>
            </route>

            <route id="blog_delete" path="/blog/{slug}" methods="DELETE">
                <default key="_controller">AcmeDemoBundle:Blog:delete</default>
            </route>
        </routes>

    .. code-block:: php

        use Symfony\Component\Routing\RouteCollection;
        use Symfony\Component\Routing\Route;

        $collection = new RouteCollection();
        $collection->add('blog_show', new Route('/blog/{slug}', array(
            '_controller' => 'AcmeDemoBundle:Blog:show',
        ), array(), array(), '', array(), array('GET')));

        $collection->add('blog_update', new Route('/blog/{slug}', array(
            '_controller' => 'AcmeDemoBundle:Blog:update',
        ), array(), array(), '', array(), array('PUT')));

        $collection->add('blog_delete', new Route('/blog/{slug}', array(
            '_controller' => 'AcmeDemoBundle:Blog:delete',
        ), array(), array(), '', array('DELETE')));

        return $collection;

Finti metodi con _method
------------------------

.. note::

    La funzionalità ``_method`` mostrata qui è disabilitata in Symfony 2.2
    e abilitata in Symfony 2.3. Per abilitarla in Symfony 2.2, occorre
    richiamare il metodo :method:`Request::enableHttpMethodParameterOverride <Symfony\\Component\\HttpFoundation\\Request::enableHttpMethodParameterOverride>` 
    prima di gestire la richiesta (p.e. nel front controller). In Symfony
    2.3, usare l'opzione :ref:`configuration-framework-http_method_override`.

Sfortunatamente, la vita non è così facile, poiché molti browser non supportano
l'invio di richieste PUT e DELETE. Per fortuna, Symfony2 fornisce un semplice modo
per aggirare tale limitazione. Includendo un parametro ``_method``
nella query string o nei parametri di una richiesta HTTP, Symfony2 lo userà
come metodo nella corrispondenza delle rotte. I form includono automaticamente un
campo nascosto per tale parametro, se il metodo di invio non è GET o POST.
Vedere :ref:`il capitolo relativo nella documentazione dei form<book-forms-changing-action-and-method>`
per maggiori informazioni.
