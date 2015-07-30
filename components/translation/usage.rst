.. index::
    single: Translation; Uso

Uso di Translator
=================

Si immagini di voler tradurra la stringa *"Symfony is great"* in francese::

    use Symfony\Component\Translation\Translator;
    use Symfony\Component\Translation\Loader\ArrayLoader;

    $translator = new Translator('fr_FR');
    $translator->addLoader('array', new ArrayLoader());
    $translator->addResource('array', array(
        'Symfony is great!' => 'J\'aime Symfony!',
    ), 'fr_FR');
    
    echo $translator->trans('Symfony is great!');

In questo esempio, il messaggio *"Symfony is great!"* sarà tradotto nel
locale impostato nel costruttore (``fr_FR``), se il messaggio esiste in uno
dei cataloghi dei messaggi.

.. _component-translation-placeholders:

Segnaposto dei messaggi
-----------------------

A volte, occorre tradurre un messaggio che contiene una variabile::

    // ...
    $translated = $translator->trans('Hello '.$name);

    echo $translated;

Tuttavia, creare una traduzione per questa stringa è impossibile, perché il traduttore
proverebbe a cercare il messaggio esatto, inclusa la parte variabile
(p.e. *"Hello Ryan"* o *"Hello Fabien"*). Invece di scrivere una traduzione
per ogni possibile occorrenza della variabile ``$name``, si può sostituire
la variabile con un "segnaposto"::

    // ...
    $translated = $translator->trans(
        'Hello %name%',
        array('%name%' => $name)
    );

    echo $translated;

Symfony ora cercherà di tradurre il messaggio grezzo (``Hello %name%``)
e *poi* di sostituire i segnaposto con i rispettivi valori. La creazione di una
traduzione si fa come in precedenza:

.. configuration-block::

    .. code-block:: xml

        <?xml version="1.0"?>
        <xliff version="1.2" xmlns="urn:oasis:names:tc:xliff:document:1.2">
            <file source-language="en" datatype="plaintext" original="file.ext">
                <body>
                    <trans-unit id="1">
                        <source>Hello %name%</source>
                        <target>Bonjour %name%</target>
                    </trans-unit>
                </body>
            </file>
        </xliff>

    .. code-block:: php

        return array(
            'Hello %name%' => 'Bonjour %name%',
        );

    .. code-block:: yaml

        'Hello %name%': Bonjour %name%

.. note::

    I segnaposto posso essere composti a piacere, perché il messaggio viene ricostruito
    usando la funzione :phpfunction:`strtr function<strtr>` di PHP. Tuttavia, la forma ``%...%``
    è quella raccomandata, per evitare problemi con Twig.

Come visto, la creazione di una traduzione è un processo in due fasi:

#. Astrarre il messaggio da tradurre, processandolo tramite
   ``Translator``.

#. Creare una traduzione per il messaggio in ogni locale che si desidera
   supportare.

Il secondo passo si esegue creando cataloghi di messaggi, che definiscono le traduzioni
per qualsiasi numero di diversi locale.

Creare traduzioni
=================

L'atto di creazione dei file di traduzione è una parte importante della "localizzazione"
(spesso abbreviata in `L10n`_). La traduzione dei file consiste in una serie di
copppie id-traduzione per una dato dominio e locale. La sorgente è l'identificativo
della singola traduzione e può essere il messaggio nel locale principale (p.e.
*"Symfony is great"*) dell'applicazione o un'identificativo univoco (p.e.
``symfony.great``, vedere il riquadro sotto).

I file di traduzione possono essere tradotti in vari formati, con XLIFF come
formato raccomandato. Questi file sono analizzati da una delle classi Loader.

.. configuration-block::

    .. code-block:: xml

        <?xml version="1.0"?>
        <xliff version="1.2" xmlns="urn:oasis:names:tc:xliff:document:1.2">
            <file source-language="en" datatype="plaintext" original="file.ext">
                <body>
                    <trans-unit id="1">
                        <source>Symfony is great</source>
                        <target>J'aime Symfony</target>
                    </trans-unit>
                    <trans-unit id="2">
                        <source>symfony.great</source>
                        <target>J'aime Symfony</target>
                    </trans-unit>
                </body>
            </file>
        </xliff>

    .. code-block:: php

        return array(
            'Symfony is great' => 'J\'aime Symfony',
            'symfony.great'    => 'J\'aime Symfony',
        );

    .. code-block:: yaml

        Symfony is great: J'aime Symfony
        symfony.great:    J'aime Symfony

