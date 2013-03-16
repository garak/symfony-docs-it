.. index::
   single: Riferimento configurazione; Framework

Configurazione di FrameworkBundle ("framework")
===============================================

Questo riferimento è ancora provvisorio. Dovrebbe essere accurato, ma
non sono pienamente coperate tutte le opzioni.

``FrameworkBundle`` contiene la maggior parte delle funzionalità di base del
framework e può essere configurato sotto la chiave ``framework`` nella
configurazione della propria applicazione. Include impostazioni relative a
sessioni, traduzione, form, validazione, rotte e altro.

Configurazione
--------------

* `secret`_
* `ide`_
* `test`_
* `trust_proxy_headers`_
* `form`_
    * enabled
* `csrf_protection`_
    * enabled
    * field_name
* `session`_
    * `cookie_lifetime`_
    * `cookie_path`_
    * `cookie_domain`_
    * `cookie_secure`_
    * `cookie_httponly`_
    * `gc_divisor`_
    * `gc_probability`_
    * `gc_maxlifetime`_
    * `save_path`_
* `templating`_
    * `assets_base_urls`_
    * `assets_version`_
    * `assets_version_format`_

secret
~~~~~~

**tipo**: ``stringa`` **obbligatorio**

Una stringa che dovrebbe essere univoca nella propria applicazione. In pratica,
è usta per generare il token anti-CSRF, ma potrebbe essere usata in ogni altro
contesto in cui è utili avere una stringa univoca. Diventa il parametro del
contenitore di servizi di nome ``kernel.secret``.

ide
~~~

**tipo**: ``stringa`` **predefinito**: ``null``

Se si usa un IDE, come TextMate o Mac Vim, allora Symfony può cambiare tutti i
percorsi del file nei messaggi di eccezione in collegamenti, che apriranno i
file nel proprio IDE.

Se si usa TextMate o Mac Vim, si possono usare semplicemente uno dei seguenti
valori:

* ``textmate``
* ``macvim``

Si può anche specificare una stringa con un collegamento personalizzato. Se lo si fa,
tutti i simboli percentuale (``%``) devono essere raddoppiati, per escape. Per esempio,
la stringa completa per TextMate sarebbe come questa:

.. code-block:: yaml

    framework:
        ide:  "txmt://open?url=file://%%f&line=%%l"

Ovviamente, poiché ogni sviluppatore usa un IDE diverso, è meglio impostarlo a livello
di sistema. Lo si può fare impostando il valore ``xdebug.file_link_format``
di php.ini alla stringa del collegamento. Se questo valore di configurazione è
impostato, non occorre specificare l'opzione ``ide``.

.. _reference-framework-test:

test
~~~~

**tipo**: ``booleano``

Se questo parametro di configurazione è presente e non è ``false``, saranno
caricati i servizi correlati ai test della propria applicazione (p.e. ``test.client``).
Questa impostazione dovrebbe essere presete nel proprio ambiente ``test`` (solitamente
tramite ``app/config/config_test.yml``). Per maggiori informazioni, vedere :doc:`/book/testing`.

trusted_proxies
~~~~~~~~~~~~~~~

**tipo**: ``array``

Configura gli indirizzi IP di cui fidarsi come proxy. Per maggiori dettagli,
vedere :doc:`/components/http_foundation/trusting_proxies`.

.. configuration-block::

    .. code-block:: yaml

        framework:
            trusted_proxies:  [192.0.0.1]

    .. code-block:: xml

        <framework:config trusted-proxies="192.0.0.1">
            <!-- ... -->
        </framework>

    .. code-block:: php

        $container->loadFromExtension('framework', array(
            'trusted_proxies' => array('192.0.0.1'),
        ));

trust_proxy_headers
~~~~~~~~~~~~~~~~~~~

.. caution::

    L'opzione ``trust_proxy_headers`` è deprecata e sarà rimossa in
    Symfony 2.3. Vedere `trusted_proxies`_ e :doc:`/components/http_foundation/trusting_proxies`
    per i dettagli su come fidarsi in modo corretto dei dati dei proxy.

**tipo**: ``booleano``

