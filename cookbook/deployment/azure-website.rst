.. index::
   single: Deploy; Deploy su Microsoft Azure

Deploy su Microsoft Azure
=========================

Questa ricetta descrivere come eseguire passo-passo il deploy di una piccola
applicazione Symfony2 sulla piattaforma di cloud Microsoft Azure. Spiegherà come
preparare un nuovo sito Azure, inclusa la configurazione della corretta versione di PHP e
delle variabili di ambiente. Verrà anche mostrato come si possono sfruttare
Git and Composer per il deploy di un'applicaizone Symfony nel cloud.

Preparazione di Azure Website
-----------------------------

Per preparare un nuovo Microsoft Azure Website, `iscriversi ad Azure`_ o entrare
con le proprie credenziali. Una volta connessi all'interfaccia `Azure Portal`_,
andare in fondo e selezionare il pannello **New**. In questo pannello, cliccare
**Web Site** e scegliere **Custom Create**:

.. image:: /images/cookbook/deployment/azure-website/step-01.png
   :alt: Creare un nuovo Azure Website

Passo 1: creare un Web Site
~~~~~~~~~~~~~~~~~~~~~~~~~~~

Sarà richiesto di fornire alcune informazioni di base.

.. image:: /images/cookbook/deployment/azure-website/step-02.png
   :alt: Setup di Azure Website

Per l'URL, inserire l'URL che si vuole usare per l'applicazione Symfony,
quindi scegliere **Create new web hosting plan** nella regione desiderata. È preselezionato
*free 20 MB SQL database* nella lista delle basi dati. In questa
guida, l'applicazione Symfony si connetterà a una base dati MySQL. Scegliere
l'opzione **Create a new MySQL database** nella lista. Si può mantenere
il nome **DefaultConnection**. Infine, spuntare la casella
**Publish from source control** per abilitare un repository Git e andare al
passo successivo.

Passo 2: nuova base dati MySQL
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

In questo passo, sarà richiesto di configurare la base dati MySQL con un
nome e una regione. La base dati MySQL è fornita da Microsoft
in collaborazione con ClearDB. Scegliere la stessa regione selezionata nel
passo precedente.

.. image:: /images/cookbook/deployment/azure-website/step-03.png
   :alt: Setup della base dati MySQL

Accettare i termini e le condizioni e cliccare sulla freccia per continuare.

Passo 3:dove si trova il codice sorgente
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Ora, scegliere **Local Git repository** e cliccare
sulla freccia per configurare le credenziali Azure.

.. image:: /images/cookbook/deployment/azure-website/step-04.png
   :alt: Setup di un repository Git locale

Passo 4: nome utente e password
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Ottimo! Questo è l'ultimo passo. Creare un nome utente e una password sicura:
diventeranno identificatori per connettersi al server FTP e
anche per il push del codice al repository Git.

.. image:: /images/cookbook/deployment/azure-website/step-05.png
   :alt: Configurazione delle credenziali Azure

Congratulazioni! Il nuovo Azure Website è pronto. Lo si può verificare
richiamando l'URL del sito configurato nel primo passo. Si dovrebbe
vedere la seguente schermata nel browser:

.. image:: /images/cookbook/deployment/azure-website/step-06.png
   :alt: Azure Website pronto

Il portale Microsoft Azure fornisce anche un pannello per controllare Azure
Website.

.. image:: /images/cookbook/deployment/azure-website/step-07.png
   :alt: Pannello di controllo Azure Website

Azure Website è pronto! Per Symfony, occorre configurare
ancora alcuni dettagli.

Configurazione di Azure Website per Symfony
-------------------------------------------

Questa sezione specifica come configurare la versione corretta di PHP
per far girare Symfony. Mostra anche come abilitare alcune estensioni PHP necessarie
e come configurare propriamente PHP per un ambiente di produzione.

Configurare l'ultimo PHP 
~~~~~~~~~~~~~~~~~~~~~~~~