.. sidebar:: Usare messaggi reali o parole chiave

    Questo esempio illustra le due diverse filosofie di creazione di
    messaggi da tradurre::

        $translator->trans('Symfony is great');

        $translator->trans('symfony.great');

    Nel primo metodo, i messaggi sono scritti nella lingua del locale
    predefinito (in questo caso, inglese). Tali messaggi sono quindi usati come "id"
    durante la creazione di traduzioni.

    Nel secondo metodo, i messaggi sono in realtà "parole chiave", che portano
    l'idea del messaggio. Le parole chiave sono quindi usate come "id" per
    ogni traduzione. In questo c aso, occorre tradurre anche per il locale
    predefinito (quindi tradurre ``symfony.great`` in ``Symfony is great``).

    Il secondo metodo è comodo, perché la chiave del messaggio non ha mai bisogno di
    essere cambiata, in ogni file di traduzione, se per esempio si decide di cambiare il messaggio
    in "Symfony is really great" nel locale predefinito.

    La scelta del metodo spetta allo sviluppatore, ma il formato "parola chiave"
    spesso è raccomandato.

    Inoltre, i formati di file ``php`` e ``yaml`` supportano id innestate,
    che evitano di ripetere molte volte la stessa parte degli
    id:

    .. configuration-block::

        .. code-block:: yaml

            symfony:
                is:
                    great: Symfony is great
                    amazing: Symfony is amazing
                has:
                    bundles: Symfony has bundles
            user:
                login: Login

        .. code-block:: php

            array(
                'symfony' => array(
                    'is' => array(
                        'great'   => 'Symfony is great',
                        'amazing' => 'Symfony is amazing',
                    ),
                    'has' => array(
                        'bundles' => 'Symfony has bundles',
                    ),
                ),
                'user' => array(
                    'login' => 'Login',
                ),
            );

    I livelli multipli vengono appiattiti in una singola coppia id/traduzione,
    aggiungendo un punto (``.``) tra ogni livello, quindi l'esempio appena visto è
    equivalente al seguente:

    .. configuration-block::

        .. code-block:: yaml

            symfony.is.great: Symfony is great
            symfony.is.amazing: Symfony is amazing
            symfony.has.bundles: Symfony has bundles
            user.login: Login

        .. code-block:: php

            return array(
                'symfony.is.great'    => 'Symfony is great',
                'symfony.is.amazing'  => 'Symfony is amazing',
                'symfony.has.bundles' => 'Symfony has bundles',
                'user.login'          => 'Login',
            );

.. _component-translation-pluralization:

Pluralizzazione
---------------

La pluralizzazione dei messaggi è un argomento difficile, con regole che possono essere molto complesse.
Per esempio, questa è una rappresentazione matematica delle regole di pluralizzazione
russe::

    (($number % 10 == 1) && ($number % 100 != 11))
        ? 0
        : ((($number % 10 >= 2)
            && ($number % 10 <= 4)
            && (($number % 100 < 10)
            || ($number % 100 >= 20)))
                ? 1
                : 2
    );

Come si può, vedere possono essere tre diverse forme plurali in russo, a cui
si può dare indice 0, 1, 2. Per ciascuna forma, il plurale è diverso e quindi
anche la traduzione è diversa.

Quanto ha una traduzione ha diverse forme di pluralizzazione, si possono
fornire tutte le forme, come stringa separata da barre (``|``)::

    'There is one apple|There are %count% apples'

Per tradurre messaggi pluralizzati, usare il metodo
:method:`Symfony\\Component\\Translation\\Translator::transChoice`::

    $translator->transChoice(
        'There is one apple|There are %count% apples',
        10,
        array('%count%' => 10)
    );

Il secondo parametro (``10`` in questo esempio), è il *numero* di oggetti
descritti e viene usato per determinare la traduzione da usare e anche per popolare
il segnaposto ``%count%``.

In base al numero fornito, il traduttore sceglie la forma corretta di plurale.
In inglese, la maggior parte delle parole ha una forma singolare, quando c'è esattamente un oggetto,
e una forma plurale, per tutti gli altri numeri (0, 2, 3...). Quindi, se ``count`` è
``1``, il traduttore userà la prima stringa (``There is one apple``)
come traduzione. Altrimenti, userà ``There are %count% apples``.

