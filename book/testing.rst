.. index::
   single: Test

Test
====

Ogni volta che si scrive una nuova riga di codice, si aggiungono potenzialmente nuovi
bug. I test automatici dovrebbero dare una copertura e questa guida mostra come scrivere
test unitari e funzionali per la propria applicazione Symfony2.

Framework dei test
------------------

I test di Symfony2 di appoggiano molto a PHPUnit, le sue best practice e ad alcune
convenzioni. Questa parte non documenta PHPUnit stesso, ma se non lo si conosce, si
può leggere la sua eccellente `documentazione`_.

.. note::

    Symfony2 funziona con PHPUnit 3.5.11 o successivi.

La configurazione predefinita di PHPUnit cerca i test sotto la cartella ``Tests/``
dei propri bundle:

.. code-block:: xml

    <!-- app/phpunit.xml.dist -->

    <phpunit bootstrap="../src/autoload.php">
        <testsuites>
            <testsuite name="Project Test Suite">
                <directory>../src/*/*Bundle/Tests</directory>
            </testsuite>
        </testsuites>

        ...
    </phpunit>

Eseguire tutti i test per una data applicazione è semplice:

.. code-block:: bash

    # specificare la cartella della configurazione nella linea di comando
    $ phpunit -c app/

    # oppure eseguire phpunit da dentro la cartella dell'applicazione
    $ cd app/
    $ phpunit

.. tip::

    La copertura del codice può essere generata con l'opzione ``--coverage-html``.

.. index::
   single: Test; Test unitari

Test unitari
------------

La scrittura di test unitari in Symfony2 non è diversa dalla scrittura standard di test
unitari in PHPUnit. Per convenzione, si raccomanda di replicare la struttura di cartella
di un bundle nella sua sotto-cartella ``Tests/``. Scriviamo dunque i test per la classe
``Acme\HelloBundle\Model\Article`` nel file
``Acme/HelloBundle/Tests/Model/ArticleTest.php``.

In un test unitario, l'autoloading è abilitato automaticamente tramite il file
``src/autoload.php`` (come configurato in modo predefinito nel file
``phpunit.xml.dist``).

Anche eseguire i test per un dato file o una data cartella è molto facile:

.. code-block:: bash

    # eseguire tutti i test per il controllore
    $ phpunit -c app src/Acme/HelloBundle/Tests/Controller/

    # eseguire tutti i test per il modello
    $ phpunit -c app src/Acme/HelloBundle/Tests/Model/

    # eseguire i test per la classe Article
    $ phpunit -c app src/Acme/HelloBundle/Tests/Model/ArticleTest.php

    # eseguire tutti i test per l'intero bundle
    $ phpunit -c app src/Acme/HelloBundle/

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

Richieste, click e invii di form sono eseguiti da un client che sa come parlare 
all'applicazione. Per accedere a tale client, i test hanno bisogno di estendere
la classe ``WebTestCase`` di Symfony2 . Symfony2 Standard Edition fornisce un semplice
test funzionale per ``DemoController``, fatto in questo modo::

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

Il metodo ``createClient()`` restituisce un client legato all'applicazione corrente::

    $crawler = $client->request('GET', '/demo/hello/Fabien');

Il metodo ``request()`` restituisce un oggetto ``Crawler``, che può essere usato per
selezionare elementi nella risposta, per cliccare su collegamenti e per inviare form.

.. tip::

    Il crawler può essere usato solo se il contenuto della risposta è un documento XML
    o HTML. Per altri tipi di contenuto, prendere il contenuto della risposta con
    ``$client->getResponse()->getContent()``.

    Si può impostare il content-type della richiesta a JSON aggiungendo 'HTTP_CONTENT_TYPE' => 'application/json'.

.. tip::

    La firma completa del metodo ``request()`` è::

        request($method,
            $uri, 
            array $parameters = array(), 
            array $files = array(), 
            array $server = array(), 
            $content = null, 
            $changeHistory = true
        )   

Cliccare su un collegamento, selezionandolo prima col crawler, usando un'espressione
XPath o un selettore CSS, quindi usar il Cliente per cliccarvi sopra::

    $link = $crawler->filter('a:contains("Greet")')->eq(1)->link();

    $crawler = $client->click($link);

Inviare un form è molto simile: selezionare il bottone di un form, eventualmente
sovrascrivere alcuni valori del form e inviare il form corrispondente::

    $form = $crawler->selectButton('submit')->form();

    // impostare alcuni valori
    $form['name'] = 'Lucas';

    // inviare il form
    $crawler = $client->submit($form);

Ogni campo ``Form`` ha metodi specializzati, a seconda del suo tipo::

    // riempire un campo di testo
    $form['name'] = 'Lucas';

    // scegliere un'opzione o un radio
    $form['country']->select('France');

    // spuntare un checkbox
    $form['like_symfony']->tick();

    // caricare un file
    $form['photo']->upload('/path/to/lucas.jpg');

Invece di cambiare un campo alla volta, si può passare un array di valori al
metodo ``submit()``::

    $crawler = $client->submit($form, array(
        'name'         => 'Lucas',
        'country'      => 'France',
        'like_symfony' => true,
        'photo'        => '/path/to/lucas.jpg',
    ));

Ora che si può facilmente navigare nell'applicazione, usare asserzioni per testare
che essa faccia veramente quello che ci si aspetta. Usare il crawler per fare asserzioni
sul DOM::

    // Asserire che la risposta corrisponda al selettore CSS dato.
    $this->assertTrue($crawler->filter('h1')->count() > 0);

Oppure, testare direttamente il contenuto delle risposta, se si vuole solamente asserire
che il contenuto contenga del testo, oppure se la risposta non è un documento
XML/HTML::

    $this->assertRegExp('/Hello Fabien/', $client->getResponse()->getContent());

.. index::
   single: Test; Asserzioni

Asserzioni utili
~~~~~~~~~~~~~~~~

Dopo qualche tempo, ci si renderà conto di scrivere sempre lo stesso tipo di
asserzioni. Per iniziare più rapidamente, ecco una lista delle asserzioni
più utili e comuni::

    // Asserire che la risposta corrisponda al selettore CSS dato.
    $this->assertTrue($crawler->filter($selector)->count() > 0);

    // Asserire che la risposta corrisponda n volte al selettore CSS dato.
    $this->assertEquals($count, $crawler->filter($selector)->count());

    // Asserire che un header della risposta abbia il valore dato.
    $this->assertTrue($client->getResponse()->headers->contains($key, $value));

    // Asserire che la risposta corrisponda a un'espressione regolare.
    $this->assertRegExp($regexp, $client->getResponse()->getContent());

    // Asserire il codice di stato della risposta.
    $this->assertTrue($client->getResponse()->isSuccessful());
    $this->assertTrue($client->getResponse()->isNotFound());
    $this->assertEquals(200, $client->getResponse()->getStatusCode());

    // Asserire che il codice di stato della risposta sia un rinvio.
    $this->assertTrue($client->getResponse()->isRedirect('google.com'));

.. _documentazione: http://www.phpunit.de/manual/3.5/en/

.. index::
   single: Test; Client

Il client dei test
------------------

Il client dei test emula un client HTTP, come un browser.

.. note::

    Il client dei test è basato sui componenti ``BrowserKit`` e
    ``Crawler``.

Effettuare richieste
~~~~~~~~~~~~~~~~~~~~

Il client sa come fare richieste a un'applicazione Symfony2::

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
Questi metodi sono il modo migliore per navigare un'applicazione, perché nascondono
diversi dettagli. Per esempio, quando si invia un form, individuano automaticamente il
metodo HTTP e l'URL del form, forniscono un'utile API per caricare file, fondono
i valori inviati con quelli predefiniti del form, e altro ancora.

.. tip::

    Gli oggetti ``Link`` e ``Form`` nel crawler saranno approfonditi nella
    sezione successiva.

Ma si possono anche emulare invii di form e richieste complesse con parametri
aggiuntivi del metodo ``request()``::

    // Invio di form
    $client->request('POST', '/submit', array('name' => 'Fabien'));

    // Invio di form di con caricamento di file
    use Symfony\Component\HttpFoundation\File\UploadedFile;

    $photo = new UploadedFile('/path/to/photo.jpg', 'photo.jpg', 'image/jpeg', 123);
    // oppure
    $photo = array('tmp_name' => '/path/to/photo.jpg', 'name' => 'photo.jpg', 'type' => 'image/jpeg', 'size' => 123, 'error' => UPLOAD_ERR_OK);

    $client->request('POST', '/submit', array('name' => 'Fabien'), array('photo' => $photo));

    // Specificare header HTTP
    $client->request('DELETE', '/post/12', array(), array(), array('PHP_AUTH_USER' => 'username', 'PHP_AUTH_PW' => 'pa$$word'));

.. tip::

    Gli invii di form possono essere molto semplificati, usando un oggetto crawler (vedere sotto).

Quando una richiesta restituisce una risposta di rinvio, il client la segue
automaticamente. Questo comportamento può essere modificato col metodo ``followRedirects()``::

    $client->followRedirects(false);

Quando il client non segue i rinvii, si può forzare il rinvio con il metodo
``followRedirect()``::

    $crawler = $client->followRedirect();

Da ultimo, ma non meno importante, si può forzare ogni righiesta a essere eseguita
in un suo processo PHP, per evitare effetti collaterali quando si lavora con molti client
nello stesso script::

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

Accesso al container
~~~~~~~~~~~~~~~~~~~~

È caldamente raccomandato che un test funzionale testi solo la risposta. Ma
sotto alcune rare circostanze, si potrebbe voler accedere ad alcuni oggetti
interni, per scrivere asserzioni. In questi casi, si può accedere al dependency
injection container::

    $container = $client->getContainer();

Attenzione, perché questo non funziona se si isola il client o se si usa un
livello HTTP.

.. tip::

    Se l'informazione che occorre verificare è disponibile nel profiler, si usi
    invece quest'ultimo.

Accedere ai dati del profiler
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Per fare asserzioni su dati raccolti dal profiler, si può prendere il profiler per
la richiesta corrente, in questo modo::

    $profile = $client->getProfile();

Rinvii
~~~~~~

Per impostazione predefinita, il client non segue i rinvii HTTP, quindi si può
prendere la risposta ed esaminarla prima del rinvio stesso. Quando si vuole che
il rinvio sia seguito, usare il metodo ``followRedirect()``::

    // fare qualcosa che causi un rinvio (p.e. riempire un form)

    // seguire il rinvio
    $crawler = $client->followRedirect();

Se si vuole che il client segua automaticamente i rinvii, si può richiamare
il metodo ``followRedirects()``::

    $client->followRedirects();

    $crawler = $client->request('GET', '/');

    // tutti i rinvii sono seguiti

    // imposta di nuovo il client al rinvio manuale
    $client->followRedirects(false);

.. index::
   single: Test; Crawler

Il crawler
----------

Ogni volta che si fa una richiesta col client, viene restituita un'istanza di crawler.
Questo consente di attraversare documenti HTML, selezionare nodi, trovare collegamenti e form..

Creare un'istanza del crawler
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Un'istanza del crawler è creata automaticamente quando si esegue una richiesta con un
client. Ma la si può creare facilmente a mano::

    use Symfony\Component\DomCrawler\Crawler;

    $crawler = new Crawler($html, $url);

Il costruttore accetta due parametri: il secondo è un URL, usato per generare URL
assoluti per collegamenti e form, il primo può essere uno dei
seguenti:

* Un documento HTML;
* Un documento XML;
* Un'istanza di ``DOMDocument``;
* Un'istanza di ``DOMNodeList``;
* Un'istanza di ``DOMNode``;
* Un array degli elementi precedenti.

Dopo la creazione, si possono aggiungere altri nodi:

+-----------------------+-----------------------------------------+
| Metodo                | Descrizione                             |
+=======================+=========================================+
| ``addHTMLDocument()`` | Un documento HTML                       |
+-----------------------+-----------------------------------------+
| ``addXMLDocument()``  | Un documento XML                        |
+-----------------------+-----------------------------------------+
| ``addDOMDocument()``  | Un'istanza di ``DOMDocument``           |
+-----------------------+-----------------------------------------+
| ``addDOMNodeList()``  | Un'istanza di ``DOMNodeList``           |
+-----------------------+-----------------------------------------+
| ``addDOMNode()``      | Un'istanza di ``DOMNode``               |
+-----------------------+-----------------------------------------+
| ``addNodes()``        | Un array degli elementi precedenti      |
+-----------------------+-----------------------------------------+
| ``add()``             | Uno qualsiasi degli elementi precedenti |
+-----------------------+-----------------------------------------+

Attraversamento
~~~~~~~~~~~~~~~

Come jQuery, il crawler ha dei metodi per attraversare il DOM di un documento
HTML/XML:

+-----------------------+----------------------------------------------------+
| Metodo                | Descrizione                                        |
+=======================+====================================================+
| ``filter('h1')``      | Nodi corrispondenti al selettore CSS               |
+-----------------------+----------------------------------------------------+
| ``filterXpath('h1')`` | Nodi corrispondenti all'espressione XPath          |
+-----------------------+----------------------------------------------------+
| ``eq(1)``             | Nodi per l'indice specificato                      |
+-----------------------+----------------------------------------------------+
| ``first()``           | Primo nodo                                         |
+-----------------------+----------------------------------------------------+
| ``last()``            | Ultimo nodo                                        |
+-----------------------+----------------------------------------------------+
| ``siblings()``        | Fratelli                                           |
+-----------------------+----------------------------------------------------+
| ``nextAll()``         | Tutti i fratelli successivi                        |
+-----------------------+----------------------------------------------------+
| ``previousAll()``     | Tutti i fratelli precedenti                        |
+-----------------------+----------------------------------------------------+
| ``parents()``         | Genitori                                           |
+-----------------------+----------------------------------------------------+
| ``children()``        | Figli                                              |
+-----------------------+----------------------------------------------------+
| ``reduce($lambda)``   | Nodi per cui la funzione non restituisce false     |
+-----------------------+----------------------------------------------------+

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
    $crawler->extract(array('_text', 'href'));

    // Esegue una funzione lambda per ogni nodo e restituisce un array di risultati
    $data = $crawler->each(function ($node, $i)
    {
        return $node->getAttribute('href');
    });

Collegamenti
~~~~~~~~~~~~

Si possono selezionare collegamenti coi metodi di attraversamento, ma la scorciatoia
``selectLink()`` è spesso più conveniente::

    $crawler->selectLink('Clicca qui');

Seleziona i collegamenti che contengono il testo dato, oppure le immagini cliccabili per
cui l'attributi ``alt`` contiene il testo dato.

Il metodo ``click()`` del client accetta un'istanza di ``Link``, come quella restituita
dal metodo ``link()``::

    $link = $crawler->link();

    $client->click($link);

.. tip::

    Il metodo ``links()`` restituisce un array di oggetti ``Link`` per tutti i nodi.

Form
~~~~

Come per i collegamenti, si possono selezionare i form col metodo ``selectButton()``::

    $crawler->selectButton('submit');

Si noti che si selezionano i bottoni dei form e non i form stessi, perché un form può avere
più bottoni; se si usa l'API di attraversamento, si tenga a mente che si deve cercare
un bottone.

Il metodo ``selectButton()`` può selezionare i tag ``button`` e i tag ``input`` con attributo
"submit". Ha diverse euristiche per trovarli:

* Il valore dell'attributo ``value``;

* Il valore dell'attributo ``id`` o ``alt`` per le immagini;

* Il valore dell'attributo ``id`` o ``name`` per i tag ``button``.

Quando si a un nodo che rappresenta un bottone, richiamare il metodo ``form()`` per
ottenere un'istanza ``Form`` per il form, che contiene il nodo bottone.

    $form = $crawler->form();

Quando si richiama il metodo ``form()``, si può anche passare un array di valori di
campi, che sovrascrivano quelli predefiniti::

    $form = $crawler->form(array(
        'name'         => 'Fabien',
        'like_symfony' => true,
    ));

Se si vuole emulare uno specifico metodo HTTP per il form, passarlo come secondo
parametro::

    $form = $crawler->form(array(), 'DELETE');

Il client puoi inviare istanze di ``Form``::

    $client->submit($form);

Si possono anche passare i valori dei campi come secondo parametro del
metodo ``submit()``::

    $client->submit($form, array(
        'name'         => 'Fabien',
        'like_symfony' => true,
    ));

Per situazioni più complesse, usare l'istanza di ``Form`` come un array, per
impostare ogni valore di campo individualmente::

    // Cambiare il valore di un campo
    $form['name'] = 'Fabien';

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
    i valori inviati, ma nel formato di PHP (convertendo le chiavi con parentesi quadre
    nella notazione degli array di PHP).

.. index::
   pair: Test; Configurazione

Configurazione dei test
-----------------------

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
dal comando ``phpunit`` (per "standard" si intendono i test sotto i namespace
Vendor\\*Bundle\\Tests). Ma si possono facilmente aggiungere altri namespace. Per esempio,
la configurazione seguente aggiunge i test per i bundle installati di terze parti:

.. code-block:: xml

    <!-- hello/phpunit.xml.dist -->
    <testsuites>
        <testsuite name="Project Test Suite">
            <directory>../src/*/*Bundle/Tests</directory>
            <directory>../src/Acme/Bundle/*Bundle/Tests</directory>
        </testsuite>
    </testsuites>

Per includere altri namespace nella copertura del codice, modificare anche la
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

Configurazione del client
~~~~~~~~~~~~~~~~~~~~~~~~~

Il client usato dai test funzionali crea un kernel che gira in uno speciale
ambiente ``test``, quindi lo si può aggiustare a volontà:

.. configuration-block::

    .. code-block:: yaml

        # app/config/config_test.yml
        imports:
            - { resource: config_dev.yml }

        framework:
            error_handler: false
            test: ~

        web_profiler:
            toolbar: false
            intercept_redirects: false

        monolog:
            handlers:
                main:
                    type:  stream
                    path:  %kernel.logs_dir%/%kernel.environment%.log
                    level: debug

    .. code-block:: xml

        <!-- app/config/config_test.xml -->
        <container>
            <imports>
                <import resource="config_dev.xml" />
            </imports>

            <webprofiler:config
                toolbar="false"
                intercept-redirects="false"
            />

            <framework:config error_handler="false">
                <framework:test />
            </framework:config>

            <monolog:config>
                <monolog:main
                    type="stream"
                    path="%kernel.logs_dir%/%kernel.environment%.log"
                    level="debug"
                 />               
            </monolog:config>
        </container>

    .. code-block:: php

        // app/config/config_test.php
        $loader->import('config_dev.php');

        $container->loadFromExtension('framework', array(
            'error_handler' => false,
            'test'          => true,
        ));

        $container->loadFromExtension('web_profiler', array(
            'toolbar' => false,
            'intercept-redirects' => false,
        ));

        $container->loadFromExtension('monolog', array(
            'handlers' => array(
                'main' => array('type' => 'stream',
                                'path' => '%kernel.logs_dir%/%kernel.environment%.log'
                                'level' => 'debug')
           
        )));

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

    $client->request('GET', '/', array(), array(
        'HTTP_HOST'       => 'en.example.com',
        'HTTP_USER_AGENT' => 'MySuperBrowser/1.0',
    ));

.. tip::

    Per usare un proprio client personalizzato, sovrascrivere il parametro
    ``test.client.class``, oppure definire un servizio ``test.client``.

Imparare di più con le ricette
------------------------------

* :doc:`/cookbook/testing/http_authentication`
* :doc:`/cookbook/testing/insulating_clients`
* :doc:`/cookbook/testing/profiling`
