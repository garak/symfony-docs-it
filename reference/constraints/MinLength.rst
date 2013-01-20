MinLength
=========

.. caution::

    Questo vincolo è deprecato dalla versione 2.1 e sarà rimosso
    in Symfony 2.3. Usare :doc:`/reference/constraints/Length` con opzione ``min``
    al suo posto.

Valida che la lunghezza di una stringa sia almeno pari al limite dato.

+----------------+-------------------------------------------------------------------------+
| Si applica a   | :ref:`proprietà o metodo<validation-property-target>`                   |
+----------------+-------------------------------------------------------------------------+
| Opzioni        | - `limit`_                                                              |
|                | - `message`_                                                            |
|                | - `charset`_                                                            |
+----------------+-------------------------------------------------------------------------+
| Classe         | :class:`Symfony\\Component\\Validator\\Constraints\\MinLength`          |
+----------------+-------------------------------------------------------------------------+
| Validatore     | :class:`Symfony\\Component\\Validator\\Constraints\\MinLengthValidator` |
+----------------+-------------------------------------------------------------------------+

Uso di base
-----------

.. configuration-block::

    .. code-block:: yaml

        # src/Acme/BlogBundle/Resources/config/validation.yml
        Acme\BlogBundle\Entity\Blog:
            properties:
                firstName:
                    - MinLength: { limit: 3, message: "Your name must have at least {{ limit }} characters." }

    .. code-block:: php-annotations

        // src/Acme/BlogBundle/Entity/Blog.php
        namespace Acme\BlogBundle\Entity;

        use Symfony\Component\Validator\Constraints as Assert;

        class Blog
        {
            /**
             * @Assert\MinLength(
             *     limit=3,
             *     message="Your name must have at least {{ limit }} characters."
             * )
             */
            protected $summary;
        }

    .. code-block:: xml

        <!-- src/Acme/BlogBundle/Resources/config/validation.xml -->
        <class name="Acme\BlogBundle\Entity\Blog">
            <property name="summary">
                <constraint name="MinLength">
                    <option name="limit">3</option>
                    <option name="message">Your name must have at least {{ limit }} characters.</option>
                </constraint>
            </property>
        </class>

    .. code-block:: php

        // src/Acme/BlogBundle/Entity/Blog.php
        namespace Acme\BlogBundle\Entity;

        use Symfony\Component\Validator\Mapping\ClassMetadata;
        use Symfony\Component\Validator\Constraints as Assert;

        class Blog
        {
            public static function loadValidatorMetadata(ClassMetadata $metadata)
            {
                $metadata->addPropertyConstraint('summary', new Assert\MinLength(array(
                    'limit'   => 3,
                    'message' => 'Your name must have at least {{ limit }} characters.',
                )));
            }
        }

Opzioni
-------

limit
~~~~~

**tipo**: ``intero`` [:ref:`opzione predefinita<validation-default-option>`]

Questa opzione obbligatoria è il valore minimo. La validazione fallisce se la lunghezza
della stringa fornita è **minore** di questo numero.

message
~~~~~~~

**tipo**: ``stringa`` **predefinito**: ``This value is too short. It should have {{ limit }} characters or more``

Messaggio mostrato se la stringa sottostante ha una lunghezza inferiore
all'opzione `limit`_.

charset
~~~~~~~

**tipo**: ``charset`` **predefinito**: ``UTF-8``

Se l'estensione "mbstring" di PHP è installata, sarà usata la funzione :phpfunction:`mb_strlen` di
PHP per calcolare la lunghezza della stringa. Il valore dell'opzione ``charset``
è passato come secondo parametro a tale funzione.
