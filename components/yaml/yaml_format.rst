.. index::
    single: Yaml; Formato Yaml

Il formato YAML
===============

Secondo il sito ufficiale di `YAML`_, YAML è "uno standard amichevole di serializzazione
dei dati per tutti i linguaggi di programmazione".

Anche se il formato YAML può
descrivere strutture di dati annidate in modo complesso, questo capitolo descrive solo
l'insieme minimo di caratteristiche per usare YAML come formato per i file di configurazione.

YAML è un semplice linguaggio che descrive dati. Come PHP, ha una sintassi per tipi
semplici, come stringhe, booleani, numeri a virgola mobile o interi. Ma, diversamente da
PHP, distingue tra array (sequenze) e hash (mappature).

Scalari
-------

La sintassi per gli scalari è simile a quella di PHP.

Stringhe
~~~~~~~~

.. code-block:: yaml

    Una stringa in YAML

.. code-block:: yaml

    'Una stringa in YAML tra apici singoli'

.. tip::

    In una stringa tra apici singoli, un apice singolo ``'`` va raddoppiato:

    .. code-block:: yaml

        'Un apice singolo '' in una stringa tra apici singoli'

.. code-block:: yaml

    "Una stringa in YAML tra apici doppi\n"

Gli apici sono utili quando una stringa inizia o finisce con uno o più spazi
significativi.

.. tip::

    Lo stile a doppi apici fornisce un modo per esprimere stringhe arbitrarie, ma
    usando sequenze di escape con ``\`` escape. È molto utile quando occorre inserire
    ``\n`` o un carattere unicode in una stringa.

Quando una stringa contiene degli a capo, si può usare lo stile letterale, indicato
dalla barra verticale (``|``), per indicare che la stringa si estende su diverse righe.
Nei letterali, gli a capo sono preservati:

.. code-block:: yaml

    |
      \/ /| |\/| |
      / / | |  | |__

In alternativa, le stringhe possono essere scritte con lo stile avvolto, denotato
da ``>``, in cui gli a capo sono sostituiti da uno spazio:

.. code-block:: yaml

    >
      Questa è una frase molto lunga
      che si espande per diverse righe in YAML
      ma che sarà resa come una stringa
      senza rimandi a capo.

.. note::

    Si notino i due spazi prima di ogni riga nell'esempio qui sopra. Non appariranno
    nella stringa PHP risultante.

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

    # un esponenziale
    1.2e+34

.. code-block:: yaml

    # infinito
    .inf

Null
~~~~

Null in YAML può essere espresso con ``null`` o con ``~``.

Booleani
~~~~~~~~

I booleani in YAML sono espressi con ``true`` e ``false``.

Date
~~~~

YAML usa lo standard ISO-8601 per esprimere le date:

.. code-block:: yaml

    2001-12-14t21:59:43.10-05:00

.. code-block:: yaml

    # data semplice
    2002-12-14

Insiemi
-------

Un file YAML è usato raramente per descrivere semplici scalari. La maggior parte delle
volte, descrive un insieme. Un insieme può essere una sequenza o una mappatura di
elementi. Sia le sequenze che le mappature sono convertite in array PHP.

Le sequenze usano un trattino, seguito da uno spazio:

.. code-block:: yaml

    - PHP
    - Perl
    - Python

Il file YAML qui sopra equivale al seguente codice PHP:

.. code-block:: php

    array('PHP', 'Perl', 'Python');

Le mappature usano un due punti, seguito da uno spazio (``:`` ) per marcare ogni coppia chiave/valore:

.. code-block:: yaml

    PHP: 5.2
    MySQL: 5.1
    Apache: 2.2.20

che equivale a questo codice PHP:

.. code-block:: php

    array('PHP' => 5.2, 'MySQL' => 5.1, 'Apache' => '2.2.20');

.. note::

    In una mappatura, una chiave può essere un qualsiasi scalare valido.

Il numero di spazi tra i due punti e il valore non è significativo:

.. code-block:: yaml

    PHP:    5.2
    MySQL:  5.1
    Apache: 2.2.20

YAML usa un'indentazione con uno o più spazi per descrivere insiemi annidati:

.. code-block:: yaml

    "symfony 1.0":
      PHP:    5.0
      Propel: 1.2
    "symfony 1.2":
      PHP:    5.2
      Propel: 1.3

Lo YAML qui sopra equivale al seguente codice PHP:

.. code-block:: php

    array(
      'symfony 1.0' => array(
        'PHP'    => 5.0,
        'Propel' => 1.2,
      ),
      'symfony 1.2' => array(
        'PHP'    => 5.2,
        'Propel' => 1.3,
      ),
    );

C'è una cosa importante da ricordare quando si usano le indentazioni in un file
YAML: *le indentazioni devono essere fatte con uno o più spazi, ma mai con
tabulazioni*.

Si possono annidare sequenze e mappature a volontà:

.. code-block:: yaml

    'Capitolo 1':
      - Introduzione
      - Tipi di eventi
    'Capitolo 2':
      - Introduzione
      - Helper

YAML può anche usare stili fluenti per gli insiemi, usando indicatori espliciti
invece che le intendantazioni, per denotare il livello.

Una sequenza può essere scritta come lista separata da virgole in parentesi quadre
(``[]``):

.. code-block:: yaml

    [PHP, Perl, Python]

Una mappatura può essere scritta come lista separata da virgole di chiavi/valori tra
parentesi graffe (`{}`):

.. code-block:: yaml

    { PHP: 5.2, MySQL: 5.1, Apache: 2.2.20 }

Si possono mescolare gli stili, per ottenere una migliore leggibilità:

.. code-block:: yaml

    'Chapter 1': [Introduzione, Tipi di eventi]
    'Chapter 2': [Introduzione, Helper]

.. code-block:: yaml

    "symfony 1.0": { PHP: 5.0, Propel: 1.2 }
    "symfony 1.2": { PHP: 5.2, Propel: 1.3 }

Commenti
--------

Si possono aggiungere commenti in YAML, usando come prefisso un cancelletto (``#``):

.. code-block:: yaml

    # Commento su una riga
    "symfony 1.0": { PHP: 5.0, Propel: 1.2 } # Commento a fine riga
    "symfony 1.2": { PHP: 5.2, Propel: 1.3 }

.. note::

    I commenti sono semplicemente ignorati dall'analizzatore YAML e non necessitano di
    indentazione in base al livello di annidamento di un insieme.

.. _YAML: http://yaml.org/
