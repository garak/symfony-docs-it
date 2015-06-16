.. index::
   single: Bundle; Installazione

Installare bundle di terze parti
================================

La maggior parte dei bundle forniscono istruzioni di installazione. Tuttavia, i
passi di base per installare un bundle sono gli stessi:

* `A) Aggiungere dipendenze a Composer`_
* `B) Abilitare il bundle`_
* `C) Configurare il bundle`_

A) Aggiungere dipendenze a Composer
-----------------------------------

Le dipendenze sono gestite con Composer, se quindi non si ha familiarità con Composer,
leggerne la `documentazione`_. Ci sono due passi:

1) Trovare il nome del bundle su Packagist
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Il file README di un bundle (per esempio quello di `FOSUserBundle`_) solitamente ne indica il nome
(p.e. ``friendsofsymfony/user-bundle``). Nel caso non lo facesse, si può cercare
la libreria sul sito `Packagist.org`_.

.. note::

    Alla ricerca di bundle? Provare a dare un'occhiata su `KnpBundles.com`_: l'archivio non
    ufficiale di bundle per Symfony.

2) Installare ile bundle con Composer
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Una volta trovato il nome del pacchetto, lo si può installare con Composer:

.. code-block:: bash

    $ composer require friendsofsymfony/user-bundle

Questo comando sceglierà la versione migliore per il progetto, aggiungendola a ``composer.json``
e scaricando la libreria nella cartella ``vendor/``. Se si ha bisogno di una specifica
versione, aggiungerla come secondo parametro del comando:

.. code-block:: bash

    $ composer require friendsofsymfony/user-bundle "~2.0"

B) Abilitare il bundle
----------------------

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
                // ...
                new FOS\UserBundle\FOSUserBundle(),
            );

            // ...
        }
    }

In alcuni rari casi, si potrebbe voler abilitare un bundle *solo* in
:doc:`ambiente </cookbook/configuration/environments>` di sviluppo. Per esempio,
DoctrineFixturesBundle può caricare dati fittizi, il che probabilmente è utile
solo durante lo sviluppo. Per caricare un bundle solo in ambiente ``dev``
e ``test``, registrarlo in questo modo::

    // app/AppKernel.php

    // ...
    class AppKernel extends Kernel
    {
        // ...

        public function registerBundles()
        {
            $bundles = array(
                // ...
            );

            if (in_array($this->getEnvironment(), array('dev', 'test'))) {
                $bundles[] = new Doctrine\Bundle\FixturesBundle\DoctrineFixturesBundle();
            }

            // ...
        }
    }

C) Configurare il bundle
------------------------

Di solito un bundle richiede un po' di configurazione, da aggiungere al
file ``app/config/config.yml``. La documentazione del bundle probabilmente
descriverà tale configurazione. Ma si può anche ottenere un riferimento alla
configurazione del bundle tramite il comando ``config:dump-reference``:

.. code-block:: bash

    $ app/console config:dump-reference AsseticBundle

Invece del nome completo del bundle, si può anche passare il nome breve, usato come radice
della configurazione del bundle stesso:

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
fare successivamente. Buon divertimento!

.. _documentazione:      http://getcomposer.org/doc/00-intro.md
.. _Packagist.org:       https://packagist.org
.. _FOSUserBundle:       https://github.com/FriendsOfSymfony/FOSUserBundle
.. _KnpBundles.com:      http://knpbundles.com/
.. _`composer require`:  https://getcomposer.org/doc/03-cli.md#require
