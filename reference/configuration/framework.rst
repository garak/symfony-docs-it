.. index::
    single: Riferimento configurazione; Framework

Configurazione di FrameworkBundle ("framework")
===============================================

FrameworkBundle contiene la maggior parte delle funzionalità di base del
framework e può essere configurato sotto la chiave ``framework`` nella
configurazione di un'applicazione. Se si usa XML, si deve utilizzare lo spazio dei nomi
``http://symfony.com/schema/dic/symfony``.

Questo include impostazioni relative a sessioni, traduzione, form, validazione,
rotte e altro.

.. tip::

   Lo schema XSD è disponibile su
   ``http://symfony.com/schema/dic/symfony/symfony-1.0.xsd``.

Configurazione
--------------

* `secret`_
* `http_method_override`_
* `trusted_proxies`_
* `ide`_
* `test`_
* `default_locale`_
* `trusted_hosts`_
* :ref:`form <reference-framework-form>`
    * :ref:`enabled <reference-form-enabled>`
* `csrf_protection`_
    * :ref:`enabled <reference-csrf_protection-enabled>`
    * `field_name`_
* `esi`_
    * :ref:`enabled <reference-esi-enabled>`
* `fragments`_
    * :ref:`enabled <reference-fragments-enabled>`
    * :ref:`path <reference-fragments-path>`
* `profiler`_
    * :ref:`enabled <reference-profiler-enabled>`
    * `collect`_
    * `only_exceptions`_
    * `only_master_requests`_
    * `dsn`_
    * `username`_
    * `password`_
    * `lifetime`_
    * `matcher`_
        * `ip`_
        * :ref:`path <reference-profiler-matcher-path>`
        * `service`_
* `router`_
    * `resource`_
    * `type`_
    * `http_port`_
    * `https_port`_
    * `strict_requirements`_
* `session`_
    * `storage_id`_
    * `handler_id`_
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
* `templating`_
    * `assets_version`_
    * `assets_version_format`_
    * `hinclude_default_template`_
    * :ref:`form <reference-templating-form>`
        * `resources`_
    * `assets_base_urls`_
        * http
        * ssl
    * :ref:`cache <reference-templating-cache>`
    * `engines`_
    * `loaders`_
    * `packages`_
* `translator`_
    * :ref:`enabled <reference-translator-enabled>`
    * `fallbacks`_
* `validation`_
    * :ref:`enabled <reference-validation-enabled>`
    * :ref:`cache <reference-validation-cache>`
    * `enable_annotations`_
    * `translation_domain`_
* `annotations`_
    * :ref:`cache <reference-annotations-cache>`
    * `file_cache_dir`_
    * `debug`_
* `serializer`_
    * :ref:`enabled <reference-serializer-enabled>`

secret
~~~~~~

**tipo**: ``stringa`` **obbligatorio**

Una stringa che dovrebbe essere univoca nella propria applicazione. In pratica,
è usata per aggiungere maggiore entropia alle operazioni di sicurezza. Il suo valore dovrebbe essere
una serie di caratteri, numeri e simboli scelti casualmente e la sua lunghezza raccomandata è
intorno ai 32 caratteri.

In pratica, Symfony usa questo valore per generare i
:ref:`token CSRF <forms-csrf>`, per criptare cookie usati nella
:doc:`funzionalità "ricordami" </cookbook/security/remember_me>`
e per creare URI firmate con :ref:`ESI (Edge Side Include) <edge-side-includes>` .

Questa opzione diventa il parametro del contenitore di nome ``kernel.secret``,
che può essere usato ogni volta che l'applicazione necessiti di una stringa casuale immutabile,
per aggiungere più entropia.

Come per ogni altro parametro legato alla sicurezza, è buona pratica cambiarne il
valore di tanto in tanto. Tuttavia, tenere a mente che la modifica di questo valore
invaliderà tutti gli URI firmati e i cookie "ricordami". Per questo motivo, dopo la modifica,
si dovrebbe rigenerare la cache dell'applicazione e disconnettere tutti
gli utenti.

.. _configuration-framework-http_method_override:

http_method_override
~~~~~~~~~~~~~~~~~~~~

.. versionadded:: 2.3
    L'opzione ``http_method_override`` è nuova in Symfony 2.3.

