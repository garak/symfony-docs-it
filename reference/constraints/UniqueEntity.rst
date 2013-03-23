UniqueEntity
============

Valida che un particolare campo (o campi) in un entità Doctrine sia unico.
Si usa di solito, per esempio, per prevenire che un nuovo utente si registri
usando un indirizzo email già esistente nel sistema.

+----------------+-------------------------------------------------------------------------------------+
| Si applica a   | :ref:`class<validation-class-target>`                                               |
+----------------+-------------------------------------------------------------------------------------+
| Opzioni        | - `fields`_                                                                         |
|                | - `message`_                                                                        |
|                | - `em`_                                                                             |
|                | - `repositoryMethod`_                                                               |
+----------------+-------------------------------------------------------------------------------------+
| Classe         | :class:`Symfony\\Bridge\\Doctrine\\Validator\\Constraints\\UniqueEntity`            |
+----------------+-------------------------------------------------------------------------------------+
| Validatore     | :class:`Symfony\\Bridge\\Doctrine\\Validator\\Constraints\\UniqueEntity\\Validator` |
+----------------+-------------------------------------------------------------------------------------+

Uso di base
-----------

Si supponga di avere un ``AcmeUserBundle`` con un entità ``User``, che ha un campo
``email``. Si può usare il vincolo ``Unique`` per garantire che il campo
``email`` rimanga unico tra tutti i vincoli della propria tabella degli
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

        // Acme/UserBundle/Entity/User.php
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

        <class name="Acme\UserBundle\Entity\Author">
            <constraint name="Symfony\Bridge\Doctrine\Validator\Constraints\UniqueEntity">
                <option name="fields">email</option>
                <option name="message">This email already exists.</option>
            </constraint>
             <property name="email">
                <constraint name="Email" />
            </property>
        </class>

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
                    'message' => 'This email already exists.',
                )));

                $metadata->addPropertyConstraint(new Assert\Email());
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

.. versionadded:: 2.1
    L'opzione ``repositoryMethod`` è stata aggiunta in Symfony 2.1. In precedenza, veniva
    usato sempre il metodo ``findBy``.

Il nome del metodo del repository da usare per eseguire la query che determina
l'univocità. Se lasciato vuoto, sarà usato il metodo ``findBy``. Questo
metodo deve restituire un risultato che sia contabile.
