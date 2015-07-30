.. index::
   single: Traduzioni

Traduzioni
==========

Il termine "internazionalizzazione" si riferisce al processo di astrazione delle stringhe 
e altri pezzi specifici dell'applicazione che variano in base al locale, in uno strato
dove possono essere tradotti e convertiti in base alle impostazioni internazionali dell'utente (ad esempio
lingua e paese). Per il testo, questo significa che ognuno viene avvolto con una funzione
capace di tradurre il testo (o "messaggio") nella lingua
dell'utente::

    // il testo verrà *sempre* stampato in inglese
    echo 'Hello World';

    // il testo può essere tradotto nella lingua dell'utente finale o
    // restare in inglese
    echo $translator->trans('Hello World');

.. note::

    Il termine *locale* si riferisce all'incirca al linguaggio dell'utente e al paese.
    Può essere qualsiasi stringa che l'applicazione utilizza poi per gestire le traduzioni
    e altre differenze di formati (ad esempio il formato di valuta). Si consiglia di utilizzare il codice di *lingua* `ISO 639-1`_,
    un carattere di sottolineatura (``_``), poi il codice di *paese* `ISO 3166-1 alpha-2`_
    (per esempio ``fr_FR`` per francese/Francia).

In questo capitolo si imparerà a usare il componente Translation nel
framework Symfony. Si può leggere la
:doc:`documentazione del componente Translation </components/translation/usage>`
per saperne di più. Nel complesso, il processo ha diverse fasi:

#. :ref:`Abilitare e configurare<book-translation-configuration>` il servizio
   translation di Symfony;

#. Astrarre le stringhe (i. "messaggi") avvolgendoli nelle chiamate al
   ``Translator`` (":ref:`book-translation-basic`");

#. :ref:`Creare risorse di traduzione <book-translation-resources>`
   per ogni lingua supportata che traducano tutti i messaggio dell'applicazione;

#. Determinare, :ref:`impostare e gestire le impostazioni locali <book-translation-user-locale>`
   dell'utente per la richiesta e, facoltativamente,
   :doc:`sull'intera sessione </cookbook/session/locale_sticky_session>`.

.. _book-translation-configuration:

Configurazione
--------------

Le traduzioni sono gestire da un :term:`servizio` ``translator``, che utilizza i
locale dell'utente per cercare e restituire i messaggi tradotti. Prima di utilizzarlo,
abilitare ``translator`` nella configurazione:

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        framework:
            translator: { fallbacks: [en] }

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <?xml version="1.0" encoding="UTF-8" ?>
        <container xmlns="http://symfony.com/schema/dic/services"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xmlns:framework="http://symfony.com/schema/dic/symfony"
            xsi:schemaLocation="http://symfony.com/schema/dic/services
                http://symfony.com/schema/dic/services/services-1.0.xsd
                http://symfony.com/schema/dic/symfony
                http://symfony.com/schema/dic/symfony/symfony-1.0.xsd">

            <framework:config>
                <framework:translator>
                    <framework:fallback>en</framework:fallback>
                </framework:translator>
            </framework:config>
        </container>

    .. code-block:: php

        // app/config/config.php
        $container->loadFromExtension('framework', array(
            'translator' => array('fallbacks' => array('en')),
        ));

Vedere :ref:`book-translation-fallback` per dettagli sulla voce ``fallbacks``
e su cosa faccia Symfony quando non trova una traduzione.

Il locale usato nelle traduzioni è quello memorizzato nella richiesta. Tipicamente,
è impostato tramite un attributo ``_locale`` in una rotta (vedere :ref:`book-translation-locale-url`).

.. _book-translation-basic:

Traduzione di base
------------------

La traduzione del testo è fatta attraverso il servizio ``translator``
(:class:`Symfony\\Component\\Translation\\Translator`). Per tradurre un blocco
di testo (chiamato *messaggio*), usare il metodo
:method:`Symfony\\Component\\Translation\\Translator::trans`. Supponiamo,
ad esempio, che stiamo traducendo un semplice messaggio all'interno del controllore::

    // ...
    use Symfony\Component\HttpFoundation\Response;

    public function indexAction()
    {
        $translated = $this->get('translator')->trans('Symfony is great');

        return new Response($translated);
    }

