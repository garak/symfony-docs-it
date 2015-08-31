.. index::
    single: Bundle; Rimuovere AcmeDemoBundle

Rimuovere AcmeDemoBundle
========================

La Standard Edition di Symfony dispone di una demo completa, che si trova in un bundle
chiamato AcmeDemoBundle. È un ottimo riferimento durante l'inizio
di un progetto, ma probabilmente si vorrà rimuoverlo in un secondo momento.

.. tip::

    Questa ricetta usa AcmeDemoBundle come esempio, ma si possono usare questi
    passi per rimuovere qualsiasi bundle.

1. De-registrare il bundle in ``AppKernel``
-------------------------------------------

Per sconnettere il bundle dal framework, occorre rimuoverlo dal metodo
``AppKernel::registerBundles()``. Il bundle si trova solitamente
nell'array ``$bundles``, ma AcmeDemoBundle viene registrato solamente
in ambiente di sviluppo, quindi lo si può trovare nell'istruzione successiva::

    // app/AppKernel.php

    // ...
    class AppKernel extends Kernel
    {
        public function registerBundles()
        {
            $bundles = array(...);

            if (in_array($this->getEnvironment(), array('dev', 'test'))) {
                // commentare o eliminare questa riga:
                // $bundles[] = new Acme\DemoBundle\AcmeDemoBundle();
                // ...
            }
        }
    }

2. Rimuovere la configurazione del bundle
-----------------------------------------

Ora che Symfony non sa più nulla del bundle, occorre rimuovere ogni 
configurazione, dalla cartella ``app/config``, che faccia
riferimento al bundle.

2.1 Rimuovere le rotte del bundle
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Le rotte di AcmeDemoBundle si trovano in ``app/config/routing_dev.yml``.
Rimuovere la voce ``_acme_demo`` alla fine di questo file.

2.2 Rimuovere la configurazione del bundle
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Alcuni bundle contengono configurazione nei file ``app/config/config*.yml``.
Assicurarsi di rimuovere la configurazione relativa da questi file. Si può
trovare rapidamente la configurazione del bundle, cercando la stringa ``acme_demo`` (o il
nome del bundle, p.e. ``fos_user`` per FOSUserBundle) nei
file di configurazione.

AcmeDemoBundle non ha configurazioni. Tuttavia, il bundle viene usato
nella configurazione del file ``app/config/security.yml``. Lo si può
usare come base per la sicurezza, ma si **può** anche rimuovere
tutto: a Symfony non importa se lo si rimuove o meno.

3. Rimuovere il bundle dal filesystem
-------------------------------------

Ora che ogni riferimento al bundle è stato rimosso, si può
cancellare il bundle dal filesystem. Il bundle si trova nella cartella
``src/Acme/DemoBundle``. Si rimuova tale cartella e si può
rimuovere anche la cartella ``Acme``.

.. tip::

    Se non si conosce la posizione di un bundle, si può usare  il metodo
    :method:`Symfony\\Component\\HttpKernel\\Bundle\\BundleInterface::getPath`
    per ottenerne il percorso::

        echo $this->container->get('kernel')->getBundle('AcmeDemoBundle')->getPath();

3.1 Rimuovere le risorse del bundle
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Rimuovere le risorse del bundle nella cartella web/ (p.e.
``web/bundles/acmedemo`` per AcmeDemoBundle).

4. Rimuovere l'integrazione con altri bundle
--------------------------------------------

.. note::

    Questa parte non si applica ad AcmeDemoBundle, non essendoci altri bundle
    che dipendono da esso, quindi si può saltare questo passo.

Alcuni bundle dipendono da altri bundle: rimuovendone solo uno, l'altro
probabilmente non funzionerà. Assicurarsi che nessun altro bundle, di terze parti o fatto in casa,
si appoggi al bundle che si vuole rimuovere.

.. tip::

    Se un bundle si appoggia a un altro, la maggior parte delle volte è perché ne usa
    dei servizi. La ricerca della stringa ``acme_demo`` può aiutare a trovare
    tali servizi.

.. tip::

    Se un bundle di terze parti si appoggia a un altro bundle, lo si può trovare
    menzionato nel file ``composer.json``, incluso nella cartella del bundle.
