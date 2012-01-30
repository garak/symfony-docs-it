.. index::
   single: Test; Profilazione

Come usare il profilatore nei test funzionali
=============================================

È caldamente raccomandato che un test funzionale testi solo la risposta. Ma se si
scrivono test funzionali che monitorano i propri server di produzione, si potrebbe
voler scrivere test sui dati di profilazione, che sono un ottimo strumento per
verificare varie cose e controllare alcune metriche.

Il :doc:`profilatore </book/internals/profiler>` di Symfony2 raccoglie diversi dati
per ogni richiesta. Usare questi dati per verificare il numero di chiamate al database,
il tempo speso nel framework, eccetera. Ma prima di scrivere asserzioni, verificare
sempre che il profilatore sia effettivamente una variabile (è abilitato per impostazione
predefinita in ambiente ``test``)::

    class HelloControllerTest extends WebTestCase
    {
        public function testIndex()
        {
            $client = static::createClient();
            $crawler = $client->request('GET', '/hello/Fabien');

            // Scrivere asserzioni sulla risposta
            // ...

            // Check that the profiler is enabled
            if ($profile = $client->getProfile()) {
                // verificare il numero di richieste
                $this->assertTrue($profile->getCollector('db')->getQueryCount() < 10);

                // verifica il tempo speso nel framework
                $this->assertTrue( $profile->getCollector('timer')->getTime() < 0.5);
            }
        }
    }

Se un test fallisce a causa dei dati di profilazione (per esempio, troppe query al DB),
si potrebbe voler usare il profilatore web per analizzare la richiesta, dopo che i test
sono finiti. È facile, basta inserire il token nel messaggio di errore::

    $this->assertTrue(
        $profile->get('db')->getQueryCount() < 30,
        sprintf('Verifica che ci siano meno di 30 query (token %s)', $profile->getToken())
    );

.. caution::

     I dati del profilatore possono essere differenti, a seconda dell'ambiente
     (specialmente se si usa SQLite, che è configurato in modo
     predefinito).

.. note::

    Le informazioni del profilatore sono disponibili anche se si isola client o se
    se si usa un livello HTTP per i propri test.

.. tip::

    Leggere le API dei :doc:`raccoglitori di dati</cookbook/profiler/data_collector>`
    per saperne di più sulle loro interfacce.
