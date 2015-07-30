Test
====

In generale, esistono due tipi di test. I test unitari consentono di
testare l'input e l'output di funzioni specifiche. I test funzionali
consentono di simulare un "browser", quindi è possibile navigare le pagine di un sito,
cliccare sui link, riempire i form e asserire di vedere certi elementi nella pagina.

Test unitari
------------

I test unitari sono usati per testare la "logica di business"; essa è
totalmente indipendente dal framework motivo per cui Symfony non include al suo interno
nessuno strumento per i test unitari. Tuttavia, gli strumenti più conosciuti
sono `PhpUnit`_ e `PhpSpec`_.

Test funzionali
---------------

Creare buoni test funzionali può essere molto difficile, per questo motivo molti sviluppatori li ignorano
del tutto e non effettuano nessun test. La raccomandazione è di non ignorarli. Infatti, definendo anche
solo qualche semplice test funzionale, si potranno individuare velocemente grandi errori prima di eseguire un deploy:

.. best-practice::

    Definire il test funzionale che almeno controlli il caricamento corretto di tutte pagine
    dell'applicazione.

Un semplice esempio di un test funzionale:

.. code-block:: php

    // src/AppBundle/Tests/ApplicationAvailabilityFunctionalTest.php
    namespace AppBundle\Tests;

    use Symfony\Bundle\FrameworkBundle\Test\WebTestCase;

    class ApplicationAvailabilityFunctionalTest extends WebTestCase
    {
        /**
         * @dataProvider urlProvider
         */
        public function testPageIsSuccessful($url)
        {
            $client = self::createClient();
            $client->request('GET', $url);

            $this->assertTrue($client->getResponse()->isSuccessful());
        }

        public function urlProvider()
        {
            return array(
                array('/'),
                array('/posts'),
                array('/post/fixture-post-1'),
                array('/blog/category/fixture-category'),
                array('/archives'),
                // ...
            );
        }
    }

Questo codice controlla che tutti gli URL vengano caricati correttamente, cioè
che il codice della risposta HTTP sia compreso tra ``200`` e ``299``.
Apparentemente potrebbe sembrare inutile, ma, considerando quanto poco sforzo viene fatto,
è sempre un bene avere il test nell'applicazione.

In informatica, questo tipo di test è chiamato `smoke test`_ e consiste
in *"un test preliminare, che rivela fallimenti di base, ma abbastanza gravi da rinviare
il rilascio di un software"*.

Definire gli URL nei test funzionali
------------------------------------

Qualcuno potrebbe chiedersi perché nel precedente test funzionale non venga usato
il servizio di generazione degli URL.

.. best-practice::

    Inserire direttamente gli URL nei test funzionali invece di usare il
    generatore di URL.

Si consideri il seguente test funzionale, che usa il servizio `router` per generare l'URL della
pagina testata:

.. code-block:: php

    public function testBlogArchives()
    {
        $client = self::createClient();
        $url = $client->getContainer()->get('router')->generate('blog_archives');
        $client->request('GET', $url);

        // ...
    }

Il test funzionerà correttamente, ma avrà un grande inconveniente. Se per sbaglio uno sviluppatore
modifica il percorso della rotta ``blog_archives``, il test continuerà ancora a funzionare, ma
l'URL originale non funzionerà più. Proprio per questo ogni segnalibro di quell'URL non sarà
più raggiungibile, con conseguenze anche sul page ranking nei motori di ricerca.

Testare JavaScript
------------------

Il client fornito da Symfony per i test funzionali funziona molto bene, ma non può essere usato per testare
il comportamento di Javascript sulle tue pagine. Se questa funzionalità è necessaria, considerare l'utilizzo della
[libreria Mink](http://mink.behat.org) con PHPUnit.

Ovviamente, se un'applicazione usa Javascript in tutte le sue funzionalità,
si dovrebbe considerare l'uso di strumenti specificatamente pensati per testare Javascript.

Per saperne di più sui test funzionali
--------------------------------------

Usare le librerie `Faker`_ e `Alice`_ per generare dati realistici e per
le fixture.

.. _`Faker`: https://github.com/fzaninotto/Faker
.. _`Alice`: https://github.com/nelmio/alice
.. _`PhpUnit`: https://phpunit.de/
.. _`PhpSpec`: http://www.phpspec.net/
.. _`Mink`: http://mink.behat.org
.. _`smoke test`: http://en.wikipedia.org/wiki/Smoke_testing_(software)
