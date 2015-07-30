Sicurezza
=========

Autenticazione e firewall (recuperare le credenziali dell'utente)
-----------------------------------------------------------------

Per autenticare gli utenti è possibile configurare Symfony in molti modi. Inoltre
è possibile caricare le informazioni degli utenti da qualsiasi fonte. 
Questo è un argomento abbastanza complesso, per maggiori informazioni si 
rimanda alla :doc:`sezione sicurezza del ricettario </cookbook/security/index>`.

A prescindere dalle necessità, l'autenticazione è configurata in ``security.yml``, sotto
la voce ``firewalls``.

.. best-practice::

    A meno che non si abbiano due meccanismi di autenticazione differenti (ad esempio il
    form di login per il sito principale e un sistema a token per le API), si
    raccomanda di definire un *unico* firewall, con l'opzione ``anonymous``
    abilitata.

La maggior parte delle applicazioni utilizza solamente un meccanismo di autenticazione e
un insieme di utenti. Per questa tipologia di applicazioni basta soltanto un *unico* firewall.
Ovviamente esistono delle eccezioni, ad esempio quando in un sito si devono proteggere delle API dalla
sezione web. L'importante è mantenere le cose semplici.

Si dovrebbe inoltre abilitare sempre l'opzione ``anonymous`` nel firewall. Se
si ha bisogno che gli utenti accedano a sezioni differenti del sito (o forse
a *tutte* le sezioni), utilizzare la configurazione dell'opzione ``access_control``.

.. best-practice::

    Usare ``bcrypt`` per codificare le password degli utenti.

Se si memorizzano le password degli utenti nel sistema, si raccomanda di usare l'encoder ``bcrypt``,
invece della tradizionale codifica SHA-512. I vantaggi più importanti
di ``bcrypt`` sono l'inclusione di un valore *salt* per la protezione contro gli
attacchi di tipo rainbow table e la sua natura adattiva, che consente di rallentare la
sua esecuzione e resistere meglio agli attacchi di forza bruta.

Detto questo, ecco un esempio di autenticazione di un'applicazione che usa un form login
per caricare gli utenti dalla base dati:

.. code-block:: yaml

    # app/config/security.yml
    security:
        encoders:
            AppBundle\Entity\User: bcrypt

        providers:
            database_users:
                entity: { class: AppBundle:User, property: username }

        firewalls:
            secured_area:
                pattern: ^/
                anonymous: true
                form_login:
                    check_path: security_login_check
                    login_path: security_login_form

                logout:
                    path: security_logout
                    target: homepage

    # ... c'è anche access_control, ma non viene mostrato qui

.. tip::

    Il codice sorgente dell'applicazione di prova include commenti che spiegheranno dettagliatamente ogni parte del file.

Autorizzazione (negare l'accesso)
---------------------------------

Symfony definisce vari modi per configurare l'autorizzazione, inclusa l'opzione ``access_control``
in :doc:`security.yml </reference/configuration/security>` e l'uso di
:ref:`isGranted <best-practices-directly-isGranted>` direttamente dal
servizio ``security.context``.

.. best-practice::

    * Per la protezione di schemi di URL ampi, usare ``access_control``
    * Per logiche di sicurezza più complesse, usare direttamente il
      servizio ``security.context``

Esistono anche diversi modi per centralizzare la logica di autorizzazione, come i
votanti e le ACL (o lista di controllo degli accessi).

.. best-practice::

    * Personalizzare un votante per definire restrizioni a grana fine;
    * Usare le ACL per definire logiche di sicurezza complesse (per gestire l'accesso di ogni oggetto da ogni
      utente attraverso un'interfaccia di amministrazione).

.. _best-practices-directly-isGranted:
.. _checking-permissions-without-security:

Controllare i permessi a mano
-----------------------------

Se non si può controllare l'accesso in base a schemi di URL, è sempre possibile
effettuare il controllo da codice PHP:

.. code-block:: php

    use Symfony\Component\Security\Core\Exception\AccessDeniedException;

    // ...

    /**
     * @Route("/{id}/edit", name="admin_post_edit")
     */
    public function editAction($id)
    {
        $post = $this->getDoctrine()->getRepository('AppBundle:Post')
            ->find($id);

        if (!$post) {
            throw $this->createNotFoundException();
        }

        if (!$post->isAuthor($this->getUser())) {
            throw new AccessDeniedException();
        }

        // ...
    }

I votanti
---------

Se la logica di sicurezza è complessa e non può essere centralizzata in un metodo
come ``isAuthor()``, si dovrebbe creare un votante personalizzato. Gestire la sicurezza con
i votanti risulta più semplice rispetto alle :doc:`ACLs </cookbook/security/acl>` e fornisce
la flessibilità richiesta in quasi tutti gli scenari.

Si inizi creando una classe votante. Il seguente esempio mostra la classe che implementa la stessa
logica del metodo ``getAuthorEmail`` vista sopra:

.. code-block:: php

    namespace AppBundle\Security;

    use Symfony\Component\Security\Core\Authorization\Voter\AbstractVoter;
    use Symfony\Component\Security\Core\User\UserInterface;

    // La classe AbstractVoter richiede Symfony 2.6 o successivi
    class PostVoter extends AbstractVoter
    {
        const CREATE = 'create';
        const EDIT   = 'edit';

        protected function getSupportedAttributes()
        {
            return array(self::CREATE, self::EDIT);
        }

        protected function getSupportedClasses()
        {
            return array('AppBundle\Entity\Post');
        }

        protected function isGranted($attribute, $post, $user = null)
        {
            if (!$user instanceof UserInterface) {
                return false;
            }

            if ($attribute === self::CREATE && in_array('ROLE_ADMIN', $user->getRoles(), true)) {
                return true;
            }

            if ($attribute === self::EDIT && $user->getEmail() === $post->getAuthorEmail()) {
                return true;
            }

            return false;
        }
    }

Per abilitare il votante nell'applicazione definire un nuovo servizio:

.. code-block:: yaml

    # app/config/services.yml
    services:
        # ...
        post_voter:
            class:      AppBundle\Security\PostVoter
            public:     false
            tags:
               - { name: security.voter }

È adesso possibile usare il votante tramite il servizio ``security.context``:

.. code-block:: php

    use Symfony\Component\Security\Core\Exception\AccessDeniedException;

    // ...

    /**
     * @Route("/{id}/edit", name="admin_post_edit")
     */
    public function editAction($id)
    {
        $post = // query for the post ...

        if (!$this->get('security.context')->isGranted('edit', $post)) {
            throw new AccessDeniedException();
        }
    }

Saperne di più
--------------

Il bundle `FOSUserBundle`_, sviluppato dalla comunità di Symfony, aggiunge il supporto
alla gestione utenti memorizzati in una base di dati. Il bundle implementa la gestione di task comuni,
come la registrazione utente e la funzionalità di recupero password.

Per consentire agli utenti di connettersi solo una volta, senza dover reinserire la
password ogni volta che visitano il sito, abilitare la funzionalità :doc:`ricordami </cookbook/security/remember_me>`.

Nel fornire assistenza ai clienti, a volte è necessario accedere all'applicazione
come *altri* utenti, in modo da poter riprodurre il problema. Symfony fornisce l'abilità di
:doc:`impersoncare gli utenti  </cookbook/security/impersonating_user>`.

Se un'azienda usa un metodo di login non supportato da Symfony, è possibile sviluppare il
:doc:`proprio fornitore di utenti </cookbook/security/custom_provider>` e il
:doc:`proprio fornitore di autenticazione </cookbook/security/custom_authentication_provider>`.

.. _`FOSUserBundle`: https://github.com/FriendsOfSymfony/FOSUserBundle
