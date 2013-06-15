.. index::
   single: Test; Simulare l'autenticazione

Simulare l'autenticazione con un token in un test funzionale
============================================================

Le richieste di autenticazione nei test funzionali possono rallentarne l'esecuzione.
Può essere un problema, specialmente quando si usa ``form_login``, poiché
necessita di ulteriori richieste da riempire e di inviare il form.

Una possibile soluzione consiste nel configurare il firewall per usare ``http_basic`` in
ambiente di t est, come spiegato in
:doc:`/cookbook/testing/http_authentication`.
Un altro modo potrebbe essere quello di creare da soli un token e memorizzarlo nella sessione.
Se si fa in questo modo, bisogna assicurarsi che venga inviato il cookie appropriato
con una richiesta. L'esempio seguente mostra tale tecnica::

    // src/Acme/DemoBundle/Tests/Controller/DemoControllerTest.php
    namespace Acme\DemoBundle\Tests\Controller;

    use Symfony\Bundle\FrameworkBundle\Test\WebTestCase;
    use Symfony\Component\BrowserKit\Cookie;
    use Symfony\Component\Security\Core\Authentication\Token\UsernamePasswordToken;

    class DemoControllerTest extends WebTestCase
    {
        private $client = null;

        public function setUp()
        {
            $this->client = static::createClient();
        }

        public function testSecuredHello()
        {
            $this->logIn();

            $this->client->request('GET', '/demo/secured/hello/Fabien');

            $this->assertTrue($this->client->getResponse()->isSuccessful());
            $this->assertGreaterThan(0, $crawler->filter('html:contains("Hello Fabien")')->count());
        }

        private function logIn()
        {
            $session = $this->client->getContainer()->get('session');

            $firewall = 'secured_area';
            $token = new UsernamePasswordToken('admin', null, $firewall, array('ROLE_ADMIN'));
            $session->set('_security_'.$firewall, serialize($token));
            $session->save();

            $cookie = new Cookie($session->getName(), $session->getId());
            $this->client->getCookieJar()->set($cookie);
        }
    }

.. note::

    La tecnica descritta in :doc:`/cookbook/testing/http_authentication`.
    è più pulita e quindi preferibile.
