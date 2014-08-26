UniqueEntity
============

Valida che un particolare campo (o campi) in un entità Doctrine sia unico.
Si usa di solito, per esempio, per prevenire che un nuovo utente si registri
usando un indirizzo email già esistente nel sistema.

+----------------+-------------------------------------------------------------------------------------+
| Si applica a   | :ref:`class <validation-class-target>`                                              |
+----------------+-------------------------------------------------------------------------------------+
| Opzioni        | - `fields`_                                                                         |
|                | - `message`_                                                                        |
|                | - `em`_                                                                             |
|                | - `repositoryMethod`_                                                               |
|                | - `errorPath`_                                                                      |
|                | - `ignoreNull`_                                                                     |
+----------------+-------------------------------------------------------------------------------------+
| Classe         | :class:`Symfony\\Bridge\\Doctrine\\Validator\\Constraints\\UniqueEntity`            |
+----------------+-------------------------------------------------------------------------------------+
| Validatore     | :class:`Symfony\\Bridge\\Doctrine\\Validator\\Constraints\\UniqueEntityValidator`   |
+----------------+-------------------------------------------------------------------------------------+

Uso di base
-----------

Si supponga di avere un ``AcmeUserBundle`` con un entità ``User``, che ha un campo
``email``. Si può usare il vincolo ``Unique`` per garantire che il
campo ``email`` rimanga unico tra tutti i vincoli della propria tabella degli
utenti:

.. configuration-block::

    .. code-block:: yaml

        # src/Acme/UserBundle/Resources/config/validation.yml
        Acme\UserBundle\Entity\Author:
            constraints:
                - Symfony\Bridge\Doctrine\Validator\Constraints\UniqueEntity: email
            properties:
                email:
                    - Email: ~

    .. code-block:: php-annotations

        // Acme/UserBundle/Entity/Author.php
        namespace Acme\UserBundle\Entity;

        use Symfony\Component\Validator\Constraints as Assert;
        use Doctrine\ORM\Mapping as ORM;

        // NON dimenticare questa istruzione!!!
        use Symfony\Bridge\Doctrine\Validator\Constraints\UniqueEntity;

        /**
         * @ORM\Entity
         * @UniqueEntity("email")
         */
        class Author
        {
            /**
             * @var string $email
             *
             * @ORM\Column(name="email", type="string", length=255, unique=true)
             * @Assert\Email()
             */
            protected $email;

            // ...
        }

    .. code-block:: xml

        <!-- src/Acme/AdministrationBundle/Resources/config/validation.xml -->
        <?xml version="1.0" encoding="UTF-8" ?>
        <constraint-mapping xmlns="http://symfony.com/schema/dic/constraint-mapping"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://symfony.com/schema/dic/constraint-mapping http://symfony.com/schema/dic/constraint-mapping/constraint-mapping-1.0.xsd">

            <class name="Acme\UserBundle\Entity\Author">
                <constraint name="Symfony\Bridge\Doctrine\Validator\Constraints\UniqueEntity">
                    <option name="fields">email</option>
                </constraint>
                <property name="email">
                    <constraint name="Email" />
                </property>
            </class>
        </constraint-mapping>

    .. code-block:: php


        // Acme/UserBundle/Entity/User.php
        namespace Acme\UserBundle\Entity;

        use Symfony\Component\Validator\Constraints as Assert;

        // NON dimenticare questa istruzione!!!
        use Symfony\Bridge\Doctrine\Validator\Constraints\UniqueEntity;

        class Author
        {
            public static function loadValidatorMetadata(ClassMetadata $metadata)
            {
                $metadata->addConstraint(new UniqueEntity(array(
                    'fields'  => 'email',
                )));

                $metadata->addPropertyConstraint('email', new Assert\Email());
            }
        }

Opzioni
-------

fields
~~~~~~

**tipo**: ``array`` o ``stringa`` [:ref:`opzione predefinita<validation-default-option>`]

Questa opzione obbligatoria è il campo (o la lista di campi) per cui l'entità deve essere
unica. Per esempio, si può specificare che i campi email e nome nell'esempio
in un unico vincolo ``UniqueEntity``, ci si assicurerà che la combinazione di valori
sia univoca (cioè che due utenti possano avere la stessa email,
purché non abbiano anche lo stesso nome).

Se servono due campi che siano individualmente univoci (p.e. un'emila univoca *e*
un nome utente univoco), usare due voci ``UniqueEntity``,
ciascuna con un singolo campo.

