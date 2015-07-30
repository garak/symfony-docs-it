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
            host:     localhost
            driver:   mysql
            username: utente
            password: pass
        sqlite:
            host:     localhost
            driver:   sqlite
            memory:   true
            username: utente
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
            ->booleanNode('auto_connessione')
                ->defaultTrue()
            ->end()
            ->scalarNode('connessione_predefinita')
                ->defaultValue('predefinito')
            ->end()
        ->end()
    ;

Lo stesso nodo radice è un nodo array e ha dei figli, come il nodo booleano
``auto_connect`` e il nodo scalare ``default_connection``. In generale:
dopo aver definito un nodo, una chiamata ``end()`` porta un gradino in alto nella gerarchia.

Tipo di nodo
~~~~~~~~~~~~

Si può validare il tipo di un valore fornito, usando l'appropriata definizione
di nodo. I tipi di nodo disponibili sono:



* scalare (tipo generico che include booleani, stringhe, interi, virgola mobile e ``null``)
* booleano
* intero (nuovo in 2.2)
* virgola mobile (nuovo in 2.2)
* enum (nuovo in 2.1) (simile a scalare, ma consente solo un insieme determinato di valori)
* array
* variabile (nessuna validazione)

e sono creati con ``node($nome, $tipo)`` o con i relativi metodi scorciatoia
``xxxxNode($nome)``.

Nodi di vincoli numerici
~~~~~~~~~~~~~~~~~~~~~~~~

.. versionadded:: 2.2
    I nodi numerici (virgola mobile e intero) sono nuovi in 2.2

I nodi numerici (virgola mobile e intero) foniscono due vincoli extra,
:method:`Symfony\\Component\\Config\\Definition\\Builder::min` e
:method:`Symfony\\Component\\Config\\Definition\\Builder::max`,
che consentono di  validare il valore::

    $rootNode
        ->children()
            ->integerNode('valore_positivo')
                ->min(0)
            ->end()
            ->floatNode('valore_grosso')
                ->max(5E45)
            ->end()
            ->integerNode('valore_tra_estremi')
                ->min(-50)->max(50)
            ->end()
        ->end()
    ;

Nodi enum
~~~~~~~~~~

.. versionadded:: 2.1
    Il nodo enum è nuovo in Symfony 2.1

I nodi enum forniscono un vincolo che fa corrispondere il dato inserito a una
serie di valori::

    $rootNode
        ->children()
            ->enumNode('genere')
                ->values(array('maschio', 'femmina'))
            ->end()
        ->end()
    ;

Questo restringe l'opzione ``genere`` ai valori ``maschio`` o ``femmina``.

Nodi array
~~~~~~~~~~

Si può aggiungere un livello ulteriore alla gerarchia, aggiungendo un nodo
array. Il nodo array stesso potrebbe avere un insieme predefinito di nodi variabili::

    $rootNode
        ->children()
            ->arrayNode('connection')
                ->children()
                    ->scalarNode('driver')->end()
                    ->scalarNode('host')->end()
                    ->scalarNode('utente')->end()
                    ->scalarNode('password')->end()
                ->end()
            ->end()
        ->end()
    ;

Oppure si può definire un prototipo per ogni nodo dentro un nodo array::

    $rootNode
        ->children()
            ->arrayNode('connections')
                ->prototype('array')
                    ->children()
                        ->scalarNode('driver')->end()
                        ->scalarNode('host')->end()
                        ->scalarNode('utente')->end()
                        ->scalarNode('password')->end()
                    ->end()
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
``addDefaultsIfNotSet()``
    Se dei nodi figli hanno valori predefiniti, usarli se non sono stati forniti dati espliciti.

Un esempio::

    $rootNode
        ->children()
            ->arrayNode('parameters')
                ->isRequired()
                ->requiresAtLeastOneElement()
                ->useAttributeAsKey('nome')
                ->prototype('array')
                    ->children()
                        ->scalarNode('valore')->isRequired()->end()
                    ->end()
                ->end()
            ->end()
        ->end()
    ;

In YAML, la configurazione potrebbe essere come questa:

.. code-block:: yaml

    database:
        parameters:
            param1: { valore: param1val }

