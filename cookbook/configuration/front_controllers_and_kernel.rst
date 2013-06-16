.. index::
   single: Come interagiscono front controller, ``AppKernel`` e 
   ambienti

Capire come interagiscono front controller, Kernel e ambienti
=============================================================

La sezione :doc:`/cookbook/configuration/environments` ha spiegato le basi di come
Symfony usi degli ambienti per eseguire la vostra applicazione con configurazioni diverse.
Questa sezione spiegherà un po' più in dettaglio cosa succeda quando la vostra applicazione
attraversa la fase di bootstrapp. Per agganciarsi a questo processo avete bisogno di capire
tre elementi che lavorano assieme:

* `The Front Controller`_
* `The Kernel Class`_
* `The Environments`_

.. note::

    Normalmente non avete bisogno di definire il vostro front controller
    o la classe ``AppKernel`` dato che `Symfony2 Standard Edition`_ fornisce
    delle ragionevoli implementazioni di default

    Questa sezione di documentazione viene fornita per spiegare cosa succeda
    dietro le quinte.

Il Front Controller
-------------------

Il `front controller`_ è un design pattern molto conosciuto; è un pezzo di codice attraverso 
il quale passano *tutte* le richieste soddisfatte da una applicazione.

In `Symfony2 Standard Edition`_, questo ruolo viene svolto dai file `app.php`_
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
* Abilitare il :doc:`Debug Component </components/debug>`.

Il frint controller può essere scelto richiedendo URL come:

.. code-block:: text

     http://localhost/app_dev.php/some/path/...

Come potete vedere, questo URL contiene lo script PHP che deve essere usato
come front controller. Potete usarlo per scambiare facilmente il front controller 
o usarne uno custom mettendolo nella cartella ``web/`` (ad es. ``app_cache.php``).

Se si usa Apache e la `RewriteRule shipped with the Standard Edition`_,
si può omettere il nome del file dall'URL e la RewriteRule userà ``app.php``
come default.

.. note::

    Praticamente ogni webserver permette di ottenere un comportamento
    simile a quello della RewriteRule descritta qui sopra.
    Controllate la documentazione del vostro webserver per i dettagli o guardate
    :doc:`/cookbook/configuration/web_server_configuration`.

.. note::

    Accertatevi di mettere in sicurezza il vostro front controller rispetto ad
    accessi non autorizzati. Per esempio non volete che l'ambiente di debug
    sia disponibile in ambiente di produzione.

Tecnicamente lo script `app/console`_ usato quando si lancia Symfony da riga di comando
è anche esso un front controller, solo che non viene usato per richieste web, bensì per 
richieste da riga di comando

La classe Kernel
----------------

La classe :class:`Symfony\\Component\\HttpKernel\\Kernel` è il cuore di 
Symfony2. È responsabile del setup di tutti i bundle che compongono la vostra
applicazione e fornisce loro la configurazione dell'applicazione.
Il Kernel crea poi un service container prima di gestire le richieste col suo metodo
:method:`Symfony\\Component\\HttpKernel\\HttpKernelInterface::handle`.

Ci sono due metodi dichiarati nell'interfaccia
:class:`Symfony\\Component\\HttpKernel\\KernelInterface` e che sono non implementati
nella classe :class:`Symfony\\Component\\HttpKernel\\Kernel`
servendo quindi come `template methods`_:

* :method:`Symfony\\Component\\HttpKernel\\KernelInterface::registerBundles`,
  che deve ritornare un array di tutti i Bundle necessari per eseguire l'applicazione.

* :method:`Symfony\\Component\\HttpKernel\\KernelInterface::registerContainerConfiguration`,
  che carica la configurazione dell'applicazione.

Per riempire questi (piccoli) buchi la vostra applicazione deve essere una sottoclasse 
del Kernel e implementare questi metodi. La classe che ne risulta viene convenzionalmente chiamata``AppKernel``.

