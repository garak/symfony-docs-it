.. index::
   single: Validazione

Validazione
===========

La validazione è un compito molto comune nella applicazioni web. I dati inseriti nei form
hanno bisogno di essere validati. I dati hanno bisogno di essere validati anche prima di
essere inseriti in una base dati o passati a un servizio web.

Symfony ha un componente `Validator`_ , che rende questo compito facile e trasparente.
Questo componente è bastato sulle `specifiche di validazione
JSR303 Bean`_. 

.. index::
   single: Validazione; Le basi

Le basi della validazione
-------------------------

Il modo migliore per capire la validazione è quello di vederla in azione. Per iniziare,
supponiamo di aver creato un classico oggetto PHP, da usare in qualche parte della
propria applicazione::

    // src/AppBundle/Entity/Author.php
    namespace AppBundle\Entity;

    class Author
    {
        public $name;
    }

Finora, questa è solo una normale classe, che ha una qualche utilità all'interno della
propria applicazione. Lo scopo della validazione è dire se i dati di un oggetto siano
validi o meno. Per poterlo fare, occorre configurare una lisa di regole (chiamate
:ref:`vincoli <validation-constraints>`) che l'oggetto deve seguire per poter essere
valido. Queste regole possono essere specificate tramite diversi formati (YAML,
XML, annotazioni o PHP).

Per esempio, per garantire che la proprietà ``$name`` non sia vuota, aggiungere il
seguente:

.. configuration-block::

    .. code-block:: php-annotations

        // src/AppBundle/Entity/Author.php

        // ...
        use Symfony\Component\Validator\Constraints as Assert;

        class Author
        {
            /**
             * @Assert\NotBlank()
             */
            public $name;
        }

    .. code-block:: yaml

        # src/AppBundle/Resources/config/validation.yml
        AppBundle\Entity\Author:
            properties:
                name:
                    - NotBlank: ~

    .. code-block:: xml

        <!-- src/AppBundle/Resources/config/validation.xml -->
        <?xml version="1.0" encoding="UTF-8" ?>
        <constraint-mapping xmlns="http://symfony.com/schema/dic/constraint-mapping"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://symfony.com/schema/dic/constraint-mapping http://symfony.com/schema/dic/constraint-mapping/constraint-mapping-1.0.xsd">

            <class name="AppBundle\Entity\Author">
                <property name="name">
                    <constraint name="NotBlank" />
                </property>
            </class>
        </constraint-mapping>

    .. code-block:: php

        // src/AppBundle/Entity/Author.php

        // ...
        use Symfony\Component\Validator\Mapping\ClassMetadata;
        use Symfony\Component\Validator\Constraints\NotBlank;

        class Author
        {
            public $name;

            public static function loadValidatorMetadata(ClassMetadata $metadata)
            {
                $metadata->addPropertyConstraint('name', new NotBlank());
            }
        }

.. tip::

    Anche le proprietà private e protette possono essere validate, così come i
    metodi "getter" (vedere :ref:`validator-constraint-targets`).

.. index::
   single: Validazione; Usare il validatore

Usare il servizio ``validator``
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Successivamente, per validare veramente un oggetto ``Author``, usare il metodo
``validate`` sul servizio ``validator`` (classe :class:`Symfony\\Component\\Validator\\Validator`).
Il compito di ``validator`` è semplice: leggere i vincoli (cioè le regole) di una
classe e verificare se i dati dell'oggetto soddisfino o no tali vincoli.
Se la validazione fallisce, viene restituita una lista di errori
(classe :class:`Symfony\\Component\\Validator\\ConstraintViolationList`).
Prendiamo questo semplice esempio dall'interno di un controllore::

    // ...
    use Symfony\Component\HttpFoundation\Response;
    use AppBundle\Entity\Author;

    // ...
    public function authorAction()
    {
        $autore = new Author();

        // ... fare qualcosa con l'oggetto $autore

        $validator = $this->get('validator');
        $errori = $validator->validate($autore);

        if (count($errori) > 0) {
            /*
             * Usa un metodo a __toString sulla variabile $errors, che è un oggetto
             * ConstraintViolationList. Questo fornisce una stringa adatta
             * al debug
             */
            $errorsString = (string) $errori;

            return new Response($errorsString);
        }

        return new Response('L\'autore è valido! Sì!');
    }

Se la proprietà ``$name`` è vuota, si vedrà il seguente messaggio di
errore:

.. code-block:: text

    AppBundle\Author.name:
        This value should not be blank

Se si inserisce un valore per la proprietà ``$name``, apparirà il messaggio di
successo.

.. tip::

    La maggior parte delle volte, non si interagirà direttamente con il servizio ``validator``,
    né ci si dovrà occupare di stampare gli errori. La maggior parte delle volte,
    si userà indirettamente la validazione, durante la gestione di dati inviati tramite form. Per
    maggiori informazioni, vedere :ref:`book-validation-forms`.

