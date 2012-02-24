.. index::
   single: Traduzioni

Traduzioni
==========

Il termine "internazionalizzazione" si riferisce al processo di astrazione delle stringhe 
e altri pezzi specifici dell'applicazione che variano in base al locale, in uno strato
dove possono essere tradotti e convertiti in base alle impostazioni internazionali dell'utente (ad esempio
lingua e paese). Per il testo, questo significa che ognuno viene avvolto con una funzione
capace di tradurre il testo (o "messaggio") nella lingua dell'utente::


    // il testo verrà *sempre* stampato in inglese
    echo 'Hello World';

    // il testo può essere tradotto nella lingua dell'utente finale o per impostazione predefinita in inglese
    echo $translator->trans('Hello World');

.. note::

    Il termine *locale* si riferisce all'incirca al linguaggio dell'utente e al paese.
    Può essere qualsiasi stringa che l'applicazione utilizza poi per gestire le traduzioni
    e altre differenze di formati (ad esempio il formato di valuta). Si consiglia di utilizzare
    il codice di *lingua* ISO639-1, un carattere di sottolineatura (``_``), poi il codice di *paese* ISO3166
    (per esempio ``fr_FR`` per francese / Francia).

In questo capitolo, si imparerà a preparare un'applicazione per supportare più
locale e poi a creare le traduzioni per più locale. Nel complesso,
il processo ha diverse fasi comuni:

1. Abilitare e configurare il componente ``Translation`` di Symfony;

2. Astrarre le stringhe (i. "messaggi") avvolgendoli nelle chiamate al ``Translator``;

3. Creare risorse di traduzione per ogni lingua supportata che traducano tutti
   i messaggio dell'applicazione;

4. Determinare, impostare e gestire le impostazioni locali nella sessione.

.. index::
   single: Traduzioni; Configurazione

Configurazione
--------------

Le traduzioni sono gestire da un :term:`servizio` ``Translator``, che utilizza i
locale dell'utente per cercare e restituire i messaggi tradotti. Prima di utilizzarlo,
abilitare il ``Translator`` nella configurazione:

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        framework:
            translator: { fallback: en }

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <framework:config>
            <framework:translator fallback="en" />
        </framework:config>

    .. code-block:: php

        // app/config/config.php
        $container->loadFromExtension('framework', array(
            'translator' => array('fallback' => 'en'),
        ));

L'opzione ``fallback`` definisce il locale da utilizzare quando una traduzione non
esiste nel locale dell'utente.

.. tip::

    Quando una traduzione non esiste per un locale, il traduttore prima prova
    a trovare la traduzione per la lingua (ad esempio ``fr`` se il locale è
    ``fr_FR``). Se non c'è, cerca una traduzione
    utilizzando il locale di ripiego.

Il locale usato nelle traduzioni è quello memorizzato nella sessione.

.. index::
   single: Traduzioni; Traduzioni di base

Traduzione di base
------------------

La traduzione del testo è fatta attraverso il servizio ``translator``
(:class:`Symfony\\Component\\Translation\\Translator`). Per tradurre un blocco
di testo (chiamato *messaggio*), usare il metodo
:method:`Symfony\\Component\\Translation\\Translator::trans`. Supponiamo,
ad esempio, che stiamo traducendo un semplice messaggio all'interno del controllore:

.. code-block:: php

    public function indexAction()
    {
        $t = $this->get('translator')->trans('Symfony2 is great');

        return new Response($t);
    }

Quando questo codice viene eseguito, Symfony2 tenterà di tradurre il messaggio
"Symfony2 is great" basandosi sul locale dell'utente. Perché questo funzioni,
bisogna dire a Symfony2 come tradurre il messaggio tramite una "risorsa di
traduzione", che è una raccolta di traduzioni dei messaggi per un dato locale.
Questo "dizionario" delle traduzioni può essere creato in diversi formati,
ma XLIFF è il formato raccomandato:

.. configuration-block::

    .. code-block:: xml

        <!-- messages.fr.xliff -->
        <?xml version="1.0"?>
        <xliff version="1.2" xmlns="urn:oasis:names:tc:xliff:document:1.2">
            <file source-language="en" datatype="plaintext" original="file.ext">
                <body>
                    <trans-unit id="1">
                        <source>Symfony2 is great</source>
                        <target>J'aime Symfony2</target>
                    </trans-unit>
                </body>
            </file>
        </xliff>

    .. code-block:: php

        // messages.fr.php
        return array(
            'Symfony2 is great' => 'J\'aime Symfony2',
        );

    .. code-block:: yaml

        # messages.fr.yml
        Symfony2 is great: J'aime Symfony2

Ora, se la lingua del locale dell'utente è il francese (per esempio ``fr_FR`` o ``fr_BE``),
il messaggio sarà tradotto in ``J'aime Symfony2``.

Il processo di traduzione
~~~~~~~~~~~~~~~~~~~~~~~~~

Per tradurre il messaggio, Symfony2 utilizza un semplice processo:

* Viene determinato il ``locale`` dell'utente corrente, che è memorizzato nella sessione;

* Un catalogo di messaggi tradotti viene caricato dalle risorse di traduzione definite
  per il ``locale`` (ad es. ``fr_FR``). Vengono anche caricati i messaggi dal locale predefinito
  e aggiunti al catalogo, se non esistono già. Il risultato
  finale è un grande "dizionario" di traduzioni. Vedere i `Cataloghi di messaggi`_
  per maggiori dettagli;

* Se il messaggio si trova nel catalogo, viene restituita la traduzione. Se
  no, il traduttore restituisce il messaggio originale.

Quando si usa il metodo ``trans()``, Symfony2 cerca la stringa esatta all'interno
del catalogo dei messaggi e la restituisce (se esiste).

.. index::
   single: Traduzioni; Segnaposto per i messaggi

Segnaposto per i messaggi
~~~~~~~~~~~~~~~~~~~~~~~~~

A volte, un messaggio contiene una variabile deve essere tradotta:

.. code-block:: php

    public function indexAction($name)
    {
        $t = $this->get('translator')->trans('Hello '.$name);

        return new Response($t);
    }

Tuttavia, la creazione di una traduzione per questa stringa è impossibile, poiché il traduttore
proverà a cercare il messaggio esatto, includendo le parti con le variabili
(per esempio "Ciao Ryan" o "Ciao Fabien"). Invece di scrivere una traduzione
per ogni possibile iterazione della variabile ``$name``, si può sostituire la
variabile con un "segnaposto":

.. code-block:: php

    public function indexAction($name)
    {
        $t = $this->get('translator')->trans('Hello %name%', array('%name%' => $name));

        new Response($t);
    }

Symfony2 cercherà ora una traduzione del messaggio raw (``Hello %name%``)
e *poi* sostituirà i segnaposto con i loro valori. La creazione di una traduzione
è fatta esattamente come prima:

.. configuration-block::

    .. code-block:: xml

        <!-- messages.fr.xliff -->
        <?xml version="1.0"?>
        <xliff version="1.2" xmlns="urn:oasis:names:tc:xliff:document:1.2">
            <file source-language="en" datatype="plaintext" original="file.ext">
                <body>
                    <trans-unit id="1">
                        <source>Hello %name%</source>
                        <target>Bonjour %name%</target>
                    </trans-unit>
                </body>
            </file>
        </xliff>

    .. code-block:: php

        // messages.fr.php
        return array(
            'Hello %name%' => 'Bonjour %name%',
        );

    .. code-block:: yaml

        # messages.fr.yml
        'Hello %name%': Hello %name%

