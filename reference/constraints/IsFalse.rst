False
=====

Valida che un valore sia ``false``. Nello specifico, verifica se il valore sia
esattamente ``false``, esattamente l'intero ``0`` o esattamente la stringa
"``0``".

Vedere anche :doc:`IsTrue <IsTrue>`.

+----------------+-----------------------------------------------------------------------+
| Si applica a   | :ref:`proprietà o metodo<validation-property-target>`                 |
+----------------+-----------------------------------------------------------------------+
| Opzioni        | - `message`_                                                          |
|                | - `payload`_                                                          |
+----------------+-----------------------------------------------------------------------+
| Classe         | :class:`Symfony\\Component\\Validator\\Constraints\\IsFalse`          |
+----------------+-----------------------------------------------------------------------+
| Validatore     | :class:`Symfony\\Component\\Validator\\Constraints\\IsFalseValidator` |
+----------------+-----------------------------------------------------------------------+

Uso di base
-----------

Il vincolo ``IsFalse`` può essere applicato a una proprietà o a un metodo "getter",
ma è usato più comunemente nel secondo caso. Per esempio, si supponga di voler
garantire che una proprietà ``state`` *non* sia in un array dinamico
``invalidStates``. Per prima cosa, creare un metodo "getter"::

    protected $state;

    protected $invalidStates = array();

    public function isStateInvalid()
    {
        return in_array($this->state, $this->invalidStates);
    }

In questo caso, l'oggetto sottostante è valido solamente se il metodo ``isStateInvalid``
restituisce **false**:

.. configuration-block::

    .. code-block:: php-annotations

        // src/AppBundle/Entity/Author.php
        namespace AppBundle\Entity;

        use Symfony\Component\Validator\Constraints as Assert;

        class Author
        {
            /**
             * @Assert\IsFalse(
             *     message = "You've entered an invalid state."
             * )
             */
             public function isStateInvalid()
             {
                // ...
             }
        }

    .. code-block:: yaml

        # src/BlogBundle/Resources/config/validation.yml
        Acme\BlogBundle\Entity\Author
            getters:
                stateInvalid:
                    - 'IsFalse':
                        message: You've entered an invalid state.

    .. code-block:: xml

        <!-- src/AppBundle/Resources/config/validation.xml -->
        <?xml version="1.0" encoding="UTF-8" ?>
        <constraint-mapping xmlns="http://symfony.com/schema/dic/constraint-mapping"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://symfony.com/schema/dic/constraint-mapping http://symfony.com/schema/dic/constraint-mapping/constraint-mapping-1.0.xsd">

            <class name="AppBundle\Entity\Author">
                <getter property="stateInvalid">
                    <constraint name="IsFalse">
                        <option name="message">You've entered an invalid state.</option>
                    </constraint>
                </getter>
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
                $metadata->addGetterConstraint('stateInvalid', new Assert\IsFalse());
            }
        }

Opzioni
-------

message
~~~~~~~

**tipo**: ``stringa`` **predefinito**: ``This value should be false``

Messaggio mostrato se i dati sottostanti non sono ``false``.

.. include:: /reference/constraints/_payload-option.rst.inc
