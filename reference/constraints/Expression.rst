Expression
==========

.. versionadded:: 2.4
    Il vincolo Expression è stato introdotto in Symfony 2.4.

Questo vincolo consente di usare una :ref:`espressione <component-expression-language-examples>`
per una validazione dinamica e complessa. Vedere `Uso di base`_ per un esempio.
Vedere :doc:`/reference/constraints/Callback` per un altro vincolo che fornisce
flessibilità simile.

+----------------+-----------------------------------------------------------------------------------------------+
| Si applica a   | :ref:`classe <validation-class-target>` o :ref:`property/method <validation-property-target>` |
+----------------+-----------------------------------------------------------------------------------------------+
| Opzioni        | - :ref:`expression <reference-constraint-expression-option>`                                  |
|                | - `message`_                                                                                  |
+----------------+-----------------------------------------------------------------------------------------------+
| Classe         | :class:`Symfony\\Component\\Validator\\Constraints\\Expression`                               |
+----------------+-----------------------------------------------------------------------------------------------+
| Validatore     | :class:`Symfony\\Component\\Validator\\Constraints\\ExpressionValidator`                      |
+----------------+-----------------------------------------------------------------------------------------------+

Uso di base
-----------

Si immagini di avere una classe ``BlogPost`` con proprietà ``category``
e ``isTechnicalPost``::

    namespace Acme\DemoBundle\Model;

    use Symfony\Component\Validator\Constraints as Assert;

    class BlogPost
    {
        private $category;

        private $isTechnicalPost;

        // ...

        public function getCategory()
        {
            return $this->category;
        }

        public function setIsTechnicalPost($isTechnicalPost)
        {
            $this->isTechnicalPost = $isTechnicalPost;
        }

        // ...
    }

Per validare questo oggetto, si definiscono alcuni requisiti:

A) Se ``isTechnicalPost`` è ``true``, allora ``category`` deve essere ``php``
   o ``symfony``;
B) Se ``isTechnicalPost`` è ``false``, allora ``category`` può avere qualsiasi valore.

Un modo per soddisfare tali requisiti è tramite un vincolo Expression:

.. configuration-block::

    .. code-block:: yaml

        # src/Acme/DemoBundle/Resources/config/validation.yml
        Acme\DemoBundle\Model\BlogPost:
            constraints:
                - Expression:
                    expression: "this.getCategory() in ['php', 'symfony'] or !this.isTechnicalPost()"
                    message: "Per un post tecnico, la categoria deve essere php o symfony!"

    .. code-block:: php-annotations

        // src/Acme/DemoBundle/Model/BlogPost.php
        namespace Acme\DemoBundle\Model;

        use Symfony\Component\Validator\Constraints as Assert;

        /**
         * @Assert\Expression(
         *     "this.getCategory() in ['php', 'symfony'] or !this.isTechnicalPost()",
         *     message="Per un post tecnico, la categoria deve essere php o symfony!"
         * )
         */
        class BlogPost
        {
            // ...
        }

    .. code-block:: xml

        <!-- src/Acme/DemoBundle/Resources/config/validation.xml -->
        <?xml version="1.0" encoding="UTF-8" ?>
        <constraint-mapping xmlns="http://symfony.com/schema/dic/constraint-mapping"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://symfony.com/schema/dic/constraint-mapping http://symfony.com/schema/dic/constraint-mapping/constraint-mapping-1.0.xsd">
            <class name="Acme\DemoBundle\Model\BlogPost">
                <constraint name="Expression">
                    <option name="expression">
                        this.getCategory() in ['php', 'symfony'] or !this.isTechnicalPost()
                    </option>
                    <option name="message">
                        Per un post tecnico, la categoria deve essere php o symfony!
                    </option>
                </constraint>
            </class>
        </constraint-mapping>

    .. code-block:: php

        // src/Acme/DemoBundle/Model/BlogPost.php
        namespace Acme\DemoBundle\Model;

        use Symfony\Component\Validator\Mapping\ClassMetadata;
        use Symfony\Component\Validator\Constraints as Assert;

        class BlogPost
        {
            public static function loadValidatorMetadata(ClassMetadata $metadata)
            {
                $metadata->addConstraint(new Assert\Expression(array(
                    'expression' => 'this.getCategory() in ["php", "symfony"] or !this.isTechnicalPost()',
                    'message' => 'Per un post tecnico, la categoria deve essere php o symfony!',
                )));
            }

            // ...
        }

