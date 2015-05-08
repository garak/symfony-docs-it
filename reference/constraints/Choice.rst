Choice
======

Questo vincolo è usato per assicurarsi che il valore fornito faccia parte
dell'insieme di scelte *valide*. Può anche essere usato per validare che ogni
elemento in un array sia una di tali scelte valide.

+----------------+-----------------------------------------------------------------------+
| Si applica a   | :ref:`proprietà o metodo <validation-property-target>`                |
+----------------+-----------------------------------------------------------------------+
| Opzioni        | - `choices`_                                                          |
|                | - `callback`_                                                         |
|                | - `multiple`_                                                         |
|                | - `min`_                                                              |
|                | - `max`_                                                              |
|                | - `message`_                                                          |
|                | - `multipleMessage`_                                                  |
|                | - `minMessage`_                                                       |
|                | - `maxMessage`_                                                       |
|                | - `strict`_                                                           |
+----------------+-----------------------------------------------------------------------+
| Classe         | :class:`Symfony\\Component\\Validator\\Constraints\\Choice`           |
+----------------+-----------------------------------------------------------------------+
| Validatore     | :class:`Symfony\\Component\\Validator\\Constraints\\ChoiceValidator`  |
+----------------+-----------------------------------------------------------------------+

Uso di base
-----------

L'idea di base di questo vincolo è quella che, una volta fornito un array di valori
validi (lo si può fare in diversi modi), esso validi che il valore della data
proprietà esista in tale array.

Se la lista di scelta è semplice, la si può passare direttamente tramite l'opzione
`choices`_:

.. configuration-block::

    .. code-block:: php-annotations

        // src/Acme/BlogBundle/Entity/Author.php
        namespace Acme\BlogBundle\Entity;

        use Symfony\Component\Validator\Constraints as Assert;

        class Author
        {
            /**
             * @Assert\Choice(choices = {"maschio", "femmina"}, message = "Scegliere un genere valido.")
             */
            protected $gender;
        }

    .. code-block:: yaml

        # src/Acme/BlogBundle/Resources/config/validation.yml
        Acme\BlogBundle\Entity\Author:
            properties:
                gender:
                    - Choice:
                        choices:  [maschio, femmina]
                        message:  Scegliere un genere valido.

    .. code-block:: xml

        <!-- src/Acme/BlogBundle/Resources/config/validation.xml -->
        <?xml version="1.0" encoding="UTF-8" ?>
        <constraint-mapping xmlns="http://symfony.com/schema/dic/constraint-mapping"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://symfony.com/schema/dic/constraint-mapping http://symfony.com/schema/dic/constraint-mapping/constraint-mapping-1.0.xsd">

            <class name="Acme\BlogBundle\Entity\Author">
                <property name="gender">
                    <constraint name="Choice">
                        <option name="choices">
                            <value>maschio</value>
                            <value>femmina</value>
                        </option>
                        <option name="message">Scegliere un genere valido.</option>
                    </constraint>
                </property>
            </class>
        </constraint-mapping>

    .. code-block:: php

        // src/Acme/BlogBundle/EntityAuthor.php
        namespace Acme\BlogBundle\Entity;

        use Symfony\Component\Validator\Mapping\ClassMetadata;
        use Symfony\Component\Validator\Constraints as Assert;

        class Author
        {
            protected $gender;

            public static function loadValidatorMetadata(ClassMetadata $metadata)
            {
                $metadata->addPropertyConstraint('gender', new Choice(
                    'choices' => array('maschio', 'femmina'),
                    'message' => 'Scegliere un genere valido.',
                ));
            }
        }

Fornire le scelte con una funzione callback
-------------------------------------------

Si può anche usare una funzione callback per specificare le opzioni. Questo è
utile, se si vogliono mantenere le scelte in un posto centralizzato, in modo
da poter accedere facilmente a tali scelte, per la validazione o per costruire
un elemento select di un form.

.. code-block:: php

    // src/Acme/BlogBundle/Entity/Author.php
    namespace Acme\BlogBundle\Entity;

    class Author
    {
        public static function getGenders()
        {
            return array('maschio', 'femmina');
        }
    }

Si può passare il nome di questo metodo all'opzione `callback`_ del vincolo
``Choice``.

.. configuration-block::

    .. code-block:: php-annotations

        // src/Acme/BlogBundle/Entity/Author.php
        namespace Acme\BlogBundle\Entity;

        use Symfony\Component\Validator\Constraints as Assert;

        class Author
        {
            /**
             * @Assert\Choice(callback = "getGenders")
             */
            protected $gender;
        }

    .. code-block:: yaml

        # src/Acme/BlogBundle/Resources/config/validation.yml
        Acme\BlogBundle\Entity\Author:
            properties:
                gender:
                    - Choice: { callback: getGenders }

    .. code-block:: xml

        <!-- src/Acme/BlogBundle/Resources/config/validation.xml -->
        <?xml version="1.0" encoding="UTF-8" ?>
        <constraint-mapping xmlns="http://symfony.com/schema/dic/constraint-mapping"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://symfony.com/schema/dic/constraint-mapping http://symfony.com/schema/dic/constraint-mapping/constraint-mapping-1.0.xsd">

            <class name="Acme\BlogBundle\Entity\Author">
                <property name="gender">
                    <constraint name="Choice">
                        <option name="callback">getGenders</option>
                    </constraint>
                </property>
            </class>
        </constraint-mapping>

    .. code-block:: php

        // src/Acme/BlogBundle/EntityAuthor.php
        namespace Acme\BlogBundle\Entity;

        use Symfony\Component\Validator\Mapping\ClassMetadata;
        use Symfony\Component\Validator\Constraints as Assert;

        class Author
        {
            protected $gender;

            public static function loadValidatorMetadata(ClassMetadata $metadata)
            {
                $metadata->addPropertyConstraint('gender', new Assert\Choice(array(
                    'callback' => 'getGenders',
                )));
            }
        }

