.. index::
   single: DependencyInjection; Scope

Lavorare con gli scope
======================

Questa ricetta parla di scope, un argomento alquanto avanzato, relativo al
:doc:`/book/service_container`. Se si ottiene un errore che menziona gli
"scope" durante la creazione di servizi oppure se si ha l'esigenza di creare un
servizio che dipenda dal servizio ``request``, questa è la ricetta giusta.

Capire gli scope
----------------

Lo scope di un servizio controlla quanto a lungo un'istanza di un servizio è usata
dal contenitore. Il componente Dependency Injection fornisce due scope
generici:

- ``container`` (quello predefinito): la stessa istanza è usata ogni volta che la si
  richiede da questo contenitore.

- ``prototype``: viene creata una nuova istanza, ogni volta che si richiede il servizio.

La classe
:class:`Symfony\\Component\\HttpKernel\\DependencyInjection\\ContainerAwareHttpKernel`
definisce anche un terzo scope: ``request``. Questo scope è legato alla richiesta,
il che vuol dire che viene creata una nuova istanza per ogni sotto-richiesta, non
disponibile al di fuori della richiesta stessa (per esempio nella CLI).

Gli scope aggiungono un vincolo sulle dipendenze di un servizio: un servizio non può
dipendere da servizi con scope più stretti. Per esempio, se si crea un generico servizio
``pippo``, ma si prova a iniettare il componente ``request``, si riceverà una
:class:`Symfony\\Component\\DependencyInjection\\Exception\\ScopeWideningInjectionException`
alla compilazione del contenitore. Leggere la nota seguente sotto per maggiori dettagli.

.. sidebar:: Scope e dipendenze

    Si immagini di aver configurato un servizio ``posta``. Non è stato configurato
    lo scope del servizio, quindi ha ``container``. In altre parole, ogni volta che si
    chiede al contenitore il servizio ``posta``, si ottiene lo stesso
    oggetto. Solitamente, si vuole che un servizio funzioni in questo modo.

    Si immagini, tuttavia, di aver bisogno del servizio ``request`` da dentro ``posta``,
    magari perché si deve leggere l'URL della richiesta corrente.
    Lo si aggiunge quindi come parametro del costruttore. Vediamo quali problemi si
    presentano:

    * Alla richiesta di `posta`, viene creata un'istanza di ``posta`` (chiamiamola
      *PostaA*), a cui viene passato il servizio `request` (chiamiamolo
      *RequestA*). Fin qui tutto bene.

    * Si effettua ora una sotto-richiesta in Symfony, che è un modo carino per dire che
      è stata chiamata, per esempio, la funzione ``{{ render(...) }}`` di Twig,
      che esegue un altro controllore. Internamente, il vecchio servizio ``request``
      (*RequestA*) viene effettivamente sostituito da una nuova istanza di richiesta
      (*RequestB*). Questo avviene in background ed è del tutto normale.

    * Nel proprio controllore incluso, si richiede nuovamente il servizio ``posta``.
      Poiché il servizio è nello scope `container` scope, viene riutilizzata
      la stessa istanza (*PostaA*). Ma ecco il problema: l'istanza *PostaA* contiene
      ancora il vecchio oggetto *RequestA*, che **non** è ora l'oggetto di richiesta
      corretto da avere (attualmente *RequestB* è il servizio `request`). La differenza
      è sottile, ma questa mancata corrispondenza potrebbe causare grossi guai, per
      questo non è consentita.

      Questa è dunque la ragione *per cui* esistono gli scope e come possono causare
      problemi. Vedremo più avanti delle soluzioni comuni.

.. note::

    Ovviamente, un servizio può dipendere senza alcun problema da un altro servizio
    che abbia uno scope più ampio, . 

Usare un servizio da uno scope più limitato
-------------------------------------------

Se un servizio ha una dipendenza da un servizio con scope (come ``request``),
si hanno tre possibili opzioni:

* Usare l'iniezione tramite setter, se la dipendenza è "sincronizzata"; questa è
  l'opzione consigliata e la soluzione migliore per l'istanza ``request``, perché è
  sincronizzata con lo scope ``request`` (vedere
  :ref:`using-synchronized-service`).

* Inserire il servizio nello stesso scope della dipendenza (o in uno più limitato). Se
  si dipende dal servizio ``request``, questo vuol dire inserire il nuovo servizio
  nello scope ``request`` (vedere :ref:`changing-service-scope`);

* Passare l'intero contenitore al servizio e recuperare la dipendenza dal
  contenitore, ogni volta che occorre, per assicurarsi di avere l'istanza giusta:
  il servizio può trovarsi nello scope predefinito ``container`` (vedere
  :ref:`passing-container`);

Ciascuno scenario è analizzato in dettaglio nelle sezioni seguenti.

.. _using-synchronized-service:

Usare un servizio sincronizzato
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

.. versionadded:: 2.3
    I servizi sincronizzati sono nuovi in Symfony 2.3.

Iniettare il contenitore o impostare un servizio a uno scopo più limitato hanno
dei contro. Per i servizi sincronizzati (come ``request``), usare l'iniezione tramite
setter è l'opzione migliore, perché non ha controindicazioni e tutto funziona
senza aggiungere codice al servizio o alla definizione::

    // src/Acme/HelloBundle/Mail/Mailer.php
    namespace Acme\HelloBundle\Mail;

    use Symfony\Component\HttpFoundation\Request;

    class Mailer
    {
        protected $request;

        public function setRequest(Request $request = null)
        {
            $this->request = $request;
        }

        public function sendEmail()
        {
            if (null === $this->request) {
                // lanciare un errore?
            }

            // ... fare qualcosa con la richiesta
        }
    }

Ogni volta che entra o esce dallo scope ``request``, il contenitore
richiamerà automaticamente il metodo ``setRequest()`` con l'istanza di ``request``
corrente.

Si può notare che il metodo ``setRequest()`` accetta anche ``null`` come
valore valido per il parametro ``request``. Questo perché, uscendo dallo scope
``request``, l'istanza di ``request`` può essere ``null`` (per esempio, per
la richiesta principale). Ovviamente, bisogna tener conto di questa possibilità
all'interno del codice. Occorre tenerne conto anche nella dichiarazione del servizio:

.. configuration-block::

    .. code-block:: yaml

        # src/Acme/HelloBundle/Resources/config/services.yml
        services:
            greeting_card_manager:
                class: Acme\HelloBundle\Mail\GreetingCardManager
                calls:
                    - [setRequest, ["@?request="]]

    .. code-block:: xml

        <!-- src/Acme/HelloBundle/Resources/config/services.xml -->
        <services>
            <service id="greeting_card_manager"
                class="Acme\HelloBundle\Mail\GreetingCardManager"
            >
                <call method="setRequest">
                    <argument type="service" id="request" on-invalid="null" strict="false" />
                </call>
            </service>
        </services>

    .. code-block:: php

        // src/Acme/HelloBundle/Resources/config/services.php
        use Symfony\Component\DependencyInjection\Definition;
        use Symfony\Component\DependencyInjection\ContainerInterface;

        $definition = $container->setDefinition(
            'greeting_card_manager',
            new Definition('Acme\HelloBundle\Mail\GreetingCardManager')
        )
        ->addMethodCall('setRequest', array(
            new Reference('request', ContainerInterface::NULL_ON_INVALID_REFERENCE, false)
        ));

.. tip::

    Si possono dichiarare servizi ``synchronized`` Molto facilmente. Ecco
    la dichiarazione del servizio ``request``, come riferimento:

    .. configuration-block::

        .. code-block:: yaml

            services:
                request:
                    scope: request
                    synthetic: true
                    synchronized: true

        .. code-block:: xml

            <services>
                <service id="request" scope="request" synthetic="true" synchronized="true" />
            </services>

        .. code-block:: php

            use Symfony\Component\DependencyInjection\Definition;
            use Symfony\Component\DependencyInjection\ContainerInterface;

            $definition = $container->setDefinition('request')
                ->setScope('request')
                ->setSynthetic(true)
                ->setSynchronized(true);