Si può anche passare un insieme di errori in un template::

    if (count($errors) > 0) {
        return $this->render('author/validation.html.twig', array(
            'errors' => $errors,
        ));
    }

Dentro al template, si può stampare la lista di errori, come necessario:

.. configuration-block::

    .. code-block:: html+jinja

        {# app/Resources/views/author/validation.html.twig #}
        <h3>L'autore ha i seguenti errori</h3>
        <ul>
        {% for error in errors %}
            <li>{{ error.message }}</li>
        {% endfor %}
        </ul>

    .. code-block:: html+php

        <!-- app/Resources/views/author/validation.html.php -->
        <h3>L'autore ha i seguenti errori</h3>
        <ul>
        <?php foreach ($errors as $error): ?>
            <li><?php echo $error->getMessage() ?></li>
        <?php endforeach ?>
        </ul>

.. note::

    Ogni errore di validazione (chiamato "violazione di vincolo") è rappresentato da
    un oggetto :class:`Symfony\\Component\\Validator\\ConstraintViolation`.

.. index::
   single: Validazione; Validazione con i form

.. _book-validation-forms:

Validazione e form
~~~~~~~~~~~~~~~~~~

Il servizio ``validator`` può essere usato per validare qualsiasi oggetto. In realtà,
tuttavia, solitamente si lavorerà con ``validator`` indirettamente, lavorando con i
form. La libreria dei form di Symfony usa internamente il servizio ``validator``, per
validare l'oggetto sottostante dopo che i valori sono stati inviati e collegati. Le
violazioni dei vincoli sull'oggetto sono convertite in oggetti ``FieldError``,
che possono essere facilmente mostrati con il proprio form. Il tipico flusso dell'invio
di un form assomiglia al seguente, all'interno di un controllore::

    // ...
    use AppBundle\Entity\Author;
    use AppBundle\Form\AuthorType;
    use Symfony\Component\HttpFoundation\Request;

    // ...
    public function updateAction(Request $request)
    {
        $author = new Author();
        $form = $this->createForm(new AuthorType(), $author);

        $form->handleRequest($request);

        if ($form->isValid()) {
            // validazione passata, fare qualcosa con l'oggetto $author

            return $this->redirect($this->generateUrl(...));
        }

        return $this->render('author/form.html.twig', array(
            'form' => $form->createView(),
        ));
    }

.. note::

    Questo esempio usa una classe ``AuthorType``, non mostrata qui.

Per maggiori informazioni, vedere il capitolo sui :doc:`form </book/forms>`.

.. index::
   pair: Validazione; Configurazione

.. _book-validation-configuration:

Configurazione
--------------

La validazione in Symfony è abilitata per configurazione predefinita, ma si devono
abilitare esplicitamente le annotazioni, se le si usano per specificare i vincoli:

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        framework:
            validation: { enable_annotations: true }

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <?xml version="1.0" encoding="UTF-8" ?>
        <container xmlns="http://symfony.com/schema/dic/services"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xmlns:framework="http://symfony.com/schema/dic/symfony"
            xsi:schemaLocation="http://symfony.com/schema/dic/services http://symfony.com/schema/dic/services/services-1.0.xsd
                http://symfony.com/schema/dic/symfony http://symfony.com/schema/dic/symfony/symfony-1.0.xsd">

            <framework:config>
                <framework:validation enable-annotations="true" />
            </framework:config>
        </container>

    .. code-block:: php

        // app/config/config.php
        $container->loadFromExtension('framework', array(
            'validation' => array(
                'enable_annotations' => true,
            ),
        ));

.. index::
   single: Validazione; Vincoli

.. _validation-constraints:

Vincoli
-------

Il servizio ``validator`` è progettato per validare oggetti in base a *vincoli* (cioè
regole). Per poter validare un oggetto, basta mappare uno o più vincoli alle
rispettive classi e quindi passarli al servizio ``validator``.

Dietro le quinte, un vincolo è semplicemente un oggetto PHP che esegue un'istruzione
assertiva. Nella vita reale, un vincolo potrebbe essere "la torta non deve essere
bruciata". In Symfony, i vincoli sono simili: sono asserzioni sulla verità di una
condizione. Dato un valore, un vincolo dirà se tale valore sia aderente o meno alle
regole del vincolo.

Vincoli supportati
~~~~~~~~~~~~~~~~~~

Symfony dispone di un gran numero dei vincoli più comunemente necessari:

.. include:: /reference/constraints/map.rst.inc

Si possono anche creare i propri vincoli personalizzati. L'argomento è discusso
nell'articolo ":doc:`/cookbook/validation/custom_constraint`" del ricettario.

.. index::
   single: Validazione; Configurazione dei vincoli

.. _book-validation-constraint-configuration:

Configurazione dei vincoli
~~~~~~~~~~~~~~~~~~~~~~~~~~

Alcuni vincoli, come :doc:`NotBlank </reference/constraints/NotBlank>`, sono
semplici, mentre altri, come :doc:`Choice </reference/constraints/Choice>`, hanno
diverse opzioni di configurazione disponibili. Supponiamo che la classe
``Author`` abbia un'altra proprietà, ``gender``, che possa valere solo
"M", "F" o "altro":

.. configuration-block::

    .. code-block:: php-annotations

        // src/AppBundle/Entity/Author.php

        // ...
        use Symfony\Component\Validator\Constraints as Assert;

        class Author
        {
            /**
             * @Assert\Choice(
             *     choices = { "M", "F", "altro" },
             *     message = "Scegliere un genere valido."
             * )
             */
            public $gender;

            // ...
        }

    .. code-block:: yaml

        # src/AppBundle/Resources/config/validation.yml
        AppBundle\Entity\Author:
            properties:
                gender:
                    - Choice: { choices: [M, F, altro], message: Scegliere un genere valido. }
                # ...

    .. code-block:: xml

        <!-- src/AppBundle/Resources/config/validation.xml -->
        <?xml version="1.0" encoding="UTF-8" ?>
        <constraint-mapping xmlns="http://symfony.com/schema/dic/constraint-mapping"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://symfony.com/schema/dic/constraint-mapping http://symfony.com/schema/dic/constraint-mapping/constraint-mapping-1.0.xsd">

            <class name="AppBundle\Entity\Author">
                <property name="gender">
                    <constraint name="Choice">
                        <option name="choices">
                            <value>M</value>
                            <value>F</value>
                            <value>altro</value>
                        </option>
                        <option name="message">Scegliere un genere valido.</option>
                    </constraint>
                </property>

                <!-- ... -->
            </class>
        </constraint-mapping>

    .. code-block:: php

        // src/AppBundle/Entity/Author.php

        // ...
        use Symfony\Component\Validator\Mapping\ClassMetadata;
        use Symfony\Component\Validator\Constraints as Assert;

        class Author
        {
            public $gender;

            // ...

            public static function loadValidatorMetadata(ClassMetadata $metadata)
            {
                // ...

                $metadata->addPropertyConstraint('gender', new Assert\Choice(array(
                    'choices' => array('M', 'F', 'altro'),
                    'message' => 'Scegliere un genere valido.',
                )));
            }
        }

.. _validation-default-option:

Le opzioni di un vincolo possono sempre essere passate come array. Alcuni vincoli,
tuttavia, consentono anche di passare il valore di una sola opzione, *predefinita*,
al posto dell'array. Nel caso del vincolo ``Choice``, l'opzione ``choices`` può
essere specificata in tal modo.

.. configuration-block::

    .. code-block:: php-annotations

        // src/AppBundle/Entity/Author.php

        // ...
        use Symfony\Component\Validator\Constraints as Assert;

        class Author
        {
            /**
             * @Assert\Choice({"M", "F", "altro"})
             */
            protected $gender;

            // ...
        }

    .. code-block:: yaml

        # src/AppBundle/Resources/config/validation.yml
        AppBundle\Entity\Author:
            properties:
                gender:
                    - Choice: [M, F, altro]
                # ...

    .. code-block:: xml

        <!-- src/AppBundle/Resources/config/validation.xml -->
        <?xml version="1.0" encoding="UTF-8" ?>
        <constraint-mapping xmlns="http://symfony.com/schema/dic/constraint-mapping"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://symfony.com/schema/dic/constraint-mapping http://symfony.com/schema/dic/constraint-mapping/constraint-mapping-1.0.xsd">

            <class name="AppBundle\Entity\Author">
                <property name="gender">
                    <constraint name="Choice">
                        <value>M</value>
                        <value>F</value>
                        <value>altro</value>
                    </constraint>
                </property>

                <!-- ... -->
            </class>
        </constraint-mapping>

    .. code-block:: php

        // src/AppBundle/Entity/Author.php

        // ...
        use Symfony\Component\Validator\Mapping\ClassMetadata;
        use Symfony\Component\Validator\Constraints as Assert;

        class Author
        {
            protected $gender;

            public static function loadValidatorMetadata(ClassMetadata $metadata)
            {
                // ...

                $metadata->addPropertyConstraint(
                    'gender',
                    new Assert\Choice(array('M', 'F', 'altro'))
                );
            }
        }

Questo ha il solo scopo di rendere la configurazione delle opzioni più comuni di un
vincolo più breve e rapida.

Se non si è sicuri di come specificare un'opzione, verificare la documentazione delle API
per il vincolo relativo, oppure andare sul sicuro passando sempre un array di opzioni
(il primo metodo mostrato sopra).

Traduzione dei messaggi dei vincoli
-----------------------------------

Per informazioni sulla traduzione dei messaggi dei vincoli, vedere
:ref:`book-translation-constraint-messages`.

.. index::
   single: Validazione; Obiettivi dei vincoli

.. _validator-constraint-targets:

Obiettivi dei vincoli
---------------------

I vincoli possono essere applicati alle proprietà di una classe (p.e. ``$name``) oppure
a un metodo getter pubblico (p.e. ``getFullName``).  Il primo è il modo più comune e
facile, ma il secondo consente di specificare regole di validazione più complesse.

.. index::
   single: Validazione; Vincoli sulle proprietà

.. _validation-property-target:

Proprietà
~~~~~~~~~

La validazione delle proprietà di una classe è la tecnica di base. Symfony
consente di validare proprietà private, protette o pubbliche. L'elenco seguente
mostra come configurare la proprietà ``$firstName`` di una classe ``Author``, per
avere almeno 3 caratteri.

.. configuration-block::

    .. code-block:: php-annotations

        // AppBundle/Entity/Author.php

        // ...
        use Symfony\Component\Validator\Constraints as Assert;

        class Author
        {
            /**
             * @Assert\NotBlank()
             * @Assert\Length(min=3)
             */
            private $firstName;
        }

    .. code-block:: yaml

        # src/AppBundle/Resources/config/validation.yml
        AppBundle\Entity\Author:
            properties:
                firstName:
                    - NotBlank: ~
                    - Length:
                        min: 3

    .. code-block:: xml

        <!-- src/AppBundle/Resources/config/validation.xml -->
        <?xml version="1.0" encoding="UTF-8" ?>
        <constraint-mapping xmlns="http://symfony.com/schema/dic/constraint-mapping"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://symfony.com/schema/dic/constraint-mapping http://symfony.com/schema/dic/constraint-mapping/constraint-mapping-1.0.xsd">

            <class name="AppBundle\Entity\Author">
                <property name="firstName">
                    <constraint name="NotBlank" />
                    <constraint name="Length">
                        <option name="min">3</option>
                    </constraint>
                </property>
            </class>
        </constraint-mapping>

    .. code-block:: php

        // src/AppBundle/Entity/Author.php

        // ...
        use Symfony\Component\Validator\Mapping\ClassMetadata;
        use Symfony\Component\Validator\Constraints as Assert;

        class Author
        {
            private $firstName;

            public static function loadValidatorMetadata(ClassMetadata $metadata)
            {
                $metadata->addPropertyConstraint('firstName', new Assert\NotBlank());
                $metadata->addPropertyConstraint(
                    'firstName',
                    new Assert\Length(array("min" => 3))
                );
            }
        }

.. index::
   single: Validazione; Vincoli sui getter

Getter
~~~~~~

I vincoli si possono anche applicare ai valori restituiti da un metodo. Symfony
consente di aggiungere un vincolo a qualsiasi metodo il cui nome inizi per
"get" o "is". In questa guida, si fa riferimento a questi due tipi di metodi come
"getter".

Il vantaggio di questa tecnica è che consente di validare gli oggetti
dinamicamente. Per esempio, supponiamo che ci si voglia assicurare che un campo
password non corrisponda al nome dell'utente (per motivi di sicurezza). Lo si può
fare creando un metodo ``isPasswordLegal`` e asserendo che tale metodo debba
restituire ``true``:

.. configuration-block::

    .. code-block:: php-annotations

        // src/AppBundle/Entity/Author.php

        // ...
        use Symfony\Component\Validator\Constraints as Assert;

        class Author
        {
            /**
             * @Assert\True(message = "La password non può essere uguale al nome")
             */
            public function isPasswordLegal()
            {
                // ... restituire true o false
            }
        }

    .. code-block:: yaml

        # src/AppBundle/Resources/config/validation.yml
        AppBundle\Entity\Author:
            getters:
                passwordLegal:
                    - "True": { message: "The password cannot match your first name" }

    .. code-block:: xml

        <!-- src/AppBundle/Resources/config/validation.xml -->
        <?xml version="1.0" encoding="UTF-8" ?>
        <constraint-mapping xmlns="http://symfony.com/schema/dic/constraint-mapping"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://symfony.com/schema/dic/constraint-mapping http://symfony.com/schema/dic/constraint-mapping/constraint-mapping-1.0.xsd">

            <class name="AppBundle\Entity\Author">
                <getter property="passwordLegal">
                    <constraint name="True">
                        <option name="message">La password non può essere uguale al nome</option>
                    </constraint>
                </getter>
            </class>
        </constraint-mapping>

    .. code-block:: php

        // src/AppBundle/Entity/Author.php

        // ...
        use Symfony\Component\Validator\Mapping\ClassMetadata;
        use Symfony\Component\Validator\Constraints as Assert;

        class Author
        {
            public static function loadValidatorMetadata(ClassMetadata $metadata)
            {
                $metadata->addGetterConstraint('passwordLegal', new Assert\True(array(
                    'message' => 'La password non può essere uguale al nome',
                )));
            }
        }

Creare ora il metodo ``isPasswordLegal()`` e includervi la logica necessaria::

    public function isPasswordLegal()
    {
        return $this->firstName !== $this->password;
    }

.. note::

    I lettori più attenti avranno notato che il prefisso del getter
    ("get" o "is") viene omesso nella mappatura. Questo consente di spostare il
    vincolo su una proprietà con lo stesso nome, in un secondo momento (o viceversa),
    senza dover cambiare la logica di validazione.

.. _validation-class-target:

Classi
~~~~~~

Alcuni vincoli si applicano all'intera classe da validare. Per esempio,
il vincolo :doc:`Callback</reference/constraints/Callback>` è un vincolo
generico, che si applica alla classe stessa. Quando tale classe viene validata,
i metodi specifici di questo vincolo vengono semplicemente eseguiti, in modo che
ognuno possa fornire una validazione personalizzata.

.. _book-validation-validation-groups:

Gruppi di validazione
---------------------

Finora, è stato possibile aggiungere vincoli a una classe e chiedere se tale
classe passasse o meno tutti i vincoli definiti. In alcuni casi, tuttavia, occorre
validare un oggetto solo per *alcuni* vincoli della sua classe. Per poterlo fare,
si può organizzare ogni vincolo in uno o più "gruppi di validazione" e quindi
applicare la validazione solo su un gruppo di
vincoli.

Per esempio, si supponga di avere una classe ``User``, usata sia quando un utente
si registra che quando aggiorna successivamente le sue informazioni:

.. configuration-block::

    .. code-block:: php-annotations

        // src/AppBundle/Entity/User.php
        namespace AppBundle\Entity;

        use Symfony\Component\Security\Core\User\UserInterface;
        use Symfony\Component\Validator\Constraints as Assert;

        class User implements UserInterface
        {
            /**
            * @Assert\Email(groups={"registration"})
            */
            private $email;

            /**
            * @Assert\NotBlank(groups={"registration"})
            * @Assert\Length(min=7, groups={"registration"})
            */
            private $password;

            /**
            * @Assert\Length(min=2)
            */
            private $city;
        }

    .. code-block:: yaml

        # src/AppBundle/Resources/config/validation.yml
        AppBundle\Entity\User:
            properties:
                email:
                    - Email: { groups: [registration] }
                password:
                    - NotBlank: { groups: [registration] }
                    - Length: { min: 7, groups: [registration] }
                city:
                    - Length:
                        min: 2

    .. code-block:: xml

        <!-- src/AppBundle/Resources/config/validation.xml -->
        <?xml version="1.0" encoding="UTF-8" ?>
        <constraint-mapping xmlns="http://symfony.com/schema/dic/constraint-mapping"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="
                http://symfony.com/schema/dic/constraint-mapping
                http://symfony.com/schema/dic/constraint-mapping/constraint-mapping-1.0.xsd
            ">

            <class name="AppBundle\Entity\User">
                <property name="email">
                    <constraint name="Email">
                        <option name="groups">
                            <value>registration</value>
                        </option>
                    </constraint>
                </property>

                <property name="password">
                    <constraint name="NotBlank">
                        <option name="groups">
                            <value>registration</value>
                        </option>
                    </constraint>
                    <constraint name="Length">
                        <option name="min">7</option>
                        <option name="groups">
                            <value>registration</value>
                        </option>
                    </constraint>
                </property>

                <property name="city">
                    <constraint name="Length">
                        <option name="min">7</option>
                    </constraint>
                </property>
            </class>
        </constraint-mapping>

    .. code-block:: php

        // src/AppBundle/Entity/User.php
        namespace AppBundle\Entity;

        use Symfony\Component\Validator\Mapping\ClassMetadata;
        use Symfony\Component\Validator\Constraints as Assert;

        class User
        {
            public static function loadValidatorMetadata(ClassMetadata $metadata)
            {
                $metadata->addPropertyConstraint('email', new Assert\Email(array(
                    'groups' => array('registration'),
                )));

                $metadata->addPropertyConstraint('password', new Assert\NotBlank(array(
                    'groups' => array('registration'),
                )));
                $metadata->addPropertyConstraint('password', new Assert\Length(array(
                    'min'    => 7,
                    'groups' => array('registration'),
                )));

                $metadata->addPropertyConstraint('city', new Assert\Length(array(
                    "min" => 3,
                )));
            }
        }

Con questa configurazione, ci sono tre gruppi di validazione:

``Default``
    Contiene i vincoli, nella classe corrente e in tutte le
    classi referenziate, che non appartengono ad altri gruppi.

``User``
    Equivalente a tutti i i vincoli dell'oggetto ``User`` nel gruppo ``Default``.
    È sempre il nome della classe. La differenza tra questo
    e ``Default`` è spiegato più avanti.

``registration``
    Contiene solo i vincoli sui campi ``email`` e ``password``.

I vincoli nel gruppo ``Default`` di una classe sono i vincoli che non hanno gruppi
esplicitamente configurato o che sono configurati su un gruppo uguale al nome della
classe o a ``Default``.

.. caution::

    Se si valida *solo* l'oggetto User, non c'è differenza tra gruppo
    ``Default`` e gruppo ``User``. C'è però differenza se ``User`` ha oggetti inclusi. Per esempio,
    quando ``User`` ha una proprietà ``address`` che contiene un oggetto ``Address``, con
    un vincolo :doc:`/reference/constraints/Valid`, in modo che sia validato
    quando si valida l'oggetto
    ``User``.

    Se si valida ``User`` usando il gruppo ``Default``, i vincoli sulla
    classe ``Address`` nel gruppo ``Default`` *saranno* usati. Se invece si
    valida ``User`` usando il gruppo ``User``, soli i vincoli sulla
    classe ``Address`` con il gruppo ``User``
    saranno validati.

    In altre parole, il gruppo ``Default`` e il gruppo con lo stesso nome della classe (come
    ``User``) sono identici, tranne quando la classe è inclusa in un altro oggetto, che
    è effettivamente l'oggetto validato.

    Se si ha ereditarietà (p.e. ``User extends BaseUser``)e si valida con
    il nome della sottoclasse (p.e. ``User``), tutti i vincoli
    in ``User`` e ``BaseUser`` saranno validati. Se invece si valida usando
    la classe base (p.e. ``BaseUser``), solo i vincoli nel
    gruppo ``BaseUser`` saranno validati.

Per dire al validatore di usare uno specifico gruppo, passare uno o più nomi di
gruppo come secondo parametro del metodo ``validate()``::

    $errors = $validator->validate($author, array('registration'));

Se non si specifica alcun gruppo, saranno applicati tutti i vincoli che appartengono
al gruppo ``Default``.

Ovviamente, di solito si lavorerà con la validazione in modo indiretto, tramite la
libreria dei form. Per informazioni su come usare i gruppi di validazione dentro ai
form, vedere :ref:`book-forms-validation-groups`.

.. index::
   single: Validazione; Validazione dei valori grezzi

.. _book-validation-group-sequence:

Sequenza di gruppi
------------------

A  volte si vogliono validare i gruppi in passi separati. Lo si può fare, usando
``GroupSequence``. In questo caso, un oggetto definisce una sequenza di gruppi
e i gruppi in tale sequenza sono validati in ordine.

Per esempio, si supponga di avere una classe ``User`` e di voler validare che
nome utente e password siano diversi, solo se le altre validazioni passano
(per evitare messaggi di errore multipli).

.. configuration-block::

    .. code-block:: php-annotations

        // src/AppBundle/Entity/User.php
        namespace AppBundle\Entity;

        use Symfony\Component\Security\Core\User\UserInterface;
        use Symfony\Component\Validator\Constraints as Assert;

        /**
         * @Assert\GroupSequence({"User", "Strict"})
         */
        class User implements UserInterface
        {
            /**
            * @Assert\NotBlank
            */
            private $username;

            /**
            * @Assert\NotBlank
            */
            private $password;

            /**
             * @Assert\True(message="La password deve essere diversa dal nome utente", groups={"Strict"})
             */
            public function isPasswordLegal()
            {
                return ($this->username !== $this->password);
            }
        }

    .. code-block:: yaml

        # src/AppBundle/Resources/config/validation.yml
        AppBundle\Entity\User:
            group_sequence:
                - User
                - Strict
            getters:
                passwordLegal:
                    - "True":
                        message: "La password deve essere diversa dal nome utente"
                        groups: [Strict]
            properties:
                username:
                    - NotBlank: ~
                password:
                    - NotBlank: ~

    .. code-block:: xml

        <!-- src/AppBundle/Resources/config/validation.xml -->
        <?xml version="1.0" encoding="UTF-8" ?>
        <constraint-mapping xmlns="http://symfony.com/schema/dic/constraint-mapping"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://symfony.com/schema/dic/constraint-mapping http://symfony.com/schema/dic/constraint-mapping/constraint-mapping-1.0.xsd">

            <class name="AppBundle\Entity\User">
                <property name="username">
                    <constraint name="NotBlank" />
                </property>

                <property name="password">
                    <constraint name="NotBlank" />
                </property>

                <getter property="passwordLegal">
                    <constraint name="True">
                        <option name="message"La password deve essere diversa dal nome utente</option>
                        <option name="groups">
                            <value>Strict</value>
                        </option>
                    </constraint>
                </getter>

                <group-sequence>
                    <value>User</value>
                    <value>Strict</value>
                </group-sequence>
            </class>
        </constraint-mapping>

    .. code-block:: php

        // src/AppBundle/Entity/User.php
        namespace AppBundle\Entity;

        use Symfony\Component\Validator\Mapping\ClassMetadata;
        use Symfony\Component\Validator\Constraints as Assert;

        class User
        {
            public static function loadValidatorMetadata(ClassMetadata $metadata)
            {
                $metadata->addPropertyConstraint('username', new Assert\NotBlank());
                $metadata->addPropertyConstraint('password', new Assert\NotBlank());

                $metadata->addGetterConstraint('passwordLegal', new Assert\True(array(
                    'message' => 'La password deve essere diversa dal nome utente',
                    'groups'  => array('Strict'),
                )));

                $metadata->setGroupSequence(array('User', 'Strict'));
            }
        }

In questo esempio, prima saranno validati i vincoli del gruppo ``User``
(che corrispondono a quelli del gruppo ``Default``). Solo se tutti i vincoli in
tale gruppo sono validi, sarà validato il secondo gruppo, ``Strict``.

.. caution::

    Come già visto nella precedente sezione, il gruppo ``Default`` e
    il gruppo contenente il nome della classe (p.e. ``User``) erano identici.
    Tuttavia, quando si usando le sequenza di gruppo, non lo sono più. Il gruppo
    ``Default`` farà ora riferimento alla sequenza digruppo, al posto di tutti i
    vincoli che non appartengono ad alcun gruppo.

    Questo vuol dire che si deve usare il gruppo ``{NomeClasse}`` (p.e. ``User``)
    quando si specifica una sequenza di gruppo. Quando si usa ``Default``, si avrà
    una ricorsione infinita (poiché il gruppo ``Default`` si riferisce alla sequenza di
    gruppo, che contiene il gruppo ``Default``, che si riferisce alla
    stessa sequenza di gruppo, ecc...).

Fornitori di sequenza di gruppo
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Si immagini un'entità ``User``, che potrebbe essere un utente normale oppure premium. Se
è premium, necessita di alcuni vincoli aggiuntivi
(p.e. dettagli sulla carta di credito). Per determinare in modo dinamico quali gruppi
attivare, si può creare un Group Sequence Provider. Creare prima
l'entità e aggiungere un nuovo gruppo di vincoli, chiamato ``Premium``:

.. configuration-block::

    .. code-block:: php-annotations

        // src/AppBundle/Entity/User.php
        namespace AppBundle\Entity;

        use Symfony\Component\Validator\Constraints as Assert;

        class User
        {
            /**
             * @Assert\NotBlank()
             */
            private $name;

            /**
             * @Assert\CardScheme(
             *     schemes={"VISA"},
             *     groups={"Premium"},
             * )
             */
            private $creditCard;

            // ...
        }

    .. code-block:: yaml

        # src/AppBundle/Resources/config/validation.yml
        AppBundle\Entity\User:
            properties:
                name:
                    - NotBlank: ~
                creditCard:
                    - CardScheme:
                        schemes: [VISA]
                        groups: [Premium]

    .. code-block:: xml

        <!-- src/AppBundle/Resources/config/validation.xml -->
        <?xml version="1.0" encoding="UTF-8" ?>
        <constraint-mapping xmlns="http://symfony.com/schema/dic/constraint-mapping"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://symfony.com/schema/dic/constraint-mapping http://symfony.com/schema/dic/constraint-mapping/constraint-mapping-1.0.xsd">

            <class name="AppBundle\Entity\User">
                <property name="name">
                    <constraint name="NotBlank" />
                </property>

                <property name="creditCard">
                    <constraint name="CardScheme">
                        <option name="schemes">
                            <value>VISA</value>
                        </option>
                        <option name="groups">
                            <value>Premium</value>
                        </option>
                    </constraint>
                </property>

                <!-- ... -->
            </class>
        </constraint-mapping>

    .. code-block:: php

        // src/AppBundle/Entity/User.php
        namespace AppBundle\Entity;

        use Symfony\Component\Validator\Constraints as Assert;
        use Symfony\Component\Validator\Mapping\ClassMetadata;

        class User
        {
            private $name;
            private $creditCard;

            // ...

            public static function loadValidatorMetadata(ClassMetadata $metadata)
            {
                $metadata->addPropertyConstraint('name', new Assert\NotBlank());
                $metadata->addPropertyConstraint('creditCard', new Assert\CardScheme(
                    'schemes' => array('VISA'),
                    'groups'  => array('Premium'),
                ));
            }
        }

Cambiare ora la classe ``User`` per implementare
:class:`Symfony\\Component\\Validator\\GroupSequenceProviderInterface` e
aggiungere
:method:`Symfony\\Component\\Validator\\GroupSequenceProviderInterface::getGroupSequence`,
che deve restituire un array di gruppi da usare::

    // src/AppBundle/Entity/User.php
    namespace AppBundle\Entity;

    // ...
    use Symfony\Component\Validator\GroupSequenceProviderInterface;

    class User implements GroupSequenceProviderInterface
    {
        // ...

        public function getGroupSequence()
        {
            $groups = array('User');

            if ($this->isPremium()) {
                $groups[] = 'Premium';
            }

            return $groups;
        }
    }

Infine, occorre notificare al componente Validator che la classe ``User``
fornisce una sequenza di gruppi da validare:

.. configuration-block::

    .. code-block:: php-annotations

        // src/AppBundle/Entity/User.php
        namespace AppBundle\Entity;

        // ...

        /**
         * @Assert\GroupSequenceProvider
         */
        class User implements GroupSequenceProviderInterface
        {
            // ...
        }

    .. code-block:: yaml

        # src/AppBundle/Resources/config/validation.yml
        AppBundle\Entity\User:
            group_sequence_provider: true

    .. code-block:: xml

        <!-- src/AppBundle/Resources/config/validation.xml -->
        <?xml version="1.0" encoding="UTF-8" ?>
        <constraint-mapping xmlns="http://symfony.com/schema/dic/constraint-mapping"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://symfony.com/schema/dic/constraint-mapping
                http://symfony.com/schema/dic/constraint-mapping/constraint-mapping-1.0.xsd">

            <class name="AppBundle\Entity\User">
                <group-sequence-provider />
                <!-- ... -->
            </class>
        </constraint-mapping>

    .. code-block:: php

        // src/AppBundle/Entity/User.php
        namespace AppBundle\Entity;

        // ...
        use Symfony\Component\Validator\Mapping\ClassMetadata;

        class User implements GroupSequenceProviderInterface
        {
            // ...

            public static function loadValidatorMetadata(ClassMetadata $metadata)
            {
                $metadata->setGroupSequenceProvider(true);
                // ...
            }
        }

.. _book-validation-raw-values:

Validare valori e array
-----------------------

Finora abbiamo visto come si possono validare oggetti interi. Ma a volte si vuole
validare solo un semplice valore, come verificare che una stringa sia un indirizzo
email valido. Lo si può fare molto facilmente. Da dentro a un controllore,
assomiglia a questo::

    // ...
    use Symfony\Component\Validator\Constraints as Assert;

    // ...
    public function addEmailAction($email)
    {
        $emailConstraint = new Assert\Email();
        // tutte le opzioni sui vincoli possono essere impostate in questo modo
        $emailConstraint->message = 'Indirizzo email non valido';

        // usa il validatore per validare il valore
        $errorList = $this->get('validator')->validateValue(
            $email,
            $emailConstraint
        );

        if (0 === count($errorList)) {
            // è un indirizzo email valido, fare qualcosa
        } else {
            // *non* è un indirizzo email valido
            $errorMessage = $errorList[0]->getMessage();

            // ... fare qualcosa con l'errore
        }

        // ...
    }

Richiamando ``validateValue`` sul validatore, si può passare un valore grezzo e
l'oggetto vincolo su cui si vuole validare tale valore. Una lista completa di vincoli
disponibili, così come i nomi completi delle classi per ciascun vincolo, è
disponibile nella sezione
:doc:`riferimento sui vincoli </reference/constraints>`.

Il metodo ``validateValue`` restituisce un oggetto :class:`Symfony\\Component\\Validator\\ConstraintViolationList`,
che si comporta come un array di errori. Ciascun errore della lista è un oggetto
:class:`Symfony\\Component\\Validator\\ConstraintViolation`, che contiene
il messaggio di errore nel suo metodo ``getMessage``.

Considerazioni finali
---------------------

``validator`` di Symfony è uno strumento potente, che può essere sfruttato per
garantire che i dati di qualsiasi oggetto siano validi. La potenza dietro alla
validazione risiede nei "vincoli", che sono regole da applicare alle proprietà o
ai metodi getter del proprio oggetto. Sebbene la maggior parte delle volte si userà il
framework della validazione indirettamente, usando i form, si ricordi che può essere
usato ovunque, per validare qualsiasi oggetto.

Imparare di più con le ricette
------------------------------

* :doc:`/cookbook/validation/custom_constraint`

.. _Validator: https://github.com/symfony/Validator
.. _specifiche di validazione JSR303 Bean: http://jcp.org/en/jsr/detail?id=303
