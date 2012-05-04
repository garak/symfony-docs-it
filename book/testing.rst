.. index::
   single: Test

Test
====

Ogni volta che si scrive una nuova riga di codice, si aggiungono potenzialmente nuovi
bug. Per costruire applicazioni migliori e più affidabili, si dovrebbe sempre testare
il proprio codice, usando sia i test funzionali che quelli unitari.

Il framework dei test PHPUnit
-----------------------------

Symfony2 si integra con una libreria indipendente, chiamata PHPUnit, per fornire
un ricco framework per i test. Questo capitolo non approfondisce PHPUnit stesso, che
ha comunque un'eccellente `documentazione`_.

.. note::

    Symfony2 funziona con PHPUnit 3.5.11 o successivi.

Ogni test, sia esso unitario o funzionale, è una classe PHP,
che dovrebbe trovarsi in una sotto-cartella `Tests/` del proprio bundle.
Seguendo questa regola, si possono eseguire tutti i test della propria applicazione con il seguente
comando:

.. code-block:: bash

    # specifica la cartella di configurazione nella linea di comando
    $ phpunit -c app/

L'opzione ``-c`` dice a PHPUnit di cercare nella cartella ``app/`` un file di configurazione.
Chi fosse curioso di conoscere le opzioni di PHPUnit, può dare uno sguardo al file
``app/phpunit.xml.dist``.

.. tip::

    Si può generare la copertura del codice, con l'opzione ``--coverage-html``.

.. index::
   single: Test; Test unitari

Test unitari
------------

Un test unitario è solitamente un test di una specifica classe PHP. Se si vuole
testare il comportamento generale della propria applicazione, vedere la sezione dei `Test funzionali`_.

La scrittura di test unitari in Symfony2 non è diversa dalla scrittura standard di test
unitari in PHPUnit. Si supponga, per esempio, di avere una classe *incredibilmente* semplice,
chiamata ``Calculator``, nella cartella ``Utility/`` del proprio bundle::

    // src/Acme/DemoBundle/Utility/Calculator.php
    namespace Acme\DemoBundle\Utility;
    
    class Calculator
    {
        public function add($a, $b)
        {
            return $a + $b;
        }
    }

Per testarla, creare un file ``CalculatorTest`` nella cartella ``Tests/Utility`` del
proprio bundle::

    // src/Acme/DemoBundle/Tests/Utility/CalculatorTest.php
    namespace Acme\DemoBundle\Tests\Utility;

    use Acme\DemoBundle\Utility\Calculator;

    class CalculatorTest extends \PHPUnit_Framework_TestCase
    {
        public function testAdd()
        {
            $calc = new Calculator();
            $result = $calc->add(30, 12);

            // asserisce che il calcolatore aggiunga correttamente i numeri!
            $this->assertEquals(42, $result);
        }
    }

.. note::

    Per convenzione, si raccomanda di replicare la struttura di cartella
    di un bundle nella sua sotto-cartella ``Tests/``. Quindi, se si testa una classe nella
    cartella ``Utility/`` del proprio bundle, mettere il test nella cartella ``Tests/Utility/``.

Proprio come per l'applicazione reale, l'autoloading è abilitato automaticamente tramite il file
``bootstrap.php.cache`` (come configurato in modo predefinito nel file
``phpunit.xml.dist``).

Anche eseguire i test per un dato file o una data cartella è molto facile:

.. code-block:: bash

    # eseguire tutti i test nella cartella Utility
    $ phpunit -c app src/Acme/DemoBundle/Tests/Utility/

    # eseguire i  test per la classe Calculator
    $ phpunit -c app src/Acme/DemoBundle/Tests/Utility/CalculatorTest.php

    # eseguire tutti i test per l'intero bundle
    $ phpunit -c app src/Acme/DemoBundle/

.. index::
   single: Test; Test funzionali

Test funzionali
---------------

I test funzionali verificano l'integrazione dei diversi livelli di un'applicazione
(dalle rotte alle viste). Non differiscono dai test unitari per quello che riguarda
PHPUnit, ma hanno un flusso di lavoro molto specifico:

* Fare una richiesta;
* Testare la risposta;
* Cliccare su un collegamento o inviare un form;
* Testare la risposta;
* Ripetere.

Un primo test funzionale
~~~~~~~~~~~~~~~~~~~~~~~~

I test funzionali sono semplici file PHP, che tipicamente risiedono nella cartella ``Tests/Controller``
del proprio bundle. Se si vogliono testare le pagine gestite dalla propria classe
``DemoController``, si inizi creando un file ``DemoControllerTest.php``, che estende
una classe speciale ``WebTestCase``.

