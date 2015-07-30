.. index::
   single: Sicurezza

Sicurezza
=========

Il sistema di sicurezza di Symfony è incredibilmente potente, ma può anche essere difficile da
configurare. In questo capitolo si vedrà come impostare passo-passo la sicurezza di
un'applicazione, dalla configurazione del firewall a come caricare utenti per negare
l'accesso e recuperare un oggetto utente. A seconda dei bisogni, a volte la prima
configurazione potrebbe essere difficoltosa. Ma, una volta a posto, il sistema di sicurezza di Symfony
sarà flessibile e (speriamo) divertente.

Questa guida è divisa in alcune
sezioni:

1) Preparazione di ``security.yml`` (*autenticazione*);

2) Negare l'accesso all'applicazione (*autorizzazione*);

3) Recuperare l'oggetto corrispondente all'utente corrente

Successivamente ci saranno un certo numero di piccole (ma interessanti) sezioni,
come :ref:`logout <book-security-logging-out>` e
:ref:`codifica delle password <security-encoding-password>`.

.. _book-security-firewalls:

1) Preparazione di security.yml (autenticazione)
------------------------------------------------

Il sistema di sicurezza è configurato in ``app/config/security.yml``. La configurazione
predefinita è simile a questa:

.. configuration-block::

    .. code-block:: yaml

        # app/config/security.yml
        security:
            providers:
                in_memory:
                    memory: ~

            firewalls:
                dev:
                    pattern: ^/(_(profiler|wdt)|css|images|js)/
                    security: false

                default:
                    anonymous: ~

    .. code-block:: xml

        <!-- app/config/security.xml -->
        <?xml version="1.0" encoding="UTF-8"?>
        <srv:container xmlns="http://symfony.com/schema/dic/security"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xmlns:srv="http://symfony.com/schema/dic/services"
            xsi:schemaLocation="http://symfony.com/schema/dic/services
                http://symfony.com/schema/dic/services/services-1.0.xsd">

            <config>
                <provider name="in_memory">
                    <memory />
                </provider>

                <firewall name="dev"
                    pattern="^/(_(profiler|wdt)|css|images|js)/"
                    security=false />

                <firewall name="default">
                    <anonymous />
                </firewall>
            </config>
        </srv:container>

    .. code-block:: php

        // app/config/security.php
        $container->loadFromExtension('security', array(
            'providers' => array(
                'in_memory' => array(
                    'memory' => array(),
                ),
            ),
            'firewalls' => array(
                'dev' => array(
                    'pattern'    => '^/(_(profiler|wdt)|css|images|js)/',
                    'security'   => false,
                ),
                'default' => array(
                    'anonymous'  => null,
                ),
            ),
        ));

La voce ``firewalls`` è il *cuore* della configurazione della sicurezza. Il firewall
``dev`` non è importante, serve solo ad assicurarsi che gli strumenti di sviluppo di Symfony,
che si trovano sotto URL come ``/_profiler`` e ``/_wdt``, non siano
bloccati.

Tutti gli altri URL saranno gestiti dal firewall ``default`` (l'assenza della chiave ``pattern``
vuol dire che corrisponde a *ogni* URL). Si può pensare al firewall come il proprio
sistema di sicurezza e quindi solitamente ha senso avere un singolo firewall.
Ma questo non vuol dire che ogni URL richieda autenticazione e quindi la voce ``anonymous``
si occupa di questo. In effetti, se ora si apre l'homepage, si potrà
accedere e si vedrà che si è "autenticati" come ``anon.``. Non lasciarsi
ingannare dalla parola "Yes" vicino ad "Authenticated", si è ancora un utente anonimo:

.. image:: /images/book/security_anonymous_wdt.png
   :align: center

Più avanti si vedrà come negare l'accesso ad alcuni URL o controllori.

.. tip::

    La sicurezza è *altamente* configurabile e c'è una guida di
    :doc:`riferimento alla configurazione della sicurezza </reference/configuration/security>`,
    che mostra tutte le opzioni, con spiegazioni aggiuntive.

A) Configurare il modo in cui gli utenti si autenticano
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Il lavoro principale di un firewall è quello di configurare il *modo* in cui gli utenti si
autenticheranno. Useranno un form? Http Basic? Il token di un'API? Tutti questi metodi insieme?

Iniziamo con Http Basic (il caro vecchio popup).
Per attivarlo, aggiungere la voce ``http_basic`` nel firewall:

.. configuration-block::

    .. code-block:: yaml

        # app/config/security.yml
        security:
            # ...

            firewalls:
                # ...
                default:
                    anonymous: ~
                    http_basic: ~

    .. code-block:: xml

        <!-- app/config/security.xml -->
        <?xml version="1.0" encoding="UTF-8"?>
        <srv:container xmlns="http://symfony.com/schema/dic/security"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xmlns:srv="http://symfony.com/schema/dic/services"
            xsi:schemaLocation="http://symfony.com/schema/dic/services
                http://symfony.com/schema/dic/services/services-1.0.xsd">

            <config>
                <!-- ... -->

                <firewall name="default">
                    <anonymous />
                    <http-basic />
                </firewall>
            </config>
        </srv:container>

    .. code-block:: php

        // app/config/security.php
        $container->loadFromExtension('security', array(
            // ...
            'firewalls' => array(
                // ...
                'default' => array(
                    'anonymous'  => null,
                    'http_basic' => null,
                ),
            ),
        ));