.. _book-translation-resources:

Quando questo codice viene eseguito, Symfony tenterà di tradurre il messaggio
"Symfony is great" basandosi sul locale dell'utente. Perché questo funzioni,
bisogna dire a Symfony come tradurre il messaggio tramite una "risorsa di
traduzione", che è una raccolta di traduzioni dei messaggi per un dato locale.
Questo "dizionario" delle traduzioni può essere creato in diversi formati,
ma XLIFF è il formato raccomandato:

.. configuration-block::

    .. code-block:: xml

        <!-- messages.fr.xlf -->
        <?xml version="1.0"?>
        <xliff version="1.2" xmlns="urn:oasis:names:tc:xliff:document:1.2">
            <file source-language="en" datatype="plaintext" original="file.ext">
                <body>
                    <trans-unit id="1">
                        <source>Symfony is great</source>
                        <target>J'aime Symfony</target>
                    </trans-unit>
                </body>
            </file>
        </xliff>

    .. code-block:: yaml

        # messages.fr.yml
        Symfony is great: J'aime Symfony

    .. code-block:: php

        // messages.fr.php
        return array(
            'Symfony is great' => 'J\'aime Symfony',
        );

Per informazioni sulla posizione di questi file, vedere
:ref:`book-translation-resource-locations`.

Ora, se la lingua del locale dell'utente è il francese (per esempio ``fr_FR`` o ``fr_BE``),
il messaggio sarà tradotto in ``J'aime Symfony``. Si può anche tradurre il
messaggio da un :ref:`template <book-translation-tags>`.

Il processo di traduzione
~~~~~~~~~~~~~~~~~~~~~~~~~

Per tradurre il messaggio, Symfony utilizza un semplice processo:

* Viene determinato il ``locale`` dell'utente corrente, che è memorizzato nella richiesta;

* Un catalogo di messaggi tradotti viene caricato dalle risorse di traduzione definite
  per il ``locale`` (ad es. ``fr_FR``). Vengono anche caricati i messaggi dal 
  :ref:`locale predefinito <book-translation-fallback>` e aggiunti  
  al catalogo, se non esistono già. Il risultato finale è un grande
  "dizionario" di traduzioni;

* Se il messaggio si trova nel catalogo, viene restituita la traduzione. Se
  no, il traduttore restituisce il messaggio originale.

Quando si usa il metodo ``trans()``, Symfony cerca la stringa esatta all'interno
del catalogo dei messaggi e la restituisce (se esiste).

Segnaposto per i messaggi
~~~~~~~~~~~~~~~~~~~~~~~~~

A volte, un messaggio da tradurre contiene una variabile::

    use Symfony\Component\HttpFoundation\Response;

    public function indexAction($name)
    {
        $translated = $this->get('translator')->trans('Hello '.$name);

        return new Response($translated);
    }

Tuttavia, la creazione di una traduzione per questa stringa è impossibile, poiché il traduttore
proverà a cercare il messaggio esatto, includendo le parti con le variabili
(per esempio "Hello Ryan" o "Hello Fabien").

Per dettagli su come gestire questa situazione, vedere :ref:`component-translation-placeholders`
nella documentazione del componente. Per i template, vedere :ref:`book-translation-tags`.

Pluralizzazione
---------------

Un'ulteriore complicazione si presenta con traduzioni che possono essere plurali o
meno, in base a una qualche variabile:

.. code-block:: text

    There is one apple.
    There are 5 apples.

Per poterlo gestire, usare il metodo :method:`Symfony\\Component\\Translation\\Translator::transChoice`
del tag o del filtro ``transchoice`` nel :ref:`template <book-translation-tags>`.

Per ulteriori e approfondite informazioni, vedere :ref:`component-translation-pluralization`
nella documentazione del componente Translation.

Traduzioni nei template
-----------------------

Le traduzioni avvengono quasi sempre all'interno di template. Symfony fornisce un supporto
nativo sia per i template Twig che per quelli PHP.

.. _book-translation-tags:

Template Twig
~~~~~~~~~~~~~

Symfony fornisce tag specifici per Twig (``trans`` e ``transchoice``), che aiutano
nella traduzioni di messaggi di *blocchi statici di testo*:

.. code-block:: jinja

    {% trans %}Hello %name%{% endtrans %}

    {% transchoice count %}
        {0} There are no apples|{1} There is one apple|]1,Inf] There are %count% apples
    {% endtranschoice %}

Il tag ``transchoice`` prende in automatico la variabile ``%count%`` dal contesto
e la passa al traduttore. Questo meccanismo funziona solo
usando un segnaposto che segue lo schema ``%variabile%``.

.. caution::

    La notazione ``%variabile%`` dei segnaposti è obbligatoria quando si traduce in un
    template Twig usando il tag.

.. tip::

    Se si deve usare un simbolo di percentuale (``%``) in una stringa, occorre
    raddoppiarlo: ``{% trans %}Percent: %percent%%%{% endtrans %}``

Si può anche specificare il dominio del messaggio e passare variabili aggiuntive:

.. code-block:: jinja

    {% trans with {'%name%': 'Fabien'} from "app" %}Hello %name%{% endtrans %}

    {% trans with {'%name%': 'Fabien'} from "app" into "fr" %}Hello %name%{% endtrans %}

    {% transchoice count with {'%name%': 'Fabien'} from "app" %}
        {0} %name%, there are no apples|{1} %name%, there is one apple|]1,Inf] %name%, there are %count% apples
    {% endtranschoice %}

.. _book-translation-filters:

I filtri ``trans`` e ``transchoice`` possono essere usati per tradurre *testi
variabili* ed espressioni complesse:

.. code-block:: jinja

    {{ message|trans }}

    {{ message|transchoice(5) }}

    {{ message|trans({'%name%': 'Fabien'}, "app") }}

    {{ message|transchoice(5, {'%name%': 'Fabien'}, 'app') }}

