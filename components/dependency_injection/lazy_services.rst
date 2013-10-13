.. index::
   single: Dependency Injection; Servizi pigri

Servizi pigri
=============

.. versionadded:: 2.3
   I servizi pigri sono stati aggiunti in Symfony 2.3.

Perché i servizi pigri?
-----------------------

A volte può essere necessario iniettare, all'interno del proprio oggetto, un servizio 
un po' pesante da istanziare e che non sempre viene utilizzato . Si supponga, ad esempio,
di avere un ``GestoreDiNewsletter`` e che si voglia iniettare un servizio ``mailer`` al suo interno. Solo
alcuni metodi del ``GestoreDiNewsletter`` usano effettivamente il ``mailer`` ma,
anche senza farne uso, il servizio ``mailer`` verrebbe comunque istanziato
in modo da poter costruire il ``GestoreDiNewsletter``.

Per risolvere questo problema è possibile usare un servizio pigro. Quando si usa un servizio pigro, 
in realtà viene iniettato un "proxy" del servizio ``mailer``. Il proxy sembra e si comporta esattamente
come se fosse il ``mailer``, a parte il fatto che ``mailer`` non viene istanziato finché
non si interagisce in qualche modo con il suo proxy.

Installazione
-------------

Per poter istanziare i servizi pigri è prima necessario installare
il `bridge ProxyManager`_:

.. code-block:: bash

    $ php composer.phar require symfony/proxy-manager-bridge:2.3.*

.. note::

    Se si usa il framework completo, questo pacchetto è già incluso,
    ma il vero gestore di proxy deve essere incluso. Aggiungere quindi

    .. code-block:: json

        "require": {
            "ocramius/proxy-manager": "0.4.*"
        }

    nel file ``composer.json``. Compilare quindi il contenitore e verificare
    di avere un proxy per i servizi pigri.

Configurazione
--------------

Si può definire un servizio come pigro, modificandone la definizione:

.. configuration-block::

    .. code-block:: yaml

        services:
           pippo:
             class: Acme\Pippo
             lazy: true

    .. code-block:: xml

        <service id="pippo" class="Acme\Pippo" lazy="true" />

    .. code-block:: php

        $definizione = new Definition('Acme\Pippo');
        $definizione->setLazy(true);
        $container->setDefinition('pippo', $definizione);

Ora è possibile richiedere il servizio dal contenitore::

    $servizio = $container->get('pippo');

A questo punto il ``$servizio`` recuperato dovrebbe essere un `proxy`_ virtuale con
la stessa firma della classe che rappresenta il servizio. È anche possibile iniettare
il servizio così come si farebbe con qualsiasi altro servizio. L'oggetto che verrà effettivamente
iniettato sarà il proxy.

Per verificare che il proxy funzioni, si può semplicemente verificare l'interfaccia
dell'oggetto ricevuto.

.. code-block:: php

    var_dump(class_implements($service));

Se la classe implementa "ProxyManager\Proxy\LazyLoadingInterface", i servizi
pigri stanno funzionando.

.. note::

    Se non si è installato il `bridge ProxyManager`_, il contenitore si limiterà
    a saltare il parametro ``lazy`` e a istanziare il servizio come
    farebbe normalmente.

Il proxy viene inizializzato e il servizio vero e proprio viene istanziato non appena
si dovesse interagire con l'oggetto.

Risorse aggiuntive
------------------

È possibile approfondire le modalità con cui i sostituti vengono istanziati, generati e inizializzati
nella `documentazione sul ProxyManager`_.


.. _`bridge ProxyManager`: https://github.com/symfony/symfony/tree/master/src/Symfony/Bridge/ProxyManager
.. _`proxy`: http://it.wikipedia.org/wiki/Proxy_pattern
.. _`documentazione sul ProxyManager`: https://github.com/Ocramius/ProxyManager/blob/master/docs/lazy-loading-value-holder.md
