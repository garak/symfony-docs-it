.. index::
    single: Configurazione

Organizzare i file di configurazione
====================================

Symfony Standard Edition definisce tre
:doc:`ambienti </cookbook/configuration/environments>`, chiamati
``dev``, ``prod`` e ``test``. Un ambiente è semplicemente un modo per
eseguire lo stesso codice con diverse configurazioni.

Per poter scegliere il file di configurazione da caricare per ciascun ambiente, Symfony
esegue il metodo ``registerContainerConfiguration()`` della classe
``AppKernel``::

    // app/AppKernel.php
    use Symfony\Component\HttpKernel\Kernel;
    use Symfony\Component\Config\Loader\LoaderInterface;

    class AppKernel extends Kernel
    {
        // ...

        public function registerContainerConfiguration(LoaderInterface $loader)
        {
            $loader->load(__DIR__.'/config/config_'.$this->getEnvironment().'.yml');
        }
    }

Questo metodo carica il file ``app/config/config_dev.yml`` per l'ambiente ``dev``
e così via. Uno alla volta, questo file carica il di configurazione comune,
che si trova in ``app/config/config.yml``. Quindi, i file di configurazione di
Symfony Standard Edition seguono questa struttura:

.. code-block:: text

    <progetto>/
    ├─ app/
    │  └─ config/
    │     ├─ config.yml
    │     ├─ config_dev.yml
    │     ├─ config_prod.yml
    │     ├─ config_test.yml
    │     ├─ parameters.yml
    │     ├─ parameters.yml.dist
    │     ├─ routing.yml
    │     ├─ routing_dev.yml
    │     └─ security.yml
    ├─ src/
    ├─ vendor/
    └─ web/

Questa struttura è stata scelta per la sua semplicità: un file per ambiente.
Ma, come ogni altra caratteristica di Symfony, la si può personalizzare per adattarsi alle proprie esigenze.
Le sezioni seguenti spiegano vari modo per organizzare dei file di configurazione.
Per semplificare gli esempi, saranno presi in considerazione solo gli ambienti
``dev`` e ``prod``.

Diverse cartelle per ambiente
-----------------------------

Invece di aggiungere i suffissi ``_dev`` e ``_prod`` ai file, questa tecnica
raggruppa tutti i file di configurazione relativi sotto una cartella con
lo stesso nome dell'ambiente:

.. code-block:: text

    <progetto>/
    ├─ app/
    │  └─ config/
    │     ├─ common/
    │     │  ├─ config.yml
    │     │  ├─ parameters.yml
    │     │  ├─ routing.yml
    │     │  └─ security.yml
    │     ├─ dev/
    │     │  ├─ config.yml
    │     │  ├─ parameters.yml
    │     │  ├─ routing.yml
    │     │  └─ security.yml
    │     └─ prod/
    │        ├─ config.yml
    │        ├─ parameters.yml
    │        ├─ routing.yml
    │        └─ security.yml
    ├─ src/
    ├─ vendor/
    └─ web/

Per fare in modo che funzioni, cambiare il codice del metodo
:method:`Symfony\\Component\\HttpKernel\\KernelInterface::registerContainerConfiguration`::


    // app/AppKernel.php
    use Symfony\Component\HttpKernel\Kernel;
    use Symfony\Component\Config\Loader\LoaderInterface;

    class AppKernel extends Kernel
    {
        // ...

        public function registerContainerConfiguration(LoaderInterface $loader)
        {
            $loader->load(__DIR__.'/config/'.$this->getEnvironment().'/config.yml');
        }
    }

Quindi, assicurarsi che ogni file ``config.yml`` carichi il resto dei file di configurazione,
inclusi i file comuni. Per esempio, il seguente è l'importazione
necessaria per il file ``app/config/dev/config.yml``:

.. configuration-block::

    .. code-block:: yaml

        # app/config/dev/config.yml
        imports:
            - { resource: '../common/config.yml' }
            - { resource: 'parameters.yml' }
            - { resource: 'security.yml' }

        # ...

    .. code-block:: xml

        <!-- app/config/dev/config.xml -->
        <?xml version="1.0" encoding="UTF-8" ?>
        <container xmlns="http://symfony.com/schema/dic/services"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://symfony.com/schema/dic/services
                http://symfony.com/schema/dic/services/services-1.0.xsd
                http://symfony.com/schema/dic/symfony
                http://symfony.com/schema/dic/symfony/symfony-1.0.xsd">

            <imports>
                <import resource="../common/config.xml" />
                <import resource="parameters.xml" />
                <import resource="security.xml" />
            </imports>

            <!-- ... -->
        </container>

    .. code-block:: php

        // app/config/dev/config.php
        $loader->import('../common/config.php');
        $loader->import('parameters.php');
        $loader->import('security.php');

        // ...

.. include:: /components/dependency_injection/_imports-parameters-note.rst.inc

File di configurazione semantica
--------------------------------

Una doverosa strategia di organizzazione potrebbe essere necessaria per applicazioni complesse,
con molti file di configurazione. Per esempio, si potrebbe creare un file per bundle
e molti file per definire tutti i servizi dell'applicazione:

.. code-block:: text

    <progetto>/
    ├─ app/
    │  └─ config/
    │     ├─ bundle/
    │     │  ├─ bundle1.yml
    │     │  ├─ bundle2.yml
    │     │  ├─ ...
    │     │  └─ bundleN.yml
    │     ├─ ambienti/
    │     │  ├─ common.yml
    │     │  ├─ dev.yml
    │     │  └─ prod.yml
    │     ├─ routing/
    │     │  ├─ common.yml
    │     │  ├─ dev.yml
    │     │  └─ prod.yml
    │     └─ servizi/
    │        ├─ frontend.yml
    │        ├─ backend.yml
    │        ├─ ...
    │        └─ security.yml
    ├─ src/
    ├─ vendor/
    └─ web/

