.. index::
   single: Intl
   single: Componenti; Intl

Il componente Intl
==================

    Un rimpiazzo PHP per l'`estensione intl`_ C, che fornisce anche
    accesso ai dati di localizzazione della `libreria ICU`_.

.. versionadded:: 2.3

    Il componente Intl è stato aggiunto in Symfony 2.3. Nelle precedenti versioni di Symfony,
    va usato il componente Locale al suo posto.

.. caution::

    Il rimpiazzo è limitato al locale "en". Se si vogliono usare
    altri locale, si dovrebbe invece  `installare l'estensione intl`_.

Installazione
-------------

Si può installare il componente in due modi:

* Usando il repository Git ufficiale (https://github.com/symfony/Intl);
* Tramite :doc:`Composer</components/using_components>` (``symfony/intl`` su `Packagist`_).

Se si installa il componente tramite Composer, saranno fornite automaticamente le seguenti classi e
funzioni dell'estensione intl, nel caso in cui l'estensione intl stessa non
sia caricata:

* :phpclass:`Collator`
* :phpclass:`IntlDateFormatter`
* :phpclass:`Locale`
* :phpclass:`NumberFormatter`
* :phpfunction:`intl_error_name`
* :phpfunction:`intl_is_failure`
* :phpfunction:`intl_get_error_code`
* :phpfunction:`intl_get_error_message`

Quando l'estensione intl non è disponibile, sono usate le seguenti classi, per
sostituire quelle di intl:

* :class:`Symfony\\Component\\Intl\\Collator\\Collator`
* :class:`Symfony\\Component\\Intl\\DateFormatter\\IntlDateFormatter`
* :class:`Symfony\\Component\\Intl\\Locale\\Locale`
* :class:`Symfony\\Component\\Intl\\NumberFormatter\\NumberFormatter`
* :class:`Symfony\\Component\\Intl\\Globals\\IntlGlobals`

Composer espone automaticamente tali classi nello spazio dei nomi globale.

Se non si usa Composer, ma il
:doc:`componente ClassLoader di Symfony</components/class_loader>`, occorre
esporre a mano le classi, aggiungendo le linee seguenti al proprio autoloader::

    if (!function_exists('intl_is_failure')) {
        require '/percorso/delle/funzioni/Icu/funzioni.php';

        $loader->registerPrefixFallback('/percorso/delle/funzioni/Icu');
    }

.. sidebar:: ICU e problemi di deploy

    L'estensione intl usa internamente la `libreria ICU`_ per ottenere dati localizzati,
    come formati numerici nelle varie lingue, nomi di paesi, eccetera.
    Per rendere disponibili tali dati alle librerie utente di PHP, Symfony2 ne possiede una copia
    nel `componente ICU`_.

    A seconda della versione di ICU compilata nell'estensione intl, occorre installare una versione
    corrispondente del componente. Sembra complicato,
    ma di solito Composer lo fa in modo automatico:

    * 1.0.*: se l'estensione intl non è disponibile
    * 1.1.*: se intl è compilato con ICU 4.0 o successivi
    * 1.2.*: se intl è compilato con ICU 4.4 o successivi

    Queste versioni sono importanti se di esegue il deploy su un **server con
    una versione di ICU  precedente** a quella della macchina di sviluppo, perché il deploy
    fallirà se

    * la macchina di sviluppo è compilata con ICU 4.4 o successivi, ma il
      server compilato con una versione di ICU precedente alla 4.4;
    * l'estensione intl è disponibile sulla macchina di sviluppo, ma non sul
      server.

    Per esempio, ipotizziamo che la macchina di sviluppo abbia ICU 4.8 e il server
    ICU 4.2. Quando si esegue ``php composer.phar update`` sulla macchina di sviluppo, sarà installata
    la versione 1.2.* del componente ICU. Ma, dopo il deploy
    dell'applicazione, ``php composer.phar install`` fallirà con il seguente errore:

    .. code-block:: bash

        $ php composer.phar install
        Loading composer repositories with package information
        Installing dependencies from lock file
        Your requirements could not be resolved to an installable set of packages.

          Problem 1
            - symfony/icu 1.2.x requires lib-icu >=4.4 -> the requested linked
              library icu has the wrong version installed or is missing from your
              system, make sure to have the extension providing it.

    L'errore dice che la versione richiesta del componente ICU, la
    1.2, non è compatibile con la versione 4.2 di ICU di PHP.

    Una possibile soluzione consiste nell'eseguire ``php composer.phar update`` invece di
    ``php composer.phar install``. Ma si raccomanda caldamente di **non** fare in questo modo.
    Il comando ``update`` installerà le versioni più recenti di ogni dipendenza di Composer
    nel server di produzione, il che potrebbe rompere l'applicazione.

    Una soluzione migliore consiste nel sistemare composer.json, inserendo la versione richiesta dal
    server di produzione. Innanzitutto, determinare la versione ICU sul server:

    .. code-block:: bash

        $ php -i | grep ICU
        ICU version => 4.2.1

    Quindi modificare il componente ICU in composer.json, inserendo una versione corrispondente:

    .. code-block:: json

        "require: {
            "symfony/icu": "1.1.*"
        }

    Impostare la versione a

    * "1.0.*" se il server non ha l'estensione intl installata;
    * "1.1.*" se il server è compilato con ICU 4.2 o precedenti.

    Infine, eseguire ``php composer.phar update symfony/icu`` sulla macchina di sviluppo, testare
    estensivamente e fare un nuovo deploy. L'installazione delle dipendenze ora avrà
    successo.

