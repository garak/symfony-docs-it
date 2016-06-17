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
| Si applica a   | :ref:`classi <validation-class-target>`                                |
+----------------+------------------------------------------------------------------------+
| Opzioni        | - :ref:`callback <callback-option>`                                    |
|                | - `payload`_                                                           |
+----------------+------------------------------------------------------------------------+
| Classe         | :class:`Symfony\\Component\\Validator\\Constraints\\Callback`          |
+----------------+------------------------------------------------------------------------+
| Validatore     | :class:`Symfony\\Component\\Validator\\Constraints\\CallbackValidator` |
+----------------+------------------------------------------------------------------------+

Preparazione
------------

.. configuration-block::

    .. code-block:: php-annotations

        // src/AppBundle/Entity/Author.php
        namespace AppBundle\Entity;

        use Symfony\Component\Validator\Constraints as Assert;
        use Symfony\Component\Validator\Context\ExecutionContextInterface;
        // se si usa la vecchia API di validazione, si deve usare invece
        // use Symfony\Component\Validator\ExecutionContextInterface;

        class Author
        {
            /**
             * @Assert\Callback
             */
            public function validate(ExecutionContextInterface $context)
            {
                // ...
            }
        }

    .. code-block:: yaml

        # src/AppBundle/Resources/config/validation.yml
        AppBundle\Entity\Author:
            constraints:
                - Callback: [validate]

    .. code-block:: xml

        <!-- src/AppBundle/Resources/config/validation.xml -->
        <?xml version="1.0" encoding="UTF-8" ?>
        <constraint-mapping xmlns="http://symfony.com/schema/dic/constraint-mapping"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://symfony.com/schema/dic/constraint-mapping http://symfony.com/schema/dic/constraint-mapping/constraint-mapping-1.0.xsd">

            <class name="AppBundle\Entity\Author">
                <constraint name="Callback">validate</constraint>
            </class>
        </constraint-mapping>

    .. code-block:: php

        // src/AppBundle/Entity/Author.php
        namespace AppBundle\Entity;

        use Symfony\Component\Validator\Mapping\ClassMetadata;
        use Symfony\Component\Validator\Constraints as Assert;

        class Author
        {
            public static function loadValidatorMetadata(ClassMetadata $metadata)
            {
                $metadata->addConstraint(new Assert\Callback('validate'));
            }
        }

Il metodo callback
------------------

Al metodo callback è passato uno speciale oggetto ``ExecutionContextInterface``. Si possono
impostare le "violazioni" direttamente su questo oggetto e determinare a quale campo
questi errori vadano attribuiti::

    // ...
    use Symfony\Component\Validator\Context\ExecutionContextInterface;
        // se si usa la vecchia API di validazione, si deve usare invece
        // use Symfony\Component\Validator\ExecutionContextInterface;

    class Author
    {
        // ...
        private $firstName;

        public function validate(ExecutionContextInterface $context)
        {
            // si ha in qualche modo un array di nomi fasulli
            $fakeNames = array(/* ... */);

            // verifica se il nome è in effetti un nome fasullo
            if (in_array($this->getFirstName(), $fakeNames)) {
                // Se si usa la nuova API di validazione (probabile)
                $context->buildViolation('Questo nome sembra proprio falso!')
                    ->atPath('firstName')
                    ->addViolation();

                // Se si usa la vecchia API di validazione
                /*
                $context->addViolationAt(
                    'firstName',
                    'Questo nome sembra proprio falso!'
                );
                */
            }
        }
    }

Callback statici
----------------

Si possono anche usare vincoli con metodi statici. Poiché i metodi statici non possono
accedere all'istanza dell'oggetto, ricevono l'oggetto stesso come primo parametro::

    public static function validate($object, ExecutionContextInterface $context)
    {
        // si ha in qualche modo un array di nomi fasulli
        $fakeNames = array(/* ... */);

        // verifica se il nome è in effetti un nome fasullo
        if (in_array($object->getFirstName(), $fakeNames)) {
            // Se si usa la nuova API di validazione (probabile)
            $context->buildViolation('Questo nome sembra proprio falso!')
                ->atPath('firstName')
                ->addViolation()
            ;

            // Se si usa la vecchia API di validazione
            /*
            $context->addViolationAt(
                'firstName',
                'Questo nome sembra proprio falso!'
            );
            */
        }
    }

