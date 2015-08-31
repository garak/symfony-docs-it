.. index::
   single: Sicurezza; "Ricordami"

Aggiungere la funzionalità "ricordami" al login
===============================================

Una volta che l'utente è autenticato, le sue credenziali sono solitamente salvate nella
sessione. Questo vuol dire che quando la sessione finisce, l'utente sarà fuori dal sito e
dovrà inserire nuovamente le sue informazioni di login, la prossima volta che vorrà
accedere all'applicazione. Si può consentire agli utenti di scegliere di rimanere dentro
più a lungo del tempo della sessione, usando un cookie con l'opzione ``remember_me`` del firewall:

.. configuration-block::

    .. code-block:: yaml

        # app/config/security.yml
        firewalls:
            main:
                remember_me:
                    key:      "%secret%"
                    lifetime: 31536000 # 365 giorni in secondi
                    path:     /

    .. code-block:: xml

        <!-- app/config/security.xml -->
        <config>
            <firewall>
                <remember-me
                    key      = "%secret%"
                    lifetime = "31536000" <!-- 365 giorni in secondi -->
                    path     = "/"
                />
            </firewall>
        </config>

    .. code-block:: php

        // app/config/security.php
        $container->loadFromExtension('security', array(
            'firewalls' => array(
                'main' => array(
                    'remember_me' => array(
                        'key'      => '%secret%',
                        'lifetime' => 31536000, // 365 giorni in secondi
                        'path'     => '/',
                    ),
                ),
            ),
        ));

Il firewall ``remember_me`` definisce le seguenti opzioni di configurazione:

``key`` (valore predefinito: ``null``)
    Il valore usato per codificare il contenuto del cookie. Di solito si usa il valore di
    ``secret``, definito nel file ``app/config/parameters.yml``.

``name`` (valore predefinito: ``REMEMBERME``)
    Il nome del cookie usato per mantenere l'utente connesso. Se si abilita il
    ``remember_me`` in vari firewall della stessa applicazione, assicurarsi di
    usare un nome diverso per il cookie di ciascun firewall. In caso contrario, si
    potrebbero avere molti problemi di sicurezza.

``lifetime`` (valore predefinito: ``31536000``)
    Il numero di secondi in cui l'utente resterà connesso. Per impostazione predefinita
    gli utenti restano connessi per un anno.

``path`` (valore predefinito: ``/``)
    Il percorso in cui è usato il cookie associato a questa funzionalità. L'impostazione predefinita
    applica il cookie all'intero sito, ma lo si può restringere a sezioni
    specifiche (p.e. ``/forum``, ``/admin``).

``domain`` (valore predefinito: ``null``)
    Il dominio in cui è usato il cookie associato a questa funzionalità. L'impostazione predefinita
    usa il dominio corrente, ottenuto da ``$_SERVER``.

``secure`` (valore predefinito: ``false``)
    Se ``true``, il cookie associato a questa funzionalità è inviato all'utente
    tramite una connessione HTTPS.

``httponly`` (valore predefinito: ``true``)
    Se ``true``, il cookie associato a questa funzionalità è accessibile solo
    tramite HTTP. Questo vuol dire che il cookie non sarà accessibile
    a linguaggi di scripting, come JavaScript.

``remember_me_parameter`` (valore predefinito: ``_remember_me``)
    Il nome del campo del form, spuntato per decidere se la funzionalità "ricordami" vada
    abilitata o meno. Si legga questa ricetta per sapere come poter abilitare tale
    funzionalità in modo condizionale.

``always_remember_me`` (valore predefinito: ``false``)
    Se ``true``, il valore ``remember_me_parameter`` è ignorato e la funzionalità
    "ricordami" è sempre abilitata, indipendentemente dalla volontà
    dell'utente finale.

Consentire all'utente di rinunciare al "ricordami"
--------------------------------------------------

È una buona idea dare all'utente la possibilità di usare o non usare la funzionalità
"ricordami", perché non sempre è appropriata. Il modo usuale per farlo è l'aggiunta di un
checkbox al form di login. Dando al checkbox il nome ``_remember_me``, il cookie sarà
automaticamente impostato quando il checkbox è spuntato e l'utente entra. Quindi, il
form di login potrebbe alla fine assomigliare a
questo:

