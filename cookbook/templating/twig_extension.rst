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

    Prima di scrivere la propria estensione, dare un'occhiata al
    `repository ufficiale delle estensioni Twig`_.
    
Creare la classe Extension
--------------------------    

.. note::

    Questa ricetta descrive come scrivere un'estensione personalizzata di Twig
    da Twig 1.12. Se si usa una versione precedente, si legga la
    `vecchia documentazione delle estensioni di Twig`_.

Per avere la propria funzionalità personalizzata, occorre prima di tutto creare una classe
Extension. Come esempio, creeremo un filtro per i prezzi, che formatta un dato numero::

    // src/Acme/DemoBundle/Twig/AcmeExtension.php
    namespace Acme\DemoBundle\Twig;

    class AcmeExtension extends \Twig_Extension
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
            $price = '$' . $price;

            return $price;
        }

        public function getName()
        {
            return 'acme_extension';
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

        # src/Acme/DemoBundle/Resources/config/services.yml
        services:
            acme.twig.acme_extension:
                class: Acme\DemoBundle\Twig\AcmeExtension
                tags:
                    - { name: twig.extension }

    .. code-block:: xml
        
        <!-- src/Acme/DemoBundle/Resources/config/services.xml -->
        <services>
            <service id="acme.twig.acme_extension" class="Acme\DemoBundle\Twig\AcmeExtension">
                <tag name="twig.extension" />
            </service>
        </services>

    .. code-block:: php

        // src/Acme/DemoBundle/Resources/config/services.php
        use Symfony\Component\DependencyInjection\Definition;

        $container
            ->register('acme.twig.acme_extension', '\Acme\DemoBundle\Twig\AcmeExtension')
            ->addTag('twig.extension');
         
.. note::

   Tenere a mente che le estensioni di Twig non sono caricate in modalità pigra. Questo
   vuol dire che c'è una buona possibilità di avere una **CircularReferenceException**
   o una **ScopeWideningInjectionException**, se un servizio
   (o un'estensione Twig, in questo caso) dipendono dal servisio della richiesta.
   Per maggiori informazioni, si veda :doc:`/cookbook/service_container/scopes`.

Usare l'estensione personalizzata                
---------------------------------

L'estensione di Twig appena creata si può usare in modo non diverso da qualsiasi altra:

.. code-block:: jinja

    {# mostra $5,500.00 #}
    {{ '5500' | price }}

Si possono passare parametri al filtro:

.. code-block:: jinja
    
    {# mostra $5500,2516 #}
    {{ '5500.25155' | price(4, ',', '') }}

Saperne di più
--------------

Per approfondire le estensioni di Twig, si può vedere la
`documentazione sulle estensioni di Twig`_.
     
.. _`repository ufficiale delle estensioni Twig`: http://github.com/fabpot/Twig-extensions
.. _`documentazione sulle estensioni di Twig`: http://twig.sensiolabs.org/doc/advanced.html#creating-an-extension
.. _`variabili globali`: http://twig.sensiolabs.org/doc/advanced.html#id1
.. _`funzioni`: http://twig.sensiolabs.org/doc/advanced.html#id2
.. _`vecchia documentazione delle estensioni di Twig`: http://twig.sensiolabs.org/doc/advanced_legacy.html#creating-an-extension