Configura se gli header HTTP (come ``HTTP_X_FORWARDED_FOR``, ``X_FORWARDED_PROTO`` e
``X_FORWARDED_HOST``) siano affidabili indicazioni per una connessione SSL. L'impostazione
predefinita è ``false`` e quindi solo le connessioni SSL_HTTPS sono considerate sicure.

Si dovrebbe abilitare questa impostazione se l'applicazione è dietro un reverse proxy.

.. _reference-framework-form:

form
~~~~

csrf_protection
~~~~~~~~~~~~~~~

session
~~~~~~~

cookie_lifetime
...............

.. versionadded:: 2.1
    Questa opzione in precedenza si chiamava ``lifetime``

**tipo**: ``intero`` **predefinito**: ``0``

Determina la durata della sessione in secondi. Per impostazione predefinita, sarà
``0``, che vuol dire che il cookie è valido per la durata della sessione del browser.

cookie_path
...........

.. versionadded:: 2.1
    Questa opzione in precedenza si chiamava ``path``

**tipo**: ``stringa`` **predefinito**: ``/``

Determina il percorso da impostare nel cookie di sessione. Per impostazione predefinita è ``/``.

cookie_domain
.............

.. versionadded:: 2.1
    Questa opzione in precedenza si chiamava ``domain``

**tipo**: ``stringa`` **predefinito**: ``''``

Determina il dominio da impostare nel cookie di sessione. Per impostazione predefinita è vuoto,
che vuol dire che sarà usato il dominio del server che ha generato il cookie,
in accordo alle specifiche.

cookie_secure
.............

.. versionadded:: 2.1
    Questa opzione in precedenza si chiamava ``secure``

**tipo**: ``booleano`` **predefinito**: ``false``

Determina se i cookie debbano essere inviati su una connessione sicura.

cookie_httponly
...............

.. versionadded:: 2.1
    Questa opzione in precedenza si chiamava ``httponly``

**tipo**: ``booleano`` **predefinito**: ``false``

Determina se i cookie debbano essere accessibili solo tramite protocollo HTTP.
Vuol dire che i cookie non saranno accessibili da linguaggi di scripting, come
JavaScript. Questa impostazione può aiutare a ridurre furti di identità
tramite attacchi XSS.

gc_probability
..............

.. versionadded:: 2.1
    L'opzione ``gc_probability`` è stata aggiunta nella versione 2.1

**tipo**: ``intero`` **predefinito**: ``1``

Definisce la probabilità che il processo del garbage collector parta a
ogni inizializzazione della sessione. La probabilità è calcolata usando
``gc_probability`` / ``gc_divisor``, p.e. 1/100 vuol dire che c'è una probabilità dell'1%
che il processo parta, in ogni richiesta.

gc_divisor
..........

.. versionadded:: 2.1
    L'opzione ``gc_divisor`` è stata aggiunta nella versione 2.1

**tipo**: ``intero`` **predefinito**: ``100``

Vedere `gc_probability`_.

gc_maxlifetime
..............

.. versionadded:: 2.1
    L'opzione ``gc_maxlifetime`` è stata aggiunta nella versione 2.1

**tipo**: ``intero`` **predefinito**: ``14400``

Determina il numero di secondi dopo i quali i dati saranno visti come "garbage"
e quindi potenzialmente cancellati. Il garbage collector può intervenire a inizio sessione
e dipende da `gc_divisor`_ e `gc_probability`_.

save_path
.........

**tipo**: ``stringa`` **predefinito**: ``%kernel.cache.dir%/sessions``

Determina il parametro da passare al gestore di salvataggio. Se si sceglie il gestore
file (quello predefinito), è il percorso in cui saranno creati i file.

templating
~~~~~~~~~~

assets_base_urls
................

**predefinito**: ``{ http: [], ssl: [] }``

Questa opzione consente di definire URL di base da usare per i riferimenti alle risorse
nelle pagine ``http`` e ``https``. Si può fornire un valore stringa al posto di un
array a elementi singoli. Se si forniscono più URL base, Symfony2 ne sceglierà una
dall'elenco ogni volta che genera il percorso di una risorsa.

