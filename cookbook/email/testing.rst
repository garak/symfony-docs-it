.. index::
   single: Email; Test

Testare l'invio di un'email in un test funzionale
=================================================

L'invio di email con Symfony è alquanto semplice, grazie a
SwiftmailerBundle, che sfrutta la potenza della libreria `Swift Mailer`_.

Per testare in modo funzionale l'invio di un'email, anche facendo asserzioni su oggetto
o contenuto dell'email, o su qualsiasi altro header, si può usare il :ref:`profilatore di Symfony <internals-profiler>`.

Iniziamo con una semplice azione in un controllore, che invia un'email::

    public function sendEmailAction($name)
    {
        $message = \Swift_Message::newInstance()
            ->setSubject('Email di saluti')
            ->setFrom('mittente@example.com')
            ->setTo('destinatario@example.com')
            ->setBody('Dovresti vedermi nel profilatore!')
        ;

        $this->get('mailer')->send($message);

        return $this->render(...);
    }

.. note::

    Non dimenticare di abilitare il profilatore, come spiegato in :doc:`/cookbook/testing/profiling`.

Nel test funzionale, usare il raccoglitore ``swiftmailer`` del profilatore,
per ottnere informazioni sui messaggi inviati nella richiesta precedente::

    // src/AppBundle/Tests/Controller/MailControllerTest.php
    use Symfony\Bundle\FrameworkBundle\Test\WebTestCase;

    class MailControllerTest extends WebTestCase
    {
        public function testMailIsSentAndContentIsOk()
        {
            $client = static::createClient();

            // Abilita il profilatore per la prossima richiesta (non fa nulla se il profilatore non è disponibile)
            $client->enableProfiler();

            $crawler = $client->request('POST', '/path/to/above/action');

            $mailCollector = $client->getProfile()->getCollector('swiftmailer');

            // verifica che sia stata inviata un'email 
            $this->assertEquals(1, $mailCollector->getMessageCount());

            $collectedMessages = $mailCollector->getMessages();
            $message = $collectedMessages[0];

            // asserzioni sui dati dell'email
            $this->assertInstanceOf('Swift_Message', $message);
            $this->assertEquals('Email di saluti', $message->getSubject());
            $this->assertEquals('mittente@example.com', key($message->getFrom()));
            $this->assertEquals('destinatario@example.com', key($message->getTo()));
            $this->assertEquals(
                'Dovresti vedermi nel profilatore!',
                 $message->getBody()
            );
        }
    }

.. _`Swift Mailer`: http://swiftmailer.org/