In XML, ciascun nodo ``parameters`` avrebbe un attributo ``name`` (insieme a
``value``), che sarebbe rimosso e usato come chiave per tale elemento nell'array
finale. L'opzione ``useAttributeAsKey`` è utile per normalizzare il modo in cui gli
array sono specificati tra formati diversi, come XML e YAML.

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
        ->children()
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
            ->arrayNode('settings')
                ->addDefaultsIfNotSet()
                ->children()
                    ->scalarNode('nome')
                        ->isRequired()
                        ->cannotBeEmpty()
                        ->defaultValue('valore')
                    ->end()
                ->end()
            ->end()
        ->end()
    ;

Documentare un'opzione
----------------------

Si possono documentare le opzioni, usando il metodo
:method:`Symfony\\Component\\Config\\Definition\\Builder\\NodeDefinition::info`.


L'informazione sarà mostrata come commento, nell'albero della configurazione, con il
comando ``config:dump``.

Sezioni facoltative
-------------------

.. versionadded:: 2.2
    I metodi ``canBeEnabled`` e ``canBeDisabled`` sono nuovi in
    Symfony 2.2.

Se si hanno intere sezioni facoltative e che possono essere abilitate/disabilitate,
si possono sfruttare le scorciatoie
:method:`Symfony\\Component\\Config\\Definition\\Builder\\ArrayNodeDefinition::canBeEnabled` e
:method:`Symfony\\Component\\Config\\Definition\\Builder\\ArrayNodeDefinition::canBeDisabled`::

    $arrayNode
        ->canBeEnabled()
    ;

    // è equivalente a

    $arrayNode
        ->treatFalseLike(array('enabled' => false))
        ->treatTrueLike(array('enabled' => true))
        ->treatNullLike(array('enabled' => true))
        ->children()
            ->booleanNode('enabled')
                ->defaultFalse()
    ;

Il metodo ``canBeDisabled`` è uguale, tranne per il fatto che la sezione
viene abilitata in modo predefinito.

Opzioni di fusione
------------------

Si possono fornire opzioni aggiuntive sul processo di fusione. Per gli array:

``performNoDeepMerging()``
    Quando il valore è definito anche in un altro array di configurazione, non
    provare a fondere un array, ma sovrascrivilo completamente

Per tutti i nodi:

``cannotBeOverwritten()``
    non consentire che altri array di configurazione sovrascrivano il valore di questo nodo

Aggiunta di sezioni
-------------------

Se occorre validare una configurazione complessa, l'albero potrebbe diventare
troppo grande, si potrebbe quindi volerlo separare in sezioni. Lo si può fare
creando una sezione come nodo separato e quindi aggiungendola all'albero
principale con ``append()``::

    public function getConfigTreeBuilder()
    {
        $treeBuilder = new TreeBuilder();
        $rootNode = $treeBuilder->root('database');

        $rootNode
            ->children()
                ->arrayNode('connection')
                    ->children()
                        ->scalarNode('driver')
                            ->isRequired()
                            ->cannotBeEmpty()
                        ->end()
                        ->scalarNode('host')
                            ->defaultValue('localhost')
                        ->end()
                        ->scalarNode('utente')->end()
                        ->scalarNode('password')->end()
                        ->booleanNode('memory')
                            ->defaultFalse()
                        ->end()
                    ->end()
                    ->append($this->addParametersNode())
                ->end()
            ->end()
        ;

        return $treeBuilder;
    }

    public function addParametersNode()
    {
        $builder = new TreeBuilder();
        $node = $builder->root('parameters');

        $node
            ->isRequired()
            ->requiresAtLeastOneElement()
            ->useAttributeAsKey('nome')
            ->prototype('array')
                ->children()
                    ->scalarNode('valore')->isRequired()->end()
                ->end()
            ->end()
        ;

        return $node;
    }

Questo è utile per evitare di ripetersi, nel caso in cui si abbiano sezioni
della configurazione ripetute in posti diversi.

.. _component-config-normalization:

Normalizzazione
---------------

Prima di essere processati, i file di configurazione vengono normalizzati, quindi fusi
e infine si usa l'albero per validare l'array risultante. Il processo di
normalizzazione si usa per rimuovere alcune differenze risultati dai vari formati
di configurazione, soprattutto tra Yaml e XML.