.. note::

    Il segnaposto può assumere qualsiasi forma visto che il messaggio è ricostruito
    utilizzando la `funzione strtr`_ di PHP. Tuttavia, la notazione ``%var%`` è
    richiesta quando si traduce utilizzando i template Twig e in generale è una 
    convenzione che è consigliato seguire.

Come si è visto, la creazione di una traduzione è un processo in due fasi:

1. Astrarre il messaggio che si deve tradurre, processandolo tramite il
   ``Translator``.

2. Creare una traduzione per il messaggio in ogni locale che si desideri
   supportare.

Il secondo passo si esegue creando cataloghi di messaggi, che definiscono le traduzioni
per ogni diverso locale.

.. index::
   single: Traduzioni; Cataloghi di messaggi

Cataloghi di messaggi
---------------------

Quando un messaggio è tradotto, Symfony2 compila un catalogo di messaggi per
il locale dell'utente e guarda in esso per cercare la traduzione di un messaggio. Un catalogo
di messaggi è come un dizionario di traduzioni per uno specifico locale. Ad
esempio, il catalogo per il locale ``fr_FR`` potrebbe contenere la seguente
traduzione:

    Symfony2 is Great => J'aime Symfony2

È compito dello sviluppatore (o traduttore) di una applicazione
internazionalizzata creare queste traduzioni. Le traduzioni sono memorizzate sul
filesystem e vengono trovate da Symfony grazie ad alcune convenzioni.

.. tip::

    Ogni volta che si crea una *nuova* risorsa di traduzione (o si installa un pacchetto
    che include una risorsa di traduzione), assicurarsi di cancellare la cache in modo
    che Symfony possa scoprire la nuova risorsa di traduzione:
    
    .. code-block:: bash
    
        php app/console cache:clear

.. index::
   single: Traduzioni; Sedi per le traduzioni e convenzioni sui nomi

Sedi per le traduzioni e convenzioni sui nomi
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Symfony2 cerca i file dei messaggi (ad esempio le traduzioni) in due sedi:

* Per i messaggi trovati in un bundle, i corrispondenti file con i messaggi dovrebbero
  trovarsi nella cartella ``Resources/translations/`` del bundle;

* Per sovrascrivere eventuali traduzioni del bundle, posizionare i file con i messaggi
  nella cartella ``app/Resources/translations``.

È importante anche il nome del file con le traduzioni, perché Symfony2 utilizza una convenzione
per determinare i dettagli sulle traduzioni. Ogni file con i messaggi deve essere nominato
secondo il seguente schema: ``domain.locale.loader``:

* **domain**: Un modo opzionale per organizzare i messaggi in gruppi (ad esempio ``admin``,
  ``navigation`` o il predefinito ``messages``) - vedere `Uso dei domini per i messaggi`_;

* **locale**: Il locale per cui sono state scritte le traduzioni (ad esempio ``en_GB``, ``en``, ecc.);

* **loader**: Come Symfony2 dovrebbe caricare e analizzare il file (ad esempio ``xliff``,
  ``php`` o ``yml``).

Il loader può essere il nome di un qualunque loader registrato. Per impostazione predefinita, Symfony
fornisce i seguenti loader:

* ``xliff``: file XLIFF;
* ``php``:   file PHP;
* ``yml``:  file YAML.

La scelta di quali loader utilizzare è interamente a carico dello sviluppatore ed è una questione
di gusti.

.. note::

    È anche possibile memorizzare le traduzioni in un database o in qualsiasi altro storage,
    fornendo una classe personalizzata che implementa
    l'interfaccia :class:`Symfony\\Component\\Translation\\Loader\\LoaderInterface`.
    Vedere :doc:`Loader per le traduzioni personalizzati </cookbook/translation/custom_loader>`
    di seguito per imparare a registrare loader personalizzati.

.. index::
   single: Traduzioni; Creazione delle traduzioni

Creazione delle traduzioni
~~~~~~~~~~~~~~~~~~~~~~~~~~

