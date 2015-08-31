.. index::
   single: Sicurezza; Proteggere un servizio
   single: Sicurezza; Proteggere un metodo

Proteggere servizi e metodi di un'applicazione
==============================================

Nel capitolo sulla sicurezza, si può vedere come :ref:`proteggere un controllore <book-security-securing-controller>`,
richiedendo il servizio ``security.context`` dal contenitore di servizi
e verificando il ruolo dell'utente attuale::

    // ...
    use Symfony\Component\Security\Core\Exception\AccessDeniedException;

    public function helloAction($name)
    {
        if (false === $this->get('security.context')->isGranted('ROLE_ADMIN')) {
            throw new AccessDeniedException();
        }

        // ...
    }

Si può anche proteggere *qualsiasi* servizio in modo simile, iniettando in esso
il servizio ``security.context``. Per un'introduzione generale all'iniezione di dipendenze
nei servizi, vedere il capitolo :doc:`/book/service_container` del libro. Per esempio,
si supponga di avere una classe ``NewsletterManager``, che invia email, e di voler
restringere il suo utilizzo ai soli utenti con un ruolo ``ROLE_NEWSLETTER_ADMIN``.
Prima di aggiungere la sicurezza, la classe assomiglia a qualcosa del genere::

    // src/AppBundle/Newsletter/NewsletterManager.php
    namespace AppBundle\Newsletter;

    class NewsletterManager
    {
        public function sendNewsletter()
        {
            // qui va la logica specifica
        }

        // ...
    }

Lo scopo è verificare il ruolo dell'utente al richiamo del metodo ``sendNewsletter()``.
Il primo passo in questa direzione è l'iniezione del servizio ``security.context``
nell'oggetto. Non avendo molto senso *non* eseguire un controllo di sicurezza, questo è
un candidato ideale per un'iniezione nel costruttore, che garantisce che l'oggetto
della sicurezza sia disponibile in tutta la classe ``NewsletterManager``::


    // ...
    use Symfony\Component\Security\Core\SecurityContextInterface;

    class NewsletterManager
    {
        protected $securityContext;

        public function __construct(SecurityContextInterface $securityContext)
        {
            $this->securityContext = $securityContext;
        }

        // ...
    }

Quindi, nella configurazione dei servizi, si può iniettare il servizio:

.. configuration-block::

    .. code-block:: yaml

        # app/config/services.yml
        services:
            newsletter_manager:
                class:     AppBundle\Newsletter\NewsletterManager
                arguments: ["@security.context"]

    .. code-block:: xml

        <!-- app/config/services.xml -->
        <services>
            <service id="newsletter_manager" class="AppBundle\Newsletter\NewsletterManager">
                <argument type="service" id="security.context"/>
            </service>
        </services>

    .. code-block:: php

        // app/config/services.php
        use Symfony\Component\DependencyInjection\Definition;
        use Symfony\Component\DependencyInjection\Reference;

        $container->setDefinition('newsletter_manager', new Definition(
            'AppBundle\Newsletter\NewsletterManager',
            array(new Reference('security.context'))
        ));

Il servizio iniettato può quindi essere usato per eseguire il controllo di sicurezza,
quando il metodo ``sendNewsletter()`` viene richiamato::

    // ...
    use Symfony\Component\Security\Core\SecurityContextInterface;

    class NewsletterManager
    {
        protected $securityContext;

        public function __construct(SecurityContextInterface $securityContext)
        {
            $this->securityContext = $securityContext;
        }

        public function sendNewsletter()
        {
            if (false === $this->securityContext->isGranted('ROLE_NEWSLETTER_ADMIN')) {
                throw new AccessDeniedException();
            }

            // ...
        }

        // ...
    }

Se l'utente attuale non ha il ruolo ``ROLE_NEWSLETTER_ADMIN``, gli sarà richiesto
di autenticarsi.

Mettere i sicurezza i metodi con le annotazioni
-----------------------------------------------

Si possono anche proteggere i metodi di un servizio tramite annotazioni, usando
il bundle `JMSSecurityExtraBundle`_. Questo bundle è incluso nella
Standard Edition di Symfony.

Per abilitare le annotazioni, assegnare il :ref:`tag<book-service-container-tags>`
``security.secure_service`` al servizio da proteggere
(si può anche abilitare automaticamente la funzionalità per tutti i servizi, vedere i
:ref:`dettagli<securing-services-annotations-sidebar>` più avanti):

.. configuration-block::

    .. code-block:: yaml

        # app/services.yml

        # ...
        services:
            newsletter_manager:
                # ...
                tags:
                    -  { name: security.secure_service }

    .. code-block:: xml

        <!-- app/services.xml -->
        <!-- ... -->

        <services>
            <service id="newsletter_manager" class="AppBundle\Newsletter\NewsletterManager">
                <!-- ... -->
                <tag name="security.secure_service" />
            </service>
        </services>

    .. code-block:: php

        // app/services.php
        use Symfony\Component\DependencyInjection\Definition;
        use Symfony\Component\DependencyInjection\Reference;

        $definition = new Definition(
            'AppBundle\Newsletter\NewsletterManager',
            array(new Reference('security.context'))
        ));
        $definition->addTag('security.secure_service');
        $container->setDefinition('newsletter_manager', $definition);

Si possono ottenere gli stessi risultati usando le annotazioni::

    namespace AppBundle\Newsletter;

    use JMS\SecurityExtraBundle\Annotation\Secure;
    // ...

    class NewsletterManager
    {

        /**
         * @Secure(roles="ROLE_NEWSLETTER_ADMIN")
         */
        public function sendNewsletter()
        {
            // ...
        }

        // ...
    }

.. note::

    Le annotazioni funzionano perché viene creata una classe proxy per la propria classe,
    che esegue i controlli di sicurezza. Questo vuol dire che, sebbene si possano usare
    le annotazioni su metodi pubblici e protetti, non si possono usare su metodi
    privati o su metodi finali.

Il bundle JMSSecurityExtraBundle consente anche di proteggere i parametri e
i valori restituiti dai metodi. Per maggiori informazioni vedere la documentazione di
`JMSSecurityExtraBundle`_.

.. _securing-services-annotations-sidebar:

.. sidebar:: Attivare le annotazioni per tutti i servizi

    Quando si proteggono i metodi di un servizio (come mostrato precedentemente),
    si possono assegnare tag a ogni servizio individualmente oppure attivare la funzionalità per
    *tutti* i servizi. Per farlo, impostare l'opzione ``secure_all_services`` a
    ``true``:

    .. configuration-block::

        .. code-block:: yaml

            # app/config/config.yml
            jms_security_extra:
                # ...
                secure_all_services: true

        .. code-block:: xml

            <!-- app/config/config.xml -->
            <?xml version="1.0" ?>
            <container xmlns="http://symfony.com/schema/dic/services"
                xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
                xmlns:jms-security-extra="http://example.org/schema/dic/jms_security_extra"
                xsi:schemaLocation="http://www.example.com/symfony/schema/ http://www.example.com/symfony/schema/hello-1.0.xsd">

                <!-- ... -->
                <jms-security-extra:config secure-controllers="true" secure-all-services="true" />

            </srv:container>

        .. code-block:: php

            // app/config/config.php
            $container->loadFromExtension('jms_security_extra', array(
                // ...
                'secure_all_services' => true,
            ));

    Lo svantaggio di questo sistema è che, se attivato, il caricamento della pagina
    iniziale potrebbe essere molto lento, a seconda di quanti servizi sono stati definiti.

.. _`JMSSecurityExtraBundle`: https://github.com/schmittjoh/JMSSecurityExtraBundle