Anche se Symfony richiede PHP 5.3.3, è sempre raccomandato
l'uso della versione più recente di PHP, dove possibile. PHP 5.3 non ha più
un supporto ufficiale, ma lo si può aggiornare facilmente in Azure.

Per aggiornare la versione PHP su Azure, andare su **Configure** nel pannello
di controllo e selezionare la versione desiderata.

.. image:: /images/cookbook/deployment/azure-website/step-08.png
   :alt: Abilitare la versione più recente di PHP nel pannello di controllo Azure Website

Cliccare sul bottone **Save** nella barra inferiore per salvare le modifiche e riavviare
il server web.

.. note::

    La scelta di una versione più recente di PHP può migliorare drasticamente le prestazioni.
    PHP 5.5 dispone di un nuovo acceleratore incluso, chiamato OPCache, che
    sostituisce APC. Su Azure Website, OPCache è già abilitato e non è necessario
    installare e configurare APC.

    La seguente schermata mostra l'output di uno script :phpfunction:`phpinfo`
    eseguito da un Azure Website per verificare che PHP 5.5 stia girando con
    OPCache abilitato.

    .. image:: /images/cookbook/deployment/azure-website/step-09.png
       :alt: Configurazione di OPCache

Sistemare le impostazioni di php.ini
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Microsoft Azure consente di personalizzare le impostazioni di configurazione in ``php.ini``,
creando un file personalizzato ``.user.ini`` nella cartella radice
del progetto (``site/wwwroot``).

.. code-block:: ini

    ; .user.ini
    expose_php = Off
    memory_limit = 256M
    upload_max_filesize = 10M

Non c'è *bisogno* di sovrascrivere alcuna di queste impostazioni. La configurazione predefinita di PHP
è già buona, quindi questo è solo un esempio per mostrare come si possano facilmente
modificare le impostazioni di PHP, caricando un proprio file ``.ini``.

Si può creare questo file a mano sul server FTP di Azure Website FTP, sotto
la cartella ``site/wwwroot``, oppure inserirlo in Git. Le credenziali FTP
si trovano nel pannello di controllo  di Azure Website, sotto la voce **Dashboard**,
nella barra laterale. Se si preferisce Git, basta inserire il file ``.user.ini``
nella radice del repository locale e fare un push sul repository di Azure
Website.

.. note::

    Questa ricetta ha una sezione dedicata a come configurare il repository Git di
    Azure Website ed eseguire push dei commit di cui fare deploy. Vedere
    `Deploy da Git`_. Si può anche approfondire la configurazione di PHP
    sulla pagina ufficiale della `documentazione MSDN su PHP`_.

Abilitare l'estensione intl di PHP
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Questa è la parte difficoltosa. Al momento della scrittura di questa ricetta,
Microsoft Azure Website forniva l'estensione ``intl``, ma non abilitata in modo
predefinito. Per abilitare l'estensione ``intl``, non è necessario caricare
alcun file DLL, poiché il file ``php_intl.dll`` esiste già su Azure. In effeteti,
basta spostare tale file nella cartella delle estensioni personalizzate.

.. note::

    La squadra di Microsoft Azure è attualmente al lavoro per abilitare l'estensione ``intl`` di PHP
    in modo predefinito. Nel prossimo futuro, i passi seguenti non
    saranno più necessari.

Per avere il file ``php_intl.dll`` sotto la cartella ``site/wwwroot``, accedere
allo strumento online **Kudu**, visitando il seguente URL:

.. code-block:: text

    https://[nome-website].scm.azurewebsites.net

**Kudu** è un insieme di strumenti per gestire un'applicazione. Dispone di un
gestore di file, una linea di comando, un flusso di log e una pagina sommario per la
configurazione. Ovviamente, si può accedere a tale sezione solo dopo l'accesso
in Azure Website.

.. image:: /images/cookbook/deployment/azure-website/step-10.png
   :alt: Il pannello di Kudu

