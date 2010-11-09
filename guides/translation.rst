Traduzioni
==========

Il componente :namespace:`Symfony\\Component\\Translation` fornisce un modo per
internazionalizzare tutti i messaggi dell'applicazione.

Un *messaggio* può essere una qualunque stringa che si desidera internazionalizzare. I messaggi
sono caratterizzati da una località e un dominio.

Un *dominio* permette di organizzare al meglio i messaggi di una data località (può
essere una stringa qualunque; per impostazione predefinita, tutti i messaggi sono memorizzati nel
dominio ``messages``).

Una *località* può essere una stringa qualunque, ma è raccomandato l'utilizzo di un codice lingua ISO639-1,
 seguito da una linea di sottolineatura (``_``), seguito da un codice nazione ISO3166
(ad esempio, usare ``fr_FR`` per Francese/Francia).

Configurazione
--------------

Prima di poter utilizzare le funzionalità per le traduzioni, bisogna abilitarle nella propria configurazione:

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        app.config:
            translator: { fallback: en }

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <app:config>
            <app:translator fallback="en" />
        </app:config>

    .. code-block:: php

        // app/config/config.php
        $container->loadFromExtension('app', 'config', array(
            'translator' => array('fallback' => 'en'),
        ));

L'attributo ``fallback`` definisce la località che deve essere restituita quando una traduzione
non esiste nella località dell'utente.

.. tip::
   Quando per una località non esiste una traduzione, il traduttore prova a trovare
   la traduzione per la lingua (ad esempio ``fr`` quando la località è ``fr_FR``);
   se anche questa ricerca fallisce, cerca una traduzione per la località indicata da
   fallback.

La località utilizzata nelle traduzioni è l'unica memorizzata nella sessione utente.

Traduzioni
----------

Le traduzioni sono disponibili attraverso il servizio ``translator``
(:class:`Symfony\\Component\\Translation\\Translator`). Utilizzare il metodo
:method:`Symfony\\Component\\Translation\\Translator::trans` per
tradurre un messaggio::

    $t = $this['translator']->trans('Symfony2 è grande!');

Se nelle stringhe ci sono dei segnaposto, bisogna passare i loro valori come secondo argomento::

    $t = $this['translator']->trans('Symfony2 is {{ what }}!', array('{{ what }}' => 'great'));

.. note::
   I segnaposto possono avere qualunque formato, ma utilizzando la notazione ``{{ var }}``
   consente al messaggio di essere utilizato nei template realizzati con Twig.

Per impostazione predefinita, il traduttore cerca i messaggi nel dominio predefinito ``messages``.
 Si può sovrascriverlo attraverso il terzo argomento::

    $t = $this['translator']->trans('Symfony2 is great!', array(), 'applications');

Cataloghi
---------

Le traduzioni sono memorizzate nel filesystem e vengono trovate da Symfony2, grazie ad
alcune convenzioni.

Memorizzare le traduzioni per i messaggi trovate in un bundle sotto la
cartella ``Resources/translations/``; e sovrascriverle sotto la cartella
``app/translations/``.

Ogni file per messaggi deve essere nominato secondo il seguente schema:
``domain.locale.loader`` (il nome del dominio, seguito da un punto (``.``), seguito
dal nome della località, seguito da un punto (``.``), seguito dal nome del loader.)

Il loader può essere il nome di qualunque loader registrato. Per impostazione predefinita,
Symfony2 fornisce i seguenti loader:

* ``php``:   PHP file;
* ``xliff``: XLIFF file;
* ``yaml``:  YAML file.

Ogni file è costituito da coppie di stringhe id/traduzione per il dato dominio
e località. L'ID può essere il messaggio nella località principale dell'applicazione
di un identificatore univoco:

.. configuration-block::

    .. code-block:: xml

        <?xml version="1.0"?>
        <xliff version="1.2" xmlns="urn:oasis:names:tc:xliff:document:1.2">
            <file source-language="en" datatype="plaintext" original="file.ext">
                <body>
                    <trans-unit id="1">
                        <source>Symfony2 is great</source>
                        <target>J'aime Symfony2</target>
                    </trans-unit>
                    <trans-unit id="2">
                        <source>symfony2.great</source>
                        <target>J'aime Symfony2</target>
                    </trans-unit>
                </body>
            </file>
        </xliff>

    .. code-block:: php

        return array(
            'Symfony2 is great' => 'J\'aime Symfony2',
            'symfony2.great'     => 'J\'aime Symfony2',
        );

.. note::
   Si possono anche memorizzare le traduzioni in un database, o qualsiasi altro dispositivo
   di archiviazione fornendo una classe presonalizzata che implementa l'interfaccia
   :class:`Symfony\\Component\\Translation\\Loader\\LoaderInterface`.
   Vedere sotto per capire come registrare loader personalizzati.

Pluralizzazione
---------------

La pluralizzazione dei messaggi è un argomento difficile dal momento che le regole possono essere
complesse. Ad esempio, questa è la rappresentazione matematica delle regole di pluralizzazione
russe::

    (($number % 10 == 1) && ($number % 100 != 11)) ? 0 : ((($number % 10 >= 2) && ($number % 10 <= 4) && (($number % 100 < 10) || ($number % 100 >= 20))) ? 1 : 2);

Come si può vedere, si possono avere tre diverse forme plurali sulla base di questo
algoritmo. Per ciascuna forma, il plurale è diverso, e quindi anche la traduzione è
diversa. In tal caso, è possibile fornire tutte le forme di pluralizzazione come
stringhe separate dal simbolo pipe di barra verticale (``|``)::

    'C\'è una mela|Ci sono {{ count }} mele'

