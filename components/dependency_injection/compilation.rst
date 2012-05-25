Compilazione del contenitore
============================

Ci sono diverse ragioni per compilare il contenitore di servizi. Tra queste, poter
verificare potenziali problemi, come i riferimenti circolari, e rendere il contenitore più
efficiente, risolvendo i parametri e rimuovendo i servizi
inutilizzati.

La compilazione avviene eseguendo::

    $container->compile();

Il metodo ``compile`` usa dei **passi di compilatore** per la compilazione. Il componente
*Dependency Injection* dispone di diversi passi, registrati automaticamente per la
compilazione. Per esempio, :class:`Symfony\\Component\\DependencyInjection\\Compiler\\CheckDefinitionValidityPass`
verifica diversi problemi potenziali con le definizioni impostate nel
contenitore. Dopo questo e molti altri passi, che verificano la validità del
contenitore, ulteriori passi sono usati per ottimizzare la configurazione, prima che sia
messa in cache. Per esempio, vengono rimossi i servizi privati e i servizi astratti, e
vengono risolti gli alias.

Creare un passo di compilatore
------------------------------

Si possono anche creare e registrare i propri passi di compilatore con il contenitore.
Per creare un passo di compilatore, si deve implementare
:class:`Symfony\\Component\\DependencyInjection\\Compiler\\CompilerPassInterface`. Il
compilatore offre la possibilità di manipolare le definizioni del servizio che sono state
compilate. Questo può essere molto potente, ma non necessario nell'uso quotidiano.

Il passo di compilatore deve avere il metodo ``process``, che viene passato al contenitore
che si sta compilando::

    public function process(ContainerBuilder $container)
    {
       //--
    }

Si possono manipolare parametri e definizioni del contenitore, usando i metodi descritti
in :doc:`/components/dependency_injection/definitions`. Un cosa che si fa solitamente in
un passo di compilatore è la ricerca di tutti i servizi con determinato tag, in modo
da poterli processare in quealche modo o collegarli dinamicamente in qualche
altro servizio.

Gestire la configurazione con le estensioni
-------------------------------------------

Oltre che caricare la configurazione direttamente nel contenitore, come mostrato in
:doc:`/components/dependency_injection/introduction`, la si può gestire registrando
estensioni con il contenitore. Le estensioni devono implementare :class:`Symfony\\Component\\DependencyInjection\\Extension\\ExtensionInterface` e possono essere registrare con il contenitore usando::

    $container->registerExtension($extension);

Il lavoro principale dell'estensione viene fatto nel metodo ``load``. In tale metodo, si
può caricare la configurazione da uno o più file di configurazione, oltre che manipolare
le definizioni del contenitore, usando i metodi mostrati in :doc:`/components/dependency_injection/definitions`. 

Il metodo ``load`` viene passato a un nuovo contenitore da configurare, poi fuso nel
contenitore con cui è registrato. Ciò consente di avere molte estensioni che gestiscono
contemporaneamente le definizioni del contenitore in modo indipendente.
Le estensioni sono si aggiungono alla configurazione dei contenitore al momento
dell'aggiunta, ma sono processati al richiamo del metodo ``compile`` del
contenitore.

.. note::

    Se si ha bisogno di manipolare la configurazione caricata da un'estensione, non lo si
    può fare da un'altra estensione, perché usa un nuovo contenitore.
    Si deve invece usare un passo di compilatore che funziona con l'intero contenitore,
    dopo che le estensioni sono state processate.

