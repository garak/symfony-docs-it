.. index::
   single: DomCrawler

Il componente DomCrawler
========================

    Il componente DomCrawler semplifica la navigazione nel DOM dei documenti HTML e XML.

Installazione
-------------

È possibile installare il componente in diversi modi:

* Utilizzando il repository ufficiale su Git (https://github.com/symfony/DomCrawler);
* Installandolo via PEAR ( `pear.symfony.com/DomCrawler`);
* Installandolo tramite Composer (`symfony/dom-crawler` on Packagist).

Utilizzo
--------

La classe :class:`Symfony\\Component\\DomCrawler\\Crawler` mette a disposizione metodi
per effettuare query e manipolare i documenti HTML e XML.

Un'istranza di Crawler rappresenta un insieme (:phpclass:`SplObjectStorage`) di 
oggetti :phpclass:`DOMElement`, che sono, in pratica, nodi facilmente 
visitabili::

    use Symfony\Component\DomCrawler\Crawler;

    $html = <<<'HTML'
    <html>
        <body>
            <p class="messaggio">Ciao Mondo!</p>
            <p>Ciao Crawler!</p>
        </body>
    </html>
    HTML;

    $crawler = new Crawler($html);

    foreach ($crawler as $elementoDom) {
        print $elementoDom->nodeName;
    }

Le classi specializzate :class:`Symfony\\Component\\DomCrawler\\Link` e
:class:`Symfony\\Component\\DomCrawler\\Form` sono utili per interagire con
collegamenti html e i form durante la visita dell'albero HTML.

Filtrare i nodi
~~~~~~~~~~~~~~~

È possibile usare facilmente le espressioni di XPath::

    $crawler = $crawler->filterXPath('descendant-or-self::body/p');

.. tip::

    internamente viene usato ``DOMXPath::query`` per eseguire le query XPath.

La ricerca è anche più semplice se si è installato il componente ``CssSelector``.
In questo modo è possibile usare lo stile JQuery per l'attraversamento::

    $crawler = $crawler->filter('body > p');

È possibile usare funzioni anonime per eseguire filtri complessi::

    $crawler = $crawler->filter('body > p')->reduce(function ($node, $i) {
        // filtra anche i nodi
        return ($i % 2) == 0;
    });

Per rimuovere i nodi, la funzione anonima dovrà restituire false.

.. note::

    Tutti i metodi dei filtri restituiscono una nuova istanza di :class:`Symfony\\Component\\DomCrawler\\Crawler`
    contenente gli elementi filtrati.

Attraversamento dei nodi
~~~~~~~~~~~~~~~~~~~~~~~~

Accedere ai nodi tramite la loro posizione nella lista::

    $crawler->filter('body > p')->eq(0);

Ottentere il primo o l'ultimo nodo della selezione::

    $crawler->filter('body > p')->first();
    $crawler->filter('body > p')->last();

Ottentere i nodi allo stesso livello della selezione attuale::

    $crawler->filter('body > p')->siblings();

Ottentere i nodi, allo stesso livello, precedenti o successivi alla selezione attuale::

    $crawler->filter('body > p')->nextAll();
    $crawler->filter('body > p')->previousAll();

Ottentere tutti i nodi figlio o padre::

    $crawler->filter('body')->children();
    $crawler->filter('body > p')->parents();

.. note::

    Tutti i metodi di attraversamento restituiscono un nuova istanza di
    :class:`Symfony\\Component\\DomCrawler\\Crawler`.

Accedere ai nodi tramite il loro valore
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Accedere al valore del primo nodo della selezione attuale::

    $message = $crawler->filterXPath('//body/p')->text();

Accedere al valore dell'attributo del primo nodo della selezione attuale::

    $class = $crawler->filterXPath('//body/p')->attr('class');

Estrarre l'attributo e/o il valore di un nodo da una lista di nodi::

    $attributes = $crawler->filterXpath('//body/p')->extract(array('_text', 'class'));

.. note::

    L'attributo speciale ``_text`` rappresenta il valore di un nodo.

Chiamare una funzione anonima su ogni nodo della lista::

    $nodeValues = $crawler->filter('p')->each(function ($nodo, $i) {
        return $nodo->nodeValue;
    });

La funzione anonima riceve la posizione e il nodo come argomenti.
Il risultato è un array contenente i valori restituiti dalle chiamate alla funzione anonima.

Aggiungere contenuti
~~~~~~~~~~~~~~~~~~~~

Il crawler supporta diversi modi per aggiungere contenuti::

    $crawler = new Crawler('<html><body /></html>');

    $crawler->addHtmlContent('<html><body /></html>');
    $crawler->addXmlContent('<root><node /></root>');

    $crawler->addContent('<html><body /></html>');
    $crawler->addContent('<root><node /></root>', 'text/xml');

    $crawler->add('<html><body /></html>');
    $crawler->add('<root><node /></root>');

Essendo l'implementazione del Crawler basata sull'estensione di DOM, è anche
possibile interagire con le classi native :phpclass:`DOMDocument`, :phpclass:`DOMNodeList`
e :phpclass:`DOMNode`:

.. code-block:: php

    $documento = new \DOMDocument();
    $documento->loadXml('<root><node /><node /></root>');
    $listaNodi = $documento->getElementsByTagName('node');
    $nodo = $documento->getElementsByTagName('node')->item(0);

    $crawler->addDocument($documento);
    $crawler->addNodeList($listaNodi);
    $crawler->addNodes(array($nodo));
    $crawler->addNode($nodo);
    $crawler->add($documento);

Supporto per i collegamenti e per i form
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Per i collegamenti e i form, contenuti nell'albero DOM, è riservato un trattamento speciale.

Collegamenti
............

Per trovare un collegamento tramite il suo nome (o un'immagine cliccabile tramite il suo
attributo ``alt``) si usa il metodo ``selectLink`` in un crawler esistente. La chiamata
retituisce un'istanza di Crawler contenente il/i solo/i collegamento/i selezionato/i. La chiamata ``link()``
restituisce l'oggetto speciale :class:`Symfony\\Component\\DomCrawler\\Link`::

    $linksCrawler = $crawler->selectLink('Vai altrove...');
    $link = $linksCrawler->link();

    // oppure, in una sola riga
    $link = $crawler->selectLink('Vai altrove...')->link();

L'oggetto :class:`Symfony\\Component\\DomCrawler\\Link` ha diversi utili metodi per
avere ulteriori informazioni relative al collegamento selezionato::

    // restituisce il valore di href
    $href = $link->getRawUri();

    // restituisce la URI che può essere utilizzata per effettuare nuove richieste
    $uri = $link->getUri();

Il metodo ``getUri()`` è specialmente utile perchè pulisce il valore di ``href``
e lo trasforma nel modo in cui dovrebbe realmente essere processato. Ad esempio, un collegamento
del tipo ``href="#foo"`` restituirà la URI completa della pagina corrente con il suffisso ``#foo``.
Il valore restituito da ``getUri()`` è sempre una URI completa sulla quale è 
possibile eseguire lavori.

I Form
......

Un trattamento speciale è riservato anche ai form. È disponibile, in Crawler,
un metodo ``selectButton()`` che restituisce un'altro Crawler relativo
al pulsante (``input[type=submit]``, ``input[type=image]``, o ``button``) con
il testo dato. Questo metodo è specialmente utile perchè può essere usato per restituire
un oggetto :class:`Symfony\\Component\\DomCrawler\\Form` che rappresenta 
il form all'interno del quale il pulsante è definito::

    $form = $crawler->selectButton('Valida')->form();

    // o "riempie" i campi del form con dati
    $form = $crawler->selectButton('Valida')->form(array(
        'nome' => 'Ryan',
    ));

L'oggetto :class:`Symfony\\Component\\DomCrawler\\Form` ha molti utilissimi
metodi che permettono di lavorare con i form:

    $uri = $form->getUri();

    $metodo = $form->getMethod();

Il metodo :method:`Symfony\\Component\\DomCrawler\\Form::getUri` fa più che
restituire il mero attributo ``action`` del form. Se il metodo del form è
GET, allora, imitando il comportamento del borwser, restituirà l'attributo
dell'azione seguito da una stringa di tutti i valori del form.

È possibile impostare e leggere virtualmente i valori nel form::

    // imposta, internamente, i valori del form
    $form->setValues(array(
        'registrazione[nomeutente]' => 'fandisymfony',
        'registrazione[termini]'    => 1,
    ));

    // restituisce un array di valori in un array "semplice", come in precedenza
    $values = $form->getValues();

    // restituisce i valori come li vedrebbe PHP con "registrazione" come array
    $values = $form->getPhpValues();

Per lavorare con i campi multi-dimensionali::

    <form>
        <input name="multi[]" />
        <input name="multi[]" />
        <input name="multi[dimensionale]" />
    </form>

È necessario specificare il nome pienamente qualificato del campo::

    // Imposta un singolo campo
    $form->setValue('multi[0]', 'valore');

    // Imposta molteplici campi in una sola volta
    $form->setValue('multi', array(
        1              => 'valore',
        'dimensionale' => 'un altro valore'
    ));

Se questo è fantastico, il resto è anche meglio! L'oggetto ``Form`` permette di
interagire con il form come se si usasse il borwser, slezionando i valori dei radio,
spuntando i checkbox e caricando file::

    $form['registrazione[nomeutente]']->setValue('fandisymfony');

    // cambia segno di spunta ad un checkbox
    $form['registrazione[termini]']->tick();
    $form['registrazione[termini]']->untick();

    // seleziona un'opzione
    $form['registrazione[data_nascita][anno]']->select(1984);

    // seleziona diverse opzioni da una lista di opzioni o da una serie di checkbox
    $form['registrazione[interessi]']->select(array('symfony', 'biscotti'));

    // può anche imitare l'upload di un file
    $form['registrazione[foto]']->upload('/percorso/al/file/lucas.jpg');

A cosa serve tutto questo ? Se si stanno eseguendo i test interni, è possibile
recuperare informazioni da tutti i form esattamente come se fossero stati inviati
utilizzando i valori PHP::

    $valori = $form->getPhpValues();
    $files = $form->getPhpFiles();

Se si utilizza un client HTTP esterno, è possibile usare il form per recuperare
tutte le informazioni necessarie per create una richiesta POST dal form::

    $uri = $form->getUri();
    $metodo = $form->getMethod();
    $valori = $form->getValues();
    $files = $form->getFiles();

    // a questo punto si usa un qualche client HTTP e si inviano le informazioni

Un ottimo esempio di sistema integrato che utilizza tutte queste funzioni è `Goutte`_.
Goutte usa a pieno gli oggetti Symfony Crawler e, con essi, può inviare i form 
direttamente::

    use Goutte\Client;

    // crea una richiesta ad un sito esterno
    $client = new Client();
    $crawler = $client->request('GET', 'https://github.com/login');

    // seleziona il form e riempie alcuni valori 
    $form = $crawler->selectButton('Log in')->form();
    $form['login'] = 'fandisymfony';
    $form['password'] = 'unapassword';

    // invia il form
    $crawler = $client->submit($form);

.. _`Goutte`: https://github.com/fabpot/goutte
