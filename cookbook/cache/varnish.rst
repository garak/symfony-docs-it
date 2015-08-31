.. index::
    single: Cache; Varnish

Usare Varnish per accelerare il proprio sito
============================================

Poiché la cache di Symfony usa gli header standard della cache HTTP,
:ref:`symfony-gateway-cache` può essere facilmente sostituito da qualsiasi altro reverse
proxy. `Varnish`_ è un acceleratore HTTP potente e open source, che è in grado di servire
contenuti in cache in modo veloce e che include il supporto per :ref:`Edge Side Include <edge-side-includes>`.

.. index::
    single: Varnish; configurazione

Reverse proxy fidati
--------------------

Varnish inoltra automaticamente l'IP come ``X-Forwarded-For`` e lascia l'header
``X-Forwarded-Proto`` nella richiesta. Se non si configura Varnish
come proxy fidato, Symfony vedrà tutte le richieste come provenienti da connessioni HTTP
non sicure, dall'host di Varnish invece che dal client reale.

Ricordarsi di configurare :ref:`framework.trusted_proxies <reference-framework-trusted-proxies>` nella
configurazione di Symfony, in modo che Varnish sia visto come proxy fidato e gli header
:ref:`X-Forwarded <varnish-x-forwarded-headers>` usati.

.. _varnish-x-forwarded-headers:

Rotte e header X-FORWARDED
--------------------------

Se l'header ``X-Forwarded-Port`` non è impostato correttamente, Symfony appenderà
la porta in cui l'applicazione PHP sta girando, durante la generazione di URL assolute,
p.e. ``http://example.com:8080/percorso``. Per essere sicuri che Symfony generi
correttamente URL con Varnish, ci deve essere un header
``X-Forwarded-Port``, in modo che Symfony usi il numero giusto di porta. Questa porta dipende dalla configurazione.

Ipotizziamo che le connessioni esterne vengano sulla porta HTTP predefinita, la 80. Per
le connessioni HTTPS, c'è un altro proxy (non facendo Varnish stesso HTTPS) sulla
porta HTTPS predefinita, la 443, che gestisce SSL e gira le richieste come richieste HTTP a
Varnish, con un header ``X-Forwarded-Proto``. In questo caso, si deve aggiungere
la seguente parte di configurazione:

.. code-block:: varnish4

    sub vcl_recv {
        if (req.http.X-Forwarded-Proto == "https" ) {
            set req.http.X-Forwarded-Port = "443";
        } else {
            set req.http.X-Forwarded-Port = "80";
        }
    }

Cookie e cache
--------------

Di solito, un proxy non mette in cache quando una richiesta è inviata
con :ref:`cookie o header basic authentication<http-cache-introduction>`.
Questo perché  ipotizza che il contenuto della pagina dipenda dal valore del cookie
o dell'header di autenticazione.

Se si è certi che il backend non userà mai sessioni o basic
authentication, fare in modo che varnish rimuova il rispettivo header dalle richieste, per
impedire che i client aggirino la cache. In pratica, si avrà bisogno delle sessioni
almeno per alcune parti del sito, p.e. usando form con
:ref:`protezione CSRF <forms-csrf>`. In questa situazione, assicurarsi solo di far partire
una sessione quando effettivamente necessario e di pulire la sessione quando non più
necessaria. In alternativa, si può dare un'occhiata
a :doc:`/cookbook/cache/form_csrf_caching`.

I cookie creati in JavaScript e usati solo in frontend, p.e. quando si usa
Google analytics sono comunque inviati al server. Questi cookie non sono
rilevanti per il backend e non dovrebbero influire sulle decisioni di cache. Configurare
Varnish per `pulire gli header dei cookie`_. È desiderabile mantenere i
cookie di sessione, se presenti, e togliere ogni altro cookie, in modo che le pagine
vengano messe in cache in assenza di sessioni attive. A meno di non aver modificato la
configurazione predefinita di PHP, il cookie di sessione si chiama PHPSESSID:

.. code-block:: varnish4

    sub vcl_recv {
        // Rimuove tutti i cookie, tranne quello della sessione
        if (req.http.Cookie) {
            set req.http.Cookie = ";" + req.http.Cookie;
            set req.http.Cookie = regsuball(req.http.Cookie, "; +", ";");
            set req.http.Cookie = regsuball(req.http.Cookie, ";(PHPSESSID)=", "; \1=");
            set req.http.Cookie = regsuball(req.http.Cookie, ";[^ ][^;]*", "");
            set req.http.Cookie = regsuball(req.http.Cookie, "^[; ]+|[; ]+$", "");

            if (req.http.Cookie == "") {
                // Se non ci sono altri cookie, rimuove l'header per far in modo che la pagina vada in cache
                remove req.http.Cookie;
            }
        }
    }

.. tip::

    Se il contenuto non è diverso per ciascun utente, ma dipende dai ruoli di un
    utente, una soluzione è separare la cache per gruppo. Questo schema è
    implementato e spiegato da FOSHttpCacheBundle_ sotto la voce
    `User Context`_.

Assicurare comportamenti di cache coerenti
------------------------------------------

