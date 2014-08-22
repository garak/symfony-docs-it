.. index::
   single: Dependency Injection; Scope

Lavorare con gli scope
======================

Questa ricetta parla di scope, un argomento alquanto avanzato, relativo al
:doc:`/book/service_container`. Se si ottiene un errore che menziona gli
"scopes" durante la creazione di servizi, questa è la ricetta giusta.

.. note::

    Se si prova a iniettare il servizio ``request``, la soluzione più semplice
    è iniettare invece il servizio ``request_stack`` e accedere alla
    richiesta richiamando il metodo
    :method:`Symfony\\Component\\HttpFoundation\\RequestStack::getCurrentRequest`
    (vedere :ref:`book-container-request-stack`). Il resto di questa ricetta
    parla di scope in modo più teorico e avanzato. Se si ha a che fare
    con gli scope per il servizio ``request``, basta iniettare ``request_stack``.

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

Un esempio: lo scope client
~~~~~~~~~~~~~~~~~~~~~~~~~~~

Oltre al servizio ``request`` (che ha una soluzione semplice, vedere la nota
precedente), nessun servizio nel contenitore predefinito di Symfony appartiene a
scope diversi da ``container`` e ``prototype``. Tuttavia, ai fini di questa
ricetta, si immagini che esiste un altro scope, chiamato ``client``, e un servizio ``client_configuration``,
che gli appartenga. Questa non è una situazione comune, ma l'idea è che si
possa entrare e uscire da molteplici scope ``client`` durante una richiesta, ciascuno dei quali
abbia il suo servizio ``client_configuration``.

Gli scope aggiungono un vincolo sulle dipendenze di un servizio: un servizio non può
dipendere da servizi con scope più stretti. Per esempio, se si crea un generico servizio
``pippo``, ma si prova a iniettare il servizio ``client_configuration``, si riceverà
una
:class:`Symfony\\Component\\DependencyInjection\\Exception\\ScopeWideningInjectionException`
alla compilazione del contenitore. Leggere la nota seguente sotto per maggiori dettagli.

.. sidebar:: Scope e dipendenze

    Si immagini di aver configurato un servizio ``posta``. Non è stato configurato
    lo scope del servizio, quindi ha ``container``. In altre parole, ogni volta che si
    chiede al contenitore il servizio ``posta``, si ottiene lo stesso
    oggetto. Solitamente, si vuole che un servizio funzioni in questo modo.

    Si immagini, tuttavia, di aver bisogno del servizio `request` da dentro `posta`,
    magari perché si deve leggere l'URL della richiesta corrente.
    Lo si aggiunge quindi come parametro del costruttore. Vediamo quali problemi si
    presentano:

    * Alla richiesta di ``posta``, viene creata un'istanza di ``post`a` (chiamiamola
      *PostaA*), a cui viene passato il servizio ``client_configuration`` (chiamiamolo
      *ConfigurationA*). Fin qui tutto bene.

    * L'applicazione ora ha bisogno di un altro client, ed è 
      stata architettata in modo da gestire questa esigenza
      entrando in uno nuovo scope ``client_configuration`` e impostando un nuovo servizio
      ``client_configuration`` nel contenitore. Chiamamolo
      *ConfigurationB*.

    * Da qualche parte nell'applicazione, si richiede nuovamente il servizio ``posta``.
      Poiché il servizio è nello scope ``container``, viene riutilizzata
      la stessa istanza (*PostaA*). Ma ecco il problema: l'istanza
      *PostaA* contiene ancora il vecchio oggetto *ConfigurationA*,
      che **non** è ora l'oggetto di configurazione corretto da avere
      (attualmente *ConfigurationB* è il servizio ``client_configuration``). La differenza
      è sottile, ma questa mancata corrispondenza potrebbe causare grossi guai, per
      questo non è consentita.

      Questa è dunque la ragione *per cui* esistono gli scope e come possono causare
      problemi. Vedremo più avanti delle soluzioni comuni.

.. note::

    Ovviamente, un servizio può dipendere senza alcun problema da un altro servizio
    che abbia uno scope più ampio, . 

Usare un servizio da uno scope più limitato
-------------------------------------------

Ci sono tre possibili opzioni alla questione degli scope:

* A) Usare l'iniezione tramite setter, se la dipendenza è sincronizzata (vedere
  :ref:`using-synchronized-service`);

* B) Inserire il servizio nello stesso scope della dipendenza (o in uno più limitatato). Se
  si dipende dal servizio ``client_configuration``, questo vuol dire inserire il nuovo servizio
  nello scope ``client`` (vedere :ref:`changing-service-scope`);

* C) Passare l'intero contenitore al servizio e recuperare la dipendenza dal
  contenitore, ogni volta che occorre, per assicurarsi di avere l'istanza giusta:
  il servizio può trovarsi nello scope predefinito ``container`` (vedere
  :ref:`passing-container`);

Ciascuno scenario è analizzato in dettaglio nelle sezioni seguenti.

.. _using-synchronized-service:

A) Usare un servizio sincronizzato
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

.. versionadded:: 2.3
    I servizi sincronizzati sono nuovi in Symfony 2.3.

