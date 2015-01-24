.. index::
   single: Riferimento configurazione; Framework

Configurazione di FrameworkBundle ("framework")
===============================================

Questo riferimento è ancora provvisorio. Dovrebbe essere accurato, ma
non sono pienamente coperte tutte le opzioni.

FrameworkBundle contiene la maggior parte delle funzionalità di base del
framework e può essere configurato sotto la chiave ``framework`` nella
configurazione di un'applicazione. Include impostazioni relative a
sessioni, traduzione, form, validazione, rotte e altro.

Configurazione
--------------

* `secret`_
* `http_method_override`_
* `ide`_
* `test`_
* `default_locale`_
* `trusted_proxies`_
* `csrf_protection`_
    * enabled
    * field_name (deprecated)
* `form`_
    * enabled
    * csrf_protection
        * enabled
        * field_name
* `session`_
    * `name`_
    * `cookie_lifetime`_
    * `cookie_path`_
    * `cookie_domain`_
    * `cookie_secure`_
    * `cookie_httponly`_
    * `gc_divisor`_
    * `gc_probability`_
    * `gc_maxlifetime`_
    * `save_path`_
* `serializer`_
    * :ref:`enabled<serializer.enabled>`
* `templating`_
    * `assets_base_urls`_
    * `assets_version`_
    * `assets_version_format`_
* `profiler`_
    * `collect`_
    * :ref:`enabled <profiler.enabled>`
* `translator`_
    * :ref:`enabled <translator.enabled>`
    * `fallback`_
    * `logging`_
* `property_accessor`_
    * `magic_call`_
    * `throw_exception_on_invalid_index`_
* `validation`_
    * `cache`_
    * `enable_annotations`_
    * `translation_domain`_
    * `strict_email`_
    * `api`_

secret
~~~~~~

**tipo**: ``stringa`` **obbligatorio**

Una stringa che dovrebbe essere univoca nella propria applicazione. In pratica,
è usata per generare il token anti-CSRF, ma potrebbe essere usata in ogni altro
contesto in cui è utile avere una stringa univoca. Diventa il parametro del
contenitore di servizi di nome ``kernel.secret``.

.. _configuration-framework-http_method_override:

http_method_override
~~~~~~~~~~~~~~~~~~~~

.. versionadded:: 2.3
    L'opzione ``http_method_override`` è nuova in Symfony 2.3.

**type**: ``booleano`` **predefinito**: ``true``

Determina se il parametro ``_method`` della richiesta sia usato come metodo HTTP inteso
sulle richieste POST. Se abilitato,
:method:`Request::enableHttpMethodParameterOverride <Symfony\\Component\\HttpFoundation\\Request::enableHttpMethodParameterOverride>`
è richiamato automaticamente. Diventa un parametro del contenitore di servizi, con nome
``kernel.http_method_override``. Per maggiori informazioni, vedere
:doc:`/cookbook/routing/method_parameters`.

ide
~~~

**tipo**: ``stringa`` **predefinito**: ``null``

Se si usa un IDE, come TextMate o Mac Vim, allora Symfony può cambiare tutti i
percorsi del file nei messaggi di eccezione in collegamenti, che apriranno i
file nell'IDE specificato.

Se si usa TextMate o Mac Vim, si possono usare semplicemente uno dei seguenti
valori:

* ``textmate``
* ``macvim``
* ``emacs``
* ``sublime``

.. versionadded:: 2.3.14
    Gli editor ``emacs`` e ``sublime`` sono stati introdotti in Symfony 2.3.14.

Si può anche specificare una stringa con un collegamento personalizzato. Se lo si fa,
tutti i simboli percentuale (``%``) devono essere raddoppiati, per escape. Per esempio,
la stringa completa per `PhpStormOpener`_ sarebbe come questa:

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        framework:
            ide: "pstorm://%%f:%%l"

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <?xml version="1.0" encoding="UTF-8" ?>
        <container xmlns="http://symfony.com/schema/dic/services"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xmlns:framework="http://symfony.com/schema/dic/symfony"
            xsi:schemaLocation="http://symfony.com/schema/dic/services http://symfony.com/schema/dic/services/services-1.0.xsd
                http://symfony.com/schema/dic/symfony http://symfony.com/schema/dic/symfony/symfony-1.0.xsd">

            <framework:config ide="pstorm://%%f:%%l" />
        </container>

    .. code-block:: php

        // app/config/config.php
        $container->loadFromExtension('framework', array(
            'ide' => 'pstorm://%%f:%%l',
        ));

