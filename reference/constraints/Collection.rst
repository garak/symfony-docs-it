Collection
==========

Questo vincolo si usa quando i dati sottostanti sono un insieme (cioè un
array o un oggetto che implementi ``Traversable`` e ``ArrayAccess``),
ma si preferisce validare diverse chiavi di tale insieme in modi
diversi. Per esempio, si potrebbe voler validare la chiave ``email`` con il
vincolo ``Email`` e la chiave ``inventory`` con il vincolo ``Min``.

Questo vincolo può anche assicurare che alcune chiavi dell'insieme siano presenti e
e che chiavi extra non siano presenti.

+----------------+--------------------------------------------------------------------------+
| Si applica a   | :ref:`proprietà o metodo<validation-property-target>`                    |
+----------------+--------------------------------------------------------------------------+
| Opzioni        | - `fields`_                                                              |
|                | - `allowExtraFields`_                                                    |
|                | - `extraFieldsMessage`_                                                  |
|                | - `allowMissingFields`_                                                  |
|                | - `missingFieldsMessage`_                                                |
+----------------+--------------------------------------------------------------------------+
| Classe         | :class:`Symfony\\Component\\Validator\\Constraints\\Collection`          |
+----------------+--------------------------------------------------------------------------+
| Validatore     | :class:`Symfony\\Component\\Validator\\Constraints\\CollectionValidator` |
+----------------+--------------------------------------------------------------------------+

Uso di base
-----------

Il vincolo ``Collection`` consente di validare le diverse chiavi di un insieme in modo
individuale. Si consideri il seguente esempio::

    namespace Acme\BlogBundle\Entity;
    
    class Author
    {
        protected $profileData = array(
            'personal_email',
            'short_bio',
        );

        public function setProfileData($key, $value)
        {
            $this->profileData[$key] = $value;
        }
    }

Per validare che l'elemento ``personal_email`` della proprietà ``profileData`` dell'array
sia un indirizzo email valido e che l'elemento ``short_bio`` non sia vuoto e non più
lungo di 100 caratteri, si potrebbe fare nel seguente modo:

.. configuration-block::

    .. code-block:: yaml

        properties:
            profileData:
                - Collection:
                    fields:
                        personal_email: Email
                        short_bio:
                            - NotBlank
                            - MaxLength:
                                limit:   100
                                message: Your short bio is too long!
                    allowMissingfields: true

    .. code-block:: php-annotations

        // src/Acme/BlogBundle/Entity/Author.php
        use Symfony\Component\Validator\Constraints as Assert;

        class Author
        {
            /**
             * @Assert\Collection(
             *     fields = {
             *         "personal_email" = @Assert\Email,
             *         "short_bio" = {
             *             @Assert\NotBlank(),
             *             @Assert\MaxLength(
             *                 limit = 100,
             *                 message = "Your bio is too long!"
             *             )
             *         }
             *     },
             *     allowMissingfields = true
             * )
             */
             protected $profileData = array(
                 'personal_email',
                 'short_bio',
             );
        }

    .. code-block:: xml

        <!-- src/Acme/BlogBundle/Resources/config/validation.xml -->
        <class name="Acme\BlogBundle\Entity\Author">
            <property name="profileData">
                <constraint name="Collection">
                    <option name="fields">
                        <value key="personal_email">
                            <constraint name="Email" />
                        </value>
                        <value key="short_bio">
                            <constraint name="NotBlank" />
                            <constraint name="MaxLength">
                                <option name="limit">100</option>
                                <option name="message">Your bio is too long!</option>
                            </constraint>
                        </value>
                    </option>
                    <option name="allowMissingFields">true</option>
                </constraint>
            </property>
        </class>

    .. code-block:: php

        // src/Acme/BlogBundle/Entity/Author.php
        use Symfony\Component\Validator\Mapping\ClassMetadata;
        use Symfony\Component\Validator\Constraints\Collection;
        use Symfony\Component\Validator\Constraints\Email;
        use Symfony\Component\Validator\Constraints\MaxLength;

        class Author
        {
            private $options = array();

            public static function loadValidatorMetadata(ClassMetadata $metadata)
            {
                $metadata->addPropertyConstraint('profileData', new Collection(array(
                    'fields' => array(
                        'personal_email' => new Email(),
                        'lastName' => array(new NotBlank(), new MaxLength(100)),
                    ),
                    'allowMissingFields' => true,
                )));
            }
        }

Presenza e assenza di campi
~~~~~~~~~~~~~~~~~~~~~~~~~~~

Per impostazione predefinita, questo vincolo valida più del semplice fatto che i
singoli campi dell'insieme passino o meno i loro rispettivi vincoli. Infatti, se una
chiave dell'insieme manca o se ci sono chiavi non riconosciute nell'insieme, saranno
lanciati degli errori di validazione.

Se si vogliono consentire chiavi assenti dall'insieme o se si vuole che chiavi "extra"
siano consentite nell'insieme, si possono modificare rispettivamente le opzioni
`allowMissingFields`_ e `allowExtraFields`_. Nell'esempio precedente, l'opzione
``allowMissingFields`` era impostata a ``true``, quindi, se gli elementi
``personal_email`` o ``short_bio`` fossero stati mancanti dalla proprietà
``$personalData``, non sarebbe occorso alcun errore di validazione.

Opzioni
-------

fields
~~~~~~

**tipo**: ``array`` [:ref:`opzione predefinita<validation-default-option>`]

Questa opzione, obbligatorio, è un array associativo, che definisce tutte le
chiavi nell'insieme e, per ogni chiave, esattamente quale validatore (o quali validatori)
vanno eseguiti su quell'elemento dell'insieme.

allowExtraFields
~~~~~~~~~~~~~~~~

**tipo**: ``booleano`` **predefinito**: false

Se questa opzione è ``false`` e l'insieme sottostante contiene uno o più elementi
non inclusi nell'opzione `fields`_, sarà restituto un errore di
validazione. Se ``true``, i campi extra sono consentiti.

extraFieldsMessage
~~~~~~~~~~~~~~~~~~

**tipo**: ``booleano`` **predefinito**: ``The fields {{ fields }} were not expected``

Messaggio mostrato se `allowExtraFields`_ è ``false`` e viene trovato un campo extra.

allowMissingFields
~~~~~~~~~~~~~~~~~~

**tipo**: ``booleano`` **predefinito**: false

Se questa opzione è ``false`` e uno o più campi dell'opzione `fields`_
mancano nell'insieme sottostante, sarà restituito un errore di
validazione. Se ``true``, alcuni campi dell'opzione `fields_` possono
mancare nell'insieme sottostante.

missingFieldsMessage
~~~~~~~~~~~~~~~~~~~~

**tipo**: ``booleano`` **predefinito**: ``The fields {{ fields }} are missing``

Messaggio mostrato se `allowMissingFields`_ è ``false`` e uno o più campo mancano
dall'insieme sottostante.
