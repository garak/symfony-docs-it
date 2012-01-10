==========
Panoramica
==========

Questo bundle migliora il componente Security di Symfony2, aggiungendo molte caratteristiche.

Caratteristiche:

- potente linguaggio di autorizzazioni basato su espressioni
- autorizzazione metodo sicurezza
- configurazione autorizzazioni tramite annotazioni

Installazione
-------------
Aggiungere le righe seguenti al proprio file ``deps``::

    [JMSSecurityExtraBundle]
        git=https://github.com/schmittjoh/JMSSecurityExtraBundle.git
        target=/bundles/JMS/SecurityExtraBundle
        
    ; Dependencies:
    ;--------------
    [metadata]
        git=https://github.com/schmittjoh/metadata.git
        version=1.1.0 ; <- make sure to get 1.1, not 1.0
    
    ; see https://github.com/schmittjoh/JMSAopBundle/blob/master/Resources/doc/index.rst    
    [JMSAopBundle]
        git=https://github.com/schmittjoh/JMSAopBundle.git
        target=/bundles/JMS/AopBundle
    
    [cg-library]
        git=https://github.com/schmittjoh/cg-library.git
        
    ; This dependency is optional (you need it if you are using non-service controllers):
    ; see https://github.com/schmittjoh/JMSDiExtraBundle/blob/master/Resources/doc/index.rst
    [JMSDiExtraBundle]
        git=https://github.com/schmittjoh/JMSDiExtraBundle.git
        target=/bundles/JMS/DiExtraBundle

Quindi registrare il bundle nel proprio Kernel::

    // in AppKernel::registerBundles()
    $bundles = array(
        // ...
        new JMS\AopBundle\JMSAopBundle(),
        new JMS\SecurityExtraBundle\JMSSecurityExtraBundle(),
        new JMS\DiExtraBundle\JMSDiExtraBundle($this),
        // ...
    );

Assicurarsi anche di registrare gli spazi dei nomi nell'autoloader::

    // app/autoload.php
    $loader->registerNamespaces(array(
        // ...
        'JMS'              => __DIR__.'/../vendor/bundles',
        'Metadata'         => __DIR__.'/../vendor/metadata/src',
        'CG'               => __DIR__.'/../vendor/cg-library/src',
        // ...
    ));

Configurazione
--------------

Quella che segue è la configurazione predefinita::

    # app/config/config.yml
    jms_security_extra:
        # Whether you want to secure all services (true), or only secure specific
        # services (false); see also below 
        secure_all_services: false
        
        # Enabling this setting will add an additional special attribute "IS_IDDQD".
        # Anybody with this attribute will effectively bypass all security checks.
        enable_iddqd_attribute: false        
        
        # Enables expression language
        expressions: false

        # Allows you to disable some, or all built-in voters
        voters:
            disable_authenticated: false
            disable_role:          false
            disable_acl:           false
            
        # Allows you to specify access control rules for specific methods, such
        # as controller actions
        method_access_control: { }


Linguaggio di autorizzazione basato su espressioni
--------------------------------------------------
Il linguaggio di espressioni è un'alternativa molto potente ai semplici attributi
del sistema di voti di sicurezza. Consente l'esecuzioni di verifiche complesse sugli
accessi e, essendo compilato in PHP puro, è molto più veloce dei voti. Inoltre, sfrutta
il caricamento pigro, quindi si salveranno anche un po' di risorse, per esempio non
dovendo inizializzare l'intero sistema di ACL a ogni
richiesta.

Uso programmatico
~~~~~~~~~~~~~~~~~
Si possono eseguire le espressioni in modo programmatico, usando il metodo ``isGranted``
di SecurityContext. Alcuni esempi::

    use JMS\SecurityExtraBundle\Security\Authorization\Expression\Expression;
    
    $securityContext->isGranted(array(new Expression('hasRole("A")')));
    $securityContext->isGranted(array(new Expression('hasRole("A") or (hasRole("B") and hasRole("C"))')));
    $securityContext->isGranted(array(new Expression('hasPermission(object, "VIEW")'), $object));
    $securityContext->isGranted(array(new Expression('token.getUsername() == "Johannes"')));