**tipo**: ``booleano`` **predefinito**: ``true``

Determina se il parametro ``_method`` della richiesta sia usato come metodo HTTP inteso
sulle richieste POST. Se abilitato,
:method:`Request::enableHttpMethodParameterOverride <Symfony\\Component\\HttpFoundation\\Request::enableHttpMethodParameterOverride>`
è richiamato automaticamente. Diventa un parametro del contenitore di servizi, con nome
``kernel.http_method_override``.

.. seealso::

    Per maggiori informazioni, vedere :doc:`/cookbook/routing/method_parameters`.

.. caution::

    Se si usa il :ref:`reverse proxy AppCache <symfony2-reverse-proxy>`
    con questa opzione, il kernel ignorerà il parametro ``_method``,
    cosa che potrebbe causare errori.

    Per risolvere, invocare il metodo ``enableHttpMethodParameterOverride()``
    prima di creare l'oggetto ``Request``::

        // web/app.php

        // ...
        $kernel = new AppCache($kernel);

        Request::enableHttpMethodParameterOverride(); // <-- aggiungere questa riga
        $request = Request::createFromGlobals();
        // ...

.. _reference-framework-trusted-proxies:

trusted_proxies
~~~~~~~~~~~~~~~

**tipo**: ``array``

Configura gli indirizzi IP da considerare proxy fidati. Per maggiori
dettagli, vedere :doc:`/cookbook/request/load_balancer_reverse_proxy`.

.. versionadded:: 2.3
    Il supporto alla notazione CIDR è stato introdotto in Symfony 2.3, si possono quindi
    specificare intere sottoreti (come ``10.0.0.0/8``, ``fc00::/7``).

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
            ide: "phpstorm://open?file=%%f&line=%%l"

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <?xml version="1.0" encoding="UTF-8" ?>
        <container xmlns="http://symfony.com/schema/dic/services"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xmlns:framework="http://symfony.com/schema/dic/symfony"
            xsi:schemaLocation="http://symfony.com/schema/dic/services http://symfony.com/schema/dic/services/services-1.0.xsd
                http://symfony.com/schema/dic/symfony http://symfony.com/schema/dic/symfony/symfony-1.0.xsd">

            <framework:config ide="phpstorm://open?file=%%f&line=%%l" />
        </container>

    .. code-block:: php

        // app/config/config.php
        $container->loadFromExtension('framework', array(
            'ide' => 'phpstorm://open?file=%%f&line=%%l',
        ));

.. tip::

    Se si usa Windows, si può installare `PhpStormOpener`_ per sfruttare
    questa caratteristica.

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
tramite ``app/config/config_test.yml``).

.. seealso::

    Per maggiori informazioni, vedere :doc:`/book/testing`.

default_locale
~~~~~~~~~~~~~~

**tipo**: ``stringa`` **predefinito**: ``en``

Opzione usata se il parametro ``_locale`` non è stato impostato nelle rotte. 
Diventa il parametro del contenitore dei servizi ``kernel.default_locale`` ed è
anche disponibile con il metodo
:method:`Request::getDefaultLocale <Symfony\\Component\\HttpFoundation\\Request::getDefaultLocale>`.

.. seealso::

    Si può approfondire l'argomento del locale predefinito in
    :ref:`book-translation-default-locale`.

trusted_hosts
~~~~~~~~~~~~~

**tipo**: ``array`` o ``stringa`` **predefinito** ``array()``

Sono stati scoperti molti attacchi basati su incoerenze nella
gestione dell'header ``Host`` in vari software (server web, reverse
proxy, framework web, ecc.). Di base, ogni volta che il framework
genera un URL assoluto (inviando una email, per
esempio), l'host potrebbe essere stato manipolato da un attaccante.

.. seealso::

    Leggere "`HTTP Host header attacks`_" per maggiori informazioni su
    questi tipi di attacchi.

