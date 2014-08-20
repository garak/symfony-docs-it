.. index::
   single: Deploy; Strumenti di deploy

Deploy di un'applicazione Symfony2
==================================

.. note::

    Il deploy può essere un'attività complessa e variegata, a seconda dell'ambiente e
    delle necessità. Questa ricetta non vuole essere esaustiva, ma offrire le idee e
    i requisiti più comuni per il deploy.

Basi per i deploy su Symfony2
-----------------------------

I tipici passi durante il deploy di un'applicazione Symfony2 includono:

#. Caricare il nuovo codice su un server;
#. Aggiornare le dipendenze (tipicamente eseguito tramite Composer, potrebbe anche
   essere fatto prima di caricare);
#. Eseguire le migrazioni della base dati, oppure compiti simili, per aggiornare strutture dati modificate;
#. Pulire (e forse più importante, scaldare) la cache.

Un deploy può anche includere altro, come:

* Assegnazione di un tag a una versione del codice, come rilascio nel proprio repository di gestione dei sorgenti;
* Creazione di un'area di stage temporanea, per costruire il proprio ambiente aggiornato "offline";
* Esecuzione dei test disponibili, per assicurarsi la stabilità del codice e/o del server;
* Rimozione dei file non necessari da ``web``, per mantenere pulito l'ambiente di produzione;
* Pulizia dei sistemi di cache esterni (come `Memcached`_ o `Redis`_).

Come eseguire il deploy
-----------------------

Ci sono molti modi per eseguire il deploy di un'applicazione Symfony2.

Iniziamo con alcune strategie di deploy di base.

Trasferimento semplice di file
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

La forma più semplice di deploy consiste nel copiare i file manualmente tramite
ftp/scp (o metodi simili). Ci sono degli svantaggi, perché manca il controllo
sul sistema mentre l'aggiornamento è in corso. Questo metodo inoltre richiede di
eseguire dei passi manuali dopo il trasferimento dei file (vedere `Azioni comuni post-deploy`_)

Uso di un controllo dei sorgenti
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Se si usa un controllo dei sorgenti (come Git o SVN), si può semplificare rendendo
l'installazione una copia del repository. Quando si è pronti per
aggiornare, basta recuperare l'ultimo aggiornamento dal sistema di controllo
dei sorgenti.

Questo rende l'aggiornamento *più facile*, ma occorre ancora prendersi cura di alcuni
passi in modo manuale (vedere `Azioni comuni post-deploy`_).

Uso di script e di altri strumenti
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Ci sono alcuni strumenti di alta qualità, che facilitano i deploy. Alcuni di questi
sono stati adattati in modo specifico ai requisiti di
Symfony2, assicurandosi di tenere conto di ogni cosa prima, durante e
dopo che un deploy sia stato eseguito correttamente.

Vedere `Strumenti`_ per un elenco di strumenti che possono aiutare con i deploy.

Azioni comuni post-deploy
-------------------------

Dopo il deploy del codice sorgente, ci sono alcune azioni comuni che
occorrerà intraprendere:

A) Verificare i requisiti
~~~~~~~~~~~~~~~~~~~~~~~~~

Verificare che il server soddisfi i requisiti, eseguendo:

.. code-block:: bash

    $ php app/check.php

B) Configurare il file ``app/config/parameters.yml``
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Questo file andrebbe personalizzato su ogni sistema. Il metodo usato per il
deploy del codice *non* dovrebbe modificare questo file. Invece, lo si dovrebbe
modificare manualmente (o tramite un processo) direttamente sul server.

C) Aggiornare i venditori
~~~~~~~~~~~~~~~~~~~~~~~~~

I venditori possono essere aggiornati prima del trasferimento del codice (aggiornando
la cartella ``vendor/``, quindi trasferendola insieme al codice
sorgente) oppure successivamente. In ogni modo, aggiornare i venditori come si
fa normalmente:

.. code-block:: bash

    $ php composer.phar install --no-dev --optimize-autoloader