Iniettare il contenitore o impostare un servizio a uno scopo più limitato hanno
dei contro. Ipotizziamo prima che il servizio ``client_configuration`` sia stato
segnato come ``synchronized``:

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        services:
            client_configuration:
                class:        Acme\HelloBundle\Client\ClientConfiguration
                scope:        client
                synchronized: true
                synthetic:    true
                # ...

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <?xml version="1.0" encoding="UTF-8" ?>
        <container xmlns="http://symfony.com/schema/dic/services"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://symfony.com/schema/dic/services
                http://symfony.com/schema/dic/services/services-1.0.xsd"
            >

            <services>
                <service
                    id="client_configuration"
                    scope="client"
                    synchronized="true"
                    synthetic="true"
                    class="Acme\HelloBundle\Client\ClientConfiguration"
                />
            </services>
        </container>

    .. code-block:: php

        // app/config/config.php
        use Symfony\Component\DependencyInjection\Definition;

        $definition = new Definition(
            'Acme\HelloBundle\Client\ClientConfiguration',
            array()
        );
        $definition->setScope('client');
        $definition->setSynchronized(true);
        $definition->setSynthetic(true);
        $container->setDefinition('client_configuration', $definition);

Se ora si inietta questo servizio tramite setter, non ci sono contro
e tutto funzionerà, senza dover aggiungere codice nel servizio o nella definizione::

    // src/Acme/HelloBundle/Mail/Mailer.php
    namespace Acme\HelloBundle\Mail;

    use Acme\HelloBundle\Client\ClientConfiguration;

    class Mailer
    {
        protected $clientConfiguration;

        public function setClientConfiguration(ClientConfiguration $clientConfiguration = null)
        {
            $this->clientConfiguration = $clientConfiguration;
        }

        public function sendEmail()
        {
            if (null === $this->clientConfiguration) {
                // throw an error?
            }

            // ... fare qualcosa con la configurazione del client
        }
    }

Ogni volta che entra o esce dallo scope ``request``, il contenitore
richiamerà automaticamente il metodo ``setRequest()`` con l'istanza di ``request``
corrente.

Si può notare che il metodo ``setClientConfiguration()`` accetta anche
``null`` come valore valido per il parametro ``client_configuration``. Questo
perché, uscendo dallo scope ``client``, l'istanza di ``client_configuration``
può essere ``null``. Ovviamente, bisogna tener conto di questa possibilità
all'interno del codice. Occorre tenerne conto anche nella dichiarazione del servizio:

.. configuration-block::

    .. code-block:: yaml

        # src/Acme/HelloBundle/Resources/config/services.yml
        services:
            my_mailer:
                class: Acme\HelloBundle\Mail\Mailer
                calls:
                    - [setClientConfiguration, ["@?client_configuration="]]

    .. code-block:: xml

        <!-- src/Acme/HelloBundle/Resources/config/services.xml -->
        <services>
            <service id="my_mailer"
                class="Acme\HelloBundle\Mail\Mailer"
            >
                <call method="setClientConfiguration">
                    <argument
                        type="service"
                        id="client_configuration"
                        on-invalid="null"
                        strict="false"
                    />
                </call>
            </service>
        </services>

    .. code-block:: php

        // src/Acme/HelloBundle/Resources/config/services.php
        use Symfony\Component\DependencyInjection\Definition;
        use Symfony\Component\DependencyInjection\ContainerInterface;

        $definition = $container->setDefinition(
            'my_mailer',
            new Definition('Acme\HelloBundle\Mail\Mailer')
        )
        ->addMethodCall('setClientConfiguration', array(
            new Reference(
                'client_configuration',
                ContainerInterface::NULL_ON_INVALID_REFERENCE,
                false
            )
        ));

.. _changing-service-scope:

B) Cambiare lo scope del servizio
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Lo scope di un servizio può essere modificato nella definizione del servizio stesso. Questo esempio
ipotizza che la classe ``Mailer`` abbia un metodo ``__construct``, il cui primo
parametro sia l'oggetto ``ClientConfiguration``:

.. configuration-block::

    .. code-block:: yaml

        # src/Acme/HelloBundle/Resources/config/services.yml
        services:
            my_mailer:
                class: Acme\HelloBundle\Mail\Mailer
                scope: client
                arguments: ["@client_configuration"]

    .. code-block:: xml

        <!-- src/Acme/HelloBundle/Resources/config/services.xml -->
        <services>
            <service id="my_mailer"
                    class="Acme\HelloBundle\Mail\Mailer"
                    scope="client">
                    <argument type="service" id="client_configuration" />
            </service>
        </services>

    .. code-block:: php

        // src/Acme/HelloBundle/Resources/config/services.php
        use Symfony\Component\DependencyInjection\Definition;

        $definition = $container->setDefinition(
            'my_mailer',
            new Definition(
                'Acme\HelloBundle\Mail\Mailer',
                array(new Reference('client_configuration'),
            ))
        )->setScope('client');

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
            $request = $this->container->get('client_configuration');
            // Fare qualcosa con la configurazione del client
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
        parameters:
            # ...
            my_mailer.class: Acme\HelloBundle\Mail\Mailer

        services:
            my_mailer:
                class:     "%my_mailer.class%"
                arguments: ["@service_container"]
                # scope: container può essere omesso, essendo il valore predefinito

    .. code-block:: xml

        <!-- src/Acme/HelloBundle/Resources/config/services.xml -->
        <parameters>
            <!-- ... -->
            <parameter key="my_mailer.class">Acme\HelloBundle\Mail\Mailer</parameter>
        </parameters>

        <services>
            <service id="my_mailer" class="%my_mailer.class%">
                 <argument type="service" id="service_container" />
            </service>
        </services>

    .. code-block:: php

        // src/Acme/HelloBundle/Resources/config/services.php
        use Symfony\Component\DependencyInjection\Definition;
        use Symfony\Component\DependencyInjection\Reference;

        // ...
        $container->setParameter('my_mailer.class', 'Acme\HelloBundle\Mail\Mailer');

        $container->setDefinition('my_mailer', new Definition(
            '%my_mailer.class%',
            array(new Reference('service_container'))
        ));

.. note::

    Iniettare l'intero contenitore in un servizio di solito non è una buona
    idea (è meglio iniettare solo ciò che serve).