Per esempio, l'edizione standard di Symfony2 fornisce un semplice test funzionale per il
suo ``DemoController`` (`DemoControllerTest`_), fatto in questo modo::

    // src/Acme/DemoBundle/Tests/Controller/DemoControllerTest.php
    namespace Acme\DemoBundle\Tests\Controller;

    use Symfony\Bundle\FrameworkBundle\Test\WebTestCase;

    class DemoControllerTest extends WebTestCase
    {
        public function testIndex()
        {
            $client = static::createClient();

            $crawler = $client->request('GET', '/demo/hello/Fabien');

            $this->assertTrue($crawler->filter('html:contains("Hello Fabien")')->count() > 0);
        }
    }

.. tip::

    Per eseguire i test funzionali, la classe ``WebTestCase`` inizializza il
    kernel dell'applicazione. Nella maggior parte dei casi, questo avviene in modo automatico.
    Tuttavia, se il proprio kernel si trova in una cartella non standard, occorre modificare
    il file ``phpunit.xml.dist`` e impostare nella variabile d'ambiente ``KERNEL_DIR`` la
    cartella del proprio kernel::

        <phpunit
            <!-- ... -->
            <php>
                <server name="KERNEL_DIR" value="/percorso/della/propria/applicazione/" />
            </php>
            <!-- ... -->
        </phpunit>

Il metodo ``createClient()`` restituisce un client, che è come un browser da usare per
visitare il proprio sito::

    $crawler = $client->request('GET', '/demo/hello/Fabien');

Il metodo ``request()`` (vedere :ref:`di più sul metodo della richiesta<book-testing-request-method-sidebar>`) restituisce un oggetto :class:`Symfony\\Component\\DomCrawler\\Crawler`,
che può essere usato per selezionare elementi nella risposta, per cliccare su
collegamenti e per inviare form.

.. tip::

    Il crawler può essere usato solo se il contenuto della risposta è un documento XML
    o HTML. Per altri tipi di contenuto, richiamare ``$client->getResponse()->getContent()``.

Cliccare su un collegamento, seleziondolo prima con il  Crawler, usando o un'espressione XPath
o un selettore CSS, quindi usando il Client per cliccarlo. Per esempio, il codice seguente
trova tutti i collegamenti con il testo ``Greet``, quindi sceglie il secondo e infine
lo clicca::

    $link = $crawler->filter('a:contains("Greet")')->eq(1)->link();

    $crawler = $client->click($link);

Inviare un form è molto simile: selezionare il bottone di un form, eventualmente
sovrascrivere alcuni valori del form e inviare il form corrispondente::

    $form = $crawler->selectButton('submit')->form();

    // impostare alcuni valori
    $form['name'] = 'Lucas';
    $form['form_name[subject]'] = 'Bella per te!';

    // inviare il form
    $crawler = $client->submit($form);

.. tip::

    Il form può anche gestire caricamenti di file e contiene metodi utili
    per riempire diversi tipi di campi (p.e. ``select()`` e ``tick()``).
    Per maggiori dettagli, vedere la sezione `Form`_ più avanti.

Ora che si è in grado di navigare facilmente nell'applicazione, usare le asserzioni
per testare che faccia effettivamente quello che ci si aspetta. Usare il Crawler
per fare asserzioni sul DOM::

    // Asserisce che la risposta corrisponda a un dato selettore CSS.
    $this->assertTrue($crawler->filter('h1')->count() > 0);

Oppure, testare direttamente il contenuto della risposta, se si vuole solo asserire che
il contenuto debba contenere del testo o se la risposta non è un documento
XML/HTML::

    $this->assertRegExp('/Hello Fabien/', $client->getResponse()->getContent());

.. _book-testing-request-method-sidebar:

.. sidebar:: Di più sul metodo ``request()``:

    La firma completa del metodo ``request()`` è::

        request(
            $method,
            $uri, 
            array $parameters = array(), 
            array $files = array(), 
            array $server = array(), 
            $content = null, 
            $changeHistory = true
        )

    L'array ``server`` contiene i valori grezzi che ci si aspetta di trovare normalmente
    nell'array superglobale `$_SERVER`_ di PHP. Per esempio, per impostare gli header HTTP
    `Content-Type` e `Referer`, passare i seguenti::

        $client->request(
            'GET',
            '/demo/hello/Fabien',
            array(),
            array(),
            array(
                'CONTENT_TYPE' => 'application/json',
                'HTTP_REFERER' => '/foo/bar',
            )
        );

