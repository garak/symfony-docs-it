.. index::
   single: Validazione; Vincoli personalizzati

Come creare vincoli di validazione personalizzati
-------------------------------------------------

È possibile creare vincoli personalizzati estendendo la classe base
:class:`Symfony\\Component\\Validator\\Constraint`. 
Come esempio, creeremo un semplice validatore, che verifica se una stringa
contenga solo caratteri alfanumerici.

Creare la classe Constraint
---------------------------

Innanzitutto, occorre creare una classe Constraint, che estenda :class:`Symfony\\Component\\Validator\\Constraint`::

    // src/Acme/DemoBundle/Validator/Constraints/ContainsAlphanumeric.php
    namespace Acme\DemoBundle\Validator\Constraints;
    
    use Symfony\Component\Validator\Constraint;

    /**
     * @Annotation
     */
    class ContainsAlphanumeric extends Constraint
    {
        public $message = 'The string "%string%" contains an illegal character: it can only contain letters or numbers.';
    }

.. note::

    In questo vincolo, l'annotazione ``@Annotation`` è necessaria per
    poterne rendere disponibile l'uso nelle altre classi, tramite annotazioni.
    Le opzioni del nuovo vincolo sono rappresentate da proprietà pubbliche della
    classe vincolo.

Creare il validatore
--------------------

Come si può vedere, un vincolo è estremamente minimalistico. La validazione
vera e propria è effettuata da un'altra classe di "validazione dei vincoli". La
classe per la validazione dei vincoli è definita dal metodo del vincolo ``validatedBy()``,
che usa una semplice logica predefinita::

    // nella classe base Symfony\Component\Validator\Constraint
    public function validatedBy()
    {
        return get_class($this).'Validator';
    }

In altre parole, se si crea un ``Constraint``, ovvero un vincolo, personalizzato (come ``MioVincolo``),
Symfony2, automaticamente, cercherà anche un'altra la classe, ``MioVincoloValidator``
per effettuare la validazione vera e propria.

Anche la classe validatrice è semplice e richiede solo un metodo obbligatorio: ``validate``::

    // src/Acme/DemoBundle/Validator/Constraints/ContainsAlphanumericValidator.php
    namespace Acme\DemoBundle\Validator\Constraints;
    
    use Symfony\Component\Validator\Constraint;
    use Symfony\Component\Validator\ConstraintValidator;

    class ContainsAlphanumericValidator extends ConstraintValidator
    {
        public function validate($value, Constraint $constraint)
        {
            if (!preg_match('/^[a-zA-Za0-9]+$/', $value, $matches)) {
                $this->context->addViolation($constraint->message, array('%string%' => $value));
            }
        }
    }

.. note::

    Il metodo ``validate`` non restituisce valori, ma aggiunge violazioni alla
    proprietà ``context`` del validatore, tramite una chiamata al metodo ``addViolation``,
    se una validazione fallisce. Quindi, un valore può essere considerato valido se non
    provoca aggiunte di violazioni al contesto.
    Il primo parametro della chiamata ad ``addViolation`` è il messaggio di errore da
    usare per tale violazione.

.. versionadded:: 2.1
    Il metodo ``isValid`` è stato rinominato ``validate`` in Symfony 2.1. Il metodo
    ``setMessage`` è stato deprecato, a favore della chiamata ad ``addViolation``
    sul contesto.

Usare il nuovo validatore
-------------------------

Usare il nuovo validatore è molto facile, come quelli forniti da Symfony2:

.. configuration-block::

    .. code-block:: yaml
        
        # src/Acme/BlogBundle/Resources/config/validation.yml
        Acme\DemoBundle\Entity\AcmeEntity:
            properties:
                name:
                    - NotBlank: ~
                    - Acme\DemoBundle\Validator\Constraints\ContainsAlphanumeric: ~

    .. code-block:: php-annotations

        // src/Acme/DemoBundle/Entity/AcmeEntity.php 
        use Symfony\Component\Validator\Constraints as Assert;
        use Acme\DemoBundle\Validator\Constraints as AcmeAssert;
            
        class AcmeEntity
        {
            // ...
            
            /**
             * @Assert\NotBlank
             * @AcmeAssert\ContainsAlphanumeric
             */
            protected $name;
            
            // ...
        }

    .. code-block:: xml
        
        <!-- src/Acme/DemoBundle/Resources/config/validation.xml -->
        <?xml version="1.0" encoding="UTF-8" ?>
        <constraint-mapping xmlns="http://symfony.com/schema/dic/constraint-mapping"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://symfony.com/schema/dic/constraint-mapping http://symfony.com/schema/dic/constraint-mapping/constraint-mapping-1.0.xsd">

            <class name="Acme\DemoBundle\Entity\AcmeEntity">
                <property name="name">
                    <constraint name="NotBlank" />
                    <constraint name="Acme\DemoBundle\Validator\Constraints\ContainsAlphanumeric" />
                </property>
            </class>
        </constraint-mapping>

    .. code-block:: php
        
        // src/Acme/DemoBundle/Entity/AcmeEntity.php
        use Symfony\Component\Validator\Mapping\ClassMetadata;
        use Symfony\Component\Validator\Constraints\NotBlank;
        use Acme\DemoBundle\Validator\Constraints\ContainsAlphanumeric;

        class AcmeEntity
        {
            public $name;

            public static function loadValidatorMetadata(ClassMetadata $metadata)
            {
                $metadata->addPropertyConstraint('name', new NotBlank());
                $metadata->addPropertyConstraint('name', new ContainsAlphanumeric());
            }
        }

Se il proprio vincolo contiene opzioni, dovrebbero essere proprietà pubbliche
nella classe Constraint creata in precedenza. Tali opzioni possono essere configurate,
come le opzioni dei vincoli del nucleo di Symfony.

Validatori di vincoli con dipendenze
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Se il proprio vincolo ha delle dipendenze, come una connessione alla base dati,
sarà necessario configurarlo come servizio nel contenitore delle dipendenze.
Questo servizio dovrà includere il tag ``validator.constraint_validator`` e
l'attributo ``alias``:

.. configuration-block::

    .. code-block:: yaml

        services:
            validator.unique.nome_proprio_validatore:
                class: Nome\Pienamente\Qualificato\Della\Classe\Validatore
                tags:
                    - { name: validator.constraint_validator, alias: nome_alias }

    .. code-block:: xml

        <service id="validator.unique.nome_proprio_validatore" class="Nome\Pienamente\Qualificato\Della\Classe\Validatore">
            <argument type="service" id="doctrine.orm.default_entity_manager" />
            <tag name="validator.constraint_validator" alias="nome_alias" />
        </service>

    .. code-block:: php

        $container
            ->register('validator.unique.nome_proprio_validatore', 'Nome\Pienamente\Qualificato\Della\Classe\Validatore')
            ->addTag('validator.constraint_validator', array('alias' => 'nome_alias'));

La classe del vincolo dovrà utilizzare l'alias appena definito per riferirsi al
validatore corretto::

    public function validatedBy()
    {
        return 'nome_alias';
    }

Come già detto, Symfony2 cercherà automaticamente una classe il cui nome
sia uguale a quello del vincolo ma con il suffisso ``Validator``. Se il proprio
validatore di vincoli è definito come servizio, è importante che si sovrascriva
il metodo ``validatedBy()``, in modo tale che restituisca l'alias utilizzato
nella definizione del servizio, altrimenti Symfony2 non utilizzerà il servizio di validazione
dei vincoli e istanzierà la classe senza che le dipendenze vengano iniettate.

Validatore con vincolo di classe
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Oltre a validare la proprietà di una classe, un vincolo può avere visibilità su una classe,
fornendo un bersaglio::

    public function getTargets()
    {
        return self::CLASS_CONSTRAINT;
    }

In questo modo, il metodo ``validate()`` del validatore accetta un oggetto come primo parametro::

    class ProtocolClassValidator extends ConstraintValidator
    {
        public function validate($protocol, Constraint $constraint)
        {
            if ($protocol->getFoo() != $protocol->getBar()) {
                $this->context->addViolationAtSubPath('foo', $constraint->message, array(), null);
            }
        }
    }

Si noti che un validatore con vincolo di classe si applica alla classe stessa e non
alla proprietà:

.. configuration-block::

    .. code-block:: yaml

        # src/Acme/BlogBundle/Resources/config/validation.yml
        Acme\DemoBundle\Entity\AcmeEntity:
            constraints:
                - ContainsAlphanumeric

    .. code-block:: php-annotations

        /**
         * @AcmeAssert\ContainsAlphanumeric
         */
        class AcmeEntity
        {
            // ...
        }

    .. code-block:: xml

        <!-- src/Acme/BlogBundle/Resources/config/validation.xml -->
        <class name="Acme\DemoBundle\Entity\AcmeEntity">
            <constraint name="ContainsAlphanumeric" />
        </class>
