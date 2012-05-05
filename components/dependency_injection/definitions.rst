Lavorare con parametri e definizioni del contenitore
====================================================

Ottenere e impostare parametri del contenitore
----------------------------------------------

Lavorare con i parametri del contenitore è facile, basta usare i metodi di accesso
del contenitore per i parametri. Si può verificare se un parametro sia stato definito
nel contenitore, con::

     $container->hasParameter($name);

Si possono recuperare parametri del contenitore, con::

    $container->getParameter($name);

e impostare parametri nel contenitore, con::

    $container->setParameter($name, $value);

Ottenere e impostare definizioni di un servizio
-----------------------------------------------

Ci sono anche alcuni utili metodi per lavorare con
le definizioni dei servizi.

Per vedere ce ci sia una definizione per l'id di un servizio:: 

    $container->hasDefinition($idServizio);

Questo è utile se si vuole fare qualcosa solo nel caso in cui una data definizione esista.

Si può recuperare una definizione, con::

    $container->getDefinition($idServizio);

o::

    $container->findDefinition($idServizio);

che, diversamente da ``getDefinition()``, risolve anche gli alias: se l'argomento ``$idServizio``
è un alias, si ottiene la definizione sottostante.

Le definizioni stesse dei servizi sono oggetti, per cui se si recupera una definizione con
tali metodi e si eseguono modifiche a essa, tali modifiche si rifletteranno nel
contenitore. Se, tuttavia, si sta creando una nuova definizione, la si può aggiungere
al contenitore, usando::

    $container->setDefinition($id, $definitzione);

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
  
    //--

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

