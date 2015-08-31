.. index::
   single: Form; L'opzione "inherit_data"

Ridurre la duplicazione di codice con "inherit_data"
====================================================

.. versionadded:: 2.3
    L'opzione ``inherit_data`` è stata introdotta in Symfony 2.3. In precedenza,
    era nota come ``virtual``.

L'opzione ``inherit_data`` di un campo di form può essere molto utile quando si hanno
campi duplicati su varie entità. Per esempi, si immagini di avere due
entità, ``Company`` e ``Customer``::

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

Come si può vedere, le entità hanno in comune alcuni campi: ``address``,
``zipcode``, ``city``, ``country``.

Costruiamo ora i form per queste entità, ``CompanyType`` e ``CustomerType``::

    // src/Acme/HelloBundle/Form/Type/CompanyType.php
    namespace Acme\HelloBundle\Form\Type;

    use Symfony\Component\Form\AbstractType;
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
    use Symfony\Component\Form\AbstractType;

    class CustomerType extends AbstractType
    {
        public function buildForm(FormBuilderInterface $builder, array $options)
        {
            $builder
                ->add('firstName', 'text')
                ->add('lastName', 'text');
        }
    }

Invece di includere i campi duplicati ``address``, ``zipcode``, ``city``
e ``country`` in entrambi i form, creeremo un terzo form apposta.
Chiameremo questo form ``LocationType``::

    // src/Acme/HelloBundle/Form/Type/LocationType.php
    namespace Acme\HelloBundle\Form\Type;

    use Symfony\Component\Form\AbstractType;
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
                'inherit_data' => true
            ));
        }

        public function getName()
        {
            return 'location';
        }
    }

Questo form ha un'opzione interessante, chiamata ``inherit_data``. Tale
opzione fa sì che il form erediti i suoi dati dal form genitore. Se incluso
nel form company, i campi del form location potranno accedere alle proprietà
dell'istanza  ``Company``. Se incluso nel form customer, i campi invece potranno
accedere alle proprietà dell'istanza ``Customer``.

.. note::

    Invece di impostare l'opzione ``inherit_data`` in ``LocationType``, si può
    anche (come per le altre opzioni) passarla come terzo parametro di
    ``$builder->add()``.

Ora aggiungiamo il form location ai due form originari::

    // src/Acme/HelloBundle/Form/Type/CompanyType.php
    public function buildForm(FormBuilderInterface $builder, array $options)
    {
        // ...

        $builder->add('foo', new LocationType(), array(
            'data_class' => 'Acme\HelloBundle\Entity\Company'
        ));
    }

.. code-block:: php

    // src/Acme/HelloBundle/Form/Type/CustomerType.php
    public function buildForm(FormBuilderInterface $builder, array $options)
    {
        // ...

        $builder->add('bar', new LocationType(), array(
            'data_class' => 'Acme\HelloBundle\Entity\Customer'
        ));
    }

Ecco fatto! La duplicazione delle definizioni dei campi è stata estratta in un form
a parte, riutilizzabili ovunque sia necessario.

.. caution::

    I form con l'opzione ``inherit_data`` impostata non possono avere ascoltatori di eventi ``*_SET_DATA``.