L'opzione :ref:`expression <reference-constraint-expression-option>` è
l'espressione che deve restituire ``true`` per far passare la validazione. Per saperne
di più sulla sintassi del linguaggio delle espressioni, vedere
:doc:`/components/expression_language/syntax`.

.. sidebar:: Mappare l'errore su un campo specifico

    Si può anche allegare il vincolo a una specifica proprietà e comunque validare
    in base ai valori di tutta l'entità. Questo torna utile se si vuole collegare
    l'errore a un campo specifico. In questo contesto, ``value`` rappresenta il valore
    di ``isTechnicalPost``.

    .. configuration-block::

        .. code-block:: yaml

            # src/Acme/DemoBundle/Resources/config/validation.yml
            Acme\DemoBundle\Model\BlogPost:
                properties:
                    isTechnicalPost:
                        - Expression:
                            expression: "this.getCategory() in ['php', 'symfony'] or value == false"
                            message: "Per un post tecnico, la categoria deve essere php o symfony!"

        .. code-block:: php-annotations

            // src/Acme/DemoBundle/Model/BlogPost.php
            namespace Acme\DemoBundle\Model;

            use Symfony\Component\Validator\Constraints as Assert;

            class BlogPost
            {
                // ...

                /**
                 * @Assert\Expression(
                 *     "this.getCategory() in ['php', 'symfony'] or value == false",
                 *     message="Per un post tecnico, la categoria deve essere php o symfony!"
                 * )
                 */
                private $isTechnicalPost;

                // ...
            }

        .. code-block:: xml

            <!-- src/Acme/DemoBundle/Resources/config/validation.xml -->
            <?xml version="1.0" encoding="UTF-8" ?>
            <constraint-mapping xmlns="http://symfony.com/schema/dic/constraint-mapping"
                xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
                xsi:schemaLocation="http://symfony.com/schema/dic/constraint-mapping http://symfony.com/schema/dic/constraint-mapping/constraint-mapping-1.0.xsd">

                <class name="Acme\DemoBundle\Model\BlogPost">
                    <property name="isTechnicalPost">
                        <constraint name="Expression">
                            <option name="expression">
                                this.getCategory() in ['php', 'symfony'] or value == false
                            </option>
                            <option name="message">
                                Per un post tecnico, la categoria deve essere php o symfony!
                            </option>
                        </constraint>
                    </property>
                </class>
            </constraint-mapping>

        .. code-block:: php

            // src/Acme/DemoBundle/Model/BlogPost.php
            namespace Acme\DemoBundle\Model;

            use Symfony\Component\Validator\Constraints as Assert;
            use Symfony\Component\Validator\Mapping\ClassMetadata;

            class BlogPost
            {
                public static function loadValidatorMetadata(ClassMetadata $metadata)
                {
                    $metadata->addPropertyConstraint('isTechnicalPost', new Assert\Expression(array(
                        'expression' => 'this.getCategory() in ["php", "symfony"] or value == false',
                        'message' => 'Per un post tecnico, la categoria deve essere php o symfony!',
                    )));
                }

                // ...
            }

Per maggiori informazioni sull'espressione e sulle variabili a disposizione,
vedere l'opzione :ref:`expression <reference-constraint-expression-option>`
di sequito.

Opzioni
-------

.. _reference-constraint-expression-option:

expression
~~~~~~~~~~

**tipo**: ``stringa`` [:ref:`default option <validation-default-option>`]

L'espressione da valutare. Se l'espressione viene valutata a ``false``
(usando ``==``, non ``===``), la validazione fallirà.

Per saperne di più sulla sintassi del linguaggio delle espressioni, vedere
:doc:`/components/expression_language/syntax`.

All'interno dell'espressione, si ha accesso a fino a due variabili:

A seconda di come si usa il vincolo, si ha accesso a una o a due variabili
nell'espressione:

* ``this``: l'oggetto in corso di validazione (p.e. un'istanza di BlogPost);
* ``value``: il valore della proprietà in corso di validazione (disponibile solo se
  il vincolo è applicato direttamente a una proprietà);

message
~~~~~~~

**tipo**: ``stringa`` **predefinito**: ``This value is not valid.``

Il messaggio fornito quando l'espressione viene valutata a ``false``.
