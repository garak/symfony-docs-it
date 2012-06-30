.. index::
   pair: Twig; Riferimento configurazione

Riferimento configurazione TwigBundle
=====================================

.. configuration-block::

    .. code-block:: yaml

        twig:
            exception_controller:  Symfony\Bundle\TwigBundle\Controller\ExceptionController::showAction
            form:
                resources:

                    # Default:
                    - form_div_layout.html.twig

                    # Esempio:
                    - MioBundle::form.html.twig
            globals:

                # Esempi:
                foo:                 "@bar"
                pi:                  3.14

                # Esempi di opzioni, ma l'uso più facile è quello visto sopra
                some_variable_name:
                    # id di un servizio
                    id:                   ~
                    # impostare con il servizio o lasciare vuoto
                    type:                 ~
                    value:                ~
            autoescape:           ~
            base_template_class:  ~ # Esempio: Twig_Template
            cache:                "%kernel.cache_dir%/twig"
            charset:              "%kernel.charset%"
            debug:                "%kernel.debug%"
            strict_variables:     ~
            auto_reload:          ~
            optimizations:        ~

    .. code-block:: xml

        <container xmlns="http://symfony.com/schema/dic/services"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xmlns:twig="http://symfony.com/schema/dic/twig"
            xsi:schemaLocation="http://symfony.com/schema/dic/services http://symfony.com/schema/dic/services/services-1.0.xsd
                                http://symfony.com/schema/dic/twig http://symfony.com/schema/dic/doctrine/twig-1.0.xsd">

            <twig:config auto-reload="%kernel.debug%" autoescape="true" base-template-class="Twig_Template" cache="%kernel.cache_dir%/twig" charset="%kernel.charset%" debug="%kernel.debug%" strict-variables="false">
                <twig:form>
                    <twig:resource>MioBundle::form.html.twig</twig:resource>
                </twig:form>
                <twig:global key="foo" id="bar" type="service" />
                <twig:global key="pi">3.14</twig:global>
            </twig:config>
        </container>

    .. code-block:: php

        $container->loadFromExtension('twig', array(
            'form' => array(
                'resources' => array(
                    'MioBundle::form.html.twig',
                )
             ),
             'globals' => array(
                 'foo' => '@bar',
                 'pi'  => 3.14,
             ),
             'auto_reload'         => '%kernel.debug%',
             'autoescape'          => true,
             'base_template_class' => 'Twig_Template',
             'cache'               => '%kernel.cache_dir%/twig',
             'charset'             => '%kernel.charset%',
             'debug'               => '%kernel.debug%',
             'strict_variables'    => false,
        ));

Configurazione
--------------

.. _config-twig-exception-controller:

exception_controller
....................

**tipo**: ``stringa`` **predefinito**: ``Symfony\\Bundle\\TwigBundle\\Controller\\ExceptionController::showAction``

Questo è il controllore che viene attivato dopo il lancio di un'eccezione nella
propria applicaizone. Il controllore predefinito
(:class:`Symfony\\Bundle\\TwigBundle\\Controller\\ExceptionController`)
è quello responsabile di rendere template specifici sotto differenti condizioni
di errore (vedere :doc:`/cookbook/controller/error_pages`). La modifica di
questa opzione è avanzata. Se occorre personalizzare una pagina di errore, si dovrebbe
usare il collegamento precedente. Se occorre eseguire qualche azioni su un'eccezione,
si dovrebbe aggiungere un ascoltatore all'evento ``kernel.exception``  (vedere :ref:`dic-tags-kernel-event-listener`).
