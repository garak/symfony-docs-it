Come gestire le dipendenza comuni con i servizi padre
=====================================================

Aggiungendo funzionalità all'applicazione si può arrivare ad un punto in cui
classi tra loro collegate condividano alcune dipendenze. Si potrebbe avere, ad esempio,
un Gestore Newsletter che usa un setter injection per configurare le proprie dipendenze::

    namespace Acme\HelloBundle\Mail;

    use Acme\HelloBundle\Mailer;
    use Acme\HelloBundle\FormattatoreMail;

    class GestoreNewsletter
    {
        protected $mailer;
        protected $formattatoreMail;

        public function setMailer(Mailer $mailer)
        {
            $this->mailer = $mailer;
        }

        public function setFormattatoreMail(FormattatoreMail $formattatoreMail)
        {
            $this->formattatoreMail = $formattatoreMail;
        }
        // ...
    }

ed una classe Cartolina che condivide le stesse dipendenze::

    namespace Acme\HelloBundle\Mail;

    use Acme\HelloBundle\Mailer;
    use Acme\HelloBundle\FormattatoreMail;

    class GestoreCartoline
    {
        protected $mailer;
        protected $formattatoreMail;

        public function setMailer(Mailer $mailer)
        {
            $this->mailer = $mailer;
        }

        public function setFormattatoreMail(FormattatoreMail $formattatoreMail)
        {
            $this->formattatoreMail = $formattatoreMail;
        }
        // ...
    }

La configurazione del servizio per queste classi sarà simile alla seguente:

.. configuration-block::

    .. code-block:: yaml

        # src/Acme/HelloBundle/Resources/config/services.yml
        parameters:
            # ...
            gestore_newsletter.class: Acme\HelloBundle\Mail\GestoreNewsletter
            gestore_cartoline.class: Acme\HelloBundle\Mail\GestoreCartoline
        services:
            mio_mailer:
                # ...
            mio_formattatore_mail:
                # ...
            gestore_newsletter:
                class:     %gestore_newsletter.class%
                calls:
                    - [ setMailer, [ @mio_mailer ] ]
                    - [ setFormattatoreMail, [ @mio_formattatore_mail] ]

            gestore_cartoline:
                class:     %gestore_cartoline.class%
                calls:
                    - [ setMailer, [ @mio_mailer ] ]
                    - [ setFormattatoreMail, [ @mio_formattatore_mail] ]

    .. code-block:: xml

        <!-- src/Acme/HelloBundle/Resources/config/services.xml -->
        <parameters>
            <!-- ... -->
            <parameter key="gestore_newsletter.class">Acme\HelloBundle\Mail\GestoreNewsletter</parameter>
            <parameter key="gestore_cartoline.class">Acme\HelloBundle\Mail\GestoreCartoline</parameter>
        </parameters>

        <services>
            <service id="mio_mailer" ... >
              <!-- ... -->
            </service>
            <service id="mio_formattatore_mail" ... >
              <!-- ... -->
            </service>
            <service id="gestore_newsletter" class="%gestore_newsletter.class%">
                <call method="setMailer">
                     <argument type="service" id="mio_mailer" />
                </call>
                <call method="setFormattatoreMail">
                     <argument type="service" id="mio_formattatore_mail" />
                </call>
            </service>
            <service id="gestore_cartoline" class="%gestore_cartoline.class%">
                <call method="setMailer">
                     <argument type="service" id="mio_mailer" />
                </call>
                <call method="setFormattatoreMail">
                     <argument type="service" id="mio_formattatore_mail" />
                </call>
            </service>
        </services>

    .. code-block:: php

        // src/Acme/HelloBundle/Resources/config/services.php
        use Symfony\Component\DependencyInjection\Definition;
        use Symfony\Component\DependencyInjection\Reference;

        // ...
        $container->setParameter('gestore_newsletter.class', 'Acme\HelloBundle\Mail\GestoreNewsletter');
        $container->setParameter('gestore_cartoline.class', 'Acme\HelloBundle\Mail\GestoreCartoline');

        $container->setDefinition('mio_mailer', ... );
        $container->setDefinition('mio_formattatore_mail', ... );
        $container->setDefinition('gestore_newsletter', new Definition(
            '%gestore_newsletter.class%'
        ))->addMethodCall('setMailer', array(
            new Reference('mio_mailer')
        ))->addMethodCall('setFormattatoreMail', array(
            new Reference('mio_formattatore_mail')
        ));
        $container->setDefinition('gestore_cartoline', new Definition(
            '%gestore_cartoline.class%'
        ))->addMethodCall('setMailer', array(
            new Reference('mio_mailer')
        ))->addMethodCall('setFormattatoreMail', array(
            new Reference('mio_formattatore_mail')
        ));

