.. index::
   single: Bundle; Installazione

Installare bundle di terze parti
================================

La maggior parte dei bundle forniscono istruzioni di installazione. Tuttavia, i
passi di base per installare un bundle sono gli stessi.

Aggiungere le dipendenze in Composer
------------------------------------

A partire da Symfony 2.1, le dipendenze sono gestite con Composer. È
una buona idea imparare le basi di Composer nella sua `documentazione`_.

Prima di poter usare Composer per installare un bundle, cercare su
su `Packagist`_ il pacchetto del bundle. Per esempio, se si cerca il popolare
`FOSUserBundle`_, si troverà un pacchetto di nome `friendsofsymfony/user-bundle`_.

.. note::

    Packagist è l'archivio principale di Composer. Se si cerca un
    bundle, la cosa migliore da fare è controllare
    `KnpBundles`_, l'archivio non ufficiale dei bundle di Symfony. Se
    un bundle contiene un file ``README``, viene mostrato lì e, se ha
    un pacchetto su Packagist, mostra un collegamento al pacchetto. È
    un sito molto utile per iniziare a cercare bundle.

Ora che si ha il nome del pacchetto, si dovrebbe determinare la versione
che si vuole usare. Di solito le varie  versioni di un bundle corrispondono
a una particolare versione di Symfonu. Questa informazione dovrebbe trovarsi nel file ``README``.
Se non c'è, si può usare la versione che si vuole. Se si sceglie una versione non
compatibile, Composer solleverà un'eccezione quando si proverà a installarla. Quando
succede, si può provare con una versione diversa.

Nel caso di FOSUserBundle, il file ``README`` avverte che la versione
1.2.0 va usata per Symfony 2.0 e la 1.3+ per Symfony 2.1+. Packagist mostra
delle istruzioni ``require`` di esempio per ogni versione di un pacchetto. La versione
attualmente in sviluppo di FOSUserBundle è ``"friendsofsymfony/user-bundle": "2.0.*@dev"``.

Ora si può aggiungere il bundle al file ``composer.json`` e aggiornare le
dipendenze. Lo si può fare a mano:

1. **Aggiungere al file composer.json:**

   .. code-block:: json

       {
           ...,
           "require": {
               ...,
               "friendsofsymfony/user-bundle": "2.0.*@dev"
           }
       }

2. **Aggiornare la dipendenza:**

   .. code-block:: bash

       $ php composer.phar update friendsofsymfony/user-bundle

   o aggiornare tutte le dipendenze

   .. code-block:: bash

       $ php composer.phar update

Oppure si può fare tutto in un solo comando:

.. code-block:: bash

    $ php composer.phar require friendsofsymfony/user-bundle:2.0.*@dev

Abilitare il bundle
-------------------

A questo punto, il bundle è installato nel progetto Symfony (in
``vendor/friendsofsymfony/``) e l'autoloader riconosce le sue classi.
L'unica cosa che resta da fare è registrare il bundle in ``AppKernel``::

    // app/AppKernel.php

    // ...
    class AppKernel extends Kernel
    {
        // ...

        public function registerBundles()
        {
            $bundles = array(
                // ...,
                new FOS\UserBundle\FOSUserBundle(),
            );

            // ...
        }
    }

Configurare il bundle
---------------------

Di solito un bundle richiede un po' di configurazione, da aggiungere al file
``app/config/config.yml``. La documentazione del bundle probabilmente
descriverà tale configurazione. Ma si può anche ottenere un riferimento alla
configurazione del bundle tramite il comando ``config:dump-reference``.

Per esepmio, per guardare il riferimento alla configurazione ``assetic``, si
può usare:

.. code-block:: bash

    $ app/console config:dump-reference AsseticBundle

oppure:

.. code-block:: bash

    $ app/console config:dump-reference assetic

Il risultato sarà simile a questo:

.. code-block:: text

    assetic:
        debug:                %kernel.debug%
        use_controller:
            enabled:              %kernel.debug%
            profiler:             false
        read_from:            %kernel.root_dir%/../web
        write_to:             %assetic.read_from%
        java:                 /usr/bin/java
        node:                 /usr/local/bin/node
        node_paths:           []
        # ...

Altre configurazioni
--------------------

A questo punto, verificare nel file ``README`` del bundle cosa si può
fare successivamente.

.. _documentazione:      http://getcomposer.org/doc/00-intro.md
.. _Packagist:           https://packagist.org
.. _FOSUserBundle:       https://github.com/FriendsOfSymfony/FOSUserBundle
.. _`friendsofsymfony/user-bundle`: https://packagist.org/packages/friendsofsymfony/user-bundle
.. _KnpBundles:          http://knpbundles.com/
