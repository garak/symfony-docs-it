.. index::
    single: Server web

Configurare un server web
=========================

La modalità preferita per lo sviluppo di un'applicazione Symfony è l'uso del
:doc:`server web interno di PHP </cookbook/web_server/built_in>`. Tuttavia,
quando si usa una vecchia versione di PHP o quando si esegue l'applicazione in produzione,
occorrerà usare un server web "classico". Questa ricetta
descrive vari modi per usare Symfony con Apache2 o Nginx.

Se si usa Apache2, si può configurare PHP come
:ref:`modulo Apache <web-server-apache-mod-php>` oppure con FastCGI, usando
:ref:`PHP FPM <web-server-apache-fpm>`. FastCGI è il modo preferito
per usare PHP :ref:`con Nginx <web-server-nginx>`.

.. sidebar:: La cartella web

    La cartella web è la casa di tutti i file pubblici e statici di un'applicazione,
    incluse immagini, fogli di stile e file JavaScript. È
    anche il posto in cui si trovano i front controller. Per maggiori dettagli, vedere :ref:`the-web-directory`.

    La cartella web funge da document root, durante la configurazione del server
    web. Negli esempi successivi, la cartella ``web/`` si troverà nella
    document root. Questa cartella è ``/var/www/progetto/web/``.

    Se un fornitore di hosting impone il cambiamento della cartella ``web/`` a una posizione
    differente (come ``public_html/``), assicurarsi di
    :ref:`modificare la posizione della cartella web/ <override-web-dir>`.

.. _web-server-apache-mod-php:

Apache con mod_php/PHP-CGI
--------------------------

La basi minime per far funzionare un'applicazione sotto Apache sono:

.. code-block:: apache

    <VirtualHost *:80>
        ServerName domain.tld
        ServerAlias www.domain.tld

        DocumentRoot /var/www/progetto/web
        <Directory /var/www/progetto/web>
            AllowOverride All
            Order allow, deny
            Allow from All
        </Directory>

        # scommentare le seguenti righe se si installano risorse come collegamenti simbolici
        # o si avranno problemi compilando risorse LESS/Sass/CoffeScript
        # <Directory /var/www/progetto>
        #     Option FollowSymlinks
        # </Directory>

        ErrorLog /var/log/apache2/progetto_error.log
        CustomLog /var/log/apache2/progetto_access.log combined
    </VirtualHost>

.. tip::

    Su un sistema che supporti la variabile ``APACHE_LOG_DIR``, si potrebbe voler
    usare ``${APACHE_LOG_DIR}/`` al posto di ``/var/log/apache2/``.

Usando la seguente **configurazione ottimizzata**, si può disabilitare il supporto a ``.htaccess``
e quindi incrementare le prestazioni del server web:

.. code-block:: apache

    <VirtualHost *:80>
        ServerName dominio.tld
        ServerAlias www.dominio.tld

        DocumentRoot /var/www/progetto/web
        <Directory /var/www/progetto/web>
            AllowOverride None
            Order allow, deny
            Allow from All

            <IfModule mod_rewrite.c>
                Options -MultiViews
                RewriteEngine On
                RewriteCond %{REQUEST_FILENAME} !-f
                RewriteRule ^(.*)$ app.php [QSA,L]
            </IfModule>
        </Directory>

        # scommentare le seguenti righe se si installano risorse come collegamenti simbolici
        # o si avranno problemi compilando risorse LESS/Sass/CoffeScript
        # <Directory /var/www/progetto>
        #     Option FollowSymlinks
        # </Directory>

        ErrorLog /var/log/apache2/progetto_error.log
        CustomLog /var/log/apache2/progetto_access.log combined
    </VirtualHost>

.. tip::

    Se si usa **php-cgi**, Apache non passa nome utente e password di HTTP basic
    a PHP, per impostazione predefinita. Per aggirare tale limitazione, si dovrebbe usare
    la seguente configurazione:

    .. code-block:: apache

        RewriteRule .* - [E=HTTP_AUTHORIZATION:%{HTTP:Authorization}]

Usare mod_php/PHP-CGI con Apache 2.4
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