.. index::
   single: Test; Asserzioni

.. sidebar: Useful Assertions

    Per iniziare più rapidamente, ecco una lista delle asserzioni
    più utili e comuni::

        // Asserire che la risposta corrisponda al selettore CSS dato.
        $this->assertTrue($crawler->filter('h2.subtitle')->count() > 0);

        // Assert that there are exactly 4 h2 tags on the page
        $this->assertEquals(4, $crawler->filter('h2')->count());

        // Assert the the "Content-Type" header is "application/json"
        $this->assertTrue($client->getResponse()->headers->contains('Content-Type', 'application/json'));

        // Asserire che la risposta corrisponda a un'espressione regolare.
        $this->assertRegExp('/pippo/', $client->getResponse()->getContent());

        // Asserire che il codice di stato della risposta sia 2xx
        $this->assertTrue($client->getResponse()->isSuccessful());
        // Asserire che il codice di stato della risposta sia 404
        $this->assertTrue($client->getResponse()->isNotFound());
        // Asserire uno specifico codice di stato 200
        $this->assertEquals(200, $client->getResponse()->getStatusCode());

        // Asserire che il codice di stato della risposta sia un rinvio a /demo/contact
        $this->assertTrue($client->getResponse()->isRedirect('/demo/contact'));
        // o verificare semplicemente che la risposta sia un rinvio
        $this->assertTrue($client->getResponse()->isRedirect());

.. index::
   single: Test; Client

Lavorare con il client dei test
-------------------------------

Il client dei test emula un client HTTP, come un browser, ed effettua richieste
all'applicazione Symfony2::

    $crawler = $client->request('GET', '/hello/Fabien');

Il metodo ``request()`` accetta come parametri il metodo HTTP e un URL e
restituisce un'istanza di ``Crawler``.

Usare il crawler per cercare elementi del DOM nella risposta. Questi elementi possono
poi essere usati per cliccare su collegamenti e inviare form::

    $link = $crawler->selectLink('Vai da qualche parte...')->link();
    $crawler = $client->click($link);

    $form = $crawler->selectButton('validare')->form();
    $crawler = $client->submit($form, array('name' => 'Fabien'));

I metodi ``click()`` e ``submit()`` restituiscono entrambi un oggetto ``Crawler``.
Questi metodi sono il modo migliore per navigare un'applicazione, perché si occupano di
diversi dettagli, come il metodo HTTP di un form e il fornire un'utile API per caricare
file.

.. tip::

    Gli oggetti ``Link`` e ``Form`` nel crawler saranno approfonditi nella
    sezione :ref:`Crawler<book-testing-crawler>`, più avanti.

Il metodo ``request()`` può anche essere usato per simulare direttamente l'invio di form
o per eseguire richieste più complesse::

    // Invio diretto di form
    $client->request('POST', '/submit', array('name' => 'Fabien'));

    // Invio di form di con caricamento di file
    use Symfony\Component\HttpFoundation\File\UploadedFile;

    $photo = new UploadedFile(
        '/path/to/photo.jpg',
        'photo.jpg',
        'image/jpeg',
        123
    );
    // oppure
    $photo = array(
        'tmp_name' => '/path/to/photo.jpg',
        'name' => 'photo.jpg',
        'type' => 'image/jpeg',
        'size' => 123,
        'error' => UPLOAD_ERR_OK
    );
    $client->request(
        'POST',
        '/submit',
        array('name' => 'Fabien'),
        array('photo' => $photo)
    );

    // Eseguire richieste DELETE e passare header HTTP
    $client->request(
        'DELETE',
        '/post/12',
        array(),
        array(),
        array('PHP_AUTH_USER' => 'username', 'PHP_AUTH_PW' => 'pa$$word')
    );

Infine, ma non meno importante, si può forzare l'esecuzione di ogni richiesta
nel suo processo PHP, per evitare effetti collaterali quando si lavora con molti
client nello stess script::

    $client->insulate();

Browser
~~~~~~~

Il client supporta molte operazioni eseguibili in un browser reale::

    $client->back();
    $client->forward();
    $client->reload();

    // Pulisce tutti i cookie e la cronologia
    $client->restart();

Accesso agli oggetti interni
~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Se si usa il client per testare la propria applicazione, si potrebbe voler accedere
agli oggetti interni del client::

    $history   = $client->getHistory();
    $cookieJar = $client->getCookieJar();