Il separatore usato nelle chiavi è tipicamente ``_`` in Yaml e ``-`` in XML. Per
esempio, ``auto_connect`` in Yaml e ``auto-connect``. La normalizzazione rende
entrambi ``auto_connect``.

.. caution::

    La chiave interessata non sarà alterata se è mista, come
    ``pippo-pluto_muu``, o se esiste già.

Un'altra differenza tra Yaml e XML è il modo in cui sono rappresentati array
di dati. In Yaml si può avere:

.. code-block:: yaml

    twig:
        extensions: ['twig.extension.pippo', 'twig.extension.pluto']

e in XML:

.. code-block:: xml

    <twig:config>
        <twig:extension>twig.extension.pippo</twig:extension>
        <twig:extension>twig.extension.pluto</twig:extension>
    </twig:config>

La normalizzazione rimuove tale differenza, pluralizzando la chiave usata
in XML. Si può specificare se si vuole una chiave pluralizzata in tal modo con
``fixXmlConfig()``::

    $rootNode
        ->fixXmlConfig('extension')
        ->children()
            ->arrayNode('extensions')
                ->prototype('scalar')->end()
            ->end()
        ->end()
    ;

Se la pluralizzazione è irregolare, si può specificare il plurale da usare,
come secondo parametro::

    $rootNode
        ->fixXmlConfig('uovo', 'uova')
        ->children()
            ->arrayNode('uova')
                // ...
            ->end()
        ->end()
    ;

Oltre a sistemare queste cose, ``fixXmlConfig`` si assicura che i singoli elementi xml
siano modificati in array. Quindi si potrebbe avere:

.. code-block:: xml

    <connessione>predefinito</connessione>
    <connessione>extra</connessione>

e a volte solo:

.. code-block:: xml

    <connessione>default</connessione>

Per impostazione predefinita, ``connessione`` sarebbe un array nel primo caso e una stringa
nel secondo, rendendo difficile la validazione. Ci si può assicurare che sia sempre
un array con ``fixXmlConfig``.

Se necessario, si può controllare ulteriormente il processo di normalizzazione. Per esempio,
si potrebbe voler consentire che una stringa sia impostata e usata come chiave particolare o che
che molte chiavi siano impostate in modo esplicito. Quindi, se tutto tranne id è facoltativo,
in questa configurazione:

.. code-block:: yaml

    connessione:
        name:     connessione_mysql
        host:     localhost
        driver:   mysql
        username: utente
        password: pass

si può consentire anche il seguente:

.. code-block:: yaml

    connection: my_mysql_connection

Cambiando un valore stringa in un array associativo con ``name`` come chiave::

    $rootNode
        ->children()
            ->arrayNode('connessione')
                ->beforeNormalization()
                    ->ifString()
                    ->then(function($v) { return array('name'=> $v); })
                ->end()
                ->children()
                    ->scalarNode('name')->isRequired()
                    // ...
                ->end()
            ->end()
        ->end()
    ;

Regole di validazione
---------------------

Si possono fornire regole di validazione avanzata, usando
:class:`Symfony\\Component\\Config\\Definition\\Builder\\ExprBuilder`. Questa classe
implementa un'interfaccia fluida per una struttura di controllo nota.
Si può usare per aggiungere regole di validazione avanzate alle definizioni dei nodi, come::

    $rootNode
        ->children()
            ->arrayNode('connessione')
                ->children()
                    ->scalarNode('driver')
                        ->isRequired()
                        ->validate()
                        ->ifNotInArray(array('mysql', 'sqlite', 'mssql'))
                            ->thenInvalid('Valore non valido "%s"')
                        ->end()
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

    $config1 = Yaml::parse(file_get_contents(__DIR__.'/src/Matthias/config/config.yml'));
    $config2 = Yaml::parse(file_get_contents(__DIR__.'/src/Matthias/config/config_extra.yml'));

    $configs = array($config1, $config2);

    $processor = new Processor();
    $configuration = new DatabaseConfiguration();
    $processedConfiguration = $processor->processConfiguration(
        $configuration,
        $configs
    );