Uso in Twig
~~~~~~~~~~~
Si possono verificare espressioni dai template Twig, usando la funzione ``is_expr_granted``.
Alcuni esempi::

    is_expr_granted("hasRole('FOO')")
    is_expr_granted("hasPermission(object, 'VIEW')", object)

Uso in access_control
~~~~~~~~~~~~~~~~~~~~~
Si possono usare espressioni anche in ``access_control``::

    security:
        access_control:
            - { path: ^/foo, access: "hasRole('FOO') and hasRole('BAR')" }

Uso basato su annotazioni
~~~~~~~~~~~~~~~~~~~~~~~~~
vedere @PreAuthorize nel riferimento sulle annotazioni

Riferimento
~~~~~~~~~~~
+-----------------------------------+--------------------------------------------+
| Espressione                       | Descrizione                                |
+===================================+============================================+
| hasRole('ROLE')                   | Verifica se il token ha un certo           |
|                                   | ruolo.                                     |
+-----------------------------------+--------------------------------------------+
| hasAnyRole('ROLE1', 'ROLE2', ...) | Verifica se il token ha uno qualsiasi dei  |
|                                   | ruoli dati.                                |
+-----------------------------------+--------------------------------------------+
| isAnonymous()                     | Verifica se il token è anonimo.            |
+-----------------------------------+--------------------------------------------+
| isRememberMe()                    | Verifica se il token è "remember me".      |
+-----------------------------------+--------------------------------------------+
| isFullyAuthenticated()            | Verifica se il token è pienamente          |
|                                   | autenticato.                               |
+-----------------------------------+--------------------------------------------+
| isAuthenticated()                 | Verifica se il token non è anonimo.        |
+-----------------------------------+--------------------------------------------+
| hasPermission(*var*, 'PERMISSION')| Verifica se il token ha il permesso dato   |
|                                   | per l'oggetto dato (richiede il sistema    |
|                                   | ACL).                                      |
+-----------------------------------+--------------------------------------------+
| token                             | Variabile riferita al token attualmente    |
|                                   | nel contesto della sicurezza.              |
+-----------------------------------+--------------------------------------------+
| user                              | Variabile riferita all'utente attualmente  |
|                                   | nel contesto della sicurezza.              |
+-----------------------------------+--------------------------------------------+
| object                            | Variabile riferita all'oggetto per cui     |
|                                   | l'accesso è stato richiesto.               |
+-----------------------------------+--------------------------------------------+
| #*paramName*                      | Un identificatore preceduto da # si        |
|                                   | riferisce a un parametro con lo stesso nome|
|                                   | passato al metodo in cui l'espr. è usata.  |
+-----------------------------------+--------------------------------------------+
| and / &&                          | Operatore binario "e"                      |
+-----------------------------------+--------------------------------------------+
| or / ||                           | Operatore binario "o"                      |
+-----------------------------------+--------------------------------------------+
| ==                                | Operatore binario "uguale"                 |
+-----------------------------------+--------------------------------------------+
| not / !                           | Operatore di negazione                     |
+-----------------------------------+--------------------------------------------+

Autorizzazione con metodo di sicurezza
--------------------------------------
Di solito, si possono proteggere tutti i metodi public o protected che non siano 
static o final. I metodi privati non possono essere protetti. Si possono anche
aggiungere meta-dati per metodi astratti o interfacce, che saranno poi applicati alle
loro effettive implementazioni in modo automatico.

Controllo degli accessi tramite configurazione DI
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Si possono specificare **espressioni** di controllo accessi nella configurazione DI::

    # config.yml
    jms_security_extra:
        method_access_control:
            ':loginAction$': 'isAnonymous()'
            'AcmeFooBundle:.*:deleteAction': 'hasRole("ROLE_ADMIN")'
            '^MyNamespace\MyService::foo$': 'hasPermission(#user, "VIEW")' 

Lo schema è un'espressione regolare sensibile alle maiuscole, che viene verificata con
due notazioni. Viene usata la prima corrispondenza.
Lo schema viene prima verificato con la notazione per i controllori che non siano servizi.
Ovviamente, questo viene fatto se la propria classe è effettivamente un controllore, p.e.
``AcmeFooBundle:Add:new`` per un controllore ``AddController`` e un metodo
``newAction`` in un sotto-spazio dei nomi ``Controller`` in un bundle ``AcmeFooBundle``. 

