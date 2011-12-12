SensioGeneratorBundle
=====================

Il bundle ``SensioGeneratorBundle`` estende l'interfaccia a linea di comando di Symfony2,
fornendo nuovi comandi interattivi e intuitivi, per generare scheletri di codice, come
bundle, classi di form o controllori CRUD basati su uno schema di
Doctrine 2.

Installazione
-------------

`Scaricare`_ il bundle e metterlo sotto lo spazio dei nomi ``Sensio\\Bundle\\``.
Quindi, come per ogni altro bundle, includerlo nella propria classe Kernel::

    public function registerBundles()
    {
        $bundles = array(
            ...

            new Sensio\Bundle\GeneratorBundle\SensioGeneratorBundle(),
        );

        ...
    }

Elenco dei comandi disponibili
------------------------------

Il bundle ``SensioGeneratorBundle`` dispone di quattro nuovi comandi, eseguibili in
modalità interattiva o meno. La modalità interattiva pone alcune domande, per configurare
i parametri di generazione del codice. L'elenco dei nuovi comandi è il
seguente:

.. toctree::
   :maxdepth: 1

   commands/generate_bundle
   commands/generate_doctrine_crud
   commands/generate_doctrine_entity
   commands/generate_doctrine_form

.. _Scaricare: http://github.com/sensio/SensioGeneratorBundle
