.. index::
   single: Dependency Injection; Scope

Come lavorare con gli scope
===========================

Questa ricetta parla di scope, un argomento alquanto avanzato, relativo al
:doc:`/book/service_container`. Se si ottiene un errore che menziona gli
"scopes" durante la creazione di servizi oppure se si ha l'esigenza di creare un
servizio che dipenda dal servizio `request`, questa è la ricetta giusta.

Capure gli scope
----------------

Lo scope di un servizio controlla quanto a lungo un'istanza di un servizio è usata
dal contenitore. Il componente Dependency Injection fornisce due scope
generici:

- `container` (quello predefinito): la stessa istanza è usata ogni volta che la si
  richiede da questo contenitore.

- `prototype`: viene creata una nuova istanza, ogni volta che si richiede il servizio.

FrameworkBundle definisce anche un terzo scope: `request`. Questi scope sono legati
alla richiesta, il che vuol dire che viene creata una nuova istanza per ogni sotto-richiesta,
non disponibile al di fuori della richiesta stessa (per esempio nella CLI).

Gli scope aggiungono un vincolo sulle dipendenze di un servizio: un servizio non può
dipendere da servizi con scope più stretti. Per esempio, se si crea un generico servizio
`pippo`, ma si prova a iniettare il componente `request`, si riceverà una
:class:`Symfony\\Component\\DependencyInjection\\Exception\\ScopeWideningInjectionException`
alla compilazione del contenitore. Leggere la nota seguente sotto per maggiori dettagli.

.. sidebar:: Scope e dipendenze

    Si immagini di aver configurato un servizio `posta`. Non è stato configurato
    lo scope del servizio, quindi ha `container`. In altre parole, ogni volta che si
    chiede al contenitore il servizio `posta`, si ottiene lo stesso
    oggetto. Solitamente, si vuole che un servizio funzioni in questo modo.
    
    Si immagini, tuttavia, di aver bisogno del servizio `request` da dentro `posta`,
    magari perché si deve leggere l'URL della richiesta corrente.
    Lo si aggiunge quindi come parametro del costruttore. Vediamo quali problemi si
    presentano:

    * Alla richiesta di `posta`, viene creata un'istanza di `posta` (chiamiamola
      *PostaA*), a cui viene passato il servizio `request` (chiamiamolo
      *RequestA*). Fin qui tutto bene.

    * Si effettua ora una sotto-richiesta in Symfony, che è un modo carino per dire che
      è stata chiamata, per esempio, la funzione `{% render ... %}` di Twig,
      che esegue un altro controllore. Internamente, il vecchio servizio `request`
      (*RequestA*) viene effettivamente sostituito da una nuova istanza di richiesta
      (*RequestB*). Questo avviene in background ed è del tutto normale.

    * Nel proprio controllore incluso, si richiede nuovamente il servizio `posta`.
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

Impostare lo scope nella definizione
------------------------------------

Lo scope di un servizio è definito nella definizione del servizio stesso:

.. configuration-block::

    .. code-block:: yaml

        # src/Acme/HelloBundle/Resources/config/services.yml
        services:
            greeting_card_manager:
                class: Acme\HelloBundle\Mail\GreetingCardManager
                scope: request

    .. code-block:: xml

        <!-- src/Acme/HelloBundle/Resources/config/services.xml -->
        <services>
            <service id="greeting_card_manager" class="Acme\HelloBundle\Mail\GreetingCardManager" scope="request" />
        </services>

    .. code-block:: php

        // src/Acme/HelloBundle/Resources/config/services.php
        use Symfony\Component\DependencyInjection\Definition;

        $container->setDefinition(
            'greeting_card_manager',
            new Definition('Acme\HelloBundle\Mail\GreetingCardManager')
        )->setScope('request');

Se non si specifica lo scope, viene usato `container`, che è quello che si desidera
la maggior parte delle volte. A meno che il proprio servizio non dipenda da un altro
servizio con uno scope più stretto (solitamente, il servizio `request`),
probabilmente non si avrà bisogno di impostare lo scope.

Usare un servizio da uno scope più stretto
------------------------------------------

Se il proprio servizio dipende da un servizio con scope, la soluzione migliore è
metterlo nello stesso scope (o in uno pià stretto). Di solito, questo vuol dire
mettere il proprio servizio nello scope `request`.

Ma questo non è sempre possibile (per esempio, un'estensione di Twig deve stare nello
scope `container`, perché l'ambiente di Twig ne ha bisogno per le sue dipendenze).
In questi casi, si dovrebbe passare l'intero contenitore dentro il proprio servizio e
recuperare le proprie dipendenze dal contenitore ogni volta che servono, per assicurarsi
di avere l'istanza giusta::

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
            // Fare qualcosa con la richiesta in questo punto
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
            posta.class: Acme\HelloBundle\Mail\Mailer
        services:
            posta:
                class:     %posta.class%
                arguments:
                    - "@service_container"
                # scope: container può essere omesso, perché è il predefinito

    .. code-block:: xml

        <!-- src/Acme/HelloBundle/Resources/config/services.xml -->
        <parameters>
            <!-- ... -->
            <parameter key="posta.class">Acme\HelloBundle\Mail\Mailer</parameter>
        </parameters>

        <services>
            <service id="posta" class="%posta.class%">
                 <argument type="service" id="service_container" />
            </service>
        </services>

    .. code-block:: php

        // src/Acme/HelloBundle/Resources/config/services.php
        use Symfony\Component\DependencyInjection\Definition;
        use Symfony\Component\DependencyInjection\Reference;

        // ...
        $container->setParameter('posta.class', 'Acme\HelloBundle\Mail\Mailer');

        $container->setDefinition('posta', new Definition(
            '%posta.class%',
            array(new Reference('service_container'))
        ));

.. note::

    Iniettare l'intero contenitore in un servizio di solito non è una buona
    idea (iniettare solo ciò che serve). In alcuni rari casi, è necessario
    quando si ha un servizio nello scope ``container`` che necessita di un
    servizio nello scope ``request``.

Se si definisce un controllore come servizio, si può ottenere l'oggetto ``Request``
senza iniettare il contenitore, facendoselo passare come parametro nel metodo
dell'azione. Vedere :ref:`book-controller-request-argument` per maggiori dettagli.
