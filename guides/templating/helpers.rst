Helper
======

.. _templating_renderer_tag:

Abilitare Template Renderer Personalizzati
------------------------------------------

Per abilitare un template renderer personlizzato aggiungerlo come un qualsiasi
altro servizio in uno dei file di configurazione, etichettandolo come 
``templating.renderer`` e definendo un attributo ``alias`` 
(il renderer sarà conosciuto tramite il suo alias):

.. configuration-block::

    .. code-block:: yaml

        services:
            templating.renderer.your_renderer_name:
                class: Fully\Qualified\Renderer\Class\Name
                tags:
                    - { name: templating.renderer, alias: alias_name }

    .. code-block:: xml

        <service id="templating.renderer.your_renderer_name" class="Fully\Qualified\Renderer\Class\Name">
            <tag name="templating.renderer" alias="alias_name" />
        </service>

    .. code-block:: php

        $container
            ->register('templating.renderer.your_renderer_name', 'Fully\Qualified\Renderer\Class\Name')
            ->addTag('templating.renderer', array('alias' => 'alias_name'))
        ;

.. _templating_helper_tag:

Abilitare Template Helper Personalizzati
----------------------------------------

Per abilitare un template helper personlizzato aggiungerlo come un qualsiasi
altro servizio in uno dei file di configurazione, etichettandolo come 
``templating.helper`` e definendo un attributo ``alias`` 
(l'helper sarà accessibilr tramite il suo alias nei template):

.. configuration-block::

    .. code-block:: yaml

        services:
            templating.helper.your_helper_name:
                class: Fully\Qualified\Helper\Class\Name
                tags:
                    - { name: templating.helper, alias: alias_name }

    .. code-block:: xml

        <service id="templating.helper.your_helper_name" class="Fully\Qualified\Helper\Class\Name">
            <tag name="templating.helper" alias="alias_name" />
        </service>

    .. code-block:: php

        $container
            ->register('templating.helper.your_helper_name', 'Fully\Qualified\Helper\Class\Name')
            ->addTag('templating.helper', array('alias' => 'alias_name'))
        ;
