.. index::
   single: Sicurezza, Votanti

Implementare dei votanti per una lista nera di indirizzi IP
===========================================================

Il componente della sicurezza di Symfony fornisce diversi livelli per autenticare gli
utenti. Uno dei livelli è chiamato `voter` (votante). Un votante è una classe dedicata a verificare
che l'utente abbia i diritti per connettersi all'applicazione. Per esempio, Symfony
fornisce un livello che verifica se l'utente è pienamente autenticato oppure se ha dei
ruoli.

A volte è utile creare un votante personalizzato, per gestire un caso specifico, non
coperto dal framework. In questa sezione si imparerà come creare un votante che consenta
di mettere gli utenti una lista nera, in base al loro IP.

L'interfaccia Voter
-------------------

Un votante personalizzato deve implementare
:class:`Symfony\\Component\\Security\\Core\\Authorization\\Voter\\VoterInterface`,
che richiede i seguenti tre metodi:

.. include:: /cookbook/security/voter_interface.rst.inc

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

    // src/AppBundle/Security/Authorization/Voter/ClientIpVoter.php
    namespace AppBundle\Security\Authorization\Voter;

    use Symfony\Component\DependencyInjection\ContainerInterface;
    use Symfony\Component\Security\Core\Authorization\Voter\VoterInterface;
    use Symfony\Component\Security\Core\Authentication\Token\TokenInterface;

    class ClientIpVoter implements VoterInterface
    {
        private $container;

        private $blacklistedIp;

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

        public function vote(TokenInterface $token, $object, array $attributes)
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

.. tip::

   Le implementazioni dei metodi
   :method:`Symfony\\Component\\Security\\Core\\Authorization\\Voter\\VoterInterface::supportsAttribute` 
   e :method:`Symfony\\Component\\Security\\Core\\Authorization\\Voter\\VoterInterface::supportsClass` 
   non sono chiamate internamente dal framework. Una volta registrato il
   votante, il metodo ``vote()`` sarà sempre richiamato, indipendentemente dal fatto
   che tali metodi restituiscano ``true`` o meno. Occorre quindi richiamare tali
   metodi nell'implementazione del metodo ``vote()`` e restituire ``ACCESS_ABSTAIN``,
   nel caso in cui il votante non supporti la classe o l'attributo.

Dichiarare il votante come servizio
-----------------------------------

Per iniettare il votante nel livello della sicurezza, dobbiamo dichiararlo come servizio
e assegnargli il tag "security.voter":

.. configuration-block::

    .. code-block:: yaml

        # src/Acme/AcmeBundle/Resources/config/services.yml
        services:
            security.access.blacklist_voter:
                class:     AppBundle\Security\Authorization\Voter\ClientIpVoter
                arguments: ["@service_container", [123.123.123.123, 171.171.171.171]]
                public:    false
                tags:
                    - { name: security.voter }

    .. code-block:: xml

        <!-- src/Acme/AcmeBundle/Resources/config/services.xml -->
        <service id="security.access.blacklist_voter"
                 class="AppBundle\Security\Authorization\Voter\ClientIpVoter" public="false">
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
            'AppBundle\Security\Authorization\Voter\ClientIpVoter',
            array(
                new Reference('service_container'),
                array('123.123.123.123', '171.171.171.171'),
            ),
        );
        $definition->addTag('security.voter');
        $definition->setPublic(false);

        $container->setDefinition('security.access.blacklist_voter', $definition);

.. tip::

   Assicurarsi di importare questo file di configurazione dal file di configurazione
   principale (p.e. ``app/config/config.yml``). Per ulteriori informazioni,
   vedere :ref:`service-container-imports-directive`. Per saperne di più sulla definizione
   di servizi in generale, vedere il capitolo :doc:`/book/service_container`.

.. _security-voters-change-strategy:

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
configurazione dell'applicazione con il codice seguente.

.. configuration-block::

    .. code-block:: yaml

        # app/config/security.yml
        security:
            access_decision_manager:
                # la strategia può essere: affirmative, unanimous o consensus
                strategy: unanimous

    .. code-block:: xml

        <!-- app/config/security.xml -->
        <config>
            <!-- la strategia può essere: affirmative, unanimous o consensus -->
            <access-decision-manager strategy="unanimous">
        </config>

    .. code-block:: php

        // app/config/security.xml
        $container->loadFromExtension('security', array(
            // la strategia può essere: affirmative, unanimous o consensus
            'access_decision_manager' => array(
                'strategy' => 'unanimous',
            ),
        ));

Ecco fatto! Ora, nella decisione sull'accesso di un utente, il nuovo votante negherà
l'accesso a ogni utente nella lista nera degli IP.

Si noti che i votanti sono chiamati solo alla verifica di un accesso. Quindi occorre
almeno qualcosa come

.. configuration-block::

    .. code-block:: yaml

        # app/config/security.yml
        security:
            access_control:
                - { path: ^/, role: IS_AUTHENTICATED_ANONYMOUSLY }

    .. code-block:: xml

        <!-- app/config/security.xml -->
        <config>
            <access-control>
               <rule path="^/" role="IS_AUTHENTICATED_ANONYMOUSLY" />
            </access-control>
        </config>

    .. code-block:: php

        // app/config/security.xml
        $container->loadFromExtension('security', array(
            'access_control' => array(
               array('path' => '^/', 'role' => 'IS_AUTHENTICATED_ANONYMOUSLY'),
            ),
         ));

.. seealso::

    Per un uno avanzato, vedere
    :ref:`components-security-access-decision-manager`.