I possono anche ottenere gli oggetti relativi all'ultima richiesta::

    $request  = $client->getRequest();
    $response = $client->getResponse();
    $crawler  = $client->getCrawler();

Se le richieste non sono isolate, si può accedere agli oggetti ``Container`` e
``Kernel``::

    $container = $client->getContainer();
    $kernel    = $client->getKernel();

Accesso al contenitore
~~~~~~~~~~~~~~~~~~~~~~

È caldamente raccomandato che un test funzionale testi solo la risposta. Ma
sotto alcune rare circostanze, si potrebbe voler accedere ad alcuni oggetti
interni, per scrivere asserzioni. In questi casi, si può accedere al dependency
injection container::

    $container = $client->getContainer();

Attenzione, perché questo non funziona se si isola il client o se si usa un
livello HTTP. Per un elenco di servizi disponibili nell'applicazione, usare
il comando ``container:debug``.

.. tip::

    Se l'informazione che occorre verificare è disponibile nel profilatore, si usi
    invece quest'ultimo.

Accedere ai dati del profilatore
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

A ogni richiesta, il profilatore di Symfony raccoglie e memorizza molti dati, che
riguardano la gestione interna della richiesta stessa. Per esempio, il profilatore
può essere usato per verificare che una data pagina esegua meno di un certo numero
di query alla base dati.

Si può ottenere il profilatore dell'ultima richiesta in questo modo::

    $profile = $client->getProfile();

Per dettagli specifici sull'uso del profilatore in un test, vedere la ricetta
:doc:`/cookbook/testing/profiling`.

Rinvii
~~~~~~

Quando una richiesta restituisce una risposta di rinvio, il client la segue automaticamente.
Se si vuole esaminare la rispostsa prima del rinvio, si può forzare il client a non
seguire i rinvii, usando il metodo ``followRedirect()``::

    $crawler = $client->followRedirect(false);

Quando il client non segue i rinvvi, lo si può forzare con
il metodo ``followRedirect()``::

    $crawler = $client->followRedirect();

.. index::
   single: Test; Crawler

.. _book-testing-crawler:

Il crawler
~~~~~~~~~~

Un'istanza del crawler è creata automaticamente quando si esegue una richiesta con un
client. Consente di attraversare i documenti HTML, selezionare nodi, trovare collegamenti e form.

Attraversamento
~~~~~~~~~~~~~~~

Come jQuery, il crawler dispone di metodi per attraversare il DOM di documenti HTML/XML.
Per esempio, per estrarre tutti gli elementi ``input[type=submit]``,
trovarne l'ultimo e quindi selezionare il suo genitore::

    $newCrawler = $crawler->filter('input[type=submit]')
        ->last()
        ->parents()
        ->first()
    ;

Ci sono molti altri metodi a disposizione:

+------------------------+----------------------------------------------------+
| Metodo                 | Descrizione                                        |
+========================+====================================================+
| ``filter('h1.title')`` | Nodi corrispondenti al selettore CSS               |
+------------------------+----------------------------------------------------+
| ``filterXpath('h1')``  | Nodi corrispondenti all'espressione XPath          |
+------------------------+----------------------------------------------------+
| ``eq(1)``              | Nodi per l'indice specificato                      |
+------------------------+----------------------------------------------------+
| ``first()``            | Primo nodo                                         |
+------------------------+----------------------------------------------------+
| ``last()``             | Ultimo nodo                                        |
+------------------------+----------------------------------------------------+
| ``siblings()``         | Fratelli                                           |
+------------------------+----------------------------------------------------+
| ``nextAll()``          | Tutti i fratelli successivi                        |
+------------------------+----------------------------------------------------+
| ``previousAll()``      | Tutti i fratelli precedenti                        |
+------------------------+----------------------------------------------------+
| ``parents()``          | Genitori                                           |
+------------------------+----------------------------------------------------+
| ``children()``         | Figli                                              |
+------------------------+----------------------------------------------------+
| ``reduce($lambda)``    | Nodi per cui la funzione non restituisce false     |
+------------------------+----------------------------------------------------+

Si può iterativamente restringere la selezione del nodo, concatenando le chiamate ai
metodi, perché ogni metodo restituisce una nuova istanza di Crawler per i nodi corrispondenti::

    $crawler
        ->filter('h1')
        ->reduce(function ($node, $i)
        {
            if (!$node->getAttribute('class')) {
                return false;
            }
        })
        ->first();