Se il callback statico è posto in una classe diversa, per esempio ``Util``,
si può passare il nome della classe e del metodo come array.

.. configuration-block::

    .. code-block:: php-annotations

        // src/Acme/BlogBundle/Entity/Author.php
        namespace Acme\BlogBundle\Entity;

        use Symfony\Component\Validator\Constraints as Assert;

        class Author
        {
            /**
             * @Assert\Choice(callback = {"Util", "getGenders"})
             */
            protected $gender;
        }

    .. code-block:: yaml

        # src/Acme/BlogBundle/Resources/config/validation.yml
        Acme\BlogBundle\Entity\Author:
            properties:
                gender:
                    - Choice: { callback: [Util, getGenders] }

    .. code-block:: xml

        <!-- src/Acme/BlogBundle/Resources/config/validation.xml -->
        <?xml version="1.0" encoding="UTF-8" ?>
        <constraint-mapping xmlns="http://symfony.com/schema/dic/constraint-mapping"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://symfony.com/schema/dic/constraint-mapping http://symfony.com/schema/dic/constraint-mapping/constraint-mapping-1.0.xsd">

            <class name="Acme\BlogBundle\Entity\Author">
                <property name="gender">
                    <constraint name="Choice">
                        <option name="callback">
                            <value>Util</value>
                            <value>getGenders</value>
                        </option>
                    </constraint>
                </property>
            </class>
        </constraint-mapping>

    .. code-block:: php

        // src/Acme/BlogBundle/EntityAuthor.php
        namespace Acme\BlogBundle\Entity;

        use Symfony\Component\Validator\Mapping\ClassMetadata;
        use Symfony\Component\Validator\Constraints as Assert;

        class Author
        {
            protected $gender;

            public static function loadValidatorMetadata(ClassMetadata $metadata)
            {
                $metadata->addPropertyConstraint('gender', new Assert\Choice(array(
                    'callback' => array('Util', 'getGenders'),
                )));
            }
        }

Opzioni disponibili
-------------------

choices
~~~~~~~

**tipo**: ``array`` [:ref:`opzione predefinita<validation-default-option>`]

Un'opzione obbligatoria (a meno che non sia specificato `callback`_), è l'array
di opzioni da considerare nell'insieme valido. Il valore di input dovrà
corrispondere a questo array.

callback
~~~~~~~~

**tipo**: ``string|array|Closure``

Un metodo callback che può essere usato, al posto dell'opzione `choices`_, per
restituire l'array delle scelte. Vedere `Fornire le scelte con una funzione callback`_
per maggiori dettagli sul suo utilizzo.

multiple
~~~~~~~~

**tipo**: ``booleano`` **predefinito**: ``false``

Se questa opzione vale `true`, ci si aspetta come valore di input un array, invece
di un singolo valore. Il vincolo verificherà che ogni valore dell'array di input possa
essere trovato nell'array di scelte valide. Se anche uno solo dei valori di input non
viene trovato, la validazione fallisce.

min
~~~

**tipo**: ``intero``

Se l'opzione ``multiple`` vale ``true``, si può usare l'opzione ``min`` per forzare
la scelta di una quantità minima di valori. Per esempio, se 
``min`` è 3, ma l'array di input contiene solo 2 valori validi, la validazione
fallisce.

max
~~~

**tipo**: ``intero``

Se l'opzione ``multiple`` vale ``true``, si può usare l'opzione ``max`` per forzare
la scelta di una quantità massima di valori. Per esempio, se 
``max`` è 3, ma l'array di input contiene 4 valori validi, la validazione
fallisce.

message
~~~~~~~

**tipo**: ``stringa`` **predefinito**: ``The value you selected is not a valid choice``

Il messaggio che si riceverà se l'opzione ``multiple`` è impostata a
``false`` e il valore sottostante non è tra quelli dell'array di scelte valide.

multipleMessage
~~~~~~~~~~~~~~~

**tipo**: ``stringa`` **predefinito**: ``One or more of the given values is invalid``

Il messaggio che si riceverà se l'opzione ``multiple`` è impostata a
``false`` e uno dei valori dell'array in corso di validazione non è tra quelli dell'array
di scelte valide.

minMessage
~~~~~~~~~~

**tipo**: ``stringa`` **predefinito**: ``You must select at least {{ limit }} choices``

Messaggi di errore mostrato quanto l'utente seleziona troppo poche scelte, in base
all'opzione `min`_.

maxMessage
~~~~~~~~~~

**tipo**: ``stringa`` **predefinito**: ``You must select at most {{ limit }} choices``

Messaggi di errore mostrato quanto l'utente seleziona troppe scelte, in base
all'opzione `max`_.

strict
~~~~~~

**tipo**: ``booleano`` **predefinito**: ``false``

Se ``true``, il validatore verificherà anche il tipo del valore di input. In particolare,
questo valore è passato al terzo parametro della funzione :phpfunction:`in_array` di PHP, durante la
verifica se un valore è nell'array di scelte valide.