.. caution::

    Il servizio che usa il servizio sincronizzato deve essere pubblico, per far sì
    che il suo setter sia richiamato al cambio di scope.

.. _changing-service-scope:

Cambiare lo scope del servizio
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Lo scope di un servizio può essere modificato nella definizione del servizio stesso:

.. configuration-block::

    .. code-block:: yaml

        # src/Acme/HelloBundle/Resources/config/services.yml
        services:
            greeting_card_manager:
                class: Acme\HelloBundle\Mail\GreetingCardManager
                scope: request
                arguments: ["@request"]

    .. code-block:: xml

        <!-- src/Acme/HelloBundle/Resources/config/services.xml -->
        <services>
            <service id="greeting_card_manager"
                    class="Acme\HelloBundle\Mail\GreetingCardManager"
                    scope="request">
                <argument type="service" id="request" />
            </service>
        </services>

    .. code-block:: php

        // src/Acme/HelloBundle/Resources/config/services.php
        use Symfony\Component\DependencyInjection\Definition;

        $definition = $container->setDefinition(
            'greeting_card_manager',
            new Definition(
                'Acme\HelloBundle\Mail\GreetingCardManager',
                array(new Reference('request'),
            ))
        )->setScope('request');

.. _passing-container:

Passare il contenitore al servizio
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Impostare uno scope più limitato non è sempre possibile (per esempio,
un'estensione di Twig deve stare nello scope ``container``, perché l'ambiente di Twig
ne ha bisogno per le sue dipendenze). In questi casi, si dovrebbe passare l'intero contenitore
dentro al servizio::

    // src/Acme/HelloBundle/Mail/Mailer.php
    namespace Acme\HelloBundle\Mail;

    use Symfony\Component\DependencyInjection\ContainerInterface;

    class Mailer
    {
        protected $container;

        public function __construct(ContainerInterface $container)
        {
            $this->container = $container;
        }

        public function sendEmail()
        {
            $request = $this->container->get('request');
            // ... fare qualcosa con la richiesta in questo punto
        }
    }

.. caution::

    Si faccia attenzione a non memorizzare la richiesta in una proprietà dell'oggetto
    per una chiamata futura del servizio, perché causerebbe lo stesso problema spiegato
    nella prima sezione (tranne per il fatto che Symfony non è in grado di individuare
    l'errore).

La configurazione del servizio per questa classe assomiglia a questa:

.. configuration-block::

    .. code-block:: yaml

        # src/Acme/HelloBundle/Resources/config/services.yml
        services:
            my_mailer:
                class:     Acme\HelloBundle\Mail\Mailer
                arguments: ["@service_container"]
                # scope: container può essere omesso, essendo il valore predefinito

    .. code-block:: xml

        <!-- src/Acme/HelloBundle/Resources/config/services.xml -->
        <services>
            <service id="my_mailer" class="Acme\HelloBundle\Mail\Mailer">
                 <argument type="service" id="service_container" />
            </service>
        </services>

    .. code-block:: php

        // src/Acme/HelloBundle/Resources/config/services.php
        use Symfony\Component\DependencyInjection\Definition;
        use Symfony\Component\DependencyInjection\Reference;

        $container->setDefinition('my_mailer', new Definition(
            'Acme\HelloBundle\Mail\Mailer',
            array(new Reference('service_container'))
        ));

.. note::

    Iniettare l'intero contenitore in un servizio di solito non è una buona
    idea (è meglio iniettare solo ciò che serve).

.. tip::

    Se si definisce un controllore come servizio, si può ottenere l'oggetto ``Request``
    senza iniettare il contenitore, facendoselo passare come parametro nel metodo
    dell'azione. Vedere
    :ref:`book-controller-request-argument` per maggiori dettagli.
