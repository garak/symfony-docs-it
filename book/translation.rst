.. index::
   single: Traduzioni

Traduzioni
==========

Il termine "internazionalizzazione" si riferisce al processo di astrazione delle stringhe 
e altri pezzi specifici dell'applicazione che variano in base al locale, in uno strato
dove possono essere tradotti e convertiti in base alle impostazioni internazionali dell'utente (ad esempio
lingua e paese). Per il testo, questo significa che ognuno viene avvolto con una funzione
capace di tradurre il testo (o "messaggio") nella lingua dell'utente::

    // il testo verrà *sempre* stampato in English
    echo 'Hello World';

    // il testo può essere tradotto nella lingua dell'utente finale o per impostazione predefinita in inglese
    echo $translator->trans('Hello World');

.. note::

    Il termine *locale* si riferisce all'incirca al linguaggio dell'utente e al paese.
    Può essere qualsiasi stringa che l'applicazione utilizza poi per gestire le traduzioni
    e altre differenze di formati (ad esempio il formato di valuta). Si consiglia
    il codice di *lingua* ISO639-1, un carattere di sottolineatura (``_``), poi il codice di *paese* ISO3166
    (per esempio ``fr_FR`` pers francese / Francia).

In questo capitolo, si imparererà a preparare un'applicazione per supportare più
locale e poi a creare le traduzioni per più locale. Nel complesso,
il processo ha diverse fasi comuni:

1. Abilitare e configurare il componente ``Translation`` di Symfony;

2. Astrarre le stringhe (i. "messaggi") avvolgendoli nelle chiamate al ``Traduttore``;

3. Creare risorse di traduzione per ogni lingua supportata che traducano tutti
   i messaggio dell'applicazione;

4. Determinare, impostare e gestire le impostazioni locali dell'utente nella sessione.

.. index::
   single: Translations; Configuration

Configurazione
--------------

Le traduzioni sono gestire da un ``Traduttore`` :term:`service` che utilizza i
locale dell'utente per cercare e restituire i messaggi tradotti. Prima di utilizzarlo,
abilitare il ``Traduttore`` nella configurazione:

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        framework:
            translator: { fallback: en }

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <framework:config>
            <framework:translator fallback="en" />
        </framework:config>

    .. code-block:: php

        // app/config/config.php
        $container->loadFromExtension('framework', array(
            'translator' => array('fallback' => 'en'),
        ));

