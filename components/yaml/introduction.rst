.. index::
   single: Yaml
   single: Componenti; Yaml

Il componente YAML
==================

    Il componente YAML carica ed esporta file YAML.

Che cos'è?
----------

Il componente YAML di Symfony2 analizza stringhe YAML da convertire in array PHP.
È anche in grado di convertire array PHP in stringhe YAML.

`YAML`_, *YAML Ain't Markup Language*, è uno standard amichevole di serializzazione di dati
per tutti i linguaggi di programmazione. YAML è un ottimo formato per i file di
configurazione. I file YAML sono espressivi quanto i file XML e leggibili quanto i file
INI.

Il componente YAML di Symfony2 implementa la versione 1.2. della
specifica.

.. tip::

    Si può sapere di più sul componente Yaml in
    :doc:`/components/yaml/yaml_format`.

Installazione
-------------

Si può installare il componente in molti modi diversi:

* Usare il repository ufficiale su Git (https://github.com/symfony/Yaml);
* :doc:`Installarlo via Composer</components/using_components>` (``symfony/yaml`` su `Packagist`_).

Perché?
-------

Veloce
~~~~~~

Uno degli scopi di YAML è trovare il giusto rapporto tra velocità e caratteristiche.
Supporta proprio la caratteristica necessaria per gestire i file di configurazione.

Analizzatore reale
~~~~~~~~~~~~~~~~~~

Dispone di un analizzatore reale, capace di analizzare un grande sotto-insieme della
specifica YAML, per tutte le necessità di configurazione. Significa anche che
l'analizzatore è molto robusto, facile da capire e facile da estendere.

Messaggi di errore chiari
~~~~~~~~~~~~~~~~~~~~~~~~~

Ogni volta che si ha un problema di sintassi con un proprio file YAML, la libreria
mostra un utile messaggio di errore, con il nome del file e il numero di riga in cui
il problema si è verificato. Questo facilita parecchio le operazioni di debug.

Supporto per l'esportazione
~~~~~~~~~~~~~~~~~~~~~~~~~~~

È anche in grado di esportare array PHP in YAML con supporto agli oggetti e
configurazione in linea per output eleganti.

Tipi supportati
~~~~~~~~~~~~~~~

Supporta la maggior parte dei tipi di YAML, come date, interi, ottali, booleani
e molto altro...

Pieno supporto alla fusione di chiavi
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Pieno supporto per riferimenti, alias e piena fusione di chiavi. Non occorre ripetersi
usando riferimenti a bit comuni di configurazione.

Usare il componente YAML di Symfony2
------------------------------------

Il componente YAML di Symfony2 è molto semplice e consiste di due classi principali:
una analizza le stringhe YAML (:class:`Symfony\\Component\\Yaml\\Parser`) e l'altra
esporta un array PHP in una stringa YAML
(:class:`Symfony\\Component\\Yaml\\Dumper`).

Sopra queste classi, la classe :class:`Symfony\\Component\\Yaml\\Yaml` funge
da involucro leggero, il che semplifica gli usi più comuni.

Leggere file YAML
~~~~~~~~~~~~~~~~~

Il metodo :method:`Symfony\\Component\\Yaml\\Parser::parse` analizza una stringa YAML
e la converte in un array PHP:

.. code-block:: php

    use Symfony\Component\Yaml\Parser;

    $yaml = new Parser();

    $value = $yaml->parse(file_get_contents('/percorso/del/file.yml'));

Se si verifica un errore durante l'analizi, l'analizzatore lancia un'eccezione
:class:`Symfony\\Component\\Yaml\\Exception\\ParseException`, che indica il tipo
di errore e la riga della stringa YAML originale in cui l'errore si
è verificato:

.. code-block:: php

    use Symfony\Component\Yaml\Exception\ParseException;

    try {
        $value = $yaml->parse(file_get_contents('/percorso/del/file.yml'));
    } catch (ParseException $e) {
        printf("Impossibile analizzare la stringa YAML: %s", $e->getMessage());
    }

.. tip::

    Poiché l'analizzatore è rientrante, si può usare lo stesso oggetto analizzatore
    per caricare diverse stringhe YAML.

Quando si carica un file YAML, a volte è meglio usare il metodo involucro
:method:`Symfony\\Component\\Yaml\\Yaml::parse`:

.. code-block:: php

    use Symfony\Component\Yaml\Yaml;

    $yaml = Yaml::parse('/percorso/del/file.yml');

Il metodo statico :method:`Symfony\\Component\\Yaml\\Yaml::parse` prende una stringa YAML
o un file contenente YAML. Internamente, richiama il metodo
:method:`Symfony\\Component\\Yaml\\Parser::parse`, ma migliora gli errori, nel
caso qualcosa vada storto, aggiungendo il nome del file al messaggio.

Eseguire PHP dentro i file YAML
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

.. versionadded:: 2.1
    Il metodo ``Yaml::enablePhpParsing()`` è nuovo in Symfony 2.1. Prima della 2.1,
    PHP era *sempre* eseguito al richiamo della funzione ``parse()``.

Per impostazione predefinita, se si inserisce codice PHP in un file YAML, non sarà analizzato.
Se si vuole che PHP sia analizzato, occorre richiamare ``Yaml::enablePhpParsing()``
prima dell'analisi del file, per attivare tale modalità. Se si vuole consentire codice
PHP in un singolo file YAML, assicurarsi di disabilitare l'analisi PHP dopo l'analisi
del singolo file, richiamando ``Yaml::$enablePhpParsing = false;`` (``$enablePhpParsing``
è una proprietà pubblica).

Scrivere file YAML
~~~~~~~~~~~~~~~~~~

Il metodo :method:`Symfony\\Component\\Yaml\\Dumper::dump` esporta un array PHP nella
corrispondente rappresentazione YAML:

.. code-block:: php

    use Symfony\Component\Yaml\Dumper;

    $array = array(
        'foo' => 'bar',
        'bar' => array('foo' => 'bar', 'bar' => 'baz')
    );

    $dumper = new Dumper();

    $yaml = $dumper->dump($array);

    file_put_contents('/percorso/del/file.yml', $yaml);

.. note::

    Ovviamente, l'esportatore YAML non è in grado di esportare risorse. Inoltre,
    anche se l'esportatore è in grado di esportare oggetti PHP, la caratteristica
    è considerata come non supportata.

Se si verifica un errore durante l'esportazione, l'esportatore lancia un'eccezione
:class:`Symfony\\Component\\Yaml\\Exception\\DumpException`.

Se si ha bisogno di esportare un solo array, si può usare come scorciatoia il metodo statico
:method:`Symfony\\Component\\Yaml\\Yaml::dump`:

.. code-block:: php

    use Symfony\Component\Yaml\Yaml;

    $yaml = Yaml::dump($array, $inline);

Il formato YAML supporta due tipi di rappresentazioni di array, quello espanso e quello
in linea. Per impostazione predefinita, l'esportatore usa la rappresentazione
in linea:

.. code-block:: yaml

    { foo: bar, bar: { foo: bar, bar: baz } }

Il secondo parametro del metodo :method:`Symfony\\Component\\Yaml\\Dumper::dump`
personalizza il livello in cui l'output cambia dalla rappresentazione espansa a
quella in linea:

.. code-block:: php

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

.. _YAML: http://yaml.org/
.. _Packagist: https://packagist.org/packages/symfony/yaml
