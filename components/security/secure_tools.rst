Sicurezza nel confronto di stringhe e nella generazione di numeri casuali
=========================================================================

Il componente Security di Symfony dispone di un insieme di utilità correlate
alla sicurezza. Queste utilità sono usate da Symfony, ma possono anche essere
usate in autonomia, per risolvere alcuni problemi.

Confronto di stringhe
~~~~~~~~~~~~~~~~~~~~~

Il tempo che occorre per confrontare due stringhe dipende dalle loro differenze. Questo dettaglio
può essere usato da un attaccante, quando le due stringhe rappresentano una password, per esempio.
Questo attacco è noto come `timing attack`_.

Internamente, quando confronta due password, Symfony usa un algoritmo a tempo
costante. Si può usare la stessa strategia nel proprio codice, grazie alla classe
:class:`Symfony\\Component\\Security\\Core\\Util\\StringUtils`::

    use Symfony\Component\Security\Core\Util\StringUtils;

    // una stringa nota (p.e. una password) è uguale a una fornita dall'utente?
    $bool = StringUtils::equals($stringaNota, $stingaUtente);

.. caution::

    Per evitare attacchi di tipo timing attack, la stringa nota deve essere il primo parametro
    e la stringa inserita dall'utente il secondo parametro.

Generazione di un numero casuale sicuro
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Ogni volta che si deve generare un numero casuale sicuro, si raccomanda caldamente
l'uso della classe
:class:`Symfony\\Component\\Security\\Core\\Util\\SecureRandom`::

    use Symfony\Component\Security\Core\Util\SecureRandom;

    $generator = new SecureRandom();
    $random = $generator->nextBytes(10);

Il metodo
:method:`Symfony\\Component\\Security\\Core\\Util\\SecureRandom::nextBytes`
restituisce una stringa casuale, composta dal numero di caratteri passati come
parametro (nell'esempio, 10).

La classe funziona meglio quando è installato SSL. In caso in cui non sia disponibile,
ai appoggia a un algoritmo interno, che ha bisogno di un seme per funzionare
correttamente. In questo caso, passare un nome di file::

    use Symfony\Component\Security\Core\Util\SecureRandom;

    $generator = new SecureRandom('/percorso/in/cui/si/trova/il/seme.txt');
    $random = $generator->nextBytes(10);

.. note::

    Se si usa il framework Symfony, si può accedere direttamente all'istanza della classe
    dal contenitore: il nome del servizio è ``security.secure_random``.

.. _`timing attack`: http://en.wikipedia.org/wiki/Timing_attack