La creazione di file di traduzione è una parte importante della "localizzazione" (spesso abbreviata in `L10n`_).
Ogni file è costituito da una serie di coppie id-traduzione per il dato dominio e
locale. L'id è l'identificativo di una traduzione individuale e può
essere il messaggio nel locale principale (ad es. "Symfony is great") dell'applicazione
o un identificatore univoci (ad es. "symfony2.great" - vedere la barra laterale di seguito):


.. configuration-block::

    .. code-block:: xml

        <!-- src/Acme/DemoBundle/Resources/translations/messages.fr.xliff -->
        <?xml version="1.0"?>
        <xliff version="1.2" xmlns="urn:oasis:names:tc:xliff:document:1.2">
            <file source-language="en" datatype="plaintext" original="file.ext">
                <body>
                    <trans-unit id="1">
                        <source>Symfony2 is great</source>
                        <target>J'aime Symfony2</target>
                    </trans-unit>
                    <trans-unit id="2">
                        <source>symfony2.great</source>
                        <target>J'aime Symfony2</target>
                    </trans-unit>
                </body>
            </file>
        </xliff>

    .. code-block:: php

        // src/Acme/DemoBundle/Resources/translations/messages.fr.php
        return array(
            'Symfony2 is great' => 'J\'aime Symfony2',
            'symfony2.great'    => 'J\'aime Symfony2',
        );

    .. code-block:: yaml

        # src/Acme/DemoBundle/Resources/translations/messages.fr.yml
        Symfony2 is great: J'aime Symfony2
        symfony2.great:    J'aime Symfony2

Symfony2 troverà questi file e li utilizzerà quando dovrà tradurre
"Symfony2 is great" o "symfony2.great" in un locale di lingua francese (ad es.
``fr_FR`` o ``fr_BE``).

.. sidebar:: Utilizzare messaggi reali o parole chiave

    Questo esempio mostra le due diverse filosofie nella creazione di
    messaggi che dovranno essere tradotti:

    .. code-block:: php

        $t = $translator->trans('Symfony2 is great');

        $t = $translator->trans('symfony2.great');

    Nel primo metodo, i messaggi vengono scritti nella lingua del locale
    predefinito (in inglese in questo caso). Questo messaggio viene quindi utilizzato come "id"
    durante la creazione delle traduzioni.

    Nel secondo metodo, i messaggi sono in realtà "parole chiave" che trasmettono
    l'idea del messaggio.Il messaggio chiave è quindi utilizzato come "id" per
    eventuali traduzioni. In questo caso, deve essere fatta anche la traduzione per il locale
    predefinito (ad esempio per tradurre ``symfony2.great`` in ``Symfony2 is great``).

    Il secondo metodo è utile perché non sarà necessario cambiare la chiave del messaggio
    in ogni file di traduzione se decidiamo che il messaggio debba essere modificato
    in "Symfony2 is really great" nel locale predefinito.

    La scelta del metodo da utilizzare è personale, ma il formato
    "chiave" è  spesso raccomandato.

    Inoltre, i formati di file ``php`` e ``yaml`` supportano gli id nidificati, per
    evitare di ripetersi se si utilizzano parole chiave al posto di testo reale per gli
    id:

    .. configuration-block::

        .. code-block:: yaml

            symfony2:
                is:
                    great: Symfony2 is great
                    amazing: Symfony2 is amazing
                has:
                    bundles: Symfony2 has bundles
            user:
                login: Login

        .. code-block:: php

            return array(
                'symfony2' => array(
                    'is' => array(
                        'great' => 'Symfony2 is great',
                        'amazing' => 'Symfony2 is amazing',
                    ),
                    'has' => array(
                        'bundles' => 'Symfony2 has bundles',
                    ),
                ),
                'user' => array(
                    'login' => 'Login',
                ),
            );

    I livelli multipli vengono appiattiti in singole coppie id/traduzione tramite
    l'aggiunta di un punto (.) tra ogni livello, quindi gli esempi di cui sopra sono
    equivalenti al seguente:

    .. configuration-block::

        .. code-block:: yaml

            symfony2.is.great: Symfony2 is great
            symfony2.is.amazing: Symfony2 is amazing
            symfony2.has.bundles: Symfony2 has bundles
            user.login: Login

        .. code-block:: php

            return array(
                'symfony2.is.great' => 'Symfony2 is great',
                'symfony2.is.amazing' => 'Symfony2 is amazing',
                'symfony2.has.bundles' => 'Symfony2 has bundles',
                'user.login' => 'Login',
            );

