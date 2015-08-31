.. index::
   single: Deploy; Deploy su Microsoft Azure

Deploy su Microsoft Azure
=========================

Questa ricetta descrive come eseguire passo-passo il deploy di una piccola
applicazione Symfony sulla piattaforma di cloud Microsoft Azure. Spiegherà come
preparare un nuovo sito Azure, inclusa la configurazione della corretta versione di PHP e
delle variabili di ambiente. Verrà anche mostrato come si possono sfruttare
Git e Composer per il deploy di un'applicazione Symfony nel cloud.

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
alcun file DLL, poiché il file ``php_intl.dll`` esiste già su Azure. In effetti,
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
con almeno il seguente contenuto:

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

Non dimenticare di sostituire i valori compresi tra ``<`` e ``>`` con le impostazioni
personalizzate mostrate sotto la voce **Deployment** del pannello Azure Website. Il comando
``git remote`` connette il repository remoto Git di Azure Website e gli
assegna un alias chiamato ``azure``. Il comando ``git push`` esegue un
push di tutti i commit al ramo remoto ``master`` del repository remoto Git
``azure``.

Il deploy con Git dovrebbe produrre un output simile a quello della schermata
seguente:

.. image:: /images/cookbook/deployment/azure-website/step-15.png
   :alt: Deploy di file nel repository Git di Azure Website

Il codice dell'applicazione Symfony ora è su Azure Website e
può essere sfogliato dal gestore di file dell'applicazione Kudu. Si dovrebbero
vedere le cartelle ``app/``, ``src/`` e ``web/`` sotto la cartella ``site/wwwroot``
sul filesystem Azure Website.

Configurare l'applicazione Symfony
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

PHP è stato configurato e il codice inviato su Git. L'ultimo passo
è configurare l'applicazione e installare le dipendenze di terze parti,
che non sono tracciate da Git. Tornare al **terminale** online
dell'applicazione Kudu ed eseguire i seguenti comandi:

.. code-block:: bash

    $ cd site\wwwroot
    $ curl -sS https://getcomposer.org/installer | php
    $ php -d extension=php_intl.dll composer.phar install

Il comando ``curl`` scarica lo strumento Composer e lo
installa nella cartella radice ``site/wwwroot``. Quindi, si esegue il comando
di Composer ``install``, che scarica e installa le necessarie librerie di
terze parti.

Ci potrebbe volere un po' di tempo, a seconda del numero di dipendenze di terze parti
configurate nel file ``composer.json``.

.. note::

    L'opzione ``-d`` consente di sovrascrivere velocemente impostazioni di ``php.ini``.
    In questo comando, PHP viene forzato a usare l'estensione ``intl``, perché
    attualmente Azure Website non la abilita in modo predefinito. Questa opzione
    ``-d`` sarà presto superflua, perché Microsoft abiliterà l'estensione
    ``intl`` in modo predefinito.

Alla fine del comando ``composer install``, verrà richiesto di compilare alcuni
valori di impostazioni di Symfony, come credenziali per la base dati, locale, credenziali
per il mailer, CSRF, ecc. Questi parametri provengono dal file
``app/config/parameters.yml.dist``.

.. image:: /images/cookbook/deployment/azure-website/step-16.png
   :alt: Configurazione dei parametri globali di Symfony

La cosa più importante in questa ricetta è configurare correttamente le impostazioni
della base dati. Si possono verificare le impostazioni di MySQL nella barra di destra del
pannello del cruscotto di Azure Website. Basta cliccare sul collegamento
**View Connection Strings** per farli apparire.

.. image:: /images/cookbook/deployment/azure-website/step-17.png
   :alt: Impostazioni di MySQL

Le impostazioni della base dati MySQL mostrate dovrebbero assomigliare al codice
seguente. Ovviamente, i valori dipendono da quanto configurato.

.. code-block:: text

    Database=mysymfony2MySQL;Data Source=eu-cdbr-azure-north-c.cloudapp.net;User Id=bff2481a5b6074;Password=bdf50b42

Tornare al terminale e rispondere alle domande, fornendo le seguenti
risposte. Non dimenticare di adattare i valori seguenti ai valori reali
della stringa di connessione MySQL.