message
~~~~~~~

**tipo**: ``stringa`` **predefinito**: ``This value is already used.``

Messaggio mostrato quanto il vincolo fallisce.

em
~~

**tipo**: ``stringa``

Nome del gestore di entità da usare per eseguire la query che determina
l'unicità. Se lasciato vuoto, sarà determinato il gestore di entità corretto
per questa classe. Per questo motivo, probabilmente non occorre usare questa
opzione.

repositoryMethod
~~~~~~~~~~~~~~~~

**tipo**: ``stringa`` **predefinito**: ``findBy``

Il nome del metodo del repository da usare per eseguire la query che determina
l'univocità. Se lasciato vuoto, sarà usato il metodo ``findBy``. Questo
metodo deve restituire un risultato che sia contabile.

errorPath
~~~~~~~~~

**tipo**: ``stringa`` **predefinito**: Nome del primo campo in `fields`_

.. versionadded:: 2.1
    L'opzione ``errorPath`` è stata aggiunta in Symfony 2.1.

Se l'entità viola il vincolo, il messaggio di errore è legato al primo
campo in `fields`_. Se ci sono più campi, si può scegliere di legare il
messaggio di errore a un altro campo.

Si consideri questo esempio:

.. configuration-block::

    .. code-block:: yaml

        # src/Acme/AdministrationBundle/Resources/config/validation.yml
        Acme\AdministrationBundle\Entity\Service:
            constraints:
                - Symfony\Bridge\Doctrine\Validator\Constraints\UniqueEntity:
                    fields: [host, port]
                    errorPath: port
                    message: 'This port is already in use on that host.'

    .. code-block:: php-annotations

        // src/Acme/AdministrationBundle/Entity/Service.php
        namespace Acme\AdministrationBundle\Entity;

        use Doctrine\ORM\Mapping as ORM;
        use Symfony\Bridge\Doctrine\Validator\Constraints\UniqueEntity;

        /**
         * @ORM\Entity
         * @UniqueEntity(
         *     fields={"host", "port"},
         *     errorPath="port",
         *     message="This port is already in use on that host."
         * )
         */
        class Service
        {
            /**
             * @ORM\ManyToOne(targetEntity="Host")
             */
            public $host;

            /**
             * @ORM\Column(type="integer")
             */
            public $port;
        }

    .. code-block:: xml

        <!-- src/Acme/AdministrationBundle/Resources/config/validation.xml -->
        <?xml version="1.0" encoding="UTF-8" ?>
        <constraint-mapping xmlns="http://symfony.com/schema/dic/constraint-mapping"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://symfony.com/schema/dic/constraint-mapping http://symfony.com/schema/dic/constraint-mapping/constraint-mapping-1.0.xsd">

            <class name="Acme\AdministrationBundle\Entity\Service">
                <constraint name="Symfony\Bridge\Doctrine\Validator\Constraints\UniqueEntity">
                    <option name="fields">
                        <value>host</value>
                        <value>port</value>
                    </option>
                    <option name="errorPath">port</option>
                    <option name="message">This port is already in use on that host.</option>
                </constraint>
            </class>

        </constraint-mapping>

    .. code-block:: php

        // src/Acme/AdministrationBundle/Entity/Service.php
        namespace Acme\AdministrationBundle\Entity;

        use Symfony\Component\Validator\Mapping\ClassMetadata;
        use Symfony\Bridge\Doctrine\Validator\Constraints\UniqueEntity;

        class Service
        {
            public $host;
            public $port;

            public static function loadValidatorMetadata(ClassMetadata $metadata)
            {
                $metadata->addConstraint(new UniqueEntity(array(
                    'fields'    => array('host', 'port'),
                    'errorPath' => 'port',
                    'message'   => 'This port is already in use on that host.',
                )));
            }
        }

Con tale configurazione, il messaggio è legato al campo ``port``.

ignoreNull
~~~~~~~~~~

**type**: ``booleano`` **default**: ``true``

.. versionadded:: 2.1
    L'opzione ``ignoreNull`` è stata aggiunta in Symfony 2.1.

Se quest'opzione è impostata a ``true`` il vincolo permetterà di avere diverse
entità con valore ``null`` per un campo specifico senza far fallire la validazione.
Se impostata a ``false`` solamente un valore ``null`` sarà permesso, in caso di un
secondo valore ``null`` la validazione fallirà.
