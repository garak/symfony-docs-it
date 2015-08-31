Come funziona access_control?
=============================

Per ogni richiesta in arrivo, Symfony verifica ogni voce in ``access_control``,
per trovarne *una* che corrisponda a tale richiesta. Non apprna trovata una corrispondente voce in
``access_control``, si ferma: solo la **prima** voce corrsipondente in ``access_control``
viene usata per controllare l'accesso.

Ciascun ``access_control`` ha varie opzioni, che configurano diversi
aspetti:

#. :ref:`se la richiesta in arrivo corrisponde a questa voce di access_control <security-book-access-control-matching-options>`
#. :ref:`una volta corrisposta, se si devono mettere in atto restrizioni di accesso <security-book-access-control-enforcement-options>`:

.. _security-book-access-control-matching-options:

1. Opzioni di corrispondenza
----------------------------

Symfony crea un'istanza di :class:`Symfony\\Component\\HttpFoundation\\RequestMatcher`
per ciascuna voce di ``access_control``, che determina se un dato
access_control debba essere usato o meno per una richiesta. Le seguenti opzioni di ``access_control``
sono usato per cercare una corrispondenza:

* ``path``
* ``ip`` o ``ips``
* ``host``
* ``methods``

Prendiamo come esempio le seguenti voci di ``access_control``:

.. configuration-block::

    .. code-block:: yaml

        # app/config/security.yml
        security:
            # ...
            access_control:
                - { path: ^/admin, roles: ROLE_USER_IP, ip: 127.0.0.1 }
                - { path: ^/admin, roles: ROLE_USER_HOST, host: symfony\.com$ }
                - { path: ^/admin, roles: ROLE_USER_METHOD, methods: [POST, PUT] }
                - { path: ^/admin, roles: ROLE_USER }

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
                    <rule path="^/admin" role="ROLE_USER_IP" ip="127.0.0.1" />
                    <rule path="^/admin" role="ROLE_USER_HOST" host="symfony\.com$" />
                    <rule path="^/admin" role="ROLE_USER_METHOD" method="POST, PUT" />
                    <rule path="^/admin" role="ROLE_USER" />
                </access-control>
            </config>
        </srv:container>

    .. code-block:: php

        // app/config/security.php
        $container->loadFromExtension('security', array(
            // ...
            'access_control' => array(
                array(
                    'path' => '^/admin',
                    'role' => 'ROLE_USER_IP',
                    'ip' => '127.0.0.1',
                ),
                array(
                    'path' => '^/admin',
                    'role' => 'ROLE_USER_HOST',
                    'host' => 'symfony\.com$',
                ),
                array(
                    'path' => '^/admin',
                    'role' => 'ROLE_USER_METHOD',
                    'method' => 'POST, PUT',
                ),
                array(
                    'path' => '^/admin',
                    'role' => 'ROLE_USER',
                ),
            ),
        ));

Per ciascuna richiesta in arrivo, Symfony deciderà quale ``access_control``
usare, in base a URI, indirizzo IP del client, nome dell'host in entrata e
metodo della richiesta. È importante ricordare che viene usata solo la prima regola corrispondente
e, se ``ip``, ``host`` o ``method`` non sono specificati, un ``access_control`` corrisponderà
a qualsiasi ``ip``, ``host`` o ``method``:

+-----------------+-------------+-------------+------------+--------------------------------+-------------------------------------------------------------+
| URI             | IP          | HOST        | METODO     | ``access_control``             | Perché?                                                     |
+=================+=============+=============+============+================================+=============================================================+
| ``/admin/user`` | 127.0.0.1   | example.com | GET        | regola #1 (``ROLE_USER_IP``)   | L'URI corrisponde a ``path`` e l'IP corrisponde a ``ip``.   |
+-----------------+-------------+-------------+------------+--------------------------------+-------------------------------------------------------------+
| ``/admin/user`` | 127.0.0.1   | symfony.com | GET        | regola #1 (``ROLE_USER_IP``)   | ``path`` e ``ip`` corrispondono. Corrisponderebbe anche     |
|                 |             |             |            |                                | ``ROLE_USER_HOST``, ma viene usata *solo* la **prima**      |
|                 |             |             |            |                                | corrispondenza di ``access_control``.                       |
+-----------------+-------------+-------------+------------+--------------------------------+-------------------------------------------------------------+
| ``/admin/user`` | 168.0.0.1   | symfony.com | GET        | regola #2 (``ROLE_USER_HOST``) | ``ip`` non soddisfa la prima regola, quindi viene usata     |
|                 |             |             |            |                                | la seconda (che corrisponde).                               |
+-----------------+-------------+-------------+------------+--------------------------------+-------------------------------------------------------------+
| ``/admin/user`` | 168.0.0.1   | symfony.com | POST       | regola #2 (``ROLE_USER_HOST``) | La seconda regola corrisponde ancora. Anche la terza regola |
|                 |             |             |            |                                | (``ROLE_USER_METHOD``) corrisponderebbe, ma solo il         |
|                 |             |             |            |                                | **primo** ``access_control`` corrispondente viene usato.    |
+-----------------+-------------+-------------+------------+--------------------------------+-------------------------------------------------------------+
| ``/admin/user`` | 168.0.0.1   | example.com | POST       | reg. #3 (``ROLE_USER_METHOD``) | ``ip`` e ``host`` non corrispondono alle prime due voci, ma |
|                 |             |             |            |                                | il terzo, ``ROLE_USER_METHOD``, corrisponde ed è usato.     |
+-----------------+-------------+-------------+------------+--------------------------------+-------------------------------------------------------------+
| ``/admin/user`` | 168.0.0.1   | example.com | GET        | regola #4 (``ROLE_USER``)      | ``ip``, ``host`` e ``method`` non fanno corrispondere le    |
|                 |             |             |            |                                | prime tre voci. Ma poiché URI corrisponde allo schema       |
|                 |             |             |            |                                | ``path`` della voce ``ROLE_USER``, è usato.                 |
+-----------------+-------------+-------------+------------+--------------------------------+-------------------------------------------------------------+
| ``/pippo``      | 127.0.0.1   | symfony.com | POST       | nessuna corrispondenza         | Non corrisponde ad alcun ``access_control``, perché il suo  |
|                 |             |             |            |                                | URI non corrisponde a nessuno dei valori di ``path``.       |
+-----------------+-------------+-------------+------------+--------------------------------+-------------------------------------------------------------+

.. _security-book-access-control-enforcement-options:

2. Controllo degli accessi
--------------------------

Una volta che Symfony ha deciso quale voce di ``access_control`` eventualmente corrisponde,
*applica* delle restrizioni all'accesso, in base alle opzioni ``roles`` e
``requires_channel``:

* ``role`` Se l'utente non ha il ruolo o i ruoli dati, l'accesso è negato
  (internamente, viene sollevata un'eccezione
  :class:`Symfony\\Component\\Security\\Core\\Exception\\AccessDeniedException`);

* ``requires_channel`` Se il canale della richiesta in entrata (p.e. ``http``)
  non corrisponde a questo valore (p.e. ``https``), l'utente sarà rinviato
  (p.e. rinviato da ``http`` a ``https`` o viceversa).