.. tip::

    Usare la funzione ``count()`` per ottenere il numero di nodi memorizzati in un
    crawler: ``count($crawler)``

Estrarre informazioni
~~~~~~~~~~~~~~~~~~~~~

Il crawler può estrarre informazioni dai nodi::

    // Restituisce il valore dell'attributo del primo nodo
    $crawler->attr('class');

    // Restituisce il valore del nodo del primo nodo
    $crawler->text();

    // Estrae un array di attributi per tutti i nodi (_text restituisce il valore del nodo)
    // restituisce un array per ogni elemento nel crawler, ciascuno con valore e href
    $info = $crawler->extract(array('_text', 'href'));

    // Esegue una funzione lambda per ogni nodo e restituisce un array di risultati
    $data = $crawler->each(function ($node, $i)
    {
        return $node->attr('href');
    });

Collegamenti
~~~~~~~~~~~~

Si possono selezionare collegamenti coi metodi di attraversamento, ma la scorciatoia
``selectLink()`` è spesso più conveniente::

    $crawler->selectLink('Clicca qui');

Seleziona i collegamenti che contengono il testo dato, oppure le immagini cliccabili per
cui l'attributi ``alt`` contiene il testo dato. Come gli altri metodi filtro, restituisce
un altro oggetto ``Crawler``.

Una volta selezionato un collegamento, si ha accesso a uno speciale oggetto ``Link``, che
ha utili metodi specifici per i collegamenti (come ``getMethod()`` e
``getUri()``). Per cliccare sul collegamento, usare il metodo ``click()`` di Client e
passargli un oggetto ``Link``::

    $link = $crawler->selectLink('Click here')->link();

    $client->click($link);

Form
~~~~

Come per i collegamenti, si possono selezionare i form col metodo ``selectButton()``::

    $buttonCrawlerNode = $crawler->selectButton('submit');

.. note::

    Si noti che si selezionano i bottoni dei form e non i form stessi, perché un form può avere
    più bottoni; se si usa l'API di attraversamento, si tenga a mente che si deve cercare
    un bottone.

Il metodo ``selectButton()`` può selezionare i tag ``button`` e i tag ``input`` con attributo
"submit". Ha diverse euristiche per trovarli:

* Il valore dell'attributo ``value``;

* Il valore dell'attributo ``id`` o ``alt`` per le immagini;

* Il valore dell'attributo ``id`` o ``name`` per i tag ``button``.

Quando si ha un nodo che rappresenta un bottone, richiamare il metodo ``form()`` per
ottenere un'istanza ``Form`` per il form, che contiene il nodo bottone.

    $form = $buttonCrawlerNode->form();

Quando si richiama il metodo ``form()``, si può anche passare un array di valori di
campi, che sovrascrivano quelli predefiniti::

    $form = $buttonCrawlerNode->form(array(
        'name'              => 'Fabien',
        'my_form[subject]'  => 'Symfony spacca!',
    ));

Se si vuole emulare uno specifico metodo HTTP per il form, passarlo come secondo
parametro::

    $form = $buttonCrawlerNode->form(array(), 'DELETE');

Il client puoi inviare istanze di ``Form``::

    $client->submit($form);

Si possono anche passare i valori dei campi come secondo parametro del
metodo ``submit()``::

    $client->submit($form, array(
        'name'              => 'Fabien',
        'my_form[subject]'  => 'Symfony spacca!',
    ));

Per situazioni più complesse, usare l'istanza di ``Form`` come un array, per
impostare ogni valore di campo individualmente::

    // Cambiare il valore di un campo
    $form['name'] = 'Fabien';
    $form['my_form[subject]'] = 'Symfony spacca!';

C'è anche un'utile API per manipolare i valori dei campi, a seconda del
tipo::

    // Selezionare un'opzione o un radio
    $form['country']->select('France');

    // Spuntare un checkbox
    $form['like_symfony']->tick();

    // Caricare un file
    $form['photo']->upload('/path/to/lucas.jpg');

.. tip::

    Si possono ottenere i valori che saranno inviati, richiamando il metodo
    ``getValues()``. I file caricati sono disponibili in un array separato, restituito dal
    metodo ``getFiles()``. Anche i metodi ``getPhpValues()`` e ``getPhpFiles()`` restituiscono
    i valori inviati, ma nel formato di PHP (convertendo le chiavi con parentesi quadre,
    p.e. ``my_form[subject]`` da, nella notazione degli array di
    PHP).

.. index::
   pair: Test; Configurazione

