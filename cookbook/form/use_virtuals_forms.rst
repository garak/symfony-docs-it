.. index::
   single: Form; Usare form virtuali

Come usare l'opzione del campo virtual dei form
===============================================

L'opzione del campo dei form ``virtual`` può essere molto utile quando si hanno alcuni
campi duplicati in diverse entità.

Per esempio, si immagini di avere due entità, ``Company`` e ``Customer``::

    // src/Acme/HelloBundle/Entity/Company.php
    namespace Acme\HelloBundle\Entity;

    class Company
    {
        private $name;
        private $website;

        private $address;
        private $zipcode;
        private $city;
        private $country;
    }

.. code-block:: php

    // src/Acme/HelloBundle/Entity/Customer.php
    namespace Acme\HelloBundle\Entity;

    class Customer
    {
        private $firstName;
        private $lastName;

        private $address;
        private $zipcode;
        private $city;
        private $country;
    }

Come si può vedere, queste entità condividono alcuni campi uguali: ``address``,
``zipcode``, ``city``, ``country``.

Ora si vogliono costruire due form: uno per ``Company`` e l'altro per
``Customer``.

Iniziamo creando ``CompanyType`` e ``CustomerType``::

    // src/Acme/HelloBundle/Form/Type/CompanyType.php
    namespace Acme\HelloBundle\Form\Type;

    use Symfony\Component\Form\FormBuilderInterface;

    class CompanyType extends AbstractType
    {
        public function buildForm(FormBuilderInterface $builder, array $options)
        {
            $builder
                ->add('name', 'text')
                ->add('website', 'text');
        }
    }

.. code-block:: php

    // src/Acme/HelloBundle/Form/Type/CustomerType.php
    namespace Acme\HelloBundle\Form\Type;

    use Symfony\Component\Form\FormBuilderInterface;

    class CustomerType extends AbstractType
    {
        public function buildForm(FormBuilderInterface $builder, array $options)
        {
            $builder
                ->add('firstName', 'text')
                ->add('lastName', 'text');
        }
    }

Ora dobbiamo gestire i campi duplicati. Ecco un semplice form per la
località::

    // src/Acme/HelloBundle/Form/Type/LocationType.php
    namespace Acme\HelloBundle\Form\Type;

    use Symfony\Component\Form\FormBuilderInterface;
    use Symfony\Component\OptionsResolver\OptionsResolverInterface;

    class LocationType extends AbstractType
    {
        public function buildForm(FormBuilderInterface $builder, array $options)
        {
            $builder
                ->add('address', 'textarea')
                ->add('zipcode', 'text')
                ->add('city', 'text')
                ->add('country', 'text');
        }

        public function setDefaultOptions(OptionsResolverInterface $resolver)
        {
            $resolver->setDefaults(array(
                'virtual' => true
            ));
        }

        public function getName()
        {
            return 'location';
        }
    }

Non abbiamo *effettuvamente* un campo per la località nelle nostre entità, quindi non
possiamo collegare direttamente ``LocationType`` a ``CompanyType`` o ``CustomerType``.
Ma vogliamo decisamente avere un form dedicato, che si occupi della località (ricordate, DRY!).

L'opzione del campo ``virtual`` è la soluzione.

Si può impostare l'opzione ``'virtual' => true`` nel metodo ``getDefaultOptions`` di
``LocationType`` e iniziare a usarlo direttamente nei due form originali.

Vediamo il risultato::

    // CompanyType
    public function buildForm(FormBuilderInterface $builder, array $options)
    {
        $builder->add('foo', new LocationType(), array(
            'data_class' => 'Acme\HelloBundle\Entity\Company'
        ));
    }

.. code-block:: php

    // CustomerType
    public function buildForm(FormBuilderInterface $builder, array $options)
    {
        $builder->add('bar', new LocationType(), array(
            'data_class' => 'Acme\HelloBundle\Entity\Customer'
        ));
    }

Con l'opzione ``virtual`` impostata a ``false`` (predefinito), il componente Form
si aspetta che ogni oggetto sottostante abbia una proprietà ``foo`` (o ``bar``), che sia
o un oggetto o un array contenente i quattro campi per la località.
Ovviamente, non abbiamo tale oggetto/array nelle nostre entità e non vogliamo averlo!

Con l'opzione ``virtual`` impostata a ``true``, il componente Form salta la proprietà
``foo`` (o ``bar``) e invece usa "get" e "set" sui quattro campi della località direttamente
sull'oggetto sottostante!

.. note::

    Invece di impostare l'opzione ``virtual`` in ``LocationType``, si può
    (come ogni altra opzione) passarla in un array di opzioni, come terzo parametro
    di ``$builder->add()``.