Ci sono molte ripetizioni sia nelle classi che nella configurazione. Quasto vuol dire
che se qualcosa viene cambiato, ad esempio le classi ``Mailer`` o ``FormattatoreMail``
che dovranno essere iniettate tramite il costruttore, sarà necessario modificare
la configurazione in due posti. Altrettanto se si volesse modificare il metodo setter
sarebbe necessario modificare entrambe le classi. Il tipico modo di gestire
i metodi comuni di queste classi sarebbe quello di far si che estendano una comune super classe::

    namespace Acme\HelloBundle\Mail;

    use Acme\HelloBundle\Mailer;
    use Acme\HelloBundle\FormattatoreMail;

    abstract class GestoreMail
    {
        protected $mailer;
        protected $formattatoreMail;

        public function setMailer(Mailer $mailer)
        {
            $this->mailer = $mailer;
        }

        public function setFormattatoreMail(EmailFormatter $formattatoreMail)
        {
            $this->formattatoreMail = $formattatoreMail;
        }
        // ...
    }

Le classi ``GestoreNewsletter`` e ``GestoreCartoline`` potranno estendere questa
super classe::

    namespace Acme\HelloBundle\Mail;

    class GestoreNewsletter extends GestoreMail
    {
        // ...
    }

and::

    namespace Acme\HelloBundle\Mail;

    class GestoreCartoline extends GestoreMail
    {
        // ...
    }

Allo stesso modo, il contenitore di servizi di Symfony2 supporta la possibilità
di estendere i servizi nella configurazione in modo da poter ridurre le ripetizioni
specificando un serizio genitore.