Scrivere e leggere i bundle delle risorse
-----------------------------------------

La classe :phpclass:`ResourceBundle` non è attualmente supportata da questo componente.
Invece, sono inclusi un insieme di lettori e scrittori, per leggere e scrivere
array (o oggetti simili ad array) da/a file dei bundle delle risorse. Sono supportate
le classi seguenti:

* `TextBundleWriter`_
* `PhpBundleWriter`_
* `BinaryBundleReader`_
* `PhpBundleReader`_
* `BufferedBundleReader`_
* `StructuredBundleReader`_

Chi fosse interessato all'uso di tali classi può continuare la lettura. Altrimenti,
si può saltare la sezione e andare ad `Accesso ai dati ICU`_.

TextBundleWriter
~~~~~~~~~~~~~~~~

:class:`Symfony\\Component\\Intl\\ResourceBundle\\Writer\\TextBundleWriter`
scrive un array o un oggetto simile ad array in un bundle di risorse in testo.
Il file .txt risultante può essere convertito in un file binario .res con la classe
:class:`Symfony\\Component\\Intl\\ResourceBundle\\Compiler\\BundleCompiler`::


    use Symfony\Component\Intl\ResourceBundle\Writer\TextBundleWriter;
    use Symfony\Component\Intl\ResourceBundle\Compiler\BundleCompiler;

    $writer = new TextBundleWriter();
    $writer->write('/percorso/del/bundle', 'en', array(
        'Data' => array(
            'voce1',
            'voce2',
            // ...
        ),
    ));

    $compiler = new BundleCompiler();
    $compiler->compile('/percorso/del/bundle', '/percorso/del/bundle/binario');

Il comando "genrb" della classe
:class:`Symfony\\Component\\Intl\\ResourceBundle\\Compiler\\BundleCompiler` deve essere
disponibile. Se il comando si trova in una posizione non standard, si può passare il
suo percorso al costruttore di
:class:`Symfony\\Component\\Intl\\ResourceBundle\\Compiler\\BundleCompiler`.


PhpBundleWriter
~~~~~~~~~~~~~~~

:class:`Symfony\\Component\\Intl\\ResourceBundle\\Writer\\PhpBundleWriter`
scrive un array o un oggetto simile ad array in un bundle di risorse .php::

    use Symfony\Component\Intl\ResourceBundle\Writer\PhpBundleWriter;

    $writer = new PhpBundleWriter();
    $writer->write('/percorso/del/bundle', 'en', array(
        'Data' => array(
            'voce1',
            'voce2',
            // ...
        ),
    ));

BinaryBundleReader
~~~~~~~~~~~~~~~~~~

