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

* `charset`_
* `secret`_
* `ide`_
* `test`_
* `form`_
    * enabled
* `csrf_protection`_
    * enabled
    * field_name
* `session`_
    * `lifetime`_
* `templating`_
    * `assets_base_urls`_
    * `assets_version`_
    * `assets_version_format`_

charset
~~~~~~~

**tipo**: ``stringa`` **predefinito**: ``UTF-8``

Il set di caratteri usato nel framework. Diventa il parametro del contenitore
di servizi di nome ``kernel.charset``.

secret
~~~~~~

**tipo**: ``stringa`` **obbligatorio**

Una stringa che dovrebbe essere univoca nella propria applicaizone. In pratica,
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

.. _reference-framework-form:

form
~~~~

csrf_protection
~~~~~~~~~~~~~~~

session
~~~~~~~

lifetime
........

**tipo**: ``integer`` **predefinito**: ``86400``

Determina il tempo di scadeza della sessione, in secondi.

templating
~~~~~~~~~~

assets_base_urls
................

**predefinito**: ``{ http: [], https: [] }``

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

Specifica uno schema per `sprintf()`_, usato con l'opzione `assets_version`_
per costruire il percorso della risorsa. Per impostazione predefinita, lo schema
aggiunge la versione della risorsa alla stringa della query. Per esempio, se
``assets_version_format`` è impostato a ``%%s?version=%%s`` e ``assets_version`` è
impostato a ``5``, il percorso della risorsa sarà ``/images/logo.png?version=5``.

.. note::

    Tutti i simboli percentuale (``%``) nel formato devono essere raddoppiati per
    escape. Senza escape, i valori sarebbero inavvertitamente interpretati come
    :ref:`book-service-container-parameters`.

.. tip::

    Alcuni CDN non sopportano la spaccatura della cache tramie stringa della query,
    quindi si rende necessario l'inserimento della versione nel vero percorso della risorsa.
    Fortunatamente, ``assets_version_format`` non è limitato alla produzionoe di stringhe
    di query con versioni.

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

            # general configuration
            charset:              ~
            secret:               ~ # Required
            ide:                  ~
            test:                 ~
            trust_proxy_headers:  false

            # form configuration
            form:
                enabled:              true
            csrf_protection:
                enabled:              true
                field_name:           _token

            # esi configuration
            esi:
                enabled:              true

            # profiler configuration
            profiler:
                only_exceptions:      false
                only_master_requests:  false
                dsn:                  sqlite:%kernel.cache_dir%/profiler.db
                username:
                password:
                lifetime:             86400
                matcher:
                    ip:                   ~
                    path:                 ~
                    service:              ~

            # router configuration
            router:
                resource:             ~ # Required
                type:                 ~
                http_port:            80
                https_port:           443

            # session configuration
            session:
                auto_start:           ~
                default_locale:       en
                storage_id:           session.storage.native
                name:                 ~
                lifetime:             86400
                path:                 ~
                domain:               ~
                secure:               ~
                httponly:             ~

            # templating configuration
            templating:
                assets_version:       ~
                assets_version_format:  "%%s?%%s"
                assets_base_urls:
                    http:                 []
                    ssl:                  []
                cache:                ~
                engines:              # Required
                form:
                    resources:        [FrameworkBundle:Form]

                    # Example:
                    - twig
                loaders:              []
                packages:

                    # Prototype
                    name:
                        version:              ~
                        version_format:       ~
                        base_urls:
                            http:                 []
                            ssl:                  []

            # translator configuration
            translator:
                enabled:              true
                fallback:             en

            # validation configuration
            validation:
                enabled:              true
                cache:                ~
                enable_annotations:   false

            # annotation configuration
            annotations:
                cache:                file
                file_cache_dir:       %kernel.cache_dir%/annotations
                debug:                true

.. _`protocol-relative`: http://tools.ietf.org/html/rfc3986#section-4.2
.. _`sprintf()`: http://php.net/manual/en/function.sprintf.php
