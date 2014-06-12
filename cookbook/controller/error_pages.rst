.. index::
   single: Controllore; Personalizzare le pagine di errore
   single: Pagine di errore

Personalizzare le pagine di errore
==================================

Quando in Symfony2 viene lanciata una qualsiasi eccezione, questa viene catturata all'interno
della classe ``Kernel`` e successivamente inoltrata a un controllore speciale,
``TwigBundle:Exception:show``, per la gestione. Questo controllore, che si trova
all'interno di TwigBundle, determina quale template di errore visualizzare e
il codice di stato che dovrebbe essere impostato per la data eccezione.

Le pagine di errore possono essere personalizzate in due diversi modi, a seconda di quanto
controllo si vuole avere:

1. Personalizzare i template di errore delle diverse pagine di errore (spiegato più avanti);

2. Sostituire il controllore predefinito delle eccezioni ``twig.controller.exception:showAction``.

ExceptionController predefinito
-------------------------------

L'``ExceptionController`` predefinito mostrerà una pagina di
*eccezione* o di *errore*, a seconda dell'impostazione di ``kernel.debug``.
Mentre le pagine di *eccezione* forniscono varie informazioni utili
durante lo sviluppo, le pagine di *errore* sono rivolte
all'utente finale.

.. sidebar:: Test delle pagine di errore durante lo sviluppo

    Non si dovrebbe impostare ``kernel.debug`` a ``false`` per poter vedere una
    pagina di errore durante lo sviluppo. Questo impedirebbe anche a
    Symfony2 di ricompilare i template Twig, tra le altre cose.

    Il bundle `WebfactoryExceptionsBundle`_ fornisce uno speciale controllore
    che consente di mostrare le pagine di errore personalizzate
    per codici di stato HTTP arbitrari, anche quando
    ``kernel.debug`` è impostato a ``true``.

Sovrascrivere i template degli errori
-------------------------------------

Tutti i template degli errori sono presenti all'interno di TwigBundle. Per sovrascrivere i
template, si può semplicemente utilizzare il metodo standard per sovrascrivere i template che
esistono all'interno di un bundle. Per maggiori informazioni, vedere
:ref:`overriding-bundle-templates`.

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
    e opzionale motore per i template che si integra con Symfony2. Per maggiori
    informazioni su Twig, vedere :doc:`/book/templating`.

In aggiunta alla pagina di errore standard HTML, Symfony fornisce una pagina di errore
predefinita per molti dei formati di risposta più comuni, tra cui JSON
(``error.json.twig``), XML, (``error.xml.twig``) e anche JavaScript
(``error.js.twig``), per citarne alcuni. Per sovrascrivere uno di questi template, basta
creare un nuovo file con lo stesso nome nella cartella
``app/Resources/TwigBundle/views/Exception``. Questo è il metodo standard
per sovrascrivere qualunque template posizionato dentro a un bundle.

.. _cookbook-error-pages-by-status-code:

Personalizzazione della pagina 404 e di altre pagine di errore
--------------------------------------------------------------

È anche possibile personalizzare specializzare specifici template di errore in base al
codice di stato. Per esempio, creare un template
``app/Resources/TwigBundle/views/Exception/error404.html.twig`` per
visualizzare una pagina speciale per gli errori 404 (pagina non trovata).

Symfony utilizza il seguente algoritmo per determinare quale template deve usare:

* Prima, cerca un template per il dato formato e codice di stato (tipo
  ``error404.json.twig``);

* Se non esiste, cerca un per il dato formato (tipo
  ``error.json.twig``);

* Se non esiste, si ricade nel template HTML (tipo
  ``error.html.twig``).

.. tip::

    Per vedere l'elenco completo dei template di errore predefiniti, vedere la
    cartella ``Resources/views/Exception`` di TwigBundle. In una
    installazione standard di Symfony2, si può trovareTwigBundle in
    ``vendor/symfony/src/Symfony/Bundle/TwigBundle``. Spesso, il modo più semplice
    per personalizzare una pagina di errore è quello di copiarlo da TwigBundle in
    ``app/Resources/TwigBundle/views/Exception`` e poi modificarlo.

.. note::

    Le pagine "amichevoli" di debug delle eccezioni mostrate allo sviluppatore possono ugualmente
    essere personalizzate, creando template come
    ``exception.html.twig`` per la pagina di eccezione standard in HTML o
    ``exception.json.twig`` per la pagina di eccezione JSON.

.. _`WebfactoryExceptionsBundle`: https://github.com/webfactory/exceptions-bundle

Sostituire il controllore Exception predefinito
-----------------------------------------------

Se dovesse servire più flessibilità, oltre a sovrascrivere solo il template
(p.e. se serve passare variabili aggiuntive a un template),
si può sovrascrivere il controllore che rende la pagina di errore.

Il controllore predefinito delle eccezioni è registrato come servizio, la classe
effettiva è ``Symfony\Bundle\TwigBundle\Controller\ExceptionController``.

Per poterlo fare, creare una nuova classe controllore e farle estendere la classe
``Symfony\Bundle\TwigBundle\Controller\ExceptionController`` di Symfony.

Ci sono molti metodi da poter sovrascrivere per personalizzare le varie parti della
resa della pagina di errore. Si può, per esempio, sovrascrivere l'intera
``showAction`` oppure solo il meteodo ``findTemplate``, che individua quale
template vada reso.

Per far usare a Symfony il nuovo controllore al posto di quello predefinito, impostare l'opzione
:ref:`twig.exception_controller <config-twig-exception-controller>`
in app/config/config.yml.

.. tip::

    La personalizzazione della gestione delle eccezioni in realtà è molto più potente
    di quanto scritto qui. Viene lanciato un evento interno, ``kernel.exception``,
    che permette un controllo completo sulla gestione delle eccezioni. Per maggiori
    informazioni, vedere :ref:`kernel-kernel.exception`.