.. configuration-block::

    .. code-block:: html+jinja

        {# src/Acme/SecurityBundle/Resources/views/Security/login.html.twig #}
        {% if error %}
            <div>{{ error.message }}</div>
        {% endif %}

        <form action="{{ path('login_check') }}" method="post">
            <label for="username">Nome utente:</label>
            <input type="text" id="username" name="_username" value="{{ last_username }}" />

            <label for="password">Password:</label>
            <input type="password" id="password" name="_password" />

            <input type="checkbox" id="remember_me" name="_remember_me" checked />
            <label for="remember_me">Ricordami</label>

            <input type="submit" name="login" />
        </form>

    .. code-block:: html+php

        <!-- src/Acme/SecurityBundle/Resources/views/Security/login.html.php -->
        <?php if ($error): ?>
            <div><?php echo $error->getMessage() ?></div>
        <?php endif; ?>

        <form action="<?php echo $view['router']->generate('login_check') ?>" method="post">
            <label for="username">Nome utente:</label>
            <input type="text" id="username"
                   name="_username" value="<?php echo $last_username ?>" />

            <label for="password">Password:</label>
            <input type="password" id="password" name="_password" />

            <input type="checkbox" id="remember_me" name="_remember_me" checked />
            <label for="remember_me">Ricordami</label>

            <input type="submit" name="login" />
        </form>

L'utente sarà quindi automaticamente autenticato nelle sue visite successive, finché
il cookie resta valido.

Costringere l'utente ad autenticarsi di nuovo prima di accedere ad alcune risorse
---------------------------------------------------------------------------------

Quando l'utente torna sul sito, viene autenticato automaticamente in base alle
informazioni memorizzate nel cookie "ricordami". Ciò consente all'utente di accedere
a risorse protette, come se si fosse effettivamente autenticato prima di entrare nel
sito.

In alcuni casi, si potrebbe desiderare di costringere l'utente ad autenticarsi nuovamente,
prima di accedere ad alcune risorse. Per esempio, si potrebbe voler consentire un
"ricordami" per vedere le informazioni di base di un account, ma poi richiedere
un'effettiva autenticazione prima di modificare le informazioni stesse.

Il componente della sicurezza fornisce un modo facile per poterlo fare. In aggiunta ai
ruoli esplicitamente assegnati loro, agli utenti viene dato automaticamente uno dei
seguenti ruoli, a seconda di come si sono autenticati:

* ``IS_AUTHENTICATED_ANONYMOUSLY`` - assegnato automaticamente a un utente che si trova
  in una parte del sito protetta dal firewall, ma che non si è effettivamente autenticato.
  Ciò è possibile solo se è consentito l'accesso anonimo.

* ``IS_AUTHENTICATED_REMEMBERED`` - assegnato automaticamente a un utente che si è
  autenticato tramite un cookie "ricordami".

* ``IS_AUTHENTICATED_FULLY`` - assegnato automaticamente a un utente che ha fornito le
  sue informazioni di autenticazione durante la sessione corrente.

Si possono usare questi ruoli, oltre a quelli espliciti, per controllare l'accesso.

.. note::

    Se si ha il ruolo ``IS_AUTHENTICATED_REMEMBERED``, si ha anche il ruolo
    ``IS_AUTHENTICATED_ANONYMOUSLY``. Se si ha il ruolo ``IS_AUTHENTICATED_FULLY``, si
    hanno anche gli altri due ruoli. In altre parole, questi ruoli rappresentano
    tre livelli incrementali della "forza" dell'autenticazione.

Si possono usare questi ruoli addizionali per affinare il controllo sugli accessi a parti
di un sito. Per esempio, si potrebbe desiderare che l'utente sia in grado di vedere il
suo account in ``/account`` se autenticato con cookie, ma che debba fornire le sue
informazioni di accesso per poterlo modificare. Lo si può fare proteggendo
specifiche azioni del controllore, usando questi ruoli. L'azione di modifica del
controllore potrebbe essere messa in sicurezza usando il contesto del servizio. 

Nel seguente esempio, l'azione è consentita solo se l'utente ha il ruolo 
``IS_AUTHENTICATED_FULLY``.

.. code-block:: php

    // ...
    use Symfony\Component\Security\Core\Exception\AccessDeniedException

    public function editAction()
    {
        if (false === $this->get('security.context')->isGranted(
            'IS_AUTHENTICATED_FULLY'
           )) {
            throw new AccessDeniedException();
        }

        // ...
    }

Si può anche installare opzionalmente JMSSecurityExtraBundle_, che può mettere in
sicurezza il controllore tramite annotazioni:

.. code-block:: php

    use JMS\SecurityExtraBundle\Annotation\Secure;

    /**
     * @Secure(roles="IS_AUTHENTICATED_FULLY")
     */
    public function editAction($name)
    {
        // ...
    }

.. tip::

    Se si avesse anche un controllo di accesso nella propria configurazione della
    sicurezza, che richiede all'utente il ruolo ``ROLE_USER`` per poter accedere all'area
    dell'account, si avrebbe la seguente situazione:

    * Se un utente non autenticato (o anonimo) tenta di accedere all'area dell'account,
      gli sarà chiesto di autenticarsi.

    * Una volta inseriti nome utente e password, ipotizzando che l'utente riceva il ruolo
      ``ROLE_USER`` in base alla configurazione, l'utente avrà il ruolo
      ``IS_AUTHENTICATED_FULLY`` e potrà accedere a qualsiasi pagina della sezione
      account, incluso il controllore ``editAction``.

    * Se la sessione scade, quando l'utente torna sul sito, potrà accedere a ogni pagina
      della sezione account, tranne per quella di modifica, senza doversi autenticare
      nuovamente. Tuttavia, quando proverà ad accedere al controllore
      ``editAction``, sarà costretto ad autenticarsi di nuovo, perché non è ancora
      pienamente autenticato.

Per maggiori informazioni sulla messa in sicurezza di servizi o metodi con questa tecnica,
vedere :doc:`/cookbook/security/securing_services`.

.. _JMSSecurityExtraBundle: https://github.com/schmittjoh/JMSSecurityExtraBundle
