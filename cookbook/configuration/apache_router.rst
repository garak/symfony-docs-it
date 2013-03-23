.. index::
   single: Rotte su Apache

Come usare le rotte su Apache
=============================

Symfony2, pur essendo veloce di suo, fornisce anche molti modi per incrementare la sua velocità,
tramite piccole modifiche. Una di queste è delegare la gestione delle rotte ad Apache, invece di usare Symfony2.

Modificare i parametri di configurazione delle rotte
----------------------------------------------------

Per esportare in Apache le rotte, occorre prima sistemare alcuni parametri di configurazione,
per dire a Symfony2 di usare ``ApacheUrlMatcher`` invece di quello predefinito:

.. configuration-block::

    .. code-block:: yaml

        # app/config/config_prod.yml
        parameters:
            router.options.matcher.cache_class: ~ # disabilita la cache delle rotte
            router.options.matcher_class: Symfony\Component\Routing\Matcher\ApacheUrlMatcher

    .. code-block:: xml

        <!-- app/config/config_prod.xml -->
        <parameters>
            <parameter key="router.options.matcher.cache_class">null</parameter> <!-- disabilita la cache delle rotte -->
            <parameter key="router.options.matcher_class">
                Symfony\Component\Routing\Matcher\ApacheUrlMatcher
            </parameter>
        </parameters>

    .. code-block:: php

        // app/config/config_prod.php
        $container->setParameter('router.options.matcher.cache_class', null); // disabilita la cache delle rotte
        $container->setParameter(
            'router.options.matcher_class',
            'Symfony\Component\Routing\Matcher\ApacheUrlMatcher'
        );

.. tip::

    Si noti che :class:`Symfony\\Component\\Routing\\Matcher\\ApacheUrlMatcher`
    estende :class:`Symfony\\Component\\Routing\\Matcher\\UrlMatcher`, quindi anche se non
    si rigenerano le regole di url_rewrite, funziona tutto ugualmente (perché viene
    fatta una chiamata a ``parent::match()`` alla fine di
    ``ApacheUrlMatcher::match()``). 

Generare le regole per mod_rewrite
----------------------------------

Per testare che tutto funzioni, creiamo una rotta molto semplice per il bundle demo:

.. configuration-block::

    .. code-block:: yaml

        # app/config/routing.yml
        hello:
            pattern:  /hello/{name}
            defaults: { _controller: AcmeDemoBundle:Demo:hello }

    .. code-block:: xml

        <!-- app/config/routing.xml -->
        <route id="hello" pattern="/hello/{name}">
            <default key="_controller">AcmeDemoBundle:Demo:hello</default>
        </route>

    .. code-block:: php

        // app/config/routing.php
        $collection->add('hello', new Route('/hello/{name}', array(
            '_controller' => 'AcmeDemoBundle:Demo:hello',
        )));

Ora generiamo le regole di **url_rewrite**:
    
.. code-block:: bash

    $ php app/console router:dump-apache -e=prod --no-debug

Il risultato dovrebbe essere simile a questo:

.. code-block:: apache

    # salta le richieste "reali"
    RewriteCond %{REQUEST_FILENAME} -f
    RewriteRule .* - [QSA,L]

    # hello
    RewriteCond %{REQUEST_URI} ^/hello/([^/]+?)$
    RewriteRule .* app.php [QSA,L,E=_ROUTING__route:hello,E=_ROUTING_name:%1,E=_ROUTING__controller:AcmeDemoBundle\:Demo\:hello]

Ora possiamo riscrivere `web/.htaccess` per usare le nuove regole, quindi con il nostro
esempio dovrebbe risultare in questo modo:

.. code-block:: apache

    <IfModule mod_rewrite.c>
        RewriteEngine On

        # salta le richieste "reali"
        RewriteCond %{REQUEST_FILENAME} -f
        RewriteRule .* - [QSA,L]

        # hello
        RewriteCond %{REQUEST_URI} ^/hello/([^/]+?)$
        RewriteRule .* app.php [QSA,L,E=_ROUTING__route:hello,E=_ROUTING_name:%1,E=_ROUTING__controller:AcmeDemoBundle\:Demo\:hello]
    </IfModule>

.. note::

   La procedura appena vista andrebbe fatta ogni volta che si aggiunge o cambia una rotta

Ecco fatto!
Ora è tutto pronto per usare le rotte di Apache.

Modifiche aggiuntive
--------------------

Per risparmiare un po' di tempo di processore, sostituire tutte le ``Request``
con ``ApacheRequest`` in ``web/app.php``::

    // web/app.php
    
    require_once __DIR__.'/../app/bootstrap.php.cache';
    require_once __DIR__.'/../app/AppKernel.php';
    //require_once __DIR__.'/../app/AppCache.php';

    use Symfony\Component\HttpFoundation\ApacheRequest;

    $kernel = new AppKernel('prod', false);
    $kernel->loadClassCache();
    //$kernel = new AppCache($kernel);
    $kernel->handle(ApacheRequest::createFromGlobals())->send();
