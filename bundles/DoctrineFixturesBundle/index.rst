DoctrineFixturesBundle
======================

Le fixture sono usate per caricare un insieme controllato di dati in un database. Tali
dati possono essere usati per i test o potrebbero essere i dati iniziali necessari a una
applicazione per essere eseguita. Symfony2 non ha un modo predefinito per gestire le
fixture, ma Doctrine2 ha una libreria che aiuta a scrivere fixtture per
l':doc:`ORM</book/doctrine>` o per l':doc:`ODM</bundles/DoctrineMongoDBBundle/index>`.

Configurazione
--------------

Se non si è ancora configurata la libreria `Doctrine Data Fixtures`_ con Symfony2,
seguire questi tre passi.

Se si usa la Standard Distribution, aggiungere al proprio file
``deps``:

.. code-block:: text

    [doctrine-fixtures]
        git=http://github.com/doctrine/data-fixtures.git

    [DoctrineFixturesBundle]
        git=http://github.com/symfony/DoctrineFixturesBundle.git
        target=/bundles/Symfony/Bundle/DoctrineFixturesBundle

Aggiornare le librerie dei venditori:

.. code-block:: bash

    $ php bin/vendors install

Se tutto ha funzioonato, si può ora trovare la libreria ``doctrine-fixtures`` sotto
``vendor/doctrine-fixtures``.

Registrare lo spazio dei nimi ``Doctrine\Common\DataFixtures`` in ``app/autoload.php``.

.. code-block:: php

    // ...
    $loader->registerNamespaces(array(
        // ...
        'Doctrine\\Common\\DataFixtures' => __DIR__.'/../vendor/doctrine-fixtures/lib',
        'Doctrine\\Common' => __DIR__.'/../vendor/doctrine-common/lib',
        // ...
    ));

.. caution::

    Assicurarsi di registrare il nuovo spazio dei nomi *prima* di ``Doctrine\Common``.
    In caso contrario, Symfony cercherà le classi delle fixture nella cartella
    ``Doctrine\Common``. L'autoloader di Symfony cerca sempre una classe nella cartella
    del primo spazio dei nomi corrispondente, quindi spazi dei nomi più specifici vanno
    sempre messi prima.

Infine, registrare il bundle ``DoctrineFixturesBundle`` in ``app/AppKernel.php``.

.. code-block:: php

    // ...
    public function registerBundles()
    {
        $bundles = array(
            // ...
            new Symfony\Bundle\DoctrineFixturesBundle\DoctrineFixturesBundle(),
            // ...
        );
        // ...
    }

Scrivere semplici fixture
-------------------------

Le fixture di Doctrine2 sono classi PHP, in cui si possono creare oggetti e persisterli
nel database. Come tutte le classi in Symfony2, le fixture dovrebbero stare all'interno
di bundle della propria applicazione.

Per un bundle in ``src/Acme/HelloBundle``, le classi delle fixture dovrebbero stare
in ``src/Acme/HelloBundle/DataFixtures/ORM`` oppure in
``src/Acme/HelloBundle/DataFixtures/MongoDB``, rispetetivamente per ORM e ODM.
Questa guida ipotizza che si stia usando l'ORM, ma le fixture si possono aggiungere
altrettanto facilemente usando l'ODM.

Si immagini di avere una classe ``User`` e di voler caricare un oggetto
``User``:

.. code-block:: php

    // src/Acme/HelloBundle/DataFixtures/ORM/LoadUserData.php
    namespace Acme\HelloBundle\DataFixtures\ORM;

    use Doctrine\Common\DataFixtures\FixtureInterface;
    use Acme\HelloBundle\Entity\User;

    class LoadUserData implements FixtureInterface
    {
        public function load($manager)
        {
            $userAdmin = new User();
            $userAdmin->setUsername('admin');
            $userAdmin->setPassword('test');

            $manager->persist($userAdmin);
            $manager->flush();
        }
    }

In Doctrine2, le fixture sono semplici oggetti, in cui caricare dati tramite interazioni
con le proprie entità, come si fa normalmente. Ciò conenste di creare esattamente le
fixture necessarie per la propria applicazione.

