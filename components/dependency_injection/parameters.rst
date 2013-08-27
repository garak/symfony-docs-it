.. index::
   single: Dependency Injection; Parametri

Introduzione ai parametri
=========================

Si possono definire, nel contenitore di servizi, parametri che possono essere usati
direttamente o come parte di definizioni di servizi. Questo è di aiuto per separare
valori che si vorranno cambiare più spesso.

Recuperare e impostare parametri del contenitore
------------------------------------------------

È facile lavorare coi parametri del contenitore, usando i metodi di accesso del
contenitore per i parametri. Si può verificare se un parametro sia stato definito
nel contenitore con::

     $container->hasParameter('mailer.transport');

Si può recuperare un parametro impostato nel contenitore con::

    $container->getParameter('mailer.transport');

e impostare un parametro nel contenitore con::

    $container->setParameter('mailer.transport', 'sendmail');

.. note::

    Si può impostare un parametro solo prima che il contenitore sia compilato. Per saperne
    di più sulla compilazione del contenitore, vedere
    :doc:`/components/dependency_injection/compilation`.

Parametri nei file di configurazione
------------------------------------

Si può anche usare la sezione ``parameters`` di un file di configurazione per impostare parametri:

.. configuration-block::

    .. code-block:: yaml

        parameters:
            mailer.transport: sendmail

    .. code-block:: xml

        <parameters>
            <parameter key="mailer.transport">sendmail</parameter>
        </parameters>

    .. code-block:: php

        $container->setParameter('mailer.transport', 'sendmail');

Come per il recupero diretto dei valori dei parametri dal contenitore, si può
anche usarli nei file di configurazione. Si può fare riferimento ai parametri da altrove,
inserendoli tra simboli di percentuale (``%``) , p.e. ``%mailer.transport%``.
Un possibile uso è iniettare i valori nei servizi. Questo consente
di configurare varie versioni dei servizi tra le applicazioni o vari
servizi basati sulle stesse classi ma configurati diversamente, in una singola
applicazione. Si può iniettare la scelta del trasporto dell'email nella classe ``Mailer``
direttamente, ma rendendola un parametro. Questo rende più facile cambiarla,
rispetto ad averla legata e nascosta nella definizione del servizio:

.. configuration-block::

    .. code-block:: yaml

        parameters:
            mailer.transport: sendmail

        services:
            mailer:
                class:     Mailer
                arguments: ['%mailer.transport%']

    .. code-block:: xml

        <parameters>
            <parameter key="mailer.transport">sendmail</parameter>
        </parameters>

        <services>
            <service id="mailer" class="Mailer">
                <argument>%mailer.transport%</argument>
            </service>
        </services>

    .. code-block:: php

        use Symfony\Component\DependencyInjection\Reference;

        // ...
        $container->setParameter('mailer.transport', 'sendmail');
        $container
            ->register('mailer', 'Mailer')
            ->addArgument('%mailer.transport%');

.. caution::

    I valori nei tag ``parameter`` nella configurazione XML mantengono
    eventuali spazi e "a capo".

    Questo vuol dire che il seguente esempio di configurazione avrà come valore
    ``\n    sendmail\n``:

    .. code-block:: xml

        <parameter key="mailer.transport">
            sendmail
        </parameter>

    In alcuni casi (per costanti o nomi di classi), questo può causare errori. Per
    prevenirlo, inserire sempre i parametri sulla stessa riga, come segue:

    .. code-block:: xml

        <parameter key="mailer.transport">sendmail</parameter>

In caso di uso altrove, occorre cambiare il valore del
parametro in un unico posto, se necessario.

Si possono anche usare i parametri nella definizione dei servizi, per esempio,
rendendo un parametro la classe di un servizio:

.. configuration-block::

    .. code-block:: yaml

        parameters:
            mailer.transport: sendmail
            mailer.class: Mailer

        services:
            mailer:
                class:     '%mailer.class%'
                arguments: ['%mailer.transport%']

    .. code-block:: xml

        <parameters>
            <parameter key="mailer.transport">sendmail</parameter>
            <parameter key="mailer.class">Mailer</parameter>
        </parameters>

        <services>
            <service id="mailer" class="%mailer.class%">
                <argument>%mailer.transport%</argument>
            </service>

        </services>

    .. code-block:: php

        use Symfony\Component\DependencyInjection\Reference;

        // ...
        $container->setParameter('mailer.transport', 'sendmail');
        $container->setParameter('mailer.class', 'Mailer');
        $container
            ->register('mailer', '%mailer.class%')
            ->addArgument('%mailer.transport%');

        $container
            ->register('newsletter_manager', 'NewsletterManager')
            ->addMethodCall('setMailer', array(new Reference('mailer')));

.. note::

    Il simbolo di percentuale dentro a un parametro o argomento, come parte della stringa, deve subire
    un escape con un ulteriore simbolo di percentuale:

    .. configuration-block::

        .. code-block:: yaml

            arguments: ['http://symfony.com/?foo=%%s&bar=%%d']

        .. code-block:: xml

            <argument type="string">http://symfony.com/?foo=%%s&bar=%%d</argument>

        .. code-block:: php

            ->addArgument('http://symfony.com/?foo=%%s&bar=%%d');