In Apache 2.4, ``Order allow,deny`` è stato sostituito da ``Require all granted``,
quindi occorre modificare le impostazioni in questo modo:

.. code-block:: apache

    <Directory /var/www/progetto/web>
        Require all granted
        # ...
    </Directory>

Per opzioni avanzate di configurazione di Apache, leggere la `documentazione di Apache`_.

.. _web-server-apache-fpm:

Apache2 con PHP-FPM
-------------------

Per usare PHP5-FPM con Apache, occorre prima accertarsi di avere il
binario di FastCGI ``php-fpm`` e il modulo FastCGI di Apache
installato (per esempio, su un sistema basato su Debian, si devono installare i pacchetti
``libapache2-mod-fastcgi`` e ``php5-fpm``).

PHP-FPM usa dei cosiddetti *pool* per gestire le richieste FastCGI in arrivo. Si può
configurare un numero arbitrario di pool nella configurazione di FPM. In un pool,
si configura un socket TCP (IP e porta) o un socket di dominio su cui
ascoltare. Ciascun pool può anche essere eseguito con UID e GID diversi:

.. code-block:: ini

    ; un pool chiamato www
    [www]
    user = www-data
    group = www-data

    ; usa un socket di dominio unix
    listen = /var/run/php5-fpm.sock

    ; oppure ascolta un socket TCP
    listen = 127.0.0.1:9000

Usare mod_proxy_fcgi con Apache 2.4
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Se si usa Apache 2.4, si può usare ``mod_proxy_fcgi`` per passare le
richieste in arrivo a PHP-FPM. Configurare PHP-FPM per ascoltare un socket TCP
(``mod_proxy`` attualmente `non supporta i socket unix`_), abilitare ``mod_proxy``
e ``mod_proxy_fcgi`` nella configurazione di Apache e usare la direttiva ``ProxyPassMatch``
per passare richieste di file PHP a PHP FPM:

.. code-block:: apache

    <VirtualHost *:80>
        ServerName dominio.tld
        ServerAlias www.dominio.tld

        # Scommentare la riga seguente per forzare Apache a passare l'header di autenticazione
        # a PHP: necessario per "basic_auth" sotto PHP-FPM e FastCGI
        #
        # SetEnvIfNoCase ^Authorization$ "(.+)" HTTP_AUTHORIZATION=$1

        # Per Apache 2.4.9 e successivi
        # Usando SetHandler si evitano problemi con ProxyPassMatch in combinazione
        # con mod_rewrite o mod_autoindex
        <FilesMatch \.php$>
            SetHandler proxy:fcgi://127.0.0.1:9000
        </FilesMatch>

        # Se si usa Apache prima di 2.4.9, si consideri di aggiornare o usare invece questo
        # ProxyPassMatch ^/(.*\.php(/.*)?)$ fcgi://127.0.0.1:9000/var/www/progetto/web/$1

        # Se si fa girare l'applicazione Symfony in una sottocartella della document root,
        # cambiare l'espressione regolare di conseguenza:
        # ProxyPassMatch ^/percorso-app/(.*\.php(/.*)?)$ fcgi://127.0.0.1:9000/var/www/progetto/web/$1

        DocumentRoot /var/www/progetto/web
        <Directory /var/www/progetto/web>
            # enable the .htaccess rewrites
            AllowOverride All
            Require all granted
        </Directory>

        # scommentare le seguenti righe se si installano risorse come collegamenti simbolici
        # o si avranno problemi compilando risorse LESS/Sass/CoffeScript
        # <Directory /var/www/project>
        #     Option FollowSymlinks
        # </Directory>

        ErrorLog /var/log/apache2/progetto_error.log
        CustomLog /var/log/apache2/progetto_access.log combined
    </VirtualHost>

PHP-FPM con Apache 2.2
~~~~~~~~~~~~~~~~~~~~~~

Su Apache 2.2 o precedenti, non si può usare ``mod_proxy_fcgi``. Si deve invece usare la
direttiva `FastCgiExternalServer`_. Di conseguenza, la configurazione di Apache
dovrebbe essere come questa:

