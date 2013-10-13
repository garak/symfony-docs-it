.. index::
   single: Dependency Injection; Tipi di iniezione

Tipi di iniezione
=================

Esplicitare le dipendenze di una classe e richiedere che vi siano iniettate
è un buono modo per rendere una classe più usabile, testabile e disaccoppiata
da altre.

Ci sono molti modi per iniettare le dipendenze. Ogni tipo di iniezione ha
vantaggi e svantaggi da considerare, così come diversi modi di
funzionare nell'uso con il contenitore di servizi.

Iniezione nel costruttore
-------------------------

Il modo più comune per iniettare dipendenze è tramite il costruttore di una classe.
Per poterlo fare, occorre aggiungere un parametro alla firma del costruttore, in modo
che accetti la dipendenza::

    class NewsletterManager
    {
        protected $mailer;

        public function __construct(\Mailer $mailer)
        {
            $this->mailer = $mailer;
        }

        // ...
    }

Si può specificare quale servizio si vuole iniettare, tramite la configurazione
del contenitore di servizi:

.. configuration-block::

    .. code-block:: yaml

       services:
            mio_mailer:
                # ...
            newsletter_manager:
                class:     NewsletterManager
                arguments: ["@mio_mailer"]

    .. code-block:: xml

        <services>
            <service id="mio_mailer" ... >
              <!-- ... -->
            </service>
            <service id="newsletter_manager" class="NewsletterManager">
                <argument type="service" id="mio_mailer"/>
            </service>
        </services>

    .. code-block:: php

        use Symfony\Component\DependencyInjection\Definition;
        use Symfony\Component\DependencyInjection\Reference;

        // ...
        $container->setDefinition('mio_mailer', ...);
        $container->setDefinition('newsletter_manager', new Definition(
            'NewsletterManager',
            array(new Reference('mio_mailer'))
        ));

.. tip::

    Forzare il tipo di un oggetto iniettato implica la certezza che venga iniettata
    una dipendenza effettivamente utilizzabile. Forzando il tipo, si otterrà subito
    un errore chiaro, nel caso in cui venga iniettata una dipendenza inadatta. Forzando
    il tipo con un'interfaccia invece che con una classe, si può rendere più
    flessibile la scelta della dipendenza. Ipotizzando di usare solo i metodi definiti
    nell'interfaccia, si può guadagnare flessibilità e mantenere la sicurezza.

Ci sono diversi vantaggi nell'uso dell'iniezione nel costruttore:

* Se la dipendenza è un requisito e la classe non funziona senza di essa,
  l'iniezione tramite il costruttore assicura che sia presente all'uso della
  classe, poiché la classe non può essere istanziata senza. 

* Il costruttore è richiamato solo una volta, alla creazione dell'oggetto, quindi si
  può essere sicuri che la dipendenza non cambierà durante il ciclo di vita dell'oggetto.

Tali vantaggi implicano che l'iniezione nel costruttore non è adatta per le dipendenze
facoltative. Inoltre è più difficile da usare in combinazione con le gerarchie di
classi: se una classe usa l'iniezione nel costruttore, estenderla e sovrascrivere il
costruttore diventa problematico.

Iniezione da setter
-------------------

Un altro possibile punto di iniezione in una classe è l'aggiunta di un metodo setter,
che accetti la dipendenza::

    class NewsletterManager
    {
        protected $mailer;

        public function setMailer(\Mailer $mailer)
        {
            $this->mailer = $mailer;
        }

        // ...
    }

.. configuration-block::

    .. code-block:: yaml

       services:
            mio_mailer:
                # ...
            newsletter_manager:
                class:     NewsletterManager
                calls:
                    - [setMailer, ["@mio_mailer"]]

    .. code-block:: xml

        <services>
            <service id="mio_mailer" ... >
              <!-- ... -->
            </service>
            <service id="newsletter_manager" class="NewsletterManager">
                <call method="setMailer">
                     <argument type="service" id="mio_mailer" />
                </call>
            </service>
        </services>

    .. code-block:: php

        use Symfony\Component\DependencyInjection\Definition;
        use Symfony\Component\DependencyInjection\Reference;

        // ...
        $container->setDefinition('mio_mailer', ...);
        $container->setDefinition('newsletter_manager', new Definition(
            'NewsletterManager'
        ))->addMethodCall('setMailer', array(new Reference('mio_mailer')));

Questa volta i vantaggi sono:

* l'iniezione da setter funziona bene con le dipendenza facoltative. Se non si ha bisogno
  della dipendenza, basta non richiamare il setter.

* Si può richiamare il setter più volte. Questo è particolarmente utile se il metodo
  aggiunge la dipendenza a un insieme. Si può quindi avere un numero variabile di
  dipendenze.

Gli svantaggi dell'iniezione da setter sono:

* Il setter può essere richiamato più volte, non solo all'istanza dell'oggetto, quindi
  non si può essere sicuri che la dipendenza non sia rimpiazzata durante il ciclo di vita
  dell'oggetto (tranne se si scrive esplicitamente il metodo setter per verificare se non
  sia stato già richiamato).

* Non si può essere sicuri che il setter sia richiamato, quindi occorre verificare che
  ogni dipendenza obbligatoria sia iniettata.

Iniezione di proprietà
----------------------

Un'altra possibilità consiste nell'impostare direttamente campi pubblici della classe::

    class NewsletterManager
    {
        public $mailer;

        // ...
    }

.. configuration-block::

    .. code-block:: yaml

       services:
            mio_mailer:
                # ...
            newsletter_manager:
                class:     NewsletterManager
                properties:
                    mailer: "@mio_mailer"

    .. code-block:: xml

        <services>
            <service id="mio_mailer" ... >
              <!-- ... -->
            </service>
            <service id="newsletter_manager" class="NewsletterManager">
                <property name="mailer" type="service" id="mio_mailer" />
            </service>
        </services>

    .. code-block:: php

        use Symfony\Component\DependencyInjection\Definition;
        use Symfony\Component\DependencyInjection\Reference;

        // ...
        $container->setDefinition('mio_mailer', ...);
        $container->setDefinition('newsletter_manager', new Definition(
            'NewsletterManager'
        ))->setProperty('mailer', new Reference('mio_mailer')));

Ci sono principalmente solo svantaggi nell'uso dell'iniezione di proprietà, che è
simile a quella da setter, ma con importanti problemi ulteriori:

* Non si può in alcun modo controllare quando la dipendenza viene impostata, potrebbe
  essere modificata in qualsiasi punto del ciclo di vita dell'oggetto.

* Non si può forzare il tipo, quindi non si può essere sicuri di quale dipendenza sia
  iniettata, a meno di non scrivere nella classe esplicitamente di testare l'istanza
  della classe prima del suo uso.

Tuttavia, è utile conoscere questa possibilità del contenitore di servizi,
specialmente se si lavora con codice fuori dal proprio controllo, come in
librerie di terze parti, che usino proprietà pubbliche per le proprie dipendenze.