.. index::
   single: Traduzioni; Domini dei messaggi

Uso dei domini per i messaggi
-----------------------------

Come abbiamo visto, i file dei messaggi sono organizzati nei diversi locale che
vanno a tradurre. I file dei messaggi possono anche essere organizzati in "domini".
Quando si creano i file dei messaggi, il dominio è la prima parte del nome del file.
Il dominio predefinito è ``messages``. Per esempio, supponiamo che, per organizzarle al meglio,
le traduzioni siano state divise in tre diversi domini: ``messages``, ``admin``
e ``navigation``. La traduzione francese avrebbe i seguenti file
per i messaggi:

* ``messages.fr.xliff``
* ``admin.fr.xliff``
* ``navigation.fr.xliff``

Quando si traducono stringhe che non sono nel dominio predefinito (``messages``),
è necessario specificare il dominio come terzo parametro di ``trans()``:

.. code-block:: php

    $this->get('translator')->trans('Symfony2 is great', array(), 'admin');

Symfony2 cercherà ora il messaggio del locale dell'utente nel dominio
``admin``.

.. index::
   single: Traduzioni; Locale dell'utente

Gestione del locale dell'utente
-------------------------------

Il locale dell'utente corrente è memorizzato nella sessione ed è accessibile
tramite il servizio ``session``:

.. code-block:: php

    $locale = $request->getLocale();

    $request->setLocale('en_US');

.. index::
   single: Traduzioni; Fallback e locale predefinito

Fallback e locale predefinito
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Se il locale non è stato impostato in modo esplicito nella sessione, sarà
utilizzato dal ``Translator`` il parametro di configurazione ``fallback_locale``. Il valore
predefinito del parametro è ``en`` (vedere `Configurazione`_).

In alternativa, è possibile garantire che un locale è impostato sulla sessione dell'utente
definendo un ``default_locale`` per il servizio di sessione:

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        framework:
            session: { default_locale: en }

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <framework:config>
            <framework:session default-locale="en" />
        </framework:config>

    .. code-block:: php

        // app/config/config.php
        $container->loadFromExtension('framework', array(
            'session' => array('default_locale' => 'en'),
        ));

.. _book-translation-locale-url:

Il locale e gli URL
~~~~~~~~~~~~~~~~~~~

Dal momento che il locale dell'utente è memorizzato nella sessione, si può essere tentati
di utilizzare lo stesso URL per visualizzare una risorsa in più lingue in base
al locale dell'utente. Per esempio, ``http://www.example.com/contact`` può
mostrare contenuti in inglese per un utente e in francese per un altro. Purtroppo
questo viola una fondamentale regola del web: un particolare URL deve restituire
la stessa risorsa indipendentemente dall'utente. Inoltre, quale
versione del contenuto dovrebbe essere indicizzata dai motori di ricerca?

Una politica migliore è quella di includere il locale nell'URL. Questo è completamente
dal sistema delle rotte utilizzando il parametro speciale ``_locale``:

.. configuration-block::

    .. code-block:: yaml

        contact:
            pattern:   /{_locale}/contact
            defaults:  { _controller: AcmeDemoBundle:Contact:index, _locale: en }
            requirements:
                _locale: en|fr|de

    .. code-block:: xml

        <route id="contact" pattern="/{_locale}/contact">
            <default key="_controller">AcmeDemoBundle:Contact:index</default>
            <default key="_locale">en</default>
            <requirement key="_locale">en|fr|de</requirement>
        </route>

    .. code-block:: php

        use Symfony\Component\Routing\RouteCollection;
        use Symfony\Component\Routing\Route;

        $collection = new RouteCollection();
        $collection->add('contact', new Route('/{_locale}/contact', array(
            '_controller' => 'AcmeDemoBundle:Contact:index',
            '_locale'     => 'en',
        ), array(
            '_locale'     => 'en|fr|de'
        )));

        return $collection;

Quando si utilizza il parametro speciale `_locale` in una rotta, il locale corrispondente
verrà *automaticamente impostato sulla sessione dell'utente*. In altre parole, se un utente
visita l'URI ``/fr/contact``, il locale ``fr`` viene impostato automaticamente
come locale per la sessione dell'utente.

È ora possibile utilizzare il locale dell'utente per creare rotte ad altre pagine tradotte
nell'applicazione.

.. index::
   single: Traduzioni; Pluralizzazione

Pluralizzazione
---------------

La pluralizzazione dei messaggi è un argomento un po' difficile, perché le regole possono essere complesse. Per
esempio, questa è la rappresentazione matematica delle regole di pluralizzazione
russe::

    (($number % 10 == 1) && ($number % 100 != 11)) ? 0 : ((($number % 10 >= 2) && ($number % 10 <= 4) && (($number % 100 < 10) || ($number % 100 >= 20))) ? 1 : 2);

Come si può vedere, in russo si possono avere tre diverse forme plurali, ciascuna
dato un indice di 0, 1 o 2. Per ciascuna forma il plurale è diverso e
quindi anche la traduzione è diversa.

Quando una traduzione ha forme diverse a causa della pluralizzazione, è possibile fornire
tutte le forme come una stringa separata da un pipe (``|``)::

    'There is one apple|There are %count% apples'

Per tradurre i messaggi pluralizzati, utilizzare il
metodo :method:`Symfony\\Component\\Translation\\Translator::transChoice`:

.. code-block:: php

    $t = $this->get('translator')->transChoice(
        'There is one apple|There are %count% apples',
        10,
        array('%count%' => 10)
    );

Il secondo parametro (``10`` in questo esempio), è il *numero* di oggetti
che vengono descritti ed è usato per determinare quale traduzione è da usare e anche per popolare
il segnaposto ``%count%``.

In base al numero dato, il traduttore sceglie la giusta forma plurale.
In inglese, la maggior parte delle parole hanno una forma singolare quando c'è esattamente un oggetto
e una forma plurale per tutti gli altri numeri (0, 2, 3...). Quindi, se ``count`` è
``1``, il traduttore utilizzerà la prima stringa (``There is one apple``)
come traduzione. Altrimenti userà ``There are %count% apples``.

Ecco la traduzione francese::

    'Il y a %count% pomme|Il y a %count% pommes'

Anche se la stringa è simile (è fatta di due sotto-stringhe separate da un
carattere pipe), le regole francesi sono differenti: la prima forma (non plurale) viene utilizzata quando
``count`` è ``0`` o ``1``. Così, il traduttore utilizzerà automaticamente la
prima stringa (``Il y a %count% pomme``) quando ``count`` è ``0`` o ``1``.

Ogni locale ha una propria serie di regole, con alcuni che hanno ben sei differenti
forme plurali con regole complesse che descrivono quali numeri mappano le forme plurali.
Le regole sono abbastanza semplici per l'inglese e il francese, ma per il russo, si
potrebbe aver bisogno di un aiuto per sapere quali regole corrispondono alle stringhe. Per aiutare i traduttori,
è possibile opzionalmente "etichettare" ogni stringa::

    'one: There is one apple|some: There are %count% apples'

    'none_or_one: Il y a %count% pomme|some: Il y a %count% pommes'