Ovviamente, poiché ogni sviluppatore usa un IDE diverso, è meglio impostarlo a livello
di sistema. Lo si può fare impostando il valore ``xdebug.file_link_format``
di php.ini alla stringa del collegamento. Se questo valore di configurazione è
impostato, non occorre specificare l'opzione ``ide``.

.. _reference-framework-test:

test
~~~~

**tipo**: ``booleano``

Se questo parametro di configurazione è presente e non è ``false``, saranno
caricati i servizi correlati ai test dell'applicazione (p.e. ``test.client``).
Questa impostazione dovrebbe essere presente in ambiente ``test`` (solitamente
tramite ``app/config/config_test.yml``). Per maggiori informazioni, vedere :doc:`/book/testing`.

.. _reference-framework-trusted-proxies:

default_locale
~~~~~~~~~~~~~~

**tipo**: ``stringa`` **predefinito**: ``en``

Opzione usata se il parametro ``_locale`` non è stato impostato nelle rotte. 
Diventa il parametro del contenitore dei servizi ``kernel.default_locale`` ed è
anche disponibile con il metodo
:method:`Request::getDefaultLocale <Symfony\\Component\\HttpFoundation\\Request::getDefaultLocale>`.


trusted_proxies
~~~~~~~~~~~~~~~

**tipo**: ``array``

Configura gli indirizzi IP di cui fidarsi come proxy. Per maggiori dettagli,
vedere :doc:`/components/http_foundation/trusting_proxies`.

.. versionadded:: 2.3
    È stato introdotto il supporto per la notazione CIDR, quindi si possono indicare
    intere sotto-reti (p.e. ``10.0.0.0/8``, ``fc00::/7``).

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        framework:
            trusted_proxies:  [192.0.0.1, 10.0.0.0/8]

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <?xml version="1.0" encoding="UTF-8" ?>
        <container xmlns="http://symfony.com/schema/dic/services"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xmlns:framework="http://symfony.com/schema/dic/symfony"
            xsi:schemaLocation="http://symfony.com/schema/dic/services http://symfony.com/schema/dic/services/services-1.0.xsd
                http://symfony.com/schema/dic/symfony http://symfony.com/schema/dic/symfony/symfony-1.0.xsd">

            <framework:config trusted-proxies="192.0.0.1, 10.0.0.0/8" />
        </container>

    .. code-block:: php

        // app/config/config.php
        $container->loadFromExtension('framework', array(
            'trusted_proxies' => array('192.0.0.1', '10.0.0.0/8'),
        ));

.. _reference-framework-form:

form
~~~~

csrf_protection
~~~~~~~~~~~~~~~

session
~~~~~~~

name
....

**tipo**: ``stringa`` **predefinito**: ``null``

Specifica in nome del cookie di sessione. Per impostazione predefinita, sarà utilizzato
il nome definito nel ``php.ini`` con la direttiva ``session.name``.

cookie_lifetime
...............

**tipo**: ``intero`` **predefinito**: ``null``

Determina la durata della sessione in secondi. Per impostazione predefinita, sarà ``null``,
che vuol dire che sarà usato il valore di ``session.cookie_lifetime`` preso da ``php.ini``.
Se si imposta questo valore a ``0``, il cookie è valido per la durata della sessione del
browser.

cookie_path
...........

**tipo**: ``stringa`` **predefinito**: ``/``

Determina il percorso da impostare nel cookie di sessione. Per impostazione predefinita è ``/``.

cookie_domain
.............

**tipo**: ``stringa`` **predefinito**: ``''``

Determina il dominio da impostare nel cookie di sessione. Per impostazione predefinita è vuoto,
che vuol dire che sarà usato il dominio del server che ha generato il cookie,
in accordo alle specifiche.