.. tip::

    L'opzione ``--optimize-autoloader`` rende l'autoloader di Composer più
    performante, costruendo una "mappa di classi". L'opzionoe ``--no-dev``
    assicura che i pacchetti di sviluppo non siano installati in ambiente
    di produzione.

D) Pulire la cache di Symfony
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Assicurarsi di pulire (e riscaldare) la cache di Symfony:

.. code-block:: bash

    $ php app/console cache:clear --env=prod --no-debug

E) Esportare le risorse di Assetic
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Se si usa Assetic, si vorranno esportare le risorse:

.. code-block:: bash

    $ php app/console assetic:dump --env=prod --no-debug

F) Altre cose!
~~~~~~~~~~~~~~

Ci possono essere molte altre cose che si potrebbe dover fare, a seconda
dell'ambiente:

* Eseguire migrazioni sulla base dati
* Pulire la cache di APC
* Eseguire ``assets:install`` (già compreso in ``composer.phar install``)
* Aggiungere/modificare script in cron
* Inviare risorsa a un CDN
* ...

Ciclo di vita dell'applicazione: integrazione continua, QA, ecc.
----------------------------------------------------------------

Sebbene questa ricetta copra i dettagli tecnici del deploy, l'intero ciclo di vita
del portare codice da sviluppo a produzione potrebbe avere molti passi ulteriori
(si pensi al deploy in stage, QA, esecuzione di test, eccetera).

L'uso di stage, test, QA, integrazione continua, migrazioni di basi dati
e la capacità di tornare indietro in caso di fallimento sono caldamente consigliati.
Ci sono strumenti semplici e più complessi e si può rendere il deploy semplice
(o sofisticato) quanto si vuole

Non dimenticare che il deploy di un'applicazione coinvolge anche l'aggiornamento di ogni dipendenza
(tipicamente via Composer), migrazioni della base dati, pulizia della cache e
altre possibili questioni, come inviare risorse a un CDN (vedere `Azioni comuni post-deploy`_).

Strumenti
---------

`Capifony`_:

    Fornisce un insieme specializzato di strumenti basati su Capistrano, adattati in
    modo specifico per i progetti symfony e Symfony2.

`sf2debpkg`_:

    Aiuta a costruire un pacchetto Debian nativo per un progetto Symfony2.

`Magallanes`_:

    Simile a Capistrano, ma scritto in PHP, potrebbe essere più facile
    per uno sviluppatore PHP da estendere in base alle necessità.

Bundle:

    Ci sono molti `bundle che aggiungono strumenti di deploy`_ direttamente alla
    console di Symfony2.

Script di base:

    Ovviamente si può usare il terminale, `Ant`_ o altri strumenti di script per
    il deploy di un progetto.

Fornitori di PaaS:

    Un modo relativamente nuovo per il deploy è rappresentato dai PaaS. Tipicamente, un PaaS
    userà un singolo file di configurazione nella cartella radice del progetto per
    determinare come costruire al volo un ambiente che supporti il proprio software.
    Un fornitore che ha confermato supporto a Symfony2 è `PagodaBox`_.

.. tip::

    In cerca di altro? Si può parlare con la comunità sul `canale IRC di Symfony`_ #symfony
    (su freenode) per maggiori informazioni.

.. _`Capifony`: http://capifony.org/
.. _`sf2debpkg`: https://github.com/liip/sf2debpkg
.. _`Ant`: http://blog.sznapka.pl/deploying-symfony2-applications-with-ant
.. _`PagodaBox`: https://github.com/jmather/pagoda-symfony-sonata-distribution/blob/master/Boxfile
.. _`Magallanes`: https://github.com/andres-montanez/Magallanes
.. _`bundle che aggiungono strumenti di deploy`: http://knpbundles.com/search?q=deploy
.. _`canale IRC di Symfony`: http://webchat.freenode.net/?channels=symfony
.. _`Memcached`: http://memcached.org/
.. _`Redis`: http://redis.io/