Facile! Per fare una prova, si deve richiedere che un utente sia connesso per poter vedere
una pagina. Per rendere le cose interessanti, creare una nuova pagina su ``/admin``. Per
esempio, se si usano le annotazioni, creare qualcosa come questo::

    // src/AppBundle/Controller/DefaultController.php
    // ...

    use Sensio\Bundle\FrameworkExtraBundle\Configuration\Route;
    use Symfony\Component\HttpFoundation\Response;

    class DefaultController extends Controller
    {
        /**
         * @Route("/admin")
         */
        public function adminAction()
        {
            return new Response('Pagina admin!');
        }
    }

Quindi, aggiungere a ``security.yml`` una voce ``access_control`` che richieda all'utente
di essere connesso per poter accedere a tale URL:

.. configuration-block::

    .. code-block:: yaml

        # app/config/security.yml
        security:
            # ...
            firewalls:
                # ...

            access_control:
                # require ROLE_ADMIN for /admin*
                - { path: ^/admin, roles: ROLE_ADMIN }

    .. code-block:: xml

        <!-- app/config/security.xml -->
        <?xml version="1.0" encoding="UTF-8"?>
        <srv:container xmlns="http://symfony.com/schema/dic/security"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xmlns:srv="http://symfony.com/schema/dic/services"
            xsi:schemaLocation="http://symfony.com/schema/dic/services
                http://symfony.com/schema/dic/services/services-1.0.xsd">

            <config>
                <!-- ... -->

                <firewall name="default">
                    <!-- ... -->
                </firewall>

                <access-control>
                    <!-- require ROLE_ADMIN for /admin* -->
                    <rule path="^/admin" role="ROLE_ADMIN" />
                </access-control>
            </config>
        </srv:container>

    .. code-block:: php

        // app/config/security.php
        $container->loadFromExtension('security', array(
            // ...
            'firewalls' => array(
                // ...
                'default' => array(
                    // ...
                ),
            ),
           'access_control' => array(
               // require ROLE_ADMIN for /admin*
                array('path' => '^/admin', 'role' => 'ROLE_ADMIN'),
            ),
        ));

.. note::

    La questione ``ROLE_ADMIN`` e l'accesso negato saranno analizzati
    più avanti, nella sezione :ref:`security-authorization`.

Ottimo! Ora, se si va su ``/admin``, si vedrà il popup HTTP Basic:

.. image:: /images/book/security_http_basic_popup.png
   :align: center

Ma come si può entrare? Da dove vengono gli utenti?

.. _book-security-form-login:

.. tip::

    E se invece si volesse un form di login tradizionoale? Nessun problema! Vedere :doc:`/cookbook/security/form_login_setup`.
    Che altri metodi sono supportati? Vedere :doc:`riferimento sulla configurazione </reference/configuration/security>`
    oppure :doc:`costruire un proprio </cookbook/security/custom_authentication_provider>`.

.. tip::

    Se l'applicazione autentica gli utenti tramite un servizio esterno, come Google,
    Facebook o Twitter, dare un'occhiata al bundle `HWIOAuthBundle`_.

.. _security-user-providers:
.. _where-do-users-come-from-user-providers:

B) Configurare come vengono caricati gli utenti
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Quando si inserisce il proprio nome utente, Symfony deve caricare le informazioni
da qualche parte. Questo viene chiamato "fornitore di utenti" ed è compito dello sviluppatore
configurarlo. Symfony ha un modo predefinito di
:doc:`caricare utenti dalla base dati </cookbook/security/entity_provider>`,
ma si può anche :doc:`creare il proprio fornitore di utenti </cookbook/security/custom_provider>`.

Il modo più facile (ma anche più limitato) è di configurare Symfony per caricare utenti inseriti
direttamente nel file ``security.yml``. Questo fornitore è chiamato "in memoria",
ma è meglio pensare a esso come fornitore "in configurazione":

.. configuration-block::

    .. code-block:: yaml

        # app/config/security.yml
        security:
            providers:
                in_memory:
                    memory:
                        users:
                            ryan:
                                password: ryanpass
                                roles: 'ROLE_USER'
                            admin:
                                password: kitten
                                roles: 'ROLE_ADMIN'
            # ...

    .. code-block:: xml

        <!-- app/config/security.xml -->
        <?xml version="1.0" encoding="UTF-8"?>
        <srv:container xmlns="http://symfony.com/schema/dic/security"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xmlns:srv="http://symfony.com/schema/dic/services"
            xsi:schemaLocation="http://symfony.com/schema/dic/services
                http://symfony.com/schema/dic/services/services-1.0.xsd">

            <config>
                <provider name="in_memory">
                    <memory>
                        <user name="ryan" password="ryanpass" roles="ROLE_USER" />
                        <user name="admin" password="kitten" roles="ROLE_ADMIN" />
                    </memory>
                </provider>
                <!-- ... -->
            </config>
        </srv:container>

    .. code-block:: php

        // app/config/security.php
        $container->loadFromExtension('security', array(
            'providers' => array(
                'in_memory' => array(
                    'memory' => array(
                        'users' => array(
                            'ryan' => array(
                                'password' => 'ryanpass',
                                'roles' => 'ROLE_USER',
                            ),
                            'admin' => array(
                                'password' => 'kitten',
                                'roles' => 'ROLE_ADMIN',
                            ),
                        ),
                    ),
                ),
            ),
            // ...
        ));

Come per i ``firewalls``, si possono avere più ``providers``, ma probabilmente
ne basterà uno solo. Se si ha bisogno di più fornitori, si può configurare il fornitore
usato dal firewall, sotto la voce ``provider`` (p.e.
``provider: in_memory``).