Il metodo :method:`Request::getHost() <Symfony\\Component\\HttpFoundation\\Request:getHost>`
potrebbe essere vulnerabile ad alcuni di questi attacchi, perché dipende
dalla configurazione del server web. Una semplice soluzione per evitare tali
attacchi è una lista bianca di host, a cui l'applicazione Symfony può
rispondere. Questo è lo scopo dell'opzione ``trusted_hosts``. Se il nome dell'host
della richiesta in arrivo non corrisponde a uno della lista, l'applicazione non
risponderà e l'utente riceverà un errore 500.

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        framework:
            trusted_hosts:  ['acme.com', 'acme.org']

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <?xml version="1.0" encoding="UTF-8" ?>
        <container xmlns="http://symfony.com/schema/dic/services"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xmlns:framework="http://symfony.com/schema/dic/symfony"
            xsi:schemaLocation="http://symfony.com/schema/dic/services http://symfony.com/schema/dic/services/services-1.0.xsd
                http://symfony.com/schema/dic/symfony http://symfony.com/schema/dic/symfony/symfony-1.0.xsd">

            <framework:config>
                <trusted-host>acme.com</trusted-host>
                <trusted-host>acme.org</trusted-host>
                <!-- ... -->
            </framework>
        </container>

    .. code-block:: php

        // app/config/config.php
        $container->loadFromExtension('framework', array(
            'trusted_hosts' => array('acme.com', 'acme.org'),
        ));

Si possono anche configurare host tramite espressioni regolari (p.e.  ``.*\.?acme.com$``),
che facilita la risposta a tutti i sottodomini.

Inoltre, si possono anche impostare gli host fidati nel front controller,
usando il metodo ``Request::setTrustedHosts()``::

    // web/app.php
    Request::setTrustedHosts(array('.*\.?acme.com$', '.*\.?acme.org$'));

Il valore predefinito di questa opzione è un array vuoto, che vuol dire che l'applicazione
risponderà a tutti gli host.

.. seealso::

    Si può approfondire l'argomento in un `post su Security Advisory Blog`_.

.. _reference-framework-form:

form
~~~~

.. _reference-form-enabled:

enabled
.......

**tipo**: ``booleano`` **predefinito**: ``false``

Se abilitare o meno il supporto per il componente Form. Se
non si usano form, impostare questa opzione a ``false`` può aumentare le prestazioni,
perché saranno caricati meno servizi nel contenitore.

Se è attivato, anche :ref:`il sistema di validazione <validation-enabled>`
è automaticamente abilitato.

.. note::

    Verrà abilitata automaticamente `validation`_.

.. seealso::

    Per maggiori dettagli, vedere :doc:`/book/forms`.

csrf_protection
~~~~~~~~~~~~~~~

.. seealso::

    Per ulteriori informazioni sulla protezione CSRF nei form, vedere :ref:`forms-csrf`.

.. _reference-csrf_protection-enabled:

enabled
.......

**tipo**: ``booleano`` **predefinito**: ``true`` se il supporto per i form è abilitato, ``false``
altrimenti

Si può usare questa opzione per disabilitare la protezione CSRF su *tutti* i form. Ma si
può anche :ref:`disabilitare la protezione CSRF su singoli form <form-disable-csrf>`.

Se si usano form, ma non si vuole far partire la sessione (p.e. si usano
form in un sito di sole API), ``csrf_protection`` va impostato a
``false``.

field_name
..........

**tipo**: ``stringa`` **predefinito**: ``"_token"``

Il nome del campo nascosto usato per rendere i :ref:`token CSRF <forms-csrf>`.

esi
~~~

.. seealso::

    Si può leggere di più su Edge Side Include (ESI) in :ref:`edge-side-includes`.

.. _reference-esi-enabled:

enabled
.......

**tipo**: ``booleano`` **predefinito**: ``false``

Se abilitare il supporto a edge side include.

Si può impostare ``esi`` a ``true`` per abilitarlo:

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        framework:
            esi: true

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <?xml version="1.0" encoding="UTF-8" ?>
        <container xmlns="http://symfony.com/schema/dic/services"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xmlns:framework="http://symfony.com/schema/dic/symfony"
            xsi:schemaLocation="http://symfony.com/schema/dic/services http://symfony.com/schema/dic/services/services-1.0.xsd
                http://symfony.com/schema/dic/symfony http://symfony.com/schema/dic/symfony/symfony-1.0.xsd">

            <framework:config>
                <esi />
            </framework:config>
        </container>

    .. code-block:: php

        // app/config/config.php
        $container->loadFromExtension('framework', array(
            'esi' => true,
        ));

fragments
~~~~~~~~~

