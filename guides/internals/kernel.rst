.. index::
   single: Interno; Nucleo

Il nucleo
=========

:class:`Symfony\\Component\\HttpKernel\\HttpKernel` è la classe 
responsabile della gestione delle richieste del client. Il suo scopo
principale è di "convertire" gli oggetti
:class:`Symfony\\Component\\HttpFoundation\\Request` in oggetti
:class:`Symfony\\Component\\HttpFoundation\\Response`.

Tutti i nuclei di Symfony2 implementano
:class:`Symfony\\Component\\HttpKernel\\HttpKernelInterface`::

    function handle(Request $request, $type = self::MASTER_REQUEST, $catch = true)

.. index::
   single: Interno; Risolutore del controllore

Controllori
-----------

Per convertire una richiesta in una risposta, il nucleo si appoggia a un
"controllore", che può essere qualsiasi funzione o metodo PHP.

Il nucleo delega la selezione del controllore da eseguire a
un'implementazione di
:class:`Symfony\\Component\\HttpKernel\\Controller\\ControllerResolverInterface`::

    public function getController(Request $request);

    public function getArguments(Request $request, $controller);

Il metodo
:method:`Symfony\\Component\\HttpKernel\\Controller\\ControllerResolverInterface::getController`
restituisce il controllore (un callable PHP) associato con la richiesta fornita.
L'implementazione predefinita
(:class:`Symfony\\Component\\HttpKernel\\Controller\\ControllerResolver`)
cerca un attributo della richiesta ``_controller``, che rappresenta il nome
del controllore (una stringa "classe::metodo", come
``Bundle\BlogBundle\PostController:indexAction``).

.. tip::

    L'implementazione predefinita usa
    :class:`Symfony\\Bundle\\FrameworkBundle\\RequestListener` per
    definire l'attributo ``_controller`` della richiesta (vedere sotto).

Il metodo
:method:`Symfony\\Component\\HttpKernel\\Controller\\ControllerResolverInterface::getArguments`
restituisce un array di parametri da passare al callable controllore.
L'implementazione predefinita risolve automaticamente i parametri del metodo,
in base agli attributi della richiesta.

.. sidebar:: Parametri del metodo del controllore corrispondenti agli attributi della richiesta

    Per ogni parametro del metodo, Symfony2 prova a prendere il valore di un
    attributo della richiesta con lo stesso nome. Se non definito, viene usato
    il valore del parametro predefinito, se presente::

        // Symfony2 cercherà un attributo 'id' (obbligatorio)
        // e uno 'admin'(opzionale)
        public function showAction($id, $admin = true)
        {
            // ...
        }

.. index::
  single: Interno; Gestione della richiesta

Gestione delle richieste
------------------------

Il metodo ``handle()`` prende una richiesta e restituisce *sempre* una
risposta. Per convertire la richiesta, ``handle()`` si appoggia sul
risolutore e su una catena ordinata di notifiche di eventi (vedere la
prossima sezione per approfondire tutti gli eventi):

1. Prima di ogni altra cosa, viene notificato l'evento ``core.request``,
   se uno degli ascoltatori restituisce una risposta, salta direttamente
   al passo 8;

2. Viene richiamato il risolutore, per determinare il controllore da
   eseguire;

3. Gli ascoltatori dell'evento ``core.controller`` possono ora manipolare
   il callable controllore nel modo che preferiscono (cambiarlo, farne un
   wrap, ecc.);

4. Il nucleo verifica che il controllore sia effettivamente un callable
   PHP valido;

5. Viene richiamato il risolutore, per determinare i parametri da passare
   al controllore;

6. Il nucleo richiama il controllore;

7. Gli ascoltatori dell'evento ``core.view`` possono cambiare il valore
   restituito dal controllore (per esempio, per convertirlo in una risposta);

8. Gli ascoltatori dell'evento ``core.response`` possono manipolare la
   risposta (contenuto e header);

9. Viene restituita la risposta.

Se durante il processo viene sollevata un'eccezione, viene notificato
l'evento ``core.exception`` e gli ascoltatori hanno la possibilità di
convertire l'eccezione in una risposta. Se funziona, viene notificato
l'evento ``core.response``, altrimenti l'eccezione viene sollevata
nuovamente.

Se non si vuole che l'eccezione sia catturata (per esempio per delle richieste
incluse), disabilitare l'evento ``core.exception``, passando ``true`` come
terzo parametro del metodo ``handle()``.

.. index::
  single: Interno; Richieste interne

Richieste interne
-----------------

In qualsiasi momento durante la gestione di una richiesta (quella "principale"),
si può gestire una sotto-richiesta. Si può passare il tipo di richiesta al
metodo ``handle()`` (il suo secondo parametro):

* ``HttpKernelInterface::MASTER_REQUEST``;
* ``HttpKernelInterface::SUB_REQUEST``.

Il tipo viene passato a tutti gli eventi e gli ascoltatori possono regolarsi
di conseguenza (alcuni processi possono avvenire solo nella richiesta
principale).