Le etichette sono solo aiuti per i traduttori e non influenzano la logica
usata per determinare quale plurale è da usare. Le etichette possono essere una qualunque stringa
che termina con due punti(``:``). Le etichette inoltre non hanno bisogno di essere le
stesse nel messaggio originale e in quello tradotto.

.. tip:

    Essendo le etichette opzionali, il traduttore non le utilizza (il traduttore
    otterrà solo una stringa basata sulla sua posizione nella stringa).

Intervallo di pluralizzazione esplicito
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Il modo più semplice per pluralizzare un messaggio è quello di lasciare che Symfony2 utilizzi la sua logica interna
per scegliere quale stringa utilizzare sulla base di un dato numero. A volte
c'è bisogno di più controllo o si vuole una traduzione diversa per casi specifici (per
``0``, o   quando il conteggio è negativo, ad esempio). In tali casi, è possibile
utilizzare espliciti intervalli matematici::

    '{0} There is no apples|{1} There is one apple|]1,19] There are %count% apples|[20,Inf] There are many apples'

Gli intervalli seguono la notazione `ISO 31-11`_. La suddetta stringa specifica
quattro diversi intervalli: esattamente ``0``, esattamente ``1``, ``2-19`` e ``20``
e superiori.

È inoltre possibile combinare le regole matematiche e le regole standard. In questo caso, se
il numero non corrisponde ad un intervallo specifico, le regole standard hanno
effetto dopo aver rimosso le regole esplicite::

    '{0} There is no apples|[20,Inf] There are many apples|There is one apple|a_few: There are %count% apples'

Ad esempio, per ``1`` mela, verrà usata la regola standard ``C'è una mela``.
Per ``2-19`` mele, verrà utilizzata la seconda regola standard
``Ci sono %count% mele``.

:class:`Symfony\\Component\\Translation\\Interval` può rappresentare un insieme finito
di numeri::

    {1,2,3,4}

O numeri tra due numeri::

    [1, +Inf[
    ]-1,2[

Il delimitatore di sinistra può essere ``[`` (incluso) o ``]`` (escluso). Il delimitatore
di destra può essere ``[`` (escluso) o ``]`` (incluso). Oltre ai numeri, si
può usare ``-Inf`` e ``+Inf`` per l'infinito.

.. index::
   single: Traduzioni; Nei template

Traduzioni nei template
-----------------------

La maggior parte delle volte, la traduzione avviene nei template. Symfony2 fornisce un supporto
nativo sia per i template Twig che per i template PHP.

Template Twig
~~~~~~~~~~~~~

Symfony2 fornisce dei tag specifici per Twig (``trans`` e ``transchoice``) per
aiutare nella traduzione di messaggi con *blocchi statici di testo*:

.. code-block:: jinja

    {% trans %}Hello %name%{% endtrans %}

    {% transchoice count %}
        {0} There is no apples|{1} There is one apple|]1,Inf] There are %count% apples
    {% endtranschoice %}

Il tag ``transchoice`` ottiene automaticamente la variabile ``%count%`` dal
contesto corrente e la passa al traduttore. Questo meccanismo funziona
solo quando si utilizza un segnaposto che segue lo schema ``%var%``.

.. tip::

    Se in una stringa è necessario usare il carattere percentuale (``%``), escapizzarlo
    raddoppiandolo: ``{% trans %}Percent: %percent%%%{% endtrans %}``

È inoltre possibile specificare il dominio del messaggio e passare alcune variabili aggiuntive:

.. code-block:: jinja

    {% trans with {'%name%': 'Fabien'} from "app" %}Hello %name%{% endtrans %}

    {% trans with {'%name%': 'Fabien'} from "app" into "fr" %}Hello %name%{% endtrans %}

    {% transchoice count with {'%name%': 'Fabien'} from "app" %}
        {0} There is no apples|{1} There is one apple|]1,Inf] There are %count% apples
    {% endtranschoice %}