.. seealso::

    Si possono approfondire i frammenti nella ricetta sulla
    :ref:`cache HTTP <book-http_cache-fragments>`.

.. _reference-fragments-enabled:

enabled
.......

**tipo**: ``booleano`` **predefinito**: ``false``

Se abilitare l'ascoltatore dei frammenti. L'ascoltatore dei frammenti è
usato per rendere frammenti ESI in modo indipendente dal resto della pagina.

Questa impostazione è automaticamente ``true`` quando una delle impostazioni figlie
è configurata.

.. _reference-fragments-path:

path
....

**tipo**: ``stringa`` **predefinito**: ``'/_fragment'``

Il prefisso dei percorsi dei frammenti. L'ascoltatore dei frammenti sarà eseguito solo
se la richiesta inizia con tale percorso.

profiler
~~~~~~~~

.. _reference-profiler-enabled:

enabled
.......

.. versionadded:: 2.2
    L'opzione ``enabled`` è stata introdotta in Symfony 2.2. Prima di Symfony
    2.2, il profilatore poteva essere disabilitato solo omettendo interamente la
    configurazione ``framework.profiler``.

**tipo**: ``booleano`` **predefinito**: ``false``

Si può abilitare il profilatore impostando questa opzione a ``true``. Se si
usa Symfony Standard Edition, il profilatore è abilitato negli ambienti ``dev``
e ``test``.

.. note::

    Il profilatore è indipendente dalla barra di debug del web, vedere
    la :doc:`configurazione di WebProfilerBundle </reference/configuration/web_profiler>`
    su come abilitare o disabilitare la barra.

collect
.......

.. versionadded:: 2.3
    L'opzione ``collect`` è stata introdotta in Symfony 2.3. Precedentemente, se
    ``profiler.enabled`` era ``false``, il profilatore era effettivamente abilitato,
    ma i raccoglitori erano disabilitati. Ora si possono controllare distintamente
    profilatore e raccoglitori.

**tipo**: ``booleano`` **predefinito**: ``true``

Questa opzione configura il modo in cui si comporta il profilatore, se abilitato.
Se ``true``, il profilatore raccoglie dati per ogni richiesta (se non
configurato diversamente, per esempio con un `matcher`_). Se si vogliono raccogliere dati
solo a richiesta, si può impostare ``collect`` a ``false``
e attivare manualmente i raccoglitori::

    $profiler->enable();

only_exceptions
...............

**tipo**: ``booleano`` **predefinito**: ``false``

Se impostato a ``true``, il profilatore sarà abilitato solo quando
un'eccezzione viene sollevata mentre si gestisce una richiesta.

only_master_requests
....................

**tipo**: ``booleano`` **predefinito**: ``false``

Se impostato a ``true``, il profilatore sarà abilitato solo sulle
richieste principali (e non sulle sottorichieste).

dsn
...

**tipo**: ``stringa`` **predefinito**: ``'file:%kernel.cache_dir%/profiler'``

Il DSN in cui memorizzare informazioni di profilazione.

.. seealso::

    Vedere :doc:`/cookbook/profiler/storage` per maggiori informazioni sulla
    memorizzazione di profilazioni.

username
........

**tipo**: ``stringa`` **predefinito**: ``''``

Se necessario, il nome utente per la memorizzazione di profilazioni.

password
........

**tipo**: ``stringa`` **predefinito**: ``''``

Se necessaria, la password per la memorizzazione di profilazioni.

lifetime
........

**tipo**: ``integer`` **predefinito**: ``86400``

Il tempo di vita, in secondi, per la memorizzazione di profilazioni. I dati saranno eliminati
allo scadere del tempo.

matcher
.......

Si possono configurare opzioni di corrispondenza, per abilitare dinamicamente il profilatore. Per
esempio, in base a `ip`_ o :ref:`path <reference-profiler-matcher-path>`.

.. seealso::

    Vedere :doc:`/cookbook/profiler/matchers` per maggiori informazioni sull'uso di un
    matcher per abilitare o disabilitare il profilatore.

ip
""

**tipo**: ``stringa``

Se impostato, il profilatore sarà abilitato solo in corrispondenza dell'indirrizo IP fornito.

.. _reference-profiler-matcher-path:

path
""""

**tipo**: ``stringa``

Se impostato, il profilatore sarà abilitato solo in corrispondenza del percorso fornito.

service
"""""""

**tipo**: ``stringa``

Questa impostazione contiene l'identificativo di un servizio per un matcher personalizzato.

router
~~~~~~

resource
........

**tipo**: ``stringa`` **required**

Il percorso alla risorsa principale (p.e. un file YAML) che contiene
rotte e importazioni, che il router deve caricare.

type
....

**tipo**: ``stringa``

Il tipo di risorsa per suggerrire ai caricatori il formato. Non è necessario
se si usano router predefiniti, che si aspettano estensioni di file
(``.xml``, ``.yml`` / ``.yaml``, ``.php``).

http_port
.........

**tipo**: ``integer`` **predefinito**: ``80``

La porta per le richieste HTTP normali (usata per far corrispondere lo schema).

https_port
..........

**tipo**: ``integer`` **predefinito**: ``443``

La porta per le richieste HTTPS (usata per far corrispondere lo schema).

strict_requirements
...................

**tipo**: ``mixed`` **predefinito**: ``true``

Determina il comportamento del generatore di rotte. Quando genera una rotta con
:ref:`requisiti <book-routing-requirements>` specifici, il generatore può
comportarsi in modo diverso se i parametri usati non soddisfano tali requisiti.

Il valore può essere uno tra:

``true``
    Lancia un'eccezione se i requisiti non sono soddisfatti;
``false``
    Disabilita le eccezioni se i requisiti non sono soddisfatti e restituisce
    invece ``null``;
``null``
    Disabilita la  verifica dei requisiti (quindi, la rotta corrisponderà anche quando
    i requisiti non sono soddisfatti).

Si raccomanda ``true`` per l'ambiente di sviluppo, ``false``
o ``null`` per quello di produzione.

session
~~~~~~~

storage_id
..........

**tipo**: ``stringa`` **predefinito**: ``'session.storage.native'``

L'identificativo del servizio usato per memorizzare la sessione. L'alias ``session.storage``
punterà a questo servizio. Questa classe deve implementare
:class:`Symfony\\Component\\HttpFoundation\\Session\\Storage\\SessionStorageInterface`.

handler_id
..........

**tipo**: ``stringa`` **predefinito**: ``'session.handler.native_file'``

L'identificativo del servizio usato per gestire la sessione. L'alias ``session.handler``
punterà a questo servizio. Questa classe deve implementare

Se impostato a ``null``, sarà usato il gestore predefinito configurato in PHP.


.. seealso::

    Un esempio di utilizzo è disponibile in
    :doc:`/cookbook/configuration/pdo_session_storage`.

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

Determina il percorso da impostare nel cookie di sessione. Per impostazione predefinita
è ``/``.

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

gc_divisor
..........

**tipo**: ``intero`` **predefinito**: ``100``

Vedere `gc_probability`_.

gc_probability
..............

**tipo**: ``intero`` **predefinito**: ``1``

Definisce la probabilità che il processo del garbage collector parta a
ogni inizializzazione della sessione. La probabilità è calcolata usando
``gc_probability`` / ``gc_divisor``, p.e. 1/100 vuol dire che c'è una probabilità dell'1%
che il processo parta, in ogni richiesta.

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
                save_path: ~

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

templating
~~~~~~~~~~

.. _reference-framework-assets-version:
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

Ora, la stessa risorsa sarà resa come ``/images/logo.png?v2``. Se si usa questa
caratteristica, si *deve* incrementare a mano il valore di ``assets_version``, prima
di ogni deploy, in modo che il parametro della query cambi.

Si può anche controllare il funzionamento della stringa della query, tramite
l'opzione `assets_version_format`_.

.. tip::

    Come per tutte le impostazioni, si può usare un parametro come valore di
    ``assets_version``. Questo rende più facile l'incremento a ogni
    deploy.

.. _reference-templating-version-format:

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

    Alcuni CDN non sopportano la spaccatura della cache tramite stringa della query,
    quindi si rende necessario l'inserimento della versione nel vero percorso della risorsa.
    Fortunatamente, ``assets_version_format`` non è limitato alla produzione di stringhe di query
    con versioni.

    Lo schema riceve il percorso originale della risorsa e la versione come primo e
    secondo parametro, rispettivamente. Poiché il percorso della risorsa è un
    parametro, non può essere al volo (p.e. ``/images/logo-v5.png``). Tuttavia,
    si può aggiungere un prefisso al percorso della risorsa, usando uno schema
    ``version-%%2$s/%%1$s``, che sarà trasformato nel percorso
    ``version-5/images/logo.png``.

    Si possono quindi usare le riscritture degli URL, per togliere il prefisso con la versione
    prima di servire la risorsa. In alternativa, si possono copiare le risorse nel percorso
    appropriato con la versione, come parte del processo di deploy, e non usare la riscrittura
    degli URL. L'ultima opzione è utile se si vuole che le vecchie versioni delle risorse restino
    disponibili nei loro URL originari.

hinclude_default_template
.........................

**tipo**: ``stringa`` **predefinito**: ``null``

Imposta il contenuto mostrato durante il caricamento del frammento o quando JavaScript
non è abilitato. Può essere il nome di un template o direttamente un contenuto.

.. seealso::

    Vedere :ref:`book-templating-hinclude` per maggiori informazioni su hinclude.

.. _reference-templating-form:

form
....

resources
"""""""""

