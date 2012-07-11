.. index::
   single: Dependency Injection; Configurazione avanzata

Configurazione avanzata del contenitore
=======================================

Segnare i servizi come pubblici / privati
-----------------------------------------

Quando si definisce un servizio, di solito si vuole potervi accedere dall'interno
di un'applicazione. Tali servizi sono chiamati "pubblici". Per esempio, il servizio
``doctrine`` registrato con il contenitore durante l'uso di DoctrineBundle
è un servizio pubblico, accessibile tramite::

   $doctrine = $container->get('doctrine');

Ci sono tuttavia dei casi in cui non si desidera che un servizio sia pubblico.
Di solito avviene quando un servizio è definito solo per essere usato come argomento
da un altro servizio.

.. note::

    Se si usa un servizio privato come argomento di più di un altro servizio,
    ciò risulterà nell'uso di due diverse istanze del servizio privato, perché
    l'istanza del servizio privata è eseguita internamente (p.e. ``new PippoPlutoPrivato()``).

In parole povere: un servizio sarà privato quanto non si vuole che sia accessibile
direttamente dal codice.

Ecco un esempio:

.. configuration-block::

    .. code-block:: yaml

        services:
           pippo:
             class: Esempio\Pippo
             public: false

    .. code-block:: xml

        <service id="pippo" class="Esempio\Pippo" public="false" />

    .. code-block:: php

        $definition = new Definition('Esempio\Pippo');
        $definition->setPublic(false);
        $container->setDefinition('pippo', $definition);

Essendo il servizio privato, *non* si può richiamare::

    $container->get('pippo');

Tuttavia, se un servizio è stato segnato come privato, gli si può comunque assegnare un
alias (vedere sotto) per accedervi (tramite alias).

.. note::

   I servizi sono predefiniti come pubblici.

Alias
-----

A volte si ha bisogno di usare scorciatoie per accedere ad alcuni servizi. Si possono
impostare degli alias e si può anche impostare un alias su un servizio non
pubblico.

.. configuration-block::

    .. code-block:: yaml

        services:
           pippo:
             class: Esempio\Pippo
           pluto:
             alias: pippo

    .. code-block:: xml

        <service id="pippo" class="Esempio\Pippo"/>

        <service id="pluto" alias="pippo" />

    .. code-block:: php

        $definition = new Definition('Esempio\Pippo');
        $container->setDefinition('pippo', $definition);

        $containerBuilder->setAlias('pluto', 'pippo');

Ciò vuol dire che, quando si usa direttamente il contenitore, si può accedere al servizio
``pippo`` richiedendo il servizio ``pluto``, in questo modo::

    $container->get('pluto'); // restituisce il servizio pippo

Richiesta di file
-----------------

Possono esserci dei casi in cui occorra includere altri file subito prima che il
servizio stesso sia caricato. Per poterlo fare, si può usare la direttiva ``file``.

.. configuration-block::

    .. code-block:: yaml

        services:
           foo:
             class: Esempio\Pippo\Pluto
             file: %kernel.root_dir%/src/percorso/del/file/pippo.php

    .. code-block:: xml

        <service id="foo" class="Esempio\Pippo\Pluto">
            <file>%kernel.root_dir%/src/percorso/del/file/pippo.php</file>
        </service>

    .. code-block:: php

        $definition = new Definition('Esempio\Pippo\Pluto');
        $definition->setFile('%kernel.root_dir%/src/percorso/del/file/pippo.php');
        $container->setDefinition('foo', $definition);

Si noti che Symfony richiamerà internamente la funzione require_once di PHP, il
che vuol dire che il file sarà incluso una sola volta per richiesta. 