:class:`Symfony\\Component\\Intl\\ResourceBundle\\Reader\\BinaryBundleReader`
legge file binari e restituisce un array o un oggetto simile ad array.
La classe attualmente funziona solo con l'`estensione intl`_ installata::

    use Symfony\Component\Intl\ResourceBundle\Reader\BinaryBundleReader;

    $reader = new BinaryBundleReader();
    $data = $reader->read('/percorso/del/bundle', 'en');

    echo $data['Data']['voce1'];

PhpBundleReader
~~~~~~~~~~~~~~~

:class:`Symfony\\Component\\Intl\\ResourceBundle\\Reader\\PhpBundleReader`
legge file .php e restituisce un array o un oggetto simile ad
array::

    use Symfony\Component\Intl\ResourceBundle\Reader\PhpBundleReader;

    $reader = new PhpBundleReader();
    $data = $reader->read('/percorso/del/bundle', 'en');

    echo $data['Data']['voce1'];

BufferedBundleReader
~~~~~~~~~~~~~~~~~~~~

:class:`Symfony\\Component\\Intl\\ResourceBundle\\Reader\\BufferedBundleReader`
avvolge un altro lettore, ma mantiene le ultime N letture in un buffer, dove N è la
dimensione passata al costruttore::

    use Symfony\Component\Intl\ResourceBundle\Reader\BinaryBundleReader;
    use Symfony\Component\Intl\ResourceBundle\Reader\BufferedBundleReader;

    $reader = new BufferedBundleReader(new BinaryBundleReader(), 10);

    // legge il file
    $data = $reader->read('/percorso/del/bundle', 'en');

    // restituisce dati dal buffer
    $data = $reader->read('/percorso/del/bundle', 'en');

    // legge il file
    $data = $reader->read('/percorso/del/bundle', 'fr');

StructuredBundleReader
~~~~~~~~~~~~~~~~~~~~~~