.. seealso::

    Vedere :doc:`/cookbook/security/multiple_user_providers` per tutti
    i dettagli sulla configurazione di fornitori multipli.

Provare a entrare con nome utente ``admin`` e password ``kitten``. Si dovrebbe
vedere un errore!

    No encoder has been configured for account "Symfony\Component\Security\Core\User\User"

Per risolvere, aggiungere una chiave ``encoders``:

.. configuration-block::

    .. code-block:: yaml

        # app/config/security.yml
        security:
            # ...

            encoders:
                Symfony\Component\Security\Core\User\User: plaintext
            # ...

    .. code-block:: xml

        <!-- app/config/security.xml -->
        <?xml version="1.0" encoding="UTF-8"?>
        <srv:container xmlns="http://symfony.com/schema/dic/security"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xmlns:srv="http://symfony.com/schema/dic/services"
            xsi:schemaLocation="http://symfony.com/schema/dic/services
                http://symfony.com/schema/dic/services/services-1.0.xsd">

            <config>
                <!-- ... -->

                <encoder class="Symfony\Component\Security\Core\User\User"
                    algorithm="plaintext" />
                <!-- ... -->
            </config>
        </srv:container>

    .. code-block:: php

        // app/config/security.php
        $container->loadFromExtension('security', array(
            // ...

            'encoders' => array(
                'Symfony\Component\Security\Core\User\User' => 'plaintext',
            ),
            // ...
        ));

I fornitori di utenti caricano le informazioni dell'utente e le inseriscono in un oggetto ``User``. Se
si :doc:`caricano utenti dalla base dati </cookbook/security/entity_provider>`
o da :doc:`altre sorgenti </cookbook/security/custom_provider>`, si userà una propria
classe personalizzata. Se invece si usa il fornitore "in memoria", esso
restituirà un oggetto ``Symfony\Component\Security\Core\User\User``.

Qualunque sia la classe di ``User``, si deve dire a Symfony quale algoritmo sia stato
usato per codificare le password. In questo caso, le password sono in chiaro,
ma tra un attimo faremo in modo di usare ``bcrypt``.

Se ora si aggiorna, ci si troverà dentro! La barra di debug del web fornirà informazioni
sul nome dell'utente e sui suoi ruoli:

.. image:: /images/book/symfony_loggedin_wdt.png
   :align: center

Poiché questo URL richiede ``ROLE_ADMIN``, se si prova a entrare come ``ryan`` ci si
vedrà negato l'accesso. Lo vedremo più avanti (:ref:`security-authorization-access-control`).

.. _book-security-user-entity:

Caricare utenti dalla base dati
...............................

Se si vogliono caricare gli utenti usando l'ORM di Doctrine, è facile! Vedere
:doc:`/cookbook/security/entity_provider` per tutti i dettagli.

.. _book-security-encoding-user-password:
.. _c-encoding-the-users-password:
.. _encoding-the-user-s-password:

C) Codifica delle password
~~~~~~~~~~~~~~~~~~~~~~~~~~

Che gli utenti siano dentro a ``security.yml``, in una base dati o da qualsiasi altra
parte, se ne vorranno codificare le password. Il miglior algoritmo da usare è
``bcrypt``:

.. configuration-block::

    .. code-block:: yaml

        # app/config/security.yml
        security:
            # ...

            encoders:
                Symfony\Component\Security\Core\User\User:
                    algorithm: bcrypt
                    cost: 12

    .. code-block:: xml

        <!-- app/config/security.xml -->
        <?xml version="1.0" encoding="UTF-8"?>
        <srv:container xmlns="http://symfony.com/schema/dic/security"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xmlns:srv="http://symfony.com/schema/dic/services"
            xsi:schemaLocation="http://symfony.com/schema/dic/services
                http://symfony.com/schema/dic/services/services-1.0.xsd">

            <config>
                <!-- ... -->

                <encoder class="Symfony\Component\Security\Core\User\User"
                    algorithm="bcrypt"
                    cost="12" />
                
                <!-- ... -->
            </config>
        </srv:container>

    .. code-block:: php

        // app/config/security.php
        $container->loadFromExtension('security', array(
            // ...

            'encoders' => array(
                'Symfony\Component\Security\Core\User\User' => array(
                    'algorithm' => 'bcrypt',
                    'cost' => 12,
                )
            ),
            // ...
        ));

.. include:: /cookbook/security/_ircmaxwell_password-compat.rst.inc

Ovviamente, sarà ora necessario codificare le password con tale algoritmo.
Per gli utenti inseriti a mano, si può usare uno `strumento online`_, che restituirà
qualcosa del genere:

.. configuration-block::

    .. code-block:: yaml

        # app/config/security.yml
        security:
            # ...

            providers:
                in_memory:
                    memory:
                        users:
                            ryan:
                                password: $2a$12$LCY0MefVIEc3TYPHV9SNnuzOfyr2p/AXIGoQJEDs4am4JwhNz/jli
                                roles: 'ROLE_USER'
                            admin:
                                password: $2a$12$cyTWeE9kpq1PjqKFiWUZFuCRPwVyAZwm4XzMZ1qPUFl7/flCM3V0G
                                roles: 'ROLE_ADMIN'

    .. code-block:: xml

        <!-- app/config/security.xml -->
        <?xml version="1.0" encoding="UTF-8"?>
        <srv:container xmlns="http://symfony.com/schema/dic/security"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xmlns:srv="http://symfony.com/schema/dic/services"
            xsi:schemaLocation="http://symfony.com/schema/dic/services
                http://symfony.com/schema/dic/services/services-1.0.xsd">

            <config>
                <provider name="in_memory">
                    <memory>
                        <user name="ryan" password="$2a$12$LCY0MefVIEc3TYPHV9SNnuzOfyr2p/AXIGoQJEDs4am4JwhNz/jli" roles="ROLE_USER" />
                        <user name="admin" password="$2a$12$cyTWeE9kpq1PjqKFiWUZFuCRPwVyAZwm4XzMZ1qPUFl7/flCM3V0G" roles="ROLE_ADMIN" />
                    </memory>
                </provider>
                <!-- ... -->
            </config>
        </srv:container>

    .. code-block:: php

        // app/config/security.php
        $container->loadFromExtension('security', array(
            'providers' => array(
                'in_memory' => array(
                    'memory' => array(
                        'users' => array(
                            'ryan' => array(
                                'password' => '$2a$12$LCY0MefVIEc3TYPHV9SNnuzOfyr2p/AXIGoQJEDs4am4JwhNz/jli',
                                'roles' => 'ROLE_USER',
                            ),
                            'admin' => array(
                                'password' => '$2a$12$cyTWeE9kpq1PjqKFiWUZFuCRPwVyAZwm4XzMZ1qPUFl7/flCM3V0G',
                                'roles' => 'ROLE_ADMIN',
                            ),
                        ),
                    ),
                ),
            ),
            // ...
        ));

Tutto funzionerà come prima. Ma se si hanno utenti dinamici
(p.e. da base dati), come si fa a codificare ogni password prima
dell'inserimento? Nessun problema, vedere
:ref:`security-encoding-password` per i dettagli.

.. tip::

    Gli algoritmi supportati dipendono dalla versione di PHP, ma
    includono gli algoritmi restituiti dalla funzione :phpfunction:`hash_algos`,
    più alcuni altri (come bcrypt). Vedere la voce ``encoders`` nella sezione
    :doc:`riferimento della sicurezza </reference/configuration/security>`
    per degli esempi.

D) Configurazione conclusa!
~~~~~~~~~~~~~~~~~~~~~~~~~~~

Congratulazioni! Ora di dispone un sistema di autenticazione funzionante, che usa Http
Basic e carica utenti dal file ``security.yml``.

I prossimi passi possono variare:

* Configurare un modo diverso per il login, come un :ref:`form di login <book-security-form-login>`
  o :doc:`qualcosa di completamente personalizzato </cookbook/security/custom_authentication_provider>`;

* Caricare utenti da un'altra sorgente, come la :doc:`base dati </cookbook/security/entity_provider>`
  o :doc:`un'altra sorgente </cookbook/security/custom_provider>`;

* Imparare come negare l'accesso, caricare l'oggetto ``User`` e trattare con i ruoli nella sezione
  :ref:`autorizzazione <security-authorization>`.

.. _`security-authorization`:

2) Accesso negato, ruoli e altre autorizzazioni
-----------------------------------------------

Ora gli utenti possono accedere all'applicazione usando ``http_basic`` o un altro metodo.
Ottimo! Ora, occorre imparare come negare l'accesso e lavorare con l'oggetto ``User``.
Questo processo prende il nome di **autorizzazione** e spetta a esso decidere se un utente possa
accedere a una determinata risorsa (un URL, un oggetto del modello, una chiamata a un metodo, ...).

Il processo di autorizzazione ha due lati:

#. L'utente riceve uno specifico insieme di ruoli, quando entra (p.e. ``ROLE_ADMIN``).
#. Si aggiunge codice in modo che una risorsa (come un URL o un controllore) richieda uno specifico
   "attributo" (solitamente un ruolo, come ``ROLE_ADMIN``) per potervi
   accedere.

.. tip::

    Oltre ai ruoli (come ``ROLE_ADMIN``), si può proteggere una risorsa
    tramite altri attributi/stringhe (come ``EDIT``) e usare i votanti o il sistema
    ACL di Symfony per dar loro un significato. Questo può essere utile nel caso serva
    verificare se l'utente A possa modificare un oggetto B (p.e. un prodotto con un determinato id).
    Vedere :ref:`security-secure-objects`.

.. _book-security-roles:

Ruoli
~~~~~

Quando un utente entra, riceve un insieme di ruoli (p.e. ``ROLE_ADMIN``). Nell'esempio
precedente, tali ruoli sono scritti a mano in ``security.yml``. Se si caricano
utenti dalla base dati, probabilmente saranno memorizzati in una colonna
della tabella.

.. caution::

    Tutti i ruoli assegnati **devono** avere il prefisso ``ROLE_``.
    In caso contrario, non possono essere gestiti dal sistema di sicurezza di Symfony
    (a meno che non si faccia qualcosa di avanzato, assegnare un
    ruolo come ``PIPPO`` a un utente e poi verificare ``PIPPO``, come descritto
    :ref:`successivamente <security-role-authorization>` non funzionerà).

I ruoli sono semplici e sono di base stringhe inventate e usate come necessario.
Per esempio, per poter iniziare a limitare l'accesso alla sezione amministrativa di un blog,
si può proteggere tale sezione usando un ruolo ``ROLE_BLOG_ADMIN``.
Non occorre definire tale ruolo in altri posti, basta iniziare a
usarlo.

.. tip::

    Assicurarsi che ciascun utente abbia almeno *un* ruolo, altrimenti sembrerà che l'utente
    non sia autenticato. Una convenzione tipica consiste nell'assegnare a *ogni*
    utente ``ROLE_USER``.

