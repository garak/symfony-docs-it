.. index::
   single: Sicurezza; Forzare HTTPS

Forzare HTTPS o HTTP per URL diversi
====================================

Si possono forzare delle aree di un sito a usare il protocollo HTTPS nella
configurazione della sicurezza. Lo si può fare tramite le regole ``access_control``,
usando l'opzione ``requires_channel``. Per esempio, se si vogliono forzare tutti gli URL
che iniziano per ``/secure`` a usare HTTPS, si può usare la seguente configurazione:

.. configuration-block::

        .. code-block:: yaml

            access_control:
                - { path: ^/secure, roles: ROLE_ADMIN, requires_channel: https }

        .. code-block:: xml

            <access-control>
                <rule path="^/secure" role="ROLE_ADMIN" requires_channel="https" />
            </access-control>

        .. code-block:: php

            'access_control' => array(
                array(
                    'path'             => '^/secure',
                    'role'             => 'ROLE_ADMIN',
                    'requires_channel' => 'https',
                ),
            ),

Il form di login deve consentire l'accesso anonimo, altrimenti l'utente sarebbe
impossibilitato ad autenticarsi. Per forzarlo a usare HTTPS, si possono usare ancora
le regole ``access_control`` con il ruolo
``IS_AUTHENTICATED_ANONYMOUSLY``:

.. configuration-block::

        .. code-block:: yaml

            access_control:
                - { path: ^/login, roles: IS_AUTHENTICATED_ANONYMOUSLY, requires_channel: https }

        .. code-block:: xml

            <access-control>
                <rule path="^/login"
                      role="IS_AUTHENTICATED_ANONYMOUSLY"
                      requires_channel="https" />
            </access-control>

        .. code-block:: php

            'access_control' => array(
                array(
                    'path'             => '^/login',
                    'role'             => 'IS_AUTHENTICATED_ANONYMOUSLY',
                    'requires_channel' => 'https',
                ),
            ),

È anche possibile specificare l'uso di HTTPS nella configurazione delle rotte,
vedere :doc:`/cookbook/routing/scheme` per maggiori dettagli.
