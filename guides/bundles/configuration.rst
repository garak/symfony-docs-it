.. index::
   single: Bundles; Configuration

Configurazione dei Bundle
=========================

Per offrire una maggiore flessibilità un bundle può avere impostazioni
configurabili tramite i meccanismi integrati in Symfony2.

Configurazione Semplice
-----------------------

Per configurazioni semplici fare affidamento sulla parte ``parameters``
di default nella configurazione di Symfony2. I parametri di Symfony2 
sono delle coppie chiave/valore; dove per valore si intendo ogni valore
valido per PHP. Ogni parametro ha un nome che deve iniziare con la versione
minuscola del nome del bundle (`hello`` per ``HelloBundle``, o 
``sensio.social.blog`` per ``Sensio\Social\BlogBundle`` per esempio).

L'utente finale può inserire valori in ogni file di configurazione:

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        parameters:
            hello.email.from: fabien@example.com

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <parameters>
            <parameter key="hello.email.from">fabien@example.com</parameter>
        </parameters>

    .. code-block:: php

        // app/config/config.php
        $container->setParameter('hello.email.from', 'fabien@example.com');

    .. code-block:: ini

        [parameters]
        hello.email.from = fabien@example.com

Ottenere i parametri di configurazione nel codice dal container::

    $container->getParameter('hello.email.from');

Anche se il meccanismo è abbastanza semplice è caldamente consigliato di
utilizzare la semantica di configurazione descritta di seguito.

.. index::
   single: Configuration; Semantic
   single: Bundle; Extension Configuration

Semantica di Configurazione
---------------------------

La semantica di configurazione offre un modo ancora più flessibile di
specificare la configurazione per un bundle con i seguenti vantaggi
a differenza dei parametri semplici:

* Possibilità di definire più che semplici parametri (servizi per esempio):

* Migliore gerarchia nella configurazione (possibilità di definire configurazioni
  annidate);

* Fusione intelligente quando diversi file di configurazione sovrascrivono
  una configurazione esistente;

* Validazione della configurazione (se si definisce un file XSD e si utilizza XML);

* Autocompletamento utilizzando XSD e XML.

.. index::
   single: Bundles; Extension
   single: Dependency Injection, Extension

Creare un Estansione
~~~~~~~~~~~~~~~~~~~~

Per definire una configurazione semantica creare una Dependency Injection extension
che estende
:class:`Symfony\\Component\\DependencyInjection\\Extension\\Extension`::

    // HelloBundle/DependencyInjection/HelloExtension.php
    use Symfony\Component\DependencyInjection\Extension\Extension;

    class HelloExtension extends Extension
    {
        public function configLoad($config, ContainerBuilder $container)
        {
            // ...
        }

        public function getXsdValidationBasePath()
        {
            return __DIR__.'/../Resources/config/';
        }

        public function getNamespace()
        {
            return 'http://www.example.com/symfony/schema/';
        }

        public function getAlias()
        {
            return 'hello';
        }
    }

La classe precedente definisce un namespace ``hello:config`` utilizzabile in
ogni file di configurazione:

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        hello.config: ~

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <?xml version="1.0" ?>

        <container xmlns="http://www.symfony-project.org/schema/dic/services"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xmlns:hello="http://www.example.com/symfony/schema/"
            xsi:schemaLocation="http://www.example.com/symfony/schema/ http://www.example.com/symfony/schema/hello-1.0.xsd">

           <hello:config />
           ...

        </container>

    .. code-block:: php

        // app/config/config.php
        $container->loadFromExtension('hello', 'config', array());

.. note::
   Si possono creare tanti metodi ``xxxLoad()`` quanti se ne hanno bisogno
   per definire più blocchi di configurazione per l'estesione.

Parsing di una Configurazione
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Ogni volta che un utente include il namespace ``hello.config`` in un file
di configurazione il metodo ``configLoad()`` dell'estensione viene chiamato
e la configurazione viene passata come un array (Symfony2 converte automaticamente
XML e YAML ad un array).

Quindi data la seguente configurazione:

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        hello.config:
            foo: foo
            bar: bar

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <?xml version="1.0" ?>

        <container xmlns="http://www.symfony-project.org/schema/dic/services"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xmlns:hello="http://www.example.com/symfony/schema/"
            xsi:schemaLocation="http://www.example.com/symfony/schema/ http://www.example.com/symfony/schema/hello-1.0.xsd">

            <hello:config foo="foo">
                <hello:bar>foo</hello:bar>
            </hello:config>

        </container>

    .. code-block:: php

        // app/config/config.php
        $container->loadFromExtension('hello', 'config', array(
            'foo' => 'foo',
            'bar' => 'bar',
        ));

L'array passato al proprio metodo assomiglierà al seguente::

    array(
        'foo' => 'foo',
        'bar' => 'bar',
    )

Dentro ``configLoad()`` la variabile ``$container`` si riferisce ad un
container che conosce solamente la configurazione dello specifico namespace.
Tale configurazione può essere manipolata a piacimento per aggiungere servizi
e parametri. La prima volta che il metodo viene utilizzato il container
conosce solo i parametri globali. Per le chiamate successive conterrà la
configurazione come definita dalla chiamate precedenti. Quindi il metodo
deve fondere assieme le nuove impostazioni di configurazione con quelle
vecchie::

    // carica solamente una volta i servizi ed i parametri predefiniti
    if (!$container->hasDefinition('xxxxx')) {
        $loader = new XmlFileLoader($container, __DIR__.'/../Resources/config');
        $loader->load('hello.xml');
    }

I parametri globali sono i seguenti:

* ``kernel.name``
* ``kernel.environment``
* ``kernel.debug``
* ``kernel.root_dir``
* ``kernel.cache_dir``
* ``kernel.logs_dir``
* ``kernel.bundle_dirs``
* ``kernel.bundles``
* ``kernel.charset``

.. caution::
   Tutti i nomi di parametri e di servizi che iniziano con ``_`` sono riservati
   per il framework ed i bundle non devono definirne di nuovi.

.. index::
   pair: Convention; Configuration

Convenzioni per le Estensioni
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Quando si crea un'estensione seguire queste semplici convenzioni:

* L'estensione deve essere memorizzate nel ``DependencyInjection`` sotto-namespace;

* Il nome dell'estensione viene dopo il nome del bundle ed ha  ``Extension``
  come suffisso (``HelloExtension`` per ``HelloBundle``) -- quando si creano
  diverse estensioni per un singolo bundle chiudere il nome solo con ``Extension``;

* L'alias deve essere univoco e deciso dopo il nome del bundle (``hello`` per
  ``HelloBundle`` o ``sensio.social.blog`` per ``Sensio\Social\BlogBundle``);

* L'estensione deve fornire uno schema XSD.

Seguendo queste semplici convenzioni l'estensione verrà registrata automaticamente
da Symfony2. In caso contrario fare override del metodo Bundle
:method:`Symfony\\Component\\HttpKernel\\Bundle\\Bundle::registerExtensions`::

    class HelloBundle extends Bundle
    {
        public function registerExtensions(ContainerBuilder $container)
        {
            // registra le estensioni trovate nella directory DependencyInjection/
            parent::registerExtensions($container);

            // registra manualmente le estensioni che non seguono le convenzioni
            $container->registerExtension(new ExtensionHello());
        }
    }

.. index::
   single: Bundles; Default Configuration

Configurazione Standard
~~~~~~~~~~~~~~~~~~~~~~~

Come visto precedentemente l'utente del bundle dovrebbe includere il namespace
trovate nella directory``hello.config`` in un file di configurazione per invocare il codice dell'estensione.
Ma è possibile registrare automaticamente una configurazione standard usando
l'override del metodo Bundle
:method:`Symfony\\Component\\HttpKernel\\Bundle\\Bundle::registerExtensions`
::

    class HelloBundle extends Bundle
    {
        public function registerExtensions(ContainerBuilder $container)
        {
            // registra le estensioni HelloBundle trovate nella directory DependencyInjection/
            parent::registerExtensions($container);

            // carica alcuni valori predefiniti
            $container->loadFromExtension('hello', 'config', array(/* your default config for the hello.config namespace */));
        }
    }

.. caution::
   Symfony2 prova ad essere più esplicito possibile. Quindi registrare una
   configurazione standard automaticamente non è forse un'ottima idea.