Si può anche specificare una :ref:`gerarchia di ruoli <security-role-hierarchy>`, in cui
determinati ruoli ne hanno automaticamente anche altri.

.. _security-role-authorization:

Aggiungere codice per negare l'accesso
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Ci sono **due** modi per negare accesso a qualcosa:

#. :ref:`access_control in security.yml <security-authorization-access-control>`
   consente di proteggere schemi di URL (p.e. ``/admin/*``). È facile,
   ma meno flessibile;

#. :ref:`nel codice, tramite il servizio security.context <book-security-securing-controller>`.

.. _security-authorization-access-control:

Proteggere schemi di URL (access_control)
.........................................

Il modo più semplice per proteggere parti di un'applicazione è proteggere un intero
schema di URL. L'abbiamo visto in precedenza, quando abbiamo richiesto che ogni URL
corrispondente all'espressione regolare ``^/admin`` richieda ``ROLE_ADMIN``:

.. configuration-block::

    .. code-block:: yaml

        # app/config/security.yml
        security:
            # ...
            firewalls:
                # ...

            access_control:
                # require ROLE_ADMIN for /admin*
                - { path: ^/admin, roles: ROLE_ADMIN }

    .. code-block:: xml

        <!-- app/config/security.xml -->
        <?xml version="1.0" encoding="UTF-8"?>
        <srv:container xmlns="http://symfony.com/schema/dic/security"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xmlns:srv="http://symfony.com/schema/dic/services"
            xsi:schemaLocation="http://symfony.com/schema/dic/services
                http://symfony.com/schema/dic/services/services-1.0.xsd">

            <config>
                <!-- ... -->

                <firewall name="default">
                    <!-- ... -->
                </firewall>

                <access-control>
                    <!-- require ROLE_ADMIN for /admin* -->
                    <rule path="^/admin" role="ROLE_ADMIN" />
                </access-control>
            </config>
        </srv:container>

    .. code-block:: php

        // app/config/security.php
        $container->loadFromExtension('security', array(
            // ...
            'firewalls' => array(
                // ...
                'default' => array(
                    // ...
                ),
            ),
           'access_control' => array(
               // require ROLE_ADMIN for /admin*
                array('path' => '^/admin', 'role' => 'ROLE_ADMIN'),
            ),
        ));

Questo va benissimo per proteggere intere sezioni, ma si potrebbero anche voler
:ref:`proteggere singoli controllori <book-security-securing-controller>`.


Si possono definire quanti schemi di URL si vuole, ciascuno con un'espressione regolare.
Tuttavia, solo **uno** di questi avrà una corrispondenza. Symfony inizierà cercando
dalla cima e si fermerà non appena troverà una voce di ``access_control`` che
corrisponda all'URL.

.. configuration-block::

    .. code-block:: yaml

        # app/config/security.yml
        security:
            # ...
            access_control:
                - { path: ^/admin/users, roles: ROLE_SUPER_ADMIN }
                - { path: ^/admin, roles: ROLE_ADMIN }

    .. code-block:: xml

        <!-- app/config/security.xml -->
        <?xml version="1.0" encoding="UTF-8"?>
        <srv:container xmlns="http://symfony.com/schema/dic/security"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xmlns:srv="http://symfony.com/schema/dic/services"
            xsi:schemaLocation="http://symfony.com/schema/dic/services
                http://symfony.com/schema/dic/services/services-1.0.xsd">

            <config>
                <!-- ... -->
                <access-control>
                    <rule path="^/admin/users" role="ROLE_SUPER_ADMIN" />
                    <rule path="^/admin" role="ROLE_ADMIN" />
                </access-control>
            </config>
        </srv:container>

    .. code-block:: php

        // app/config/security.php
        $container->loadFromExtension('security', array(
            // ...
            'access_control' => array(
                array('path' => '^/admin/users', 'role' => 'ROLE_SUPER_ADMIN'),
                array('path' => '^/admin', 'role' => 'ROLE_ADMIN'),
            ),
        ));

Aggiungendo un ``^`` iniziale, solo gli URL *che iniziano* con lo
schema corrisponderanno. Per esempio, un percorso ``/admin`` (senza
``^``) corrisponderebbe ad ``/admin/pippo``, ma anche a URL come ``/pippo/admin``.

.. _security-book-access-control-explanation:

.. sidebar:: Capire come funziona ``access_control``

    La sezione ``access_control`` è molto potente, ma può anche essere pericolosa
    (perché si tratta di sicurezza), se non ci comprende *come* funzioni.
    Oltre all'URL, ``access_control`` può corrispondere un indirizzo IP,
    un nome di host e metodi HTTP. Può anche essere usato per rinviare un utente alla
    versione ``https`` di uno schema di URL.

    Per approfondire questi argomenti, vedere :doc:`/cookbook/security/access_control`.

.. _`book-security-securing-controller`:

Proteggere controllori e altro codice
.....................................

Si può negare accesso da dentro un controllore::

    // ...
    use Symfony\Component\Security\Core\Exception\AccessDeniedException;

    public function helloAction($name)
    {
        if (!$this->get('security.context')->isGranted('ROLE_ADMIN')) {
            throw new AccessDeniedException();
        }

        // ...
    }

Ecco fatto! Se l'utente non è ancora connesso, gli sarà richiesto il login (p.e. sarà
rinviato alla pagina di login). Se invece *è* connesso, ma *non* ha il ruolo
``ROLE_ADMIN``, gli sarà mostrata una pagina di errore 403 (che si può
:ref:`personalizzare <cookbook-error-pages-by-status-code>`). Se è connesso e ha i
ruoli giusti, il codice sarà eseguito.

.. _book-security-template:

Controllo degli accessi nei template
....................................

Se si vuole verificare in un template che l'utente corrente abbia un ruolo, usare
la funzione aiutante predefinita:

.. configuration-block::

    .. code-block:: html+jinja

        {% if is_granted('ROLE_ADMIN') %}
            <a href="...">Elimina</a>
        {% endif %}

    .. code-block:: html+php

        <?php if ($view['security']->isGranted('ROLE_ADMIN')): ?>
            <a href="...">Elimina</a>
        <?php endif ?>

Se si usa questa funzione *non* essendo dietro a un firewall, sarà lanciata
un'ecceezione. È quindi sempre una buona idea avere almeno un firewall
principale, che copra tutti gli URL (come mostrato in questo capitolo).

.. caution::

    Prestare attenzione nel layout e nelle pagine di errore! A causa di alcuni dettagli
    interno di Symfony, per evitare di rompere le pagine di errore in ambiente ``prod``,
    verificare prima se sia definito ``app.user``:

    .. code-block:: html+jinja

        {% if app.user and is_granted('ROLE_ADMIN') %}

Proteggere altri servizi
........................

In Symfony, ogni cosa può essere protetta facendo qualcosa di simile a
questo. Per esempio, si supponga di avere un servizio (cioè una classe PHP), il cui
compito è inviare email. Si può restringere l'uso di questa classe, non importa dove
venga usata, solo ad alcuni utenti.

Per maggiori informazioni, vedere :doc:`/cookbook/security/securing_services`.

Verificare se un utente sia connesso (IS_AUTHENTICATED_FULLY)
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Finora, i controlli sugli accessi sono stati basati su ruoli, stringhe che iniziano con
``ROLE_`` e assegnate agli utenti. Se invece si vuole *solo* verificare se un
utente sia connesso (senza curarsi dei ruoli), si può usare
``IS_AUTHENTICATED_FULLY``::

    // ...
    use Symfony\Component\Security\Core\Exception\AccessDeniedException;

    public function helloAction($name)
    {
        if (!$this->get('security.context')->isGranted('IS_AUTHENTICATED_FULLY')) {
            throw new AccessDeniedException();
        }

        // ...
    }

.. tip::

    Si può usare questo metodo anche in ``access_control``.

``IS_AUTHENTICATED_FULLY`` non è un ruolo, ma si comporta come tale ed è assegnato a ciascun
utente che sia sia connesso. In effetti, ci sono tre attributi
speciali di questo tipo:

* ``IS_AUTHENTICATED_REMEMBERED``: Assegnato a *tutti* gli utenti connessi, anche se
  si sono connessi tramite un cookie "ricordami". Anche se non si usa
  la :doc:`funzionalità "ricordami" </cookbook/security/remember_me>`,
  lo si può usare per verificare se l'utente sia connesso.

* ``IS_AUTHENTICATED_FULLY``: Simile a ``IS_AUTHENTICATED_REMEMBERED``,
  ma più forte. Gli utenti connessi tramite un cookie "ricordami"
  avranno ``IS_AUTHENTICATED_REMEMBERED``, ma non ``IS_AUTHENTICATED_FULLY``.

* ``IS_AUTHENTICATED_ANONYMOUSLY``: Assegnato a *tutti* gli utenti (anche quelli anonimi).
  Utile per mettere URL in una *lista bianca* per garantire accesso, alcuni
  dettagli sono in :doc:`/cookbook/security/access_control`.

.. _security-secure-objects:

Access Control List (ACL): proteggere singoli oggetti della base dati
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Si immagini di progettare un blog in cui gli utenti possono commentare i post. Si
vuole anche che un utente sia in grado di modificare i propri commenti, ma non quelli di
altri utenti. Inoltre, come utente amministratore, si vuole essere in grado di modificare
*tutti* i commenti.

Per la realizzazione, si hanno due opzioni:

* :doc:`I votanti </cookbook/security/voters_data_permission>` consentono di
  usare logica di business (p.e. l'utente può modificare i suoi commenti perché ne
  è il creatore) per stabilire l'accesso. Probabilmente si userà questa opzione, è
  abbastanza flessibile per risolvere la situazione.

* :doc:`Le ACL </cookbook/security/acl>` consentono di creare una struttura di base dati
  in cui si può assegnare *qualsiasi* accesso (p.e. EDIT, VIEW) a *quasliasi* utente
  su *qualsiasi* oggetto del sistema. Usarla se si ha bisogno che l'utente amministratore possa
  garantire accessi personalizzati nel sistema, tramite una qualche interfaccia di amministrazione.

In entrambi i casi, occorre comunque negare l'accesso usando metodi simili a quelli
visti in precedenza.

Recuperare l'oggetto utente
---------------------------

Dopo l'autenticazione, si può accedere all'oggetto ``User`` dell'uente corrente
tramite il servizio ``security.context``. Da dentro un controllore, Sarà una
cosa simile::

    public function indexAction()
    {
        if (!$this->get('security.context')->isGranted('IS_AUTHENTICATED_FULLY')) {
            throw new AccessDeniedException();
        }

        $user = $this->getUser();

        // il precedente è una scorciatoia per questo
        $user = $this->get('security.context')->getToken()->getUser();
    }

.. tip::

    L'oggetto e la classe dell'utente dipenderanno dal
    proprio :ref:`fornitore di utenti <security-user-providers>`.

Ora si possono chiamare i metodi desiderati sul *proprio* oggetto utente. Per esempio,
se il proprio oggetto utente ha un metodo ``getFirstName()``, lo si può usare::

    use Symfony\Component\HttpFoundation\Response;
    // ...

    public function indexAction()
    {
        // ...

        return new Response('Ciao '.$user->getFirstName());
    }

Verificare sempre se l'utente è connesso
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

È importante verificare prima se l'utente sia autenticato. Se non lo è,
``$user`` sarà ``null`` oppure la stringa ``anon.``. Come? Esatto,
c'è una stranezza. Se non si è leggati, l'utente è tecnicamente la stringa
``anon.``, anche se la scorciatoia ``getUser()`` del controllore la converte in
``null`` per convenienza.

Il punto è questo: verificare sempre se l'utente sia connesso, prima di usare
l'oggetto ``User`` e usare il metodo ``isGranted`` (o
:ref:`access_control <security-authorization-access-control>`) per farlo::

    // Usare questo per vedere se l'utente sia connesso
    if (!$this->get('security.context')->isGranted('IS_AUTHENTICATED_FULLY')) {
        throw new AccessDeniedException();
    }

    // Non verificare mai l'oggetto User per verdere se l'utente sia connesso
    if ($this->getUser()) {

    }

Recuperare l'utente in un template
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

In un template Twig, si può accedere all'oggetto tramite :ref:`app.user <reference-twig-global-app>`_:


.. configuration-block::

    .. code-block:: html+jinja

        {% if is_granted('IS_AUTHENTICATED_FULLY') %}
            <p>Nome utente: {{ app.user.username }}</p>
        {% endif %}

    .. code-block:: html+php

        <?php if ($view['security']->isGranted('IS_AUTHENTICATED_FULLY')): ?>
            <p>Nome utente: <?php echo $app->getUser()->getUsername() ?></p>
        <?php endif; ?>

.. _book-security-logging-out:

Logout
------

Solitamente, si desidera che gli utenti possano eseguire un logout. Per fortuna,
il firewall può gestirlo automaticamente, se si attiva il parametro
``logout`` nella configurazione:

.. configuration-block::

    .. code-block:: yaml

        # app/config/security.yml
        security:
            firewalls:
                secured_area:
                    # ...
                    logout:
                        path:   /logout
                        target: /
            # ...

    .. code-block:: xml

        <!-- app/config/security.xml -->
        <?xml version="1.0" encoding="UTF-8"?>
        <srv:container xmlns="http://symfony.com/schema/dic/security"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xmlns:srv="http://symfony.com/schema/dic/services"
            xsi:schemaLocation="http://symfony.com/schema/dic/services
                http://symfony.com/schema/dic/services/services-1.0.xsd">

            <config>
                <firewall name="secured_area" pattern="^/">
                    <!-- ... -->
                    <logout path="/logout" target="/" />
                </firewall>
                <!-- ... -->
            </config>
        </srv:container>

    .. code-block:: php

        // app/config/security.php
        $container->loadFromExtension('security', array(
            'firewalls' => array(
                'secured_area' => array(
                    // ...
                    'logout' => array('path' => 'logout', 'target' => '/'),
                ),
            ),
            // ...
        ));

Quindi, si deve creare una rotta per tale URL (non serve invece un controllore):

.. configuration-block::

    .. code-block:: yaml

        # app/config/routing.yml
        logout:
            path:   /logout

    .. code-block:: xml

        <!-- app/config/routing.xml -->
        <?xml version="1.0" encoding="UTF-8" ?>
        <routes xmlns="http://symfony.com/schema/routing"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://symfony.com/schema/routing
                http://symfony.com/schema/routing/routing-1.0.xsd">

            <route id="logout" path="/logout" />
        </routes>

    ..  code-block:: php

        // app/config/routing.php
        use Symfony\Component\Routing\RouteCollection;
        use Symfony\Component\Routing\Route;

        $collection = new RouteCollection();
        $collection->add('logout', new Route('/logout', array()));

        return $collection;

Ecco fatto! Se l'utente va su ``/logout`` (o sull'URL configurato
in ``path``), Symfony disconnetterà l'utente corrente.

Una volta eseguito il logout, l'utente sarà rinviato al percorso
definito nel parametro ``target`` (p.e. su ``homepage``).

.. tip::

    Se si ha bisogno di fare qualcosa d'altro dopo il logout, si può
    specificare un gestore di logout, aggiungendo la voce ``success_handler``
    e puntandola a un servizio, che implementi
    :class:`Symfony\\Component\\Security\\Http\\Logout\\LogoutSuccessHandlerInterface`.
    Vedere :doc:`Security Configuration Reference </reference/configuration/security>`.

.. _`security-encoding-password`:

Codifica dinamica di una password
---------------------------------

Se, per esempio, gli utenti sono memorizzati in una base dati, occorrerà codificare
le loro password, prima di inserirle. Non importa quale algoritmo sia
configurato per l'oggetto utente, l'hash della password può sempre essere determinato
nel modo seguente, in un controllore::

    $factory = $this->get('security.encoder_factory');
    // qualunque sia il *proprio* oggetto User
    $user = new AppBundle\Entity\User();

    $encoder = $factory->getEncoder($user);
    $password = $encoder->encodePassword('ryanpass', $user->getSalt());
    $user->setPassword($password);

Per poter funzionare, assicurarsi di avere un codificatore per la classe
utente (p.e. ``AppBundle\Entity\User``) configurato sotto la  voce ``encoders``
in ``app/config/security.yml``.

L'oggetto ``$encoder`` ha anche un metodo ``isPasswordValid``, che accetta
l'oggetto ``User`` come primo parametro e la password in chiaro, da verificare,
come secondo parametro.

.. caution::

    Quando si consente a un utente di inviare una password in chiaro (p.e. un form
    di registrazione, un form di cambio password), si *deve* avere una validazione che garantisca
    una lunghezza massima della password di 4096 caratteri. Maggiori dettagli su
    :ref:`implementare una semplice form di registrazione <cookbook-registration-password-max>`.

.. _security-role-hierarchy:

Gerarchia dei ruoli
-------------------

Invece di associare molti ruoli agli utenti, si possono definire regole di ereditarietà
dei ruoli, creando una gerarchia:

.. configuration-block::

    .. code-block:: yaml

        # app/config/security.yml
        security:
            role_hierarchy:
                ROLE_ADMIN:       ROLE_USER
                ROLE_SUPER_ADMIN: [ROLE_ADMIN, ROLE_ALLOWED_TO_SWITCH]

    .. code-block:: xml

        <!-- app/config/security.xml -->
        <?xml version="1.0" encoding="UTF-8"?>
        <srv:container xmlns="http://symfony.com/schema/dic/security"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xmlns:srv="http://symfony.com/schema/dic/services"
            xsi:schemaLocation="http://symfony.com/schema/dic/services
                http://symfony.com/schema/dic/services/services-1.0.xsd">

            <config>
                <role id="ROLE_ADMIN">ROLE_USER</role>
                <role id="ROLE_SUPER_ADMIN">ROLE_ADMIN, ROLE_ALLOWED_TO_SWITCH</role>
            </config>
        </srv:container>

    .. code-block:: php

        // app/config/security.php
        $container->loadFromExtension('security', array(
            'role_hierarchy' => array(
                'ROLE_ADMIN'       => 'ROLE_USER',
                'ROLE_SUPER_ADMIN' => array(
                    'ROLE_ADMIN',
                    'ROLE_ALLOWED_TO_SWITCH',
                ),
            ),
        ));

In questa configurazione, gli utenti con il ruolo ``ROLE_ADMIN`` avranno anche il ruolo
``ROLE_USER``. Il ruolo ``ROLE_SUPER_ADMIN`` ha ``ROLE_ADMIN``, ``ROLE_ALLOWED_TO_SWITCH``
e ``ROLE_USER`` (ereditato da ``ROLE_ADMIN``).

Autenticazione senza stato
--------------------------

Symfony si appoggia a un cookie (la sessione) per persistere il contesto di sicurezza
dell'utente. Se però si usano certificati o autenticazione HTTP, per
esempio, non serve persistenza, perché le credenziali sono disponibili a
ogni richiesta. In tal caso, e non si ha bisogno di memorizzare altro tra una
richiesta e l'altro, si può attivare l'autenticazione senza stato (che vuol dire che
Symfony non creerà alcun cookie):

.. configuration-block::

    .. code-block:: yaml

        # app/config/security.yml
        security:
            firewalls:
                main:
                    http_basic: ~
                    stateless:  true

    .. code-block:: xml

        <!-- app/config/security.xml -->
        <?xml version="1.0" encoding="UTF-8"?>
        <srv:container xmlns="http://symfony.com/schema/dic/security"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xmlns:srv="http://symfony.com/schema/dic/services"
            xsi:schemaLocation="http://symfony.com/schema/dic/services
                http://symfony.com/schema/dic/services/services-1.0.xsd">

            <config>
                <firewall stateless="true">
                    <http-basic />
                </firewall>
            </config>
        </srv:container>

    .. code-block:: php

        // app/config/security.php
        $container->loadFromExtension('security', array(
            'firewalls' => array(
                'main' => array('http_basic' => array(), 'stateless' => true),
            ),
        ));

.. note::

    Se si usa un form di login, Symfony creerà un cookie anche se si imposta
    ``stateless`` a ``true``.

Considerazioni finali
---------------------

Ora sappiamo più di qualche base sulla sicurezza. Le parti più difficili
coinvolgono i requisiti personalizzati: una strategia di autenticazione
personalizzata (p.e. token API), logica di autorizzazione complessa e molte altre cose
(perché la sicurezza è complessa!).

Fortunatamente, ci sono molte :doc:`ricette sulla sicurezza </cookbook/security/index>`,
che descrivono molte di queste situazioni. Inoltre, vedere la
:doc:`sezione di riferimento della sicurezza </reference/configuration/security>`. Molte
opzioni non hanno dettagli specifici, ma analizzare l'intero albero di configurazione
potrebbe essere utile.

Buona fortuna!

Saperne di più con il ricettario
--------------------------------

* :doc:`Forzare HTTP/HTTPS </cookbook/security/force_https>`
* :doc:`Impersonare un utente </cookbook/security/impersonating_user>`
* :doc:`Blacklist di utenti per indirizzo IP </cookbook/security/voters>`
* :doc:`Access Control List (ACL) </cookbook/security/acl>`
* :doc:`/cookbook/security/remember_me`
* :doc:`/cookbook/security/multiple_user_providers`

.. _`strumento online`: https://www.dailycred.com/blog/12/bcrypt-calculator
.. _`frameworkextrabundle documentation`: http://symfony.com/doc/current/bundles/SensioFrameworkExtraBundle/index.html
.. _`HWIOAuthBundle`: https://github.com/hwi/HWIOAuthBundle