.. index::
   pair: Nucleo; Evento

Eventi
------

Tutti gli eventi hanno un parametro ``request_type``, che consente agli
ascoltatori di conoscere il tipo di richiesta. Per esempio, se un
ascoltatore deve essere attivo solo per la richiesta principale, aggiungere
il codice seguente all'inizio del metodo ascoltatore::

    if (HttpKernelInterface::MASTER_REQUEST !== $event->get('request_type')) {
        // esce immediatamente
        // se l'evento è un filtro, restituisce invece il valore filtrato
        return;
    }

.. tip::

    Se non si ha ancora familiarità con il risolutore di eventi di Symfony2,
    leggere prima il :doc:`relativo capitolo </guides/event/overview>`.

.. index::
   single: Evento; core.request

L'evento ``core.request``
~~~~~~~~~~~~~~~~~~~~~~~~~

*Ttipo*: ``notifyUntil()``

*Parametri*: ``request_type`` e ``request``

Essendo l'evento notificato col metodo ``notifyUntil()``, se un ascoltatore
restituisce un oggetto risposta, gli altri ascoltatori non saranno chiamati.

Questo evento è usato da ``FrameworkBundle`` per popolare l'attributo
``_controller`` della richiesta, tramite 
:class:`Symfony\\Bundle\\FrameworkBundle\\RequestListener`. RequestListener
usa un oggetto :class:`Symfony\\Component\\Routing\\RouterInterface` per far
corrispondere la richiesta e determinare il nome del controllore (memorizzato
nell'attributo  ``_controller`` della richiesta).

.. index::
   single: Evento; core.controller

L'evento ``core.controller``
~~~~~~~~~~~~~~~~~~~~~~~~~~~~

*Tipo*: ``filter``

*Parametri*: ``request_type`` e ``request``

*Valore da filtrare*: Il valore del controllore

Questo evento non è usato da ``FrameworkBundle``.

.. index::
   single: Evento; core.view

L'evento ``core.view``
~~~~~~~~~~~~~~~~~~~~~~

*Tipo*: ``filter``

*Parametri*: ``request_type`` and ``request``

*Valore da filtrare*: Il valore del controllore restituito

Questo evento non è usato da ``FrameworkBundle``. Può essere usato
per implementare un sotto-sistema di viste.

.. index::
   single: Evento; core.response

L'evento ``core.response``
~~~~~~~~~~~~~~~~~~~~~~~~~~

*Tipo*: ``filter``

*Parametri*: ``request_type`` and ``request``

*Valore da filtrare*: L'istanza della risposta

``FrameworkBundle`` registra diversi ascoltatori:

* :class:`Symfony\\Component\\HttpKernel\\Profiler\\ProfilerListener`:
  raccoglie dati per la richiesta corrente;

* :class:`Symfony\\Bundle\\WebProfilerBundle\\WebDebugToolbarListener`:
  inserisce la Web Debug Toolbar;

* :class:`Symfony\\Component\\HttpKernel\\ResponseListener`: aggiusta il
  ``Content-Type`` della risposta;

* :class:`Symfony\\Component\\HttpKernel\\Cache\\EsiListener`: aggiunge un
  header HTTP ``Surrogate-Control`` quando la risposta ha bisogno di essere
  analizzata per i tag ESI.

.. index::
   single: Evento; core.exception

L'evento ``core.exception``
~~~~~~~~~~~~~~~~~~~~~~~~~~~

*Tipo*: ``notifyUntil``

*Parametri*: ``request_type``, ``request`` e ``exception``

``FrameworkBundle`` registra un
:class:`Symfony\\Bundle\\FrameworkBundle\\Controller\\ExceptionListener` che
gira la richiesta a un controllore dato (il valore del parametro
``exception_listener.controller``, che deve essere nella notazione
``classe::metodo``).

.. _kernel_listener_tag:

Abilitare ascoltatori personalizzati
------------------------------------

Per abilitare un ascoltatore personalizzato, aggiungerlo come servizio
regolare in una delle proprie configurazioni ed etichettarlo come
``kernel.listener``:

.. configuration-block::

    .. code-block:: yaml

        services:
            kernel.listener.your_listener_name:
                class: Fully\Qualified\Listener\Class\Name
                tags:
                    - { name: kernel.listener }

    .. code-block:: xml

        <service id="kernel.listener.your_listener_name" class="Fully\Qualified\Listener\Class\Name">
            <tag name="kernel.listener" />
        </service>

    .. code-block:: php

        $container
            ->register('kernel.listener.your_listener_name', 'Fully\Qualified\Listener\Class\Name')
            ->addTag('kernel.listener')
        ;

L'ascoltatore deve avere un metodo ``register()``, che accetti come
parametro un ``EventDispatcher`` e che registri sé stesso::

    /**
     * Registra un ascoltatore core.*
     *
     * @param EventDispatcher $dispatcher An EventDispatcher instance
     */
    public function register(EventDispatcher $dispatcher)
    {
        $dispatcher->connect('core.*', array($this, 'xxxxxxx'));
    }
