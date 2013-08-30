.. index::
    single: Options Resolver
    single: Componenti; OptionsResolver

Il componente OptionsResolver
=============================

    Il Componente OptionsResolver aiuta a configurare gli oggetti con un array
    di opzioni. Esso supporta valori predefiniti, opzioni con vincoli e opzioni pigre.

Installazione
-------------

E' possibile installare i componente in due modi differenti:

* utilizzare il repository ufficiale Git (https://github.com/symfony/OptionsResolver
* :doc:`installare il componente via Composer </components/using_components>` (``symfony/options-resolver`` su `Packagist`_)

Utilizzo
--------

Si immagini di avere una classe ``Person`` che ha 2 opzioni: ``firstName`` e
``secondName``. Queste opzioni stanno per essere gestite dal Componente 
OptionsResolver.

Innanzitutto, creare la classe ``Person``::

    class Person
    {
        protected $options;

        public function __construct(array $options = array())
        {
        }
    }

Si potrebbe impostare il valore di ``$options`` direttamente nella proprietà. Invece,
utilizzare la classe :class:`Symfony\\Component\\OptionsResolver\\OptionsResolver`
e lasciare che essa risolva le opzioni tramite la chiamata al metodo
:method:`Symfony\\Component\\OptionsResolver\\OptionsResolver::resolve`.
I vantaggi di operare in questo modo saranno più ovvi andando avanti::

    use Symfony\Component\OptionsResolver\OptionsResolver;

    // ...
    public function __construct(array $options = array())
    {
        $resolver = new OptionsResolver();

        $this->options = $resolver->resolve($options);
    }

La proprietà ``$options`` è un'istanza di
:class:`Symfony\\Component\\OptionsResolver\\Options`, la quale implementa
:phpclass:`ArrayAccess`, :phpclass:`Iterator` e :phpclass:`Countable`. Questo
significa che è possibile gestirla come un normale array::

    // ...
    public function getFirstName()
    {
        return $this->options['firstName'];
    }

    public function getFullName()
    {
        $name = $this->options['firstName'];

        if (isset($this->options['lastName'])) {
            $name .= ' '.$this->options['lastName'];
        }

        return $name;
    }

Adesso, si provi a utilizzare effettivamente la classe::

    $person = new Person(array(
        'firstName' => 'Wouter',
        'lastName'  => 'de Jong',
    ));

    echo $person->getFirstName();

In questo momento, si riceverà una 
:class:`Symfony\\Component\\OptionsResolver\\Exception\\InvalidOptionsException`,
la quale informa che le opzioni ``firstName`` e ``lastName`` non esistono.
Questo perché è necessario configurare prima l'``OptionsResolver``, in modo che
sappia quali opzioni devono essere risolte.

.. tip::

    Per controllare se un'opzione esiste, si può utilizzare la
    funzione
    :method:`Symfony\\Component\\OptionsResolver\\OptionsResolver::isKnown`
    .

Una buona pratica è porre la configurazione in un metodo (per esempio
``setDefaultOptions``). Il metodo viene invocato nel costruttore per configurare
la classe ``OptionsResolver``::

    use Symfony\Component\OptionsResolver\OptionsResolver;
    use Symfony\Component\OptionsResolver\OptionsResolverInterface;

    class Person
    {
        protected $options;

        public function __construct(array $options = array())
        {
            $resolver = new OptionsResolver();
            $this->setDefaultOptions($resolver);

            $this->options = $resolver->resolve($options);
        }

        protected function setDefaultOptions(OptionsResolverInterface $resolver)
        {
            // ... configura il resolver, come si apprendererà nelle sezioni successive
        }
    }

Opzioni Obbligatorie
--------------------

Supponiamo che l'opzione ``firstName`` sia obbligatoria: la classe non può funzionare senza
di essa. Si possono settare le opzioni obbligatorie invocando
:method:`Symfony\\Component\\OptionsResolver\\OptionsResolver::setRequired`::

    // ...
    protected function setDefaultOptions(OptionsResolverInterface $resolver)
    {
        $resolver->setRequired(array('firstName'));
    }

A questo punto è possible usare la classe senza errori::

    $person = new Person(array(
        'firstName' => 'Wouter',
    ));

    echo $person->getFirstName(); // 'Wouter'

Se un'opzione obbligatoria non viene passata, una
:class:`Symfony\\Component\\OptionsResolver\\Exception\\MissingOptionsException`
sarà lanciata.

Per determinare se un'opzione è obbligatoria, si può usare il
metodo
:method:`Symfony\\Component\\OptionsResolver\\OptionsResolver::isRequired`.

Opzioni Facoltative
-------------------

Qualche volta, un'opzione può essere facoltativa (per esempio l'opzione ``lastName`` nella classe
``Person``). E' possibile configurare queste opzioni invocando
:method:`Symfony\\Component\\OptionsResolver\\OptionsResolver::setOptional`::

    // ...
    protected function setDefaultOptions(OptionsResolverInterface $resolver)
    {
        // ...

        $resolver->setOptional(array('lastName'));
    }