.. code-block:: text

    database_driver: pdo_mysql
    database_host: u-cdbr-azure-north-c.cloudapp.net
    database_port: null
    database_name: mysymfony2MySQL
    database_user: bff2481a5b6074
    database_password: bdf50b42
    // ...

Non dimenticare di rispondere a tutte le domande. È importante impostare una stringa unica
e casuale per la variabile ``secret``. Per la configurazione del mailer, Azure Website
non fornisce un servizio mailer predefinito. Si consideri l'ipotesi di configurare
nome host e credenziali di un qualche servizio di mailer esterno, se
l'applicazione ha bisogno di inviare email.

.. image:: /images/cookbook/deployment/azure-website/step-18.png
   :alt: Configurazione di Symfony

L'applicazione Symfony è ora configurata e dovrebbe essere quasi operativa. Il passo
finale è costruire lo schema della base dati. Lo si può fare facilmente con
l'interfaccia a linea di comando, se si usa Doctrine. Nel terminale online
dell'applicazione Kudu, eseguire i comandi seguenti per creare le tabelle nella
base dati MySQL.

.. code-block:: bash

    $ php app/console doctrine:schema:update --force

Questo comando costruisce tabelle e indici per la base dati MySQL. Se
l'applicazione Symfony è più complessa della semplice Standard Edition, potrebbero
essere necessari comandi aggiuntivi (vedere :doc:`/cookbook/deployment/tools`).

Assicurarsi che l'applicazione funzioni, aprendo il front controller ``app.php``
con un browser, nel seguente URL:

.. code-block:: bash

    http://<nome-website>.azurewebsites.net/web/app.php

Se Symfony è installato correttamente, si dovrebbe vedere la pagina iniziale
dell'applicazione Symfony.

Configurare il server web
~~~~~~~~~~~~~~~~~~~~~~~~~

A questo punto, l'applicazione Symfony funziona perfettamente su
Azure Website. Tuttavia, l'URL comprende ancora la cartella ``web``, che non
è desiderabile. Niente paura! Si può facilmente configurare il server web
per puntare alla cartella ``web`` e rimuovere quindi la parte ``web`` dell'URL (e
garantire che nessuno possa accedere a file esterni alla cartella ``web``.)

Per poterlo fare, creare (come visto nella precedente sezione su Git) il seguente file
``web.config``. Questo file deve trovarsi nella radice del progetto, accanto
al file ``composer.json``. Questo file è l'equivalente per Microsoft IIS Server
del ben noto file ``.htaccess`` di Apache. Per un'applicazione Symfony,
configurarlo con il seguente contenuto:

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

Come si può vedere, l'ultima regola ``RewriteRequestsToPublic`` si occupa
di riscrivere ogni URL verso il front controller ``web/app.php``, il che consente
di evitare la cartella ``web/`` nell'URL. La prima regola ``BlockAccessToPublic``
corrisponde a tutti gli schemi di URL che contengano la cartella ``web/`` e serve invece una risposta HTTP
``403 Forbidden``. Questo esempio è basato su un codice di Benjamin
Eberlei, che si può trovare nel bundle `SymfonyAzureEdition`_ su Github.

Inviare questo file sotto la cartella ``site/wwwroot`` di Azure Website e
navigare l'applicazione senza parte ``web/app.php`` dell'URL.

Conclusione
-----------

Bel lavoro! Il deploy dell'applicazione Symfony sulla piattaforma Microsoft
Azure Website Cloud è completo. Abbiamo anche visto quanto sia facile configurare ed eseguire
Symfony su un server web Microsoft IIS. Il processo e semplice e facile
da implementare. Come bonus, Microsoft sta riducendo gradualmente il numero di
passi necessari, per rendere il deploy ancora più facile.

.. _`iscriversi ad Azure`: https://signup.live.com/signup.aspx
.. _`Azure Portal`: https://manage.windowsazure.com
.. _`documentazione MSDN su PHP`: http://blogs.msdn.com/b/silverlining/archive/2012/07/10/configuring-php-in-windows-azure-websites-with-user-ini-files.aspx
.. _`git-scm.com`: http://git-scm.com/download
.. _`SymfonyAzureEdition`: https://github.com/beberlei/symfony-azure-edition/