:class:`Symfony\\Component\\Intl\\ResourceBundle\\Reader\\StructuredBundleReader`
avvolge un altro lettore e offre un metodo
:method:`Symfony\\Component\\Intl\\ResourceBundle\\Reader\\StructuredBundleReaderInterface::readEntry`
per leggere un elemento del bundle risorsa senza doversi preoccupare
se le chiavi dell'array siano impostate o meno. Se un percorso non può essere risolto, viene
restituito ``null```::

    use Symfony\Component\Intl\ResourceBundle\Reader\BinaryBundleReader;
    use Symfony\Component\Intl\ResourceBundle\Reader\StructuredBundleReader;

    $reader = new StructuredBundleReader(new BinaryBundleReader());

    $data = $reader->read('/percorso/del/bundle', 'en');

    // provoca un errore se la chiave "Data" non esiste
    echo $data['Data']['entry1'];

    // restituice null se la chiave "Data" non esiste
    echo $reader->readEntry('/percorso/del/bundle', 'en', array('Data', 'entry1'));

Inoltre, il metodo
:method:`Symfony\\Component\\Intl\\ResourceBundle\\Reader\\StructuredBundleReaderInterface::readEntry`
risolve i locale a cascata. Per esempio, il locale a cascata di "en_GB" è
"en". Per elementi a valore singolo (stringhe, numeri ecc.), l'elemento sarà letto
dal locale a cascata, se non trovato nel locale specifico. Per elementi a
valori multipli (array), il valore del locale specifico e di quello a cascata
saranno fusi. Per evitare tale comportamento, si può impostare il parametro
``$fallback`` a ``false``::

    echo $reader->readEntry('/percorso/del/bundle', 'en', array('Data', 'entry1'), false);

Accesso ai dati ICU
-------------------

I dati ICU si trovano in vari "bundle risorsa". Si può accedere a un wrapper PHP
di tali bundle, tramite la classe statica
:class:`Symfony\\Component\\Intl\\Intl`. Al momento, sono supportati i dati
seguenti:

* `Nomi di lingue e di script`_
* `Nomi di paesi`_
* `Locale`_
* `Valute`_

Nomi di lingue e di script
~~~~~~~~~~~~~~~~~~~~~~~~~~

Le traduzioni di nomi di lingue e di script si trovano nel bundle
"language"::

    use Symfony\Component\Intl\Intl;

    \Locale::setDefault('en');

    $languages = Intl::getLanguageBundle()->getLanguageNames();
    // => array('ab' => 'Abkhazian', ...)

    $language = Intl::getLanguageBundle()->getLanguageName('de');
    // => 'German'

    $language = Intl::getLanguageBundle()->getLanguageName('de', 'AT');
    // => 'Austrian German'

    $scripts = Intl::getLanguageBundle()->getScriptNames();
    // => array('Arab' => 'Arabic', ...)

    $script = Intl::getLanguageBundle()->getScriptName('Hans');
    // => 'Simplified'

Tutti i metodi accettano il locale come ultimo parametro, opzionale,
con valore predefinito il locale predefinito::

    $languages = Intl::getLanguageBundle()->getLanguageNames('de');
    // => array('ab' => 'Abchasisch', ...)

Nomi di paesi
~~~~~~~~~~~~~

Le traduzioni di nomi di paesi si trovano nel bundle "region"::

    use Symfony\Component\Intl\Intl;

    \Locale::setDefault('en');

    $countries = Intl::getRegionBundle()->getCountryNames();
    // => array('AF' => 'Afghanistan', ...)

    $country = Intl::getRegionBundle()->getCountryName('GB');
    // => 'United Kingdom'

Tutti i metodi accettano il locale come ultimo parametro, opzionale,
con valore predefinito il locale predefinito::

    $countries = Intl::getRegionBundle()->getCountryNames('de');
    // => array('AF' => 'Afghanistan', ...)

Locale
~~~~~~

Le traduzioni di nomi di locale si trovano nel bundle "locale"::

    use Symfony\Component\Intl\Intl;

    \Locale::setDefault('en');

    $locales = Intl::getLocaleBundle()->getLocaleNames();
    // => array('af' => 'Afrikaans', ...)

    $locale = Intl::getLocaleBundle()->getLocaleName('zh_Hans_MO');
    // => 'Chinese (Simplified, Macau SAR China)'

Tutti i metodi accettano il locale come ultimo parametro, opzionale,
con valore predefinito il locale predefinito::

    $locales = Intl::getLocaleBundle()->getLocaleNames('de');
    // => array('af' => 'Afrikaans', ...)

Valute
~~~~~~

Le traduzioni di nomi di valute e altre informazioni relative alle valute
si trovano nel bundle "currency"::

    use Symfony\Component\Intl\Intl;

    \Locale::setDefault('en');

    $currencies = Intl::getCurrencyBundle()->getCurrencyNames();
    // => array('AFN' => 'Afghan Afghani', ...)

    $currency = Intl::getCurrencyBundle()->getCurrencyName('INR');
    // => 'Indian Rupee'

    $symbol = Intl::getCurrencyBundle()->getCurrencySymbol('INR');
    // => '₹'

    $fractionDigits = Intl::getCurrencyBundle()->getFractionDigits('INR');
    // => 2

    $roundingIncrement = Intl::getCurrencyBundle()->getRoundingIncrement('INR');
    // => 0

Tutti i metodi (tranne
:method:`Symfony\\Component\\Intl\\ResourceBundle\\CurrencyBundleInterface::getFractionDigits`
e
:method:`Symfony\\Component\\Intl\\ResourceBundle\\CurrencyBundleInterface::getRoundingIncrement`)
accettano il locale come ultimo parametro, opzionale,
con valore predefinito il locale predefinito::

    $currencies = Intl::getCurrencyBundle()->getCurrencyNames('de');
    // => array('AFN' => 'Afghanische Afghani', ...)

Questo è tutto quello che occorre sapere, per ora. Buon divertimento con il codice!

.. _Packagist: https://packagist.org/packages/symfony/intl
.. _componente ICU: https://packagist.org/packages/symfony/icu
.. _estensione intl: http://www.php.net/manual/en/book.intl.php
.. _installare l'estensione intl: http://www.php.net/manual/en/intl.setup.php
.. _libreria ICU: http://site.icu-project.org/
