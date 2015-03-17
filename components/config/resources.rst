.. index::
   single: Config; Caricare risorse

Caricare risorse
================

.. caution::

    ``IniFileLoader`` analizza il contenuto di un file con la funzione
    :phpfunction:`parse_ini_file`. Si possono quindi impostare parametri solo come
    valori stringa. Per impostare parametri come altri tipi di dato
    (come booleano, intero, ecc.), si raccomandano altri tipi di caricatori.

Trovare le risorse
------------------

Il caricamento della configurazione solitamente inizia con la ricerca delle risorse,
nella maggior parte dei casi dei file. Lo si può fare con :class:`Symfony\\Component\\Config\\FileLocator`::

    use Symfony\Component\Config\FileLocator;

    $configDirectories = array(__DIR__.'/app/config');

    $locator = new FileLocator($configDirectories);
    $yamlUserFiles = $locator->locate('utenti.yml', null, false);

Il cercatore di risorse riceve un insieme di posizioni in cui cercare file.
Il primo parametro di ``locate()`` è il nome del file da cercare. Il
secondo parametro può essere il percorso e, se fornito, il cercatore cercherà
prima in tale cartella. Il terzo parametro indica se il cercatore debba
restituire il primo file trovato oppure un array con tutte le
corrispondenze.

Caricatori di risorse
---------------------

Per ciascun tipo di risorsa (Yaml, XML, annotazioni, ecc.) va definito un caricatore.
Ogni caricatore deve implementare :class:`Symfony\\Component\\Config\\Loader\\LoaderInterface`
o estendere la classe astratta :class:`Symfony\\Component\\Config\\Loader\\FileLoader`,
che consente di importare ricorsivamente altre risorse::

    use Symfony\Component\Config\Loader\FileLoader;
    use Symfony\Component\Yaml\Yaml;

    class YamlUserLoader extends FileLoader
    {
        public function load($resource, $type = null)
        {
            $configValues = Yaml::parse(file_get_contents($resource));

            // ... gestione dei valori di configurazione

            // possibile importazione di altri risorse:

            // $this->import('altri_utenti.yml');
        }

        public function supports($resource, $type = null)
        {
            return is_string($resource) && 'yml' === pathinfo(
                $resource,
                PATHINFO_EXTENSION
            );
        }
    }

Trovare il giusto caricatore
----------------------------

La classe :class:`Symfony\\Component\\Config\\Loader\\LoaderResolver` riceve un insieme
di caricatori come primo parametro del suo costruttore. Quando una risorsa (per
esempio un file XML) va caricata, cerca in questo insieme di caricatori
e restituisce il caricatore che supporta questo particolare tipo di risorsa.

La classe :class:`Symfony\\Component\\Config\\Loader\\DelegatingLoader` fa uso
di :class:`Symfony\\Component\\Config\\Loader\\LoaderResolver`. Quando gli viene
richiesto di caricare una risorsa, delega la questione a
:class:`Symfony\\Component\\Config\\Loader\\LoaderResolver`. Se quest'ultimo
trova un caricatore adatto, a tale caricatore sarà chiesto di caricare la risorsa::

    use Symfony\Component\Config\Loader\LoaderResolver;
    use Symfony\Component\Config\Loader\DelegatingLoader;

    $loaderResolver = new LoaderResolver(array(new YamlUserLoader($locator)));
    $delegatingLoader = new DelegatingLoader($loaderResolver);

    $delegatingLoader->load(__DIR__.'/utenti.yml');
    /*
    Sarà usato YamlUserLoader per caricare questa risorsa,
    poiché supporta file con estensione "yml"
    */
