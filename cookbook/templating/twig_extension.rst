.. index::
   single: Estensioni Twig

Scrivere un'estensione Twig personalizzata
==========================================

Il motivo principale per scrivere un'estensione è lo spostamento di codice usato
frequentemente in una classe riusabile, come per esempio l'aggiunta del supporto per
l'internazionalizzazione. Un'estensione può definire tag, filtri, test, operatori,
variabili globali, funzioni e visitatori di nodi.

La creazione di un'estensione migliora anche la separazione tra codice eseguito al momento
della compilazione e codice eseguito a runtime. Per questo motivo, rende il codice
più veloce.

.. tip::

    Prima di scrivere una nuova estensione, dare un'occhiata al
    `repository ufficiale delle estensioni Twig`_.

Creare la classe Extension
--------------------------

.. note::

    Questa ricetta descrive come scrivere un'estensione personalizzata di Twig
    da Twig 1.12. Se si usa una versione precedente, si legga la
    `vecchia documentazione delle estensioni di Twig`_.

Per avere una funzionalità personalizzata, occorre prima di tutto creare una classe Extension.
Come esempio, creeremo un filtro per i prezzi, che formatta un dato numero::

    // src/AppBundle/Twig/AppExtension.php
    namespace AppBundle\Twig;

    class AppExtension extends \Twig_Extension
    {
        public function getFilters()
        {
            return array(
                new \Twig_SimpleFilter('price', array($this, 'priceFilter')),
            );
        }

        public function priceFilter($number, $decimals = 0, $decPoint = '.', $thousandsSep = ',')
        {
            $price = number_format($number, $decimals, $decPoint, $thousandsSep);
            $price = '$'.$price;

            return $price;
        }

        public function getName()
        {
            return 'app_extension';
        }
    }

.. tip::

    Oltre ai filtri, si possono aggiungere `funzioni`_ personalizzate e registrare
    `variabili globali`_.

Registrare un'estensione come servizio
--------------------------------------

Ora si deve rendere nota al contenitore di servizi l'estensione di Twig appena creata:

.. configuration-block::

    .. code-block:: yaml

        # app/config/services.yml
        services:
            app.twig_extension:
                class: AppBundle\Twig\AppExtension
                public: false
                tags:
                    - { name: twig.extension }

    .. code-block:: xml

        <!-- app/config/services.xml -->
        <services>
            <service id="app.twig_extension"
                class="AppBundle\Twig\AppExtension"
                public="false">
                <tag name="twig.extension" />
            </service>
        </services>

    .. code-block:: php

        // app/config/services.php
        use Symfony\Component\DependencyInjection\Definition;

        $container
            ->register('app.twig_extension', '\AppBundle\Twig\AppExtension')
            ->setPublic(false)
            ->addTag('twig.extension');

.. note::

   Tenere a mente che le estensioni di Twig non sono caricate in modalità pigra. Questo
   vuol dire che c'è una buona possibilità di avere una
   :class:`Symfony\\Component\\DependencyInjection\\Exception\\ServiceCircularReferenceException`
   o una
   :class:`Symfony\\Component\\DependencyInjection\\Exception\\ScopeWideningInjectionException`
   se un servizio (o un'estensione Twig, in questo caso) dipendono dal servizio della
   richiesta. Per maggiori informazioni, si veda :doc:`/cookbook/service_container/scopes`.

Usare l'estensione personalizzata                
---------------------------------

L'estensione di Twig appena creata può essere usata in modo non diverso da qualsiasi altra:

.. code-block:: jinja

    {# mostra $5,500.00 #}
    {{ '5500'|price }}

Si possono passare parametri al filtro:

.. code-block:: jinja

    {# mostra $5500,2516 #}
    {{ '5500.25155'|price(4, ',', '') }}

Saperne di più
--------------

Per approfondire le estensioni di Twig, si può vedere la
`documentazione sulle estensioni di Twig`_.

.. _`repository ufficiale delle estensioni Twig`: http://github.com/fabpot/Twig-extensions
.. _`documentazione sulle estensioni di Twig`: http://twig.sensiolabs.org/doc/advanced.html#creating-an-extension
.. _`variabili globali`: http://twig.sensiolabs.org/doc/advanced.html#id1
.. _`funzioni`: http://twig.sensiolabs.org/doc/advanced.html#id2
.. _`vecchia documentazione delle estensioni di Twig`: http://twig.sensiolabs.org/doc/advanced_legacy.html#creating-an-extension
