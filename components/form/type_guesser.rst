.. index::
    single: Form; Indovino di tipi personalizzato

Creare un indovino di tipi
==========================

Il componente Form può indovinare il tipo e alcune opzioni di un campo, usando
gli indovini di tipo. Il componente include già un indovino di tipi, che usa
le asserzioni del componente Validation, ma si possono anche aggiungere
degli indovini personalizzati.

.. sidebar:: Indovini di tipi di form nei bridge

    Symfony fornisce anche alcuni indovini di tipi di form nei bridge:

    * :class:`Symfony\\Bridge\\Propel1\\Form\\PropelTypeGuesser` fornito dal
      Propel1 bridge;
    * :class:`Symfony\\Bridge\\Doctrine\\Form\\DoctrineOrmTypeGuesser`
      fornito dal Doctrine bridge.

Creare un indovino di tipi PHPDoc
---------------------------------

In questa sezione verrà costruito un indovino, che legga informazioni su un
campo dal PHPDoc delle proprietà. Per iniziare, occorre creare una classe
che implementi :class:`Symfony\\Component\\Form\\FormTypeGuesserInterface`.
Tale interfaccia richiede quattro metodi:

* :method:`Symfony\\Component\\Form\\FormTypeGuesserInterface::guessType` -
  cerca di indovinare il tipo di un campo;
* :method:`Symfony\\Component\\Form\\FormTypeGuesserInterface::guessRequired` -
  cerca di indovinare il valore per l'opzione :ref:`required <reference-form-option-required>`
  ;
* :method:`Symfony\\Component\\Form\\FormTypeGuesserInterface::guessMaxLength` -
  cerca di indovinare il valore per l'opzione :ref:`max_length <reference-form-option-max_length>`
  ;
* :method:`Symfony\\Component\\Form\\FormTypeGuesserInterface::guessPattern` -
  cerca di indovinare il valore per l'opzione :ref:`pattern <reference-form-option-pattern>`
  .

Bisogna dunque creare una classe con questi metodi. Saranno riempiti in un momento successivo.

.. code-block:: php

    namespace Acme\Form;

    use Symfony\Component\Form\FormTypeGuesserInterface;

    class PhpdocTypeGuesser implements FormTypeGuesserInterface
    {
        public function guessType($class, $property)
        {
        }

        public function guessRequired($class, $property)
        {
        }

        public function guessMaxLength($class, $property)
        {
        }

        public function guessPattern($class, $property)
        {
        }
    }

Indovinare il tipo
~~~~~~~~~~~~~~~~~~

Quando si indovina un tipo, il metodo restituisce un'istanza di
:class:`Symfony\\Component\\Form\\Guess\\TypeGuess` oppure niente, per determinare
che l'indovino di tipi non riesce a indovinare il tipo.

Il costruttore di ``TypeGuess`` ha bisogno di tre opzioni:

* Il nome del tipo (uno dei :doc:`tipi di form </reference/forms/types>`);
* Opzioni aggiuntive (per esempio, quando il tipo è ``entity``, si deve anche
  impostare l'opzione ``class``). Se non viene indovinato alcun tipo, va impostato
  a un array vuoto;
* La fiducia sulla correttezza del tipo indovinato. Può essere una delle
  costanti della classe :class:`Symfony\\Component\\Form\\Guess\\Guess`:
  ``LOW_CONFIDENCE``, ``MEDIUM_CONFIDENCE``, ``HIGH_CONFIDENCE``,
  ``VERY_HIGH_CONFIDENCE``. Dopo che tutti gli indovini di tipo sono stati eseguiti, viene
  usato il tipo con fiducia maggiore.

Con questo in mente, si può facilmente implementare il metodo ``guessType`` di
``PHPDocTypeGuesser``::

    namespace Acme\Form;

    use Symfony\Component\Form\Guess\Guess;
    use Symfony\Component\Form\Guess\TypeGuess;

    class PhpdocTypeGuesser implements FormTypeGuesserInterface
    {
        public function guessType($class, $property)
        {
            $annotations = $this->readPhpDocAnnotations($class, $property);

            if (!isset($annotations['var'])) {
                return; // non indovinare niente se l'annotazione @var non è disponibile
            }

            // altrimenti, basa il tipo sull'annotazione @var
            switch ($annotations['var']) {
                case 'string':
                    // c'è una fiducia alta che il tipo sia testo, se è stata trovata
                    // @var string
                    return new TypeGuess('text', array(), Guess::HIGH_CONFIDENCE);

                case 'int':
                case 'integer':
                    // gli interi possono essere l'id di un'entità o un checkbox (0 o 1)
                    return new TypeGuess('integer', array(), Guess::MEDIUM_CONFIDENCE);

                case 'float':
                case 'double':
                case 'real':
                    return new TypeGuess('number', array(), Guess::MEDIUM_CONFIDENCE);

                case 'boolean':
                case 'bool':
                    return new TypeGuess('checkbox', array(), Guess::HIGH_CONFIDENCE);

                default:
                    // c'è una fiducia molto bassa che questo sia corretto
                    return new TypeGuess('text', array(), Guess::LOW_CONFIDENCE);
            }
        }

        protected function readPhpDocAnnotations($class, $property)
        {
            $reflectionProperty = new \ReflectionProperty($class, $property);
            $phpdoc = $reflectionProperty->getDocComment();

            // analizza $phpdoc in un array come:
            // array('type' => 'string', 'since' => '1.0')
            $phpdocTags = ...;

            return $phpdocTags;
        }
    }

Questo indovino di tipi ora può indovinare il tipo di campo per una proprietà, se
questa dispone di PHPDoc.

Indovinare le opzioni del campo
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

I restanti tre metodi (``guessMaxLength``, ``guessRequired`` e
``guessPattern``) restituiscono un'istanza di :class:`Symfony\\Component\\Form\\Guess\\ValueGuess`,
con il valore dell'opzione. Questo costruttore ha due parametri:

* Il valore dell'opzione;
* La fiducia che il valore indovinato sia corretto (usando le costanti della
  classe ``Guess``).

Viene restituito ``null`` quando si ritiene di non poter impostare il valore
dell'opzione.

.. caution::

    Occorre molta cautela con il metodo ``guessPattern``. Quando il
    tipo è un float, non lo si può usare per determinare un valore minimo o massimo per il
    float (p.e. si vuole che un float sia maggiore di ``5``, ``4.512313`` non è valido
    ma ``length(4.512314) > length(5)``, quindi lo schema avrebbe success). In
    questo caso, il valore va impostato a ``null`` con ``MEDIUM_CONFIDENCE``.

Registrare un indovino di tipi
------------------------------

L'ultimo passo da eseguire è registrare l'indovino di tipi, usando
:method:`Symfony\\Component\\Form\\FormFactoryBuilder::addTypeGuesser` o
:method:`Symfony\\Component\\Form\\FormFactoryBuilder::addTypeGuessers`::

    use Symfony\Component\Form\Forms;
    use Acme\Form\PHPDocTypeGuesser;

    $formFactory = Forms::createFormFactoryBuilder()
        // ...
        ->addTypeGuesser(new PHPDocTypeGuesser())
        ->getFormFactory();

    // ...

.. note::

    Se si usa il framework Symfony, occorre registrare l'indovino di tipi
    con il tag ``form.type_guesser``. Per maggiori informazioni, vedere
    :ref:`the tag reference <reference-dic-type_guesser>`.
