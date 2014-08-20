Configurare Symfony per funzionare dietro a un load balancer o a un reverse proxy
=================================================================================

Durante il deploy di un'applicazione, ci si potrebbe trovare dietro a un bilanciatore di carico 
(come AWS Elastic Load Balancer) o a un reverse proxy (come Varnish per la
:doc:`cache </book/http_cache>`).

La maggior parte delle volte, non ci sono problemi con Symfony. Se però
una richiesta passa attraverso un proxy, alcune informazioni sulla richiesta sono inviate usando
header speciali ``X-Forwarded-*``. Per esempio, invece di leggere l'header ``REMOTE_ADDR``
(che ora è l'indirizzo IP del reverse proxy), il
vero IP dell'utente sarà memorizzato nell'header ``X-Forwarded-For``.

.. tip::

    Se si usa :ref:`AppCache <symfony-gateway-cache>` di Symfony per la cache,
    allora si sta effettivamente usando un reverse proxy con indirizzo IP ``127.0.0.1``.
    Occorrerà configurare quell'indirizzo come proxy fidato successivamente.

Se non si configura Symfony per cercare questi header, si otterranno informazioni
non corrette sull'indirizzo IP del client, indipendentemente dalla connessione
tramite HTTPS, dalla porta e dal nome host richiesto.

Soluzione: proxy fidati
-----------------------

Questo non è un problam, ma occorre dire a Symfony che sta accadendo
e quale indirizzi IP del reverse proxy faranno questo tipo di cose:

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        # ...
        framework:
            trusted_proxies:  [192.0.0.1, 10.0.0.0/8]

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <?xml version="1.0" encoding="UTF-8" ?>
        <container xmlns="http://symfony.com/schema/dic/services"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xmlns:framework="http://symfony.com/schema/dic/symfony"
            xsi:schemaLocation="http://symfony.com/schema/dic/services http://symfony.com/schema/dic/services/services-1.0.xsd
                                http://symfony.com/schema/dic/symfony http://symfony.com/schema/dic/symfony/symfony-1.0.xsd">

            <framework:config trusted-proxies="192.0.0.1, 10.0.0.0/8">
                <!-- ... -->
            </framework>
        </container>

    .. code-block:: php

        // app/config/config.php
        $container->loadFromExtension('framework', array(
            'trusted_proxies' => array('192.0.0.1', '10.0.0.0/8'),
        ));

In questo esempio, si sta dicendo che i reverse proxy hanno
indirizzo IP ``192.0.0.1`` o corrispondono alla gamma di indirizzi IP con
notazione CIDR ``10.0.0.0/8``. Per ulteriori dettagli, vedere l'opzione
:ref:`framework.trusted_proxies <reference-framework-trusted-proxies>`.

Ecco fatto! Symfony ora cercherà gli header corretti ``X-Forwarded-*`` per
ottenere informazioni, come l'indirizzo IP del client, l'host, la porta e
se la richiesta usi HTTPS.

Che fare se l'IP del reverse proxy cambia continuamente
-------------------------------------------------------

Alcuni reverse proxy (come Elastic Load Balancer di Amazon) non hanno
un indirizzo IP statico né yba gamma di IP da usare con notazione CIDR.
In tal caso, occorre, *con molta cautela*, fidarsi di *tutti* i proxy.

#. Configurare il server  web per *non* rispondere al traffico proveniente da *qualsiasi* client
   che non sia il bilanciatore di carico. Per AWS, lo si può fare con `security groups`_.

#. Una volta garantito che il traffico proverrà solamente da reverse proxy
   fidati, configurare Symfony per fidarsi *sempre* delle richieste in entrata. Lo si può fare
   all'interno del front controller::

       // web/app.php

       // ...
       Request::setTrustedProxies(array($request->server->get('REMOTE_ADDR')));

       $response = $kernel->handle($request);
       // ...

Ecco fatto! È molto importante che si prevenga il traffico da tutte le sorgenti non fidate.
Se si consente traffico dall'esterno, qualcuno potrebbe fare uno spoof del proprio vero indirizzo IP
e di altre informazioni.

Il reverse proxy usa header non standard (non X-Forwarded)
----------------------------------------------------------

La maggior parte dei reverse proxy memorizza informazioni su specifici header ``X-Forwarded-*``.
Ma se il reverse proxy usa nomi di header non standard, li si può
configurare (vedere ":doc:`/components/http_foundation/trusting_proxies`").
Il codice per poterlo fare deve trovarsi nel front controller (p.e. ``web/app.php``).

.. _`security groups`: http://docs.aws.amazon.com/ElasticLoadBalancing/latest/DeveloperGuide/using-elb-security-groups.html
