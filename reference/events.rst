Eventi del framework Symfony
============================

Quando il framework Symfony (o qualsiasi altra cosa che usi :class:`Symfony\\Component\\HttpKernel\\HttpKernel`)
gestisce una richiesta, alcuni eventi sono distribuiti, in modo che sia possibile aggiungere ascoltatori
lungo il processo. Questi prendono il nome di "eventi del kernel". Per maggiori dettagli,
vedere :doc:`/components/http_kernel/introduction`.

Eventi del kernel
-----------------

Ogni evento distribuito dal kernel è una sottoclasse di
:class:`Symfony\\Component\\HttpKernel\\Event\\KernelEvent`. Quindi ogni evento
ha accesso alle seguenti informazioni:

:method:`Symfony\\Component\\HttpKernel\\Event\\KernelEvent::getRequestType`
    Restituisce il *tipo* di richiesta (``HttpKernelInterface::MASTER_REQUEST`` o
    ``HttpKernelInterface::SUB_REQUEST``).

:method:`Symfony\\Component\\HttpKernel\\Event\\KernelEvent::getKernel`
    Restituisce il Kernel che sta gestendo la richiesta.

:method:`Symfony\\Component\\HttpKernel\\Event\\KernelEvent::getRequest`
    Restituisce la richiesta gestita.

.. _kernel-core-request:

``kernel.request``
~~~~~~~~~~~~~~~~~~

**Classe evento**: :class:`Symfony\\Component\\HttpKernel\\Event\\GetResponseEvent`

Evento distribuito molto presto in Symfony, prima che venga determinato
il controllore.

.. seealso::

    Saperne di più sull'evento :ref:`kernel.request <component-http-kernel-kernel-request>`.

Ecco gli ascoltatori che Symfony registra su questo evento:

=============================================================================  ========
Nome classe ascoltatore                                                        Priorità
=============================================================================  ========
:class:`Symfony\\Component\\HttpKernel\\EventListener\\ProfilerListener`       1024
:class:`Symfony\\Bundle\\FrameworkBundle\\EventListener\\TestSessionListener`  192
:class:`Symfony\\Bundle\\FrameworkBundle\\EventListener\\SessionListener`      128
:class:`Symfony\\Component\\HttpKernel\\EventListener\\RouterListener`         32
:class:`Symfony\\Component\\HttpKernel\\EventListener\\LocaleListener`         16
:class:`Symfony\\Component\\Security\\Http\\Firewall`                          8
=============================================================================  ========

``kernel.controller``
~~~~~~~~~~~~~~~~~~~~~

**Classe evento**: :class:`Symfony\\Component\\HttpKernel\\Event\\FilterControllerEvent`

Questo evento può essere un punto di ingresso per modificare il controllore da eseguire::

    use Symfony\Component\HttpKernel\Event\FilterControllerEvent;

    public function onKernelController(FilterControllerEvent $event)
    {
        $controller = $event->getController();
        // ...

        // si può sostituire il controllore con qualsiasi Callable PHP
        $event->setController($controller);
    }

.. seealso::

    Saperne di più sull'evento :ref:`kernel.controller <component-http-kernel-kernel-controller>`.

Symfony registra un ascoltatore legato a questo evento:

==============================================================================  ========
Nome classe ascoltatore                                                         Priorità
==============================================================================  ========
:class:`Symfony\\Bundle\\FrameworkBundle\\DataCollector\\RequestDataCollector`  0
==============================================================================  ========

``kernel.view``
~~~~~~~~~~~~~~~

**Classe evento**: :class:`Symfony\\Component\\HttpKernel\\Event\\GetResponseForControllerResultEvent`

Questo evento non è usato da FrameworkBundle, ma può essere usato per implementare un
sistema di sottoviste. Questo evento è richiamato *solo* se il controllore *non*
restituisce un oggetto ``Response``. Lo scopo di questo evento è quello di dare la possibilità
a un qualche altro valore restituito di essere convertito in ``Response``.

Il valore restituito dal controllore è accessibile tramite il metodo
``getControllerResult``::

    use Symfony\Component\HttpKernel\Event\GetResponseForControllerResultEvent;
    use Symfony\Component\HttpFoundation\Response;

    public function onKernelView(GetResponseForControllerResultEvent $event)
    {
        $val = $event->getControllerResult();
        $response = new Response();

        // ... personalizzare in qualche modo la risposta, a partire da $val

        $event->setResponse($response);
    }

.. seealso::

    Saperne di più sull'evento :ref:`kernel.view <component-http-kernel-kernel-view>`.

``kernel.response``
~~~~~~~~~~~~~~~~~~~

**Classe evento**: :class:`Symfony\\Component\\HttpKernel\\Event\\FilterResponseEvent`