.. configuration-block::

    .. code-block:: yaml

        # src/Acme/HelloBundle/Resources/config/services.yml
        parameters:
            # ...
            gestore_newsletter.class: Acme\HelloBundle\Mail\GestoreNewsletter
            gestore_cartoline.class: Acme\HelloBundle\Mail\GestoreCartoline
            gestore_mail.class: Acme\HelloBundle\Mail\GestoreMail
        services:
            mio_mailer:
                # ...
            mio_formattatore_mail:
                # ...
            gestore_mail:
                class:     %gestore_mail.class%
                abstract:  true
                calls:
                    - [ setMailer, [ @mio_mailer ] ]
                    - [ setFormattatoreMail, [ @mio_formattatore_mail] ]
            
            gestore_newsletter:
                class:     %gestore_newsletter.class%
                parent: gestore_mail
            
            gestore_cartoline:
                class:     %gestore_cartoline.class%
                parent: gestore_mail
            
    .. code-block:: xml

        <!-- src/Acme/HelloBundle/Resources/config/services.xml -->
        <parameters>
            <!-- ... -->
            <parameter key="gestore_newsletter.class">Acme\HelloBundle\Mail\GestoreNewsletter</parameter>
            <parameter key="gestore_cartoline.class">Acme\HelloBundle\Mail\GestoreCartoline</parameter>
            <parameter key="gestore_mail.class">Acme\HelloBundle\Mail\GestoreMail</parameter>
        </parameters>

        <services>
            <service id="mio_mailer" ... >
              <!-- ... -->
            </service>
            <service id="mio_formattatore_mail" ... >
              <!-- ... -->
            </service>
            <service id="gestore_mail" class="%gestore_mail.class%" abstract="true">
                <call method="setMailer">
                     <argument type="service" id="mio_mailer" />
                </call>
                <call method="setFormattatoreMail">
                     <argument type="service" id="mio_formattatore_mail" />
                </call>
            </service>
            <service id="gestore_newsletter" class="%gestore_newsletter.class%" parent="gestore_mail"/>
            <service id="gestore_cartoline" class="%gestore_cartoline.class%" parent="gestore_mail"/>
        </services>

    .. code-block:: php

        // src/Acme/HelloBundle/Resources/config/services.php
        use Symfony\Component\DependencyInjection\Definition;
        use Symfony\Component\DependencyInjection\Reference;

        // ...
        $container->setParameter('gestore_newsletter.class', 'Acme\HelloBundle\Mail\GestoreNewsletter');
        $container->setParameter('gestore_cartoline.class', 'Acme\HelloBundle\Mail\GestoreCartoline');
        $container->setParameter('gestore_mail.class', 'Acme\HelloBundle\Mail\GestoreMail');

        $container->setDefinition('mio_mailer', ... );
        $container->setDefinition('mio_formattatore_mail', ... );
        $container->setDefinition('gestore_mail', new Definition(
            '%gestore_mail.class%'
        ))->SetAbstract(
            true
        )->addMethodCall('setMailer', array(
            new Reference('mio_mailer')
        ))->addMethodCall('setFormattatoreMail', array(
            new Reference('mio_formattatore_mail')
        ));
        $container->setDefinition('gestore_newsletter', new DefinitionDecorator(
            'gestore_mail'
        ))->setClass(
            '%gestore_newsletter.class%'
        );
        $container->setDefinition('gestore_cartoline', new DefinitionDecorator(
            'gestore_mail'
        ))->setClass(
            '%gestore_cartoline.class%'
        );

In questo contesto, avere un servizio ``padre`` implica che gli argomenti e le
chiamate dei metodi del servizio padre dovrebbero essere utilizzati per i servizi figli.
Nello specifico, i metodi setter definiti nel servizio padre verranno chiamati
quando i servizi figli saranno istanziati.

.. note::

   Rimuovendo la chiave di configurazione ``parent`` i servizi verranno comunque istanziati
   e estenderanno comunque la classe ``GestoreMail``. La differenza è che,
   ommettendo la chiave di configurazione ``parent``, le ``chiamate`` definite nel
   servizio ``gestore_mail non saranno eseguite quando i servizi figli
   saranno istanziati.

La classe padre è astratta e dovrebbe essere istanziata direttamente. Configurandola
come astratta nel file di configurazione, così come è stato fatto precedentemente, implica
che potrà essere usata come servizio padre e che non potrà essere utilizzata direttamente
come servizio da iniettare e verrà rimossa in fase di compilazione. In altre parole, esisterà
semplicemente come un "template" che altri servizi potranno usare.

Override delle dipendenze della classe padre
--------------------------------------------

Potrebbe succedere che sia preferibile fare l'override della classe passata
come dipendenza di un servizio figlio. Fortunatamente, aggiungendo la configurazione
della chiamata al metodo per il servizio figlio, le dipendenze configurate nella
classe padre verranno sostituite. Perciò, nel caso si volesse passare una dipendenza diversa
solo per la classe ``GestoreNewsletter``, la configurazione sarà simile alla seguente:

.. configuration-block::

    .. code-block:: yaml

        # src/Acme/HelloBundle/Resources/config/services.yml
        parameters:
            # ...
            gestore_newsletter.class: Acme\HelloBundle\Mail\GestoreNewsletter
            gestore_cartoline.class: Acme\HelloBundle\Mail\GestoreCartoline
            gestore_mail.class: Acme\HelloBundle\Mail\GestoreMail
        services:
            mio_mailer:
                # ...
            mio_mailer_alternativo:
                # ...
            mio_formattatore_mail:
                # ...
            gestore_mail:
                class:     %gestore_mail.class%
                abstract:  true
                calls:
                    - [ setMailer, [ @mio_mailer ] ]
                    - [ setFormattatoreMail, [ @mio_formattatore_mail] ]
            
            gestore_newsletter:
                class:     %gestore_newsletter.class%
                parent: gestore_mail
                calls:
                    - [ setMailer, [ @mio_mailer_alternativo ] ]
            
            gestore_cartoline:
                class:     %gestore_cartoline.class%
                parent: gestore_mail
            
    .. code-block:: xml

        <!-- src/Acme/HelloBundle/Resources/config/services.xml -->
        <parameters>
            <!-- ... -->
            <parameter key="gestore_newsletter.class">Acme\HelloBundle\Mail\GestoreNewsletter</parameter>
            <parameter key="gestore_cartoline.class">Acme\HelloBundle\Mail\GestoreCartoline</parameter>
            <parameter key="gestore_mail.class">Acme\HelloBundle\Mail\GestoreMail</parameter>
        </parameters>

        <services>
            <service id="mio_mailer" ... >
              <!-- ... -->
            </service>
            <service id="mio_mailer_alternativo" ... >
              <!-- ... -->
            </service>
            <service id="mio_formattatore_mail" ... >
              <!-- ... -->
            </service>
            <service id="gestore_mail" class="%gestore_mail.class%" abstract="true">
                <call method="setMailer">
                     <argument type="service" id="mio_mailer" />
                </call>
                <call method="setFormattatoreMail">
                     <argument type="service" id="mio_formattatore_mail" />
                </call>
            </service>
            <service id="gestore_newsletter" class="%gestore_newsletter.class%" parent="gestore_mail">
                 <call method="setMailer">
                     <argument type="service" id="mio_mailer_alternativo" />
                </call>
            </service>
            <service id="gestore_cartoline" class="%gestore_cartoline.class%" parent="gestore_mail"/>
        </services>

    .. code-block:: php

        // src/Acme/HelloBundle/Resources/config/services.php
        use Symfony\Component\DependencyInjection\Definition;
        use Symfony\Component\DependencyInjection\Reference;

        // ...
        $container->setParameter('gestore_newsletter.class', 'Acme\HelloBundle\Mail\GestoreNewsletter');
        $container->setParameter('gestore_cartoline.class', 'Acme\HelloBundle\Mail\GestoreCartoline');
        $container->setParameter('gestore_mail.class', 'Acme\HelloBundle\Mail\GestoreMail');

        $container->setDefinition('mio_mailer', ... );
        $container->setDefinition('mio_mailer_alternativo', ... );
        $container->setDefinition('mio_formattatore_mail', ... );
        $container->setDefinition('gestore_mail', new Definition(
            '%gestore_mail.class%'
        ))->SetAbstract(
            true
        )->addMethodCall('setMailer', array(
            new Reference('mio_mailer')
        ))->addMethodCall('setFormattatoreMail', array(
            new Reference('mio_formattatore_mail')
        ));
        $container->setDefinition('gestore_newsletter', new DefinitionDecorator(
            'gestore_mail'
        ))->setClass(
            '%gestore_newsletter.class%'
        )->addMethodCall('setMailer', array(
            new Reference('mio_mailer_alternativo')
        ));
        $container->setDefinition('gestore_newsletter', new DefinitionDecorator(
            'gestore_mail'
        ))->setClass(
            '%gestore_cartoline.class%'
        );

Il ``GestoreCartoline`` riceverà le stesse dipendenze di prima mentre al 
``GestoreNewsletter`` verrà passato il ``mio_mailer_alternativo``
invece del servizio ``mio_mailer``.

Collezioni di dipendenze
------------------------

È da notare che il metodo setter di cui si è fatto l'override nel precedente esempio
viene chiamato due volte:: una volta nella definizione del padre e una nella
definizione del figlio. Nel precedente esempio, la cosa va bene, visto che la chiamata
al secondo ``setMailer`` sostituisce l'oggetto mailer configurato nella prima chiamata.

