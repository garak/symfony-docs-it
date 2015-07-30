Template
========

Quando è nato PHP, 20 anni fa, gli sviluppatori rimasero incantati dalla sua semplicità e dalla
possibilità di mescolare facilmente HTML e codice dinamico. Con il passare del tempo, sono nati tanti
linguaggi per i template, come `Twig`, in grado di gestire i template dell'applicazione in modo migliore.

.. best-practice::

    Usare Twig per i template

Generalmente parlando, i template in PHP sono più prolissi di Twig, per
la mancanza di supporto nativo a molte caratteristiche necessarie nei template moderni,
come l'ereditarietà, l'escape automatico, i filtri e le
funzioni.

Twig è il formato predefinito per i template in Symfony e può contare sul supporto della più grande
comunità di utenti fra tutti gli engine non PHP (è usato in progetti molto importanti,
come Drupal 8).

Inoltre, Twig è l'unico formato con supporto garantito in Symfony
3.0. Il formato PHP potrebbe non essere più supportato
ufficialmente.

Posizione dei template
----------------------

.. best-practice::

    Inserire tutti i template dell'applicazione nella cartella ``app/Resource/views/``

Solitamente gli sviluppatori Symfony mettevano i template dell'applicazione nella cartella
``Resources/views/`` di ciascun bundle. Per riferirsi ad essi usavano il nome logico
(p.e. ``AcmeDemoBundle:Default:index.html.twig``).

Anche se per i bundle di terzi questa abitudine è corretta, è molto più conveniente, invece,
inserire i template dell'applicazione nella cartella ``app/Resources/views/``.
Innanzitutto questo semplifica drasticamente il nome logico dei template:

=================================================  ==================================
Template dentro i bundle                           Template dentro ``app/``
=================================================  ==================================
``AcmeDemoBundle:Default:index.html.twig``         ``default/index.html.twig``
``::layout.html.twig``                             ``layout.html.twig``
``AcmeDemoBundle::index.html.twig``                ``index.html.twig``
``AcmeDemoBundle:Default:subdir/index.html.twig``  ``default/subdir/index.html.twig``
``AcmeDemoBundle:Default/subdir:index.html.twig``  ``default/subdir/index.html.twig``
=================================================  ==================================

Un altro vantaggio è che centralizzare i template semplifica il lavoro dei
grafici. Essi non dovranno cercare più i template in tante cartelle sparpagliate fra
i bundle.

.. best-practice::

    Usare la notazione_serpente in minuscolo per nomi di cartelle e template.

Estensioni Twig
---------------

.. best-practice::

    Definire le estensioni di Twig nella cartella ``AppBundle/Twig``, configurandole
    nel file ``app/config/services.yml``.

All'applicazione serve un filtro Twig personalizzato `m2html` in modo da poter
trasformare il contenuto di ogni post da Markdown in HTML.

Per fare questo, per prima cosa, installare l'ottimo parser Markdown  `Parsedown`_
come nuova dipendenza del progetto:

.. code-block:: bash

    $ composer require erusev/parsedown

Quindi creare un nuovo servizio chiamato ``Markdown``, che sarà usato successivamente
dall' estensione Twig. La definizione del servizio richiede solamente di specificare il percorso della classe.

.. code-block:: yaml

    # app/config/services.yml
    services:
        # ...
        markdown:
            class: AppBundle\Utils\Markdown

La classe ``Markdown``  definisce un unico metodo per trasformare il contenuto
Markdown in contenuto HTML::

    namespace AppBundle\Utils;

    class Markdown
    {
        private $parser;

        public function __construct()
        {
            $this->parser = new \Parsedown();
        }

        public function toHtml($text)
        {
            $html = $this->parser->text($text);

            return $html;
        }
    }

Quindi, creare un'estensione Twig e definire un nuovo filtro chiamato ``md2html``,
utilizzando la classe ``Twig_SimpleFilter``. Iniettare il nuovo servizio, appena
definito, ``markdown`` nel costruttore dell'estensione Twig:

.. code-block:: php

    namespace AppBundle\Twig;

    use AppBundle\Utils\Markdown;

    class AppExtension extends \Twig_Extension
    {
        private $parser;

        public function __construct(Markdown $parser)
        {
            $this->parser = $parser;
        }

        public function getFilters()
        {
            return array(
                new \Twig_SimpleFilter(
                    'md2html',
                    array($this, 'markdownToHtml'),
                    array('is_safe' => array('html'))
                ),
            );
        }

        public function markdownToHtml($content)
        {
            return $this->parser->toHtml($content);
        }

        public function getName()
        {
            return 'app_extension';
        }
    }

Infine, definire un nuovo servizio, assegnandogli il tag ``twig.extension`` (il nome
del servizio è irrilevante, perché non verrà mai usato nel codice).

.. code-block:: yaml

    # app/config/services.yml
    services:
        app.twig.app_extension:
            class:     AppBundle\Twig\AppExtension
            arguments: ["@markdown"]
            public:    false
            tags:
                - { name: twig.extension }

.. _`Twig`: http://twig.sensiolabs.org/
.. _`Parsedown`: http://parsedown.org/
