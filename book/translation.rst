.. index::
   single: Traduzioni

Traduzioni
==========

Il termine "internazionalizzazione" si riferisce al processo di astrazione delle stringhe 
e altri pezzi specifici dell'applicazione che variano in base al locale, in uno strato
dove possono essere tradotti e convertiti in base alle impostazioni internazionali dell'utente (ad esempio
lingua e paese). Per il testo, questo significa che ognuno viene avvolto con una funzione
capace di tradurre il testo (o "messaggio") nella lingua dell'utente::

    // il testo verrà *sempre* stampato in English
    echo 'Hello World';

    // il testo può essere tradotto nella lingua dell'utente finale o per impostazione predefinita in inglese
    echo $translator->trans('Hello World');

.. note::

    Il termine *locale* si riferisce all'incirca al linguaggio dell'utente e al paese.
    Può essere qualsiasi stringa che l'applicazione utilizza poi per gestire le traduzioni
    e altre differenze di formati (ad esempio il formato di valuta). Si consiglia di utilizzare
    il codice di *lingua* ISO639-1, un carattere di sottolineatura (``_``), poi il codice di *paese* ISO3166
    (per esempio ``fr_FR`` per francese / Francia).

In questo capitolo, si imparererà a preparare un'applicazione per supportare più
locale e poi a creare le traduzioni per più locale. Nel complesso,
il processo ha diverse fasi comuni:

1. Abilitare e configurare il componente ``Translation`` di Symfony;

2. Astrarre le stringhe (i. "messaggi") avvolgendoli nelle chiamate al ``Traduttore``;

3. Creare risorse di traduzione per ogni lingua supportata che traducano tutti
   i messaggio dell'applicazione;

4. Determinare, impostare e gestire le impostazioni locali dell'utente nella sessione.

.. index::
   single: Translations; Configuration

Configurazione
--------------

Le traduzioni sono gestire da un ``Traduttore`` :term:`service` che utilizza i
locale dell'utente per cercare e restituire i messaggi tradotti. Prima di utilizzarlo,
abilitare il ``Traduttore`` nella configurazione:

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

Il locale usato nelle traduzioni è quello memorizzato nella sessione utente.

.. index::
   single: Translations; Basic translation

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
  finale è un grande "dizionario" di traduzioni. Vedere i `Cataloghi dei messaggi`_
  per maggiori dettagli;

* Se il messaggio si trova nel catalogo, viene restituita la traduzione. Se
  no, il traduttore restituisce il messaggio originale.

Quando si usa il metodo ``trans()``, Symfony2 cerca la stringa esatta all'interno
del catalogo dei messaggi e la restituisce (se esiste).

.. index::
   single: Translations; Message placeholders

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