Varnish usa gli header di cache inviati dall'applicazione per determinare in che modo
mettere in cache il contenuto. Tuttavia, le versioni precedenti a Varnish 4 non rispettavano
``Cache-Control: no-cache``, ``no-store`` e ``private``. Per assicurare un
comportamento coerente, usare la seguente configurazione, se si usa
ancora Varnish 3:

.. configuration-block::

    .. code-block:: varnish3

        sub vcl_fetch {
            /* Varnish3 ignora Cache-Control: no-cache e private
               https://www.varnish-cache.org/docs/3.0/tutorial/increasing_your_hitrate.html#cache-control
             */
            if (beresp.http.Cache-Control ~ "private" ||
                beresp.http.Cache-Control ~ "no-cache" ||
                beresp.http.Cache-Control ~ "no-store"
            ) {
                return (hit_for_pass);
            }
        }

.. tip::

    Si può vedere il comportamento predefinito di Varnish in forma di file VCL:
    `default.vcl`_ per Varnish 3, `builtin.vcl`_ per Varnish 4.

Abilitare Edge Side Include (ESI)
---------------------------------

Come spiegato nella :ref:`sezione Edge Side Include <edge-side-includes>`,
Symfony capisce se sta parlando o meno a un reverse proxy che capisca ESI.
Quando si usa il reverse proxy di Symfony, non occorre fare nulla.
Se invece si usa Varnish per risolvere i tag ESI, serve una ulteriore
configurazione in Varnish. Symfony usa l'header ``Surrogate-Capability``
da `Edge Architecture`_, descritto da Akamai.

.. note::

    Varnish supporta solo l'attributo ``src`` dei tag ESI (``onerror`` e
    ``alt`` vengono ignorati).

Innanzitutto, configurare Varnish in modo che pubblicizzi il supporto ESI, aggiungendo un header
``Surrogate-Capability`` alle richieste rimandate all'applicazione di
backend:

.. code-block:: varnish4

    sub vcl_recv {
        // Aggiunge un header Surrogate-Capability per dichiarare il supporto a ESI.
        set req.http.Surrogate-Capability = "abc=ESI/1.0";
    }

.. note::

    La parte ``abc`` dell'header non è importante, a meno non si abbiamo più
    "surrogati" che debbano pubblicizzare le loro capacità. Vedere
    `Surrogate-Capability Header`_ per dettagli.

Quindi, ottimizzare Varnish, in modo che analizzi solo il contenuto di risposte quando ci
sia almeno un tag ESI, verificando l'header ``Surrogate-Control``, aggiunto automaticamente da
Symfony:

.. configuration-block::

    .. code-block:: varnish4

        sub vcl_backend_response {
            // Verifica il riconoscimento di ESI e rimuove l'header Surrogate-Control
            if (beresp.http.Surrogate-Control ~ "ESI/1.0") {
                unset beresp.http.Surrogate-Control;
                set beresp.do_esi = true;
            }
        }

    .. code-block:: varnish3

        sub vcl_fetch {
            // Verifica il riconoscimento di ESI e rimuove l'header Surrogate-Control
            if (beresp.http.Surrogate-Control ~ "ESI/1.0") {
                unset beresp.http.Surrogate-Control;
                set beresp.do_esi = true;
            }
        }

.. tip::

    Per chi ha seguito il consiglio che assicura il comportamento coerente di cache,
    queste funzioni vcl esistono già. Basta aggiungere il codice all
    fine della funzione, non interferiranno a vicenda.

.. index::
    single: Varnish; Invalidazione

Invalidare la cache
-------------------

Se si vuole mettere in cache un contenuto che cambia di frequente e servire comunque
agli utenti la sua versione più recente, occorre invalidare tale contenuto.
Anche se l'`invalidazione della cache`_ consente di purgare il contenuto dal
proxy prima che scada, aggiunge complessità al sistema di cache.

.. tip::

    Il bundle `FOSHttpCacheBundle`_ si occupa di invalidazione di cache,
    aiutando a organizzare la strategia di cache e di
    invalidazione.

    La documentazione di `FOSHttpCacheBundle`_ spiega come configurare
    Varnish e altri reverse proxy per l'invalidazione della cache.

.. _`Varnish`: https://www.varnish-cache.org
.. _`Edge Architecture`: http://www.w3.org/TR/edge-arch
.. _`GZIP e Varnish`: https://www.varnish-cache.org/docs/3.0/phk/gzip.html
.. _`pulire gli header dei cookie`: https://www.varnish-cache.org/trac/wiki/VCLExampleRemovingSomeCookies
.. _`Surrogate-Capability Header`: http://www.w3.org/TR/edge-arch
.. _`invalidazione della cache`: http://tools.ietf.org/html/rfc2616#section-13.10
.. _`FOSHttpCacheBundle`: http://foshttpcachebundle.readthedocs.org/
.. _`default.vcl`: https://www.varnish-cache.org/trac/browser/bin/varnishd/default.vcl?rev=3.0
.. _`builtin.vcl`: https://www.varnish-cache.org/trac/browser/bin/varnishd/builtin.vcl?rev=4.0
.. _`User Context`: http://foshttpcachebundle.readthedocs.org/en/latest/features/user-context.html
