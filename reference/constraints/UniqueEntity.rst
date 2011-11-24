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
+----------------+-------------------------------------------------------------------------------------+
| Classe         | :class:`Symfony\\Bridge\\Doctrine\\Validator\\Constraints\\UniqueEntity`            |
+----------------+-------------------------------------------------------------------------------------+
| Validatore     | :class:`Symfony\\Bridge\\Doctrine\\Validator\\Constraints\\UniqueEntity\\Validator` |
+----------------+-------------------------------------------------------------------------------------+

Uso di base
-----------

Si supponga di avere un ``AcmeUserBundle`` con un entità ``User``, che ha un campo
``email``. Si può usare il vincolo ``Unique`` per garantire che il campo
``email`` rimanga unico tra tutti i vincoli della propria tabella degli utenti:

.. configuration-block::

    .. code-block:: php-annotations

        // Acme/UserBundle/Entity/User.php
        use Symfony\Component\Validator\Constraints as Assert;
        use Symfony\Bridge\Doctrine\Validator\Constraints as DoctrineAssert;
        use Doctrine\ORM\Mapping as ORM;

        /**
         * @ORM\Entity
         * @DoctrineAssert\UniqueEntity("email")
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

    .. code-block:: yaml

        # src/Acme/UserBundle/Resources/config/validation.yml
        vincoli:
            - UniqueEntity: email

Opzioni
-------

fields
~~~~~~

**tipo**: ``array``|``stringa`` [:ref:`opzione predefinita<validation-default-option>`]

Questa opzione obbligatoria è il campo (o la lista di campi) per cui l'entità deve essere
unica. Per esempio, si può specificare che i campi email e nome nell'esempio
precedente debbano essere unici.

message
~~~~~~~

**tipo**: ``stringa`` **predefinito**: ``This value is already used.``

Messaggio mostrato quanto il vincolo fallisce.

em
~~

**tipo**: ``stringa``

Nome del gestore di entità da usare per eseguire la query che determina
l'unicità. Se lasciato vuoto, sarà usato il gestore di entità predefinito.