Per praticità, ``assets_base_urls`` può essere impostata direttamente con una stringa
o array di stringhe, che saranno automaticamente organizzate in liste di URL base per
le richieste ``http`` e ``https``. Se un URL inizia con ``https://`` o
è `protocol-relative`_ (cioè inizia con `//`), sarà aggiunto a entrambe le
liste. Gli URL che iniziano con ``http://`` saranno aggiunti solo alla lista
``http``.

.. versionadded:: 2.1
    Diversamente dalla maggior parte dei blocchi di configurazione, i valori successivi di ``assets_base_urls``
    si sovrascrivono a vicenda invece di essere fusi. È stato scelto questo comportamento
    perché solitamente gli sviluppatori definiscono URL di base per ogni ambiente.
    Dato che la maggior parte dei progetti tende a ereditare configurazioni
    (p.e. ``config_test.yml`` importa ``config_dev.yml``) e/o condividere una configurazione
    comune di base (p.e. ``config.yml``), la fusione avrebbe portato a un insieme di URL di base
    per ambienti multipli.

.. _ref-framework-assets-version:

assets_version
..............

**tipo**: ``stringa``

Questa opzione è usata per spaccare le risorse in cache, aggiungendo globalmente
un parametro di query a tutti i percorsi delle risorse (p.e. ``/images/logo.png?v2``).
Si applica solo alle risorse rese tramite la funzione ``asset`` di Twig (o al suo equivalente
PHP), come pure alle risorse rese con Assetic.

Per esempio, si supponga di avere il seguente:

.. configuration-block::

    .. code-block:: html+jinja

        <img src="{{ asset('images/logo.png') }}" alt="Symfony!" />

    .. code-block:: php

        <img src="<?php echo $view['assets']->getUrl('images/logo.png') ?>" alt="Symfony!" />

Per impostazione predefinita, renderà un percorso alla propria immagine, come ``/images/logo.png``.
Ora, attivare l'opzione ``assets_version``:

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        framework:
            # ...
            templating: { engines: ['twig'], assets_version: v2 }

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <framework:templating assets-version="v2">
            <framework:engine id="twig" />
        </framework:templating>

    .. code-block:: php

        // app/config/config.php
        $container->loadFromExtension('framework', array(
            // ...
            'templating'      => array(
                'engines' => array('twig'),
                'assets_version' => 'v2',
            ),
        ));

Ora, la stessa risora sarà resa come ``/images/logo.png?v2``. Se si usa questa
caratteristica, si *deve* incrementare a mano il valore di ``assets_version``, prima
di ogni deploy, in modo che il parametro della query cambi.

Si può anche contollare il funzionamento della stringa della query, tramite
l'opzione `assets_version_format`_.

assets_version_format
.....................

**tipo**: ``stringa`` **predefinito**: ``%%s?%%s``

Specifica uno schema per :phpfunction:`sprintf`, usato con l'opzione `assets_version`_
per costruire il percorso della risorsa. Per impostazione predefinita, lo schema aggiunge
la versione della risorsa alla stringa della query. Per esempio, se ``assets_version_format`` è
impostato a ``%%s?version=%%s`` e ``assets_version`` è impostato a ``5``, il percorso della
risorsa sarà ``/images/logo.png?version=5``.

.. note::

    Tutti i simboli percentuale (``%``) nel formato devono essere raddoppiati per
    escape. Senza escape, i valori sarebbero inavvertitamente interpretati come
    :ref:`book-service-container-parameters`.

.. tip::

    Alcuni CDN non sopportano la spaccatura della cache tramie stringa della query,
    quindi si rende necessario l'inserimento della versione nel vero percorso della risorsa.
    Fortunatamente, ``assets_version_format`` non è limitato alla produzione di stringhe di query con versioni.

    Lo schema riceve il percorso originale della risorsa e la versione come primo e
    secondo parametro, rispettivamente. Poiché il percorso della risorsa è un parametro,
    non possiamo modificarlo al volo (p.e. ``/images/logo-v5.png``). Tuttavia, possiamo
    aggiungere un prefisso al percorso della risorsa, usando uno schema ``version-%%2$s/%%1$s``,
    che risulta nel percorso ``version-5/images/logo.png``.

    Si possono quindi usare le riscritture degli URL, per togliere il prefissod con la versione
    prima di servire la risorsa. In alternativa, si possono copiare le risorse nel percorso
    appropriato con la versione, come parte del processo di deploy, e non usare la riscrittura
    degli URL. L'ultima opzione è utile se si vuole che le vecchie versioni delle risorse restino
    disponibili nei loro URL originari.