In alcuni casi, però, questo potrebbe creare problemi. Ad esempio, nel caso in cui
il metodo per cui si fa l'override dovesse aggiungere qualcosa ad una collezione, 
potrebbero essere aggiunti due oggetti alla collezione. Di seguito se ne può vedere
un esempio::

    namespace Acme\HelloBundle\Mail;

    use Acme\HelloBundle\Mailer;
    use Acme\HelloBundle\EmailFormatter;

    abstract class GestoreMail
    {
        protected $filtri;

        public function setFiltro($filtro)
        {
            $this->filtri[] = $filtro;
        }
        // ...
    }

Se si avesse la seguente configurazione:

.. configuration-block::

    .. code-block:: yaml

        # src/Acme/HelloBundle/Resources/config/services.yml
        parameters:
            # ...
            gestore_newsletter.class: Acme\HelloBundle\Mail\GestoreNewsletter
            gestore_mail.class: Acme\HelloBundle\Mail\GestoreMail
        services:
            mio_filtro:
                # ...
            altro_filtro:
                # ...
            gestore_mail:
                class:     %gestore_mail.class%
                abstract:  true
                calls:
                    - [ setFiltro, [ @mio_filtro ] ]
                    
            gestore_newsletter:
                class:     %gestore_newsletter.class%
                parent: gestore_mail
                calls:
                    - [ setFiltro, [ @altro_filtro ] ]
            
    .. code-block:: xml

        <!-- src/Acme/HelloBundle/Resources/config/services.xml -->
        <parameters>
            <!-- ... -->
            <parameter key="gestore_newsletter.class">Acme\HelloBundle\Mail\GestoreNewsletter</parameter>
            <parameter key="gestore_mail.class">Acme\HelloBundle\Mail\GestoreMail</parameter>
        </parameters>

        <services>
            <service id="mio_filtro" ... >
              <!-- ... -->
            </service>
            <service id="altro_filtro" ... >
              <!-- ... -->
            </service>
            <service id="gestore_mail" class="%gestore_mail.class%" abstract="true">
                <call method="setFiltro">
                     <argument type="service" id="mio_filtro" />
                </call>
            </service>
            <service id="gestore_newsletter" class="%gestore_newsletter.class%" parent="gestore_mail">
                 <call method="setFiltro">
                     <argument type="service" id="altro_filtro" />
                </call>
            </service>
        </services>

    .. code-block:: php

        // src/Acme/HelloBundle/Resources/config/services.php
        use Symfony\Component\DependencyInjection\Definition;
        use Symfony\Component\DependencyInjection\Reference;

        // ...
        $container->setParameter('gestore_newsletter.class', 'Acme\HelloBundle\Mail\GestoreNewsletter');
        $container->setParameter('gestore_mail.class', 'Acme\HelloBundle\Mail\GestoreMail');

        $container->setDefinition('mio_filtro', ... );
        $container->setDefinition('altro_filtro', ... );
        $container->setDefinition('gestore_mail', new Definition(
            '%gestore_mail.class%'
        ))->SetAbstract(
            true
        )->addMethodCall('setFiltro', array(
            new Reference('mio_filtro')
        ));
        $container->setDefinition('gestore_newsletter', new DefinitionDecorator(
            'gestore_mail'
        ))->setClass(
            '%gestore_newsletter.class%'
        )->addMethodCall('setFiltro', array(
            new Reference('altro_filtro')
        ));

In questo caso il metodo ``setFiltro`` del servizio ``gestore_newsletter`` 
verrebbe chiamato due volte cosa che produrrà, come risultato che l'array ``$filtri``
conterrà sia l'oggetto ``mio_filtro`` che l'oggetto ``altro_filtro``. Il che va bene
se l'obbiettivo è quello di avere più filtri nella sotto classe. Ma se si volesse sostituire
il filtro passato alla sotto classe, la rimozione della configurazione della classe
padre eviterà che la classe base chiami il metodo ``setFiltro``.