cookie_secure
.............

**tipo**: ``booleano`` **predefinito**: ``false``

Determina se i cookie debbano essere inviati su una connessione sicura.

cookie_httponly
...............

**tipo**: ``booleano`` **predefinito**: ``false``

Determina se i cookie debbano essere accessibili solo tramite protocollo HTTP.
Vuol dire che i cookie non saranno accessibili da linguaggi di scripting, come
JavaScript. Questa impostazione può aiutare a ridurre furti di identità
tramite attacchi XSS.

gc_probability
..............

**tipo**: ``intero`` **predefinito**: ``1``

Definisce la probabilità che il processo del garbage collector parta a
ogni inizializzazione della sessione. La probabilità è calcolata usando
``gc_probability`` / ``gc_divisor``, p.e. 1/100 vuol dire che c'è una probabilità dell'1%
che il processo parta, in ogni richiesta.

gc_divisor
..........

**tipo**: ``intero`` **predefinito**: ``100``

Vedere `gc_probability`_.

gc_maxlifetime
..............

**tipo**: ``intero`` **predefinito**: ``14400``

Determina il numero di secondi dopo i quali i dati saranno visti come "garbage"
e quindi potenzialmente cancellati. Il garbage collector può intervenire a inizio sessione
e dipende da `gc_divisor`_ e `gc_probability`_.

save_path
.........

**tipo**: ``stringa`` **predefinito**: ``%kernel.cache.dir%/sessions``

Determina il parametro da passare al gestore di salvataggio. Se si sceglie il gestore
file (quello predefinito), è il percorso in cui saranno creati i file.
Per maggiori informazioni, vedere :doc:`/cookbook/session/sessions_directory`.

Si può anche impostare questo  valore a quello di ``save_path`` di ``php.ini``, impostandolo
a ``null``:

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        framework:
            session:
                save_path: null

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <?xml version="1.0" encoding="UTF-8" ?>
        <container xmlns="http://symfony.com/schema/dic/services"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xmlns:framework="http://symfony.com/schema/dic/symfony"
            xsi:schemaLocation="http://symfony.com/schema/dic/services http://symfony.com/schema/dic/services/services-1.0.xsd
                http://symfony.com/schema/dic/symfony http://symfony.com/schema/dic/symfony/symfony-1.0.xsd">

            <framework:config>
                <framework:session save-path="null" />
            </framework:config>
        </container>

    .. code-block:: php

        // app/config/config.php
        $container->loadFromExtension('framework', array(
            'session' => array(
                'save_path' => null,
            ),
        ));

.. _configuration-framework-serializer:

serializer
~~~~~~~~~~

.. _serializer.enabled:

enabled
.......

**tipo**: ``booleano`` **predefinito**: ``false``

Se abilitare o meno il servizio ``serializer`` nel contenitore.

Per maggiori dettagli, vedere :doc:`/cookbook/serializer`.

templating
~~~~~~~~~~

assets_base_urls
................

**predefinito**: ``{ http: [], ssl: [] }``

Questa opzione consente di definire URL di base da usare per i riferimenti alle risorse
nelle pagine ``http`` e ``https``. Si può fornire un valore stringa al posto di un
array a elementi singoli. Se si forniscono più URL base, Symfony ne sceglierà una
dall'elenco ogni volta che genera il percorso di una risorsa.

Per praticità, ``assets_base_urls`` può essere impostata direttamente con una stringa
o array di stringhe, che saranno automaticamente organizzate in liste di URL base per
le richieste ``http`` e ``https``. Se un URL inizia con ``https://`` o
è `protocol-relative`_ (cioè inizia con `//`), sarà aggiunto a entrambe le
liste. Gli URL che iniziano con ``http://`` saranno aggiunti solo alla lista
``http``.

.. _ref-framework-assets-version:

assets_version
..............

**tipo**: ``stringa``