I filtri ``trans`` e ``transchoice`` possono essere usati per tradurre *variabili
di testo* ed espressioni complesse:

.. code-block:: jinja

    {{ message|trans }}

    {{ message|transchoice(5) }}

    {{ message|trans({'%name%': 'Fabien'}, "app") }}

    {{ message|transchoice(5, {'%name%': 'Fabien'}, 'app') }}

.. tip::

    Utilizzare i tag di traduzione o i filtri ha lo stesso effetto, ma con
    una sottile differenza: l'escape automatico dell'output è applicato solo alle
    variabili tradotte utilizzando un filtro. In altre parole, se è necessario
    essere sicuri che la variabile tradotta *non* venga escapizzata, è necessario
    applicare il filtro raw dopo il filtro di traduzione:

    .. code-block:: jinja

            {# il testo tradotto tra i tag non è mai sotto escape #}
            {% trans %}
                <h3>foo</h3>
            {% endtrans %}

            {% set message = '<h3>foo</h3>' %}

            {# una variabile tradotta tramite filtro è sotto escape per impostazione predefinita #}
            {{ message|trans|raw }}

            {# le stringhe statiche non sono mai sotto escape #}
            {{ '<h3>foo</h3>'|trans }}

Template PHP
~~~~~~~~~~~~

Il servizio di traduzione è accessibile nei template PHP attraverso
l'helper ``translator``:

.. code-block:: html+php

    <?php echo $view['translator']->trans('Symfony2 is great') ?>

    <?php echo $view['translator']->transChoice(
        '{0} There is no apples|{1} There is one apple|]1,Inf[ There are %count% apples',
        10,
        array('%count%' => 10)
    ) ?>

Forzare il locale della traduzione
----------------------------------

Quando si traduce un messaggio, Symfony2 utilizza il lodale della sessione utente
o il locale ``fallback`` se necessario. È anche possibile specificare manualmente il
locale da usare per la traduzione:

.. code-block:: php

    $this->get('translator')->trans(
        'Symfony2 is great',
        array(),
        'messages',
        'fr_FR',
    );

    $this->get('translator')->trans(
        '{0} There are no apples|{1} There is one apple|]1,Inf[ There are %count% apples',
        10,
        array('%count%' => 10),
        'messages',
        'fr_FR',
    );

Tradurre contenuti da un database
---------------------------------

La traduzione del contenuto di un database dovrebbero essere gestite da Doctrine attraverso
l'`Estensione Translatable`_. Per maggiori informazioni, vedere la documentazione
di questa libreria.

Riepilogo
---------

Con il componente Translation di Symfony2, la creazione e l'internazionalizzazione di applicazioni
non è più un processo doloroso	e si riduce solo a pochi semplici
passi:

* Astrarre i messaggi dell'applicazione avvolgendoli utilizzando i metodi
  :method:`Symfony\\Component\\Translation\\Translator::trans` o
  :method:`Symfony\\Component\\Translation\\Translator::transChoice`;

* Tradurre ogni messaggio in più locale creando dei file con i messaggi
  per la traduzione. Symfony2 scopre ed elabora ogni file perché i suoi nomi seguono
  una specifica convenzione;

* Gestire il locale dell'utente, che è memorizzato nella sessione.

.. _`i18n`: http://it.wikipedia.org/wiki/Internazionalizzazione_e_localizzazione
.. _`L10n`: http://it.wikipedia.org/wiki/Internazionalizzazione_e_localizzazione
.. _`funzione strtr`: http://www.php.net/manual/en/function.strtr.php
.. _`ISO 31-11`: http://en.wikipedia.org/wiki/Interval_%28mathematics%29#The_ISO_notation
.. _`Estensione Translatable`: https://github.com/l3pp4rd/DoctrineExtensions
