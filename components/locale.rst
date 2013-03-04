.. index::
   single: Locale
   single: Componenti; Locale

Il componente Locale
====================

    Il componente Locale fornisce codice per gestire casi in cui manchi l'estensione ``intl``.
    Inoltre, estende l'implementazione di una classe nativa :phpclass:`Locale` con diversi metodi.

Viene fornito un rimpiazzo per le seguenti funzioni e classi:

* :phpfunction:`intl_is_failure`
* :phpfunction:`intl_get_error_code`
* :phpfunction:`intl_get_error_message`
* :phpclass:`Collator`
* :phpclass:`IntlDateFormatter`
* :phpclass:`Locale`
* :phpclass:`NumberFormatter`

.. note::

     L'implementazione di stub supporta solo il locale ``en``.

Installazione
-------------

Si può installare il componente in molti modi diversi:

* Usare il repository ufficiale su Git (https://github.com/symfony/Locale);
* Installarlo via Composer (``symfony/locale`` su `Packagist`_).

Uso
---

Tra i vantaggi presenti, è inclusa la richiesta di funzioni stub e l'aggiunta di classi stub all'autoloader.

Quando si usa il componente ClassLoader, il codice seguente basta a fornire l'estensione ``intl`` mancante:

.. code-block:: php

    if (!function_exists('intl_get_error_code')) {
        require __DIR__.'/percorso/src/Symfony/Component/Locale/Resources/stubs/functions.php';

        $loader->registerPrefixFallbacks(
            array(__DIR__.'/percorso/src/Symfony/Component/Locale/Resources/stubs')
        );
    }

La classe :class:`Symfony\\Component\\Locale\\Locale` arricchisce la classe nativa :phpclass:`Locale` con caratteristiche aggiuntive:

.. code-block:: php

    use Symfony\Component\Locale\Locale;

    // Nomi dei paesi per un locale, o tutti i codici dei paesi
    $countries = Locale::getDisplayCountries('pl');
    $countryCodes = Locale::getCountries();

    // Nomi delle lingue per un locale, o tutti i codici delle lingue
    $languages = Locale::getDisplayLanguages('fr');
    $languageCodes = Locale::getLanguages();

    // Nomi dei locale per un dato codice, o tutti i codici dei locale
    $locales = Locale::getDisplayLocales('en');
    $localeCodes = Locale::getLocales();

    // Versioni ICU
    $icuVersion = Locale::getIcuVersion();
    $icuDataVersion = Locale::getIcuDataVersion();

.. _Packagist: https://packagist.org/packages/symfony/locale
