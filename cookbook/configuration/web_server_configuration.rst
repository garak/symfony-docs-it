.. index::
   single: Server web

Configurare un server web
=========================

La cartella web è la casa di tutti i file pubblici e statici di
un'applicazione, inclusi fogli di stile, file JavaScript e immagini. È anche il posto
in cui si trovano i front controller. Per maggiori dettagli, vedere :ref:`the-web-directory`.

La cartella web funge da document root nella configurazione del server web.
Negli esempi seguenti, tale cartella è in ``/var/www/project/web/``.

Apache2
-------

Per opzioni avanzate di configurazione di Apache, vedere la documentazione ufficiale di `Apache`_.
La basi minime per far funzionare un'applicazione sotto Apache2
sono:

.. code-block:: apache

    <VirtualHost *:80>
        ServerName domain.tld
        ServerAlias www.domain.tld

        DocumentRoot /var/www/project/web
        <Directory /var/www/project/web>
            # abilita la lettura di .htaccess
            AllowOverride All
            Order allow,deny
            Allow from All
        </Directory>

        ErrorLog /var/log/apache2/project_error.log
        CustomLog /var/log/apache2/project_access.log combined
    </VirtualHost>

.. note::

    Per questioni di prestazione, probabilmente si vorrà impostare
    ``AllowOverride None`` e implementare le regole di riscrittura presenti in ``web/.htaccess``
    direttamente nella cofigurazione dell'host virtuale.

Se si usa **php-cgi**, Apache non passa nome utente e password di HTTP basic
a PHP, per impostazione predefinita. Per aggirare tale limitazione, si dovrebbe usare
la seguente configurazione:

.. code-block:: apache

    RewriteRule .* - [E=HTTP_AUTHORIZATION:%{HTTP:Authorization}]

Nginx
-----

Per opzioni avanzate di configurazione di Nginx, vedere la documentazione ufficiale di `Nginx`_.
La basi minime per far funzionare un'applicazione sotto Nginx
sono:

.. code-block:: nginx

    server {
        server_name domain.tld www.domain.tld;
        root /var/www/project/web;

        location / {
            # prova a servire direttamente i file, fallback su riscrittura
            try_files $uri @rewriteapp;
        }

        location @rewriteapp {
            # riscrittura di tutto su app.php
            rewrite ^(.*)$ /app.php/$1 last;
        }

        location ~ ^/(app|app_dev|config)\.php(/|$) {
            fastcgi_pass unix:/var/run/php5-fpm.sock;
            fastcgi_split_path_info ^(.+\.php)(/.*)$;
            include fastcgi_params;
            fastcgi_param SCRIPT_FILENAME $document_root$fastcgi_script_name;
            fastcgi_param HTTPS off;
        }

        error_log /var/log/nginx/project_error.log;
        access_log /var/log/nginx/project_access.log;
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

.. _`Apache`: http://httpd.apache.org/docs/current/mod/core.html#documentroot
.. _`Nginx`: http://wiki.nginx.org/Symfony
