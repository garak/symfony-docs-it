.. index::
    single: Translation; Aggiungere supporto per un formato personalizzato

Aggiungere supporto per un formato personalizzato
=================================================

A volte, si ha l'esigenza di un formato personalizzato per i file di traduzione. Il componente
Translation è abbastanza flessibile da supportarlo. Basta creare un
caricatore (per caricare traduzioni) e, eventualmente, un esportatore (per esportarle).

Si immagini di avere un formato personalizzato, in cui i messaggi di traduzione sono definiti
usando una riga per ogni traduzione e parentesi intorno a chiave e
messaggio. Un file di traduzione sarebbe simile a questo:

.. code-block:: text

    (welcome)(accueil)
    (goodbye)(au revoir)
    (hello)(bonjour)

.. _components-translation-custom-loader:

Creare un caricatore personalizzato
-----------------------------------

Per definire un caricatore personalizzato, che possa leggere questo tipo di file, si deve creare una
nuova classe, che implementi
:class:`Symfony\\Component\\Translation\\Loader\\LoaderInterface`. Il metodo
:method:`Symfony\\Component\\Translation\\Loader\\LoaderInterface::load`
prende un nome di file e lo analizza, producendo un array. Quindi, crea
il catalogo, che viene restituito::

    use Symfony\Component\Translation\MessageCatalogue;
    use Symfony\Component\Translation\Loader\LoaderInterface;

    class MyFormatLoader implements LoaderInterface
    {
        public function load($resource, $locale, $domain = 'messages')
        {
            $messages = array();
            $lines = file($resource);

            foreach ($lines as $line) {
                if (preg_match('/\(([^\)]+)\)\(([^\)]+)\)/', $line, $matches)) {
                    $messages[$matches[1]] = $matches[2];
                }
            }

            $catalogue = new MessageCatalogue($locale);
            $catalogue->add($messages, $domain);

            return $catalogue;
        }

    }

Una volta creato, può essere usato, come ogni altro caricatore::

    use Symfony\Component\Translation\Translator;

    $translator = new Translator('fr_FR');
    $translator->addLoader('my_format', new MyFormatLoader());

    $translator->addResource('my_format', __DIR__.'/translations/messages.txt', 'fr_FR');

    echo $translator->trans('welcome');

Mostrerà *"accueil"*.

.. _components-translation-custom-dumper:

Creare un esportatore personalizzato
------------------------------------

Si può anche creare un esportatore personalizzato per un formato, il che è utile
se si usano i comandi di estrazione. Per farlo, occorre creare una nuova classe,
che implementi
:class:`Symfony\\Component\\Translation\\Dumper\\DumperInterface`.
Per scrivere il contenuto esportato in un file, estendendo la classe
:class:`Symfony\\Component\\Translation\\Dumper\\FileDumper`
farà risparmiare alcune righe::

    use Symfony\Component\Translation\MessageCatalogue;
    use Symfony\Component\Translation\Dumper\FileDumper;

    class MyFormatDumper extends FileDumper
    {
        protected function format(MessageCatalogue $messages, $domain = 'messages')
        {
            $output = '';

            foreach ($messages->all($domain) as $source => $target) {
                $output .= sprintf("(%s)(%s)\n", $source, $target);
            }

            return $output;
        }

        protected function getExtension()
        {
            return 'txt';
        }
    }

Il metodo :method:`Symfony\\Component\\Translation\\Dumper\\FileDumper::format`
crea la stringa di output, che sarà usata dal metodo
:method:`Symfony\\Component\\Translation\\Dumper\\FileDumper::dump`
della classe FileDumper per creare il file. L'esportatore può essere usato come ogni altro
esportatore predefinito. Nell'esempio seguente, i messaggi di traduzione definiti nel file
YAML sono esportati in un file di testo, con il formato personalizzato::

    use Symfony\Component\Translation\Loader\YamlFileLoader;

    $loader = new YamlFileLoader();
    $catalogue = $loader->load(__DIR__ . '/translations/messages.fr_FR.yml' , 'fr_FR');

    $dumper = new MyFormatDumper();
    $dumper->dump($catalogue, array('path' => __DIR__.'/dumps'));
