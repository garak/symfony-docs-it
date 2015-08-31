.. index::
   single: Template; Variabili globali

Iniettare variabili in tutti i template (variabili globali)
===========================================================

A volte si vuole che una variabile sia accessibile in tutti i template usati.
Lo si può fare, modificando il file ``app/config/config.yml``:

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        twig:
            # ...
            globals:
                ga_tracking: UA-xxxxx-x

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <twig:config>
            <!-- ... -->
            <twig:global key="ga_tracking">UA-xxxxx-x</twig:global>
        </twig:config>

    .. code-block:: php

        // app/config/config.php
        $container->loadFromExtension('twig', array(
             // ...
             'globals' => array(
                 'ga_tracking' => 'UA-xxxxx-x',
             ),
        ));

Ora, la variabile ``ga_tracking`` è disponibile in tutti i template Twig

.. code-block:: html+jinja

    <p>Il codice di tracciamento Google è: {{ ga_tracking }} </p>

È molto facile!

Usare parametri del contenitore di servizi
------------------------------------------

Si può anche usare il sistema dei :ref:`book-service-container-parameters`,
che consente di isolare o riutilizzare il valore:

.. code-block:: yaml

    # app/config/parameters.yml
    parameters:
        ga_tracking: UA-xxxxx-x

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        twig:
            globals:
                ga_tracking: "%ga_tracking%"

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <twig:config>
            <twig:global key="ga_tracking">%ga_tracking%</twig:global>
        </twig:config>

    .. code-block:: php

        // app/config/config.php
        $container->loadFromExtension('twig', array(
             'globals' => array(
                 'ga_tracking' => '%ga_tracking%',
             ),
        ));

La stessa variabile è disponibile esattamente come prima.

Fare riferimento ai servizi
---------------------------

Invece di usare valori statici, si può anche impostare il valore a un servizio.
Ogni volta che un template accederà alla variabile globale, il servizio sarà
richiesto dal contenitore e si avrà accesso all'oggetto relativo.

.. note::

    Il servizio non è caricato pigramente. In altre parole, non appena viene caricato Twig,
    il servizio sarà istanziato, anche se la variabile globale non verrà
    mai usata.

Per definire un servizio come variabile globale Twig, aggiungere un prefisso ``@``.
Dovrebbe essere familiare, essendo la stessa sintassi usata nella configurazione del servizio.

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        twig:
            # ...
            globals:
                user_management: "@acme_user.user_management"

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <twig:config>
            <!-- ... -->
            <twig:global key="user_management">@acme_user.user_management</twig:global>
        </twig:config>

    .. code-block:: php

        // app/config/config.php
        $container->loadFromExtension('twig', array(
             // ...
             'globals' => array(
                 'user_management' => '@acme_user.user_management',
             ),
        ));

Usare un'estensione Twig
------------------------

Se la variabile globale da impostare è più complicata, come un oggetto,
non si potrà usare il metodo appena visto. Occorrearà invece creare
una :ref:`estensione Twig <reference-dic-tags-twig-extension>` e restituire
la variabile globale come una delle voci del metodo ``getGlobals``.
