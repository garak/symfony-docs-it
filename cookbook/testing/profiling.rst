.. index::
   single: Test; Profilazione

Usare il profilatore nei test funzionali
========================================

È caldamente raccomandato che un test funzionale testi solo la risposta. Ma se si
scrivono test funzionali che monitorano i server di produzione, si potrebbe
voler scrivere test sui dati di profilazione, che sono un ottimo strumento per
verificare varie cose e controllare alcune metriche.

Il :ref:`profilatore <internals-profiler>` di Symfony raccoglie diversi dati
per ogni richiesta. Usare questi dati per verificare il numero di chiamate alla base dati,
il tempo speso nel framework, eccetera. Ma, prima di scrivere asserzioni, verificare
sempre che il profilatore sia effettivamente una variabile (è abilitato per impostazione
predefinita in ambiente ``test``)::

    class HelloControllerTest extends WebTestCase
    {
        public function testIndex()
        {
            $client = static::createClient();

            // Abilita il profilatore per la richiesta successiva (se il profilatore non è abilitato, non succede nulla)
            $client->enableProfiler();

            $crawler = $client->request('GET', '/hello/Fabien');

            // Scrivere asserzioni sulla risposta

            // Verifica che il profilatore sia abilitato
            if ($profile = $client->getProfile()) {
                // verificare il numero di richieste
                $this->assertLessThan(
                    10,
                    $profile->getCollector('db')->getQueryCount()
                );

                // verifica il tempo speso nel framework
                $this->assertLessThan(
                    500,
                    $profile->getCollector('time')->getDuration()
                );
            }
        }
    }

Se un test fallisce a causa dei dati di profilazione (per esempio, troppe query alla base dati),
si potrebbe voler usare il profilatore web per analizzare la richiesta, dopo che i test
sono finiti. È facile, basta inserire il token nel messaggio di errore::

    $this->assertLessThan(
        30,
        $profile->get('db')->getQueryCount(),
        sprintf(
            'Verifica che ci siano meno di 30 query (token %s)',
            $profile->getToken()
        )
    );

.. caution::

     I dati del profilatore possono essere differenti, a seconda dell'ambiente
     (specialmente se si usa SQLite, che è configurato in modo
     predefinito).

.. note::

    Le informazioni del profilatore sono disponibili anche se si isola il client o se
    se si usa un livello HTTP per i test.

.. tip::

    Leggere le API dei :doc:`raccoglitori di dati </cookbook/profiler/data_collector>`
    per saperne di più sulle loro interfacce.

Accellerare i test non raccogliendo dati dal profilatore
--------------------------------------------------------

Per evitare di raccogliere dati in ogni test, si può impostare il parametro ``collect``
nella configurazione:

.. configuration-block::

    .. code-block:: yaml

        # app/config/config_test.yml

        # ...
        framework:
            profiler:
                enabled: true
                collect: false

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <?xml version="1.0" encoding="UTF-8" ?>
        <container xmlns="http://symfony.com/schema/dic/services"
            xmlns:framework="http://symfony.com/schema/dic/symfony"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://symfony.com/schema/dic/services http://symfony.com/schema/dic/services/services-1.0.xsd
                        http://symfony.com/schema/dic/symfony http://symfony.com/schema/dic/symfony/symfony-1.0.xsd">

            <!-- ... -->

            <framework:config>
                <framework:profiler enabled="true" collect="false" />
            </framework:config>
        </container>

    .. code-block:: php

        // app/config/config.php

        // ...
        $container->loadFromExtension('framework', array(
            'profiler' => array(
                'enabled' => true,
                'collect' => false,
            ),
        ));

In questo modo, solo i test che richiamano ``$client->enableProfiler()`` raccoglieranno dati.