Questa opzione è usata per evitare che le risorse vadano in cache, aggiungendo globalmente
un parametro di query a tutti i percorsi delle risorse (p.e. ``/images/logo.png?v2``).
Si applica solo alle risorse rese tramite la funzione ``asset`` di Twig (o al suo equivalente PHP),
come pure alle risorse rese con Assetic.

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
        <?xml version="1.0" encoding="UTF-8" ?>
        <container xmlns="http://symfony.com/schema/dic/services"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xmlns:framework="http://symfony.com/schema/dic/symfony"
            xsi:schemaLocation="http://symfony.com/schema/dic/services http://symfony.com/schema/dic/services/services-1.0.xsd
                http://symfony.com/schema/dic/symfony http://symfony.com/schema/dic/symfony/symfony-1.0.xsd">

            <framework:templating assets-version="v2">
                <!-- ... -->
                <framework:engine>twig</framework:engine>
            </framework:templating>
        </container>

    .. code-block:: php

        // app/config/config.php
        $container->loadFromExtension('framework', array(
            // ...
            'templating'      => array(
                'engines'        => array('twig'),
                'assets_version' => 'v2',
            ),
        ));

Ora, la stessa risora sarà resa come ``/images/logo.png?v2``. Se si usa questa
caratteristica, si *deve* incrementare a mano il valore di ``assets_version``, prima
di ogni deploy, in modo che il parametro della query cambi.

È anche possibile impostare il valore della versione per singola risorsa (invece
di usare una versione globale, come fatto qui con ``v2``)). Vedere
:ref:`versioni per risorsa <book-templating-version-by-asset>` per i dettagli.

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

profiler
~~~~~~~~

.. _profiler.enabled:

enabled
.......

.. versionadded:: 2.2
    L'opzione ``enabled`` è stata aggiunta in Symfony 2.2. Precedentemente il profilatore
    poteva essere disabilitato solamente omettendo interamente la configurazione
    ``framework.profiler``.

**tipo**: ``booleano`` **predefinito**: ``false``

Il profilatore può essere abilitato impostando questa chiave a ``true``. Se si
usa Symfony Standard Edition, il profilatore è abilitato in ambiente ``dev``
e ``test``.

collect
.......

.. versionadded:: 2.3
    L'opzione ``collect`` è nuova in Symfony 2.3. Precedentemente, quando
    ``profiler.enabled``  era ``false``, il profilatore *era* effettivamente attivo,
    ma i raccoglitori disabilitati. Ora profilatore e raccoglitori sono
    controllabili separatamente.

**predefinito**: ``true``

Questa opzione configura il modo in cui il profilatore si comporta quando abilitato. Se
``true``, il profilatore raccoglie dati per ogni richiesta. Se si vogliono raccogliere
informazioni solo in casi specifici, impostare ``collect`` a ``false``
e attivare i raccoglitori di dati manualmente::

    $profiler->enable();

translator
~~~~~~~~~~

.. _translator.enabled:

enabled
.......

**tipo**: ``booleano`` **predefinito**: ``false``

Se abilitare o meno il servizio ``translator`` nel contenitore.

fallback
........

**predefinito**: ``en``

Opzione usata quando non viene trovata la chiave di traduzione del locale corrente.

Per maggiori dettagli, vedere :doc:`/book/translation`.

.. _reference-framework-translator-logging:

logging
.......

.. versionadded:: 2.6
    L'opzione ``logging`` è stata introdotta in Symfony 2.6.

**predefinito**: ``true`` in modalità di debug, ``false`` altrimenti.

Se ``true``, ogni volta che il traduttore non trova una traduzione per una chiave, la
salverà nel log. I log sono scritti sul canale ``translation`` e su
``debug`` per livelli per chiavi in cui ci sia una traduzione nel locale predefinito
e a livello ``warning`` se non c'è alcuna traduzione utilizzabile.

property_accessor
~~~~~~~~~~~~~~~~~

magic_call
..........

**tipo**: ``booleano`` **predefinito**: ``false``

Se abilitato, il servizio ``property_accessor`` usa il metodo
:ref:`magico __call() di PHP <components-property-access-magic-call>` quando
viene richiamato il metodo ``getValue()``.

throw_exception_on_invalid_index
................................

**tipo**: ``booleano`` **predefinito**: ``false``

