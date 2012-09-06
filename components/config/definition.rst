.. index::
   single: Config; Definire e processare valori di configurazione

Definire e processare valori di configurazione
==============================================

Validare i valori di configurazione
-----------------------------------

Dopo aver caricato i valori di configurazione da ogni tipo di risorsa, i valori e
le loro strutture possono essere validati, usando la parte "Definition" del componente
Config. Solitamente ci si aspetta che i valori di configurazione mostrino un qualche tipo
di gerarchia. Inoltre, i valori dovrebbero essere di un certo tipo, ristretti in numero
o all'interno di un determinato insieme di valori. Per esempio, la configurazione seguente
(in Yaml) mostra una chiara gerarchia e alcune regole di validazione che vi andrebbero
applicate (come: "il valore per ``auto_connect`` deve essere booleano"):

.. code-block:: yaml

    auto_connect: true
    default_connection: mysql
    connections:
        mysql:
            host: localhost
            driver: mysql
            username: user
            password: pass
        sqlite:
            host: localhost
            driver: sqlite
            memory: true
            username: user
            password: pass

Quando si caricano diversi file di configurazione, dovrebbe essere possibile
fondere e sovrascrivere alcuni valori. Gli altri valori non vanno fusi e devono
rimanere come prima. Inoltre, alcune chiavi sono disponibili solo quando un
altra chiave ha uno specifico valore (nell'esempio precedente: la chiave
``memory`` ha senso solo quando ``driver`` è ``sqlite``).

Definire una gerarchia di valori di configurazione con TreeBuilder
------------------------------------------------------------------

Tutte le regole relative ai valori di configurazione possono essere definite tramite
:class:`Symfony\\Component\\Config\\Definition\\Builder\\TreeBuilder`.

Un'istanza di :class:`Symfony\\Component\\Config\\Definition\\Builder\\TreeBuilder`
va restituita da una classe personalizzata ``Configuration``, che implementa
:class:`Symfony\\Component\\Config\\Definition\\ConfigurationInterface`::

    namespace Acme\DatabaseConfiguration;

    use Symfony\Component\Config\Definition\ConfigurationInterface;
    use Symfony\Component\Config\Definition\Builder\TreeBuilder;

    class DatabaseConfiguration implements ConfigurationInterface
    {
        public function getConfigTreeBuilder()
        {
            $treeBuilder = new TreeBuilder();
            $rootNode = $treeBuilder->root('database');

            // ... aggiungere definizioni di nodi alla radice dell'albero

            return $treeBuilder;
        }
    }

Aggiungere definizioni di nodi all'albero
-----------------------------------------

Nodi variabili
~~~~~~~~~~~~~~

Un albero contiene definizioni di nodi, che possono essere stratificati in modo semantico.
Questo vuol dire che, usando l'indentazione e la notazione fluida, è possibile
riflettere la reale struttura dei valori di configurazione::

    $rootNode
        ->children()
            ->booleanNode('auto_connect')
                ->defaultTrue()
            ->end()
            ->scalarNode('default_connection')
                ->defaultValue('default')
            ->end()
        ->end()
    ;

Lo stesso nodo radice è un nodo array e ha dei figli, come il nodo booleano
``auto_connect`` e il nodo scalare ``default_connection``. In generale:
dopo aver definito un nodo, una chiamata ``end()`` porta un gradino in alto nella gerarchia.

Nodi array
~~~~~~~~~~

Si può aggiungere un livello ulteriore alla gerarchia, aggiungendo un nodo
array. Il nodo array stesso potrebbe avere un insieme predefinito di nodi variabili:

.. code-block:: php

    $rootNode
        ->arrayNode('connection')
            ->scalarNode('driver')->end()
            ->scalarNode('host')->end()
            ->scalarNode('username')->end()
            ->scalarNode('password')->end()
        ->end()
    ;

Oppure si può definire un prototipo per ogni nodo dentro un nodo array:

.. code-block:: php

    $rootNode
        ->arrayNode('connections')
            ->prototype('array')
                ->children()
                    ->scalarNode('driver')->end()
                    ->scalarNode('host')->end()
                    ->scalarNode('username')->end()
                    ->scalarNode('password')->end()
                ->end()
            ->end()
        ->end()
    ;

Si può usare un prototipo per aggiungere una definizione, che potrebbe essere ripetuta
molte volte dentro il nodo corrente. In base alla definizione del prototipo nell'esempio
precedente, è possibile avere molte array di connessione (contenenti ``driver``,
``host``, ecc.).

Opzioni dei nodi array
~~~~~~~~~~~~~~~~~~~~~~

Prima di definire i figli di un nodo array, si possono fornire opzioni, come:

``useAttributeAsKey()``
    Fornisce il nome di un nodo figlio, i cui valori sono usati come chiavi nell'array risultante
``requiresAtLeastOneElement()``
    Dovrebbe esserci almeno un elemento nell'array (funziona solo se viene richiamato anche
    ``isRequired()``).

Un esempio:

.. code-block:: php

    $rootNode
        ->arrayNode('parameters')
            ->isRequired()
            ->requiresAtLeastOneElement()
            ->prototype('array')
                ->useAttributeAsKey('name')
                ->children()
                    ->scalarNode('name')->isRequired()->end()
                    ->scalarNode('value')->isRequired()->end()
                ->end()
            ->end()
        ->end()
    ;

Valori predefiniti e obbligatori
--------------------------------

Per tutti i tipi di nodo, è possibile definire valori predefiniti e valori di
rimpiazzo nel caso in cui un nodo
abbia un determinato valore:

``defaultValue()``
    Imposta un valore predefinito
``isRequired()``
    Deve essere definito (ma può essere vuoto)
``cannotBeEmpty()``
    Non può contenere un valore vuoto
``default*()``
    (``null``, ``true``, ``false``), scorciatoia per ``defaultValue()``
``treat*Like()``
    (``null``, ``true``, ``false``), fornisce un valore di rimpiazzo in caso in cui il valore sia ``*.``

.. code-block:: php

    $rootNode
        ->arrayNode('connection')
            ->children()
                ->scalarNode('driver')
                    ->isRequired()
                    ->cannotBeEmpty()
                ->end()
                ->scalarNode('host')
                    ->defaultValue('localhost')
                ->end()
                ->scalarNode('username')->end()
                ->scalarNode('password')->end()
                ->booleanNode('memory')
                    ->defaultFalse()
                ->end()
            ->end()
        ->end()
    ;

Opzioni di fusione
------------------

Si possono fornire opzioni aggiuntive sul processo di fusione. Per gli array:

``performNoDeepMerging()``
    Quando il valore è definito anche in un altro array di configurazione, non
    provare a fondere un array, ma sovrascrivilo completamente

Per tutti i nodi:

``cannotBeOverwritten()``
    non consentire che altri array di configurazione sovrascrivano il valore di questo nodo

Regole di validazione
---------------------

Si possono fornire regole di validazione avanzata, usando
:class:`Symfony\\Component\\Config\\Definition\\Builder\\ExprBuilder`. Questa classe
implementa un'interfaccia fluida per una struttura di controllo nota.
Si può usare per aggiungere regole di validazione avanzate alle definizioni dei nodi, come::

    $rootNode
        ->arrayNode('connection')
            ->children()
                ->scalarNode('driver')
                    ->isRequired()
                    ->validate()
                        ->ifNotInArray(array('mysql', 'sqlite', 'mssql'))
                        ->thenInvalid('Invalid database driver "%s"')
                    ->end()
                ->end()
            ->end()
        ->end()
    ;

Una regola di validazione ha sempre una parte "if". Si può specificare tale parte
nel modo seguente:

- ``ifTrue()``
- ``ifString()``
- ``ifNull()``
- ``ifArray()``
- ``ifInArray()``
- ``ifNotInArray()``
- ``always()``

Una regola di validazione richiede anche una parte "then":

- ``then()``
- ``thenEmptyArray()``
- ``thenInvalid()``
- ``thenUnset()``

Di solito, "then" è una closure. Il suo valore di ritorno sarà usato come nuovo valore
del nodo, al posto del valore
originale del nodo.

Processare i valori di configurazione
-------------------------------------

La classe :class:`Symfony\\Component\\Config\\Definition\\Processor` usa l'albero,
costruito usando :class:`Symfony\\Component\\Config\\Definition\\Builder\\TreeBuilder`,
per processare molteplici array di valori di configurazione da fondere.
Se un valore non è del tipo atteso, è obbligatorio e non ancora definito oppure non può
essere validato in altri modi, sarà lanciata un'eccezione.
Altrimenti, il risultato è un array pulito di valori di configurazione::

    use Symfony\Component\Yaml\Yaml;
    use Symfony\Component\Config\Definition\Processor;
    use Acme\DatabaseConfiguration;

    $config1 = Yaml::parse(__DIR__.'/src/Matthias/config/config.yml');
    $config2 = Yaml::parse(__DIR__.'/src/Matthias/config/config_extra.yml');

    $configs = array($config1, $config2);

    $processor = new Processor();
    $configuration = new DatabaseConfiguration;
    $processedConfiguration = $processor->processConfiguration($configuration, $configs);