Il limite più grosso in questo approccio è l'impossibilità di condividere oggetti tramite
le fixture. Più avanti, vedremo come superare questo limite.

Eseguire le fixture
-------------------

Una volta scritte le fixture, si possono caricarle tramite la linea di comando,
usando il comando ``doctrine:fixtures:load``:

.. code-block:: bash

    php app/console doctrine:fixtures:load

Se si usa l'ODM, usare invece il comando ``doctrine:mongodb:fixtures:load``:

.. code-block:: bash

    php app/console doctrine:mongodb:fixtures:load

Il comando cercherà nella cartella ``DataFixtures/ORM`` (o ``DataFixtures/MongoDB``
per l'ODM) di ogni bundle ed eseguirà ogni classe che implementa
``FixtureInterface``.

Entrambi i comandi hanno delle opzioni:

* ``--fixtures=/path/to/fixture`` - Usare questa opzione per specificare manualmente
  la cartella in cui le classi delle fixture vanno caricate;

* ``--append`` - Usare questo flag per appendere dati, invece di cancellare e ricaricare
  i dati (la cancellazione è il comportamento predefinito);

* ``--em=manager_name`` - Specifica manualmente l'entity manager da usare per caricare
  i dati.

.. note::

   Se si usa il comando ``doctrine:mongodb:fixtures:load``, sostituire l'opzione
   ``--em=`` con ``--dm=``, per specificare manualmente il document manager.

Un esempio completo potrebbe assomigliare a questo:

.. code-block:: bash

   php app/console doctrine:fixtures:load --fixtures=/percorso/di/fixture1 --fixtures=/percorso/di/fixture2 --append --em=foo_manager

Condividere oggetti tra le fixture
----------------------------------

La scrittura di fixture di base è semplice. Ma se si hanno molte classi di fixture e
si vuole poter fare riferimento a dati caricati in altre fixture, cosa fare?
Per esempio, se si vuole caricare un oggetto ``User`` in una fixture e poi si vuole
farvi riferimento in un'altra fixture, per poter assegnare quell'utente a un
determinato gruppo?

La libreria delle fixture di Doctrine gestisce facilmente questa situaizone, consentendo
di specificare l'ordine in cui le fixture sono caricate.

.. code-block:: php

    // src/Acme/HelloBundle/DataFixtures/ORM/LoadUserData.php
    namespace Acme\HelloBundle\DataFixtures\ORM;

    use Doctrine\Common\DataFixtures\AbstractFixture;
    use Doctrine\Common\DataFixtures\OrderedFixtureInterface;
    use Acme\HelloBundle\Entity\User;

    class LoadUserData extends AbstractFixture implements OrderedFixtureInterface
    {
        public function load($manager)
        {
            $userAdmin = new User();
            $userAdmin->setUsername('admin');
            $userAdmin->setPassword('test');

            $manager->persist($userAdmin);
            $manager->flush();

            $this->addReference('admin-user', $userAdmin);
        }

        public function getOrder()
        {
            return 1; // ordine in cui le fixture saranno caricate
        }
    }

La classe fixture ora implementa ``OrderedFixtureInterface``, che dice a
Doctrine che si vuole controllare l'ordine delle fixture. Creare un'altra classe fixture
e farla caricare dopo ``LoadUserData`` restituendo l'ordine
2:

.. code-block:: php

    // src/Acme/HelloBundle/DataFixtures/ORM/LoadGroupData.php
    namespace Acme\HelloBundle\DataFixtures\ORM;

    use Doctrine\Common\DataFixtures\AbstractFixture;
    use Doctrine\Common\DataFixtures\OrderedFixtureInterface;
    use Acme\HelloBundle\Entity\Group;

    class LoadGroupData extends AbstractFixture implements OrderedFixtureInterface
    {
        public function load($manager)
        {
            $groupAdmin = new Group();
            $groupAdmin->setGroupName('admin');

            $manager->persist($groupAdmin);
            $manager->flush();

            $this->addReference('admin-group', $groupAdmin);
        }

        public function getOrder()
        {
            return 2; // ordine in cui le fixture saranno caricate
        }
    }

Entrambe le classi fixture estendono ``AbstractFixture``, che consente di creare
oggetti e impostarli come riferimenti, in modo che possano successivamente essere
usati in altre fixture. Per esempio, gli oggetti ``$userAdmin`` e ``$groupAdmin``
possono essere riferiti successivamente tramite  i riferimenti ``admin-user`` e
``admin-group``:

.. code-block:: php

    // src/Acme/HelloBundle/DataFixtures/ORM/LoadUserGroupData.php
    namespace Acme\HelloBundle\DataFixtures\ORM;

    use Doctrine\Common\DataFixtures\AbstractFixture;
    use Doctrine\Common\DataFixtures\OrderedFixtureInterface;
    use Acme\HelloBundle\Entity\UserGroup;

    class LoadUserGroupData extends AbstractFixture implements OrderedFixtureInterface
    {
        public function load($manager)
        {
            $userGroupAdmin = new UserGroup();
            $userGroupAdmin->setUser($manager->merge($this->getReference('admin-user')));
            $userGroupAdmin->setGroup($manager->merge($this->getReference('admin-group')));

            $manager->persist($userGroupAdmin);
            $manager->flush();
        }

        public function getOrder()
        {
            return 3;
        }
    }

Questa fixture sarà ora eseguite nell'ordine del valore restituito dal metodo
``getOrder()``. Si può accedere a ogni oggetto impostato con il metodo ``setReference()``,
ramite il metodo ``getReference()`` in classi fixture con un ordine più
alto.

Le fixture consentono di creare ogni tipo di dato necessario, tramite la normale
interfaccia di PHP per creare e persistere oggetti. Controllando l'ordine delle fixture
e impostando dei riferimenti, si può gestire quasi tutto tramite fixture.

Usare il contenitore nelle fixture
----------------------------------

In alcuni casi occorre accedere ad alcuni servizi, per caricare le fixture.
Symfony2 rende il processo molto facile: il contenitore sarà iniettato in tutte le classi
fixture che implementano :class:`Symfony\\Component\\DependencyInjection\\ContainerAwareInterface`.

Riscriviamo la prima fixture per codificare la password, prima che sia memorizzata nel
database (una buona pratica). Userà il factory encoder per codificare la password,
assicurando che sia codificata nello stesso modo usato dal componente security 
quando si verifica:

.. code-block:: php

    // src/Acme/HelloBundle/DataFixtures/ORM/LoadUserData.php
    namespace Acme\HelloBundle\DataFixtures\ORM;

    use Doctrine\Common\DataFixtures\FixtureInterface;
    use Symfony\Component\DependencyInjection\ContainerAwareInterface;
    use Symfony\Component\DependencyInjection\ContainerInterface;
    use Acme\HelloBundle\Entity\User;

    class LoadUserData implements FixtureInterface, ContainerAwareInterface
    {
        private $container;

        public function setContainer(ContainerInterface $container = null)
        {
            $this->container = $container;
        }

        public function load($manager)
        {
            $userAdmin = new User();
            $userAdmin->setUsername('admin');
            $userAdmin->setSalt(md5(time()));

            $encoder = $this->container->get('security.encoder_factory')->getEncoder($userAdmin);
            $userAdmin->setPassword($encoder->encodePassword('test', $userAdmin->getSalt()));

            $manager->persist($userAdmin);
            $manager->flush();
        }
    }

Come si può vedere, occorre solo aggiungere ``ContainerAwareInterface`` alla classe e
poi creare un nuovo metodo ``setContainer()``, che implementi tale interfaccia.
Prima che la fixture sia eseguita, Symfony richiamerà il metodo ``setContainer()``
automaticamente. Se si memorizza il contenitore come proprietà della classe, come
mostrato sopra, vi si può accedere nel metodo ``load()``.

.. _`Doctrine Data Fixtures`: https://github.com/doctrine/data-fixtures