Dalla pagina principale di Kudu, cliccare su **Debug Console** nel
menù principale e scegliere **CMD**. Si dovrebbe aprire la pagina **Debug Console**,
che mostra un gestore di file una linea di comando più sotto.

Nella linea di comando, inserire i seguenti tre comandi, per copiare il file
``php_intl.dll`` nella cartella ``ext/``. Questa nuova
cartella va creata sotto la cartella principale, ``site/wwwroot``.

.. code-block:: bash

    $ cd site\wwwroot
    $ mkdir ext
    $ copy "D:\Program Files (x86)\PHP\v5.5\ext\php_intl.dll" ext

L'intero processo e l'output dovrebbero essere così:

.. image:: /images/cookbook/deployment/azure-website/step-11.png
   :alt: Eseguire comandi nel terminale online di Kudu

Per completare l'attivazione dell'estensione ``php_intl.dll``, si deve dire ad
Azure Website  di caricare la nuova cartella ``ext``. Lo si può fare
registrando una variabile di ambiente globale ``PHP_EXTENSIONS`` dalla
voce **Configure** del pannello di controllo di Azure Website.

Nella sezione **app settings**, registrare la variabile di ambiente ``PHP_EXTENSIONS``
con il valore ``ext\php_intl.dll``, come mostrato in questa schermata:

.. image:: /images/cookbook/deployment/azure-website/step-12.png
   :alt: Registrare estensioni PHP

Cliccare su "save" per confermare le modifiche e far ripartire il server web. L'estensione ``Intl``
dovrebbe ora essere disponibile sul server web. La schermata seguente
di una pagina :phpfunction:`phpinfo` verifica che l'estensione ``intl`` sia
propriamente abilitata:

.. image:: /images/cookbook/deployment/azure-website/step-13.png
   :alt: Estensione Intl abilitata

Ottimo! La configurazione dell'ambiente PHP è ora completa. Il prossimo passo è quello
di configurare il repository Git e inviare codice in produzione. Si vedrà anche
come installare e configurare l'app Symfony, dopo il suo deploy.

Deploy da Git
~~~~~~~~~~~~~

Assicurarsi innanzitutto che Git sia installato correttamente sulla macchina locale,
con il seguente comando da terminale:

.. code-block:: bash

    $ git --version

.. note::

    Scaricare Git dal sito `git-scm.com`_ e seguire le istruzioni
    per installarlo e configuralo sulla macchina locale.

Nel pannello di controllo di Azure Website, andare su **Deployment** per ottenere
l'URL del repository Git da usare:

.. image:: /images/cookbook/deployment/azure-website/step-14.png
   :alt: Pannello Git

Ora, si connetteranno l'applicazione Symfony locale con questo repository remoto
Git su Azure Website. Se l'applicazione Symfony non è ancora
su Git, occorre prima creare un repository Git nella cartella dell'applicazione Symfony,
con il comando ``git init``, ed eseguire il commit con il comando
``git commit``.

Assicurarsi anche di avere un file ``.gitignore`` nella cartella radice del repository,
con almento il seguente contenuto:

.. code-block:: text

    /app/bootstrap.php.cache
    /app/cache/*
    /app/config/parameters.yml
    /app/logs/*
    !app/cache/.gitkeep
    !app/logs/.gitkeep
    /app/SymfonyRequirements.php
    /build/
    /vendor/
    /bin/
    /composer.phar
    /web/app_dev.php
    /web/bundles/
    /web/config.php 

Il file ``.gitignore`` dice a Git di non tracciare i file e le cartelle che corrispondono
a questi schemi. Questo vuol dire che questi file non saranno inclusi nel deploy su Azure
Website.

Ora, dalla linea di comando della macchina local, inserire i seguenti comandi, dalla
cartella radice del progetto Symfony:

.. code-block:: bash

    $ git remote add azure https://<nomeutente>@<nome-website>.scm.azurewebsites.net:443/<nome-website>.git
    $ git push azure master

(TODO traduzione da finire...)
Don't forget to replace the values enclosed by ``<`` and ``>`` with your custom
settings displayed in the **Deployment** tab of your Azure Website panel. The
``git remote`` command connects the Azure Website remote Git repository and
assigns an alias to it with the name ``azure``. The second ``git push`` command
pushes all your commits to the remote ``master`` branch of your remote ``azure``
Git repository.

The deployment with Git should produce an output similar to the screenshot
below:

.. image:: /images/cookbook/deployment/azure-website/step-15.png
   :alt: Deploying files to the Git Azure Website repository

The code of the Symfony application has now been deployed to the Azure Website
which you can browse from the file explorer of the Kudu application. You should
see the ``app/``, ``src/`` and ``web/`` directories under your ``site/wwwroot``
directory on the Azure Website filesystem.

Configurare l'applicazione Symfony
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

PHP has been configured and your code has been pushed with Git. The last
step is to configure the application and install the third party dependencies
it requires that aren't tracked by Git. Switch back to the online **Console**
of the Kudu application and execute the following commands in it:

.. code-block:: bash

    $ cd site\wwwroot
    $ curl -sS https://getcomposer.org/installer | php
    $ php -d extension=php_intl.dll composer.phar install

The ``curl`` command retrieves and downloads the Composer command line tool and
installs it at the root of the ``site/wwwroot`` directory. Then, running
the Composer ``install`` command downloads and installs all necessary third-party
libraries.

This may take a while depending on the number of third-party dependencies
you've configured in your ``composer.json`` file.

.. note::

    The ``-d`` switch allows you to quickly override/add any ``php.ini`` settings.
    In this command, we are forcing PHP to use the ``intl`` extension, because
    it is not enabled by default in Azure Website at the moment. Soon, this
    ``-d`` option will no longer be needed since Microsoft will enable the
    ``intl`` extension by default.

At the end of the ``composer install`` command, you will be prompted to fill in
the values of some Symfony settings like database credentials, locale, mailer
credentials, CSRF token protection, etc. These parameters come from the
``app/config/parameters.yml.dist`` file.

.. image:: /images/cookbook/deployment/azure-website/step-16.png
   :alt: Configuring Symfony global parameters

The most important thing in this cookbook is to correctly setup your database
settings. You can get your MySQL database settings on the right sidebar of the
**Azure Website Dashboard** panel. Simply click on the
**View Connection Strings** link to make them appear in a pop-in.

.. image:: /images/cookbook/deployment/azure-website/step-17.png
   :alt: MySQL database settings

The displayed MySQL database settings should be something similar to the code
below. Of course, each value depends on what you've already configured.

.. code-block:: text

    Database=mysymfony2MySQL;Data Source=eu-cdbr-azure-north-c.cloudapp.net;User Id=bff2481a5b6074;Password=bdf50b42

Switch back to the console and answer the prompted questions and provide the
following answers. Don't forget to adapt the values below with your real values
from the MySQL connection string.

.. code-block:: text

    database_driver: pdo_mysql
    database_host: u-cdbr-azure-north-c.cloudapp.net
    database_port: null
    database_name: mysymfony2MySQL
    database_user: bff2481a5b6074
    database_password: bdf50b42
    // ...

Don't forget to answer all the questions. It's important to set a unique random
string for the ``secret`` variable. For the mailer configuration, Azure Website
doesn't provide a built-in mailer service. You should consider configuring
the host-name and credentials of some other third-party mailing service if
your application needs to send emails.

.. image:: /images/cookbook/deployment/azure-website/step-18.png
   :alt: Configuring Symfony

Your Symfony application is now configured and should be almost operational. The
final step is to build the database schema. This can easily be done with the
command line interface if you're using Doctrine. In the online **Console** tool
of the Kudu application, run the following command to mount the tables into your
MySQL database.

.. code-block:: bash

    $ php app/console doctrine:schema:update --force

This command builds the tables and indexes for your MySQL database. If your
Symfony application is more complex than a basic Symfony Standard Edition, you
may have additional commands to execute for setup (see :doc:`/cookbook/deployment/tools`).

Make sure that your application is running by browsing the ``app.php`` front
controller with your web browser and the following url:

.. code-block:: bash

    http://<your-website-name>.azurewebsites.net/web/app.php

If Symfony is correctly installed, you should see the front page of your Symfony
application showing.

Configurare il server web
~~~~~~~~~~~~~~~~~~~~~~~~~

At this point, the Symfony application has been deployed and works perfectly on
the Azure Website. However, the ``web`` folder is still part of the url, which
you definitely don't want. But don't worry! You can easily configure the web
server to point to the ``web`` folder and remove the ``web`` in the URL (and
guarantee that nobody can access files outside of the ``web`` directory.)

To do this, create and deploy (see previous section about Git) the following
``web.config`` file. This file must be located at the root of your project
next to the ``composer.json`` file. This file is the Microsoft IIS Server
equivalent to the well-known ``.htaccess`` file from Apache. For a Symfony
application, configure it with the following content:

.. code-block:: xml

    <!-- web.config -->
    <?xml version="1.0" encoding="UTF-8"?>
    <configuration>
      <system.webServer>
        <rewrite>
          <rules>
            <clear />
            <rule name="BlockAccessToPublic" patternSyntax="Wildcard" stopProcessing="true">
              <match url="*" />
              <conditions logicalGrouping="MatchAll" trackAllCaptures="false">
                <add input="{URL}" pattern="/web/*" />
              </conditions>
              <action type="CustomResponse" statusCode="403" statusReason="Forbidden: Access is denied." statusDescription="You do not have permission to view this directory or page using the credentials that you supplied." />
            </rule>
            <rule name="RewriteAssetsToPublic" stopProcessing="true">
              <match url="^(.*)(\.css|\.js|\.jpg|\.png|\.gif)$" />
              <conditions logicalGrouping="MatchAll" trackAllCaptures="false">
              </conditions>
              <action type="Rewrite" url="web/{R:0}" />
            </rule>
            <rule name="RewriteRequestsToPublic" stopProcessing="true">
              <match url="^(.*)$" />
              <conditions logicalGrouping="MatchAll" trackAllCaptures="false">
              </conditions>
              <action type="Rewrite" url="web/app.php/{R:0}" />
            </rule>
          </rules>
        </rewrite>
      </system.webServer>
    </configuration>

As you can see, the latest rule ``RewriteRequestsToPublic`` is responsible for
rewriting any urls to the ``web/app.php`` front controller which allows you to
skip the ``web/`` folder in the URL. The first rule called ``BlockAccessToPublic``
matches all url patterns that contain the ``web/`` folder and serves a
``403 Forbidden`` HTTP response instead. This example is based on Benjamin
Eberlei's sample you can find on Github in the `SymfonyAzureEdition`_ bundle.

Deploy this file under the ``site/wwwroot`` directory of the Azure Website and
browse to your application without the ``web/app.php`` segment in the URL.

Conclusione
-----------

Nice work! You've now deployed your Symfony application to the Microsoft
Azure Website Cloud platform. You also saw that Symfony can be easily configured
and executed on a Microsoft IIS web server. The process is simple and easy
to implement. And as a bonus, Microsoft is continuing to reduce the number
of steps needed so that deployment becomes even easier.

.. _`iscriversi ad Azure`: https://signup.live.com/signup.aspx
.. _`Azure Portal`: https://manage.windowsazure.com
.. _`documentazione MSDN su PHP`: http://blogs.msdn.com/b/silverlining/archive/2012/07/10/configuring-php-in-windows-azure-websites-with-user-ini-files.aspx
.. _`git-scm.com`: http://git-scm.com/download
.. _`SymfonyAzureEdition`: https://github.com/beberlei/symfony-azure-edition/