.. code-block:: apache

    <VirtualHost *:80>
        ServerName dominio.tld
        ServerAlias www.dominio.tld

        AddHandler php5-fcgi .php
        Action php5-fcgi /php5-fcgi
        Alias /php5-fcgi /usr/lib/cgi-bin/php5-fcgi
        FastCgiExternalServer /usr/lib/cgi-bin/php5-fcgi -host 127.0.0.1:9000 -pass-header Authorization

        DocumentRoot /var/www/progetto/web
        <Directory /var/www/progetto/web>
            # enable the .htaccess rewrites
            AllowOverride All
            Order allow,deny
            Allow from all
        </Directory>

        # scommentare le seguenti righe se si installano risorse come collegamenti simbolici
        # o si avranno problemi compilando risorse LESS/Sass/CoffeScript
        # <Directory /var/www/project>
        #     Option FollowSymlinks
        # </Directory>

        ErrorLog /var/log/apache2/progetto_error.log
        CustomLog /var/log/apache2/progetto_access.log combined
    </VirtualHost>

Se si preferisce usare un socket unix, si deve invece usare l'opzione
``-socket``:

.. code-block:: apache

    FastCgiExternalServer /usr/lib/cgi-bin/php5-fcgi -socket /var/run/php5-fpm.sock -pass-header Authorization

.. _web-server-nginx:

Nginx
-----

La basi minime per far funzionare un'applicazione sotto Nginx sono:

.. code-block:: nginx

    server {
        server_name dominio.tld www.dominio.tld;
        root /var/www/progetto/web;

        location / {
            # prova a servire direttamente i file, fallback su app.php
            try_files $uri /app.php$is_args$args;
        }
        # DEV
        # Questa regola va messa solo in ambiente di sviluppo
        # In produzione, non includerla e non eseguire deploy di app_dev.php né di config.php
        location ~ ^/(app_dev|config)\.php(/|$) {
            fastcgi_pass unix:/var/run/php5-fpm.sock;
            fastcgi_split_path_info ^(.+\.php)(/.*)$;
            include fastcgi_params;
            fastcgi_param SCRIPT_FILENAME $document_root$fastcgi_script_name;
            fastcgi_param HTTPS off;
        }
        # PROD
        location ~ ^/app\.php(/|$) {
            fastcgi_pass unix:/var/run/php5-fpm.sock;
            fastcgi_split_path_info ^(.+\.php)(/.*)$;
            include fastcgi_params;
            fastcgi_param SCRIPT_FILENAME $document_root$fastcgi_script_name;
            fastcgi_param HTTPS off;
            # Previene URI che includono il front controller. Questa andrà in 404:
            # http://dominio.tld/app.php/un-percorso
            # Rimuovere la direttiva internal per consentire questi URI
            internal;
        }

        error_log /var/log/nginx/progetto_error.log;
        access_log /var/log/nginx/progetto_access.log;
    }

.. note::

    A seconda della configurazione di PHP-FPM, ``fastcgi_pass`` può anche essere
    ``fastcgi_pass 127.0.0.1:9000``.

.. tip::

    Questa configurazione esegue **solo** ``app.php``, ``app_dev.php`` e ``config.php`` nella
    cartella web. Tutti gli altri file saranno serviti come testo. Ci si **deve**
    anche assicurare se, se si pubblicano ``app_dev.php`` o ``config.php``,
    tali file siano protetti e non disponibili a utenti esterni (il codice
    di controllo a inizio file fa proprio questo).

    Se si hanno altri file PHP nella cartella web e si vuole che siano eseguiti,
    assicurarsi di includerli nel blocco ``location`` visto sopra.

Per opzioni avanzate di configurazione di Nginx, leggere la `documentazione di Nginx`_.

.. _`documentazione di Apache`: http://httpd.apache.org/docs/
.. _`non supporta i socket unix`: https://issues.apache.org/bugzilla/show_bug.cgi?id=54101
.. _`FastCgiExternalServer`: http://www.fastcgi.com/mod_fastcgi/docs/mod_fastcgi.html#FastCgiExternalServer
.. _`documentazione di Nginx`: http://wiki.nginx.org/Symfony
