Callback
========

Lo scopo del vincolo Callback è di poter creare delle regole di validazione
completamente personalizzate e di assegnare qualsiasi errore di validazione a
campi specifici di un oggetto. Se si usa la validazione con i form, questo vuol dire
che si possono mostrare questi errori personalizzati accanto a campi specifici, invece di
mostrarli semplicemente in cima al form.

Questo processo funziona specificando uno o più metodi di *callback*, ciascuno dei quali
sarà richiamato durante il processo di validazione. Ognuno di questi metodi può
fare qualsiasi cosa, incluso creare e assegnare errori di validazione.

.. note::

    Un metodo callback per sé stesso non *fallisce*, né restituisce valori. Invece,
    come vedremo nell'esempio, un metodo callback ha l'abilità di aggiungere direttamente
    "violazioni" al validatore.

+----------------+------------------------------------------------------------------------+
| Si applica a   | :ref:`classe <validation-class-target>`                                |
+----------------+------------------------------------------------------------------------+
| Opzioni        | - `methods`_                                                           |
+----------------+------------------------------------------------------------------------+
| Classe         | :class:`Symfony\\Component\\Validator\\Constraints\\Callback`          |
+----------------+------------------------------------------------------------------------+
| Validatore     | :class:`Symfony\\Component\\Validator\\Constraints\\CallbackValidator` |
+----------------+------------------------------------------------------------------------+

Preparazione
------------

.. configuration-block::

    .. code-block:: php-annotations

        // src/Acme/BlogBundle/Entity/Author.php
        namespace Acme\BlogBundle\Entity;

        use Symfony\Component\Validator\Constraints as Assert;

        /**
         * @Assert\Callback(methods={"isAuthorValid"})
         */
        class Author
        {
        }

    .. code-block:: yaml

        # src/Acme/BlogBundle/Resources/config/validation.yml
        Acme\BlogBundle\Entity\Author:
            constraints:
                - Callback:
                    methods:   [isAuthorValid]

    .. code-block:: xml

        <!-- src/Acme/BlogBundle/Resources/config/validation.xml -->
        <?xml version="1.0" encoding="UTF-8" ?>
        <constraint-mapping xmlns="http://symfony.com/schema/dic/constraint-mapping"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://symfony.com/schema/dic/constraint-mapping http://symfony.com/schema/dic/constraint-mapping/constraint-mapping-1.0.xsd">

            <class name="Acme\BlogBundle\Entity\Author">
                <constraint name="Callback">
                    <option name="methods">
                        <value>isAuthorValid</value>
                    </option>
                </constraint>
            </class>
        </constraint-mapping>

    .. code-block:: php

        // src/Acme/BlogBundle/Entity/Author.php
        namespace Acme\BlogBundle\Entity;

        use Symfony\Component\Validator\Mapping\ClassMetadata;
        use Symfony\Component\Validator\Constraints as Assert;

        class Author
        {
            public static function loadValidatorMetadata(ClassMetadata $metadata)
            {
                $metadata->addConstraint(new Assert\Callback(array(
                    'methods' => array('isAuthorValid'),
                )));
            }
        }

Il metodo callback
------------------

Il metodo callback è passato a uno speciale oggetto ``ExecutionContextInterface``. Si possono
impostare le "violazioni" direttamente su questo oggetto e determinare a quale campo
questi errori vadano attribuiti::

    // ...
    use Symfony\Component\Validator\ExecutionContextInterface;

    class Author
    {
        // ...
        private $firstName;

        public function isAuthorValid(ExecutionContextInterface $context)
        {
            // si ha in qualche modo un array di nomi fasulli
            $fakeNames = array();

            // verifica se il nome è in effetti un nome fasullo
            if (in_array($this->getFirstName(), $fakeNames)) {
                $context->addViolationAt(
                    'firstname',
                    'Questo nome  sembra proprio falso!',
                    array(),
                    null
                );
            }
        }
    }

Opzioni
-------

methods
~~~~~~~

**tipo**: ``array`` **predefinito**: ``array()`` [:ref:`opzione predefinita<validation-default-option>`]

Un array di metodi che andrebbero eseguiti durante il processo di validazione.
Ogni metodo può avere uno dei seguenti formati:

1) **Stringa con il nome del metodo**

    Se il nome di un metodo è una semplice stringa (p.e. ``isAuthorValid``), quel
    metodo sarà richiamato sullo stesso oggetto in corso di validazione e
    ``ExecutionContext`` sarà l'unico parametro (vedere esempio
    precedente).

2) **Array statico callback**

    Ogni metodo può anche essere specificato con un array callback:

    .. configuration-block::

        .. code-block:: php-annotations

            // src/Acme/BlogBundle/Entity/Author.php
            use Symfony\Component\Validator\Constraints as Assert;

            /**
             * @Assert\Callback(methods={
             *     { "Acme\BlogBundle\MyStaticValidatorClass", "isAuthorValid" }
             * })
             */
            class Author
            {
            }

        .. code-block:: yaml

            # src/Acme/BlogBundle/Resources/config/validation.yml
            Acme\BlogBundle\Entity\Author:
                constraints:
                    - Callback:
                        methods:
                            -    [Acme\BlogBundle\MyStaticValidatorClass, isAuthorValid]

        .. code-block:: xml

            <!-- src/Acme/BlogBundle/Resources/config/validation.xml -->
            <?xml version="1.0" encoding="UTF-8" ?>
            <constraint-mapping xmlns="http://symfony.com/schema/dic/constraint-mapping"
                xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
                xsi:schemaLocation="http://symfony.com/schema/dic/constraint-mapping http://symfony.com/schema/dic/constraint-mapping/constraint-mapping-1.0.xsd">

                <class name="Acme\BlogBundle\Entity\Author">
                    <constraint name="Callback">
                        <option name="methods">
                            <value>
                                <value>Acme\BlogBundle\MyStaticValidatorClass</value>
                                <value>isAuthorValid</value>
                            </value>
                        </option>
                    </constraint>
                </class>
            </constraint-mapping>

        .. code-block:: php

            // src/Acme/BlogBundle/Entity/Author.php

            use Symfony\Component\Validator\Mapping\ClassMetadata;
            use Symfony\Component\Validator\Constraints\Callback;

            class Author
            {
                public $name;

                public static function loadValidatorMetadata(ClassMetadata $metadata)
                {
                    $metadata->addConstraint(new Callback(array(
                        'methods' => array(
                            array(
                                'Acme\BlogBundle\MyStaticValidatorClass',
                                'isAuthorValid',
                            ),
                        ),
                    )));
                }
            }

    In questo caso, sarà richiamato il metodo statico ``isAuthorValid`` della classe
    ``Acme\BlogBundle\MyStaticValidatorClass``. Gli verrà passato sia l'oggetto originale
    in corso di validazione (p.e. ``Author``) che
    ``ExecutionContextInterface``::

        namespace Acme\BlogBundle;

        use Symfony\Component\Validator\ExecutionContextInterface;
        use Acme\BlogBundle\Entity\Author;

        class MyStaticValidatorClass
        {
            public static function isAuthorValid(
                Author $author,
                ExecutionContextInterface $context
            ) {
                // ...
            }
        }

    .. tip::

        Se si specifica il vincolo ``Callback`` tramite PHP, c'è anche l'opzione
        di rendere il callback una closure PHP o un callback non statico.
        Tuttavia, *non* è attualmente possibile specificare un :term:`servizio`
        come vincolo. Per validare usando un servizio, si dovrebbe
        :doc:`creare un vincolo personalizzato
        </cookbook/validation/custom_constraint>` e
        aggiungere il nuovo vincolo alla propria classe.
