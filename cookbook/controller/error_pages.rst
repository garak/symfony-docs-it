.. index::
   single: Controllore; Personalizzare le pagine di errore
   single: Pagine di errore

Personalizzare le pagine di errore
==================================

Quando viene lanciata un'eccezione, la classe ``HttpKernel`` la cattura e
distribuisce un evento ``kernel.exception``. Questo fornisce la possibilità di convertire
l'eccezione in una ``Response``, in vari modi.

Il bundle TwigBundle ha un ascoltatore per tale evento, che eseguirà un controllore configurabile
(anche se arbitrario), per generare la risposta. Il controllore predefinito ha un modo
intelligente per scegliere uno dei template di errore a disposizione:

.. image:: /images/cookbook/controller/error_pages/exceptions-in-dev-environment.png
   :alt: Una tipica pagina di eccezione in ambiente di sviluppo

Quindi, le pagine di errore possono essere personalizzate in diversi modi, a seconda di quanto
controllo si vuole avere:

#. :ref:`Usare ExceptionController predefinito e creare alcuni
   template, che consentono di personalizzare l'aspetto delle varie
   pagine di errore (facile); <use-default-exception-controller>`

#. :ref:`Sostituire il controllore predefinito con uno personalizzato
   (intermedio). <custom-exception-controller>`

#. :ref:`Usare l'evento kernel.exception e implementare una gestione
   personalizzata (avanzato). <use-kernel-exception-event>`

.. _use-default-exception-controller:
.. _using-the-default-exceptioncontroller:

ExceptionController predefinito
-------------------------------

Il metodo ``showAction()`` del controllore
:class:`Symfony\\Bundle\\TwigBundle\\Controller\\ExceptionController`
sarà richiamato al verificarsi di un'eccezione.

Questo controllore mostrerà una pagina di
*eccezione* o di *errore*, a seconda dell'impostazione di ``kernel.debug``.
Mentre le pagine di *eccezione* forniscono varie informazioni utili
durante lo sviluppo, le pagine di *errore* sono rivolte
all'utente finale.

.. sidebar:: Test delle pagine di errore durante lo sviluppo

    Non si dovrebbe impostare ``kernel.debug`` a ``false`` per poter vedere una
    pagina di errore durante lo sviluppo. Questo impedirebbe anche a
    Symfony di ricompilare i template Twig, tra le altre cose.

    Il bundle `WebfactoryExceptionsBundle`_ fornisce uno speciale controllore
    che consente di mostrare le pagine di errore personalizzate
    per codici di stato HTTP arbitrari, anche quando
    ``kernel.debug`` è impostato a ``true``.

.. _`WebfactoryExceptionsBundle`: https://github.com/webfactory/exceptions-bundle

.. _cookbook-error-pages-by-status-code:

Come sono scelti i template per le pagine di errore e di eccezione
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Il bundle TwigBundle ha dei template predefiniti per le pagine di errore e
di eccezione, nella sua cartella ``Resources/views/Exception``.

.. tip::

    In una tipica installazione di Symfony, si può trovare TwigBundle sotto
    ``vendor/symfony/symfony/src/Symfony/Bundle/TwigBundle``. Oltre alla pagina
    di errore standard HTML, fornisce anche una pagina di errore per molti
    dei formati di risposta più comuni, inclusi
    JSON (``error.json.twig``), XML (``error.xml.twig``) e anche
    JavaScript (``error.js.twig``), solo per citarne alcuni.

Ecco come ``ExceptionController`` sceglierà uno dei template
disponibili, in base al codice di stato HTTP e al formato della richiesta:

* Per le pagine di *errore*, cerca prima un template per il formato e il codice
  di stato dati (come ``error404.json.twig``);

* Se non lo trova, cerca per un template generico per il formato
  dato (come ``error.json.twig`` o
  ``exception.json.twig``);

* Infine, ignora il formato e usa il template HTML
  (come ``error.html.twig`` o ``exception.html.twig``).

.. tip::

    Se l'eccezione implementa l'interfaccia
    :class:`Symfony\\Component\\HttpKernel\\Exception\\HttpExceptionInterface`,
    il metodo ``getStatusCode()`` sarà richiamato per
    ricavare il codice di stato HTTP da utilizzare. Altrimenti,
    il codice di stato sarà "500".

Sovrascrivere i template degli errori
-------------------------------------

Per sovrascrivere questi template, si può semplicemente utilizzare il metodo standard
per sovrascrivere i template che esistono all'interno di un bundle. Per maggiori informazioni,
vedere :ref:`overriding-bundle-templates`.

Ad esempio, per sovrascrivere il template di errore predefinito che è mostrato
all'utente finale, creare un nuovo template posizionato in
``app/Resources/TwigBundle/views/Exception/error.html.twig``:

.. code-block:: html+jinja

    <!DOCTYPE html>
    <html>
    <head>
        <meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
        <title>Si è verificato un errore: {{ status_text }}</title>
    </head>
    <body>
        <h1>Oops! Si è verificato un errore</h1>
        <h2>Il server ha restituito un "{{ status_code }} {{ status_text }}".</h2>
    </body>
    </html>

.. caution::

    **Non si deve** usare ``is_granted`` nelle pagine di errore (o nei layout usati
    dalle pagine di errore), perché il router gira prima del firewall. Se
    il router lancia un'eccezione (per esempio, quando la rotta non
    esiste), l'uso di ``is_granted`` lancerà un'ulteriore eccezione. Si
    può usare ``is_granted`` in modo sicuro con ``{% if app.user and is_granted('...') %}``.

.. tip::

    Non bisogna preoccuparsi, se non si ha familiarità con Twig. Twig è un semplice, potente
    e opzionale motore per i template che si integra con Symfony. Per maggiori
    informazioni su Twig, vedere :doc:`/book/templating`.

Questa logica funziona non solo per sostituire i template predefiniti, ma anche per
crearne di nuovi.

Per esempio, creare un template ``app/Resources/TwigBundle/views/Exception/error404.html.twig``,
per mostrare una pagina speciale per gli errori 404 (non trovato).
Fare riferimento alla sezione precedente per l'ordine in cui
``ExceptionController`` cerca i vari nomi di template.

.. tip::

    Spesso, il modo più semplice per personalizzare una pagina di errore è quello di copiarla
    da TwigBundle in ``app/Resources/TwigBundle/views/Exception`` e
    poi modificarla.

.. note::

    Anche le pagine di eccezione mostrate allo sviluppatore durante il debug possono essere
    personalizzate, creando template come
    ``exception.html.twig``, per la pagina di eccezione standard HTML, o
    ``exception.json.twig``, per la pagina di eccezione JSON.

.. _custom-exception-controller:
.. _replacing-the-default-exceptioncontroller:

Sostituire ExceptionController
------------------------------

Chi avesse bisogno di un po' più di flessibilità, oltre a riscrivere il
template, può cambiare il controllore che rende la pagina di errore.
Per esempio, si potrebbero voler passare variabili aggiuntive
al template.

.. caution::

    Assicurarsi di non perdere le pagine di eccezione che rendono gli utili
    messaggi di errore durante lo sviluppo.

Per farlo, basta creare un nuovo controllore e impostare l'opzione
:ref:`twig.exception_controller <config-twig-exception-controller>` per
puntarvi.

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        twig:
            exception_controller:  AppBundle:Exception:showException

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <?xml version="1.0" encoding="UTF-8" ?>
        <container xmlns="http://symfony.com/schema/dic/services"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xmlns:twig="http://symfony.com/schema/dic/twig"
            xsi:schemaLocation="http://symfony.com/schema/dic/services
                http://symfony.com/schema/dic/services/services-1.0.xsd
                http://symfony.com/schema/dic/twig
                http://symfony.com/schema/dic/twig/twig-1.0.xsd">

            <twig:config>
                <twig:exception-controller>AppBundle:Exception:showException</twig:exception-controller>
            </twig:config>
        </container>

    .. code-block:: php

        // app/config/config.php
        $container->loadFromExtension('twig', array(
            'exception_controller' => 'AppBundle:Exception:showException',
            // ...
        ));

.. tip::

    Si può anche impostare il controllore come servizio.

    Il valore predefinito di ``twig.controller.exception:showAction`` si riferisce
    al metodo ``showAction`` di ``ExceptionController``, descritto
    in precedenza, che è registrato nel contenitore dei servizi come
    ``twig.controller.exception``.

Al controllore saranno passati due parametri: ``exception``,
che è un'istanza di :class:`\\Symfony\\Component\\Debug\\Exception\\FlattenException`,
creata dall'eccezione gestita, e ``logger``,
un'istanza di :class:`\\Symfony\\Component\\HttpKernel\\Log\\DebugLoggerInterface`
(che potrebbe essere ``null``).

.. tip::

    La Request che sarà inviata al controllore è creata
    in :class:`Symfony\\Component\\HttpKernel\\EventListener\\ExceptionListener`.
    Questo ascoltatore di eventi è impostato da TwigBundle.

Ovviamente, si può anche estendere
:class:`Symfony\\Bundle\\TwigBundle\\Controller\\ExceptionController`, come descritto prima.
In tal caso, si potrebbe voler sovrascrivere uno o entrambi i metodi
``showAction`` e ``findTemplate``. Il secondo individua il
template da usare.

.. caution::

    Attualmente, ``ExceptionController`` *non* fa parte delle API di
    Symfony, quindi fare attenzione: potrebbe cambiare in futuro.

.. _use-kernel-exception-event:

Usare l'evento kernel.exception
-------------------------------

Come menzionato in precedenza, l'evento ``kernel.exception`` viene distribuito
quando il kernel di Symfony Kernel deve gestire un'eccezione.
Per approfondire, si veda :ref:`kernel-kernel.exception`.

L'utilizzo di questo evento è in realtà molto più potente di quanto sia stato detto in
precedenza, ma richiede anche una chiara comprensione del funzionamento
interno di Symfony.

Per fornire solo un esempio, ipotizziamo che un'applicazione lanci eccezioni
specializzate, con un significato particolare per il suo dominio.

In questo caso, tutto quello che ``ExceptionListener`` e
``ExceptionController`` possono fare è provare a immaginare il codice
di stato HTTP corretto e mostrare una pagina di errore generica.

La :doc:`scrittura di un ascoltatore di eventi personalizzato </cookbook/service_container/event_listener>`
per l'evento ``kernel.exception`` consente di dare uno sguardo più da vicino
all'eccezione e intraprendere azioni diverse. Tali azioni possono
includere il log dell'eccezione, il rinvio dell'utente a
un'altra pagina o la resa di pagine di errore specializzate.

.. note::

    Se l'ascoltatore richiama ``setResponse()`` su
    :class:`Symfony\\Component\\HttpKernel\\Event\\GetResponseForExceptionEvent`,
    la propagazione dell'evento sarà stoppata e la risposta inviata
    al client.

Questo approccio consente di creare una gestione centralizzata e strutturata degli
errori: invece di catturare (e gestire) le stesse eccezioni
in vari controllori ogni volta, si può avere un solo ascoltatore (ma anche più
di uno) che se ne occupi.

.. tip::

    Per un esempio, dare un'occhiata a `ExceptionListener`_ nel componente
    Security.

    Gestisce varie eccezioni legate alla sicurezza, lanciate in
    un'applicazione (come :class:`Symfony\\Component\\Security\\Core\\Exception\\AccessDeniedException`)
    e mette in atto misure come il rinvio dell'utente alla pagina di login,
    la disconnessione e altre cose.

Buona fortuna!

.. _`ExceptionListener`: https://github.com/symfony/symfony/blob/master/src/Symfony/Component/Security/Http/Firewall/ExceptionListener.php