Sulla base di un dato numero, il traduttore sceglie la giusta forma plurale.
Se ``count`` è ``1``, il traduttore userà come traduzione la prima stringa
 (``C\'è una mela``), altrimenti userà Ci sono {{ count }} mele.

Di seguito la traduzione in francese::

    'Il y a {{ count }} pomme|Il y a {{ count }} pommes'

Anche se la stringa sembra simile (è fatta da due sotto stringhe separate da
una barra verticale), in francese le regole sono differenti: la prima forma (senza plurale) è usata quando
``count`` è ``0`` o ``1``. Quindi il traduttore utilizzerà automaticamente la
prima stringa (``Il y a {{ count }} pomme``) quando ``count`` è ``0`` o ``1``.

Le regole sono abbastanza semplici per le lingue inglese e francese, ma per il russo
è meglio avere un suggerimento per sapere quale regola è in accordo con la stringa.
Per aiutare i traduttori, si può, opzionalmente "etichettare" ciascuna stringa in questo modo::

    'one: There is one apple|some: There are {{ count }} apples'

    'none_or_one: Il y a {{ count }} pomme|some: Il y a {{ count }} pommes'

Le etichette sono davvero solo note per i traduttori, per aiutarli a capire il
contesto della traduzione (si noti che le etichette non hanno bisogno di essere le stesse nel
messaggio originale e in quello tradotto).

.. tip:
   Poiché le etichette sono opzionali, il traduttore non li usa (il traduttore
   otterrà solo una stringa in base alla sua posizione nella stringa).

A volte, si vuole una traduzione diversa per casi specifici (per ``0``, o
per quando il contatore ha un valore abbastanza grande, quando il contatore è
negativo, ...). Per tali casi, si possono usare intervalli matematici espliciti::

    '{0} Non ci sono mele|{1} C\'è una mela|]1,19] Ci sono {{ count }} mele|[20,Inf] Ci sono molte mele'

Si possono anche mischiare regole matematiche esplicite con regole di base. La 
posizione per le regole di base è definita dopo aver rimosso le regole esplicite::

    '{0} Non ci sono mele|[20,Inf] Ci sono molte mele|C\'è una mela|a_few: Ci sono {{ count }} mele'

:class:`Symfony\\Component\\Translation\\Interval` può rappresentare un insieme finito di numeri::

    {1,2,3,4}

O numeri compresi tra due altri numeri::

    [1, +Inf[
    ]-1,2[

Il delimitatore a sinistra può essere ``[`` (compreso) o ``]`` (escluso). Il delimitatore
a destra può essere ``[`` (escluso) o ``]`` (compreso). Oltre ai numeri, si
può usare ``-Inf`` e ``+Inf`` per l'infinito.

.. note::
   Symfony2 utilizza lo standard `ISO 31-11`_ per la notazione degli intervalli.

Il metodo traduttore
:method:`Symfony\\Component\\Translation\\Translator::transChoice`
sa come trattare i plurali::

    $t = $this['translator']->transChoice(
        '{0} There is no apples|{1} There is one apple|]1,Inf[ There are {{ count }} apples',
        10,
        array('{{ count }}' => 10)
    );

Notare che il secondo argomento è il numero da usare per determinare quale stringa utilizzare
per il plurale.

Traduzioni nei template
-----------------------

La maggior parte delle volte, la traduzione avviene nei template. Symfony2 fornisce un supporto
nativo per i template PHP e Twig.

Template PHP
~~~~~~~~~~~~

Il servizio di traduzione è disponibile nei template PHP templates atrtaverso
l'helper ``translator``:

.. code-block:: html+php

    <?php echo $view['translator']->trans('Symfony2 is great!') ?>

    <?php echo $view['translator']->transChoice(
        '{0} There is no apples|{1} There is one apple|]1,Inf[ There are {{ count }} apples',
        10,
        array('{{ count }}' => 10)
    ) ?>

Template Twig
~~~~~~~~~~~~~

Symfony2 fornisce appositi tag per Twig (``trans`` e ``transChoice``) utili
per la traduzione dei messaggi:

.. code-block:: jinja

    {% trans "Symfony2 è grande!" %}

    {% trans %}
        Foo {{ name }}
    {% endtrans %}

    {% transchoice count %}
        {0} Non ci sono mele|{1} C\'è una mela|]1,Inf] Ci sono {{ count }} mele
    {% endtranschoice %}

Il tag ``transChoice`` recupera automaticamente le variabili dal contesto corrente
e le passa al traduttore. Questo meccanismo funziona solo quando si usano
segnaposto che utilizzano lo schema ``{{ var }}``.

Si può anche specificare il dominio del messaggio:

.. code-block:: jinja

    {% trans "Foo {{ name }}" from app %}

    {% trans from app %}
        Foo {{ name }}
    {% endtrans %}

    {% transchoice count from app %}
        {0} non ci sono mele|{1} C'è una mela|]1,Inf] Ci sono {{ count }} mele
    {% endtranschoice %}

.. _translation_loader_tag:

Abilitare loader personalizzati
-------------------------------

Per abilitare un loader personalizzato, aggiungerlo come normale servizio in una
delle proprie configurazioni, etichettarlo con ``translation.loader`` e definire
un attributo ``alias`` (per loader basati su filesystem, l'alias è l'estensione
del file che bisogna usare per fare riferimento al loader):

.. configuration-block::

    .. code-block:: yaml

        services:
            translation.loader.your_helper_name:
                class: Fully\Qualified\Loader\Class\Name
                tags:
                    - { name: translation.loader, alias: alias_name }

    .. code-block:: xml

        <service id="translation.loader.your_helper_name" class="Fully\Qualified\Loader\Class\Name">
            <tag name="translation.loader" alias="alias_name" />
        </service>

    .. code-block:: php

        $container
            ->register('translation.loader.your_helper_name', 'Fully\Qualified\Loader\Class\Name')
            ->addTag('translation.loader', array('alias' => 'alias_name'))
        ;

.. _ISO 31-11: http://en.wikipedia.org/wiki/Interval_%28mathematics%29#The_ISO_notation