Se abilitato, il servizio ``property_accessor`` lancerà un'eccezione se si
prova ad accedere a un indice non valido di un array.

validation
~~~~~~~~~~

cache
.....

**tipo**: ``stringa``

Questo valore è usato per deterimnare il servizio utilizzato per persistere i metadati di classe in una cache. Il
servizio deve implementare :class:`Symfony\\Component\\Validator\\Mapping\\Cache\\CacheInterface`.

enable_annotations
..................

**tipo**: ``Booleano`` **predefinito**: ``false``

Se questa opzione è abilitata, si possone definire vincoli di validazione tramite annotazioni.

translation_domain
..................

**tipo**: ``stringa`` **predefinito**: ``validators``

Il dominio di traduzione usato quando si traducono i messaggi di errore dei
vincoli di validazione.

strict_email
............

.. versionadded:: 2.5
    L'opzione ``strict_email`` è stata introdotta in Symfony 2.5.

**tipo**: ``Booleano`` **predefinito**: ``false``

Se questa opzione è abilitati, sarà usata la libreria `egulias/email-validator`_ dal
vincolo di validazione :doc:`/reference/constraints/Email`. Altrimenti,
il validare usa una semplice espressione regolare per validare indirizzi email.

api
...

.. versionadded:: 2.5
    L'opzione ``api`` è stata introdotta in Symfony 2.5.

**tipo**: ``stringa``

A partire da Symfony 2.5, il componente Validator ha introdotto una nuova
API di validazione. L'opzione ``api`` si usa per cambiare implementazione:

``2.4``
    Usa l'API di validazione compatibile con le vecchie versioni di Symfony.

``2.5``
    Usa l'API di validazione introdotta in Symfony 2.5.

``2.5-bc`` or ``auto``
    Se si omette un valore o si imposta ``api`` a ``2.5-bc`` o ``auto``,
    Symfony userà un'API compatibile sia con la vecchia
    implementazione che con quella ``2.5``. Occorre usare
    PHP 5.3.9 o successivi per poter usare questa implementazione.

Per salvare i log in ambiente ``prod``, configurare un
:doc:`gestore di canale </cookbook/logging/channels_handlers>` in ``config_prod.yml`` per il
canale ``translation`` e impostare ``level`` a ``debug``.

Configurazione predefinita completa
-----------------------------------

.. configuration-block::

    .. code-block:: yaml

        framework:
            secret:               ~
            http_method_override: true
            trusted_proxies:      []
            ide:                  ~
            test:                 ~
            default_locale:       en

            csrf_protection:
                enabled:              false
                field_name:           _token # Deprecato da 2.4, da rimuovere in 3.0. Usare invece form.csrf_protection.field_name

            # configurazione dei form
            form:
                enabled:              false
                csrf_protection:
                    enabled:          true
                    field_name:       ~

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
                collect:              true
                only_exceptions:      false
                only_master_requests: false
                dsn:                  "file:%kernel.cache_dir%/profiler"
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
                save_path:            "%kernel.cache_dir%/sessions"

            # configurazione dei serializer
            serializer:
               enabled: false

            # configurazione dei template
            templating:
                assets_version:       ~
                assets_version_format:  "%%s?%%s"
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

                    # Prototipo
                    nome:
                        version:              ~
                        version_format:       "%%s?%%s"
                        base_urls:
                            http:                 []
                            ssl:                  []

            # configurazione della traduzione
            translator:
                enabled:              false
                fallback:             en
                logging:              "%kernel.debug%"

            # configurazione della validazione
            validation:
                enabled:              false
                cache:                ~
                enable_annotations:   false
                translation_domain:   validators

            # configurazione delle annotazioni
            annotations:
                cache:                file
                file_cache_dir:       "%kernel.cache_dir%/annotations"
                debug:                "%kernel.debug%"

.. _`protocol-relative`: http://tools.ietf.org/html/rfc3986#section-4.2
.. _`PhpStormOpener`: https://github.com/pinepain/PhpStormOpener
.. _`egulias/email-validator`: https://github.com/egulias/EmailValidator