.. _component-di-parameters-array:

Parametri array
---------------

I parametri non devono necessariamente essere semplici stringhe, possono anche essere
array. Per il formato YAML, occorre usare l'attributo type="collection" per tutti i
parametri che sono array.

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        parameters:
            my_mailer.gateways:
                - mail1
                - mail2
                - mail3
            my_multilang.language_fallback:
                en:
                    - en
                    - fr
                fr:
                    - fr
                    - en

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <parameters>
            <parameter key="my_mailer.gateways" type="collection">
                <parameter>mail1</parameter>
                <parameter>mail2</parameter>
                <parameter>mail3</parameter>
            </parameter>
            <parameter key="my_multilang.language_fallback" type="collection">
                <parameter key="en" type="collection">
                    <parameter>en</parameter>
                    <parameter>fr</parameter>
                </parameter>
                <parameter key="fr" type="collection">
                    <parameter>fr</parameter>
                    <parameter>en</parameter>
                </parameter>
            </parameter>
        </parameters>

    .. code-block:: php

        // app/config/config.php
        use Symfony\Component\DependencyInjection\Definition;

        $container->setParameter('my_mailer.gateways', array('mail1', 'mail2', 'mail3'));
        $container->setParameter('my_multilang.language_fallback', array(
            'en' => array('en', 'fr'),
            'fr' => array('fr', 'en'),
        ));

.. _component-di-parameters-constants:

Costanti come parametri
-----------------------

Il contenitore supporta anche l'impostazione di costanti PHP come parametri. Per
sfruttare questa caratteristica, mappare il nome della costante a un parametro
e definire il tipo come ``constant``.

.. configuration-block::

    .. code-block:: xml

        <?xml version="1.0" encoding="UTF-8"?>

        <container xmlns="http://symfony.com/schema/dic/services"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance">

            <parameters>
                <parameter key="global.constant.value" type="constant">COSTANTE_GLOBALE</parameter>
                <parameter key="my_class.constant.value" type="constant">Mia_Classe::NOME_COSTANTE</parameter>
            </parameters>
        </container>

    .. code-block:: php

            $container->setParameter('global.constant.value', COSTANTE_GLOBALE);
            $container->setParameter('my_class.constant.value', Mia_Classe::NOME_COSTANTE);

.. note::

    Questo non funziona per configurazioni Yaml. Se si usa Yaml, si può
    importare un file XML per sfruttare tale funzionalità:

    .. configuration-block::

        .. code-block:: yaml

            # app/config/config.yml
            imports:
                - { resource: parameters.xml }

Parole chiave di PHP in XML
---------------------------

Per impostazione predefinita, ``true``, ``false`` e ``null`` in XML sono convertiti nelle rispettive parole
chiave di PHP (rispettivamente ``true``, ``false`` e ``null``):

.. code-block:: xml

    <parameters>
        <parameter key="mailer.send_all_in_once">false</parameters>
    </parameters>

    <!-- dopo l'analisi
    $container->getParameter('mailer.send_all_in_once'); // restituisce false
    -->

Per disabilitare questo comportamento, usare il tipo ``string``:

.. code-block:: xml

    <parameters>
        <parameter key="mailer.some_parameter" type="string">true</parameter>
    </parameters>

    <!-- dopo l'analisi
    $container->getParameter('mailer.some_parameter'); // restituisce "true"
    -->

.. note::

    Questa caratteristica non è disponibile per Yaml e PHP, che hanno già un supporto
    interno per le parole chiave di PHP.

Fare riferimento ai servizi
---------------------------

Si possono creare dei riferimento i servizi, che hanno un aspetto un po' differente in
ciascun formato. Si può configurare il comportamento da tenere nel caso in cui il servizio di riferimento
non esista. Per impostazione predefinita, viene lanciata un'eccezione nel caso di
servizio non esistente.

Yaml
~~~~

Prefissare la stringa con  ``@`` o ``@?`` per fare riferimento a un servizio in Yaml.

* ``@mailer`` fa riferimento al servizio ``mailer``. Se il servizio non
  esiste, verrà lanciata un'eccezione;
* ``@?mailer`` fa riferimento al servizio ``mailer``. Se il servizio non
  esiste, verrà ignorato;

Xml
~~~

In XML, usare il tipo ``service``. Il comportamento in caso di servizio non esistente
può essere specificato con il parametro ``on-invalid``. Per impostazione predefinita, viene
lanciata un'eccezione. Valori validi per ``on-invalid`` sono ``null`` (usa ``null`` al posto
del servizio mancante) o ``ignored`` (molto simile, ma rimuove la chiamata al metodo, se
usato su una chiamata a metodo).

Php
~~~

In PHP, si può usare la classe
:class:`Symfony\\Component\\DependencyInjection\\Reference` per fare riferimento a
un servizio. Il comportamento non valido è configurabile con il secondo parametro del
costruttore e le costanti di
:class:`Symfony\\Component\\DependencyInjection\\ContainerInterface`.