.. tip::

    Se l'accesso è negato, il sistema proverà ad autenticare l'utente, se non lo
    è già (p.e. rinviare l'utente alla pagina di login). Se l'utente è già
    connesso, gli sarà mostrata la pagina di errore 403 (accesso negato). Vedere
    :doc:`/cookbook/controller/error_pages` per maggiori informazioni.

.. _book-security-securing-ip:

Corrispondere access_control per IP
-----------------------------------

Ci sono alcune situazioni in cui occorre avere una voce ``access_control``
che corrisponda *solo* a richieste provenienti da alcuni indirizzi IP.
Per esempio, si potrebbe fare in questo modo per negare accesso ad alcuni URL per
ogni richiesta, *tranne* quelle provenienti da un server fidato.

.. caution::

    Come si vedrà nelle spiegazioni successive, l'opzione ``ips``
    non restringe a uno specifico indirizzo IP. Invece, usando la chiave ``ips``,
    la voce ``access_control`` corrisponderà solo a quello specifico indirizzo IP
    e gli utenti che accedono da indirizzi IP diversi passeranno oltre nella
    lista degli ``access_control``.

Ecco un esempio di come si possono configurare alcuni schemi di URL ``/internal*``,
in modo che siano accessibili solo da richieste provenienti dal server locale:

.. configuration-block::

    .. code-block:: yaml

        # app/config/security.yml
        security:
            # ...
            access_control:
                #
                - { path: ^/internal, roles: IS_AUTHENTICATED_ANONYMOUSLY, ips: [127.0.0.1, ::1] }
                - { path: ^/internal, roles: ROLE_NO_ACCESS }

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
                    <rule path="^/esi" role="IS_AUTHENTICATED_ANONYMOUSLY"
                        ips="127.0.0.1, ::1" />
                    <rule path="^/esi" role="ROLE_NO_ACCESS" />
                </access-control>
            </config>
        </srv:container>

    .. code-block:: php

        // app/config/security.php
        $container->loadFromExtension('security', array(
            // ...
            'access_control' => array(
                array(
                    'path' => '^/esi',
                    'role' => 'IS_AUTHENTICATED_ANONYMOUSLY',
                    'ips' => '127.0.0.1, ::1'
                ),
                array(
                    'path' => '^/esi',
                    'role' => 'ROLE_NO_ACCESS'
                ),
            ),
        ));

Eccome come funziona quando il percorso è ``/internal/something`` e la richiesta
proviene dall'indirizzo IP esterno ``10.0.0.1``:

* La prima regola è ignorata, perché ``path`` corrisponde, ma l'indirizzo
  IP non è tra quelli elencati;

* La seconda regola è abilitata (essendo l'unica condizione
  ``path``) e quindi corrisponde. Se ci si assicura che nessun utente abbia il ruolo
  ``ROLE_NO_ACCESS``, l'accesso è negato (``ROLE_NO_ACCESS`` può essere qualsiasi cosa
  che non sia un ruolo esistente, serve solo come trucco per negare sempre
  l'accesso).

Se la stessa richiesta viene invece da ``127.0.0.1`` o ``::1`` (l'indirizzo di loopback
IPv6):

* Ora, la prima regola è abilitata, perché sia ``path`` sia
  ``ip`` corrispondono: l'accesso è consentito, perché l'utente ha sempre il ruolo
  ``IS_AUTHENTICATED_ANONYMOUSLY``.

* La seconda regola non è esaminata, perché la prima ha trovato corrispondenza.

.. _book-security-securing-channel:

Protezione tramite canale (http, https)
---------------------------------------

Si può anche richiedere che un utente acceda un URL tramite SSL. Basta usare il parametro
``requires_channel`` in una voce di ``access_control``. Se tale
``access_control`` trova corrispondenza a la richiesta usa il canale ``http``,
l'utente sarà rinviato a ``https``:

.. configuration-block::

    .. code-block:: yaml

        # app/config/security.yml
        security:
            # ...
            access_control:
                - { path: ^/cart/checkout, roles: IS_AUTHENTICATED_ANONYMOUSLY, requires_channel: https }

    .. code-block:: xml

        <!-- app/config/security.xml -->
        <?xml version="1.0" encoding="UTF-8"?>
        <srv:container xmlns="http://symfony.com/schema/dic/security"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xmlns:srv="http://symfony.com/schema/dic/services"
            xsi:schemaLocation="http://symfony.com/schema/dic/services
                http://symfony.com/schema/dic/services/services-1.0.xsd">

            <access-control>
                <rule path="^/cart/checkout"
                    role="IS_AUTHENTICATED_ANONYMOUSLY"
                    requires-channel="https" />
            </access-control>
        </srv:container>

    .. code-block:: php

        // app/config/security.php
        $container->loadFromExtension('security', array(
            'access_control' => array(
                array(
                    'path' => '^/cart/checkout',
                    'role' => 'IS_AUTHENTICATED_ANONYMOUSLY',
                    'requires_channel' => 'https',
                ),
            ),
        ));
