.. index::
   single: Controllore; Come servizi

Definire i controllori come servizi
===================================

Nel libro, abbiamo imparato quanto è facile usare un controllore quando estende la
classe base :class:`Symfony\\Bundle\\FrameworkBundle\\Controller\\Controller`. Oltre
a questo metodo, i controllori possono anche essere specificati come
servizi.

Per fare rifermento a un controllore definito come servizio, usare la notazione con
un solo "due punti" (:). Per esempio, si supponga di aver definito un servizio chiamato
``mio controllore`` e che si voglia rimandare a un metodo chiamato ``indexAction()``
all'interno di tale servizio::

    $this->forward('mio_controllore:indexAction', array('pippo' => $pluto));

Occorre usare la stessa notazione, quando si definisce il valore ``_controller``
della rotta:

.. code-block:: yaml

    mio_controllore:
        pattern:   /
        defaults:  { _controller: mio_controllore:indexAction }

Per usare un controllore in questo modo, deve essere definito nella configurazione del
contenitore di servizi. Per ulteriori informazioni, si veda il capitolo
:doc:`Contenitore di servizi</book/service_container>`.

Quando si usa un controllore definito come servizio, esso probabilmente non estenderà
la classe base ``Controller``. Invece di appoggiarsi ai metodi scorciatoia di tale classe,
si interagirà direttamente coi servizi necessari. Fortunatamente, questo è di solito
abbastanza facile e la classe ``Controller`` è una grande risorsa per sapere come
eseguire i compiti più comuni.

.. note::

    Specificare un controllore come servizio richiede un po' più di lavoro. Il vantaggio
    principale è che l'intero controllore o qualsiasi servizio passato al controllore
    possono essere modificati tramite la configurazione del contenitore di servizi.
    Questo è particolarmente utile quando si sviluppa un bundle open source o un bundle
    che sarà usato in progetti diversi. Quindi, anche non specificando i propri
    controllori come servizi, probabilmente si vedrà questo aspetto in diversi bundle
    open source di Symfony2.
