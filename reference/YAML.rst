.. index::
   single: YAML
   single: Configurazione; YAML

YAML
====

Il sito web di `YAML`_ dice "uno standard amichevole per la serializzazione dei dati
per tutti i linguaggi di programmazione". YAML è un semplice linguaggio per descrivere dati.
Come il PHP, ha una sintassi per i tipi semplici come stringhe, booleani, decimali, o interi.
Ma a differenza di PHP, tratta in modo differente array (sequenze) e hash
(mapping).

Il componente di Symfony2 :namespace:`Symfony\\Component\\Yaml` sa come interpretare
lo YAML e trasformare un array PHP in YAML.

.. note::

    Anche se il formato YAML può descrivere complesse strutture dati nidificate, questa
    guida tratta solo il minimo insieme di funzionalità necessarie per utilizzare YAML
    come file per il formato di configurazione.

Leggere file YAML
-----------------

Il metodo :method:`Symfony\\Component\\Yaml\\Parser::parse` analizza una stringa
YAML e la converte in array PHP::

    use Symfony\Component\Yaml\Parser;

    $yaml = new Parser();
    $value = $yaml->parse(file_get_contents('/path/to/file.yaml'));

Se durante l'analisi si incontra un errore, il parser lancia una eccezione, che indica
il tipo di errore e la linea della stringa YAML originale dove è stato riscontrato
l'errore::

    try {
        $value = $yaml->parse(file_get_contents('/path/to/file.yaml'));
    } catch (\InvalidArgumentException $e) {
        // durante l'analisi è stato riscontrato un errore
        echo "Impossibile analizzare la stringa YAML: ".$e->getMessage();
    }

.. tip::

    Poiché il parser riparte ogni volta, è possibile utilizzare lo stesso oggetto parser
    per caricare stringhe YAML diverse.

Quando si carica un file YAML, spesso è meglio utilizzare il
metodo wrapper :method:`Symfony\\Component\\Yaml\\Yaml::load`::

    use Symfony\Component\Yaml\Yaml;

    $loader = Yaml::parse('/path/to/file.yml');

Il metodo statico ``Yaml::load()`` prende una stringa YAML o un file che contengono
YAML. Internamente, viene chiamato il metodo ``Parser::parse()``, ma con alcuni bonus
aggiunti:

* Esegue il file YAML come se fosse un file PHP, ed in questo modo è possibile incorporare
  comandi PHP in file YAML;

* Quando un file non può essere analizzato, questo aggiunge automaticamente il nome del file
  al messaggio di errore, semplificando il debug nel caso l'applicazione stia caricando
  diversi file YAML.

Scrivere file YAML
------------------

Il metodo :method:`Symfony\\Component\\Yaml\\Dumper::dump` converte qualunque array PHP
nella sua rappresentazione YAML::

    use Symfony\Component\Yaml\Dumper;

    $array = array('foo' => 'bar', 'bar' => array('foo' => 'bar', 'bar' => 'baz'));

    $dumper = new Dumper();
    $yaml = $dumper->dump($array);
    file_put_contents('/path/to/file.yaml', $yaml);

.. note::

    Ci sono alcune limitazioni: il metodo non è in grado di convertire risorse e
    convertire oggetti PHP è da considerarsi una funzionalità in stato alpha.

Se c'è bisogno di convertire solo un array, si può usare una
scorciatoia con il metodo statico :method:`Symfony\\Component\\Yaml\\Yaml::dump`::

    $yaml = Yaml::dump($array, $inline);

Il formato YAML supporta le due rappresentazioni di array di YAML. Per impostazione
predefinita, il dumper utilizza la rappresentazione in linea:

.. code-block:: yaml

    { foo: bar, bar: { foo: bar, bar: baz } }

Ma il secondo parametro del metodo ``dump()`` permette di personalizzare il livello nel
quale l'output commuta dalla rappresentazione espansa a quella in linea::

    echo $dumper->dump($array, 1);

.. code-block:: yaml

    foo: bar
    bar: { foo: bar, bar: baz }

.. code-block:: php

    echo $dumper->dump($array, 2);

.. code-block:: yaml

    foo: bar
    bar:
        foo: bar
        bar: baz

La sintassi YAML
----------------

Stringhe
~~~~~~~~

.. code-block:: yaml

    Una stringa in YAML

.. code-block:: yaml

    'Una stringa in YAML individuata da apici singoli'

.. tip::
   In una stringa individuata da apici singoli, un singolo apice ``'`` deve essere raddoppiato:

   .. code-block:: yaml

        'Un singolo apice '' in una stringa individuata da apici singoli'

.. code-block:: yaml

    "Una stringa in YAML individuata da apici doppi\n"

L'utilizzo degli apici è utile quando una stringa inizia o finisce con uno o più spazi
utili.

.. tip::

    L'utilizzo del doppio apice fornisce un modo per esprimere stringhe arbitrarie,
    utilizzando ``\`` sequenze di escape. E' molto utile quando c'è bisogno di incorporare
    uno ``\n`` o un carattere unicode in una stringa.

Quando una stringa contiene interruzioni di linea, si può usare lo stile letterale, indicato
con un simbolo pipe di riga verticale (``|``), per indicare che la stringa si estende su più righe. Nei
letterali, gli a capo sono preservati:

.. code-block:: yaml

    |
      \/ /| |\/| |
      / / | |  | |__

In alternativa, le stringhe possono essere scritte con lo stile indentato, indicato con
``>``, in cui ogni interruzione di riga viene sostituita da uno spazio:

.. code-block:: yaml

    >
      Questa è una frase molto lunga
      che occupa diverse linee in YAML
      ma che verrà visualizzato in una stringa
      senza ritorni a capo.

.. note::

    Notare i due spazi prima di ciascuna linea dell'esempio precedente. Non compariranno
    nella stringa PHP risultante .

Numeri
~~~~~~

.. code-block:: yaml

    # un intero
    12

.. code-block:: yaml

    # un ottale
    014

.. code-block:: yaml

    # un esadecimale
    0xC

.. code-block:: yaml

    # un numero a virgola mobile
    13.4

.. code-block:: yaml

    # un numero esponenziale
    1.2e+34

.. code-block:: yaml

    # infinito
    .inf

Null
~~~~~

Null in YAML può essere espresso con ``null`` o ``~``.

Booleani
~~~~~~~~

I booleani in YAML vengono espressi con ``true`` e ``false``.

Date
~~~~

YAML utilizza lo standard ISO-8601 per esprimere le date:

.. code-block:: yaml

    2001-12-14t21:59:43.10-05:00

.. code-block:: yaml

    # una semplice data
    2002-12-14

Insiemi
~~~~~~~

Un file YAML è usato raramente per descrivere un semplice scalare. La maggior parte
delle volte, è utilizzato per descrive un insieme. Un insieme può essere
una sequenza o una mappa di elementi. Sia le sequenze che le mappe sono convertite in array PHP.

Le sequenze utilizzano un trattino seguito da uno spazio (``- ``):

.. code-block:: yaml

    - PHP
    - Perl
    - Python

Il precedente file YAML è equivalente al seguente codice PHP::

    array('PHP', 'Perl', 'Python');

Le mappe usano i due punti seguiti da uno spazio (``: ``) per segnare ciascuna coppia chiave/valore:

.. code-block:: yaml

    PHP: 5.2
    MySQL: 5.1
    Apache: 2.2.20

che è equivalente a questo codice PHP::

    array('PHP' => 5.2, 'MySQL' => 5.1, 'Apache' => '2.2.20');

.. note::

    In una mappa, una chiave può essere qualunque valore scalare.

Il numero di spazi tra i due punti e il valore non ha importanza:

.. code-block:: yaml

    PHP:    5.2
    MySQL:  5.1
    Apache: 2.2.20

YAML utilizza l'indentazione con uno o più spazi per descrivere insiemi annidati:

.. code-block:: yaml

    "symfony 1.4":
        PHP:      5.2
        Doctrine: 1.2
    "Symfony2":
        PHP:      5.3
        Doctrine: 2.0

Il codice YAML sovrastante è equivalente al seguente codice PHP::

    array(
        'symfony 1.4' => array(
            'PHP'      => 5.2,
            'Doctrine' => 1.2,
        ),
        'Symfony2' => array(
            'PHP'      => 5.3,
            'Doctrine' => 2.0,
        ),
    );

C'è una cosa importante da ricordare quando si usa l'indentazione in un
file YAML: *L'indentazione deve essere fatta con uno o più spazi, ma mai con
tabulazioni*.

È possibile nidificare a piacere sequenze e mappature:

.. code-block:: yaml

    'Capitolo 1':
        - Introduzione
        - Tipi di eventi
    'Capitolo 2':
        - Introduzione
        - Helper

YAML può anche usare gli stili flow per gli insiemi, utilizzando indicatori espliciti
al posto dell'indentazione per denotare l'ambito.

Una sequenza può essere scritta come un elenco separato da virgole, tra parentesi quadre
(``[]``):

.. code-block:: yaml

    [PHP, Perl, Python]

Una mappa può essere scritta come un elenco separato da virgole, tra parentesi graffe
(``{}``):

.. code-block:: yaml

    { PHP: 5.2, MySQL: 5.1, Apache: 2.2.20 }

È possibile combinare gli stili per ottenere una migliore leggibilità:

.. code-block:: yaml

    'Capitolo 1': [Introduzione, Tipi di eventi]
    'Capitolo 2': [Introduzione, Helper]

.. code-block:: yaml

    "symfony 1.4": { PHP: 5.2, Doctrine: 1.2 }
    "Symfony2":    { PHP: 5.3, Doctrine: 2.0 }

Commenti
~~~~~~~~

Possono essere aggiunti commenti in YAML, aggiungendo un simbolo di cancelletto (``#``) a inizio riga:

.. code-block:: yaml

    # Commento su una linea
    "Symfony2": { PHP: 5.3, Doctrine: 2.0 } # Commento alla fine di una linea

.. note::

    I commenti sono semplicemente ignorati dal parser YAML e non necessitano di essere
    indentati in accordo con l'attuale livello di nidificazione in un insieme.

File YAML dinamici
~~~~~~~~~~~~~~~~~~

In Symfony2, un file YAML può contenere codice PHP, che è valutato poco prima
dell'analisi:

.. code-block:: yaml

    1.0:
        version: <?php echo file_get_contents('1.0/VERSION')."\n" ?>
    1.1:
        version: "<?php echo file_get_contents('1.1/VERSION') ?>"

Fare attenzione a non pasticciare con i rientri. Quando si aggiunge codice PHP
ad un file YAML, tenere a mente i seguenti consigli:

* La dichiarazione ``<?php ?>`` deve sempre iniziare la linea, o essere compresa in un
  valore.

* Se una dichiarazione ``<?php ?>`` termina una linea, bisogna aggiungere l'output esplicito
  di una nuova linea ("\n").

.. _YAML: http://yaml.org/
