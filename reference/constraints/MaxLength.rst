MaxLength
=========

.. caution::

    Questo vincolo è deprecato dalla versione 2.1 e sarà rimosso
    in Symfony 2.3. Usare :doc:`/reference/constraints/Length` con opzione ``max``
    al suo posto.

Valida che la lunghezza di una stringa non sia superiore al limite dato.

+----------------+-------------------------------------------------------------------------+
| Si applica a   | :ref:`proprietà o metodo<validation-property-target>`                   |
+----------------+-------------------------------------------------------------------------+
| Opzioni        | - `limit`_                                                              |
|                | - `message`_                                                            |
|                | - `charset`_                                                            |
+----------------+-------------------------------------------------------------------------+
| Classe         | :class:`Symfony\\Component\\Validator\\Constraints\\MaxLength`          |
+----------------+-------------------------------------------------------------------------+
| Validatore     | :class:`Symfony\\Component\\Validator\\Constraints\\MaxLengthValidator` |
+----------------+-------------------------------------------------------------------------+

Uso di base
-----------

.. configuration-block::

    .. code-block:: yaml

        # src/Acme/BlogBundle/Resources/config/validation.yml
        Acme\BlogBundle\Entity\Blog:
            properties:
                summary:
                    - MaxLength: 100
    
    .. code-block:: php-annotations

        // src/Acme/BlogBundle/Entity/Blog.php
        namespace Acme\BlogBundle\Entity;

        use Symfony\Component\Validator\Constraints as Assert;

        class Blog
        {
            /**
             * @Assert\MaxLength(100)
             */
            protected $summary;
        }
    
    .. code-block:: xml

        <!-- src/Acme/BlogBundle/Resources/config/validation.xml -->
        <?xml version="1.0" encoding="UTF-8" ?>
        <constraint-mapping xmlns="http://symfony.com/schema/dic/constraint-mapping"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://symfony.com/schema/dic/constraint-mapping http://symfony.com/schema/dic/constraint-mapping/constraint-mapping-1.0.xsd">

            <class name="Acme\BlogBundle\Entity\Blog">
                <property name="summary">
                    <constraint name="MaxLength">
                        <option name="limit">100</option>
                    </constraint>
                </property>
            </class>
        </constraint-mapping>

    .. code-block:: php

        // src/Acme/BlogBundle/Entity/Blog.php
        namespace Acme\BlogBundle\Entity;

        use Symfony\Component\Validator\Mapping\ClassMetadata;
        use Symfony\Component\Validator\Constraints as Assert;

        class Blog
        {
            public static function loadValidatorMetadata(ClassMetadata $metadata)
            {
                $metadata->addPropertyConstraint('summary', new Assert\MaxLength(array(
                    'limit' => 100,
                )));
            }
        }

Opzioni
-------

limit
~~~~~

**tipo**: ``intero`` [:ref:`opzione predefinita<validation-default-option>`]

Questa opzione obbligatoria è il valore massimo. La validazione fallisce se la lunghezza
della stringa fornita è **maggiore** di questo numero.

message
~~~~~~~

**tipo**: ``stringa`` **predefinito**: ``This value is too long. It should have {{ limit }} characters or less``

Messaggio mostrato se la stringa sottostante ha una lunghezza superiore
all'opzione `limit`_.

charset
~~~~~~~

**tipo**: ``charset`` **predefinito**: ``UTF-8``

Se l'estensione "mbstring" di PHP è installata, sarà usata la funzione :phpfunction:`mb_strlen` di
PHP per calcolare la lunghezza della stringa. Il valore dell'opzione ``charset``
è passato come secondo parametro a tale funzione.
