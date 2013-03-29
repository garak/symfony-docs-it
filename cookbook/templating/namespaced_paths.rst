.. index::
   single: Template; Percorsi di Twig con spazi di nomi

Usare e registrare percorsi di Twig con spazi di nomi
=====================================================

.. versionadded:: 2.2
    I percorsi con spazi di nomi sono stati aggiunti nella 2.2.

DI solito, quando ci si riferisce a un template, si usa il formato ``MioBundle:Cartella:filename.html.twig``
(vedere :ref:`template-naming-locations`).

Twig offre anche, nativamente, una caratteristica chiamata "percorsi con spazi di nomi", supportata
automaticamente per tutti i bundle.

Si prendano come esempio i seguenti percorsi:

.. code-block:: jinja

    {% extends "AcmeDemoBundle::layout.html.twig" %}
    {% include "AcmeDemoBundle:Pippo:pluto.html.twig" %}

Con i percorsi con spazi di nomi, funziona anche in questo modo:

.. code-block:: jinja

    {% extends "@AcmeDemo/layout.html.twig" %}
    {% include "@AcmeDemo/Pippo/pluto.html.twig" %}

Entrambi i percorsi sono validi e funzionanti in Symfony2.

.. tip::

    Come bonus aggiuntivo, la sintassi con gli spazi di nomi sono più veloci.

Registrare i propri spazi di nomi
---------------------------------

Si possono anche registrare propri spazi di nomi. Si supponga di usare
una libreria di terze parti che includa template Twig che si trovano in
``vendor/acme/pippo-pluto/templates``. Prima, registrare uno spazio di nomi per tale
cartella:

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        twig:
            # ...
            paths:
                "%kernel.root_dir%/../vendor/acme/pippo-pluto/templates": foo_bar

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <?xml version="1.0" ?>
        <container xmlns="http://symfony.com/schema/dic/services"
                   xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
                   xmlns:twig="http://symfony.com/schema/dic/twig"
        >

            <twig:config debug="%kernel.debug%" strict-variables="%kernel.debug%">
                <twig:path namespace="foo_bar">%kernel.root_dir%/../vendor/acme/pippo-pluto/templates</twig:path>
            </twig:config>
        </container>

    .. code-block:: php

        // app/config/config.php
        $container->loadFromExtension('twig', array(
            'paths' => array(
                '%kernel.root_dir%/../vendor/acme/pippo-pluto/templates' => 'foo_bar',
            );
        ));

Lo spazio di nomi registrato si chiama ``pippo_pluto``, riferito alla cartella
``vendor/acme/pippo-pluto/templates``. Supponendo che ci sia un file
di nome ``sidebar.twig`` in tale cartella, si può usare facilmente:

.. code-block:: jinja

    {% include '@pippo_pluto/sidebar.twig` %}