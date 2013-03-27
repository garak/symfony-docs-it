.. index::
   single: Rotte; _method

Usare i metodi HTTP oltre a GET e POST nelle rotte
==================================================

.. versionadded:: 2.2
    Questa funzionalità è disabilitata in Symfony 2.2. Per abilitarla, 
    richiamare il metodo :method:`Request::enableHttpMethodParameterOverride <Symfony\\Component\\HttpFoundation\\Request::enableHttpMethodParameterOverride>` 
    prima di gestire la richiesta.

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

Sfortunatamente, la vita non è così facile, poiché molti browser non supportano
l'invio di richieste PUT e DELETE. Per fortuna, Symfony2 fornisce un semplice modo
per aggirare tale limitazione. Includendo un parametro ``_method``
nella query string o nei parametri di una richiesta HTTP, Symfony2 lo userà
come metodo nella corrispondenza delle rotte. Lo si può fare facilmente nei form,
usando un campo nascosto. Si supponga di avere un form per modificare il post di un blog:

.. code-block:: html+jinja

    <form action="{{ path('blog_update', {'slug': blog.slug}) }}" method="post">
        <input type="hidden" name="_method" value="PUT" />
        {{ form_widget(form) }}
        <input type="submit" value="Update" />
    </form>

La richiesta inviata ora corrisponderà alla rotta ``blog_update`` e quindi l'azione
``updateAction`` processerà il form.

In modo simile, il form di cancellazione può essere modificato come segue:

.. code-block:: html+jinja

    <form action="{{ path('blog_delete', {'slug': blog.slug}) }}" method="post">
        <input type="hidden" name="_method" value="DELETE" />
        {{ form_widget(delete_form) }}
        <input type="submit" value="Delete" />
    </form>

Corrisponderà quindi alla rotta ``blog_delete``.
