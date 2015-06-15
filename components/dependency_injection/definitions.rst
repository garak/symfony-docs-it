.. index::
   single: DependencyInjection; Definizione di servizi

Lavorare con parametri e definizioni del contenitore
====================================================

Ottenere e impostare parametri del contenitore
----------------------------------------------

Ci sono alcuni metodi utili per lavorare con le definizioni dei servizi.

Per trovare se ci sia una definizione per un id di un servizio::

    $container->hasDefinition($idServizio);

Questo è utile se si vuole far qualcosa solo se una particolare definizione esiste.

Si possono recuperare una definizione con::

    $container->getDefinition($idServizio);

oppure::

    $container->findDefinition($idServizio);

che, diversamente da ``getDefinition()``, risolve anche gli alias, in modo che, se ``$idServizio``
è un alias, si otterrà la definizione sottostante.

Le definizioni stesse dei servizi sono oggetti, quindi se si recupera una definizione
con tali metodi e li si modifica, tali modifiche si rifletteranno nel
contenitore. Se, tuttavia, si crea una nuova definizione, la si può aggiungere
al contenitore, usando::

    $container->setDefinition($id, $definizione);

Lavorare con una definizione
----------------------------

Creare una nuova definizione
~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Se occorre creare una nuova definizione, piuttosto che manipolarne una recuperata dal
contenitore, alla la classe della definizione è :class:`Symfony\\Component\\DependencyInjection\\Definition`.

Classe
~~~~~~

Quella vista sopra è la classe di una definizione, questa è la classe dell'oggetto
restituito quando si richiede un servizio dal contenitore.

Per trovare quale classe sia impostata per una definizione::

    $definition->getClass();

e per impostare una classe diversa::

    $definition->setClass($classe); // Il nome pienamente qualificato di una classe, come stringa

Parametri del costruttore
~~~~~~~~~~~~~~~~~~~~~~~~~

Per ottenere un array di parametri del costruttore per una definizione, si può usare::

    $definition->getArguments();

oppure si può ottenere un singolo parametro, in base alla sua posizione::

    $definition->getArgument($indice); 
    // p.e. $definition->getArguments(0) per il primo parametro

Si può aggiungere un nuovo parametro alla fine dell'array dei parametri, usando::

    $definition->addArgument($parametro);

Il parametro può essere una stringa, un array, il parametro di un servizio, se si usa
``%paramater_name%``, oppure l'id di un servizio, se si usa::

    use Symfony\Component\DependencyInjection\Reference;

    // ...

    $definition->addArgument(new Reference('id_servizio'));

In modo simile, si può sostituire un parametro già impostato, usando::

    $definition->replaceArgument($indice, $parametro);

Si possono anche sostituire tutti i parametri (o impostarne alcuni, se non ce ne sono) con
un array di parametri::

    $definition->replaceArguments($parametri);

Chiamate a metodi
~~~~~~~~~~~~~~~~~

Se il servizio con cui si sta lavorando usa l'iniezione per setter, si può manipolare
qualsiasi chiamata a metodi nelle definizioni.

Si può ottenere un array di tutte le chiamate a metodi, con::

    $definition->getMethodCalls();

E la chiamata a un metodo, con::

   $definition->addMethodCall($metodo, $parametri);

Dove ``$metodo`` è il nome del metodo e ``$parametri`` è un array dei parametri con
cui richiamare il metodo. I parametri possono essere stringhe, array, parametri o
id di servizi, come per i parametri del costruttore.

Si possono anche sostituire le chiamate a metodi esistenti con un array di nuove, con::

    $definition->setMethodCalls($chiamate);

.. tip::

    Ci sono ulteriori esempi di modi specifici di lavorare con le definizioni
    nei blocchi di codice PHP degli esempi di configurazione nelle pagine come
    :doc:`/components/dependency_injection/factories` e
    :doc:`/components/dependency_injection/parentservices`.

.. note::

    I metodi visti qui che cambiano definizione dei servizi possono essere usati solo
    prima che il contenitore sia compilato: una volta che il contenitore è compilato, non si
    possono manipolare ultetiormente le definizioni dei servizi. Per saperne di più sulla compilazione
    del contenitore, vedere :doc:`/components/dependency_injection/compilation`.

Richiedere file
~~~~~~~~~~~~~~~

Ci sono dei casi d'uso in cui si vuole includere un altro file, appena prima che
il servizio sia caricato. Per poterlo fare, si può usare il metodo
:method:`Symfony\\Component\\DependencyInjection\\Definition::setFile`::

    $definition->setFile('/percorso/del/file/pippo.php');

Si noti che Symfony richiamerà internamente l'istruzione ``require_once`` di PHP,
il che vuol dire che il file sarà incluso una volta sola per richiesta.