Lo scopo di questo evento è quello di consentire ad altri sistemi di modificare o sostituire l'oggetto
``Response``, dopo la sua creazione::

    public function onKernelResponse(FilterResponseEvent $event)
    {
        $response = $event->getResponse();

        // ... modificare l'oggetto Response
    }

FrameworkBundle registra vari ascoltatori:

:class:`Symfony\\Component\\HttpKernel\\EventListener\\ProfilerListener`
    Raccoglie dati per la richiesta corrente.

:class:`Symfony\\Bundle\\WebProfilerBundle\\EventListener\\WebDebugToolbarListener`
    Inietta la barra di debug del web.

:class:`Symfony\\Component\\HttpKernel\\EventListener\\ResponseListener`
    Aggiusta il ``Content-Type`` della risposta, in base al formato della richiesta.

:class:`Symfony\\Component\\HttpKernel\\EventListener\\EsiListener`
    Aggiunge un header HTTP ``Surrogate-Control`` quando la risposta deve essere analizzata alla
    ricerca di tag ESI.

.. seealso::

    Saperne di più sull'evento :ref:`kernel.response <component-http-kernel-kernel-response>`.

Symfony registra i seguenti ascoltatori per questo evento:

===================================================================================  ========
Nome classe ascoltatore                                                              Priorità
===================================================================================  ========
:class:`Symfony\\Component\\HttpKernel\\EventListener\\EsiListener`                  0
:class:`Symfony\\Component\\HttpKernel\\EventListener\\ResponseListener`             0
:class:`Symfony\\Bundle\\SecurityBundle\\EventListener\\ResponseListener`            0
:class:`Symfony\\Component\\HttpKernel\\EventListener\\ProfilerListener`             -100
:class:`Symfony\\Bundle\\FrameworkBundle\\EventListener\\TestSessionListener`        -128
:class:`Symfony\\Bundle\\WebProfilerBundle\\EventListener\\WebDebugToolbarListener`  -128
:class:`Symfony\\Component\\HttpKernel\\EventListener\\StreamedResponseListener`     -1024
===================================================================================  ========

``kernel.terminate``
~~~~~~~~~~~~~~~~~~~~

**Classe evento**: :class:`Symfony\\Component\\HttpKernel\\Event\\PostResponseEvent`

Lo scopo di questo evento è quello di eseguire alcuni compiti dopo che la risposta è
stata servita al client.

.. seealso::

    Saperne di più sull'evento :ref:`kernel.terminate <component-http-kernel-kernel-terminate>`.

Symfony registra un ascoltatore di questo evento:

=========================================================================  ========
Nome classe ascoltatore                                                    Priorità
=========================================================================  ========
`EmailSenderListener`_                                                     0
=========================================================================  ========


.. _kernel-kernel.exception:

``kernel.exception``
~~~~~~~~~~~~~~~~~~~~

**Classe evento**: :class:`Symfony\\Component\\HttpKernel\\Event\\GetResponseForExceptionEvent`

TwigBundle registra :class:`Symfony\\Component\\HttpKernel\\EventListener\\ExceptionListener`,
che gira ``Request`` a un controllore dato, definito dal parametro
``exception_listener.controller``.

Un ascoltatore su questo evento può creare e impostare un oggetto ``Response``, creare e
impostare un nuovo oggetto ``Exception`` oppure non fare nulla::

    use Symfony\Component\HttpKernel\Event\GetResponseForExceptionEvent;
    use Symfony\Component\HttpFoundation\Response;

    public function onKernelException(GetResponseForExceptionEvent $event)
    {
        $exception = $event->getException();
        $response = new Response();
        // prepara l'oggetto Response in base all'eccezione catturata
        $event->setResponse($response);

        // in alternativa, si può impostare una nuova eccezione
        // $exception = new \Exception('Una nuova eccezione');
        // $event->setException($exception);
    }

.. note::

    Poiché Symfony imposta il codice di stato della risposta con il valore più
    appropriato (a seconda dell'eccezione), non bisogna impostare lo stato
    della risposta. Se si vuole ridefinire il codice di stato (solo se
    si ha una buona ragione), impostare l'header ``X-Status-Code``::

        $response = Response(
            'Error',
            404 // ignorato,
            array('X-Status-Code' => 200)
        );

.. seealso::

    Saperne di più sull'evento :ref:`kernel.exception <component-http-kernel-kernel-exception>`.

Questi ascoltatori sono registrati da Symfony su questo evento:

=========================================================================  ========
Nome classe ascoltatore                                                    Priorità
=========================================================================  ========
:class:`Symfony\\Component\\HttpKernel\\EventListener\\ProfilerListener`   0
:class:`Symfony\\Component\\HttpKernel\\EventListener\\ExceptionListener`  -128
=========================================================================  ========

.. _`EmailSenderListener`: https://github.com/symfony/SwiftmailerBundle/blob/master/EventListener/EmailSenderListener.php