Ancora una volta Symfony2 Standard Edition fornisce un `AppKernel`_ nella cartella ``app/``. 
Per decidere quali Bundle creare questa classe usa il nome dell'ambiente, che viene passato al 
metodo del Kernel  :method:`constructor<Symfony\\Component\\HttpKernel\\Kernel::__construct>`
ed è ottenibile tramite il metodo :method:`Symfony\\Component\\HttpKernel\\Kernel::getEnvironment` -.
La logica per ottenere questo si trova nel metodo ``registerBundles()``,
un metodo pensato per essere esteso da voi quando iniziate ad aggiungere bundles alla vostra applicazione.

Siete ovviamente liberi di creare la vostra variante di ``AppKernel``,
alternativa o aggiuntiva a quella di default.
Tutto quello che dovete fare è adattare il vostro front controller (o aggiungerne uno novo)
perché usi il nuovo kernel.

.. note::

    Il nome e la posizione di ``AppKernel`` non sono fissati. QUando
    si mettono kernel multipli in una singola applicazione, può avere senso 
    aggiungere sotto-cartelle aggiuntive, ad esempio: ``app/admin/AdminKernel.php`` e
    ``app/api/ApiKernel.php``. Quello che conta è che il vostro front controller sia 
    in grado di creare una istanza del kernel appropriato.

Avere diversi ``AppKernels`` può essere utile per abilitare diversi front-controller
(potenzialmente su diversi server) per eseguire indipendentemente parti della vostra 
applicazione (per esempio la UI lato admin, la UI del frontend e le migrazioni di database).

.. note::

    Ci sono molti altri casi in cui si può usare ``AppKernel``,
    ad esempio :doc:`overriding the default directory structure </cookbook/configuration/override_dir_structure>`.
    Ma ci sono ottime probabilità che non abbiate bisogno di cambiare cose di questo genere al volo
    se avete varie implementazioni multiple dell'``AppKernel``.

Gli Ambienti
------------

Abbiamo appena menzionato un altro metoodo che l'``AppKernel`` deve implementare -
:method:`Symfony\\Component\\HttpKernel\\KernelInterface::registerContainerConfiguration`.
Questo metodo è responsabile del caricamento della configurazione dell'applicazione dall'*ambiente* corretto.

Gli ambienti sono stati trattati in amniera estesa
:doc:`in the previous chapter</cookbook/configuration/environments>`,
e probabilmente ricorderete che la Standard Edition ne ha tre - ``dev``, ``prod`` e ``test``.

Più tecnicamente questi nomi non sono altro che stringhe passate dal front controller al costruttore dell'
``AppKernel``. Questo nome può essere usato nel metodo :method:`Symfony\\Component\\HttpKernel\\KernelInterface::registerContainerConfiguration`
che decide quale file di configurazione caricare.

La classe `AppKernel`_ della Standard Edition implementa questo metodo caricando semplicemente 
il file ``app/config/config_*environment*.yml`` . Voi siete ovviamente liberi di implementare questo 
metodo diversamente se vi serve un sistema più sofisticato per caricare la vostra configurazione.

.. _front controller: http://en.wikipedia.org/wiki/Front_Controller_pattern
.. _Symfony2 Standard Edition: https://github.com/symfony/symfony-standard
.. _app.php: https://github.com/symfony/symfony-standard/blob/master/web/app.php
.. _app_dev.php: https://github.com/symfony/symfony-standard/blob/master/web/app_dev.php
.. _app/console: https://github.com/symfony/symfony-standard/blob/master/app/console
.. _AppKernel: https://github.com/symfony/symfony-standard/blob/master/app/AppKernel.php
.. _decorate: http://en.wikipedia.org/wiki/Decorator_pattern
.. _RewriteRule shipped with the Standard Edition: https://github.com/symfony/symfony-standard/blob/master/web/.htaccess)
.. _template methods: http://en.wikipedia.org/wiki/Template_method_pattern