Ecco la traduzione in francese:

.. code-block:: text

    'Il y a %count% pomme|Il y a %count% pommes'

Anche se la stringa sembra simile (è fatta di due sottostringhe separate da una
barra), le regole francesi sono diverse: la prima forma (senza plurale) è usata quando
``count`` è ``0`` o ``1``. Quindi, il traduttore usera automaticamente la
prima stringa (``Il y a %count% pomme``) quando ``count`` è ``0`` o ``1``.

Ogni locale ha il suo insieme di regole, alcuni con fino a sei diverse
forme plurali, con regole complesse in base a cui mappare i numeri alle forme plurali.
Le regole sono semplici per inglese e francese, ma per il russo si potrebbe
volere un suggerimento su quale regola corrisponda a quale stringa. Per aiutare i traduttori,
si può, facoltativamente, assegnare un tag a ogni stringa:

.. code-block:: text

    'uno: There is one apple|some: There are %count% apples'

    'nessuno_o_uno: Il y a %count% pomme|some: Il y a %count% pommes'

I tag sono solo suggerimenti per il traduttore e non hanno effetto sulla logica
usata per determinare la forma plurale da usare. I tag possono essere qualsiasi stringa
descrittiva che finisca con duepunti (``:``). I tag inoltre non devono necessariamente essere
uguali nel messaggio originale e in quello tradotto.

.. tip::

    Essendo i tag opzionali, il traduttore non li usa (il traduttore otterrà
    solo una stringa basata sulle loro posizioni nella stringa).

Intervallo di pluralizzazione esplicito
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Il modo più facile per pluralizzare un messaggio è lasciare che il traduttore usi
la logica interna per scegliere la stringa da usare in base al numero dato. A volte,
sarà necessario maggior controllo o si vorrà una traduzione diversa per casi specifici (per
``0`` o quanto il conteggio è negativo, per esempio). Per questi casi, si possono usare
intervalli matematici espliciti:

.. code-block:: text

    '{0} There are no apples|{1} There is one apple|]1,19] There are %count% apples|[20,Inf] There are many apples'

Gli intervalli seguono la notazione `ISO 31-11`. La stringa vista sopra specifica
quattro diversi intervalli: esattamente ``0``, esattamente ``1``, ``2-19`` e da ``20``
in poi.

Si possono anche mischiare regole matematiche espliciti e regole standard. In questo caso, se
il conteggio non corrisponde a un intervallo specifico, le regole standard hanno
effetto dopo la rimozione delle regole esplicite:

.. code-block:: text

    '{0} There are no apples|[20,Inf] There are many apples|There is one apple|a_few: There are %count% apples'

Per esempio, per ``1`` mela, sarà usata la regola standard ``There is one apple``.
Per ``2-19`` mele, sarà scelta la seconda regola standard, ``There are %count%
apples``.

Un oggetto :class:`Symfony\\Component\\Translation\\Interval` può rappresentare un insieme finito
di numeri:

.. code-block:: text

    {1,2,3,4}

Oppure numeri compresi tra due numeri:

.. code-block:: text

    [1, +Inf[
    ]-1,2[

Il delimitatore sinistro può essere ``[`` (inclusivo) o ``]`` (esclusivo). Il
delimitatore destro può essere ``[`` (esclusivo) o ``]`` (inclusivo). Oltre ai numeri, si
possono usare ``-Inf`` e ``+Inf`` per l'infinito.

Forzare il locale in Translator
-------------------------------

Durante la traduzione di un messaggio, Translator usa il locale specificato o il locale
``fallback``, se necessario. Si può anche specificare manualmente il locale
da usare::

    $translator->trans(
        'Symfony is great',
        array(),
        'messages',
        'fr_FR'
    );

    $translator->transChoice(
        '{0} There are no apples|{1} There is one apple|]1,Inf[ There are %count% apples',
        10,
        array('%count%' => 10),
        'messages',
        'fr_FR'
    );

.. _`L10n`: http://en.wikipedia.org/wiki/Internationalization_and_localization
.. _`ISO 31-11`: http://en.wikipedia.org/wiki/Interval_(mathematics)#Notations_for_intervals
