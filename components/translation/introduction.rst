.. index::
    single: Translation
    single: Componenti; Translation

Il componente Translation 
=========================

    Il componente Translation fornisce strumenti per internazionalizzare
    un'applicazione.

Installazione
-------------

Si può installare il componente in due modi:

* :doc:`Installarlo tramite Composer </components/using_components>` (``symfony/translation`` su `Packagist`_);
* Usare il repository ufficiale su Git (https://github.com/symfony/Translation).

.. include:: /components/require_autoload.rst.inc

Costruire il Translator
-----------------------

Il punto di accesso principale al componente Translation è
:class:`Symfony\\Component\\Translation\\Translator`.Prima di poterlo usare,
occorre configurarlo e caricare i messaggi da tradurre (chiamati *cataloghi
di messaggi*).

Configurazione
~~~~~~~~~~~~~~

Il costruttore della classe ``Translator`` ha bisogno di un solo parametro: il locale.

.. code-block:: php

    use Symfony\Component\Translation\Translator;
    use Symfony\Component\Translation\MessageSelector;

    $translator = new Translator('fr_FR', new MessageSelector());

.. note::

    Il locale impostato qui è quello predefinito da usare. Lo può ridefinire
    durante la traduzione delle stringhe.

.. note::

    Il termine *locale* si riferisce più o meno a lingua e paese dell'utente. Può
    essere unq qualsiasi stringa usata da un'applicazione per gestire traduzioni e
    altre variazioni di formato (p.e. la valuta). Si raccomanda un codice `ISO 639-1`_ della
    *lingua*, un trattino basso (``_``), quindi il codice `ISO 3166-1 alpha-2`_ del
    *paese* (p.e. ``fr_FR`` per francese/Francia).

.. _component-translator-message-catalogs:

Caricare i cataloghi di messaggi
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

I messaggi sono memorizzati in cataloghi, all'interno della classe ``Translator``.
Un catalogo di messaggi è come un dizionario di traduzioni per uno specifico
locale.

Il componente Translation usa della classi Loader per caricare i cataloghi. Si possono caricare
più risorse per lo stesso locale, saranno combinato in un unico
catalogo.

Il componente dispone di alcuni Loade predefiniti:

* :class:`Symfony\\Component\\Translation\\Loader\\ArrayLoader` - per caricare
  cataloghi da array PHP.
* :class:`Symfony\\Component\\Translation\\Loader\\CsvFileLoader` - per caricare
  cataloghi da file CSV.
* :class:`Symfony\\Component\\Translation\\Loader\\IcuDatFileLoader` - per caricare
  cataloghi da bundle di risorse.
* :class:`Symfony\\Component\\Translation\\Loader\\IcuResFileLoader` - per caricare
  cataloghi da rundle di risorse.
* :class:`Symfony\\Component\\Translation\\Loader\\IniFileLoader` - per caricare
  cataloghi da file ini.
* :class:`Symfony\\Component\\Translation\\Loader\\MoFileLoader` - per caricare
  cataloghi da file gettext.
* :class:`Symfony\\Component\\Translation\\Loader\\PhpFileLoader` - per caricare
  cataloghi da file PHP.
* :class:`Symfony\\Component\\Translation\\Loader\\PoFileLoader` - per caricare
  cataloghi da file gettext.
* :class:`Symfony\\Component\\Translation\\Loader\\QtFileLoader` - per caricare
  cataloghi da file QT XML.
* :class:`Symfony\\Component\\Translation\\Loader\\XliffFileLoader` - per caricare
  cataloghi da file Xliff.
* :class:`Symfony\\Component\\Translation\\Loader\\YamlFileLoader` - per caricare
  cataloghi da file Yaml (richiede il :doc:`componente Yaml</components/yaml/introduction>`).

.. versionadded:: 2.1
    ``IcuDatFileLoader``, ``IcuResFileLoader``, ``IniFileLoader``,
    ``MoFileLoader``, ``PoFileLoader`` e ``QtFileLoader`` sono stati introdotti
    in Symfony 2.1.

Tutti i Loader di file richiedono il :doc:`componente Config </components/config/index>`.

Si possono anche :doc:`creare Loader personalizzati </components/translation/custom_formats>`,
nel caso in cui il formato non fosse già supportato da uno di quelli predefiniti.

Per prima cosa, aggiungere uno o più Loader a ``Translator``::

    // ...
    $translator->addLoader('array', new ArrayLoader());

Il primo parametro è il nome con cui si può fare riferimento al Loader in
Translator e il secondo parametro è un'istanza del Loader stesso. Successivamente,
si possono aggiungere risorse, usando il Loader corretto.

Caricare messaggi con ``ArrayLoader``
.....................................

Si possono caricare messaggi richiamando
:method:`Symfony\\Component\\Translation\\Translator::addResource`. Il primo
parametro è il nome del Loader (che era il primo parametro del metodo ``addLoader``),
il secondo è la risorsa e il terzo è il locale::

    // ...
    $translator->addResource('array', array(
        'Hello World!' => 'Bonjour',
    ), 'fr_FR');

Caricare messaggi i caricatori di file
......................................

Se si usa uno dei Loader di file, si dovrebbe usare anche il metodo ``addResource``.
L'unica differenza è che si dovrebbe mettere il percorso della risorsa del
file come secondo parametro, invece di un array::

    // ...
    $translator->addLoader('yaml', new YamlFileLoader());
    $translator->addResource('yaml', 'path/to/messages.fr.yml', 'fr_FR');

Il processo di traduzione
-------------------------

Per tradurre effettivamente il messaggio, Translator usa un semplice processo:

* Carica un catalogo di messaggi tradotti dalle risorse di traduzione definite
  per ``locale`` (p.e. ``fr_FR``). Carica anche i
  :ref:`components-fallback-locales` e li aggiunge al
  catalogo, se non esistono ancora. Il risultato finale è un grosso "dizionario"
  di traduzioni;

* Se il messaggio si trova nel catalogo, ne restituisce la traduzione. Altrimenti,
  restituisce il messaggio originale.

Il processo inizia quando di richiama
:method:`Symfony\\Component\\Translation\\Translator::trans` o
:method:`Symfony\\Component\\Translation\\Translator::transChoice`. Quindi,
Translator cerca la string nell'appropriato catalogo di messaggi
e la restituisce (se esiste).

.. _components-fallback-locales:

Locale predefiniti
~~~~~~~~~~~~~~~~~~

Se il messaggio non si trova nel catalogo speficiato dal locale,
Translator cercherà nei cataloghi dei locale predefiniti. Per
esempio, se si prova a tradurre nel locale ``fr_FR``:

#. Translator cerca prima la traduzione nel locale ``fr_FR``;

#. Se non la trova, cerca la traduzione nel locale
   ``fr``;

#. Se non la trova ancora, usa uno o più
   locale predefiniti, impostati esplicitamente.

Per il terzo punto, i locale predefiniti possono essere impostati richiamando
:method:`Symfony\\Component\\Translation\\Translator::setFallbackLocale`::

    // ...
    $translator->setFallbackLocale(array('en'));

.. _using-message-domains:

Uso dei domini dei messaggi
---------------------------

Come già visto, i file dei messaggi sono organizzati nei vari locale che
traducono. I file dei messaggi possono anche essere ulteriormente organizzati in "domini".

Il dominio è specificato nel quarto parametro del metodo ``addResource()``.
Il dominio predefinito è ``messages``. Per esempio, si supponga che, per
organizzarle meglio, le traduzioni siano suddivise in tre domini:
``messages``, ``admin`` e ``navigation``. La traduzione francese sarebbe
caricata in questo modo::

    // ...
    $translator->addLoader('xlf', new XliffFileLoader());

    $translator->addResource('xlf', 'messages.fr.xlf', 'fr_FR');
    $translator->addResource('xlf', 'admin.fr.xlf', 'fr_FR', 'admin');
    $translator->addResource(
        'xlf',
        'navigation.fr.xlf',
        'fr_FR',
        'navigation'
    );

Quando si traducono stringhe che non sono nel dominio predefinito (``messages``),
si deve specificare il dominio come terzo parametro di ``trans()``::

    $translator->trans('Symfony is great', array(), 'admin');

Symfony ora cercherà il messaggio nel dominio ``admin`` del locale
specificato.

Uso
---

Leggere come usare il componente Translation in :doc:`/components/translation/usage`.

.. _Packagist: https://packagist.org/packages/symfony/translation
.. _`ISO 3166-1 alpha-2`: http://en.wikipedia.org/wiki/ISO_3166-1#Current_codes
.. _`ISO 639-1`: http://en.wikipedia.org/wiki/List_of_ISO_639-1_codes
