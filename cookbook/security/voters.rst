.. index::
   single: Sicurezza, Votanti

Come implementare i propri votanti per una lista nera di indirizzi IP
=====================================================================

Il componente della sicurezza di Symfony2 fornisce diversi livelli per autenticare gli
utenti. Uno dei livelli è chiamato `voter`. Un votante è una classe dedicata a verificare
che l'utente abbia i diritti per connettersi all'applicazione. Per esempio, Symfony2
fornisce un livello che verifica se l'utente è pienamente autenticato oppure se ha dei
ruoli.

A volte è utile creare un votante personalizzato, per gestire un caso specifico, non
coperto dal framework. In questa sezione, si imparerà come creare un votante che consenta
di mettere gli utenti una lista nera, in base al loro IP.

L'interfaccia Voter
-------------------

Un votante personalizzato deve implementare
:class:`Symfony\\Component\\Security\\Core\\Authorization\\Voter\\VoterInterface`,
che richiede i seguenti tre metodi:

.. code-block:: php

    interface VoterInterface
    {
        function supportsAttribute($attribute);
        function supportsClass($class);
        function vote(TokenInterface $token, $object, array $attributes);
    }


Il metodo ``supportsAttribute()`` è usato per verificare che il votante supporti
l'attributo utente dato (p.e.: un ruolo, un'ACL, ecc.)

Il metodo ``supportsClass()`` è usato per verificare che il votante supporti l'attuale
classe per il token dell'utente.

Il metodo ``vote()`` deve implementare la logica di business che verifica se l'utente
possa avere accesso o meno. Questo metodo deve restituire uno dei seguenti
valori:

* ``VoterInterface::ACCESS_GRANTED``: L'utente può accedere all'applicazione
* ``VoterInterface::ACCESS_ABSTAIN``: Il votante non può decidere se l'utente possa accedere o meno
* ``VoterInterface::ACCESS_DENIED``: L'utente non può accedere all'applicazione

In questo esempio, verificheremo la corrispondenza dell'indirizzo IP dell'utente con una
lista nera di indirizzi. Se l'IP dell'utente è nella lista nera, restituiremo
``VoterInterface::ACCESS_DENIED``, altrimenti restituiremo
``VoterInterface::ACCESS_ABSTAIN``, perché lo scopo del votante è solo quello di negare
l'accesso, non di consentirlo.

Creare un votante personalizzato
--------------------------------

Per inserire un utente nella lista nera in base al suo IP, possiamo usare il servizio
``request`` e confrontare l'indirizzo IP con un insieme di indirizzi IP in lista nera:

.. code-block:: php

    // src/Acme/DemoBundle/Security/Authorization/Voter/ClientIpVoter.php
    namespace Acme\DemoBundle\Security\Authorization\Voter;

    use Symfony\Component\DependencyInjection\ContainerInterface;
    use Symfony\Component\Security\Core\Authorization\Voter\VoterInterface;
    use Symfony\Component\Security\Core\Authentication\Token\TokenInterface;

    class ClientIpVoter implements VoterInterface
    {
        public function __construct(ContainerInterface $container, array $blacklistedIp = array())
        {
            $this->container     = $container;
            $this->blacklistedIp = $blacklistedIp;
        }

        public function supportsAttribute($attribute)
        {
            // non verifichiamo l'attributo utente, quindi restituiamo true
            return true;
        }

        public function supportsClass($class)
        {
            // il nostro votante supporta ogni tipo di classe token, quindi restituiamo true
            return true;
        }

        function vote(TokenInterface $token, $object, array $attributes)
        {
            $request = $this->container->get('request');
            if (in_array($request->getClientIp(), $this->blacklistedIp)) {
                return VoterInterface::ACCESS_DENIED;
            }

            return VoterInterface::ACCESS_ABSTAIN;
        }
    }

Ecco fatto! Il votante è pronto. Il prossimo passo consiste nell'iniettare il votante
dentro al livello della sicurezza. Lo si può fare facilmente tramite il contenitore di servizi.

Dichiarare il votante come servizio
-----------------------------------

Per iniettare il votante nel livello della sicurezza, dobbiamo dichiararlo come servizio
e assegnargli il tag "security.voter":

.. configuration-block::

    .. code-block:: yaml

        # src/Acme/AcmeBundle/Resources/config/services.yml
        services:
            security.access.blacklist_voter:
                class:      Acme\DemoBundle\Security\Authorization\Voter\ClientIpVoter
                arguments:  [@service_container, [123.123.123.123, 171.171.171.171]]
                public:     false
                tags:
                    -       { name: security.voter }

    .. code-block:: xml

        <!-- src/Acme/AcmeBundle/Resources/config/services.xml -->
        <service id="security.access.blacklist_voter"
                 class="Acme\DemoBundle\Security\Authorization\Voter\ClientIpVoter" public="false">
            <argument type="service" id="service_container" strict="false" />
            <argument type="collection">
                <argument>123.123.123.123</argument>
                <argument>171.171.171.171</argument>
            </argument>
            <tag name="security.voter" />
        </service>

    .. code-block:: php

        // src/Acme/AcmeBundle/Resources/config/services.php
        use Symfony\Component\DependencyInjection\Definition;
        use Symfony\Component\DependencyInjection\Reference;

        $definition = new Definition(
            'Acme\DemoBundle\Security\Authorization\Voter\ClientIpVoter',
            array(
                new Reference('service_container'),
                array('123.123.123.123', '171.171.171.171'),
            ),
        );
        $definition->addTag('security.voter');
        $definition->setPublic(false);

        $container->setDefinition('security.access.blacklist_voter', $definition);

.. tip::

   Assicurarsi di importare questo file di configurazione dal proprio file di configurazione
   principale (p.e. ``app/config/config.yml``). Per ulteriori informazioni,
   vedere :ref:`service-container-imports-directive`. Per saperne di più sulla definizione
   di servizi in generale, vederre il capitolo :doc:`/book/service_container`.

Cambiare la strategia decisionale per l'accesso
-----------------------------------------------

Per far sì che il votante abbia effetto, occorre modificare la strategia decisionale
predefinita per l'accesso, che, per impostazione predefinita, consente l'accesso se
*uno qualsiasi* dei votanti consente l'accesso.

Nel nostro caso, sceglieremo la strategia ``unanimous``. A differenza della strategia
``affirmative`` (quella predefinita), con la strategia ``unanimous``, l'accesso all'utente
è negato se anche solo uno dei votanti (p.e. ``ClientIpVoter``) lo
nega.

Per poterlo fare, sovrascrivere la sezione ``access_decision_manager`` del file di
configurazione della propria applicazione con il codice seguente.

.. configuration-block::

    .. code-block:: yaml

        # app/config/security.yml
        security:
            access_decision_manager:
                # la strategia piò essere: affirmative, unanimous o consensus
                strategy: unanimous

Ecco fatto! Ora, nella decisione sull'accesso di un utente, il nuovo votante negherà
l'accesso a ogni utente nella lista nera degli IP.
