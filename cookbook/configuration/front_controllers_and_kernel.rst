.. index::
    single: Come interagiscono front controller, ``AppKernel`` e
    ambienti

Capire come interagiscono front controller, Kernel e ambienti
=============================================================

La sezione :doc:`/cookbook/configuration/environments` ha spiegato le basi di come
Symfony usi degli ambienti per eseguire un'applicazione con configurazioni diverse.
Questa sezione spiegherà un po' più in dettaglio cosa succede quando l'applicazione
attraversa la fase di bootstrap. Per agganciarsi a questo processo, è necessario capire
tre elementi che lavorano assieme:

* `Il front controller`_
* `La classe Kernel`_
* `Gli ambienti`_

.. note::

    Normalmente non si ha bisogno di definire un proprio front controller
    o la classe ``AppKernel``, dato che `Symfony Standard Edition`_ fornisce
    delle ragionevoli implementazioni predefinite

    Questa sezione di documentazione viene fornita per spiegare cosa succeda
    dietro le quinte.

Il front controller
-------------------

Il `front controller`_ è un design pattern molto conosciuto; è un pezzo di codice attraverso 
il quale passano *tutte* le richieste soddisfatte da una applicazione.

In `Symfony Standard Edition`_, questo ruolo viene svolto dai file `app.php`_
e `app_dev.php`_ che si trovano nella cartella ``web/``. Questi sono i primi file in assoluto 
che vengono eseguiti quando una richiesta viene processata.

Lo scopo principale del front controller è quello di creare una istanza dell'``AppKernel`` 
(maggiori informazioni su questo fra un attimo), fargli gestire la richiesta e fornire al 
browser la risposta che ne risulta al risulta.

Dato che ogni richiesta ci passa attraverso, il front controller può essere usato per effettuare
una inizializzazione globale prima di fare il setup del kernel o di decorare (`decorate`_) il kernel
con caratteristiche aggiuntive. Fra gli esempi ci sono:

* Configurare l'autoloader o aggiungere meccanismi di autoload aggiuntivi;
* Aggiungere un livello di cache HTTP wrapperizzando il kernel in una istanza di
  :ref:`AppCache<symfony-gateway-cache>`;
* Abilitare (o evitare) la :doc:`ClassCache </cookbook/debugging>`
* Abilitare il :doc:`componente Debug </components/debug>`.

Il front controller può essere scelto richiedendo URL come:

.. code-block:: text

     http://localhost/app_dev.php/un/percorso/...

Come si può vedere, questo URL contiene lo script PHP che deve essere usato
come front controller. Lo si può usare per scambiare facilmente il front controller 
o usarne uno personalizzato, mettendolo nella cartella ``web/`` (ad es. ``app_cache.php``).

Se si usa Apache e la `RewriteRule distribuita con la Standard Edition`_,
si può omettere il nome del file dall'URL e la RewriteRule userà ``app.php``
come default.

.. note::

    Praticamente ogni server web permette di ottenere un comportamento
    simile a quello della RewriteRule descritta qui sopra.
    Controllare la documentazione del server web usato per i dettagli o vedere
    :doc:`/cookbook/configuration/web_server_configuration`.

.. note::

    Accertarsi di mettere in sicurezza il front controller contro
    accessi non autorizzati. Per esempio, non si vuole che l'ambiente di debug
    sia disponibile in ambiente di produzione.

Tecnicamente, lo script `app/console`_ usato quando si lancia Symfony da riga di comando
è anche esso un front controller, solo che non viene usato per richieste web, bensì per 
richieste da riga di comando

La classe Kernel
----------------

La classe :class:`Symfony\\Component\\HttpKernel\\Kernel` è il cuore di 
Symfony. È responsabile del setup di tutti i bundle che compongono
l'applicazione e fornisce loro la configurazione dell'applicazione.
Il Kernel crea poi un contenitore di servizi, prima di gestire le richieste col suo
metodo
:method:`Symfony\\Component\\HttpKernel\\HttpKernelInterface::handle`.

Ci sono due metodi dichiarati nell'interfaccia
:class:`Symfony\\Component\\HttpKernel\\KernelInterface` e che sono non implementati
nella classe :class:`Symfony\\Component\\HttpKernel\\Kernel`,
servendo quindi come `metodi template`_:

:method:`Symfony\\Component\\HttpKernel\\KernelInterface::registerBundles`
    Deve restituire un array di tutti i Bundle necessari per eseguire l'applicazione.