**tipo**: ``stringa[]`` **predefinito**: ``['FrameworkBundle:Form']``

Un elenco di risorse per i temi di form in PHP. Questa impostazione non è obbligatoria
se si usa il formato Twig per i template, nel qual caso si può fare riferimento al
:ref:`capitolo sui form <book-forms-theming-twig>`.

Si ipotizzi di avere temi di form personalizzati globali in
``src/WebsiteBundle/Resources/views/Form``, si può configurare come:

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        framework:
            templating:
                form:
                    resources:
                        - 'WebsiteBundle:Form'

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <?xml version="1.0" encoding="UTF-8" ?>
        <container xmlns="http://symfony.com/schema/dic/services"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xmlns:framework="http://symfony.com/schema/dic/symfony"
            xsi:schemaLocation="http://symfony.com/schema/dic/services http://symfony.com/schema/dic/services/services-1.0.xsd
                http://symfony.com/schema/dic/symfony http://symfony.com/schema/dic/symfony/symfony-1.0.xsd">

            <framework:config>

                <framework:templating>

                    <framework:form>

                        <framework:resource>WebsiteBundle:Form</framework:resource>

                    </framework:form>

                </framework:templating>

            </framework:config>
        </container>

    .. code-block:: php

        // app/config/config.php
        $container->loadFromExtension('framework', array(
            'templating' => array(
                'form' => array(
                    'resources' => array(
                        'WebsiteBundle:Form'
                    ),
                ),
            ),
        ));

.. note::

    I template predefiniti, che si trovano in ``FrameworkBundle:Form``, saranno
    sempre inclusi tra le risorse dei form.

.. seealso::

    Vedere :ref:`book-forms-theming-global` per maggiori informazioni.

.. _reference-templating-base-urls:

assets_base_urls
................

**predefinito**: ``{ http: [], ssl: [] }``

Questa opzione consente di definire URL di base, da usare per risorse a cui fare riferimento
in pagine ``http`` e ``ssl`` (``https``). Se si forniscono più URL di
base, Symfony ne sceglierà una ogni volta che genererà il percorso
di una risorsa:

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        framework:
            # ...
            templating:
                assets_base_urls:
                    http:
                        - "http://cdn.example.com/"
                # si può anche passare una semplice stringa:
                # assets_base_urls:
                #     http: "//cdn.example.com/"

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <?xml version="1.0" encoding="UTF-8" ?>
        <container xmlns="http://symfony.com/schema/dic/services"
            xmlns:framework="http://symfony.com/schema/dic/symfony">

            <framework:config>
                <!-- ... -->

                <framework:templating>
                    <framework:assets-base-url>
                        <framework:http>http://cdn.example.com/</framework:http>
                    </framework:assets-base-url>
                </framework:templating>
            </framework:config>
        </container>

    .. code-block:: php

        // app/config/config.php
        $container->loadFromExtension('framework', array(
            // ...
            'templating' => array(
                'assets_base_urls' => array(
                    'http' => array(
                        'http://cdn.example.com/',
                    ),
                ),
                // si può anche passare una semplice stringa:
                // 'assets_base_urls' => array(
                //     'http' => '//cdn.example.com/',
                // ),
            ),
        ));