Settare Valori Predefiniti
--------------------------

La maggior parte delle opzioni facoltative hanno un valore predefinito. E' possibile configurare queste
opzioni invocando
:method:`Symfony\\Component\\OptionsResolver\\OptionsResolver::setDefaults`::

    // ...
    protected function setDefaultOptions(OptionsResolverInterface $resolver)
    {
        // ...

        $resolver->setDefaults(array(
            'age' => 0,
        ));
    }

L'età predefinita adesso sarà ``0``. Quando l'utente specifica una data, questa viene
rimpiazzata. Non è necessario configurare ``age`` come una opzione facoltativa.
``OptionsResolver`` sa già che le opzioni con un valore predefinito sono
facoltative.

Il componente ``OptionsResolver`` ha anche un
metodo :method:`Symfony\\Component\\OptionsResolver\\OptionsResolver::replaceDefaults`. 
Questo può essere usato per sovrascrivere il valore precedente. La closure
che è passata ha 2 parametri:

* ``$options`` (un'istanza di :class:`Symfony\\Component\\OptionsResolver\\Options), 
con tutti i valori predefiniti
* ``$value``, il set precedente di valori predefiniti

Valori Predefiniti che dipendono da altre Opzioni
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Supponiamo di aggiungere un'opzione ``gender`` alla classe ``Person``, il cui valore predefinito
è indovinato sulla base del nome. E' possibile fare questo facilmente usando una
Closure come valore predefinito::

    use Symfony\Component\OptionsResolver\Options;

    // ...
    protected function setDefaultOptions(OptionsResolverInterface $resolver)
    {
        // ...

        $resolver->setDefaults(array(
            'gender' => function (Options $options) {
                if (GenderGuesser::isMale($options['firstName'])) {
                    return 'male';
                }
                
                return 'female';
            },
        ));
    }

.. caution::

    Il primo argomento della Closure deve essere di tipo ``Options``,
    altrimenti sarà considerata come il valore.

Configurare i Valori consentiti
-------------------------------

Non tutti i valori sono validi per le opzioni. Per esempio, l'opzione ``gender``
può essere solo ``female`` o ``male``. E' possibile configurare questi valori consentiti
invocando
:method:`Symfony\\Component\\OptionsResolver\\OptionsResolver::setAllowedValues`::

    // ...
    protected function setDefaultOptions(OptionsResolverInterface $resolver)
    {
        // ...

        $resolver->setAllowedValues(array(
            'gender' => array('male', 'female'),
        ));
    }

Esiste anche un metodo
:method:`Symfony\\Component\\OptionsResolver\\OptionsResolver::addAllowedValues`, 
che è possibile utilizzare se si vuole aggiungere un valore consentito al precedente
set di valori consentiti.

Configurare i Tipi consentiti
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

E' possibile anche specificare i valori consentiti. Per esempio, l'opzione ``firstName`` può
essere qualsiasi cosa, ma deve essere una stringa. E' possibile configurare questi tipi invocando
:method:`Symfony\\Component\\OptionsResolver\\OptionsResolver::setAllowedTypes`::

    // ...
    protected function setDefaultOptions(OptionsResolverInterface $resolver)
    {
        // ...

        $resolver->setAllowedTypes(array(
            'firstName' => 'string',
        ));
    }

I possibili tipi sono quelli associati alle funzioni php ``is_*`` o al nome
della classe. E' possibile passare anche un array di tipi come valore. Per esempio,
``array('null', 'string')`` consente a ``firstName`` di essere ``null`` o una
``string``.

Esiste anche un metodo
:method:`Symfony\\Component\\OptionsResolver\\OptionsResolver::addAllowedTypes`, 
che può essere utilizzato per aggiungere un tipo consentito a quelli precedentemente indicati.

Normalizzare le Opzioni
-----------------------

Alcuni valori devono essere normalizzati prima che possano essere usati. Per esempio,
``firstName`` dovrebbe sempre iniziare con una lettera maiuscola. Per fare ciò, si posso 
scrivere dei normalizzatori. Queste Closure saranno eseguite dopo che tutte le opzioni sono state
passate e ritornano il valore normalizzato. I normalizzatori possono essere configurati
invocando
:method:`Symfony\\Components\\OptionsResolver\\OptionsResolver::setNormalizers`::

    // ...
    protected function setDefaultOptions(OptionsResolverInterface $resolver)
    {
        // ...

        $resolver->setNormalizers(array(
            'firstName' => function (Options $options, $value) {
                return ucfirst($value);
            },
        ));
    }

E' possibile notare che la closure riceve un parametetro ``$options``. Qualche volta, è
necessario utilizzare altre opzioni per normalizzare.

.. _Packagist: https://packagist.org/packages/symfony/options-resolver
