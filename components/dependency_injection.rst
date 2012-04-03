.. index::
    single: Dependency Injection

Il componente Dependency Injection
==================================

    Il componente Dependency Injection consente di standardizzare e centralizzare
    il modo in cui gli oggetti sono costruiti nelle applicazioni.

Per un'introduzione a Dependency Injection e ai contenitori di servizi, vedere
:doc:`/book/service_container`

Installazione
-------------

È possibile installare il componente in diversi modi:

* Utilizzando il repository ufficiale su Git (https://github.com/symfony/DependencyInjection);
* Installandolo via PEAR ( `pear.symfony.com/DependencyInjection`);
* Installandolo via Composer (`symfony/dependency-injection` su Packagist).

Utilizzo
--------

Si potrebbe avere una semplice classe, come la seguente ``Mailer``, che si vuole
rendere disponibile come servizio:

.. code-block:: php

    class Mailer
    {
        private $transport;

        public function __construct()
        {
            $this->transport = 'sendmail';
        }

        // ...
    }

La si può registrare nel contenitore come servizio:

.. code-block:: php

    use Symfony\Component\DependencyInjection\ContainerBuilder;

    $sc = new ContainerBuilder();
    $sc->register('mailer', 'Mailer');

Un possibile miglioramento alla classe per renderla più flessibile sarebbe consentire 
al contenitore di impostare il trasporto usato. Si può cambiare la classe, 
in modo che il trasporto sia passato al costruttore:

.. code-block:: php

    class Mailer
    {
        private $transport;

        public function __construct($transport)
        {
            $this->transport = $transport;
        }

        // ...
    }

Si può quindi impostare la scelta del trasporto nel contenitore:

.. code-block:: php

    use Symfony\Component\DependencyInjection\ContainerBuilder;

    $sc = new ContainerBuilder();
    $sc->register('mailer', 'Mailer')
        ->addArgument('sendmail'));

La classe ora è molto più flessibile, perché la scelta del trasporto è stata
separata dall'implementazione e posta nel contenitore.

La scelta del trasporto potrebbe interessare anche altri servizi.
Si può evitare di doverlo cambiare in posti differenti, trasformandolo in
un parametro nel contenitore e facendo riferimento a tale parametro per
il costruttore del servizio ``Mailer``:


.. code-block:: php

    use Symfony\Component\DependencyInjection\ContainerBuilder;

    $sc = new ContainerBuilder();
    $sc->setParameter('mailer.transport', 'sendmail');
    $sc->register('mailer', 'Mailer')
        ->addArgument('%mailer.transport%'));

Ora che il servizio ``mailer`` è nel contenitore, lo si può iniettare come 
dipendenza di altre classi. Se si ha una classe ``NewsletterManager`` come
questa:

.. code-block:: php

    use Mailer;

    class NewsletterManager
    {
        private $mailer;

        public function __construct(Mailer $mailer)
        {
            $this->mailer = $mailer;
        }

        // ...
    }

Allora la si può registrare come servizio e passarle il servizio ``mailer``:

.. code-block:: php

    use Symfony\Component\DependencyInjection\ContainerBuilder;
    use Symfony\Component\DependencyInjection\Reference;

    $sc = new ContainerBuilder();

    $sc->setParameter('mailer.transport', 'sendmail');
    $sc->register('mailer', 'Mailer')
        ->addArgument('%mailer.transport%'));

    $sc->register('newsletter_manager', 'NewsletterManager')
        ->addArgument(new Reference('mailer'));

Se ``NewsletterManager`` non richiedesse ``Mailer`` e l'iniezione fosse quindi
solamente opzionale, la si potrebbe passare usando un setter:

.. code-block:: php

    use Mailer;

    class NewsletterManager
    {
        private $mailer;

        public function setMailer(Mailer $mailer)
        {
            $this->mailer = $mailer;
        }

        // ...
    }

Ora si può scegliere di non iniettare un ``Mailer`` dentro ``NewsletterManager``.
Se comunque lo si volesse fare, il contenitore può richiamare il metodo setter:

