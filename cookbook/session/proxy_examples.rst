.. index::
   single: Sessioni, proxy di sessione, proxy

Esempi di proxy di sessione
---------------------------

Il meccanismo dei proxy di sessione ha molti usi e questo esempio ne dimostra
due comuni. Piuttosto che iniettare il gestore di sessioni, si inietta nel proxy un
gestore registrato con il driver di memorizzazione della sessione::

    use Symfony\Component\HttpFoundation\Session\Session;
    use Symfony\Component\HttpFoundation\Session\Storage\NativeSessionStorage;
    use Symfony\Component\HttpFoundation\Session\Storage\Handler\PdoSessionStorage;

    $proxy = new YourProxy(new PdoSessionStorage());
    $session = new Session(new NativeSessionStorage($proxy));

Di seguito, vedremo due esempi reali, utilizzabili per ``YourProxy``:
criptazione della sessione e sessioni ospiti in sola lettura.

Criptare i dati della sessione
------------------------------

Se si vogliono criptare i dati della sessione, si può usare il proxy per criptare
e decriptare la sessione, come richiesto::

    use Symfony\Component\HttpFoundation\Session\Storage\Proxy\SessionHandlerProxy;

    class EncryptedSessionProxy extends SessionHandlerProxy
    {
        private $key;

        public function __construct(\SessionHandlerInterface $handler, $key)
        {
            $this->key = $key;

            parent::__construct($handler);
        }

        public function read($id)
        {
            $data = parent::write($id, $data);

            return mcrypt_decrypt(\MCRYPT_3DES, $this->key, $data);
        }

        public function write($id, $data)
        {
            $data = mcrypt_encrypt(\MCRYPT_3DES, $this->key, $data);

            return parent::write($id, $data);
        }
    }

Sessioni ospiti in sola lettura
-------------------------------

In alcune applicazioni, serve una sessione per gli utenti ospiti, senza
l'esigenza di doverla persistere. In questo caso, si può
intercettare la sessione prima della sua scrittura::

    use Foo\User;
    use Symfony\Component\HttpFoundation\Session\Storage\Proxy\SessionHandlerProxy;

    class ReadOnlyGuestSessionProxy extends SessionHandlerProxy
    {
        private $user;

        public function __construct(\SessionHandlerInterface $handler, User $user)
        {
            $this->user = $user;

            parent::__construct($handler);
        }

        public function write($id, $data)
        {
            if ($this->user->isGuest()) {
                return;
            }

            return parent::write($id, $data);
        }
    }