.. tip::

    L'uso dei tag o dei filtri di traduzione ha il medesimo effetto, ma con una
    sottile differenza: l'escape automatico si applica solo alla traduzione
    che usa un filtro. In altre parole, se ci si deve assicurare che
    il testo tradotto *non* abbia escape, occorre applicare il filtro
    ``raw`` dopo il filtro di traduzione:

    .. code-block:: jinja

            {# il testo tra tag non subisce escape #}
            {% trans %}
                <h3>foo</h3>
            {% endtrans %}

            {% set message = '<h3>foo</h3>' %}

            {# stringhe e variabili tradotte con filtro subiscono escape #}
            {{ message|trans|raw }}
            {{ '<h3>bar</h3>'|trans|raw }}

.. tip::

    Si può impostare il dominio di un intero template Twig con un semplice tag:

    .. code-block:: jinja

           {% trans_default_domain "app" %}

    Notare che questo influenza solo in template attuale, non i template "inclusi"
    (per evitare effetti collaterali).

.. versionadded:: 2.1
    Il tag ``trans_default_domain`` è nuovo in Symfony2.1

Template PHP
~~~~~~~~~~~~

Il servizio di traduzione è accessibile nei template PHP attraverso
l'aiutante ``translator``:

.. code-block:: html+php

    <?php echo $view['translator']->trans('Symfony is great') ?>

    <?php echo $view['translator']->transChoice(
        '{0} There are no apples|{1} There is one apple|]1,Inf[ There are %count% apples',
        10,
        array('%count%' => 10)
    ) ?>

.. _book-translation-resource-locations:

Sedi per le traduzioni e convenzioni sui nomi
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Symfony cerca i file dei messaggi (ad esempio le traduzioni) in due sedi:

* la cartella ``app/Resources/translations``;

* la cartella ``app/Resources/<nome bundle>/translations``;

* la cartella ``Resources/translations/`` del bundle.

I posti sono elencati in ordine di priorità. Quindi, si possono sovrascrivere i
messaggi di traduzione di un bundle in una qualsiasi delle due cartelle superiori.

Il meccanismo di priorità si basa sulle chiavi: occorre dichiarare solamente le chiavi
da sovrascrivere in un file di messaggi a priorità superiore. Se una chiave non viene trovata
in un file di messaggi, il traduttore si appoggerà automaticamente ai file di messaggi
a priorità inferiore.

È importante anche il nome del file con le traduzioni: ogni file con i messaggi
deve essere nominato secondo il seguente schema: ``dominio.locale.caricatore``:

* **dominio**: Un modo opzionale per organizzare i messaggi in gruppi (ad esempio ``admin``,
  ``navigation`` o il predefinito ``messages``, vedere ":ref:`using-message-domains`");

* **locale**: Il locale per cui sono state scritte le traduzioni (ad esempio ``en_GB``, ``en``, ecc.);

* **caricatore**: Come Symfony dovrebbe caricare e analizzare il file (ad esempio ``xliff``,
  ``php`` o ``yml``).

Il caricatore può essere il nome di un qualunque caricatore registrato. Per impostazione predefinita, Symfony
fornisce i seguenti caricatori:

* ``xliff``: file XLIFF;
* ``php``:  file PHP;
* ``yml``:  file YAML.

La scelta di quali caricatori utilizzare è interamente a carico dello sviluppatore ed è una questione
di gusti. L'opzione raccomandata è l'uso di ``xliff`` per le  traduzioni.
Per ulteriori opzioni, vedere :ref:`component-translator-message-catalogs`.

.. note::

    È anche possibile memorizzare le traduzioni in una base dati  o in qualsiasi altro mezzo,
    fornendo una classe personalizzata che implementa
    l'interfaccia :class:`Symfony\\Component\\Translation\\Loader\\LoaderInterface`.
    Vedere :ref:`dic-tags-translation-loader` per maggiori informazioni.

.. caution::

    Ogni volta che si crea una *nuova* risorsa di traduzione (o si installa un bundle
    che include risorse di traduzioni), assicurarsi di pulire la cache, in modo
    che Symfony possa rilevare le nuove risorse:

    .. code-block:: bash

        $ php app/console cache:clear

.. _book-translation-fallback:

Fallback e locale predefinito
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Ipotizzando che il locale dell'utente sia ``fr_FR`` e che si stia traducendo la
chiave ``Symfony is great``. Per trovare la traduzione francese, Symfony
verifica le  risorse di traduzione di vari locale:

#. Prima, Symfony cerca la traduzione in una risorsa di traduzione ``fr_FR``
   (p.e. ``messages.fr_FR.xfliff``);

#. Se non la trova, Symfony cerca una traduzione per una risorsa di traduzione ``fr``
   (p.e. ``messages.fr.xliff``);

#. Se non trova nemmeno questa, Symfony usa il parametro di configurazione ``fallback``,
   che ha come valore predefinito ``en`` (vedere `Configurazione`_).

.. _book-translation-user-locale:

Gestire il locale dell'utente
-----------------------------

Il locale dell'utente attuale è memorizzato nella richiesta e accessibile
tramite l'oggetto ``request``::

    use Symfony\Component\HttpFoundation\Request;

    public function indexAction(Request $request)
    {
        $locale = $request->getLocale();
    }

Per impostare il locale dell'utente, si potrebbe voler creare un ascoltatore di eventi personalizzato,
in modo che sia impostato prima che altre parti del sistema (come il traduttore)
ne abbiano bisogno::

        public function onKernelRequest(GetResponseEvent $event)
        {
            $request = $event->getRequest();

            // della logica che determina $locale
            $request->getSession()->set('_locale', $locale);
        }

Leggere :doc:`/cookbook/session/locale_sticky_session` per approfondimenti sull'argomento.

.. note::

    Se si usa ``$request->setLocale()`` in un controllore, è troppo tardi
    per influenzare il traduttore. Si deve impostare il locale tramite un ascoltatore
    (vedere sopra), l'URL (vedere avanti) o richiamare ``setLocale()`` direttamente sul
    servizio ``translator``.

Vedere la sezione seguente, :ref:`book-translation-locale-url`, per impostare il
locale tramite rotte.

.. _book-translation-locale-url:

Il locale e gli URL
~~~~~~~~~~~~~~~~~~~

Dal momento che si può memorizzare il locale dell'utente nella sessione, si può essere tentati
di utilizzare lo stesso URL per visualizzare una risorsa in più lingue in base
al locale dell'utente. Per esempio, ``http://www.example.com/contact`` può
mostrare contenuti in inglese per un utente e in francese per un altro. Purtroppo
questo viola una fondamentale regola del web: un particolare URL deve restituire
la stessa risorsa indipendentemente dall'utente. Inoltre, quale
versione del contenuto dovrebbe essere indicizzata dai motori di ricerca?

Una politica migliore è quella di includere il locale nell'URL. Questo è completamente
supportato dal sistema delle rotte utilizzando il parametro speciale ``_locale``:

.. configuration-block::

    .. code-block:: yaml

        # app/config/routing.yml
        contact:
            path:     /{_locale}/contact
            defaults: { _controller: AppBundle:Contact:index }
            requirements:
                _locale: en|fr|de

    .. code-block:: xml

        <!-- app/config/routing.xml -->
        <?xml version="1.0" encoding="UTF-8" ?>
        <routes xmlns="http://symfony.com/schema/routing"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://symfony.com/schema/routing
                http://symfony.com/schema/routing/routing-1.0.xsd">

            <route id="contact" path="/{_locale}/contact">
                <default key="_controller">AppBundle:Contact:index</default>
                <requirement key="_locale">en|fr|de</requirement>
            </route>
        </routes>

    .. code-block:: php

        // app/config/routing.php
        use Symfony\Component\Routing\RouteCollection;
        use Symfony\Component\Routing\Route;

        $collection = new RouteCollection();
        $collection->add('contact', new Route(
            '/{_locale}/contact',
            array(
                '_controller' => 'AppBundle:Contact:index',
            ),
            array(
                '_locale'     => 'en|fr|de',
            )
        ));

        return $collection;

Quando si utilizza il parametro speciale `_locale` in una rotta, il locale corrispondente
verrà *automaticamente impostato sulla richiesta* e potrà essere recuperate tramite il metodo
:method:`Symfony\\Component\\HttpFoundation\\Request::getLocale`.
In altre parole, se un utente
visita l'URI ``/fr/contact``, il locale ``fr`` viene impostato automaticamente
come locale per la richiesta corrente.

È ora possibile utilizzare il locale dell'utente per creare rotte ad altre pagine tradotte
nell'applicazione.

.. tip::

    Leggere :doc:`/cookbook/routing/service_container_parameters` per imparare come
    evitare di inserire manualmente il requisito ``_locale`` in ogni rotta.

.. index::
   single: Traduzioni; Rimandare al locale predefinito

.. _book-translation-default-locale:

Impostare un locale predefinito
-------------------------------

Che fare se non si è in grado di determinare il locale dell'utente? Si può garantire che
un locale sia impostato a ogni richiesta, definendo un ``default_locale`` per
il framework:

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        framework:
            default_locale: en

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <?xml version="1.0" encoding="UTF-8" ?>
        <container xmlns="http://symfony.com/schema/dic/services"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xmlns:framework="http://symfony.com/schema/dic/symfony"
            xsi:schemaLocation="http://symfony.com/schema/dic/services
                http://symfony.com/schema/dic/services/services-1.0.xsd
                http://symfony.com/schema/dic/symfony
                http://symfony.com/schema/dic/symfony/symfony-1.0.xsd">

            <framework:config default-locale="en" />
        </container>

    .. code-block:: php

        // app/config/config.php
        $container->loadFromExtension('framework', array(
            'default_locale' => 'en',
        ));

.. versionadded:: 2.1
     Il parametro ``default_locale`` era in precedenza definito sotto la chiave ``session``,
     ma è stato spostato a partire dalla versione 2.1. Questo perché ora il
     locale è impostato nella richiesta, non più nella sessione.

.. _book-translation-constraint-messages:

Tradurre i messaggi dei vincoli
-------------------------------

Se si usano i vincoli di validazione dei form, la traduzione dei
messaggi di errore è facile: basta creare una risorsa di traduzione per
il :ref:`dominio <using-message-domains>` ``validators``.

Per iniziare, supponiamo di aver creato un oggetto PHP, necessario da
qualche parte in un'applicazione::

    // src/AppBundle/Entity/Author.php
    namespace AppBundle\Entity;

    class Author
    {
        public $name;
    }

Aggiungere i vincoli tramite uno dei metodi supportati. Impostare l'opzione del messaggio
al testo sorgente della traduzione. Per esempio, per assicurarsi che la proprietà ``$name``
non sia vuota, aggiungere il seguente:

.. configuration-block::

    .. code-block:: php-annotations

        // src/AppBundle/Entity/Author.php
        use Symfony\Component\Validator\Constraints as Assert;

        class Author
        {
            /**
             * @Assert\NotBlank(message = "author.name.not_blank")
             */
            public $name;
        }

    .. code-block:: yaml

        # src/AppBundle/Resources/config/validation.yml
        AppBundle\Entity\Author:
            properties:
                name:
                    - NotBlank: { message: "author.name.not_blank" }

    .. code-block:: xml

        <!-- src/AppBundle/Resources/config/validation.xml -->
        <?xml version="1.0" encoding="UTF-8" ?>
        <constraint-mapping xmlns="http://symfony.com/schema/dic/constraint-mapping"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://symfony.com/schema/dic/constraint-mapping
                http://symfony.com/schema/dic/constraint-mapping/constraint-mapping-1.0.xsd">

            <class name="AppBundle\Entity\Author">
                <property name="name">
                    <constraint name="NotBlank">
                        <option name="message">author.name.not_blank</option>
                    </constraint>
                </property>
            </class>
        </constraint-mapping>

    .. code-block:: php

        // src/AppBundle/Entity/Author.php

        // ...
        use Symfony\Component\Validator\Mapping\ClassMetadata;
        use Symfony\Component\Validator\Constraints\NotBlank;

        class Author
        {
            public $name;

            public static function loadValidatorMetadata(ClassMetadata $metadata)
            {
                $metadata->addPropertyConstraint('name', new NotBlank(array(
                    'message' => 'author.name.not_blank',
                )));
            }
        }

Creare un file di traduzione sotto il catalogo ``validators`` per i messaggi
dei vincoli, tipicamente nella cartella ``Resources/translations/`` del
bundle.

.. configuration-block::

    .. code-block:: xml

        <!-- validators.it.xlf -->
        <?xml version="1.0"?>
        <xliff version="1.2" xmlns="urn:oasis:names:tc:xliff:document:1.2">
            <file source-language="en" datatype="plaintext" original="file.ext">
                <body>
                    <trans-unit id="1">
                        <source>author.name.not_blank</source>
                        <target>Inserire un nome per l'autore.</target>
                    </trans-unit>
                </body>
            </file>
        </xliff>

    .. code-block:: yaml

        # validators.it.yml
        author.name.not_blank: Inserire un nome per l'autore.

    .. code-block:: php

        // validators.it.php
        return array(
            'author.name.not_blank' => 'Inserire un nome per l\'autore.',
        );

Tradurre contenuti della base dati
----------------------------------

La traduzione di contenuti della base dati andrebbe affidata a Doctrine, tramite
l'`estensione Translatable`_ o il `behavior Translatable`_ (per PHP 5.4+).
Per maggiori informazioni, vedere la documentazione di queste librerie.

Riepilogo
---------

Con il componente Translation di Symfony, la creazione e l'internazionalizzazione di applicazioni
non è più un processo doloroso	e si riduce solo a pochi semplici
passi:

* Astrarre i messaggi dell'applicazione avvolgendoli utilizzando i metodi
  :method:`Symfony\\Component\\Translation\\Translator::trans` o
  :method:`Symfony\\Component\\Translation\\Translator::transChoice`;
  (vedere anche ":doc:`/components/translation/usage`");

* Tradurre ogni messaggio in più locale creando dei file con i messaggi
  per la traduzione. Symfony scopre ed elabora ogni file perché i suoi nomi seguono
  una specifica convenzione;

* Gestire il locale dell'utente, che è memorizzato nella richiesta, ma può
  anche essere memorizzato nella sessione.

.. _`i18n`: http://it.wikipedia.org/wiki/Internazionalizzazione_e_localizzazione
.. _`ISO 3166-1 alpha-2`: http://en.wikipedia.org/wiki/ISO_3166-1#Current_codes
.. _`ISO 639-1`: http://en.wikipedia.org/wiki/List_of_ISO_639-1_codes
.. _`estensione Translatable`: https://github.com/l3pp4rd/DoctrineExtensions
.. _`behavior Translatable`: https://github.com/KnpLabs/DoctrineBehaviors
