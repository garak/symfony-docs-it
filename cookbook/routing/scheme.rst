.. index::
   single: Rotte; Requisiti dello schema

Come forzare le rotte per utilizzare sempre HTTPS
=================================================

A volte, si desidera proteggere alcune rotte ed essere sicuri che siano sempre
accessibili solo tramite il protocollo HTTPS. Il componente Routing consente di forzare
lo schema HTTP attraverso il requisito ``_scheme``:

.. configuration-block::

    .. code-block:: yaml

        secure:
            pattern:  /secure
            defaults: { _controller: AcmeDemoBundle:Main:secure }
            requirements:
                _scheme:  https

    .. code-block:: xml

        <?xml version="1.0" encoding="UTF-8" ?>

        <routes xmlns="http://symfony.com/schema/routing"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://symfony.com/schema/routing http://symfony.com/schema/routing/routing-1.0.xsd">

            <route id="secure" pattern="/secure">
                <default key="_controller">AcmeDemoBundle:Main:secure</default>
                <requirement key="_scheme">https</requirement>
            </route>
        </routes>

    .. code-block:: php

        use Symfony\Component\Routing\RouteCollection;
        use Symfony\Component\Routing\Route;

        $collection = new RouteCollection();
        $collection->add('secure', new Route('/secure', array(
            '_controller' => 'AcmeDemoBundle:Main:secure',
        ), array(
            '_scheme' => 'https',
        )));

        return $collection;

La configurazione sopra forza la rotta ``secure`` a utilizzare sempre HTTPS.

Quando si genera l'URL ``secure`` e se lo schema corrente è HTTP, Symfony
genererà automaticamente un URL assoluto con HTTPS come schema:

.. code-block:: text

    # Se lo schema corrente è HTTPS
    {{ path('secure') }}
    # generates /secure

    # Se lo schema corrente è HTTP
    {{ path('secure') }}
    # generates https://example.com/secure

L'esigenza è anche quella di forzare le richieste in arrivo. Se si tenta di accedere
al percorso ``/secure`` con HTTP, si verrà automaticamente rinviati allo
stesso URL, ma con lo schema HTTPS.

L'esempio precedente utilizza  ``https`` per ``_scheme``, ma si può anche forzare un
URL per usare sempre ``http``.

.. note::

    La componente di sicurezza fornisce un altro modo per forzare lo schema HTTP, tramite
    l'impostazione ``requires_channel``. Questo metodo alternativo è più adatto
    per proteggere un'"area" del sito web (tutti gli URL sotto ``/admin``) o quando
    si vuole proteggere URL definiti in un bundle di terze parti.