Poi, lo schema viene verificato con la concatenazione del nome della classe e del metodo
che si sta richiamando, p.e. ``Mia\Classe\Pienamente\Qualificata::mioMetodo``.

**Nota:** Se si vogliono proteggere controlli che non siano servizi, si deve
installare ``JMSDiExtraBundle``.

Controllo degli accessi tramite annotazioni
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Se si vuole proteggere un servizio con le annotazioni, occorre abilitare la
configurazione delle annotazioni per tale servizio::

    <service id="foo" class="Bar">
        <tag name="security.secure_service"/>
    </service>

In caso si vogliano configurare tutti i servizi tramite le annotazioni, si può anche
impostare ``secure_all_services`` a ``true``. Non sarà quindi necessario aggiungere un
tag per ogni servizio.


Annotazioni
-----------
@PreAuthorize
~~~~~~~~~~~~~
Questa annotazione consente di definire un'espressione (si veda il paragrafo sul
linguaggio delle espressioni), eseguita prima di invocare un metodo::

    <?php
    
    use JMS\SecurityExtraBundle\Annotation\PreAuthorize;
    
    class MyService
    {
        /** @PreAuthorize("hasRole('A') or (hasRole('B') and hasRole('C'))") */
        public function secureMethod()
        {
            // ...
        }
    }

@Secure
~~~~~~~
Questa annotazione consente di definire a chi è consentito invocare un metodo::

    <?php
    
    use JMS\SecurityExtraBundle\Annotation\Secure;
    
    class MyService
    {
        /**
         * @Secure(roles="ROLE_USER, ROLE_FOO, ROLE_ADMIN")
         */
        public function secureMethod() 
        {
            // ...
        }
    }

@SecureParam
~~~~~~~~~~~~
Questa annotazione consente di definire restrizioni per i parametri passsati al metodo.
È utile solo se i parametri sono oggetti del dominio::

    <?php
    
    use JMS\SecurityExtraBundle\Annotation\SecureParam;
    
    class MyService
    {
        /**
         * @SecureParam(name="comment", permissions="EDIT, DELETE")
         * @SecureParam(name="post", permissions="OWNER")
         */
        public function secureMethod($comment, $post)
        {
            // ...
        }
    }

@SecureReturn
~~~~~~~~~~~~~
Questa annotazione consente di definire restrizioni per il valore restituito dal metodo.
È utile solo se il valore restituito è un oggetto del dominio::

    <?php
    
    use JMS\SecurityExtraBundle\Annotation\SecureReturn;
    
    class MyService
    {
        /**
         * @SecureReturn(permissions="VIEW")
         */
        public function secureMethod()
        {
            // ...
            
            return $domainObject;
        }
    }
    
@RunAs
~~~~~~
Questa annotazione consente di specificare ruoli da aggiungere solo per la durata
dell'invocazione del metodo. Tali ruoli non saranno presi in considerazione per decisioni
di accesso precedenti o successive all'invocazione. 

Solitamente si usa per implementare un livelli di servizio doppio, in cui si hanno
servizi pubblici e privati e in cui i servizi privati devono essere invocati solamente
tramite uno specifico servizio pubblico::

    <?php
    
    use JMS\SecurityExtraBundle\Annotation\Secure;
    use JMS\SecurityExtraBundle\Annotation\RunAs;
    
    class MyPrivateService
    {
        /**
         * @Secure(roles="ROLE_PRIVATE_SERVICE")
         */
        public function aMethodOnlyToBeInvokedThroughASpecificChannel()
        {
            // ...
        }
    }
    
    class MyPublicService
    {
        protected $myPrivateService;
    
        /**
         * @Secure(roles="ROLE_USER")
         * @RunAs(roles="ROLE_PRIVATE_SERVICE")
         */
        public function canBeInvokedFromOtherServices()
        {
            return $this->myPrivateService->aMethodOnlyToBeInvokedThroughASpecificChannel();
        }
    }

@SatisfiesParentSecurityPolicy
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Deve essere definito su un metodo che sovrascrive un altro metodo, che ha meta-dati di
sicurezza. Serve ad assicurare la consapevolezza che la sicurezza del metodo sovrascritto
non può più essere assicurata e che si devono copiare tutte le annotazioni che si vogliono
mantenere.
