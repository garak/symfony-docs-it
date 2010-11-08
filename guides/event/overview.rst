.. index::
   single: Event Dispatcher

Event Dispatcher
================

L'Object Oriented ha fatto molta strada per assicurare l'estensibilità del 
codice. Creando classi che hanno responsabilità ben definite, il codice 
diventa più flessibile dato che uno sviluppatore può estenderle con delle
sotto-classi per modificarne il comportamento. Ma se volesse condividere 
le proprie modifiche con altri sviluppatori che a loro volta hanno sviluppato 
le loro sotto-classi l'ereditarietà del codice sarebbe discutibile.

Un esempio dal mondo reale è rappresentato dal voler offrire un sistema di
plugin per un'applicazione. Un plugin dovrebbe essere in grado di aggiungere
metodi, o di fare qualcosa prima o dopo l'esecuzione di un certo metodo,
senza interferire con gli altri plugin. Questo non è un problema risolvibile
in modo semplice con l'ereditarietà singola e l'ereditarietà multipla ha
i suoi svantaggi.

L'Event Dispatcher di Symfony2 implementa il pattern `Observer`_ in maniera
semplice ed efficace per rendere tutte queste cose possibili e per rendere
le applicazioni realmente estensibili (vedere la sezione :doc:`recipes`
per alcuni esempi sull'implementazione).

L'Event Dispatcher offre un *dispatcher* che permette agli oggetti di 
comunicare tra di loro senza conoscersi. Gli oggetti (*ascoltatori*) possono
*connettersi* al dispatcher per ascoltare eventi specifici mentre altri 
possono *notificare* un *evento* al dispatcher. Ogni volta che un evento
viene notificato il dispatcher richiama tutti gli ascoltatori collegati.

.. index::
   pair: Event Dispatcher; Convenzioni di nomenclatura

Eventi
------

Diversamente da molte altre implementazioni del pattern Observer, non è
necessario creare una classe per definire un evento. Tutti gli eventi sono
invece istanze della classe ``Event`` e sono identificati in modo univoco
dai loro nomi, una stringa che opzionalmente segue semplici convenzioni
di nomenclatura:

* usare solo lettere minuscole, numeri e underscore (``_``);

* aggiungere come prefisso ai nomi un namespace seguito da un punto (``.``);

* usare un verbo per indicare quale azione verrà eseguita.

Di seguito alcuni buoni esempi di nomi per gli eventi:

* user.change_culture
* response.filter_content

.. note::
   Chiaramente è possibile estendere la classe ``Event`` per specializzare
   ulteriormente un evento, rinforzarne dei vincoli, ma molto spesso questo
   aggiunge un inutile livello di complessità.

Oltre al nome l'istanza di un ``Event`` può memorizzare informazioni aggiuntive
relative all'evento da notificare:

* Il *soggetto* dell'evento (la maggior parte delle volte è l'oggetto che
  notifica l'evento, ma può trattarsi di qualsiasi altro oggetto o di un ``null``);

* Il nome dell'evento;

* Un array di parametri da passare agli ascoltatori (un array vuoto di default).

Questi dati vengono passati come argomenti al costruttore della classe ``Event``::

    use Symfony\Component\EventDispatcher\Event;

    $event = new Event($this, 'user.change_culture', array('culture' => $culture));

L'oggetto dell'evento offre molti metodi per recuperare le informazioni relative 
all'evento:

* ``getName()``: Restituisce il nome dell'evento;

* ``getSubject()``: Restituisce il soggetto dell'oggetto associato all'evento;

* ``getParameters()``: Restituisce i parametri dell'evento.

All'oggetto dell'evento si può accedere come si farebbe con un array per
leggerne i valori dei parametri::

    echo $event['culture'];

Il Dispatcher
-------------

Il dispatcher mantiene un registro degli ascoltatori e li chiama ogni volta
che un evento viene notificato::

    use Symfony\Component\EventDispatcher\EventDispatcher;

    $dispatcher = new EventDispatcher();

.. index::
   single: Event Dispatcher; Ascoltatori

Connettere gli Ascoltatori
--------------------------

Ovviamente è necessario connettere alcuni ascoltatori al dispatcher perché
questo si renda utile. Una chiamata al metodo ``connect()`` del dispatcher
associa un PHP callable ad un evento::

    $dispatcher->connect('user.change_culture', $callable);

Il metodo ``connect()`` accetta due argomenti:

* Il nome dell'evento;

* Un PHP callable da chiamare alla notifica dell'evento.

.. note::
   Un `PHP callable`_ è una variabile PHP che può essere usata dalla funzione
   ``call_user_func()`` e che restituisce ``true`` quando passata alla funzione
   ``is_callable()``. Può trattarsi dell'istanza di una ``\Closure``, una stringa
   rappresentante una funzione, o di un array rappresentante un metodo di un oggetto
   o un metodo di classe.

Una volta che un ascoltatore è registrato al dispatcher, esso resta in attesa
fino a che l'evento viene notificato. Per l'esempio precedente il dispatcher
chiama ``$callable`` ogni volta che l'evento ``user.change_culture`` viene
notificato; l'ascoltatore riceve un'istanza di ``Event`` come argomento.

.. note::
   Gli ascoltatori vengono chiamati dall'event dispatcher nello stesso ordine
   con cui sono connessi ad esso.

.. tip::
   Se si utilizza il framework MVC Symfony2 gli ascoltatori vengono registrati
   automaticamente in base al :ref:`configuration <kernel_listener_tag>`.

.. index::
   single: Event Dispatcher; Notifica

Notificare Eventi
-----------------

Gli eventi possono essere notificati utilizzando tre metodi:

* ``notify()``

* ``notifyUntil()``

* ``filter()``

``notify``
~~~~~~~~~~

Il metodo ``notify()`` notifica tutti gli ascoltatori a turno::

    $dispatcher->notify($event);

Utilizzando il metodo ``notify()``, ci si assicura che tutti gli ascoltatori
registrati per l'evento vengano eseguiti ignorando i valori di ritorno.

``notifyUntil``
~~~~~~~~~~~~~~~

In alcuni casi è necessario permettere ad un ascoltatore di interrompere
l'evento impedendo così di notificare agli altri ascoltatori l'evento stesso.
In questo caso si deve utilizzare ``notifyUntil()`` invece di ``notify()``.
Il dispatcher eseguirà quindi tutti gli ascoltatori fino a che uno restituisce
``true`` arrestando la notifica dell'evento::

    $dispatcher->notifyUntil($event);

L'ascoltatore che interrompe la catena deve inoltre chiamare il metodo
``setReturnValue()`` per restituire alcuni valori al soggetto::

    $event->setReturnValue('foo');

    return true;

Il notificatore può verificare se un ascoltatore ha processato l'evento
invocando il metodo ``isProcessed()``::

    if ($event->isProcessed()) {
        $ret = $event->getReturnValue();

        // ...
    }

``filter``
~~~~~~~~~~

Il metodo ``filter()`` chiede a tutti gli ascoltatori di filtrare un dato valore
passato dal notificatore come suo secondo argomento e recuperato dal callable
dell'ascoltatore come secondo argomenti::

    $dispatcher->filter($event, $response->getContent());

    $listener = function (Event $event, $content)
    {
        // fare qualcosa con $content

        // non dimenticarsi di restituire il contenuto
        return $content;
    };

A tutti gli ascoltatori viene passato il valore che devono restituire filtrato,
indipendentemente dal fatto che lo abbiano modificato o meno. L'esecuzione di
tutti gli ascoltatori è garantita.

Il notificatore può ottenere il valore filtrato invocando il metodo 
`getReturnValue()``::

    $ret = $event->getReturnValue();

.. _Observer:     http://en.wikipedia.org/wiki/Observer_pattern
.. _PHP callable: http://www.php.net/manual/en/function.is-callable.php