Configurazione dei test
-----------------------

Il client usato dai test funzionali crea un kernel che gira in uno speciale
ambiente ``test``. Siccome Symfony carica ``app/config/config_test.yml``
in ambiente ``test``, si possono modificare le impostazioni della propria
applicazione specificatamente per i test.

Per esempio, swiftmailer è configurato in modo predefinito per *non* inviare le email
in ambiente ``test``. Lo si può vedere sotto l'opzione di configurazione
``swiftmailer``:

.. configuration-block::

    .. code-block:: yaml

        # app/config/config_test.yml
        # ...

        swiftmailer:
            disable_delivery: true

    .. code-block:: xml

        <!-- app/config/config_test.xml -->
        <container>
            <!-- ... -->

            <swiftmailer:config disable-delivery="true" />
        </container>

    .. code-block:: php

        // app/config/config_test.php
        // ...

        $container->loadFromExtension('swiftmailer', array(
            'disable_delivery' => true
        ));

Si può anche cambiare l'ambiente predefinito (``test``) e sovrascrivere la modalità
predefinita di debug (``true``) passandoli come opzioni al metodo
``createClient()``::

    $client = static::createClient(array(
        'environment' => 'my_test_env',
        'debug'       => false,
    ));

Se la propria applicazione necessita di alcuni header HTTP, passarli come secondo
parametro di ``createClient()``::

    $client = static::createClient(array(), array(
        'HTTP_HOST'       => 'en.example.com',
        'HTTP_USER_AGENT' => 'MySuperBrowser/1.0',
    ));

Si possono anche sovrascrivere gli header HTTP a ogni richiesta::

    $client->request('GET', '/', array(), array(), array(
        'HTTP_HOST'       => 'en.example.com',
        'HTTP_USER_AGENT' => 'MySuperBrowser/1.0',
    ));

.. tip::

    Il client dei test è disponibile come servizio nel contenitore, in ambiente
    ``test`` (o dovunque sia abilitata l'opzione :ref:`framework.test<reference-framework-test>`).
    Questo vuol dire che si può ridefinire completamente il servizio, qualora se ne
    avesse la necessità.

.. index::
   pair: PHPUnit; Configurazione

Configurazione di PHPUnit
~~~~~~~~~~~~~~~~~~~~~~~~~

Ogni applicazione ha la sua configurazione di PHPUnit, memorizzata nel file
``phpunit.xml.dist``. Si può modificare tale file per cambiare i default, oppure creare
un file ``phpunit.xml`` per aggiustare la configurazione per la propria macchina locale.

.. tip::

    Inserire il file ``phpunit.xml.dist`` nel proprio repository e ignorare il
    file ``phpunit.xml``.

Per impostazione predefinita, solo i test memorizzati nei bundle "standard" sono eseguiti
dal comando ``phpunit`` (per "standard" si intendono i test nelle cartelle
``src/*/Bundle/Tests`` o ``src/*/Bundle/*Bundle/Tests``). Ma si possono facilmente aggiungere altri spazi dei nomi. Per esempio,
la configurazione seguente aggiunge i test per i bundle installati di terze
parti:

.. code-block:: xml

    <!-- hello/phpunit.xml.dist -->
    <testsuites>
        <testsuite name="Project Test Suite">
            <directory>../src/*/*Bundle/Tests</directory>
            <directory>../src/Acme/Bundle/*Bundle/Tests</directory>
        </testsuite>
    </testsuites>

Per includere altre cartelle nella copertura del codice, modificare anche la
sezione ``<filter>``:

.. code-block:: xml

    <filter>
        <whitelist>
            <directory>../src</directory>
            <exclude>
                <directory>../src/*/*Bundle/Resources</directory>
                <directory>../src/*/*Bundle/Tests</directory>
                <directory>../src/Acme/Bundle/*Bundle/Resources</directory>
                <directory>../src/Acme/Bundle/*Bundle/Tests</directory>
            </exclude>
        </whitelist>
    </filter>

Imparare di più con le ricette
------------------------------

* :doc:`/cookbook/testing/http_authentication`
* :doc:`/cookbook/testing/insulating_clients`
* :doc:`/cookbook/testing/profiling`


.. _`DemoControllerTest`: https://github.com/symfony/symfony-standard/blob/master/src/Acme/DemoBundle/Tests/Controller/DemoControllerTest.php
.. _`$_SERVER`: http://php.net/manual/en/reserved.variables.server.php
.. _documentazione: http://www.phpunit.de/manual/3.5/en/
