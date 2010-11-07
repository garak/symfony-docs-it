.. index::
   single: Twig
   single: View; Twig

Twig e Symfony2
===============

`Twig`_ è un linguaggio per template per PHP flessibile, veloce e sicuro.
Symfony2 ha supporto nativo per Twig tramite ``TwigBundle``.

.. index::
   single: Twig; Installazione
   single: Twig; Configurazione

Installazione e Configurazione
------------------------------

Abilitare ``TwigBundle`` nel kernel::

    // app/HelloKernel.php

    public function registerBundles()
    {
        $bundles = array(
            // ...
            new Symfony\Bundle\TwigBundle\TwigBundle(),
        );

        // ...
    }

Configurarlo:

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        twig.config: ~

        # app/config/config_dev.yml
        twig.config:
            auto_reload: true

    .. code-block:: xml

        <!--
        xmlns:twig="http://www.symfony-project.org/schema/dic/twig"
        xsi:schemaLocation="http://www.symfony-project.org/schema/dic/twig http://www.symfony-project.org/schema/dic/twig/twig-1.0.xsd
        -->

        <!-- app/config/config.xml -->
        <twig:config />

        <!-- app/config/config_dev.xml -->
        <twig:config auto_reload="true" />

    .. code-block:: php

        // app/config/config.php
        $container->loadFromExtension('twig', 'config');

        // app/config/config_dev.php
        $container->loadFromExtension('twig', 'config', array('auto_reload' => true));

.. tip::
   Le opzioni di configurazione sono le stesse che si passano al
   `costruttore`_ di ``Twig_Environment``.

Renderizzare Template Twig
--------------------------

Per renderizzare un template Twig invece di uno in PHP aggiungere il suffisso
``.twig`` alla fine del nome del template. Il controllore seguente renderizza
il template ``index.twig``::

    // src/Application/HelloBundle/Controller/HelloController.php

    public function indexAction($name)
    {
        return $this->render('HelloBundle:Hello:index.twig', array('name' => $name));
    }

.. code-block:: jinja

    {# src/Application/HelloBundle/Resources/views/Hello/index.twig #}

    {% extends "HelloBundle::layout.twig" %}

    Hello {{ $name }}!

.. note::
   I template Twig devono utilizzare l'estensione ``twig``.

E qui un layout classico:

.. code-block:: jinja

   {# src/Application/HelloBundle/Resources/views/layout.twig #}
    <!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
    <html>
        <head>
            <meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
        </head>
        <body>
            {% block body %}{% endblock %}
        </body>
    </html>

Includere altri Template
------------------------

Il modo migliore di condividere una porzione di codice tra diversi e distinti
template è quello di definire un template che possa essere incluso in un altro.

Creare un template ``hello.twig``:

.. code-block:: jinja

    {# src/Application/HelloBundle/Resources/views/Hello/hello.twig #}
    Hello {{ $name }}

E modificare il template ``index.twig`` per includerlo:

.. code-block:: jinja

    {# src/Application/HelloBundle/Resources/views/Hello/index.php #}
    {% extends "HelloBundle::layout.twig" %}

    {% include "HelloBundle:Hello:hello.twig" %}

.. tip:
   Si può anche includere un template PHP in un template Twig:

    .. code-block:: jinja

        {# index.twig #}

        {% render 'HelloBundle:Hello:sidebar.php' %}

Includere altri Controllori
---------------------------

Come fare se si volesse includere il risultato di un altro controllore in
un template? È molto utile quando si lavora con Ajax, o quando il template
incluso ha bisogno di alcune variabili non disponibili nel template principale.

Se si creasse un'azione ``fancy``, e si volesse includerla nel template ``index``
, si potrebbe usare il semplice codice seguente:

.. code-block:: jinja

    <!-- src/Application/HelloBundle/Resources/views/Hello/index.php -->
    {% render "HelloBundle:Hello:fancy" with ['name': name, 'color': 'green'] %}

Qui la stringa ``HelloBundle:Hello:fancy`` si riferisce all'azione ``fancy`` del
controllore ``Hello``, l'argomento è utilizzato come simulazione dei valori della richiesta::

    // src/Application/HelloBundle/Controller/HelloController.php

    class HelloController extends Controller
    {
        public function fancyAction($name, $color)
        {
            // create some object, based on the $color variable
            $object = ...;

            return $this->render('HelloBundle:Hello:fancy.twig', array('name' => $name, 'object' => $object));
        }

        // ...
    }

.. index::
   single: Twig; Helpers

Utilizzo dei Template Helper
----------------------------

Gli helper predefiniti in Symfony2 sono disponibili in un template Twig
utilizzando dei tag specifici:

.. code-block:: jinja

    {# aggiungere un javascript #}
    {% javascript 'bundles/blog/js/blog.js' %}

    {# aggiungere un foglio di stile #}
    {% stylesheet 'bundles/blog/css/blog.css' with ['media': 'screen'] %}

    {# pubblicare javascript e fogli di stile nel layout #}
    {% javascripts %}
    {% stylesheets %}

    {# generare una URL per un contenuto #}
    {% asset 'css/blog.css' %}
    {% asset 'images/logo.png' %}

    {# generare un path (/blog/1) #}
    {% path 'blog_post' with ['id': post.id] %}

    {# generare un URL (http://example.com/blog/1) #}
    {% url 'blog_post' with ['id': post.id] %}

    {# renderizzare un template #}
    {% include 'BlogBundle:Post:list.twig' %}

    {# includere la risposta di un altro controller #}
    {% render 'BlogBundle:Post:list' with ['limit': 2], ['alt': 'BlogBundle:Post:error'] %}

.. _twig_extension_tag:

Abilitare Estensioni Twig Personalizzate
----------------------------------------

Per abilitare un'estensione Twig aggiungerla come un normale servizio
in un file di configurazione ed etichettarla come ``twig.extension``:

.. configuration-block::

    .. code-block:: yaml

        services:
            twig.extension.your_extension_name:
                class: Fully\Qualified\Extension\Class\Name
                tags:
                    - { name: twig.extension }

    .. code-block:: xml

        <service id="twig.extension.your_extension_name" class="Fully\Qualified\Extension\Class\Name">
            <tag name="twig.extension" />
        </service>

    .. code-block:: php

        $container
            ->register('twig.extension.your_extension_name', 'Fully\Qualified\Extension\Class\Name')
            ->addTag('twig.extension')
        ;

.. _Twig:        http://www.twig-project.org/
.. _costruttore: http://www.twig-project.org/book/03-Twig-for-Developers