Nel caso, si può passare una stringa o un array di stringhe a
``assets_base_urls`` direttamente. Saranno organizzati automaticamente in URL
di base ``http`` e ``ssl`` (URL ``https://`` e `protocol-relative`_
saranno aggiunte a entrambi gli insiemi e ``http://`` solo all'insieme ``http``):


.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        framework:
            # ...
            templating:
                assets_base_urls:
                    - "//cdn.example.com/"
                # si può anche passare una semplice stringa:
                # assets_base_urls: "//cdn.example.com/"

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <?xml version="1.0" encoding="UTF-8" ?>
        <container xmlns="http://symfony.com/schema/dic/services"
            xmlns:framework="http://symfony.com/schema/dic/symfony">

            <framework:config>
                <!-- ... -->

                <framework:templating>
                    <framework:assets-base-url>//cdn.example.com/</framework:assets-base-url>
                </framework:templating>
            </framework:config>
        </container>

    .. code-block:: php

        // app/config/config.php
        $container->loadFromExtension('framework', array(
            // ...
            'templating' => array(
                'assets_base_urls' => array(
                    '//cdn.example.com/',
                ),
                // si può anche passare una semplice stringa:
                // 'assets_base_urls' => '//cdn.example.com/',
            ),
        ));

.. _reference-templating-cache:

cache
.....

**tipo**: ``stringa``

Il percorso della cartella della cache per i template. Se non impostato, la cache
è disabilitata.

.. note::

    Se si usano i template Twig, la cache è già gestita da
    TwigBundle e non è necessario abilitarla in FrameworkBundle.

engines
.......

**tipo**: ``stringa[]`` / ``stringa`` **required**

Il motore di template da usare. Può essere una stringa (se si configura
un solo motore) o un array.

Si deve configurare almeno un motore.

loaders
.......

**tipo**: ``stringa[]``

Un array (o una stringa, se si configura un solo caricatore) di identificativi di servizi per i
caricatori di template. I caricatori di template sono usati per trovare e caricare template
da una risorsa (p.e. un filesystem o una base dati). I caricatori di template devono
implementare :class:`Symfony\\Component\\Templating\\Loader\\LoaderInterface`.

packages
........

Si possono raggruppare le risorse in pacchetti, per specificarne diversi URL di base:

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        framework:
            # ...
            templating:
                packages:
                    avatars:
                        base_urls: 'http://static_cdn.example.com/avatars'

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <?xml version="1.0" encoding="UTF-8" ?>
        <container xmlns="http://symfony.com/schema/dic/services"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xmlns:framework="http://symfony.com/schema/dic/symfony"
            xsi:schemaLocation="http://symfony.com/schema/dic/services http://symfony.com/schema/dic/services/services-1.0.xsd
                http://symfony.com/schema/dic/symfony http://symfony.com/schema/dic/symfony/symfony-1.0.xsd">

            <framework:config>

                <framework:templating>

                    <framework:package
                        name="avatars"
                        base-url="http://static_cdn.example.com/avatars">

                </framework:templating>

            </framework:config>
        </container>

    .. code-block:: php

        // app/config/config.php
        $container->loadFromExtension('framework', array(
            // ...
            'templating' => array(
                'packages' => array(
                    'avatars' => array(
                        'base_urls' => 'http://static_cdn.example.com/avatars',
                    ),
                ),
            ),
        ));

Ora si può usare il pacchetto ``avatars`` nei template:

.. configuration-block:: php

    .. code-block:: html+jinja

        <img src="{{ asset('...', 'avatars') }}">

    .. code-block:: html+php

        <img src="<?php echo $view['assets']->getUrl('...', 'avatars') ?>">

Per ogni pacchetto si possono configurare le opzioni:

* :ref:`base_urls <reference-templating-base-urls>`
* :ref:`version <reference-framework-assets-version>`
* :ref:`version_format <reference-templating-version-format>`

translator
~~~~~~~~~~

.. _reference-translator-enabled:

enabled
.......

**tipo**: ``booleano`` **predefinito**: ``false``

Se abilitare o meno il servizio ``translator`` nel contenitore.

.. _fallback:

fallbacks
.........

**tipo**: ``stringa|array`` **predefinito**: ``array('en')``

.. versionadded:: 2.3.25
    L'opzione ``fallbacks`` è stata introdotta in Symfony 2.3.25. Prima
    di Symfony 2.3.25, si chiamava ``fallback`` e consentiva un solo
    linguaggio, definito come stringa. Notare che è ancora possibile usare la
    vecchia opzione ``fallback``, se si vuole definire un solo linguaggio.

Opzione usata quando non viene trovata la chiave di traduzione del locale
corrente.

.. seealso::

    Per maggiori dettagli, vedere :doc:`/book/translation`.

validation
~~~~~~~~~~

.. _reference-validation-enabled:

enabled
.......

**tipo**: ``booleano`` **predefinito**: ``true`` se :ref:`il supporto ai form è abilitato <form-enabled>`,
``false`` altrimenti

Se abilitare o meno il supporto alla validazione.

Questa opzione sarà impostata automaticamente a ``true`` quando una delle impostazioni figlie
è configurata.

.. _reference-validation-cache:

cache
.....

**tipo**: ``stringa``

Questo valore è usato per deterimnare il servizio utilizzato per persistere i metadati di classe
in una cache. L'effettivo nome del servizio è costruito aggiungendo un prefisso 
``validator.mapping.cache.`` al valore configurato (p.e. se il valore è ``apc``, sarà iniettato il servizio
``validator.mapping.cache.apc``). Il servizio deve
implementare :class:`Symfony\\Component\\Validator\\Mapping\\Cache\\CacheInterface`.

enable_annotations
..................

**tipo**: ``Booleano`` **predefinito**: ``false``

Se questa opzione è abilitata, si possono definire vincoli di validazione tramite annotazioni.

translation_domain
..................

**tipo**: ``stringa`` **predefinito**: ``validators``

Il dominio di traduzione usato durante la traduzione dei messaggi di errore dei
vincoli di validazione.

annotations
~~~~~~~~~~~

.. _reference-annotations-cache:

cache
.....

**tipo**: ``stringa`` **predefinito**: ``'file'``

Questa opzione può avere uno dei seguenti valori:

file
    Usa il filesystem per la cache delle annotazioni
none
    Disabilita la cache delle annotazioni
un id di servizio
    Un id di servizio che fa riferimento all'implementazione `Doctrine Cache`_

file_cache_dir
..............

**tipo**: ``stringa`` **predefinito**: ``'%kernel.cache_dir%/annotations'``

La cartella in cui memorizzare i file per le annotazioni, se
``annotations.cache`` è impostato a ``'file'``.

debug
.....

**tipo**: ``booleano`` **predefinito**: ``%kernel.debug%``

Se abilitare il debug per la cache. Se abilitato, la cache sarà aggiornata
automaticamente se il file originale viene modificato (sia nel codice che
nelle annotazioni). Per motivi prestazionali, si raccomanda di disabilitare
il debug in produzione, cosa che avviene automaticamente se si usa il
valore predefinito.

.. _configuration-framework-serializer:

serializer
~~~~~~~~~~

.. _reference-serializer-enabled:

enabled
.......

**tipo**: ``boolean`` **predefinito**: ``false``

Se abilitare il servizio ``serializer`` nel contenitore.

For more details, see :doc:`/cookbook/serializer`.

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
                collect:              true
                only_exceptions:      false
                only_master_requests: false
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

                # * impostare a true per lanciare un'eccezione se un parametro
                #   non corrisponde ai requisiti
                # * impostare a false per disabilitare le eccezioni se un parametro non
                #   corrisponde ai requisiti (e restituire null)
                # * impostare a null per disabilitare la verifica dei requisiti dei parametri
                #
                # true è preferibile durante lo sviluppo, mentre 
                # false o null sono preferibili in produzione
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
                fallbacks:            [en]

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
.. _`HTTP Host header attacks`: http://www.skeletonscribe.net/2013/05/practical-http-host-header-attacks.html
.. _`post su Security Advisory Blog`: http://symfony.com/blog/security-releases-symfony-2-0-24-2-1-12-2-2-5-and-2-3-3-released#cve-2013-4752-request-gethost-poisoning
.. _`Doctrine Cache`: http://docs.doctrine-project.org/projects/doctrine-common/en/latest/reference/caching.html
.. _`PhpStormOpener`: https://github.com/pinepain/PhpStormOpener