.. code-block:: php

    use Symfony\Component\DependencyInjection\ContainerBuilder;
    use Symfony\Component\DependencyInjection\Reference;

    $sc = new ContainerBuilder();

    $sc->setParameter('mailer.transport', 'sendmail');
    $sc->register('mailer', 'Mailer')
        ->addArgument('%mailer.transport%'));

    $sc->register('newsletter_manager', 'NewsletterManager')
        ->addMethodCall('setMailer', new Reference('mailer'));

Si può quindi ottenere il servizio ``newsletter_manager`` dal contenitore,
in questo modo:

.. code-block:: php

    use Symfony\Component\DependencyInjection\ContainerBuilder;
    use Symfony\Component\DependencyInjection\Reference;

    $sc = new ContainerBuilder();

    //--

    $newsletterManager = $sc->get('newsletter_manager');

Evitare che il proprio codice dipenda dal contenitore
-----------------------------------------------------

Sebbene si possano recuperare servizi direttamente dal contenitore, sarebbe
meglio minimizzarlo. Per esempio, in ``NewsletterManager`` abbiamo iniettato
il servizio ``mailer``, piuttosto che richiederlo al contenitore.
Avremo potuto iniettare il contenitore e recuperare da esso il servizio ``mailer``,
ma allora sarebbe stato legato a questo particolare contenitore, rendendo
difficile riusare la classe altrove.

A un certo punto si avrà la necessità di ottenere un servizio dal contenitore,
ma lo si dovrebbe fare il meno possibile e all'inizio della propria applicazione.

Impostare il contenitore con file di configurazione
---------------------------------------------------

Oltre a impostare servizi usando PHP, come sopra, si possono usare dei file di
configurazione. Per poterlo fare, occorre installare anche il componente Config:

* Usare il repository Git (https://github.com/symfony/Config);
* Installarlo tramite PEAR ( `pear.symfony.com/Config`);
* Installarlo tramite Composer (`symfony/config` on Packagist).

Caricare un file di configurazione xml:

.. code-block:: php

    use Symfony\Component\DependencyInjection\ContainerBuilder;
    use Symfony\Component\Config\FileLocator;
    use Symfony\Component\DependencyInjection\Loader\XmlFileLoader;

    $sc = new ContainerBuilder();
    $loader = new XmlFileLoader($container, new FileLocator(__DIR__));
    $loader->load('services.xml');

Caricare un file di configurazione yaml:

.. code-block:: php

    use Symfony\Component\DependencyInjection\ContainerBuilder;
    use Symfony\Component\Config\FileLocator;
    use Symfony\Component\DependencyInjection\Loader\YamlFileLoader;

    $sc = new ContainerBuilder();
    $loader = new YamlFileLoader($container, new FileLocator(__DIR__));
    $loader->load('services.yml');

I servizi ``newsletter_manager`` e `` mailer`` possono essere impostati da file di configurazione:

.. configuration-block::

    .. code-block:: yaml

        # src/Acme/HelloBundle/Resources/config/services.yml
        parameters:
            # ...
            mailer.transport: sendmail

        services:
            my_mailer:
                class:     Mailer
                arguments: [@mailer]
            newsletter_manager:
                class:     NewsletterManager
                calls:
                    - [ setMailer, [ @mailer ] ]

    .. code-block:: xml

        <!-- src/Acme/HelloBundle/Resources/config/services.xml -->
        <parameters>
            <!-- ... -->
            <parameter key="mailer.transport">sendmail</parameter>
        </parameters>

        <services>
            <service id="mailer" class="Mailer">
                <argument>%mailer.transport%</argument>
            </service>

            <service id="newsletter_manager" class="NewsletterManager">
                <call method="setMailer">
                     <argument type="service" id="mailer" />
                </call>
            </service>
        </services>

    .. code-block:: php

        use Symfony\Component\DependencyInjection\Reference;

        // ...
        $sc->setParameter('mailer.transport', 'sendmail');
        $sc->register('mailer', 'Mailer')
           ->addArgument('%mailer.transport%'));

        $sc->register('newsletter_manager', 'NewsletterManager')
           ->addMethodCall('setMailer', new Reference('mailer'));


Imprare di più dalle ricette
----------------------------

* :doc:`/cookbook/service_container/factories`
* :doc:`/cookbook/service_container/parentservices`
