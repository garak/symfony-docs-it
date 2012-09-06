.. index::
   single: Config; Cache basata sulle risorse

Cache basata sulle risorse
==========================

Quando tutte le risorse di configurazione sono state caricate, si potrebbero voler
processare i valori di configurazione e combinarli un unico file. Questo file agisce da
cache. I suoi contenuti non devono essere rigenerati ogni volta che gira l'applicazione,
ma solo quando le risorse di configurazione vengono modificate.

Per esempio, il componente Routing di Symfony consente di caricare tutte le rotte e poi
di esportare un matcher di UL o un generatore di URL, basati su tali rotte. In questo
caso, quando una delle risorse viene modificata (e si sta lavorando in un ambiente di
sviluppo), il file generato va invalidato e rigenerato.
Si può ottenere questo risutlato usando la classe
:class:`Symfony\\Component\\Config\\ConfigCache`.

L'esempio successivo mostra come raccogliere le risorse e generare un codice, basato
sulle risorse caricate, e scrivere tale codice in cache. La cache
riceve anche l'insieme di risorse usate per generare il
codice. Cercando il timestamp "last modified" di tali risorse,
la cache può dirci se è ancora fresca o se i suoi contenuti vanno rigenerati::

    use Symfony\Component\Config\ConfigCache;
    use Symfony\Component\Config\Resource\FileResource;

    $cachePath = __DIR__.'/cache/appUserMatcher.php';

    // il secondo parametro indica se si è in debug o meno
    $userMatcherCache = new ConfigCache($cachePath, true);

    if (!$userMatcherCache->isFresh()) {
        // inserire un array di percorsi per il file 'utenti.yml' file paths
        $yamlUserFiles = ...;

        $resources = array();

        foreach ($yamlUserFiles as $yamlUserFile) {
            $delegatingLoader->load($yamlUserFile);
            $resources[] = new FileResource($yamlUserFile);
        }

        // il codice per UserMatcher è generato altrove
        $code = ...;

        $userMatcherCache->write($code, $resources);
    }

    // si potrebbe voler richiedere il codice in cache:
    require $cachePath;

In debug, sarà creato un file ``.meta`` nella stessa cartella del file di
cache stesso. Tale file ``.meta``  contiene le risorse serializzate, i cui
timestamp sono usati per determinare se la cache è ancora fresca. Se non si è
in debug, la cache è considerata fresca fintanto che esiste, per cui
non viene generato alcun file ``.meta``.