Configurazione predefinita completa
-----------------------------------

.. configuration-block::

    .. code-block:: yaml

        framework:
            charset:              ~
            secret:               ~
            trust_proxy_headers:  false
            trusted_proxies:      []
            ide:                  ~
            test:                 ~
            default_locale:       en

            # configurazione dei form
            form:
                enabled:              false
            csrf_protection:
                enabled:              false
                field_name:           _token

            # configurazione di esi
            esi:
                enabled:              false

            # configurazione dei frammenti
            fragments:
                enabled:              false
                path:                 /_fragment

            # configurazione del profilatore
            profiler:
                enabled:              false
                only_exceptions:      false
                only_master_requests:  false
                dsn:                  file:%kernel.cache_dir%/profiler
                username:
                password:
                lifetime:             86400
                matcher:
                    ip:                   ~

                    # usare il formato urldecoded
                    path:                 ~ # Esempio: ^/percorso della risorsa/
                    service:              ~

            # configurazione delle rotte
            router:
                resource:             ~ # Obbligatorio
                type:                 ~
                http_port:            80
                https_port:           443

                # impostare a true per lanciare un'eccezione se un parametro non corrisponde ai requisiti
                # impostare a false per disabilitare le eccezioni se un parametro non corrisponde ai requisiti (e restituire null)
                # impostare a null per disabilitare la verifica dei requisiti dei parametri
                # true è preferibile durante lo sviluppo, mentre false o null sono preferibili in produzione
                strict_requirements:  true

            # configurazione della sessione
            session:
                # DEPRECATO! La sessione parte su richiesta
                auto_start:           false
                storage_id:           session.storage.native
                handler_id:           session.handler.native_file
                name:                 ~
                cookie_lifetime:      ~
                cookie_path:          ~
                cookie_domain:        ~
                cookie_secure:        ~
                cookie_httponly:      ~
                gc_divisor:           ~
                gc_probability:       ~
                gc_maxlifetime:       ~
                save_path:            %kernel.cache_dir%/sessions

                # DEPRECATO! Usare: cookie_lifetime
                lifetime:             ~

                # DEPRECATO! Usare: cookie_path
                path:                 ~

                # DEPRECATO! Usare: cookie_domain
                domain:               ~

                # DEPRECATO! Usare: cookie_secure
                secure:               ~

                # DEPRECATO! Usare: cookie_httponly
                httponly:             ~

            # configurazione dei template
            templating:
                assets_version:       ~
                assets_version_format:  %%s?%%s
                hinclude_default_template:  ~
                form:
                    resources:

                        # Predefinito:
                        - FrameworkBundle:Form
                assets_base_urls:
                    http:                 []
                    ssl:                  []
                cache:                ~
                engines:              # Obbligatorio

                    # Esempio:
                    - twig
                loaders:              []
                packages:

                    # Un insieme di nomi di pacchetti
                    nome_di_un_pacchetto:
                        version:              ~
                        version_format:       %%s?%%s
                        base_urls:
                            http:                 []
                            ssl:                  []

            # configurazione della traduzione
            translator:
                enabled:              true
                fallback:             en

            # configurazione della validazione
            validation:
                enabled:              true
                cache:                ~
                enable_annotations:   false
                translation_domain:   validators

            # configurazione delle annotazioni
            annotations:
                cache:                file
                file_cache_dir:       %kernel.cache_dir%/annotations
                debug:                %kernel.debug%


.. versionadded:: 2.1
    L'impostazione ```framework.session.auto_start`` è stata rimossa in Symfony2.1,
    che inizia la sessione su richiesta.

.. _`protocol-relative`: http://tools.ietf.org/html/rfc3986#section-4.2