Anche qui, va cambiato il codice del metodo ``registerContainerConfiguration()``, per far
conoscere a Symfony la nuova organizzazione dei file::

    // app/AppKernel.php
    use Symfony\Component\HttpKernel\Kernel;
    use Symfony\Component\Config\Loader\LoaderInterface;

    class AppKernel extends Kernel
    {
        // ...

        public function registerContainerConfiguration(LoaderInterface $loader)
        {
            $loader->load(__DIR__.'/config/ambienti/'.$this->getEnvironment().'.yml');
        }
    }

Seguendo la stessa tecnica spiegata nella sezione precedente, assicurarsi di
importare i file di configurazione appropriati per ciascun file principale (``common.yml``,
``dev.yml`` e ``prod.yml``).

Tecniche avanzate
-----------------

Symfony carica i file di configurazione usando il
:doc:`componente Config </components/config/introduction>`, che fornisce alcune
caratteristiche avanzate.

Mescolare i formati di configurazione
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

I file di configurazione possono importare file definiti con altri formati di configurazione
predefiniti (``.yml``, ``.xml``, ``.php``, ``.ini``):

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        imports:
            - { resource: 'parameters.yml' }
            - { resource: 'services.xml' }
            - { resource: 'security.yml' }
            - { resource: 'legacy.php' }

        # ...

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <?xml version="1.0" encoding="UTF-8" ?>
        <container xmlns="http://symfony.com/schema/dic/services"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://symfony.com/schema/dic/services
                http://symfony.com/schema/dic/services/services-1.0.xsd
                http://symfony.com/schema/dic/symfony
                http://symfony.com/schema/dic/symfony/symfony-1.0.xsd">

            <imports>
                <import resource="parameters.yml" />
                <import resource="services.xml" />
                <import resource="security.yml" />
                <import resource="legacy.php" />
            </imports>

            <!-- ... -->
        </container>

    .. code-block:: php

        // app/config/config.php
        $loader->import('parameters.yml');
        $loader->import('services.xml');
        $loader->import('security.yml');
        $loader->import('legacy.php');

        // ...

.. caution::

    La classe ``IniFileLoader`` analizza il contenuto dei file usando la funzione
    :phpfunction:`parse_ini_file`. Si possono quindi impostare parametri
    solo con valori stringhe. Usare uno degli altri caricatori, se si vogliono
    usare altri tipi di dati (p.e. booleano, intero, ecc.)

Se si usano altri formati di configurazione, si deve definire una propria classe di caricamento,
che estenda :class:`Symfony\\Component\\DependencyInjection\\Loader\\FileLoader`.
Quando i valori di configurazione sono dinamici, si può usare il file di configurazione PHP
per eseguire una logica. Inoltre, si possono definire servizi che
caricano configurazioni dalla base dati o da servizi web.

File di configurazione globale
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Alcuni sistemisti preferiscono impostare parametri sensibili in file esterni
alla cartella del progetto. Si immagini che le credenziali della base dati per un
sito siano memorizzati nel file ``/etc/sites/mysite.com/parameters.yml`` file. È semplice
caricare questo file, basta indicarne il percorso completo quando lo si importa
da un altro file di configurazione:

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        imports:
            - { resource: 'parameters.yml' }
            - { resource: '/etc/sites/mysite.com/parameters.yml' }

        # ...

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <?xml version="1.0" encoding="UTF-8" ?>
        <container xmlns="http://symfony.com/schema/dic/services"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://symfony.com/schema/dic/services
                http://symfony.com/schema/dic/services/services-1.0.xsd
                http://symfony.com/schema/dic/symfony
                http://symfony.com/schema/dic/symfony/symfony-1.0.xsd">

            <imports>
                <import resource="parameters.yml" />
                <import resource="/etc/sites/mysite.com/parameters.yml" />
            </imports>

            <!-- ... -->
        </container>

    .. code-block:: php

        // app/config/config.php
        $loader->import('parameters.yml');
        $loader->import('/etc/sites/mysite.com/parameters.yml');

        // ...

La maggior parte delle volte, gli sviluppatore locali non hanno gli stessi file che si trovano
sui server di produzione. Per questa ragione, il componente Config fornisce l'opzione
``ignore_errors``, che scarta silenziosamente gli errori quando il file caricato
non esiste:

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        imports:
            - { resource: 'parameters.yml' }
            - { resource: '/etc/sites/mysite.com/parameters.yml', ignore_errors: true }

        # ...

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <?xml version="1.0" encoding="UTF-8" ?>
        <container xmlns="http://symfony.com/schema/dic/services"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://symfony.com/schema/dic/services
                http://symfony.com/schema/dic/services/services-1.0.xsd
                http://symfony.com/schema/dic/symfony
                http://symfony.com/schema/dic/symfony/symfony-1.0.xsd">

            <imports>
                <import resource="parameters.yml" />
                <import resource="/etc/sites/mysite.com/parameters.yml" ignore-errors="true" />
            </imports>

            <!-- ... -->
        </container>

    .. code-block:: php

        // app/config/config.php
        $loader->import('parameters.yml');
        $loader->import('/etc/sites/mysite.com/parameters.yml', null, true);

        // ...

Come mostrato, ci sono molti modi per organizzare i file di configurazione. Si può
scegliere uno di questi o anche crearne uno personalizzato. Non serve farsi limitare
dalla Standard Edition di Symfony. Per ulteriori
personalizzazioni, vedere ":doc:`/cookbook/configuration/override_dir_structure`".
