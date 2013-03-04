SensioFrameworkExtraBundle
==========================

Il bundle ``FrameworkBundle`` di Symfony2 implementa un framework MVC semplice, ma
robusto. Il bundle `SensioFrameworkExtraBundle`_ lo estende, aggiungendo utili
convenzioni e annotazioni. Inoltre consente controllori più concisi.

Installazione
-------------

`Scaricare`_ il bundle e metterlo sotto lo spazio dei nomi ``Sensio\Bundle\``.
Quindi, come per ogni altro bundle, includerlo nella propria classe Kernel::

    public function registerBundles()
    {
        $bundles = array(
            ...

            new Sensio\Bundle\FrameworkExtraBundle\SensioFrameworkExtraBundle(),
        );

        ...
    }

Configurazione
--------------

Tutte le caratteristiche fornite dal bundle sono abilitate in modo predefinito alla
registrazione del bundle stesso nella propria classe Kernel.

La configurazione predefinita è la seguente:

.. configuration-block::

    .. code-block:: yaml

        sensio_framework_extra:
            router:  { annotations: true }
            request: { converters: true }
            view:    { annotations: true }
            cache:   { annotations: true }

    .. code-block:: xml

        <!-- xmlns:sensio-framework-extra="http://symfony.com/schema/dic/symfony_extra" -->
        <sensio-framework-extra:config>
            <router annotations="true" />
            <request converters="true" />
            <view annotations="true" />
            <cache annotations="true" />
        </sensio-framework-extra:config>

    .. code-block:: php

        // carica il profilatore
        $container->loadFromExtension('sensio_framework_extra', array(
            'router'  => array('annotations' => true),
            'request' => array('converters' => true),
            'view'    => array('annotations' => true),
            'cache'   => array('annotations' => true),
        ));

Si possono disabilitare alcune annotazioni e convenzioni, definendo una o più
impostazioni come ``false``.

Annotazioni per i controllori
-----------------------------

Le annotazioni sono un bel modo per configurare facilmente i proprio controllori, dalle
rotte alla configurazione della cache.

Anche se le annotazioni non sono una caratteristica nativa di PHP, hanno comunque molti
vantaggi rispetto ai metodi classici di configurazione di Symfony2:

* Il codice e la configurazione sono nello stesso posto (la classe del controllore);
* Semplici da imparare e da usare;
* Concise da scrivere;
* Rendono snello il controllore (lasciandogli solo la responsabilità di prendere i dati
  dal modello).

.. tip::

   Se si usano classi per le viste, le annotazioni sono un bel modo per evitare la
   creazioni di tali classi, per i casi più comuni.

Il bundle definisce le seguenti annotazioni:

.. toctree::
   :maxdepth: 1

   annotations/routing
   annotations/converters
   annotations/view
   annotations/cache

Questo esempio mostra tutte le annotazioni disponibili in azione::

    use Sensio\Bundle\FrameworkExtraBundle\Configuration\Route;
    use Sensio\Bundle\FrameworkExtraBundle\Configuration\Cache;
    use Sensio\Bundle\FrameworkExtraBundle\Configuration\Template;
    use Sensio\Bundle\FrameworkExtraBundle\Configuration\ParamConverter;
    use Sensio\Bundle\FrameworkExtraBundle\Configuration\Method;

    /**
     * @Route("/blog")
     * @Cache(expires="tomorrow")
     */
    class AnnotController extends Controller
    {
        /**
         * @Route("/")
         * @Template
         */
        public function indexAction()
        {
            $posts = ...;

            return array('posts' => $posts);
        }

        /**
         * @Route("/{id}")
         * @Method("GET")
         * @ParamConverter("post", class="SensioBlogBundle:Post")
         * @Template("SensioBlogBundle:Annot:post.html.twig", vars={"post"})
         * @Cache(smaxage="15")
         */
        public function showAction(Post $post)
        {
        }
    }

Poiché il metodo ``showAction`` segue alcune convenzioni, si possono omettere alcune
annotazioni::

    /**
     * @Route("/{id}")
     * @Cache(smaxage="15")
     */
    public function showAction(Post $post)
    {
    }

.. _`SensioFrameworkExtraBundle`: https://github.com/sensio/SensioFrameworkExtraBundle
.. _`Scaricare`: http://github.com/sensio/SensioFrameworkExtraBundle
