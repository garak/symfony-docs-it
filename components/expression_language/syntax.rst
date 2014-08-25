.. index::
    single: Sintassi; ExpressionLanguage

Sintassi di Expression
======================

Il componente ExpressionLanguage usa una sintassi specifica, basata sulla
sintassi delle espressioni di Twig. In questo documento si potranno trovare tutte
le sintassi supportate.

Letterali supportati
--------------------

Il componente supporta:

* **stringhe** - con virgolette singole e doppie (p.e. ``'ciao'``)
* **numeri** - p.e. ``103``
* **array** - con notazione tipo JSON (p.e. ``[1, 2]``)
* **hash** - con notazione tipo JSON (p.e. ``{ pippo: 'pluto' }``)
* **booleani** - ``true`` e ``false``
* **nullo** - ``null``

.. _component-expression-objects:

Lavorare con gli oggetti
------------------------

Quando si passano oggetti in un'espressione, si possono usare varie sintassi per
accedere a proprietà e richiamare metodi.

Accedere a proprietà pubbliche
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Si può accedere a proprietà pubbliche degli oggetti usando la sintassi ``.``, similmente
a JavaScript::

    class Apple
    {
        public $variety;
    }

    $apple = new Apple();
    $apple->variety = 'Honeycrisp';

    echo $language->evaluate(
        'fruit.variety',
        array(
            'fruit' => $apple,
        )
    );

Mostrerà ``Honeycrisp``.

Richiamare metodi
~~~~~~~~~~~~~~~~~

Si può usare la sintassi ``.`` anche per richiamare metodi dell'oggetto, similmente a
JavaScript::

    class Robot
    {
        public function sayHi($times)
        {
            $greetings = array();
            for ($i = 0; $i < $times; $i++) {
                $greetings[] = 'Ciao';
            }

            return implode(' ', $greetings).'!';
        }
    }

    $robot = new Robot();

    echo $language->evaluate(
        'robot.sayHi(3)',
        array(
            'robot' => $robot,
        )
    );

Mostrerà ``Ciao Ciao Ciao!``.

.. _component-expression-functions:

Lavorare con le funzioni
------------------------

Si possono anche usare funzioni registrate nell'espressione, usando la stessa
sintassi di PHP e JavaScript. Il componente ExpressionLanguage dispone già di una
funzione: ``constant()``, che restituisce il valore di una costante
PHP::

    define('UTENTE_DB', 'root');

    echo $language->evaluate(
        'constant("UTENTE_DB")'
    );

Mostrerà ``root``.

.. tip::

    Le sapere come registrare funzioni da usare in un'espressione, vedere
    ":doc:`/components/expression_language/extending`".

.. _component-expression-arrays:

Lavorare con gli array
----------------------

Se si passa un array in un'espressione, usare la sintassi ``[]`` per accedere
all'array, similmente a JavaScript::

    $data = array('vita' => 10, 'universo' => 10, 'tutto_quanto' => 22);

    echo $language->evaluate(
        'data["vita"] + data["universo"] + data["tutto_quanto"]',
        array(
            'data' => $data,
        )
    );

Mostrerà ``42``.

Operatori supportati
--------------------

Il componente disponde di vari operatori:

Operatori aritmetici
~~~~~~~~~~~~~~~~~~~~

* ``+`` (addizione)
* ``-`` (sottrazione)
* ``*`` (moltiplicazione)
* ``/`` (divisione)
* ``%`` (modulo)
* ``**`` (potenza)

Per esempio::

    echo $language->evaluate(
        'vita + universo + tutto_quanto',
        array(
            'vita' => 10,
            'universe' => 10,
            'tutto quanto' => 22,
        )
    );

Mostrerà ``42``.

Operatori di bit
~~~~~~~~~~~~~~~~

* ``&`` (and)
* ``|`` (or)
* ``^`` (xor)

Operatori di confronto
~~~~~~~~~~~~~~~~~~~~~~

* ``==`` (uguale)
* ``===`` (identico)
* ``!=`` (diverso)
* ``!==`` (non identico)
* ``<`` (minore)
* ``>`` (maggiore)
* ``<=`` (minore o uguale)
* ``>=`` (maggiore o uguale)
* ``matches`` (espressione regolare)

.. tip::

    Per verificare che una stringa *non* soddisfi un'espressione regolare, usare l'operatore logico ``not``
    in combinazione con l'operatore ``matches``::

        $language->evaluate('not ("pippo" matches "/pluto/")'); // restituisce true

    Si devono usare le parentesi, perché l'operatore unario ``not`` ha precedenza
    sull'operatore binario ``matches``.

Esempi::

    $ret1 = $language->evaluate(
        'vita == tutto_quanto',
        array(
            'vita' => 10,
            'universe' => 10,
            'tutto quanto' => 22,
        )
    );

    $ret2 = $language->evaluate(
        'vita > tutto_quanto',
        array(
            'vita' => 10,
            'universe' => 10,
            'tutto quanto' => 22,
        )
    );

Entrambe le variabili saranno impostate a ``false``.

Operatori logici
~~~~~~~~~~~~~~~~

* ``not`` o ``!``
* ``and`` o ``&&``
* ``or`` o ``||``

Per esempio::

    $ret = $language->evaluate(
        'vita < universo or vita < tutto_quanto',
        array(
            'vita' => 10,
            'universe' => 10,
            'tutto quanto' => 22,
        )
    );

La variabile ``$ret`` sarà impostata a ``true``.

Operatori di stringhe
~~~~~~~~~~~~~~~~~~~~~

* ``~`` (concatenazione)

Per esempio::

    echo $language->evaluate(
        'nome~" "~cognome',
        array(
            'nome' => 'Arthur',
            'cognome' => 'Dent',
        )
    );

Mostrerà ``Arthur Dent``.

Operatori di array
~~~~~~~~~~~~~~~~~~

* ``in`` (contiene)
* ``not in`` (non contiene)

For example::

    class User
    {
        public $group;
    }

    $user = new User();
    $user->group = 'risorse_umane';

    $inGroup = $language->evaluate(
        'user.group in ["risorse_umane", "marketing"]',
        array(
            'user' => $user
        )
    );

``$inGroup`` sarà valutata a ``true``.

Operatori numerici
~~~~~~~~~~~~~~~~~~

* ``..`` (gamma)

Per esempio::

    class User
    {
        public $age;
    }

    $user = new User();
    $user->age = 34;

    $language->evaluate(
        'user.age in 18..45',
        array(
            'user' => $user,
        )
    );

Sarà valutata a ``true``, perché ``user.age`` è compreso tra
``18`` e ``45``.

Operatori ternari
~~~~~~~~~~~~~~~~~

* ``pippo ? 'sì' : 'no'``
* ``pippo ?: 'no'`` (uguale a ``pippo ? pippo : 'no'``)
* ``pippo ? 'sì'`` (uguale a ``pippo ? 'sì' : ''``)
