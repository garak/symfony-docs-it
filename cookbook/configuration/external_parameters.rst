.. index::
   single: Ambienti; Parametri esterni

Configurare parametri esterni nel contenitore dei servizi
=========================================================

Nel capitolo :doc:`/cookbook/configuration/environments`, si è visto come
gestire la configurazione dell'applicazione. Alle volte potrebbe essere utile,
per l'applicazione, salvare alcune credenziali al di fuori del codice del progetto.
Ad esempio la configurazione dell'accesso alla base dati. La flessibilità del
contenitore dei servizi di symfony permette di farlo in modo agevole.

Variabili d'ambiente
--------------------

Symfony recupera qualsiasi variabile d'ambiente, il cui prefisso sia ``SYMFONY__``
e la usa come un parametro all'interno del contenitore dei servizi. Al nome del
parametro vengono applicate alcune trasformazioni:

* Il prefisso ``SYMFONY__`` viene rimosso;
* Il nome del parametro viene convertito in minuscolo;
* Il doppio trattino basso viene sostituito da un punto, dato che il punto non è un 
  carattere valido per i nomi delle variabili d'ambiente.

Ad esempio, se si usa l'ambiente Apache, le variabili d'ambiente possono
essere configurate utilizzando la seguente configurazione del ``VirtualHost``:

.. code-block:: apache

    <VirtualHost *:80>
        ServerName      Symfony
        DocumentRoot    "/percorso/applicazione/symfony_2/web"
        DirectoryIndex  index.php index.html
        SetEnv          SYMFONY__UTENTE__DATABASE utente
        SetEnv          SYMFONY__PASSWORD__DATABASE segreto

        <Directory "/percorso/applicazione/symfony_2/web">
            AllowOverride All
            Allow from All
        </Directory>
    </VirtualHost>

.. note::

    Il precedente esempio è relativo alla configurazione di Apache e utilizza la
    direttiva `SetEnv`_. Comunque, lo stesso concetto si applica a qualsiasi
    server web che supporti la configurazione delle variabili d'ambiente.

    Inoltre, per far si che possa funzionare anche per la riga di comando (che non utilizza Apache),
    sarà necessario esportare i parametri come variabili di shell. Su di un sistema Unix,
    lo si può fare con il seguente comando:

    .. code-block:: bash

        $ export SYMFONY__UTENTE__DATABASE=utente
        $ export SYMFONY__PASSWORD__DATABASE=segreta

Una volta dichiarate, le variabili saranno disponibili all'interno
della variabile globale ``$_SERVER`` di PHP. Symfony si occuperà di trasformare
tutte le variabili di ``$_SERVER``, con prefisso ``SYMFONY__``, in parametri
per il contenitore dei servizi.

A questo punto, sarà possibile richiamare questi parametri ovunque sia necessario.

.. configuration-block::

    .. code-block:: yaml

        doctrine:
            dbal:
                driver    pdo_mysql
                dbname:   progetto_symfony
                user:     "%utente.database%"
                password: "%password.database%"

    .. code-block:: xml

        <!-- xmlns:doctrine="http://symfony.com/schema/dic/doctrine" -->
        <!-- xsi:schemaLocation="http://symfony.com/schema/dic/doctrine http://symfony.com/schema/dic/doctrine/doctrine-1.0.xsd"> -->

        <doctrine:config>
            <doctrine:dbal
                driver="pdo_mysql"
                dbname="progetto_symfony"
                user="%utente.database%"
                password="%password.database%"
            />
        </doctrine:config>

    .. code-block:: php

        $container->loadFromExtension('doctrine', array(
            'dbal' => array(
                'driver'   => 'pdo_mysql',
                'dbname'   => 'progetto_symfony',
                'user'     => '%utente.database%',
                'password' => '%password.database%',
            )
        ));

Costanti
--------

Il contenitore supporta anche l'impostazione di costanti PHP come parametri.
Vedere :ref:`component-di-parameters-constants` per ulteriori dettagli.

Configurazioni varie
--------------------

La direttiva ``imports`` può essere usata per importare parametri conservati in qualsiasi parte.
Importare un file PHP permette di avere la flessibilità di aggiungere qualsiasi cosa sia
necessaria al contenitore. Il seguente esempio importa un file di nome ``parametri.php``.

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        imports:
            - { resource: parameters.php }

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <imports>
            <import resource="parameters.php" />
        </imports>

    .. code-block:: php

        // app/config/config.php
        $loader->import('parameters.php');

.. note::

    Un file di risorse può essere espresso in diversi formati. PHP, XML, YAML, INI e
    risorse di closure, sono tutti supportati dalla direttiva ``imports``.

``parametri.php`` conterrà i parametri che si vuole che il contenitore dei 
servizi configuri. Questo è specialmente utile nel caso si voglia importare una
configurazione con formato non standard. Il seguente esempio importa la configurazione
di una base dati per Drupal in un contenitore di servizi symfony.

.. code-block:: php

    // app/config/parameters.php
    include_once('/percorso/al/sito/drupal/default/settings.php');
    $container->setParameter('url.database.drupal', $db_url);

.. _`SetEnv`: http://httpd.apache.org/docs/current/env.html