Callback esterni e closure
--------------------------

Se si vuole eseguire un metodo callback statico che non faccia parte della classe
dell'oggetto da validare, si può configurare il vincolo per invocare un array di un
callable, come supportato dalla funzione :phpfunction:`call_user_func` di PHP. Si supponga
che la funzione di validazione sia ``Vendor\Package\Validator::validate()``::

    namespace Vendor\Package;

    use Symfony\Component\Validator\Context\ExecutionContextInterface;
    // if you're using the older 2.4 validation API, you'll need this instead
    // use Symfony\Component\Validator\ExecutionContextInterface;

    class Validator
    {
        public static function validate($object, ExecutionContextInterface $context)
        {
            // ...
        }
    }

Si può quindi usare la seguente configurazione per invocare il validatore:

.. configuration-block::

    .. code-block:: php-annotations

        // src/AppBundle/Entity/Author.php
        namespace AppBundle\Entity;

        use Symfony\Component\Validator\Constraints as Assert;

        /**
         * @Assert\Callback({"Vendor\Package\Validator", "validate"})
         */
        class Author
        {
        }

    .. code-block:: yaml

        # src/AppBundle/Resources/config/validation.yml
        AppBundle\Entity\Author:
            constraints:
                - Callback: [Vendor\Package\Validator, validate]

    .. code-block:: xml

        <!-- src/AppBundle/Resources/config/validation.xml -->
        <?xml version="1.0" encoding="UTF-8" ?>
        <constraint-mapping xmlns="http://symfony.com/schema/dic/constraint-mapping"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://symfony.com/schema/dic/constraint-mapping http://symfony.com/schema/dic/constraint-mapping/constraint-mapping-1.0.xsd">

            <class name="AppBundle\Entity\Author">
                <constraint name="Callback">
                    <value>Vendor\Package\Validator</value>
                    <value>validate</value>
                </constraint>
            </class>
        </constraint-mapping>

    .. code-block:: php

        // src/AppBundle/Entity/Author.php
        namespace AppBundle\Entity;

        use Symfony\Component\Validator\Mapping\ClassMetadata;
        use Symfony\Component\Validator\Constraints as Assert;

        class Author
        {
            public static function loadValidatorMetadata(ClassMetadata $metadata)
            {
                $metadata->addConstraint(new Assert\Callback(array(
                    'Vendor\Package\Validator',
                    'validate',
                )));
            }
        }

.. note::

    Il vincolo Callback *non* supporta funzioni globali di callback, né
    è possibile specificare una funzione globale o un metodo :term:`servizio`
    come callback. Per una validazione tramite servizio, si dovrebbe
    :doc:`creare un vincolo personalizzato </cookbook/validation/custom_constraint>`
    e aggiungerlo alla classe.

Quando si configura il vincolo tramite PHP, si può anche passare una closure al
costruttore di Callback::

    // src/AppBundle/Entity/Author.php
    namespace AppBundle\Entity;

    use Symfony\Component\Validator\Context\ExecutionContextInterface;
    // Se si usa la vecchia API di validazione, usare invece
    // use Symfony\Component\Validator\ExecutionContextInterface;

    use Symfony\Component\Validator\Mapping\ClassMetadata;
    use Symfony\Component\Validator\Constraints as Assert;

    class Author
    {
        public static function loadValidatorMetadata(ClassMetadata $metadata)
        {
            $callback = function ($object, ExecutionContextInterface $context) {
                // ...
            };

            $metadata->addConstraint(new Assert\Callback($callback));
        }
    }

Opzioni
-------

.. _callback-option:

callback
~~~~~~~~

**tipo**: ``string``, ``array`` o ``Closure`` [:ref:`default option <validation-default-option>`]

Questa opzione accetta tre diversi formati per specificare il metodo
callback:

* Una **stringa** con il nome del metodo concreto o statico;

* Un array con un callable in formato ``array('<Classe>', '<metodo>')``;

* Una closure.

I callback concreti ricevono un'istanza di :class:`Symfony\\Component\\Validator\\Context\\ExecutionContextInterface`
come unico parametro.

I callback statici o closuer ricevono l'oggetto da validare come primo parametro
e un'istanza di :class:`Symfony\\Component\\Validator\\ExecutionContextInterface`
come secondo parametro.

.. include:: /reference/constraints/_payload-option.rst.inc