:method:`Symfony\\Component\\HttpKernel\\KernelInterface::registerContainerConfiguration`,
    Carica la configurazione dell'applicazione.

Per riempire questi (piccoli) buchi, l'applicazione deve essere una sottoclasse 
del Kernel e implementare questi metodi. La classe che ne risulta viene convenzionalmente
chiamata ``AppKernel``.

Ancora una volta Symfony Standard Edition fornisce un `AppKernel`_ nella cartella ``app/``. 
Per decidere quali Bundle creare questa classe usa il nome dell'ambiente, che viene passato al
:method:`costruttore<Symfony\\Component\\HttpKernel\\Kernel::__construct>` del Kernel
ed è ottenibile tramite il metodo :method:`Symfony\\Component\\HttpKernel\\Kernel::getEnvironment`,
per decidere quale bundle creare. Questa logica si trova in ``registerBundles()``,
un metodo pensato per essere esteso dallo sviluppatore, quando inizia ad aggiungere bundle
all'applicazione.

Si è ovviamente liberi di creare la propria variante di ``AppKernel``,
alternativa o aggiuntiva a quella di default. Tutto quello che occorre è adattare il
front controller (o aggiungerne uno nuovo) perché usi il nuovo kernel.

.. note::

    Il nome e la posizione di ``AppKernel`` non sono fissati. Quando
    si mettono kernel multipli in una singola applicazione, 
    può avere senso aggiungere sotto-cartelle aggiuntive, ad
    esempio: ``app/admin/AdminKernel.php`` e
    ``app/api/ApiKernel.php``. Quello che conta è che il front
    controller sia in grado di creare una istanza del kernel appropriato.

Avere diversi ``AppKernel`` può essere utile per abilitare diversi front
controller (potenzialmente su diversi server) per eseguire indipendentemente parti dell'applicazione
(per esempio l'interfaccia di amministrazione, l'interfaccia utente e le migrazioni della base dati).

.. note::

    Ci sono molti altri casi in cui si può usare ``AppKernel``, ad esempio per
    :doc:`modificare la struttura predefinita della cartelle </cookbook/configuration/override_dir_structure>`.
    Ma ci sono ottime probabilità che si abbia bisogno di cambiare cose di questo genere al volo,
    se si hanno implementazioni multiple di ``AppKernel``.

Gli ambienti
------------

Abbiamo appena menzionato un altro metoodo che l'``AppKernel`` deve implementare:
:method:`Symfony\\Component\\HttpKernel\\KernelInterface::registerContainerConfiguration`.
Questo metodo è responsabile del caricamento della configurazione dell'applicazione 
dall'*ambiente* corretto.

Gli ambienti sono stati trattati in amniera estesa nel
:doc:`capitolo precedente </cookbook/configuration/environments>`,
e probabilmente si ricorderà che la Standard Edition ne ha tre:
``dev``, ``prod`` e ``test``.

Più tecnicamente, questi nomi non sono altro che stringhe passate dal
front controller al costruttore di ``AppKernel``. Questo nome può essere
usato nel metodo :method:`Symfony\\Component\\HttpKernel\\KernelInterface::registerContainerConfiguration`,
che decide quale file di configurazione caricare.

La classe `AppKernel`_ della Standard Edition implementa questo metodo 
caricando semplicemente  il file ``app/config/config_*ambiente*.yml`` .
Si è ovviamente liberi di implementare questo metodo diversamente,
se serve un sistema più sofisticato per caricare la configurazione.

.. _front controller: http://en.wikipedia.org/wiki/Front_Controller_pattern
.. _Symfony Standard Edition: https://github.com/symfony/symfony-standard
.. _app.php: https://github.com/symfony/symfony-standard/blob/master/web/app.php
.. _app_dev.php: https://github.com/symfony/symfony-standard/blob/master/web/app_dev.php
.. _app/console: https://github.com/symfony/symfony-standard/blob/master/app/console
.. _AppKernel: https://github.com/symfony/symfony-standard/blob/master/app/AppKernel.php
.. _decorate: http://en.wikipedia.org/wiki/Decorator_pattern
.. _RewriteRule  distribuita con la Standard Edition: https://github.com/symfony/symfony-standard/blob/master/web/.htaccess)
.. _metodi template: http://en.wikipedia.org/wiki/Template_method_pattern
